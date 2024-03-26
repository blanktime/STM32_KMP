#include "uart.h"

void Uart_Init(u32 bround)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);//开启复用接口的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//开启串口1的时钟

	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //设置PA9为USART1的复用接口
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);//设置PA10为USART1的复用接口
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;//设置gpio口为复用模式
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;//推挽模式
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10;//pin9，pin10
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;//浮空
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;//高速

	GPIO_Init(GPIOA,&GPIO_InitStruct);//gpio初始化
	
	USART_InitTypeDef USART_InitStruct;
	
	USART_InitStruct.USART_BaudRate = bround;//这里的bps为初始化函数可以填入的bround
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//不使用硬件流控制
	USART_InitStruct.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;//设置串口模式为接收与发送
	USART_InitStruct.USART_Parity = USART_Parity_No;//不使用验证位
	USART_InitStruct.USART_StopBits = USART_StopBits_1;//停止位为1位
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;//数据长度为8位
	
	USART_Init(USART1,&USART_InitStruct);//串口初始化

	NVIC_InitTypeDef NVIC_InitStructure_usart;
	NVIC_InitStructure_usart.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure_usart.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure_usart.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure_usart.NVIC_IRQChannelSubPriority = 2;

	NVIC_Init(&NVIC_InitStructure_usart);//初始化中断
	
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);//打开接收中断
	
	USART_Cmd(USART1,ENABLE);//启动串口
	
}
