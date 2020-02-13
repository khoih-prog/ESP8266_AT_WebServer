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

// Change _ESP_AT_LOGLEVEL_ to set tracing and logging verbosity
// 0: DISABLED: no logging
// 1: ERROR: errors
// 2: WARN: errors and warnings
// 3: INFO: errors, warnings and informational (default)
// 4: DEBUG: errors, warnings, informational and debug

#ifndef _ESP_AT_LOGLEVEL_
#define _ESP_AT_LOGLEVEL_       1
#endif


#define LOGERROR(x)    if(_ESP_AT_LOGLEVEL_>0) { Serial.print("[ESP_AT] "); Serial.println(x); }
#define LOGERROR1(x,y) if(_ESP_AT_LOGLEVEL_>0) { Serial.print("[ESP_AT] "); Serial.print(x); Serial.print(" "); Serial.println(y); }
#define LOGWARN(x)     if(_ESP_AT_LOGLEVEL_>1) { Serial.print("[ESP_AT] "); Serial.println(x); }
#define LOGWARN1(x,y)  if(_ESP_AT_LOGLEVEL_>1) { Serial.print("[ESP_AT] "); Serial.print(x); Serial.print(" "); Serial.println(y); }
#define LOGINFO(x)     if(_ESP_AT_LOGLEVEL_>2) { Serial.print("[ESP_AT] "); Serial.println(x); }
#define LOGINFO1(x,y)  if(_ESP_AT_LOGLEVEL_>2) { Serial.print("[ESP_AT] "); Serial.print(x); Serial.print(" "); Serial.println(y); }

#define LOGDEBUG(x)      if(_ESP_AT_LOGLEVEL_>3) { Serial.println(x); }
#define LOGDEBUG0(x)     if(_ESP_AT_LOGLEVEL_>3) { Serial.print(x); }
#define LOGDEBUG1(x,y)   if(_ESP_AT_LOGLEVEL_>3) { Serial.print(x); Serial.print(" "); Serial.println(y); }
#define LOGDEBUG2(x,y,z) if(_ESP_AT_LOGLEVEL_>3) { Serial.print(x); Serial.print(" "); Serial.print(y); Serial.print(" "); Serial.println(z); }


#endif
