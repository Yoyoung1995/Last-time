#ifndef DS18B20_H
#define DS18B20_H

#include "public.h"


extern float   DS18b20_FinalTemp ;


//读温度值（低位放tempL;高位放tempH;）
//与底层无关
extern float ReadTemperature(void);

#endif
