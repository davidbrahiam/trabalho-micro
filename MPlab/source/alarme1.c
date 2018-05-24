/*********************************************************************
Nome do arquivo:	amarme1.c   
Proteus:            SimpleProject4         
Data:	    		28 de dezembro de 2008          
Versao:				1.0                              
MPLAB IDE:			MPLAB-X v 4.10 
Autor:				Carlúcio Silva              
*********************************************************************/
#include <p18f4550.h>		//diretiva de compilação
#include "MCC18_18F4550.h"

//Constantes
#define CHAVE_A PORTBbits.RB2
#define CHAVE_B PORTBbits.RB3
#define AL_DISPARADO PORTDbits.RD0
#define AL_DEFEITO   PORTDbits.RD1
#define AL_NORMAL    PORTDbits.RD2

//********************************************************************
//protótipos de funções
 void Inic_Regs (void);
//********************************************************************
 
void main(void)					//função main					
{	    
	Inic_Regs ();				//chamada a funcão
        
	while (1)				//loop infinito
	{   
        // 1- VERIFICA SE ALARME ESTÁ OPERANDO NORMALMENTE
        if(CHAVE_A ^ CHAVE_B){
            AL_DEFEITO = 0; // RESET DO LED INDICADOR DE DEFEITO
            AL_NORMAL = 1;  // SET DO LED INDICADOR NORMALIDADE
            // 2- VERIFICA SE ALARME DISPAROU
            if(!CHAVE_A){
               AL_DISPARADO = 1; // SET DO LED INDICADOR DE INVASÃO 
            }
            else AL_DISPARADO = 0; // RESET DO LED INDICAR DE INVASÃO
        }	
        else{
            AL_DEFEITO = 1;   // SET DO LED INDICADOR DE DEFEITO
            AL_NORMAL = 0;    // RESET DO LED INDICADOR DE NORMALIDADE
            AL_DISPARADO = 0; // RESET DO LED INDICADOR DE INVASÃO
        }
	}
}
/*******************************************************************
Esta funcao inicializa os resgistradores SFRs.*/
 void Inic_Regs (void)
{
	TRISA = 0x00;			//PORTA saída
	TRISB = 0x3C;			//pinos RB5:RB2 entrada e demais pinos do PORTB saída
	TRISC = 0x00;			//PORTC saida
	TRISD = 0x00;			//PORTD saida
	TRISE = 0x00;			//PORTE saida
	ADCON1 = 0x0F;			//configura pinos dos PORTA e PORTE como digitais
	PORTA = 0;				//limpa PORTA
	PORTB = 0;				//limpa PORTB
	PORTC = 0;				//limpa PORTC
	PORTD = 0;			    //apaga LEDs
	PORTE = 0;				//limpa PORTE
//********************************************************************
}
