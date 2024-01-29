package com.example.defectdetection

import android.Manifest
import android.content.pm.PackageManager
import android.graphics.Bitmap
import android.os.Build
import android.os.Bundle
import android.util.Log
import android.widget.ImageView
import android.widget.Toast
import androidx.activity.result.contract.ActivityResultContracts
import androidx.appcompat.app.AppCompatActivity
import androidx.camera.core.CameraSelector
import androidx.camera.core.ImageAnalysis
import androidx.camera.core.ImageProxy
import androidx.camera.core.Preview
import androidx.camera.lifecycle.ProcessCameraProvider
import androidx.core.content.ContextCompat
import com.example.defectdetection.databinding.ActivityMainBinding
import java.util.concurrent.ExecutorService
import java.util.concurrent.Executors
import android.content.res.AssetFileDescriptor
import android.content.res.AssetManager
import org.tensorflow.lite.Interpreter
import java.io.FileInputStream
import java.io.IOException
import java.nio.ByteBuffer
import java.nio.ByteOrder
import java.nio.MappedByteBuffer
import java.nio.channels.FileChannel
import kotlin.math.ln1p
import android.graphics.Canvas
import android.graphics.Paint
import android.graphics.Color
import kotlin.math.max
import kotlin.math.min
import android.widget.TextView


typealias ImageProcessedListener = (mutableBitmap: Bitmap) -> Unit

var counter = 0

class MainActivity : AppCompatActivity() {

    private lateinit var originalImageView: ImageView
    private lateinit var tflite: Interpreter
    private lateinit var cameraAnalyzer: CameraAnalyzer
    private lateinit var textView: TextView

    class CameraAnalyzer(private var textView: TextView, private val tflite: Interpreter, private val listener: ImageProcessedListener) : ImageAnalysis.Analyzer {

        override fun analyze(image: ImageProxy) {

            val startTime = System.currentTimeMillis()
            // Get the Y plane
            val buffer = image.planes[0].buffer
            val data = ByteArray(buffer.remaining())
            buffer.get(data)

            // Convert the 1D pixel array to a 2D array
            val width = image.width
            val height = image.height
            val pixelValues = Array(height) { IntArray(width) }
            val originalValues = Array(height) { ByteArray(width) }
            var index = 0

            for (y in 0 until height) {
                for (x in 0 until width) {
                    pixelValues[y][x] = data[index].toInt() and 0xFF
                    originalValues[y][x] = data[index++]
                }
            }

            // Apply single-scale Retinex algorithm
            val retinexValues = applySingleScaleRetinex(pixelValues)

            // Convert the processed pixel array back to a Bitmap
            val processedBitmap = convertToBitmap(retinexValues, width, height)

            // Resize the original preview
            val cameraPreviewResized = convertToBitmap(originalValues, width, height)

            // Input shapes for the model
            val inputShape = intArrayOf(1, 224, 224, 3)
            val outputShape = intArrayOf(1, 5, 1029)

            val preprocessedImage = preprocessInputImage(processedBitmap, inputShape)
            // Convert the preprocessed image to a ByteBuffer
            val inputBuffer = convertToByteBuffer(preprocessedImage)

            // Create a 3D Float Array with the specified dimensions
            val outputArray = Array(outputShape[0]) { Array(outputShape[1]) { FloatArray(outputShape[2]) } }
            // Run the model
            tflite.run(inputBuffer, outputArray)

            // Create a canvas
            val mutableBitmap = cameraPreviewResized.copy(Bitmap.Config.ARGB_8888, true)
            val canvas = Canvas(mutableBitmap)
            val paint = Paint()
            paint.style = Paint.Style.STROKE
            paint.strokeWidth = 4f

            val numBoxes = outputShape[2]


            // Extract raw bounding boxes and confidence scores
            val boundingBoxes = Array(numBoxes) { FloatArray(4) }

            // Populate the bounding boxes
            for (i in 0 until numBoxes) {
                boundingBoxes[i][0] = outputArray[0][0][i]
                boundingBoxes[i][1] = outputArray[0][1][i]
                boundingBoxes[i][2] = outputArray[0][2][i]
                boundingBoxes[i][3] = outputArray[0][3][i]
            }

            val rawScores = FloatArray(numBoxes) { outputArray[0][4][it] }
            val confidenceThreshold = 0.5f
            // Apply non-maximum suppression
            val keepIndices = nonMaxSuppression(boundingBoxes, rawScores, confidenceThreshold)

            for (i in keepIndices) {
                val center_x = boundingBoxes[i][0] * processedBitmap.width
                val center_y = boundingBoxes[i][1] * processedBitmap.height
                val width = boundingBoxes[i][2] * processedBitmap.width
                val height = boundingBoxes[i][3] * processedBitmap.height

                val confidence = rawScores[i]

                // Only show the detections above the threshold
                if (confidence >= confidenceThreshold) {
                    println("Detection $i: $numBoxes (Confidence: $confidence), Bounding Box: ($center_x, $center_y, $width, $height)")

                    paint.color = Color.RED
                    val left = (center_x - width / 2)
                    val top = (center_y - height / 2)
                    val right = (center_x + width / 2)
                    val bottom = (center_y + height / 2)

                    canvas.drawRect(left, top, right, bottom, paint)

                    paint.textSize = 30f
                    canvas.drawText("Confidence: $confidence", left, top - 10f, paint)
                }
            }


            listener(mutableBitmap)
            image.close()
            val endTime = System.currentTimeMillis()
            val latency = endTime - startTime
            val resultStr = "Latency : $latency"
            if(counter % 15 == 0)
                textView.text = resultStr
            if(counter == 10000)
                counter = 0
            counter++

        }

