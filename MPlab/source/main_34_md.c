/*******************************************************************************
Nome do arquivo:	main_34_md.c            
Data:	    		22 abril de 2017          
Versao:				1.0                              
MPLAB IDE:			MPLAB-X v.3.55 
Autor:				Wagner Zanco
Modificado por:     Carlúcio Silva  
Descrição: Configuração do TIMER0 para operar como temporizador no modo 16 bits, 
com interrupção com a codificação desenvolvida dentro de configTMR0() com passagem
de parâmetro e com a inicialização do TIMER desmembrada da configuração. Com
uma função para tratar a interrupção em separado.              
*******************************************************************************/
#include <p18f4520.h>			//diretiva de compilação
#include "MCC18.h"              //diretiva de compilação

//******************************************************************************
//Protótipos de funções
void Inic_Regs(void);
void setupTIMER0(void);
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
void main(void)							//função main				
{	
	Inic_Regs();						//configurar SFRs - Geral  
    setupTIMER0();                      //configurar SFRs - TIMER 0
	while(1);							//loop infinito
}

/*******************************************************************************
Esta funcao inicializa os registradores SFRs.*/
 void Inic_Regs(void)
{
	TRISA = 0x00;				//PORTA saída
	TRISB = 0x00;				//PORTB saída
	TRISC = 0x00;				//PORTC saída
	TRISD = 0x00;				//PORTD saída
	TRISE = 0x00;				//PORTE saída
	ADCON1 = 0x0F;				//configura pinos dos PORTA e PORTE como digitais
	PORTA = 0;					//limpa PORTA
	PORTB = 0;					//limpa PORTB
	PORTC = 0;					//limpa PORTC
	PORTD = 0x00;				//acende os LEDS
	PORTE = 0;					//limpa PORTE   
}
 /******************************************************************************
  Esta funcao inicializa os registradores SFRs do TIMER 0*/  
 void setupTIMER0(void)
 {
//******************************************************************************     
//Passo 1: Habilitar a interrupção de estouro de TMR0
	INTCONbits.TMR0IE = 1;	//liga chave individual de interrupção do TIMER 0     
	INTCONbits.GIE = 1;		//liga chave geral de interrupção
    
//******************************************************************************
/* Passo 2: Configurar o TMR0 para operar como temporizador e estourar a cada 1s
 * T0CON<7> (TMR0ON = 1): Liga o TIMER 0
 * T0CON<6> (T08BIT = 0): Operar no modo 16 bits
 * T0CON<5> (T0CS = 0)  : Operar com o clock do Fosc
 * T0CON<4> (T0SE = 0)  : Operar na borda de subida **
 * T0CON<3> (PSA = 0)   : Operar a partir do prescaler
 * T0CON<2:0> (T0PS2 = 1; T0PS1 = 1; T0PS0 = 1): Configura o prescaler para 1:256,
 * com essa configuração o prescaler proporciona um atrso de 128us. */   
	T0CON = 0b10000111;
    
//******************************************************************************    
/*Passo 3: Inicializar o TMR0. 
 * Como esta configuração contempla o modo 16 bits, inicializando o TMR0 com o 
 * valor 49911, este contará de 0 - 15624, ao final de tudo, proporcionando um 
 * atraso de 2 segundos. */
    TMR0H = 0b11000010; 
    TMR0L = 0b11110111;     
 }
 
 /****************************************************************************** 
 * Rotina de tratamento de interrupção (ISR)
 * Depois que ocorre a interrupção, o vetor de interrupção direciona o fluxo do
 * programa para a localização da Memória de Programa onde encontra-se essa 
 * instrução, "high_isr".*/
#pragma interrupt high_isr
void high_isr(void)
{
    //**************************************************************************
    /* - if(INTCONbits.TMR0IF);
     * Se TMR0IF for igual a 1 trata a interrupção vinda do TIMER 0.*/
	if(INTCONbits.TMR0IF);
	{
        //**********************************************************************
        /* - INTCONbits.TMR0IF;
         * Como houve interrupção pelo TIMER 0, o bit INTCON<2> (INTCONbits.TMR0IF)
         * precisa ser limpo (levado a LÓGICO 0) para que, quando houver outra 
         * interrupção esta possa ser identificada. O hardaware não faz isso para
         * o programador, sendo necessário ser feito a nível de softwarre.*/
		INTCONbits.TMR0IF=0;	
        	
        //**********************************************************************
        /* Neste ponto coloca-se as instruções que deverão ocorrer dentro da
         * interrupção, como por exemplo a seqüência de comutação dos LEDS.*/
        if(!flagPORTD)
        {
            //******************************************************************
            flagPORTD = 1;          /*
                                     Controla o acendimento dos LEDS segundo a
                                     interrupção, na verdade o que esta variável
                                     faz é definir um tipo de permutação entre os
                                     estados acessos e apagados dos LEDS. Na próxima 
                                     interrupção ela direciona o fluxo do programa
                                     para a estrutura de decisão else().*/ 
            
            //******************************************************************
            PORTD = 0xFF;           /* Apaga os LEDS */ 
            
            //******************************************************************
            /* TMR0L = 123 <-> 0x7B; TMR0H = 225 <-> 0xE1 <-> TMR0 = 65535 - 57723 = 7812
            * De acordo com a necessidade é útil inicializar o registro TMR0
            * (modo 16 bits: TMR0L:TMR0H) para definir alguma quantidade de contagem. 
            * Neste caso, o TMR0 = 0b1110000101111011 (57723) iniciado com esse
            * valor temos que o TMR0 contará de 0 - 7812, proporcionando ao final de 
            * tudo, um atraso de 1 segundo.*/
            TMR0H = 0b11100001;              
            TMR0L = 0b01111011;            
        }
        else 
        {
            //******************************************************************
            flagPORTD = 0;          /*
                                     Controla o acendimento dos LEDS segundo a
                                     interrupção, na verdade o que esta variável
                                     faz é definir um tipo de permutação entre os
                                     estados acessos e apagados dos LEDS. Na próxima 
                                     interrupção ela direciona o fluxo do programa
                                     para a estrutura de decisão if().*/
            
            //******************************************************************
            PORTD = 0x00;          // Acende os LEDS   
            /* TMR0L = 247 <-> 0xF7; TMR0H = 194 <-> 0xC2 <-> TMR0 = 65535 - 49911 = 15624 
            * De acordo com a necessidade é útil inicializar o registro TMR0
            * (modo 16 bits: TMR0L:TMR0H) para definir alguma quantidade de contagem. 
            * Neste caso, o TMR0 = 0b1100001011110111 (49911) , proporcionando ao final de 
            * tudo, um atraso de 2 segundos, contando de 0 - 15624.*/
            TMR0H = 0b11000010;            
            TMR0L = 0b11110111;              
        }        
	}
}
 
