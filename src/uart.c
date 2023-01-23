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

int read_uart_message(int uart_filestream, unsigned char* message, int msg_size) {
    usleep(100000);
    int length = -1;

    if (uart_filestream != -1) {
        // Read up to 255 characters from the port if they are there
        length = read(uart_filestream, (void*)message, msg_size);      //Filestream, buffer to store in, number of bytes to read (max)

        if (length < 0) printf("Erro de leitura.\n"); //An error occured (will occur if there are no bytes)
        else if (length == 0) printf("Nenhum dado disponível.\n"); //No data waiting
        else message[msg_size] = '\0';
    }

    return length;
}

int write_uart_message(int uart_filestream, unsigned char *message, int msg_size) {
    if (uart_filestream != -1) {
        int length = write(uart_filestream, message, msg_size);

        if (length < 0) {
            printf("Erro de transmissão\n");
            return -1;
        }
    }

    return 0;
}
