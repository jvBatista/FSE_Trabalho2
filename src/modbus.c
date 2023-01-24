#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "crc16.h"
#include "uart.h"

void send_request(int uart_filestream, unsigned char code, unsigned char sub_code, float data, int data_size, char type) {
  unsigned char message[200], *msg_ptr = message;
  int msg_size = 7, int_message = 0;

  *msg_ptr++ = 0x01;
  *msg_ptr++ = code;
  *msg_ptr++ = sub_code;
  *msg_ptr++ = 0x09;
  *msg_ptr++ = 0x09;
  *msg_ptr++ = 0x06;
  *msg_ptr++ = 0x03;

  if (data != -1) {
    if (type == 'i') {
      int_message = (int)data;
      memcpy(&message[msg_size], &int_message, data_size);
    }
    else memcpy(&message[msg_size], &data, data_size);

    msg_size += data_size;
  }

  short crc = calcula_CRC(message, msg_size);

  memcpy(&message[msg_size], &crc, 2);
  write_uart_message(uart_filestream, message, msg_size + 2);
}

float get_response(int uart_filestream, char type) {
  unsigned char response[256];
  int response_length;
  short expected_crc, recieved_crc;
  float float_data;
  int int_data = -1;

  response_length = read_uart_message(uart_filestream, response, 10);

  if (response_length < 0) return -1.0;

  memcpy(&recieved_crc, &response[7], 2);
  expected_crc = calcula_CRC(response, 7);

  if (recieved_crc != expected_crc) float_data = -1.0; // CRCs diferentes
  else if (response_length != 9) float_data = -1.0; // tamanho mensagem incorreto
  else {
    if (type == 'i') {
      memcpy(&int_data, &response[3], 4);
      return (int)int_data;
    }

    memcpy(&float_data, &response[3], 4);
  }

  return float_data;
}