/****************************************************************************************************************************
  ESP8266_AT_Drv.cpp - Dead simple web-server.
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

#include <Arduino.h>

////////////////////////////////////////

// ESP32/ESP8266 includes <pgmspace.h> by default, and memccpy_P was already defined there
#if !(ESP32 || ESP8266 || BOARD_SIPEED_MAIX_DUINO)
  #include <avr/pgmspace.h>
  #define memccpy_P(dest, src, c, n) memccpy((dest), (src), (c), (n))
#endif

////////////////////////////////////////

#define _ESP_AT_LOGLEVEL_       1

////////////////////////////////////////

#if    ( defined(ARDUINO_SAMD_ZERO) || defined(ARDUINO_SAMD_MKR1000) || defined(ARDUINO_SAMD_MKRWIFI1010) \
      || defined(ARDUINO_SAMD_NANO_33_IOT) || defined(ARDUINO_SAMD_MKRFox1200) || defined(ARDUINO_SAMD_MKRWAN1300) \
      || defined(ARDUINO_SAMD_MKRWAN1310)  || defined(ARDUINO_SAMD_MKRGSM1400) || defined(ARDUINO_SAMD_MKRNB1500) \
      || defined(ARDUINO_SAMD_MKRVIDOR4000) || defined(__SAMD21G18A__) || defined(ARDUINO_SAMD_CIRCUITPLAYGROUND_EXPRESS) \
      || defined(__SAMD21E18A__) || defined(__SAMD51__) || defined(__SAMD51J20A__) || defined(__SAMD51J19A__) \
      || defined(__SAMD51G19A__) || defined(__SAMD21G18A__) )
#if defined(ESP8266_AT_USE_SAMD)
  #undef ESP8266_AT_USE_SAMD
#endif
#define ESP8266_AT_USE_SAMD      true

#if(_ESP_AT_LOGLEVEL_> 3)
  #warning Use SAMD architecture from ESP8266_AT_Drv_h
#endif

////////////////////////////////////////

#elif ( defined(NRF52840_FEATHER) || defined(NRF52832_FEATHER) || defined(NRF52_SERIES) || defined(ARDUINO_NRF52_ADAFRUIT) || \
      defined(NRF52840_FEATHER_SENSE) || defined(NRF52840_ITSYBITSY) || defined(NRF52840_CIRCUITPLAY) || defined(NRF52840_CLUE) || \
      defined(NRF52840_METRO) || defined(NRF52840_PCA10056) || defined(PARTICLE_XENON) || defined(NINA_B302_ublox) || defined(NINA_B112_ublox) )
#if defined(ESP8266_AT_USE_NRF528XX)
  #undef ESP8266_AT_USE_NRF528XX
#endif
#define ESP8266_AT_USE_NRF528XX      true

#if(_ESP_AT_LOGLEVEL_> 3)
  #warning Use nFR52 architecture from ESP8266_AT_WebServer
#endif

////////////////////////////////////////

#elif ( defined(ARDUINO_SAM_DUE) || defined(__SAM3X8E__) )
#if defined(ESP8266_AT_USE_SAM_DUE)
  #undef ESP8266_AT_USE_SAM_DUE
#endif
#define ESP8266_AT_USE_SAM_DUE      true

////////////////////////////////////////

#endif

////////////////////////////////////////

#if (ESP8266_AT_USE_SAMD || ESP8266_AT_USE_SAM_DUE || ESP8266_AT_USE_NRF528XX || defined(ARDUINO_ARCH_MBED) )
  #include <cstdarg>
  #define vsnprintf_P vsnprintf
#endif

////////////////////////////////////////

#include "utility/ESP8266_AT_Drv.h"
#include "utility/ESP8266_AT_Debug.h"

////////////////////////////////////////

#define NUMESPTAGS 5

////////////////////////////////////////

const char* ESPTAGS[] =
{
  "\r\nOK\r\n",
  "\r\nERROR\r\n",
  "\r\nFAIL\r\n",
  "\r\nSEND OK\r\n",
  " CONNECT\r\n"
};

////////////////////////////////////////

typedef enum
{
  TAG_OK,
  TAG_ERROR,
  TAG_FAIL,
  TAG_SENDOK,
  TAG_CONNECT
} TagsEnum;

////////////////////////////////////////

Stream *ESP8266_AT_Drv::espSerial = NULL;

#if ( defined(ARDUINO_AVR_MEGA) || defined(ARDUINO_AVR_MEGA2560) || defined(STM32F2) || defined(STM32F3) )
  AT_RingBuffer ESP8266_AT_Drv::ringBuf(512);

  #if(_ESP_AT_LOGLEVEL_> 3)
    #warning Using RingBuffer(512) from ESP8266_AT_Drv
  #endif
#elif ( defined(STM32F1) )
  AT_RingBuffer ESP8266_AT_Drv::ringBuf(256);

  #if(_ESP_AT_LOGLEVEL_> 3)
    #warning Using RingBuffer(256) from ESP8266_AT_Drv
  #endif
#else
  AT_RingBuffer ESP8266_AT_Drv::ringBuf(2048);

  #if(_ESP_AT_LOGLEVEL_> 3)
    #warning Using RingBuffer(2048) from ESP8266_AT_Drv
  #endif
#endif

////////////////////////////////////////

// Array of data to cache the information related to the networks discovered
char    ESP8266_AT_Drv::_networkSsid[][WL_SSID_MAX_LENGTH]    = {{"1"}, {"2"}, {"3"}, {"4"}, {"5"}};
int32_t ESP8266_AT_Drv::_networkRssi[WL_NETWORKS_LIST_MAXNUM] = { 0 };
uint8_t ESP8266_AT_Drv::_networkEncr[WL_NETWORKS_LIST_MAXNUM] = { 0 };

// Cached values of retrieved data
char    ESP8266_AT_Drv::_ssid[]     = {0};
uint8_t ESP8266_AT_Drv::_bssid[]    = {0};
uint8_t ESP8266_AT_Drv::_mac[]      = {0};
uint8_t ESP8266_AT_Drv::_localIp[]  = {0};
char    ESP8266_AT_Drv::fwVersion[] = {0};

long    ESP8266_AT_Drv::_bufPos     = 0;
uint8_t ESP8266_AT_Drv::_connId     = 0;

uint16_t  ESP8266_AT_Drv::_remotePort = 0;
uint8_t   ESP8266_AT_Drv::_remoteIp[] = {0};

////////////////////////////////////////

// KH New from v1.0.8
// ReInit, don't need to specify but use current espSerial
void ESP8266_AT_Drv::wifiDriverReInit()
{
  if (ESP8266_AT_Drv::espSerial == NULL)
  {
    AT_LOGDEBUG(F("> Error. Can't use wifiDriverReInit"));
    return;
  }

  bool initOK = false;

  for (int i = 0; i < 5; i++)
  {
    if (sendCmd(F("AT")) == TAG_OK)
    {
      initOK = true;
      break;
    }

    delay(1000);
  }

  if (!initOK)
  {
    AT_LOGERROR(F("Cannot initialize ESP module"));
    delay(5000);
    return;
  }

  reset();

  // check firmware version
  getFwVersion();

  bool versionSupported = true;

  if (useESP32_AT)
  {
    // prints a warning message if the firmware is not v3.X or v4.X
    if ((fwVersion[1] != '3' and fwVersion[1] != '4') or
        fwVersion[2] != '.')
    {
      versionSupported = false;
    }
  }
  else
  {
    // prints a warning message if the firmware is not 1.X or 2.X
    if ((fwVersion[0] != '1' and fwVersion[0] != '2') or
        fwVersion[1] != '.')
    {
      versionSupported = false;
    }
  }

  // prints a warning message if the firmware is not 3.X or 4.X
  if (versionSupported)
  {
    AT_LOGINFO1(F("Firmware Init OK -"), fwVersion);
  }
  else
  {
    AT_LOGWARN1(F("Warning: Unsupported Firmware"), fwVersion);
  }

  delay(4000);
}

////////////////////////////////////////

void ESP8266_AT_Drv::wifiDriverInit(Stream *espSerial)
{
  AT_LOGDEBUG(F("> wifiDriverInit"));

  ESP8266_AT_Drv::espSerial = espSerial;

  wifiDriverReInit();
}

////////////////////////////////////////

void ESP8266_AT_Drv::reset()
{
  AT_LOGDEBUG(F("> reset"));

  AT_LOGINFO(F("AT+RST"));
  sendCmd(F("AT+RST"));

  delay(3000);
  espEmptyBuf(false);  // empty dirty characters from the buffer

  // disable echo of commands
  AT_LOGINFO(F("ATE0"));
  sendCmd("ATE0");

  // set station mode
  if (useESP32_AT)
  {
    AT_LOGERROR(F("Using ESP32-AT Command"));
    AT_LOGINFO(F("AT+CWMODE=1"));
    sendCmd(F("AT+CWMODE=1"));
  }
  else
  {
    AT_LOGERROR(F("Use ES8266-AT Command"));
    AT_LOGINFO(F("AT+CWMODE_CUR=1"));
    sendCmd(F("AT+CWMODE_CUR=1"));
  }

  delay(200);

  // set multiple connections mode
  AT_LOGINFO(F("AT+CIPMUX=1"));
  sendCmd(F("AT+CIPMUX=1"));

  // Show remote IP and port with "+IPD"
  AT_LOGINFO(F("AT+CIPDINFO=1"));
  sendCmd(F("AT+CIPDINFO=1"));

  // Disable autoconnect
  // Automatic connection can create problems during initialization phase at next boot
  AT_LOGINFO(F("AT+CWAUTOCONN=0"));
  sendCmd(F("AT+CWAUTOCONN=0"));

  // enable STA DHCP
  if (useESP32_AT)
  {
    // ESP32
    // AT+CWDHCP=<operate>,<mode>
    // • <operate>:
    //  ‣ 0: disable, ‣ 1: enable
    // • <mode>:
    //  ‣ Bit0: Station DHCP, ‣ Bit1: SoftAP DHCP
    AT_LOGINFO(F("AT+CWDHCP=1,1"));
    sendCmd(F("AT+CWDHCP=1,1"));
  }
  else
  {
    // ESP8266
    // AT+CWDHCP=<mode>,<en>
    // • <mode>:
    //  ‣ 0: Sets ESP8266 SoftAP, ‣ 1: Sets ESP8266 Station, ‣ 2: Sets both SoftAP and Station
    // • <en>:
    // ‣ 0: Disables DHCP, ‣ 1: Enables DHCP
    AT_LOGINFO(F("AT+CWDHCP=1,1"));
    sendCmd(F("AT+CWDHCP_CUR=1,1"));
  }

  delay(200);
}

////////////////////////////////////////

// KH New from v1.0.8
// For ESP32-AT to restores the Factory Default Settings
void ESP8266_AT_Drv::restore()
{
  AT_LOGDEBUG(F("> restore"));

  AT_LOGINFO(F("AT+RESTORE"));
  sendCmd(F("AT+RESTORE"));

  delay(1000);
  espEmptyBuf(false);  // empty dirty characters from the buffer
}

////////////////////////////////////////

bool ESP8266_AT_Drv::wifiConnect(const char* ssid, const char* passphrase)
{
  AT_LOGDEBUG(F("> wifiConnect"));

  // TODO
  // Escape character syntax is needed if "SSID" or "password" contains
  // any special characters (',', '"' and '/')

  // connect to access point, use CUR mode to avoid connection at boot
  int ret;

  if (useESP32_AT)
  {
    AT_LOGINFO2(F("AT+CWJAP="), ssid, passphrase);
    ret = sendCmd(F("AT+CWJAP=\"%s\",\"%s\""), 20000, ssid, passphrase);
  }
  else
  {
    AT_LOGINFO2(F("AT+CWJAP_CUR="), ssid, passphrase);
    ret = sendCmd(F("AT+CWJAP_CUR=\"%s\",\"%s\""), 20000, ssid, passphrase);
  }

  if (ret == TAG_OK)
  {
    AT_LOGINFO1(F("Connected to"), ssid);
    return true;
  }

  AT_LOGWARN1(F("Failed connecting to"), ssid);

  // clean additional messages logged after the FAIL tag
  delay(1000);
  espEmptyBuf(false);

  return false;
}

////////////////////////////////////////

// espMode
// ‣ 0: Null mode, Wi-Fi RF will be disabled *
// ‣ 1: Station mode
// ‣ 2: SoftAP mode
// ‣ 3: SoftAP+Station mode
bool ESP8266_AT_Drv::wifiStartAP(const char* ssid, const char* pwd, uint8_t channel, uint8_t enc, uint8_t espMode)
{
  AT_LOGDEBUG(F("> wifiStartAP"));

  // set AP mode, use CUR mode to avoid automatic start at boot
  int ret;

  if (useESP32_AT)
  {
    AT_LOGINFO1(F("AT+CWMODE="), espMode);
    ret = sendCmd(F("AT+CWMODE=%d"), 10000, espMode);
  }
  else
  {
    AT_LOGINFO1(F("AT+CWMODE_CUR="), espMode);
    ret = sendCmd(F("AT+CWMODE_CUR=%d"), 10000, espMode);
  }

  if (ret != TAG_OK)
  {
    AT_LOGWARN1(F("Failed to set AP mode"), ssid);
    return false;
  }

  // TODO
  // Escape character syntax is needed if "SSID" or "password" contains
  // any special characters (',', '"' and '/')

  // Configures the ESP8266/ESP32 SoftAP
  if (useESP32_AT)
  {
    AT_LOGINFO2(F("AT+CWSAP="), ssid, pwd);
    AT_LOGINFO2(F("AT+CWSAP="), channel, enc);
    ret = sendCmd(F("AT+CWSAP=\"%s\",\"%s\",%d,%d"), 10000, ssid, pwd, channel, enc);
  }
  else
  {
    AT_LOGINFO2(F("AT+CWSAP_CUR="), ssid, pwd);
    AT_LOGINFO2(F("AT+CWSAP_CUR="), channel, enc);
    ret = sendCmd(F("AT+CWSAP_CUR=\"%s\",\"%s\",%d,%d"), 10000, ssid, pwd, channel, enc);
  }

  if (ret != TAG_OK)
  {
    AT_LOGWARN1(F("Failed to start AP"), ssid);

    return false;
  }

  // Get info if DHCP Enabled or Disabled
  // ESP32-AT not support _CUR and _DEF here and has different command format
  // Be careful
  // ESP32-AT => AT+CWDHCP=<operate>,<mode>
  // • <operate>:
  //    ‣ 0: disable
  //    ‣ 1: enable
  // • <mode>:
  //    ‣ Bit0: Station DHCP
  //    ‣ Bit1: SoftAP DHCP
  //
  // ESP8266-AT => AT+CWDHCP=<mode>,<en>
  // • <mode>:
  //    ‣ 0: Sets ESP8266 SoftAP
  //    ‣ 1: Sets ESP8266 Station
  //    ‣ 2: Sets both SoftAP and Station
  // • <en>:
  //    ‣ 0: disable DHCP
  //    ‣ 1: enable DHCP

  if (espMode == 2)
  {
    // ‣ 2: SoftAP mode
    if (useESP32_AT)
    {
      AT_LOGINFO(F("AT+CWDHCP=1,2"));
      sendCmd(F("AT+CWDHCP=1,2"));         // enable DHCP for SoftAP mode
    }
    else
    {
      AT_LOGINFO(F("AT+CWDHCP_CUR=0,1"));
      sendCmd(F("AT+CWDHCP_CUR=0,1"));     // enable DHCP for SoftAP mode
    }
  }
  else if (espMode == 3)
  {
    // ‣ 3: SoftAP+Station mode
    if (useESP32_AT)
    {
      AT_LOGINFO(F("AT+CWDHCP=1,3"));
      sendCmd(F("AT+CWDHCP=1,3"));         // enable DHCP for station and SoftAP mode
    }
    else
    {
      AT_LOGINFO(F("AT+CWDHCP_CUR=2,1"));
      sendCmd(F("AT+CWDHCP_CUR=2,1"));     // enable DHCP for station and SoftAP mode
    }
  }

  AT_LOGINFO1(F("Access point started"), ssid);

  return true;
}

////////////////////////////////////////

int8_t ESP8266_AT_Drv::disconnect()
{
  AT_LOGDEBUG(F("> disconnect"));

  AT_LOGINFO(F("AT+CWQAP"));

  if (sendCmd(F("AT+CWQAP")) == TAG_OK)
    return WL_DISCONNECTED;

  // wait and clear any additional message
  delay(2000);
  espEmptyBuf(false);

  return WL_DISCONNECTED;
}

////////////////////////////////////////

// Set STA IP
void ESP8266_AT_Drv::config(IPAddress ip)
{
  AT_LOGDEBUG(F("> config"));

  // Get info if DHCP Enabled or Disabled
  // ESP32-AT not support _CUR and _DEF here and has different command format
  // Be careful
  // ESP32-AT => AT+CWDHCP=<operate>,<mode>
  // • <operate>:
  //    ‣ 0: disable
  //    ‣ 1: enable
  // • <mode>:
  //    ‣ Bit0: Station DHCP
  //    ‣ Bit1: SoftAP DHCP
  //
  // ESP8266-AT => AT+CWDHCP=<mode>,<en>
  // • <mode>:
  //    ‣ 0: Sets ESP8266 SoftAP
  //    ‣ 1: Sets ESP8266 Station
  //    ‣ 2: Sets both SoftAP and Station
  // • <en>:
  //    ‣ 0: disable DHCP
  //    ‣ 1: enable DHCP

  // disable station DHCP
  if (useESP32_AT)
  {
    //KH, AT+CWDHCP=<operate>,<mode>
    AT_LOGINFO(F("AT+CWDHCP=0,1"));
    sendCmd(F("AT+CWDHCP=0,1"));
  }
  else
  {
    // KH, AT+CWDHCP=<<mode>,<en>
    AT_LOGINFO(F("AT+CWDHCP_CUR=1,0"))
    sendCmd(F("AT+CWDHCP_CUR=1,0"));
  }

  // it seems we need to wait here...
  delay(500);

  char buf[16];

  // KH, Restore PROGMEM commands
  sprintf_P(buf, PSTR("%d.%d.%d.%d"), ip[0], ip[1], ip[2], ip[3]);

  int ret;

  if (useESP32_AT)
  {
    // Set STA IP address
    AT_LOGINFO1(F("AT+CIPSTA="), buf);
    ret = sendCmd(F("AT+CIPSTA=\"%s\""), 2000, buf);
  }
  else
  {
    // Set STA IP address
    AT_LOGINFO1(F("AT+CIPSTA_CUR="), buf);
    ret = sendCmd(F("AT+CIPSTA_CUR=\"%s\""), 2000, buf);
  }

  delay(500);

  if (ret == TAG_OK)
  {
    AT_LOGINFO1(F("IP address set"), buf);
  }
}

////////////////////////////////////////

// Set AP IP
void ESP8266_AT_Drv::configAP(IPAddress ip)
{
  AT_LOGDEBUG(F("> config"));

  // Get info if DHCP Enabled or Disabled
  // ESP32-AT not support _CUR and _DEF here and has different command format
  // Be careful
  // ESP32-AT => AT+CWDHCP=<operate>,<mode>
  // • <operate>:
  //    ‣ 0: disable
  //    ‣ 1: enable
  // • <mode>:
  //    ‣ Bit0: Station DHCP
  //    ‣ Bit1: SoftAP DHCP
  //
  // ESP8266-AT => AT+CWDHCP=<mode>,<en>
  // • <mode>:
  //    ‣ 0: Sets ESP8266 SoftAP
  //    ‣ 1: Sets ESP8266 Station
  //    ‣ 2: Sets both SoftAP and Station
  // • <en>:
  //    ‣ 0: disable DHCP
  //    ‣ 1: enable DHCP

  if (useESP32_AT)
  {
    // SoftAP Mode
    AT_LOGINFO(F("AT+CWMODE=2"));
    sendCmd(F("AT+CWMODE=2"));

    //KH, AT+CWDHCP=<operate>,<mode>
    // disable AP and STA DHCP
    AT_LOGINFO(F("AT+CWDHCP=0,3"));
    sendCmd(F("AT+CWDHCP=0,3"));
  }
  else
  {
    // SoftAP Mode
    AT_LOGINFO(F("AT+CWMODE_CUR=2"));
    sendCmd(F("AT+CWMODE_CUR=2"));

    // KH, AT+CWDHCP=<<mode>,<en>
    // disable AP and STA DHCP
    AT_LOGINFO(F("AT+CWDHCP=2,0"));
    sendCmd(F("AT+CWDHCP_CUR=2,0"));
  }

  // it seems we need to wait here...
  delay(500);

  char buf[16];

  // KH, Restore PROGMEM commands
  sprintf_P(buf, PSTR("%d.%d.%d.%d"), ip[0], ip[1], ip[2], ip[3]);

  int ret;

  if (useESP32_AT)
  {
    // Set AP IP address
    AT_LOGINFO1(F("AT+CIPAP="), buf);
    ret = sendCmd(F("AT+CIPAP=\"%s\""), 2000, buf);
  }
  else
  {
    // Set AP IP address
    AT_LOGINFO1(F("AT+CIPAP_CUR="), buf);
    ret = sendCmd(F("AT+CIPAP_CUR=\"%s\""), 2000, buf);
  }

  delay(500);

  if (ret == TAG_OK)
  {
    AT_LOGINFO1(F("IP address set"), buf);
  }
}

////////////////////////////////////////

uint8_t ESP8266_AT_Drv::getConnectionStatus()
{
  AT_LOGDEBUG(F("> getConnectionStatus"));

  /*
    AT+CIPSTATUS

    Response

      STATUS:<stat>
      +CIPSTATUS:<link ID>,<type>,<remote_IP>,<remote_port>,<local_port>,<tetype>

    Parameters

      <stat>
        2: Got IP
        3: Connected
        4: Disconnected
      <link ID> ID of the connection (0~4), for multi-connect
      <type> string, "TCP" or "UDP"
      <remote_IP> string, remote IP address.
      <remote_port> remote port number
      <local_port> ESP8266 local port number
      <tetype>
        0: ESP8266 runs as client
        1: ESP8266 runs as server
  */

  char buf[10];

  memset(buf, 0, sizeof(buf));

  AT_LOGINFO(F("AT+CIPSTATUS"));

  if (!sendCmdGet(F("AT+CIPSTATUS"), F("STATUS:"), F("\r\n"), buf, sizeof(buf)))
    return WL_NO_SHIELD;

  // 4: client disconnected
  // 5: wifi disconnected
  int s = atoi(buf);

  if (s == 2 or s == 3 or s == 4)
    return WL_CONNECTED;
  else if (s == 5)
    return WL_DISCONNECTED;

  return WL_IDLE_STATUS;
}

