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
//#include "eeprom.h"
#include "users.h"

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
void configTMR0(unsigned char); //configura o TIMER0 - modo 8 bits
void initTMR0_08BIT(unsigned char); //inicializa o contador de 8 bits do TIMER0
void initTMR0_16BIT(unsigned int); //inicializa o contador de 16 bits do TIMER0
void configInterruptTMR0(unsigned char); //inicializa ineterrup??o
void high_isr(void); //trata a interrup??o
void Inic_Regs (void);
void escreveLed(char esc);
void initLCD(void);
char timer0(void);
void escreveCaracterL1(char esc[17]);
void escreveCaracterL2(char esc[17]);

//******************************************************************************
// Declara??o de vari?veis globais
unsigned char connect1 [17] = "Connecting...";                  //declara??o de vetor inicializado
unsigned char connect2 [17] = "Please Wait!";                  //declara??o de vetor inicializado
unsigned char init [17] =    "OK!";                  //declara??o de vetor inicializado
unsigned char limpa [17] = " ";                  //declara??o de vetor inicializado
unsigned char user [3] = "";                  //declara??o de vetor inicializado
unsigned char senha [17] = "SENHA:";                  //declara??o de vetor inicializado
unsigned char usuario [17] = "USUARIO:";                  //declara??o de vetor inicializado
unsigned char nova_senha [17] = "NOVA SENHA:";                  //declara??o de vetor inicializado
unsigned char password[6] = "";
unsigned char success [17] =  "Seja bem vindo! ";                  //declara??o de vetor inicializado
unsigned char invalid [17] = "Senha inválida  ";                  //declara??o de vetor inicializado

int userOrPass = 1;                                               // 1=user e 0=passward
int position_password = 0;
int position_user = 0;
int qtdHashtag = 0;
int dly = 0; 
char controle = 1;
int qtdLinha = 0;
//******************************************************************************
/* Vetor de interrup??o de alta prioridade.
 * Toda vez que ocorre uma interrup??o o fluxo do program ? desviado para a 
 * regi?o de mem?ria definida por essa instru??o, mais especificamente, para o 
 * endere?o 0x08 da Mem?ria de Programa. */
#pragma code high_vector=0x08

void interrupt_at_high_vector(void) {
    /*Desvia programa para rotina de tratamento da interrup??o*/
    _asm GOTO high_isr _endasm
}
#pragma code

//********************************************************************
void main(void)										//fun??o main					
{    
    unsigned char address;
    unsigned char flagPORTD = 0;
    unsigned char buf [17] = "USER";                       //declara??o de vetor inicializado
    unsigned char buf02 [17] = "NOVA";                  //declara??o de vetor inicializado
    
    Inic_Regs ();									//configurar SFRs
    // Configura??o do TIMER0
    configTMR0(0b11000000); //Passo 1
    initTMR0_08BIT(50); //Passo 2
    configInterruptTMR0(1); //Passo 3
    INTCONbits.PEIE = 1;
//**********************************
	
	IniciaLCD (2);									//inicializar LCD controller HD44780
    initLCD();

//**********************************
    
//EEPROM_Read_Block(0x40, read, 8);
//saveNewUser(buf, buf02);
//if (authenticateUser(buf, buf02)) {
//    escreveCaracterL1(success);
//} else {
//    escreveCaracterL1(invalid);
//}

	while(1);										//loop infinito    		
}
/*******************************************************************
Esta funcao inicializa os resgistradores SFRs.*/
void Inic_Regs (void)
{
    TRISA = 0x00; //PORTA sa?da
    TRISB = 0x00; //PORTB sa?da
    TRISC = 0x00; //PORTC sa?da
    TRISD = 0xF0; //PORTD sa?da
    TRISE = 0x00; //PORTE sa?da
    ADCON1 = 0x0F; //configura pinos dos PORTA e PORTE como digitais
    PORTA = 0; //limpa PORTA
    PORTB = 0; //limpa PORTB
    PORTC = 0; //limpa PORTC
    PORTD = 0xFF; //apaga displays
    PORTE = 0; //limpa PORTE
//********************************************************************
}

