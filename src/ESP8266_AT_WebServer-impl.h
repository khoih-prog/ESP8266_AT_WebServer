/****************************************************************************************************************************
  ESP8266_AT_WebServer-impl.h - Dead simple web-server.
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

#ifndef ESP8266_AT_WebServer_impl_h
#define ESP8266_AT_WebServer_impl_h

#include <Arduino.h>
#include <libb64/cencode.h>
#include <ESP8266_AT.h>
#include "ESP8266_AT_WebServer.h"
#include "utility/RequestHandlersImpl.h"
#include "utility/ESP8266_AT_Debug.h"
#include "utility/mimetable.h"

const char * AUTHORIZATION_HEADER = "Authorization";

// New to use EWString

////////////////////////////////////////

ESP8266_AT_WebServer::ESP8266_AT_WebServer(int port)
  : _corsEnabled(false)
  , _server(port)
  , _currentMethod(HTTP_ANY)
  , _currentVersion(0)
  , _currentHandler(0)
  , _firstHandler(0)
  , _lastHandler(0)
  , _currentArgCount(0)
  , _currentArgs(0)
  , _headerKeysCount(0)
  , _currentHeaders(0)
  , _contentLength(0)
  , _chunked(false)
{
}

////////////////////////////////////////

ESP8266_AT_WebServer::~ESP8266_AT_WebServer()
{
  if (_currentHeaders)
    delete[]_currentHeaders;

  _headerKeysCount = 0;
  RequestHandler* handler = _firstHandler;

  while (handler)
  {
    RequestHandler* next = handler->next();
    delete handler;
    handler = next;
  }

  close();
}

////////////////////////////////////////

void ESP8266_AT_WebServer::begin()
{
  close();
  _server.begin();
}

////////////////////////////////////////

bool ESP8266_AT_WebServer::authenticate(const char * username, const char * password)
{
  if (hasHeader(AUTHORIZATION_HEADER))
  {
    String authReq = header(AUTHORIZATION_HEADER);

    if (authReq.startsWith("Basic"))
    {
      authReq = authReq.substring(6);
      authReq.trim();
      char toencodeLen = strlen(username) + strlen(password) + 1;
      char *toencode = new char[toencodeLen + 1];

      if (toencode == NULL)
      {
        authReq = String();
        return false;
      }

      char *encoded = new char[base64_encode_expected_len(toencodeLen) + 1];

      if (encoded == NULL)
      {
        authReq = String();
        delete[] toencode;
        return false;
      }

      sprintf(toencode, "%s:%s", username, password);

      if (base64_encode_chars(toencode, toencodeLen, encoded) > 0 && authReq.equals(encoded))
      {
        authReq = String();
        delete[] toencode;
        delete[] encoded;
        return true;
      }

      delete[] toencode;
      delete[] encoded;
    }

    authReq = String();
  }

  return false;
}

////////////////////////////////////////

void ESP8266_AT_WebServer::requestAuthentication()
{
  sendHeader("WWW-Authenticate", "Basic realm=\"Login Required\"");
  send(401);
}

////////////////////////////////////////

void ESP8266_AT_WebServer::on(const String &uri, ESP8266_AT_WebServer::THandlerFunction handler)
{
  on(uri, HTTP_ANY, handler);
}

////////////////////////////////////////

void ESP8266_AT_WebServer::on(const String &uri, HTTPMethod method, ESP8266_AT_WebServer::THandlerFunction fn)
{
  on(uri, method, fn, _fileUploadHandler);
}

////////////////////////////////////////

void ESP8266_AT_WebServer::on(const String &uri, HTTPMethod method, ESP8266_AT_WebServer::THandlerFunction fn,
                              ESP8266_AT_WebServer::THandlerFunction ufn)
{
  _addRequestHandler(new FunctionRequestHandler(fn, ufn, uri, method));
}

////////////////////////////////////////

void ESP8266_AT_WebServer::addHandler(RequestHandler* handler)
{
  _addRequestHandler(handler);
}

////////////////////////////////////////

void ESP8266_AT_WebServer::_addRequestHandler(RequestHandler* handler)
{
  if (!_lastHandler)
  {
    _firstHandler = handler;
    _lastHandler = handler;
  }
  else
  {
    _lastHandler->next(handler);
    _lastHandler = handler;
  }
}

////////////////////////////////////////

void ESP8266_AT_WebServer::handleClient()
{
  if (_currentStatus == HC_NONE)
  {
    ESP8266_AT_Client client = _server.available();

    if (!client)
    {
      return;
    }

    AT_LOGDEBUG(F("handleClient: New Client"));

    _currentClient = client;
    _currentStatus = HC_WAIT_READ;
    _statusChange = millis();
  }

  bool keepCurrentClient = false;
  bool callYield = false;

  if (_currentClient.connected() || _currentClient.available())
  {
    switch (_currentStatus)
    {
      case HC_NONE:
        // No-op to avoid C++ compiler warning
        break;

      case HC_WAIT_READ:

        // Wait for data from client to become available
        if (_currentClient.available())
        {
          if (_parseRequest(_currentClient))
          {
            _currentClient.setTimeout(HTTP_MAX_SEND_WAIT);
            _contentLength = CONTENT_LENGTH_NOT_SET;
            _handleRequest();
          }
        }
        else
        {
          if (millis() - _statusChange <= HTTP_MAX_DATA_WAIT)
          {
            keepCurrentClient = true;
          }

          callYield = true;
        }

        break;

      case HC_WAIT_CLOSE:

        // Wait for client to close the connection
        if (millis() - _statusChange <= HTTP_MAX_CLOSE_WAIT)
        {
          keepCurrentClient = true;
          callYield = true;
        }
    }
  }

  if (!keepCurrentClient)
  {
    AT_LOGDEBUG(F("handleClient: Don't keepCurrentClient"));
    _currentClient = ESP8266_AT_Client();
    _currentStatus = HC_NONE;
    // KH
    //_currentUpload.reset();
  }

  if (callYield)
  {
    yield();
  }

  // KH, fix bug. Have to close the connection.
  _currentClient.stop();
  AT_LOGINFO(F("handleClient: Client disconnected"));
}

////////////////////////////////////////

void ESP8266_AT_WebServer::close()
{
  // TODO: Write close method for ESP8266_AT library and uncomment this
  _currentStatus = HC_NONE;

  if (!_headerKeysCount)
    collectHeaders(0, 0);
}

////////////////////////////////////////

void ESP8266_AT_WebServer::stop()
{
  close();
}

////////////////////////////////////////

void ESP8266_AT_WebServer::sendHeader(const String& name, const String& value, bool first)
{
  EWString headerLine = fromString(name);

  headerLine += ": ";
  headerLine += fromString(value);
  headerLine += RETURN_NEWLINE;

  if (first)
  {
    _responseHeaders = fromEWString(headerLine + fromString(_responseHeaders));
  }
  else
  {
    _responseHeaders = fromEWString(fromString(_responseHeaders) + headerLine);
  }
}

////////////////////////////////////////

void ESP8266_AT_WebServer::setContentLength(size_t contentLength)
{
  _contentLength = contentLength;
}

////////////////////////////////////////

void ESP8266_AT_WebServer::_prepareHeader(String& response, int code, const char* content_type, size_t contentLength)
{
  EWString aResponse = fromString(response);

  aResponse = "HTTP/1." + fromString(String(_currentVersion)) + " ";
  aResponse += fromString(String(code));
  aResponse += " ";
  aResponse += fromString(_responseCodeToString(code));
  aResponse += RETURN_NEWLINE;

  using namespace mime;

  if (!content_type)
    content_type = mimeTable[html].mimeType;

  sendHeader("Content-Type", content_type, true);

  if (_contentLength == CONTENT_LENGTH_NOT_SET)
  {
    sendHeader("Content-Length", String(contentLength));
  }
  else if (_contentLength != CONTENT_LENGTH_UNKNOWN)
  {
    sendHeader("Content-Length", String(_contentLength));
  }
  else if (_contentLength == CONTENT_LENGTH_UNKNOWN && _currentVersion)
  {
    //HTTP/1.1 or above client
    //let's do chunked
    _chunked = true;
    sendHeader("Accept-Ranges", "none");
    sendHeader("Transfer-Encoding", "chunked");
  }

  if (_corsEnabled)
  {
    sendHeader("Access-Control-Allow-Origin",  "*");
    sendHeader("Access-Control-Allow-Methods", "*");
    sendHeader("Access-Control-Allow-Headers", "*");
  }

  AT_LOGDEBUG(F("_prepareHeader sendHeader Conn close"));

  sendHeader("Connection", "close");

  aResponse += fromString(_responseHeaders);
  aResponse += RETURN_NEWLINE;

  response = fromEWString(aResponse);

  _responseHeaders = String("");
}

////////////////////////////////////////

#if !ESP_AT_USE_AVR

////////////////////////////////////////

void ESP8266_AT_WebServer::_prepareHeader(EWString& response, int code, const char* content_type, size_t contentLength)
{
  response = "HTTP/1." + fromString(String(_currentVersion)) + " ";
  response += fromString(String(code));
  response += " ";
  response += fromString(_responseCodeToString(code));
  response += RETURN_NEWLINE;

  using namespace mime;

  if (!content_type)
    content_type = mimeTable[html].mimeType;

  sendHeader("Content-Type", content_type, true);

  if (_contentLength == CONTENT_LENGTH_NOT_SET)
  {
    sendHeader("Content-Length", String(contentLength));
  }
  else if (_contentLength != CONTENT_LENGTH_UNKNOWN)
  {
    sendHeader("Content-Length", String(_contentLength));
  }
  else if (_contentLength == CONTENT_LENGTH_UNKNOWN && _currentVersion)
  {
    //HTTP/1.1 or above client
    //let's do chunked
    _chunked = true;
    sendHeader("Accept-Ranges", "none");
    sendHeader("Transfer-Encoding", "chunked");
  }

  if (_corsEnabled)
  {
    sendHeader("Access-Control-Allow-Origin",  "*");
    sendHeader("Access-Control-Allow-Methods", "*");
    sendHeader("Access-Control-Allow-Headers", "*");
  }

  AT_LOGDEBUG(F("_prepareHeader sendHeader Conn close"));

  sendHeader("Connection", "close");

  response += fromString(_responseHeaders);
  response += RETURN_NEWLINE;

  _responseHeaders = String("");
}

////////////////////////////////////////

#endif

////////////////////////////////////////

void ESP8266_AT_WebServer::send(int code, const char* content_type, const String& content)
{
  EWString header;

  _prepareHeader(header, code, content_type, content.length());

  _currentClient.write((const uint8_t *)header.c_str(), header.length());

  if (content.length())
  {
    sendContent(content, content.length());
  }
}

////////////////////////////////////////

void ESP8266_AT_WebServer::send(int code, char* content_type, const String& content, size_t contentLength)
{
  EWString header;

  char type[64];

  memccpy((void*)type, content_type, 0, sizeof(type));
  _prepareHeader(header, code, (const char* )type, contentLength);

  _currentClient.write((const uint8_t *) header.c_str(), header.length());

  if (contentLength)
  {
    sendContent(content, contentLength);
  }
}

////////////////////////////////////////

void ESP8266_AT_WebServer::send(int code, char* content_type, const String& content)
{
  send(code, (const char*)content_type, content);
}

////////////////////////////////////////

void ESP8266_AT_WebServer::send(int code, const String& content_type, const String& content)
{
  send(code, (const char*)content_type.c_str(), content);
}

////////////////////////////////////////

void ESP8266_AT_WebServer::send(int code, const char* content_type, const char* content)
{
  send(code, content_type, content, content ? strlen(content) : 0);
}

////////////////////////////////////////

void ESP8266_AT_WebServer::send(int code, const char* content_type, const char* content, size_t contentLength)
{
  String header;

  _prepareHeader(header, code, content_type, contentLength);

  _currentClient.write((const uint8_t *) header.c_str(), header.length());

  if (contentLength)
  {
    sendContent(content, contentLength);
  }
}

////////////////////////////////////////

void ESP8266_AT_WebServer::sendContent(const String& content)
{
  const char * footer = RETURN_NEWLINE;
  size_t len = content.length();

  if (_chunked)
  {
    char * chunkSize = (char *) malloc(11);

    if (chunkSize)
    {
      sprintf(chunkSize, "%x%s", (unsigned int) len, footer);
      _currentClient.write(chunkSize, strlen(chunkSize));
      free(chunkSize);
    }
  }

  _currentClient.write(content.c_str(), len);

  if (_chunked)
  {
    _currentClient.write(footer, 2);
  }
}

////////////////////////////////////////

void ESP8266_AT_WebServer::sendContent(const String& content, size_t size)
{
  const char * footer = RETURN_NEWLINE;

  if (_chunked)
  {
    char * chunkSize = (char *) malloc(11);

    if (chunkSize)
    {
      AT_LOGDEBUG(F("sendContent: _chunked"));

      sprintf(chunkSize, "%x%s", (unsigned int) size, footer);
      _currentClient.write(chunkSize, strlen(chunkSize));
      free(chunkSize);
    }
  }

  AT_LOGDEBUG1(F("sendContent: Client.write content: "), content);

  _currentClient.write(content.c_str(), size);

  if (_chunked)
  {
    _currentClient.write(footer, 2);
  }
}

////////////////////////////////////////

// KH, Restore PROGMEM commands
void ESP8266_AT_WebServer::send_P(int code, PGM_P content_type, PGM_P content)
{
  size_t contentLength = 0;

  if (content != NULL)
  {
    contentLength = strlen_P(content);
  }

  String header;
  char type[64];

  memccpy_P((void*)type, (PGM_VOID_P)content_type, 0, sizeof(type));
  _prepareHeader(header, code, (const char* )type, contentLength);

  AT_LOGDEBUG1(F("send_P: len = "), contentLength);
  AT_LOGDEBUG1(F("content = "), content);
  AT_LOGDEBUG1(F("send_P: hdrlen = "), header.length());
  AT_LOGDEBUG1(F("header = "), header);

  _currentClient.write((const uint8_t *) header.c_str(), header.length());

  if (contentLength)
  {
    sendContent_P(content);
  }
}

////////////////////////////////////////

void ESP8266_AT_WebServer::send_P(int code, PGM_P content_type, PGM_P content, size_t contentLength)
{
  EWString header;

  char type[64];

  memccpy_P((void*)type, (PGM_VOID_P)content_type, 0, sizeof(type));
  _prepareHeader(header, code, (const char* )type, contentLength);

  AT_LOGDEBUG1(F("send_P: len = "), contentLength);
  AT_LOGDEBUG1(F("content = "), content);
  AT_LOGDEBUG1(F("send_P: hdrlen = "), header.length());
  AT_LOGDEBUG1(F("header = "), fromEWString(header));

  _currentClient.write((const uint8_t *) header.c_str(), header.length());

  if (contentLength)
  {
    sendContent_P(content, contentLength);
  }
}

////////////////////////////////////////

void ESP8266_AT_WebServer::sendContent_P(PGM_P content)
{
  sendContent_P(content, strlen_P(content));
}

////////////////////////////////////////

void ESP8266_AT_WebServer::sendContent_P(PGM_P content, size_t size)
{
  const char * footer = RETURN_NEWLINE;

  if (_chunked)
  {
    char * chunkSize = (char *) malloc(11);

    if (chunkSize)
    {
      sprintf(chunkSize, "%x%s", (unsigned int) size, footer);
      _currentClient.write(chunkSize, strlen(chunkSize));
      free(chunkSize);
    }
  }

  uint8_t* buffer = new uint8_t[SENDCONTENT_P_BUFFER_SZ];

  if (buffer)
  {
    uint16_t count = size / SENDCONTENT_P_BUFFER_SZ;
    uint16_t remainder = size % SENDCONTENT_P_BUFFER_SZ;
    uint16_t i = 0;

    for (i = 0; i < count; i++)
    {
      /* code */
      memcpy_P(buffer, &content[i * SENDCONTENT_P_BUFFER_SZ], SENDCONTENT_P_BUFFER_SZ);
      _currentClient.write(buffer, SENDCONTENT_P_BUFFER_SZ);
    }

    memcpy_P(buffer, &content[i * SENDCONTENT_P_BUFFER_SZ], remainder);
    _currentClient.write(buffer, remainder);

    delete [] buffer;
  }
  else
  {
    AT_LOGERROR1(F("sendContent_P: Error, can't allocate buffer, Sz ="), SENDCONTENT_P_BUFFER_SZ);
    return;
  }

  if (_chunked)
  {
    _currentClient.write(footer, 2);
  }
}

