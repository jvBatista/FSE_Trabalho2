#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <softPwm.h>

void update_pin(int pin, int value) {
    int status = -1;
    do {
        status = wiringPiSetup();
    } while (status == -1);

    pinMode(pin, OUTPUT);       /* set GPIO as output */
    softPwmCreate(pin, 1, 100); /* set PWM channel along with range*/
    softPwmWrite(pin, value);   /* change the value of PWM */
    delay(10);
}