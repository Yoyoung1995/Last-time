#ifndef TEMP_CONTROL_RESOURCES_H 
#define TEMP_CONTROL_RESOURCES_H

#include "public.h"






//制热运行逻辑
extern void HeatAir_Running(void);
//制冷运行逻辑
extern void CoolAir_Running(void);
//Modbus预置单个寄存器
//填入 从机地址、寄存器起始地址、寄存器的设置数值
extern void Modbus_SetRegister( uint8_t SlaveAdd, uint16_t RegisterAdd, int16_t Data);
#endif
