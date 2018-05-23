/*********************************************************************
Nome do arquivo:	main_05.c            
Data:	    		13 de novembro de 2008          
Versao:				1.0                              
MPLAB IDE:			v8.20a 
Autor:				Wagner Zanco              
*********************************************************************/
#include <stdio.h>			//diretiva de compilação
//*******************************************************************
unsigned char x;			/*declaração de variável global não inicializada*/
//*******************************************************************
void main(void)				//função main					
{		
	x = 10;					//seqüência de comandos
	soma();					//chamada à função soma()
	printf("x = %u\n",x); 	//envia para a USART o valor de x
	while (1);				//loop infinito
//*******************************************************************
}