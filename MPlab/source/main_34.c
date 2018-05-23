/*********************************************************************
Nome do arquivo:	main_34.c            
Data:	    		25 abril de 2010          
Versao:				1.0                              
MPLAB IDE:			v8.20a 
Autor:				Wagner Zanco              
*********************************************************************/
#include <p18f4520.h>			//diretiva de compilação
#include <delays.h>				//diretiva de compilação
#include "DSP_7Seg_x4.h"		//diretiva de compilação
//********************************************************************
//protótipos de funções
void Inic_Regs (void);
void high_isr (void);
//********************************************************************
//variáveis globais
volatile unsigned char Dsp1=0;   //declaração  de variável global inicializada
volatile unsigned char Dsp2=0;	//declaração  de variável global inicializada
volatile unsigned char Dsp3=0;	//declaração  de variável global inicializada
volatile unsigned char Dsp4=0;	//declaração  de variável global inicializada
int x = 0;						//declaração  de variável global inicializada
//********************************************************************/
//vetor de interrupção de alta prioridade
#pragma code high_vector=0x08
void interrupt_at_high_vector(void)
{
	_asm GOTO high_isr _endasm		//desvia programa para rotina de tratamento da interrupção
}
#pragma code 
//*****************************
//Rotina de tratamento de interrupção (ISR)
#pragma interrupt high_isr 
void high_isr (void)
{
	if(!INTCONbits.TMR0IF);
	else
	{
		INTCONbits.TMR0IF=0;						//interrupção de estouro de TMR0?
		TMR0L = 5;									//inicializa TMR0
		Aciona_DPS_7_seg (Dsp4, Dsp3, Dsp2, Dsp1);	//chamada à função: atualizar display
		if(!(x==250))x++;							//se x diferente de 250, incrementa x
		else 										//senão, incrementa contador
		{
			x=0;									//x=0
			Dsp1+=1;								//incrementa unidade
			if(Dsp1==10)							//unidade estourou?
			{
			Dsp1=0;									//sim, zera unidade
			Dsp2+=1;								//incrementa dezena
			}
		if(Dsp2==10)								//dezena estourou?
		{
			Dsp2=0;									//sim, zera dezena
			Dsp3+=1;								//incrementa centena
		}
		if(Dsp3==10)								//centena estourou?
		{
			Dsp3=0;									//sim, zera centena
			Dsp4+=1;								//incrementa unidade de milhar
			}
			if(Dsp4==10)							//unidade de milhar estourou?
			{
				Dsp4=Dsp3=Dsp2=Dsp1=0;				//sim,zera contador
			}
		}
	}
}
//********************************************************************/
void main(void)										//função main				
{	
	Inic_Regs ();									//configurar SFRs
	while(1);										//loop infinito
}
/*******************************************************************
Esta funcao inicializa os registradores SFRs.*/
 void Inic_Regs (void)
{
	TRISA = 0x00;					//PORTA saída
	TRISB = 0x00;					//PORTB saída
	TRISC = 0x00;					//PORTC saída
	TRISD = 0x00;					//PORTD saída
	TRISE = 0x00;					//PORTE saída
	ADCON1 = 0x0F;					//configura pinos dos PORTA e PORTE como digitais
	PORTA = 0;						//limpa PORTA
	PORTB = 0;						//limpa PORTB
	PORTC = 0;						//limpa PORTC
	PORTD = 0x00;					//apaga displays
	PORTE = 0;						//limpa PORTE
//*****************************************
//habilita interrupção de estouro de TMR0
	INTCONbits.GIE = 1;				//liga chave geral de interrupção
	INTCONbits.TMR0IE = 1;			//liga chave individual de interrupção externa 0
	INTCON2bits.TMR0IP = 1;			//interrupção externa 0 ocorrerá na borda de descida
//********************************************************************
//configura TMR0 para operar como temporizador e estourar a cada 4ms
	T0CON = 0b11000100;				//Timer0 configurado como temporizador<5>
									//prescaler ativado<3>
									//fator de prescaler de 1:32<2:0>
									//liga TMR0
	TMR0L = 5;						//inicializa TMR0
}//*******************************************************************	
