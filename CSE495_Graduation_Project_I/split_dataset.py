import os
import random
import shutil

# Set the path to your original dataset folder
original_dataset_path = '/Users/senaozb/Documents/495/NEU-DET/IMAGES/'
original_annotation_path = '/Users/senaozb/Documents/495/NEU-DET/ANNOTATIONS/'

# Set the paths for the new train and validation folders
train_folder = '/Users/senaozb/Documents/495/NEU-DET/TRAIN/'
validation_folder = '/Users/senaozb/Documents/495/NEU-DET/VAL/'

# Set the percentage of data to use for validation
validation_split = 0.2

# Get the list of all images in the original dataset folder
all_images = os.listdir(original_dataset_path)
all_images.remove('.DS_Store')

# Calculate the number of images to move to the validation set
num_validation = int(validation_split * len(all_images))

# Randomly shuffle the list of images
random.shuffle(all_images)

# Move images to the validation set
for image_name in all_images[:num_validation]:
    source_path = os.path.join(original_dataset_path, image_name)
    destination_path = os.path.join(validation_folder+'IMAGES/', image_name)
    shutil.copy(source_path, destination_path)

    source_path = os.path.join(original_annotation_path, image_name.split('.')[0]+'.xml')
    destination_path = os.path.join(validation_folder+'ANNOTATIONS/', image_name.split('.')[0]+'.xml')
    shutil.copy(source_path, destination_path)

# Move the remaining images to the training set
for image_name in all_images[num_validation:]:
    source_path = os.path.join(original_dataset_path, image_name)
    destination_path = os.path.join(train_folder+'IMAGES/', image_name)
    shutil.copy(source_path, destination_path)
    
    source_path = os.path.join(original_annotation_path, image_name.split('.')[0]+'.xml')
    destination_path = os.path.join(train_folder+'ANNOTATIONS/', image_name.split('.')[0]+'.xml')
    shutil.copy(source_path, destination_path)