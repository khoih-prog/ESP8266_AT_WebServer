/****************************************************************************************************************************
  defines.h
  For ESP8266/ESP32 AT-command running shields
  
  ESP8266_AT_WebServer is a library for the ESP8266/ESP32 AT-command shields to run WebServer
  Based on and modified from ESP8266 https://github.com/esp8266/Arduino/releases
  Built by Khoi Hoang https://github.com/khoih-prog/ESP8266_AT_WebServer
  Licensed under MIT license
  
  Original author:
  @file       Esp8266WebServer.h
  @author     Ivan Grokhotkov
  
  Version: 1.1.0
  
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
 *****************************************************************************************************************************/

#ifndef defines_h
#define defines_h

#define DEBUG_ESP8266_AT_WEBSERVER_PORT Serial

// Debug Level from 0 to 4
#define _ESP_AT_LOGLEVEL_       1

// Uncomment to use ESP32-AT commands
//#define USE_ESP32_AT      true

#if    ( defined(ARDUINO_SAMD_ZERO) || defined(ARDUINO_SAMD_MKR1000) || defined(ARDUINO_SAMD_MKRWIFI1010) \
      || defined(ARDUINO_SAMD_NANO_33_IOT) || defined(ARDUINO_SAMD_MKRFox1200) || defined(ARDUINO_SAMD_MKRWAN1300) || defined(ARDUINO_SAMD_MKRWAN1310) \
      || defined(ARDUINO_SAMD_MKRGSM1400) || defined(ARDUINO_SAMD_MKRNB1500) || defined(ARDUINO_SAMD_MKRVIDOR4000) || defined(__SAMD21G18A__) \
      || defined(ARDUINO_SAMD_CIRCUITPLAYGROUND_EXPRESS) || defined(__SAMD21E18A__) || defined(__SAMD51__) || defined(__SAMD51J20A__) || defined(__SAMD51J19A__) \
      || defined(__SAMD51G19A__) || defined(__SAMD51P19A__) || defined(__SAMD21G18A__) )
#if defined(ESP8266_AT_USE_SAMD)
#undef ESP8266_AT_USE_SAMD
#endif
#define ESP8266_AT_USE_SAMD      true
#endif

#if ( defined(NRF52840_FEATHER) || defined(NRF52832_FEATHER) || defined(NRF52_SERIES) || defined(ARDUINO_NRF52_ADAFRUIT) || \
      defined(NRF52840_FEATHER_SENSE) || defined(NRF52840_ITSYBITSY) || defined(NRF52840_CIRCUITPLAY) || defined(NRF52840_CLUE) || \
      defined(NRF52840_METRO) || defined(NRF52840_PCA10056) || defined(PARTICLE_XENON) || defined(NINA_B302_ublox) || defined(NINA_B112_ublox) )
#if defined(ESP8266_AT_USE_NRF528XX)
#undef ESP8266_AT_USE_NRF528XX
#endif
#define ESP8266_AT_USE_NRF528XX      true
#endif

#if ( defined(ARDUINO_SAM_DUE) || defined(__SAM3X8E__) )
#if defined(ESP8266_AT_USE_SAM_DUE)
#undef ESP8266_AT_USE_SAM_DUE
#endif
#define ESP8266_AT_USE_SAM_DUE      true
#endif

#if ( defined(STM32F0) || defined(STM32F1) || defined(STM32F2) || defined(STM32F3)  ||defined(STM32F4) || defined(STM32F7) || \
       defined(STM32L0) || defined(STM32L1) || defined(STM32L4) || defined(STM32H7)  ||defined(STM32G0) || defined(STM32G4) || \
       defined(STM32WB) || defined(STM32MP1) )
#if defined(ESP8266_AT_USE_STM32)
#undef ESP8266_AT_USE_STM32
#endif
#define ESP8266_AT_USE_STM32      true
#endif

#ifdef CORE_TEENSY
// For Teensy 4.1/4.0
#define EspSerial Serial2   //Serial2, Pin RX2 : 7, TX2 : 8
#if defined(__IMXRT1062__)
// For Teensy 4.1/4.0
#define BOARD_TYPE      "TEENSY 4.1/4.0"
#elif defined(__MK66FX1M0__)
#define BOARD_TYPE "Teensy 3.6"
#elif defined(__MK64FX512__)
#define BOARD_TYPE "Teensy 3.5"
#elif defined(__MKL26Z64__)
#define BOARD_TYPE "Teensy LC"
#elif defined(__MK20DX256__)
#define BOARD_TYPE "Teensy 3.2" // and Teensy 3.1 (obsolete)
#elif defined(__MK20DX128__)
#define BOARD_TYPE "Teensy 3.0"
#elif defined(__AVR_AT90USB1286__)
#error Teensy 2.0++ not supported yet
#elif defined(__AVR_ATmega32U4__)
#error Teensy 2.0 not supported yet
#else
// For Other Boards
#define BOARD_TYPE      "Unknown Teensy Board"
#endif

