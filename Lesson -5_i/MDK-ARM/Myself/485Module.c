//485模块驱动
//			RX--|RO					 |
//		IO--|-|RE					B|
//				|-|DE					A|
//			TX--|DI					 |
//
//			IO高电平时： MCU只能向485模块发送数据 ;   IO低电平时： MCU只能从485模块接收数据 

#include "485Module.h"

//串口2 485信息发送函数
void USART2_485_Send(uint8_t * pBuff, uint16_t len)
{
	taskENTER_CRITICAL();
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_4,GPIO_PIN_SET);
	HAL_UART_Transmit(&huart2,pBuff,len,osWaitForever);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_4,GPIO_PIN_RESET);
	taskEXIT_CRITICAL();
}


//串口3 485信息发送函数
void USART3_485_Send(uint8_t * pBuff, uint16_t len)
{
	taskENTER_CRITICAL();
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_SET);
	HAL_UART_Transmit(&huart3,pBuff,len,osWaitForever);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_RESET);
	taskEXIT_CRITICAL();
}
