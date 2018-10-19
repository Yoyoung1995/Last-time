//一个任务对应一组资源 ： 信号量、消息队列、事件标志组 等
#include "GPRS_DataSendTask_Resources.h"


GPRS_DataSendXXXX GPRS_DataSend;




//一些方便调用的函数

//Modbus 03 功能码查询指令 函数
//填入 从机地址、寄存器起始地址、寄存器的数量
void Modbus_03_Search(uint8_t SlaveAddr, uint16_t StartingAddr, uint16_t Num)
{
	uint8_t sendBuf[8] = {0};
	uint16_t myCRC = 0;
	
	sendBuf[0] = SlaveAddr;       //从机地址
	sendBuf[1] = 3;								//功能码
	sendBuf[2] = StartingAddr>>8;									//起始地址 高位
	sendBuf[3] = StartingAddr%256;									//起始地址 低位
	sendBuf[4] = Num>>8;									//寄存器数量 高位
	sendBuf[5] = Num%256;									//寄存器数量 低位
	//CRC
	myCRC = gprsCRC(sendBuf, 6);
	sendBuf[6] = 	myCRC%256;								//CRC 低位
	sendBuf[7] = 	myCRC>>8;								//CRC 高位
	
	USART2_485_Send(sendBuf,8);
}


//参数修改  函数
//根据 发过来的消息，提取修改相关的一些数据
//返回值 :        0---  成功修改
//                1---  CRC校验错误
//  							2---  功能码错误
//                3---  返回字节长度错误
uint8_t Modbus_Modify(USART_RECEIVETYPE * pMail)
{
	uint8_t  len = 0 ;		//字节长度
	uint16_t myCRC = 0;
	uint8_t return_Data = 0;
	float   f_data = 0.0; //所有读取来的float型数据都去掉小数转换成整数存入节能控制板的对应寄存器
	
	if( pMail->RX_pData[0] == 63) //先判断数据来自哪个设备？
	{ //-------- 若是来自数字电表
		if( pMail->RX_pData[1] != 3)//			检验功能码
			return_Data = 2;
		len = pMail->RX_pData[2];
		myCRC = pMail->RX_pData[len+3] + ( pMail->RX_pData[len+4]<<8 );//      检验CRC
		if( myCRC != gprsCRC(pMail->RX_pData,len+3) )
			return_Data =  1;
		switch(len)//			根据返回的字节长度判断 对应的是哪一条指令
		{	 //			分别对应数据提取&参数修改
			case 4:		//对应4000H~4001H的指令
				memcpy(&f_data,&(pMail->RX_pData[3]),4 );		//恢复float型数据
				ArrayTurn((uint8_t *)&f_data,4);
				BoardRegister[16] = (uint16_t)f_data;
				break;
			case 32:	//对应2000H~200FH指令
				memcpy(&f_data,&(pMail->RX_pData[3]),4 );  //恢复float型数据
				ArrayTurn((uint8_t *)&f_data,4);
				BoardRegister[11] = (uint16_t)f_data;		
			
				memcpy(&f_data,&(pMail->RX_pData[7]),4 );  
				ArrayTurn((uint8_t *)&f_data,4);
				BoardRegister[12] = (uint16_t)f_data;	
			
				memcpy(&f_data,&(pMail->RX_pData[11]),4 );  
				ArrayTurn((uint8_t *)&f_data,4);
				BoardRegister[13] = (uint16_t)f_data;	
			
				memcpy(&f_data,&(pMail->RX_pData[23]),4 ); 
				ArrayTurn((uint8_t *)&f_data,4);
				BoardRegister[14] = (uint16_t)f_data;	
					
				memcpy(&f_data,&(pMail->RX_pData[31]),4 );  
				ArrayTurn((uint8_t *)&f_data,4);
				BoardRegister[15] = (uint16_t)f_data;					
				break;
			default:
				return_Data = 3;
				break;
		}
	}
	else if( pMail->RX_pData[0] == 1 )//-------- 若是来自外机主板
	{
		//			同上
		if( pMail->RX_pData[1] != 3)//			检验功能码
			return_Data = 2;
		len = pMail->RX_pData[2];
		myCRC = pMail->RX_pData[len+3] + ( pMail->RX_pData[len+4]<<8 );//      检验CRC
		if( myCRC != gprsCRC(pMail->RX_pData,len+3) )
			return_Data =  1;
		switch( len )
		{
			case 56:	//对应1200~1227的指令
				BoardRegister[17] = ( pMail->RX_pData[3]<<8 ) + pMail->RX_pData[4];
				BoardRegister[18] = ( pMail->RX_pData[7]<<8 ) + pMail->RX_pData[8];
				BoardRegister[19] = ( pMail->RX_pData[9]<<8 ) + pMail->RX_pData[10];
				BoardRegister[20] = ( pMail->RX_pData[11]<<8 ) + pMail->RX_pData[12];
				BoardRegister[21] = ( pMail->RX_pData[13]<<8 ) + pMail->RX_pData[14];
				BoardRegister[22] = ( pMail->RX_pData[15]<<8 ) + pMail->RX_pData[16];
				BoardRegister[32] = ( pMail->RX_pData[55]<<8 ) + pMail->RX_pData[56];
				BoardRegister[33] = ( pMail->RX_pData[57]<<8 ) + pMail->RX_pData[58];
				break;
			case 38:	//对应5104~5122的指令
				BoardRegister[23] = ( pMail->RX_pData[3]<<8 ) + pMail->RX_pData[4];
				BoardRegister[24] = ( pMail->RX_pData[7]<<8 ) + pMail->RX_pData[8];
				BoardRegister[25] = ( pMail->RX_pData[9]<<8 ) + pMail->RX_pData[10];
				BoardRegister[26] = ( pMail->RX_pData[11]<<8 ) + pMail->RX_pData[12];
				BoardRegister[27] = ( pMail->RX_pData[19]<<8 ) + pMail->RX_pData[20];
				BoardRegister[28] = ( pMail->RX_pData[21]<<8 ) + pMail->RX_pData[22];
				BoardRegister[29] = ( pMail->RX_pData[23]<<8 ) + pMail->RX_pData[24];
				BoardRegister[30] = ( pMail->RX_pData[29]<<8 ) + pMail->RX_pData[30];
				BoardRegister[31] = ( pMail->RX_pData[31]<<8 ) + pMail->RX_pData[32];
				BoardRegister[34] = ( pMail->RX_pData[35]<<8 ) + pMail->RX_pData[36];
				BoardRegister[35] = ( pMail->RX_pData[37]<<8 ) + pMail->RX_pData[38];
				BoardRegister[34] = ( pMail->RX_pData[39]<<8 ) + pMail->RX_pData[40];
				break;
			default:
				return_Data = 3;
				break;
		}
	}
	return return_Data;
}