////////////////////////////////////////

uint8_t ESP8266_AT_Drv::getClientState(uint8_t sock)
{
  // KH add to fix dirty buf issue
  espEmptyBuf(true);  // empty dirty characters from the buffer

  AT_LOGDEBUG1(F("> getClientState"), sock);

  char findBuf[20];

  // KH, Restore PROGMEM commands
  sprintf_P(findBuf, PSTR("+CIPSTATUS:%d,"), sock);

  char buf[10];

  memset(buf, 0, sizeof(buf));

  //AT_LOGINFO1(F("getClientState: AT+CIPSTATUS"), sock);
  if (sendCmdGet("AT+CIPSTATUS", findBuf, ",", buf, sizeof(buf)))
  {
    AT_LOGDEBUG(F("Connected"));
    return true;
  }

  AT_LOGDEBUG(F("Not connected"));

  return false;
}

////////////////////////////////////////

uint8_t* ESP8266_AT_Drv::getMacAddress()
{
  AT_LOGDEBUG(F("> getMacAddress"));

  memset(_mac, 0, WL_MAC_ADDR_LENGTH);

  char buf[20];

  memset(buf, 0, sizeof(buf));

  AT_LOGINFO(F("AT+CIFSR"));

  if (sendCmdGet(F("AT+CIFSR"), F(":STAMAC,\""), F("\""), buf, sizeof(buf)))
  {
    char* token;

    token = strtok(buf, ":");
    _mac[5] = (byte)strtol(token, NULL, 16);
    token = strtok(NULL, ":");
    _mac[4] = (byte)strtol(token, NULL, 16);
    token = strtok(NULL, ":");
    _mac[3] = (byte)strtol(token, NULL, 16);
    token = strtok(NULL, ":");
    _mac[2] = (byte)strtol(token, NULL, 16);
    token = strtok(NULL, ":");
    _mac[1] = (byte)strtol(token, NULL, 16);
    token = strtok(NULL, ":");
    _mac[0] = (byte)strtol(token, NULL, 16);
  }

  return _mac;
}

