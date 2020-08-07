/********************************************************
 * This demo is only supported after RUI firmware version 3.0.0.13.X on RAK811
 * Master Board Uart Receive buffer size at least 128 bytes. 
 ********************************************************/

#include "RAK811.h"
#include "SoftwareSerial.h"
#define WORK_MODE LoRaP2P   //  LoRaWAN or LoRaP2P

#define TXpin 11   // Set the virtual serial port pins
#define RXpin 10
#define DebugSerial Serial
SoftwareSerial ATSerial(RXpin,TXpin);    // Declare a virtual serial port
char buffer[]= "72616B776972656C657373";

RAK811 RAKLoRa(ATSerial,DebugSerial);


void setup() {
  DebugSerial.begin(115200);
  while(DebugSerial.available())
  {
    DebugSerial.read(); 
  }
  
  ATSerial.begin(9600); //set ATSerial baudrate:This baud rate has to be consistent with  the baud rate of the WisNode device.
  while(ATSerial.available())
  {
    ATSerial.read(); 
  }

  if(!RAKLoRa.rk_setWorkingMode(WORK_MODE))  //set WisNode work_mode to LoRaP2P.
  {
    DebugSerial.println(F("set work_mode failed, please reset module."));
    while(1);
  }
  
  RAKLoRa.rk_getVersion();  //get RAK811 firmware version
  DebugSerial.println(RAKLoRa.rk_recvData());  //print version number

  DebugSerial.println(F("Start init LoRaP2P parameters..."));  
  
  if (!RAKLoRa.rk_initP2P("869525000",12,0,1,8,20))  //init LoRaP2P
  {
    DebugSerial.println(F("Init error,please reset module.")); 
    while(1);
  }else DebugSerial.println(F("Init OK"));
}

void loop() {
  DebugSerial.println(F("Start send data..."));
  if (RAKLoRa.rk_sendP2PData(buffer))
  {   
      String ret = RAKLoRa.rk_recvP2PData();
      if(ret != NULL)
      {     
        DebugSerial.println(ret);
      }
  }
  delay(10000);  //delay 10s
}
