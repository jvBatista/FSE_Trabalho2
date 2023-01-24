#ifndef UTILS_
#define UTILS_

#define ON 1
#define OFF 0
#define POTENTIOMETER_SOURCE 0
#define TERMINAL_SOURCE 1

#define REQUEST_CODE 0x23
#define SEND_CODE 0x16

#define INTERNAL_TEMP_CODE 0xC1
#define POTENTIOMETER_TEMP_CODE 0xC2
#define USER_ACTION_CODE 0xC3

#define CONTROL_SIGNAL_CODE 0xD1
#define TR_CMD_SOURCE 0xD2
#define SET_RT_CODE 0xD2
#define SYSTEM_STATE_CODE 0xD3
#define TEMP_SOURCE_CODE 0xD4
#define WORKING_STATE_CODE 0xD5
#define ROOM_TEMP_CODE 0xD6

#define MAX_TRIES 5
#define NO_DATA_FLAG -1
#define INT_TYPE 'i'
#define FLOAT_TYPE 'f'
#define RESISTOR_PIN 4 // GPIO 23
#define FAN_PIN 5 // GPIO 24

int switch_system(int uart_filestream, int system_state);
int send_working_state(int uart_filestream, int state);
int set_temperature_source(int uart_filestream, int temp_source);
void set_target_temperature(int uart_filestream, int target_temperature);
int send_external_temperature(int uart_filestream, float external_temp);
float get_internal_temperature(int uart_filestream);
float get_target_temperature(int uart_filestream);
int get_user_action(int uart_filestream);
void close_app(int uart_filestream);

#endif