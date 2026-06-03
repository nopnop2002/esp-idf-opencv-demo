# People detection example for IDF OpenCV component
The example use opencv library to detect people image on camera image.   
This project will use [this](https://github.com/espressif/esp-opencv-component/tree/master) component.   
I based it on [this](https://github.com/espressif/esp-opencv-component/tree/master/examples/people_detection).   

<img width="520" height="817" alt="Image" src="https://github.com/user-attachments/assets/85c8303e-fc28-4b80-9d97-9e1b42b3d86b" />

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
cd esp-idf-opencv-demo/people_detection
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
This project uses the Enter key on the keyboard as a trigger.   
When the Enter key is pressed, the camera will capture an image.   
Press the Enter key at this point.   
<img width="659" height="486" alt="Image" src="https://github.com/user-attachments/assets/7046cfc0-d57f-43d7-9f7c-92246421b2c7" />

OpenCV then searches for people image within the image.   
This search takes a considerable amount of time.   
<img width="659" height="486" alt="Image" src="https://github.com/user-attachments/assets/7dfba246-fe0f-4354-ac63-a1a0a35c1754" />

# View picture using Built-in WEB Server   
ESP32 works as a web server.   
You can view the pictures taken using the built-in WEB server.   
Enter the ESP32's IP address and port number in the address bar of your browser.   
You can connect using mDNS hostname instead of IP address.   
<img width="520" height="817" alt="Image" src="https://github.com/user-attachments/assets/85c8303e-fc28-4b80-9d97-9e1b42b3d86b" />
<img width="520" height="817" alt="Image" src="https://github.com/user-attachments/assets/750eca03-46fd-4672-bb37-08296ef2d1b1" />
