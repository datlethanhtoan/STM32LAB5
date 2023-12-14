/*
 * fsm.h
 *
 *  Created on: Nov 29, 2023
 *      Author: ADMIN
 */

#ifndef INC_FSM_H_
#define INC_FSM_H_

#include "main.h"
#include "string.h"
#include "software_timer.h"

#define MAX_BUFFER_SIZE  30

#define WAIT	1
#define COMMAND	2

#define WAIT_SEND	10
#define RESPONE		11
#define REPEAT		12

ADC_HandleTypeDef hadc1;
UART_HandleTypeDef huart2;

uint8_t temp;
uint8_t buffer_flag;

extern int comStatus;
extern int uartStatus;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
void command_parser_fsm();
void uart_communication_fsm();

#endif /* INC_FSM_H_ */
