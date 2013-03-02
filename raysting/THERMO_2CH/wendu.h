#ifndef _WEN_DU_H
#define _WEN_DU_H
#include "define.h"
#include "stdarg.h"
double RValueToTValue(double r, u8 prbid, u8 type);
double MValueToTValue(double r,i8 type);
double GetWr(double t);
double GetT(double w); 
void update_display();
void update_led(u8 mode);
#endif
