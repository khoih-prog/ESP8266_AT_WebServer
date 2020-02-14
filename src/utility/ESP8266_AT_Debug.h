/****************************************************************************************************************************
 * ESP8266_AT_Debug.h - Dead simple web-server.
 * For ESP8266 AT-command running shields
 *
 * ESP_AT_WebServer is a library for the ESP8266 AT-command shields to run WebServer
 *
 * Forked and modified from Arduino ESP_AT library
 * Built by Khoi Hoang https://github.com/khoih-prog/ESP_AT_WebServer
 * Licensed under MIT license
 * Version: 1.0.0
 *
 *
 * Version Modified By   Date      Comments
 * ------- -----------  ---------- -----------
 *  1.0.0   K Hoang      12/02/2020 Initial coding for Arduino Mega, Teensy, etc
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
