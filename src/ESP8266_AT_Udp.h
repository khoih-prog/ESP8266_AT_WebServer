/****************************************************************************************************************************
  ESP8266_AT_UDP.h - Dead simple web-server.
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

#ifndef ESP8266_AT_UDP_h
#define ESP8266_AT_UDP_h

////////////////////////////////////////

#include <Udp.h>

////////////////////////////////////////

//#define UDP_TX_PACKET_MAX_SIZE 24
#define UDP_TX_PACKET_MAX_SIZE 1000

////////////////////////////////////////

class ESP8266_AT_UDP : public UDP
{
  private:
    uint8_t _sock;  // socket ID for Wiz5100
    uint16_t _port; // local port to listen on


    uint16_t _remotePort;
    char _remoteHost[30];

  public:
    ESP8266_AT_UDP();  // Constructor

    // initialize, start listening on specified port.
    // Returns 1 if successful, 0 if there are no sockets available to use
    virtual uint8_t begin(uint16_t);

    // KH, add to support MultiCast for v1.1.0
    // initialize, start listening on specified multicast IP address and port.
    // Returns 1 if successful, 0 if there are no sockets available to use
    virtual uint8_t beginMulticast(IPAddress, uint16_t);
    //////

    virtual void stop();  // Finish with the UDP socket

    // Sending UDP packets

    // Start building up a packet to send to the remote host specific in ip and port
    // Returns 1 if successful, 0 if there was a problem with the supplied IP address or port
    virtual int beginPacket(IPAddress ip, uint16_t port);

    // Start building up a packet to send to the remote host specific in host and port
    // Returns 1 if successful, 0 if there was a problem resolving the hostname or port
    virtual int beginPacket(const char *host, uint16_t port);

    // Finish off this packet and send it
    // Returns 1 if the packet was sent successfully, 0 if there was an error
    virtual int endPacket();

    // Write a single byte into the packet
    virtual size_t write(uint8_t);

    // Write size bytes from buffer into the packet
    virtual size_t write(const uint8_t *buffer, size_t size);

    using Print::write;

    // Start processing the next available incoming packet
    // Returns the size of the packet in bytes, or 0 if no packets are available
    virtual int parsePacket();

    // Number of bytes remaining in the current packet
    virtual int available();

    // Read a single byte from the current packet
    virtual int read();

    // Read up to len bytes from the current packet and place them into buffer
    // Returns the number of bytes read, or 0 if none are available
    virtual int read(unsigned char* buffer, size_t len);

    ////////////////////////////////////////

    // Read up to len characters from the current packet and place them into buffer
    // Returns the number of characters read, or 0 if none are available
    virtual int read(char* buffer, size_t len)
    {
      return read((unsigned char*)buffer, len);
    };

    ////////////////////////////////////////

    // Return the next byte from the current packet without moving on to the next byte
    virtual int peek();

    virtual void flush(); // Finish reading the current packet

    // Return the IP address of the host who sent the current incoming packet
    virtual IPAddress remoteIP();

    // Return the port of the host who sent the current incoming packet
    virtual uint16_t remotePort();


    friend class ESP8266_AT_Server;
};

////////////////////////////////////////

#include "ESP8266_AT_Udp-impl.h"

////////////////////////////////////////

#endif    //ESP8266_AT_UDP_h
