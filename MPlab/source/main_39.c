/*********************************************************************
Nome do arquivo:	main_39.c            
Data:	    		17 de maio de 2010          
Versao:				1.0                              
MPLAB IDE:			v8.20a 
Autor:				Wagner Zanco              
*********************************************************************/
#include <p18f4520.h>		//diretiva de compilação
#include "Lcd_8bits.h"		//diretiva de compilação
//********************************************************************
//protótipos de funções
void Inic_Regs (void);
void Atual_LCD (char dado);
void Configura_UART (void);
void Transmite_UART (char dado);
//********************************************************************
//variáveis globais
char count=0;
//********************************************************************
void main(void)							//função main					
{	
float x=0;								//declaração  de variável local inicializada
int dly=0;								//declaração  de variável local inicializada
char DADO;								//declaração  de variável local inicializada
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
	Configura_UART();					//configura UART com taxa de 2400bps
//**********************************/
//rotina principal
	while(1)
	{
		while(!PIR1bits.RCIF);				//aguarda chegar um novo byte
		if (RCSTAbits.FERR)					//houve erro de transmissão?
		{
			RCSTAbits.CREN = 0;				//sim, desabilita recepção
			RCSTAbits.CREN = 1;				//habilita recepção
		}
		else								//não houve erro de transmissão, transmite dado recebido
		{
			DADO = RCREG;					//obtém caractere
			Transmite_UART (DADO);			//transmite DADO
			Atual_LCD(DADO);				//atualiza LCD
		}
	}
}//******************************************************************
//							funções
//*******************************************************************
/*Esta funcao inicializa os resgistradores SFRs.*/
 void Inic_Regs (void)
{
	TRISA = 0x00;						//PORTA saída
	TRISB = 0x00;						//PORTB saída
	TRISC = 0x00;						//PORTC saída
	TRISD = 0x00;						//PORTD saída
	TRISE = 0x00;						//PORTE saída
	ADCON1 = 0x0F;						//configura pinos dos PORTA e PORTE como digitais
	PORTA = 0;							//limpa PORTA
	PORTB = 0;							//limpa PORTB
	PORTC = 0;							//limpa PORTC
	PORTD = 0x00;						//apaga displays
	PORTE = 0;							//limpa PORTE
}//********************************************************************
//inicializa USART
void Configura_UART (void)
{
	TRISCbits.TRISC7 = 1;			//configura pino RX como entrada
	TRISCbits.TRISC6 = 1;			//configura pino TX como entrada
	TXSTA = 0b00100100;				//transmissão habilitada<5>
									//transmissão assíncrona<4>
									//transmissão em alta velocidade<2>
	RCSTA = 0b10010000;				//porta serial habilitada<7>
									//recepção contínua habilitada<4>
	BAUDCON = 0b00000000; 			//TX ocioso em nível alto<4>
									//gerador de baud rate de 8 bits<3>
	SPBRG = 207;					//2400bps
}//*********************************
//transmite o caractere
void Transmite_UART (char dado)
{
	TXREG = dado;					//inicia a transmissão
	while(TXSTAbits.TRMT);			//aguarda transmissão terminar
}//***************************************************************************
//função  que  atualiza o LCD. 
void Atual_LCD (char dado)
{
	if(count==16)							//se cursor chegou no final da primeira linha
	{
		EscInstLCD(0xC0);					//move cursor para a primeira posição da segunda linha
		while(TesteBusyFlag());				//espera LCD controller terminar de executar instrução
	}
	else if(count==32)						//se cursor chegou no final da segunda linha		
	{
		EscInstLCD(0x01);					//limpa display e mostra cursor piscando na primeira posição da primmeira linha
		while(TesteBusyFlag());				//espera LCD controller terminar de executar instrução
	}
	EscDataLCD(dado);						//escreve caractere no LCD na posição apontada pelo cursor	
	while(TesteBusyFlag());					//espera LCD controller terminar de executar instrução
	count+=1;								//incrementa count
//*******************************************************************
}
