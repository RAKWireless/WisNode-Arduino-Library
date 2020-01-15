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
/*
  @param serial Needs to be an already opened Stream ({Software/Hardware}Serial) to write to and read from.
*/
RAK811::RAK811(Stream &serial, Stream &serial1) : _serial(serial), _serial1(serial1)
{
  _serial.setTimeout(2000);
  _serial1.setTimeout(1000);
}

String RAK811::rk_getVersion()
{
  ret = sendRawCommand(F("at+version"));
  ret.trim();
  return ret;
}

String RAK811::rk_getLoRaStatus()
{
  ret = sendRawCommand(F("at+get_config=lora:status"));
  ret.trim();
  return ret;
}

String RAK811::rk_getChannelList()
{
  ret = sendRawCommand(F("at+get_config=lora:channel"));
  ret.trim();
  return ret;
}

bool RAK811::rk_setRate(int rate)
{
  ret = sendRawCommand("at+set_config=lora:dr:" + (String)rate);
#if defined DEBUG_MODE
  _serial1.println(ret);
#endif
  if (ret.indexOf("OK") > 0)
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
    _serial1.println("Parameter error");
    return false;
  }
  ret = sendRawCommand("at+set_config=lora:class:" + (String) classMode);
#if defined DEBUG_MODE
  _serial1.println(ret);
#endif
  if (ret.indexOf("OK") > 0)
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
    _serial1.println("Parameter error");
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
  ret = sendRawCommand("at+set_config=lora:region:" + REGION);
#if defined DEBUG_MODE
  _serial1.println(ret);
#endif
  if (ret.indexOf("OK") > 0)
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
    _serial1.println("Parameter error");
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

  ret = sendRawCommand("at+set_config=lora:send_interval:" + (String)mode + ":" + (String)value);
  if (ret.indexOf("OK") > 0)
  {
    return true;
  }
  else
  {
    return false;
  }
}

void RAK811::rk_reset(int mode)
{
  sendRawCommand(F("at+set_config=device:restart"));
}

bool RAK811::rk_setWorkingMode(int mode)
{
  switch (mode)
  {
    case 0:
      ret = sendRawCommand(F("at+set_config=lora:work_mode:0")); //Set LoRaWAN Mode.
      break;
    case 1:
      ret = sendRawCommand(F("at+set_config=lora:work_mode:1")); //Set LoRaP2P Mode.
      break;
    default:
      return false;
  }
  if (ret.indexOf("OK") > 0)
  {
    return true;
  }
  else
  {
    return false;
  }
}

bool RAK811::rk_setJoinMode(int mode)
{
  switch (mode)
  {
    case 0:
      ret = sendRawCommand(F("at+set_config=lora:join_mode:0")); //join Network through OTAA mode.
      break;
    case 1:
      ret = sendRawCommand(F("at+set_config=lora:join_mode:1")); //join Network through ABP mode.
      break;
    default:
      return false;
  }
  if (ret.indexOf("OK") > 0)
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
  ret = sendRawCommand(command);
  for (int i = 0; i <= timeout/2; i++)
  {
    ret = _serial.readStringUntil('\0');
    ret.trim();
    if (ret != NULL)
    {
      _serial1.print("<- " + ret);
      if (ret.indexOf("OK") > 0)
        return true;
      else if(ret.indexOf("ERROR") >= 0)
        return false;
    }
  }

  return false;
}

bool RAK811::rk_initOTAA(String devEUI, String appEUI, String appKEY)
{
  String command = "";
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
  ret = sendRawCommand(command);
  if (ret.indexOf("OK") > 0)
  {
    command = "at+set_config=lora:app_eui:" + _appEUI;
    //  _serial1.println(command);
    ret = sendRawCommand(command);
    if (ret.indexOf("OK") > 0)
    {
      command = "at+set_config=lora:app_key:" + _appKEY;
      //  _serial1.println(command);
      ret = sendRawCommand(command);
      if (ret.indexOf("OK") > 0)
      {
        return true;
      }
    }
  }

  return false;
}

