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

#ifndef ESP8266_AT_Client_h
#define ESP8266_AT_Client_h


#include "Arduino.h"
#include "Print.h"
#include "Client.h"
#include "IPAddress.h"

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

#include "ESP8266_AT_Client-impl.h"

#endif
