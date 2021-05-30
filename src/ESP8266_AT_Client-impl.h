/****************************************************************************************************************************
  ESP8266_AT_Client-impl.h - Dead simple web-server.
  For ESP8266/ESP32 AT-command running shields

  ESP8266_AT_WebServer is a library for the ESP8266/ESP32 AT-command shields to run WebServer
  Based on and modified from ESP8266 https://github.com/esp8266/Arduino/releases
  Built by Khoi Hoang https://github.com/khoih-prog/ESP8266_AT_WebServer
  Licensed under MIT license

  Original author:
  @file       Esp8266WebServer.h
  @author     Ivan Grokhotkov

  Version: 1.3.0

  Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
  1.0.0   K Hoang      12/02/2020 Initial coding for Arduino Mega, Teensy, etc
  1.0.1   K Hoang      17/02/2020 Add support to server's lambda function calls
  1.0.2   K Hoang      22/02/2020 Add support to SAMD (DUE, ZERO, MKR, NANO_33_IOT, M0, M0 Pro, AdaFruit, etc) boards
  1.0.3   K Hoang      03/03/2020 Add support to STM32 (STM32,F0,F1, F2, F3, F4, F7, etc) boards
  1.0.4   K Hoang      19/03/2020 Fix bug. Sync with ESP8266WebServer library of core v2.6.3
  1.0.5   K Hoang      17/04/2020 Add support to SAMD51 and SAM DUE boards
  1.0.6   K Hoang      11/06/2020 Add support to nRF52 boards, such as AdaFruit Feather nRF52832, nRF52840 Express, BlueFruit Sense, 
                                  Itsy-Bitsy nRF52840 Express, Metro nRF52840 Express, NINA_B302_ublox, NINA_B112_ublox, etc.
  1.0.7   K Hoang      23/06/2020 Add support to ESP32-AT. Update deprecated ESP8266-AT commands. Restructure examples. 
  1.0.8   K Hoang      01/07/2020 Fix bug. Add features to ESP32-AT.   
  1.0.9   K Hoang      03/07/2020 Fix bug. Add functions. Restructure codes.
  1.0.10  K Hoang      22/07/2020 Fix bug not closing client and releasing socket.
  1.0.11  K Hoang      25/07/2020 Add support to all STM32F/L/H/G/WB/MP1 and Seeeduino SAMD21/SAMD51 boards  
  1.0.12  K Hoang      26/07/2020 Add example and sample Packages_Patches for STM32F/L/H/G/WB/MP boards
  1.1.0   K Hoang      21/09/2020 Add support to UDP Multicast. Fix bugs.
  1.1.1   K Hoang      26/09/2020 Restore support to PROGMEM-related commands, such as sendContent_P() and send_P()
  1.1.2   K Hoang      28/12/2020 Suppress all possible compiler warnings
  1.2.0   K Hoang      11/05/2021 Add support to BOARD_SIPEED_MAIX_DUINO and RASPBERRY_PI_PICO using Arduino-pico core
  1.3.0   K Hoang      29/05/2021 Add support to Nano_RP2040_Connect, RASPBERRY_PI_PICO using Arduino mbed code
 *****************************************************************************************************************************/

#ifndef ESP8266_AT_Client_impl_h
#define ESP8266_AT_Client_impl_h

#include <inttypes.h>

#include "ESP8266_AT.h"

#include "ESP8266_AT_Server.h"

#include "utility/ESP8266_AT_Drv.h"
#include "utility/ESP8266_AT_Debug.h"

ESP8266_AT_Client::ESP8266_AT_Client() : _sock(255)
{
}

ESP8266_AT_Client::ESP8266_AT_Client(uint8_t sock) : _sock(sock)
{
}


////////////////////////////////////////////////////////////////////////////////
// Overrided Print methods
////////////////////////////////////////////////////////////////////////////////

// the standard print method will call write for each character in the buffer
// this is very slow on ESP
size_t ESP8266_AT_Client::print(const __FlashStringHelper *ifsh)
{
  return printFSH(ifsh, false);
}

// if we do override this, the standard println will call the print
// method twice
size_t ESP8266_AT_Client::println(const __FlashStringHelper *ifsh)
{
  return printFSH(ifsh, true);
}


////////////////////////////////////////////////////////////////////////////////
// Implementation of Client virtual methods
////////////////////////////////////////////////////////////////////////////////

int ESP8266_AT_Client::connectSSL(const char* host, uint16_t port)
{
  return connect(host, port, SSL_MODE);
}

int ESP8266_AT_Client::connectSSL(IPAddress ip, uint16_t port)
{
  char s[16];
  
  sprintf_P(s, PSTR("%d.%d.%d.%d"), ip[0], ip[1], ip[2], ip[3]);
  //sprintf(s, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
  
  return connect(s, port, SSL_MODE);
}

int ESP8266_AT_Client::connect(const char* host, uint16_t port)
{
  return connect(host, port, TCP_MODE);
}

int ESP8266_AT_Client::connect(IPAddress ip, uint16_t port)
{
  char s[16];
  
  sprintf_P(s, PSTR("%d.%d.%d.%d"), ip[0], ip[1], ip[2], ip[3]);
  //sprintf(s, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);

  return connect(s, port, TCP_MODE);
}

