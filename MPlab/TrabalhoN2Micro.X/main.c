/*********************************************************************
Nome do arquivo:    main_32.c            
Data:               29 de mar?o de 2010          
Versao:             1.0                              
MPLAB IDE:          v8.20a 
Autor:              Wagner Zanco              
 *********************************************************************/
#include <p18f4550.h>           //diretiva de compila??o
#include "MCC18_18F4550.h"
#include "Lcd_8bits.h"          //diretiva de compila??o
#include <stdio.h>
#include <stdlib.h>
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
void Inic_Regs(void);
void escreveLed(char esc);
void initLCD(void);
char timer0(void);
void delay(void);
void startCursorL2(void);
void escreveCaracterL1(char esc[17]);
void escreveCaracterL2(char esc[17]);
void eliminarCaracter(void);

//******************************************************************************
// Declara??o de vari?veis globais
unsigned char limpa [17] = " "; //declara??o de vetor inicializado
unsigned char userG [3] = ""; //declara??o de vetor inicializado
unsigned char senha [17] = "SENHA:"; //declara??o de vetor inicializado
unsigned char usuario [17] = "USUARIO:"; //declara??o de vetor inicializado
unsigned char nova_senha [17] = "NOVA SENHA:"; //declara??o de vetor inicializado
unsigned char password[6] = "";
unsigned char pass[6] = "";
unsigned char pass_repeat[6] = "";
unsigned char newUser [3] = ""; //declara??o de vetor inicializado
unsigned char rootMessage [17] = "ROOT:";
unsigned char successSenha [17] = "SENHA OK! ";
unsigned char alterRootL1 [17] =  "MOD. ALTERACAO";
unsigned char alterRootL2 [17] =  "DE USUARIO ROOT";
unsigned char repeatMessage [17] = "REPETE:          ";
unsigned char success [17] = "SEJA BEM VINDO! "; //declara??o de vetor inicializado
unsigned char invalid [17] = "SENHA INVALIDA  "; //declara??o de vetor inicializado

int userOrPass = 1; // 1=user e 0=passward
int position_password = 0;
int position_user = 0;
int qtdHashtag = 0;
int qtdAsterisco = 0;
int position_pass = 0;
int isRoot = 0;
int isNewUser = 0;
int needAuth = 1;
int needRepeat = 1;
int initializing = 1;
char controle = 1;
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

void main(void) //fun??o main                   
{
    Inic_Regs(); //configurar SFRs
    // Configura??o do TIMER0
    configTMR0(0b11000000); //Passo 1
    initTMR0_08BIT(50); //Passo 2
    configInterruptTMR0(1); //Passo 3
    INTCONbits.PEIE = 1;
    //**********************************

    IniciaLCD(2); //inicializar LCD controller HD44780
    initLCD();

    //**********************************
    while (1); //loop infinito          
}

