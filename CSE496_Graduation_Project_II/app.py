from keras.datasets import mnist
from extra_keras_datasets import kmnist
import numpy as np
import matplotlib.pyplot as plt
from random import sample
from PIL import Image
import cv2
from sklearn.preprocessing import MinMaxScaler
from sklearn.metrics import euclidean_distances
import sys
from PyQt5.QtWidgets import QApplication, QWidget, QVBoxLayout, QFormLayout, QLineEdit, QComboBox, QPushButton, QLabel, QDoubleSpinBox, QSpinBox


# The size of the images
new_width, new_height = 20, 20

# Define functions for loading the datasets and preprocessing images
def load_dataset(option):
    
    if option == "MNIST":
       # Load the MNIST dataset
        (x_train, y_train), (x_test, y_test) = mnist.load_data()

        # Select a random subset of images for each digit
        images = []
        labels = []
        for digit in range(10):
            digit_indices = np.where(y_train == digit)[0]
            selected_indices = sample(list(digit_indices), 100)  # Select 100 random images for each digit
            for index in selected_indices:
                images.append(x_train[index])
                labels.append(y_train[index])

        images_array = np.array(images)
        labels_array = np.array(labels)
        
    elif option == "KMNIST":
        (x_train, y_train), (x_test, y_test) = kmnist.load_data(type='kmnist')
   
        # Select a random subset of images for each class
        images = []
        labels = []
        for unique_class in range(0, 10):
            unique_class_indices = np.where(y_train == unique_class)[0]
            selected_indices = sample(list(unique_class_indices), 100)  # Select 100 random images for each class
            for index in selected_indices:
                images.append(x_train[index])
                labels.append(y_train[index])
      
        images_array = np.array(images)
        labels_array = np.array(labels)

      
        
    # Resize the images using Pillow
    resized_images = []
    for image in images_array:
        # Convert the NumPy array to a PIL image
        pil_image = Image.fromarray(image.astype(np.uint8))
        # Resize the image to the new dimensions
        resized_pil_image = pil_image.resize((new_width, new_height))
        # Convert the resized PIL image back to a NumPy array
        resized_image = np.array(resized_pil_image)
        # Add the resized image to the list
        resized_images.append(resized_image)

    # Convert the list of resized images back to a NumPy array
    resized_images_array = np.array(resized_images)

    # Reshape images array to 1D array
    images_array = resized_images_array.reshape((-1, new_width * new_height))
        
    # Scale the images
    images_array = MinMaxScaler().fit_transform(images_array)    
    
    return images_array, labels_array


# Define the function to prepare dataset  
def data_prep(images, std):
    # Get the mean image
    mean_image = np.mean(images, axis=0)
    
    # Define noise params
    mean = 0.0
    std_dev = std

    # Make the images noisy via Gaussian noise
    noise = np.random.normal(mean, std_dev, images.shape)
    noised_images = images + noise
    
    # Center the noisy images
    noised_images_centered = noised_images - mean_image
    
    return mean_image, noised_images, noised_images_centered

# Define the RBF kernel function
def rbf_kernel(X, gamma=0.01):
    K = euclidean_distances(X, squared=True)
    K *= -gamma
    np.exp(K, K) 
    return K

# Define the polynomial kernel function
def polynomial_kernel(X, degree=4, coef0=2):
    return np.power(np.abs(np.dot(X, X.T) + coef0), degree)

# Define the sigmoid kernel function
def sigmoid_kernel(X, gamma=0.01, coef0=0):
    return np.tanh(gamma * np.dot(X, X.T) + coef0)

def eigen_decomposition(option, data, num_components, kernel_function, gamma, degree, coef0):
    if option == "Linear":
        # Compute the covariance matrix
        matrix = np.cov(data, rowvar=False)
    elif option == "Kernel":
        # Compute the kernel matrix
        if kernel_function == polynomial_kernel: 
            K = polynomial_kernel(data, degree=degree, coef0=coef0)
        elif kernel_function == sigmoid_kernel:
            K = sigmoid_kernel(data, gamma=gamma, coef0=coef0)
        elif kernel_function == rbf_kernel:
            K = rbf_kernel(data, gamma)
        else:
            raise ValueError("Invalid kernel function")

        # Center the kernel matrix
        N = K.shape[0]
        one_n = np.ones((N, N)) / N
        matrix = K - one_n.dot(K) - K.dot(one_n) + one_n.dot(K).dot(one_n)
        
    

    # Perform eigen decomposition
    eigenvalues, eigenvectors = np.linalg.eigh(matrix)

    # Sort eigenvectors based on eigenvalues
    sorted_indices = np.argsort(eigenvalues)[::-1]
    eigenvalues = eigenvalues[sorted_indices]
    eigenvectors = eigenvectors[:, sorted_indices]
    
    # Select the top principal components
    principal_components = eigenvectors[:, :num_components]
    
    return principal_components, matrix, eigenvalues
   

