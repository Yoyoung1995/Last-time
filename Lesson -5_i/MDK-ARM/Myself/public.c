#include "public.h"

//------------------ Public Functions : Begin
/**
  * @brief  This function handles USART1 IDLE interrupt.
	*			This function handles USARTx IDLE interrupt.  ���� 2018/09/28 10:15��
	*			This function handles USARTx IDLE interrupt.  
  * @param  huartX:  can be huart1 ... huartN
	* @param 	pStruct:  choosing the space to store.
	*																										���� 2018/09/28 10:20��
  */
void UsartReceive_IDLE(UART_HandleTypeDef *huartX, USART_RECEIVETYPE * pStruct)  
{  
    uint32_t temp;  
  
    if((__HAL_UART_GET_FLAG(huartX,UART_FLAG_IDLE) != RESET))  
    {   
        __HAL_UART_CLEAR_IDLEFLAG(huartX);  
        HAL_UART_DMAStop(huartX);  
        temp = (*huartX).hdmarx->Instance->CNDTR;  
        (*pStruct).RX_Size =  RX_LEN - temp;   
        (*pStruct).RX_flag=1;  
        HAL_UART_Receive_DMA(huartX,(*pStruct).RX_pData,RX_LEN);  
    }  
}  


//�ͷ�����ͨѶ��CRCУ���㷨   ͬMODBUS��CRC
uint16_t gprsCRC(const uint8_t * pBuf, int nNum)
{
	uint16_t wCrc = 0xFFFF;
	for(int i=0; i<nNum; i++)	
	{
		wCrc ^= pBuf[i];
		for(int j=0; j<8; j++)		
		{
			if(wCrc & 1)		
			{	
				wCrc >>= 1; 		
				wCrc ^= 0xA001; 	
			}
			else	
			{
				wCrc >>= 1; 
			}	
		}	
	}
	return wCrc;
}

//��С��ת������
// pBuf: ����ָ��  ,  len  ���鳤��
void ArrayTurn(uint8_t * pBuf, uint8_t len)
{
	uint8_t temp[128] = {0};
	
	memcpy(temp,pBuf,len);
	
	for(uint8_t i=0;i<len;i++)
	{
		*(pBuf+i) = temp[len-1- i];
	}
}

//------------------ Public Functions : End



//------------------ Public Variables : Begin
USART_RECEIVETYPE UsartType_1 = {0};
USART_RECEIVETYPE UsartType_2 = {0};
USART_RECEIVETYPE UsartType_3 = {0};


//���ܿ��ư�Ĵ���
uint16_t BoardRegister[REGISTER_SIZE] = {0};
//���ܿ��ư�Ĭ���豸���
uint8_t DeviceNum = 1;  
//���ܿ��ư�Ĭ������MAC��ַ
uint8_t theMAC[6] = {0};

//------------------ Public Variables : End
