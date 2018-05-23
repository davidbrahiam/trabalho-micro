/*********************************************************************
Nome do arquivo:	main_12.c            
Data:	    		04 de janeiro de 2009          
Versao:				1.0                              
MPLAB IDE:			v8.20a 
Autor:				Wagner Zanco              
*********************************************************************/
#include <p18f4520.h>		//diretiva de compilação
#include <stdio.h>			//diretiva de compilação
//*******************************************************************
void main(void)				//função main					
{	
int x;														//declaração de variável local inicializada
	for(x=0;x<=5;x++)	printf("x eh igual a %d\r\n", x);	//envia para a USART o valor de x. 
	while (1);												//loop infinito
//*******************************************************************
}