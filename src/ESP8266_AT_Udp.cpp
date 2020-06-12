/****************************************************************************************************************************
   ESP8266_AT_UDP.h - Dead simple web-server.
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

#include "ESP8266_AT.h"
#include "ESP8266_AT_Udp.h"

#include "utility/ESP8266_AT_Drv.h"
#include "utility/ESP8266_AT_Debug.h"

/* Constructor */
ESP8266_AT_UDP::ESP8266_AT_UDP() : _sock(NO_SOCKET_AVAIL) {}

/* Start WiFiUDP socket, listening at local port PORT */

uint8_t ESP8266_AT_UDP::begin(uint16_t port)
{
  uint8_t sock = ESP8266_AT_Class::getFreeSocket();
  if (sock != NO_SOCKET_AVAIL)
  {
    ESP8266_AT_Drv::startClient("0", port, sock, UDP_MODE);

    ESP8266_AT_Class::allocateSocket(sock);  // allocating the socket for the listener
    ESP8266_AT_Class::_server_port[sock] = port;
    _sock = sock;
    _port = port;
    return 1;
  }
  return 0;

}

/* return number of bytes available in the current packet,
   will return zero if parsePacket hasn't been called yet */
int ESP8266_AT_UDP::available()
{
  if (_sock != NO_SOCKET_AVAIL)
  {
    int bytes = ESP8266_AT_Drv::availData(_sock);
    if (bytes > 0)
    {
      return bytes;
    }
  }

  return 0;
}

/* Release any resources being used by this WiFiUDP instance */
void ESP8266_AT_UDP::stop()
{
  if (_sock == NO_SOCKET_AVAIL)
    return;

  // Discard data that might be in the incoming buffer
  flush();

  // Stop the listener and return the socket to the pool
  ESP8266_AT_Drv::stopClient(_sock);
  ESP8266_AT_Class::_state[_sock] = NA_STATE;
  ESP8266_AT_Class::_server_port[_sock] = 0;

  _sock = NO_SOCKET_AVAIL;
}

int ESP8266_AT_UDP::beginPacket(const char *host, uint16_t port)
{
  if (_sock == NO_SOCKET_AVAIL)
    _sock = ESP8266_AT_Class::getFreeSocket();

  if (_sock != NO_SOCKET_AVAIL)
  {
    //ESP8266_AT_Drv::startClient(host, port, _sock, UDP_MODE);
    _remotePort = port;
    strcpy(_remoteHost, host);
    ESP8266_AT_Class::allocateSocket(_sock);
    return 1;
  }
  return 0;
}


int ESP8266_AT_UDP::beginPacket(IPAddress ip, uint16_t port)
{
  char s[18];
  sprintf_P(s, PSTR("%d.%d.%d.%d"), ip[0], ip[1], ip[2], ip[3]);

  return beginPacket(s, port);
}


int ESP8266_AT_UDP::endPacket()
{
  return 1; //ServerDrv::sendUdpData(_sock);
}

size_t ESP8266_AT_UDP::write(uint8_t byte)
{
  return write(&byte, 1);
}

size_t ESP8266_AT_UDP::write(const uint8_t *buffer, size_t size)
{
  bool r = ESP8266_AT_Drv::sendDataUdp(_sock, _remoteHost, _remotePort, buffer, size);
  if (!r)
  {
    return 0;
  }

  return size;
}

int ESP8266_AT_UDP::parsePacket()
{
  return available();
}

int ESP8266_AT_UDP::read()
{
  uint8_t b;
  if (!available())
    return -1;

  bool connClose = false;

  // Read the data and handle the timeout condition
  if (! ESP8266_AT_Drv::getData(_sock, &b, false, &connClose))
    return -1;  // Timeout occured

  return b;
}

int ESP8266_AT_UDP::read(uint8_t* buf, size_t size)
{
  if (!available())
    return -1;
  return ESP8266_AT_Drv::getDataBuf(_sock, buf, size);
}

int ESP8266_AT_UDP::peek()
{
  uint8_t b;
  if (!available())
    return -1;

  return b;
}

void ESP8266_AT_UDP::flush()
{
  // Discard all input data
  int count = available();
  while (count-- > 0)
    read();
}


IPAddress  ESP8266_AT_UDP::remoteIP()
{
  IPAddress ret;
  ESP8266_AT_Drv::getRemoteIpAddress(ret);
  return ret;
}

uint16_t  ESP8266_AT_UDP::remotePort()
{
  return ESP8266_AT_Drv::getRemotePort();
}



////////////////////////////////////////////////////////////////////////////////
// Private Methods
////////////////////////////////////////////////////////////////////////////////