////////////////////////////////////////

String ESP8266_AT_WebServer::arg(const String& name)
{
  for (int i = 0; i < _currentArgCount; ++i)
  {
    if ( _currentArgs[i].key == name )
      return _currentArgs[i].value;
  }

  return String();
}

////////////////////////////////////////

String ESP8266_AT_WebServer::arg(int i)
{
  if (i < _currentArgCount)
    return _currentArgs[i].value;

  return String();
}

////////////////////////////////////////

String ESP8266_AT_WebServer::argName(int i)
{
  if (i < _currentArgCount)
    return _currentArgs[i].key;

  return String();
}

////////////////////////////////////////

int ESP8266_AT_WebServer::args()
{
  return _currentArgCount;
}

////////////////////////////////////////

bool ESP8266_AT_WebServer::hasArg(const String& name)
{
  for (int i = 0; i < _currentArgCount; ++i)
  {
    if (_currentArgs[i].key == name)
      return true;
  }

  return false;
}

////////////////////////////////////////

String ESP8266_AT_WebServer::header(const String& name)
{
  for (int i = 0; i < _headerKeysCount; ++i)
  {
    if (_currentHeaders[i].key == name)
      return _currentHeaders[i].value;
  }

  return String();
}

////////////////////////////////////////

void ESP8266_AT_WebServer::collectHeaders(const char* headerKeys[], const size_t headerKeysCount)
{
  _headerKeysCount = headerKeysCount + 1;

  if (_currentHeaders)
    delete[]_currentHeaders;

  _currentHeaders = new RequestArgument[_headerKeysCount];
  _currentHeaders[0].key = AUTHORIZATION_HEADER;

  for (int i = 1; i < _headerKeysCount; i++)
  {
    _currentHeaders[i].key = headerKeys[i - 1];
  }
}

