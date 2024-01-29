import cv2
import math
import numpy as np

def interpolation_calc(x, y, image):
    
    # Find the corners 
    x1 = int(math.floor(x))
    y1 = int(math.floor(y))
    x2 = min(math.ceil(x), len(image[0])-1)
    y2 = min(math.ceil(y), len(image)-1)
    
    # Apply for all z dimensions (for 3)
    
    # Get the neighbor pixels' values
    q11, q21, q12, q22 = image[y1][x1][0], image[y1][x2][0], image[y2][x1][0], image[y2][x2][0]
    
    # Get the final intensity regarding the neighbor pixels
    if (x2 == x1) and (y2 == y1): # If there is no need for neighbors (integers)
        P_0 = image[int(y)][int(x)][0]
    elif (x2 == x1):  # If there are only two neighbors vertically
        P_0 =  image[y1][int(x)][0] * (y2-y) / (y2-y1) + image[y2][int(x)][0] * (y-y1) / (y2-y1)
    elif (y2 == y1):  # If there are only two neighbors horizontally
        P_0 =  image[int(y)][x1][0] * (x2-x) / (x2-x1) + image[int(y)][x2][0] * (x-x1) / (x2-x1)
    else: # If all neighbors are available
        R1_0 =  q11 * (x2-x) / (x2-x1) + q21 * (x-x1) / (x2-x1)
        R2_0 = q12 * (x2-x) / (x2-x1) + q22 * (x-x1) / (x2-x1)
        P_0 = R1_0 * (y2-y) / (y2-y1) + R2_0 * (y-y1) / (y2-y1) 
    
    # Get the neighbor pixels' values
    q11, q21, q12, q22 = image[y1][x1][1], image[y1][x2][1], image[y2][x1][1], image[y2][x2][1]
    
    # Get the final intensity regarding the neighbor pixels
    if (x2 == x1) and (y2 == y1): # If there is no need for neighbors (integers)
        P_1 = image[int(y)][int(x)][1]
    elif (x2 == x1): # If there are only two neighbors vertically
        P_1 =  image[y1][int(x)][1] * (y2-y) / (y2-y1) + image[y2][int(x)][1] * (y-y1) / (y2-y1)
    elif (y2 == y1): # If there are only two neighbors horizontally
        P_1 =  image[int(y)][x1][1] * (x2-x) / (x2-x1) + image[int(y)][x2][1] * (x-x1) / (x2-x1)
    else: # If all neighbors are available
        R1_1 =  q11 * (x2-x) / (x2-x1) + q21 * (x-x1) / (x2-x1)
        R2_1 = q12 * (x2-x) / (x2-x1) + q22 * (x-x1) / (x2-x1)
        P_1 = R1_1 * (y2-y) / (y2-y1) + R2_1 * (y-y1) / (y2-y1) 
    
    # Get the neighbor pixels' values
    q11, q21, q12, q22 = image[y1][x1][2], image[y1][x2][2], image[y2][x1][2], image[y2][x2][2]
    
    # Get the final intensity regarding the neighbor pixels
    if (x2 == x1) and (y2 == y1): # If there is no need for neighbors (integers)
        P_2 = image[int(y)][int(x)][2]
    elif (x2 == x1): # If there are only two neighbors vertically
        P_2 =  image[y1][int(x)][2] * (y2-y) / (y2-y1) + image[y2][int(x)][2] * (y-y1) / (y2-y1)
    elif (y2 == y1): # If there are only two neighbors horizontally
        P_2 =  image[int(y)][x1][2] * (x2-x) / (x2-x1) + image[int(y)][x2][2] * (x-x1) / (x2-x1)
    else: # If all neighbors are available
        R1_2 =  q11 * (x2-x) / (x2-x1) + q21 * (x-x1) / (x2-x1)
        R2_2 = q12 * (x2-x) / (x2-x1) + q22 * (x-x1) / (x2-x1)
        P_2 = R1_2 * (y2-y) / (y2-y1) + R2_2 * (y-y1) / (y2-y1) 
    
    results = [P_0, P_1, P_2]
    
    return results

