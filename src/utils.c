#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "modbus.h"
#include "uart.h"
#include "pwm.h"

void close_app(int uart_filestream){
  printf("Encerrando...\n\n");

  send_request(uart_filestream, SEND_CODE, SYSTEM_STATE_CODE, 0, 1, INT_TYPE);
  send_request(uart_filestream, SEND_CODE, CONTROL_SIGNAL_CODE, 0, 4, INT_TYPE);  

  update_pin(FAN_PIN, 0);
  update_pin(RESISTOR_PIN, 0);

  close_uart(uart_filestream);
  printf("Sistema encerrado\n");

  exit(0);
}

int switch_system(int uart_filestream, int state) {
  int response = -1, tries_num = 0;
  send_request(uart_filestream, SEND_CODE, SYSTEM_STATE_CODE, state, 1, INT_TYPE);

  do {
    response = get_response(uart_filestream, INT_TYPE);
    tries_num++;
  } while (response == -1.0 && tries_num >= MAX_TRIES);

  return response;
}

int set_temperature_source(int uart_filestream, int temp_source) {
  int response = -1, tries_num = 0;
  send_request(uart_filestream, SEND_CODE, TEMP_SOURCE_CODE, temp_source, 1, INT_TYPE);

  do {
    response = get_response(uart_filestream, INT_TYPE);
    tries_num++;
  } while (response == -1.0 && tries_num >= MAX_TRIES);

  return response;
}

void set_target_temperature(int uart_filestream, int target_temperature) {
  send_request(uart_filestream, SEND_CODE, SET_RT_CODE, target_temperature, 4, FLOAT_TYPE);
}


float get_internal_temperature(int uart_filestream) {
  float internal_temp = 0.0, tries_num = 0;
  send_request(uart_filestream, REQUEST_CODE, INTERNAL_TEMP_CODE, NO_DATA_FLAG, 0, FLOAT_TYPE);

  do {
    internal_temp = get_response(uart_filestream, FLOAT_TYPE);
    tries_num++;
  } while (internal_temp == -1.0 && tries_num >= MAX_TRIES);

  return internal_temp;
}

float get_target_temperature(int uart_filestream) {
  float target_temp = 0.0, tries_num = 0;
  send_request(uart_filestream, REQUEST_CODE, POTENTIOMETER_TEMP_CODE, NO_DATA_FLAG, 0, FLOAT_TYPE);

  do {
    target_temp = get_response(uart_filestream, FLOAT_TYPE);
    tries_num++;
  } while (target_temp == -1.0 && tries_num >= MAX_TRIES);

  return target_temp;
}

int get_user_action(int uart_filestream) {
  int action = -1, tries_num = 0;
  send_request(uart_filestream, REQUEST_CODE, USER_ACTION_CODE, NO_DATA_FLAG, 0, FLOAT_TYPE);

  do {
    action = (int)get_response(uart_filestream, INT_TYPE);
    tries_num++;
  } while (action == -1 && tries_num <= MAX_TRIES);

  return action;
}