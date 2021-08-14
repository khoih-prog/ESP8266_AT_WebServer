/****************************************************************************************************************************
  ESP8266_AT_Debug.h - Dead simple web-server.
  For ESP8266/ESP32 AT-command running shields

  ESP8266_AT_WebServer is a library for the ESP8266/ESP32 AT-command shields to run WebServer
  Based on and modified from ESP8266 https://github.com/esp8266/Arduino/releases
  Built by Khoi Hoang https://github.com/khoih-prog/ESP8266_AT_WebServer
  Licensed under MIT license

  Original author:
  @file       Esp8266WebServer.h
  @author     Ivan Grokhotkov

  Version: 1.4.0

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
  1.4.0   K Hoang      14/08/2021 Add support to Adafruit nRF52 core v0.22.0+
 *****************************************************************************************************************************/

#ifndef ESP8266_AT_Debug_H
#define ESP8266_AT_Debug_H

#include <stdio.h>

#ifdef DEBUG_ESP8266_AT_WEBSERVER_PORT
  #define DEBUG_OUTPUT DEBUG_ESP8266_AT_WEBSERVER_PORT
#else
  #define DEBUG_OUTPUT Serial
#endif

// Change _ESP_AT_LOGLEVEL_ to set tracing and logging verbosity
// 0: DISABLED: no logging
// 1: ERROR: errors
// 2: WARN: errors and warnings
// 3: INFO: errors, warnings and informational (default)
// 4: DEBUG: errors, warnings, informational and debug

#ifndef _ESP_AT_LOGLEVEL_
  #define _ESP_AT_LOGLEVEL_       1
#endif


#define AT_LOGERROR(x)         if(_ESP_AT_LOGLEVEL_>0) { DEBUG_OUTPUT.print("[ESP_AT] "); DEBUG_OUTPUT.println(x); }
#define AT_LOGERROR0(x)        if(_ESP_AT_LOGLEVEL_>0) { DEBUG_OUTPUT.print(x); }
#define AT_LOGERROR1(x,y)      if(_ESP_AT_LOGLEVEL_>0) { DEBUG_OUTPUT.print("[ESP_AT] "); DEBUG_OUTPUT.print(x); DEBUG_OUTPUT.print(" "); DEBUG_OUTPUT.println(y); }
#define AT_LOGERROR2(x,y,z)    if(_ESP_AT_LOGLEVEL_>0) { DEBUG_OUTPUT.print(x); DEBUG_OUTPUT.print(" "); DEBUG_OUTPUT.print(y); DEBUG_OUTPUT.print(" "); DEBUG_OUTPUT.println(z); }
#define AT_LOGERROR3(x,y,z,w)  if(_ESP_AT_LOGLEVEL_>0) { DEBUG_OUTPUT.print("[ESP_AT] "); DEBUG_OUTPUT.print(x); DEBUG_OUTPUT.print(" "); DEBUG_OUTPUT.print(y); DEBUG_OUTPUT.print(" "); DEBUG_OUTPUT.print(z); DEBUG_OUTPUT.print(" "); DEBUG_OUTPUT.println(w);}

#define AT_LOGWARN(x)         if(_ESP_AT_LOGLEVEL_>1) { DEBUG_OUTPUT.print("[ESP_AT] "); DEBUG_OUTPUT.println(x); }
#define AT_LOGWARN0(x)        if(_ESP_AT_LOGLEVEL_>1) { DEBUG_OUTPUT.print(x); }
#define AT_LOGWARN1(x,y)      if(_ESP_AT_LOGLEVEL_>1) { DEBUG_OUTPUT.print("[ESP_AT] "); DEBUG_OUTPUT.print(x); DEBUG_OUTPUT.print(" "); DEBUG_OUTPUT.println(y); }
#define AT_LOGWARN2(x,y,z)    if(_ESP_AT_LOGLEVEL_>1) { DEBUG_OUTPUT.print(x); DEBUG_OUTPUT.print(" "); DEBUG_OUTPUT.print(y); DEBUG_OUTPUT.print(" "); DEBUG_OUTPUT.println(z); }
#define AT_LOGWARN3(x,y,z,w)  if(_ESP_AT_LOGLEVEL_>1) { DEBUG_OUTPUT.print("[ESP_AT] "); DEBUG_OUTPUT.print(x); DEBUG_OUTPUT.print(" "); DEBUG_OUTPUT.print(y); DEBUG_OUTPUT.print(" "); DEBUG_OUTPUT.print(z); DEBUG_OUTPUT.print(" "); DEBUG_OUTPUT.println(w);}

#define AT_LOGINFO(x)         if(_ESP_AT_LOGLEVEL_>2) { DEBUG_OUTPUT.print("[ESP_AT] "); DEBUG_OUTPUT.println(x); }
#define AT_LOGINFO0(x)        if(_ESP_AT_LOGLEVEL_>2) { DEBUG_OUTPUT.print(x); }
#define AT_LOGINFO1(x,y)      if(_ESP_AT_LOGLEVEL_>2) { DEBUG_OUTPUT.print("[ESP_AT] "); DEBUG_OUTPUT.print(x); DEBUG_OUTPUT.print(" "); DEBUG_OUTPUT.println(y); }
#define AT_LOGINFO2(x,y,z)    if(_ESP_AT_LOGLEVEL_>2) { DEBUG_OUTPUT.print(x); DEBUG_OUTPUT.print(" "); DEBUG_OUTPUT.print(y); DEBUG_OUTPUT.print(" "); DEBUG_OUTPUT.println(z); }
#define AT_LOGINFO3(x,y,z,w)  if(_ESP_AT_LOGLEVEL_>2) { DEBUG_OUTPUT.print("[ESP_AT] "); DEBUG_OUTPUT.print(x); DEBUG_OUTPUT.print(" "); DEBUG_OUTPUT.print(y); DEBUG_OUTPUT.print(" "); DEBUG_OUTPUT.print(z); DEBUG_OUTPUT.print(" "); DEBUG_OUTPUT.println(w);}

#define AT_LOGDEBUG(x)        if(_ESP_AT_LOGLEVEL_>3) { DEBUG_OUTPUT.print("[ESP_AT] "); DEBUG_OUTPUT.println(x); }
#define AT_LOGDEBUG0(x)       if(_ESP_AT_LOGLEVEL_>3) { DEBUG_OUTPUT.print(x); }
#define AT_LOGDEBUG1(x,y)     if(_ESP_AT_LOGLEVEL_>3) { DEBUG_OUTPUT.print("[ESP_AT] "); DEBUG_OUTPUT.print(x); DEBUG_OUTPUT.print(" "); DEBUG_OUTPUT.println(y); }
#define AT_LOGDEBUG2(x,y,z)   if(_ESP_AT_LOGLEVEL_>3) { DEBUG_OUTPUT.print("[ESP_AT] "); DEBUG_OUTPUT.print(x); DEBUG_OUTPUT.print(" "); DEBUG_OUTPUT.print(y); DEBUG_OUTPUT.print(" "); DEBUG_OUTPUT.println(z); }
#define AT_LOGDEBUG3(x,y,z,w) if(_ESP_AT_LOGLEVEL_>3) { DEBUG_OUTPUT.print("[ESP_AT] "); DEBUG_OUTPUT.print(x); DEBUG_OUTPUT.print(" "); DEBUG_OUTPUT.print(y); DEBUG_OUTPUT.print(" "); DEBUG_OUTPUT.print(z); DEBUG_OUTPUT.print(" "); DEBUG_OUTPUT.println(w);}


#endif