////////////////////////////////////////

String ESP8266_AT_WebServer::header(int i)
{
  if (i < _headerKeysCount)
    return _currentHeaders[i].value;

  return String();
}

////////////////////////////////////////

String ESP8266_AT_WebServer::headerName(int i)
{
  if (i < _headerKeysCount)
    return _currentHeaders[i].key;

  return String();
}

////////////////////////////////////////

int ESP8266_AT_WebServer::headers()
{
  return _headerKeysCount;
}

////////////////////////////////////////

bool ESP8266_AT_WebServer::hasHeader(const String& name)
{
  for (int i = 0; i < _headerKeysCount; ++i)
  {
    if ((_currentHeaders[i].key == name) &&  (_currentHeaders[i].value.length() > 0))
      return true;
  }

  return false;
}

////////////////////////////////////////

String ESP8266_AT_WebServer::hostHeader()
{
  return _hostHeader;
}

////////////////////////////////////////

void ESP8266_AT_WebServer::onFileUpload(THandlerFunction fn)
{
  _fileUploadHandler = fn;
}

////////////////////////////////////////

void ESP8266_AT_WebServer::onNotFound(THandlerFunction fn)
{
  _notFoundHandler = fn;
}

////////////////////////////////////////

void ESP8266_AT_WebServer::_handleRequest()
{
  bool handled = false;

  if (!_currentHandler)
  {
    AT_LOGDEBUG(F("_handleRequest: request handler not found"));
  }
  else
  {
    AT_LOGDEBUG(F("_handleRequest handle"));

    handled = _currentHandler->handle(*this, _currentMethod, _currentUri);

    if (!handled)
    {
      AT_LOGDEBUG(F("_handleRequest: _handleRequest failed"));
    }
    else
    {
      AT_LOGDEBUG(F("_handleRequest OK"));
    }
  }

  if (!handled && _notFoundHandler)
  {
    _notFoundHandler();
    handled = true;
  }

  if (!handled)
  {
    using namespace mime;

    send(404, mimeTable[html].mimeType, String("Not found: ") + _currentUri);
    handled = true;
  }

  if (handled)
  {
    _finalizeResponse();
  }

  _currentUri = String("");

}

