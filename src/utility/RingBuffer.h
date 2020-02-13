/****************************************************************************************************************************
 * RingBuffer.h - Dead simple web-server.
 * For ESP8266 AT-command running shields
 *
 * ESP_AT_WebServer is a library for the ESP8266 AT-command shields to run WebServer
 *
 * Forked and modified from Arduino ESP_AT library
 * Built by Khoi Hoang https://github.com/khoih-prog/ESP_AT_WebServer
 * Licensed under MIT license
 * Version: 1.0.0
 *
 *
 * Version Modified By   Date      Comments
 * ------- -----------  ---------- -----------
 *  1.0.0   K Hoang      12/02/2020 Initial coding for Arduino Mega, Teensy, etc
 *****************************************************************************************************************************/

#ifndef RingBuffer_h
#define RingBuffer_h


class RingBuffer
{
public:
	RingBuffer(unsigned int size);
	~RingBuffer();

	void reset();
	void init();
	void push(char c);
	int getPos();
	bool endsWith(const char* str);
	void getStr(char * destination, unsigned int skipChars);
	void getStrN(char * destination, unsigned int skipChars, unsigned int num);


private:

	unsigned int _size;
	char* ringBuf;
	char* ringBufEnd;
	char* ringBufP;

};

#endif
