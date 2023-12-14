#include "fsm.h"
#include "stdio.h"

// Buffer to store data received from UART
uint8_t buffer[MAX_BUFFER_SIZE];
uint8_t index_buffer;

// ADC value and string to be transmitted via UART
uint32_t ADC_value;
uint8_t str[30];

// Buffer index and buffer flag variables
uint8_t index_buffer = 0;
uint8_t buffer_flag = 0;

// Status of command processing and UART communication state machine
int comStatus = WAIT;
int uartStatus = WAIT_SEND;

// Function to return a pointer to the buffer to retrieve command from UART
char* get_command() {
    return (char*) buffer;
}

// Function to clear the entire buffer
void clr_command() {
    for (int i = 0; i < MAX_BUFFER_SIZE; i++) {
        buffer[i] = '\0';
    }
    index_buffer = 0;
}

// Callback function called when a character is received from UART
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART2) {
        buffer_flag = 1;
        HAL_UART_Transmit(&huart2, &temp, 1, 50);
        HAL_UART_Receive_IT(&huart2, &temp, 1);
    }
}

// State machine for processing commands from UART
void command_parser_fsm() {
    switch (comStatus) {
        case WAIT:
            if (temp == '!') {
                comStatus = COMMAND;
            }
            break;
        case COMMAND:
            if (temp == '#') {
                comStatus = WAIT;
                uartStatus = RESPONE;
            } else {
                buffer[index_buffer++] = temp;
                if (index_buffer >= MAX_BUFFER_SIZE) {
                    comStatus = WAIT;
                    clr_command();
                }
            }
            break;
        default:
            break;
    }
}

// State machine for UART data transmission
void uart_communication_fsm() {
    switch (uartStatus) {
        case WAIT_SEND:
            break;
        case RESPONE:
            if (strcmp(get_command(), "RST") == 0) {
                ADC_value = HAL_ADC_GetValue(&hadc1);
                HAL_UART_Transmit(&huart2, (void *)str,  sprintf(str, "\r\n!ADC=%d#", ADC_value), 1000);
                uartStatus = REPEAT;
                clr_command();
                setTimer0(300);
            } else {
                uartStatus = WAIT_SEND;
            }
            clr_command();
            break;
        case REPEAT:
            if (timer0_flag == 1) {
                ADC_value = HAL_ADC_GetValue(&hadc1);
                HAL_UART_Transmit(&huart2, (void*) str,  sprintf(str, "\r\n!ADC=%d#", ADC_value), 1000);
                setTimer0(300);
            }
            if (strcmp(get_command(), "OK") == 0) {
                uartStatus = WAIT_SEND;
            }
            break;
        default:
            break;
    }
}
