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

ESP8266_AT_WebServer::ESP8266_AT_WebServer(int port)
  : _server(port)
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

void ESP8266_AT_WebServer::begin() 
{
  _currentStatus = HC_NONE;
  _server.begin();
  
  if (!_headerKeysCount)
    collectHeaders(0, 0);
}

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

void ESP8266_AT_WebServer::requestAuthentication() 
{
  sendHeader("WWW-Authenticate", "Basic realm=\"Login Required\"");
  send(401);
}

void ESP8266_AT_WebServer::on(const String &uri, ESP8266_AT_WebServer::THandlerFunction handler) 
{
  on(uri, HTTP_ANY, handler);
}

void ESP8266_AT_WebServer::on(const String &uri, HTTPMethod method, ESP8266_AT_WebServer::THandlerFunction fn) 
{
  on(uri, method, fn, _fileUploadHandler);
}

void ESP8266_AT_WebServer::on(const String &uri, HTTPMethod method, ESP8266_AT_WebServer::THandlerFunction fn, ESP8266_AT_WebServer::THandlerFunction ufn) 
{
  _addRequestHandler(new FunctionRequestHandler(fn, ufn, uri, method));
}

void ESP8266_AT_WebServer::addHandler(RequestHandler* handler) 
{
  _addRequestHandler(handler);
}

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

//KH
#if USE_NEW_WEBSERVER_VERSION

