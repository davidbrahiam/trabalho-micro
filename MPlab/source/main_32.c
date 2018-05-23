/*********************************************************************
Nome do arquivo:	main_32.c            
Data:	    		29 de março de 2010          
Versao:				1.0                              
MPLAB IDE:			v8.20a 
Autor:				Wagner Zanco              
*********************************************************************/
#include <p18f4520.h>			//diretiva de compilação
//#include <delays.h>			//diretiva de compilação
#include "Lcd_8bits.h"			//diretiva de compilação
//********************************************************************
//protótipos de funções
 void Inic_Regs (void);
 //********************************************************************
void main(void)										//função main					
{	
char buf [17] = {"Seja bem vindo!"};				//declaração de vetor inicializado
char buf02 [17] = {"   LCD 16 x 2"};				//declaração de vetor inicializado
int dly=0;											//declaração de variável local inicializada
//**********************************
	Inic_Regs ();									//configurar SFRs
	IniciaLCD (2);									//inicializar LCD controller HD44780
	TestPixelsLCD();								//teste no LCD - acende todos os pixels.
//**********************************
//delay de 3 segundos
	for(dly=0;dly<600;dly++)						//comando de iteração
	{
		_Delay5ms();								//delay de 5ms
	}
//**********************************
	EscInstLCD(0x01);								//limpa display e mostra cursor piscando na primeira posição da primmeira linha
	while(TesteBusyFlag());							//espera LCD controller terminar de executar instrução

	EscStringLCD(buf);								//escreve string no LCD
	while(TesteBusyFlag());							//espera LCD controller terminar de executar instrução

	EscInstLCD(0xC0);								//posiciona cursor na primeir aposicção  da segunda linha
	while(TesteBusyFlag());							//espera LCD controller terminar de executar instrução

	EscStringLCD(buf02);							//escreve string no LCD					
	while(TesteBusyFlag());							//espera LCD controller terminar de executar instrução

	EscInstLCD(0x0C);								//desativa cursor
	while(TesteBusyFlag());							//espera LCD controller terminar de executar instrução
	while(1);										//loop infinito
}
/*******************************************************************
Esta funcao inicializa os resgistradores SFRs.*/
 void Inic_Regs (void)
{
	TRISA = 0x00;			//PORTA saída
	TRISB = 0x00;			//PORTB saída
	TRISC = 0x00;			//PORTC saída
	TRISD = 0x00;			//PORTD saída
	TRISE = 0x00;			//PORTE saída
	ADCON1 = 0x0F;			//configura pinos dos PORTA e PORTE como digitais
	PORTA = 0;				//limpa PORTA
	PORTB = 0;				//limpa PORTB
	PORTC = 0;				//limpa PORTC
	PORTD = 0x00;			//apaga displays
	PORTE = 0;				//limpa PORTE
//********************************************************************
}

