/*******************************************************************************
Nome do arquivo:	main_34-4_md.c            
Data:	    		27 abril de 2018          
Versao:				1.4                              
MPLAB IDE:			MPLAB-X v.4.15 
Autor:				Wagner Zanco
Modificado por:     Carlúcio Silva         
Descrição: Configuração do TIMER0 para operar como temporizador no modo 8 bits, 
com interrupção com a codificação desenvolvida dentro de configTMR0() com passagem
de parâmetro e com a inicialização do TIMER desmembrada da configuração. Com
uma função para tratar a interrupção em separado.     
*******************************************************************************/
#include <p18f4550.h>			//diretiva de compilação
#include "MCC18_18F4550.h"      //diretiva de compilação

//******************************************************************************
//Protótipos de funções
void configSFR(void);
void configTMR0(unsigned char);
void initTMR0_08BIT(unsigned char);
void initTMR0_16BIT(unsigned int);
void configInterruptTMR0(unsigned char);
void high_isr(void);


//******************************************************************************
// Declaração de variáveis globais
unsigned char flagPORTD = 0;

//******************************************************************************
/* Vetor de interrupção de alta prioridade.
 * Toda vez que ocorre uma interrupção o fluxo do program é desviado para a 
 * região de memória definida por essa instrução, mais especificamente, para o 
 * endereço 0x08 da Memória de Programa. */ 
#pragma code high_vector=0x08
void interrupt_at_high_vector(void)
{
    /*Desvia programa para rotina de tratamento da interrupção*/
	_asm GOTO high_isr _endasm		
}
#pragma code

//******************************************************************************
void main(void){
    configSFR();
    // Configuração do TIMER0
    configTMR0(0b11000111);     //Passo 1
    initTMR0_08BIT(0);          //Passo 2
    configInterruptTMR0(0);     //Passo 3
    
	while(1);
}

/*******************************************************************************
Esta função inicializa os registradores SFRs.*/
 void configSFR(void)
{
	TRISA = 0x00;		//PORTA saída
	TRISB = 0x00;		//PORTB saída
	TRISC = 0xFF;		//PORTC entrada
	TRISD = 0x00;		//PORTD saída
	TRISE = 0x00;		//PORTE saída
	ADCON1 = 0x0F;		//configura pinos dos PORTA e PORTE como digitais
	PORTA = 0;			//limpa PORTA
	PORTB = 0;			//limpa PORTB
	PORTC = 0;			//limpa PORTC
	PORTD = 0;  		//limpa PORTC
	PORTE = 0;			//limpa PORTE  
}
 
 /****************************************************************************** 
 * Rotina de tratamento de interrupção (ISR)
 * Depois que ocorre a interrupção, o vetor de interrupção direciona o fluxo do
 * programa para a localização da Memória de Programa onde encontra-se essa 
 * instrução, "high_isr".*/
#pragma interrupt high_isr
void high_isr(void)
{
    if(INTCONbits.TMR0IF){
        if(flagPORTD){
            PORTD = 0xFF;
            flagPORTD = 0x00;
        }
        else{
            PORTD = 0x00;
            flagPORTD = 0xFF;
        }
        INTCONbits.TMR0IF = 0;      //limpa a flag de estouro  
    }
} 
 
 /******************************************************************************
  Esta funcao inicializa os registradores SFRs do TIMER 0*/  
 void configTMR0(unsigned char configTMR0)
 {
//******************************************************************************
/* Passo 1: Configurar o TMR0 para operar como temporizador e estourar a cada 1s
 * T0CON<7> (TMR0ON = 1): Liga o TIMER 0
 * T0CON<6> (T08BIT = 1): Operar no modo 8 bits
 * T0CON<5> (T0CS = 0)  : Operar com o clock do Fosc/4
 * T0CON<4> (T0SE = 0)  : Operar na borda de subida **
 * T0CON<3> (PSA = 0)   : Operar a partir do prescaler
 * T0CON<2:0> (T0PS2 = 1; T0PS1 = 1; T0PS0 = 1): Configura o prescaler para 1:256,
 * com essa configuração o prescaler proporciona um atrso de 128us. */   
     T0CON = configTMR0;  //0b11000111
 } 

 /******************************************************************************
Esta funcao habilita/desabilita a interrupção por estouro do TIMER0*/ 
 void configInterruptTMR0(unsigned char interruptTMR0){
     if(interruptTMR0){
         INTCONbits.TMR0IE = 1;	//liga chave individual de interrupção do TIMER 0     
         INTCONbits.GIE = 1;	//liga chave geral de interrupção             
     }
     else{
         INTCONbits.TMR0IE = 0;	//desliga chave individual de interrupção do TIMER 0     
         INTCONbits.GIE = 0;    //desliga chave geral de interrupção            
     }
 }
 
/******************************************************************************
Esta funcao inicializa o registrador TIMER0 para o modo 8 bits*/ 
 void initTMR0_08BIT(unsigned char initTMR0){
     TMR0L = initTMR0;          
 }
 
 /******************************************************************************
Esta funcao inicializa o registrador TIMER0 para o modo 16 bits*/ 
 void initTMR0_16BIT(unsigned int initTMR0){
     TMR0H = initTMR0 >> 8;
     TMR0L = initTMR0;          
 }