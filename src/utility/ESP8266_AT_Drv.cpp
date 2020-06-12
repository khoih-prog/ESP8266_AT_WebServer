/****************************************************************************************************************************
   ESP8266_AT_Drv.cpp - Dead simple web-server.
   For ESP8266 AT-command running shields

   ESP8266_AT_WebServer is a library for the ESP8266 AT-command shields to run WebServer
   Based on and modified from ESP8266 https://github.com/esp8266/Arduino/releases
   Built by Khoi Hoang https://github.com/khoih-prog/ESP8266_AT_WebServer
   Licensed under MIT license
   Version: 1.0.6

   Version Modified By   Date      Comments
   ------- -----------  ---------- -----------
    1.0.0   K Hoang      12/02/2020 Initial coding for Arduino Mega, Teensy, etc
    1.0.1   K Hoang      17/02/2020 Add support to server's lambda function calls
    1.0.2   K Hoang      22/02/2020 Add support to SAMD (DUE, ZERO, MKR, NANO_33_IOT, M0, M0 Pro, AdaFruit, etc) boards
    1.0.3   K Hoang      03/03/2020 Add support to STM32 (STM32,F0,F1, F2, F3, F4, F7, etc) boards
    1.0.4   K Hoang      19/03/2020 Fix bug. Sync with ESP8266WebServer library of core v2.6.3
    1.0.5   K Hoang      17/04/2020 Add support to SAMD51 and SAM DUE boards
    1.0.6   K Hoang      11/06/2020 Add support to nRF52 boards, such as AdaFruit Feather nRF52832, nRF52840 Express, BlueFruit Sense, 
                                    Itsy-Bitsy nRF52840 Express, Metro nRF52840 Express, NINA_B30_ublox, etc. 
 *****************************************************************************************************************************/

#include <Arduino.h>
#include <avr/pgmspace.h>

#if    ( defined(ARDUINO_SAMD_ZERO) || defined(ARDUINO_SAMD_MKR1000) || defined(ARDUINO_SAMD_MKRWIFI1010) \
      || defined(ARDUINO_SAMD_NANO_33_IOT) || defined(ARDUINO_SAMD_MKRFox1200) || defined(ARDUINO_SAMD_MKRWAN1300) || defined(ARDUINO_SAMD_MKRWAN1310) \
      || defined(ARDUINO_SAMD_MKRGSM1400) || defined(ARDUINO_SAMD_MKRNB1500) || defined(ARDUINO_SAMD_MKRVIDOR4000) || defined(__SAMD21G18A__) \
      || defined(ARDUINO_SAMD_CIRCUITPLAYGROUND_EXPRESS) || defined(__SAMD21E18A__) || defined(__SAMD51__) || defined(__SAMD51J20A__) || defined(__SAMD51J19A__) \
      || defined(__SAMD51G19A__) || defined(__SAMD21G18A__) )
#if defined(ESP8266_AT_USE_SAMD)
#undef ESP8266_AT_USE_SAMD
#endif
#define ESP8266_AT_USE_SAMD      true
#warning Use SAMD architecture from ESP8266_AT_Drv_h
#endif

#if ( defined(NRF52840_FEATHER) || defined(NRF52832_FEATHER) || defined(NRF52_SERIES) || defined(ARDUINO_NRF52_ADAFRUIT) || \
      defined(NRF52840_FEATHER_SENSE) || defined(NRF52840_ITSYBITSY) || defined(NRF52840_CIRCUITPLAY) || defined(NRF52840_CLUE) || \
      defined(NRF52840_METRO) || defined(NRF52840_PCA10056) || defined(PARTICLE_XENON) || defined(NINA_B302_ublox) || defined(NINA_B112_ublox) )
#if defined(ESP8266_AT_USE_NRF528XX)
#undef ESP8266_AT_USE_NRF528XX
#endif
#define ESP8266_AT_USE_NRF528XX      true
#warning Use nFR52 architecture from ESP8266_AT_WebServer
#endif

