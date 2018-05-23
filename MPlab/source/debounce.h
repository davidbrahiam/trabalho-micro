/***************************************************************
*  Name    : debounce.h                                        *
*  Author  : Owen O' Hehir                                     *
*  Notice  : Copyright (c) 2007                                *
*          : All Rights Reserved                               *
*  Date    : 19/6/2007                                         *
*  Version : 1.1                                               *
*  Notes   : To debounce a switch						       *
****************************************************************
*/

#ifndef debounce_header_code
#define debounce_header_code

#include <delays.h>
#include "standard.h"	/*Personnel Standard header*/

// Function prototypes
unsigned char debounce (unsigned char *port,unsigned char pin);
	//Arg: Port & pin number
	//Purpose: Debounce a switch
	//Return: Level of pin when asserted

#endif