/*
 * A library for controlling RAK811 LoRa radio.
 *
 * @Author Leopold.wang
 * @Date 14/01/2020
 *
 */

#include "Arduino.h"
#include "RAK811.h"

extern "C"
{
#include "string.h"
#include "stdlib.h"
}
String ret = "";
String command = "";
/*
  @param serial Needs to be an already opened Stream ({Software/Hardware}Serial) to write to and read from.
*/
RAK811::RAK811(Stream &serial, Stream &serial1) : _serial(serial), _serial1(serial1)
{
  _serial.setTimeout(2000);
  _serial1.setTimeout(1000);
}

bool RAK811::rk_getVersion()
{
  return sendRawCommand(F("at+version"));
}

String RAK811::rk_getLoRaStatus()
{
  sendRawCommand(F("at+get_config=lora:status"));
  ret = rk_recvData();
  ret.trim();
  return ret;
}

String RAK811::rk_getChannelList()
{
  sendRawCommand(F("at+get_config=lora:channel"));
  ret = rk_recvData();
  ret.trim();
  return ret;
}

bool RAK811::rk_setRate(int rate)
{
  sendRawCommand("at+set_config=lora:dr:" + (String)rate);
  ret = rk_recvData();
  if (ret.indexOf("OK") >= 0)
  {
    return true;
  }
  else
  {
    return false;
  }
}

bool RAK811::rk_setClass(int classMode)
{
  if (classMode > 2)
  {
    _serial1.println(F("Parameter error"));
    return false;
  }
  command = "at+set_config=lora:class:" + (String)classMode;
  sendRawCommand(command);
  ret = rk_recvData();
#if defined DEBUG_MODE
  _serial1.println(ret);
#endif
  if (ret.indexOf("OK") >= 0)
  {
    return true;
  }
  else
  {
    return false;
  }
}

bool RAK811::rk_setRegion(int region)
{
  if (region > 9)
  {
    _serial1.println(F("Parameter error"));
    return false;
  }
  String REGION;
  switch (region)
  {
    case 0:REGION="AS923";
    
      break;
    case 1:REGION="AU915";
      break;
    case 2:REGION="CN470";
      break;
    case 3:REGION="CN779";
      break;
    case 4:REGION="EU433";
      break;
    case 5:REGION="EU868";
      break;
    case 6:REGION="KR920";
      break;
    case 7:REGION="IN865";
      break;
    case 8:REGION="US915";
      break;
    case 9:REGION="US915_Hybrid";
      break;
  }
  _serial1.println("Current work region: "+REGION);
  sendRawCommand("at+set_config=lora:region:" + REGION);
  ret = rk_recvData();
#if defined DEBUG_MODE
  _serial1.println(ret);
#endif
  if (ret.indexOf("OK") >= 0)
  {
    return true;
  }
  else
  {
    return false;
  }
}

void RAK811::rk_sleep(int mode)
{
  switch (mode)
  {
  case 1:
    sendRawCommand(F("at+set_config=device:sleep:1"));
    break;
  case 0:
    sendRawCommand(F("at+set_config=device:sleep:0"));
    break;
  default:
    _serial1.println(F("Parameter error"));
    break;
  }
}

bool RAK811::rk_setSendinterval(int mode, int value)
{
  if (mode > 2)
  {
    _serial1.println("The mode set Error,the mode is '0','1' or '2'.");
    return false;
  }

  sendRawCommand("at+set_config=lora:send_interval:" + (String)mode + ":" + (String)value);
  ret = rk_recvData();
  if (ret.indexOf("OK") >= 0)
  {
    return true;
  }
  else
  {
    return false;
  }
}

void RAK811::rk_reset(void)
{
  sendRawCommand(F("at+set_config=device:restart"));
}

