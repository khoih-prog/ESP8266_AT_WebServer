## ESP8266_AT_WebServer

[![arduino-library-badge](https://www.ardu-badge.com/badge/ESP8266_AT_WebServer.svg?)](https://www.ardu-badge.com/ESP8266_AT_WebServer)
[![GitHub release](https://img.shields.io/github/release/khoih-prog/ESP8266_AT_WebServer.svg)](https://github.com/khoih-prog/ESP8266_AT_WebServer/releases)
[![GitHub](https://img.shields.io/github/license/mashape/apistatus.svg)](https://github.com/khoih-prog/ESP8266_AT_WebServer/blob/master/LICENSE)
[![contributions welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat)](#Contributing)
[![GitHub issues](https://img.shields.io/github/issues/khoih-prog/ESP8266_AT_WebServer.svg)](http://github.com/khoih-prog/ESP8266_AT_WebServer/issues)

<a href="https://www.buymeacoffee.com/khoihprog6" title="Donate to my libraries using BuyMeACoffee"><img src="https://cdn.buymeacoffee.com/buttons/v2/default-yellow.png" alt="Donate to my libraries using BuyMeACoffee" style="height: 50px !important;width: 181px !important;" ></a>
<a href="https://www.buymeacoffee.com/khoihprog6" title="Donate to my libraries using BuyMeACoffee"><img src="https://img.shields.io/badge/buy%20me%20a%20coffee-donate-orange.svg?logo=buy-me-a-coffee&logoColor=FFDD00" style="height: 20px !important;width: 200px !important;" ></a>
<a href="https://profile-counter.glitch.me/khoih-prog/count.svg" title="Total khoih-prog Visitor count"><img src="https://profile-counter.glitch.me/khoih-prog/count.svg" style="height: 30px;width: 200px;"></a>
<a href="https://profile-counter.glitch.me/khoih-prog-ESP8266_AT_WebServer/count.svg" title="ESP8266_AT_WebServer Visitor count"><img src="https://profile-counter.glitch.me/khoih-prog-ESP8266_AT_WebServer/count.svg" style="height: 30px;width: 200px;"></a>


---
---

## Table of Contents

* [Important Note from v1.6.0](#Important-Note-from-v160)
* [Why do we need this ESP8266_AT_WebServer library](#why-do-we-need-this-esp8266_at_webserver-library)
  * [Features](#features)
  * [Currently Supported Boards](#currently-supported-boards)
  * [Currently Supported AT-command shields](#currently-supported-at-command-shields)
* [Changelog](changelog.md)
* [Prerequisites](#prerequisites)
* [Important Notes about AT Firmwares](#important-notes-about-at-firmwares)
  * [1. Tested OK with ESP8266-AT shields](#1-tested-ok-with-esp8266-at-shields)
  * [2. Tested OK with ESP32-AT shields](#2-tested-ok-with-esp32-at-shields)
  * [3. Where to upload AT-Firmware](#3-where-to-upload-at-firmware)
  * [4. Important Notes](#4-important-notes)
* [Installation](#installation)
  * [Use Arduino Library Manager](#use-arduino-library-manager)
  * [Manual Install](#manual-install)
  * [VS Code & PlatformIO](#vs-code--platformio)
* [Packages' Patches](#packages-patches)
  * [1. For Adafruit nRF52840 and nRF52832 boards](#1-for-adafruit-nRF52840-and-nRF52832-boards)
  * [2. For Teensy boards](#2-for-teensy-boards)
  * [3. For Arduino SAM DUE boards](#3-for-arduino-sam-due-boards)
  * [4. For Arduino SAMD boards](#4-for-arduino-samd-boards)
      * [For core version v1.8.10+](#for-core-version-v1810)
      * [For core version v1.8.9-](#for-core-version-v189-)
  * [5. For Adafruit SAMD boards](#5-for-adafruit-samd-boards)
  * [6. For Seeeduino SAMD boards](#6-for-seeeduino-samd-boards)
  * [7. For STM32 boards](#7-for-stm32-boards) 
    * [7.1. For STM32 boards to use LAN8720](#71-for-stm32-boards-to-use-lan8720)
    * [7.2. For STM32 boards to use Serial1](#72-for-stm32-boards-to-use-serial1)
  * [8. For RP2040-based boards using Earle Philhower arduino-pico core](#8-for-rp2040-based-boards-using-earle-philhower-arduino-pico-core)
    * [8.1. To use BOARD_NAME](#81-to-use-board_name)
    * [8.2. To avoid compile error relating to microsecondsToClockCycles](#82-to-avoid-compile-error-relating-to-microsecondstoclockcycles)
  * [9. For Portenta_H7 boards using Arduino IDE in Linux](#9-for-portenta_h7-boards-using-arduino-ide-in-linux)
  * [10. For RTL8720DN boards using AmebaD core](#10-for-rtl8720dn-boards-using-amebad-core)
  * [11. For SAMD21 and SAMD51 boards using ArduinoCore-fab-sam core](#11-For-SAMD21-and-SAMD51-boards-using-ArduinoCore-fab-sam-core)
  * [12. For Seeeduino RP2040 boards](#12-For-Seeeduino-RP2040-boards)
  * [13. For Seeeduino nRF52840 boards](#13-For-Seeeduino-nRF52840-boards)
* [Usage](#usage) 
  * [Class Constructor](#class-constructor) 
  * [Basic Operations](#basic-operations) 
  * [Advanced Options](#advanced-options) 
  * [Other Function Calls](#other-function-calls) 
* [Examples](#examples)
  * [ 1. AdvancedWebServer](examples/AdvancedWebServer)
  * [ 2. AdvancedWebServer_STM32](examples/AdvancedWebServer_STM32)
  * [ 3. ConnectWPA](examples/ConnectWPA)
  * [ 4. HelloServer](examples/HelloServer)
  * [ 5. HelloServer2](examples/HelloServer2)
  * [ 6. HttpBasicAuth](examples/HttpBasicAuth)
  * [ 7. **MQTTClient_Auth**](examples/MQTTClient_Auth)
  * [ 8. **MQTTClient_Basic**](examples/MQTTClient_Basic)
  * [ 9. **MQTT_ThingStream**](examples/MQTT_ThingStream)
  * [10. PostServer](examples/PostServer)
  * [11. ScanNetworks](examples/ScanNetworks)
  * [12. SimpleAuthentication](examples/SimpleAuthentication)
  * [13. UdpNTPClient](examples/UdpNTPClient)
  * [14. UdpSendReceive](examples/UdpSendReceive)
  * [15. WebClient](examples/WebClient) 
  * [16. WebClientRepeating](examples/WebClientRepeating)
  * [17. WebServer](examples/WebServer)
  * [18. WebServerAP](examples/WebServerAP)
  * [19. ATWebServer_BigData](examples/ATWebServer_BigData) **New**
* [Example AdvancedWebServer](#example-advancedwebserver)
  * [1. File AdvancedWebServer.ino](#1-file-advancedwebserverino)
  * [2. File defines.h](#2-file-definesh)
* [Debug Terminal Output Samples](#debug-terminal-output-samples)
  * [1. AdvancedWebServer on Adafruit NRF52840_ITSYBITSY_EXPRESS using ESP8266-AT shield](#1-advancedwebserver-on-adafruit-nrf52840_itsybitsy_express-using-esp8266-at-shield)
  * [2. MQTT_ThingStream on Adafruit SAMD51 ITSYBITSY_M4 using ESP8266-AT shield](#2-mqtt_thingstream-on-adafruit-samd51-itsybitsy_m4-using-esp8266-at-shield)
  * [3. MQTTClient_Auth on Adafruit SAMD51 ITSYBITSY_M4 using ESP8266-AT shield](#3-mqttclient_auth-on-adafruit-samd51-itsybitsy_m4-using-esp8266-at-shield)
  * [4. MQTTClient_Basic on Adafruit SAMD51 ITSYBITSY_M4 using ESP8266-AT shield](#4-mqttclient_basic-on-adafruit-samd51-itsybitsy_m4-using-esp8266-at-shield)
  * [5. WebClientRepeating on RASPBERRY_PI_PICO using ESP8266-AT shield](#5-webclientrepeating-on-RASPBERRY_PI_PICO-using-esp8266-at-shield)
  * [6. MQTTClient_Auth on RASPBERRY_PI_PICO using ESP8266-AT shield](#6-MQTTClient_Auth-on-RASPBERRY_PI_PICO-using-esp8266-at-shield)
  * [7. UdpNTPClient on MBED RASPBERRY_PI_PICO using ESP8266-AT shield](#7-UdpNTPClient-on-mbed-RASPBERRY_PI_PICO-using-esp8266-at-shield)
  * [8. AdvancedWebServer on NRF52840_ITSYBITSY](#8-AdvancedWebServer-on-nrf52840_itsybitsy)
  * [9. ATWebServer_BigData on NRF52840_ITSYBITSY](#9-ATWebServer_BigData-on-NRF52840_ITSYBITSY)
* [Screen Shots](#screen-shots)
  * [1. SAMD51 Itsy-Bitsy M4](#1-samd51-itsy-bitsy-m4)
  * [2. STM32 Nucleo-144 NUCLEO_F767ZI](#2-stm32-nucleo-144-nucleo_f767zi)
  * [3. nRF52 Itsy-Bitsy nRF52840](#3-nrf52-itsy-bitsy-nrf52840)
  * [4. Seeeduino SEEED_XIAO_M0](#4-seeeduino-seeed_xiao_m0)
  * [5. RASPBERRY_PI_PICO](#5-raspberry_pi_pico)
  * [6. MBED RASPBERRY_PI_PICO](#6-mbed-raspberry_pi_pico)
  * [7. nRF52 Itsy-Bitsy nRF52840 BigData](#7-nrf52-itsy-bitsy-nrf52840-BigData)
* [Debug](#debug)
* [Troubleshooting](#troubleshooting)
* [Issues](#issues)
* [TO DO](#to-do)
* [DONE](#done)
* [Contributions and Thanks](#contributions-and-thanks)
* [Contributing](#contributing)
* [License](#license)
* [Copyright](#copyright)


---
---

### Important Note from v1.6.0

The new [ESP8266_AT_WebServer v1.6.0+](https://github.com/khoih-prog/ESP8266_AT_WebServer/releases/tag/v1.6.0) fixes severe limitation to permit sending much larger data than total than **2K** buffer of **ESP8266/ESP32 AT-command shield**.

---
---

### Why do we need this [ESP8266_AT_WebServer library](https://github.com/khoih-prog/ESP8266_AT_WebServer)

#### Features

This [ESP8266_AT_WebServer library](https://github.com/khoih-prog/ESP8266_AT_WebServer) is a simple yet complete WebServer library for **AVR, Teensy, SAM DUE, Arduino SAMD21, Adafruit SAMD21/SAMD51, Adafruit nRF52, STM32F/L/H/G/WB/MP1, etc. boards, using ESP8266/ESP32 AT-command WiFi modules/shields.** 

The functions are similar and compatible to those of [`ESP32 WebServer`](https://github.com/espressif/arduino-esp32/tree/master/libraries/WebServer) and [`ESP8266WebServer`](https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WebServer) libraries to make life much easier to port sketches from ESP8266/ESP32.

The library provides supports to:

1. WiFi Client, STA and AP mode
2. TCP Server and Client
3. UDP Server and Client
4. HTTP Server and Client
5. HTTP GET and POST requests, provides argument parsing, handles one client at a time.
6. UDP Multicast and Broadcast.

It is based on and modified from:

1. [Ivan Grokhotkov's ESP8266WebServer](https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WebServer)
2. [bportaluri's WiFiEsp library](https://github.com/bportaluri/WiFiEsp)

The ESP8266_AT_Web_Server class found in `ESP8266_AT_Web_Server.h` header, is a simple web server that knows how to handle HTTP requests such as GET and POST and can only support one simultaneous client.


#### Currently Supported Boards

 1. **nRF52 boards**, such as **AdaFruit Feather nRF52832, nRF52840 Express, BlueFruit Sense, Itsy-Bitsy nRF52840 Express, Metro nRF52840 Express, NINA_B302_ublox, NINA_B112_ublox, etc.**
 
 2. **SAM DUE**
 
 3. **SAMD21**
  - Arduino SAMD21: ZERO, MKRs, NANO_33_IOT, etc.
  - Adafruit SAMD21 (M0): ItsyBitsy M0, Feather M0, Feather M0 Express, Metro M0 Express, Circuit Playground Express, Trinket M0, PIRkey, Hallowing M0, Crickit M0, etc.
  - Seeeduino: LoRaWAN, Zero, Femto M0, XIAO M0, Wio GPS Board, etc.
  
 4. **SAMD51**
  - Adafruit SAMD51 (M4): Metro M4, Grand Central M4, ItsyBitsy M4, Feather M4 Express, Trellis M4, Metro M4 AirLift Lite, MONSTER M4SK Express, Hallowing M4, etc.
  - Seeeduino: Wio Terminal, Grove UI Wireless
  
 5. **Teensy (4.1, 4.0, 3.6, 3.5, 3,2, 3.1, 3.0, LC)**
 
 6. **STM32F/L/H/G/WB/MP1 boards (with 64+K Flash)**

- Nucleo-144
- Nucleo-64
- Discovery
- Generic STM32F0, STM32F1, STM32F2, STM32F3, STM32F4, STM32F7 (with 64+K Flash): x8 and up
- STM32L0, STM32L1, STM32L4, **STM32L5**
- STM32G0, STM32G4
- STM32H7
- STM32WB
- STM32MP1
- LoRa boards
- 3-D printer boards
- Generic Flight Controllers
- Midatronics boards

 7. **SIPEED_MAIX_DUINO** boards

 8. RP2040-based boards, such as **Nano RP2040 Connect**, using [**Arduino mbed OS for Nano boards**](https://github.com/arduino/ArduinoCore-mbed).

 9. RP2040-based boards, such as **RASPBERRY_PI_PICO, ADAFRUIT_FEATHER_RP2040 and GENERIC_RP2040**, using [**Arduino-mbed RP2040** core](https://github.com/arduino/ArduinoCore-mbed) or [**Earle Philhower's arduino-pico** core](https://github.com/earlephilhower/arduino-pico).
 

---

#### Currently Supported AT-command shields

 1. ESP8266-AT-command shield
 2. ESP32-AT-command shield
 3. W600 and WIS600-01S AT-command shield
 
---
---


## Prerequisites

 1. [`Arduino IDE 1.8.19+` for Arduino](https://github.com/arduino/Arduino). [![GitHub release](https://img.shields.io/github/release/arduino/Arduino.svg)](https://github.com/arduino/Arduino/releases/latest)
 2. [`Arduino AVR core 1.8.6+`](https://github.com/arduino/ArduinoCore-avr) for Arduino (Use Arduino Board Manager) for AVR boards. [![GitHub release](https://img.shields.io/github/release/arduino/ArduinoCore-avr.svg)](https://github.com/arduino/ArduinoCore-avr/releases/latest)
 3. [`Teensy core v1.57+`](https://www.pjrc.com/teensy/td_download.html) for Teensy (4.1, 4.0, 3.6, 3.5, 3,2, 3.1, 3.0) boards.
 4. [`Arduino SAM DUE core v1.6.12+`](https://github.com/arduino/ArduinoCore-sam) for SAM DUE ARM Cortex-M3 boards.
 5. [`Arduino SAMD core 1.8.13+`](https://github.com/arduino/ArduinoCore-samd) for SAMD ARM Cortex-M0+ boards. [![GitHub release](https://img.shields.io/github/release/arduino/ArduinoCore-samd.svg)](https://github.com/arduino/ArduinoCore-samd/releases/latest)
 6. [`Adafruit SAMD core 1.7.11+`](https://github.com/adafruit/ArduinoCore-samd) for SAMD ARM Cortex-M0+ and M4 boards (Nano 33 IoT, etc.). [![GitHub release](https://img.shields.io/github/release/adafruit/ArduinoCore-samd.svg)](https://github.com/adafruit/ArduinoCore-samd/releases/latest)
 7. [`Seeeduino SAMD core 1.8.3+`](https://github.com/Seeed-Studio/ArduinoCore-samd) for SAMD21/SAMD51 boards (XIAO M0, Wio Terminal, etc.). [![Latest release](https://img.shields.io/github/release/Seeed-Studio/ArduinoCore-samd.svg)](https://github.com/Seeed-Studio/ArduinoCore-samd/releases/latest/)
 8. [`Adafruit nRF52 v1.3.0+`](https://www.adafruit.com) for nRF52 boards such as Adafruit NRF52840_FEATHER, NRF52832_FEATHER, NRF52840_FEATHER_SENSE, NRF52840_ITSYBITSY, NRF52840_CIRCUITPLAY, NRF52840_CLUE, NRF52840_METRO, NRF52840_PCA10056, PARTICLE_XENON, **NINA_B302_ublox**, etc. [![GitHub release](https://img.shields.io/github/release/adafruit/Adafruit_nRF52_Arduino.svg)](https://github.com/adafruit/Adafruit_nRF52_Arduino/releases/latest). Please remember to use latest [`Packages_Patches`](https://github.com/khoih-prog/WiFiWebServer/tree/master/Packages_Patches) or getting compiler errors.
 9. [`Arduino Core for STM32 v2.3.0+`](https://github.com/stm32duino/Arduino_Core_STM32) for STM32F/L/H/G/WB/MP1 boards. [![GitHub release](https://img.shields.io/github/release/stm32duino/Arduino_Core_STM32.svg)](https://github.com/stm32duino/Arduino_Core_STM32/releases/latest)
10. [`Arduino mbed_rp2040 core 3.4.1+`](https://github.com/arduino/ArduinoCore-mbed) for Arduino (Use Arduino Board Manager) RP2040-based boards, such as **Arduino Nano RP2040 Connect, RASPBERRY_PI_PICO, etc.**. [![GitHub release](https://img.shields.io/github/release/arduino/ArduinoCore-mbed.svg)](https://github.com/arduino/ArduinoCore-mbed/releases/latest)
11. [`Earle Philhower's arduino-pico core v2.6.3+`](https://github.com/earlephilhower/arduino-pico) for RP2040-based boards such as **RASPBERRY_PI_PICO, ADAFRUIT_FEATHER_RP2040 and GENERIC_RP2040**, etc. [![GitHub release](https://img.shields.io/github/release/earlephilhower/arduino-pico.svg)](https://github.com/earlephilhower/arduino-pico/releases/latest)
12. [`Sipeed Maixduino core v0.3.11+`](https://github.com/sipeed/Maixduino) for K210-RISC-V-based boards such as **Maixduino AI Development Kit K210 RISC-V AI + IoT ESP32, Sipeed Maix Go, Sipeed Maix Bit**, etc. [![GitHub release](https://img.shields.io/github/release/sipeed/Maixduino.svg)](https://github.com/sipeed/Maixduino/releases/latest)
13. [`Functional-Vlpp library v1.0.2+`](https://github.com/khoih-prog/functional-vlpp) to use server's lambda function. To install. check [![arduino-library-badge](https://www.ardu-badge.com/badge/Functional-Vlpp.svg?)](https://www.ardu-badge.com/Functional-Vlpp)
14. [`Ai-Thinker AT Firmware v1.5.4`](AT_Firmwares/At_firmware_bin1.54.zip) or [`AT Firmware v1.7.4.0`](AT_Firmwares/AT_Firmware_bin_1.7.4.0.zip) for ESP8266-AT shields.
15. [`AT version_2.1.0.0_dev`](AT_Firmwares/AT_version_2.1.0.0_dev.zip) for ESP32-AT shields.
16. `AT version_1.1.4` for WIS600-01S and W600-AT WiFi shields.

---
---

### Important Notes about AT Firmwares

#### 1. Tested OK with ESP8266-AT shields

  - [`Ai-Thinker AT Firmware v1.5.4`](https://github.com/khoih-prog/ESP8266_AT_WebServer/blob/master/AT_Firmwares/At_firmware_bin1.54.zip)
  
    ```cpp
    AT version:1.1.0.0(May 11 2016 18:09:56)
    SDK version:1.5.4(baaeaebb)
    Ai-Thinker Technology Co. Ltd.
    Jun 13 2016 11:29:20
    ```
  - [`AT Firmware v1.7.4.0`](https://github.com/khoih-prog/ESP8266_AT_WebServer/blob/master/AT_Firmwares/AT_Firmware_bin_1.7.4.0.zip)
  
    ```cpp
    AT version:1.7.4.0(May 11 2020 19:13:04)
    SDK version:3.0.4(9532ceb)
    compile time:May 27 2020 10:12:17
    Bin version(Wroom 02):1.7.4
    ```
    
  - [`WIS600-01S`](https://www.aliexpress.com/item/32956707239.html) and [`W600`](https://www.seeedstudio.com/w600.html) using either ESP8266 or ESP32-AT commands and stock firmware
  
    ```cpp
    AT version:1.1.4(Dec 05 2018 11:06:45)
    SDK version:3.0.0
    Dec 05 2018 11:06:45
    ```
  
  
#### 2. Tested OK with ESP32-AT shields

  - [`AT version_2.1.0.0_dev`](https://github.com/khoih-prog/ESP8266_AT_WebServer/blob/master/AT_Firmwares/AT_version_2.1.0.0_dev.zip)
    
    ```cpp
    AT version:2.1.0.0-dev(4f6b92c - Jun 10 2020 10:36:54)
    SDK version:v4.0.1-193-ge7ac221b4
    compile time(b85a8df):Jun 18 2020 14:00:59
    Bin version:2.0.0(WROOM-32)
    ```
    
    See instructions at [AT Command Core](https://github.com/espressif/esp-at) and [ESP_AT_Get_Started](https://github.com/espressif/esp-at/blob/master/docs/en/get-started/ESP_AT_Get_Started.md)
  
  
#### 3. Where to upload AT-Firmware
  
Upload [`AT Firmware v1.7.4.0`](https://github.com/khoih-prog/ESP8266_AT_WebServer/blob/master/AT_Firmwares/AT_Firmware_bin_1.7.4.0.zip) bin files to correct locations as follows:

```ini
# BOOT MODE

### Flash size 8Mbit: 512KB+512KB
    boot_v1.2+.bin              0x00000
    user1.1024.new.2.bin        0x01000
    esp_init_data_default.bin   0xfc000
    blank.bin                   0x7e000 & 0xfe000


### Flash size 16Mbit-C1: 1024KB+1024KB
    boot_v1.2+.bin              0x00000
    user1.2048.new.5.bin        0x01000
    esp_init_data_default.bin   0x1fc000
    blank.bin                   0xfe000 & 0x1fe000
```

#### 4. Important Notes

* Test before using different AT-Firmware Version at your own risks. Just use any simple example to verify if the AT-firmware is OK.

* Compatible AT-Firmare version will be updated. Check for all supported AT Firmwares and download them from [AT_Firmwares](https://github.com/khoih-prog/ESP8266_AT_WebServer/tree/master/AT_Firmwares).

* Support to ESP32-AT-command shields has been added to permit using new library [ESP_AT_Lib](https://github.com/khoih-prog/ESP_AT_Lib) to replace [Blynk's BlynkESP8266_Lib](https://github.com/blynkkk/blynk-library/releases/Blynk_Release_v0.6.1.zip). The same [ESP_AT_Lib](https://github.com/khoih-prog/ESP_AT_Lib) can also be use for ESP8266-AT shields.

---
---

## Installation

### Use Arduino Library Manager

The best and easiest way is to use `Arduino Library Manager`. Search for `ESP8266_AT_Web_Server`, then select / install the latest version.
You can also use this link [![arduino-library-badge](https://www.ardu-badge.com/badge/ESP8266_AT_WebServer.svg?)](https://www.ardu-badge.com/ESP8266_AT_WebServer) for more detailed instructions.

### Manual Install

1. Navigate to [ESP8266_AT_WebServer](https://github.com/khoih-prog/ESP8266_AT_WebServer) page.
2. Download the latest release `ESP8266_AT_WebServer-master.zip`.
3. Extract the zip file to `ESP8266_AT_WebServer-master` directory 
4. Copy whole 
  - `ESP8266_AT_WebServer-master` folder to Arduino libraries' directory such as `~/Arduino/libraries/`.

### VS Code & PlatformIO:

1. Install [VS Code](https://code.visualstudio.com/)
2. Install [PlatformIO](https://platformio.org/platformio-ide)
3. Install [**ESP8266_AT_WebServer** library](https://registry.platformio.org/libraries/khoih-prog/ESP8266_AT_WebServer) by using [Library Manager](https://registry.platformio.org/libraries/khoih-prog/ESP8266_AT_WebServer/installation) or [Library Manager](https://platformio.org/lib/show/11453/ESP8266_AT_WebServer/installation). Search for **ESP8266_AT_WebServer** in [Platform.io Author's Libraries](https://platformio.org/lib/search?query=author:%22Khoi%20Hoang%22)
4. Use included [platformio.ini](platformio/platformio.ini) file from examples to ensure that all dependent libraries will installed automatically. Please visit documentation for the other options and examples at [Project Configuration File](https://docs.platformio.org/page/projectconf.html)


---
---

### Packages' Patches

#### 1. For Adafruit nRF52840 and nRF52832 boards

**To be able to compile, run and automatically detect and display BOARD_NAME on nRF52840/nRF52832 boards**, you have to copy the whole [nRF52 Packages_Patches](Packages_Patches/adafruit/hardware/nrf52/1.3.0) directory into Adafruit nRF52 directory (~/.arduino15/packages/adafruit/hardware/nrf52/1.3.0). 

Supposing the Adafruit nRF52 version is 1.3.0. These files must be copied into the directory:
- `~/.arduino15/packages/adafruit/hardware/nrf52/1.3.0/platform.txt`
- `~/.arduino15/packages/adafruit/hardware/nrf52/1.3.0/boards.txt`
- `~/.arduino15/packages/adafruit/hardware/nrf52/1.3.0/cores/nRF5/Udp.h`
- `~/.arduino15/packages/adafruit/hardware/nrf52/1.3.0/cores/nRF5/Print.h`
- `~/.arduino15/packages/adafruit/hardware/nrf52/1.3.0/cores/nRF5/Print.cpp`
- `~/.arduino15/packages/adafruit/hardware/nrf52/1.3.0/variants/NINA_B302_ublox/variant.h`
- `~/.arduino15/packages/adafruit/hardware/nrf52/1.3.0/variants/NINA_B302_ublox/variant.cpp`
- `~/.arduino15/packages/adafruit/hardware/nrf52/1.3.0/variants/NINA_B112_ublox/variant.h`
- `~/.arduino15/packages/adafruit/hardware/nrf52/1.3.0/variants/NINA_B112_ublox/variant.cpp`
- **`~/.arduino15/packages/adafruit/hardware/nrf52/1.3.0/cores/nRF5/Udp.h`**

Whenever a new version is installed, remember to copy these files into the new version directory. For example, new version is x.yy.z
These files must be copied into the directory:

- `~/.arduino15/packages/adafruit/hardware/nrf52/x.yy.z/platform.txt`
- `~/.arduino15/packages/adafruit/hardware/nrf52/x.yy.z/boards.txt`
- `~/.arduino15/packages/adafruit/hardware/nrf52/x.yy.z/cores/nRF5/Udp.h`
- `~/.arduino15/packages/adafruit/hardware/nrf52/x.yy.z/cores/nRF5/Print.h`
- `~/.arduino15/packages/adafruit/hardware/nrf52/x.yy.z/cores/nRF5/Print.cpp`
- `~/.arduino15/packages/adafruit/hardware/nrf52/x.yy.z/variants/NINA_B302_ublox/variant.h`
- `~/.arduino15/packages/adafruit/hardware/nrf52/x.yy.z/variants/NINA_B302_ublox/variant.cpp`
- `~/.arduino15/packages/adafruit/hardware/nrf52/x.yy.z/variants/NINA_B112_ublox/variant.h`
- `~/.arduino15/packages/adafruit/hardware/nrf52/x.yy.z/variants/NINA_B112_ublox/variant.cpp`
- **`~/.arduino15/packages/adafruit/hardware/nrf52/x.yy.z/cores/nRF5/Udp.h`**

#### 2. For Teensy boards
 
 **To be able to compile and run on Teensy boards**, you have to copy the files in [**Packages_Patches for Teensy directory**](Packages_Patches/hardware/teensy/avr) into Teensy hardware directory (./arduino-1.8.19/hardware/teensy/avr/boards.txt). 

Supposing the Arduino version is 1.8.19. These files must be copied into the directory:

- `./arduino-1.8.19/hardware/teensy/avr/boards.txt`
- `./arduino-1.8.19/hardware/teensy/avr/cores/teensy/Stream.h`
- `./arduino-1.8.19/hardware/teensy/avr/cores/teensy3/Stream.h`
- `./arduino-1.8.19/hardware/teensy/avr/cores/teensy4/Stream.h`

Whenever a new version is installed, remember to copy this file into the new version directory. For example, new version is x.yy.zz
These files must be copied into the directory:

- `./arduino-x.yy.zz/hardware/teensy/avr/boards.txt`
- `./arduino-x.yy.zz/hardware/teensy/avr/cores/teensy/Stream.h`
- `./arduino-x.yy.zz/hardware/teensy/avr/cores/teensy3/Stream.h`
- `./arduino-x.yy.zz/hardware/teensy/avr/cores/teensy4/Stream.h`

#### 3. For Arduino SAM DUE boards
 
 **To be able to compile and run on SAM DUE boards**, you have to copy the whole [SAM DUE](Packages_Patches/arduino/hardware/sam/1.6.12) directory into Arduino sam directory (~/.arduino15/packages/arduino/hardware/sam/1.6.12). 

Supposing the Arduino SAM core version is 1.6.12. This file must be copied into the directory:

- `~/.arduino15/packages/arduino/hardware/sam/1.6.12/platform.txt`

Whenever a new version is installed, remember to copy this file into the new version directory. For example, new version is x.yy.zz
This file must be copied into the directory:

- `~/.arduino15/packages/arduino/hardware/sam/x.yy.zz/platform.txt`

#### 4. For Arduino SAMD boards
 
 ***To be able to compile, run and automatically detect and display BOARD_NAME on Arduino SAMD (Nano-33-IoT, etc) boards***, you have to copy the whole [Arduino SAMD Packages_Patches](Packages_Patches/arduino/hardware/samd/1.8.13) directory into Arduino SAMD directory (~/.arduino15/packages/arduino/hardware/samd/1.8.13).
 
#### For core version v1.8.10+

Supposing the Arduino SAMD version is 1.8.13. Now only one file must be copied into the directory:

- `~/.arduino15/packages/arduino/hardware/samd/1.8.13/platform.txt`

Whenever a new version is installed, remember to copy this files into the new version directory. For example, new version is x.yy.zz

This file must be copied into the directory:

- `~/.arduino15/packages/arduino/hardware/samd/x.yy.zz/platform.txt`
 
#### For core version v1.8.9-

Supposing the Arduino SAMD version is 1.8.9. These files must be copied into the directory:

- `~/.arduino15/packages/arduino/hardware/samd/1.8.9/platform.txt`
- ***`~/.arduino15/packages/arduino/hardware/samd/1.8.9/cores/arduino/Arduino.h`***

Whenever a new version is installed, remember to copy these files into the new version directory. For example, new version is x.yy.z

These files must be copied into the directory:

- `~/.arduino15/packages/arduino/hardware/samd/x.yy.z/platform.txt`
- ***`~/.arduino15/packages/arduino/hardware/samd/x.yy.z/cores/arduino/Arduino.h`***
 
 This is mandatory to fix the ***notorious Arduino SAMD compiler error***. See [Improve Arduino compatibility with the STL (min and max macro)](https://github.com/arduino/ArduinoCore-samd/pull/399)
 
```
 ...\arm-none-eabi\include\c++\7.2.1\bits\stl_algobase.h:243:56: error: macro "min" passed 3 arguments, but takes just 2
     min(const _Tp& __a, const _Tp& __b, _Compare __comp)
```

Whenever the above-mentioned compiler error issue is fixed with the new Arduino SAMD release, you don't need to copy the `Arduino.h` file anymore.

#### 5. For Adafruit SAMD boards
 
 ***To be able to compile, run and automatically detect and display BOARD_NAME on Adafruit SAMD (Itsy-Bitsy M4, etc) boards***, you have to copy the whole [Adafruit SAMD Packages_Patches](Packages_Patches/adafruit/hardware/samd/1.7.11) directory into Adafruit samd directory (~/.arduino15/packages/adafruit/hardware/samd/1.7.11). 

Supposing the Adafruit SAMD core version is 1.7.11. These files must be copied into the directory:

- `~/.arduino15/packages/adafruit/hardware/samd/1.7.11/platform.txt`
- `~/.arduino15/packages/adafruit/hardware/samd/1.7.11/cores/arduino/Print.h`
- `~/.arduino15/packages/adafruit/hardware/samd/1.7.11/cores/arduino/Print.cpp`

Whenever a new version is installed, remember to copy this file into the new version directory. For example, new version is x.yy.zz
These files must be copied into the directory:

- `~/.arduino15/packages/adafruit/hardware/samd/x.yy.zz/platform.txt`
- `~/.arduino15/packages/adafruit/hardware/samd/x.yy.zz/cores/arduino/Print.h`
- `~/.arduino15/packages/adafruit/hardware/samd/x.yy.zz/cores/arduino/Print.cpp`

#### 6. For Seeeduino SAMD boards
 
 ***To be able to compile, run and automatically detect and display BOARD_NAME on Seeeduino SAMD (XIAO M0, Wio Terminal, etc) boards***, you have to copy the whole [Seeeduino SAMD Packages_Patches](Packages_Patches/Seeeduino/hardware/samd/1.8.3) directory into Seeeduino samd directory (~/.arduino15/packages/Seeeduino/hardware/samd/1.8.3). 

Supposing the Seeeduino SAMD core version is 1.8.3. These files must be copied into the directory:

- `~/.arduino15/packages/Seeeduino/hardware/samd/1.8.3/platform.txt`
- `~/.arduino15/packages/Seeeduino/hardware/samd/1.8.3/cores/arduino/Arduino.h`
- `~/.arduino15/packages/Seeeduino/hardware/samd/1.8.3/cores/arduino/Print.h`
- `~/.arduino15/packages/Seeeduino/hardware/samd/1.8.3/cores/arduino/Print.cpp`

Whenever a new version is installed, remember to copy this file into the new version directory. For example, new version is x.yy.zz
These files must be copied into the directory:

- `~/.arduino15/packages/Seeeduino/hardware/samd/x.yy.zz/platform.txt`
- `~/.arduino15/packages/Seeeduino/hardware/samd/x.yy.zz/cores/arduino/Arduino.h`
- `~/.arduino15/packages/Seeeduino/hardware/samd/x.yy.zz/cores/arduino/Print.h`
- `~/.arduino15/packages/Seeeduino/hardware/samd/x.yy.zz/cores/arduino/Print.cpp`

#### 7. For STM32 boards

#### 7.1 For STM32 boards to use LAN8720

To use LAN8720 on some STM32 boards 

- **Nucleo-144 (F429ZI, NUCLEO_F746NG, NUCLEO_F746ZG, NUCLEO_F756ZG)**
- **Discovery (DISCO_F746NG)**
- **STM32F4 boards (BLACK_F407VE, BLACK_F407VG, BLACK_F407ZE, BLACK_F407ZG, BLACK_F407VE_Mini, DIYMORE_F407VGT, FK407M1)**

you have to copy the files [stm32f4xx_hal_conf_default.h](Packages_Patches/STM32/hardware/stm32/2.3.0/system/STM32F4xx) and [stm32f7xx_hal_conf_default.h](Packages_Patches/STM32/hardware/stm32/2.3.0/system/STM32F7xx) into STM32 stm32 directory (~/.arduino15/packages/STM32/hardware/stm32/2.3.0/system) to overwrite the old files.

Supposing the STM32 stm32 core version is 2.3.0. These files must be copied into the directory:

- `~/.arduino15/packages/STM32/hardware/stm32/2.3.0/system/STM32F4xx/stm32f4xx_hal_conf_default.h` for STM32F4.
- `~/.arduino15/packages/STM32/hardware/stm32/2.3.0/system/STM32F7xx/stm32f7xx_hal_conf_default.h` for Nucleo-144 STM32F7.

Whenever a new version is installed, remember to copy this file into the new version directory. For example, new version is x.yy.zz,
theses files must be copied into the corresponding directory:

- `~/.arduino15/packages/STM32/hardware/stm32/x.yy.zz/system/STM32F4xx/stm32f4xx_hal_conf_default.h`
- `~/.arduino15/packages/STM32/hardware/stm32/x.yy.zz/system/STM32F7xx/stm32f7xx_hal_conf_default.h


#### 7.2 For STM32 boards to use Serial1

**To use Serial1 on some STM32 boards without Serial1 definition (Nucleo-144 NUCLEO_F767ZI, Nucleo-64 NUCLEO_L053R8, etc.) boards**, you have to copy the files [STM32 variant.h](Packages_Patches/STM32/hardware/stm32/2.3.0) into STM32 stm32 directory (~/.arduino15/packages/STM32/hardware/stm32/2.3.0). You have to modify the files corresponding to your boards, this is just an illustration how to do.

Supposing the STM32 stm32 core version is 2.3.0. These files must be copied into the directory:

- `~/.arduino15/packages/STM32/hardware/stm32/2.3.0/variants/STM32F7xx/F765Z(G-I)T_F767Z(G-I)T_F777ZIT/NUCLEO_F767ZI/variant.h` for Nucleo-144 NUCLEO_F767ZI.
- `~/.arduino15/packages/STM32/hardware/stm32/2.3.0/variants/STM32L0xx/L052R(6-8)T_L053R(6-8)T_L063R8T/NUCLEO_L053R8/variant.h` for Nucleo-64 NUCLEO_L053R8.

Whenever a new version is installed, remember to copy this file into the new version directory. For example, new version is x.yy.zz,
theses files must be copied into the corresponding directory:

- `~/.arduino15/packages/STM32/hardware/stm32/x.yy.zz/variants/STM32F7xx/F765Z(G-I)T_F767Z(G-I)T_F777ZIT/NUCLEO_F767ZI/variant.h`
- `~/.arduino15/packages/STM32/hardware/stm32/x.yy.zz/variants/STM32L0xx/L052R(6-8)T_L053R(6-8)T_L063R8T/NUCLEO_L053R8/variant.h`

#### 8. For RP2040-based boards using [Earle Philhower arduino-pico core](https://github.com/earlephilhower/arduino-pico)

#### 8.1 To use BOARD_NAME

 **To be able to automatically detect and display BOARD_NAME on RP2040-based boards (RASPBERRY_PI_PICO, ADAFRUIT_FEATHER_RP2040, GENERIC_RP2040, etc) boards**, you have to copy the file [RP2040 platform.txt](Packages_Patches/rp2040/hardware/rp2040/1.4.0) into rp2040 directory (~/.arduino15/packages/rp2040/hardware/rp2040/1.4.0). 

Supposing the rp2040 core version is 1.4.0. This file must be copied into the directory:

- `~/.arduino15/packages/rp2040/hardware/rp2040/1.4.0/platform.txt`

Whenever a new version is installed, remember to copy this file into the new version directory. For example, new version is x.yy.zz
This file must be copied into the directory:

- `~/.arduino15/packages/rp2040/hardware/rp2040/x.yy.zz/platform.txt`

With core after v1.5.0, this step is not necessary anymore thanks to the PR [Add -DBOARD_NAME="{build.board}" #136](https://github.com/earlephilhower/arduino-pico/pull/136).

#### 8.2 To avoid compile error relating to microsecondsToClockCycles

Some libraries, such as [Adafruit DHT-sensor-library](https://github.com/adafruit/DHT-sensor-library), require the definition of microsecondsToClockCycles(). **To be able to compile and run on RP2040-based boards**, you have to copy the files in [**RP2040 Arduino.h**](Packages_Patches/rp2040/hardware/rp2040/1.4.0/cores/rp2040/Arduino.h) into rp2040 directory (~/.arduino15/packages/rp2040/hardware/rp2040/1.4.0).

Supposing the rp2040 core version is 1.4.0. This file must be copied to replace:

- `~/.arduino15/packages/rp2040/hardware/rp2040/1.4.0/cores/rp2040/Arduino.h`

Whenever a new version is installed, remember to copy this file into the new version directory. For example, new version is x.yy.zz
This file must be copied to replace:

- `~/.arduino15/packages/rp2040/hardware/rp2040/x.yy.zz/cores/rp2040/Arduino.h`

With core after v1.5.0, this step is not necessary anymore thanks to the PR [Add defs for compatibility #142](https://github.com/earlephilhower/arduino-pico/pull/142).


#### 9. For Portenta_H7 boards using Arduino IDE in Linux

  **To be able to upload firmware to Portenta_H7 using Arduino IDE in Linux (Ubuntu, etc.)**, you have to copy the file [portenta_post_install.sh](Packages_Patches/arduino/hardware/mbed_portenta/3.4.1/portenta_post_install.sh) into mbed_portenta directory (~/.arduino15/packages/arduino/hardware/mbed_portenta/3.4.1/portenta_post_install.sh). 
  
  Then run the following command using `sudo`
  
```
$ cd ~/.arduino15/packages/arduino/hardware/mbed_portenta/3.4.1
$ chmod 755 portenta_post_install.sh
$ sudo ./portenta_post_install.sh
```

This will create the file `/etc/udev/rules.d/49-portenta_h7.rules` as follows:

```
# Portenta H7 bootloader mode UDEV rules

SUBSYSTEMS=="usb", ATTRS{idVendor}=="2341", ATTRS{idProduct}=="035b", GROUP="plugdev", MODE="0666"
```

Supposing the ArduinoCore-mbed core version is 3.4.1. Now only one file must be copied into the directory:

- `~/.arduino15/packages/arduino/hardware/mbed_portenta/3.4.1/portenta_post_install.sh`

Whenever a new version is installed, remember to copy this files into the new version directory. For example, new version is x.yy.zz

This file must be copied into the directory:

- `~/.arduino15/packages/arduino/hardware/mbed_portenta/x.yy.zz/portenta_post_install.sh`


#### 10. For RTL8720DN boards using AmebaD core
 
 To avoid compile error relating to PROGMEM, you have to copy the file [Realtek AmebaD core pgmspace.h](Packages_Patches/realtek/hardware/AmebaD/3.1.4/cores/ambd/avr/pgmspace.h) into Realtek AmebaD directory (~/.arduino15/packages/realtek/hardware/AmebaD/3.1.4/cores/ambd/avr/pgmspace.h). 

Supposing the Realtek AmebaD core version is 3.1.4. This file must be copied into the directory:

- `~/.arduino15/packages/realtek/hardware/AmebaD/3.1.4/cores/ambd/avr/pgmspace.h`

Whenever a new version is installed, remember to copy this file into the new version directory. For example, new version is x.yy.zz
This file must be copied into the directory:

- `~/.arduino15/packages/realtek/hardware/AmebaD/x.yy.zz/cores/ambd/avr/pgmspace.h`


#### 11. For SAMD21 and SAMD51 boards using ArduinoCore-fab-sam core
 
 To avoid compile error relating to SAMD21/SAMD51, you have to copy the file [ArduinoCore-fab-sam core pgmspace.h](Packages_Patches/Fab_SAM_Arduino/hardware/samd/1.9.0/boards.txt) into `ArduinoCore-fab-sam` samd directory (~/.arduino15/packages/Fab_SAM_Arduino/hardware/samd/1.9.0/boards.txt). 

Supposing the `ArduinoCore-fab-sam` samd core version is 1.9.0. This file must be copied into the directory:

- `~/.arduino15/packages/Fab_SAM_Arduino/hardware/samd/1.9.0/boards.txt`

Whenever a new version is installed, remember to copy this file into the new version directory. For example, new version is x.yy.zz
This file must be copied into the directory:

- `~/.arduino15/packages/Fab_SAM_Arduino/hardware/samd/x.yy.zz/boards.txt`


#### 12. For Seeeduino RP2040 boards
 
 ***To be able to compile, run and automatically detect and display BOARD_NAME on Seeeduino RP2040 (XIAO RP2040, Wio RP2040 Mini) boards***, you have to copy the whole [Seeeduino RP2040 Packages_Patches](Packages_Patches/Seeeduino/hardware/rp2040/2.7.2) directory into Seeeduino samd directory (~/.arduino15/packages/Seeeduino/hardware/rp2040/2.7.2). 

Supposing the Seeeduino RP2040 core version is 2.7.2. These files must be copied into the directory:

- `~/.arduino15/packages/Seeeduino/hardware/rp2040/2.7.2/boards.txt`
- `~/.arduino15/packages/Seeeduino/hardware/rp2040/2.7.2/variants/Seeed_XIAO_RP2040/pins_arduino.h`

Whenever a new version is installed, remember to copy this file into the new version directory. For example, new version is x.yy.zz
These files must be copied into the directory:

- `~/.arduino15/packages/Seeeduino/hardware/samd/x.yy.zz/boards.txt`
- `~/.arduino15/packages/Seeeduino/hardware/samd/x.yy.zz/variants/Seeed_XIAO_RP2040/pins_arduino.h`


---

#### 13. For Seeeduino nRF52840 boards

**To be able to compile and run on Xiao nRF52840 boards**, you have to copy the whole [nRF52 1.0.0](Packages_Patches/Seeeduino/hardware/nrf52/1.0.0) directory into Seeeduino nRF52 directory (~/.arduino15/packages/Seeeduino/hardware/nrf52/1.0.0). 

Supposing the Seeeduino nRF52 version is 1.0.0. These files must be copied into the directory:

- **`~/.arduino15/packages/Seeeduino/hardware/nrf52/1.0.0/platform.txt`**
- **`~/.arduino15/packages/Seeeduino/hardware/nrf52/1.0.0/cores/nRF5/Print.h`**
- **`~/.arduino15/packages/Seeeduino/hardware/nrf52/1.0.0/cores/nRF5/Print.cpp`**
- **`~/.arduino15/packages/Seeeduino/hardware/nrf52/1.0.0/cores/nRF5/Udp.h`**

Whenever a new version is installed, remember to copy these files into the new version directory. For example, new version is x.yy.z
These files must be copied into the directory:

- **`~/.arduino15/packages/Seeeduino/hardware/nrf52/x.yy.z/platform.txt`**
- **`~/.arduino15/packages/Seeeduino/hardware/nrf52/x.yy.z/cores/nRF5/Print.h`**
- **`~/.arduino15/packages/Seeeduino/hardware/nrf52/x.yy.z/cores/nRF5/Print.cpp`**
- **`~/.arduino15/packages/Seeeduino/hardware/nrf52/x.yy.z/cores/nRF5/Udp.h`**

---
---

### Usage

#### Class Constructor

```cpp
  ESP8266_AT_WebServer server(80);
```

Creates the ESP8266_AT_WebServer class object.

*Parameters:* 
 
host port number: ``int port`` (default is the standard HTTP port 80)

#### Basic Operations

**Starting the server**

```cpp
  void begin();
```

**Handling incoming client requests**

```cpp
  void handleClient();
```

**Disabling the server**

```cpp
  void close();
  void stop();
```

Both methods function the same

**Client request handlers**

```cpp
  void on();
  void addHandler();
  void onNotFound();
  void onFileUpload();	
```

Example:

```cpp
  server.on("/", handlerFunction);
  server.onNotFound(handlerFunction);   // called when handler is not assigned
  server.onFileUpload(handlerFunction); // handle file uploads
```

**Sending responses to the client**

```cpp
  void send();
  void send_P();
```

`Parameters:`

`code` - HTTP response code, can be `200` or `404`, etc.

`content_type` - HTTP content type, like `"text/plain"` or `"image/png"`, etc.

`content` - actual content body

#### Advanced Options

**Getting information about request arguments**

```cpp
  const String & arg();
  const String & argName();
  int args();
  bool hasArg();
```

`Function usage:`

`arg` - get request argument value, use `arg("plain")` to get POST body
	
`argName` - get request argument name
	
`args` - get arguments count
	
`hasArg` - check if argument exist

**Getting information about request headers**

```cpp
  const String & header();
  const String & headerName();
  const String & hostHeader();
  int headers();
  bool hasHeader();
``` 
`Function usage:`

`header` - get request header value

`headerName` - get request header name

`hostHeader` - get request host header if available, else empty string
  
`headers` - get header count
	
`hasHeader` - check if header exist

**Authentication**

```cpp
  bool authenticate();
  void requestAuthentication();
```

`Function usage:`

`authenticate` - server authentication, returns true if client is authenticated else false

`requestAuthentication` - sends authentication failure response to the client

`Example Usage:`

```cpp

  if(!server.authenticate(username, password)){
    server.requestAuthentication();
  }
```

#### Other Function Calls

```cpp
  const String & uri(); // get the current uri
  HTTPMethod  method(); // get the current method 
  WiFiClient client(); // get the current client
  HTTPUpload & upload(); // get the current upload
  void setContentLength(); // set content length
  void sendHeader(); // send HTTP header
  void sendContent(); // send content
  void sendContent_P(); 
  void collectHeaders(); // set the request headers to collect
  void serveStatic();
  size_t streamFile();
```

---
---

### Examples

Also see examples:

 1. [AdvancedWebServer](examples/AdvancedWebServer)
 2. [AdvancedWebServer_STM32](examples/AdvancedWebServer_STM32)
 3. [ConnectWPA](examples/ConnectWPA)
 4. [HelloServer](examples/HelloServer)
 5. [HelloServer2](examples/HelloServer2)
 6. [HttpBasicAuth](examples/HttpBasicAuth)
 7. [**MQTTClient_Auth**](examples/MQTTClient_Auth)
 8. [**MQTTClient_Basic**](examples/MQTTClient_Basic)
 9. [**MQTT_ThingStream**](examples/MQTT_ThingStream)
10. [PostServer](examples/PostServer)
11. [ScanNetworks](examples/ScanNetworks)
12. [SimpleAuthentication](examples/SimpleAuthentication)
13. [UdpNTPClient](examples/UdpNTPClient)
14. [UdpSendReceive](examples/UdpSendReceive)
15. [WebClient](examples/WebClient)
16. [WebClientRepeating](examples/WebClientRepeating)
17. [WebServer](examples/WebServer)
18. [WebServerAP](examples/WebServerAP)
19. [ATWebServer_BigData](examples/ATWebServer_BigData) **New**


---
---

### Example [AdvancedWebServer](examples/AdvancedWebServer)

Please take a look at other examples, as well.

#### 1. File [AdvancedWebServer.ino](examples/AdvancedWebServer/AdvancedWebServer.ino)


https://github.com/khoih-prog/ESP8266_AT_WebServer/blob/8ca8effb9c9707ac930b9298ef0d9e6af48f6e14/examples/AdvancedWebServer/AdvancedWebServer.ino#L60-L303

---

#### 2. File [defines.h](examples/AdvancedWebServer/defines.h)

https://github.com/khoih-prog/ESP8266_AT_WebServer/blob/8ca8effb9c9707ac930b9298ef0d9e6af48f6e14/examples/AdvancedWebServer/defines.h#L15-L397

---

### Debug Terminal Output Samples

#### 1. AdvancedWebServer on Adafruit NRF52840_ITSYBITSY_EXPRESS using ESP8266-AT shield

The following are debug terminal output when running example [AdvancedWebServer](examples/AdvancedWebServer) on NRF52840_ITSYBITSY_EXPRESS and ESP8266-AT shield.


```
Starting AdvancedWebServer on NRF52840_ITSYBITSY_EXPRESS
ESP8266_AT_WebServer v1.6.0
[ESP_AT] Initializing ESP module
[ESP_AT] Use ES8266-AT Command
WiFi shield init done
Connecting to WPA SSID: HueNet1
Starting server
[ESP_AT] Server started on port 80
HTTP server started @ 192.168.2.44
[ESP_AT] New client 0
ESP8266_AT_WebServer::handleClient: New Client
method:  GET
url:  /
search:
headerName: Host
headerValue: 192.168.2.44
headerName: Connection
headerValue: keep-alive
headerName: Cache-Control
headerValue: max-age=0
headerName: Upgrade-Insecure-Requests
headerValue: 1
headerName: DNT
headerValue: 1
headerName: Accept
headerValue: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9
headerName: Referer
headerValue: http://192.168.2.44/
headerName: Accept-Encoding
headerValue: gzip, deflate
[ESP_AT] TIMEOUT: 104
args:
args count:  0
args:
args count:  0
Request: /
Arguments: 
Final list of key/value pairs:
ESP8266_AT_WebServer::_handleRequest handle
[ESP_AT] ReqHandler::handle
AT_WebServer::send1: len =  314
content =  <html><head><meta http-equiv='refresh' content='5'/><title>ESP-AT NRF52840_ITSYBITSY_EXPRESS</title><style>body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }</style></head><body><h1>Hello from ESP-AT</h1><h3>on NRF52840_ITSYBITSY_EXPRESS</h3><p>Uptime: 0 d 00:00:23</p><img src="/test.svg" /></body></html>
AT_WebServer::_prepareHeader sendHeader Conn close
AT_WebServer::send1: write header =  HTTP/1.1 200 OK
Content-Type: text/html
Content-Length: 314
Connection: close

ESP8266_AT_WebServer::sendContent: Client.write content:  <html><head><meta http-equiv='refresh' content='5'/><title>ESP-AT AVR Mega</title><style>body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }</style></head><body><h1>Hello from ESP-AT</h1><h3>on NRF52840_ITSYBITSY_EXPRESS</h3><p>Uptime: 0 d 00:00:23</p><img src="/test.svg" /></body></html>
[ESP_AT] ReqHandler::handle done
ESP8266_AT_WebServer::_handleRequest OK
ESP8266_AT_WebServer::handleClient: Don't keepCurrentClient
ESP8266_AT_WebServer::handleClient: Client disconnected
[ESP_AT] New client 0
ESP8266_AT_WebServer::handleClient: New Client
method:  GET
url:  /test.svg
search:
headerName: Host
headerValue: 192.168.2.44
headerName: Connection
headerValue: keep-alive
headerName: User-Agent
headerValue: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 ike Gecko) Chrome/84.0.4147.89 Safari/537.36
headerName: DNT
headerValue: 1
headerName: Accept
headerValue: image/webp,image/apng,image/*,*/*;q=0.8
args:
args count:  0
args:
args count:  0
[ESP_AT] TIMEOUT: 81
Request: /test.svg
Arguments: 
Final list of key/value pairs:
ESP8266_AT_WebServer::_handleRequest handle
[ESP_AT] ReqHandler::handle
AT_WebServer::send1: len =  1949
content =  <svg xmlns="http://www.w3.org/2000/svg" version="1.1" width="310" height="150">
<rect width="310" height="150" fill="rgb(250, 230, 210)" stroke-width="1" stroke="rgb(0, 0, 0)" />
<g stroke="blue">
<line x1="10" y1="103" x2="20" y2="131" stroke-width="1" />
<line x1="20" y1="131" x2="30" y2="99" stroke-width="1" />
<line x1="30" y1="99" x2="40" y2="16" stroke-width="1" />
<line x1="40" y1="16" x2="50" y2="100" stroke-width="1" />
<line x1="50" y1="100" x2="60" y2="40" stroke-width="1" />
<line x1="60" y1="40" x2="70" y2="110" stroke-width="1" />
<line x1="70" y1="110" x2="80" y2="52" stroke-width="1" />
<line x1="80" y1="52" x2="90" y2="131" stroke-width="1" />
<line x1="90" y1="131" x2="100" y2="79" stroke-width="1" />
<line x1="100" y1="79" x2="110" y2="84" stroke-width="1" />
<line x1="110" y1="84" x2="120" y2="35" stroke-width="1" />
<line x1="120" y1="35" x2="130" y2="72" stroke-width="1" />
<line x1="130" y1="72" x2="140" y2="18" stroke-width="1" />
<line x1="140" y1="18" x2="150" y2="37" stroke-width="1" />
<line x1="150" y1="37" x2="160" y2="29" stroke-width="1" />
<line x1="160" y1="29" x2="170" y2="85" stroke-width="1" />
<line x1="170" y1="85" x2="180" y2="123" stroke-width="1" />
<line x1="180" y1="123" x2="190" y2="126" stroke-width="1" />
<line x1="190" y1="126" x2="200" y2="88" stroke-width="1" />
<line x1="200" y1="88" x2="210" y2="17" stroke-width="1" />
<line x1="210" y1="17" x2="220" y2="35" stroke-width="1" />
<line x1="220" y1="35" x2="230" y2="53" stroke-width="1" />
<line x1="230" y1="53" x2="240" y2="49" stroke-width="1" />
<line x1="240" y1="49" x2="250" y2="122" stroke-width="1" />
<line x1="250" y1="122" x2="260" y2="43" stroke-width="1" />
<line x1="260" y1="43" x2="270" y2="37" stroke-width="1" />
<line x1="270" y1="37" x2="280" y2="42" stroke-width="1" />
<line x1="280" y1="42" x2="290" y2="90" stroke-width="1" />
<line x1="290" y1="90" x2="300" y2="119" stroke-width="1" />
</g>
</svg>

AT_WebServer::_prepareHeader sendHeader Conn close
AT_WebServer::send1: write header =  HTTP/1.1 200 OK
Content-Type: image/svg+xml
Content-Length: 1949
Connection: close
```

---

#### 2. MQTT_ThingStream on Adafruit SAMD51 ITSYBITSY_M4 using ESP8266-AT shield

The following are debug terminal output when running example [MQTT_ThingStream](examples/MQTT_ThingStream) on Adafruit SAMD51 ITSYBITSY_M4 and ESP8266-AT shield.


```
Start MQTT_ThingStream on ITSYBITSY_M4 with ESP8266-AT & ESP8266_AT_WebServer Library
ESP8266_AT_WebServer v1.6.0
[ESP_AT] Use ES8266-AT Command
WiFi shield init done
Connecting to SSID: HueNet1
You're connected to the network, IP = 192.168.2.62
SSID: HueNet1, Signal strength (RSSI): -43 dBm
***************************************
STM32_Pub
***************************************
Attempting MQTT connection to broker.emqx.io
...connected
Published connection message successfully!
Subcribed to: STM32_Sub
MQTT Message Send : STM32_Pub => Hello from MQTT_ThingStream on ITSYBITSY_M4 with ESP8266-AT & ESP8266_AT_WebServer Library
MQTT Message receive [STM32_Pub] Hello from MQTT_ThingStream on ITSYBITSY_M4 with ESP8266-AT & ESP8266_AT_WebServer Library
MQTT Message Send : STM32_Pub => Hello from MQTT_ThingStream on ITSYBITSY_M4 with ESP8266-AT & ESP8266_AT_WebServer Library
MQTT Message receive [STM32_Pub] Hello from MQTT_ThingStream on ITSYBITSY_M4 with ESP8266-AT & ESP8266_AT_WebServer Library
MQTT Message Send : STM32_Pub => Hello from MQTT_ThingStream on ITSYBITSY_M4 with ESP8266-AT & ESP8266_AT_WebServer Library
MQTT Message receive [STM32_Pub] Hello from MQTT_ThingStream on ITSYBITSY_M4 with ESP8266-AT & ESP8266_AT_WebServer Library
MQTT Message Send : STM32_Pub => Hello from MQTT_ThingStream on ITSYBITSY_M4 with ESP8266-AT & ESP8266_AT_WebServer Library
MQTT Message receive [STM32_Pub] Hello from MQTT_ThingStream on ITSYBITSY_M4 with ESP8266-AT & ESP8266_AT_WebServer Library
MQTT Message Send : STM32_Pub => Hello from MQTT_ThingStream on ITSYBITSY_M4 with ESP8266-AT & ESP8266_AT_WebServer Library
MQTT Message receive [STM32_Pub] Hello from MQTT_ThingStream on ITSYBITSY_M4 with ESP8266-AT & ESP8266_AT_WebServer Library
MQTT Message Send : STM32_Pub => Hello from MQTT_ThingStream on ITSYBITSY_M4 with ESP8266-AT & ESP8266_AT_WebServer Library
MQTT Message receive [STM32_Pub] Hello from MQTT_ThingStream on ITSYBITSY_M4 with ESP8266-AT & ESP8266_AT_WebServer Library
```

---

#### 3. MQTTClient_Auth on Adafruit SAMD51 ITSYBITSY_M4 using ESP8266-AT shield

The following are debug terminal output when running example [MQTTClient_Auth](examples/MQTTClient_Auth) on Adafruit SAMD51 ITSYBITSY_M4 and ESP8266-AT shield.

```
Starting MQTTClient_Auth on ITSYBITSY_M4 with ESP8266-AT & ESP8266_AT_WebServer Library
ESP8266_AT_WebServer v1.6.0
[ESP_AT] Use ES8266-AT Command
WiFi shield init done
Connecting to SSID: HueNet1
You're connected to the network, IP = 192.168.2.62
SSID: HueNet1, Signal strength (RSSI): -43 dBm
Attempting MQTT connection to broker.emqx.io...connected
Message Send : MQTT_Pub => Hello from MQTTClient_Auth on ITSYBITSY_M4 with ESP8266-AT & ESP8266_AT_WebServer Library
Message arrived [MQTT_Pub] Hello from MQTTClient_Auth on ITSYBITSY_M4 with ESP8266-AT & ESP8266_AT_WebServer Library
Message Send : MQTT_Pub => Hello from MQTTClient_Auth on ITSYBITSY_M4 with ESP8266-AT & ESP8266_AT_WebServer Library
Message arrived [MQTT_Pub] Hello from MQTTClient_Auth on ITSYBITSY_M4 with ESP8266-AT & ESP8266_AT_WebServer Library
Message Send : MQTT_Pub => Hello from MQTTClient_Auth on ITSYBITSY_M4 with ESP8266-AT & ESP8266_AT_WebServer Library
Message arrived [MQTT_Pub] Hello from MQTTClient_Auth on ITSYBITSY_M4 with ESP8266-AT & ESP8266_AT_WebServer Library
Message Send : MQTT_Pub => Hello from MQTTClient_Auth on ITSYBITSY_M4 with ESP8266-AT & ESP8266_AT_WebServer Library
Message arrived [MQTT_Pub] Hello from MQTTClient_Auth on ITSYBITSY_M4 with ESP8266-AT & ESP8266_AT_WebServer Library
```

---

#### 4. MQTTClient_Basic on Adafruit SAMD51 ITSYBITSY_M4 using ESP8266-AT shield

The following are debug terminal output when running example [MQTTClient_Basic](examples/MQTTClient_Basic) on Adafruit SAMD51 ITSYBITSY_M4 and ESP8266-AT shield.


```
MQTTClient_Basic on ITSYBITSY_M4 with ESP8266-AT & ESP8266_AT_WebServer Library
ESP8266_AT_WebServer v1.6.0
[ESP_AT] Use ES8266-AT Command
WiFi shield init done
Connecting to SSID: HueNet1
You're connected to the network, IP = 192.168.2.62
SSID: HueNet1, Signal strength (RSSI): -41 dBm
Attempting MQTT connection to broker.shiftr.io...connected
Message Send : MQTT_Pub => Hello from MQTTClient_Basic on ITSYBITSY_M4 with ESP8266-AT & ESP8266_AT_WebServer Library
Message arrived [MQTT_Pub] Hello from MQTTClient_Basic on ITSYBITSY_M4 with ESP8266-AT & ESP8266_AT_WebServer Library
Message Send : MQTT_Pub => Hello from MQTTClient_Basic on ITSYBITSY_M4 with ESP8266-AT & ESP8266_AT_WebServer Library
Message Send : MQTT_Pub => Hello from MQTTClient_Basic on ITSYBITSY_M4 with ESP8266-AT & ESP8266_AT_WebServer Library
Message Send : MQTT_Pub => Hello from MQTTClient_Basic on ITSYBITSY_M4 with ESP8266-AT & ESP8266_AT_WebServer Library
Message Send : MQTT_Pub => Hello from MQTTClient_Basic on ITSYBITSY_M4 with ESP8266-AT & ESP8266_AT_WebServer Library
Message Send : MQTT_Pub => Hello from MQTTClient_Basic on ITSYBITSY_M4 with ESP8266-AT & ESP8266_AT_WebServer Library
```

---

#### 5. WebClientRepeating on RASPBERRY_PI_PICO using ESP8266-AT shield

The following are debug terminal output when running example [WebClientRepeating](examples/WebClientRepeating) on RP2040-based RASPBERRY_PI_PICO and ESP8266-AT shield.


```
Starting WebClientRepeating on RASPBERRY_PI_PICO with ESP8266-AT & ESP8266_AT_WebServer Library
ESP8266_AT_WebServer v1.6.0
[ESP_AT] Use ES8266-AT Command
WiFi shield init done
Connecting to SSID: HueNet1
You're connected to the network, IP = 192.168.2.185
SSID: HueNet1, Signal strength (RSSI): -37 dBm

Connecting...
HTTP/1.1 200 OK
Server: nginx/1.4.2
Date: Tue, 11 May 2021 06:21:15 GMT
Content-Type: text/plain
Content-Length: 2263
Last-Modified: Wed, 02 Oct 2013 13:46:47 GMT
Connection: close
Vary: Accept-Encoding
ETag: "524c23c7-8d7"
Accept-Ranges: bytes


           `:;;;,`                      .:;;:.           
        .;;;;;;;;;;;`                :;;;;;;;;;;:     TM 
      `;;;;;;;;;;;;;;;`            :;;;;;;;;;;;;;;;      
     :;;;;;;;;;;;;;;;;;;         `;;;;;;;;;;;;;;;;;;     
    ;;;;;;;;;;;;;;;;;;;;;       .;;;;;;;;;;;;;;;;;;;;    
   ;;;;;;;;:`   `;;;;;;;;;     ,;;;;;;;;.`   .;;;;;;;;   
  .;;;;;;,         :;;;;;;;   .;;;;;;;          ;;;;;;;  
  ;;;;;;             ;;;;;;;  ;;;;;;,            ;;;;;;. 
 ,;;;;;               ;;;;;;.;;;;;;`              ;;;;;; 
 ;;;;;.                ;;;;;;;;;;;`      ```       ;;;;;`
 ;;;;;                  ;;;;;;;;;,       ;;;       .;;;;;
`;;;;:                  `;;;;;;;;        ;;;        ;;;;;
,;;;;`    `,,,,,,,,      ;;;;;;;      .,,;;;,,,     ;;;;;
:;;;;`    .;;;;;;;;       ;;;;;,      :;;;;;;;;     ;;;;;
:;;;;`    .;;;;;;;;      `;;;;;;      :;;;;;;;;     ;;;;;
.;;;;.                   ;;;;;;;.        ;;;        ;;;;;
 ;;;;;                  ;;;;;;;;;        ;;;        ;;;;;
 ;;;;;                 .;;;;;;;;;;       ;;;       ;;;;;,
 ;;;;;;               `;;;;;;;;;;;;                ;;;;; 
 `;;;;;,             .;;;;;; ;;;;;;;              ;;;;;; 
  ;;;;;;:           :;;;;;;.  ;;;;  ;;;;;;;`       .;;;;;;;,    ;;;;;;;;        ;;;;;;;:  
    ;;;;;;;;;:,:;;;;;;;;;:      ;;;;;;;;;;:,;;;;;;;;;;   
    `;;;;;;;;;;;;;;;;;;;.        ;;;;;;;;;;;;;;;;;;;;    
      ;;;;;;;;;;;;;;;;;           :;;;;;;;;;;;;;;;;:     
       ,;;;;;;;;;;;;;,              ;;;;;;;;;;;;;;       
         .;;;;;;;;;`                  ,;;;;;;;;:         
                                                         
                                                         
                                                         
                                                         
    ;;;   ;;;;;`  ;;;;:  .;;  ;; ,;;;;;, ;;. `;,  ;;;;   
    ;;;   ;;:;;;  ;;;;;; .;;  ;; ,;;;;;: ;;; `;, ;;;:;;  
   ,;:;   ;;  ;;  ;;  ;; .;;  ;;   ,;,   ;;;,`;, ;;  ;;  
   ;; ;:  ;;  ;;  ;;  ;; .;;  ;;   ,;,   ;;;;`;, ;;  ;;. 
   ;: ;;  ;;;;;:  ;;  ;; .;;  ;;   ,;,   ;;`;;;, ;;  ;;` 
  ,;;;;;  ;;`;;   ;;  ;; .;;  ;;   ,;,   ;; ;;;, ;;  ;;  
  ;;  ,;, ;; .;;  ;;;;;:  ;;;;;: ,;;;;;: ;;  ;;, ;;;;;;  
  ;;   ;; ;;  ;;` ;;;;.   `;;;:  ,;;;;;, ;;  ;;,  ;;;;   
3,CLOSED
```

---

#### 6. MQTTClient_Auth on RASPBERRY_PI_PICO using ESP8266-AT shield

The following are debug terminal output when running example [MQTTClient_Auth](examples/MQTTClient_Auth) on RP2040-based RASPBERRY_PI_PICO and ESP8266-AT shield using [Earle Philhower's arduino-pico core](https://github.com/earlephilhower/arduino-pico).

```
Starting MQTTClient_Auth on RASPBERRY_PI_PICO with ESP8266-AT & ESP8266_AT_WebServer Library
ESP8266_AT_WebServer v1.6.0
[ESP_AT] Use ES8266-AT Command
WiFi shield init done
Connecting to SSID: HueNet1
You're connected to the network, IP = 192.168.2.185
SSID: HueNet1, Signal strength (RSSI): -36 dBm
Attempting MQTT connection to broker.emqx.io...connected
Message Send : MQTT_Pub => Hello from MQTTClient_Auth on RASPBERRY_PI_PICO with ESP8266-AT & ESP8266_AT_WebServer Library
Message arrived [MQTT_Pub] Hello from MQTTClient_Auth on RASPBERRY_PI_PICO with ESP8266-AT & ESP8266_AT_WebServer Library
Message Send : MQTT_Pub => Hello from MQTTClient_Auth on RASPBERRY_PI_PICO with ESP8266-AT & ESP8266_AT_WebServer Library
Message arrived [MQTT_Pub] Hello from MQTTClient_Auth on RASPBERRY_PI_PICO with ESP8266-AT & ESP8266_AT_WebServer Library
Message Send : MQTT_Pub => Hello from MQTTClient_Auth on RASPBERRY_PI_PICO with ESP8266-AT & ESP8266_AT_WebServer Library
Message arrived [MQTT_Pub] Hello from MQTTClient_Auth on RASPBERRY_PI_PICO with ESP8266-AT & ESP8266_AT_WebServer Library
Message Send : MQTT_Pub => Hello from MQTTClient_Auth on RASPBERRY_PI_PICO with ESP8266-AT & ESP8266_AT_WebServer Library
Message arrived [MQTT_Pub] Hello from MQTTClient_Auth on RASPBERRY_PI_PICO with ESP8266-AT & ESP8266_AT_WebServer Library
```

---

#### 7. UdpNTPClient on MBED RASPBERRY_PI_PICO using ESP8266-AT shield

The following are debug terminal output when running example [UdpNTPClient](examples/UdpNTPClient) on RP2040-based MBED RASPBERRY_PI_PICO and ESP8266-AT shield using [**Arduino-mbed RP2040** core](https://github.com/arduino/ArduinoCore-mbed).

```
Starting UdpNTPClient on MBED RASPBERRY_PI_PICO with ESP8266-AT & ESP8266_AT_WebServer Library
ESP8266_AT_WebServer v1.6.0
[ESP_AT] Use ES8266-AT Command
WiFi shield init done
Connecting to WPA SSID: HueNet1
You're connected to the network, IP = 192.168.2.76
UDP Packet received, size 48
From 132.163.96.1, port 123
Seconds since Jan 1 1900 = 3831340334
Unix time = 1622351534
The UTC time is 5:12:14
UDP Packet received, size 48
From 132.163.96.1, port 123
Seconds since Jan 1 1900 = 3831340344
Unix time = 1622351544
The UTC time is 5:12:24
```

---

#### 8. [AdvancedWebServer](examples/AdvancedWebServer) on NRF52840_ITSYBITSY

The following is debug terminal output when running example [AdvancedServer](examples/AdvancedServer) on `NRF52840_ITSYBITSY` board, to demo how to send much larger data than total 2K of `ESP8266/ESP32 AT-command` shields

##### MULTIPLY_FACTOR = 4.0f


```
Starting AdvancedServer on NRF52840_ITSYBITSY with ESP8266-AT & ESP8266_AT_WebServer Library
ESP8266_AT_WebServer v1.6.0
[ESP_AT] Initializing ESP module
[ESP_AT] Use ES8266-AT Command
WiFi shield init done
Connecting to WPA SSID: HueNet1
[ESP_AT] Server started on port 80
HTTP server started @ 192.168.2.142, Port = 80
.[ESP_AT] New client 0
[ESP_AT] ESP8266_AT_Client::write: size =  84 , MAX_SIZE = 2048
[ESP_AT] ESP8266_AT_Client::write: written =  84 , totalBytesSent = 84
[ESP_AT] ESP8266_AT_Client::write: Done, written =  84 , totalBytesSent = 84
[ESP_AT] ESP8266_AT_Client::write: size =  396 , MAX_SIZE = 2048
[ESP_AT] ESP8266_AT_Client::write: written =  396 , totalBytesSent = 396
[ESP_AT] ESP8266_AT_Client::write: Done, written =  396 , totalBytesSent = 396
[ESP_AT] handleClient: Client disconnected
[ESP_AT] New client 0
[ESP_AT] String Len =  11219
[ESP_AT] ESP8266_AT_Client::write: size =  90 , MAX_SIZE = 2048
[ESP_AT] ESP8266_AT_Client::write: written =  90 , totalBytesSent = 90
[ESP_AT] ESP8266_AT_Client::write: Done, written =  90 , totalBytesSent = 90
[ESP_AT] ESP8266_AT_Client::write: size =  11219 , MAX_SIZE = 2048
[ESP_AT] ESP8266_AT_Client::write: written =  2048 , totalBytesSent = 2048
[ESP_AT] ESP8266_AT_Client::write: Partially Done, written =  2048 , bytesRemaining = 9171
[ESP_AT] ESP8266_AT_Client::write: written =  2048 , totalBytesSent = 4096
[ESP_AT] ESP8266_AT_Client::write: Partially Done, written =  2048 , bytesRemaining = 7123
[ESP_AT] ESP8266_AT_Client::write: written =  2048 , totalBytesSent = 6144
[ESP_AT] ESP8266_AT_Client::write: Partially Done, written =  2048 , bytesRemaining = 5075
[ESP_AT] ESP8266_AT_Client::write: written =  2048 , totalBytesSent = 8192
[ESP_AT] ESP8266_AT_Client::write: Partially Done, written =  2048 , bytesRemaining = 3027
[ESP_AT] ESP8266_AT_Client::write: written =  2048 , totalBytesSent = 10240
[ESP_AT] ESP8266_AT_Client::write: Partially Done, written =  2048 , bytesRemaining = 979
[ESP_AT] ESP8266_AT_Client::write: written =  979 , totalBytesSent = 11219
[ESP_AT] ESP8266_AT_Client::write: Done, written =  979 , totalBytesSent = 11219
[ESP_AT] handleClient: Client disconnected
[ESP_AT] New client 0
[ESP_AT] ESP8266_AT_Client::write: size =  84 , MAX_SIZE = 2048
[ESP_AT] ESP8266_AT_Client::write: written =  84 , totalBytesSent = 84
[ESP_AT] ESP8266_AT_Client::write: Done, written =  84 , totalBytesSent = 84
[ESP_AT] ESP8266_AT_Client::write: size =  396 , MAX_SIZE = 2048
[ESP_AT] ESP8266_AT_Client::write: written =  396 , totalBytesSent = 396
[ESP_AT] ESP8266_AT_Client::write: Done, written =  396 , totalBytesSent = 396
[ESP_AT] handleClient: Client disconnected
[ESP_AT] New client 0
[ESP_AT] String Len =  11209
[ESP_AT] ESP8266_AT_Client::write: size =  90 , MAX_SIZE = 2048
[ESP_AT] ESP8266_AT_Client::write: written =  90 , totalBytesSent = 90
[ESP_AT] ESP8266_AT_Client::write: Done, written =  90 , totalBytesSent = 90
[ESP_AT] ESP8266_AT_Client::write: size =  11209 , MAX_SIZE = 2048
[ESP_AT] ESP8266_AT_Client::write: written =  2048 , totalBytesSent = 2048
[ESP_AT] ESP8266_AT_Client::write: Partially Done, written =  2048 , bytesRemaining = 9161
[ESP_AT] ESP8266_AT_Client::write: written =  2048 , totalBytesSent = 4096
[ESP_AT] ESP8266_AT_Client::write: Partially Done, written =  2048 , bytesRemaining = 7113
[ESP_AT] ESP8266_AT_Client::write: written =  2048 , totalBytesSent = 6144
[ESP_AT] ESP8266_AT_Client::write: Partially Done, written =  2048 , bytesRemaining = 5065
[ESP_AT] ESP8266_AT_Client::write: written =  2048 , totalBytesSent = 8192
[ESP_AT] ESP8266_AT_Client::write: Partially Done, written =  2048 , bytesRemaining = 3017
[ESP_AT] ESP8266_AT_Client::write: written =  2048 , totalBytesSent = 10240
[ESP_AT] ESP8266_AT_Client::write: Partially Done, written =  2048 , bytesRemaining = 969
[ESP_AT] ESP8266_AT_Client::write: written =  969 , totalBytesSent = 11209
[ESP_AT] ESP8266_AT_Client::write: Done, written =  969 , totalBytesSent = 11209
[ESP_AT] handleClient: Client disconnected
```

---

#### 9. [ATWebServer_BigData](examples/ATWebServer_BigData) on NRF52840_ITSYBITSY

The following is debug terminal output when running example [ATWebServer_BigData](examples/ATWebServer_BigData) on `NRF52840_ITSYBITSY` board, to demo how to send much larger data than total 2K of `ESP8266/ESP32 AT-command` shields

##### MULTIPLY_FACTOR = 3.0f


```
Start ATWebServer_BigData on NRF52840_ITSYBITSY with ESP8266-AT & ESP8266_AT_WebServer Library
ESP8266_AT_WebServer v1.6.0
[ESP_AT] Initializing ESP module
[ESP_AT] Use ES8266-AT Command
WiFi shield init done
Connecting to WPA SSID: HueNet1
[ESP_AT] Server started on port 80
HTTP server started @ 192.168.2.142, Port = 80
[ESP_AT] New client 0
String Len = 23106
[ESP_AT] ESP8266_AT_Client::write: size =  86 , MAX_SIZE = 2048
[ESP_AT] ESP8266_AT_Client::write: written =  86 , totalBytesSent = 86
[ESP_AT] ESP8266_AT_Client::write: Done, written =  86 , totalBytesSent = 86
[ESP_AT] ESP8266_AT_Client::write: size =  23106 , MAX_SIZE = 2048
[ESP_AT] ESP8266_AT_Client::write: written =  2048 , totalBytesSent = 2048
[ESP_AT] ESP8266_AT_Client::write: Partially Done, written =  2048 , bytesRemaining = 21058
[ESP_AT] ESP8266_AT_Client::write: written =  2048 , totalBytesSent = 4096
[ESP_AT] ESP8266_AT_Client::write: Partially Done, written =  2048 , bytesRemaining = 19010
[ESP_AT] ESP8266_AT_Client::write: written =  2048 , totalBytesSent = 6144
[ESP_AT] ESP8266_AT_Client::write: Partially Done, written =  2048 , bytesRemaining = 16962
[ESP_AT] ESP8266_AT_Client::write: written =  2048 , totalBytesSent = 8192
[ESP_AT] ESP8266_AT_Client::write: Partially Done, written =  2048 , bytesRemaining = 14914
[ESP_AT] ESP8266_AT_Client::write: written =  2048 , totalBytesSent = 10240
[ESP_AT] ESP8266_AT_Client::write: Partially Done, written =  2048 , bytesRemaining = 12866
[ESP_AT] ESP8266_AT_Client::write: written =  2048 , totalBytesSent = 12288
[ESP_AT] ESP8266_AT_Client::write: Partially Done, written =  2048 , bytesRemaining = 10818
[ESP_AT] ESP8266_AT_Client::write: written =  2048 , totalBytesSent = 14336
[ESP_AT] ESP8266_AT_Client::write: Partially Done, written =  2048 , bytesRemaining = 8770
[ESP_AT] ESP8266_AT_Client::write: written =  2048 , totalBytesSent = 16384
[ESP_AT] ESP8266_AT_Client::write: Partially Done, written =  2048 , bytesRemaining = 6722
[ESP_AT] ESP8266_AT_Client::write: written =  2048 , totalBytesSent = 18432
[ESP_AT] ESP8266_AT_Client::write: Partially Done, written =  2048 , bytesRemaining = 4674
[ESP_AT] ESP8266_AT_Client::write: written =  2048 , totalBytesSent = 20480
[ESP_AT] ESP8266_AT_Client::write: Partially Done, written =  2048 , bytesRemaining = 2626
[ESP_AT] ESP8266_AT_Client::write: written =  2048 , totalBytesSent = 22528
[ESP_AT] ESP8266_AT_Client::write: Partially Done, written =  2048 , bytesRemaining = 578
[ESP_AT] ESP8266_AT_Client::write: written =  578 , totalBytesSent = 23106
[ESP_AT] ESP8266_AT_Client::write: Done, written =  578 , totalBytesSent = 23106
[ESP_AT] handleClient: Client disconnected
```


---
---

### Screen Shots

#### 1. SAMD51 Itsy-Bitsy M4

This is the screen shot when running example [AdvancedWebServer](https://github.com/khoih-prog/ESP8266_AT_WebServer/tree/master/examples/AdvancedWebServer) on **Adafruit SAMD51 Itsy-Bitsy M4** board using this [ESP8266_AT_WebServer Library](https://github.com/khoih-prog/ESP8266_AT_WebServer)

<p align="center">
    <img src="https://github.com/khoih-prog/ESP8266_AT_WebServer/raw/master/pics/AdvancedWebServer.png">
</p>


#### 2. STM32 Nucleo-144 NUCLEO_F767ZI

This is the screen shot when running example [AdvancedWebServer_STM32](https://github.com/khoih-prog/ESP8266_AT_WebServer/tree/master/examples/AdvancedWebServer_STM32) on **STM32 Nucleo-144 NUCLEO_F767ZI** board using this [ESP8266_AT_WebServer Library](https://github.com/khoih-prog/ESP8266_AT_WebServer)

<p align="center">
    <img src="https://github.com/khoih-prog/ESP8266_AT_WebServer/raw/master/pics/AdvancedWebServer_STM32.png">
</p>

#### 3. nRF52 Itsy-Bitsy nRF52840

This is the screen shot when running example [AdvancedWebServer](https://github.com/khoih-prog/ESP8266_AT_WebServer/tree/master/examples/AdvancedWebServer) on **Adadruit nRF52840 Itsy-Bitsy** board using this [ESP8266_AT_WebServer Library](https://github.com/khoih-prog/ESP8266_AT_WebServer)

<p align="center">
    <img src="https://github.com/khoih-prog/ESP8266_AT_WebServer/raw/master/pics/AdvancedWebServer_nRF52840.png">
</p>


#### 4. Seeeduino SEEED_XIAO_M0

This is the screen shot when running an example using UDP Multicast on **Seeeduino SEEED_XIAO_M0** board using this [ESP8266_AT_WebServer Library](https://github.com/khoih-prog/ESP8266_AT_WebServer). The UDP MUlticast is used to manage Internet Gateway's Port Forwarding so that we can access from WAN, port 5990, the WebServer running in LAN, port 5990.

<p align="center">
    <img src="https://github.com/khoih-prog/ESP8266_AT_WebServer/raw/master/pics/UPnP_Multicast.png">
</p>


#### 5. RASPBERRY_PI_PICO

This is the screen shot when running example [AdvancedWebServer](https://github.com/khoih-prog/ESP8266_AT_WebServer/tree/master/examples/AdvancedWebServer) on **RASPBERRY_PI_PICO** board using this [ESP8266_AT_WebServer Library](https://github.com/khoih-prog/ESP8266_AT_WebServer)

<p align="center">
    <img src="https://github.com/khoih-prog/ESP8266_AT_WebServer/raw/master/pics/AdvancedWebServer_RPi_Pico.png">
</p>


#### 6. MBED RASPBERRY_PI_PICO

This is the screen shot when running example [AdvancedWebServer](https://github.com/khoih-prog/ESP8266_AT_WebServer/tree/master/examples/AdvancedWebServer) on **MBED RASPBERRY_PI_PICO** board using this [ESP8266_AT_WebServer Library](https://github.com/khoih-prog/ESP8266_AT_WebServer)

<p align="center">
    <img src="https://github.com/khoih-prog/ESP8266_AT_WebServer/raw/master/pics/AdvancedWebServer_Mbed_RPi_Pico.png">
</p>


#### 7. nRF52 Itsy-Bitsy nRF52840 BigData

This is the screen shot when running example [AdvancedWebServer](https://github.com/khoih-prog/ESP8266_AT_WebServer/tree/master/examples/AdvancedWebServer) on **Adadruit nRF52840 Itsy-Bitsy** board using this [ESP8266_AT_WebServer Library](https://github.com/khoih-prog/ESP8266_AT_WebServer) sending `BigData`

<p align="center">
    <img src="https://github.com/khoih-prog/ESP8266_AT_WebServer/raw/master/pics/AdvancedWebServer_BigData.png">
</p>

---
---

### Debug

Debug is enabled by default on Serial. Debug Level from 0 to 4. To disable, change the _WIFI_LOGLEVEL_ and _WIFININA_LOGLEVEL_ to 0

```cpp
// Use this to output debug msgs to Serial
#define DEBUG_ESP8266_AT_WEBSERVER_PORT   Serial

// Debug Level from 0 to 4
#define _ESP_AT_LOGLEVEL_                 1
```

---

## Troubleshooting

If you get compilation errors, more often than not, you may need to install a newer version of the board's core, applying Libraries' Patches, Packages' Patches or this library latest version.


---
---


### Issues

Submit issues to: [ESP8266_AT_WebServer issues](https://github.com/khoih-prog/ESP8266_AT_WebServer/issues)
 
---
---

### TO DO

1. Bug Searching and Killing
2. Add SSL/TLS Client and Server support
3. Support more types of boards using ESP8266 AT-command shields.
4. Add mDNS features.

### DONE

 1. Add support to **Arduino SAMD21 (ZERO, MKR, NANO_33_IOT, etc.)**
 2. Add support to **Adafruit SAMD21 (Itsy-Bitsy M0, Metro M0, Feather M0 Express, etc.)**.
 3. Add support to **Adafruit SAMD51 (Itsy-Bitsy M4, Metro M4, Grand Central M4, Feather M4 Express, etc.)**.
 4. Add support to **Adafruit nRF52 ( Feather nRF52832, nRF52840 Express, BlueFruit Sense, Itsy-Bitsy nRF52840 Express, Metro nRF52840 Express, NINA_B302_ublox, NINA_B112_ublox, etc.)**.
 5. Add support to SAM DUE.
 6. Add support to ESP32-AT
 7. Add support to W600 and WIS600-01S
 8. Add support to all **STM32F/L/H/G/WB/MP1**
 9. Add support to **Seeeduino SAMD21/SAMD51 boards (SEEED_WIO_TERMINAL, SEEED_FEMTO_M0, SEEED_XIAO_M0, Wio_Lite_MG126, WIO_GPS_BOARD, SEEEDUINO_ZERO, SEEEDUINO_LORAWAN, SEEED_GROVE_UI_WIRELESS, etc.)**
10. Add UDP Multicast feature.
11. Add PROGMEM and FlashString related commands such as sendContent_P() and send_P(), and use in examples.
12. Clean-up all compiler warnings possible.
13. Add Version String 
14. Add Table of Contents
15. Add support to SIPEED_MAIX_DUINO
16. Add support to **Arduino Nano RP2040 Connect** using [**Arduino mbed OS for Nano boards**](https://github.com/arduino/ArduinoCore-mbed).
17. Add support to RP2040-based boards, such as **RASPBERRY_PI_PICO, ADAFRUIT_FEATHER_RP2040 and GENERIC_RP2040**, using [**Earle Philhower's arduino-pico** core](https://github.com/earlephilhower/arduino-pico).
18. Add support to RP2040-based boards, such as **RASPBERRY_PI_PICO, ADAFRUIT_FEATHER_RP2040 and GENERIC_RP2040**, using [**Arduino-mbed RP2040** core](https://github.com/arduino/ArduinoCore-mbed)
19. Reduce usage of Arduino String with std::string
20. Optimize library code and examples by using **reference-passing instead of value-passing**.
21. Fix severe limitation to permit sending larger data than 2K buffer of ESP8266/ESP32 AT-command shields
22. Add example [ATWebServer_BigData](https://github.com/khoih-prog/ESP8266_AT_WebServer/tree/master/examples/ATWebServer_BigData) to demo how to send larger data than 2K buffer
23. Modify example [AdvancedWebServer](https://github.com/khoih-prog/ESP8266_AT_WebServer/tree/master/examples/AdvancedWebServer) to demo how to send larger data than 2K buffer


---
---
 
### Contributions and Thanks

1. Based on and modified [Ivan Grokhotkov's ESP8266WebServer](https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WebServer)
2. Based on and modified from [bportaluri's WiFiEsp library](https://github.com/bportaluri/WiFiEsp)
3. Thanks to good work of [Miguel Alexandre Wisintainer](https://github.com/tcpipchip) for initiating, inspriring, working with, developing, debugging and testing. Without that, support to nRF52, especially **U-Blox B302 running as nRF52840 and U-Blox B112 running as nRF52832**, has never been started and finished. Also see [U-BLOX NINA B302/ENC28J60 wth Alexa/SinricPro](https://u-blox-ethernet-ninaw.blogspot.com/).
4. Thanks to [reltkaine](https://github.com/reltkaine) to report the compile error issue with Sipeed Maixduino core, leading to v1.4.1 to introduce `Packages_Patches` for Sipeed Maixduino core


<table>
  <tr>
    <td align="center"><a href="https://github.com/igrr"><img src="https://github.com/igrr.png" width="100px;" alt="igrr"/><br /><sub><b>⭐️⭐️ Ivan Grokhotkov</b></sub></a><br /></td>
    <td align="center"><a href="https://github.com/bportaluri"><img src="https://github.com/bportaluri.png" width="100px;" alt="bportaluri"/><br /><sub><b>⭐️ Bruno</b></sub></a><br /></td>
    <td align="center"><a href="https://github.com/tcpipchip"><img src="https://github.com/tcpipchip.png" width="100px;" alt="tcpipchip"/><br /><sub><b>⭐️ Miguel Wisintainer</b></sub></a><br /></td>
    <td align="center"><a href="https://github.com/reltkaine"><img src="https://github.com/reltkaine.png" width="100px;" alt="reltkaine"/><br /><sub><b>reltkaine</b></sub></a><br /></td>
  </tr> 
</table>

---

### Contributing

If you want to contribute to this project:
- Report bugs and errors
- Ask for enhancements
- Create issues and pull requests
- Tell other people about this library

---

### License

- The library is licensed under [MIT](https://github.com/khoih-prog/ESP8266_AT_WebServer/blob/master/LICENSE)

---

### Copyright

Copyright (C) 2020- Khoi Hoang


