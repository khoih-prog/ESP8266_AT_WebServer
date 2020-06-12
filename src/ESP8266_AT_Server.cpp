/****************************************************************************************************************************
   ESP8266_AT_Server.cpp - Dead simple web-server.
   For ESP8266 AT-command running shields

   ESP8266_AT_WebServer is a library for the ESP8266 AT-command shields to run WebServer
   Based on and modified from ESP8266 https://github.com/esp8266/Arduino/releases
   Built by Khoi Hoang https://github.com/khoih-prog/ESP8266_AT_WebServer
   Licensed under MIT license
   Version: 1.0.6

   Original author:
   @file       Esp8266WebServer.h
   @author     Ivan Grokhotkov

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

#include "ESP8266_AT_Server.h"

#include "utility/ESP8266_AT_Drv.h"
#include "utility/ESP8266_AT_Debug.h"


ESP8266_AT_Server::ESP8266_AT_Server(uint16_t port)
{
  _port = port;
}

void ESP8266_AT_Server::begin()
{
  LOGDEBUG(F("Starting server"));

  /* The ESP Module only allows socket 1 to be used for the server */
#if 0
  _sock = ESP8266_AT_Class::getFreeSocket();
  if (_sock == SOCK_NOT_AVAIL)
  {
    LOGERROR(F("No socket available for server"));
    return;
  }
#else
  _sock = 1; // If this is already in use, the startServer attempt will fail
#endif
  ESP8266_AT_Class::allocateSocket(_sock);

  _started = ESP8266_AT_Drv::startServer(_port, _sock);

  if (_started)
  {
    LOGINFO1(F("Server started on port"), _port);
  }
  else
  {
    LOGERROR(F("Server failed to start"));
  }
}

ESP8266_AT_Client ESP8266_AT_Server::available(byte* status)
{
  // TODO the original method seems to handle automatic server restart

  int bytes = ESP8266_AT_Drv::availData(0);
  if (bytes > 0)
  {
    LOGINFO1(F("New client"), ESP8266_AT_Drv::_connId);
    ESP8266_AT_Class::allocateSocket(ESP8266_AT_Drv::_connId);
    ESP8266_AT_Client client(ESP8266_AT_Drv::_connId);
    return client;
  }

  return ESP8266_AT_Client(255);
}

uint8_t ESP8266_AT_Server::status()
{
  return ESP8266_AT_Drv::getServerState(0);
}

size_t ESP8266_AT_Server::write(uint8_t b)
{
  return write(&b, 1);
}

size_t ESP8266_AT_Server::write(const uint8_t *buffer, size_t size)
{
  size_t n = 0;

  for (int sock = 0; sock < MAX_SOCK_NUM; sock++)
  {
    if (ESP8266_AT_Class::_state[sock] != 0)
    {
      ESP8266_AT_Client client(sock);
      n += client.write(buffer, size);
    }
  }
  return n;
}
