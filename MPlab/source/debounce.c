/***************************************************************
*  Name    : debounce.c                                        *
*  Author  : Owen O' Hehir                                     *
*  Notice  : Copyright (c) 2007                                *
*          : All Rights Reserved                               *
*  Date    : 19/6/2007                                         *
*  Version : 1.1                                               *
*  Notes   : To debounce a switch						       *
****************************************************************
*/
#include "debounce.h"

unsigned char debounce (unsigned char *port,unsigned char pin)
{
overlay unsigned char count_low;
overlay unsigned char count_high;

#define COUNT_TARGET 25u

//Assumes a 20Mhz crystal!
#if FOSC != 2 
	#error "Must alter the value of the delay in debounce.c"
#endif

//	8MHz crystal ==> Instruction Frequency(Fosc) = 2MHz 
//	==> 2 Million instructions per second (MIPS)
//	==> Approx 2,000 instructions per millisecond

count_low = count_high = 0;

do
{
// Delay 1ms
Delay1KTCYx(FOSC*2);
	
if (*port & pin == LOW){
	count_low++;
	count_high = NULL;
	}	
	else{
	count_low = NULL;
	count_high++;
	}

//Wait till pin has been at same level for 20ms
}while(count_low< COUNT_TARGET && count_high < COUNT_TARGET);

if (count_low >= COUNT_TARGET){
	return LOW;
	}	
	else{
	return HIGH;
	}
}