        // Intersection over Union Calculation
        private fun iou(box1: FloatArray, box2: FloatArray): Float {
            val x1 = max(box1[0], box2[0])
            val y1 = max(box1[1], box2[1])
            val x2 = min(box1[2], box2[2])
            val y2 = min(box1[3], box2[3])

            val interArea = max(0f, x2 - x1 + 1) * max(0f, y2 - y1 + 1)

            val box1Area = (box1[2] - box1[0] + 1) * (box1[3] - box1[1] + 1)
            val box2Area = (box2[2] - box2[0] + 1) * (box2[3] - box2[1] + 1)

            val iou = interArea / (box1Area + box2Area - interArea)
            return iou
        }

        // Non-max Suppression for detections
        private fun nonMaxSuppression(boxes: Array<FloatArray>, scores: FloatArray, threshold: Float): List<Int> {
            val keep = mutableListOf<Int>()
            var order = scores.indices.sortedByDescending { scores[it] }.toIntArray()

            while (order.size > 0) {
                val i = order[0]
                keep.add(i)

                val ious = (1 until order.size)
                    .map { j -> iou(boxes[i], boxes[order[j]]) }
                    .toFloatArray()

                val inds = ious.indices.filter { ious[it] <= threshold }.toIntArray()
                val updatedOrder = IntArray(inds.size) { i -> order[inds[i] + 1] }
                updatedOrder.copyInto(order)

                order = updatedOrder
            }

            return keep
        }

        // Preprocess the image by resizing
        private fun preprocessInputImage(inputBitmap: Bitmap, inputShape: IntArray): Bitmap {
            return Bitmap.createScaledBitmap(inputBitmap, inputShape[2], inputShape[1], true)
        }

        private fun convertToByteBuffer(bitmap: Bitmap): ByteBuffer {
            val byteBuffer = ByteBuffer.allocateDirect(4 * 224 * 224 * 3)
            byteBuffer.order(ByteOrder.nativeOrder())
            val intValues = IntArray(bitmap.width * bitmap.height)
            bitmap.getPixels(intValues, 0, bitmap.width, 0, 0, bitmap.width, bitmap.height)
            var pixel = 0
            for (y in 0 until bitmap.height) {
                for (x in 0 until bitmap.width) {
                    val value = intValues[pixel++]
                    // Extract grayscale value and replicate to three channels
                    val normalizedValue = (value shr 16 and 0xFF) / 255.0
                    byteBuffer.putFloat(normalizedValue.toFloat())
                    byteBuffer.putFloat(normalizedValue.toFloat())
                    byteBuffer.putFloat(normalizedValue.toFloat())
                }
            }
            return byteBuffer
        }

        private fun applySingleScaleRetinex(pixelValues: Array<IntArray>): Array<ByteArray> {

            val retinexValues = Array(pixelValues.size) { DoubleArray(pixelValues[0].size) }
            for (i in 0 until pixelValues.size) {
                for (j in 0 until pixelValues[0].size) {
                    retinexValues[i][j] = ln1p(pixelValues[i][j].toDouble())
                }
            }

            var minValue = Double.MAX_VALUE // Initialize with a large positive value
            var maxValue = Double.MIN_VALUE
            for (i in 0 until retinexValues.size) {
                for (j in 0 until retinexValues[0].size) {
                    if (retinexValues[i][j] > maxValue) {
                        maxValue = retinexValues[i][j]
                    }
                    if (retinexValues[i][j] < minValue) {
                        minValue = retinexValues[i][j]
                    }
                }
            }

            // Normalize the values
            val normalizedArr = Array(pixelValues.size) { ByteArray(pixelValues[0].size) }

            for (i in 0 until retinexValues.size) {
                normalizedArr[i] = retinexValues[i].map { ((it - minValue) / (maxValue - minValue) * 255).toInt().toByte() }.toByteArray()
            }


            return normalizedArr
        }