////////////////////////////////////////

void ESP8266_AT_Drv::getIpAddress(IPAddress& ip)
{
  AT_LOGDEBUG(F("> getIpAddress"));

  char buf[20];

  memset(buf, 0, sizeof(buf));

  AT_LOGINFO(F("AT+CIFSR"));

  if (sendCmdGet(F("AT+CIFSR"), F(":STAIP,\""), F("\""), buf, sizeof(buf)))
  {
    char* token;

    token = strtok(buf, ".");
    _localIp[0] = atoi(token);
    token = strtok(NULL, ".");
    _localIp[1] = atoi(token);
    token = strtok(NULL, ".");
    _localIp[2] = atoi(token);
    token = strtok(NULL, ".");
    _localIp[3] = atoi(token);

    ip = _localIp;
  }
}

////////////////////////////////////////

void ESP8266_AT_Drv::getIpAddressAP(IPAddress& ip)
{
  AT_LOGDEBUG(F("> getIpAddressAP"));

  char buf[20];

  memset(buf, 0, sizeof(buf));

  bool ret;

  if (useESP32_AT)
  {
    // Get AP IP address
    AT_LOGINFO(F("AT+CIPAP?"));
    ret = sendCmdGet(F("AT+CIPAP?"), F("+CIPAP:ip:\""), F("\""), buf, sizeof(buf));
  }
  else
  {
    // Get AP IP address
    AT_LOGINFO(F("AT+CIPAP?"));

    // Send sendCmdGet with _CUR => received also with _CUR
    ret = sendCmdGet(F("AT+CIPAP_CUR?"), F("+CIPAP_CUR:ip:\""), F("\""), buf, sizeof(buf));
  }

  if (ret)
  {
    char* token;

    token = strtok(buf, ".");
    _localIp[0] = atoi(token);
    token = strtok(NULL, ".");
    _localIp[1] = atoi(token);
    token = strtok(NULL, ".");
    _localIp[2] = atoi(token);
    token = strtok(NULL, ".");
    _localIp[3] = atoi(token);

    ip = _localIp;
  }
}