def data_transformation(option, data, pc, mean_image, matrix):    
    if option == "Linear":
        # Project data onto the selected principal components
        projected_data = np.dot(data, pc)
        # Reconstruct the denoised data 
        denoised_data = np.dot(projected_data, pc.T)
        # Add the mean image
        denoised_data = denoised_data + mean_image
    elif option == "Kernel":
        # Project data onto the selected principal components
        projected_data = np.dot(matrix, pc)
        # Reconstruct the denoised data in the kernel space
        denoised_data = np.dot(projected_data, pc.T)
        # Inverse transform the denoised data back to the original space 
        denoised_data = np.dot(denoised_data, data) + mean_image

    return denoised_data, projected_data

def plot_cumulative_explained_variance(eigenvalues):
    # Calculate cumulative explained variance
    eigenvalues = np.maximum(eigenvalues, 0)
    total_variance = np.sum(eigenvalues)
    cumulative_variance = np.cumsum(eigenvalues) / total_variance
   
    # Plot cumulative explained variance
    plt.figure()
    plt.plot(range(1, len(cumulative_variance) + 1), cumulative_variance)
    plt.xlabel('Number of Principal Components')
    plt.ylabel('Cumulative Explained Variance')
    plt.title('Cumulative Explained Variance')
    plt.grid(True)
    plt.show()

def plot_pca_components(pca_type, principal_components, num_components, new_width, new_height, data):
   
    fig = plt.figure(figsize=(15, 5))
    fig.suptitle(f'First {num_components} Principal Components', fontsize=16)
    
    for i in range(num_components):
        # For linear PCA: reshape the principal component into the original image dimensions
        if pca_type == "Linear":
            component_image = principal_components[:, i].reshape((new_width, new_height))
        
        # For kernel PCA: reconstruct the eigenvector (principal component) from the data
        elif pca_type == "Kernel":
            # Reconstruct the component using the data
            component_image = np.dot(principal_components[:, i], data).reshape((new_width, new_height))

        # Plot the component
        plt.subplot(1, num_components, i + 1)
        plt.imshow(component_image, cmap='Greys')
        plt.axis('off')

    plt.show()

def plot_pca_comparison(projected_data, c):
    # Project data onto the first and second principal components for plotting
    X_pc1 = projected_data[:, 0]
    X_pc2 = projected_data[:, 1]
    
    # Plotting the data points on the first and second principal components
    plt.figure(figsize=(8, 6))
    scatter = plt.scatter(X_pc1, X_pc2, c=c, cmap='viridis', alpha=0.6)
    plt.xlabel('Principal Component 1')
    plt.ylabel('Principal Component 2')
    plt.title('First and Second Principal Components')
    plt.colorbar(scatter, label='Class')
    plt.show()
    
def normalize_image(image):
    """
    Normalize the image to have values between 0 and 1.
    """
    min_val = np.min(image)
    max_val = np.max(image)
    if max_val == min_val:
        return image - min_val  # To handle cases where the image is constant
    normalized_image = (image - min_val) / (max_val - min_val)
    return normalized_image
    
def calculate_rmse(original_images, denoised_images):
    """
    Calculate the Root Mean Square Error (RMSE) between arrays of original and denoised images.
    """
    rmse_values = []
    for original, denoised in zip(original_images, denoised_images):
        # Normalize the denoised image
        denoised_normalized = normalize_image(denoised)
    
        # Flatten the images
        original_flat = original.flatten()
        denoised_flat = denoised_normalized.flatten()
    
        # Calculate the squared error
        squared_error = np.square(original_flat - denoised_flat)
        
        # Calculate the mean squared error
        mse = np.mean(squared_error)
        
        # Calculate the RMSE
        rmse = np.sqrt(mse)
        
        rmse_values.append(rmse)
    
    return np.mean(rmse_values)