#if ( defined(ARDUINO_SAM_DUE) || defined(__SAM3X8E__) )
#if defined(ESP8266_AT_USE_SAM_DUE)
#undef ESP8266_AT_USE_SAM_DUE
#endif
#define ESP8266_AT_USE_SAM_DUE      true
#endif

#if (ESP8266_AT_USE_SAMD || ESP8266_AT_USE_SAM_DUE || ESP8266_AT_USE_NRF528XX)
#include <cstdarg>
#define vsnprintf_P vsnprintf
#endif

#include "utility/ESP8266_AT_Drv.h"
#include "utility/ESP8266_AT_Debug.h"


#define NUMESPTAGS 5

const char* ESPTAGS[] =
{
  "\r\nOK\r\n",
  "\r\nERROR\r\n",
  "\r\nFAIL\r\n",
  "\r\nSEND OK\r\n",
  " CONNECT\r\n"
};

typedef enum
{
  TAG_OK,
  TAG_ERROR,
  TAG_FAIL,
  TAG_SENDOK,
  TAG_CONNECT
} TagsEnum;


Stream *ESP8266_AT_Drv::espSerial;

#ifdef CORE_TEENSY
//AT_RingBuffer ESP8266_AT_Drv::ringBuf(4096);
AT_RingBuffer ESP8266_AT_Drv::ringBuf(512);
//AT_RingBuffer ESP8266_AT_Drv::ringBuf(256);
#else
//AT_RingBuffer ESP8266_AT_Drv::ringBuf(32);
//AT_RingBuffer ESP8266_AT_Drv::ringBuf(256);
AT_RingBuffer ESP8266_AT_Drv::ringBuf(512);
#endif

// Array of data to cache the information related to the networks discovered
char 	  ESP8266_AT_Drv::_networkSsid[][WL_SSID_MAX_LENGTH]    = {{"1"}, {"2"}, {"3"}, {"4"}, {"5"}};
int32_t ESP8266_AT_Drv::_networkRssi[WL_NETWORKS_LIST_MAXNUM] = { 0 };
uint8_t ESP8266_AT_Drv::_networkEncr[WL_NETWORKS_LIST_MAXNUM] = { 0 };

// Cached values of retrieved data
char    ESP8266_AT_Drv::_ssid[] = {0};
uint8_t ESP8266_AT_Drv::_bssid[] = {0};
uint8_t ESP8266_AT_Drv::_mac[] = {0};
uint8_t ESP8266_AT_Drv::_localIp[] = {0};
char    ESP8266_AT_Drv::fwVersion[] = {0};

long    ESP8266_AT_Drv::_bufPos = 0;
uint8_t ESP8266_AT_Drv::_connId = 0;

uint16_t  ESP8266_AT_Drv::_remotePort  = 0;
uint8_t   ESP8266_AT_Drv::_remoteIp[] = {0};


void ESP8266_AT_Drv::wifiDriverInit(Stream *espSerial)
{
  LOGDEBUG(F("> wifiDriverInit"));

  ESP8266_AT_Drv::espSerial = espSerial;

  bool initOK = false;

  for (int i = 0; i < 5; i++)
  {
    if (sendCmd("AT") == TAG_OK)
    {
      initOK = true;
      break;
    }
    delay(1000);
  }

  if (!initOK)
  {
    LOGERROR(F("Cannot initialize ESP module"));
    delay(5000);
    return;
  }

  reset();

  // check firmware version
  getFwVersion();

  // prints a warning message if the firmware is not 1.X or 2.X
  if ((fwVersion[0] != '1' and fwVersion[0] != '2') or
      fwVersion[1] != '.')
  {
    LOGWARN1(F("Warning: Unsupported firmware"), fwVersion);
    delay(4000);
  }
  else
  {
    LOGINFO1(F("Initilization successful -"), fwVersion);
  }
}

