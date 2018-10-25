//485ģ������
//			RX--|RO					 |
//		IO--|-|RE					B|
//				|-|DE					A|
//			TX--|DI					 |
//
//			IO�ߵ�ƽʱ�� MCUֻ����485ģ�鷢������ ;   IO�͵�ƽʱ�� MCUֻ�ܴ�485ģ��������� 

#include "485Module.h"

//����2 485��Ϣ���ͺ���
void USART2_485_Send(uint8_t * pBuff, uint16_t len)
{
	taskENTER_CRITICAL();
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_4,GPIO_PIN_SET);
	HAL_UART_Transmit(&huart2,pBuff,len,osWaitForever);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_4,GPIO_PIN_RESET);
	taskEXIT_CRITICAL();
}


//����3 485��Ϣ���ͺ���
void USART3_485_Send(uint8_t * pBuff, uint16_t len)
{
	taskENTER_CRITICAL();
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_SET);
	HAL_UART_Transmit(&huart3,pBuff,len,osWaitForever);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_RESET);
	taskEXIT_CRITICAL();
}
