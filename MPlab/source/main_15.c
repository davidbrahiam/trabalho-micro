/*********************************************************************
Nome do arquivo:	main_15.c            
Data:	09 de janeiro de 2009          
Versao:	1.0                              
MPLAB IDE:	v8.20a 
Autor:	Wagner Zanco              
*********************************************************************/
#include <p18f4520.h>	//diretiva de compilação
#include <stdio.h>		//diretiva de compilação
//*******************************************************************
//protótipos de funções
int	produto(int y);		//protótipo da função produto
//*******************************************************************
void main(void)			//função main					
{	
int x = 1;				//declaração de variável local inicializada
	while (x ^ 16)		//executa bloco de comandos enquanto x for diferente de 16
	{
		x = produto();						//chamada à função com retorno de valor	
		printf("x eh igual a %d\r\n", x);	//envia para a USART o valor de x. 
	}
	while (1);			//loop infinito
//*******************************************************************
}
//*******************************************************************
int	produto(int y)		//função produto
	{
	return (y*=2);		//retorna o resultado de y * 2
	}
//*******************************************************************
