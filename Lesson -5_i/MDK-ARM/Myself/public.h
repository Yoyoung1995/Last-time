#ifndef PUBLIC_H
#define PUBLIC_H

#include "stm32f1xx_hal.h"
#include "cmsis_os.h"
#include "FreeRTOS.h"
#include "task.h"


#define		RX_LEN  512

typedef struct{
	uint8_t RX_flag:1;
	uint16_t  RX_Size;
	uint8_t  RX_pData[RX_LEN];
}USART_RECEIVETYPE;

extern USART_RECEIVETYPE UsartType_1;
extern USART_RECEIVETYPE UsartType_2;
extern USART_RECEIVETYPE UsartType_3;

extern void UsartReceive_IDLE(UART_HandleTypeDef *huartX, USART_RECEIVETYPE * pStruct) ;

extern osSemaphoreId bSem_USART1_ServeHandle;
extern osSemaphoreId bSem_USART2_ServeHandle;
extern osSemaphoreId bSem_USART3_ServeHandle;
#endif