def inverse_matrix(matrix):
    
    a, b, c = matrix[0]
    d, e, f = matrix[1]
    g, h, i = matrix[2]
    
    # Calculate the determinant
    det = a * (e * i - f * h) - b * (d * i - f * g) + c * (d * h - e * g)

    # Check whether the determinant is zero. If it is zero, this matrix does not have an inverse version
    if det == 0:
        print("This matrix does not have an inverse version")
    else:
        a, b, c = matrix[0]
        d, e, f = matrix[1]
        g, h, i = matrix[2]
        
        inverse_matrix = [[(e * i - f * h) / det, (c * h - b * i) / det, (b * f - c * e) / det],
                        [(f * g - d * i) / det, (a * i - c * g) / det, (c * d - a * f) / det],
                        [(d * h - e * g) / det, (b * g - a * h) / det, (a * e - b * d) / det]]

            
    return inverse_matrix
            
            

def matrix_vector_multiplication(matrix, vector):
    result = [0] * len(matrix)

    # Perform matrix-vector multiplication
    for i in range(len(matrix)):
        for j in range(len(vector)):
            result[i] += matrix[i][j] * vector[j]
            
    return result



def scale_img(image, scale_x, scale_y, mapping, interpolation):
    # Define the affine matrix for scaling
    scale_affine_matrix = [[scale_x, 0, 0], [0, scale_y, 0], [0, 0, 1]]
   
    # Calculate the new dimensions using old dimensions
    old_width = len(image[0])
    old_height = len(image)
    new_width = int(old_width * scale_x)
    new_height = int(old_height * scale_y)
    
    # Create a new matrix for the new image
    new_image = [[[0 for k in range(3)] for j in range(new_width)] for i in range(new_height)]
    
    if mapping == 'forward':
        for y in range(old_height):
            for x in range(old_width):
                # Calculate the new coordinates for the current coordinates
                point = [x, y, 1]
                result = matrix_vector_multiplication(scale_affine_matrix, point)
                transformed_x = int(result[0])
                transformed_y = int(result[1])
                
                # Use forward mapping to transform the intensity
                if transformed_x >= 0 and transformed_x < new_width and transformed_y >= 0 and transformed_y < new_height:
                    new_image[transformed_y][transformed_x][0] = image[y][x][0]
                    new_image[transformed_y][transformed_x][1] = image[y][x][1]
                    new_image[transformed_y][transformed_x][2] = image[y][x][2]
    else:
        for y in range(new_height):
            for x in range(new_width):
            # Calculate the new coordinates for the current coordinates
                point = [x, y, 1]
                inverse_matrix_result = inverse_matrix(scale_affine_matrix)
                result = matrix_vector_multiplication(inverse_matrix_result, point)
                transformed_x = result[0]
                transformed_y = result[1]
                
                # Use backward mapping to transform the intensity
                if interpolation == False:
                    transformed_x = int(transformed_x)
                    transformed_y = int(transformed_y)
                    if transformed_x >= 0 and transformed_x < old_width and transformed_y >= 0 and transformed_y < old_height:
                        new_image[y][x][0] = image[transformed_y][transformed_x][0]
                        new_image[y][x][1] = image[transformed_y][transformed_x][1]
                        new_image[y][x][2] = image[transformed_y][transformed_x][2]
                else:
                    if transformed_x >= 0 and transformed_x < old_width and transformed_y >= 0 and transformed_y < old_height:
                        results = interpolation_calc(transformed_x, transformed_y, image)
                        new_image[y][x][0] = results[0]
                        new_image[y][x][1] = results[1]
                        new_image[y][x][2] = results[2]
                        
            
    return new_image


    
