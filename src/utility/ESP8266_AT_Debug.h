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

#ifndef ESP8266_AT_Debug_H
#define ESP8266_AT_Debug_H

////////////////////////////////////////

#include <stdio.h>

////////////////////////////////////////

#ifdef DEBUG_ESP8266_AT_WEBSERVER_PORT
  #define DEBUG_OUTPUT DEBUG_ESP8266_AT_WEBSERVER_PORT
#else
  #define DEBUG_OUTPUT Serial
#endif

////////////////////////////////////////

// Change _ESP_AT_LOGLEVEL_ to set tracing and logging verbosity
// 0: DISABLED: no logging
// 1: ERROR: errors
// 2: WARN: errors and warnings
// 3: INFO: errors, warnings and informational (default)
// 4: DEBUG: errors, warnings, informational and debug

#ifndef _ESP_AT_LOGLEVEL_
  #define _ESP_AT_LOGLEVEL_       1
#endif

////////////////////////////////////////

const char EAT_MARK[]  = "[ESP_AT] ";
const char EAT_SPACE[] = " ";
const char EAT_LINE[]  = "========================================\n";

#define EAT_PRINT_MARK   EAT_PRINT(EAT_MARK)
#define EAT_PRINT_SP     EAT_PRINT(EAT_SPACE)
#define EAT_PRINT_LINE   EAT_PRINT(EAT_LINE)

#define EAT_PRINT        DEBUG_OUTPUT.print
#define EAT_PRINTLN      DEBUG_OUTPUT.println

////////////////////////////////////////

#define AT_LOGERROR(x)         if(_ESP_AT_LOGLEVEL_>0) { EAT_PRINT_MARK; EAT_PRINTLN(x); }
#define AT_LOGERROR_LINE(x)    if(_ESP_AT_LOGLEVEL_>0) { EAT_PRINT_MARK; EAT_PRINTLN(x); EAT_PRINT_LINE; }
#define AT_LOGERROR0(x)        if(_ESP_AT_LOGLEVEL_>0) { EAT_PRINT(x); }
#define AT_LOGERROR1(x,y)      if(_ESP_AT_LOGLEVEL_>0) { EAT_PRINT_MARK; EAT_PRINT(x); EAT_PRINT_SP; EAT_PRINTLN(y); }
#define AT_LOGERROR2(x,y,z)    if(_ESP_AT_LOGLEVEL_>0) { EAT_PRINT_MARK; EAT_PRINT(x); EAT_PRINT_SP; EAT_PRINT(y); EAT_PRINT_SP; EAT_PRINTLN(z); }
#define AT_LOGERROR3(x,y,z,w)  if(_ESP_AT_LOGLEVEL_>0) { EAT_PRINT_MARK; EAT_PRINT(x); EAT_PRINT_SP; EAT_PRINT(y); EAT_PRINT_SP; EAT_PRINT(z); EAT_PRINT_SP; EAT_PRINTLN(w); }

////////////////////////////////////////

#define AT_LOGWARN(x)          if(_ESP_AT_LOGLEVEL_>1) { EAT_PRINT_MARK; EAT_PRINTLN(x); }
#define AT_LOGWARN_LINE(x)     if(_ESP_AT_LOGLEVEL_>1) { EAT_PRINT_MARK; EAT_PRINTLN(x); EAT_PRINT_LINE; }
#define AT_LOGWARN0(x)         if(_ESP_AT_LOGLEVEL_>1) { EAT_PRINT(x); }
#define AT_LOGWARN1(x,y)       if(_ESP_AT_LOGLEVEL_>1) { EAT_PRINT_MARK; EAT_PRINT(x); EAT_PRINT_SP; EAT_PRINTLN(y); }
#define AT_LOGWARN2(x,y,z)     if(_ESP_AT_LOGLEVEL_>1) { EAT_PRINT_MARK; EAT_PRINT(x); EAT_PRINT_SP; EAT_PRINT(y); EAT_PRINT_SP; EAT_PRINTLN(z); }
#define AT_LOGWARN3(x,y,z,w)   if(_ESP_AT_LOGLEVEL_>1) { EAT_PRINT_MARK; EAT_PRINT(x); EAT_PRINT_SP; EAT_PRINT(y); EAT_PRINT_SP; EAT_PRINT(z); EAT_PRINT_SP; EAT_PRINTLN(w); }

////////////////////////////////////////

#define AT_LOGINFO(x)          if(_ESP_AT_LOGLEVEL_>2) { EAT_PRINT_MARK; EAT_PRINTLN(x); }
#define AT_LOGINFO_LINE(x)     if(_ESP_AT_LOGLEVEL_>2) { EAT_PRINT_MARK; EAT_PRINTLN(x); EAT_PRINT_LINE; }
#define AT_LOGINFO0(x)         if(_ESP_AT_LOGLEVEL_>2) { EAT_PRINT(x); }
#define AT_LOGINFO1(x,y)       if(_ESP_AT_LOGLEVEL_>2) { EAT_PRINT_MARK; EAT_PRINT(x); EAT_PRINT_SP; EAT_PRINTLN(y); }
#define AT_LOGINFO2(x,y,z)     if(_ESP_AT_LOGLEVEL_>2) { EAT_PRINT_MARK; EAT_PRINT(x); EAT_PRINT_SP; EAT_PRINT(y); EAT_PRINT_SP; EAT_PRINTLN(z); }
#define AT_LOGINFO3(x,y,z,w)   if(_ESP_AT_LOGLEVEL_>2) { EAT_PRINT_MARK; EAT_PRINT(x); EAT_PRINT_SP; EAT_PRINT(y); EAT_PRINT_SP; EAT_PRINT(z); EAT_PRINT_SP; EAT_PRINTLN(w); }

////////////////////////////////////////

#define AT_LOGDEBUG(x)         if(_ESP_AT_LOGLEVEL_>3) { EAT_PRINT_MARK; EAT_PRINTLN(x); }
#define AT_LOGDEBUG_LINE(x)    if(_ESP_AT_LOGLEVEL_>3) { EAT_PRINT_MARK; EAT_PRINTLN(x); EAT_PRINT_LINE; }
#define AT_LOGDEBUG0(x)        if(_ESP_AT_LOGLEVEL_>3) { EAT_PRINT(x); }
#define AT_LOGDEBUG1(x,y)      if(_ESP_AT_LOGLEVEL_>3) { EAT_PRINT_MARK; EAT_PRINT(x); EAT_PRINT_SP; EAT_PRINTLN(y); }
#define AT_LOGDEBUG2(x,y,z)    if(_ESP_AT_LOGLEVEL_>3) { EAT_PRINT_MARK; EAT_PRINT(x); EAT_PRINT_SP; EAT_PRINT(y); EAT_PRINT_SP; EAT_PRINTLN(z); }
#define AT_LOGDEBUG3(x,y,z,w)  if(_ESP_AT_LOGLEVEL_>3) { EAT_PRINT_MARK; EAT_PRINT(x); EAT_PRINT_SP; EAT_PRINT(y); EAT_PRINT_SP; EAT_PRINT(z); EAT_PRINT_SP; EAT_PRINTLN(w); }

////////////////////////////////////////

#endif
