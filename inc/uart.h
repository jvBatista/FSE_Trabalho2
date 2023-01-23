#ifndef UART_H_
#define UART_H_

int init_uart();
void close_uart(int uart_filestream);
int read_uart_message(int uart_filestream, unsigned char* message, int msg_size);
int write_uart_message(int uart_filestream, unsigned char *message, int msg_size);

#endif