void ESP8266_AT_Drv::reset()
{
  LOGDEBUG(F("> reset"));

  sendCmd("AT+RST");
  delay(3000);
  espEmptyBuf(false);  // empty dirty characters from the buffer

  // disable echo of commands
  sendCmd("ATE0");

  // set station mode
  sendCmd("AT+CWMODE=1");
  delay(200);

  // set multiple connections mode
  sendCmd("AT+CIPMUX=1");

  // Show remote IP and port with "+IPD"
  sendCmd("AT+CIPDINFO=1");

  // Disable autoconnect
  // Automatic connection can create problems during initialization phase at next boot
  sendCmd("AT+CWAUTOCONN=0");

  // enable DHCP
  sendCmd("AT+CWDHCP=1,1");
  delay(200);
}

bool ESP8266_AT_Drv::wifiConnect(const char* ssid, const char* passphrase)
{
  LOGDEBUG(F("> wifiConnect"));

  // TODO
  // Escape character syntax is needed if "SSID" or "password" contains
  // any special characters (',', '"' and '/')

  // connect to access point, use CUR mode to avoid connection at boot
  int ret = sendCmd("AT+CWJAP_CUR=\"%s\",\"%s\"", 20000, ssid, passphrase);

  if (ret == TAG_OK)
  {
    LOGINFO1(F("Connected to"), ssid);
    return true;
  }

  LOGWARN1(F("Failed connecting to"), ssid);

  // clean additional messages logged after the FAIL tag
  delay(1000);
  espEmptyBuf(false);

  return false;
}

bool ESP8266_AT_Drv::wifiStartAP(const char* ssid, const char* pwd, uint8_t channel, uint8_t enc, uint8_t espMode)
{
  LOGDEBUG(F("> wifiStartAP"));

  // set AP mode, use CUR mode to avoid automatic start at boot
  int ret = sendCmd("AT+CWMODE_CUR=%d", 10000, espMode);
  if (ret != TAG_OK)
  {
    LOGWARN1(F("Failed to set AP mode"), ssid);
    return false;
  }

  // TODO
  // Escape character syntax is needed if "SSID" or "password" contains
  // any special characters (',', '"' and '/')

  // start access point
  ret = sendCmd("AT+CWSAP_CUR=\"%s\",\"%s\",%d,%d", 10000, ssid, pwd, channel, enc);

  if (ret != TAG_OK)
  {
    LOGWARN1(F("Failed to start AP"), ssid);
    return false;
  }

  if (espMode == 2)
    sendCmd("AT+CWDHCP_CUR=0,1");    // enable DHCP for AP mode
  if (espMode == 3)
    sendCmd("AT+CWDHCP_CUR=2,1");    // enable DHCP for station and AP mode

  LOGINFO1(F("Access point started"), ssid);
  return true;
}

int8_t ESP8266_AT_Drv::disconnect()
{
  LOGDEBUG(F("> disconnect"));

  if (sendCmd("AT+CWQAP") == TAG_OK)
    return WL_DISCONNECTED;

  // wait and clear any additional message
  delay(2000);
  espEmptyBuf(false);

  return WL_DISCONNECTED;
}

void ESP8266_AT_Drv::config(IPAddress ip)
{
  LOGDEBUG(F("> config"));

  // disable station DHCP
  sendCmd("AT+CWDHCP_CUR=1,0");

  // it seems we need to wait here...
  delay(500);

  char buf[16];
  //sprintf_P(buf, PSTR("%d.%d.%d.%d"), ip[0], ip[1], ip[2], ip[3]);
  sprintf(buf, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);

  int ret = sendCmd("AT+CIPSTA_CUR=\"%s\"", 2000, buf);
  delay(500);

  if (ret == TAG_OK)
  {
    LOGINFO1(F("IP address set"), buf);
  }
}

