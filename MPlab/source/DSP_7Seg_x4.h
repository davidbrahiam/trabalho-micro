/*********************************************************************
Nome do arquivo:	DSP_7Seg_x4.h            
Data:	    		14 de março de 2010          
Versao:				1.0                              
MPLAB IDE:			v8.20a 
Autor:				Wagner Zanco              
*********************************************************************/
#ifndef __DSP_7SEGx4_H
#define __DSP_7SEGx4_H
//********************************************************************
#include <p18cxxx.h>		//diretiva de compilação
//********************************************************************
//definições
#define DSP_1 	PORTAbits.RA5
#define DSP_2 	PORTAbits.RA2
#define DSP_3 	PORTEbits.RE0
#define DSP_4 	PORTEbits.RE2
#define L_DADOS TRISD
#define DIR_A1 	TRISAbits.TRISA5
#define DIR_A2 	TRISAbits.TRISA2
#define DIR_A3 	TRISEbits.TRISE0
#define DIR_A4 	TRISEbits.TRISE2
//********************************************************************
void Aciona_DPS_7_seg (unsigned char Dsp4, unsigned char Dsp3, unsigned char Dsp2, unsigned char Dsp1);
#endif