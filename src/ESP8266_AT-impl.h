/****************************************************************************************************************************
  ESP8266_AT-impl.h - Dead simple web-server.
  For ESP8266/ESP32 AT-command running shields

  ESP8266_AT_WebServer is a library for the ESP8266/ESP32 AT-command shields to run WebServer
  Based on and modified from ESP8266 https://github.com/esp8266/Arduino/releases
  Built by Khoi Hoang https://github.com/khoih-prog/ESP8266_AT_WebServer
  Licensed under MIT license

  Original author:
  @file       Esp8266WebServer.h
  @author     Ivan Grokhotkov

  Version: 1.6.0

  Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
  1.0.0   K Hoang      12/02/2020 Initial coding for Arduino Mega, Teensy, etc
  ...
  1.4.0   K Hoang      14/08/2021 Add support to Adafruit nRF52 core v0.22.0+
  1.4.1   K Hoang      08/12/2021 Add Packages_Patches and instructions for BOARD_SIPEED_MAIX_DUINO
  1.5.0   K Hoang      19/12/2021 Reduce usage of Arduino String with std::string
  1.5.1   K Hoang      24/12/2021 Fix bug
  1.5.2   K Hoang      28/12/2021 Fix wrong http status header bug
  1.5.3   K Hoang      12/01/2022 Fix authenticate issue caused by libb64
  1.5.4   K Hoang      26/04/2022 Use new arduino.tips site. Improve examples
  1.6.0   K Hoang      16/11/2022 Fix severe limitation to permit sending larger data than 2K buffer. Add CORS
 *****************************************************************************************************************************/

#ifndef ESP8266_AT_impl_h
#define ESP8266_AT_impl_h

////////////////////////////////////////

#include "ESP8266_AT_Client.h"
#include "ESP8266_AT_Server.h"

#include "utility/RingBuffer.h"
#include "utility/ESP8266_AT_Debug.h"

////////////////////////////////////////

int16_t   ESP8266_AT_Class::_state[MAX_SOCK_NUM]        = { NA_STATE, NA_STATE, NA_STATE, NA_STATE };
uint16_t  ESP8266_AT_Class::_server_port[MAX_SOCK_NUM]  = { 0, 0, 0, 0 };

////////////////////////////////////////

uint8_t ESP8266_AT_Class::espMode = 0;

////////////////////////////////////////

ESP8266_AT_Class::ESP8266_AT_Class()
{
}

////////////////////////////////////////

void ESP8266_AT_Class::init(Stream* espSerial)
{
  AT_LOGINFO(F("Initializing ESP module"));
  ESP8266_AT_Drv::wifiDriverInit(espSerial);
}

////////////////////////////////////////

void ESP8266_AT_Class::reInit(void)
{
  AT_LOGINFO(F("Initializing ESP module"));
  ESP8266_AT_Drv::wifiDriverReInit();
}

////////////////////////////////////////

char* ESP8266_AT_Class::firmwareVersion()
{
  return ESP8266_AT_Drv::getFwVersion();
}

////////////////////////////////////////

int ESP8266_AT_Class::begin(const char* ssid, const char* passphrase)
{
  espMode = 1;

  if (ESP8266_AT_Drv::wifiConnect(ssid, passphrase))
    return WL_CONNECTED;

  return WL_CONNECT_FAILED;
}

////////////////////////////////////////

int ESP8266_AT_Class::beginAP(const char* ssid, uint8_t channel, const char* pwd, uint8_t enc, bool apOnly)
{
  if (apOnly)
    espMode = 2;
  else
    espMode = 3;

  if (ESP8266_AT_Drv::wifiStartAP(ssid, pwd, channel, enc, espMode))
    return WL_CONNECTED;

  return WL_CONNECT_FAILED;
}

////////////////////////////////////////

int ESP8266_AT_Class::beginAP(const char* ssid)
{
  return beginAP(ssid, 10, "", 0);
}

////////////////////////////////////////

int ESP8266_AT_Class::beginAP(const char* ssid, uint8_t channel)
{
  return beginAP(ssid, channel, "", 0);
}

////////////////////////////////////////

void ESP8266_AT_Class::config(IPAddress ip)
{
  ESP8266_AT_Drv::config(ip);
}

////////////////////////////////////////

void ESP8266_AT_Class::configAP(IPAddress ip)
{
  ESP8266_AT_Drv::configAP(ip);
}

////////////////////////////////////////

int ESP8266_AT_Class::disconnect()
{
  return ESP8266_AT_Drv::disconnect();
}

