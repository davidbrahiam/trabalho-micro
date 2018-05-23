/*********************************************************************
Nome do arquivo:	main_09.c            
Data:	    		26 de novembro de 2008          
Versao:				1.0                              
MPLAB IDE:			v8.20a 
Autor:				Wagner Zanco              
*********************************************************************/
#include <stdio.h>					//diretiva de compilação
//*******************************************************************
void main(void)						//função main					
{	
unsigned char x = 5;				//declaração de variável inicializada
unsigned char y;					//declaração de variável não inicializada
unsigned char z;					//declaração de variável não inicializada
	y = x << 2;						//y = x multiplicado por 4
	z = y >> 1;						//z = y dividido por 2
	printf("y = %d\r\n",y);			//envia para a USART o valor de y
	printf("z = %d\r\n",z);			//envia para a USART o valor de z
	while (1);						//loop infinito	
//*******************************************************************
}
