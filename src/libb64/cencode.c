/**************************************************************************************************************************************
  cencoder.c - c source to a base64 decoding algorithm implementation
  For ESP8266/ESP32 AT-command running shields

  ESP8266_AT_WebServer is a library for the ESP8266/ESP32 AT-command shields to run WebServer
  Based on and modified from ESP8266 https://github.com/esp8266/Arduino/releases
  Built by Khoi Hoang https://github.com/khoih-prog/ESP8266_AT_WebServer
  Licensed under MIT license

  Original author:
  @file       Esp8266WebServer.h
  @author     Ivan Grokhotkov

  Version: 1.4.0

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
  1.4.0   K Hoang      14/08/2021 Add support to Adafruit nRF52 core v0.22.0+
 ***************************************************************************************************************************************/

#include "cencode.h"

void base64_init_encodestate(base64_encodestate* state_in)
{
  state_in->step = step_A;
  state_in->result = 0;
  state_in->stepcount = 0;
  state_in->stepsnewline = BASE64_CHARS_PER_LINE;
}


void base64_init_encodestate_nonewlines(base64_encodestate* state_in)
{
  base64_init_encodestate(state_in);
  state_in->stepsnewline = -1;
}

char base64_encode_value(const char n)
{
  char r;

  if (n < 26)
    r = n + 'A';
  else if (n < 26 + 26)
    r = n - 26 + 'a';
  else if (n < 26 + 26 + 10 )
    r = n - 26 - 26 + '0';
  else if (n == 62 )
    r = '+';
  else
    r = '/';
  return r;
}

int base64_encode_block(const char* plaintext_in, int length_in, char* code_out, base64_encodestate* state_in)
{
  const char* plainchar = plaintext_in;
  const char* const plaintextend = plaintext_in + length_in;
  char* codechar = code_out;
  char result;
  char fragment;

  result = state_in->result;

  switch (state_in->step)
  {
      while (1)
      {
      case step_A:
        if (plainchar == plaintextend)
        {
          state_in->result = result;
          state_in->step = step_A;
          return codechar - code_out;
        }

        fragment = *plainchar++;
        result = (fragment & 0x0fc) >> 2;
        *codechar++ = base64_encode_value(result);
        result = (fragment & 0x003) << 4;
      // falls through
      case step_B:
        if (plainchar == plaintextend)
        {
          state_in->result = result;
          state_in->step = step_B;
          return codechar - code_out;
        }

        fragment = *plainchar++;
        result |= (fragment & 0x0f0) >> 4;
        *codechar++ = base64_encode_value(result);
        result = (fragment & 0x00f) << 2;
      // falls through
      case step_C:
        if (plainchar == plaintextend)
        {
          state_in->result = result;
          state_in->step = step_C;
          return codechar - code_out;
        }

        fragment = *plainchar++;
        result |= (fragment & 0x0c0) >> 6;
        *codechar++ = base64_encode_value(result);
        result  = (fragment & 0x03f) >> 0;
        *codechar++ = base64_encode_value(result);

        ++(state_in->stepcount);

        if ((state_in->stepcount == BASE64_CHARS_PER_LINE / 4) && (state_in->stepsnewline > 0))
        {
          *codechar++ = '\n';
          state_in->stepcount = 0;
        }
      }
  }

  /* control should not reach here */
  return codechar - code_out;
}

int base64_encode_blockend(char* code_out, base64_encodestate* state_in)
{
  char* codechar = code_out;

  switch (state_in->step)
  {
    case step_B:
      *codechar++ = base64_encode_value(state_in->result);
      *codechar++ = '=';
      *codechar++ = '=';
      break;
    case step_C:
      *codechar++ = base64_encode_value(state_in->result);
      *codechar++ = '=';
      break;
    case step_A:
      break;
  }

  *codechar = 0x00;

  return codechar - code_out;
}

int base64_encode_chars(const char* plaintext_in, int length_in, char* code_out)
{
  base64_encodestate _state;
  base64_init_encodestate(&_state);

  int len = base64_encode_block(plaintext_in, length_in, code_out, &_state);

  return len + base64_encode_blockend((code_out + len), &_state);
}

