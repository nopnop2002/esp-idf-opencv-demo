# Crop image example for IDF OpenCV component
The example use opencv library to swap images on a camera image.   
This project will use [this](https://github.com/espressif/esp-opencv-component/tree/master) component.   
I based it on [this](https://github.com/espressif/esp-opencv-component/tree/master/examples/text_area_detection).   

<img width="507" height="818" alt="Image" src="https://github.com/user-attachments/assets/fe3eba0e-5bd6-4f21-85fe-108761991ac2" />

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
cd esp-idf-opencv-demo/swap_image
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
<img width="659" height="486" alt="Image" src="https://github.com/user-attachments/assets/b353ef84-7819-4d2b-a8d6-86f91bf999f6" />

# View picture using Built-in WEB Server   
ESP32 works as a web server.   
You can view the pictures taken using the built-in WEB server.   
Enter the ESP32's IP address and port number in the address bar of your browser.   
You can connect using mDNS hostname instead of IP address.   
<img width="507" height="818" alt="Image" src="https://github.com/user-attachments/assets/ec0f0884-eb31-4e6e-b9b6-1793d41b3ecf" />
<img width="507" height="818" alt="Image" src="https://github.com/user-attachments/assets/15ff5d1b-08e2-4dfe-bfc4-07bdfd0311f9" />
<img width="507" height="818" alt="Image" src="https://github.com/user-attachments/assets/ade0244b-11c2-4037-aa0d-e433ee53e3e0" />
<img width="507" height="818" alt="Image" src="https://github.com/user-attachments/assets/da239e5b-cfd7-45ff-ac9b-b405fc1307be" />
