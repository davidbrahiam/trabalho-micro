/*********************************************************************
Nome do arquivo:	main_35.c            
Data:	    		19 Maio de 2018          
Versao:				1.1                              
MPLAB IDE:			MPLAB-X v.4.15 
Autor:				Wagner Zanco
Modificado por:     Carlúcio Silva     
Descrição: Configuração do conversor A/D para operar com clock de 4 MHz,
com VREF- igual a GND/VSS, VREF+ igual a VCC, no modo "Automático de
Conversão",  
*********************************************************************/

/**** Arquivos de cabeçalho ****/
#include <p18f4550.h>		//diretiva de compilação
#include "MCC18_18F4550.h"  //diretiva de compilação
#include <delays.h>			//diretiva de compilação
#include <stdio.h>			//diretiva de compilação
#include "Lcd_8bits.h"		//diretiva de compilação


/**** Protótipos de funções ****/
void configSFR(void);
void configAD(void);
int convAD(void);
char *convFloatString(float);

/**** Variáveis globais ****/
char buf [17];


/**** Definições ****/
#define	AJUSTA_DECIMAL	10	//precisão de uma casa decimal


void main(void) //função main					
{	
    //Variáveis locais
    float x=0;				
    int dly=0;

/****** Configuração dos SFRs *******/    
	configSFR();						
    
/******* Inicialização do LCD *******/ 
	IniciaLCD(2);						//inicializar LCD controller HD44780
	TestPixelsLCD();					//teste no LCD - acende todos os pixels.
	EscInstLCD(0x0C);					//desativa cursor
	while(TesteBusyFlag());				//espera LCD controller terminar de executar instrução
/***********************************/
    
//delay de 3 segundos
	for(dly=0;dly<600;dly++)			//comando de iteração
	{
		_Delay5ms();					//delay de 5ms
	}
//**********************************/
configAD();

    EscInstLCD(0x01);           //limpa display e mostra cursor piscando na primeira linha
    while(TesteBusyFlag());		//espera LCD controller terminar de executar instrução
    sprintf(buf,"TEMPERATURA"); //armazena string no vetor buf
    EscStringLCD(buf);			//escreve string no LCD
    while(TesteBusyFlag());		//espera LCD controller terminar de executar instrução

//Rotina principal
	while (1)
	{
        
        x = (float)(convAD()/2.048); //Faz a conversão A/D
		Delay1KTCYx(20);			 //delay de 20ms        
        convFloatString(x);         //Converte o valor da Temperatura para string
                                    //e salva o resultado em buf[]        
        EscInstLCD(0xC0);           //Posiciona o cursor na segunda linha do LCD
        while(TesteBusyFlag());     //Espera LCD controller terminar de executar instrução
        EscStringLCD(buf);			//Escreve string no LCD
        while(TesteBusyFlag());		//Espera LCD controller terminar de executar instrução    
        Delay10KTCYx(40);
	}
}

/*******************************************************************************
Esta função inicializa os registradores SFRs.*/
 void configSFR(void)
{
	TRISA = 0x01;		//RA0 como entrada e os demais de PORTA como saída
	TRISB = 0x00;		//PORTB saída
	TRISC = 0x00;		//PORTC saída
	TRISD = 0x00;		//PORTD saída
	TRISE = 0x00;		//PORTE saída
	ADCON1 = 0x0F;		//configura pinos dos PORTA e PORTE como digitais
	PORTA = 0;			//limpa PORTA
	PORTB = 0;			//limpa PORTB
	PORTC = 0;			//limpa PORTC
	PORTD = 0;  		//limpa PORTC
	PORTE = 0;			//limpa PORTE  
}
 
/*******************************************************************************
Esta função configura o conversor A/D.*/ 
void configAD(void)
{
	ADCON0=0b00000001;					/*canal AN0 selecionado<5:2>
										Módulo conversor ligado<1>*/
	ADCON1=0b00001110;					/*Vref- = VSS<5>
										Vref+ = VDD<4>
										pino RA0/AN0 analógico e demais pinos digitais<3:0>*/
	ADCON2=0b10101100;					/*resultado justificado à direita<7>
										Tempo de aquisição de 12TAD<5:3>
										TAD = 1µs<2:0>*/
}

/*******************************************************************************
Esta função efetua uma ocnversão A/D.*/
int convAD(void)
{
int Result_AD;							//declaração de variável local
	ADCON0bits.GO = 1;					//inicia conversão
	while (ADCON0bits.GO);				//aguarda finalizar conversão
	Result_AD = (((int)ADRESH)<<8)|(ADRESL);	//obtém valor da conversão 
	return Result_AD;					//retorna valor da conversão
}

/********************************************************************/
/*Conversão de float para ASCII. Esta função converte valores float 
na faixa de -65535.998 a +65535.998 em uma string. 
//*******************************************************************/
char *convFloatString(float inputFloat)
{
    unsigned int parte_inteira;				//declaração de variável local
    unsigned int parte_decimal;				//declaração de variável local
    char sinal, *pt;						//declaração de variável local
    unsigned char x, y;						//declaração de variável local
    unsigned char z = 0xB2;

//*******************************************************************
	if(inputFloat < 0)					//valor é menor que 0?
	{
		sinal = '-';					//sim, sinal negativo
		inputFloat = inputFloat * (-1);		//inverte o sinal de float_in
	}
	else sinal = '+';					//não, sinal positivo
    
//*******************************************************************
	parte_inteira = inputFloat;			//resgata parte inteira do valor
    
//*******************************************************************
	parte_decimal =(unsigned int)((inputFloat - parte_inteira) * AJUSTA_DECIMAL);	//resgata parte fracionária do valor
	sprintf(buf," Temp = %c%u.%01u *C",sinal,parte_inteira,parte_decimal);		//converte valor em string e armazena no vetor buf	
	pt = buf;							//passa para o ponteiro pt o endereço de buf
	return (pt);						//retorna o endereço de buf.
}