////////////////////////////////////////

char* ESP8266_AT_Drv::getCurrentSSID()
{
  AT_LOGDEBUG(F("> getCurrentSSID"));

  _ssid[0] = 0;

  if (useESP32_AT)
  {
    // Get AP Info
    AT_LOGINFO(F("AT+CWJAP?"));
    sendCmdGet(F("AT+CWJAP?"), F("+CWJAP:\""), F("\""), _ssid, sizeof(_ssid));
  }
  else
  {
    // Get AP Info
    AT_LOGINFO(F("AT+CWJAP_CUR?"));
    // Send sendCmdGet with _CUR => received also with _CUR
    sendCmdGet(F("AT+CWJAP_CUR?"), F("+CWJAP_CUR:\""), F("\""), _ssid, sizeof(_ssid));
  }

  return _ssid;
}

////////////////////////////////////////

uint8_t* ESP8266_AT_Drv::getCurrentBSSID()
{
  AT_LOGDEBUG(F("> getCurrentBSSID"));

  memset(_bssid, 0, WL_MAC_ADDR_LENGTH);

  char buf[20];

  memset(buf, 0, sizeof(buf));

  bool ret;

  if (useESP32_AT)
  {
    // Get AP Info
    AT_LOGINFO(F("AT+CWJAP?"));
    ret = sendCmdGet(F("AT+CWJAP?"), F(",\""), F("\","), buf, sizeof(buf));
  }
  else
  {
    // Get AP Info
    AT_LOGINFO(F("AT+CWJAP_CUR?"));
    ret = sendCmdGet(F("AT+CWJAP_CUR?"), F(",\""), F("\","), buf, sizeof(buf));
  }

  if (ret)
  {
    char* token;

    token = strtok(buf, ":");
    _bssid[5] = (byte)strtol(token, NULL, 16);
    token = strtok(NULL, ":");
    _bssid[4] = (byte)strtol(token, NULL, 16);
    token = strtok(NULL, ":");
    _bssid[3] = (byte)strtol(token, NULL, 16);
    token = strtok(NULL, ":");
    _bssid[2] = (byte)strtol(token, NULL, 16);
    token = strtok(NULL, ":");
    _bssid[1] = (byte)strtol(token, NULL, 16);
    token = strtok(NULL, ":");
    _bssid[0] = (byte)strtol(token, NULL, 16);
  }

  return _bssid;

}

