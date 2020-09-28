/****************************************************************************************************************************
  WebServerAP.ino - Simple Arduino web server sample for ESP8266 AT-command shield
  For ESP8266/ESP32 AT-command running shields

  ESP8266_AT_WebServer is a library for the ESP8266/ESP32 AT-command shields to run WebServer
  Based on and modified from ESP8266 https://github.com/esp8266/Arduino/releases
  Built by Khoi Hoang https://github.com/khoih-prog/ESP8266_AT_WebServer
  Licensed under MIT license
  
  Original author:
  @file       Esp8266WebServer.h
  @author     Ivan Grokhotkov
  
  Version: 1.1.1
  
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
 *****************************************************************************************************************************/

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

int status = WL_IDLE_STATUS;          // the Wifi radio's status
int reqCount = 0;                     // number of requests received

ESP8266_AT_WebServer server(80);

void printWifiStatus()
{
  // print the SSID/PASS of the network you're created:
  Serial.print(F("AP IP Address = "));
  Serial.println(WiFi.localIP());

  Serial.print(F("SSID: "));
  Serial.print(ssid);
  Serial.print(F(", PASS: "));
  Serial.println(pass);
}

void handleRoot()
{
#define BUFFER_SIZE     400
  
  char temp[BUFFER_SIZE];
  
  memset(temp, 0, sizeof(temp));
  
  int sec = millis() / 1000;
  int min = sec / 60;
  int hr = min / 60;
  int day = hr / 24;

  snprintf(temp, BUFFER_SIZE - 1,
           "<html>\
<head>\
<meta http-equiv='refresh' content='5'/>\
<title>%s</title>\
<style>\
body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
</style>\
</head>\
<body>\
<h1>Hello from %s</h1>\
<h2>running WebServerAP</h2>\
<h3>on %s</h3>\
<h3>Uptime: %d d %02d:%02d:%02d</h3>\
<p>Requests received: %d</p>\
<p>Analog input A0: %d</p>\
</body>\
</html>", BOARD_NAME, BOARD_NAME, SHIELD_TYPE, day, hr, min % 60, sec % 60, ++reqCount, analogRead(0));

  server.send(200, F("text/html"), temp);
}

void handleNotFound()
{ 
  String message = F("File Not Found\n\n");
  
  message += F("URI: ");
  message += server.uri();
  message += F("\nMethod: ");
  message += (server.method() == HTTP_GET) ? F("GET") : F("POST");
  message += F("\nArguments: ");
  message += server.args();
  message += F("\n");
  
  for (uint8_t i = 0; i < server.args(); i++)
  {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  
  server.send(404, F("text/plain"), message);
}

void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  while (!Serial);

  Serial.print("\nStarting WebServerAP on " + String(BOARD_NAME));
  Serial.println(" with " + String(SHIELD_TYPE));

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

  Serial.print(F("Attempting to start AP with SSID = "));
  Serial.print(ssid);
  Serial.print(F(" and PW = "));
  Serial.println(pass);

  // uncomment these two lines if you want to set the IP address of the AP
  IPAddress localIp(192, 168, 120, 1);
  WiFi.configAP(localIp);

  // start access point
  int AP_channel = 1;
  status = WiFi.beginAP(ssid, AP_channel, pass, ENC_TYPE_WPA2_PSK);

  Serial.print(F("Access point started @ channel "));
  Serial.println(AP_channel);

  printWifiStatus();

  server.on(F("/"), handleRoot);
  
  server.on(F("/inline"), []()
  {
    server.send(200, F("text/plain"), F("This works as well"));
  });

  server.onNotFound(handleNotFound);

  // start the web server on port 80
  server.begin();

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  
  Serial.print(F("WebServer started @ "));
  Serial.println(ip);
  
  // print where to go in a browser:
  Serial.print(F("To see this page in action, open a browser to http://"));
  Serial.println(ip);
}


void loop()
{
  server.handleClient();
}
