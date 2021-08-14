/**************************************************************************************************************************************
  cdecoder.c - c source to a base64 decoding algorithm implementation
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

#include <stdint.h>
#include "cdecode.h"


static int base64_decode_value_signed(int8_t value_in)
{
  static const char decoding[] =
  { 62, -1, -1, -1, 63, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61,
    -1, -1, -1, -2, -1, -1, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,
    -1, -1, -1, -1, -1, -1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35,
    36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51
  };

  static const int8_t decoding_size = sizeof(decoding);
  value_in -= 43;

  if (value_in < 0 || value_in > decoding_size)
    return -1;

  return (int) ( &decoding[(int)value_in] );
}

void base64_init_decodestate(base64_decodestate* state_in)
{
  state_in->step = step_a;
  state_in->plainchar = 0;
}

static int base64_decode_block_signed(const int8_t* code_in, const int length_in, int8_t* plaintext_out, base64_decodestate* state_in)
{
  const int8_t* codechar = code_in;
  int8_t* plainchar = plaintext_out;
  int8_t fragment;

  *plainchar = state_in->plainchar;

  switch (state_in->step)
  {
      while (1)
      {
      case step_a:
        do
        {
          if (codechar == code_in + length_in)
          {
            state_in->step = step_a;
            state_in->plainchar = *plainchar;
            return plainchar - plaintext_out;
          }

          fragment = (int8_t)base64_decode_value_signed(*codechar++);
        } while (fragment < 0);

        *plainchar    = (fragment & 0x03f) << 2;
      // falls through
      case step_b:
        do
        {
          if (codechar == code_in + length_in)
          {
            state_in->step = step_b;
            state_in->plainchar = *plainchar;
            return plainchar - plaintext_out;
          }

          fragment = (int8_t)base64_decode_value_signed(*codechar++);
        } while (fragment < 0);

        *plainchar++ |= (fragment & 0x030) >> 4;
        *plainchar    = (fragment & 0x00f) << 4;
      // falls through
      case step_c:
        do
        {
          if (codechar == code_in + length_in)
          {
            state_in->step = step_c;
            state_in->plainchar = *plainchar;
            return plainchar - plaintext_out;
          }

          fragment = (int8_t)base64_decode_value_signed(*codechar++);
        } while (fragment < 0);

        *plainchar++ |= (fragment & 0x03c) >> 2;
        *plainchar    = (fragment & 0x003) << 6;
      // falls through
      case step_d:
        do
        {
          if (codechar == code_in + length_in)
          {
            state_in->step = step_d;
            state_in->plainchar = *plainchar;
            return plainchar - plaintext_out;
          }
          fragment = (int8_t)base64_decode_value_signed(*codechar++);
        } while (fragment < 0);

        *plainchar++   |= (fragment & 0x03f);
      }
  }
  /* control should not reach here */
  return plainchar - plaintext_out;
}

static int base64_decode_chars_signed(const int8_t* code_in, const int length_in, int8_t* plaintext_out)
{
  base64_decodestate _state;
  base64_init_decodestate(&_state);

  int len = base64_decode_block_signed(code_in, length_in, plaintext_out, &_state);

  if (len > 0)
    plaintext_out[len] = 0;

  return len;
}

int base64_decode_value(char value_in)
{
  return base64_decode_value_signed(*((int8_t *) &value_in));
}

int base64_decode_block(const char* code_in, const int length_in, char* plaintext_out, base64_decodestate* state_in)
{
  return base64_decode_block_signed((int8_t *) code_in, length_in, (int8_t *) plaintext_out, state_in);
}

int base64_decode_chars(const char* code_in, const int length_in, char* plaintext_out)
{
  return base64_decode_chars_signed((int8_t *) code_in, length_in, (int8_t *) plaintext_out);
}