////////////////////////////////////////

int32_t ESP8266_AT_Drv::getCurrentRSSI()
{
  AT_LOGDEBUG(F("> getCurrentRSSI"));

  int ret = 0;
  char buf[10];

  memset(buf, 0, sizeof(buf));

  if (useESP32_AT)
  {
    // Get AP Info
    AT_LOGINFO(F("AT+CWJAP?"));
    sendCmdGet(F("AT+CWJAP?"), F(",-"), F("\r\n"), buf, sizeof(buf));
  }
  else
  {
    // Get AP Info
    AT_LOGINFO(F("AT+CWJAP_CUR?"));
    sendCmdGet(F("AT+CWJAP_CUR?"), F(",-"), F("\r\n"), buf, sizeof(buf));
  }

  if (isDigit(buf[0]))
  {
    ret = -atoi(buf);
  }

  return ret;
}

////////////////////////////////////////

uint8_t ESP8266_AT_Drv::getScanNetworks()
{
  uint8_t ssidListNum = 0;
  int idx;

  espEmptyBuf();

  AT_LOGDEBUG(F("----------------------------------------------"));
  AT_LOGDEBUG(F(">> AT+CWLAP"));

  espSerial->println("AT+CWLAP");
  AT_LOGINFO(F("AT+CWLAP"));

  idx = readUntil(10000, "+CWLAP:(");

  AT_LOGDEBUG3(F("idx = "), idx, F(", NUMESPTAGS = "), NUMESPTAGS);

  while (idx == NUMESPTAGS)
  {
    _networkEncr[ssidListNum] = espSerial->parseInt();

    AT_LOGDEBUG3(F("_networkEncr["), ssidListNum, F(" ] ="), _networkEncr[ssidListNum]);

    // discard , and " characters
    readUntil(1000, "\"");

    idx = readUntil(1000, "\"", false);

    if (idx == NUMESPTAGS)
    {
      memset(_networkSsid[ssidListNum], 0, WL_SSID_MAX_LENGTH );
      ringBuf.getStrN(_networkSsid[ssidListNum], 1, WL_SSID_MAX_LENGTH - 1);
    }

    // discard , character
    readUntil(1000, ",");

    _networkRssi[ssidListNum] = espSerial->parseInt();

    AT_LOGDEBUG3(F("_networkRssi["), ssidListNum, F(" ] ="), _networkRssi[ssidListNum]);

    idx = readUntil(1000, "+CWLAP:(");

    if (ssidListNum == WL_NETWORKS_LIST_MAXNUM - 1)
      break;

    ssidListNum++;
  }

  if (idx == -1)
    return -1;

  AT_LOGDEBUG1(F("---------------------------------------------- >"), ssidListNum);
  AT_LOGDEBUG();

  return ssidListNum;
}

////////////////////////////////////////

bool ESP8266_AT_Drv::getNetmask(IPAddress& mask)
{
  AT_LOGDEBUG(F("> getNetmask"));

  char buf[20];

  memset(buf, 0, sizeof(buf));

  bool ret;

  if (useESP32_AT)
  {
    // Get STA IP Info
    AT_LOGINFO(F("AT+CIPSTA?"));
    ret = sendCmdGet(F("AT+CIPSTA?"), F("+CIPSTA:netmask:\""), F("\""), buf, sizeof(buf));
  }
  else
  {
    // Get STA IP Info
    AT_LOGINFO(F("AT+CIPSTA_CUR?"));
    // Send sendCmdGet with _CUR => received also with _CUR
    ret = sendCmdGet(F("AT+CIPSTA_CUR?"), F("+CIPSTA_CUR:netmask:\""), F("\""), buf, sizeof(buf));
  }

  if (ret)
  {
    mask.fromString (buf);
    return true;
  }

  return false;
}

////////////////////////////////////////

bool ESP8266_AT_Drv::getGateway(IPAddress& gw)
{
  AT_LOGDEBUG(F("> getGateway"));

  char buf[20];

  memset(buf, 0, sizeof(buf));

  bool ret;

  if (useESP32_AT)
  {
    // Get STA IP Info
    AT_LOGINFO(F("AT+CIPSTA?"));
    ret = sendCmdGet(F("AT+CIPSTA?"), F("+CIPSTA:gateway:\""), F("\""), buf, sizeof(buf));
  }
  else
  {
    // Get STA IP Info
    AT_LOGINFO(F("AT+CIPSTA_CUR?"));

    // KH, fix bug, v1.1.0
    // Send sendCmdGet with _CUR => received also with _CUR
    ret = sendCmdGet(F("AT+CIPSTA_CUR?"), F("+CIPSTA_CUR:gateway:\""), F("\""), buf, sizeof(buf));
    //////
  }

  if (ret)
  {
    gw.fromString (buf);
    AT_LOGINFO1(F("Gateway IP = "), buf);
    return true;
  }

  return false;
}

////////////////////////////////////////

char* ESP8266_AT_Drv::getSSIDNetoworks(uint8_t networkItem)
{
  if (networkItem >= WL_NETWORKS_LIST_MAXNUM)
    return NULL;

  return _networkSsid[networkItem];
}

////////////////////////////////////////

uint8_t ESP8266_AT_Drv::getEncTypeNetowrks(uint8_t networkItem)
{
  if (networkItem >= WL_NETWORKS_LIST_MAXNUM)
    return 0;

  return _networkEncr[networkItem];
}

////////////////////////////////////////

int32_t ESP8266_AT_Drv::getRSSINetoworks(uint8_t networkItem)
{
  if (networkItem >= WL_NETWORKS_LIST_MAXNUM)
    return 0;

  return _networkRssi[networkItem];
}

////////////////////////////////////////

char* ESP8266_AT_Drv::getFwVersion()
{
  AT_LOGDEBUG(F("> getFwVersion"));

  fwVersion[0] = 0;

  AT_LOGINFO(F("AT+GMR"));
  sendCmdGet(F("AT+GMR"), F("SDK version:"), F("\r\n"), fwVersion, sizeof(fwVersion));

  return fwVersion;
}

////////////////////////////////////////

bool ESP8266_AT_Drv::ping(const char *host)
{
  AT_LOGDEBUG(F("> ping"));

  AT_LOGINFO1(F("AT+PING="), host);

  int ret = sendCmd(F("AT+PING=\"%s\""), 8000, host);

  if (ret == TAG_OK)
    return true;

  return false;
}

////////////////////////////////////////

