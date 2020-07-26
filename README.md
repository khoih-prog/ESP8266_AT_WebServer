## ESP8266_AT_WebServer

[![arduino-library-badge](https://www.ardu-badge.com/badge/ESP8266_AT_WebServer.svg?)](https://www.ardu-badge.com/ESP8266_AT_WebServer)
[![GitHub release](https://img.shields.io/github/release/khoih-prog/ESP8266_AT_WebServer.svg)](https://github.com/khoih-prog/ESP8266_AT_WebServer/releases)
[![GitHub](https://img.shields.io/github/license/mashape/apistatus.svg)](https://github.com/khoih-prog/ESP8266_AT_WebServer/blob/master/LICENSE)
[![contributions welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat)](#Contributing)
[![GitHub issues](https://img.shields.io/github/issues/khoih-prog/ESP8266_AT_WebServer.svg)](http://github.com/khoih-prog/ESP8266_AT_WebServer/issues)

---

### New in v1.0.12

1. Add sample Packages_Patches for STM32 (Nucleo-144 NUCLEO_F767ZI, Nucleo-64 NUCLEO_L053R8)
2. Add example for STM32 boards.
3. Add instructions to use EspSerial/Serial1 on some STM32 boards without Serial1 definition (Nucleo-144 NUCLEO_F767ZI, Nucleo-64 NUCLEO_L053R8, etc.)

### New in v1.0.11

1. Add support to all STM32F/L/H/G/WB/MP1 (Nucleo-144 NUCLEO_F767ZI, Nucleo-64 NUCLEO_L053R8, etc.)
2. Add support to Seeeduino SAMD21/SAMD51 boards (SEEED_WIO_TERMINAL, SEEED_FEMTO_M0, SEEED_XIAO_M0, Wio_Lite_MG126, WIO_GPS_BOARD, SEEEDUINO_ZERO, SEEEDUINO_LORAWAN, SEEED_GROVE_UI_WIRELESS, etc.)

### New in v1.0.10

1. Fix bug not closing client and releasing socket.
2. Enhance examples.
3. Add function to be enable to work with [WebSockets2_Generic Library](https://github.com/khoih-prog/WebSockets2_Generic)

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

1. Add support to ***nRF52*** boards, such as ***AdaFruit Feather nRF52832, nRF52840 Express, BlueFruit Sense, Itsy-Bitsy nRF52840 Express, Metro nRF52840 Express, NINA_B302_ublox, NINA_B112_ublox, etc.*** 

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
 1. [`Arduino IDE 1.8.12+` for Arduino](https://www.arduino.cc/en/Main/Software)
 2. `Arduino AVR core 1.8.2+` for Arduino (Use Arduino Board Manager) for AVR boards
 3. [`Arduino Core for STM32 v1.9.0+`](https://github.com/khoih-prog/Arduino_Core_STM32) for STM32 boards (Nucleo-144 NUCLEO_F767ZI, Nucleo-64 NUCLEO_L053R8, etc.)
 4. [`Teensy core 1.53+`](https://www.pjrc.com/teensy/td_download.html) for Teensy (4.1, 4.0, 3.6, 3.5, 3,2, 3.1, 3.0, LC) boards
 5. [`Arduino SAM DUE core 1.6.12+`](https://www.arduino.cc/en/Guide/ArduinoDue) for SAM DUE ARM Cortex-M3 boards
 6. [`Arduino SAMD core 1.8.6+`](https://www.arduino.cc/en/Guide/ArduinoM0) for SAMD ARM Cortex-M0+ boards  (Nano 33 IoT, etc.).
 7. [`Adafruit SAMD core 1.6.0+`](https://www.adafruit.com/)  for SAMD ARM Cortex-M0+ and M4 boards (Itsy-Bitsy M4, etc.)
 8. [`Adafruit nRF52 v0.20.5+`](www.adafruit.com) for nRF52 boards such as AdaFruit Feather nRF52840 Express, NINA_B302_ublox, NINA_B112_ublox, etc.
 9. [`Seeeduino SAMD core 1.7.7+`](https://www.seeedstudio.com/) for SAMD21/SAMD51 boards (XIAO M0, Wio Terminal, etc.) 
10. [`Functional-VLPP library v1.0.1+`](https://github.com/khoih-prog/functional-vlpp) to use server's lambda function. To install. check [![arduino-library-badge](https://www.ardu-badge.com/badge/Functional-Vlpp.svg?)](https://www.ardu-badge.com/Functional-Vlpp)
11. [`Ai-Thinker AT Firmware v1.5.4`](AT_Firmwares/At_firmware_bin1.54.zip) or [`AT Firmware v1.7.4.0`](AT_Firmwares/AT_Firmware_bin_1.7.4.0.zip) for ESP8266-AT shields.
12. [`AT version_2.1.0.0_dev`](AT_Firmwares/AT_version_2.1.0.0_dev.zip) for ESP32-AT shields.

---

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

### VS Code & PlatformIO:
1. Install [VS Code](https://code.visualstudio.com/)
2. Install [PlatformIO](https://platformio.org/platformio-ide)
3. Install **ESP8266_AT_WebServer** library by using [Library Manager](https://docs.platformio.org/en/latest/librarymanager/). Search for ***ESP8266_AT_WebServer*** in [Platform.io Author's Libraries](https://platformio.org/lib/search?query=author:%22Khoi%20Hoang%22)
4. Use included [platformio.ini](examples/platformio/platformio.ini) file from examples to ensure that all dependent libraries will installed automatically.

---

### Packages' Patches

 1. ***To be able to compile, run and automatically detect and display BOARD_NAME on nRF52840/nRF52832 boards***, you have to copy the whole [nRF52 0.20.5](https://github.com/khoih-prog/ESP8266_AT_WebServer/tree/master/Packages_Patches/adafruit/hardware/nrf52/0.20.5) directory into Adafruit nRF52 directory (~/.arduino15/packages/adafruit/hardware/nrf52/0.20.5). 

Supposing the Adafruit nRF52 version is 0.20.5. These files must be copied into the directory:
- `~/.arduino15/packages/adafruit/hardware/nrf52/0.20.5/platform.txt`
- `~/.arduino15/packages/adafruit/hardware/nrf52/0.20.5/boards.txt`
- `~/.arduino15/packages/adafruit/hardware/nrf52/0.20.5/variants/NINA_B302_ublox/variant.h`
- `~/.arduino15/packages/adafruit/hardware/nrf52/0.20.5/variants/NINA_B302_ublox/variant.cpp`
- `~/.arduino15/packages/adafruit/hardware/nrf52/0.20.5/variants/NINA_B112_ublox/variant.h`
- `~/.arduino15/packages/adafruit/hardware/nrf52/0.20.5/variants/NINA_B112_ublox/variant.cpp`

Whenever a new version is installed, remember to copy these files into the new version directory. For example, new version is x.yy.z
These files must be copied into the directory:

- `~/.arduino15/packages/adafruit/hardware/nrf52/x.yy.z/platform.txt`
- `~/.arduino15/packages/adafruit/hardware/nrf52/x.yy.z/boards.txt`
- `~/.arduino15/packages/adafruit/hardware/nrf52/x.yy.z/variants/NINA_B302_ublox/variant.h`
- `~/.arduino15/packages/adafruit/hardware/nrf52/x.yy.z/variants/NINA_B302_ublox/variant.cpp`
- `~/.arduino15/packages/adafruit/hardware/nrf52/x.yy.z/variants/NINA_B112_ublox/variant.h`
- `~/.arduino15/packages/adafruit/hardware/nrf52/x.yy.z/variants/NINA_B112_ublox/variant.cpp`

 2. ***To be able to compile and run on Teensy boards***, you have to copy the file [Teensy boards.txt](https://github.com/khoih-prog/ESP8266_AT_WebServer/blob/master/Packages_Patches/hardware/teensy/avr/boards.txt) into Teensy hardware directory (./arduino-1.8.12/hardware/teensy/avr/boards.txt). 

Supposing the Arduino version is 1.8.12. This file must be copied into the directory:

- `./arduino-1.8.12/hardware/teensy/avr/boards.txt`

Whenever a new version is installed, remember to copy this file into the new version directory. For example, new version is x.yy.zz
This file must be copied into the directory:

- `./arduino-x.yy.zz/hardware/teensy/avr/boards.txt`

 3. ***To be able to compile and run on SAM DUE boards***, you have to copy the whole [SAM DUE](https://github.com/khoih-prog/ESP8266_AT_WebServer/tree/master/Packages_Patches/arduino/hardware/sam/1.6.12) directory into Arduino sam directory (~/.arduino15/packages/arduino/hardware/sam/1.6.12). 

Supposing the Arduino SAM core version is 1.6.12. This file must be copied into the directory:

- `~/.arduino15/packages/arduino/hardware/sam/1.6.12/platform.txt`

Whenever a new version is installed, remember to copy this file into the new version directory. For example, new version is x.yy.zz
This file must be copied into the directory:

- `~/.arduino15/packages/arduino/hardware/sam/x.yy.zz/platform.txt`

 4. ***To be able to automatically detect and display BOARD_NAME on Arduino SAMD (Nano-33-IoT, etc) boards***, you have to copy the file [Arduino SAMD platform.txt](https://github.com/khoih-prog/ESP8266_AT_WebServer/tree/master/Packages_Patches/arduino/hardware/samd/1.8.6) into Arduino samd directory (~/.arduino15/packages/arduino/hardware/samd/1.8.6). 

Supposing the Arduino SAMD core version is 1.8.6. This file must be copied into the directory:

- `~/.arduino15/packages/arduino/hardware/samd/1.8.6/platform.txt`

Whenever a new version is installed, remember to copy this file into the new version directory. For example, new version is x.yy.zz
This file must be copied into the directory:

- `~/.arduino15/packages/arduino/hardware/samd/x.yy.zz/platform.txt`

 5. ***To be able to automatically detect and display BOARD_NAME on Adafruit SAMD (Itsy-Bitsy M4, etc) boards***, you have to copy the file [Adafruit SAMD platform.txt](https://github.com/khoih-prog/ESP8266_AT_WebServer/tree/master/Packages_Patches/adafruit/hardware/samd/1.6.0) into Adafruit samd directory (~/.arduino15/packages/adafruit/hardware/samd/1.6.0). 

Supposing the Adafruit SAMD core version is 1.6.0. This file must be copied into the directory:

- `~/.arduino15/packages/adafruit/hardware/samd/1.6.0/platform.txt`

Whenever a new version is installed, remember to copy this file into the new version directory. For example, new version is x.yy.zz
This file must be copied into the directory:

- `~/.arduino15/packages/adafruit/hardware/samd/x.yy.zz/platform.txt`

 6. ***To be able to automatically detect and display BOARD_NAME on Seeeduino SAMD (XIAO M0, Wio Terminal, etc) boards***, you have to copy the file [Seeeduino SAMD platform.txt](https://github.com/khoih-prog/ESP8266_AT_WebServer/tree/master/Packages_Patches/Seeeduino/hardware/samd/1.7.7) into Adafruit samd directory (~/.arduino15/packages/Seeeduino/hardware/samd/1.7.7). 

Supposing the Seeeduino SAMD core version is 1.7.7. This file must be copied into the directory:

- `~/.arduino15/packages/Seeeduino/hardware/samd/1.7.7/platform.txt`

Whenever a new version is installed, remember to copy this file into the new version directory. For example, new version is x.yy.zz
This file must be copied into the directory:

- `~/.arduino15/packages/Seeeduino/hardware/samd/x.yy.zz/platform.txt`

7. ***To use Serial1 on some STM32 boards without Serial1 definition (Nucleo-144 NUCLEO_F767ZI, Nucleo-64 NUCLEO_L053R8, etc.) boards***, you have to copy the files [STM32 variant.h](https://github.com/khoih-prog/ESP8266_AT_WebServer/tree/master/Packages_Patches/STM32/hardware/stm32/1.9.0) into STM32 stm32 directory (~/.arduino15/packages/STM32/hardware/stm32/1.9.0). You have to modify the files corresponding to your boards, this is just an illustration how to do.

Supposing the STM32 stm32 core version is 1.9.0. These files must be copied into the directory:

- `~/.arduino15/packages/STM32/hardware/stm32/1.9.0/variants/NUCLEO_F767ZI/variant.h` for Nucleo-144 NUCLEO_F767ZI.
- `~/.arduino15/packages/STM32/hardware/stm32/1.9.0/variants/NUCLEO_L053R8/variant.h` for Nucleo-64 NUCLEO_L053R8.

Whenever a new version is installed, remember to copy this file into the new version directory. For example, new version is x.yy.zz,
theses files must be copied into the corresponding directory:

- `~/.arduino15/packages/STM32/hardware/stm32/x.yy.zz/variants/NUCLEO_F767ZI/variant.h`
- `~/.arduino15/packages/STM32/hardware/stm32/x.yy.zz/variants/NUCLEO_L053R8/variant.h`

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

## Examples

Also see examples:
 1. [AdvancedWebServer](examples/AdvancedWebServer)
 2. [AdvancedWebServer_STM32](examples/AdvancedWebServer_STM32)
 3. [HelloServer](examples/HelloServer)
 4. [HelloServer2](examples/HelloServer2)
 5. [HttpBasicAuth](examples/HttpBasicAuth)
 6. [PostServer](examples/PostServer)
 7. [SimpleAuthentication](examples/SimpleAuthentication)
 8. [ConnectWPA](examples/ConnectWPA)
 9. [ScanNetworks](examples/ScanNetworks)
10. [UdpNTPClient](examples/UdpNTPClient)
11. [UdpSendReceive](examples/UdpSendReceive)
12. [WebClient](examples/WebClient)
13. [WebClientRepeating](examples/WebClientRepeating)
14. [WebServer](examples/WebServer)
15. [WebServerAP](examples/WebServerAP)


### Example [AdvancedWebServer](examples/AdvancedWebServer)
Please take a look at other examples, as well.

### File [AdvancedWebServer](examples/AdvancedWebServer/AdvancedWebServer.ino)

```cpp
// Credits of [Miguel Alexandre Wisintainer](https://github.com/tcpipchip) for this simple yet effective method
// For some STM32, there is only definition of Serial in variant.h, and is used for Serial/USB Debugging
// For example, in Nucleo-144 F767ZI original variant.h
//
// #define SERIAL_PORT_MONITOR     Serial
// #define SERIAL_PORT_HARDWARE    Serial
//
// To use ESP8266/ESP32-AT, we need another Serial, such as Serial1
// To do this, first, in corresponding variant.h, modify as follows:

// #define SERIAL_PORT_HARDWARE    Serial1
//
// then assign pins D0 = RX/D1 = TX to be Hardware Serial1 by putting in sketch as follows:
//
// #define EspSerial      SERIAL_PORT_HARDWARE    //Serial1
// HardwareSerial         Serial1(D0, D1);
//
// This must be included in defines.h for each board you'd like to use ESPSerial as Serial1
//
// The pin usage must be modified according to your boards.

#include "defines.h"

int status = WL_IDLE_STATUS;     // the Wifi radio's status
int reqCount = 0;                // number of requests received

ESP8266_AT_WebServer server(80);

const int led = 13;

void handleRoot()
{
#define BUFFER_SIZE     400
  
  digitalWrite(led, 1);
  char temp[BUFFER_SIZE];
  int sec = millis() / 1000;
  int min = sec / 60;
  int hr = min / 60;
  int day = hr / 24;

  snprintf(temp, BUFFER_SIZE - 1,
           "<html>\
<head>\
<meta http-equiv='refresh' content='5'/>\
<title>ESP-AT %s</title>\
<style>\
body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
</style>\
</head>\
<body>\
<h1>Hello from ESP-AT</h1>\
<h3>on %s</h3>\
<p>Uptime: %d d %02d:%02d:%02d</p>\
<img src=\"/test.svg\" />\
</body>\
</html>", BOARD_NAME, BOARD_NAME, day, hr, min % 60, sec % 60);

  server.send(200, "text/html", temp);
  digitalWrite(led, 0);
}

void handleNotFound()
{
  digitalWrite(led, 1);
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

void drawGraph()
{
  String out;
  out.reserve(3000);
  char temp[70];
  out += "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" width=\"310\" height=\"150\">\n";
  out += "<rect width=\"310\" height=\"150\" fill=\"rgb(250, 230, 210)\" stroke-width=\"1\" stroke=\"rgb(0, 0, 0)\" />\n";
  out += "<g stroke=\"black\">\n";
  int y = rand() % 130;

  for (int x = 10; x < 300; x += 10)
  {
    int y2 = rand() % 130;
    sprintf(temp, "<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" stroke-width=\"1\" />\n", x, 140 - y, x + 10, 140 - y2);
    out += temp;
    y = y2;
  }
  out += "</g>\n</svg>\n";

  server.send(200, "image/svg+xml", out);
}

void setup(void)
{
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);

  Serial.begin(115200);
  while (!Serial);
  
  Serial.println("\nStarting AdvancedWebServer on " + String(BOARD_NAME));

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

  server.on("/", handleRoot);
  server.on("/test.svg", drawGraph);
  server.on("/inline", []()
  {
    server.send(200, "text/plain", "this works as well");
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
```

---

### Debug Terminal Output

The following are debug terminal output when running example [AdvancedWebServer](examples/AdvancedWebServer) on NRF52840_ITSYBITSY_EXPRESS and ESP8266-AT shield.


```
Starting AdvancedWebServer on NRF52840_ITSYBITSY_EXPRESS
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
<g stroke="black">
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

### Screen Shots

#### Seeeduino SEEED_XIAO_M0

This is the screen shot when running example [AdvancedWebServer](https://github.com/khoih-prog/ESP8266_AT_WebServer/tree/master/examples/AdvancedWebServer) on ***Seeeduino SEEED_XIAO_M0*** board using this [ESP8266_AT_WebServer Library](https://github.com/khoih-prog/ESP8266_AT_WebServer)

<p align="center">
    <img src="https://github.com/khoih-prog/ESP8266_AT_WebServer/blob/master/pics/AdvancedWebServer.png">
</p>

### Screen Shots

#### STM32 Nucleo-144 NUCLEO_F767ZI

This is the screen shot when running example [AdvancedWebServer_STM32](https://github.com/khoih-prog/ESP8266_AT_WebServer/tree/master/examples/AdvancedWebServer_STM32) on ***STM32 Nucleo-144 NUCLEO_F767ZI*** board using this [ESP8266_AT_WebServer Library](https://github.com/khoih-prog/ESP8266_AT_WebServer)

<p align="center">
    <img src="https://github.com/khoih-prog/ESP8266_AT_WebServer/blob/master/pics/AdvancedWebServer_STM32.png">
</p>

---

### New in v1.0.12

1. Add sample Packages_Patches for STM32 (Nucleo-144 NUCLEO_F767ZI, Nucleo-64 NUCLEO_L053R8)
2. Add example for STM32 boards.
3. Add instructions to use EspSerial/Serial1 on some STM32 boards without Serial1 definition (Nucleo-144 NUCLEO_F767ZI, Nucleo-64 NUCLEO_L053R8, etc.)

### New in v1.0.11

1. Add support to all STM32F/L/H/G/WB/MP1 (Nucleo-144 NUCLEO_F767ZI, Nucleo-64 NUCLEO_L053R8, etc.)
2. Add support to Seeeduino SAMD21/SAMD51 boards (SEEED_WIO_TERMINAL, SEEED_FEMTO_M0, SEEED_XIAO_M0, Wio_Lite_MG126, WIO_GPS_BOARD, SEEEDUINO_ZERO, SEEEDUINO_LORAWAN, SEEED_GROVE_UI_WIRELESS, etc.)

### New in v1.0.10

1. Fix bug not closing client and releasing socket.
2. Enhance examples.
3. Add function to be enable to work with [WebSockets2_Generic Library](https://github.com/khoih-prog/WebSockets2_Generic)

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

1. Add support to ***nRF52*** boards, such as ***AdaFruit Feather nRF52832, nRF52840 Express, BlueFruit Sense, Itsy-Bitsy nRF52840 Express, Metro nRF52840 Express, NINA_B302_ublox, NINA_B112_ublox, etc.*** 

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

### TO DO
1. Bug Searching and Killing
2. Add SSL/TLS Client and Server support
3. Support more types of boards using ESP8266 AT-command shields.
4. Add mDNS features.

### DONE

 1. Add support to ***Arduino SAMD21 (ZERO, MKR, NANO_33_IOT, etc.)***
 2. Add support to ***Adafruit SAMD21 (Itsy-Bitsy M0, Metro M0, Feather M0 Express, etc.)***.
 3. Add support to ***Adafruit SAMD51 (Itsy-Bitsy M4, Metro M4, Grand Central M4, Feather M4 Express, etc.)***.
 4. Add support to ***Adafruit nRF52 ( Feather nRF52832, nRF52840 Express, BlueFruit Sense, Itsy-Bitsy nRF52840 Express, Metro nRF52840 Express, NINA_B302_ublox, NINA_B112_ublox, etc.)***.
 5. Add support to SAM DUE.
 6. Add support to ESP32-AT
 7. Add support to W600 and WIS600-01S
 8. Add support to all ***STM32F/L/H/G/WB/MP1***
 9. Add support to ***Seeeduino SAMD21/SAMD51 boards (SEEED_WIO_TERMINAL, SEEED_FEMTO_M0, SEEED_XIAO_M0, Wio_Lite_MG126, WIO_GPS_BOARD, SEEEDUINO_ZERO, SEEEDUINO_LORAWAN, SEEED_GROVE_UI_WIRELESS, etc.)***
 
---
 
### Contributions and Thanks

1. Based on and modified [Ivan Grokhotkov's ESP8266WebServer](https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WebServer)
2. Based on and modified from [bportaluri's WiFiEsp library](https://github.com/bportaluri/WiFiEsp)
3. Thanks to good work of [Miguel Alexandre Wisintainer](https://github.com/tcpipchip) for initiating, inspriring, working with, developing, debugging and testing. Without that, support to nRF52, especially ***U-Blox B302 running as nRF52840 and U-Blox B112 running as nRF52832***, has never been started and finished. Also see [ESP32-based U-BLOX NINA W102 running ENC28J60](https://u-blox-ethernet-ninaw.blogspot.com/).

<table>
  <tr>
    <td align="center"><a href="https://github.com/igrr"><img src="https://github.com/igrr.png" width="100px;" alt="igrr"/><br /><sub><b>⭐️ igrr</b></sub></a><br /></td>
    <td align="center"><a href="https://github.com/bportaluri"><img src="https://github.com/bportaluri.png" width="100px;" alt="bportaluri"/><br /><sub><b>⭐️ bportaluri</b></sub></a><br /></td>
<td align="center"><a href="https://github.com/tcpipchip"><img src="https://github.com/tcpipchip.png" width="100px;" alt="tcpipchip"/><br /><sub><b>⭐️ tcpipchip</b></sub></a><br /></td>
  </tr> 
</table>

### Contributing

If you want to contribute to this project:
- Report bugs and errors
- Ask for enhancements
- Create issues and pull requests
- Tell other people about this library

## Copyright

Copyright 2020- Khoi Hoang


