/****************************************************************************************************************************
 * ESP8266_AT_Server.h - Dead simple web-server.
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

#ifndef ESP8266_AT_Server_h
#define ESP8266_AT_Server_h

#include <Server.h>

#include "ESP8266_AT.h"


class ESP8266_AT_Client;

class ESP8266_AT_Server : public Server
{

public:
	ESP8266_AT_Server(uint16_t port);


	/*
	* Gets a client that is connected to the server and has data available for reading.
	* The connection persists when the returned client object goes out of scope; you can close it by calling client.stop().
	* Returns a Client object; if no Client has data available for reading, this object will evaluate to false in an if-statement.
	*/
	ESP8266_AT_Client available(uint8_t* status = NULL);

	/*
	* Start the TCP server
	*/
	void begin();

	virtual size_t write(uint8_t);
	virtual size_t write(const uint8_t *buf, size_t size);

	uint8_t status();

	using Print::write;


private:
	uint16_t _port;
	uint8_t _sock;
	bool _started;

};

#endif