// Start server TCP on port specified
bool ESP8266_AT_Drv::startServer(uint16_t port, uint8_t sock)
{
  AT_LOGDEBUG1(F("> startServer"), port);

  AT_LOGINFO2(F("AT+CIPSERVER="), sock, port);

  int ret = sendCmd(F("AT+CIPSERVER=%d,%d"), 1000, sock, port);

  return ret == TAG_OK;
}

////////////////////////////////////////

bool ESP8266_AT_Drv::startClient(const char* host, uint16_t port, uint8_t sock, uint8_t protMode)
{
  AT_LOGDEBUG2(F("> startClient"), host, port);

  // TCP
  // AT+CIPSTART=<link ID>,"TCP",<remote IP>,<remote port>

  // UDP
  // AT+CIPSTART=<link ID>,"UDP",<remote IP>,<remote port>[,<UDP local port>,<UDP mode>]

  // for UDP we set a dummy remote port and UDP mode to 2
  // this allows to specify the target host/port in CIPSEND


  int ret = -1;

  if (protMode == TCP_MODE)
  {
    AT_LOGINFO2(F("TCP => AT+CIPSTART="), sock, host);
    AT_LOGINFO1(F("TCP => AT+CIPSTART="), port);

    ret = sendCmd(F("AT+CIPSTART=%d,\"TCP\",\"%s\",%u"), 5000, sock, host, port);
  }
  else if (protMode == SSL_MODE)
  {
    // better to put the CIPSSLSIZE here because it is not supported before firmware 1.4
    if (!useESP32_AT)
    {
      // Set SSL Buffer to 4K, only for ESP8266
      AT_LOGINFO(F("AT+CIPSSLSIZE=4096"));

      sendCmd(F("AT+CIPSSLSIZE=4096"));
    }

    AT_LOGINFO2(F("SSL => AT+CIPSTART="), sock, host);
    AT_LOGINFO1(F("SSL => AT+CIPSTART="), port);

    ret = sendCmd(F("AT+CIPSTART=%d,\"SSL\",\"%s\",%u"), 5000, sock, host, port);
  }
  else if (protMode == UDP_MODE)
  {
    AT_LOGINFO2(F("UDP => AT+CIPSTART="), sock, host);
    AT_LOGINFO1(F("UDP => AT+CIPSTART="), port);
    ret = sendCmd(F("AT+CIPSTART=%d,\"UDP\",\"%s\",0,%u,2"), 5000, sock, host, port);
  }
  // KH, add to support MultiCast for v1.1.0
  else if (protMode == UDP_MULTICAST_MODE)
  {
    AT_LOGINFO2(F("UDP => AT+CIPSTART="), sock, host);
    AT_LOGINFO1(F("UDP => AT+CIPSTART="), port);

    ret = sendCmd(F("AT+CIPSTART=%d,\"UDP\",\"%s\",0,%u,0"), 5000, sock, host, port);
  }

  //////

  return ret == TAG_OK;
}

////////////////////////////////////////

// Start server TCP on port specified
void ESP8266_AT_Drv::stopClient(uint8_t sock)
{
  AT_LOGDEBUG1(F("> stopClient"), sock);

  AT_LOGINFO1(F("AT+CIPCLOSE="), sock);

  sendCmd(F("AT+CIPCLOSE=%d"), 4000, sock);
}

////////////////////////////////////////

uint8_t ESP8266_AT_Drv::getServerState(uint8_t sock)
{
  ESP_AT_UNUSED(sock);

  return 0;
}

////////////////////////////////////////////////////////////////////////////
// TCP/IP functions
////////////////////////////////////////////////////////////////////////////

uint16_t ESP8266_AT_Drv::availData(uint8_t connId)
{
  // if there is data in the buffer
  if (_bufPos > 0)
  {
    // KH
    //AT_LOGDEBUG1(F("availData: _bufPos="), _bufPos);
    //AT_LOGDEBUG1(_connId, connId);

    if (_connId == connId)
      return _bufPos;
    else if (_connId == 0)
      return _bufPos;
  }

  int bytes = espSerial->available();

  // KH
  //AT_LOGERROR1(F("availData: bytes="), bytes);

  if (bytes)
  {
    // KH
    //AT_LOGERROR1(F("Bytes in the serial buffer: "), bytes);

    if (espSerial->find((char *)"+IPD,"))
    {
      // format is : +IPD,<id>,<len>:<data>
      // format is : +IPD,<ID>,<len>[,<remote IP>,<remote port>]:<data>

      _connId = espSerial->parseInt();    // <ID>
      espSerial->read();                  // ,
      _bufPos = espSerial->parseInt();    // <len>
      espSerial->read();                  // "
      _remoteIp[0] = espSerial->parseInt();    // <remote IP>
      espSerial->read();                  // .
      _remoteIp[1] = espSerial->parseInt();
      espSerial->read();                  // .
      _remoteIp[2] = espSerial->parseInt();
      espSerial->read();                  // .
      _remoteIp[3] = espSerial->parseInt();
      espSerial->read();                  // "
      espSerial->read();                  // ,
      _remotePort = espSerial->parseInt();     // <remote port>

      espSerial->read();                  // :

      AT_LOGDEBUG();
      AT_LOGDEBUG2(F("Data packet"), _connId, _bufPos);

      if (_connId == connId || connId == 0)
        return _bufPos;
    }
  }

  return 0;
}

////////////////////////////////////////

bool ESP8266_AT_Drv::getData(uint8_t connId, uint8_t *data, bool peek, bool* connClose)
{
  if (connId != _connId)
    return false;

  // see Serial.timedRead

  long _startMillis = millis();

  do
  {
    if (espSerial->available())
    {
      if (peek)
      {
        *data = (char)espSerial->peek();
      }
      else
      {
        *data = (char)espSerial->read();
        _bufPos--;
      }

      //AT_LOGDEBUG1(F("Data at location = 0x"), String( (char)*data, HEX));
      //AT_LOGERROR3(F("Data at location = "), _bufPos + 1, ", data =", (char)*data );

      if (_bufPos == 0)
      {
        // after the data packet a ",CLOSED" string may be received
        // this means that the socket is now closed

        delay(5);

        if (espSerial->available())
        {
          //AT_LOGDEBUG(F(".2"));
          //AT_LOGDEBUG(espSerial->peek());

          // 48 = '0'
          if (espSerial->peek() == 48 + connId)
          {
            int idx = readUntil(500, ",CLOSED\r\n", false);

            if (idx != NUMESPTAGS)
            {
              AT_LOGERROR(F("Tag CLOSED not found"));
            }

            AT_LOGDEBUG();
            AT_LOGDEBUG(F("Connection closed"));

            *connClose = true;
          }
        }
      }

      return true;
    }
  } while (millis() - _startMillis < 500 /*2000*/);

  // timed out, reset the buffer
  AT_LOGDEBUG1(F("ESP8266_AT_Drv::getData: TIMEOUT @ bufPos ="), _bufPos);
  AT_LOGDEBUG1(F("Data at bad location = 0x"), String( (char)*data, HEX));

  _bufPos = 0;
  _connId = 0;
  *data = 0;

  return false;
}

////////////////////////////////////////

