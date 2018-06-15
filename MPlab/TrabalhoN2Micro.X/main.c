/*********************************************************************
Nome do arquivo:	main_32.c            
Data:	    		29 de mar?o de 2010          
Versao:				1.0                              
MPLAB IDE:			v8.20a 
Autor:				Wagner Zanco              
*********************************************************************/
#include <p18f4550.h>			//diretiva de compila??o
//#include <delays.h>			//diretiva de compila??o
#include "MCC18_18F4550.h"
#include "Lcd_8bits.h"			//diretiva de compila??o
#include <stdio.h>
#include <stdlib.h>
#include "eeprom.h"
#include "stdint.h"

#define col_1 PORTDbits.RD0
#define col_2 PORTDbits.RD1
#define col_3 PORTDbits.RD2
#define col_4 PORTDbits.RD3
#define row_1 PORTDbits.RD4
#define row_2 PORTDbits.RD5
#define row_3 PORTDbits.RD6
#define row_4 PORTDbits.RD7
//********************************************************************
//prot?tipos de fun??es
 void Inic_Regs (void);
 //********************************************************************
void main(void)										//fun??o main					
{	
unsigned char read[25];								//buffer para leitura da eeprom
unsigned char write[25];							//buffer para escrita na eeprom
char buf [17] = {"Seja bem vindo!"};				//declara??o de vetor inicializado
char buf02 [17] = {"   LCD 16 x 2"};				//declara??o de vetor inicializado
unsigned char x = 0x00;
int dly=0;											//declara??o de vari?vel local inicializada
//**********************************
	Inic_Regs ();									//configurar SFRs
	IniciaLCD (2);									//inicializar LCD controller HD44780
	TestPixelsLCD();								//teste no LCD - acende todos os pixels.
//**********************************
//delay de 3 segundos
	for(dly=0;dly<100;dly++)						//comando de itera??o
	{
		_Delay5ms();								//delay de 5ms
	}
//**********************************
    EEPROM_Write_Block(0x00,strcpypgm2ram(write, "Ola Mundo!"), 10); //escreve na eeprom
    EEPROM_Read_Block(0x00, read, sizeof(read));							   //le da eeprom
//**********************************
	EscInstLCD(0x01);								//limpa display e mostra cursor piscando na primeira posi??o da primmeira linha
	while(TesteBusyFlag());							//espera LCD controller terminar de executar instru??o

//	EscStringLCD(buf);								//escreve string no LCD
//    EscStringLCD(read);
//	while(TesteBusyFlag());							//espera LCD controller terminar de executar instru??o

//	EscInstLCD(0xC0);								//posiciona cursor na primeir aposic??o  da segunda linha
//	while(TesteBusyFlag());							//espera LCD controller terminar de executar instru??o
//
//	EscStringLCD(buf02);							//escreve string no LCD					
//	while(TesteBusyFlag());							//espera LCD controller terminar de executar instru??o
//
//	EscInstLCD(0x0C);								//desativa cursor
//	while(TesteBusyFlag());							//espera LCD controller terminar de executar instru??o
    
	while(1){
//        EscInstLCD(GetKey());								//limpa display e mostra cursor piscando na primeira posi??o da primmeira linha
//        while(TesteBusyFlag());
        x = GetKey();
        Delay1KTCYx(40);
        if(x=='G') continue;
        {
            if(x!='\0'){
                if(x=='1'){
                    EscStringLCD(x);							//escreve string no LCD					
                    while(TesteBusyFlag());
                } else if (x=='2'){
                    EscStringLCD(x);							//escreve string no LCD					
                    while(TesteBusyFlag());
                } else if (x=='3'){
                    EscStringLCD(x);							//escreve string no LCD					
                    while(TesteBusyFlag());
                } else if (x=='4'){
                    EscStringLCD(x);							//escreve string no LCD					
                    while(TesteBusyFlag());
                } else if (x=='5'){
                    EscStringLCD(x);							//escreve string no LCD					
                    while(TesteBusyFlag());
                } else if (x=='6'){
                    EscStringLCD(x);							//escreve string no LCD					
                    while(TesteBusyFlag());
                } else if (x=='7'){
                    EscStringLCD(x);							//escreve string no LCD					
                    while(TesteBusyFlag());
                } else if (x=='8'){
                    EscStringLCD(x);							//escreve string no LCD					
                    while(TesteBusyFlag());
                }
            }
        }
    };										//loop infinito    		
}
/*******************************************************************
Esta funcao inicializa os resgistradores SFRs.*/
 void Inic_Regs (void)
{
	TRISA = 0x00;			//PORTA sa?da
	TRISB = 0x00;			//PORTB sa?da
	TRISC = 0x00;			//PORTC sa?da
	TRISD = 0x00;			//PORTD sa?da
	TRISE = 0x00;			//PORTE sa?da
	ADCON1 = 0x0F;			//configura pinos dos PORTA e PORTE como digitais
	PORTA = 0;				//limpa PORTA
	PORTB = 0;				//limpa PORTB
	PORTC = 0;				//limpa PORTC
	PORTD = 0x00;			//apaga displays
	PORTE = 0;				//limpa PORTE
//********************************************************************
}

// encryption routine
void encrypt(uint32_t* v, uint32_t* k) {
    uint32_t v0 = v[0], v1 = v[1], sum = 0, i;
    uint32_t delta = 0x9e3779b9;
    uint32_t k0 = k[0], k1 = k[1], k2 = k[2], k3 = k[3];
    for (i = 0; i < 32; i++) {
        sum += delta;
        v0 += ((v1 << 4) + k0) ^ (v1 + sum) ^ ((v1 >> 5) + k1);
        v1 += ((v0 << 4) + k2) ^ (v0 + sum) ^ ((v0 >> 5) + k3);
    }
    v[0] = v0;
    v[1] = v1;
}

// decryption routine
void decrypt(uint32_t* v, uint32_t* k) {
    uint32_t v0 = v[0], v1 = v[1], sum = 0xC6EF3720, i;
    uint32_t delta = 0x9e3779b9;
    uint32_t k0 = k[0], k1 = k[1], k2 = k[2], k3 = k[3];
    for (i = 0; i < 32; i++) {
        v1 -= ((v0 << 4) + k2) ^ (v0 + sum) ^ ((v0 >> 5) + k3);
        v0 -= ((v1 << 4) + k0) ^ (v1 + sum) ^ ((v1 >> 5) + k1);
        sum -= delta;
    }
    v[0] = v0;
    v[1] = v1;
}