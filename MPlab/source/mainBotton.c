/* 
 * File:   mainBotton.h
 * Author: Carlúcio Silva
 *
 * Created on 8 de Março de 2017, 14:37
 */

/*Local para inserção dos arquivos de cabeçalho*/
#include <p18F4520.h>
#include "MCC18.h"

main()
{
    /*Configuração dos SFRs do projeto para ler um botão e acender um LED*/
    TRISA = 0b00000000;     // Configura o registro PORTA como saída
    TRISB = 0b00000001;     // Configura o pino RB0 do registro PORTB como entrada 
                            // e os demais como saída. Este pino será utilizado para 
                            // ler um botão.
    TRISC = 0b00000000;     // Configura o registro PORTC como saída
    TRISD = 0b00000000;     // Configura o registro PORTD como saída
    TRISE = 0b00000000;     // Configura o registro PORTE como saída
    ADCON1 = 0b00001111;    // Configura os registros PORTA e PORTE como digitais
    PORTA = 0b00000000;     // Limpa o registro PORTA, coloca seus bits em LÓGICO 0
    PORTB = 0b00000000;     // Limpa o registro PORTB, coloca seus bits em LÓGICO 0
    PORTC = 0b00000000;     // Limpa o registro PORTC, coloca seus bits em LÓGICO 0
    PORTD = 0b00000001;     // Limpa o registro PORTD, coloca seus bits em LÓGICO 0, 
                            // exceto o bit RD0, que ficará com LÓGICO 1. Neste bit (pino)
                            // estará conectado um LED.    
    PORTE = 0b00000000;     // Limpa o registro PORTE, coloca seus bits em LÓGICO 0
    /*Fim da configuração dos SFRs*/
    
    while(1)                // Loop principal
    {
        if(PORTBbits.RB0==0)      // Verifica se o botão BT1 foi pressionado
        {
            PORTDbits.RD0=0;      // Se foi pressionado, acender LED
        }
        else PORTDbits.RD0=1;     // Se não foi pressionado, apagar o LED
    }
}