void ESP8266_AT_Drv::configAP(IPAddress ip)
{
  LOGDEBUG(F("> config"));

  sendCmd("AT+CWMODE_CUR=2");

  // disable station DHCP
  sendCmd("AT+CWDHCP_CUR=2,0");

  // it seems we need to wait here...
  delay(500);

  char buf[16];
  //sprintf_P(buf, PSTR("%d.%d.%d.%d"), ip[0], ip[1], ip[2], ip[3]);
  sprintf(buf, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);

  int ret = sendCmd("AT+CIPAP_CUR=\"%s\"", 2000, buf);
  delay(500);

  if (ret == TAG_OK)
  {
    LOGINFO1(F("IP address set"), buf);
  }
}

uint8_t ESP8266_AT_Drv::getConnectionStatus()
{
  LOGDEBUG(F("> getConnectionStatus"));

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
  if (!sendCmdGet("AT+CIPSTATUS", "STATUS:", "\r\n", buf, sizeof(buf)))
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

uint8_t ESP8266_AT_Drv::getClientState(uint8_t sock)
{
  LOGDEBUG1(F("> getClientState"), sock);

  char findBuf[20];
  //sprintf_P(findBuf, PSTR("+CIPSTATUS:%d,"), sock);
  sprintf(findBuf, "+CIPSTATUS:%d,", sock);

  char buf[10];
  if (sendCmdGet("AT+CIPSTATUS", findBuf, ",", buf, sizeof(buf)))
  {
    LOGDEBUG(F("Connected"));
    return true;
  }

  LOGDEBUG(F("Not connected"));
  return false;
}

uint8_t* ESP8266_AT_Drv::getMacAddress()
{
  LOGDEBUG(F("> getMacAddress"));

  memset(_mac, 0, WL_MAC_ADDR_LENGTH);

  char buf[20];
  if (sendCmdGet("AT+CIFSR", ":STAMAC,\"", "\"", buf, sizeof(buf)))
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

void ESP8266_AT_Drv::getIpAddress(IPAddress& ip)
{
  LOGDEBUG(F("> getIpAddress"));

  char buf[20];
  memset(buf, '\0', sizeof(buf));
  if (sendCmdGet("AT+CIFSR", ":STAIP,\"", "\"", buf, sizeof(buf)))
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

void ESP8266_AT_Drv::getIpAddressAP(IPAddress& ip)
{
  LOGDEBUG(F("> getIpAddressAP"));

  char buf[20];
  memset(buf, '\0', sizeof(buf));
  if (sendCmdGet("AT+CIPAP?", "+CIPAP:ip:\"", "\"", buf, sizeof(buf)))
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

char* ESP8266_AT_Drv::getCurrentSSID()
{
  LOGDEBUG(F("> getCurrentSSID"));

  _ssid[0] = 0;
  sendCmdGet("AT+CWJAP?", "+CWJAP:\"", "\"", _ssid, sizeof(_ssid));

  return _ssid;
}

uint8_t* ESP8266_AT_Drv::getCurrentBSSID()
{
  LOGDEBUG(F("> getCurrentBSSID"));

  memset(_bssid, 0, WL_MAC_ADDR_LENGTH);

  char buf[20];
  if (sendCmdGet("AT+CWJAP?", ",\"", "\",", buf, sizeof(buf)))
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

int32_t ESP8266_AT_Drv::getCurrentRSSI()
{
  LOGDEBUG(F("> getCurrentRSSI"));

  int ret = 0;
  char buf[10];
  sendCmdGet("AT+CWJAP?", ",-", "\r\n", buf, sizeof(buf));

  if (isDigit(buf[0])) {
    ret = -atoi(buf);
  }

  return ret;
}

uint8_t ESP8266_AT_Drv::getScanNetworks()
{
  uint8_t ssidListNum = 0;
  int idx;

  espEmptyBuf();

  LOGDEBUG(F("----------------------------------------------"));
  LOGDEBUG(F(">> AT+CWLAP"));

  espSerial->println("AT+CWLAP");

  idx = readUntil(10000, "+CWLAP:(");
  
  //Serial.printf("idx = %d, NUMESPTAGS = %d\n", idx, NUMESPTAGS);

  while (idx == NUMESPTAGS)
  {
    _networkEncr[ssidListNum] = espSerial->parseInt();
    
    //Serial.printf("_networkEncr[%d] = %d\n", ssidListNum, _networkEncr[ssidListNum]);

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
    //Serial.printf("_networkEncr[%d] = %d\n", ssidListNum, _networkRssi[ssidListNum]);

    idx = readUntil(1000, "+CWLAP:(");

    if (ssidListNum == WL_NETWORKS_LIST_MAXNUM - 1)
      break;

    ssidListNum++;
  }

  if (idx == -1)
    return -1;

  LOGDEBUG1(F("---------------------------------------------- >"), ssidListNum);
  LOGDEBUG();
  return ssidListNum;
}

bool ESP8266_AT_Drv::getNetmask(IPAddress& mask) {
  LOGDEBUG(F("> getNetmask"));

  char buf[20];
  if (sendCmdGet("AT+CIPSTA?", "+CIPSTA:netmask:\"", "\"", buf, sizeof(buf)))
  {
    mask.fromString (buf);
    return true;
  }

  return false;
}

bool ESP8266_AT_Drv::getGateway(IPAddress& gw)
{
  LOGDEBUG(F("> getGateway"));

  char buf[20];
  if (sendCmdGet("AT+CIPSTA?", "+CIPSTA:gateway:\"", "\"", buf, sizeof(buf)))
  {
    gw.fromString (buf);
    return true;
  }

  return false;
}

char* ESP8266_AT_Drv::getSSIDNetoworks(uint8_t networkItem)
{
  if (networkItem >= WL_NETWORKS_LIST_MAXNUM)
    return NULL;

  return _networkSsid[networkItem];
}

uint8_t ESP8266_AT_Drv::getEncTypeNetowrks(uint8_t networkItem)
{
  if (networkItem >= WL_NETWORKS_LIST_MAXNUM)
    return 0;

  return _networkEncr[networkItem];
}

int32_t ESP8266_AT_Drv::getRSSINetoworks(uint8_t networkItem)
{
  if (networkItem >= WL_NETWORKS_LIST_MAXNUM)
    return 0;

  return _networkRssi[networkItem];
}

char* ESP8266_AT_Drv::getFwVersion()
{
  LOGDEBUG(F("> getFwVersion"));

  fwVersion[0] = 0;

  sendCmdGet("AT+GMR", "SDK version:", "\r\n", fwVersion, sizeof(fwVersion));

  return fwVersion;
}

bool ESP8266_AT_Drv::ping(const char *host)
{
  LOGDEBUG(F("> ping"));

  int ret = sendCmd("AT+PING=\"%s\"", 8000, host);

  if (ret == TAG_OK)
    return true;

  return false;
}

// Start server TCP on port specified
bool ESP8266_AT_Drv::startServer(uint16_t port, uint8_t sock)
{
  LOGDEBUG1(F("> startServer"), port);

  int ret = sendCmd("AT+CIPSERVER=%d,%d", 1000, sock, port);

  return ret == TAG_OK;
}

bool ESP8266_AT_Drv::startClient(const char* host, uint16_t port, uint8_t sock, uint8_t protMode)
{
  LOGDEBUG2(F("> startClient"), host, port);

  // TCP
  // AT+CIPSTART=<link ID>,"TCP",<remote IP>,<remote port>

  // UDP
  // AT+CIPSTART=<link ID>,"UDP",<remote IP>,<remote port>[,<UDP local port>,<UDP mode>]

  // for UDP we set a dummy remote port and UDP mode to 2
  // this allows to specify the target host/port in CIPSEND


  int ret = -1;
  if (protMode == TCP_MODE)
    ret = sendCmd("AT+CIPSTART=%d,\"TCP\",\"%s\",%u", 5000, sock, host, port);
  else if (protMode == SSL_MODE)
  {
    // better to put the CIPSSLSIZE here because it is not supported before firmware 1.4
    sendCmd("AT+CIPSSLSIZE=4096");
    ret = sendCmd("AT+CIPSTART=%d,\"SSL\",\"%s\",%u", 5000, sock, host, port);
  }
  else if (protMode == UDP_MODE)
    ret = sendCmd("AT+CIPSTART=%d,\"UDP\",\"%s\",0,%u,2", 5000, sock, host, port);

  return ret == TAG_OK;
}

// Start server TCP on port specified
void ESP8266_AT_Drv::stopClient(uint8_t sock)
{
  LOGDEBUG1(F("> stopClient"), sock);

  sendCmd("AT+CIPCLOSE=%d", 4000, sock);
}

uint8_t ESP8266_AT_Drv::getServerState(uint8_t sock)
{
  return 0;
}

////////////////////////////////////////////////////////////////////////////
// TCP/IP functions
////////////////////////////////////////////////////////////////////////////



uint16_t ESP8266_AT_Drv::availData(uint8_t connId)
{
  //LOGDEBUG(bufPos);

  // if there is data in the buffer
  if (_bufPos > 0)
  {
    if (_connId == connId)
      return _bufPos;
    else if (_connId == 0)
      return _bufPos;
  }

  int bytes = espSerial->available();

  if (bytes)
  {
    //LOGDEBUG1(F("Bytes in the serial buffer: "), bytes);
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

      LOGDEBUG();
      LOGDEBUG2(F("Data packet"), _connId, _bufPos);

      if (_connId == connId || connId == 0)
        return _bufPos;
    }
  }
  return 0;
}

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

      // KH
      //if ( _bufPos == 293 )
      //Serial.println((char)*data);


      if (_bufPos == 0)
      {
        // after the data packet a ",CLOSED" string may be received
        // this means that the socket is now closed

        delay(5);

        if (espSerial->available())
        {
          //LOGDEBUG(".2");
          //LOGDEBUG(espSerial->peek());

          // 48 = '0'
          if (espSerial->peek() == 48 + connId)
          {
            int idx = readUntil(500, ",CLOSED\r\n", false);
            if (idx != NUMESPTAGS)
            {
              LOGERROR(F("Tag CLOSED not found"));
            }

            LOGDEBUG();
            LOGDEBUG(F("Connection closed"));

            *connClose = true;
          }
        }
      }

      return true;
    }
  } while (millis() - _startMillis < 500 /*2000*/);

  // timed out, reset the buffer
  LOGERROR1(F("TIMEOUT:"), _bufPos);

  _bufPos = 0;
  _connId = 0;
  *data = 0;

  return false;
}

/**
   Receive the data into a buffer.
   It reads up to bufSize bytes.
   @return	received data size for success else -1.
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
    //LOGDEBUG(c);
    if (c == -1)
      return -1;

    buf[i] = (char)c;
    _bufPos--;
  }

  // KH
  LOGDEBUG2(F("> getDataBuf:"), bufSize, (char*)buf);

  return bufSize;
}

bool ESP8266_AT_Drv::sendData(uint8_t sock, const uint8_t *data, uint16_t len)
{
  //LOGERROR1(F("AT_Drv::sendData1: socket"), sock);
  //LOGERROR1(F("AT_Drv::sendData1: len"), len);
  //LOGERROR1(F("AT_Drv::sendData1: data"), (char*) data);

  char cmdBuf[20];
  //sprintf_P(cmdBuf, PSTR("AT+CIPSEND=%d,%u"), sock, len);
  sprintf(cmdBuf, "AT+CIPSEND=%d,%u", sock, len);
  espSerial->println(cmdBuf);

  int idx = readUntil(1000, (char *)">", false);
  if (idx != NUMESPTAGS)
  {
    //LOGERROR(F("Data packet send error (1)"));
    return false;
  }

  espSerial->write(data, len);

  idx = readUntil(2000);
  if (idx != TAG_SENDOK)
  {
    //LOGERROR(F("Data packet send error (2)"));
    return false;
  }

  return true;
}

// Overrided sendData method for __FlashStringHelper strings
bool ESP8266_AT_Drv::sendData(uint8_t sock, const __FlashStringHelper *data, uint16_t len, bool appendCrLf)
{
  LOGDEBUG2(F("> sendData2:"), sock, len);

  char cmdBuf[20];
  uint16_t len2 = len + 2 * appendCrLf;
  //sprintf_P(cmdBuf, PSTR("AT+CIPSEND=%d,%u"), sock, len2);
  sprintf(cmdBuf, "AT+CIPSEND=%d,%u", sock, len2);
  espSerial->println(cmdBuf);

  int idx = readUntil(1000, (char *)">", false);
  if (idx != NUMESPTAGS)
  {
    //LOGERROR(F("Data packet send error (1)"));
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
    //LOGERROR(F("Data packet send error (2)"));
    return false;
  }

  return true;
}

bool ESP8266_AT_Drv::sendDataUdp(uint8_t sock, const char* host, uint16_t port, const uint8_t *data, uint16_t len)
{
  LOGDEBUG2(F("> sendDataUdp:"), sock, len);
  LOGDEBUG2(F("> sendDataUdp:"), host, port);

  char cmdBuf[40];
  //sprintf_P(cmdBuf, PSTR("AT+CIPSEND=%d,%u,\"%s\",%u"), sock, len, host, port);
  sprintf(cmdBuf, "AT+CIPSEND=%d,%u,\"%s\",%u", sock, len, host, port);
  //LOGDEBUG1(F("> sendDataUdp:"), cmdBuf);
  espSerial->println(cmdBuf);

  int idx = readUntil(1000, (char *)">", false);
  if (idx != NUMESPTAGS)
  {
    //LOGERROR(F("Data packet send error (1)"));
    return false;
  }

  espSerial->write(data, len);

  idx = readUntil(2000);
  if (idx != TAG_SENDOK)
  {
    //LOGERROR(F("Data packet send error (2)"));
    return false;
  }

  return true;
}

void ESP8266_AT_Drv::getRemoteIpAddress(IPAddress& ip)
{
  ip = _remoteIp;
}

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

  LOGDEBUG(F("----------------------------------------------"));
  LOGDEBUG1(F(">>"), cmd);

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
      LOGWARN(F("End tag not found"));
    }
  }
  else if (idx >= 0 and idx < NUMESPTAGS)
  {
    // the command has returned but no start tag is found
    LOGDEBUG1(F("No start tag found:"), idx);
  }
  else
  {
    // the command has returned but no tag is found
    LOGWARN(F("No tag found"));
  }

  LOGDEBUG1(F("---------------------------------------------- >"), outStr);
  LOGDEBUG();

  return ret;
}

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

/*
  Sends the AT command and returns the id of the TAG.
  Return -1 if no tag is found.
*/
int ESP8266_AT_Drv::sendCmd(const char* cmd, int timeout)
{
  espEmptyBuf();

  LOGDEBUG(F("----------------------------------------------"));
  LOGDEBUG1(F(">>"), cmd);

  espSerial->println(cmd);

  int idx = readUntil(timeout);

  LOGDEBUG1(F("---------------------------------------------- >"), idx);
  LOGDEBUG();

  return idx;
}

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

  LOGDEBUG(F("----------------------------------------------"));
  LOGDEBUG1(F(">>"), cmdBuf);

  espSerial->println(cmdBuf);

  int idx = readUntil(timeout);

  LOGDEBUG1(F("---------------------------------------------- >"), idx);
  LOGDEBUG();

  return idx;
}

