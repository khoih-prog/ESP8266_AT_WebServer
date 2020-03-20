/****************************************************************************************************************************
 * WebServerAP.ino - Simple Arduino web server sample for ESP8266 AT-command shield
 * For ESP8266 AT-command running shields
 *
 * ESP8266_AT_WebServer is a library for the ESP8266 AT-command shields to run WebServer
 * Forked and modified from ESP8266 https://github.com/esp8266/Arduino/releases
 * Built by Khoi Hoang https://github.com/khoih-prog/ESP8266_AT_WebServer
 * Licensed under MIT license
 * Version: 1.0.4
 *
 * A simple web server that shows the value of the analog input pins via a web page using an ESP8266 module.
 * This sketch will start an access point and print the IP address of your ESP8266 module to the Serial monitor. 
 * From there, you can open that address in a web browser to display the web page.
 * The web page will be automatically refreshed each 20 seconds.
 * 
 * For more details see: http://yaab-arduino.blogspot.com/p/wifiesp.html
 *
 * Version Modified By   Date      Comments
 * ------- -----------  ---------- -----------
 *  1.0.0   K Hoang      12/02/2020 Initial coding for Arduino Mega, Teensy, etc
 *  1.0.1   K Hoang      17/02/2020 Add support to server's lambda function calls
 *  1.0.2   K Hoang      22/02/2020 Add support to SAMD (DUE, ZERO, MKR, NANO_33_IOT, M0, Mo Pro, AdaFruit, etc) boards
 *  1.0.3   K Hoang      03/03/2020 Add support to STM32 (STM32,F0,F1, F2, F3, F4, F7, etc) boards
 *  1.0.4   K Hoang      19/03/2020 Fix bug. Sync with ESP8266WebServer library of core v2.6.3
 *****************************************************************************************************************************/

#define DEBUG_ESP8266_AT_WEBSERVER_PORT Serial

#include "ESP8266_AT.h"

#if    ( defined(ARDUINO_SAM_DUE) || defined(ARDUINO_SAMD_ZERO) || defined(ARDUINO_SAMD_MKR1000) || defined(ARDUINO_SAMD_MKRWIFI1010) \
      || defined(ARDUINO_SAMD_NANO_33_IOT) || defined(ARDUINO_SAMD_MKRFox1200) || defined(ARDUINO_SAMD_MKRWAN1300) || defined(ARDUINO_SAMD_MKRWAN1310) \
      || defined(ARDUINO_SAMD_MKRGSM1400) || defined(ARDUINO_SAMD_MKRNB1500) || defined(ARDUINO_SAMD_MKRVIDOR4000) || defined(__SAMD21G18A__) \
      || defined(ARDUINO_SAMD_CIRCUITPLAYGROUND_EXPRESS) || defined(__SAM3X8E__) || defined(__CPU_ARC__) )      
  #if defined(ESP8266_AT_USE_SAMD)
    #undef ESP8266_AT_USE_SAMD
  #endif
  #define ESP8266_AT_USE_SAMD      true
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
  #elif ( defined(__SAM3X8E__) || (__SAM3X8E__) || (__CPU_ARC__) )
    #define BOARD_TYPE      "SAMD Board"
  #else
    #define BOARD_TYPE      "SAMD Unknown"
  #endif

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

#ifdef CORE_TEENSY
char ssid[] = "ESP8266_AT_TEENSY";    // your network SSID (name)
char pass[] = "ESP8266_AT_PW";        // your network password
#elif defined(ESP8266_AT_USE_SAMD)
char ssid[] = "ESP8266_AT_SAMD";    // your network SSID (name)
char pass[] = "ESP8266_AT_PW";        // your network password
#else
char ssid[] = "ESP8266_AT_AVR";       // your network SSID (name)
char pass[] = "ESP8266_AT_PW";        // your network password
#endif

int status = WL_IDLE_STATUS;          // the Wifi radio's status
int reqCount = 0;                     // number of requests received

ESP8266_AT_Server server(80);

// use a ring buffer to increase speed and reduce memory allocation
AT_RingBuffer buf(8);

void printWifiStatus()
{
  // print the SSID of the network you're attached to:
  // you're connected now, so print out the data
  Serial.print(F("AP IP Address = "));
  Serial.println(WiFi.localIP());

  Serial.print(F("SSID: "));
  Serial.print(ssid);
  Serial.print(F(", PASS: "));
  Serial.println(pass);
}

void sendHttpResponse(ESP8266_AT_Client client)
{
  client.print(
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: text/html\r\n"
    "Connection: close\r\n"  // the connection will be closed after completion of the response
    "Refresh: 20\r\n"        // refresh the page automatically every 20 sec
    "\r\n");
  client.print("<!DOCTYPE HTML>\r\n");
  client.print("<html>\r\n");
  client.print("<h1>Hello World!</h1>\r\n");
  client.print("Requests received: ");
  client.print(++reqCount);
  client.print("<br>\r\n");
  client.print("Analog input A0: ");
  client.print(analogRead(0));
  client.print("<br>\r\n");
  client.print("</html>\r\n");
}

void setup()
{
  Serial.begin(115200);

  // initialize serial for ESP module
  EspSerial.begin(115200);

  // initialize ESP module
  WiFi.init(&EspSerial);

  Serial.println("\nStarting WebServerAP on " + String(BOARD_TYPE));

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
  int AP_channel = 10;
  status = WiFi.beginAP(ssid, AP_channel, pass, ENC_TYPE_WPA2_PSK);

  Serial.println(F("Access point started"));
  
  printWifiStatus();

  // start the web server on port 80
  server.begin();
  Serial.print(F("WebServer started @ "));
  Serial.println(WiFi.localIP());
}


void loop()
{
  ESP8266_AT_Client client = server.available();  // listen for incoming clients

  if (client)
  {
    // if you get a client,
    Serial.println(F("New client"));             // print a message out the serial port
    buf.init();                               // initialize the circular buffer
    while (client.connected())
    {
      // loop while the client's connected
      if (client.available())
      {
        // if there's bytes to read from the client,
        char c = client.read();               // read a byte, then
        buf.push(c);                          // push it to the ring buffer

        // you got two newline characters in a row
        // that's the end of the HTTP request, so send a response
        if (buf.endsWith("\r\n\r\n")) {
          sendHttpResponse(client);
          break;
        }
      }
    }

    // give the web browser time to receive the data
    delay(10);

    // close the connection
    client.stop();
    Serial.println(F("Client disconnected"));
  }
}
