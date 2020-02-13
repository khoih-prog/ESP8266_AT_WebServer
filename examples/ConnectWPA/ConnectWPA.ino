/****************************************************************************************************************************
    ConnectWPA.ino - Simple Arduino web server sample for ESP8266 AT-command shield
    For ESP8266 AT-command running shields

    ESP8266_AT_WebServer is a library for the ESP8266 AT-command shields to run WebServer
    Forked and modified from ESP8266 https://github.com/esp8266/Arduino/releases
    Built by Khoi Hoang https://github.com/khoih-prog/ESP8266_AT_WebServer
    Licensed under MIT license
    Version: 1.0.0

    This example connects to an encrypted WiFi network using an ESP8266 module.
    Then it prints the  MAC address of the WiFi shield, the IP address obtained
    and other network details.
    For more details see: http://yaab-arduino.blogspot.com/p/wifiesp-example-connect.html

    Version Modified By   Date      Comments
    ------- -----------  ---------- -----------
     1.0.0   K Hoang      12/02/2020 Initial coding for Arduino Mega, Teensy, etc
 *****************************************************************************************************************************/

#include "ESP8266_AT.h"

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

#else
// For Mega
#define EspSerial Serial3
#define BOARD_TYPE      "AVR Mega"
#endif

char ssid[] = "****";        // your network SSID (name)
char pass[] = "****";        // your network password

int status = WL_IDLE_STATUS;     // the Wifi radio's status
int reqCount = 0;                // number of requests received

void printWifiData()
{
  // print your WiFi shield's IP address
  IPAddress ip = WiFi.localIP();
  Serial.print(F("IP Address: "));
  Serial.println(ip);

  // print your MAC address
  byte mac[6];
  WiFi.macAddress(mac);
  char buf[20];
  sprintf(buf, "%02X:%02X:%02X:%02X:%02X:%02X", mac[5], mac[4], mac[3], mac[2], mac[1], mac[0]);
  Serial.print(F("MAC address: "));
  Serial.println(buf);
}

void printCurrentNet()
{
  // print the SSID of the network you're attached to
  Serial.print(F("SSID: "));
  Serial.println(WiFi.SSID());

  // print the MAC address of the router you're attached to
  byte bssid[6];
  WiFi.BSSID(bssid);
  char buf[20];
  sprintf(buf, "%02X:%02X:%02X:%02X:%02X:%02X", bssid[5], bssid[4], bssid[3], bssid[2], bssid[1], bssid[0]);
  Serial.print(F("BSSID: "));
  Serial.println(buf);

  // print the received signal strength
  long rssi = WiFi.RSSI();
  Serial.print(F("Signal strength (RSSI): "));
  Serial.println(rssi);
}

void setup()
{
  Serial.begin(115200);

  // initialize serial for ESP module
  EspSerial.begin(115200);
  // initialize ESP module
  WiFi.init(&EspSerial);

  Serial.println("\nStarting ConnectWPA on " + String(BOARD_TYPE));

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

  Serial.println("You're connected to the network");
}

void loop()
{
  // print the network connection information every 10 seconds
  Serial.println();
  printCurrentNet();
  printWifiData();

  delay(10000);
}