#if !( defined(CORE_TEENSY) || (ESP8266_AT_USE_SAMD) || (ESP8266_AT_USE_STM32) || (ESP8266_AT_USE_SAM_DUE) || (ESP8266_AT_USE_NRF528XX) )
////////////////////////////////////////////////////////////////////////////
// Utility functions for __FlashStringHelper
////////////////////////////////////////////////////////////////////////////

/*
  Sends the AT command and stops if any of the TAGS is found.
  Extract the string enclosed in the passed tags and returns it in the outStr buffer.
  Returns true if the string is extracted, false if tags are not found of timed out.
*/
bool ESP8266_AT_Drv::sendCmdGet(const __FlashStringHelper* cmd, const char* startTag, const char* endTag, char* outStr, int outStrLen)
{
  int idx;
  bool ret = false;

  outStr[0] = 0;

  espEmptyBuf();

  LOGDEBUG(F("----------------------------------------------"));
  LOGDEBUG1(F(">>"), cmd);

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
      LOGWARN(F("End tag not found"));
    }
  }
  else if (idx >= 0 and idx < NUMESPTAGS)
  {
    // the command has returned but no start tag is found
    LOGDEBUG1(F("No start tag found:"), idx);
  }
  else
  {
    // the command has returned but no tag is found
    LOGWARN(F("No tag found"));
  }

  LOGDEBUG1(F("---------------------------------------------- >"), outStr);
  LOGDEBUG();

  return ret;
}

