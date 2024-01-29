import xml.etree.ElementTree as ET
import os

def convert_annotation(xml_path, image_width, image_height):
    tree = ET.parse(xml_path)
    root = tree.getroot()

    yolo_labels = []
    for obj in root.iter('object'):
        class_name = 'defect'
        class_index = 0

        bbox = obj.find('bndbox')
        x_center = (float(bbox.find('xmin').text) + float(bbox.find('xmax').text)) / 2.0 / image_width
        y_center = (float(bbox.find('ymin').text) + float(bbox.find('ymax').text)) / 2.0 / image_height
        width = (float(bbox.find('xmax').text) - float(bbox.find('xmin').text)) / image_width
        height = (float(bbox.find('ymax').text) - float(bbox.find('ymin').text)) / image_height

        yolo_labels.append(f"{class_index} {x_center} {y_center} {width} {height}")

    return yolo_labels

def main():
    # Define your classes
    classes = ['defect']

    # Path to the folder containing VOC XML annotations
    xml_folder1 = '/Users/senaozb/Documents/495/dataset/train/labels-org/'
    xml_folder2 = '/Users/senaozb/Documents/495/dataset/val/labels-org/'

    # Output folder for YOLO format labels
    output_folder1 = '/Users/senaozb/Documents/495/dataset/train/labels/'
    output_folder2 = '/Users/senaozb/Documents/495/dataset/val/labels/'

    for xml_file in os.listdir(xml_folder1):
        if xml_file.endswith('.xml'):
            xml_path = os.path.join(xml_folder1, xml_file)

            image_height, image_width = 200, 200

            # Convert annotation
            yolo_labels = convert_annotation(xml_path, image_width, image_height)

            # Write to YOLO format file
            yolo_file_path = os.path.join(output_folder1, xml_file.replace('.xml', '.txt'))
            with open(yolo_file_path, 'w') as yolo_file:
                yolo_file.write('\n'.join(yolo_labels))
                
    for xml_file in os.listdir(xml_folder2):
        if xml_file.endswith('.xml'):
            xml_path = os.path.join(xml_folder2, xml_file)

            image_height, image_width = 200, 200

            # Convert annotation
            yolo_labels = convert_annotation(xml_path, image_width, image_height)

            # Write to YOLO format file
            yolo_file_path = os.path.join(output_folder2, xml_file.replace('.xml', '.txt'))
            with open(yolo_file_path, 'w') as yolo_file:
                yolo_file.write('\n'.join(yolo_labels))
                
                

if __name__ == '__main__':
    main()
