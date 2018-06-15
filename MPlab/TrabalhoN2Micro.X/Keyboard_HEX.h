/*********************************************************************
Nome do arquivo:	Keyboard_HEX.h            
Data:	    		14 de mar�o de 2010          
Versao:				1.0                              
MPLAB IDE:			v8.20a
Autor:				Wagner Zanco              
*********************************************************************/
#ifndef __KEY_H
#define __KEY_H
//**********************************************************************
//defini��es  
#define Out 0		// defini��o de dire��o de I/O
#define In 1		// defini��o de dire��o de I/O
//**********************************************************************
//defini��es do teclado
#define x4 	PORTDbits.RD0 
#define x3 	PORTDbits.RD1
#define x2 	PORTDbits.RD2
#define x1 	PORTDbits.RD3
#define y4 	PORTDbits.RD7
#define y3 	PORTDbits.RD6
#define y2 	PORTDbits.RD5
#define y1	PORTDbits.RD4

#define x4_DIR  TRISDbits.TRISD0
#define x3_DIR	TRISDbits.TRISD1
#define x2_DIR	TRISDbits.TRISD2
#define x1_DIR	TRISDbits.TRISD3
#define y4_DIR	TRISDbits.TRISD7
#define y3_DIR	TRISDbits.TRISD6
#define y2_DIR	TRISDbits.TRISD5
#define y1_DIR	TRISDbits.TRISD4
//************************************************************
//prot�tipos de fun��es
unsigned char GetKey(void);
//************************************************************
#endif