bool RAK811::rk_initABP(String devADDR, String nwksKEY, String appsKEY)
{
  String command = "";
  if (devADDR.length() == 8)
  {
    _devADDR = devADDR;
  }
  else
  {
    _serial1.println("The parameter devADDR is set incorrectly!");
  }
  if (nwksKEY.length() == 32)
  {
    _nwksKEY = nwksKEY;
  }
  else
  {
    _serial1.println("The parameter nwksKEY is set incorrectly!");
  }
  if (appsKEY.length() == 32)
  {
    _appsKEY = appsKEY;
  }
  else
  {
    _serial1.println("The parameter appsKEY is set incorrectly!");
  }

  command = "at+set_config=lora:dev_addr:" + _devADDR;
  //  _serial1.println(command);
  ret = sendRawCommand(command);
  if (ret.indexOf("OK") > 0)
  {
    command = "at+set_config=lora:nwks_key:" + _nwksKEY;
    //  _serial1.println(command);
    ret = sendRawCommand(command);
    if (ret.indexOf("OK") > 0)
    {
      command = "at+set_config=lora:apps_key:" + _appsKEY;
      //  _serial1.println(command);
      ret = sendRawCommand(command);
      if (ret.indexOf("OK") > 0)
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
      ret = sendRawCommand(F("at+set_config=lora:confirm:0")); //LoRa data send with unconfirm.
      break;
    case 1:
      ret = sendRawCommand(F("at+set_config=lora:confirm:1")); //LoRa data send with confirm.
      break;
    default:
      return false;
  }
  if (ret.indexOf("OK") > 0)
  {
    return true;
  }
  else
  {
    return false;
  }
}

bool RAK811::rk_sendData(int port, char *datahex)
{
  String command = "";
  command = "at+send=lora:" + (String)port + ":" + datahex;
  //  _serial1.println(command);
  ret = sendRawCommand(command);

  if (ret == command)
  {
    return true;
  }
  else
  {
    return false;
  }    
}

String RAK811::rk_recvData(void)
{
  _serial.setTimeout(2000);
  ret = _serial.readStringUntil('\0');
  ret.trim();
#if defined DEBUG_MODE
  _serial1.print("<- " + ret);
#endif
  return ret;
}

bool RAK811::rk_initP2P(String FREQ, int SF, int BW, int CR, int PRlen, int PWR)
{
  String command = "";
  command = "at+set_config=lorap2p:" + FREQ + "," + SF + "," + BW + "," + CR + "," + PRlen + "," + PWR;
  //  _serial1.println(command);
  ret = sendRawCommand(command);
  if (ret.indexOf("OK") > 0)
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
  delay(500);
  return ret;
}

bool RAK811::rk_sendP2PData(char *datahex)
{
  String DATAHEX = datahex;
  String command = "at+send=lorap2p:" + DATAHEX;
  //  _serial1.println(command);
  ret = sendRawCommand(command);
  if (ret.indexOf("OK") > 0)
  {
    return true;
  }
  else
  {
    return false;
  }
}

String RAK811::rk_checkDeviceStatus(void)
{
  ret = sendRawCommand(F("at+get_config=device:status"));
  ret.trim();
  return ret;
}

bool RAK811::rk_setUARTConfig(int UartPort, int Baud)
{
  String command = "";
  command = "at+set_config=device:uart:" + (String)UartPort + ":" + (String)Baud;
  //  _serial1.println(command);
  ret = sendRawCommand(command);
  if (ret.indexOf("OK") > 0)
  {
    return true;
  }
  else
  {
    return false;
  }
}

String RAK811::sendRawCommand(String command)
{
  // delay(100);
  while (_serial.available())
  {
#if defined DEBUG_MODE
    _serial1.println("-> " + _serial.readStringUntil('\0')); //Clear buffer
#else
    _serial.read();
#endif
  }
  // delay(100);
  _serial.println(command);
  delay(200);
  ret = _serial.readStringUntil('\0');
  ret.trim();
  // delay(100);
#if defined DEBUG_MODE
  _serial1.println("-> " + command);
  _serial1.println("<- " + ret);
#endif
  return ret;
}
