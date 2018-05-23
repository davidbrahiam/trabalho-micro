/********************************************************************
;	Nome do arquivo:		LCD_8bits.c            
;	Data:				    10 de março de 2010          
;	Versao:		  			1.0                              
;	MPLAB IDE:				v8.20a  
;	Autor:				    Wagner Zanco              
*********************************************************************/

/********************************************************************	
Esta biblioteca contém um conjunto de funções que permitem ao microcontrolador
se comunicar com o LCD controller HD44780. 
//********************************************************************/
#include <p18cxxx.h>					//diretiva de compilação
#include <delays.h>						//diretiva de compilação
#include "LCD_8bits.h"					//diretiva de compilação
/********************************************************************
A função IniciaLCD() recebe como argumento um valor que irá inializar o LCD com:

valor = 1 -> inicializa o LCD com uma linha
valor != 1 -> inicializa o LCD com linha dupla

Quando o programa retorna ao ponto de chamada, o LCD mostra o cursor piscando
na primeira posição da primeira linha.
//*********************************************************************/
void IniciaLCD (unsigned char NL)
{
	const unsigned char Seq_Inic[3] = {0x0F, 0x06, 0x01};	//declaração de vetor inicizlizado
	unsigned char i;										//declaração de variável local
	char x;													//declaração de variável local
	_EN = 0;							//envia intrução
	_RS = 0;							//limpa pino enable
	_RW = 0;							//ativa ciclo de escrita
	ADCON1 = 0x0F;						//configura PORT de controle com digital
	TRIS_CONT_LCD = 0;					//configura PORT de controle como saída 
	TRIS_PORT_LCD = 0;					//configura PORT de dados como saída
//*******************************
//envia para o LCD o comando 0x30 três vezes	
	for(i=0;i<3;i++)
		{
		PORT_LCD = 0x30;				//comando 0x30
		Pulse();						//aplica pulso enable no LCD
		_Delay5ms();					//delay 5ms
		}
//*******************************
//configura linha simples ou linha dupla
	if(NL == 1)PORT_LCD =  0x30;		//se NL=1, ativa uma linha 
	else PORT_LCD =  0x38;				//se NL!=1, ativa duas linhas 
	Pulse();							//aplica pulso enable no LCD
	_Delay5ms();						//delay 5ms
//*******************************
/*configura:
- display ativo com cursor piscando
- desloca cursor para a direita na entrada de um novo caractere
- limpa display e retorna cursor a primeira posição da primeira linha
*/
	for(i=0;i<3;i++)
		{
		PORT_LCD = Seq_Inic[i];			//LCD recebe comando
		Pulse();						//aplica pulso enable no LCD
		_Delay5ms();					//delay 5ms
		}
	TRIS_PORT_LCD = 0xFF;				//configura PORT de dados como entrada
//**********************************************************************
}										//final  da função IniciaLCD
//**********************************************************************
//esta função escreve comando/dado no LCD
void Pulse(void)
	{
	DelayFor20TCY();					//delay de 20 ciclos de clock					
	_EN = 1;							//seta pino enable
	DelayFor20TCY();					//delay de 20 ciclos de clock					
	_EN = 0;							//limpa pino enable
	}
//**********************************************************************
//							funções de delay
//**********************************************************************
//delay de 100us
void _Delay100us(void)
{
	Delay100TCYx(2);					//delay 100us
}
//**********************************************************************
//delay de 5ms
void _Delay5ms(void)
{
	Delay10KTCYx(1);					//delay 5ms
}
//**********************************************************************
//delay 20 ciclos do oscilador principal
void DelayFor20TCY( void )
{
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
}
void DelayFor18TCY( void )
{
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
}

