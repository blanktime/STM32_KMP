#include "delay.h"

static u8 fac_us = 0;
static u16 fac_ms = 0;
static __IO u32 sTimingDelay;

void delay_init(void)   //延时模块初始化函数
{
	//SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
	
	//fac_us = SystemCoreClock / 8000000;
	
	//fac_ms = (u16)fac_us * 1000;
	
	 if (SysTick_Config(84000000 / 1000)) { //
        while (1);
    }
    NVIC_SetPriority(SysTick_IRQn, 0x0);

}

static void TimingDelayDecrement(void)
{
    if (sTimingDelay != 0x00) {
        sTimingDelay--;
    }
}

void SysTick_Handler(void)
{
    TimingDelayDecrement();
}

void delay_us(__IO u32 count)      //微秒延时函数
{
	sTimingDelay = count/1000;
  while(sTimingDelay != 0);

}
void delay_ms(__IO u32 count)     //毫秒延时函数
{
	sTimingDelay = count;
  while(sTimingDelay != 0);


}
void delay_s(u32 s)      //秒延时函数
{

	while(s--)
	{
		delay_ms(1000);
	}
		


}
