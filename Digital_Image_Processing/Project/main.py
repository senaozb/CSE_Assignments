import cv2
import numpy as np
from skimage import filters

# Load the image
image = cv2.imread('image1.jpg')

# Resize the image
image = cv2.resize(image, (1500,1500))

# Apply bilateral filter
denoised_image = cv2.bilateralFilter(image, 9, 75, 75)

# Convert the image to grayscale
denoised_image = cv2.cvtColor(denoised_image, cv2.COLOR_BGR2GRAY)

# Apply non-local means denoising
denoised_image = cv2.fastNlMeansDenoising(denoised_image, None, h=10, templateWindowSize=10, searchWindowSize=21)

# Apply histogram equalization
clahe = cv2.createCLAHE(clipLimit=2.0, tileGridSize=(8, 8))
clahe_image = clahe.apply(denoised_image)

# Apply Sobel operator
sobel_x = cv2.Sobel(clahe_image, cv2.CV_64F, 1, 0, ksize=5)
sobel_y = cv2.Sobel(clahe_image, cv2.CV_64F, 0, 1, ksize=5)
sobel_magnitude = np.sqrt(sobel_x**2 + sobel_y**2)

# Apply Meijering filter
sigmas = range(1, 3)
filtered = filters.meijering(sobel_magnitude, sigmas=sigmas, black_ridges=False)
frame = np.ones_like(filtered, dtype=bool)
d = 2 * np.max(sigmas) + 1 
d += 1 
frame[d:-d, d:-d] = False
filtered[frame] = np.min(filtered)

# Thresholds for hysteresis edge tracking
threshold_low = 0.03
threshold_high = 0.05

weak_edges = (filtered > threshold_low) & (filtered <= threshold_high)
strong_edges = (filtered > threshold_high)

# Perform hysteresis edge tracking
def track_edges(i, j):
    if i < 0 or i >= weak_edges.shape[0] or j < 0 or j >= weak_edges.shape[1]:
        return
    if weak_edges[i, j]:
        weak_edges[i, j] = 0
        strong_edges[i, j] = 1
        for ii in range(i - 1, i + 2):
            for jj in range(j - 1, j + 2):
                track_edges(ii, jj)

for i in range(1, weak_edges.shape[0] - 1):
    for j in range(1, weak_edges.shape[1] - 1):
        if weak_edges[i, j]:
            track_edges(i, j)
            
# Combine strong and linked weak edges into an array
all_edges = (strong_edges | weak_edges).astype(np.uint8) * 255
# Combine edges with the original image
result_image = cv2.bitwise_or(image, cv2.merge([all_edges, all_edges, all_edges]))

image = cv2.resize(image, (800, 800))
clahe_image = cv2.resize(clahe_image, (800, 800))
sobel_magnitude = cv2.resize(cv2.convertScaleAbs(sobel_magnitude), (800, 800))
filtered = cv2.resize(filtered, (800, 800))
all_edges = cv2.resize(all_edges, (800, 800))
result_image = cv2.resize(result_image, (800, 800))

# Display the images
cv2.imshow('Original Image', image)
cv2.imshow('Denoised Image', clahe_image)
cv2.imshow('Sobel Magnitude', cv2.convertScaleAbs(sobel_magnitude))
cv2.imshow('Meijering Enhanced', filtered)
cv2.imshow('Hysteresis Edge Tracking', all_edges)
cv2.imshow('Result', result_image)
cv2.waitKey(0)
cv2.destroyAllWindows()
