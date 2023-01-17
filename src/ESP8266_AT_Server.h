/****************************************************************************************************************************
  ESP8266_AT_Server.h - Dead simple web-server.
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

#ifndef ESP8266_AT_Server_h
#define ESP8266_AT_Server_h

////////////////////////////////////////

#include <Server.h>

////////////////////////////////////////

class ESP8266_AT_Client;

////////////////////////////////////////

class ESP8266_AT_Server : public Server
{
  public:
    ESP8266_AT_Server(uint16_t port);

    /*
      Gets a client that is connected to the server and has data available for reading.
      The connection persists when the returned client object goes out of scope; you can close it by calling client.stop().
      Returns a Client object; if no Client has data available for reading, this object will evaluate to false in an if-statement.
    */
    ESP8266_AT_Client available(uint8_t* status = NULL);

    /*
      Start the TCP server
    */
    void begin();

    void begin(uint16_t port);

    virtual size_t write(uint8_t);
    virtual size_t write(const uint8_t *buf, size_t size);

    uint8_t status();

    using Print::write;

  private:
    uint16_t _port;
    uint8_t _sock;
    bool _started;
};

////////////////////////////////////////

#include "ESP8266_AT_Server-impl.h"

////////////////////////////////////////

#endif
