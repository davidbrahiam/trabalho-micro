/*******************************************************************************
Nome do arquivo:	elevador.c            
Data:	    		10 de maio de 2017          
Versao:				1.0                              
MPLAB IDE:			v3.55a 
Compilado:          C18 Microchip
Autor:				Jullyano              
********************************************************************************/
#include <p18f4520.h>		//diretiva de compilação
#include "MCC18.h"
#include "standard.h"
#include "debounce.h"
//******************************************************************************
//protótipos de funções
void inicRegs(void);
void setupTIMER0(void);
void high_isr(void);
void confgPWM(void);
void motorControl(unsigned char velocidade, unsigned char sentido);
void ledsControl(unsigned char ligthControl);
void statusElevator(void);
void configDutyCycle(unsigned char x);
void getKey(void);
void getElevator(unsigned char botton);
void displaySetup(unsigned char andar);
//******************************************************************************
//variáveis globais
/* Armazena o status do elevador (variável de controle do elevador)
 * status[0]: Salva a posição da cabine, qual andar o levador está parado
 * status[1]: Salva a posição na qual a porta do levador encontra-se - status[1]<2:0>
 * status[2]: Salva o último botão pressionado pelo teclado
 */
unsigned char status[] = {0,0}; 
unsigned char timeOcioso = 0;   //controle de tempo para o elevador ficar ocioso
const char displayPainel[] = {  //tabela de acendimentos dos displays
                                0x06,   //andar 1
                                0x5B,   //andar 2
                                0x4F,   //andar 3
                                0x66,   //andar 4
                                0x00    //não posicionado
};
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
//variáveis locais    
unsigned char key = 0;  //salva o valor atual do teclado


inicRegs();         // Inicializa o SFRs
confgPWM();         // Configura e inicializa o PWM com duty cycle d5 50%
configDutyCycle(0); // Ajusta o Duty Cycle (0%) 
ledsControl(0);     // Estado default do elevador: ocioso
statusElevator();   // Verifica a posição da cabine do elevador    
   