/**
   Receive the data into a buffer.
   It reads up to bufSize bytes.
   @return  received data size for success else -1.
*/
int ESP8266_AT_Drv::getDataBuf(uint8_t connId, uint8_t *buf, uint16_t bufSize)
{
  if (connId != _connId)
    return false;

  if (_bufPos < bufSize)
    bufSize = _bufPos;

  for (uint16_t i = 0; i < bufSize; i++)
  {
    int c = timedRead();
    //AT_LOGDEBUG(c);

    if (c == -1)
      return -1;

    buf[i] = (char)c;
    _bufPos--;
  }

  // KH
  AT_LOGDEBUG2(F("> getDataBuf:"), bufSize, (char*)buf);

  return bufSize;
}

////////////////////////////////////////

bool ESP8266_AT_Drv::sendData(uint8_t sock, const uint8_t *data, uint16_t len)
{
  AT_LOGDEBUG1(F("AT_Drv::sendData1: socket ="), sock);
  AT_LOGDEBUG1(F("AT_Drv::sendData1: len ="), len);
  AT_LOGDEBUG1(F("AT_Drv::sendData1: data ="), (char*) data);

  char cmdBuf[24];

  // KH, Restore PROGMEM commands
  sprintf_P(cmdBuf, PSTR("AT+CIPSEND=%d,%u"), sock, len);

  espSerial->println(cmdBuf);

  int idx = readUntil(1000, (char *)">", false);

  if (idx != NUMESPTAGS)
  {
    AT_LOGDEBUG(F("Data packet send error (1)"));
    return false;
  }

  espSerial->write(data, len);

  idx = readUntil(2000);

  AT_LOGDEBUG1(F("AT_Drv::sendData1: idx ="), idx);

  if (idx != TAG_SENDOK)
  {
    AT_LOGDEBUG(F("Data packet send error (2)"));
    // KH test comment out
    //return false;
  }

  return true;
}

////////////////////////////////////////

// Overrided sendData method for __FlashStringHelper strings
bool ESP8266_AT_Drv::sendData(uint8_t sock, const __FlashStringHelper *data, uint16_t len, bool appendCrLf)
{
  AT_LOGDEBUG2(F("> sendData2:"), sock, len);

  char cmdBuf[24];
  uint16_t len2 = len + 2 * appendCrLf;

  // KH, Restore PROGMEM commands
  sprintf_P(cmdBuf, PSTR("AT+CIPSEND=%d,%u"), sock, len2);

  espSerial->println(cmdBuf);

  int idx = readUntil(1000, (char *)">", false);

  if (idx != NUMESPTAGS)
  {
    AT_LOGDEBUG(F("Data packet send error (1)"));

    return false;
  }

  //espSerial->write(data, len);
  PGM_P p = reinterpret_cast<PGM_P>(data);

  for (uint16_t i = 0; i < len; i++)
  {
    unsigned char c = pgm_read_byte(p++);
    espSerial->write(c);
  }

  if (appendCrLf)
  {
    espSerial->write('\r');
    espSerial->write('\n');
  }

  idx = readUntil(2000);

  if (idx != TAG_SENDOK)
  {
    AT_LOGDEBUG(F("Data packet send error (2)"));
    return false;

  }

  return true;
}

////////////////////////////////////////

bool ESP8266_AT_Drv::sendDataUdp(uint8_t sock, const char* host, uint16_t port, const uint8_t *data, uint16_t len)
{
  AT_LOGDEBUG2(F("> sendDataUdp:"), sock, len);
  AT_LOGDEBUG2(F("> sendDataUdp:"), host, port);

  char cmdBuf[48];

  // KH, Restore PROGMEM commands
  sprintf_P(cmdBuf, PSTR("AT+CIPSEND=%d,%u,\"%s\",%u"), sock, len, host, port);

  //AT_LOGDEBUG1(F("> sendDataUdp:"), cmdBuf);
  espSerial->println(cmdBuf);

  int idx = readUntil(1000, (char *)">", false);

  if (idx != NUMESPTAGS)
  {
    AT_LOGDEBUG(F("Data packet send error (1)"));

    return false;
  }

  espSerial->write(data, len);

  idx = readUntil(2000);

  if (idx != TAG_SENDOK)
  {
    AT_LOGDEBUG(F("Data packet send error (2)"));

    return false;
  }

  return true;
}

////////////////////////////////////////

void ESP8266_AT_Drv::getRemoteIpAddress(IPAddress& ip)
{
  ip = _remoteIp;
}

////////////////////////////////////////

uint16_t ESP8266_AT_Drv::getRemotePort()
{
  return _remotePort;
}

////////////////////////////////////////////////////////////////////////////
// Utility functions for String
////////////////////////////////////////////////////////////////////////////

/*
  Sends the AT command and stops if any of the TAGS is found.
  Extract the string enclosed in the passed tags and returns it in the outStr buffer.
  Returns true if the string is extracted, false if tags are not found of timed out.
*/
bool ESP8266_AT_Drv::sendCmdGet(const char* cmd, const char* startTag, const char* endTag, char* outStr, int outStrLen)
{
  int idx;
  bool ret = false;

  outStr[0] = 0;

  espEmptyBuf();

  AT_LOGDEBUG(F("----------------------------------------------"));
  AT_LOGDEBUG1(F(">>"), cmd);

  // send AT command to ESP
  espSerial->println(cmd);

  // read result until the startTag is found
  idx = readUntil(1000, startTag);

  if (idx == NUMESPTAGS)
  {
    // clean the buffer to get a clean string
    ringBuf.init();

    // start tag found, search the endTag
    idx = readUntil(500, endTag);

    if (idx == NUMESPTAGS)
    {
      // end tag found
      // copy result to output buffer avoiding overflow
      ringBuf.getStrN(outStr, strlen(endTag), outStrLen - 1);

      // read the remaining part of the response
      readUntil(2000);

      ret = true;
    }
    else
    {
      AT_LOGWARN(F("End tag not found"));
    }
  }
  else if (idx >= 0 and idx < NUMESPTAGS)
  {
    // the command has returned but no start tag is found
    AT_LOGDEBUG1(F("No start tag found:"), idx);
  }
  else
  {
    // the command has returned but no tag is found
    AT_LOGWARN(F("No tag found"));
  }

  AT_LOGDEBUG1(F("---------------------------------------------- >"), outStr);
  AT_LOGDEBUG();

  return ret;
}

////////////////////////////////////////

#if 0
bool ESP8266_AT_Drv::sendCmdGet(const char* cmd, const char* startTag, const char* endTag, char* outStr, int outStrLen)
{
  char _startTag[strlen((char*)startTag) + 1];
  strcpy(_startTag,  (char*)startTag);

  char _endTag[strlen((char*)endTag) + 1];
  strcpy(_endTag,  (char*)endTag);

  return sendCmdGet(cmd, _startTag, _endTag, outStr, outStrLen);
}
#endif

////////////////////////////////////////

/*
  Sends the AT command and returns the id of the TAG.
  Return -1 if no tag is found.
*/
int ESP8266_AT_Drv::sendCmd(const char* cmd, int timeout)
{
  espEmptyBuf();

  AT_LOGDEBUG(F("----------------------------------------------"));
  AT_LOGDEBUG1(F(">>"), cmd);

  espSerial->println(cmd);

  int idx = readUntil(timeout);

  AT_LOGDEBUG1(F("---------------------------------------------- >"), idx);
  AT_LOGDEBUG();

  return idx;
}

