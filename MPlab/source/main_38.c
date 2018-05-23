/*********************************************************************
Nome do arquivo:	main_38.c            
Data:	    		17 de maio de 2010          
Versao:				1.0                              
MPLAB IDE:			v8.20a 
Autor:				Wagner Zanco              
*********************************************************************/
#include <p18f4520.h>		//diretiva de compilação
#include <delays.h>			//diretiva de compilação
#include <stdio.h>			//diretiva de compilação
#include "Lcd_8bits.h"		//diretiva de compilação
//********************************************************************
//protótipos de funções
void Inic_Regs (void);
void Atual_LCD (void);
void Configura_AD	(void);
int Conv_AD	(void);
char *Conv_Float_String(float float_in);
char Detecta_fim_escrita(void);
void configura_SPI (void);
void Transmite_Comando_SPI (unsigned char Comand, unsigned char dado);
//********************************************************************
//variáveis globais
char buf [17];							//declaração de vetor
char buf2 [17];							//declaração de vetor
float temp=0;							//declaração  de variável inicializada
unsigned char Pos_cursor=0;				//declaração  de variável inicializada
//********************************************************************
//definições
#define	AJUSTA_DECIMAL	10				//precisão de uma casa decimal
#define Botao1	PORTBbits.RB0			//define Botao 1
#define _SS		PORTCbits.RC1			//define linha chip select	
//********************************************************************
void main(void)							//função main					
{	
float x=0;								//declaração  de variável local inicializada
int dly=0;								//declaração  de variável local inicializada
char status_botao=0;					//declaração  de variável local inicializada
unsigned char Comando=0x11;				//declaração  de variável local inicializada
//**********************************
	Inic_Regs ();						//configurar SFRs
//**********************************
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
//**********************************/
	configura_SPI();					//configura interface SPI como mestre
	Configura_AD();						//configura conversor A/D
//**********************************
	Transmite_Comando_SPI (Comando, Pos_cursor);						
	temp = (float)(Conv_AD()*4.88/1000);//chamada à função com retorno de valor
	Conv_Float_String(temp);			//chamada à função
	Atual_LCD();						//atualiza LCD
	Pos_cursor +=25;					//atualiza posição do cursor
//**********************************/
//rotina principal
	while (1)										//loop infinito
	{
		if(!Botao1) 								//Botão 1 pressionado?
		{											//sim, executa bloco de código
			if(!status_botao)						//Botão 1 foi tratado?
			{										//não, executa bloco de código
				status_botao=1;						//sinaliza que Botão foi tratado
				Transmite_Comando_SPI (Comando, Pos_cursor);	//chamada à função. Atualiza posição do cursor						
				temp = (float)((Conv_AD()*4.88/1000));		//temp recebe o valor convertido pelo conversor A/D
				Conv_Float_String(temp);			//chamada à função
				Atual_LCD();						//atualiza LCD
				if(Pos_cursor==250)Pos_cursor=0;	//Se cursor chegou a posição 250retorna a posição 0
				else Pos_cursor +=25;				//não, incrementa cursor 10 posições
			}
		} 
		else status_botao=0;						//sinaliza que Botão foi tratado
		Delay1KTCYx(100);							//delay de 20ms
	}
}//********************************************************************
//							funções
//*******************************************************************
/*Esta funcao inicializa os resgistradores SFRs.*/
 void Inic_Regs (void)
{
	TRISA = 0x01;						//RA0 como entrada e demais pinos do PORTA como saída
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
//esta função efetua uma conversão A/D
int Conv_AD (void)
{
int Result_AD;									//declaração de variável local
	ADCON0bits.GO = 1;							//inicia conversão
	while (ADCON0bits.GO);						//aguarda finalizar conversão
	Result_AD = (((int)ADRESH)<<8)|(ADRESL);	//obtém valor da conversão 
	return Result_AD;							//retorna valor da conversão
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
	sprintf(buf," Tensao: %c%u.%01uV",sinal,parte_inteira,parte_decimal);		//converte valor em string e armazena no vetor buf	
	pt = buf;							//passa para o ponteiro pt o endereço de buf
	return (pt);						//retorna o endereço de buf.
}//******************************************************************

//********************************************************************
//Configuração do modulo SPI  como mestre com taxa de 125kbps
void  configura_SPI  (void)
{
	TRISCbits.TRISC5 = 0;				//configura SDO como saída
	TRISCbits.TRISC4 = 1;				//configura SDI como entrada
	TRISCbits.TRISC3 = 0;				//configura SCK como saída
	TRISCbits.TRISC1 = 0;				//configura linha _SS como saída
	_SS = 1;							//desabilita chip select
	SSPCON1 =  0b00100010;				/*habilita módulo SPI <5> 										
										bit rate 125kbps (Fosc/64)<3:0> 
										clock ocioso em nível baixo<4>*/						
	SSPSTAT = 0b11000000;				/*dado de entrada é amostrado no fim do time bit do dado de saída<7>
										transmissão ocorre do estado ocioso para o estado ativo do clock <6>*/
}//*********************************************************************
/*função  que  atualiza o LCD. Será mostrado no LCD o código e
a tensão entre o cursor e o terra do DPOT.*/
void Atual_LCD (void)
{

	EscInstLCD(0x01);					//limpa display e mostra cursor piscando na primeira posição da primmeira linha
	while(TesteBusyFlag());				//espera LCD controller terminar de executar instrução

		sprintf(buf2,"Codigo: %u",Pos_cursor);	//converte valor em string e armazena no vetor buf	
		EscStringLCD(buf2);						//escreve string no LCD
		while(TesteBusyFlag());					//espera LCD controller terminar de executar instrução


		EscInstLCD(0xC0);				//posiciona cursor na primeira posição da segunda linha
		while(TesteBusyFlag());			//espera LCD controller terminar de executar instrução

		EscStringLCD(buf);				//escreve string no LCD
		while(TesteBusyFlag());			//espera LCD controller terminar de executar instrução
}//*******************************************************************
//transmite e recebe um byte de dados  
unsigned char  Transmite_SPI  (unsigned char dado)
{
	SSPBUF = dado;						//inicia transmissão	
	while(!SSPSTATbits.BF);				//aguarde byte chegar
	dado = SSPBUF; 						//leitura burra
	return dado;						//retorna o dado recebido
}//*********************************************************************
//transmite comando para o MCP41010
void Transmite_Comando_SPI (unsigned char Comand, unsigned char dado)
{
	_SS = 0;							//habilita chip select
	Transmite_SPI (Comand);				//transmite comando
	Transmite_SPI (dado);				//transmite dado	
	_SS = 1;							//desabilita chip select
//*********************************************************************
}
