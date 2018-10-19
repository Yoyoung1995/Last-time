//һ�������Ӧһ����Դ �� �ź�������Ϣ���С��¼���־�� ��
#include "GPRS_DataSendTask_Resources.h"


GPRS_DataSendXXXX GPRS_DataSend;




//һЩ������õĺ���

//Modbus 03 �������ѯָ�� ����
//���� �ӻ���ַ���Ĵ�����ʼ��ַ���Ĵ���������
void Modbus_03_Search(uint8_t SlaveAddr, uint16_t StartingAddr, uint16_t Num)
{
	uint8_t sendBuf[8] = {0};
	uint16_t myCRC = 0;
	
	sendBuf[0] = SlaveAddr;       //�ӻ���ַ
	sendBuf[1] = 3;								//������
	sendBuf[2] = StartingAddr>>8;									//��ʼ��ַ ��λ
	sendBuf[3] = StartingAddr%256;									//��ʼ��ַ ��λ
	sendBuf[4] = Num>>8;									//�Ĵ������� ��λ
	sendBuf[5] = Num%256;									//�Ĵ������� ��λ
	//CRC
	myCRC = gprsCRC(sendBuf, 6);
	sendBuf[6] = 	myCRC%256;								//CRC ��λ
	sendBuf[7] = 	myCRC>>8;								//CRC ��λ
	
	USART2_485_Send(sendBuf,8);
}


//�����޸�  ����
//���� ����������Ϣ����ȡ�޸���ص�һЩ����
//����ֵ :        0---  �ɹ��޸�
//                1---  CRCУ�����
//  							2---  ���������
//                3---  �����ֽڳ��ȴ���
uint8_t Modbus_Modify(USART_RECEIVETYPE * pMail)
{
	uint8_t  len = 0 ;		//�ֽڳ���
	uint16_t myCRC = 0;
	uint8_t return_Data = 0;
	float   f_data = 0.0; //���ж�ȡ����float�����ݶ�ȥ��С��ת��������������ܿ��ư�Ķ�Ӧ�Ĵ���
	
	if( pMail->RX_pData[0] == 63) //���ж����������ĸ��豸��
	{ //-------- �����������ֵ��
		if( pMail->RX_pData[1] != 3)//			���鹦����
			return_Data = 2;
		len = pMail->RX_pData[2];
		myCRC = pMail->RX_pData[len+3] + ( pMail->RX_pData[len+4]<<8 );//      ����CRC
		if( myCRC != gprsCRC(pMail->RX_pData,len+3) )
			return_Data =  1;
		switch(len)//			���ݷ��ص��ֽڳ����ж� ��Ӧ������һ��ָ��
		{	 //			�ֱ��Ӧ������ȡ&�����޸�
			case 4:		//��Ӧ4000H~4001H��ָ��
				memcpy(&f_data,&(pMail->RX_pData[3]),4 );		//�ָ�float������
				ArrayTurn((uint8_t *)&f_data,4);
				BoardRegister[16] = (uint16_t)f_data;
				break;
			case 32:	//��Ӧ2000H~200FHָ��
				memcpy(&f_data,&(pMail->RX_pData[3]),4 );  //�ָ�float������
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
	else if( pMail->RX_pData[0] == 1 )//-------- ���������������
	{
		//			ͬ��
		if( pMail->RX_pData[1] != 3)//			���鹦����
			return_Data = 2;
		len = pMail->RX_pData[2];
		myCRC = pMail->RX_pData[len+3] + ( pMail->RX_pData[len+4]<<8 );//      ����CRC
		if( myCRC != gprsCRC(pMail->RX_pData,len+3) )
			return_Data =  1;
		switch( len )
		{
			case 56:	//��Ӧ1200~1227��ָ��
				BoardRegister[17] = ( pMail->RX_pData[3]<<8 ) + pMail->RX_pData[4];
				BoardRegister[18] = ( pMail->RX_pData[7]<<8 ) + pMail->RX_pData[8];
				BoardRegister[19] = ( pMail->RX_pData[9]<<8 ) + pMail->RX_pData[10];
				BoardRegister[20] = ( pMail->RX_pData[11]<<8 ) + pMail->RX_pData[12];
				BoardRegister[21] = ( pMail->RX_pData[13]<<8 ) + pMail->RX_pData[14];
				BoardRegister[22] = ( pMail->RX_pData[15]<<8 ) + pMail->RX_pData[16];
				BoardRegister[32] = ( pMail->RX_pData[55]<<8 ) + pMail->RX_pData[56];
				BoardRegister[33] = ( pMail->RX_pData[57]<<8 ) + pMail->RX_pData[58];
				break;
			case 38:	//��Ӧ5104~5122��ָ��
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