/* Private method */
int ESP8266_AT_Client::connect(const char* host, uint16_t port, uint8_t protMode)
{
  AT_LOGINFO1(F("Connecting to"), host);

  _sock = ESP8266_AT_Class::getFreeSocket();

  if (_sock != NO_SOCKET_AVAIL)
  {
    if (!ESP8266_AT_Drv::startClient(host, port, _sock, protMode))
      return 0;

    ESP8266_AT_Class::allocateSocket(_sock);
  }
  else
  {
    AT_LOGERROR(F("No socket available"));
    return 0;
  }
  
  return 1;
}

size_t ESP8266_AT_Client::write(uint8_t b)
{
  return write(&b, 1);
}

size_t ESP8266_AT_Client::write(const uint8_t *buf, size_t size)
{
  AT_LOGDEBUG3(F("Client_write: buf ="), (char *) buf, ", size =", size);
  AT_LOGDEBUG1(F("Client_write: sock ="), _sock);
  
  // KH, v1.1.0
  //if ( (_sock >= MAX_SOCK_NUM) || (size == 0) )
  if ( (_sock >= MAX_SOCK_NUM) || (size == 0) || buf[0] == 0 )
  {
    setWriteError();
    return 0;
  }

  bool r = ESP8266_AT_Drv::sendData(_sock, buf, size);
  
  if (!r)
  {
    setWriteError();
    AT_LOGERROR1(F("Client_write: Failed to write to socket"), _sock);
    delay(4000);
    stop();
    
    return 0;
  }

  return size;
}

int ESP8266_AT_Client::available()
{
  if (_sock != 255)
  {
    int bytes = ESP8266_AT_Drv::availData(_sock);
    
    if (bytes > 0)
    {
      return bytes;
    }
  }

  return 0;
}

int ESP8266_AT_Client::read()
{
  uint8_t b;
  
  if (!available())
    return -1;

  bool connClose = false;
  ESP8266_AT_Drv::getData(_sock, &b, false, &connClose);

  if (connClose)
  {
    ESP8266_AT_Class::releaseSocket(_sock);
    _sock = 255;
  }

  return b;
}

int ESP8266_AT_Client::read(uint8_t* buf, size_t size)
{
  if (!available())
    return -1;
  return ESP8266_AT_Drv::getDataBuf(_sock, buf, size);
}

int ESP8266_AT_Client::peek()
{
  uint8_t b;
  
  if (!available())
    return -1;

  bool connClose = false;
  ESP8266_AT_Drv::getData(_sock, &b, true, &connClose);

  if (connClose)
  {
    ESP8266_AT_Class::releaseSocket(_sock);
    _sock = 255;
  }

  return b;
}

void ESP8266_AT_Client::flush()
{
  while (available())
    read();
}

void ESP8266_AT_Client::stop()
{
  if (_sock == 255)
    return;

  AT_LOGINFO1(F("Disconnecting "), _sock);

  ESP8266_AT_Drv::stopClient(_sock);

  ESP8266_AT_Class::releaseSocket(_sock);
  _sock = 255;
}


uint8_t ESP8266_AT_Client::connected()
{
  return (status() == ESTABLISHED);
}


ESP8266_AT_Client::operator bool()
{
  return _sock != 255;
}

////////////////////////////////////////////////////////////////////////////////
// Additional WiFi standard methods
////////////////////////////////////////////////////////////////////////////////


uint8_t ESP8266_AT_Client::status()
{
  if (_sock == 255)
  {
    AT_LOGDEBUG(F("Client::status: sock closed"));
    return CLOSED;
  }

  if (ESP8266_AT_Drv::availData(_sock))
  {
    AT_LOGDEBUG(F("Client::status: availData OK"));
    return ESTABLISHED;
  }

  if (ESP8266_AT_Drv::getClientState(_sock))
  {
    AT_LOGDEBUG(F("Client::status: getClientState OK"));
    return ESTABLISHED;
  }

  ESP8266_AT_Class::releaseSocket(_sock);
  _sock = 255;

  AT_LOGDEBUG(F("Client::status: sock released"));
  
  return CLOSED;
}

IPAddress ESP8266_AT_Client::remoteIP()
{
  IPAddress ret;
  
  ESP8266_AT_Drv::getRemoteIpAddress(ret);
  
  return ret;
}

////////////////////////////////////////////////////////////////////////////////
// Private Methods
////////////////////////////////////////////////////////////////////////////////

size_t ESP8266_AT_Client::printFSH(const __FlashStringHelper *ifsh, bool appendCrLf)
{
  size_t size = strlen_P((char*)ifsh);

  if ( (_sock >= MAX_SOCK_NUM) || (size == 0) )
  {
    setWriteError();
    return 0;
  }

  bool r = ESP8266_AT_Drv::sendData(_sock, ifsh, size, appendCrLf);
  
  if (!r)
  {
    setWriteError();
    AT_LOGERROR1(F("printFSH: Failed to write to socket"), _sock);
    delay(4000);
    stop();
    
    return 0;
  }

  return size;
}

#endif    //ESP8266_AT_Client_impl_h
