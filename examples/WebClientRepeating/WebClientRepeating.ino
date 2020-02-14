/****************************************************************************************************************************
    WebClientRepeating.ino - Simple Arduino web server sample for ESP8266 AT-command shield
    For ESP8266 AT-command running shields

    ESP8266_AT_WebServer is a library for the ESP8266 AT-command shields to run WebServer
    Forked and modified from ESP8266 https://github.com/esp8266/Arduino/releases
    Built by Khoi Hoang https://github.com/khoih-prog/ESP8266_AT_WebServer
    Licensed under MIT license
    Version: 1.0.0

    This sketch connects to a web server and makes an HTTP request
    using an Arduino ESP8266 module.
    It repeats the HTTP call each 10 seconds.

    For more details see: http://yaab-arduino.blogspot.com/p/wifiesp.html

    For more details see: http://yaab-arduino.blogspot.com/p/wifiesp-example-client.html

    Version Modified By   Date      Comments
    ------- -----------  ---------- -----------
     1.0.0   K Hoang      12/02/2020 Initial coding for Arduino Mega, Teensy, etc
 *****************************************************************************************************************************/
#define DEBUG_ESP8266_AT_WEBSERVER_PORT Serial

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

int status = WL_IDLE_STATUS;      // the Wifi radio's status

char server[] = "arduino.cc";

unsigned long lastConnectionTime = 0;         // last time you connected to the server, in milliseconds
const unsigned long postingInterval = 10000L; // delay between updates, in milliseconds

// Initialize the Web client object
ESP8266_AT_Client client;

void printWifiStatus() 
{
  // print the SSID of the network you're attached to:
  // you're connected now, so print out the data
  Serial.print(F("You're connected to the network, IP = "));
  Serial.println(WiFi.localIP());
  
  Serial.print(F("SSID: "));
  Serial.print(WiFi.SSID());

  // print the received signal strength:
  int32_t rssi = WiFi.RSSI();
  Serial.print(F(", Signal strength (RSSI):"));
  Serial.print(rssi);
  Serial.println(F(" dBm"));
}

// this method makes a HTTP connection to the server
void httpRequest()
{
  Serial.println();
    
  // close any connection before send a new request
  // this will free the socket on the WiFi shield
  client.stop();

  // if there's a successful connection
  if (client.connect(server, 80)) 
  {
    Serial.println(F("Connecting..."));
    
    // send the HTTP PUT request
    client.println(F("GET /asciilogo.txt HTTP/1.1"));
    client.println(F("Host: arduino.cc"));
    client.println(F("Connection: close"));
    client.println();

    // note the time that the connection was made
    lastConnectionTime = millis();
  }
  else 
  {
    // if you couldn't make a connection
    Serial.println(F("Connection failed"));
  }
}

void setup()
{
  Serial.begin(115200);
  
  // initialize serial for ESP module
  EspSerial.begin(115200);

  // initialize ESP module
  WiFi.init(&EspSerial);

  Serial.println("\nStarting WebClientRepeating on " + String(BOARD_TYPE));

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
    Serial.print(F("Connecting to SSID: "));
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
  }

  // you're connected now, so print out the data 
  printWifiStatus();
}

void loop()
{
  // if there's incoming data from the net connection send it out the serial port
  // this is for debugging purposes only
  while (client.available()) 
  {
    char c = client.read();
    Serial.write(c);
  }

  // if 10 seconds have passed since your last connection,
  // then connect again and send data
  if (millis() - lastConnectionTime > postingInterval) 
  {
    httpRequest();
  }
}