void initLCD(){
    EscInstLCD(0x01); //limpa display e mostra cursor piscando na primeira posi??o da primmeira linha
    while (TesteBusyFlag()); //espera LCD controller terminar de executar instru??o
    
    escreveCaracterL1(connect1);
    escreveCaracterL2(connect2);
    
    //delay de 3 segundos
	for(dly=0;dly<50;dly++) _Delay5ms();
    
    escreveCaracterL1(init);
    escreveCaracterL2(limpa);
    
    //delay de 3 segundos
	for(dly=0;dly<50;dly++) _Delay5ms();
    
    escreveCaracterL1(usuario);
}

void escreveCaracterL1(char esc[17]) {
    int i=0;
    EscInstLCD(0x80); //posiciona cursor na primeir aposic??o  da segunda linha
    while (TesteBusyFlag()); //espera LCD controller terminar de executar instru??o
    for(i=0;i<17;i++)						//comando de itera??o
	{
		EscDataLCD(esc[i]); //escreve string no LCD					
        while (TesteBusyFlag()); //espera LCD controller terminar de executar instru??o
	}
}

void escreveCaracterL2(char esc[17]) {
    int i=0;
    EscInstLCD(0xC0); //posiciona cursor na primeir aposic??o  da segunda linha
    while (TesteBusyFlag()); //espera LCD controller terminar de executar instru??o
    for(i=0;i<17;i++)						//comando de itera??o
	{
		EscDataLCD(esc[i]); //escreve string no LCD					
        while (TesteBusyFlag()); //espera LCD controller terminar de executar instru??o
	}
}

void addUser(char x){
    user[position_user] = x;
    position_user++;
    //userOrPass = 1;
    if(position_user == 3 ){
        userOrPass = 0;
        escreveCaracterL1(senha);
        escreveCaracterL2(limpa);
        EscInstLCD(0xC0); //posiciona cursor na primeir aposic??o  da segunda linha
        while (TesteBusyFlag()); //espera LCD controller terminar de executar instru??o
    }
}

void addPassword(char x){
    password[position_password] = x;
    position_password++;
    if(position_password == 6 ){
        position_password = 0;
        position_user = 0;
        PORTCbits.RC0 = PORTCbits.RC0;
        PORTCbits.RC1 = !PORTCbits.RC0;
        if (authenticateUser(user, password)) escreveCaracterL1(success);
        else escreveCaracterL1(invalid);
        escreveCaracterL2(limpa);
        //delay de 3 segundos
        for(dly=0;dly<50;dly++) _Delay5ms();
        userOrPass = 1;  // valida senha
        escreveCaracterL1(usuario);
        EscInstLCD(0xC0); //posiciona cursor na primeir aposic??o  da segunda linha
        while (TesteBusyFlag()); //espera LCD controller terminar de executar instru??o
    }
}

void escreveCaracter(char esc) {
    if(qtdLinha==0){
        EscInstLCD(0xC0); //posiciona cursor na primeir aposic??o  da segunda linha
        while (TesteBusyFlag()); //espera LCD controller terminar de executar instru??o
    }
    
    if(userOrPass){
        EscDataLCD(esc); //escreve string no LCD quando não é senha	
    }else{
        EscDataLCD('*'); //escreve string no LCD quando é senha
    }
    
    while (TesteBusyFlag()); //espera LCD controller terminar de executar instru??o
    Delay10KTCYx(20);
    qtdLinha++;
    
    if(position_user == 3){
        addPassword(esc);
    }else{
        addUser(esc);
    }
    
//    if ('#' == esc){
//        qtdHashtag++;
////        if(qtdHashtag==3){
//            
//            PORTCbits.RC0 = !PORTCbits.RC0;
//            PORTCbits.RC1 = !PORTCbits.RC0;
//            EscInstLCD(0x80); //posiciona cursor na primeir aposic??o  da segunda linha
//            while (TesteBusyFlag()); //espera LCD controller terminar de executar instru??o
//            // Pegar o valor da senha e buscar na EPROM
//            EscStringLCD(password); //escreve string no LCD					
//            while (TesteBusyFlag()); //espera LCD controller terminar de executar instru??o
//            Delay10KTCYx(20);
//            EscDataLCD('#'); //escreve string no LCD					
//            qtdLinha++;
//            
//            
////            qtdHashtag=0;
////        }
//    }
    
    
}