bool ESP8266_AT_Drv::sendCmdGet(const __FlashStringHelper* cmd, const __FlashStringHelper* startTag, const __FlashStringHelper* endTag, char* outStr, int outStrLen)
{
  char _startTag[strlen_P((char*)startTag) + 1];
  strcpy_P(_startTag,  (char*)startTag);

  char _endTag[strlen_P((char*)endTag) + 1];
  strcpy_P(_endTag,  (char*)endTag);

  return sendCmdGet(cmd, _startTag, _endTag, outStr, outStrLen);
}


/*
  Sends the AT command and returns the id of the TAG.
  Return -1 if no tag is found.
*/
int ESP8266_AT_Drv::sendCmd(const __FlashStringHelper* cmd, int timeout)
{
  espEmptyBuf();

  LOGDEBUG(F("----------------------------------------------"));
  LOGDEBUG1(F(">>"), cmd);

  espSerial->println(cmd);

  int idx = readUntil(timeout);

  LOGDEBUG1(F("---------------------------------------------- >"), idx);
  LOGDEBUG();

  return idx;
}

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

  LOGDEBUG(F("----------------------------------------------"));
  LOGDEBUG1(F(">>"), cmdBuf);

  espSerial->println(cmdBuf);

  int idx = readUntil(timeout);

  LOGDEBUG1(F("---------------------------------------------- >"), idx);
  LOGDEBUG();

  return idx;
}

#endif

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

  //LOGERROR1(F("AT_Drv::readUntil, millis ="), start);

  while ((millis() - start < timeout) && (ret < 0) )
  {
    if (espSerial->available())
    {
      c = (char)espSerial->read();
      LOGDEBUG0(c);
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
    LOGWARN(F(">>> TIMEOUT >>>"));
  }

  return ret;
}

void ESP8266_AT_Drv::espEmptyBuf(bool warn)
{
  char c;
  int i = 0;

  while (espSerial->available() > 0)
  {
    c = espSerial->read();
    if (i > 0 and warn == true)
      LOGDEBUG0(c);
    i++;
  }
  if (i > 0 and warn == true)
  {
    LOGDEBUG(F(""));
    LOGDEBUG1(F("Dirty characters in the serial buffer! >"), i);
  }
}

// copied from Serial::timedRead
int ESP8266_AT_Drv::timedRead()
{
  unsigned int _timeout = 1000;
  int c;
  long _startMillis = millis();
  do
  {
    c = espSerial->read();
    if (c >= 0) return c;
  } while (millis() - _startMillis < _timeout);

  return -1; // -1 indicates timeout
}


ESP8266_AT_Drv esp8266_AT_Drv;
