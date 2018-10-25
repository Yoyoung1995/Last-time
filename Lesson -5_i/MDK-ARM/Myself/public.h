#ifndef PUBLIC_H
#define PUBLIC_H

#include "stm32f1xx_hal.h"
#include "cmsis_os.h"
#include "FreeRTOS.h"
#include "task.h"
#include "string.h"
#include "usart.h"
#include "485Module.h"
#include "DS18b20Module.h"
#include "BoardSet_Parameters.h"

//---------------------------------串口不定长接收 
#define		RX_LEN  256

typedef struct{
	uint8_t RX_flag:1;
	uint16_t  RX_Size;
	uint8_t  RX_pData[RX_LEN];
}USART_RECEIVETYPE;
//----------------------------------节能控制板寄存器
#define REGISTER_SIZE 37  //地址从0开始 0~36

extern uint16_t BoardRegister[REGISTER_SIZE];

//节能控制板默认设备编号
extern uint8_t DeviceNum ;  
//节能控制板默认网卡MAC地址
extern uint8_t theMAC[6] ;


extern USART_RECEIVETYPE UsartType_1;
extern USART_RECEIVETYPE UsartType_2;
extern USART_RECEIVETYPE UsartType_3;

extern void UsartReceive_IDLE(UART_HandleTypeDef *huartX, USART_RECEIVETYPE * pStruct) ;

extern osSemaphoreId bSem_USART1_ServeHandle;
extern osSemaphoreId bSem_USART2_ServeHandle;
extern osSemaphoreId bSem_USART3_ServeHandle;

//和服务器通讯的CRC校验算法
extern uint16_t gprsCRC(const uint8_t * pBuf, int nNum);

//大小端转换函数
// pBuf: 数组指针  ,  len  数组长度
extern void ArrayTurn(uint8_t * pBuf, uint8_t len);

#endif
