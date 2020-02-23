/****************************************************************************************************************************
 * UDPSendReceive.ino - Simple Arduino web server sample for ESP8266 AT-command shield
 * For ESP8266 AT-command running shields
 *
 * ESP8266_AT_WebServer is a library for the ESP8266 AT-command shields to run WebServer
 * Forked and modified from ESP8266 https://github.com/esp8266/Arduino/releases
 * Built by Khoi Hoang https://github.com/khoih-prog/ESP8266_AT_WebServer
 * Licensed under MIT license
 * Version: 1.0.2
 *
 * This sketch wait an UDP packet on localPort using a WiFi shield.
 * When a packet is received an 'ACK' packet is sent to the client on port remotePort.
 * For more details see: http://yaab-arduino.blogspot.com/p/wifiesp-example-client.html
 *
 * Version Modified By   Date      Comments
 * ------- -----------  ---------- -----------
 *  1.0.0   K Hoang      12/02/2020 Initial coding for Arduino Mega, Teensy, etc
 *  1.0.1   K Hoang      17/02/2020 Add support to server's lambda function calls
 *  1.0.2   K Hoang      22/02/2020 Add support to SAMD (DUE, ZERO, MKR, NANO_33_IOT, M0, Mo Pro, AdaFruit, etc) boards
 *****************************************************************************************************************************/

#define DEBUG_ESP8266_AT_WEBSERVER_PORT Serial

#include "ESP8266_AT.h"
#include "ESP8266_AT_Udp.h"

#if    ( defined(ARDUINO_SAM_DUE) || defined(ARDUINO_SAMD_ZERO) || defined(ARDUINO_SAMD_MKR1000) || defined(ARDUINO_SAMD_MKRWIFI1010) \
      || defined(ARDUINO_SAMD_NANO_33_IOT) || defined(ARDUINO_SAMD_MKRFox1200) || defined(ARDUINO_SAMD_MKRWAN1300) || defined(ARDUINO_SAMD_MKRWAN1310) \
      || defined(ARDUINO_SAMD_MKRGSM1400) || defined(ARDUINO_SAMD_MKRNB1500) || defined(ARDUINO_SAMD_MKRVIDOR4000) || defined(__SAMD21G18A__) \
      || defined(ARDUINO_SAMD_CIRCUITPLAYGROUND_EXPRESS) || defined(__SAM3X8E__) || defined(__CPU_ARC__) )      
  #if defined(ESP8266_AT_USE_SAMD)
    #undef ESP8266_AT_USE_SAMD
  #endif
  #define ESP8266_AT_USE_SAMD      true
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

#else
  // For Mega
  #define EspSerial Serial3
  #define BOARD_TYPE      "AVR Mega"
#endif

char ssid[] = "****";        // your network SSID (name)
char pass[] = "****";        // your network password

int status = WL_IDLE_STATUS;      // the Wifi radio's status

unsigned int localPort = 1883;    //10002;  // local port to listen on

char packetBuffer[255];          // buffer to hold incoming packet
char ReplyBuffer[] = "ACK";      // a string to send back

ESP8266_AT_UDP Udp;

void printWifiStatus() 
{
  // print the SSID of the network you're attached to:
  // you're connected now, so print out the data
  Serial.print(F("You're connected to the network, IP = "));
  Serial.println(WiFi.localIP());
  
  Serial.print(F("SSID: "));
  Serial.print(WiFi.SSID());

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print(F(", Signal strength (RSSI):"));
  Serial.print(rssi);
  Serial.println(F(" dBm"));
}

void setup() 
{
  Serial.begin(115200);
  
  // initialize serial for ESP module
  EspSerial.begin(115200);

  // initialize ESP module
  WiFi.init(&EspSerial);

  Serial.println("\nStarting UDPSendReceive on " + String(BOARD_TYPE));

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

  printWifiStatus();

  Serial.println(F("\nStarting connection to server..."));
  // if you get a connection, report back via serial:
  Udp.begin(localPort);
  
  Serial.print(F("Listening on port "));
  Serial.println(localPort);
}

void loop() 
{
  // if there's data available, read a packet
  int packetSize = Udp.parsePacket();
  
  if (packetSize) 
  {
    Serial.print(F("Received packet of size "));
    Serial.println(packetSize);
    Serial.print(F("From "));
    IPAddress remoteIp = Udp.remoteIP();
    Serial.print(remoteIp);
    Serial.print(F(", port "));
    Serial.println(Udp.remotePort());

    // read the packet into packetBufffer
    int len = Udp.read(packetBuffer, 255);
    if (len > 0) 
    {
      packetBuffer[len] = 0;
    }
    
    Serial.println(F("Contents:"));
    Serial.println(packetBuffer);

    // send a reply, to the IP address and port that sent us the packet we received
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    Udp.write(ReplyBuffer);
    Udp.endPacket();
  }
}
