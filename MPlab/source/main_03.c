/*********************************************************************
Nome do arquivo:	main_03.c            
Data:	    		13 de novembro de 2008          
Versao:				1.0                              
MPLAB IDE:			v8.20a 
Autor:				Wagner Zanco              
*********************************************************************/
#include <stdio.h>					//diretiva de compilação
//*******************************************************************
//declaração de função
void _parametros (unsigned char c)	//função _parametros
{
	unsigned char y;				//declaração de variável local inicializada
	y = c + 25;						//y = y + 1
	printf("y = %u\n",y); 			//envia para a USART o valor de x
}
//*******************************************************************
//*******************************************************************
void main(void)						//função main					
{	
	_parametros(15);				//chamada à função com passagem de parâmetro
	while (1);						//loop infinito	
//*******************************************************************
}
