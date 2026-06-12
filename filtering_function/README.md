# Filtering function example for IDF OpenCV component
The OpenCV library has numerous filtering functions.   
We can use these to process images.   
This project will showcase them.   
This project will use [this](https://github.com/espressif/esp-opencv-component/tree/master) component.   

<img width="506" height="814" alt="Image" src="https://github.com/user-attachments/assets/3b0009e7-5ed4-423d-924f-4baa7b02d6f1" />

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
ESP-IDF V5.5.   
___It cannot be built with ESP-IDF V6.0.___   

# Installation
For AiThinker ESP32-CAM, you need to use a USB-TTL converter and connect GPIO0 to GND.   

|ESP-32|USB-TTL|
|:-:|:-:|
|U0TXD|RXD|
|U0RXD|TXD|
|GPIO0|GND|
|5V|5V|
|GND|GND|


```
git clone https://github.com/nopnop2002/esp-idf-opencv-demo
cd esp-idf-opencv-demo/filtering_function
idf.py set-target {esp32/esp32s3}
idf.py menuconfig
idf.py flash monitor
```

# Start firmware
For AiThinker ESP32-CAM, Change GPIO0 to open and press the RESET button.

# Configuration
Set the following items using menuconfig.   
<img width="659" height="486" alt="Image" src="https://github.com/user-attachments/assets/5b81adcb-e390-4663-8cda-b04b877262a3" />
<img width="659" height="486" alt="Image" src="https://github.com/user-attachments/assets/c53c0730-258c-4fd4-963e-f5bcf9600174" />

## Wifi Setting
Set the information of your access point.   
<img width="659" height="486" alt="Image" src="https://github.com/user-attachments/assets/2cfe16bc-119e-4922-9123-51cab3e57ef8" />

## Select Board
<img width="659" height="486" alt="Image" src="https://github.com/user-attachments/assets/b2bee3ce-1028-49fd-b7c9-16a8fb8b5118" />

## Flash Light   
ESP32-CAM by AI-Thinker has a flash light on GPIO4.   
<img width="659" height="486" alt="Image" src="https://github.com/user-attachments/assets/82de0202-c769-413f-9cd9-e1993c61ca96" />

## PSRAM
When using ESP32S3, you need to set the PSRAM type according to the hardware.   
ESP32S3-WROVER CAM has Octal Mode PSRAM.   
UICPAL ESPS3 CAM has Quad Mode PSRAM.   
<img width="659" height="486" alt="Image" src="https://github.com/user-attachments/assets/6b2e84ac-6617-4c91-9f1a-3f59112ac94a" />

# How to use   
This project uses the key on the keyboard as a trigger.   
Pressing the key will cause the camera to take a picture.   
Press one of the keys at this point.   
<img width="659" height="486" alt="Image" src="https://github.com/user-attachments/assets/328c4f32-ad4b-4ff7-8e81-970dff90f05b" />

# View picture using Built-in WEB Server   
ESP32 works as a web server.   
You can view the pictures taken using the built-in WEB server.   
Enter the ESP32's IP address and port number in the address bar of your browser.   
You can connect using mDNS hostname instead of IP address.   
<img width="526" height="814" alt="Image" src="https://github.com/user-attachments/assets/bbcfa9cc-8159-4bd5-8f31-39c0bbf109d6" />
<img width="526" height="814" alt="Image" src="https://github.com/user-attachments/assets/6e5be78b-5d0d-4b55-9a47-69844b48b6bd" />
<img width="526" height="814" alt="Image" src="https://github.com/user-attachments/assets/5969f060-adff-45d0-a618-3c064cda08e5" />
<img width="506" height="814" alt="Image" src="https://github.com/user-attachments/assets/e7fbfa55-073f-48f2-a7ed-8c241af05125" />
<img width="526" height="814" alt="Image" src="https://github.com/user-attachments/assets/bb4db182-fa7e-4f80-a9b9-d4db7845905c" />
<img width="526" height="814" alt="Image" src="https://github.com/user-attachments/assets/12df24b3-3a85-4232-9c4d-9f8bfd0309f1" />
<img width="526" height="814" alt="Image" src="https://github.com/user-attachments/assets/c016663a-db86-4a1c-a83a-53eea56ae506" />
<img width="526" height="814" alt="Image" src="https://github.com/user-attachments/assets/88bb75ac-1f60-4bb5-8d66-9b94733d0ddd" />
<img width="526" height="814" alt="Image" src="https://github.com/user-attachments/assets/f24b3961-ac94-48b1-9fca-d42e1ae666ea" />
<img width="526" height="814" alt="Image" src="https://github.com/user-attachments/assets/64d5694a-bb10-417f-a82a-4cb62cb3ac4c" />
<img width="506" height="814" alt="Image" src="https://github.com/user-attachments/assets/3b0009e7-5ed4-423d-924f-4baa7b02d6f1" />
<img width="506" height="814" alt="Image" src="https://github.com/user-attachments/assets/3153d00b-3940-4f39-a71b-faac673f3e8a" />

