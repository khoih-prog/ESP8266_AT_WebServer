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

#ifndef ESP8266_AT_UDP_h
#define ESP8266_AT_UDP_h

#include <Udp.h>

#define UDP_TX_PACKET_MAX_SIZE 24

class ESP8266_AT_UDP : public UDP {
  private:
    uint8_t _sock;  // socket ID for Wiz5100
    uint16_t _port; // local port to listen on


    uint16_t _remotePort;
    char _remoteHost[30];


  public:
    ESP8266_AT_UDP();  // Constructor

    virtual uint8_t begin(uint16_t);	// initialize, start listening on specified port. Returns 1 if successful, 0 if there are no sockets available to use
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

    // Read up to len characters from the current packet and place them into buffer
    // Returns the number of characters read, or 0 if none are available
    virtual int read(char* buffer, size_t len) {
      return read((unsigned char*)buffer, len);
    };

    // Return the next byte from the current packet without moving on to the next byte
    virtual int peek();

    virtual void flush();	// Finish reading the current packet

    // Return the IP address of the host who sent the current incoming packet
    virtual IPAddress remoteIP();

    // Return the port of the host who sent the current incoming packet
    virtual uint16_t remotePort();


    friend class ESP8266_AT_Server;
};

#endif
