/****************************************************************************************************************************
  Parsing-impl.h - Dead simple web-server.
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

#ifndef ESP8266_AT_Parsing_impl_h
#define ESP8266_AT_Parsing_impl_h

////////////////////////////////////////

#include <Arduino.h>
#include <ESP8266_AT.h>
#include "ESP8266_AT_WebServer.h"

////////////////////////////////////////

#ifndef WEBSERVER_MAX_POST_ARGS
  #define WEBSERVER_MAX_POST_ARGS 32
#endif

////////////////////////////////////////

static bool readBytesWithTimeout(ESP8266_AT_Client& client, size_t maxLength, String& data, int timeout_ms)
{
  if (!data.reserve(maxLength + 1))
    return false;

  data[0] = 0;  // data.clear()??

  while (data.length() < maxLength)
  {
    int tries = timeout_ms;
    size_t avail;

    while (!(avail = client.available()) && tries--)
      delay(1);

    if (!avail)
      break;

    if (data.length() + avail > maxLength)
      avail = maxLength - data.length();

    while (avail--)
      data += (char)client.read();
  }

  return data.length() == maxLength;
}

////////////////////////////////////////

bool ESP8266_AT_WebServer::_parseRequest(ESP8266_AT_Client& client)
{
  // Read the first line of HTTP request
  String req = client.readStringUntil('\r');
  client.readStringUntil('\n');

  //reset header value
  for (int i = 0; i < _headerKeysCount; ++i)
  {
    _currentHeaders[i].value = String();
  }

  // First line of HTTP request looks like "GET /path HTTP/1.1"
  // Retrieve the "/path" part by finding the spaces
  int addr_start = req.indexOf(' ');
  int addr_end = req.indexOf(' ', addr_start + 1);

  if (addr_start == -1 || addr_end == -1)
  {
    AT_LOGDEBUG1(F("_parseRequest: Invalid request: "), req);
    return false;
  }

  AT_LOGDEBUG1(F("_parseRequest: request ="), req);

  String methodStr  = req.substring(0, addr_start);
  String url        = req.substring(addr_start + 1, addr_end);
  String versionEnd = req.substring(addr_end + 8);
  _currentVersion   = atoi(versionEnd.c_str());
  String searchStr  = "";
  int hasSearch     = url.indexOf('?');

  if (hasSearch != -1)
  {
    searchStr = urlDecode(url.substring(hasSearch + 1));
    url       = url.substring(0, hasSearch);
  }

  _currentUri = url;
  _chunked    = false;

  HTTPMethod method = HTTP_GET;

  if (methodStr == "HEAD")
  {
    method = HTTP_HEAD;
  }
  else if (methodStr == "POST")
  {
    method = HTTP_POST;
  }
  else if (methodStr == "DELETE")
  {
    method = HTTP_DELETE;
  }
  else if (methodStr == "OPTIONS")
  {
    method = HTTP_OPTIONS;
  }
  else if (methodStr == "PUT")
  {
    method = HTTP_PUT;
  }
  else if (methodStr == "PATCH")
  {
    method = HTTP_PATCH;
  }

  _currentMethod = method;

  AT_LOGDEBUG1(F("method: "), methodStr);
  AT_LOGDEBUG1(F("url: "), url);
  AT_LOGDEBUG1(F("search: "), searchStr);

  //attach handler
  RequestHandler* handler;

  for (handler = _firstHandler; handler; handler = handler->next())
  {
    if (handler->canHandle(_currentMethod, _currentUri))
      break;
  }

  _currentHandler = handler;

  String formData;

  // below is needed only when POST type request
  if (method == HTTP_POST || method == HTTP_PUT || method == HTTP_PATCH || method == HTTP_DELETE)
  {
    String boundaryStr;
    String headerName;
    String headerValue;

    bool isEncoded  = false;
    bool isForm     = false;

    uint32_t contentLength = 0;

    //parse headers
    while (1)
    {
      req = client.readStringUntil('\r');
      client.readStringUntil('\n');

      if (req == "")
      {
        AT_LOGDEBUG(F("POST_PUT_PATCH_DELETE: No more header"));

        break;//no more headers
      }

      int headerDiv = req.indexOf(':');

      if (headerDiv == -1)
      {
        AT_LOGDEBUG(F("headerDiv = -1"));

        break;
      }

      headerName  = req.substring(0, headerDiv);
      headerValue = req.substring(headerDiv + 1);

      headerValue.trim();
      _collectHeader(headerName.c_str(), headerValue.c_str());

      AT_LOGDEBUG1(F("headerName: "), headerName);
      AT_LOGDEBUG1(F("headerValue: "), headerValue);

      //KH
      if (headerName.equalsIgnoreCase("Content-Type"))
      {
        using namespace mime;

        if (headerValue.startsWith(mimeTable[txt].mimeType))
        {
          isForm = false;
        }
        else if (headerValue.startsWith("application/x-www-form-urlencoded"))
        {
          isForm = false;
          isEncoded = true;
        }
        else if (headerValue.startsWith("multipart/"))
        {
          boundaryStr = headerValue.substring(headerValue.indexOf('=') + 1);
          // KH
          boundaryStr.replace("\"", "");
          //
          isForm = true;
        }
      }
      //KH
      else if (headerName.equalsIgnoreCase("Content-Length"))
      {
        contentLength = headerValue.toInt();
      }
      //KH
      else if (headerName.equalsIgnoreCase("Host"))
      {
        _hostHeader = headerValue;
      }
    }

    String plainBuf;

    if (   !isForm
           && // read content into plainBuf
           (   !readBytesWithTimeout(client, contentLength, plainBuf, HTTP_MAX_POST_WAIT)
               || (plainBuf.length() < contentLength)
           )
       )
    {
      return false;
    }

    if (isEncoded)
    {
      // isEncoded => !isForm => plainBuf is not empty
      // add plainBuf in search str
      if (searchStr.length())
        searchStr += '&';

      searchStr += plainBuf;
    }

    // parse searchStr for key/value pairs
    _parseArguments(searchStr);

    if (!isForm)
    {
      if (contentLength)
      {
        // add key=value: plain={body} (post json or other data)
        RequestArgument& arg = _currentArgs[_currentArgCount++];
        arg.key = F("plain");
        arg.value = plainBuf;
      }
    }
    else
    {
      // isForm is true
      // here: content is not yet read (plainBuf is still empty)
      if (!_parseForm(client, boundaryStr, contentLength))
      {
        return false;
      }
    }
  }
  else
  {
    String headerName;
    String headerValue;

    //parse headers
    while (1)
    {
      AT_LOGDEBUG(F("GET_HEAD_OPTIONS: while start"));

      //req = client.readStringUntil('\r');
      //client.readStringUntil('\n');
      req = client.readStringUntil(0x0D);

      AT_LOGDEBUG1(F("GET_HEAD_OPTIONS: req ="), req);

      client.readStringUntil(0x0A);

      if (req == "")
      {
        AT_LOGDEBUG(F("GET_HEAD_OPTIONS: No more header"));

        break;//no more headers
      }

      int headerDiv = req.indexOf(':');

      if (headerDiv == -1)
      {
        AT_LOGDEBUG(F("headerDiv = -1"));

        break;
      }

      headerName  = req.substring(0, headerDiv);
      headerValue = req.substring(headerDiv + 2);
      _collectHeader(headerName.c_str(), headerValue.c_str());

      AT_LOGDEBUG1(F("headerName:"), headerName);
      AT_LOGDEBUG1(F("headerValue:"), headerValue);

      if (headerName.equalsIgnoreCase(F("Host")))
      {
        _hostHeader = headerValue;
      }
    }

    _parseArguments(searchStr);

    AT_LOGDEBUG(F("GET_HEAD_OPTIONS: while end"));
  }

  client.flush();

  AT_LOGDEBUG1(F("Request:"), url);
  AT_LOGDEBUG1(F("Arguments:"), searchStr);
  AT_LOGDEBUG (F("Final list of key/value pairs:"));

  for (int i = 0; i < _currentArgCount; i++)
  {
    AT_LOGDEBUG1("key:",   _currentArgs[i].key.c_str());
    AT_LOGDEBUG1("value:", _currentArgs[i].value.c_str());
  }

  return true;
}

////////////////////////////////////////

bool ESP8266_AT_WebServer::_collectHeader(const char* headerName, const char* headerValue)
{
  for (int i = 0; i < _headerKeysCount; i++)
  {
    //KH
    if (_currentHeaders[i].key.equalsIgnoreCase(headerName))
    {
      _currentHeaders[i].value = headerValue;
      return true;
    }
  }

  return false;
}

////////////////////////////////////////

struct storeArgHandler
{
  void operator() (String& key, String& value, const String& data, int equal_index, int pos, int key_end_pos,
                   int next_index)
  {
    key = ESP8266_AT_WebServer::urlDecode(data.substring(pos, key_end_pos));

    if ((equal_index != -1) && ((equal_index < next_index - 1) || (next_index == -1)))
      value = ESP8266_AT_WebServer::urlDecode(data.substring(equal_index + 1, next_index));
  }
};

////////////////////////////////////////

struct nullArgHandler
{
  void operator() (String& key, String& value, const String& data, int equal_index, int pos, int key_end_pos,
                   int next_index)
  {
    (void)key;
    (void)value;
    (void)data;
    (void)equal_index;
    (void)pos;
    (void)key_end_pos;
    (void)next_index;
    // do nothing
  }
};

////////////////////////////////////////

void ESP8266_AT_WebServer::_parseArguments(const String& data)
{
  if (_currentArgs)
    delete[] _currentArgs;

  _currentArgCount = _parseArgumentsPrivate(data, nullArgHandler());

  // allocate one more, this is needed because {"plain": plainBuf} is always added
  _currentArgs = new RequestArgument[_currentArgCount + 1];

  (void)_parseArgumentsPrivate(data, storeArgHandler());
}

////////////////////////////////////////

int ESP8266_AT_WebServer::_parseArgumentsPrivate(const String& data,
                                                 vl::Func<void(String&, String&, const String&, int, int, int, int)> handler)
{
  AT_LOGDEBUG1(F("args: "), data);

  size_t pos = 0;
  int arg_total = 0;

  while (true)
  {
    // skip empty expression
    while (data[pos] == '&' || data[pos] == ';')
      if (++pos >= data.length())
        break;

    // locate separators
    int equal_index = data.indexOf('=', pos);
    int key_end_pos = equal_index;
    int next_index  = data.indexOf('&', pos);
    int next_index2 = data.indexOf(';', pos);

    if ((next_index == -1) || (next_index2 != -1 && next_index2 < next_index))
      next_index = next_index2;

    if ((key_end_pos == -1) || ((key_end_pos > next_index) && (next_index != -1)))
      key_end_pos = next_index;

    if (key_end_pos == -1)
      key_end_pos = data.length();

    // handle key/value
    if ((int)pos < key_end_pos)
    {
      RequestArgument& arg = _currentArgs[arg_total];
      handler(arg.key, arg.value, data, equal_index, pos, key_end_pos, next_index);

      ++arg_total;
      pos = next_index + 1;
    }

    if (next_index == -1)
      break;
  }

  AT_LOGDEBUG1(F("args count: "), arg_total);

  return arg_total;
}

////////////////////////////////////////

void ESP8266_AT_WebServer::_uploadWriteByte(uint8_t b)
{
  if (_currentUpload->currentSize == HTTP_UPLOAD_BUFLEN)
  {
    if (_currentHandler && _currentHandler->canUpload(_currentUri))
      _currentHandler->upload(*this, _currentUri, *_currentUpload);

    _currentUpload->totalSize += _currentUpload->currentSize;
    _currentUpload->currentSize = 0;
  }

  _currentUpload->buf[_currentUpload->currentSize++] = b;
}

////////////////////////////////////////

uint8_t ESP8266_AT_WebServer::_uploadReadByte(ESP8266_AT_Client& client)
{
  int res = client.read();

  if (res == -1)
  {
    while (!client.available() && client.connected())
      yield();

    res = client.read();
  }

  return (uint8_t)res;
}

////////////////////////////////////////

bool ESP8266_AT_WebServer::_parseForm(ESP8266_AT_Client& client, const String& boundary, uint32_t len)
{
  (void) len;

  AT_LOGDEBUG1(F("Parse Form: Boundary: "), boundary);
  AT_LOGDEBUG1(F("Length: "), len);

  String line;
  int retry = 0;

  do
  {
    line = client.readStringUntil('\r');
    ++retry;
  } while (line.length() == 0 && retry < 3);

  client.readStringUntil('\n');

  //start reading the form
  if (line == ("--" + boundary))
  {
    if (_postArgs)
      delete[] _postArgs;

    _postArgs = new RequestArgument[WEBSERVER_MAX_POST_ARGS];
    _postArgsLen = 0;

    while (1)
    {
      String argName;
      String argValue;
      String argType;
      String argFilename;

      bool argIsFile = false;

      line = client.readStringUntil('\r');
      client.readStringUntil('\n');

      if (line.length() > 19 && line.substring(0, 19).equalsIgnoreCase(F("Content-Disposition")))
      {
        int nameStart = line.indexOf('=');

        if (nameStart != -1)
        {
          argName = line.substring(nameStart + 2);
          nameStart = argName.indexOf('=');

          if (nameStart == -1)
          {
            argName = argName.substring(0, argName.length() - 1);
          }
          else
          {
            argFilename = argName.substring(nameStart + 2, argName.length() - 1);
            argName = argName.substring(0, argName.indexOf('"'));
            argIsFile = true;

            AT_LOGDEBUG1(F("PostArg FileName: "), argFilename);

            //use GET to set the filename if uploading using blob
            if (argFilename == F("blob") && hasArg("filename"))
              argFilename = arg("filename");
          }

          AT_LOGDEBUG1(F("PostArg Name: "), argName);

          using namespace mime;

          argType = mimeTable[txt].mimeType;
          line    = client.readStringUntil('\r');
          client.readStringUntil('\n');

          if (line.length() > 12 && line.substring(0, 12).equalsIgnoreCase("Content-Type"))
          {
            argType = line.substring(line.indexOf(':') + 2);
            //skip next line
            client.readStringUntil('\r');
            client.readStringUntil('\n');
          }

          AT_LOGDEBUG1(F("PostArg Type: "), argType);

          if (!argIsFile)
          {
            while (1)
            {
              line = client.readStringUntil('\r');
              client.readStringUntil('\n');

              if (line.startsWith("--" + boundary))
                break;

              if (argValue.length() > 0)
                argValue += "\n";

              argValue += line;
            }

            AT_LOGDEBUG1(F("PostArg Value: "), argValue);

            RequestArgument& arg = _postArgs[_postArgsLen++];
            arg.key   = argName;
            arg.value = argValue;

            if (line == ("--" + boundary + "--"))
            {
              AT_LOGDEBUG(F("Done Parsing POST"));

              break;
            }
          }
          else
          {
            //_currentUpload.reset(new HTTPUpload());
            if (!_currentUpload)
              _currentUpload = new HTTPUpload();

            _currentUpload->status        = UPLOAD_FILE_START;
            _currentUpload->name          = argName;
            _currentUpload->filename      = argFilename;
            _currentUpload->type          = argType;
            _currentUpload->totalSize     = 0;
            _currentUpload->currentSize   = 0;
            _currentUpload->contentLength = len;

            AT_LOGDEBUG1(F("Start File: "), _currentUpload->filename);
            AT_LOGDEBUG1(F("Type: "), _currentUpload->type);

            if (_currentHandler && _currentHandler->canUpload(_currentUri))
              _currentHandler->upload(*this, _currentUri, *_currentUpload);

            _currentUpload->status  = UPLOAD_FILE_WRITE;
            uint8_t argByte         = _uploadReadByte(client);

readfile:

            while (argByte != 0x0D)
            {
              if (!client.connected())
                return _parseFormUploadAborted();

              _uploadWriteByte(argByte);
              argByte = _uploadReadByte(client);
            }

            argByte = _uploadReadByte(client);

            if (!client.connected())
              return _parseFormUploadAborted();

            if (argByte == 0x0A)
            {
              argByte = _uploadReadByte(client);

              if (!client.connected())
                return _parseFormUploadAborted();

              if ((char)argByte != '-')
              {
                //continue reading the file
                _uploadWriteByte(0x0D);
                _uploadWriteByte(0x0A);
                goto readfile;
              }
              else
              {
                argByte = _uploadReadByte(client);

                if (!client.connected())
                  return _parseFormUploadAborted();

                if ((char)argByte != '-')
                {
                  //continue reading the file
                  _uploadWriteByte(0x0D);
                  _uploadWriteByte(0x0A);
                  _uploadWriteByte((uint8_t)('-'));
                  goto readfile;
                }
              }

              uint8_t endBuf[boundary.length()];
              client.readBytes(endBuf, boundary.length());

              if (strstr((const char*)endBuf, boundary.c_str()) != NULL)
              {
                if (_currentHandler && _currentHandler->canUpload(_currentUri))
                  _currentHandler->upload(*this, _currentUri, *_currentUpload);

                _currentUpload->totalSize  += _currentUpload->currentSize;
                _currentUpload->status      = UPLOAD_FILE_END;

                if (_currentHandler && _currentHandler->canUpload(_currentUri))
                  _currentHandler->upload(*this, _currentUri, *_currentUpload);

                AT_LOGDEBUG1(F("End File: "), _currentUpload->filename);
                AT_LOGDEBUG1(F("Type: "), _currentUpload->type);
                AT_LOGDEBUG1(F("Size: "), _currentUpload->totalSize);

                line = client.readStringUntil(0x0D);
                client.readStringUntil(0x0A);

                if (line == "--")
                {
                  AT_LOGDEBUG(F("Done Parsing POST"));
                  break;
                }

                continue;
              }
              else
              {
                _uploadWriteByte(0x0D);
                _uploadWriteByte(0x0A);
                _uploadWriteByte((uint8_t)('-'));
                _uploadWriteByte((uint8_t)('-'));

                uint32_t i = 0;

                while (i < boundary.length())
                {
                  _uploadWriteByte(endBuf[i++]);
                }

                argByte = _uploadReadByte(client);
                goto readfile;
              }
            }
            else
            {
              _uploadWriteByte(0x0D);
              goto readfile;
            }

            break;
          }
        }
      }
    }

    int iarg;
    int totalArgs = ((WEBSERVER_MAX_POST_ARGS - _postArgsLen) < _currentArgCount) ? (WEBSERVER_MAX_POST_ARGS - _postArgsLen)
                    : _currentArgCount;

    for (iarg = 0; iarg < totalArgs; iarg++)
    {
      RequestArgument& arg = _postArgs[_postArgsLen++];
      arg.key   = _currentArgs[iarg].key;
      arg.value = _currentArgs[iarg].value;
    }

    if (_currentArgs)
      delete[] _currentArgs;

    _currentArgs = new RequestArgument[_postArgsLen];

    for (iarg = 0; iarg < _postArgsLen; iarg++)
    {
      RequestArgument& arg = _currentArgs[iarg];
      arg.key = _postArgs[iarg].key;
      arg.value = _postArgs[iarg].value;
    }

    _currentArgCount = iarg;

    if (_postArgs)
    {
      delete[] _postArgs;
      _postArgs = nullptr;
      _postArgsLen = 0;
    }

    return true;
  }

  AT_LOGDEBUG1(F("Error: line: "), line);

  return false;
}

////////////////////////////////////////

bool ESP8266_AT_WebServer::_parseFormUploadAborted()
{
  _currentUpload->status = UPLOAD_FILE_ABORTED;

  if (_currentHandler && _currentHandler->canUpload(_currentUri))
    _currentHandler->upload(*this, _currentUri, *_currentUpload);

  return false;
}

////////////////////////////////////////

String ESP8266_AT_WebServer::urlDecode(const String& text)
{
  String decoded    = "";
  char temp[]       = "0x00";
  unsigned int len  = text.length();
  unsigned int i    = 0;

  while (i < len)
  {
    char decodedChar;
    char encodedChar = text.charAt(i++);

    if ((encodedChar == '%') && (i + 1 < len))
    {
      temp[2] = text.charAt(i++);
      temp[3] = text.charAt(i++);

      decodedChar = strtol(temp, NULL, 16);
    }
    else
    {
      if (encodedChar == '+')
      {
        decodedChar = ' ';
      }
      else
      {
        decodedChar = encodedChar;  // normal ascii char
      }
    }

    decoded += decodedChar;
  }

  return decoded;
}

////////////////////////////////////////

#endif //ESP8266_AT_Parsing_impl_h
