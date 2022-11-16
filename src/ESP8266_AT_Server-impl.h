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