#elif defined(ESP8266_AT_USE_SAMD)
// For SAMD
#define EspSerial Serial1

#if defined(ARDUINO_SAMD_ZERO)
#define BOARD_TYPE      "SAMD Zero"
#elif defined(ARDUINO_SAMD_MKR1000)
#define BOARD_TYPE      "SAMD MKR1000"
#elif defined(ARDUINO_SAMD_MKRWIFI1010)
#define BOARD_TYPE      "SAMD MKRWIFI1010"
#elif defined(ARDUINO_SAMD_NANO_33_IOT)
#define BOARD_TYPE      "SAMD NANO_33_IOT"
#elif defined(ARDUINO_SAMD_MKRFox1200)
#define BOARD_TYPE      "SAMD MKRFox1200"
#elif ( defined(ARDUINO_SAMD_MKRWAN1300) || defined(ARDUINO_SAMD_MKRWAN1310) )
#define BOARD_TYPE      "SAMD MKRWAN13X0"
#elif defined(ARDUINO_SAMD_MKRGSM1400)
#define BOARD_TYPE      "SAMD MKRGSM1400"
#elif defined(ARDUINO_SAMD_MKRNB1500)
#define BOARD_TYPE      "SAMD MKRNB1500"
#elif defined(ARDUINO_SAMD_MKRVIDOR4000)
#define BOARD_TYPE      "SAMD MKRVIDOR4000"
#elif defined(ARDUINO_SAMD_CIRCUITPLAYGROUND_EXPRESS)
#define BOARD_TYPE      "SAMD ARDUINO_SAMD_CIRCUITPLAYGROUND_EXPRESS"
#elif defined(ADAFRUIT_FEATHER_M0_EXPRESS)
#define BOARD_TYPE      "SAMD21 ADAFRUIT_FEATHER_M0_EXPRESS"
#elif defined(ADAFRUIT_METRO_M0_EXPRESS)
#define BOARD_TYPE      "SAMD21 ADAFRUIT_METRO_M0_EXPRESS"
#elif defined(ADAFRUIT_CIRCUITPLAYGROUND_M0)
#define BOARD_TYPE      "SAMD21 ADAFRUIT_CIRCUITPLAYGROUND_M0"
#elif defined(ADAFRUIT_GEMMA_M0)
#define BOARD_TYPE      "SAMD21 ADAFRUIT_GEMMA_M0"
#elif defined(ADAFRUIT_TRINKET_M0)
#define BOARD_TYPE      "SAMD21 ADAFRUIT_TRINKET_M0"
#elif defined(ADAFRUIT_ITSYBITSY_M0)
#define BOARD_TYPE      "SAMD21 ADAFRUIT_ITSYBITSY_M0"
#elif defined(ARDUINO_SAMD_HALLOWING_M0)
#define BOARD_TYPE      "SAMD21 ARDUINO_SAMD_HALLOWING_M0"
#elif defined(ADAFRUIT_METRO_M4_EXPRESS)
#define BOARD_TYPE      "SAMD51 ADAFRUIT_METRO_M4_EXPRESS"
#elif defined(ADAFRUIT_GRAND_CENTRAL_M4)
#define BOARD_TYPE      "SAMD51 ADAFRUIT_GRAND_CENTRAL_M4"
#elif defined(ADAFRUIT_FEATHER_M4_EXPRESS)
#define BOARD_TYPE      "SAMD51 ADAFRUIT_FEATHER_M4_EXPRESS"
#elif defined(ADAFRUIT_ITSYBITSY_M4_EXPRESS)
#define BOARD_TYPE      "SAMD51 ADAFRUIT_ITSYBITSY_M4_EXPRESS"
#elif defined(ADAFRUIT_TRELLIS_M4_EXPRESS)
#define BOARD_TYPE      "SAMD51 ADAFRUIT_TRELLIS_M4_EXPRESS"
#elif defined(ADAFRUIT_PYPORTAL)
#define BOARD_TYPE      "SAMD51 ADAFRUIT_PYPORTAL"
#elif defined(ADAFRUIT_PYPORTAL_M4_TITANO)
#define BOARD_TYPE      "SAMD51 ADAFRUIT_PYPORTAL_M4_TITANO"
#elif defined(ADAFRUIT_PYBADGE_M4_EXPRESS)
#define BOARD_TYPE      "SAMD51 ADAFRUIT_PYBADGE_M4_EXPRESS"
#elif defined(ADAFRUIT_METRO_M4_AIRLIFT_LITE)
#define BOARD_TYPE      "SAMD51 ADAFRUIT_METRO_M4_AIRLIFT_LITE"
#elif defined(ADAFRUIT_PYGAMER_M4_EXPRESS)
#define BOARD_TYPE      "SAMD51 ADAFRUIT_PYGAMER_M4_EXPRESS"
#elif defined(ADAFRUIT_PYGAMER_ADVANCE_M4_EXPRESS)
#define BOARD_TYPE      "SAMD51 ADAFRUIT_PYGAMER_ADVANCE_M4_EXPRESS"
#elif defined(ADAFRUIT_PYBADGE_AIRLIFT_M4)
#define BOARD_TYPE      "SAMD51 ADAFRUIT_PYBADGE_AIRLIFT_M4"
#elif defined(ADAFRUIT_MONSTER_M4SK_EXPRESS)
#define BOARD_TYPE      "SAMD51 ADAFRUIT_MONSTER_M4SK_EXPRESS"
#elif defined(ADAFRUIT_HALLOWING_M4_EXPRESS)
#define BOARD_TYPE      "SAMD51 ADAFRUIT_HALLOWING_M4_EXPRESS"
#elif defined(SEEED_WIO_TERMINAL)
#define BOARD_TYPE      "SAMD SEEED_WIO_TERMINAL"
#elif defined(SEEED_FEMTO_M0)
#define BOARD_TYPE      "SAMD SEEED_FEMTO_M0"
#elif defined(SEEED_XIAO_M0)
#define BOARD_TYPE      "SAMD SEEED_XIAO_M0"
#elif defined(Wio_Lite_MG126)
#define BOARD_TYPE      "SAMD SEEED Wio_Lite_MG126"
#elif defined(WIO_GPS_BOARD)
#define BOARD_TYPE      "SAMD SEEED WIO_GPS_BOARD"
#elif defined(SEEEDUINO_ZERO)
#define BOARD_TYPE      "SAMD SEEEDUINO_ZERO"
#elif defined(SEEEDUINO_LORAWAN)
#define BOARD_TYPE      "SAMD SEEEDUINO_LORAWAN"
#elif defined(SEEED_GROVE_UI_WIRELESS)
#define BOARD_TYPE      "SAMD SEEED_GROVE_UI_WIRELESS"
#elif defined(__SAMD21E18A__)
#define BOARD_TYPE      "SAMD21E18A"
#elif defined(__SAMD21G18A__)
#define BOARD_TYPE      "SAMD21G18A"
#elif defined(__SAMD51G19A__)
#define BOARD_TYPE      "SAMD51G19A"
#elif defined(__SAMD51J19A__)
#define BOARD_TYPE      "SAMD51J19A"
#elif defined(__SAMD51J20A__)
#define BOARD_TYPE      "SAMD51J20A"
#elif defined(__SAM3X8E__)
#define BOARD_TYPE      "SAM3X8E"
#elif defined(__CPU_ARC__)
#define BOARD_TYPE      "CPU_ARC"
#elif defined(__SAMD51__)
#define BOARD_TYPE      "SAMD51"
#else
#define BOARD_TYPE      "SAMD Unknown"
#endif

