/****************************************************************************************************************************
  RequestHandler.h - Dead simple web-server.
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

#pragma once

#ifndef RequestHandler_h
#define RequestHandler_h

////////////////////////////////////////

#ifndef ESP_AT_UNUSED
  #define ESP_AT_UNUSED(x) (void)(x)
#endif

////////////////////////////////////////

class RequestHandler
{
  public:

    virtual ~RequestHandler() { }

    ////////////////////////////////////////

    virtual bool canHandle(const HTTPMethod& method, const String& uri)
    {
      ESP_AT_UNUSED(method);
      ESP_AT_UNUSED(uri);

      return false;
    }

    ////////////////////////////////////////

    virtual bool canUpload(const String& uri)
    {
      ESP_AT_UNUSED(uri);

      return false;
    }

    ////////////////////////////////////////

    virtual bool handle(ESP8266_AT_WebServer& server, const HTTPMethod& requestMethod, const String& requestUri)
    {
      ESP_AT_UNUSED(server);
      ESP_AT_UNUSED(requestMethod);
      ESP_AT_UNUSED(requestUri);

      return false;
    }

    ////////////////////////////////////////

    virtual void upload(ESP8266_AT_WebServer& server, const String& requestUri, const HTTPUpload& upload)
    {
      ESP_AT_UNUSED(server);
      ESP_AT_UNUSED(requestUri);
      ESP_AT_UNUSED(upload);
    }

    ////////////////////////////////////////

    RequestHandler* next()
    {
      return _next;
    }

    ////////////////////////////////////////

    void next(RequestHandler* r)
    {
      _next = r;
    }

    ////////////////////////////////////////

  private:

    RequestHandler* _next = nullptr;
};

////////////////////////////////////////

#endif //RequestHandler_h