        private fun convertToBitmap(pixelValues: Array<ByteArray>, width: Int, height: Int): Bitmap {
            val bitmap = Bitmap.createBitmap(width, height, Bitmap.Config.ARGB_8888)
            for (y in 0 until height) {
                for (x in 0 until width) {
                    val pixel = pixelValues[y][x].toInt() and 0xFF
                    val grayscalePixel = 0xFF000000.toInt() or (pixel shl 16) or (pixel shl 8) or pixel
                    bitmap.setPixel(x, y, grayscalePixel)
                }
            }
            return bitmap
        }
    }

    private lateinit var viewBinding: ActivityMainBinding
    private lateinit var cameraExecutor: ExecutorService

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        // Initialize TensorFlow Lite Interpreter
        try {
            tflite = Interpreter(loadModelFile())
        } catch (e: IOException) {
            e.printStackTrace()
        }

        viewBinding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(viewBinding.root)

        textView = findViewById(R.id.textView)
        textView.text = "Latency: 0"


        originalImageView = findViewById(R.id.originalImageView)

        cameraAnalyzer = CameraAnalyzer(textView, tflite) { mutableBitmap ->
            runOnUiThread {
                originalImageView.setImageBitmap(mutableBitmap)
            }
        }

        // Request camera permissions
        if (allPermissionsGranted()) {
            startCamera()
        } else {
            requestPermissions()
        }

        cameraExecutor = Executors.newSingleThreadExecutor()
    }

    private fun loadModelFile(): MappedByteBuffer {
        val assetManager: AssetManager = assets
        val fileDescriptor: AssetFileDescriptor = assetManager.openFd("best_float32.tflite")
        val inputStream = FileInputStream(fileDescriptor.fileDescriptor)
        val fileChannel: FileChannel = inputStream.channel
        val startOffset: Long = fileDescriptor.startOffset
        val declaredLength: Long = fileDescriptor.declaredLength
        return fileChannel.map(FileChannel.MapMode.READ_ONLY, startOffset, declaredLength)
    }

    private fun startCamera() {
        val cameraProviderFuture = ProcessCameraProvider.getInstance(this)

        cameraProviderFuture.addListener({
            // Used to bind the lifecycle of cameras to the lifecycle owner
            val cameraProvider: ProcessCameraProvider = cameraProviderFuture.get()

            // Preview
            val preview = Preview.Builder()
                .build()
                .also {
                    it.setSurfaceProvider(viewBinding.viewFinder.surfaceProvider)
                }

            val imageAnalyzer = ImageAnalysis.Builder()
                .build()
                .also {

                    it.setAnalyzer(cameraExecutor, cameraAnalyzer)
                }

            // Select back camera as a default
            val cameraSelector = CameraSelector.DEFAULT_BACK_CAMERA

            try {
                // Unbind use cases before rebinding
                cameraProvider.unbindAll()

                // Bind use cases to camera
                cameraProvider.bindToLifecycle(
                    this, cameraSelector, preview, imageAnalyzer)

            } catch(exc: Exception) {
                Log.e(TAG, "Use case binding failed", exc)
            }

        }, ContextCompat.getMainExecutor(this))
    }

    private fun requestPermissions() {
        activityResultLauncher.launch(REQUIRED_PERMISSIONS)
    }

    private fun allPermissionsGranted() = REQUIRED_PERMISSIONS.all {
        ContextCompat.checkSelfPermission(
            baseContext, it) == PackageManager.PERMISSION_GRANTED
    }

    override fun onDestroy() {
        super.onDestroy()
        tflite.close()
        cameraExecutor.shutdown()
    }

    companion object {
        private const val TAG = "CameraXApp"
        private const val FILENAME_FORMAT = "yyyy-MM-dd-HH-mm-ss-SSS"
        private val REQUIRED_PERMISSIONS =
            mutableListOf (
                Manifest.permission.CAMERA,
                Manifest.permission.RECORD_AUDIO
            ).apply {
                if (Build.VERSION.SDK_INT <= Build.VERSION_CODES.P) {
                    add(Manifest.permission.WRITE_EXTERNAL_STORAGE)
                }
            }.toTypedArray()
    }



    private val activityResultLauncher =
        registerForActivityResult(
            ActivityResultContracts.RequestMultiplePermissions())
        { permissions ->
            // Handle Permission granted/rejected
            var permissionGranted = true
            permissions.entries.forEach {
                if (it.key in REQUIRED_PERMISSIONS && it.value == false)
                    permissionGranted = false
            }
            if (!permissionGranted) {
                Toast.makeText(baseContext,
                    "Permission request denied",
                    Toast.LENGTH_SHORT).show()
            } else {
                startCamera()
            }
        }
}