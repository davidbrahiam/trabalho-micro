/*********************************************************************
Nome do arquivo:	main_11.c            
Data:	    		28 de dezembro de 2008          
Versao:				1.0                              
MPLAB IDE:			v8.20a 
Autor:				Wagner Zanco              
*********************************************************************/
#include <p18f4520.h>		//diretiva de compilação
/*******************************************************************
Esta funcao inicializa os resgistradores SFRs.*/
 void Inic_Regs (void)
{
	TRISA = 0x00;			//PORTA saída
	TRISB = 0x07;			//RB2:RB0 entrada e demais pinos do PORTB saida
	TRISC = 0x00;			//PORTC saida
	TRISD = 0x00;			//PORTD saida
	TRISE = 0x00;			//PORTE saida
	ADCON1 = 0x0F;			//configura PORTA e PORTE como digitais
	PORTA = 0;				//limpa PORTA
	PORTB = 0;				//limpa PORTB
	PORTC = 0;				//limpa PORTC
	PORTD = 0;				//limpa PORTD
	PORTE = 0;				//limpa PORTE
}
//********************************************************************
void main(void)		//função main					
{	
	Inic_Regs ();								//chamada a funcão
	while (1)									//loop infinito
	{
		if(PORTB & 0x01)PORTD = 0x7F;			//seta pino RD7 se pino RB0 = 0
		else	if(PORTB & 0x02)PORTD = 0xBF;	//se não, limpa pino RD6 se pino RB1 = 0
		else	if(PORTB & 0x04)PORTD = 0xDF;	//se não, limpa pino RD5 se pino RB2 = 0
		else PORTD = 0xFF;						//se não, seta PORTD se RB2:RB0 = 111
	}							
//********************************************************************
}
