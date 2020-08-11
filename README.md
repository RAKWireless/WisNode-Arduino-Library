# WisNode-LoRa-Arduino Library User Manual

## Introduction

The core of WisNode is the RAK811 LoRa module, and its interface is a universal Arduino interface, which means you can easily connect to any open source hardware with such an interface. As long as the user writes the control code on the Arduino IDE, the communication between LoRaWAN and LoRa P2P can be quickly realized.

To develop Arduino program, you can refer to the API manual:[WisNode_Arduino_Library API Manual V1.1.pdf](https://github.com/RAKWireless/WisNode-Arduino-Library/blob/master/Documents%20and%20tools/WisNode_Arduino_Library%20API%20Manual%20V1.1.pdf)

## Setup

### Arduino 

1. Install Arduino IDE
   First, you need to install the Arduino IDE. From the link below, select the version that suits your platform.
   [https://www.arduino.cc/en/main/software](https://www.arduino.cc/en/main/software) 

2. Install the Arduino library

   WisNode is controlled by AT commands. RAKWireless provides an encapsulation library for Arduino developers. AT commands are encapsulated as APIs, which are convenient for developers to call.

   a. Download the `Arduino-RAK811-Library folder` in the repository.
   
   The current warehouse supports version RAK811 WisNode V1.3.
   
   If you are using RAK811 WisNode in V1.1 version, please download RAK811 WisNode through this link:
   
      https://github.com/RAKWireless/WisNode-Arduino-Library/tree/master/Documents%20and%20tools/WisNodeV1.1%20Arduino%20Library
   
      If you are using RAK811 WisNode in V1.2 version, please download RAK811 WisNode through this link:
   
      https://github.com/RAKWireless/WisNode-Arduino-Library/tree/master/Documents%20and%20tools/WisNodeV1.2%20Arduino%20Library
   
    b. Copy to the Arduino library directory, usually it's in `C:\Users\<UserName>\Documents\Arduino\libraries` for Windows, and in `~/Documents/Arduino/libraries/` for Linux and Mac OS X.
   
      <div align=center> <img src="https://github.com/RAKWireless/WisNode-Arduino-Library/blob/master/Documents%20and%20tools/image/Library_directory.png" /> </div>
   
    c. Reopen the Arduino IDE, you can see `Arduino-RAK811-Library` in the menu, indicating that the library is successfully installed.
   
    <div align=center> <img src="https://github.com/RAKWireless/WisNode-Arduino-Library/blob/master/Documents%20and%20tools/image/Library_in_the_menu.png" /> </div>
   
     d. Before compiling the routine, you need to modify the serial port configuration of the development board. Open the Arduino IDE installation directory, find the `<Arduino_installation_directory>\hardware\arduino\avr\cores\arduino\HardwareSerial.h` file, and modify the size of the serial port receiving and sending buffers.
   
      <div align=center> <img src="https://github.com/RAKWireless/WisNode-Arduino-Library/blob/master/Documents%20and%20tools/image/Serial_port_configuration_modification.png" /> </div>
### RAK811

1. Please install the “STM32CubeProgrammer” tool on your Windows PC. You can download it from here:

   https://www.st.com/content/st_com/en/products/development-tools/software-development-tools/stm32-software-development-tools/stm32-programmers/stm32cubeprog.html#overview

   or from RAK website:

   https://downloads.rakwireless.com/en/LoRa/RAK811/Tools/SetupSTM32CubeProgrammer-2.1.0.rar 

2. Please download the WisNode-Arduino version of the firmware from here：

   https://github.com/RAKWireless/WisNode-Arduino-Library/tree/master/Documents%20and%20tools/WisNode-Arduino%20version-Firmware

   ps：The firmware version has H and L distinction, which corresponds to different frequencies available.

   WisNode-Arduino version-H：EU868、IN865、US915、AU915、KR920、AS923

   WisNode-Arduino version-L：EU433、CN470

3. jump the “BOOT” pin and “3V3” pin for boot mode as following:

   <div align=center> <img src="https://github.com/RAKWireless/WisNode-Arduino-Library/blob/master/Documents%20and%20tools/image/jump_boot.png" /> </div>

4. Connect RAK811 WisNode with your PC’s USB interface, Open the “STM32CubeProgrammer” tool, and select UART type, then configure the Port, Baudrate, and Pairty .Then press “Connect” button at the top right corner.

   <div align=center> <img src="https://github.com/RAKWireless/WisNode-Arduino-Library/blob/master/Documents%20and%20tools/image/connect_STM32cubeprogrammer.png" /> </div>

5. Press “Open file” and selectthe WisNode-Arduino version hex file in the pop-up window,And press the “Download” button to start the burning process:

   <div align=center> <img src="https://github.com/RAKWireless/WisNode-Arduino-Library/blob/master/Documents%20and%20tools/image/burn_firmware.png" /> </div>

6. Burn successfully, restore to the initial state.

   

## Example

### JoinNetworkOTAA

RAKWireless provides LoRaWAN communication routines for WisNode, including OTAA and ABP modes. Open the OTAA example, you need to get the value of app_eui, dev_eui, app_key from your LoRa Server. And set these values to the corresponding DevEui, AppEui, AppKey variables.

Compile the code, download it to the target board, and see the log from the Arduino serial monitor.

<div align=center> <img src="https://github.com/RAKWireless/WisNode-Arduino-Library/blob/master/Documents%20and%20tools/image/Download_Firmware.png" /> </div>

### Hardware Configuration

After completing the above steps, connect RAK WisNode to Arduino Uno, and connect RAK WisNode to the jumper as shown in the figure below.

<div align=center> <img src="https://github.com/RAKWireless/WisNode-Arduino-Library/blob/master/Documents%20and%20tools/image/Arduino_mode_v1.3.png" /> </div>

<div align=center> <img src="https://github.com/RAKWireless/WisNode-Arduino-Library/blob/master/Documents%20and%20tools/image/merge_connect.png" /> </div>

ps:Since there are two other previous versions of the RAK Wisnode hardware,to accommodate all users, here are the other two versions of the connection and the success stories.



If your Version of RAK WisNode is V1.1, see how to connect in this link:

https://github.com/RAKWireless/WisNode-Arduino-Library/blob/master/Documents%20and%20tools/WisNodeV1.1%20Arduino%20Library/RAK811%20WisNode%20V1.1%20Hardware.md

If your Version of RAK WisNode is V1.2, see how to connect in this link:

https://github.com/RAKWireless/WisNode-Arduino-Library/blob/master/Documents%20and%20tools/WisNodeV1.2%20Arduino%20Library/RAK811%20WisNode%20V1.2%20Hardware.md



### Test LoRa Node with LoRaWAN

Server and serial console log:

<div align=center> <img src="https://github.com/RAKWireless/WisNode-Arduino-Library/blob/master/Documents%20and%20tools/image/LoRaWAN_log.png" /> </div>

If the log does not display properly, it can be resolved by restarting the board.

## Contact us

If you have any questions, welcome to the forum.

[forum.rakwireless.com](https://forum.rakwireless.com) 

