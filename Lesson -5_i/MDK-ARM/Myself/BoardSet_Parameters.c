//根据PCB板跳线、拨码等初始化 文件。

#include "BoardSet_Parameters.h"









//一些方便调用的函数


//根据跳线、拨码 进行参数初始化
void Board_Init(void)
{
	uint8_t TiaoXian_Hot = 0;
	uint8_t TiaoXian_Cold = 0;
	uint8_t BoMa_1 = 0;
	uint8_t BoMa_2 = 0;
	uint8_t BoMa_3 = 0;
	uint8_t BoMa_4 = 0;
	uint8_t BoMa_Num = 0;
	
	TiaoXian_Hot = HAL_GPIO_ReadPin( GPIOA, GPIO_PIN_4 );		//检查跳线   默认上拉
	TiaoXian_Cold = HAL_GPIO_ReadPin( GPIOA, GPIO_PIN_5 );	
	if( (TiaoXian_Hot==0) && (TiaoXian_Cold==1) )	//修改参数
	{
		BoardRegister[1] = 2;			//制热，20度
		BoardRegister[2] = 20;
	}
	else if( (TiaoXian_Hot==1) && (TiaoXian_Cold==0) )
	{
		BoardRegister[1] = 3;			//制冷，24度
		BoardRegister[2] = 24;		
	}
	else
	{
		BoardRegister[1] = 1;			//关机
	}
	BoMa_1 = HAL_GPIO_ReadPin( GPIOA, GPIO_PIN_0 );	//检查拨码
	BoMa_2 = HAL_GPIO_ReadPin( GPIOA, GPIO_PIN_1 );	
	BoMa_3 = HAL_GPIO_ReadPin( GPIOA, GPIO_PIN_2 );	
	BoMa_4 = HAL_GPIO_ReadPin( GPIOA, GPIO_PIN_3 );	
	BoMa_Num = BoMa_1*8 + BoMa_2*4 + BoMa_3*2 + BoMa_4;
	DeviceNum = BoMa_Num;	//修改设备编号
	//done
}
