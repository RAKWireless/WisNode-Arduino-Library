# WisNode-LoRa-Arduino Library User Manual

## Introduction

The core of WisNode is the RAK811 LoRa module, and its interface is a universal Arduino interface, which means you can easily connect to any open source hardware with such an interface. As long as the user writes the control code on the Arduino IDE, the communication between LoRaWAN and LoRa P2P can be quickly realized.



## Setup

### Arduino 

1. Install Arduino IDE
   First, you need to install the Arduino IDE. From the link below, select the version that suits your platform.
   [https://www.arduino.cc/en/main/software](https://www.arduino.cc/en/main/software) 

2. Install the Arduino library

   WisNode is controlled by AT commands. RAKWireless provides an encapsulation library for Arduino developers. AT commands are encapsulated as APIs, which are convenient for developers to call.

   a. Download the `Arduino-RAK811-Library folder` in the repository.
   

   
If you are using RAK811 WisNode in V1.1 version, please download RAK811 WisNode through this link:
   

   
If you are using RAK811 WisNode in V1.2 version, please download RAK811 WisNode through this link:
   

   
   b. Copy to the Arduino library directory, usually it's in `C:\Users\<UserName>\Documents\Arduino\libraries` for Windows, and in `~/Documents/Arduino/libraries/` for Linux and Mac OS X.
   
   ![Library directory](image/Library directory.png)
   
   c. Reopen the Arduino IDE, you can see `Arduino-RAK811-Library` in the menu, indicating that the library is successfully installed.
   
   ![Library in the menu](image/Library in the menu.png)
   
   d. Before compiling the routine, you need to modify the serial port configuration of the development board. Open the Arduino IDE installation directory, find the `<Arduino_installation_directory>\hardware\arduino\avr\cores\arduino\HardwareSerial.h` file, and modify the size of the serial port receiving and sending buffers.
   
   ![Serial port configuration modification](image/Serial port configuration modification.png)

### RAK811

1. Please install the “STM32CubeProgrammer” tool on your Windows PC. You can download it from here:

   https://www.st.com/content/st_com/en/products/development-tools/software-development-tools/stm32-software-development-tools/stm32-programmers/stm32cubeprog.html#overview

   or from RAK website:

   https://downloads.rakwireless.com/en/LoRa/RAK811/Tools/SetupSTM32CubeProgrammer-2.1.0.rar 

2. Please download the WisNode-Arduino version of the firmware from here：

   

3. jump the “BOOT” pin and “3V3” pin for boot mode as following:

   ![微信图片_20200807110601](E:\放文件\RUI\WisNode User Manual\image\微信图片_20200807110601.png)

4. Connect RAK811 WisNode with your PC’s USB interface, Open the “STM32CubeProgrammer” tool, and select UART type, then configure the Port, Baudrate, and Pairty .Then press “Connect” button at the top right corner.

   ![image-20200807102503737](E:\放文件\RUI\WisNode User Manual\image\image-20200807102503737.png)

5. Press “Open file” and selectthe WisNode-Arduino version hex file in the pop-up window,And press the “Download” button to start the burning process:

   ![image-20200807102633251](E:\放文件\RUI\WisNode User Manual\image\image-20200807102633251.png)

6. Burn successfully, restore to the initial state.

   

## Example

### JoinNetworkOTAA

RAKWireless provides LoRaWAN communication routines for WisNode, including OTAA and ABP modes. Open the OTAA example, you need to get the value of app_eui, dev_eui, app_key from your LoRa Server. And set these values to the corresponding DevEui, AppEui, AppKey variables.

Compile the code, download it to the target board, and see the log from the Arduino serial monitor.

![Download Firmware.png](image/Download Firmware.png)

### Hardware Configuration

After completing the above steps, connect RAK WisNode to Arduino Uno, and connect RAK WisNode to the jumper as shown in the figure below.

![Arduino mode v1.3.png](image/Arduino mode v1.3.png)

![3310531c5fcb5be7e64d3b6b619cf60](E:\放文件\RUI\WisNode User Manual\image\3310531c5fcb5be7e64d3b6b619cf60.jpg)

ps:Since there are two other previous versions of the RAK Wisnode hardware,to accommodate all users, here are the other two versions of the connection and the success stories.



If your Version of RAK WisNode is V1.1, see how to connect in this link:



If your Version of RAK WisNode is V1.2, see how to connect in this link:





### Test LoRa Node with LoRaWAN

Server and serial console log:

![LoRaWAN log.png](image/LoRaWAN log.png)

If the log does not display properly, it can be resolved by restarting the board.

## Contact us

If you have any questions, welcome to the forum.

[forum.rakwireless.com](https://forum.rakwireless.com) 

