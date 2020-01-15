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
#define ATSerial Serial
SoftwareSerial DebugSerial(RXpin,TXpin);    // Declare a virtual serial port
char buffer[]= "72616B776972656C657373";

bool InitLoRaWAN(void);
RAK811 RAKLoRa(ATSerial,DebugSerial);


void setup() {
  DebugSerial.begin(115200);
  while(DebugSerial.read()>= 0) {}
  while(!DebugSerial);
  DebugSerial.println("StartUP");
  
  ATSerial.begin(115200); // Note: Please manually set the baud rate of the WisNode device to 115200.
  while(ATSerial.available())
  {
    ATSerial.read(); 
  }
  
  DebugSerial.println(RAKLoRa.rk_getVersion());  //get RAK811 firmware version
  delay(200);

  DebugSerial.println("Start init RAK811 parameters...");
  if(!RAKLoRa.rk_setSendinterval(0,0))  //close auto join and send to LoRaWAN
  {
    DebugSerial.println("Close auto join and send to LoRaWAN failed, please reset module.");
    while(1);
  }
  
  if (!InitLoRaWAN())  //init LoRaWAN
  {
    DebugSerial.println("Init error,please reset module."); 
    while(1);
  }

  DebugSerial.println("Start joining LoRaWAN...");
  while(!RAKLoRa.rk_joinLoRaNetwork(60))  //Joining LoRaNetwork timeout 60s
  {
    DebugSerial.println();
    DebugSerial.println("Rejoin again after 5s...");
    delay(5000);
  }

  if(!RAKLoRa.rk_isConfirm(0))  //set LoRa data send package type:0->unconfirm, 1->confirm
  {
    DebugSerial.println("LoRa data send package set error,please reset module."); 
    while(1);    
  }
}

bool InitLoRaWAN(void)
{
  if(RAKLoRa.rk_setJoinMode(JOIN_MODE))
  {
    if(RAKLoRa.rk_setRegion(5))  //set region EU868
    {
      if (RAKLoRa.rk_initOTAA(DevEui, AppEui, AppKey))
      {
        DebugSerial.println("RAK811 init OK!");  
        return true;    
      }
    }
  }
  return false;
}

void loop() {
  DebugSerial.println("Start send data...");
  if (RAKLoRa.rk_sendData(1, buffer))
  {    
    for (unsigned long start = millis(); millis() - start < 90000L;)
    {
      String ret = RAKLoRa.rk_recvData();
      if(ret != NULL)
      {
        ret.trim();
        DebugSerial.println(ret);
      }
      if((ret.indexOf("OK")>0)||(ret.indexOf("ERROR")>0))
      {
        RAKLoRa.rk_sleep(1);  //Set RAK811 enter sleep mode
        delay(10000);  //delay 10s
        RAKLoRa.rk_sleep(0);  //Wakeup RAK811 from sleep mode
        break;
      }
    }
  }
}
