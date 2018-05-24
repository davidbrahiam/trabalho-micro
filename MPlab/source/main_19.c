/*********************************************************************
Nome do arquivo:	main_19.c            
Data:				02 de fevereiro de 2009          
Versao:				1.0                              
MPLAB IDE:			v8.20 
Autor:				Wagner Zanco              
*********************************************************************/
#include <p18f4520.h>					//diretiva de compilação
#include <stdio.h>						//diretiva de compilação
//*******************************************************************
//protótipos de funções
void Recebe_Matriz(char pa[4] [2]);		
//*******************************************************************
void main(void)								//função main					
{	
char solar2[4] [2] = {2,4,3,9,4,16,5,25};	//declaração de matriz 4x2 inicializada	
int x;										//declaração de variável local não inicializada
	x=sizeof(solar2);						////obtém o número de elementos da matriz solar
	Recebe_Matriz(solar2);					//chamada à função com passagem de parâmetro 
	printf("O conteudo da matriz solar eh: %s\n", solar2);				//envia para a UART o conteúdo do vetor alfabeto. 
	printf("A quantidade de elementos da matriz solar eh: %d\n", x);	/*envia para a UART a quantidade de elementos 
																		da matriz solar.*/
	while (1);								//loop infinito
}
//******************************************************************
void Recebe_Matriz(char pa[4] [2])			//funçãoRecebe_Matriz
{
	pa[0][0]='G';
	pa[0][1]='r';
	pa[1][0]='e';
	pa[1][1]='c';
	pa[2][0]='i';
	pa[2][1]='a';
	pa[3][0]='\0';
//******************************************************************
}
