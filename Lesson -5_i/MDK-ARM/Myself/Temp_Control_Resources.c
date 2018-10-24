//一个任务对应一组资源 ： 信号量、消息队列、事件标志组 等

#include "Temp_Control_Resources.h"











//一些方便调用的函数


//Modbus预置单个寄存器
//填入 从机地址、寄存器起始地址、寄存器的设置数值
void Modbus_SetRegister( uint8_t SlaveAdd, uint16_t RegisterAdd, int16_t Data)
{
	uint8_t Buf[10] = {0};
	uint16_t myCRC = 0;
	
	Buf[0] = SlaveAdd;
	Buf[1] = 0x06;
	Buf[2] = RegisterAdd>>8;
	Buf[3] = RegisterAdd%256;
	Buf[4] = Data>>8;
	Buf[5] = Data%256;
	//CRC
	myCRC = gprsCRC(Buf,6);
	Buf[6] = myCRC%256;
	Buf[7] = myCRC>>8;
	
	USART2_485_Send(Buf,8);
}


//制热运行逻辑
void HeatAir_Running(void)
{
	float diff = 0.0;
	float SetTemp = 0.0;		//设定水温 buff
	
	diff = DS18b20_FinalTemp - BoardRegister[2];	//温差
	
	if( diff<-2 )
	{
		Modbus_SetRegister(1,999,2);
		osDelay(100);
		Modbus_SetRegister(1,1059,55);
	}
	else if( (-1<diff) && (diff<0) ) 
	{
		SetTemp = (BoardRegister[22]-3000)*0.01 + 1 ;
		Modbus_SetRegister(1,1059,(int16_t)SetTemp);
	}
	else if( (0<diff) && (diff<1) ) 
	{
		SetTemp = (BoardRegister[22]-3000)*0.01 ;
		Modbus_SetRegister(1,1059,(int16_t)SetTemp);		
	}
	else if( 1<diff) 
	{
		Modbus_SetRegister(1,999,0);
	}
		
}


//制冷运行逻辑
void CoolAir_Running(void)
{
	float diff = 0.0;
	float SetTemp = 0.0;		//设定水温 buff
	
	diff = DS18b20_FinalTemp - BoardRegister[2];	//温差
	
	if( diff>2 )
	{
		Modbus_SetRegister(1,999,3);
		osDelay(100);
		Modbus_SetRegister(1,1089,12);
	}
	else if( (0<diff) && (diff<1) ) 
	{
		SetTemp = (BoardRegister[18]-3000)*0.01 - 1 ;
		Modbus_SetRegister(1,1089,(int16_t)SetTemp);
	}
	else if( (0>diff) && (diff>-1) ) 
	{
		SetTemp = (BoardRegister[18]-3000)*0.01 ;
		Modbus_SetRegister(1,1089,(int16_t)SetTemp);		
	}
	else if( -1>diff) 
	{
		Modbus_SetRegister(1,999,0);
	}
			
}
