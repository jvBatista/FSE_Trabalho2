#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "bme280.h"

int main()
{
    printf("FSE Trabalho 2\n");
    float external_temp = 0;

    while (1)
    {
        getBMETemperature(&external_temp);
        if (external_temp == -1.0) external_temp = 0;

        printf("Temp Ext: %.2f\n\n", external_temp);

        sleep(1);
    }

    return 0;
}