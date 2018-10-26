//汉枫 Elfin-EG11 GPRS模块

#include "Elfin-EG11_GPRS_Module.h"










//发送AT指令
//--在这里不需要加<CR>回车符
//例： AT_Send((unsigned char *)"AT+UART=1");
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
 *	GPRS模块初始化函数，波特率配置
 * 	服务器配置  (可以用手机短信修改的方式替代)
 *	
 *  字符串拼接，然后发送。
 ***/
uint16_t GPRS_Init(void)
{
				//进入AT命令模式         由于模块本身问题，模块要多发几次
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_SET);
	HAL_UART_Transmit(&huart3, (unsigned char *)"+++", 3, 0xFFFF);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_RESET);
	HAL_Delay(1000);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_SET);
	HAL_UART_Transmit(&huart3, (unsigned char *)"a", 1, 0xFFFF);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_RESET);
	HAL_Delay(1000);
				//默认出厂波特率  115200bps
//	AT_Send((uint8_t *)"AT+FCLR");	//恢复出厂设置
//	HAL_Delay(10000);
	
//				//进入AT命令模式         由于模块本身问题，模块要多发几次
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
	
				//连接到远程服务器          由于模块本身问题，模块要多发几次
	//AT_Send((uint8_t *)"AT+SOCKA=TCP,3006,nat2.iotworkshop.com,LONG");
	AT_Send((uint8_t *)"AT+NETP=A,1,TCP,nat2.iotworkshop.com,3006,Long");
	//AT_Send((uint8_t *)"AT+NETP=A,1,117.136.40.159,8868,Long");
	HAL_Delay(2000);
	
				//重启GPRS模块
	AT_Send((uint8_t *)"AT+Z");
	HAL_Delay(5000);
	return 0;
}

