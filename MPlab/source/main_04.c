/*********************************************************************
Nome do arquivo:	main_04.c            
Data:	    		13 de novembro de 2008          
Versao:				1.0                              
MPLAB IDE:			v8.20a 
Autor:				Wagner Zanco              
*********************************************************************/
#include <stdio.h>			//diretiva de compilação
//*******************************************************************
unsigned char x,y;			//declaração de variável global não inicializada
unsigned char y;			//declaração de variável global não inicializada
unsigned int  z = 0;		//declaração de variável global inicializada
//*******************************************************************
void main(void)				//função main					
{	
	x = 20;					//inicializa a variável x com 20
	y = 50;					//inicializa a variável x com 50
	z = (int) x * y;		//z = x * 1
	printf("z = %u\r\n",z);	//envia para a USART o valor de z
	while (1);				//loop infinito	
//*******************************************************************
} 