////////////////////////////////////////

uint8_t* ESP8266_AT_Class::macAddress(uint8_t* mac)
{
  // TODO we don't need _mac variable
  uint8_t* _mac = ESP8266_AT_Drv::getMacAddress();

  memcpy(mac, _mac, WL_MAC_ADDR_LENGTH);

  return mac;
}

////////////////////////////////////////

IPAddress ESP8266_AT_Class::localIP()
{
  IPAddress ret;

  if (espMode == 1)
    ESP8266_AT_Drv::getIpAddress(ret);
  else
    ESP8266_AT_Drv::getIpAddressAP(ret);

  return ret;
}

////////////////////////////////////////

IPAddress ESP8266_AT_Class::subnetMask()
{
  IPAddress mask;

  if (espMode == 1)
    ESP8266_AT_Drv::getNetmask(mask);

  return mask;
}

////////////////////////////////////////

IPAddress ESP8266_AT_Class::gatewayIP()
{
  IPAddress gw;

  if (espMode == 1)
    ESP8266_AT_Drv::getGateway(gw);

  return gw;
}

////////////////////////////////////////

char* ESP8266_AT_Class::SSID()
{
  return ESP8266_AT_Drv::getCurrentSSID();
}

////////////////////////////////////////

uint8_t* ESP8266_AT_Class::BSSID(uint8_t* bssid)
{
  // TODO we don't need _bssid
  uint8_t* _bssid = ESP8266_AT_Drv::getCurrentBSSID();

  memcpy(bssid, _bssid, WL_MAC_ADDR_LENGTH);

  return bssid;
}

////////////////////////////////////////

int32_t ESP8266_AT_Class::RSSI()
{
  return ESP8266_AT_Drv::getCurrentRSSI();
}

////////////////////////////////////////

int8_t ESP8266_AT_Class::scanNetworks()
{
  return ESP8266_AT_Drv::getScanNetworks();
}

////////////////////////////////////////

char* ESP8266_AT_Class::SSID(uint8_t networkItem)
{
  return ESP8266_AT_Drv::getSSIDNetoworks(networkItem);
}

////////////////////////////////////////

int32_t ESP8266_AT_Class::RSSI(uint8_t networkItem)
{
  return ESP8266_AT_Drv::getRSSINetoworks(networkItem);
}

////////////////////////////////////////

uint8_t ESP8266_AT_Class::encryptionType(uint8_t networkItem)
{
  return ESP8266_AT_Drv::getEncTypeNetowrks(networkItem);
}

////////////////////////////////////////

uint8_t ESP8266_AT_Class::status()
{
  return ESP8266_AT_Drv::getConnectionStatus();
}

////////////////////////////////////////////////////////////////////////////
// Non standard methods
////////////////////////////////////////////////////////////////////////////

void ESP8266_AT_Class::reset(void)
{
  ESP8266_AT_Drv::reset();
}

////////////////////////////////////////

// Restore the the Factory Default Settings of ESP module.
// Sometimes necessaty for ESP32-AT
void ESP8266_AT_Class::restore(void)
{
  ESP8266_AT_Drv::restore();
}

////////////////////////////////////////

/*
  void ESP8266::hardReset(void)
  {
  connected = false;
  strcpy(ip, "");
  digitalWrite(ESP8266_RST, LOW);
  delay(ESP8266_HARD_RESET_DURATION);
  digitalWrite(ESP8266_RST, HIGH);
  delay(ESP8266_HARD_RESET_DURATION);
  }
*/

////////////////////////////////////////

bool ESP8266_AT_Class::ping(const char *host)
{
  return ESP8266_AT_Drv::ping(host);
}

////////////////////////////////////////

uint8_t ESP8266_AT_Class::getFreeSocket()
{
  // ESP Module assigns socket numbers in ascending order, so we will assign them in descending order
  for (int i = MAX_SOCK_NUM - 1; i >= 0; i--)
  {
    if (_state[i] == NA_STATE)
    {
      return i;
    }
  }

  return SOCK_NOT_AVAIL;
}

////////////////////////////////////////

void ESP8266_AT_Class::allocateSocket(uint8_t sock)
{
  _state[sock] = sock;
}

////////////////////////////////////////

void ESP8266_AT_Class::releaseSocket(uint8_t sock)
{
  _state[sock] = NA_STATE;
}

////////////////////////////////////////

ESP8266_AT_Class WiFi;

////////////////////////////////////////

#endif    //ESP8266_AT_impl_h
