/****************************************************************************************************************************
  ESP8266_AT_Client.h - Dead simple web-server.
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

#ifndef ESP8266_AT_Client_h
#define ESP8266_AT_Client_h

////////////////////////////////////////

#include "Arduino.h"
#include "Print.h"
#include "Client.h"
#include "IPAddress.h"

////////////////////////////////////////

class ESP8266_AT_Client : public Client
{
  public:
    ESP8266_AT_Client();
    ESP8266_AT_Client(uint8_t sock);

    // override Print.print method

    size_t print(const __FlashStringHelper *ifsh);
    size_t println(const __FlashStringHelper *ifsh);

    /*
      Connect to the specified IP address and port. The return value indicates success or failure.
      Returns true if the connection succeeds, false if not.
    */
    virtual int connect(IPAddress ip, uint16_t port);

    /*
      Connect to the specified host and port. The return value indicates success or failure.
      Returns true if the connection succeeds, false if not.
    */
    virtual int connect(const char *host, uint16_t port);

    /*
      Connect to the specified IP address and port using SSL. The return value indicates success or failure.
      Returns true if the connection succeeds, false if not.
    */
    int connectSSL(IPAddress ip, uint16_t port);

    /*
      Connect to the specified host and port using SSL. The return value indicates success or failure.
      Returns true if the connection succeeds, false if not.
    */
    int connectSSL(const char* host, uint16_t port);

    /*
      Write a character to the server the client is connected to.
      Returns the number of characters written.
    */
    virtual size_t write(uint8_t);

    /*
      Write data to the server the client is connected to.
      Returns the number of characters written.
    */
    virtual size_t write(const uint8_t *buf, size_t size);

    virtual int available();

    /*
      Read the next byte received from the server the client is connected to (after the last call to read()).
      Returns the next byte (or character), or -1 if none is available.
    */
    virtual int read();


    virtual int read(uint8_t *buf, size_t size);

    /*
      Returns the next byte (character) of incoming serial data without removing it from the internal serial buffer.
    */
    virtual int peek();

    /*
      Discard any bytes that have been written to the client but not yet read.
    */
    virtual void flush();

    /*
      Disconnect from the server.
    */
    virtual void stop();

    /*
      Whether or not the client is connected.
      Note that a client is considered connected if the connection has been closed but there is still unread data.
      Returns true if the client is connected, false if not.
    */
    virtual uint8_t connected();


    uint8_t status();

    virtual operator bool();

    // needed to correctly handle overriding
    // see http://stackoverflow.com/questions/888235/overriding-a-bases-overloaded-function-in-c
    using Print::write;
    using Print::print;
    using Print::println;

    /*
      Returns the remote IP address.
    */
    IPAddress remoteIP();

    friend class ESP8266_AT_Server;

  private:

    uint8_t _sock;     // connection id

    int connect(const char* host, uint16_t port, uint8_t protMode);

    size_t printFSH(const __FlashStringHelper *ifsh, bool appendCrLf);
};

////////////////////////////////////////

#include "ESP8266_AT_Client-impl.h"

#endif
