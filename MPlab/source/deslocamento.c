/*********************************************************************
Nome do arquivo:	main_29.c            
Data:	    		28 de dezembro de 2008          
Versao:				1.0                              
MPLAB IDE:			v8.10 
Autor:				Wagner Zanco              
*********************************************************************/
#include <p18f4550.h>		//diretiva de compilação
#include "MCC18_18F4550.h"
#include <delays.h>

//********************************************************************
//macros
#define BT1 PORTBbits.RB2
#define BT2 PORTBbits.RB3
#define BT3 PORTBbits.RB4
#define BT4 PORTBbits.RB5
#define BT5 PORTBbits.RB6

//********************************************************************
//protótipos de funções
 void Inic_Regs (void);
 void LSB_MSB(unsigned char);
 void MSB_LSB(unsigned char);
 void MEIO_FINAL(unsigned char);
 void FINAL_MEIO(unsigned char);
 void CRUZAMENTO(unsigned char);
//********************************************************************
void main(void)					//função main					
{	    
	Inic_Regs ();				//chamada a funcão

	while (1)                   //loop infinito
	{
		if (!BT1) LSB_MSB(0b00000001);          	//se botão BT1 estiver pressionado:move do LSB para o MSB            
		else if (!BT2) MSB_LSB(0b10000000);         //se botão BT2 estiver pressionado:move do MSB para o LSB
		else if (!BT3) MEIO_FINAL(0b00011000);      //se botão BT3 estiver pressionado:move do MEIO para o FINAL
        else if (!BT4) FINAL_MEIO(0b10000001);      //se botão BT4 estiver pressionado:move do FINAL para o MEIO				
        else if (!BT5) CRUZAMENTO(0b10000001);      //se botão BT5 estiver pressionado:move do CRUZAMENTO	
        else PORTD = 0;
	}
}
/*******************************************************************
Esta funcao inicializa os resgistradores SFRs.*/
 void Inic_Regs (void)
{
	TRISA = 0x00;			//PORTA saída
	TRISB = 0xFC;			//PORTB entrada
	TRISC = 0x00;			//PORTC saida
	TRISD = 0x00;			//PORTD saida
	TRISE = 0x00;			//PORTE saida
	ADCON1 = 0x0F;			//configura pinos dos PORTA e PORTE como digitais
	PORTA = 0;				//limpa PORTA
	PORTB = 0;				//limpa PORTB
	PORTC = 0;				//limpa PORTC
	PORTD = 0;              //apaga LEDs
	PORTE = 0;				//limpa PORTE
//********************************************************************
}
/*******************************************************************
Esta funcao desloca os bits do LSB para o MSB.*/
void LSB_MSB(unsigned char LEDS)
{
    while(!BT1){
        PORTD = LEDS;
        Delay1KTCYx(250);
        LEDS = LEDS << 1;
        if(LEDS == 0) LEDS = 1;
    }

//********************************************************************
}
/*******************************************************************
Esta funcao desloca os bits do MSB para o LSB.*/
void MSB_LSB(unsigned char LEDS)
{
    while(!BT2){
        PORTD = LEDS;
        Delay1KTCYx(250);
        LEDS = LEDS >> 1;
        if(LEDS == 0) LEDS = 128;
    }

//********************************************************************
}
/*******************************************************************
Esta funcao desloca os bits do MEIO para o FINAL.*/
void MEIO_FINAL(unsigned char LEDS)
{
    unsigned char backupLEDS = LEDS;
    while(!BT3){
        PORTD = backupLEDS | LEDS;
        Delay1KTCYx(250);
        backupLEDS = ~backupLEDS & (backupLEDS << 1);
        LEDS = ~LEDS & (LEDS >> 1);
        if(LEDS == 0) LEDS = backupLEDS = 24;
    }

//********************************************************************
}
/*******************************************************************
Esta funcao desloca os bits do FINAL para o MEIO.*/
void FINAL_MEIO(unsigned char LEDS)
{
    unsigned char backupLEDS = LEDS;
    while(!BT4){
        PORTD = backupLEDS | LEDS;
        Delay1KTCYx(250);
        backupLEDS = ~backupLEDS & (backupLEDS >> 1);
        LEDS = ~LEDS & (LEDS << 1);
        if(LEDS == 16) LEDS = backupLEDS = 129;
    }

//********************************************************************
}
/*******************************************************************
Esta funcao desloca os bits cruzando-os.*/
void CRUZAMENTO(unsigned char LEDS)
{
    unsigned char backupLEDS = LEDS;
    while(!BT5){
        PORTD = backupLEDS | LEDS;
        Delay1KTCYx(250);
        backupLEDS = ~backupLEDS & (backupLEDS >> 1);
        LEDS = ~LEDS & (LEDS << 1);
        if(LEDS == 0) LEDS = backupLEDS = 129;
    }

//********************************************************************
}