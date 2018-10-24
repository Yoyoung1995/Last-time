//DS18B20模块   STM32  72Mhz
//  DQ 替换成 PD10口   STM32双向口

#include "DS18b20Module.h"
#include "tim.h"

//从DS18b20 读出的温度寄存器的值
static uint8_t tempL=0; 		 
static uint8_t tempH=0; 
float   DS18b20_FinalTemp = 0.0;

static uint8_t fg=1;        			//温度正负标志







/*******************************************************************************
* 函数名      : delay_us
* 函数功能    : 延时函数，延时us
* 输入        : i
* 注意：函数中的参数 i 不能超过1800   
*********** 该函数被禁止，FreeRTOS使用SysTick作为时钟基准了。
*******************************************************************************/
void delay_us(uint32_t i)
{
	__HAL_TIM_SET_COUNTER(&htim7, 0);//htim17
	__HAL_TIM_ENABLE(&htim7);

	while(__HAL_TIM_GET_COUNTER(&htim7) < (72 * i));//72是系统时钟，这里改
	/* Disable the Peripheral */
	__HAL_TIM_DISABLE(&htim7);
}


void Init_DS18B20(void)
{
	unsigned char x=0;
	
	//DQ=1; 					//DQ先置高 
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_10,GPIO_PIN_SET);
	delay_us(72); 				//稍延时
	//DQ=0; 					//发送复位脉冲 
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_10,GPIO_PIN_RESET);
	delay_us(720); 				//延时（>480us) 
	//DQ=1; 					//拉高数据线 
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_10,GPIO_PIN_SET);
	delay_us(45); 				//等待（15~60us) 
	//x=DQ; 					//用X的值来判断初始化有没有成功，18B20存在的话X=0，否则X=1 
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_10,GPIO_PIN_SET);
	x= HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_10);
	x=x;      //消除警告
	delay_us(180); 
}


//写一个字节
void WriteOneChar(unsigned char dat) 
{ 
	unsigned char i=0; 		//数据线从高电平拉至低电平，产生写起始信号。15us之内将所需写的位送到数据线上，
	for(i=8;i>0;i--) 		//在15~60us之间对数据线进行采样，如果是高电平就写1，低写0发生。 
	{
		//DQ=0; 				//在开始另一个写周期前必须有1us以上的高电平恢复期。 
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_10,GPIO_PIN_RESET);
		//DQ=dat&0x01; 
		if(dat&0x01)
			HAL_GPIO_WritePin(GPIOD,GPIO_PIN_10,GPIO_PIN_SET);
		else
			HAL_GPIO_WritePin(GPIOD,GPIO_PIN_10,GPIO_PIN_RESET);
		delay_us(45); 
		//DQ=1; 
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_10,GPIO_PIN_SET);
		dat>>=1;
	} 
	delay_us(36);
}


//读一个字节
uint8_t ReadOneChar(void)  			//主机数据线先从高拉至低电平1us以上，再使数据线升为高电平，从而产生读信号
{
	unsigned char i=0; 		//每个读周期最短的持续时间为60us，各个读周期之间必须有1us以上的高电平恢复期
	unsigned char dat=0; 
	for (i=8;i>0;i--) 		//一个字节有8位 
	{
		//DQ=1; 
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_10,GPIO_PIN_SET);
		delay_us(9); 
		//DQ=0;
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_10,GPIO_PIN_RESET);
		dat>>=1; 
		//DQ=1; 
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_10,GPIO_PIN_SET);
		//if(DQ) 
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_10,GPIO_PIN_SET);
		if( HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_10) )
		dat|=0x80; 
		delay_us(36);
	} 
	return(dat);
}


//读温度值（低位放tempL;高位放tempH;）
//与底层无关
float ReadTemperature(void) 
{ 
	float  Temp = 0.0;
	Init_DS18B20(); 					//初始化
	WriteOneChar(0xcc); 				//跳过读序列号的操作
	WriteOneChar(0x44); 				//启动温度转换
	osDelay(750); 						//转换需要一点时间，延时 750ms
	Init_DS18B20(); 					//初始化
	WriteOneChar(0xcc); 				//跳过读序列号的操作 
	WriteOneChar(0xbe); 				//读温度寄存器（头两个值分别为温度的低位和高位） 
	tempL=ReadOneChar(); 				//读出温度的低位LSB
	tempH=ReadOneChar(); 				//读出温度的高位MSB	
	if(tempH>0x7f)      				//最高位为1时温度是负
	{
		tempL=~tempL;					//补码转换，取反加一
		tempH=~tempH+1;       
		fg=0;      						//读取温度为负时fg=0
	}
	
	Temp = tempH*16 + tempL*0.0625;		//温度换算
	
	if( fg )
		Temp = Temp;
	else 
		Temp = -Temp;
	
	fg = 1;		//恢复默认
	
	return Temp;
//	sdata = tempL/16+tempH*16;      	//整数部分
//	xiaoshu1 = (tempL&0x0f)*10/16; 		//小数第一位
//	xiaoshu2 = (tempL&0x0f)*100/16%10;	//小数第二位
//	xiaoshu=xiaoshu1*10+xiaoshu2; 		//小数两位
}