/****************************************************************************** 
 * Rotina de tratamento de interrup??o (ISR)
 * Depois que ocorre a interrup??o, o vetor de interrup??o direciona o fluxo do
 * programa para a localiza??o da Mem?ria de Programa onde encontra-se essa 
 * instru??o, "high_isr".*/
#pragma interrupt high_isr

void high_isr(void) {
    if (INTCONbits.TMR0IF) {
        INTCONbits.TMR0IF = 0;
        TMR0L = 50;

        if (col_1 && controle == 1) {
            if (row_1) escreveCaracter('1');
            else if (row_2) escreveCaracter('6');
            else if (row_3) escreveCaracter('7');
            else if (row_4) escreveCaracter('*');
            col_1 = 0;
            col_2 = 1;
            col_3 = 0;
            col_4 = 0;
            controle = 2;

        } else if (col_2 && controle == 2) {
            if (row_1) escreveCaracter('2');
            else if (row_2) escreveCaracter('5');
            else if (row_3) escreveCaracter('8');
            else if (row_4) escreveCaracter('0');
            col_1 = 0;
            col_2 = 0;
            col_3 = 1;
            col_4 = 0;
            controle = 3;

        } else if (col_3 && controle == 3) {
            if (row_1) escreveCaracter('3');
            else if (row_2) escreveCaracter('4');
            else if (row_3) escreveCaracter('9');
            else if (row_4) escreveCaracter('#');
            col_1 = 0;
            col_2 = 0;
            col_3 = 0;
            col_4 = 1;
            controle = 4;

        } else if (col_4 && controle == 4) {
            if (row_1) escreveCaracter('A');
            else if (row_2) escreveCaracter('B');
            else if (row_3) escreveCaracter('C');
            else if (row_4) escreveCaracter('D');
            col_1 = 1;
            col_2 = 0;
            col_3 = 0;
            col_4 = 0;
            controle = 1;
        }

    }
}

/******************************************************************************
 Esta funcao inicializa os registradores SFRs do TIMER 0*/
void configTMR0(unsigned char configTMR0) {
    //******************************************************************************
    /* Passo 1: Configurar o TMR0 para operar como temporizador e estourar a cada 1s
     * T0CON<7> (TMR0ON = 1): Liga o TIMER 0
     * T0CON<6> (T08BIT = 0): Operar no modo 8 bits
     * T0CON<5> (T0CS = 0)  : Operar com o clock do Fosc/4
     * T0CON<4> (T0SE = 0)  : Operar na borda de subida **
     * T0CON<3> (PSA = 0)   : Operar a partir do prescaler
     * T0CON<2:0> (T0PS2 = 1; T0PS1 = 1; T0PS0 = 1): Configura o prescaler para 1:256,
     * com essa configura??o o prescaler proporciona um atrso de 128us. */
    T0CON = configTMR0; //0b10000111
}

/******************************************************************************
Esta funcao habilita/desabilita a interrup??o por estouro do TIMER0*/
void configInterruptTMR0(unsigned char interruptTMR0) {
    if (interruptTMR0) {
        INTCONbits.TMR0IE = 1; //liga chave individual de interrup??o do TIMER 0     
        INTCONbits.GIE = 1; //liga chave geral de interrup??o             
    } else {
        INTCONbits.TMR0IE = 0; //desliga chave individual de interrup??o do TIMER 0     
        INTCONbits.GIE = 0; //desliga chave geral de interrup??o            
    }
}

/******************************************************************************
Esta funcao inicializa o registrador TIMER0 para o modo 8 bits*/
void initTMR0_08BIT(unsigned char initTMR0) {
    TMR0L = initTMR0;
}

/******************************************************************************
Esta funcao inicializa o registrador TIMER0 para o modo 16 bits*/
void initTMR0_16BIT(unsigned int initTMR0) {
    TMR0H = initTMR0 >> 8;
    TMR0L = initTMR0;
}