def calculate_snr(original_images, noisy_images):
    """
    Calculate the Signal-to-Noise Ratio (SNR) in decibels (dB) between arrays of original and noisy images.
    """
    snr_values = []
    for original, noisy in zip(original_images, noisy_images):
        # Normalize both the noisy images
        noisy_normalized = normalize_image(noisy)
        
        # Flatten the images
        original_flat = original.flatten()
        noisy_flat = noisy_normalized.flatten()
        
        # Calculate signal power
        signal_power = np.sum(np.square(original_flat))
        
        # Calculate noise power
        noise_power = np.sum(np.square(original_flat - noisy_flat))
        
        # Calculate SNR in decibels (dB)
        snr_db = 10 * np.log10(signal_power / noise_power)
        
        snr_values.append(snr_db)
    
    return np.mean(snr_values)

def show_error_values(rmse, snr):
    # Create a new figure for displaying RMSE and SNR
    plt.figure(figsize=(10, 5))

    # Display RMSE
    plt.subplot(1, 2, 1)
    plt.text(0.5, 0.5, f'Root Mean Square Error: {rmse:.4f}', ha='center', va='center', fontsize=12)
    plt.axis('off')

    # Display SNR
    plt.subplot(1, 2, 2)
    plt.text(0.5, 0.5, f'Signal-to-Noise Ratio: {snr:.4f} dB', ha='center', va='center', fontsize=12)
    plt.axis('off')

    # Show the plot
    plt.show()
    

def process(image, alpha, darkness):  
        
    # Increase the contrast
    contrasted = cv2.convertScaleAbs(image, alpha=alpha, beta=0) 
    
    # Create a mask for non-white pixels (255 becomes 0, everything else stays the same)
    mask = cv2.compare(contrasted, 255, cv2.CMP_NE)  # Not equal to 255
    inverted_mask = cv2.bitwise_not(mask)
     
   # Darken the digit area
    darkened_digit_area = cv2.subtract(contrasted, darkness)  # Adjust the subtraction value to control darkness

    # Create a white canvas
    white_background = np.full_like(image, 255, dtype=np.uint8)

    # Combine the darkened digit area with the white background using the mask
    result = cv2.bitwise_or(cv2.bitwise_and(white_background, inverted_mask), cv2.bitwise_and(darkened_digit_area, mask))


    return result

def show_processed_image(pca_type, original_images, denoised_images):
   
    fig, axs = plt.subplots(3, 10, figsize=(20, 6))
    fig.suptitle('Original vs Denoised vs Processed Images', fontsize=16)
    for i in range(10):
        # Original images
        axs[0, i].imshow(original_images[i*100], cmap='Greys')
        axs[0, i].axis('off')
        
        # Denoised images
        axs[1, i].imshow(denoised_images[i*100], cmap='Greys')
        axs[1, i].axis('off')
    
        img = cv2.imread(f"denoised_{i}.png", cv2.IMREAD_GRAYSCALE) 
        if img[0][0] >= 200:
            result = process(img, 1.4, 30)  
       
        elif img[0][0] >=150 and img[0][0] < 200:
            result = process(img, 1.8, 40)
            
        else:
            result = process(img, 2.2, 50)

        # Plot the component
        axs[2, i].imshow(result, cmap='gray')
        axs[2, i].axis('off')

   
    plt.tight_layout()
    plt.show()