////////////////////////////////////////

/*
  Sends the AT command and returns the id of the TAG.
  The additional arguments are formatted into the command using sprintf.
  Return -1 if no tag is found.
*/
int ESP8266_AT_Drv::sendCmd(const char* cmd, int timeout, ...)
{
  char cmdBuf[CMD_BUFFER_SIZE];

  va_list args;
  va_start (args, timeout);

  vsnprintf (cmdBuf, CMD_BUFFER_SIZE, (char*)cmd, args);

  va_end (args);

  espEmptyBuf();

  AT_LOGDEBUG(F("----------------------------------------------"));
  AT_LOGDEBUG1(F(">>"), cmdBuf);

  espSerial->println(cmdBuf);

  int idx = readUntil(timeout);

  AT_LOGDEBUG1(F("---------------------------------------------- >"), idx);
  AT_LOGDEBUG();

  return idx;
}

// KH, Restore PROGMEM commands

////////////////////////////////////////////////////////////////////////////
// Utility functions for __FlashStringHelper
////////////////////////////////////////////////////////////////////////////

/*
  Sends the AT command and stops if any of the TAGS is found.
  Extract the string enclosed in the passed tags and returns it in the outStr buffer.
  Returns true if the string is extracted, false if tags are not found of timed out.
*/
bool ESP8266_AT_Drv::sendCmdGet(const __FlashStringHelper* cmd, const char* startTag, const char* endTag, char* outStr,
                                int outStrLen)
{
  int idx;
  bool ret = false;

  outStr[0] = 0;

  espEmptyBuf();

  AT_LOGDEBUG(F("----------------------------------------------"));
  AT_LOGDEBUG1(F(">>"), cmd);

  // send AT command to ESP
  espSerial->println(cmd);

  // read result until the startTag is found
  idx = readUntil(1000, startTag);

  if (idx == NUMESPTAGS)
  {
    // clean the buffer to get a clean string
    ringBuf.init();

    // start tag found, search the endTag
    idx = readUntil(500, endTag);

    if (idx == NUMESPTAGS)
    {
      // end tag found
      // copy result to output buffer avoiding overflow
      ringBuf.getStrN(outStr, strlen(endTag), outStrLen - 1);

      // read the remaining part of the response
      readUntil(2000);

      ret = true;
    }
    else
    {
      AT_LOGWARN(F("End tag not found"));
    }
  }
  else if (idx >= 0 and idx < NUMESPTAGS)
  {
    // the command has returned but no start tag is found
    AT_LOGDEBUG1(F("No start tag found:"), idx);
  }
  else
  {
    // the command has returned but no tag is found
    AT_LOGWARN(F("No tag found"));
  }

  AT_LOGDEBUG1(F("---------------------------------------------- >"), outStr);
  AT_LOGDEBUG();

  return ret;
}

////////////////////////////////////////

bool ESP8266_AT_Drv::sendCmdGet(const __FlashStringHelper* cmd, const __FlashStringHelper* startTag,
                                const __FlashStringHelper* endTag, char* outStr, int outStrLen)
{
  char _startTag[strlen_P((char*)startTag) + 1];
  strcpy_P(_startTag,  (char*)startTag);

  char _endTag[strlen_P((char*)endTag) + 1];
  strcpy_P(_endTag,  (char*)endTag);

  return sendCmdGet(cmd, _startTag, _endTag, outStr, outStrLen);
}

////////////////////////////////////////

/*
  Sends the AT command and returns the id of the TAG.
  Return -1 if no tag is found.
*/
int ESP8266_AT_Drv::sendCmd(const __FlashStringHelper* cmd, int timeout)
{
  espEmptyBuf();

  AT_LOGDEBUG(F("----------------------------------------------"));
  AT_LOGDEBUG1(F(">>"), cmd);

  espSerial->println(cmd);

  int idx = readUntil(timeout);

  AT_LOGDEBUG1(F("---------------------------------------------- >"), idx);
  AT_LOGDEBUG();

  return idx;
}

////////////////////////////////////////

/*
  Sends the AT command and returns the id of the TAG.
  The additional arguments are formatted into the command using sprintf.
  Return -1 if no tag is found.
*/
int ESP8266_AT_Drv::sendCmd(const __FlashStringHelper* cmd, int timeout, ...)
{
  char cmdBuf[CMD_BUFFER_SIZE];

  va_list args;
  va_start (args, timeout);

  vsnprintf_P (cmdBuf, CMD_BUFFER_SIZE, (char*)cmd, args);

  va_end (args);

  espEmptyBuf();

  AT_LOGDEBUG(F("----------------------------------------------"));
  AT_LOGDEBUG1(F(">>"), cmdBuf);

  espSerial->println(cmdBuf);

  int idx = readUntil(timeout);

  AT_LOGDEBUG1(F("---------------------------------------------- >"), idx);
  AT_LOGDEBUG();

  return idx;
}

////////////////////////////////////////

// Read from serial until one of the tags is found
// Returns:
//   the index of the tag found in the ESPTAGS array
//   -1 if no tag was found (timeout)
int ESP8266_AT_Drv::readUntil(unsigned int timeout, const char* tag, bool findTags)
{
  ringBuf.reset();

  char c;
  unsigned long start = millis();
  int ret = -1;

  //AT_LOGERROR1(F("AT_Drv::readUntil, millis ="), start);

  while ((millis() - start < timeout) && (ret < 0) )
  {
    if (espSerial->available())
    {
      c = (char)espSerial->read();

      AT_LOGDEBUG0(c);

      ringBuf.push(c);

      if (tag != NULL)
      {
        if (ringBuf.endsWith(tag))
        {
          ret = NUMESPTAGS;
        }
      }

      if (findTags)
      {
        for (int i = 0; i < NUMESPTAGS; i++)
        {
          if (ringBuf.endsWith(ESPTAGS[i]))
          {
            ret = i;
            break;
          }
        }
      }
    }
  }

  if (millis() - start >= timeout)
  {
    AT_LOGWARN(F(">>> TIMEOUT >>>"));
  }

  return ret;
}

////////////////////////////////////////

void ESP8266_AT_Drv::espEmptyBuf(bool warn)
{
  char c;
  int i = 0;

  while (espSerial->available() > 0)
  {
    c = espSerial->read();

    if (i > 0 and warn == true)
      AT_LOGDEBUG0(c);

    i++;
  }

  if (i > 0 and warn == true)
  {
    AT_LOGDEBUG(F(""));
    AT_LOGDEBUG1(F("Dirty characters in the serial buffer! >"), i);
  }
}

////////////////////////////////////////

// copied from Serial::timedRead
int ESP8266_AT_Drv::timedRead()
{
  unsigned int _timeout = 1000;
  int c;
  long _startMillis = millis();

  //AT_LOGDEBUG(F("ESP8266_AT_Drv::timedRead"));

  do
  {
    c = espSerial->read();

    if (c >= 0)
      return c;
  } while (millis() - _startMillis < _timeout);

  return -1; // -1 indicates timeout
}

////////////////////////////////////////

ESP8266_AT_Drv esp8266_AT_Drv;

////////////////////////////////////////

