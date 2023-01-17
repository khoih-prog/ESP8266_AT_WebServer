/****************************************************************************************************************************
  ESP8266_AT_Server-impl.h - Dead simple web-server.
  For ESP8266/ESP32 AT-command running shields

  ESP8266_AT_WebServer is a library for the ESP8266/ESP32 AT-command shields to run WebServer
  Based on and modified from ESP8266 https://github.com/esp8266/Arduino/releases
  Built by Khoi Hoang https://github.com/khoih-prog/ESP8266_AT_WebServer
  Licensed under MIT license

  Original author:
  @file       Esp8266WebServer.h
  @author     Ivan Grokhotkov

  Version: 1.7.1

  Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
  1.0.0   K Hoang      12/02/2020 Initial coding for Arduino Mega, Teensy, etc
  ...
  1.6.0   K Hoang      16/11/2022 Fix severe limitation to permit sending larger data than 2K buffer. Add CORS
  1.7.0   K Hoang      16/01/2023 Add support to WizNet WizFi360 such as WIZNET_WIZFI360_EVB_PICO
  1.7.1   K Hoang      17/01/2023 Fix AP and version bugs for WizNet WizFi360
 *****************************************************************************************************************************/
#ifndef ESP8266_AT_Server_impl_h
#define ESP8266_AT_Server_impl_h

////////////////////////////////////////

#include "ESP8266_AT.h"
#include "utility/ESP8266_AT_Drv.h"
#include "utility/ESP8266_AT_Debug.h"

////////////////////////////////////////

ESP8266_AT_Server::ESP8266_AT_Server(uint16_t port)
{
  _port = port;
}

////////////////////////////////////////

void ESP8266_AT_Server::begin()
{
  AT_LOGDEBUG(F("Starting server"));

  /* The ESP Module only allows socket 1 to be used for the server */
#if 0
  _sock = ESP8266_AT_Class::getFreeSocket();

  if (_sock == SOCK_NOT_AVAIL)
  {
    AT_LOGERROR(F("No socket available for server"));
    return;
  }

#else
  _sock = 1; // If this is already in use, the startServer attempt will fail
#endif

  ESP8266_AT_Class::allocateSocket(_sock);

  _started = ESP8266_AT_Drv::startServer(_port, _sock);

  if (_started)
  {
    AT_LOGINFO1(F("Server started on port"), _port);
  }
  else
  {
    AT_LOGERROR(F("Server failed to start"));
  }
}

////////////////////////////////////////

void ESP8266_AT_Server::begin(uint16_t port)
{
  _port = port;
  begin();
}

////////////////////////////////////////

ESP8266_AT_Client ESP8266_AT_Server::available(byte* status)
{
  // TODO the original method seems to handle automatic server restart
  ESP_AT_UNUSED(status);

  int bytes = ESP8266_AT_Drv::availData(0);

  if (bytes > 0)
  {
    AT_LOGINFO1(F("New client"), ESP8266_AT_Drv::_connId);
    ESP8266_AT_Class::allocateSocket(ESP8266_AT_Drv::_connId);
    ESP8266_AT_Client client(ESP8266_AT_Drv::_connId);

    return client;
  }

  return ESP8266_AT_Client(255);
}

////////////////////////////////////////

uint8_t ESP8266_AT_Server::status()
{
  return ESP8266_AT_Drv::getServerState(0);
}

////////////////////////////////////////

size_t ESP8266_AT_Server::write(uint8_t b)
{
  return write(&b, 1);
}

////////////////////////////////////////

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

////////////////////////////////////////

#endif    //ESP8266_AT_Server_impl_h
