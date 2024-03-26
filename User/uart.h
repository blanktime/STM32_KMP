#ifndef __UART_H
#define __UART_H

#include "stm32f4xx.h"
#include  "stdio.h"

void Uart_Init(u32 bround);


extern uint8_t RX_BUFF[64];

extern uint8_t RX_NUM;

#endif