def rotate_img(image, rotate_degree, mapping, interpolation):
    # Define the affine matrix for rotating
    rotate_affine_matrix = [[math.cos(rotate_degree), -math.sin(rotate_degree), 0], [math.sin(rotate_degree), math.cos(rotate_degree), 0], [0, 0, 1]]
    
    # Calculate the dimensions
    old_width = len(image[0])
    old_height = len(image)
    
    new_width = int(old_width*math.cos(rotate_degree) + old_height*math.sin(rotate_degree))
    new_height = int(old_height*math.cos(rotate_degree) + old_width*math.sin(rotate_degree))

    # Create a new matrix for the new image 
    new_image = [[[0 for k in range(3)] for j in range(new_width)] for i in range(new_height)]
    
    if mapping == 'forward':
        for y in range(old_height):
            for x in range(old_width):
                # Calculate the new coordinates for the current coordinates
                point = [x, y, 1]
                result = matrix_vector_multiplication(rotate_affine_matrix, point)
                transformed_x = int(result[0])
                transformed_y = int(result[1])
                
                # Use forward mapping to transform the intensity
                transformed_x += int(old_height*math.sin(rotate_degree)) # to show the whole image
                if transformed_x >= 0 and transformed_x < new_width and transformed_y >= 0 and transformed_y < new_height:
                    new_image[transformed_y][transformed_x][0] = image[y][x][0]
                    new_image[transformed_y][transformed_x][1] = image[y][x][1]
                    new_image[transformed_y][transformed_x][2] = image[y][x][2]
    else:
        for y in range(new_height):
            for x in range(new_width):
                # Calculate the new coordinates for the current coordinates
                point = [x-int(old_height*math.sin(rotate_degree)), y, 1]  # to access the parts in the negative side on x-axis
                inverse_matrix_result = inverse_matrix(rotate_affine_matrix)
                result = matrix_vector_multiplication(inverse_matrix_result, point)
                transformed_x = result[0]
                transformed_y = result[1]
                
                # Use backward mapping to transform the intensity
                if interpolation == False:
                    transformed_x = int(transformed_x)
                    transformed_y = int(transformed_y)
                    if transformed_x >= 0 and transformed_x < old_width and transformed_y >= 0 and transformed_y < old_height:
                        new_image[y][x][0] = image[transformed_y][transformed_x][0]
                        new_image[y][x][1] = image[transformed_y][transformed_x][1]
                        new_image[y][x][2] = image[transformed_y][transformed_x][2]
                else:
                    if transformed_x >= 0 and transformed_x < old_width and transformed_y >= 0 and transformed_y < old_height:
                        results = interpolation_calc(transformed_x, transformed_y, image)
                        new_image[y][x][0] = results[0]
                        new_image[y][x][1] = results[1]
                        new_image[y][x][2] = results[2]
            
    return new_image



