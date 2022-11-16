/****************************************************************************************************************************
  ESP8266_AT_WebServer.h - Dead simple web-server.
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

#ifndef ESP8266_AT_WebServer_h
#define ESP8266_AT_WebServer_h

////////////////////////////////////////

#define ESP8266_AT_WEBSERVER_VERSION          "ESP8266_AT_WebServer v1.6.0"

#define ESP8266_AT_WEBSERVER_VERSION_MAJOR    1
#define ESP8266_AT_WEBSERVER_VERSION_MINOR    6
#define ESP8266_AT_WEBSERVER_VERSION_PATCH    0

#define ESP8266_AT_WEBSERVER_VERSION_INT      1006000

////////////////////////////////////////

#ifndef ESP_AT_UNUSED
  #define ESP_AT_UNUSED(x) (void)(x)
#endif

////////////////////////////////////////

#ifndef USE_ESP32_AT
  // Use ESP8266-AT commands only, some ESP32-AT commands not support _CUR and _DEF options
  // _CUR (CURrent) for not saving Config Data to Flash
  // _DEF (DEFault) for saving Config Data to Flash
  #define USE_ESP32_AT     false
#endif

////////////////////////////////////////

#if USE_ESP32_AT
  bool useESP32_AT = true;

  #if(_ESP_AT_LOGLEVEL_> 3)
    #warning Using USE_ESP32_AT from ESP8266_AT_WebServer.h
  #endif
#else
  bool useESP32_AT = false;

  #if(_ESP_AT_LOGLEVEL_> 3)
    #warning Using USE_ESP8266_AT from ESP8266_AT_WebServer.h
  #endif
#endif

////////////////////////////////////////

#if ( defined(ARDUINO_SAMD_ZERO) || defined(ARDUINO_SAMD_MKR1000) || defined(ARDUINO_SAMD_MKRWIFI1010) \
   || defined(ARDUINO_SAMD_NANO_33_IOT) || defined(ARDUINO_SAMD_MKRFox1200) || defined(ARDUINO_SAMD_MKRWAN1300) || defined(ARDUINO_SAMD_MKRWAN1310) \
   || defined(ARDUINO_SAMD_MKRGSM1400) || defined(ARDUINO_SAMD_MKRNB1500) || defined(ARDUINO_SAMD_MKRVIDOR4000) || defined(__SAMD21G18A__) \
   || defined(ARDUINO_SAMD_CIRCUITPLAYGROUND_EXPRESS) || defined(__SAMD21E18A__) || defined(__SAMD51__) || defined(__SAMD51J20A__) || defined(__SAMD51J19A__) \
   || defined(__SAMD51G19A__) || defined(__SAMD51P19A__) || defined(__SAMD21G18A__) )
#if defined(ESP8266_AT_USE_SAMD)
  #undef ESP8266_AT_USE_SAMD
#endif
#define ESP8266_AT_USE_SAMD      true

#if(_ESP_AT_LOGLEVEL_> 3)
  #warning Use SAMD architecture from ESP8266_AT_WebServer
#endif

////////////////////////////////////////

#elif (defined(NRF52840_FEATHER) || defined(NRF52832_FEATHER) || defined(NRF52_SERIES) || defined(ARDUINO_NRF52_ADAFRUIT) || \
     defined(NRF52840_FEATHER_SENSE) || defined(NRF52840_ITSYBITSY) || defined(NRF52840_CIRCUITPLAY) || \
     defined(NRF52840_CLUE) || defined(NRF52840_METRO) || defined(NRF52840_PCA10056) || defined(PARTICLE_XENON) || \
     defined(MDBT50Q_RX) || defined(NINA_B302_ublox) || defined(NINA_B112_ublox) )
#if defined(ESP8266_AT_USE_NRF528XX)
  #undef ESP8266_AT_USE_NRF528XX
#endif
#define ESP8266_AT_USE_NRF528XX      true

#if(_ESP_AT_LOGLEVEL_> 3)
  #warning Use nFR52 architecture from ESP8266_AT_WebServer
#endif

#include <Adafruit_TinyUSB.h>

////////////////////////////////////////

#elif ( defined(ARDUINO_SAM_DUE) || defined(__SAM3X8E__) )
#if defined(ESP8266_AT_USE_SAM_DUE)
  #undef ESP8266_AT_USE_SAM_DUE
#endif
#define ESP8266_AT_USE_SAM_DUE      true
#warning Use SAM_DUE architecture

////////////////////////////////////////

#elif ( defined(STM32F0) || defined(STM32F1) || defined(STM32F2) || defined(STM32F3)  ||defined(STM32F4) || defined(STM32F7) || \
       defined(STM32L0) || defined(STM32L1) || defined(STM32L4) || defined(STM32H7)  ||defined(STM32G0) || defined(STM32G4) || \
       defined(STM32WB) || defined(STM32MP1) )

#if(_ESP_AT_LOGLEVEL_> 3)
  #warning STM32 board selected
#endif

#if defined(ESP8266_AT_USE_STM32)
  #undef ESP8266_AT_USE_STM32
#endif
#define ESP8266_AT_USE_STM32      true

////////////////////////////////////////

#elif ( defined(ARDUINO_NANO_RP2040_CONNECT) || defined(ARDUINO_ARCH_RP2040) || defined(ARDUINO_RASPBERRY_PI_PICO) || \
      defined(ARDUINO_GENERIC_RP2040) || defined(ARDUINO_ADAFRUIT_FEATHER_RP2040) )

#if(_ESP_AT_LOGLEVEL_> 3)
  #warning RP2040 board selected
#endif

#if defined(ESP8266_AT_USE_RP2040)
  #undef ESP8266_AT_USE_RP2040
#endif
#define ESP8266_AT_USE_RP2040      true

////////////////////////////////////////

#endif

////////////////////////////////////////

// To support lambda function in class
#include <functional-vlpp.h>
#include <ESP8266_AT.h>
#include "utility/mimetable.h"

////////////////////////////////////////

// KH, For PROGMEM commands
// ESP32/ESP8266 includes <pgmspace.h> by default, and memccpy_P was already defined there
#if !(ESP32 || ESP8266 || BOARD_SIPEED_MAIX_DUINO)
  #include <avr/pgmspace.h>
  #define memccpy_P(dest, src, c, n) memccpy((dest), (src), (c), (n))
#endif

////////////////////////////////////////

// Permit redefinition of SENDCONTENT_P_BUFFER_SZ in sketch, default is 4K, minimum is 256 bytes
#ifndef SENDCONTENT_P_BUFFER_SZ
  #define SENDCONTENT_P_BUFFER_SZ     4096

  #if(_ESP_AT_LOGLEVEL_> 3)
    #warning SENDCONTENT_P_BUFFER_SZ using default 4 Kbytes
  #endif
#else
  #if (SENDCONTENT_P_BUFFER_SZ < 256)
    #undef SENDCONTENT_P_BUFFER_SZ
    #define SENDCONTENT_P_BUFFER_SZ   256

    #if(_ESP_AT_LOGLEVEL_> 3)
      #warning SENDCONTENT_P_BUFFER_SZ reset to min 256 bytes
    #endif
  #endif
#endif

////////////////////////////////////////

#ifndef PGM_VOID_P
  #define PGM_VOID_P const void *
#endif

////////////////////////////////////////

enum HTTPMethod
{
  HTTP_ANY,
  HTTP_GET,
  HTTP_HEAD,
  HTTP_POST,
  HTTP_PUT,
  HTTP_PATCH,
  HTTP_DELETE,
  HTTP_OPTIONS
};

////////////////////////////////////////

enum HTTPUploadStatus
{
  UPLOAD_FILE_START,
  UPLOAD_FILE_WRITE,
  UPLOAD_FILE_END,
  UPLOAD_FILE_ABORTED
};

////////////////////////////////////////

enum HTTPClientStatus
{
  HC_NONE,
  HC_WAIT_READ,
  HC_WAIT_CLOSE
};

////////////////////////////////////////

enum HTTPAuthMethod
{
  BASIC_AUTH,
  DIGEST_AUTH
};

////////////////////////////////////////

#define HTTP_DOWNLOAD_UNIT_SIZE 1460

// Permit user to increase HTTP_UPLOAD_BUFLEN larger than default 2K
#if !defined(HTTP_UPLOAD_BUFLEN)
  #define HTTP_UPLOAD_BUFLEN 2048
#endif

#define HTTP_MAX_DATA_WAIT      5000 //ms to wait for the client to send the request
#define HTTP_MAX_POST_WAIT      5000 //ms to wait for POST data to arrive
#define HTTP_MAX_SEND_WAIT      5000 //ms to wait for data chunk to be ACKed
#define HTTP_MAX_CLOSE_WAIT     2000 //ms to wait for the client to close the connection

#define CONTENT_LENGTH_UNKNOWN  ((size_t) -1)
#define CONTENT_LENGTH_NOT_SET  ((size_t) -2)

/////////////////////////////////////////////////////////////////////////

#define RETURN_NEWLINE       "\r\n"

////////////////////////////////////////

#include <Arduino.h>

////////////////////////////////////////

#if ESP_AT_USE_AVR

// AVR has no string library
typedef String EWString;

// Do nothing as this is String
#define fromString
#define fromEWString

#else

#include <string>

typedef std::string EWString;

EWString fromString(const String& str)
{
  return str.c_str();
}

EWString fromString(const String&& str)
{
  return str.c_str();
}

String fromEWString(const EWString& str)
{
  return str.c_str();
}

String fromEWString(const EWString&& str)
{
  return str.c_str();
}

#endif

/////////////////////////////////////////////////////////////////////////

class ESP8266_AT_WebServer;

typedef struct
{
  HTTPUploadStatus status;
  String  filename;
  String  name;
  String  type;
  size_t  totalSize;      // file size
  size_t  currentSize;    // size of data currently in buf
  size_t  contentLength;  // size of entire post request, file size + headers and other request data.
  uint8_t buf[HTTP_UPLOAD_BUFLEN];
} HTTPUpload;

////////////////////////////////////////

#include "utility/RequestHandler.h"

////////////////////////////////////////

class ESP8266_AT_WebServer
{
  public:
    ESP8266_AT_WebServer(int port = 80);
    ~ESP8266_AT_WebServer();

    void begin();
    void handleClient();

    void close();
    void stop();

    bool authenticate(const char * username, const char * password);
    void requestAuthentication();

    typedef vl::Func<void(void)> THandlerFunction;
    //typedef std::function<void(void)> THandlerFunction;
    //typedef void (*THandlerFunction)(void);

    void on(const String &uri, THandlerFunction handler);
    void on(const String &uri, HTTPMethod method, THandlerFunction fn);
    void on(const String &uri, HTTPMethod method, THandlerFunction fn, THandlerFunction ufn);
    void addHandler(RequestHandler* handler);
    void onNotFound(THandlerFunction fn);  //called when handler is not assigned
    void onFileUpload(THandlerFunction fn); //handle file uploads

    ////////////////////////////////////////

    String uri()
    {
      return _currentUri;
    }

    ////////////////////////////////////////

    HTTPMethod method()
    {
      return _currentMethod;
    }

    ////////////////////////////////////////

    ESP8266_AT_Client client()
    {
      return _currentClient;
    }

    ////////////////////////////////////////

    HTTPUpload& upload()
    {
      return *_currentUpload;
    }

    ////////////////////////////////////////

    String arg(const String& name);         // get request argument value by name
    String arg(int i);                      // get request argument value by number
    String argName(int i);                  // get request argument name by number

    int args();                             // get arguments count
    bool hasArg(const String& name);        // check if argument exists
    void collectHeaders(const char* headerKeys[], const size_t headerKeysCount); // set the request headers to collect
    String header(const String& name);      // get request header value by name
    String header(int i);                   // get request header value by number
    String headerName(int i);               // get request header name by number
    int headers();                          // get header count
    bool hasHeader(const String& name);     // check if header exists

    String hostHeader();                    // get request host header if available or empty String if not

    // send response to the client
    // code - HTTP response code, can be 200 or 404
    // content_type - HTTP content type, like "text/plain" or "image/png"
    // content - actual content body
    void send(int code, const char* content_type = NULL, const String& content = String(""));
    void send(int code, char* content_type, const String& content);
    void send(int code, const String& content_type, const String& content);

    void send(int code, char*  content_type, const String& content, size_t contentLength);
    void send(int code, const char* content_type, const char* content);
    void send(int code, const char* content_type, const char* content, size_t contentLength);

    ////////////////////////////////////////

    inline void enableCORS(bool value = true)
    {
      _corsEnabled = value;
    }

    ////////////////////////////////////////

    inline void enableCrossOrigin(bool value = true)
    {
      enableCORS(value);
    }

    ////////////////////////////////////////

    void setContentLength(size_t contentLength);
    void sendHeader(const String& name, const String& value, bool first = false);
    void sendContent(const String& content);
    void sendContent(const String& content, size_t size);

    // KH, Restore PROGMEM commands
    void send_P(int code, PGM_P content_type, PGM_P content);
    void send_P(int code, PGM_P content_type, PGM_P content, size_t contentLength);

    void sendContent_P(PGM_P content);
    void sendContent_P(PGM_P content, size_t size);

    static String urlDecode(const String& text);

    ////////////////////////////////////////

    template<typename T> size_t streamFile(T &file, const String& contentType)
    {
      using namespace mime;
      setContentLength(file.size());

      if (String(file.name()).endsWith(mimeTable[gz].endsWith) && contentType != mimeTable[gz].mimeType
          && contentType != mimeTable[none].mimeType)
      {
        sendHeader("Content-Encoding", "gzip");
      }

      send(200, contentType, "");

      return _currentClient.write(file);
    }

    ////////////////////////////////////////

  protected:
    void _addRequestHandler(RequestHandler* handler);
    void _handleRequest();
    void _finalizeResponse();
    bool _parseRequest(ESP8266_AT_Client& client);

    void _parseArguments(const String& data);
    int  _parseArgumentsPrivate(const String& data,
                                vl::Func<void(String&, String&, const String&, int, int, int, int)> handler);
    bool _parseForm(ESP8266_AT_Client& client, const String& boundary, uint32_t len);

    static String _responseCodeToString(int code);
    bool _parseFormUploadAborted();
    void _uploadWriteByte(uint8_t b);
    uint8_t _uploadReadByte(ESP8266_AT_Client& client);
    void _prepareHeader(String& response, int code, const char* content_type, size_t contentLength);

#if !ESP_AT_USE_AVR
    void _prepareHeader(EWString& response, int code, const char* content_type, size_t contentLength);
#endif

    bool _collectHeader(const char* headerName, const char* headerValue);

    ////////////////////////////////////////

    struct RequestArgument
    {
      String key;
      String value;
    };

    ////////////////////////////////////////

    bool              _corsEnabled;

    ESP8266_AT_Server  _server;

    ESP8266_AT_Client  _currentClient;
    HTTPMethod          _currentMethod;
    String              _currentUri;
    uint8_t             _currentVersion;
    HTTPClientStatus    _currentStatus;
    unsigned long       _statusChange;

    RequestHandler*   _currentHandler   = nullptr;
    RequestHandler*   _firstHandler     = nullptr;
    RequestHandler*   _lastHandler      = nullptr;
    THandlerFunction    _notFoundHandler;
    THandlerFunction    _fileUploadHandler;

    int                 _currentArgCount;
    RequestArgument*  _currentArgs      = nullptr;

    HTTPUpload*       _currentUpload    = nullptr;
    int               _postArgsLen;
    RequestArgument*  _postArgs         = nullptr;

    int              _headerKeysCount;
    RequestArgument*  _currentHeaders   = nullptr;
    size_t           _contentLength;
    String           _responseHeaders;

    String           _hostHeader;
    bool             _chunked;
};

////////////////////////////////////////

#include "ESP8266_AT_WebServer-impl.h"
#include "Parsing-impl.h"

#endif //ESP8266_AT_WebServer_h 
