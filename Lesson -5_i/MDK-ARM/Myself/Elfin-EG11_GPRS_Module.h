#ifndef GPRS_MODULE_H
#define GPRS_MODULE_H

#include "public.h"



//����ATָ��
//--�����ﲻ��Ҫ��<CR>�س���
//���� AT_Send((unsigned char *)"AT+UART=1");
extern void AT_Send(unsigned char * s);

/****
 *	GPRSģ���ʼ������������������
 * 	����������  (�������ֻ������޸ĵķ�ʽ���)
 *	
 *  �ַ���ƴ�ӣ�Ȼ���͡�
 ***/
extern uint16_t GPRS_Init(void);



#endif
