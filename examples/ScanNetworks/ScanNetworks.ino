/****************************************************************************************************************************
 *  ScanNetworks.ino - Simple Arduino web server sample for ESP8266 AT-command shield
 *  For ESP8266 AT-command running shields
 *
 *  ESP8266_AT_WebServer is a library for the ESP8266 AT-command shields to run WebServer
 *  Forked and modified from ESP8266 https://github.com/esp8266/Arduino/releases
 *  Built by Khoi Hoang https://github.com/khoih-prog/ESP8266_AT_WebServer
 *  Licensed under MIT license
 *  Version: 1.0.2
 *
 *  This example  prints the Wifi shield's MAC address, and
 *  scans for available Wifi networks using the Wifi shield.
 *  Every ten seconds, it scans again. It doesn't actually
 *  connect to any network, so no encryption scheme is specified.
 *
 *  For more details see: http://yaab-arduino.blogspot.com/p/wifiesp.html
 *  Version Modified By   Date      Comments
 * ------- -----------  ---------- -----------
 *  1.0.0   K Hoang      12/02/2020 Initial coding for Arduino Mega, Teensy, etc
 *  1.0.1   K Hoang      17/02/2020 Add support to server's lambda function calls
 *  1.0.2   K Hoang      22/02/2020 Add support to SAMD (DUE, ZERO, MKR, NANO_33_IOT, M0, Mo Pro, AdaFruit, etc) boards
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

int status = WL_IDLE_STATUS;     // the Wifi radio's status
int reqCount = 0;                // number of requests received

void printMacAddress()
{
  // get your MAC address
  byte mac[6];
  WiFi.macAddress(mac);
  
  // print MAC address
  char buf[20];
  sprintf(buf, "%02X:%02X:%02X:%02X:%02X:%02X", mac[5], mac[4], mac[3], mac[2], mac[1], mac[0]);
  Serial.print("MAC address: ");
  Serial.println(buf);
}

void listNetworks()
{
  // scan for nearby networks
  int numSsid = WiFi.scanNetworks();
  
  if (numSsid == -1) 
  {
    Serial.println(F("Couldn't get a wifi connection"));
    while (true);
  }

  // print the list of networks seen
  Serial.print(F("Number of available networks:"));
  Serial.println(numSsid);

  // print the network number and name for each network found
  for (int thisNet = 0; thisNet < numSsid; thisNet++) {
    Serial.print(thisNet);
    Serial.print(F(") "));
    Serial.print(WiFi.SSID(thisNet));
    Serial.print(F("\tSignal: "));
    Serial.print(WiFi.RSSI(thisNet));
    Serial.print(F(" dBm"));
    Serial.print(F("\tEncryption: "));
    printEncryptionType(WiFi.encryptionType(thisNet));
  }
}

void printEncryptionType(int thisType) 
{
  // read the encryption type and print out the name
  switch (thisType) 
  {
    case ENC_TYPE_WEP:
      Serial.print(F("WEP"));
      break;
    case ENC_TYPE_WPA_PSK:
      Serial.print(F("WPA_PSK"));
      break;
    case ENC_TYPE_WPA2_PSK:
      Serial.print(F("WPA2_PSK"));
      break;
    case ENC_TYPE_WPA_WPA2_PSK:
      Serial.print(F("WPA_WPA2_PSK"));
      break;
    case ENC_TYPE_NONE:
      Serial.print(F("None"));
      break;
  }
  Serial.println();
}

void setup() 
{
  Serial.begin(115200);

  // initialize serial for ESP module
  EspSerial.begin(115200);
  // initialize ESP module
  WiFi.init(&EspSerial);

  Serial.println("\nStarting ScanNetworks on " + String(BOARD_TYPE));

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

  Serial.print(F("You're connected to the network, IP = "));
  Serial.println(WiFi.localIP());

  // Print WiFi MAC address
  printMacAddress();
}

void loop()
{
  // scan for existing networks
  Serial.println();
  Serial.println(F("Scanning available networks..."));
  listNetworks();
  delay(10000);
}
