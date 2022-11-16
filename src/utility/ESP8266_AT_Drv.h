/****************************************************************************************************************************
  ESP8266_AT_Drv.h - Dead simple web-server.
  For ESP8266/ESP32 AT-command running shields

  ESP8266_AT_WebServer is a library for the ESP8266/ESP32 AT-command shields to run WebServer
  Based on and modified from ESP8266 https://github.com/esp8266/Arduino/releases
  Built by Khoi Hoang https://github.com/khoih-prog/ESP8266_AT_WebServer
  Licensed under MIT license

  Original author:
  @file       Esp8266WebServer.h
  @author     Ivan Grokhotkov

  Version: 1.6.0

  Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
  1.0.0   K Hoang      12/02/2020 Initial coding for Arduino Mega, Teensy, etc
  ...
  1.4.0   K Hoang      14/08/2021 Add support to Adafruit nRF52 core v0.22.0+
  1.4.1   K Hoang      08/12/2021 Add Packages_Patches and instructions for BOARD_SIPEED_MAIX_DUINO
  1.5.0   K Hoang      19/12/2021 Reduce usage of Arduino String with std::string
  1.5.1   K Hoang      24/12/2021 Fix bug
  1.5.2   K Hoang      28/12/2021 Fix wrong http status header bug
  1.5.3   K Hoang      12/01/2022 Fix authenticate issue caused by libb64
  1.5.4   K Hoang      26/04/2022 Use new arduino.tips site. Improve examples
  1.6.0   K Hoang      16/11/2022 Fix severe limitation to permit sending larger data than 2K buffer. Add CORS
 *****************************************************************************************************************************/

#ifndef ESP8266_AT_Drv_h
#define ESP8266_AT_Drv_h

////////////////////////////////////////

#if defined(ARDUINO_ARCH_MBED)
  #include <stdarg.h>
#endif

#include "Stream.h"
#include "IPAddress.h"

#include "RingBuffer.h"

////////////////////////////////////////

#ifndef ESP_AT_UNUSED
  #define ESP_AT_UNUSED(x) (void)(x)
#endif

////////////////////////////////////////

extern bool useESP32_AT;

// Maximum size of a SSID
#define WL_SSID_MAX_LENGTH 32

// Size of a MAC-address or BSSID
#define WL_MAC_ADDR_LENGTH 6

// Size of a MAC-address or BSSID
#define WL_IPV4_LENGTH 4

// Maximum size of a SSID list
#define WL_NETWORKS_LIST_MAXNUM 10

// Maxmium number of socket
#define MAX_SOCK_NUM    4

// Socket not available constant
#define SOCK_NOT_AVAIL  255

// Default state value for Wifi state field
#define NA_STATE -1

#define WL_FW_VER_LENGTH 6

#define NO_SOCKET_AVAIL 255

// maximum size of AT command
#define CMD_BUFFER_SIZE 200

////////////////////////////////////////

// KH, add UDP_MULTICAST_MODE to support MultiCast for v1.1.0
typedef enum eProtMode
{
  TCP_MODE,
  UDP_MODE,
  SSL_MODE,
  UDP_MULTICAST_MODE
} tProtMode;

////////////////////////////////////////

typedef enum
{
  WL_FAILURE = -1,
  WL_SUCCESS = 1,
} wl_error_code_t;

////////////////////////////////////////

/* Authentication modes */
enum wl_auth_mode
{
  AUTH_MODE_INVALID,
  AUTH_MODE_AUTO,
  AUTH_MODE_OPEN_SYSTEM,
  AUTH_MODE_SHARED_KEY,
  AUTH_MODE_WPA,
  AUTH_MODE_WPA2,
  AUTH_MODE_WPA_PSK,
  AUTH_MODE_WPA2_PSK
};

////////////////////////////////////////

