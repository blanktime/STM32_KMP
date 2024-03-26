#ifndef DELAY_H_
#define DELAY_H_

#include "stm32f4xx.h"

void delay_init(void);      //延时模块初始化函数
void delay_us(__IO u32 count);      //微秒延时函数
void delay_ms(__IO u32 count);      //毫秒延时函数
void delay_s(u32 s);        //秒延时函数

#endif
