# esp-idf-opencv-demo
OpenCV example for ESP-IDF.   
OpenCV library usable with ESP-IDF is available [here](https://github.com/espressif/esp-opencv-component).   
You can use it as a component in your project.   
I used OpenCV versions 4.10.0~3.   

## text_area_detection
The example use opencv library to detect text areas on camera image.   
<img width="510" height="819" alt="Image" src="https://github.com/user-attachments/assets/45a9c22a-add1-4388-aa19-0d5c1caa86de" />

## line_detection
The example use opencv library to detect line on camera image.   
<img width="506" height="821" alt="Image" src="https://github.com/user-attachments/assets/de8da199-256a-40ac-818d-c6890028c7f7" />

## corner_detection
The example use opencv library to detect corner on camera image.   
<img width="506" height="821" alt="Image" src="https://github.com/user-attachments/assets/864fe6b0-ad33-4413-a2d6-d03402abb529" />

## shape_detection
The example use opencv library to detect shapes on camera image.   
<img width="510" height="819" alt="Image" src="https://github.com/user-attachments/assets/083d6609-41db-4978-90cb-4b67e3be7a31" />

## people_detection
The example use opencv library to detect people image on camera image.   
<img width="520" height="817" alt="Image" src="https://github.com/user-attachments/assets/85c8303e-fc28-4b80-9d97-9e1b42b3d86b" />

## insert_text
The example use opencv library to insert text on camera image.   
<img width="566" height="828" alt="Image" src="https://github.com/user-attachments/assets/ebf46889-ce9b-4010-a6a8-f024204b2c79" />

## overlap_image
The example use opencv library to overlap one image onto a camera image.   
<img width="503" height="819" alt="Image" src="https://github.com/user-attachments/assets/97133a3c-ffc2-451f-ad81-13cd954eec42" />


# Limitations
The following limitations exist for OpenCV in ESP-IDF:
- dram0_0_seg overflow   
	Some functions, such as `cv::getRotationMatrix2D` will cause a dram0_0_seg overflow.   
	As a result, the project cannot be built.

- Insufficient SRAM Capacity   
	Some functions will cause an SRAM overflow.   
	This causes a panic when the project is run.
