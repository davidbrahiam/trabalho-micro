/*********************************************************************
Nome do arquivo:	main_31.c            
Data:	    		29 de março de 2010          
Versao:				1.0                              
MPLAB IDE:			v8.20a 
Autor:				Wagner Zanco              
*********************************************************************/
#include <p18f4520.h>		//diretiva de compilação
#include "DSP_7Seg_x4.h"	//diretiva de compilação
#include <delays.h>			//diretiva de compilação
//********************************************************************
//protótipos de funções
 void Inic_Regs (void);
//********************************************************************
void main(void)										//função main					
{	
unsigned char Dsp1=0;								//declaração  de variável local inicializada
unsigned char Dsp2=0;								//declaração  de variável local inicializada
unsigned char Dsp3=0;								//declaração  de variável local inicializada
unsigned char Dsp4=0;								//declaração  de variável local inicializada
unsigned char x=0;
	Inic_Regs ();									//chamada a funcão: configurar SFRs
	while(1)										//loop infinito
	{
		Aciona_DPS_7_seg (Dsp4, Dsp3, Dsp2, Dsp1);	//chamada à função: atualizar display
		Delay1KTCYx(8);								//delay de 4ms
		if(!(x==250))x++;							//se x diferente de 250, incrementa x
		else 										//senão, incrementa contador
		{
			x=0;									//x=0
			Dsp1+=1;								//incrementa unidade
			if(Dsp1==10)							//unidade estourou?
			{
				Dsp1=0;								//sim, zera unidade
				Dsp2+=1;							//incrementa dezena
			}
			if(Dsp2==10)							//dezena estourou?
			{
				Dsp2=0;								//sim, zera dezena
				Dsp3+=1;							//incrementa centena
			}
			if(Dsp3==10)							//centena estourou?
			{
				Dsp3=0;								//sim, zera centena
				Dsp4+=1;							//incrementa unidade de milhar
			}
			if(Dsp4==10)							//unidade de milhar estourou?
			{
				Dsp4=Dsp3=Dsp2=Dsp1=0;				//sim, zera contador
			}
		}
	}
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
