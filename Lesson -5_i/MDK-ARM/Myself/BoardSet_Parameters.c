//����PCB�����ߡ�����ȳ�ʼ�� �ļ���

#include "BoardSet_Parameters.h"









//һЩ������õĺ���


//�������ߡ����� ���в�����ʼ��
void Board_Init(void)
{
	uint8_t TiaoXian_Hot = 0;
	uint8_t TiaoXian_Cold = 0;
	uint8_t BoMa_1 = 0;
	uint8_t BoMa_2 = 0;
	uint8_t BoMa_3 = 0;
	uint8_t BoMa_4 = 0;
	uint8_t BoMa_Num = 0;
	
	TiaoXian_Hot = HAL_GPIO_ReadPin( GPIOA, GPIO_PIN_4 );		//�������   Ĭ������
	TiaoXian_Cold = HAL_GPIO_ReadPin( GPIOA, GPIO_PIN_5 );	
	if( (TiaoXian_Hot==0) && (TiaoXian_Cold==1) )	//�޸Ĳ���
	{
		BoardRegister[1] = 2;			//���ȣ�20��
		BoardRegister[2] = 20;
	}
	else if( (TiaoXian_Hot==1) && (TiaoXian_Cold==0) )
	{
		BoardRegister[1] = 3;			//���䣬24��
		BoardRegister[2] = 24;		
	}
	else
	{
		BoardRegister[1] = 1;			//�ػ�
	}
	BoMa_1 = HAL_GPIO_ReadPin( GPIOA, GPIO_PIN_0 );	//��鲦��
	BoMa_2 = HAL_GPIO_ReadPin( GPIOA, GPIO_PIN_1 );	
	BoMa_3 = HAL_GPIO_ReadPin( GPIOA, GPIO_PIN_2 );	
	BoMa_4 = HAL_GPIO_ReadPin( GPIOA, GPIO_PIN_3 );	
	BoMa_Num = BoMa_1*8 + BoMa_2*4 + BoMa_3*2 + BoMa_4;
	DeviceNum = BoMa_Num;	//�޸��豸���
	//done
}
