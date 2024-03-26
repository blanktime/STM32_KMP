#include "uart.h"

void Uart_Init(u32 bround)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);//�������ýӿڵ�ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//��������1��ʱ��

	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //����PA9ΪUSART1�ĸ��ýӿ�
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);//����PA10ΪUSART1�ĸ��ýӿ�
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;//����gpio��Ϊ����ģʽ
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;//����ģʽ
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10;//pin9��pin10
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;//����
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;//����

	GPIO_Init(GPIOA,&GPIO_InitStruct);//gpio��ʼ��
	
	USART_InitTypeDef USART_InitStruct;
	
	USART_InitStruct.USART_BaudRate = bround;//�����bpsΪ��ʼ���������������bround
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��ʹ��Ӳ��������
	USART_InitStruct.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;//���ô���ģʽΪ�����뷢��
	USART_InitStruct.USART_Parity = USART_Parity_No;//��ʹ����֤λ
	USART_InitStruct.USART_StopBits = USART_StopBits_1;//ֹͣλΪ1λ
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;//���ݳ���Ϊ8λ
	
	USART_Init(USART1,&USART_InitStruct);//���ڳ�ʼ��

	NVIC_InitTypeDef NVIC_InitStructure_usart;
	NVIC_InitStructure_usart.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure_usart.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure_usart.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure_usart.NVIC_IRQChannelSubPriority = 2;

	NVIC_Init(&NVIC_InitStructure_usart);//��ʼ���ж�
	
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);//�򿪽����ж�
	
	USART_Cmd(USART1,ENABLE);//��������
	
}
