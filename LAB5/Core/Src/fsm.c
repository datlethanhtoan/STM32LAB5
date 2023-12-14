/*
 * fsm.c
 *
 *  Created on: Nov 29, 2023
 *      Author: ADMIN
 */

#include "fsm.h"
#include "stdio.h"

uint8_t buffer[MAX_BUFFER_SIZE];
uint8_t index_buffer;

uint32_t ADC_value;
uint8_t str[30];

uint8_t index_buffer = 0;
uint8_t buffer_flag = 0;

int comStatus = WAIT;
int uartStatus = WAIT_SEND;

char* get_command(){
	return (char*) buffer;
}
void clr_command(){
	for (int i = 0; i < MAX_BUFFER_SIZE; i++){
		buffer[i] = '\0';
	}
	index_buffer = 0;
}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	if(huart->Instance == USART2){
		buffer_flag = 1;
		HAL_UART_Transmit(&huart2 , &temp, 1, 50);
		HAL_UART_Receive_IT(&huart2, &temp, 1);
	}
}
void command_parser_fsm(){
	switch (comStatus) {
		case WAIT:
			if (temp == '!'){
				comStatus = COMMAND;
			}
			break;
		case COMMAND:
			if (temp == '#'){
				comStatus = WAIT;
				uartStatus = RESPONE;
			}
			else {
				buffer[index_buffer++] = temp;
				if (index_buffer >= MAX_BUFFER_SIZE){
					comStatus = WAIT;
					clr_command();
				}
			}
			break;
		default:
			break;
	}
}
void uart_communication_fsm(){
	switch (uartStatus) {
		case WAIT_SEND:
			break;
		case RESPONE:
			if (strcmp(get_command(), "RST") == 0){
				ADC_value = HAL_ADC_GetValue(&hadc1);
				HAL_UART_Transmit(&huart2, (void *)str,  sprintf(str, "\r\n!ADC=%d#", ADC_value), 1000);
				uartStatus = REPEAT;
				clr_command();
				setTimer0(300);
			}
			else {
				uartStatus = WAIT_SEND;
			}
			clr_command();
			break;
		case REPEAT:
			if (timer0_flag == 1){
				ADC_value = HAL_ADC_GetValue(&hadc1);
				HAL_UART_Transmit(&huart2, (void*) str,  sprintf(str, "\r\n!ADC=%d#", ADC_value), 1000);
				setTimer0(300);
			}
			if (strcmp(get_command(), "OK") == 0){
				uartStatus = WAIT_SEND;
			}
			break;
		default:
			break;
	}
}