#elif (ESP8266_AT_USE_NRF528XX)

#if defined(NRF52840_FEATHER)
#define BOARD_TYPE      "NRF52840_FEATHER_EXPRESS"
#elif defined(NRF52832_FEATHER)
#define BOARD_TYPE      "NRF52832_FEATHER"
#elif defined(NRF52840_FEATHER_SENSE)
#define BOARD_TYPE      "NRF52840_FEATHER_SENSE"
#elif defined(NRF52840_ITSYBITSY)
#define BOARD_TYPE      "NRF52840_ITSYBITSY_EXPRESS"
#elif defined(NRF52840_CIRCUITPLAY)
#define BOARD_TYPE      "NRF52840_CIRCUIT_PLAYGROUND"
#elif defined(NRF52840_CLUE)
#define BOARD_TYPE      "NRF52840_CLUE"
#elif defined(NRF52840_METRO)
#define BOARD_TYPE      "NRF52840_METRO_EXPRESS"
#elif defined(NRF52840_PCA10056)
#define BOARD_TYPE      "NORDIC_NRF52840DK"
#elif defined(NINA_B302_ublox)
#define BOARD_TYPE      "NINA_B302_ublox"
#elif defined(NINA_B112_ublox)
#define BOARD_TYPE      "NINA_B112_ublox"
#elif defined(PARTICLE_XENON)
#define BOARD_TYPE      "PARTICLE_XENON"
#elif defined(MDBT50Q_RX)
#define BOARD_TYPE      "RAYTAC_MDBT50Q_RX"
#elif defined(ARDUINO_NRF52_ADAFRUIT)
#define BOARD_TYPE      "ARDUINO_NRF52_ADAFRUIT"
#else
#define BOARD_TYPE      "nRF52 Unknown"
#endif

