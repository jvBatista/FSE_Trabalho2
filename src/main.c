#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include "bme280.h"
#include "uart.h"
#include "modbus.h"
#include "utils.h"
#include "pid.h"
#include "pwm.h"
#include "csv.h"

int main()
{
    printf("FSE Trabalho 2\n");
    
    int uart_filestream;
    float external_temp = 0, internal_temp = 0, target_temp = 0;
    float prev_external_temp = 0, prev_internal_temp = 0, prev_target_temp = 0;
    int user_action = -1;
    int fan_value, resistor_value;
    int temperature_source = 0; // 1 = teclado, 2 = potenciomentro, 3 = curva de temperatura
    double pid_value = 0;
    int reflow_times[10], reflow_temps[10], current_reflow_pos = -1;
    float reflow_timer_count = 0.0;
    float kp, ki, kd;

    uart_filestream = init_uart();
    signal(SIGINT, close_app);
    init_csv();

    printf("Selecione o modo de controle da temperatura de referência:\n\n1-Teclado\n2-Potenciometro\n3-Curva de temperatura\n\n");
    scanf("%d", &temperature_source);

    switch (temperature_source) {
        case 1:
            printf("Digite a temperatura desejada:\n");
            scanf("%f", &target_temp);
            set_target_temperature(uart_filestream, target_temp);
            set_temperature_source(uart_filestream, TERMINAL_SOURCE);
            break;
        case 2:
            set_temperature_source(uart_filestream, POTENTIOMETER_SOURCE);
            break;
        case 3:
            set_temperature_source(uart_filestream, TERMINAL_SOURCE);
            read_reflow_csv(reflow_times, reflow_temps, 10);
            break;
        
        default:
            printf("\n\nOpção invalida\n\n");
            break;
    }

    printf("\n\n Escolha as constantes a serem usadas:\n\n");
    printf("Insira Kp:\n");
    scanf("%f", &kp);

    printf("Insira Ki:\n");
    scanf("%f", &ki);

    printf("Insira Kd:\n");
    scanf("%f", &kd);

    pid_configura_constantes(kp, ki, kd);

    switch_system(uart_filestream, ON);

    while (1)
    {
        internal_temp = get_internal_temperature(uart_filestream);
        internal_temp == -1.0 ? (internal_temp = prev_internal_temp) : (prev_internal_temp = internal_temp);

        switch (temperature_source) {
            case 1:
            set_target_temperature(uart_filestream, target_temp);
            break;

            case 2:
            target_temp = get_target_temperature(uart_filestream);
            target_temp == -1.0 ? (target_temp = prev_target_temp) : (prev_target_temp = target_temp);
            break;
            
            default:
            reflow_timer_count+=1.3;
            if(current_reflow_pos != (int)(reflow_timer_count/60)){
                current_reflow_pos = (int)(reflow_timer_count/60);
                target_temp = reflow_temps[current_reflow_pos];
                set_target_temperature(uart_filestream, target_temp);
            }
            break;
        }

        get_external_temp(&external_temp);
        external_temp == -1.0 ? (external_temp = prev_external_temp) : (prev_external_temp = external_temp);

        user_action = get_user_action(uart_filestream);
        switch (user_action) {
            case 1:
            printf("Ligando sistema...\n\n");
            send_request(uart_filestream, SEND_CODE, SYSTEM_STATE_CODE, 1, 1, INT_TYPE);
            switch_system(uart_filestream, ON);
            break;

            case 2:
            close_app(uart_filestream);
            break;

            case 3:
            printf("Controle via potenciometro...\n\n");
            temperature_source = 2;
            set_temperature_source(uart_filestream, POTENTIOMETER_SOURCE);
            break;

            case 4:
            printf("Controle via curva...\n\n");
            temperature_source = 3;
            set_temperature_source(uart_filestream, TERMINAL_SOURCE);
            read_reflow_csv(reflow_times, reflow_temps, 10);
            reflow_timer_count = 0;
            break;
            
            default:
            break;
        }

        // pid calc 
        pid_atualiza_referencia(target_temp);
        pid_value = pid_controle(internal_temp);

        if(pid_value > 0){
            update_pin(RESISTOR_PIN, (int)pid_value);
            resistor_value = (int)pid_value;

            update_pin(FAN_PIN, 0);
            fan_value = 0;
        }else{
            if(pid_value < -40) {
                update_pin(FAN_PIN, (int)(-1.0*pid_value));
                fan_value = (int)(-1.0*pid_value);
            }
            else {
                update_pin(FAN_PIN, 40);
                fan_value = 40;
            }

            update_pin(RESISTOR_PIN, 0);
            resistor_value = 0;
        }

        send_request(uart_filestream, SEND_CODE, CONTROL_SIGNAL_CODE, (int)pid_value, 4, INT_TYPE);  

        write_on_csv(internal_temp, target_temp, external_temp, target_temp, fan_value, resistor_value, pid_value);

        // debug
        printf("Modo atual: %d | TI: %.2f | TR: %.2f | TE: %.2f | PID: %.2f\n\n", temperature_source, internal_temp, target_temp, external_temp, pid_value); // debug

        sleep(1);
    }

    return 0;
}