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

#ifndef ESP8266_AT_Client_impl_h
#define ESP8266_AT_Client_impl_h

#include <inttypes.h>

#include "ESP8266_AT.h"

#include "ESP8266_AT_Server.h"

#include "utility/ESP8266_AT_Drv.h"
#include "utility/ESP8266_AT_Debug.h"

////////////////////////////////////////

ESP8266_AT_Client::ESP8266_AT_Client() : _sock(255)
{
}

////////////////////////////////////////

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

////////////////////////////////////////

int ESP8266_AT_Client::connectSSL(IPAddress ip, uint16_t port)
{
  char s[16];

  sprintf_P(s, PSTR("%d.%d.%d.%d"), ip[0], ip[1], ip[2], ip[3]);
  //sprintf(s, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);

  return connect(s, port, SSL_MODE);
}

////////////////////////////////////////

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

////////////////////////////////////////

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

////////////////////////////////////////

size_t ESP8266_AT_Client::write(uint8_t b)
{
  return write(&b, 1);
}

////////////////////////////////////////

// KH rewrite to enable chunk-sending for large file

#define AT_CLIENT_MAX_WRITE_RETRY       100

////////////////////////////////////////

size_t ESP8266_AT_Client::write(const uint8_t *buf, size_t size)
{
  int written = 0;
  int retry = AT_CLIENT_MAX_WRITE_RETRY;

  size_t totalBytesSent = 0;
  size_t bytesRemaining = size;

  uint16_t AT_CLIENT_SEND_MAX_SIZE = 2048;

  AT_LOGINFO3("ESP8266_AT_Client::write: size = ", size, ", MAX_SIZE =", AT_CLIENT_SEND_MAX_SIZE);

  if ( (_sock >= MAX_SOCK_NUM) || (size == 0) || buf[0] == 0 )
  {
    setWriteError();

    return 0;
  }

  while (retry)
  {
    //written =  Ethernet.socketSend(_sockindex, buf, min(bytesRemaining, (size_t) AT_CLIENT_SEND_MAX_SIZE) );
    if ( ESP8266_AT_Drv::sendData(_sock, buf, min(bytesRemaining, (size_t) AT_CLIENT_SEND_MAX_SIZE) ) )
    {
      written =  min(bytesRemaining, (size_t) AT_CLIENT_SEND_MAX_SIZE);
    }
    else
      written =  0;

    if (written > 0)
    {
      totalBytesSent += written;

      AT_LOGINFO3("ESP8266_AT_Client::write: written = ", written, ", totalBytesSent =", totalBytesSent);

      if (totalBytesSent >= size)
      {
        AT_LOGINFO3("ESP8266_AT_Client::write: Done, written = ", written, ", totalBytesSent =", totalBytesSent);

        //completed successfully
        retry = 0;
      }
      else
      {
        buf += written;
        bytesRemaining -= written;
        retry = AT_CLIENT_MAX_WRITE_RETRY;

        AT_LOGINFO3("ESP8266_AT_Client::write: Partially Done, written = ", written, ", bytesRemaining =", bytesRemaining);
      }
    }
    else if (written < 0)
    {
      AT_LOGINFO("ESP8266_AT_Client::write: written error");

      setWriteError();

      written = 0;
      retry = 0;
    }

    // Looping
  }

  setWriteError();

  return 0;
}

////////////////////////////////////////

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

////////////////////////////////////////

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

////////////////////////////////////////

int ESP8266_AT_Client::read(uint8_t* buf, size_t size)
{
  if (!available())
    return -1;

  return ESP8266_AT_Drv::getDataBuf(_sock, buf, size);
}

////////////////////////////////////////

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

////////////////////////////////////////

void ESP8266_AT_Client::flush()
{
  while (available())
    read();
}

////////////////////////////////////////

void ESP8266_AT_Client::stop()
{
  if (_sock == 255)
    return;

  AT_LOGINFO1(F("Disconnecting "), _sock);

  ESP8266_AT_Drv::stopClient(_sock);

  ESP8266_AT_Class::releaseSocket(_sock);
  _sock = 255;
}

////////////////////////////////////////

uint8_t ESP8266_AT_Client::connected()
{
  return (status() == ESTABLISHED);
}

////////////////////////////////////////

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

////////////////////////////////////////

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

////////////////////////////////////////

#endif    //ESP8266_AT_Client_impl_h
