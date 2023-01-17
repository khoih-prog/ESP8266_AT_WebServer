/****************************************************************************************************************************
  RequestHandlerImpl.h - Dead simple web-server.
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

#ifndef RequestHandlerImpl_h
#define RequestHandlerImpl_h

////////////////////////////////////////

#include "RequestHandler.h"
#include "mimetable.h"

////////////////////////////////////////

class FunctionRequestHandler : public RequestHandler
{
  public:

    ////////////////////////////////////////

    FunctionRequestHandler(ESP8266_AT_WebServer::THandlerFunction fn, ESP8266_AT_WebServer::THandlerFunction ufn,
                           const String &uri, const HTTPMethod& method)
      : _fn(fn)
      , _ufn(ufn)
      , _uri(uri)
      , _method(method)
    {
    }

    ////////////////////////////////////////

    bool canHandle(const HTTPMethod& requestMethod, const String& requestUri) override
    {
      if (_method != HTTP_ANY && _method != requestMethod)
        return false;

      if (requestUri == _uri)
        return true;

      if (_uri.endsWith("/*"))
      {
        String _uristart = _uri;
        _uristart.replace("/*", "");

        if (requestUri.startsWith(_uristart))
          return true;
      }

      return false;
    }

    ////////////////////////////////////////

    bool canUpload(const String& requestUri) override
    {
      if (!_ufn || !canHandle(HTTP_POST, requestUri))
        return false;

      return true;
    }

    ////////////////////////////////////////

    bool handle(ESP8266_AT_WebServer& server, const HTTPMethod& requestMethod, const String& requestUri) override
    {
      ESP_AT_UNUSED(server);

      if (!canHandle(requestMethod, requestUri))
        return false;

      _fn();
      return true;
    }

    ////////////////////////////////////////

    void upload(ESP8266_AT_WebServer& server, const String& requestUri, const HTTPUpload& upload) override
    {
      ESP_AT_UNUSED(server);
      ESP_AT_UNUSED(upload);

      if (canUpload(requestUri))
        _ufn();
    }

    ////////////////////////////////////////

  protected:
    ESP8266_AT_WebServer::THandlerFunction _fn;
    ESP8266_AT_WebServer::THandlerFunction _ufn;
    String _uri;
    HTTPMethod _method;
};

////////////////////////////////////////

class StaticRequestHandler : public RequestHandler
{
  public:

    ////////////////////////////////////////

    bool canHandle(const HTTPMethod& requestMethod, const String& requestUri) override
    {
      if (requestMethod != HTTP_GET)
        return false;

      if ((_isFile && requestUri != _uri) || !requestUri.startsWith(_uri))
        return false;

      return true;
    }

    ////////////////////////////////////////

    static String getContentType(const String& path)
    {
      using namespace mime;
      char buff[sizeof(mimeTable[0].mimeType)];

      // Check all entries but last one for match, return if found
      for (size_t i = 0; i < sizeof(mimeTable) / sizeof(mimeTable[0]) - 1; i++)
      {
        strcpy(buff, mimeTable[i].endsWith);

        if (path.endsWith(buff))
        {
          strcpy(buff, mimeTable[i].mimeType);
          return String(buff);
        }
      }

      // Fall-through and just return default type
      strcpy(buff, mimeTable[sizeof(mimeTable) / sizeof(mimeTable[0]) - 1].mimeType);
      return String(buff);
    }

    ////////////////////////////////////////

  protected:

    String _uri;
    String _path;
    String _cache_header;
    bool _isFile;
    size_t _baseUriLength;
};

////////////////////////////////////////

#endif //RequestHandlerImpl_h  
