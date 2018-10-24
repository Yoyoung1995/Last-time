//DS18B20ģ��   STM32  72Mhz
//  DQ �滻�� PD10��   STM32˫���

#include "DS18b20Module.h"
#include "tim.h"

//��DS18b20 �������¶ȼĴ�����ֵ
static uint8_t tempL=0; 		 
static uint8_t tempH=0; 
float   DS18b20_FinalTemp = 0.0;

static uint8_t fg=1;        			//�¶�������־







/*******************************************************************************
* ������      : delay_us
* ��������    : ��ʱ��������ʱus
* ����        : i
* ע�⣺�����еĲ��� i ���ܳ���1800   
*********** �ú�������ֹ��FreeRTOSʹ��SysTick��Ϊʱ�ӻ�׼�ˡ�
*******************************************************************************/
void delay_us(uint32_t i)
{
	__HAL_TIM_SET_COUNTER(&htim7, 0);//htim17
	__HAL_TIM_ENABLE(&htim7);

	while(__HAL_TIM_GET_COUNTER(&htim7) < (72 * i));//72��ϵͳʱ�ӣ������
	/* Disable the Peripheral */
	__HAL_TIM_DISABLE(&htim7);
}


void Init_DS18B20(void)
{
	unsigned char x=0;
	
	//DQ=1; 					//DQ���ø� 
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_10,GPIO_PIN_SET);
	delay_us(72); 				//����ʱ
	//DQ=0; 					//���͸�λ���� 
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_10,GPIO_PIN_RESET);
	delay_us(720); 				//��ʱ��>480us) 
	//DQ=1; 					//���������� 
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_10,GPIO_PIN_SET);
	delay_us(45); 				//�ȴ���15~60us) 
	//x=DQ; 					//��X��ֵ���жϳ�ʼ����û�гɹ���18B20���ڵĻ�X=0������X=1 
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_10,GPIO_PIN_SET);
	x= HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_10);
	x=x;      //��������
	delay_us(180); 
}


//дһ���ֽ�
void WriteOneChar(unsigned char dat) 
{ 
	unsigned char i=0; 		//�����ߴӸߵ�ƽ�����͵�ƽ������д��ʼ�źš�15us֮�ڽ�����д��λ�͵��������ϣ�
	for(i=8;i>0;i--) 		//��15~60us֮��������߽��в���������Ǹߵ�ƽ��д1����д0������ 
	{
		//DQ=0; 				//�ڿ�ʼ��һ��д����ǰ������1us���ϵĸߵ�ƽ�ָ��ڡ� 
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


//��һ���ֽ�
uint8_t ReadOneChar(void)  			//�����������ȴӸ������͵�ƽ1us���ϣ���ʹ��������Ϊ�ߵ�ƽ���Ӷ��������ź�
{
	unsigned char i=0; 		//ÿ����������̵ĳ���ʱ��Ϊ60us������������֮�������1us���ϵĸߵ�ƽ�ָ���
	unsigned char dat=0; 
	for (i=8;i>0;i--) 		//һ���ֽ���8λ 
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


//���¶�ֵ����λ��tempL;��λ��tempH;��
//��ײ��޹�
float ReadTemperature(void) 
{ 
	float  Temp = 0.0;
	Init_DS18B20(); 					//��ʼ��
	WriteOneChar(0xcc); 				//���������кŵĲ���
	WriteOneChar(0x44); 				//�����¶�ת��
	osDelay(750); 						//ת����Ҫһ��ʱ�䣬��ʱ 750ms
	Init_DS18B20(); 					//��ʼ��
	WriteOneChar(0xcc); 				//���������кŵĲ��� 
	WriteOneChar(0xbe); 				//���¶ȼĴ�����ͷ����ֵ�ֱ�Ϊ�¶ȵĵ�λ�͸�λ�� 
	tempL=ReadOneChar(); 				//�����¶ȵĵ�λLSB
	tempH=ReadOneChar(); 				//�����¶ȵĸ�λMSB	
	if(tempH>0x7f)      				//���λΪ1ʱ�¶��Ǹ�
	{
		tempL=~tempL;					//����ת����ȡ����һ
		tempH=~tempH+1;       
		fg=0;      						//��ȡ�¶�Ϊ��ʱfg=0
	}
	
	Temp = tempH*16 + tempL*0.0625;		//�¶Ȼ���
	
	if( fg )
		Temp = Temp;
	else 
		Temp = -Temp;
	
	fg = 1;		//�ָ�Ĭ��
	
	return Temp;
//	sdata = tempL/16+tempH*16;      	//��������
//	xiaoshu1 = (tempL&0x0f)*10/16; 		//С����һλ
//	xiaoshu2 = (tempL&0x0f)*100/16%10;	//С���ڶ�λ
//	xiaoshu=xiaoshu1*10+xiaoshu2; 		//С����λ
}
