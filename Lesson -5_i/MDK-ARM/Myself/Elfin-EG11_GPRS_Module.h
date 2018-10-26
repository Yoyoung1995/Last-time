#ifndef GPRS_MODULE_H
#define GPRS_MODULE_H

#include "public.h"



//发送AT指令
//--在这里不需要加<CR>回车符
//例： AT_Send((unsigned char *)"AT+UART=1");
extern void AT_Send(unsigned char * s);

/****
 *	GPRS模块初始化函数，波特率配置
 * 	服务器配置  (可以用手机短信修改的方式替代)
 *	
 *  字符串拼接，然后发送。
 ***/
extern uint16_t GPRS_Init(void);



#endif
