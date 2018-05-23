/*********************************************************************
Nome do arquivo:	main_36_pwm.c            
Data:	    		17 de maio de 2010          
Versao:				1.0                              
MPLAB IDE:			v8.20a 
Autor:				Wagner Zanco              
*********************************************************************/
#include <p18f4520.h>		//diretiva de compilação
#include "MCC18.h"
//********************************************************************
//protótipos de funções
void Inic_Regs (void);
void Confg_PWM (void);
void Config_Duty_Cycle (int x);
//********************************************************************
void main(void)							//função main					
{	
    
Inic_Regs ();           // Inicializa o SFRs
Confg_PWM ();           // Configura e inicializa o PWM com duty cycle d5 50%
//Config_Duty_Cycle(30);  // Ajusta o Duty Cycle (%) 
    
//rotina principal
	while (1)
	{
	} 
}//******************************************************************
//							funções
//*******************************************************************
/*Esta funcao inicializa os resgistradores SFRs.*/
 void Inic_Regs (void)
{
	TRISA = 0x00;						//PORTA saída
	TRISB = 0x01;						//RB0 como entrada e demais pinos do PORTB como saída
	TRISC = 0x00;						//PORTC saída
	TRISD = 0x00;						//PORTD saída
	TRISE = 0x00;						//PORTE saída
	ADCON1 = 0x0F;						//configura pinos dos PORTA e PORTE como digitais
	PORTA = 0;							//limpa PORTA
	PORTB = 0;							//limpa PORTB
	PORTC = 0;							//limpa PORTC
	PORTD = 0x00;						//apaga displays
	PORTE = 0;							//limpa PORTE
}
//********************************************************************
//Ativa sinal PWM com um duty cycle de 50%. 
//Fosc=8MHz e FPWM=15625Hz.
void Confg_PWM (void)
{
    /* PR2: 127
     * 1- Configura o período do sinal PWM = 15625Hz 
     * 2- Define o valor máximo do número de passos, Np = 512
     */ 
	//PR2 = 127;
    PR2 = 15;
    
    /*Bits de Controle: 256 
     * 1- CCPR1L: Bits mais significativos
     * 2- CCP1CON<5:4>: Bits menos significativos
     * 3- Com o valor 256 configura o duty cycle para 50%
     */    
	CCPR1L = 0b00001000;
	CCP1CONbits.DC1B1 = 0;
	CCP1CONbits.DC1B0 = 0;	
	
    /*TRISC<2>: 0
     * 1- Configura pino do sinal PWM como saída (Full-bridge e single output)
     * 2- Para este microcontrolador o pino é o RC2 (Full-bridge e single output)
     * 3- Configura os pinos do sinal PWM como saída (Full-bridge output)
     */    
	TRISCbits.RC2 = 0;	
    TRISDbits.RD5 = 0; //  (3)
    TRISDbits.RD6 = 0; //  (3)
    TRISDbits.RD7 = 0; //  (3)
    
    /*Configura TMR2: 
     * 1- Fator de postscaler de 1:1 - T2CON<6:3>
     * 2- Fator de prescaler de 1:1  - T2CON<1:0>
     * 3- Liga T2CON<2>
     */
	//T2CON = 0b00000100;
    
    /*Inicializa IMER2: TMR2 = 0*/
	TMR2 = 0;	
    
    /*Ativa o mode PWM: 
     * 1- CCP1CON<7:6> = 00
     * 2- CCP1CON<3:0> = 1100
     * 3- CCP1CON = CCP1CON & 0b00110000 - Ativa o modo PWM (Full-bridge e single output)
     * 4- CCP1CON = CCP1CON | 0b00001100 - Ativa modo inicial dos pinos do PWM (Single output)
     * 5- CCP1CON = CCP1CON | 0b01001100 - Ativa modo inicial dos pinos do PWM (Full-bridge output - Forward mode)
     * 6- CCP1CON = CCP1CON | 0b11001100 - Ativa modo inicial dos pinos do PWM (Full-bridge output - Reverse mode)
     */	
	//CCP1CON *= 0b00110000;  // (3)
	//CCP1CON += 0b00001100;	// (4)
	CCP1CON = 0b01001100;	// (5)   
    //CCP1CON += 0b11001100;	// (6)
}//******************************************************************
//esta função atualiza o duty cycle do sinal PWM
void  Config_Duty_Cycle (int x)
{
int temp;								//declaração de variável local

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
}//*******************************************************************
