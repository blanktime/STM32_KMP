#ifndef DELAY_H_
#define DELAY_H_

#include "stm32f4xx.h"

void delay_init(void);      //��ʱģ���ʼ������
void delay_us(__IO u32 count);      //΢����ʱ����
void delay_ms(__IO u32 count);      //������ʱ����
void delay_s(u32 s);        //����ʱ����

#endif
