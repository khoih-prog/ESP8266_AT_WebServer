/****************************************************************************************************************************
  RingBuffer.h - Dead simple web-server.
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

#ifndef RingBuffer_h
#define RingBuffer_h

////////////////////////////////////////

class AT_RingBuffer
{
  public:

    AT_RingBuffer(unsigned int size);
    ~AT_RingBuffer();

    void reset();
    void init();
    void push(char c);
    int getPos();
    bool endsWith(const char* str);
    void getStr(char * destination, unsigned int skipChars);
    void getStrN(char * destination, unsigned int skipChars, unsigned int num);

  private:

    unsigned int _size;
    char* ringBuf;
    char* ringBufEnd;
    char* ringBufP;
};

////////////////////////////////////////

#endif    //RingBuffer_h
