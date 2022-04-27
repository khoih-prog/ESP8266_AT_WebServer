## ESP8266_AT_WebServer

[![arduino-library-badge](https://www.ardu-badge.com/badge/ESP8266_AT_WebServer.svg?)](https://www.ardu-badge.com/ESP8266_AT_WebServer)
[![GitHub release](https://img.shields.io/github/release/khoih-prog/ESP8266_AT_WebServer.svg)](https://github.com/khoih-prog/ESP8266_AT_WebServer/releases)
[![GitHub](https://img.shields.io/github/license/mashape/apistatus.svg)](https://github.com/khoih-prog/ESP8266_AT_WebServer/blob/master/LICENSE)
[![contributions welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat)](#Contributing)
[![GitHub issues](https://img.shields.io/github/issues/khoih-prog/ESP8266_AT_WebServer.svg)](http://github.com/khoih-prog/ESP8266_AT_WebServer/issues)

---
---

## Table of Contents

* [Changelog](#changelog)
  * [Releases v1.5.4](#releases-v154)
  * [Releases v1.5.3](#releases-v153)
  * [Releases v1.5.2](#releases-v152)
  * [Releases v1.5.1](#releases-v151)
  * [Releases v1.5.0](#releases-v150)
  * [Releases v1.4.1](#releases-v141)
  * [Releases v1.4.0](#releases-v140)
  * [Releases v1.3.0](#releases-v130)
  * [Releases v1.2.0](#releases-v120)
  * [Releases v1.1.2](#releases-v112)
  * [Releases v1.1.1](#releases-v111)
  * [Releases v1.1.0](#releases-v110)
  * [Releases v1.0.12](#releases-v1012)
  * [Releases v1.0.11](#releases-v1011)
  * [Releases v1.0.10](#releases-v1010)
  * [Releases v1.0.9](#releases-v109)
  * [Releases v1.0.8](#releases-v108)
  * [Releases v1.0.7](#releases-v107)
  * [Releases v1.0.6](#releases-v106)
  * [Releases v1.0.5](#releases-v105)
  * [Releases v1.0.4](#releases-v104)
  * [Releases v1.0.3](#releases-v103)
  * [Releases v1.0.2](#releases-v102)
 
---
---

## Changelog

### Releases v1.5.4

1. Use new `arduino.tips` instead of `arduino.cc`.
2. Improve examples
3. Update `Packages' Patches`

### Releases v1.5.3

1. Fix authenticate issue caused by libb64
2. Update `Packages' Patches`

### Releases v1.5.2

1. Fix wrong http status header bug. Check [fix for wrong http status header #42](https://github.com/khoih-prog/EthernetWebServer/pull/42)

### Releases v1.5.1

1. Fix bug related to String in library and examples

### Releases v1.5.0

1. Reduce usage of Arduino String with std::string
2. Optimize library code and examples by using **reference-passing instead of value-passing**.
3. Update `Packages' Patches`


### Releases v1.4.1

1. Add instructions for SIPEED_MAIX_DUINO
2. Add SIPEED_MAIX_DUINO and update remaining `Packages' Patches`

### Releases v1.4.0

1. Add support to Adafruit nRF52 core v0.22.0+
2. Add support to Raytac MDBT50Q_RX Dongle
3. Update Adafruit `Packages' Patches`

### Releases v1.3.0

1. Add support to RP2040-based boards, such as **Nano_RP2040_Connect, RASPBERRY_PI_PICO, ADAFRUIT_FEATHER_RP2040 and GENERIC_RP2040**, using [**Arduino-mbed RP2040** core](https://github.com/arduino/ArduinoCore-mbed).
2. Fix bug.

### Releases v1.2.0

1. Add support to **RASPBERRY_PI_PICO boards** using [Earle Philhower's arduino-pico core](https://github.com/earlephilhower/arduino-pico)
2. Add support to **SIPEED_MAIX_DUINO boards**

### Releases v1.1.2

1. Clean-up all compiler warnings possible.
2. Add MQTT examples
3. Add Version String 
4. Add Table of Contents

### Releases v1.1.1

1. Add support to PROGMEM-related commands, such as sendContent_P() and send_P().
2. Update Platform.ini to support PlatformIO 5.x owner-based dependency declaration.
3. Clean up code. Use FlashString and related functions extensively in code as well in examples.
4. Enhance examples.

#### Releases v1.1.0

1. Add support to UDP **Multicast**. Now ESP8266/ESP32 AT-command shields can run uPnP libraries to manage **Internet Gateway's port forwarding**.
2. Fix bugs.
3. Enhance debug support.

#### Releases v1.0.12

1. Add sample Packages_Patches for STM32 (Nucleo-144 NUCLEO_F767ZI, Nucleo-64 NUCLEO_L053R8)
2. Add example for STM32F/L/H/G/WB/MP1 boards.
3. Add instructions to use EspSerial/Serial1 on some STM32 boards without Serial1 definition (Nucleo-144 NUCLEO_F767ZI, Nucleo-64 NUCLEO_L053R8, etc.)

#### Releases v1.0.11

1. Add support to all **STM32F/L/H/G/WB/MP1 (Nucleo-144 NUCLEO_F767ZI, Nucleo-64 NUCLEO_L053R8, etc.)**
2. Add support to **Seeeduino SAMD21/SAMD51 boards (SEEED_WIO_TERMINAL, SEEED_FEMTO_M0, SEEED_XIAO_M0, Wio_Lite_MG126, WIO_GPS_BOARD, SEEEDUINO_ZERO, SEEEDUINO_LORAWAN, SEEED_GROVE_UI_WIRELESS, etc.)**

#### Releases v1.0.10

1. Fix bug not closing client and releasing socket.
2. Enhance examples.
3. Add function to be enable to work with [WebSockets2_Generic Library](https://github.com/khoih-prog/WebSockets2_Generic)

#### Releases v1.0.9

1. Fix bug. 
2. Add functions (ESP8266_AT_Drv::wifiDriverReInit and ESP8266_AT_Class::reInit). 
3. Restructure codes. Increase RingBuffer Size.
4. Add support to WIS600-01S and W600 WiFi shields

#### Releases v1.0.8

1. Fix bug. 
2. Add features to ESP32-AT. 

#### Releases v1.0.7

1. Add support to **ESP32-AT-command shield**. 
2. Update deprecated ESP8266-AT commands. 
3. Restructure examples to separate defines header files.

#### Releases v1.0.6

1. Add support to **nRF52** boards, such as **AdaFruit Feather nRF52832, nRF52840 Express, BlueFruit Sense, Itsy-Bitsy nRF52840 Express, Metro nRF52840 Express, NINA_B302_ublox, NINA_B112_ublox, etc.** 

#### Releases v1.0.5

1. Add support to **SAM51 (Itsy-Bitsy M4, Metro M4, Grand Central M4, Feather M4 Express, etc.) and SAM DUE**.

#### Releases v1.0.4

1. Sync with ESP8266WebServer library of ESP8266 core v2.6.3
2. Fix bug.

#### Releases v1.0.3

1. Add support to STM32 (STM32F0, F1, F2, F3, F4, F7, etc) boards

#### Releases v1.0.2

1. Add support to SAMD (ZERO, MKR, **NANO_33_IOT**, M0, M0 Pro, AdaFruit CIRCUITPLAYGROUND_EXPRESS, etc.) boards