#define EspSerial Serial1

#elif defined(ESP8266_AT_USE_SAM_DUE)
// For SAM DUE
#define EspSerial Serial1
#define BOARD_TYPE      "SAM DUE"

#elif defined(ESP8266_AT_USE_STM32)
  // For STM32
  #warning EspSerial using SERIAL_PORT_HARDWARE, can be Serial or Serial1. See your board variant.h
  #define EspSerial     SERIAL_PORT_HARDWARE    //Serial1

  #if defined(STM32F0)
    #warning STM32F0 board selected
    #define BOARD_TYPE  "STM32F0"
  #elif defined(STM32F1)
    #warning STM32F1 board selected
    #define BOARD_TYPE  "STM32F1"
  #elif defined(STM32F2)
    #warning STM32F2 board selected
    #define BOARD_TYPE  "STM32F2"
  #elif defined(STM32F3)
    #warning STM32F3 board selected
    #define BOARD_TYPE  "STM32F3"
  #elif defined(STM32F4)
    #warning STM32F4 board selected
    #define BOARD_TYPE  "STM32F4"
  #elif defined(STM32F7)

    #if defined(ARDUINO_NUCLEO_F767ZI)
      #warning Nucleo-144 NUCLEO_F767ZI board selected, using HardwareSerial Serial1 @ pin D0/RX and D1/TX
      // RX TX
      HardwareSerial Serial1(D0, D1);
    #else
    
      #warning STM32F7 board selected
      #define BOARD_TYPE  "STM32F7"

    #endif
    
  #elif defined(STM32L0)
    #if defined(ARDUINO_NUCLEO_L053R8)
      #warning Nucleo-64 NUCLEO_L053R8 board selected, using HardwareSerial Serial1 @ pin D0/RX and D1/TX
      // RX TX
      HardwareSerial Serial1(D0, D1);   // (PA3, PA2);
    #else
    
      #warning STM32L0 board selected
      #define BOARD_TYPE  "STM32L0"

    #endif
    
  #elif defined(STM32L1)
    #warning STM32L1 board selected
    #define BOARD_TYPE  "STM32L1"
  #elif defined(STM32L4)
    #warning STM32L4 board selected
    #define BOARD_TYPE  "STM32L4"
  #elif defined(STM32H7)
    #warning STM32H7 board selected
    #define BOARD_TYPE  "STM32H7"
  #elif defined(STM32G0)
    #warning STM32G0 board selected
    #define BOARD_TYPE  "STM32G0"
  #elif defined(STM32G4)
    #warning STM32G4 board selected
    #define BOARD_TYPE  "STM32G4"
  #elif defined(STM32WB)
    #warning STM32WB board selected
    #define BOARD_TYPE  "STM32WB"
  #elif defined(STM32MP1)
    #warning STM32MP1 board selected
    #define BOARD_TYPE  "STM32MP1"
  #else
    #warning STM32 unknown board selected
    #define BOARD_TYPE  "STM32 Unknown"
  #endif

#else
// For Mega
#define EspSerial Serial3
#define BOARD_TYPE      "AVR Mega"
#endif

#ifndef BOARD_NAME
  #define BOARD_NAME    BOARD_TYPE
#endif

#include <ESP8266_AT_WebServer.h>

char ssid[] = "****";        // your network SSID (name)
char pass[] = "****";        // your network password

#endif    //defines_h
