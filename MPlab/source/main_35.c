/*********************************************************************
Nome do arquivo:	main_35.c            
Data:	    		17 de maio de 2010          
Versao:				1.0                              
MPLAB IDE:			v8.20a 
Autor:				Wagner Zanco              
*********************************************************************/
#include <p18f4520.h>		//diretiva de compilação
#include "MCC18.h"          //diretiva de compilação
#include <delays.h>			//diretiva de compilação
#include <stdio.h>			//diretiva de compilação
#include "Lcd_8bits.h"		//diretiva de compilação
//********************************************************************
//protótipos de funções
void Inic_Regs (void);
void Configura_AD	(void);
int Conv_AD	(void);
void Atual_LCD (void);
char *Conv_Float_String(float float_in);
//********************************************************************
//variáveis globais
char buf [17];
//********************************************************************
//definições
#define	AJUSTA_DECIMAL	10				//precisão de uma casa decimal
#define Botao1			PORTBbits.RB0
#define Resistencia		PORTCbits.RC1
//********************************************************************
void main(void)							//função main					
{	
float x=0;								//declaração  de variável local inicializada
char y=0;								//declaração  de variável local inicializada
char i=0;								//declaração  de variável local inicializada
float temp=0;							//declaração  de variável local inicializada
int dly=0;
//**********************************
	Inic_Regs ();						//configurar SFRs
//*
	IniciaLCD (2);						//inicializar LCD controller HD44780
	TestPixelsLCD();					//teste no LCD - acende todos os pixels.
	EscInstLCD(0x0C);								//desativa cursor
	while(TesteBusyFlag());							//espera LCD controller terminar de executar instrução
//**********************************
//delay de 3 segundos
	for(dly=0;dly<600;dly++)			//comando de iteração
	{
		_Delay5ms();					//delay de 5ms
	}
//**********************************/
Configura_AD();
//**********************************
//rotina principal
	while (1)
	{
		for(i=0;i<=49;i++)				//laço de iteração
		{
			if(!Botao1) Resistencia =1;	//liga a resistência se o botão estiver pressionado
			else Resistencia =0;		//senão, desliga resistência
			x = (float)(Conv_AD()/2);	//chamada à função com retorno de valor
			temp += x;					//obtém temperatura medida
			Delay1KTCYx(40);			//delay de 20ms
		}
		Conv_Float_String(temp/50);		//chamada à função
		Atual_LCD ();					//chamada à função
		temp=0;							//temp = 0
	}
}
//*******************************************************************
/*Esta funcao inicializa os resgistradores SFRs.*/
 void Inic_Regs (void)
{
	TRISA = 0x01;						//PORTA saída
	TRISB = 0x01;						//PORTB saída
	TRISC = 0x00;						//PORTC saída
	TRISD = 0x00;						//PORTD saída
	TRISE = 0x00;						//PORTE saída
	ADCON1 = 0x0F;						//configura pinos dos PORTA e PORTE como digitais
	PORTA = 0;							//limpa PORTA
	PORTB = 0;							//limpa PORTB
	PORTC = 0;							//limpa PORTC
	PORTD = 0;						    //limpa PORTD
	PORTE = 0;							//limpa PORTE
}//********************************************************************
//esta função configura o conversor A/D
void Configura_AD	(void)
{
	ADCON0=0b00000001;					/*canal AN0 selecionado<5:2>
										Módulo conversor ligado<1>*/
	ADCON1=0b00001110;					/*Vref- = VSS<5>
										Vref+ = VDD<4>
										pino RA0/AN0 analógico e demais pinos digitais<3:0>*/
	ADCON2=0b10101001;					/*resultado justificado à direita<7>
										Tempo de aquisição de 12TAD<5:3>
										TAD = 1µs<2:0>*/
}//********************************************************************
//esta função efetua uma ocnversão A/D
int Conv_AD (void)
{
int Result_AD;							//declaração de variável local
	ADCON0bits.GO = 1;					//inicia conversão
	while (ADCON0bits.GO);				//aguarda finalizar conversão
	Result_AD = (((int)ADRESH)<<8)|(ADRESL);	//obtém valor da conversão 
	return Result_AD;					//retorna valor da conversão
}//********************************************************************
/********************************************************************/
/*Conversão de float para ASCII. Esta função converte valores float 
na faixa de -65535.998 a +65535.998 em uma string. 
//*******************************************************************/
char *Conv_Float_String(float float_in)
{
unsigned int parte_inteira;				//declaração de variável local
unsigned int parte_decimal;				//declaração de variável local
char sinal, *pt;						//declaração de variável local
unsigned char x, y;						//declaração de variável local
unsigned char z = 0xB2;

//*******************************************************************
	if(float_in < 0)					//valor é menor que 0?
	{
		sinal = '-';					//sim, sinal negativo
		float_in = float_in * (-1);		//inverte o sinal de float_in
	}
	else sinal = ' ';					//não, sinal positivo
//*******************************************************************
	parte_inteira = float_in;			//resgata parte inteira do valor
//*******************************************************************
	parte_decimal =(unsigned int)((float_in - parte_inteira) * AJUSTA_DECIMAL);	//resgata parte fracionária do valor
	sprintf(buf," Temp =%c%u.%01u*C",sinal,parte_inteira,parte_decimal);		//converte valor em string e armazena no vetor buf	
	pt = buf;							//passa para o ponteiro pt o endereço de buf
	return (pt);						//retorna o endereço de buf.
}//******************************************************************
//esta função atualiza o LCD
void Atual_LCD (void)
{
	EscInstLCD(0x01);					//limpa display e mostra cursor piscando na primeira posição da primeira linha
	while(TesteBusyFlag());				//espera LCD controller terminar de executar instrução

	EscStringLCD(buf);					//escreve string no LCD
	while(TesteBusyFlag());				//espera LCD controller terminar de executar instrução
}//******************************************************************