def shear_img(image, shear_x, shear_y, mapping, interpolation):
    # Define the affine matrix for shearing
    shear_affine_matrix = [[1, shear_x, 0], [shear_y, 1, 0], [0, 0, 1]]
    
    # Calculate the new dimensions by calculating the new right-down corner 
    old_width = len(image[0])
    old_height = len(image)
    point = [old_width-1, old_height-1, 1]
    
    result = matrix_vector_multiplication(shear_affine_matrix, point)
    
    new_width = int(result[0])
    new_height = int(result[1])
    
    # Create a new matrix for the new image 
    new_image = [[[0 for k in range(3)] for j in range(new_width)] for i in range(new_height)]
    
    if mapping == 'forward':
        for y in range(old_height):
            for x in range(old_width):
                # Calculate the new coordinates for the current coordinates
                point = [x, y, 1]
                result = matrix_vector_multiplication(shear_affine_matrix, point)
                transformed_x = int(result[0])
                transformed_y = int(result[1])
                
                # Use forward mapping to transform the intensity
                if transformed_x >= 0 and transformed_x < new_width and transformed_y >= 0 and transformed_y < new_height:
                    new_image[transformed_y][transformed_x][0] = image[y][x][0]
                    new_image[transformed_y][transformed_x][1] = image[y][x][1]
                    new_image[transformed_y][transformed_x][2] = image[y][x][2]
    else:
        for y in range(new_height):
            for x in range(new_width):
            # Calculate the new coordinates for the current coordinates
                point = [x, y, 1]
                inverse_matrix_result = inverse_matrix(shear_affine_matrix)
                result = matrix_vector_multiplication(inverse_matrix_result, point)
                transformed_x = result[0]
                transformed_y = result[1]
                
                # Use backward mapping to transform the intensity
                if interpolation == False:
                    transformed_x = int(transformed_x)
                    transformed_y = int(transformed_y)
                    if transformed_x >= 0 and transformed_x < old_width and transformed_y >= 0 and transformed_y < old_height:
                        new_image[y][x][0] = image[transformed_y][transformed_x][0]
                        new_image[y][x][1] = image[transformed_y][transformed_x][1]
                        new_image[y][x][2] = image[transformed_y][transformed_x][2]
                else:
                    if transformed_x >= 0 and transformed_x < old_width and transformed_y >= 0 and transformed_y < old_height:   
                        results = interpolation_calc(transformed_x, transformed_y, image)
                        new_image[y][x][0] = results[0]
                        new_image[y][x][1] = results[1]
                        new_image[y][x][2] = results[2]
                
    return new_image
            
    
    
def zoom_img(image, zoom, mapping, interpolation):
    # Scale the image by a factor of 1.6
    new_image_array = scale_img(image, zoom, zoom, mapping, interpolation)
    
    # Get the current dimensions and original dimensions
    old_width = len(image[0])
    old_height = len(image)
    new_width = len(new_image_array[0])
    new_height = len(new_image_array)
    
    
    # Create a matrix with original dimensions
    original_dimension_image = [[[0 for k in range(3)] for j in range(old_width)] for i in range(old_height)]
    
    # Calculate the differences between dimensions and divide them by 2 
    # This is used to crop the image
    width_diff = new_width - old_width
    height_diff = new_height - old_height
    
    width_diff = width_diff // 2
    height_diff = height_diff // 2
    
    for y in range(old_height):
        for x in range(old_width):
            # Crop the image by removing the outer parts from all sides equally
            original_dimension_image[y][x][0] = new_image_array[height_diff + y][width_diff + x][0]
            original_dimension_image[y][x][1] = new_image_array[height_diff + y][width_diff + x][1]
            original_dimension_image[y][x][2] = new_image_array[height_diff + y][width_diff + x][2]
    
            
    return original_dimension_image



def main():
    # load the image
    image = cv2.imread('istanbul.jpg')

    # convert the image to an array
    if image is not None:
        image_array = image.tolist()
    else:
        print("Image not available")
        exit()
    
    
    # affine matrix parameters
    scale_x = 2
    scale_y = 0.5
    rotate_degree = math.radians(30)
    shear_x = 2
    shear_y = 0
    zoom = 1.6

    mapping = 'forward'
    interpolation = False
    operation = 'scale' # scale / rotate / zoom / shear
    
    if operation == 'scale':
        new_image_array = scale_img(image_array, scale_x, scale_y, mapping, interpolation)
    elif operation == 'rotate':
        new_image_array = rotate_img(image_array, rotate_degree, mapping, interpolation)
    elif operation == 'zoom':
        new_image_array = zoom_img(image_array, zoom, mapping, interpolation)
    elif operation == 'shear':
        new_image_array = shear_img(image_array, shear_x, shear_y, mapping, interpolation)
    else:
        print('Operation not supported')
        exit()
    
    
    new_image = np.array(new_image_array, dtype=np.uint8)
    cv2.imwrite('transformed_image.jpg', new_image)

    # Display the image
    cv2.imshow('Image', new_image)
    cv2.waitKey(0)
    cv2.destroyAllWindows()
    
main()
