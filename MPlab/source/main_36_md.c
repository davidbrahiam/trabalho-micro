/*********************************************************************
Nome do arquivo:	main_36.c            
Data:	    		17 de maio de 2010          
Versao:				1.0                              
MPLAB IDE:			v8.20a 
Autor:				Wagner Zanco              
*********************************************************************/
//#include <p18f4550.h>		//diretiva de compilação
#define _XTAL_FREQ 8000000
#include <xc.h>
#include "__MCC18__4550__.h"          //diretiva de compilação
#include <delays.h>			//diretiva de compilação
#include <stdio.h>			//diretiva de compilação
#include "Lcd_8bits.h"		//diretiva de compilação
//********************************************************************
//protótipos de funções
void Inic_Regs (void);
void Atual_LCD (void);
void Confg_PWM (void);
void Config_Duty_Cycle (int x);
//********************************************************************
//variáveis globais
char buf [17];
//char buf2 [17];
char z=0;
//********************************************************************
//definições
#define Botao1	PORTBbits.RB0
//********************************************************************
void main(void)							//função main					
{	
float temp=0;							//declaração  de variável local inicializada
int dly=0;								//declaração  de variável local inicializada
char status_botao=0;					//declaração  de variável local inicializada
unsigned char count=0;					//declaração  de variável local inicializada
//**********************************
	Inic_Regs ();						//configurar SFRs
	IniciaLCD (2);						//inicializar LCD controller HD44780
	TestPixelsLCD();					//teste no LCD - acende todos os pixels.

	EscInstLCD(0x0C);					//desativa cursor
	while(TesteBusyFlag());				//espera LCD controller terminar de executar instrução
//**********************************
//delay de 3 segundos
	for(dly=0;dly<600;dly++)			//comando de iteração
	{
		_Delay5ms();					//delay de 5ms
	}
//**********************************
	EscInstLCD(0x01);					//desativa cursor
	while(TesteBusyFlag());				//espera LCD controller terminar de executar instrução

	sprintf(buf,"PWM desligado");		//converte valor em string e armazena no vetor buf	
	EscStringLCD(buf);					//escreve string no LCD
	while(TesteBusyFlag());				//espera LCD controller terminar de executar instrução
//**********************************/	
//rotina principal
	while (1)
	{
		if(!Botao1) 						//Botão 1 pressionado?
		{									//sim, executa bloco de código
			if(!status_botao)				//Botão 1 foi tratado?
			{								//não, executa bloco de código
				status_botao=1;				//sinaliza que Botão foi tratado
			 	if(z==0)					//PWM está desligado?
				{							//sim, executa bloco de código
					Confg_PWM ();			//ativa PWM com duty cycle de 50%
					z=1;					//PWM ligado com duty cycle de 50%
				}
				else if(z==1)				//PWM está ligado com duty cycle de 50%? 
				{							//sim, executa bloco de código
					Config_Duty_Cycle (100);//atualiza duty cycle para 100%
					z+=1;					//PWM ligado com duty cycle de 100%
				}
				else if(z==2)				//PWM ligado com duty cycle de 100%?
				{
					CCP1CON=0;				//sim, desliga PWM 
					z=0;					//PWM desligado	
				}
			}
		} 
		else status_botao=0;				//sinaliza que Botão foi tratado
		Delay1KTCYx(100);					//delay de 20ms
		count+=1;							//incrementa count
		if(count==50)
		{
			Atual_LCD();					//atualiza LCD a cada segundo
			count=0;						//zera count
		}
	}
}//******************************************************************
//							funções
//*******************************************************************
/*Esta funcao inicializa os resgistradores SFRs.*/
 void Inic_Regs (void)
{
	TRISA = 0x00;						//PORTA saída
	TRISB = 0x01;						//RB0 como entrada e demais pinos do PORTB como saída
	TRISC = 0x00;						//PORTC saída
	TRISD = 0x00;						//PORTD saída
	TRISE = 0x00;						//PORTE saída
	ADCON1 = 0x0F;						//configura pinos dos PORTA e PORTE como digitais
	PORTA = 0;							//limpa PORTA
	PORTB = 0;							//limpa PORTB
	PORTC = 0;							//limpa PORTC
	PORTD = 0x00;						//apaga displays
	PORTE = 0;							//limpa PORTE
}
//********************************************************************
void Atual_LCD (void)
{
	EscInstLCD(0x01);						//limpa display e mostra cursor piscando na primeira posição da primmeira linha
	while(TesteBusyFlag());					//espera LCD controller terminar de executar instrução


	if(z==0)								//PWM desligado?
	{
		sprintf(buf,"PWM desligado");		//sim, converte valor em string e armazena no vetor buf	
		EscStringLCD(buf);					//escreve string no LCD
		while(TesteBusyFlag());				//espera LCD controller terminar de executar instrução
	}
	else if(z==1)							//PWM ligado com duty cycle de 50%?
	{
		sprintf(buf," PWM ligado");			//sim, converte valor em string e armazena no vetor buf	
		EscStringLCD(buf);					//escreve string no LCD
		while(TesteBusyFlag());				//espera LCD controller terminar de executar instrução

		EscInstLCD(0xC0);					//limpa display e mostra cursor piscando na primeira posição da primmeira linha
		while(TesteBusyFlag());				//espera LCD controller terminar de executar instrução

		sprintf(buf,"duty cycle 50%");		//converte valor em string e armazena no vetor buf	
		EscStringLCD(buf);					//escreve string no LCD
		while(TesteBusyFlag());				//espera LCD controller terminar de executar instrução
	}

	else if(z==2)							//PWM ligado com duty cycle de 100%?
	{
		sprintf(buf,"PWM ligado");			//sim, converte valor em string e armazena no vetor buf	
		EscStringLCD(buf);					//escreve string no LCD
		while(TesteBusyFlag());				//espera LCD controller terminar de executar instrução

		EscInstLCD(0xC0);					//limpa display e mostra cursor piscando na primeira posição da primmeira linha
		while(TesteBusyFlag());				//espera LCD controller terminar de executar instrução

		sprintf(buf,"duty cycle 100%");		//converte valor em string e armazena no vetor buf	
		EscStringLCD(buf);					//escreve string no LCD
		while(TesteBusyFlag());				//espera LCD controller terminar de executar instrução
	}

}//*******************************************************************
//Ativa sinal PWM com um duty cycle de 50%. 
//Fosc=8MHz e FPWM=15625Hz.
void Confg_PWM (void)
{
	PR2 = 127;								//período do sinal PWM = 15625Hz
	CCPR1L = 0b01000000;
	CCP1CONbits.DC1B1 = 0;
	CCP1CONbits.DC1B0 = 0;					//bits de controle = 256
	TRISCbits.RC2 = 0;						//configura pino de saída do sinal PWM como saída
//*******************************************************************						
//configura TMR2 para operar como temporizador e estourar a cada 4ms
	T2CON = 0b00000100;					//fator de postscaler de 1:1 <6:3>
										//fator de prescaler de 1:1<1:0>
										//liga TMR2<1>
	TMR2 = 0;							//inicializa TMR2
//*******************************************************************	
	CCP1CON *= 0b00110000;
	CCP1CON += 0b00001100;				//ativa modo PWM
}//******************************************************************
//esta função atualiza o duty cycle do sinal PWM
void  Config_Duty_Cycle (int x)
{
int temp;								//declaração de variável local
	if(x>100)x=100;						//ajusta valor de x
	x = (x*100)/512;					//obtém valor dos bits de controle
	temp =  x >> 2;						//obtém os seis bits mais significativos						
	CCPR1L = temp;						//carrega bits de controle mais significativos 
//********************
//carrega segundo bit de controle menos significativo
	temp = x & 2;
	if (temp==1)CCP1CONbits.DC1B1 = 1;			
	else 	CCP1CONbits.DC1B1 = 0;
//********************
//carrega bit de controle menos significativo
	temp = x & 1;
	if (temp==1)CCP1CONbits.DC1B0 = 1;
	else 	CCP1CONbits.DC1B0 = 0;
}//*******************************************************************
