/*********************************************************************
Nome do arquivo:	main_29.c            
Data:	    		28 de dezembro de 2008          
Versao:				1.0                              
MPLAB IDE:			v8.10 
Autor:				Wagner Zanco              
*********************************************************************/
#include <p18f4550.h>		//diretiva de compilação
#include "MCC18_18F4550.h"

//********************************************************************
//protótipos de funções
 void Inic_Regs (void);
//********************************************************************
void main(void)					//função main					
{	
    unsigned char A, B, C;
    
    A = 0b11110011;
    B = 0b00110011;
    
    C = A & B;
    
    C = A | B;
    
    C = ~A;
    
	Inic_Regs ();				//chamada a funcão
    
	while (1);				//loop infinito
//	{
////		if (PORTBbits.RB2==0)PORTD = 0b00000001;			//led3 emite se botão BT1 estiver pressionado
//////		else	if (PORTBbits.RB3==0)PORTD = 0b00000010;	//se não, led2 emite se botão BT2 estiver pressionado
//////		else	if (PORTBbits.RB4==0)PORTD = 0b00000100;	//se não, led1 emite se botão BT3 estiver pressionado
//////        else	if (PORTBbits.RB5==0)PORTD = 0b00001000;	//se não, led1 emite se botão BT3 estiver pressionado
////		else 	PORTD = 0x00;	 					//se não, acende todos os leds se nenhum botão estiver pressionado					
//	}
}
/*******************************************************************
Esta funcao inicializa os resgistradores SFRs.*/
 void Inic_Regs (void)
{
	TRISA = 0x00;			//PORTA saída
	TRISB = 0x3C;			//pinos RB5:RB2 entrada e demais pinos do PORTB saída
	TRISC = 0x00;			//PORTC saida
	TRISD = 0x00;			//PORTD saida
	TRISE = 0x00;			//PORTE saida
	ADCON1 = 0x0F;			//configura pinos dos PORTA e PORTE como digitais
	PORTA = 0;				//limpa PORTA
	PORTB = 0;				//limpa PORTB
	PORTC = 0;				//limpa PORTC
	PORTD = 0xFF;			//apaga LEDs
	PORTE = 0;				//limpa PORTE
//********************************************************************
}