bool RAK811::rk_setWorkingMode(int mode)
{
  switch (mode)
  {
    case 0:
      sendRawCommand(F("at+set_config=lora:work_mode:0")); //Set LoRaWAN Mode.
      break;
    case 1:
      sendRawCommand(F("at+set_config=lora:work_mode:1")); //Set LoRaP2P Mode.
      break;
    default:
      return false;
  }
  
  for(int i=0; i<10; i++)
  {
    ret = rk_recvData();
    if ((ret.indexOf("Initialization OK") >= 0) || (ret.indexOf("No switch work_mode") >= 0))
    {
      return true;
    }
  }

  return false;

}

bool RAK811::rk_setJoinMode(int mode)
{
  switch (mode)
  {
    case 0:
      sendRawCommand(F("at+set_config=lora:join_mode:0")); //join Network through OTAA mode.
      break;
    case 1:
      sendRawCommand(F("at+set_config=lora:join_mode:1")); //join Network through ABP mode.
      break;
    default:
      return false;
  }
  ret = rk_recvData();
  if (ret.indexOf("OK") >= 0)
  {
    return true;
  }
  else
  {
    return false;
  }
}

bool RAK811::rk_joinLoRaNetwork(int timeout)
{
  String command = "at+join";  
  //  _serial1.println(command);
  sendRawCommand(command);
  ret = rk_recvData();
  if (ret != NULL)
  {
    if (ret.indexOf("OK") >= 0)
      return true;
    else if(ret.indexOf("ERROR") >= 0)
      return false;
  }
  for (int i = 0; i <= timeout/2; i++)
  {
    ret = rk_recvData();
    if (ret != NULL)
    {
      _serial1.print("<- " + ret);
      if (ret.indexOf("OK") >= 0)
        return true;
      // else if(ret.indexOf("ERROR") >= 0)
      //   return false;
    }
  }

  return false;
}

bool RAK811::rk_initOTAA(String devEUI, String appEUI, String appKEY)
{
  // String command = "";
  if (devEUI.length() == 16)
  {
    _devEUI = devEUI;
  }
  else
  {
    _serial1.println("The parameter devEUI is set incorrectly!");
  }
  if (appEUI.length() == 16)
  {
    _appEUI = appEUI;
  }
  else
  {
    _serial1.println("The parameter appEUI is set incorrectly!");
  }
  if (appKEY.length() == 32)
  {
    _appKEY = appKEY;
  }
  else
  {
    _serial1.println("The parameter appKEY is set incorrectly!");
  }

  command = "at+set_config=lora:dev_eui:" + _devEUI;
  //  _serial1.println(command);
  sendRawCommand(command);
  ret = rk_recvData();
  if (ret.indexOf("OK") >= 0)
  {
    command = "at+set_config=lora:app_eui:" + _appEUI;
    //  _serial1.println(command);
    sendRawCommand(command);

    ret = rk_recvData();
    if (ret.indexOf("OK") >= 0)
    {
      command = "at+set_config=lora:app_key:" + _appKEY;
      //  _serial1.println(command);
      sendRawCommand(command);

      ret = rk_recvData();
      if (ret.indexOf("OK") >= 0)
      {
        return true;
      }
    }
  }

  return false;
}

bool RAK811::rk_initABP(String devADDR, String nwksKEY, String appsKEY)
{
  // String command = "";
  if (devADDR.length() == 8)
  {
    _devADDR = devADDR;
  }
  else
  {
    _serial1.println(F("The parameter devADDR is set incorrectly!"));
  }
  if (nwksKEY.length() == 32)
  {
    _nwksKEY = nwksKEY;
  }
  else
  {
    _serial1.println(F("The parameter nwksKEY is set incorrectly!"));
  }
  if (appsKEY.length() == 32)
  {
    _appsKEY = appsKEY;
  }
  else
  {
    _serial1.println(F("The parameter appsKEY is set incorrectly!"));
  }
  command = "at+set_config=lora:dev_addr:" + _devADDR;
  sendRawCommand(command);  
  ret = rk_recvData();
  // _serial1.println(ret);
  if (ret.indexOf("OK") >= 0)
  {
    command = "at+set_config=lora:nwks_key:" + _nwksKEY;
    sendRawCommand(command);
    ret = rk_recvData();
    if (ret.indexOf("OK") >= 0)
    {
      command = "at+set_config=lora:apps_key:" + _appsKEY;
      sendRawCommand(command);
      ret = rk_recvData();
      if (ret.indexOf("OK") >= 0)
      {
        return true;
      }
    }
  }
  return false;
}