typedef enum
{
  WL_NO_SHIELD    = 255,
  WL_IDLE_STATUS  = 0,
  //WL_NO_SSID_AVAIL,
  //WL_SCAN_COMPLETED,
  WL_CONNECTED,
  WL_CONNECT_FAILED,
  //WL_CONNECTION_LOST,
  WL_DISCONNECTED
} wl_status_t;

////////////////////////////////////////

/* Encryption modes */
enum wl_enc_type
{
  ENC_TYPE_NONE         = 0,
  ENC_TYPE_WEP          = 1,
  ENC_TYPE_WPA_PSK      = 2,
  ENC_TYPE_WPA2_PSK     = 3,
  ENC_TYPE_WPA_WPA2_PSK = 4
};

////////////////////////////////////////

enum wl_tcp_state
{
  CLOSED      = 0,
  LISTEN      = 1,
  SYN_SENT    = 2,
  SYN_RCVD    = 3,
  ESTABLISHED = 4,
  FIN_WAIT_1  = 5,
  FIN_WAIT_2  = 6,
  CLOSE_WAIT  = 7,
  CLOSING     = 8,
  LAST_ACK    = 9,
  TIME_WAIT   = 10
};

////////////////////////////////////////

class ESP8266_AT_Drv
{
  public:

    // KH New from v1.0.8
    // ReInit, don't need to specify but use current espSerial
    static void wifiDriverReInit(void);

    static void wifiDriverInit(Stream *espSerial);

    /* Start Wifi connection with passphrase

       param ssid: Pointer to the SSID string.
       param passphrase: Passphrase. Valid characters in a passphrase must be between ASCII 32-126 (decimal).
    */
    static bool wifiConnect(const char* ssid, const char* passphrase);

    /*
       Start the Access Point
    */
    static bool wifiStartAP(const char* ssid, const char* pwd, uint8_t channel, uint8_t enc, uint8_t espMode);

    /*
       Set ip configuration disabling dhcp client
    */
    static void config(IPAddress local_ip);

    /*
       Set ip configuration disabling dhcp client
    */
    static void configAP(IPAddress local_ip);

    /*
       Disconnect from the network

       return: WL_SUCCESS or WL_FAILURE
    */
    static int8_t disconnect();

    /*
       return: one value of wl_status_t enum
    */
    static uint8_t getConnectionStatus();

    /*
       Get the interface MAC address.

       return: pointer to uint8_t array with length WL_MAC_ADDR_LENGTH
    */
    static uint8_t* getMacAddress();

    /*
       Get the interface IP address.

       return: copy the ip address value in IPAddress object
    */
    static void getIpAddress(IPAddress& ip);

    static void getIpAddressAP(IPAddress& ip);

    /*
       Get the interface IP netmask.
       This can be used to retrieve settings configured through DHCP.

       return: true if successful
    */
    static bool getNetmask(IPAddress& mask);

    /*
       Get the interface IP gateway.
       This can be used to retrieve settings configured through DHCP.

       return: true if successful
    */
    static bool getGateway(IPAddress& mask);

    /*
       Return the current SSID associated with the network

       return: ssid string
    */
    static char* getCurrentSSID();

    /*
       Return the current BSSID associated with the network.
       It is the MAC address of the Access Point

       return: pointer to uint8_t array with length WL_MAC_ADDR_LENGTH
    */
    static uint8_t* getCurrentBSSID();

    /*
       Return the current RSSI /Received Signal Strength in dBm)
       associated with the network

       return: signed value
    */
    static int32_t getCurrentRSSI();

    /*
       Get the networks available

       return: Number of discovered networks
    */
    static uint8_t getScanNetworks();

    /*
       Return the SSID discovered during the network scan.

       param networkItem: specify from which network item want to get the information

       return: ssid string of the specified item on the networks scanned list
    */
    static char* getSSIDNetoworks(uint8_t networkItem);

    /*
       Return the RSSI of the networks discovered during the scanNetworks

       param networkItem: specify from which network item want to get the information

       return: signed value of RSSI of the specified item on the networks scanned list
    */
    static int32_t getRSSINetoworks(uint8_t networkItem);