/*******************************************************************
Esta funcao inicializa os resgistradores SFRs.*/
void Inic_Regs(void) {
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

void initLCD() {
    unsigned char connect1 [17] = "Connecting..."; //declara??o de vetor inicializado
    unsigned char connect2 [17] = "Please Wait!"; //declara??o de vetor inicializado
    unsigned char init [17] = "OK!"; //declara??o de vetor inicializado
    EscInstLCD(0x01); //limpa display e mostra cursor piscando na primeira posi??o da primmeira linha
    while (TesteBusyFlag()); //espera LCD controller terminar de executar instru??o

    escreveCaracterL1(connect1);
    escreveCaracterL2(connect2);

    delay();
    escreveCaracterL1(init);
    escreveCaracterL2(limpa);
    
    delay();
    escreveCaracterL1(usuario);
    startCursorL2();
    initializing = 0;
    
    // salva um novo usuário root caso não exista nenhum
    saveRoot();
}

void delay(){
    int dly = 0;
    //delay de 3 segundos
    for (dly = 0; dly < 50; dly++) _Delay5ms();
}

void escreveCaracterL1(char esc[17]) {
    int i = 0;
    EscInstLCD(0x80); //posiciona cursor na primeir aposic??o  da segunda linha
    while (TesteBusyFlag()); //espera LCD controller terminar de executar instru??o
    for (i = 0; i < 17; i++) //comando de itera??o
    {
        EscDataLCD(esc[i]); //escreve string no LCD                 
        while (TesteBusyFlag()); //espera LCD controller terminar de executar instru??o
    }
}

void escreveCaracterL2(char esc[17]) {
    int i = 0;
    startCursorL2();
    for (i = 0; i < 17; i++) //comando de itera??o
    {
        EscDataLCD(esc[i]); //escreve string no LCD                 
        while (TesteBusyFlag()); //espera LCD controller terminar de executar instru??o
    }
}

void addUser(char x) {
    userG[position_user] = x;
    position_user++;
    if (position_user == 3) {
        userOrPass = 0;
        escreveCaracterL1(senha);
        escreveCaracterL2(limpa);
        startCursorL2();
    }
}

void addPassword(char x) {
    password[position_password] = x;
    position_password++;
    if (position_password == 6) {
        position_password = 0;
        position_user = 0;
        escreveCaracterL2(limpa);
        if (authenticateUser(userG, password)) {
            PORTCbits.RC0 = 1;
            escreveCaracterL1(success);
            delay();
            PORTCbits.RC0 = 0;
        } else {
            PORTCbits.RC1 = 1;
            escreveCaracterL1(invalid);
            delay();
            PORTCbits.RC1 = 0;
        }
        
        userOrPass = 1; // valida senha
        escreveCaracterL1(usuario);
        escreveCaracterL2(limpa);
        startCursorL2();
        delay();
    }
}

void startCursorL2(){
    EscInstLCD(0xC0); //posiciona cursor na primeir aposic??o  da segunda linha
    while (TesteBusyFlag()); //espera LCD controller terminar de executar instru??o
}

void escreveL2(char x) {
    pass[position_pass] = x;
    position_pass++;
}

void escreveCaracter(char esc) {
    if (initializing) return;
    if ('*' == esc) {
        qtdAsterisco++;
        if (qtdAsterisco == 3) {
            isRoot = 1;
            userOrPass = 0;
            escreveCaracterL1(alterRootL1);
            escreveCaracterL2(alterRootL2);
            delay();
            escreveCaracterL1(rootMessage);
            escreveCaracterL2(limpa);
            startCursorL2();
            qtdAsterisco = 0;
        }
        Delay10KTCYx(20);
        return;
    } else qtdAsterisco = 0;

    if ('#' == esc) {
        qtdHashtag++;
        if (qtdHashtag == 4) {
            unsigned char cadastroL1 [17] = "MODO DE CADASTRO";
            unsigned char cadastroL2 [17] = "  DE USUARIOS   ";
            escreveCaracterL1(cadastroL1);
            escreveCaracterL2(cadastroL2);
            delay();
            escreveCaracterL1(rootMessage);
            escreveCaracterL2(limpa);
            startCursorL2();
            qtdHashtag = 0;
            isNewUser = 1;
            needAuth = 1;
            userOrPass = 0;
        }
        Delay10KTCYx(20);
        return;
    } else qtdHashtag = 0;
    if (isRoot) {
        escreveL2(esc);
        EscDataLCD('*'); //escreve string no LCD quando é senha
        while (TesteBusyFlag()); //espera LCD controller terminar de executar instru??o
        Delay10KTCYx(20);
        if (position_pass == 6) {
            position_pass = 0;
            position_user = 0;
            if (needAuth) {
                int i = 0;
                for (i = 0; i < 3; i++) userG[i] = '9';
                if (authenticateUser(userG, pass)) {
                    needAuth = 0;
                    needRepeat = 1;
                    escreveCaracterL1(nova_senha);
                    escreveCaracterL2(limpa);
                } else {
                    isRoot = 0;
                    needRepeat = 1;
                    needAuth = 1;
                    userOrPass = 1;
                    escreveCaracterL1(invalid);
                    escreveCaracterL2(limpa);
                    delay();
                    escreveCaracterL1(usuario);
                }
            } else if (needRepeat) {
                int i = 0;
                unsigned char message [17] = "ROOT: 999";
                for (i = 0; i < 6; i++)pass_repeat[i] = pass[i];
                escreveCaracterL1(message);
                startCursorL2();
                EscDataLCD('R');
                EscDataLCD('E');
                EscDataLCD('P');
                EscDataLCD('E');
                EscDataLCD('T');
                EscDataLCD('E');
                EscDataLCD(':');
                needRepeat = 0;
                return;
            } else {
                int j = 1;
                int i = 0;
                position_pass = 0;
                position_password = 0;
                position_user = 0;
                isRoot = 0;
                needRepeat = 1;
                needAuth = 1;
                userOrPass = 1;
                for (i = 0; i < 6; i++) {
                    if (j) {
                        if (pass_repeat[i] == pass[i])j = 1;
                        else j = 0;
                    } else break;
                }
                if (j) {
                    updateRoot(pass);
                    escreveCaracterL1(successSenha);
                } else escreveCaracterL1(invalid);
                escreveCaracterL2(limpa);
                delay();
                escreveCaracterL1(usuario);
            }
            Delay10KTCYx(20);
            startCursorL2();
        }
    } else if (isNewUser) {
        if (needAuth) {
            escreveL2(esc);
            EscDataLCD('*');
            while (TesteBusyFlag()); //espera LCD controller terminar de executar instru??o
            Delay10KTCYx(20);
            if (position_pass == 6) {
                int i = 0;
                position_pass = 0;
                position_user = 0;
                for (i = 0; i < 3; i++) userG[i] = '9';
                if (authenticateUser(userG, pass)) {
                    unsigned char novo_usuario [17] = "NOVO USUARIO:";
                    needAuth = 0;
                    needRepeat = 1;
                    userOrPass = 1;
                    escreveCaracterL1(novo_usuario);
                    escreveCaracterL2(limpa);
                    Delay10KTCYx(20);
                    startCursorL2();
                } else {
                    isNewUser = 0;
                    needRepeat = 1;
                    needAuth = 1;
                    userOrPass = 1;
                    escreveCaracterL1(invalid);
                    escreveCaracterL2(limpa);
                    delay();
                    escreveCaracterL1(usuario);
                    startCursorL2();
                }
            }
        } else if (userOrPass) {
            escreveL2(esc);
            EscDataLCD(esc);
            while (TesteBusyFlag()); //espera LCD controller terminar de executar instru??o
            Delay10KTCYx(20);
            if (position_pass == 3) {
                int i = 0;
                position_pass = 0;
                position_user = 0;
                needRepeat = 1;
                userOrPass = 0;
                for (i = 0; i < 3; i++) userG[i] = pass[i];
                escreveCaracterL1(senha);
                escreveCaracterL2(limpa);
                Delay10KTCYx(20);
                startCursorL2();
            }            
        } else if (needRepeat) {
            escreveL2(esc);
            EscDataLCD('*');
            while (TesteBusyFlag()); //espera LCD controller terminar de executar instru??o
            Delay10KTCYx(20);
            if (position_pass == 6) {
                int i = 0;
                position_pass = 0;
                EscInstLCD(0x80);
                while (TesteBusyFlag()); 
                EscDataLCD('R');
                EscDataLCD('O');
                EscDataLCD('O');
                EscDataLCD('T');
                EscDataLCD(':');
                EscDataLCD(' ');
                while (TesteBusyFlag()); 
                for (i = 0; i < 3; i++) EscDataLCD(userG[i]);
                startCursorL2();
                for (i = 0; i < 6; i++)pass_repeat[i] = pass[i];
                EscDataLCD('R');
                EscDataLCD('E');
                EscDataLCD('P');
                EscDataLCD('E');
                EscDataLCD('T');
                EscDataLCD('E');
                EscDataLCD(':');
                needRepeat = 0;
            }
        }else{
            escreveL2(esc);
            EscDataLCD('*');
            while (TesteBusyFlag()); //espera LCD controller terminar de executar instru??o
            Delay10KTCYx(20);
            if (position_pass == 6) {
                int j = 1;
                int i = 0;
                position_pass = 0;
                position_password = 0;
                position_user = 0;
                isNewUser = 0;
                needRepeat = 1;
                needAuth = 1;
                userOrPass = 1;
                for (i = 0; i < 6; i++) {
                    if (j) {
                        if (pass_repeat[i] == pass[i])j = 1;
                        else j = 0;
                    } else break;
                }
                if (j) {
                    saveNewUser(userG, pass);
                    escreveCaracterL1(successSenha);
                } else escreveCaracterL1(invalid);
                escreveCaracterL2(limpa);
                delay();
                escreveCaracterL1(usuario);
                startCursorL2();
            }
        }
    } else {
        if (userOrPass) EscDataLCD(esc); //escreve string no LCD quando não é senha   
        else EscDataLCD('*'); //escreve string no LCD quando é senha
        
        while (TesteBusyFlag()); //espera LCD controller terminar de executar instru??o
        Delay10KTCYx(20);

        if (position_user == 3) addPassword(esc);
        else addUser(esc);
    }
}

void eliminarCaracter(){
    if(userOrPass){
        if (position_user) position_user--;
        else return;
    }else if(isRoot){
        if (position_pass) position_pass--;
        else return;
    }else if (isNewUser){
        if (position_pass) position_pass--;
        else return;
    }else{
        if(position_password) position_password--;
        else return;
    }
    EscInstLCD(0x10); 
    while (TesteBusyFlag()); 
    EscDataLCD(' '); //limpa display e mostra cursor piscando na primeira posi??o da primmeira linha
    while (TesteBusyFlag()); //espera LCD controller terminar de executar instru??o
    EscInstLCD(0x10); //limpa display e mostra cursor piscando na primeira posi??o da primmeira linha
    while (TesteBusyFlag()); //espera LCD controller terminar de executar instru??o
    Delay10KTCYx(20);
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
//            if (row_1) escreveCaracter('A');
//            else if (row_2) escreveCaracter('B');
//            else if (row_3) escreveCaracter('C');
            if (row_4) eliminarCaracter();
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