//rotina principal
	while (1)
	{
        getKey();   //captura o que foi dgitado no teclado
        if(status[1] != 0){ //status[1]: armazena os botões pressionados
            timeOcioso = 0; //reinicia a contagem para entrada no modo ocioso
            switch(status[1]){
                case 1:     //andar de destino igual a 1 
                    if(status[0] == status[1]) break; //botao pressionado igual ao andar
                    getElevator(0);
                    break;
                case 2:     //andar de destino igual a 2
                    if(status[0] == status[1]) break; //botao pressionado igual ao andar
                    getElevator(0);
                    break;
                case 3:     //andar de destino igual a 3
                    if(status[0] == status[1]) break; //botao pressionado igual ao andar
                    getElevator(0);
                    break;
                case 4:     //andar de destino igual a 4
                    if(status[0] == status[1]) break; //botao pressionado igual ao andar
                    getElevator(0);
                    break;
                case 5:     //chama elevador para o andar 1
                    if(status[0] == status[1]) break; //botao pressionado igual ao andar
                    getElevator(1);
                    break;                    
                case 6:     //chama elevador para o andar 2
                    if(status[0] == status[1]) break; //botao pressionado igual ao andar
                    getElevator(1);
                    break;                    
                case 7:     //chama elevador para o andar 3
                    if(status[0] == status[1]) break; //botao pressionado igual ao andar
                    getElevator(1);
                    break;                    
                case 8:     //chama elevador para o andar 4
                    if(status[0] == status[1]) break; //botao pressionado igual ao andar
                    getElevator(1);
                    break;                    
                default:
                    status[1] = 0;
                    break;
            }
        }
        status[1] = 0;
	} 
}
//******************************************************************************
//							funções
//******************************************************************************
/*Esta funcao inicializa os resgistradores SFRs.*/
 void inicRegs(void)
{
	TRISA = 0x0F;       //PORTA<3:0> como entrada e demais pinos do PORTA como saída
	TRISB = 0xFF;		//PORTB entrada
	TRISC = 0x00;		//PORTC saída
	TRISD = 0x00;		//PORTD saída
	TRISE = 0x00;		//PORTE saída
	ADCON1 = 0x0F;		//configura pinos dos PORTA e PORTE como digitais
	PORTA = 0;			//limpa PORTA
	PORTB = 0;			//limpa PORTB
	PORTC = 0;			//limpa PORTC
	PORTD = 0;			//limpa PORTD
	PORTE = 0;			//limpa PORTE
}
/*******************************************************************************
  Esta funcao inicializa os registradores SFRs do TIMER 0*/  
 void setupTIMER0(void)
 {    
//Passo 1: Habilitar a interrupção de estouro de TMR0
	INTCONbits.TMR0IE = 1;	//liga chave individual de interrupção do TIMER 0     
	INTCONbits.GIE = 1;		//liga chave geral de interrupção
/* Passo 2: Configurar o TMR0 para operar como temporizador e estourar a cada 1s
 * T0CON<7> (TMR0ON = 1): Liga o TIMER 0
 * T0CON<6> (T08BIT = 0): Operar no modo 16 bits
 * T0CON<5> (T0CS = 0)  : Operar com o clock do Fosc
 * T0CON<4> (T0SE = 0)  : Operar na borda de subida **
 * T0CON<3> (PSA = 0)   : Operar a partir do prescaler
 * T0CON<2:0> (T0PS2 = 1; T0PS1 = 1; T0PS0 = 1): Configura o prescaler para 1:256,
 * com essa configuração o prescaler proporciona um atrso de 128us. */   
	T0CON = 0b10000111;  
/*Passo 3: Inicializar o TMR0. 
 * Como esta configuração contempla o modo 16 bits, inicializando o TMR0 com o 
 * valor 0, este contará de 0 - 65536, ou seja, 65536 contagens, ao final de tudo, 
 * proporcionando um atraso de 8,36 segundos. */
    TMR0H = 0b00000000; 
    TMR0L = 0b00000000;     
 }
  /*****************************************************************************
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
        
        //******************************************************************
        /* TMR0L = 0x00; TMR0H = 0x00 <-> TMR0 = 0x00. 
         * Com essa configuração o estouro do TIMER 0 ocorrerá a cada 128us, no 
         * entanto precisa-se de um atrso total de 1 minuto, para isso será utilizada 
         * uma variável chamada timeOcioso, quando essa chegar ao valor 7 terá 
         * ocorrido o tail 1 minuto atraso esperado, proporcionando ao final de 
        * tudo, um atraso de 1 minuto.*/
        TMR0H = 0b00000000;              
        TMR0L = 0b00000000;         
        	
        //**********************************************************************
        /* Neste ponto coloca-se as instruções que deverão ocorrer dentro da
         * interrupção, como por exemplo, colocar o elevador em modo acioso.*/
        if(timeOcioso == 7)
        {
            ledsControl(3); //LEDs de status da porta: elevador ocioso
            timeOcioso = 0;
        }
	}
} 
//******************************************************************************
//Ativa sinal PWM com um duty cycle de 50%. 
//Fosc=8MHz e FPWM=15625Hz.
void confgPWM(void)
{
    /* PR2: 127
     * 1- Configura o período do sinal PWM = 15625Hz 
     * 2- Define o valor máximo do número de passos, Np = 512
     */ 
	PR2 = 127;
    /*Bits de Controle: 256 = 0b0100000000 
     * 1- CCPR1L: Bits mais significativos
     * 2- CCP1CON<5:4>: Bits menos significativos
     * 3- Com o valor 256 configura o duty cycle para 50%
     */    
	CCPR1L = 0b01000000;
	CCP1CONbits.DC1B1 = 0;
	CCP1CONbits.DC1B0 = 0;	
    /*TRISC<2>: 0
     * 1- Configura pino do sinal PWM como saída (Full-bridge e single output)
     * 2- Para este microcontrolador o pino é o RC2 (Full-bridge e single output)
     * 3- Configura os pinos do sinal PWM como saída (Full-bridge output)
     */    
	TRISCbits.RC2 = 0;	
    /*Configura TMR2: 
     * 1- Fator de postscaler de 1:1 - T2CON<6:3>
     * 2- Fator de prescaler de 1:1  - T2CON<1:0>
     * 3- Liga T2CON<2>
     */
	T2CON = 0b00000100;
    /*Inicializa IMER2: TMR2 = 0*/
	TMR2 = 0;	  
    /*Ativa o mode PWM: 
     * 1- CCP1CON<7:6> = 00
     * 2- CCP1CON<3:0> = 1100
     * 3- CCP1CON = CCP1CON & 0b00110000 - Ativa o modo PWM (single output)
     * 4- CCP1CON = CCP1CON | 0b00001100 - Ativa modo inicial dos pinos do PWM (Single output)
     */	
	CCP1CON *= 0b00110000;  // (3)
	CCP1CON += 0b00001100;	// (4)
}
//******************************************************************************
//esta função atualiza o duty cycle do sinal PWM
void  configDutyCycle(unsigned char x)
{
    unsigned char temp;								//declaração de variável local
    /* 1- Verifica se o intervalo está entre 0 e 100%, senão ajusta x para tal.
     * 2- Parametriza x e obtém valor dos bits de controle
     * 3- Obtém os seis bits mais significativos: CCPR1L<5:0> 
     * 4- Carrega os bits de controle mais significativos: CCPR1L<5:0> = temp
     */
	if(x>100)x=100;     // (1)						
	x = (x*512)/100;    // (2) - Havia um erro aqui!				
	temp =  x >> 2;		// (3)						
	CCPR1L = temp;		// (4)		
    
    /*Inicializa o segundo bit menos significativo dos bits de controle, CCP1CON<5>. 
     * 1- Verifica x<1> 
     * 2- Se x<1> == 1, CCP1CON<5> = 1
     * 3- Senão, CCP1CON<5> = 0
     */
	temp = x & 2;                       // (1)
	if (temp==2)CCP1CONbits.DC1B1 = 1;	// (2) - Havia um erro aqui!		
	else 	CCP1CONbits.DC1B1 = 0;      // (3)
    
    /*Inicializa o bit menos significativo dos bits de controle, CCP1CON<4>.
     * 1- Verifica x<0>  
     * 2- Se x<0> == 1, CCP1CON<4> = 1
     * 3- Senão, CCP1CON<4> = 0
     */
	temp = x & 1;                       // (1)
	if (temp==1)CCP1CONbits.DC1B0 = 1;  // (2)
	else 	CCP1CONbits.DC1B0 = 0;      // (3)
}
//******************************************************************************
//esta função configura o motor: velocidade e sentido
void motorControl(unsigned char velocidade, unsigned char sentido)
{
    //motor gira no sentido de subir (horário)
    if(sentido == 1){
        PORTCbits.RC0 = 1;
        PORTCbits.RC1 = 0;
    }
    //motor gira no sentido de descer (anti-horário)
    else{
        PORTCbits.RC0 = 0;
        PORTCbits.RC1 = 1;
    }
    configDutyCycle(velocidade); //velocidade relativa a 50% do duty cycle
}
//******************************************************************************
//esta função controla a operação dos leds da cabine do elevador
void ledsControl(unsigned char ligthControl)
{
    switch(ligthControl)
    {
        case 0b00000011:    //código 3: Parado e com porta aberta 
            PORTE = 0;
            PORTEbits.RE0 = 1;  //luz da cabine acesa
            PORTEbits.RE1 = 1;  //porta aberta
            break;
        case 0b00000100:    //código 8: Parado e com porta fechada (ocioso)
            PORTE = 0;
            PORTEbits.RE2 = 1;  //porta fechada e luz da cabine apagada
            break;
        case 0b00000101:    //código 9: Elevador em movimento 
            PORTE = 0;
            PORTEbits.RE0 = 1;  //luz da cabine acesa
            PORTEbits.RE2 = 1;  //porta fechada 
        default:    //modo incial do elevador - ocioso
            PORTE = 0;
            PORTEbits.RE2 = 1;  //porta fechada e luz da cabine apagada            
            break;
    }
}
//******************************************************************************
//esta função define o status do elevador: posicionamento da cabine e porta
void statusElevator(void)
{
    //verifica se o elevador está no andar 1
    if(PORTAbits.RA0 == 0){
        if(debounce(PORTA,0) == 0){
            status[0] = 1;  //atualiza a variável status[0] com o andar corrente
            displaySetup(1);    //acende o display com o valor 1: andar 1
        } 
        else status[0] = 0;
    } 
    //verifica se o elevador está no andar 2
    else if(PORTAbits.RA1 == 0){
        if(debounce(PORTA,1) == 0){
            status[0] = 2;  //atualiza a variável status[0] com o andar corrente
            displaySetup(2);    //acende o display com o valor 2: andar 2
        } 
        else status[0] = 0;
    }  
    //verifica se o elevador está no andar 3
    else if(PORTAbits.RA2 == 0){
        if(debounce(PORTA,2) == 0){
            status[0] = 3;  //atualiza a variável status[0] com o andar corrente
            displaySetup(3);    //acende o display com o valor 3: andar 3
        } 
        else status[0] = 0;
    }  
    //verifica se o elevador está no andar 4
    else{
        if(debounce(PORTA,3) == 0){
            status[0] = 4;  //atualiza a variável status[0] com o andar corrente
            displaySetup(4);    //acende o display com o valor 4: andar 4
        } 
        else status[0] = 0;
    } 
}
//******************************************************************************
//esta função faz a leitura do teclado de funções do elevador
void getKey(void)
{
    //verifica o teclado apenas se houve modificação em PORTB
    if(PORTB != 0XFF){
        //checa se o pino RB0 alterou
        if(PORTBbits.RB0 == 0){
            //confirma se houve realmente alteração em RB0 
            if(debounce(PORTB,0) == 0) status[1] = 1; //BT1 pressionado
        }
        //checa se o pino RB1 alterou
        else if(PORTBbits.RB1 == 0){
            //confirma se houve realmente alteração em RB1
            if(debounce(PORTB,1) == 0) status[1] = 2; //BT2 pressionado
        }     
        //checa se o pino RB2 alterou
        else if(PORTBbits.RB2 == 0){
            //confirma se houve realmente alteração em RB2
            if(debounce(PORTB,2) == 0) status[1] = 3; //BT3 pressionado
        }    
        //checa se o pino RB3 alterou
        else if(PORTBbits.RB3 == 0){
            //confirma se houve realmente alteração em RB3 
            if(debounce(PORTB,3) == 0) status[1] = 4; //BT4 pressionado
        }    
        //checa se o pino RB4 alterou
        else if(PORTBbits.RB4 == 0){
            //confirma se houve realmente alteração em RB4 
            if(debounce(PORTB,4) == 0) status[1] = 5; //CH1 pressionado
        }        
        //checa se o pino RB5 alterou
        else if(PORTBbits.RB5 == 0){
            //confirma se houve realmente alteração em RB5 
            if(debounce(PORTB,5) == 0) status[1] = 6; //CH2 pressionado
        }   
        //checa se o pino RB6 alterou
        else if(PORTBbits.RB6 == 0){
            //confirma se houve realmente alteração em RB6 
            if(debounce(PORTB,6) == 0) status[1] = 7; //CH3 pressionado
        }           
        //checa se o pino RB7 alterou
        else if(PORTBbits.RB7 == 0){
            //confirma se houve realmente alteração em RB7 
            if(debounce(PORTB,7) == 0) status[1] = 8; //CH4 pressionado
        } 
        else status[1] = 0; //não houve alteração tempo suficiente em PORTB
    }
    else status[1] = 0; //não houve alteração em PORTB
}
//******************************************************************************
//esta função controla a movimentação da cabine do elevador
void getElevator(unsigned char class)
{
    //variáveis locais
    unsigned char passos = 0;   //contador de andares
    
    ledsControl(9); //LEDs de status da porta: elevador em movimento (9)
    passos = status[0] - status[1]; //verifica se o elevador sobe ou desce
    
    if(class == 0){
        if(passos > 0){            
            motorControl(50,0); //liga o motor descendo                           
        }
        else{
            motorControl(50,1); //liga o motor subindo
        }        
    }
    else{
        if(passos > 0){            
            motorControl(50,1); //liga o motor subindo                           
        }
        else{
            motorControl(50,0); //liga o motor descendo
        }
    }
    do{
        statusElevator();   //atualiza o status do elevador
    }while(status[0] != status[1]); 
    motorControl(0,0); //desliga o motor, pára o elevador
    ledsControl(3); //LEDs de status da porta: elevador parado    
}
//******************************************************************************
//esta fução é responsável por atualizar o display do painel de controle
void displaySetup(unsigned char andar){
    PORTD = displayPainel[andar - 1];   //o -1 é apenas para coerência com o valor
                                        // do andar passado para a função em relação
                                        // ao armazenado na tabela
}
//******************************************************************************