////////////////////////////////////////

void ESP8266_AT_WebServer::_finalizeResponse()
{
  if (_chunked)
  {
    sendContent(String());
  }
}

////////////////////////////////////////

String ESP8266_AT_WebServer::_responseCodeToString(int code)
{
  switch (code)
  {
    case 100:
      return F("Continue");

    case 101:
      return F("Switching Protocols");

    case 200:
      return F("OK");

    case 201:
      return F("Created");

    case 202:
      return F("Accepted");

    case 203:
      return F("Non-Authoritative Information");

    case 204:
      return F("No Content");

    case 205:
      return F("Reset Content");

    case 206:
      return F("Partial Content");

    case 300:
      return F("Multiple Choices");

    case 301:
      return F("Moved Permanently");

    case 302:
      return F("Found");

    case 303:
      return F("See Other");

    case 304:
      return F("Not Modified");

    case 305:
      return F("Use Proxy");

    case 307:
      return F("Temporary Redirect");

    case 400:
      return F("Bad Request");

    case 401:
      return F("Unauthorized");

    case 402:
      return F("Payment Required");

    case 403:
      return F("Forbidden");

    case 404:
      return F("Not Found");

    case 405:
      return F("Method Not Allowed");

    case 406:
      return F("Not Acceptable");

    case 407:
      return F("Proxy Authentication Required");

    case 408:
      return F("Request Time-out");

    case 409:
      return F("Conflict");

    case 410:
      return F("Gone");

    case 411:
      return F("Length Required");

    case 412:
      return F("Precondition Failed");

    case 413:
      return F("Request Entity Too Large");

    case 414:
      return F("Request-URI Too Large");

    case 415:
      return F("Unsupported Media Type");

    case 416:
      return F("Requested range not satisfiable");

    case 417:
      return F("Expectation Failed");

    case 500:
      return F("Internal Server Error");

    case 501:
      return F("Not Implemented");

    case 502:
      return F("Bad Gateway");

    case 503:
      return F("Service Unavailable");

    case 504:
      return F("Gateway Time-out");

    case 505:
      return F("HTTP Version not supported");

    default:
      return "";
  }
}

////////////////////////////////////////

#endif //ESP8266_AT_WebServer_impl_h
