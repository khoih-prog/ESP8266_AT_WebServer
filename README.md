## ESP8266_AT_WebServer

[![arduino-library-badge](https://www.ardu-badge.com/badge/ESP8266_AT_WebServer.svg?)](https://www.ardu-badge.com/ESP8266_AT_WebServer)
[![GitHub release](https://img.shields.io/github/release/khoih-prog/ESP8266_AT_WebServer.svg)](#releases)
[![GitHub](https://img.shields.io/github/license/mashape/apistatus.svg)](https://github.com/khoih-prog/ESP8266_AT_WebServer/blob/master/LICENSE)
[![contributions welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat)](#Contributing)
[![GitHub issues](https://img.shields.io/github/issues/khoih-prog/ESP8266_AT_WebServer.svg)](http://github.com/khoih-prog/ESP8266_AT_WebServer/issues)

---

### New Version v1.0.9

1. Fix bug. 
2. Add functions (ESP8266_AT_Drv::wifiDriverReInit and ESP8266_AT_Class::reInit). 
3. Restructure codes. Increase RingBuffer Size.
4. Add support to WIS600-01S and W600 WiFi shields

### New Version v1.0.8

1. Fix bug. 
2. Add features to ESP32-AT. 

### New Version v1.0.7

1. Add support to ***ESP32-AT-command shield***. 
2. Update deprecated ESP8266-AT commands. 
3. Restructure examples to separate defines header files.

### New Version v1.0.6

1. Add support to ***nRF52*** boards, such as ***AdaFruit Feather nRF52832, nRF52840 Express, BlueFruit Sense, Itsy-Bitsy nRF52840 Express, Metro nRF52840 Express, NINA_B30_ublox, NINA_B112_ublox, etc.*** 


### New Version v1.0.5

1. Add support to ***SAM51 (Itsy-Bitsy M4, Metro M4, Grand Central M4, Feather M4 Express, etc.) and SAM DUE***.

### New Version v1.0.4

1. Sync with ESP8266WebServer library of ESP8266 core v2.6.3
2. Fix bug.

### New Version v1.0.3

1. Add support to STM32 (STM32F0, F1, F2, F3, F4, F7, etc) boards

### New Version v1.0.2

1. Add support to SAMD (ZERO, MKR, ***NANO_33_IOT***, M0, M0 Pro, AdaFruit CIRCUITPLAYGROUND_EXPRESS, etc.) boards

---

This is simple yet complete WebServer library for `AVR, Teensy, SAM DUE, SAMD, STM32, etc.` boards running `ESP8266 AT-command` shields. The functions are similar and compatible to ESP8266/ESP32 WebServer libraries to make life much easier to port sketches from ESP8266/ESP32.

The library supports 
1. WiFi Client, STA and AP mode
2. TCP Server and Client
3. UDP Server and Client
4. HTTP Server and Client
5. HTTP GET and POST requests, provides argument parsing, handles one client at a time.

Library is based on and modified from:
1. [Ivan Grokhotkov's ESP8266WebServer](https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WebServer)
2. [bportaluri's WiFiEsp library](https://github.com/bportaluri/WiFiEsp)

The ESP8266_AT_Web_Server class found in `ESP8266_AT_Web_Server.h` header, is a simple web server that knows how to handle HTTP requests such as GET and POST and can only support one simultaneous client.

---

## Prerequisite
 1. [`Arduino IDE 1.8.12 or later` for Arduino](https://www.arduino.cc/en/Main/Software)
 2. [`Arduino Core for STM32 v1.8.0 or later`](https://github.com/khoih-prog/Arduino_Core_STM32) for STM32 boards
 3. [`Teensy core 1.51 or later`](https://www.pjrc.com/teensy/td_download.html) for Teensy (4.0, 3.6, 3.5, 3,2, 3.1, 3.0, LC) boards
 4. [`Arduino SAM DUE core 1.6.12 or later`](https://www.arduino.cc/en/Guide/ArduinoDue) for SAM DUE ARM Cortex-M3 boards
 5. [`Arduino SAMD core 1.8.5 or later`](https://www.arduino.cc/en/Guide/ArduinoM0) for SAMD ARM Cortex-M0+ boards
 6. [`Adafruit SAMD core 1.5.11 or later`](https://www.adafruit.com/) for SAMD ARM Cortex-M0+ and M4 boards (Nano 33 IoT, etc.)
 7. [`Adafruit nRF52 v0.20.1 or later`](www.adafruit.com) for nRF52 boards such as AdaFruit Feather nRF52840 Express, NINA_B302_ublox, etc.
 8. [`Functional-VLPP library`](https://github.com/khoih-prog/functional-vlpp) to use server's lambda function. To install. check [![arduino-library-badge](https://www.ardu-badge.com/badge/Functional-Vlpp.svg?)](https://www.ardu-badge.com/Functional-Vlpp)
10. [`Ai-Thinker AT Firmware v1.5.4`](AT_Firmwares/At_firmware_bin1.54.zip) or [`AT Firmware v1.7.4.0`](AT_Firmwares/AT_Firmware_bin_1.7.4.0.zip) for ESP8266-AT shields.
11. [`AT version_2.1.0.0_dev`](AT_Firmwares/AT_version_2.1.0.0_dev.zip) for ESP32-AT shields.

### Important Notes

1. Tested OK with for ESP8266-AT shields:
  - [`Ai-Thinker AT Firmware v1.5.4`](AT_Firmwares/At_firmware_bin1.54.zip)
  
    ```
    AT version:1.1.0.0(May 11 2016 18:09:56)
    SDK version:1.5.4(baaeaebb)
    Ai-Thinker Technology Co. Ltd.
    Jun 13 2016 11:29:20
    ```
  - [`AT Firmware v1.7.4.0`](AT_Firmwares/AT_Firmware_bin_1.7.4.0.zip)
  
    ```
    AT version:1.7.4.0(May 11 2020 19:13:04)
    SDK version:3.0.4(9532ceb)
    compile time:May 27 2020 10:12:17
    Bin version(Wroom 02):1.7.4
    ```
    
  - [`WIS600-01S`](https://www.aliexpress.com/item/32956707239.html) and [`W600`](https://www.seeedstudio.com/w600.html) using either ESP8266 or ESP32-AT commands and stock firmware
  
    ```
    AT version:1.1.4(Dec 05 2018 11:06:45)
    SDK version:3.0.0
    Dec 05 2018 11:06:45
    ```
  
  
2. Tested OK with for ESP32-AT shields:
  - [`AT version_2.1.0.0_dev`](AT_Firmwares/AT_version_2.1.0.0_dev.zip)
    
    ```
    AT version:2.1.0.0-dev(4f6b92c - Jun 10 2020 10:36:54)
    SDK version:v4.0.1-193-ge7ac221b4
    compile time(b85a8df):Jun 18 2020 14:00:59
    Bin version:2.0.0(WROOM-32)
    ```
    
    See instructions at [AT Command Core](https://github.com/espressif/esp-at) and [ESP_AT_Get_Started](https://github.com/espressif/esp-at/blob/master/docs/en/get-started/ESP_AT_Get_Started.md)
  
3. Upload [`AT Firmware v1.7.4.0`](AT_Firmwares/AT_Firmware_bin_1.7.4.0.zip) bin files to correct locations as follows:

```
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

3. Test before using different AT-Firmware Version at your own risks. Just use any simple Blynk sketch to verify if the AT-firmware is OK.
4. Compatible AT-Firmare version will be updated. Check for and download Firmwares from [AT_Firmwares](AT_Firmwares) all supported AT Firmwares.
5. Support to ESP32-AT-command shields will be added by using new library [ESP_AT_Lib](https://github.com/khoih-prog/ESP_AT_Lib) to replace [Blynk's BlynkESP8266_Lib](https://github.com/blynkkk/blynk-library/releases/Blynk_Release_v0.6.1.zip).

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

---

#### Usage

#### Class Constructor

```cpp
  ESP8266_AT_WebServer server(80);
```

Creates the ESP8266_AT_WebServer class object.

*Parameters:* 
 
host port number: ``int port`` (default is the standard HTTP port 80)

#### Basic Operations

***Starting the server***

```cpp
  void begin();
```

***Handling incoming client requests***

```cpp
  void handleClient();
```

***Disabling the server***

```cpp
  void close();
  void stop();
```

Both methods function the same

***Client request handlers***

```cpp
  void on();
  void addHandler();
  void onNotFound();
  void onFileUpload();	
```

Example:*

```cpp
  server.on("/", handlerFunction);
  server.onNotFound(handlerFunction);   // called when handler is not assigned
  server.onFileUpload(handlerFunction); // handle file uploads
```

***Sending responses to the client***

```cpp
  void send();
  void send_P();
```

`Parameters:`

`code` - HTTP response code, can be `200` or `404`, etc.

`content_type` - HTTP content type, like `"text/plain"` or `"image/png"`, etc.

`content` - actual content body

#### Advanced Options

***Getting information about request arguments***

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

***Getting information about request headers***

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

***Authentication***

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

Also see examples: 
 1. [HelloServer](examples/HelloServer)
 2. [HelloServer2](examples/HelloServer2)
 3. [AdvancedWebServer](examples/AdvancedWebServer)
 4. [HttpBasicAuth](examples/HttpBasicAuth)
 5. [PostServer](examples/PostServer)
 6. [SimpleAuthentication](examples/SimpleAuthentication)
 7. [ConnectWPA](examples/ConnectWPA)
 8. [ScanNetworks](examples/ScanNetworks)
 9. [UdpNTPClient](examples/UdpNTPClient)
10. [UdpSendReceive](examples/UdpSendReceive)
11. [WebClient](examples/WebClient)
12. [WebClientRepeating](examples/WebClientRepeating)
13. [WebServer](examples/WebServer)
14. [WebServerAP](examples/WebServerAP)


## Example [HelloServer](examples/HelloServer)
Please take a look at other examples, as well.

### File [HelloServer](examples/HelloServer/HelloServer.ino)

```cpp
#include "defines.h"

int status = WL_IDLE_STATUS;     // the Wifi radio's status
int reqCount = 0;                // number of requests received

ESP8266_AT_WebServer server(80);

const int led = 13;

void handleRoot()
{
  server.send(200, "text/plain", "Hello from ESP8266_AT_WebServer!");
}

void handleNotFound()
{
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++)
  {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}

void setup(void)
{
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  while (!Serial);

  Serial.println("\nStarting HelloServer on " + String(BOARD_TYPE));

  // initialize serial for ESP module
  EspSerial.begin(115200);
  // initialize ESP module
  WiFi.init(&EspSerial);

  Serial.println(F("WiFi shield init done"));

  // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD)
  {
    Serial.println(F("WiFi shield not present"));
    // don't continue
    while (true);
  }

  // attempt to connect to WiFi network
  while ( status != WL_CONNECTED)
  {
    Serial.print(F("Connecting to WPA SSID: "));
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
  }

  server.begin();

  Serial.print("WebServer is @ ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);

  server.on("/inline", []()
  {
    server.send(200, "text/plain", "This works as well");
  });

  server.onNotFound(handleNotFound);

  server.begin();

  Serial.print(F("HTTP server started @ "));
  Serial.println(WiFi.localIP());
}

void loop(void)
{
  server.handleClient();
}
```

### File [defines.h](examples/HelloServer/defines.h)

```cpp
#ifndef defines_h
#define defines_h

#define DEBUG_ESP8266_AT_WEBSERVER_PORT Serial

// Uncomment to use ESP32-AT commands
//#define USE_ESP32_AT      true

#if    ( defined(ARDUINO_SAMD_ZERO) || defined(ARDUINO_SAMD_MKR1000) || defined(ARDUINO_SAMD_MKRWIFI1010) \
      || defined(ARDUINO_SAMD_NANO_33_IOT) || defined(ARDUINO_SAMD_MKRFox1200) || defined(ARDUINO_SAMD_MKRWAN1300) || defined(ARDUINO_SAMD_MKRWAN1310) \
      || defined(ARDUINO_SAMD_MKRGSM1400) || defined(ARDUINO_SAMD_MKRNB1500) || defined(ARDUINO_SAMD_MKRVIDOR4000) || defined(__SAMD21G18A__) \
      || defined(ARDUINO_SAMD_CIRCUITPLAYGROUND_EXPRESS) || defined(__SAMD21E18A__) || defined(__SAMD51__) || defined(__SAMD51J20A__) || defined(__SAMD51J19A__) \
      || defined(__SAMD51G19A__) || defined(__SAMD21G18A__) )
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

#if ( defined(STM32F0) || defined(STM32F1) || defined(STM32F2) || defined(STM32F3)  ||defined(STM32F4) || defined(STM32F7) )
#if defined(ESP8266_AT_USE_STM32)
#undef ESP8266_AT_USE_STM32
#endif
#define ESP8266_AT_USE_STM32      true
#endif

#ifdef CORE_TEENSY
// For Teensy 4.0
#define EspSerial Serial2   //Serial2, Pin RX2 : 7, TX2 : 8
#if defined(__IMXRT1062__)
#define BOARD_TYPE      "TEENSY 4.0"
#elif ( defined(__MKL26Z64__) || defined(ARDUINO_ARCH_AVR) )
#define BOARD_TYPE      "TEENSY LC or 2.0"
#else
#define BOARD_TYPE      "TEENSY 3.X"
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
#define EspSerial Serial1

#if defined(STM32F0)
#define BOARD_TYPE  "STM32F0"
#elif defined(STM32F1)
#define BOARD_TYPE  "STM32F1"
#elif defined(STM32F2)
#define BOARD_TYPE  "STM32F2"
#elif defined(STM32F3)
#define BOARD_TYPE  "STM32F3"
#elif defined(STM32F4)
#define BOARD_TYPE  "STM32F4"
#elif defined(STM32F7)
#define BOARD_TYPE  "STM32F7"
#else
#warning STM32 unknown board selected
#define BOARD_TYPE  "STM32 Unknown"
#endif

#else
// For Mega
#define EspSerial Serial3
#define BOARD_TYPE      "AVR Mega"
#endif

#include <ESP8266_AT_WebServer.h>

char ssid[] = "****";        // your network SSID (name)
char pass[] = "****";        // your network password

#endif    //defines_h
```

---

The following are debug terminal output and screen shot when running example [AdvancedWebServer](examples/AdvancedWebServer) on NRF52840_ITSYBITSY_EXPRESS and ESP8266-AT shield.

<p align="center">
    <img src="https://github.com/khoih-prog/ESP8266_AT_WebServer/blob/master/pics/AdvancedWebServer.png">
</p>

```
Starting AdvancedServer on NRF52840_ITSYBITSY_EXPRESS
[ESP_AT] Use ES8266-AT Command
WiFi shield init done
Connecting to SSID: HueNet1
HTTP server started @ 192.168.2.107
[ESP_AT] send1: len =  289
[ESP_AT] content =  <html><head><meta http-equiv='refresh' content='5'/><title>ESP8266 Demo</title><style>body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }</style></head><body><h1>Hello from ESP8266!</h1><p>Uptime: 00:00:27</p><img src="/test.svg" /></body></html>
[ESP_AT] send1: len =  1946
[ESP_AT] content =  <svg xmlns="http://www.w3.org/2000/svg" version="1.1" width="310" height="150">
<rect width="310" height="150" fill="rgb(250, 230, 210)" stroke-width="1" stroke="rgb(0, 0, 0)" />
<g stroke="black">
<line x1="10" y1="77" x2="20" y2="67" stroke-width="1" />
<line x1="20" y1="67" x2="30" y2="98" stroke-width="1" />
<line x1="30" y1="98" x2="40" y2="111" stroke-width="1" />
<line x1="40" y1="111" x2="50" y2="90" stroke-width="1" />
<line x1="50" y1="90" x2="60" y2="22" stroke-width="1" />
<line x1="60" y1="22" x2="70" y2="98" stroke-width="1" />
<line x1="70" y1="98" x2="80" y2="64" stroke-width="1" />
<line x1="80" y1="64" x2="90" y2="104" stroke-width="1" />
<line x1="90" y1="104" x2="100" y2="31" stroke-width="1" />
<line x1="100" y1="31" x2="110" y2="59" stroke-width="1" />
<line x1="110" y1="59" x2="120" y2="139" stroke-width="1" />
<line x1="120" y1="139" x2="130" y2="117" stroke-width="1" />
<line x1="130" y1="117" x2="140" y2="75" stroke-width="1" />
<line x1="140" y1="75" x2="150" y2="72" stroke-width="1" />
<line x1="150" y1="72" x2="160" y2="137" stroke-width="1" />
<line x1="160" y1="137" x2="170" y2="20" stroke-width="1" />
<line x1="170" y1="20" x2="180" y2="94" stroke-width="1" />
<line x1="180" y1="94" x2="190" y2="81" stroke-width="1" />
<line x1="190" y1="81" x2="200" y2="38" stroke-width="1" />
<line x1="200" y1="38" x2="210" y2="33" stroke-width="1" />
<line x1="210" y1="33" x2="220" y2="53" stroke-width="1" />
<line x1="220" y1="53" x2="230" y2="88" stroke-width="1" />
<line x1="230" y1="88" x2="240" y2="32" stroke-width="1" />
<line x1="240" y1="32" x2="250" y2="110" stroke-width="1" />
<line x1="250" y1="110" x2="260" y2="87" stroke-width="1" />
<line x1="260" y1="87" x2="270" y2="11" stroke-width="1" />
<line x1="270" y1="11" x2="280" y2="98" stroke-width="1" />
<line x1="280" y1="98" x2="290" y2="76" stroke-width="1" />
<line x1="290" y1="76" x2="300" y2="121" stroke-width="1" />
</g>
</svg>

```

---

### New Version v1.0.9

1. Fix bug. 
2. Add functions (ESP8266_AT_Drv::wifiDriverReInit and ESP8266_AT_Class::reInit). 
3. Restructure codes. Increase RingBuffer Size.
4. Add support to WIS600-01S and W600 WiFi shields

### New Version v1.0.8

1. Fix bug. 
2. Add features to ESP32-AT. 

### New Version v1.0.7

1. Add support to ESP32-AT-command shield. 
2. Update deprecated ESP8266-AT commands. 
3. Restructure examples to separate defines header files.

### New Version v1.0.6

1. Add support to ***nRF52*** boards, such as ***AdaFruit Feather nRF52832, nRF52840 Express, BlueFruit Sense, Itsy-Bitsy nRF52840 Express, Metro nRF52840 Express, NINA_B30_ublox, NINA_B112_ublox, etc.*** 

### New Version v1.0.5

1. Add support to ***SAM51 (Itsy-Bitsy M4, Metro M4, Grand Central M4, Feather M4 Express, etc.)and SAM DUE***.

### New Version v1.0.4

1. Sync with ESP8266WebServer library of ESP8266 core v2.6.3
2. Fix bug.

### New Version v1.0.3

1. Add support to STM32 (STM32F0, F1, F2, F3, F4, F7, etc) boards

### Version v1.0.2

1. Add support to SAMD (ZERO, MKR, ***NANO_33_IOT***, M0, Mo Pro, AdaFruit CIRCUITPLAYGROUND_EXPRESS, etc.) boards

### Version v1.0.1

1. Add support to server's lambda function calls with dependency on [`Functional-VLPP library`](https://github.com/khoih-prog/functional-vlpp)

### Initial Version v1.0.0

This is simple yet complete WebServer library for `AVR, Teensy, etc.` boards running `ESP8266 AT-command` shields. ***The functions are similar and compatible to ESP8266/ESP32 WebServer libraries*** to make life much easier to port sketches from ESP8266/ESP32.

The library supports 
1. WiFi Client, STA and AP mode
2. TCP Server and Client
3. UDP Server and Client
4. HTTP Server and Client
5. HTTP GET and POST requests, provides argument parsing, handles one client at a time.

---

## TO DO
1. Bug Searching and Killing
2. Add SSL/TLS Client and Server support
3. Support more types of boards using ESP8266 AT-command shields.


### Contributions and thanks
1. Based on and modified [Ivan Grokhotkov's ESP8266WebServer](https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WebServer)
2. Based on and modified from [bportaluri's WiFiEsp library](https://github.com/bportaluri/WiFiEsp)
3. Thanks to good work of [Miguel Alexandre Wisintainer](https://github.com/tcpipchip) for initiating, inspriring, working with, developing, debugging and testing. Without that, support to nRF52, especially ***U-Box B302, B112 running as nRF52840 / nRF52832***, has never been started and finished.

## Contributing

If you want to contribute to this project:
- Report bugs and errors
- Ask for enhancements
- Create issues and pull requests
- Tell other people about this library

## Copyright

Copyright 2020- Khoi Hoang


