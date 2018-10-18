#include "public.h"

//------------------ Public Functions : Begin
/**
  * @brief  This function handles USART1 IDLE interrupt.
	*			This function handles USARTx IDLE interrupt.  ¡ª¡ª 2018/09/28 10:15¸Ä
	*			This function handles USARTx IDLE interrupt.  
  * @param  huartX:  can be huart1 ... huartN
	* @param 	pStruct:  choosing the space to store.
	*																										¡ª¡ª 2018/09/28 10:20¸Ä
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







//------------------ Public Functions : End



//------------------ Public Variables : Begin
USART_RECEIVETYPE UsartType_1 = {0};
USART_RECEIVETYPE UsartType_2 = {0};
USART_RECEIVETYPE UsartType_3 = {0};






//------------------ Public Variables : End
