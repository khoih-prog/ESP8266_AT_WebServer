/****************************************************************************************************************************
   RingBuffer.cpp - Dead simple web-server.
   For ESP8266 AT-command running shields

   ESP_AT_WebServer is a library for the ESP8266 AT-command shields to run WebServer

   Forked and modified from Arduino ESP_AT library
   Built by Khoi Hoang https://github.com/khoih-prog/ESP_AT_WebServer
   Licensed under MIT license
   Version: 1.0.3

   Version Modified By   Date      Comments
   ------- -----------  ---------- -----------
    1.0.0   K Hoang      12/02/2020 Initial coding for Arduino Mega, Teensy, etc
    1.0.1   K Hoang      17/02/2020 Add support to server's lambda function calls
    1.0.2   K Hoang      22/02/2020 Add support to SAMD (DUE, ZERO, MKR, NANO_33_IOT, M0, M0 Pro, AdaFruit, etc) boards
    1.0.3   K Hoang      03/03/2020 Add support to STM32 (STM32,F0,F1, F2, F3, F4, F7, etc) boards
 *****************************************************************************************************************************/

#include "RingBuffer.h"

#include <Arduino.h>

AT_RingBuffer::AT_RingBuffer(unsigned int size)
{
  _size = size;
  // add one char to terminate the string
  ringBuf = new char[size + 1];
  ringBufEnd = &ringBuf[size];
  init();
}

AT_RingBuffer::~AT_RingBuffer() {}

void AT_RingBuffer::reset()
{
  ringBufP = ringBuf;
}

void AT_RingBuffer::init()
{
  ringBufP = ringBuf;
  memset(ringBuf, 0, _size + 1);
}

void AT_RingBuffer::push(char c)
{
  *ringBufP = c;
  ringBufP++;
  if (ringBufP >= ringBufEnd)
    ringBufP = ringBuf;
}

bool AT_RingBuffer::endsWith(const char* str)
{
  int findStrLen = strlen(str);

  // b is the start position into the ring buffer
  char* b = ringBufP - findStrLen;
  if (b < ringBuf)
    b = b + _size;

  char *p1 = (char*)&str[0];
  char *p2 = p1 + findStrLen;

  for (char *p = p1; p < p2; p++)
  {
    if (*p != *b)
      return false;

    b++;
    if (b == ringBufEnd)
      b = ringBuf;
  }

  return true;
}

void AT_RingBuffer::getStr(char * destination, unsigned int skipChars)
{
  //int len = ringBufP-ringBuf-skipChars;
  unsigned int len = ringBufP - ringBuf - skipChars;

  // copy buffer to destination string
  strncpy(destination, ringBuf, len);

  // terminate output string
  //destination[len]=0;
}

void AT_RingBuffer::getStrN(char * destination, unsigned int skipChars, unsigned int num)
{
  //int len = ringBufP-ringBuf-skipChars;
  unsigned int len = ringBufP - ringBuf - skipChars;

  if (len > num)
    len = num;

  // copy buffer to destination string
  strncpy(destination, ringBuf, len);

  // terminate output string
  //destination[len]=0;
}
