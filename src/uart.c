#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 
#include <fcntl.h>  
#include <termios.h>


int init_uart() {
    int uart_filestream = -1;
    struct termios options;

    uart_filestream = open("/dev/serial0", O_RDWR | O_NOCTTY | O_NDELAY);  

    if (uart_filestream == -1) {
        printf("Erro de inicialização\n");
        exit(1);
    }
    else printf("UART inicializada\n");

    tcgetattr(uart_filestream, &options);
    options.c_cflag = B9600 | CS8 | CLOCAL | CREAD;
    options.c_iflag = IGNPAR;
    options.c_oflag = 0;
    options.c_lflag = 0;
    tcflush(uart_filestream, TCIFLUSH);
    tcsetattr(uart_filestream, TCSANOW, &options);

    return uart_filestream;
}


void close_uart(int uart_filestream) {
    close(uart_filestream);
}
