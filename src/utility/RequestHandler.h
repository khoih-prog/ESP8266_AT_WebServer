/****************************************************************************************************************************
 * RequestHandler.h - Dead simple web-server.
 * For ESP8266 AT-command running shields
 *
 * ESP8266_AT_WebServer is a library for the ESP8266 AT-command shields to run WebServer
 *
 * Forked and modified from Arduino ESP8266_AT library
 * Built by Khoi Hoang https://github.com/khoih-prog/ESP8266_AT_WebServer
 * Licensed under MIT license
 * Version: 1.0.0
 *
 *
 * Version Modified By   Date      Comments
 * ------- -----------  ---------- -----------
 *  1.0.0   K Hoang      12/02/2020 Initial coding for Arduino Mega, Teensy, etc
 *****************************************************************************************************************************/

#ifndef RequestHandler_h
#define RequestHandler_h

class RequestHandler 
{
public:
    virtual ~RequestHandler() { }
    virtual bool canHandle(HTTPMethod method, String uri) { return false; }
    virtual bool canUpload(String uri) { return false; }
    virtual bool handle(ESP8266_AT_WebServer& server, HTTPMethod requestMethod, String requestUri) { return false; }
    virtual void upload(ESP8266_AT_WebServer& server, String requestUri, HTTPUpload& upload) {}

    RequestHandler* next() { return _next; }
    void next(RequestHandler* r) { _next = r; }

private:
    RequestHandler* _next = nullptr;
};

#endif //REQUESTHANDLER_H
