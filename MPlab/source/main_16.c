/*********************************************************************
Nome do arquivo:	main_16.c            
Data:	26 de janeiro de 2009          
Versao:	1.0                              
MPLAB IDE:	v8.10 
Autor:	Wagner Zanco              
*********************************************************************/
#include <p18f452.h>	//diretiva de compilação
#include <stdio.h>	//diretiva de compilação
//*******************************************************************
//protótipos de funções
int	produto(int y);	//protótipo da função produto
//*******************************************************************
void main(void)		//função main					
{	
int x = 1;
	while (x ^ 16)		//executa bloco de comandos enquanto x for diferente de 16
	{
		x = produto(x);						//sai do laço se x > 3	
		printf("x eh igual a %d\r\n", x);	//envia para a USART o valor de x. 
	}
	while (1);		//loop infinito
//*******************************************************************
}
