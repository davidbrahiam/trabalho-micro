/*********************************************************************
Nome do arquivo:	main_06.c            
Data:	    		14 de novembro de 2008          
Versao:				1.0                              
MPLAB IDE:			v8.20a 
Autor:				Wagner Zanco              
*********************************************************************/
#include <stdio.h>				//diretiva de compilação
//*******************************************************************
unsigned char x = 0;			//declaração de variável global inicializada
//*******************************************************************
//declaração de função
char inc_y(void)				//função inc_y
{
	static char y = 0;			//declaração de variável local inicializada
	y++;						//y = y + 1
	return y;					//retorna de função com valor 
}
//*******************************************************************
void main(void)					//função main					
{		
	while(x < 10)				//executa bloco de código a seguir enquanto x < 10
	{
		x = inc_y();			//chamda à função com retorno de valor
		printf("x = %u\n",x); 	//envia para a USART o valor de x
	}
	while(1);					//loop infinito
//*******************************************************************
}