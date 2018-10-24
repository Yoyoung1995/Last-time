#ifndef GPRS_DATA_SEND_TASK_RESOURCES_H
#define GPRS_DATA_SEND_TASK_RESOURCES_H

#include "public.h"

typedef struct {
	QueueHandle_t xPointerQueue;
	//邮件类型 USART_RECEIVETYPE *
	//信号量  osSemaphoreId bSem_GPRS_DataSendHandle;
}GPRS_DataSendXXXX;

extern GPRS_DataSendXXXX GPRS_DataSend;





//一些方便调用的函数

//Modbus 03 功能码查询指令 函数
//填入 从机地址、寄存器起始地址、寄存器的数量
extern void Modbus_03_Search(uint8_t SlaveAddr, uint16_t StartingAddr, uint16_t Num);

//参数修改  函数
//根据 发过来的消息，提取修改相关的一些数据
//返回值 :        0---  成功修改
//                1---  CRC校验错误
//  							2---  功能码错误
//                3---  返回字节长度错误
extern uint8_t Modbus_Modify(USART_RECEIVETYPE * pMail);

//向服务器回传节能控制板寄存器数据
//MAC : 当前网卡地址MAC
//DevNum : 设备编号
extern void GPRS_Send(uint8_t * MAC, uint8_t DevNum );
#endif
