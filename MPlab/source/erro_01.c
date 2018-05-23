/*********************************************************************
Nome do arquivo:	erro_01.c            
Data:	    	13 de novembro de 2008          
Versao:		1.0                              
MPLAB IDE:	v8.20a
Autor:		Wagner Zanco              
*********************************************************************/
#include <stdio.h>				//diretiva de compilação
//*******************************************************************
const unsigned int  x = 10;     //declaração de variável global inicializada
//*******************************************************************
void main(void)					//função main					
{	
	x = 20;						//atribui a variável x o valor  20
	while (1);					//loop infinito	
//*******************************************************************
}
