/****************************************************************************************************************************
   ESP8266_AT_Debug.h - Dead simple web-server.
   For ESP8266 AT-command running shields

   ESP8266_AT_WebServer is a library for the ESP8266 AT-command shields to run WebServer
   Based on and modified from ESP8266 https://github.com/esp8266/Arduino/releases
   Built by Khoi Hoang https://github.com/khoih-prog/ESP8266_AT_WebServer
   Licensed under MIT license
   Version: 1.0.6

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
#define _ESP_AT_LOGLEVEL_       0
#endif


#define LOGERROR(x)    if(_ESP_AT_LOGLEVEL_>0) { DEBUG_OUTPUT.print("[ESP_AT] "); DEBUG_OUTPUT.println(x); }
#define LOGERROR1(x,y) if(_ESP_AT_LOGLEVEL_>0) { DEBUG_OUTPUT.print("[ESP_AT] "); DEBUG_OUTPUT.print(x); DEBUG_OUTPUT.print(" "); DEBUG_OUTPUT.println(y); }
#define LOGWARN(x)     if(_ESP_AT_LOGLEVEL_>1) { DEBUG_OUTPUT.print("[ESP_AT] "); DEBUG_OUTPUT.println(x); }
#define LOGWARN1(x,y)  if(_ESP_AT_LOGLEVEL_>1) { DEBUG_OUTPUT.print("[ESP_AT] "); DEBUG_OUTPUT.print(x); DEBUG_OUTPUT.print(" "); DEBUG_OUTPUT.println(y); }
#define LOGINFO(x)     if(_ESP_AT_LOGLEVEL_>2) { DEBUG_OUTPUT.print("[ESP_AT] "); DEBUG_OUTPUT.println(x); }
#define LOGINFO1(x,y)  if(_ESP_AT_LOGLEVEL_>2) { DEBUG_OUTPUT.print("[ESP_AT] "); DEBUG_OUTPUT.print(x); DEBUG_OUTPUT.print(" "); DEBUG_OUTPUT.println(y); }

#define LOGDEBUG(x)      if(_ESP_AT_LOGLEVEL_>3) { DEBUG_OUTPUT.println(x); }
#define LOGDEBUG0(x)     if(_ESP_AT_LOGLEVEL_>3) { DEBUG_OUTPUT.print(x); }
#define LOGDEBUG1(x,y)   if(_ESP_AT_LOGLEVEL_>3) { DEBUG_OUTPUT.print(x); DEBUG_OUTPUT.print(" "); DEBUG_OUTPUT.println(y); }
#define LOGDEBUG2(x,y,z) if(_ESP_AT_LOGLEVEL_>3) { DEBUG_OUTPUT.print(x); DEBUG_OUTPUT.print(" "); DEBUG_OUTPUT.print(y); DEBUG_OUTPUT.print(" "); DEBUG_OUTPUT.println(z); }


#endif
