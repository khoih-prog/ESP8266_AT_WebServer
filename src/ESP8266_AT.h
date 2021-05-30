/****************************************************************************************************************************
  ESP8266_AT.h - Dead simple web-server.
  For ESP8266/ESP32 AT-command running shields

  ESP8266_AT_WebServer is a library for the ESP8266/ESP32 AT-command shields to run WebServer
  Based on and modified from ESP8266 https://github.com/esp8266/Arduino/releases
  Built by Khoi Hoang https://github.com/khoih-prog/ESP8266_AT_WebServer
  Licensed under MIT license

  Original author:
  @file       Esp8266WebServer.h
  @author     Ivan Grokhotkov

  Version: 1.3.0

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
  1.1.2   K Hoang      28/12/2020 Suppress all possible compiler warnings
  1.2.0   K Hoang      11/05/2021 Add support to BOARD_SIPEED_MAIX_DUINO and RASPBERRY_PI_PICO using Arduino-pico core
  1.3.0   K Hoang      29/05/2021 Add support to Nano_RP2040_Connect, RASPBERRY_PI_PICO using Arduino mbed code
 *****************************************************************************************************************************/

#ifndef ESP8266_AT_h
#define ESP8266_AT_h

#include <Arduino.h>
#include <Stream.h>
#include <IPAddress.h>
#include <inttypes.h>

#include "utility/ESP8266_AT_Drv.h"

class ESP8266_AT_Class
{

  public:

    static int16_t _state[MAX_SOCK_NUM];
    static uint16_t _server_port[MAX_SOCK_NUM];

    ESP8266_AT_Class();

    // KH New from v1.0.8
    /**
      ReInitialize the ESP module, using current espSerial

      param espSerial: the serial interface (HW or SW) used to communicate with the ESP module
    */
    static void reInit(void);

    /**
      Initialize the ESP module.

      param espSerial: the serial interface (HW or SW) used to communicate with the ESP module
    */
    static void init(Stream* espSerial);


    /**
      Get firmware version
    */
    static char* firmwareVersion();


    // NOT IMPLEMENTED
    //int begin(char* ssid);

    // NOT IMPLEMENTED
    //int begin(char* ssid, uint8_t key_idx, const char* key);


    /**
      Start Wifi connection with passphrase
      the most secure supported mode will be automatically selected

      param ssid: Pointer to the SSID string.
      param passphrase: Passphrase. Valid characters in a passphrase
    		  must be between ASCII 32-126 (decimal).
    */
    int begin(const char* ssid, const char* passphrase);


    /**
      Change Ip configuration settings disabling the DHCP client

      param local_ip:	Static ip configuration
    */
    void config(IPAddress local_ip);


    // NOT IMPLEMENTED
    //void config(IPAddress local_ip, IPAddress dns_server);

    // NOT IMPLEMENTED
    //void config(IPAddress local_ip, IPAddress dns_server, IPAddress gateway);

    // NOT IMPLEMENTED
    //void config(IPAddress local_ip, IPAddress dns_server, IPAddress gateway, IPAddress subnet);

    // NOT IMPLEMENTED
    //void setDNS(IPAddress dns_server1);

    // NOT IMPLEMENTED
    //void setDNS(IPAddress dns_server1, IPAddress dns_server2);

    /**
      Disconnect from the network

      return: one value of wl_status_t enum
    */
    int disconnect(void);

    /**
      Get the interface MAC address.

      return: pointer to uint8_t array with length WL_MAC_ADDR_LENGTH
    */
    uint8_t* macAddress(uint8_t* mac);

    /**
      Get the interface IP address.

      return: Ip address value
    */
    IPAddress localIP();


    /**
      Get the interface subnet mask address.

      return: subnet mask address value
    */
    IPAddress subnetMask();

    /**
      Get the gateway ip address.

      return: gateway ip address value
    */
    IPAddress gatewayIP();

    /**
      Return the current SSID associated with the network

      return: ssid string
    */
    char* SSID();

    /**
      Return the current BSSID associated with the network.
      It is the MAC address of the Access Point

      return: pointer to uint8_t array with length WL_MAC_ADDR_LENGTH
    */
    uint8_t* BSSID(uint8_t* bssid);


    /**
      Return the current RSSI /Received Signal Strength in dBm)
      associated with the network

      return: signed value
    */
    int32_t RSSI();


    /**
      Return Connection status.

      return: one of the value defined in wl_status_t
              see https://www.arduino.cc/en/Reference/WiFiStatus
    */
    uint8_t status();


    /*
        Return the Encryption Type associated with the network

        return: one value of wl_enc_type enum
    */
    //uint8_t	encryptionType();

    /*
       Start scan WiFi networks available

       return: Number of discovered networks
    */
    int8_t scanNetworks();

    /*
       Return the SSID discovered during the network scan.

       param networkItem: specify from which network item want to get the information

       return: ssid string of the specified item on the networks scanned list
    */
    char*	SSID(uint8_t networkItem);

    /*
       Return the encryption type of the networks discovered during the scanNetworks

       param networkItem: specify from which network item want to get the information

       return: encryption type (enum wl_enc_type) of the specified item on the networks scanned list
    */
    uint8_t	encryptionType(uint8_t networkItem);

    /*
       Return the RSSI of the networks discovered during the scanNetworks

       param networkItem: specify from which network item want to get the information

       return: signed value of RSSI of the specified item on the networks scanned list
    */
    int32_t RSSI(uint8_t networkItem);


    // NOT IMPLEMENTED
    //int hostByName(const char* aHostname, IPAddress& aResult);



    ////////////////////////////////////////////////////////////////////////////
    // Non standard methods
    ////////////////////////////////////////////////////////////////////////////

    /**
      Start the ESP access point.

      param ssid: Pointer to the SSID string.
      param channel: WiFi channel (1-14)
      param pwd: Passphrase. Valid characters in a passphrase
    		  must be between ASCII 32-126 (decimal).
      param enc: encryption type (enum wl_enc_type)
      param apOnly: Set to false if you want to run AP and Station modes simultaneously
    */
    int beginAP(const char* ssid, uint8_t channel, const char* pwd, uint8_t enc, bool apOnly = true);

    /*
      Start the ESP access point with open security.
    */
    int beginAP(const char* ssid);
    int beginAP(const char* ssid, uint8_t channel);

    /**
      Change IP address of the AP

      param ip:	Static ip configuration
    */
    void configAP(IPAddress ip);

    /**
      Restart the ESP module.
    */
    void reset();
    
    /**
      Restore the the Factory Default Settings of ESP module.
      Sometimes necessaty for ESP32-AT
    */
    void restore();

    /**
      Ping a host.
    */
    bool ping(const char *host);


    friend class ESP8266_AT_Client;
    friend class ESP8266_AT_Server;
    friend class ESP8266_AT_UDP;

  private:
    static uint8_t getFreeSocket();
    static void allocateSocket(uint8_t sock);
    static void releaseSocket(uint8_t sock);

    static uint8_t espMode;
};

extern ESP8266_AT_Class WiFi;

#include "ESP8266_AT-impl.h"

#endif
