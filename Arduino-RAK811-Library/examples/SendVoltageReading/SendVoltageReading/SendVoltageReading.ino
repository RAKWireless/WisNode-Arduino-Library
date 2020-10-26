/********************************************************
 * This demo is only supported after RUI firmware version 3.0.0.13.X on RAK811
 * Master Board Uart Receive buffer size at least 128 bytes. 
 ********************************************************/

#include "RAK811.h"
#include "SoftwareSerial.h"
#define WORK_MODE LoRaWAN   //  LoRaWAN or LoRaP2P
#define JOIN_MODE OTAA    //  OTAA or ABP
#if JOIN_MODE == OTAA
String DevEui = "8680000000000001";
String AppEui = "70B3D57ED00285A7";
String AppKey = "DDDFB1023885FBFF74D3A55202EDF2B1";
#else JOIN_MODE == ABP
String NwkSKey = "69AF20AEA26C01B243945A28C9172B42";
String AppSKey = "841986913ACD00BBC2BE2479D70F3228";
String DevAddr = "260125D7";
#endif
#define TXpin 11   // Set the virtual serial port pins
#define RXpin 10
#define DebugSerial Serial
SoftwareSerial ATSerial(RXpin,TXpin);    // Declare a virtual serial port
char buffer[60] = {0};
char res[15];
char hi[3] = {'h','i', '\0'};

bool InitLoRaWAN(void);
RAK811 RAKLoRa(ATSerial,DebugSerial); 

void setup() {
  DebugSerial.begin(115200);
  while(DebugSerial.available())
  {
    DebugSerial.read(); 
  }

  while(!DebugSerial) delay(10);
  Serial.println("RAK811: Join Network OTAA Example");
  
  ATSerial.begin(115200); //set ATSerial baudrate:This baud rate has to be consistent with  the baud rate of the WisNode device.
  while(ATSerial.available())
  {
    ATSerial.read(); 
  }

  RAKLoRa.rk_setWorkingMode(0);
  //if(!RAKLoRa.rk_setWorkingMode(0))  //set WisNode work_mode to LoRaWAN.
  //{
  //  DebugSerial.println(F("set work_mode failed, please reset module."));
  //  while(1);
  //}
  
  RAKLoRa.rk_getVersion();  //get RAK811 firmware version
  DebugSerial.println(RAKLoRa.rk_recvData());  //print version number

  DebugSerial.println(F("Start init RAK811 parameters..."));
 
  if (!InitLoRaWAN())  //init LoRaWAN
  {
    DebugSerial.println(F("Init error,please reset module.")); 
    while(1);
  }

  DebugSerial.println(F("Start to join LoRaWAN..."));
  while(!RAKLoRa.rk_joinLoRaNetwork(60))  //Joining LoRaNetwork timeout 60s
  {
    DebugSerial.println();
    DebugSerial.println(F("Rejoin again after 5s..."));
    delay(5000);
  }
  DebugSerial.println(F("Join LoRaWAN success"));

  if(!RAKLoRa.rk_isConfirm(0))  //set LoRa data send package type:0->unconfirm, 1->confirm
  {
    DebugSerial.println(F("LoRa data send package set error,please reset module.")); 
    while(1);    
  }
}

bool InitLoRaWAN(void)
{
  if(RAKLoRa.rk_setJoinMode(JOIN_MODE))  //set join_mode:OTAA
  {
    if(RAKLoRa.rk_setRegion(1))  //set region AU915
    {
      if (RAKLoRa.rk_initOTAA(DevEui, AppEui, AppKey))
      {
        DebugSerial.println(F("RAK811 init OK!"));  
        return true;    
      }
    }
  }
  return false;
}

void loop() {
  // ATENTION: You will receive the voltage as int and need to divide by 100 to get the actual value
  int voltage = (int)((analogRead(A1)+250) * (3.3/1023.0) * 100);
  itoa(voltage, vol, 10);
  sprintf(buffer, "%s:%s", hi, vol);

  DebugSerial.print("Resistance: "); DebugSerial.println(voltage/100.);
  
  DebugSerial.println(F("Start send data..."));
  if (RAKLoRa.rk_sendDataASCII(1, buffer, strlen(buffer)))
  {    
    for (unsigned long start = millis(); millis() - start < 60000L;)
    {
      String ret = RAKLoRa.rk_recvData();
      if(ret != "")
      { 
        DebugSerial.println(ret);
      }
    }
  }
}
