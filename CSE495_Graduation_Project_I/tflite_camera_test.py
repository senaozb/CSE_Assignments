import numpy as np
import tensorflow as tf
import cv2

def apply_single_scale_retinex(image):
   # Convert to float32 for Retinex
    image_float = np.float32(image)

    # Apply Retinex (Single Scale)
    sigma = 50
    retinex_image = np.log1p(image_float) - np.log1p(cv2.GaussianBlur(image_float, (0, 0), sigma))

    # Normalize the result to the range [0, 255]
    image = np.uint8(255 * (retinex_image - np.min(retinex_image)) / (np.max(retinex_image) - np.min(retinex_image)))

    return image

def filter_boxes(output, threshold):
   
    boxes = output[:, :4]
    scores = output[:, 4:]
    #print(scores)
    max_scores = np.max(scores, axis=1)
    #print(max_scores)
    
    keep = max_scores >= threshold
 
    return boxes[keep], scores[keep]

def iou(box1, box2):
   
    x1 = max(box1[0], box2[0])
    y1 = max(box1[1], box2[1])
    x2 = min(box1[2], box2[2])
    y2 = min(box1[3], box2[3])
   
    inter_area = max(0, x2 - x1 + 1) * max(0, y2 - y1 + 1)
    
    box1_area = (box1[2] - box1[0] + 1) * (box1[3] - box1[1] + 1)
    box2_area = (box2[2] - box2[0] + 1) * (box2[3] - box2[1] + 1)
   
    iou = inter_area / float(box1_area + box2_area - inter_area)
    return iou

def non_max_suppression(boxes, scores, threshold):
    keep = []
    order = scores.argsort()[::-1]
   
    while order.size > 0:
        
        i = order[0]
        keep.append(i)
        
        ious = np.array([iou(boxes[i], boxes[j]) for j in order[1:]])
       
        inds = np.where(ious <= threshold)[0]
        
        order = order[inds + 1]
    return keep

def detect(interpreter, input_image, min_prop=0.50, non_max_value=0.5):

    input_image_f32 = input_image.astype(dtype=np.float32)/ 255
    input_image_f32 = np.expand_dims(input_image_f32, axis=-1)
    # Add batch dimension
    input_data = np.expand_dims(input_image_f32, axis=0)
    # Repeat the single channel to create three channels
    input_data = np.repeat(input_data, 3, axis=-1)
    
    
    input_details = interpreter.get_input_details()
    output_details = interpreter.get_output_details()
    
    interpreter.set_tensor(input_details[0]['index'], input_data)
    
    interpreter.invoke()
    
    detect_scores = interpreter.get_tensor(output_details[0]['index'])
    detect_score = np.squeeze(detect_scores)  
    output_data = np.transpose(detect_score) 
    
    filtered_boxes, filtered_scores = filter_boxes(output_data, min_prop)
    
    max_scores = np.max(filtered_scores, axis=1)
    
    keep = non_max_suppression(filtered_boxes, max_scores, non_max_value)
   
    final_boxes = filtered_boxes[keep]
    final_scores = filtered_scores[keep]
    indexs = np.argmax(final_scores, axis=1)
    return final_boxes, final_scores, indexs

def pre_img(image):
    height, width, _ = image.shape

   
    if height > width:
        new_height = 224
        new_width = int(224 * width / height)
    else:
        new_width = 224
        new_height = int(224 * height / width)
    image_resized = cv2.resize(image, (new_width, new_height))

    background = np.ones((224, 224, 3), dtype=np.uint8) * 255


    start_x = (224 - new_width) // 2
    start_y = (224 - new_height) // 2
    background[start_y:start_y+new_height, start_x:start_x+new_width] = image_resized
    return background

# Load the TensorFlow Lite model
interpreter = tf.lite.Interpreter(model_path='/Users/senaozb/495/results_pre/runs/detect/train/weights/best_saved_model/best_float32.tflite')
interpreter.allocate_tensors()

# Open a connection to the camera (camera index 0 by default)
cap = cv2.VideoCapture(0)

while True:
    # Capture a frame from the camera
    ret, frame = cap.read()
    if not ret:
        print("Error reading frame")
        break

    # Preprocess the input frame
    input_image = pre_img(frame)
    
    # Convert to grayscale
    input_image = cv2.cvtColor(input_image, cv2.COLOR_BGR2GRAY)
    
    input_image = apply_single_scale_retinex(input_image)

    # Perform object detection
    final_boxes, final_scores, indexs = detect(interpreter, input_image)
    
    # Display the results on the frame
    names = ["defect"]
    colors = [(0, 0, 255), (255, 0, 0)]
    width, height = 224, 224
    original_height, original_width, _ = frame.shape

    for i, dt in enumerate(final_boxes):
        center_x = int(dt[0] * original_width)
        center_y = int(dt[1] * original_height)
        w = int(dt[2] * original_width)
        h = int(dt[3] * original_height)
        x = int(center_x - w / 2)
        y = int(center_y - h / 2)
        index = indexs[i]
        confidence = final_scores[i][index]
        cv2.rectangle(frame, (x, y), (x + w, y + h), colors[index], 2)
        cv2.putText(frame, f"{names[index]}: {confidence:.2f}", (x, y - 4), cv2.FONT_HERSHEY_SIMPLEX, 1, colors[index], 2, cv2.LINE_AA)

    # Display the frame
    cv2.imshow("Camera Feed", frame)

    # Exit when 'q' key is pressed
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# Release the camera and close all OpenCV windows
cap.release()
cv2.destroyAllWindows()