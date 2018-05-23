/*********************************************************************
Nome do arquivo:	main_18.c            
Data:				02 de fevereiro de 2009          
Versao:				1.0                              
MPLAB IDE:			v8.20a 
Autor:				Wagner Zanco              
*********************************************************************/
#include <p18f4520.h>		//diretiva de compilação
#include <stdio.h>			//diretiva de compilação
//*******************************************************************
//protótipos de funções
void Recebe_Vetor(char *);	// portótipo da função Recebe_Vetor()
//*******************************************************************
void main(void)						//função main					
{	
char alfabeto[10]={"Wagner"};		//declaração de vetor inicializado com string

	Recebe_Vetor(alfabeto);			//chamada à função com passagem de parâmetro 	
	while (1);						//loop infinito
}
//*******************************************************************
void Recebe_Vetor(char *p)			//função Recebe_Vetor
{
	printf("O conteudo o vetor alfabeto eh igual a %s\n", p);		//envia para a UART o conteúdo do vetor apontado por p. 
//******************************************************************
}

