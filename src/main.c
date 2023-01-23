#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include "bme280.h"
#include "uart.h"

void close_app(int uart_filestream){
  close_uart(uart_filestream);
  printf("Sistema encerrado\n");
  exit(0);
}

int main()
{
    printf("FSE Trabalho 2\n");
    float external_temp = 0;
    int uart_filestream;

    uart_filestream = init_uart();
    signal(SIGINT, close_app);

    while (1)
    {
        get_external_temp(&external_temp);
        if (external_temp == -1.0) external_temp = 0;

        printf("Temp Ext: %.2f\n\n", external_temp);

        sleep(1);
    }

    return 0;
}