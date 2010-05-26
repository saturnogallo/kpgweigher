#include "define.h"
sbit BEEP = P2^3;
void main()
{
 BEEP = 0;
 PT6312_Test();
}

