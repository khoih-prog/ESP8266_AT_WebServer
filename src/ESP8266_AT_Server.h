/****************************************************************************************************************************
 * ESP8266_AT_Server.h - Dead simple web-server.
 * For ESP8266 AT-command running shields
 *
 * ESP8266_AT_WebServer is a library for the ESP8266 AT-command shields to run WebServer
 *
 * Forked and modified from Arduino ESP8266_AT library
 * Built by Khoi Hoang https://github.com/khoih-prog/ESP8266_AT_WebServer
 * Licensed under MIT license
 * Version: 1.0.3
 *
 * Version Modified By   Date      Comments
 * ------- -----------  ---------- -----------
 *  1.0.0   K Hoang      12/02/2020 Initial coding for Arduino Mega, Teensy, etc
 *  1.0.1   K Hoang      17/02/2020 Add support to server's lambda function calls
 *  1.0.2   K Hoang      22/02/2020 Add support to SAMD (DUE, ZERO, MKR, NANO_33_IOT, M0, M0 Pro, AdaFruit, etc) boards
 *  1.0.3   K Hoang      03/03/2020 Add support to STM32 (STM32,F0,F1, F2, F3, F4, F7, etc) boards
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