void ESP8266_AT_WebServer::handleClient() 
{
  if (_currentStatus == HC_NONE) 
  {
    ESP8266_AT_Client client = _server.available();
    
    if (!client) 
    {
      //AT_LOGDEBUG(F("handleClient: No Client"));
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

          if (_currentClient.connected()) 
          {
            _currentStatus = HC_WAIT_CLOSE;
            _statusChange = millis();
            keepCurrentClient = true;
          }
        }
      } 
      else 
      { // !_currentClient.available()
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
  
  // KH, fix bug. Have to close the connection
  _currentClient.stop();
  AT_LOGDEBUG(F("handleClient: Client disconnected"));
}
  
#else

void ESP8266_AT_WebServer::handleClient() 
{
  if (_currentStatus == HC_NONE)
  {
    ESP8266_AT_Client client = _server.available();
    
    if (!client)
    {
      //LOGINFO(F("handleClient:No client"));
      return;
    }

    AT_LOGDEBUG(F("handleClient: New Client"));

    _currentClient = client;
    _currentStatus = HC_WAIT_READ;
    _statusChange = millis();
  }

  if (!_currentClient.connected())
  {
    AT_LOGDEBUG(F("handleClient: Client not connected"));
    _currentClient = ESP8266_AT_Client();
    _currentStatus = HC_NONE;
    return;
  }

  // Wait for data from client to become available
  if (_currentStatus == HC_WAIT_READ)
  {
    AT_LOGDEBUG(F("handleClient: _currentStatus = HC_WAIT_READ"));
    
    if (!_currentClient.available())
    {
      AT_LOGDEBUG(F("handleClient: Client not available"));
      
      if (millis() - _statusChange > HTTP_MAX_DATA_WAIT)
      {
        AT_LOGDEBUG(F("handleClient: HTTP_MAX_DATA_WAIT Timeout"));
        
        _currentClient = ESP8266_AT_Client();
        _currentStatus = HC_NONE;
      }
      
      yield();
      return;
    }

    AT_LOGDEBUG(F("handleClient: Parsing Request"));
    
    if (!_parseRequest(_currentClient))
    {
      AT_LOGDEBUG(F("handleClient: Can't parse request"));
      
      _currentClient = ESP8266_AT_Client();
      _currentStatus = HC_NONE;
      return;
    }
    
    _currentClient.setTimeout(HTTP_MAX_SEND_WAIT);
    _contentLength = CONTENT_LENGTH_NOT_SET;
    
    //AT_LOGDEBUG(F("handleClient _handleRequest"));
    _handleRequest();

    if (!_currentClient.connected())
    {
      AT_LOGDEBUG(F("handleClient: Connection closed"));
      
      _currentClient = ESP8266_AT_Client();
      _currentStatus = HC_NONE;
      return;
    }
    else
    {
      _currentStatus = HC_WAIT_CLOSE;
      _statusChange = millis();
      return;
    }
  }

  if (_currentStatus == HC_WAIT_CLOSE)
  {
    if (millis() - _statusChange > HTTP_MAX_CLOSE_WAIT)
    {
      _currentClient = ESP8266_AT_Client();
      _currentStatus = HC_NONE;
      
      AT_LOGDEBUG(F("handleClient: HTTP_MAX_CLOSE_WAIT Timeout"));
      
      yield();
    }
    else
    {
      yield();
      return;
    }
  }
  
  // KH, fix bug. Have to close the connection
  _currentClient.stop();
  AT_LOGDEBUG(F("handleClient: Client disconnected"));
}

#endif

void ESP8266_AT_WebServer::close() 
{
  // TODO: Write close method for ESP8266_AT library and uncomment this
  //_server.close();
}

void ESP8266_AT_WebServer::stop() 
{
  close();
}

void ESP8266_AT_WebServer::sendHeader(const String& name, const String& value, bool first) 
{
  String headerLine = name;
  headerLine += ": ";
  headerLine += value;
  headerLine += "\r\n";

  if (first) 
  {
    _responseHeaders = headerLine + _responseHeaders;
  }
  else 
  {
    _responseHeaders += headerLine;
  }
}

void ESP8266_AT_WebServer::setContentLength(size_t contentLength) 
{
  _contentLength = contentLength;
}

void ESP8266_AT_WebServer::_prepareHeader(String& response, int code, const char* content_type, size_t contentLength) 
{
  response = "HTTP/1." + String(_currentVersion) + " ";
  response += String(code);
  response += " ";
  response += _responseCodeToString(code);
  response += "\r\n";

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
  { //HTTP/1.1 or above client
    //let's do chunked
    _chunked = true;
    sendHeader("Accept-Ranges", "none");
    sendHeader("Transfer-Encoding", "chunked");
  }

  AT_LOGDEBUG(F("_prepareHeader sendHeader Conn close"));

  sendHeader("Connection", "close");

  response += _responseHeaders;
  response += "\r\n";
    
  //MR & KH fix
  //_responseHeaders = String();
  _responseHeaders = *(new String());
}

void ESP8266_AT_WebServer::send(int code, const char* content_type, const String& content) 
{
  String header;
  
  // Can we asume the following?
  //if(code == 200 && content.length() == 0 && _contentLength == CONTENT_LENGTH_NOT_SET)
  //  _contentLength = CONTENT_LENGTH_UNKNOWN;

  AT_LOGDEBUG1(F("send1: len = "), content.length());
  AT_LOGDEBUG1(F("content = "), content);

  _prepareHeader(header, code, content_type, content.length());

  _currentClient.write((const uint8_t *)header.c_str(), header.length());

  if (content.length())
  {
    AT_LOGDEBUG1(F("send1: write header = "), header);
    //sendContent(content);
    sendContent(content, content.length());
  }
}

void ESP8266_AT_WebServer::send(int code, char* content_type, const String& content, size_t contentLength)
{
  String header;

  AT_LOGDEBUG1(F("send2: len = "), contentLength);
  AT_LOGDEBUG1(F("content = "), content);

  char type[64];
  
  memccpy((void*)type, content_type, 0, sizeof(type));
  _prepareHeader(header, code, (const char* )type, contentLength);

  AT_LOGDEBUG1(F("send2: hdrlen = "), header.length());
  AT_LOGDEBUG1(F("header = "), header);

  _currentClient.write((const uint8_t *) header.c_str(), header.length());
  
  if (contentLength)
  {
    sendContent(content, contentLength);
  }
}

void ESP8266_AT_WebServer::send(int code, char* content_type, const String& content) 
{
  send(code, (const char*)content_type, content);
}

void ESP8266_AT_WebServer::send(int code, const String& content_type, const String& content) 
{
  send(code, (const char*)content_type.c_str(), content);
}

void ESP8266_AT_WebServer::sendContent(const String& content) 
{
  const char * footer = "\r\n";
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

void ESP8266_AT_WebServer::sendContent(const String& content, size_t size)
{
  const char * footer = "\r\n";
  
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

void ESP8266_AT_WebServer::send_P(int code, PGM_P content_type, PGM_P content, size_t contentLength) 
{
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
    sendContent_P(content, contentLength);
  }
}

void ESP8266_AT_WebServer::sendContent_P(PGM_P content) 
{
  sendContent_P(content, strlen_P(content));
}

void ESP8266_AT_WebServer::sendContent_P(PGM_P content, size_t size) 
{
  const char * footer = "\r\n";
  
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
//////


String ESP8266_AT_WebServer::arg(String name) 
{
  for (int i = 0; i < _currentArgCount; ++i) 
  {
    if ( _currentArgs[i].key == name )
      return _currentArgs[i].value;
  }
  
  return String();
}

String ESP8266_AT_WebServer::arg(int i) 
{
  if (i < _currentArgCount)
    return _currentArgs[i].value;
    
  return String();
}

String ESP8266_AT_WebServer::argName(int i) 
{
  if (i < _currentArgCount)
    return _currentArgs[i].key;
    
  return String();
}

int ESP8266_AT_WebServer::args() 
{
  return _currentArgCount;
}

bool ESP8266_AT_WebServer::hasArg(String  name) 
{
  for (int i = 0; i < _currentArgCount; ++i) 
  {
    if (_currentArgs[i].key == name)
      return true;
  }
  
  return false;
}


String ESP8266_AT_WebServer::header(String name) 
{
  for (int i = 0; i < _headerKeysCount; ++i) 
  {
    if (_currentHeaders[i].key == name)
      return _currentHeaders[i].value;
  }
  
  return String();
}

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

String ESP8266_AT_WebServer::header(int i) 
{
  if (i < _headerKeysCount)
    return _currentHeaders[i].value;
    
  return String();
}

String ESP8266_AT_WebServer::headerName(int i) 
{
  if (i < _headerKeysCount)
    return _currentHeaders[i].key;
    
  return String();
}

int ESP8266_AT_WebServer::headers() 
{
  return _headerKeysCount;
}

bool ESP8266_AT_WebServer::hasHeader(String name) 
{
  for (int i = 0; i < _headerKeysCount; ++i) 
  {
    if ((_currentHeaders[i].key == name) &&  (_currentHeaders[i].value.length() > 0))
      return true;
  }
  
  return false;
}

String ESP8266_AT_WebServer::hostHeader() 
{
  return _hostHeader;
}

void ESP8266_AT_WebServer::onFileUpload(THandlerFunction fn) 
{
  _fileUploadHandler = fn;
}

void ESP8266_AT_WebServer::onNotFound(THandlerFunction fn) 
{
  _notFoundHandler = fn;
}

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
  
  //MR & KH fix
  //_currentUri = String();
  _currentUri = *(new String());
  
}

void ESP8266_AT_WebServer::_finalizeResponse() 
{
  if (_chunked) 
  {
    sendContent(String());
  }
}

String ESP8266_AT_WebServer::_responseCodeToString(int code) {
  switch (code) {
    case 100: return F("Continue");
    case 101: return F("Switching Protocols");
    case 200: return F("OK");
    case 201: return F("Created");
    case 202: return F("Accepted");
    case 203: return F("Non-Authoritative Information");
    case 204: return F("No Content");
    case 205: return F("Reset Content");
    case 206: return F("Partial Content");
    case 300: return F("Multiple Choices");
    case 301: return F("Moved Permanently");
    case 302: return F("Found");
    case 303: return F("See Other");
    case 304: return F("Not Modified");
    case 305: return F("Use Proxy");
    case 307: return F("Temporary Redirect");
    case 400: return F("Bad Request");
    case 401: return F("Unauthorized");
    case 402: return F("Payment Required");
    case 403: return F("Forbidden");
    case 404: return F("Not Found");
    case 405: return F("Method Not Allowed");
    case 406: return F("Not Acceptable");
    case 407: return F("Proxy Authentication Required");
    case 408: return F("Request Time-out");
    case 409: return F("Conflict");
    case 410: return F("Gone");
    case 411: return F("Length Required");
    case 412: return F("Precondition Failed");
    case 413: return F("Request Entity Too Large");
    case 414: return F("Request-URI Too Large");
    case 415: return F("Unsupported Media Type");
    case 416: return F("Requested range not satisfiable");
    case 417: return F("Expectation Failed");
    case 500: return F("Internal Server Error");
    case 501: return F("Not Implemented");
    case 502: return F("Bad Gateway");
    case 503: return F("Service Unavailable");
    case 504: return F("Gateway Time-out");
    case 505: return F("HTTP Version not supported");
    default:  return "";
  }
}

#endif //ESP8266_AT_WebServer_impl_h