//*******************************************************************
//				funções de acesso ao LCD
//*******************************************************************
/*esta função fica aguardando o LCD controller terminar de executar
a instrução atual. ela retorna o  valor 0 quando a instrução terminar.*/
unsigned char TesteBusyFlag(void)
{
//	TRIS_PORT_LCD = 0xFF;				//configura PORT de dados como entrada

    _RW = 1;             				//ativa ciclo de leitura
	_RS = 0;							//ciclo de intrução			
    DelayFor20TCY();					//delay de 20 cliclos de clock
    _EN = 1;               		       	//seta pino enable
    DelayFor20TCY();					//delay de 20 cliclos de clock
	if(PORT_LCD&0x80)          	    	//leitura do bit busy flag
    {                          	     	//se bit busy == 1, LCD ocupado 
    	_EN = 0;              			//reseta pino enable
        _RW = 0;             			//reseta linha de escrita
        return 1;              		 	//LCD ocupado, retorna 1
    }
    else                     	        //se busy flag == 0, LCD livre
    {
        _EN = 0;              		    //reseta pino enable
        _RW = 0;             			//ativa ciclo de escrita
        return 0;               		//LCD livre, retorna 0
    }
//*******************************************************************
}										//final da função TesteBusyFlag
//*******************************************************************
//esta função escreve um caractere na pocição apontada pelo cursor.
void EscDataLCD(char _data)
{
	TRIS_PORT_LCD = 0;					//configura PORT de dados como saída
	PORT_LCD = _data;					//escreve dado
	_RS = 1;							//envia dado
	_RW = 0;							//ativa ciclo de escrita
	Pulse();							//aplica pulso enable no LCD
	_RS = 0;							//envia instrução
    DelayFor20TCY();					//delay de 20 ciclos de clock
	TRIS_PORT_LCD = 0xFF;				//configura PORT de dados como entrada
//*******************************************************************
}										//final da função EscDataLCD
//*******************************************************************
//esta função envia uma instrução para o LCD.
void EscInstLCD(unsigned char _inst)
{
	TRIS_PORT_LCD = 0;					//configura PORT de dados como saída
	PORT_LCD = _inst;					//escreve instrução
	_RS = 0;							//envia instrução
	_RW = 0;							//ativa ciclo de escrita
	Pulse();							//aplica pulso enable no LCD
	_RS = 0;							//envia dado
    DelayFor20TCY();					//delay de 20 ciclos de clock
	TRIS_PORT_LCD = 0xFF;				//configura PORT de dados como entrada
//*******************************************************************
}										//final da função EscInstLCD
//*******************************************************************/
/*esta função escreve no LCD uma string lida da memória RAM a partir 
da posição aponntada pelo cursor.*/
#pragma code My_codigo = 0x200
void EscStringLCD(char *buff)
{
    while(*buff)                  		//escreve caractere até econtrar null
   {
		while(TesteBusyFlag());			//espera LCD terminar de executar instrução
		EscDataLCD(*buff);				//escreve no LCD caractere apontado por bufff
        buff++;               			// Incrementa buffer
   }
//*******************************************************************
}										//final da função EscStringLCD
#pragma code
//*******************************************************************
/*esta função escreve no LCD uma string lida da memória de programa 
a partir da posição aponntada pelo cursor.*/
void EscStringLCD_ROM(const rom char *buff)
{
    while(*buff)                  		// Write data to LCD up to null
   {
		while(TesteBusyFlag());			//espera LCD controller terminar de executar instrução
     	EscDataLCD(*buff);				//escreve no LCD caractere apontado por bufff
        buff++;               			// Incrementa buffer
   }
        return;
//*******************************************************************
}										//final da função EscStringLCD_ROM
//*******************************************************************
//esta função testa o LCD acendendo todos os pixels do display.
void TestPixelsLCD(void)
{
unsigned char BffCheio[32];				//declaração de vetor 
unsigned char i;						//declaração de variável local

	EscInstLCD(0x80);					//posiciona cursor na primeira posição da primeira linha
	while(TesteBusyFlag());				//espera LCD controller terminar de executar instrução

	for(i=0;i<32;i++)					//laço de iteração
	{
		if(i<16)						//i < 16? 
		{								//sim, executa bloco de código a seguir
			EscDataLCD(0xFF);			//escreve caractere na posição apontada pelo  cursor
			while(TesteBusyFlag());		//espera LCD controller terminar de executar instrução
		}
		else 	if(i==16)				//i==16?
		{								//sim, executa bloco de código a seguir
			EscInstLCD(0xC0);			//posiciona cursor na primeira posição da segunda linha
			while(TesteBusyFlag());		//espera LCD controller terminar de executar instrução

			EscDataLCD(0xFF);			//escreve caractere na posição pantada pelo  cursor
			while(TesteBusyFlag());		//espera LCD controller terminar de executar instrução
		}
		else							//se i !=16 executa bloco de c[odigo a seguir
		{
			EscDataLCD(0xFF);			//escreve caractere na posição pantada pelo  cursor
			while(TesteBusyFlag());		//espera LCD controller terminar de executar instrução
		}
	}
//*******************************************************************
}
										