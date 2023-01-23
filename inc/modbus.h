#ifndef MODBUS_H_
#define MODBUS_H_

void send_request(int uart_filestream, unsigned char code, unsigned char sub_code, float data, int data_size, char type);
float get_response(int uart_filestream, char type);

#endif