    /*
       Return the encryption type of the networks discovered during the scanNetworks

       param networkItem: specify from which network item want to get the information

       return: encryption type (enum wl_enc_type) of the specified item on the networks scanned list
    */
    static uint8_t getEncTypeNetowrks(uint8_t networkItem);

    /*
       Get the firmware version
    */
    static char* getFwVersion();

    ////////////////////////////////////////////////////////////////////////////
    // Client/Server methods
    ////////////////////////////////////////////////////////////////////////////

    static bool startServer(uint16_t port, uint8_t sock);
    static bool startClient(const char* host, uint16_t port, uint8_t sock, uint8_t protMode);
    static void stopClient(uint8_t sock);
    static uint8_t getServerState(uint8_t sock);
    static uint8_t getClientState(uint8_t sock);
    static bool getData(uint8_t connId, uint8_t *data, bool peek, bool* connClose);
    static int getDataBuf(uint8_t connId, uint8_t *buf, uint16_t bufSize);
    static bool sendData(uint8_t sock, const uint8_t *data, uint16_t len);
    static bool sendData(uint8_t sock, const __FlashStringHelper *data, uint16_t len, bool appendCrLf = false);
    static bool sendDataUdp(uint8_t sock, const char* host, uint16_t port, const uint8_t *data, uint16_t len);
    static uint16_t availData(uint8_t connId);

    static bool ping(const char *host);
    static void reset();

    // KH New from v1.0.8
    // For ESP32-AT to restores the Factory Default Settings
    static void restore();

    static void getRemoteIpAddress(IPAddress& ip);
    static uint16_t getRemotePort();

    ////////////////////////////////////////

  private:
    static Stream *espSerial;

    static long     _bufPos;
    static uint8_t  _connId;

    static uint16_t _remotePort;
    static uint8_t  _remoteIp[WL_IPV4_LENGTH];

    // firmware version string
    static char   fwVersion[WL_FW_VER_LENGTH];

    // settings of requested network
    static char     _networkSsid[WL_NETWORKS_LIST_MAXNUM][WL_SSID_MAX_LENGTH];
    static int32_t  _networkRssi[WL_NETWORKS_LIST_MAXNUM];
    static uint8_t  _networkEncr[WL_NETWORKS_LIST_MAXNUM];

    // settings of current selected network
    static char     _ssid[WL_SSID_MAX_LENGTH];
    static uint8_t  _bssid[WL_MAC_ADDR_LENGTH];
    static uint8_t  _mac[WL_MAC_ADDR_LENGTH];
    static uint8_t  _localIp[WL_IPV4_LENGTH];

    // the ring buffer is used to search the tags in the stream
    static AT_RingBuffer ringBuf;

    static int sendCmd(const char* cmd, int timeout = 1000);
    static int sendCmd(const char* cmd, int timeout, ...);

    static bool sendCmdGet(const char* cmd, const char* startTag, const char* endTag, char* outStr, int outStrLen);

    static int sendCmd(const __FlashStringHelper* cmd, int timeout = 1000);
    static int sendCmd(const __FlashStringHelper* cmd, int timeout, ...);

    static bool sendCmdGet(const __FlashStringHelper* cmd, const char* startTag, const char* endTag, char* outStr,
                           int outStrLen);
    static bool sendCmdGet(const __FlashStringHelper* cmd, const __FlashStringHelper* startTag,
                           const __FlashStringHelper* endTag, char* outStr, int outStrLen);

    static int readUntil(unsigned int timeout, const char* tag = NULL, bool findTags = true);

    static void espEmptyBuf(bool warn = true);

    static int timedRead();

    ////////////////////////////////////////

    friend class ESP8266_AT;
    friend class ESP8266_AT_Server;
    friend class ESP8266_AT_Client;
    friend class ESP8266_AT_UDP;
};

////////////////////////////////////////

extern ESP8266_AT_Drv esp8266_AT_Drv;

////////////////////////////////////////

#endif
