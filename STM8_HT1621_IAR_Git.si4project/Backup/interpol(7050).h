#ifndef __INTERPOL_H
#define __INTERPOL_H

//头文件调用
#include "stm32f10x.h"
#include "hardware.h"

int Interpol_Calculate(int x1,int y1,int x2,int y2, int x);

int Interpol_FindPoint(int x);
char Get_FuelLevelFaule_State(void);

#endif
