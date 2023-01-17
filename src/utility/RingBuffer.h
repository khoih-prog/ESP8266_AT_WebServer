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

  Version: 1.7.1

  Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
  1.0.0   K Hoang      12/02/2020 Initial coding for Arduino Mega, Teensy, etc
  ...
  1.6.0   K Hoang      16/11/2022 Fix severe limitation to permit sending larger data than 2K buffer. Add CORS
  1.7.0   K Hoang      16/01/2023 Add support to WizNet WizFi360 such as WIZNET_WIZFI360_EVB_PICO
  1.7.1   K Hoang      17/01/2023 Fix AP and version bugs for WizNet WizFi360
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
