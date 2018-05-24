/*********************************************************************
Nome do arquivo:	main_08.c            
Data:	    		23 de novembro de 2008          
Versao:				1.0                              
MPLAB IDE:			v8.20a 
Autor:				Wagner Zanco              
*********************************************************************/
#include <stdio.h>									//diretiva de compilação
//*******************************************************************
void main(void)										//função main					
{	
unsigned char x;									//declaração de variável não inicializada
unsigned char y;									//declaração de variável não inicializada
unsigned int z;										//declaração de variável não inicializada
unsigned int w;										//declaração de variável não inicializada
	x = 37;											//atribui a z o valor 37
	y = 6;											//atribui a z o valor 37
	z = x / y; 										//z = x / y
	w = x%y;										//w recebe o resto da divisão x/y
	printf("37/6 = %d",z);							//envia para a USART o rsultado da divisão x/y
	printf(", e o resto da divisao = %d\r\n",w);	//envia para a USART o resto da divisão x/y
	while (1);										//loop infinito	
//*******************************************************************
}
