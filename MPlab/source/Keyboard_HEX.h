/*********************************************************************
Nome do arquivo:	Keyboard_HEX.h            
Data:	    		14 de março de 2010          
Versao:				1.0                              
MPLAB IDE:			v8.20a
Autor:				Wagner Zanco              
*********************************************************************/
#ifndef __KEY_H
#define __KEY_H
//**********************************************************************
//definições  
#define Out 0		// definição de direção de I/O
#define In 1		// definição de direção de I/O
//**********************************************************************
//definições do teclado
#define x4 	PORTBbits.RB0 
#define x3 	PORTBbits.RB1
#define x2 	PORTBbits.RB2
#define x1 	PORTBbits.RB3
#define y4 	PORTBbits.RB7
#define y3 	PORTBbits.RB6
#define y2 	PORTBbits.RB5
#define y1	PORTBbits.RB4

#define x4_DIR  TRISBbits.TRISB0
#define x3_DIR	TRISBbits.TRISB1
#define x2_DIR	TRISBbits.TRISB2
#define x1_DIR	TRISBbits.TRISB3
#define y4_DIR	TRISBbits.TRISB7
#define y3_DIR	TRISBbits.TRISB6
#define y2_DIR	TRISBbits.TRISB5
#define y1_DIR	TRISBbits.TRISB4
//************************************************************
//protótipos de funções
unsigned char GetKey(void);
//************************************************************
#endif