#ifndef MODULE485_H
#define MODULE485_H


#include "public.h"


//串口2 485信息发送函数
extern void USART2_485_Send(uint8_t * pBuff, uint16_t len);

//串口3 485信息发送函数
extern void USART3_485_Send(uint8_t * pBuff, uint16_t len);

#endif
