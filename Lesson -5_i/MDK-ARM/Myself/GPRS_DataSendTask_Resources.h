#ifndef GPRS_DATA_SEND_TASK_RESOURCES_H
#define GPRS_DATA_SEND_TASK_RESOURCES_H

#include "public.h"

typedef struct {
	QueueHandle_t xPointerQueue;
	//�ʼ����� USART_RECEIVETYPE *
	//�ź���  osSemaphoreId bSem_GPRS_DataSendHandle;
}GPRS_DataSendXXXX;

extern GPRS_DataSendXXXX GPRS_DataSend;





//һЩ������õĺ���

//Modbus 03 �������ѯָ�� ����
//���� �ӻ���ַ���Ĵ�����ʼ��ַ���Ĵ���������
extern void Modbus_03_Search(uint8_t SlaveAddr, uint16_t StartingAddr, uint16_t Num);

//�����޸�  ����
//���� ����������Ϣ����ȡ�޸���ص�һЩ����
//����ֵ :        0---  �ɹ��޸�
//                1---  CRCУ�����
//  							2---  ���������
//                3---  �����ֽڳ��ȴ���
extern uint8_t Modbus_Modify(USART_RECEIVETYPE * pMail);

//��������ش����ܿ��ư�Ĵ�������
//MAC : ��ǰ������ַMAC
//DevNum : �豸���
extern void GPRS_Send(uint8_t * MAC, uint8_t DevNum );
#endif
