import cv2
import numpy as np
import os


def preprocess_image(image_path, opt):
    # Load the image
    image = cv2.imread(image_path)
    
    # Convert to grayscale
    image = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    
    if opt == 1:
        image  = 1 - image
    
    # Convert to float32 for Retinex
    image_float = np.float32(image)

    # Apply Retinex (Single Scale)
    sigma = 50
    retinex_image = np.log1p(image_float) - np.log1p(cv2.GaussianBlur(image_float, (0, 0), sigma))

    # Normalize the result to the range [0, 255]
    image = np.uint8(255 * (retinex_image - np.min(retinex_image)) / (np.max(retinex_image) - np.min(retinex_image)))

    
    return image
    


def main():
    
    # Directory containing images
    input_directory = '/Users/senaozb/Documents/495/NEU-DET/IMAGES/'

    # Output directory to save processed images
    output_directory = '/Users/senaozb/Documents/495/NEU-DET/PREPROCESSED/'


    # List all files in the input directory
    image_files = [f for f in os.listdir(input_directory) if os.path.isfile(os.path.join(input_directory, f))]

    # Iterate over each image file
    for image_file in image_files:
        if image_file.startswith('scratches_'):
            opt = 1
        else:
            opt = 0
            
        # Load the image
        if image_file.endswith('.jpg'):
            image_path = os.path.join(input_directory, image_file)
            print(image_path)
            
            preprocess_image_res = preprocess_image(image_path, opt)

            # Save the processed image to the output directory
            output_path = os.path.join(output_directory, image_file)
            cv2.imwrite(output_path, preprocess_image_res)  


main()

