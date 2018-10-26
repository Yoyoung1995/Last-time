//���� Elfin-EG11 GPRSģ��

#include "Elfin-EG11_GPRS_Module.h"










//����ATָ��
//--�����ﲻ��Ҫ��<CR>�س���
//���� AT_Send((unsigned char *)"AT+UART=1");
void AT_Send(unsigned char * s)
{
	char buffer[256] = {0};
	uint8_t len = 0;
	
	len = snprintf(buffer, sizeof(buffer) , "%s%c",s,'\r');

	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_SET);
	HAL_UART_Transmit(&huart3,(uint8_t *)buffer,len,osWaitForever);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_RESET);
}


/****
 *	GPRSģ���ʼ������������������
 * 	����������  (�������ֻ������޸ĵķ�ʽ���)
 *	
 *  �ַ���ƴ�ӣ�Ȼ���͡�
 ***/
uint16_t GPRS_Init(void)
{
				//����AT����ģʽ         ����ģ�鱾�����⣬ģ��Ҫ�෢����
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_SET);
	HAL_UART_Transmit(&huart3, (unsigned char *)"+++", 3, 0xFFFF);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_RESET);
	HAL_Delay(1000);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_SET);
	HAL_UART_Transmit(&huart3, (unsigned char *)"a", 1, 0xFFFF);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_RESET);
	HAL_Delay(1000);
				//Ĭ�ϳ���������  115200bps
//	AT_Send((uint8_t *)"AT+FCLR");	//�ָ���������
//	HAL_Delay(10000);
	
//				//����AT����ģʽ         ����ģ�鱾�����⣬ģ��Ҫ�෢����
//	for(uint8_t i=0; i<3; i++ )
//	{
//		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_SET);
//		HAL_UART_Transmit(&huart3, (unsigned char *)"+++", 3, 0xFFFF);
//		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_RESET);
//		HAL_Delay(1000);
//		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_SET);
//		HAL_UART_Transmit(&huart3, (unsigned char *)"a", 1, 0xFFFF);
//		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_RESET);
//		HAL_Delay(1000);
//	}
	
				//���ӵ�Զ�̷�����          ����ģ�鱾�����⣬ģ��Ҫ�෢����
	//AT_Send((uint8_t *)"AT+SOCKA=TCP,3006,nat2.iotworkshop.com,LONG");
	AT_Send((uint8_t *)"AT+NETP=A,1,TCP,nat2.iotworkshop.com,3006,Long");
	//AT_Send((uint8_t *)"AT+NETP=A,1,117.136.40.159,8868,Long");
	HAL_Delay(2000);
	
				//����GPRSģ��
	AT_Send((uint8_t *)"AT+Z");
	HAL_Delay(5000);
	return 0;
}