class ParameterSelector(QWidget):
    def __init__(self):
        super().__init__()

        # Set up the layout
        self.setWindowTitle("Noise Reduction with PCA")
        layout = QVBoxLayout()
        form_layout = QFormLayout()

        # Dataset
        self.dataset_combo = QComboBox()
        self.dataset_combo.addItems(["MNIST", "KMNIST"])
        form_layout.addRow("Dataset:", self.dataset_combo)

        # Noise
        self.noise_spin = QDoubleSpinBox()
        self.noise_spin.setRange(0.0, 1.0)
        self.noise_spin.setSingleStep(0.05)
        self.noise_spin.setValue(0.20)
        form_layout.addRow("Noise:", self.noise_spin)

        # k
        self.k_spin = QSpinBox()
        self.k_spin.setRange(1, 1000)
        self.k_spin.setValue(20)
        form_layout.addRow("Principal Components:", self.k_spin)

        # PCA
        self.pca_combo = QComboBox()
        self.pca_combo.addItems(["Linear", "Kernel"])
        form_layout.addRow("PCA:", self.pca_combo)

        # Kernel Function
        self.kernel_function_combo = QComboBox()
        self.kernel_function_combo.addItems(["RBF", "Polynomial", "Sigmoid"])
        form_layout.addRow("Kernel Function:", self.kernel_function_combo)

        # Gamma
        self.gamma_combo = QComboBox()
        self.gamma_combo.addItems(["0.1", "0.01", "0.001"])
        form_layout.addRow("Gamma for Sigmoid and RBF:", self.gamma_combo)

        # coef0
        self.coef0_spin = QDoubleSpinBox()
        self.coef0_spin.setRange(0, 3)
        self.coef0_spin.setSingleStep(1)
        self.coef0_spin.setValue(0)
        form_layout.addRow("coef0 for Sigmoid and Polynomial:", self.coef0_spin)

        # degree
        self.degree_spin = QSpinBox()
        self.degree_spin.setRange(1, 5)
        self.degree_spin.setValue(2)
        form_layout.addRow("Degree for Polynomial:", self.degree_spin)

        # Submit Button
        self.submit_button = QPushButton("Denoise")
        self.submit_button.clicked.connect(self.submit)
        form_layout.addRow(self.submit_button)

        # Result Label
        self.result_label = QLabel("")
        form_layout.addRow(self.result_label)

        layout.addLayout(form_layout)
        self.setLayout(layout)

    def submit(self):
        dataset = self.dataset_combo.currentText()
        noise = self.noise_spin.value()
        k = self.k_spin.value()
        pca = self.pca_combo.currentText()
        kernel_option = self.kernel_function_combo.currentText()
        gamma = float(self.gamma_combo.currentText())
        coef0 = self.coef0_spin.value()
        degree = self.degree_spin.value()
        
        # Define kernel function based on user selection
        if kernel_option == "RBF":
            kernel_function = rbf_kernel
        elif kernel_option == "Polynomial":
            kernel_function = polynomial_kernel
        elif kernel_option == "Sigmoid":
            kernel_function = sigmoid_kernel
        
                
        images, labels = load_dataset(dataset)
        mean_image, noised_images, images_prep = data_prep(images, noise)
        pc, matrix, eigenvalues = eigen_decomposition(pca, images_prep, k, kernel_function, gamma, degree, coef0)
        denoised_images, projected_data = data_transformation(pca, images_prep, pc, mean_image, matrix)

        # Reshape the original, noised, and denoised images
        original_images = images.reshape((-1, new_width, new_height))
        noised_images = noised_images.reshape((-1, new_width, new_height))
        denoised_images = denoised_images.reshape((-1, new_width, new_height))

        fig, axs = plt.subplots(3, 20, figsize=(20, 6))
        fig.suptitle('Original vs Noisy vs Denoised Images', fontsize=16)
        for i in range(20):
            # Original images
            axs[0, i].imshow(original_images[i*50], cmap='Greys')
            axs[0, i].axis('off')

            # Noised images
            axs[1, i].imshow(noised_images[i*50], cmap='Greys')
            axs[1, i].axis('off')

            # Denoised images
            axs[2, i].imshow(denoised_images[i*50], cmap='Greys')
            axs[2, i].axis('off')
            

        plt.tight_layout()
        plt.show()


        # Plot the cumulative variance
        plot_cumulative_explained_variance(eigenvalues)

        # Plot the top 20 principal components 
        if k < 20 : 
            chosen_k = k 
        else:
            chosen_k = 20
            
        plot_pca_components(pca, pc, chosen_k, new_width, new_height, images_prep)

        # Plot the first and second components with class labels
        plot_pca_comparison(projected_data, labels)

        # Calculate RMSE between original and denoised images
        rmse = calculate_rmse(original_images, denoised_images)

        # Calculate SNR between original and denoised images
        snr = calculate_snr(original_images, denoised_images)

        # Show the values
        show_error_values(rmse, snr)

        # Process the images via image processing
        for i in range(10):
            plt.imsave(f"denoised_{i}.png", denoised_images[i*100], cmap='Greys')

        show_processed_image(pca, original_images, denoised_images)

       
       
if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = ParameterSelector()
    window.show()
    sys.exit(app.exec_())








