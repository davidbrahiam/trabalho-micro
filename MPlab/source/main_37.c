/*********************************************************************
Nome do arquivo:	main_37.c            
Data:	    		17 de maio de 2010          
Versao:				1.0                              
MPLAB IDE:			v8.20a 
Autor:			Wagner Zanco              
*********************************************************************/
#include <p18f4520.h>		//diretiva de compilação
#include "MCC18.h"
#include <delays.h>			//diretiva de compilação
#include <stdio.h>			//diretiva de compilação
#include "Lcd_8bits.h"		//diretiva de compilação
#include "I2C_Master.h"		//diretiva de compilação
//********************************************************************
//protótipos de funções
void Inic_Regs (void);
void Atual_LCD (void);
void Master_I2C (void);
char  ESCRITA_24C08  (unsigned char ENDH, unsigned char ENDL, char DADO);
char LEITURA_24C08 (unsigned char _ENDH, unsigned char _ENDL);
char Detecta_fim_escrita(void);
//********************************************************************
//variáveis globais
char buf [17];
unsigned char ENDH=0b01010000;
unsigned char ENDL=0;
char DADO='A';
unsigned char TEMP;
//********************************************************************
//definições
#define Botao1			PORTBbits.RB0
//********************************************************************
void main(void)							//função main					
{	
float temp=0;							//declaração  de variável local inicializada
int dly=0;								//declaração  de variável local inicializada
char status_botao=0;					//declaração  de variável local inicializada
//**********************************
	Inic_Regs ();						//configurar SFRs
	Master_I2C ();						//configura Mestre I2C
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
//rotina principal
	ESCRITA_24C08(ENDH, ENDL, DADO);						//escreve dado na EEPROM
	while(Detecta_fim_escrita()==-1)Detecta_fim_escrita ();	//aguarda final da escrita na EEPROM
	DADO=LEITURA_24C08 (ENDH, ENDL);						//lê EEPROM
	Atual_LCD();											//atualiza LCD
	while (1)												//loop infinito
	{
		if(!Botao1) 										//Botão 1 pressionado?
		{													//sim, executa bloco de código
			if(!status_botao)								//Botão 1 foi tratado?
			{												//não, executa bloco de código
				status_botao=1;								//sinaliza que Botão foi tratado
				if(DADO=='Z')DADO='A';						//reinicializa DADO se atingiu o valor final
				else DADO+=1;								//atualiza DADO
				
				ESCRITA_24C08(ENDH, ENDL, DADO);						//escreve dado na EEPROM
				while(Detecta_fim_escrita()==-1)Detecta_fim_escrita ();	//aguarda finalizar ciclo de escrita na EEPROM
				DADO=LEITURA_24C08 (ENDH, ENDL);						//lê EEPROM
				ENDL+=1;												//incrementa endereço baixo
				Atual_LCD();											//atualiza LCD
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
//configura o microcontrolador como mestre com bit rate de 100kb.
void Master_I2C (void)
{
	TRISC = ((TRISC * 0b11100111) | 0b00011000 );	//configura pinos SDA e SCL como entrada
	SSPCON1 = 0b00101000;				//configura dispositivo como mestre
										//Habilita porta serial e ativa pinos SDA E SCL <5>
										//ativa modo mestre <3:0>
	SSPADD = 19;						//bit rate de 100kbps a Fosc = 8MHz
	SSPSTAT = 0b10000000;				//controle de inclinação desativado <7>
										//níveis de entrada conforme especificação I2C <6>
}//*********************************************************************
/*função  que  atualiza o LCD. Será mostrado no LCD o endereço e
o conteúdo armazenado no endereço*/
void Atual_LCD (void)
{
int END;									//declaração de variável local
//*************************
	END = ENDH & 3;
	END = END << 8;
	END += ENDL;							//obtem endereço					  
//*************************
	EscInstLCD(0x01);						//limpa display e mostra cursor piscando na primeira posição da primmeira linha
	while(TesteBusyFlag());					//espera LCD controller terminar de executar instrução
	if(DADO==-1)							//houve erro de comunicação?
	{
		sprintf(buf,"Erro na transmissão");	//sim, converte valor em string e armazena no vetor buf	
		EscStringLCD(buf);					//escreve string no LCD
		while(TesteBusyFlag());				//espera LCD controller terminar de executar instrução
	}
	else 									//não houve erro de comunicação
	{
		sprintf(buf,"End: %#xh",END);		//converte valor em string e armazena no vetor buf	
		EscStringLCD(buf);					//escreve string no LCD
		while(TesteBusyFlag());				//espera LCD controller terminar de executar instrução

		EscInstLCD(0xC0);					//posiciona cursor na primeira posição da segunda linha
		while(TesteBusyFlag());				//espera LCD controller terminar de executar instrução

		sprintf(buf,"Dado: %c",DADO);		//converte valor em string e armazena no vetor buf	
		EscStringLCD(buf);					//escreve string no LCD
		while(TesteBusyFlag());				//espera LCD controller terminar de executar instrução
	}
}//*******************************************************************
/*função que escreve o valor no endereço de memória recebido  como parâmetro
o valor escrito é retornado. Caso ocorra um erro na transmissão é retornado o valor -1.*/
char  ESCRITA_24C08  (unsigned char ENDH, unsigned char ENDL, char DADO)
{
char x;									//declaração de variável local 
	x = I2C_LIVRE ();					//chamada à função com retorno de valor. Verifica se barramento está livre
	if (x == 0)							//se barramento ocupado, aborta transmissão e retorna
	{
		I2C_STOP();						//gera bit STOP
		return -1;						//erro na transmissão
	}
	else 								//barramento livre
	{
		I2C_START();					//barramento livre, gera condição START
		TEMP= ENDH << 1;				//rotaciona END_I2C para a esquerda (insere bit R_W para escrita) 
		I2C_TRANSMITE(TEMP);			//transmite endereço alto
		if (!I2C_TESTA_ACK())	 		//se erro na transmissão, aborta transmissão
		{
			I2C_STOP();					//gera bit STOP
			return -1;					//erro na transmissão
 		}
		I2C_TRANSMITE(ENDL);			//transmite endereço baixo
		if (!I2C_TESTA_ACK())			//se erro na transmissão, aborta transmissão
		{
			I2C_STOP();					//gera bit STOP
			return -1;					//erro na transmissão
		}
		I2C_TRANSMITE(DADO);			//transmite dado
		if (!I2C_TESTA_ACK())	 		//se erro na transmissão, aborta transmissão
		{
			I2C_STOP();					//gera bit STOP
			return -1;					//erro na transmissão
 		}
		I2C_STOP();						//gera bit STOP
		return DADO;					//transmissão feita com sucesso
	}
}//*********************************************************************
/*função que lê o valor no endereço de memória recebido  como parâmetro.
o valor lido é retornado. Caso ocorra um erro na transmissão é retornado o valor -1.*/
char LEITURA_24C08 (unsigned char _ENDH, unsigned char _ENDL)
{
	char x;								//declaração de variável local 
	unsigned char DADO_I2C;				//declaração de variável local 
	x = I2C_LIVRE ();					//chamada à função com retorno de valor. Verifica se barramento está livre
	if (x == 0)							//se barramento ocupado, aborta transmissão e retorna
	{
		I2C_STOP();						//gera bit STOP
		return -1;						//erro na transmissão
	}
	else 								//barramento livre
	{
		I2C_START();					//barramento livre, gera condição START
		TEMP = _ENDH << 1;				//rotaciona END_I2C para a esquerda 
		I2C_TRANSMITE(TEMP);			//transmite endereço
		if (!I2C_TESTA_ACK()) 			//se erro na transmissão, aborta transmissão
		{
			I2C_STOP();					//gera bit STOP
			return -1;					//erro na transmissão
 		}
		I2C_TRANSMITE(_ENDL);			//transmite endereço baixo
		if (!I2C_TESTA_ACK())			//se erro na transmissão, aborta transmissão
		{
			I2C_STOP();					//gera bit STOP
			return -1;					//erro na transmissão
		}
		I2C_RESTART();
		TEMP |= 0b00000001;				//insere bit R_W para leitura
		I2C_TRANSMITE(TEMP);			//transmite endereço
		if (!I2C_TESTA_ACK()) 			//se erro na transmissão, aborta transmissão
		{
			I2C_STOP();					//gera bit STOP
			return -1;					//erro na transmissão
 		}
		DADO_I2C = I2C_RECEBE();		//recebe dado
		I2C_NACK();						//gera bit NACK
		I2C_STOP();						//gera bit STOP
		return DADO_I2C;				//transmissão feita com sucesso
	}
}//*********************************************************************
/*esta função detecta se o ciclo de escrita na EEPROM terminou.Caso a 
escrita tenha terminado ela retornará 0, caso contrario retornará -1*/
char Detecta_fim_escrita (void)
{
	char x;								//declaração de variável local 
	unsigned char DADO_I2C;				//declaração de variável local 
	x = I2C_LIVRE ();					//chamada à função com retorno de valor. Verifica se barramento está livre
	if (x == 0)							//se barramento ocupado, aborta transmissão e retorna
	{
		I2C_STOP();						//gera bit STOP
		return -1;						//barramento ocupado
	}
	else 								//barramento livre
	{
		I2C_START();					//barramento livre, gera condição START
		TEMP = ENDH << 1;				//rotaciona END_I2C para a esquerda 
		I2C_TRANSMITE(TEMP);			//transmite endereço
		if (!I2C_TESTA_ACK()) 			//se erro na transmissão, aborta transmissão
		{
			I2C_STOP();					//gera bit STOP
			return -1;					//ciclode esrita não terminou
 		}
		return 0;						//ciclo de escrita terminou
	}
//***********************************************************************
}