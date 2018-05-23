/*********************************************************************
Nome do arquivo:	amarme2.c   
Proteus:            SimpleProject5         
Data:	    		28 de dezembro de 2008          
Versao:				1.0                              
MPLAB IDE:			MPLAB-X v 4.10 
Autor:				Carlúcio Silva              
*********************************************************************/
#include <p18f4550.h>		//diretiva de compilação
#include "MCC18_18F4550.h"
#include <delays.h>

//Constantes


//Variáveis globais
volatile union {
    unsigned char display;
    struct{
        unsigned lsbDisplay:4;
        unsigned msbDisplay:4;
    };
}setDisplay;


//********************************************************************
//protótipos de funções
 void Inic_Regs(void);
 void interZonas(void);
//********************************************************************
 
void main(void)					//função main					
{	    
    setDisplay.display = 0;     // Inicializa dislay
    
    Inic_Regs ();				//chamada a funcão - inicializa SFRs.
        
	while (1)				   //loop infinito
	{   
        PORTC = setDisplay.lsbDisplay;           // 
        
        setDisplay.lsbDisplay++;
        if(setDisplay.lsbDisplay > 16) setDisplay.lsbDisplay = 0;
    }
}
/*******************************************************************
Esta funcao inicializa os resgistradores SFRs.*/
 void Inic_Regs (void){
	TRISA = 0x00;			//PORTA saída
	TRISB = 0x01;			//pino RB0 como entrada e demais pinos do PORTB saída
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
/*******************************************************************
Esta funcao adicona marcação entre as zonas.*/ 
 void interZonas(void){
     unsigned char cont;
     for(cont = 0; cont < 6; cont++){
        PORTD = 0xFF;
        Delay10KTCYx(10);
        PORTD = 0;
        Delay10KTCYx(10);
     }
//********************************************************************
} 
