/*********************************************************************
Nome do arquivo:	main_25.c            
Data:				16 de fevereiro de 2010          
Versao:				1.0                              
MPLAB IDE:			v8.20 
Autor:				Wagner Zanco              
*********************************************************************/
#include <p18f4520.h>		//diretiva de compilação
#include <stdio.h>			//diretiva de compilação
//*******************************************************************
//definições
#define max  55				//diretiva de compilação
//*******************************************************************
//protótipos de funções
int divisao (int x, int y);
//*******************************************************************
//variáveis globais
char buffer[max] = "";		//declaração de vetor de caracteres
//*******************************************************************
void main (void)
{
int x = 150;				//declaração de variável local incializada
int y = 2;					//declaração de variável local incializada
int z =0;					//declaração de variável local incializada
	z = divisao(x,y);		//chamada à função com passagem de parâmetros
	while(1);				//loop infinito
}
//*******************************************************************
int _user_putc (char c)
{
static char x=0;			//declaração de variável local estática  inicializada
	if(c!=0 && c!=0x0A)		//executa bloco de código se c!=0 e c!=0Ah
	{
		buffer[x]=c;		//carrega caractere recebido no índice do buffer apontado por x
		x++;				//incrementa x
		buffer[x]='\0';		//carrega o caractere null no índice apontado por x
	}
	else 	if(c==0x0A)		//se não e se c==0Ah executa bloco de código
		{
		buffer[x]=c;		//carrega o caractere recebido no índice apontado por x
		x++;				//incrementa x
		buffer[x]='\0';		//carrega o caractere null no índice apontado por x
		x=0;				//x=0
		}
}
//*******************************************************************
int divisao (int x, int y)		//função divisao
{
int result;						//declaração  de variável local não  inicializada
	if (y==0) 					//se denominador é zero
	{
		fprintf(_H_USER,"A divisao nao foi efetuada, o denominador eh zero.\n");	/*envia para a UART que a divisão não pode ser 
																					efetuada porque o denominador é zero*/
		return -1;				//denominador é zero. retorna -1
	}
	else 						//se denominador não é zero
	{
		result = x / y;			//result = x /y
	    fprintf(_H_USART,"O resultado da divisao eh: %d\n",result);	//envia para a UART o resultado da divisão
		return result;			//retorna resultado da divisão
	}

//*******************************************************************
}
