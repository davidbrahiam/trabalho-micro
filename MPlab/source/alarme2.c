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
#define CHAVE_A PORTBbits.RB2
#define CHAVE_B PORTBbits.RB3

//Variáveis globais
volatile union {
    unsigned char PORT;
    struct{
        unsigned LSB:4;
        unsigned MSB:4;
    };
}REG_PORTD;


//********************************************************************
//protótipos de funções
 void Inic_Regs (void);
//********************************************************************
 
void main(void)					//função main					
{	    
    unsigned char contLSB, contMSB;
    contLSB = contMSB = 0;
    REG_PORTD.LSB = REG_PORTD.MSB = 0;
    
    Inic_Regs ();				//chamada a funcão
        
	while (1)				//loop infinito
	{   
        // 1- INCREMENTA O CONTADOR BCD UTILIZANDO DUAS VARIÁVEIS.
        // PARA ISSO MOVIMENTA O NIBLLE MSB DE UMA DAS VARIÁVEIS
        if(!CHAVE_A){
            Delay10KTCYx( 50 );
            contLSB++;
            if(contLSB > 9){
                contLSB = 0;
                contMSB++;
                if(contMSB > 9)contMSB = 0;
            }
            PORTD = contLSB | (contMSB << 4);
        }
        // 2- FAZ O MESMO QUE EM 1, CONTUDO UTILIZA UMA VARIÁVEL COMPLEXA (struct)
        // E UMA UNION.
        else if(!CHAVE_B){
            Delay10KTCYx( 50 );
            REG_PORTD.LSB++;
            if(REG_PORTD.LSB > 9){
                REG_PORTD.LSB = 0;
                REG_PORTD.MSB++;
                if(REG_PORTD.MSB > 9)REG_PORTD.MSB = 0;
            }
            PORTD = REG_PORTD.PORT;
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
