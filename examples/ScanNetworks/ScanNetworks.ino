/****************************************************************************************************************************
    ScanNetworks.ino - Simple Arduino web server sample for ESP8266/ESP32 AT-command shield
    For ESP8266 AT-command running shields

    ESP8266_AT_WebServer is a library for the ESP8266 AT-command shields to run WebServer
    Forked and modified from ESP8266 https://github.com/esp8266/Arduino/releases
    Built by Khoi Hoang https://github.com/khoih-prog/ESP8266_AT_WebServer
    Licensed under MIT license
    Version: 1.0.9

    This example  prints the Wifi shield's MAC address, and
    scans for available Wifi networks using the Wifi shield.
    Every ten seconds, it scans again. It doesn't actually
    connect to any network, so no encryption scheme is specified.

    For more details see: http://yaab-arduino.blogspot.com/p/wifiesp.html

    Version Modified By   Date      Comments
   ------- -----------  ---------- -----------
    1.0.0   K Hoang      12/02/2020 Initial coding for Arduino Mega, Teensy, etc
    1.0.1   K Hoang      17/02/2020 Add support to server's lambda function calls
    1.0.2   K Hoang      22/02/2020 Add support to SAMD (DUE, ZERO, MKR, NANO_33_IOT, M0, Mo Pro, AdaFruit, etc) boards
    1.0.3   K Hoang      03/03/2020 Add support to STM32 (STM32,F0,F1, F2, F3, F4, F7, etc) boards
    1.0.4   K Hoang      19/03/2020 Fix bug. Sync with ESP8266WebServer library of core v2.6.3
    1.0.5   K Hoang      17/04/2020 Add support to SAMD51 and SAM DUE boards
    1.0.6   K Hoang      11/06/2020 Add support to nRF52 boards, such as AdaFruit Feather nRF52832, nRF52840 Express, BlueFruit Sense, 
                                    Itsy-Bitsy nRF52840 Express, Metro nRF52840 Express, NINA_B30_ublox, etc. 
    1.0.7   K Hoang      23/06/2020 Add support to ESP32-AT. Update deprecated ESP8266-AT commands. Restructure examples. 
    1.0.8   K Hoang      01/07/2020 Fix bug. Add features to ESP32-AT.   
    1.0.9   K Hoang      03/07/2020 Fix bug. Add functions. Restructure codes.   
 *****************************************************************************************************************************/

#include "defines.h"

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
  while (!Serial);

  Serial.println("\nStarting ScanNetworks on " + String(BOARD_TYPE));

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