bool RAK811::rk_isConfirm(int type)
{
  switch (type)
  {
    case 0:
      sendRawCommand(F("at+set_config=lora:confirm:0")); //LoRa data send with unconfirm.
      break;
    case 1:
      sendRawCommand(F("at+set_config=lora:confirm:1")); //LoRa data send with confirm.
      break;
    default:
      return false;
  }
  ret = rk_recvData();
  if (ret.indexOf("OK") >= 0)
  {
    return true;
  }
  else
  {
    return false;
  }
}

bool RAK811::rk_sendDataASCII(int port, char *dataASCII, uint8_t PAYLOAD_LENGTH)
{ 
  command = "at+send=lora:" + (String)port + ":";
  sendRawCommand(command, dataASCII, PAYLOAD_LENGTH);

  return true;  
}

bool RAK811::rk_sendData(int port, char *datahex)
{
  // String command = "";
  command = "at+send=lora:" + (String)port + ":" + datahex;
  // _serial1.println(command);
  sendRawCommand(command);

  return true;  
}

String RAK811::rk_recvData(void)
{
  _serial.setTimeout(2000);
  ret = _serial.readStringUntil('\0');
  // ret.trim();
  // _serial1.println(ret);
  return ret;
}

bool RAK811::rk_initP2P(String FREQ, int SF, int BW, int CR, int PRlen, int PWR)
{
  // String command = "";
  command = "at+set_config=lorap2p:" + FREQ + "," + SF + "," + BW + "," + CR + "," + PRlen + "," + PWR;
  //  _serial1.println(command);
  sendRawCommand(command);
  ret = rk_recvP2PData();
  if (ret.indexOf("OK") >= 0)
  {
    return true;
  }
  else
  {
    return false;
  }
}

String RAK811::rk_recvP2PData(void)
{
  ret = _serial.readStringUntil('\0');
  ret.trim();
  // delay(500);
  return ret;
}

bool RAK811::rk_sendP2PData(char *datahex)
{
  String DATAHEX = datahex;
  String command = "at+send=lorap2p:" + DATAHEX;
  //  _serial1.println(command);
  sendRawCommand(command);
  // ret = rk_recvP2PData();
  // if (ret.indexOf("OK") > 0)
  // {
  //   return true;
  // }
  // else
  // {
  //   return false;
  // }
  return true;
}

String RAK811::rk_checkDeviceStatus(void)
{
  sendRawCommand(F("at+get_config=device:status"));
  ret = rk_recvP2PData();
  return ret;
}

bool RAK811::rk_setUARTConfig(int UartPort, int Baud)
{
  // String command = "";
  command = "at+set_config=device:uart:" + (String)UartPort + ":" + (String)Baud;
  //  _serial1.println(command);
  sendRawCommand(command);

  return true;
}

bool RAK811::sendRawCommand(String cmd)
{
  while (_serial.available())
  {
    _serial.read();
  }
  //_serial1.println(cmd);
  _serial.println(cmd);
  delay(200);
  return true;
}

bool RAK811::sendRawCommand(String cmd, char *payloadASCII, uint8_t PAYLOAD_LENGTH)
{
  while (_serial.available())
  {
    _serial.read();
  }

  _serial.print(cmd);
  for(uint8_t i=0; i<PAYLOAD_LENGTH; i++)
  {
    if(payloadASCII[i] == '\0')
    {
      break;
    }
    _serial.print(payloadASCII[i], HEX);
  }
  _serial.println(""); //is it needed?

  delay(200);
  return true;
}

