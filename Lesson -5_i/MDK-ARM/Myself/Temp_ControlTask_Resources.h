#ifndef TEMP_CONTROL_RESOURCES_H 
#define TEMP_CONTROL_RESOURCES_H

#include "public.h"






//���������߼�
extern void HeatAir_Running(void);
//���������߼�
extern void CoolAir_Running(void);
//ModbusԤ�õ����Ĵ���
//���� �ӻ���ַ���Ĵ�����ʼ��ַ���Ĵ�����������ֵ
extern void Modbus_SetRegister( uint8_t SlaveAdd, uint16_t RegisterAdd, int16_t Data);
#endif
