# esp-idf-opencv-demo
OpenCV example for ESP-IDF.   
The OpenCV library available for use with ESP-IDF can be obtained [here](https://github.com/espressif/esp-opencv-component).   
You can use it as a component in your project.   

# Hardware requirements
ESP32 development board with OV2640 camera.   
If you use other camera, edit sdkconfig.default.   
From the left:   
- Aithinker ESP32-CAM   
- Freenove ESP32-WROVER CAM   
- UICPAL ESPS3 CAM   
- Freenove ESP32S3-WROVER CAM (Clone)   

![es32-camera](https://github.com/nopnop2002/esp-idf-websocket-camera/assets/6020549/38dbef9a-ed85-4df2-8d22-499b2b497278)

# Software requirements
ESP-IDF V5.5 or higher.   

___Note for ESP-IDF V6___   
Since ESP-IDF v6.0, the default LibC used in builds has switched to the PicolibC implementation.   
You can build using ESP-IDF V6 by following the steps below.   
```
$ cd corner_detection
$ idf.py set-target {esp32/esp32s3}

-- Configuring incomplete, errors occurred!
HINT: newlib component has been renamed to esp_libc. Any `REQUIRES newlib` can simply be deleted as esp_libc is REQUIRED by default.
cmake failed with exit code 1, output of the command is in the /home/nop/rtos/esp-idf-opencv-demo/build/log/idf_py_stderr_output_3125 and /home/nop/rtos/esp-idf-opencv-demo/build/log/idf_py_stdout_output_3125

$ vi managed_components/espressif__opencv/CMakeLists.txt

#set(deps esp_rom vfs lwip pthread newlib)
set(deps esp_rom vfs lwip pthread)

$ idf.py menuconfig
```

# Installation
Please refer to the README in the project folder.

# text_area_detection
The example use opencv library to detect text areas on camera image.   
<img width="510" height="819" alt="Image" src="https://github.com/user-attachments/assets/45a9c22a-add1-4388-aa19-0d5c1caa86de" />

# line_detection
The example use opencv library to detect line on camera image.   
<img width="506" height="821" alt="Image" src="https://github.com/user-attachments/assets/de8da199-256a-40ac-818d-c6890028c7f7" />

# corner_detection
The example use opencv library to detect corner on camera image.   
<img width="506" height="821" alt="Image" src="https://github.com/user-attachments/assets/864fe6b0-ad33-4413-a2d6-d03402abb529" />

# shape_detection
The example use opencv library to detect shapes on camera image.   
<img width="510" height="819" alt="Image" src="https://github.com/user-attachments/assets/083d6609-41db-4978-90cb-4b67e3be7a31" />

# people_detection
The example use opencv library to detect people image on camera image.   
<img width="520" height="817" alt="Image" src="https://github.com/user-attachments/assets/85c8303e-fc28-4b80-9d97-9e1b42b3d86b" />

# filtering_function
The OpenCV library has numerous filtering functions.   
This project will showcase them.   
<img width="506" height="814" alt="Image" src="https://github.com/user-attachments/assets/3b0009e7-5ed4-423d-924f-4baa7b02d6f1" />

# insert_text
The example use opencv library to insert text on camera image.   
<img width="566" height="828" alt="Image" src="https://github.com/user-attachments/assets/ebf46889-ce9b-4010-a6a8-f024204b2c79" />

# crop_image
The example use opencv library to crop an image from a camera image.   
<img width="507" height="818" alt="Image" src="https://github.com/user-attachments/assets/f50e3725-7171-45fe-9fc1-20fefdbf4824" />

# swap_image
The example use opencv library to swap images on a camera image.   
<img width="507" height="818" alt="Image" src="https://github.com/user-attachments/assets/fe3eba0e-5bd6-4f21-85fe-108761991ac2" />

# overlap_image
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
