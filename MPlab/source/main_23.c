/*********************************************************************
Nome do arquivo:	main_23.c            
Data:				11 de fevereiro de 2010          
Versao:				1.0                              
MPLAB IDE:			v8.10 
Autor:				Wagner Zanco              
*********************************************************************/
#include <p18f4520.h>			//diretiva de compilação
#include <stdio.h>
#include <string.h>				//diretiva de compilação
//*******************************************************************
//declaração de estruturas
 struct data
{
	int dia;
	int mes;
	int ano;
};							
//***************************
struct pessoa
{
	char nome[10];
	char sobrenome[10];
	struct data nasc;					//estrutura aninhada
};						
 //*******************************************************************
//rotina principal
	void main (void)					//função main
	{
		struct pessoa wagner ={"Wagner","Zanco",{10,02,1969}};	//declaração de variável de estrutura inicializada
		printf("Meu nome eh: %s ",wagner.nome);					//envia para a UART o conteúdo do elemento nome
		printf("%s \n",wagner.sobrenome);						//envia para a UART o conteúdo do elemento sobrenome
		printf("Eu nasci em: %d/",wagner.nasc.dia);				//envia para a UART o conteúdo do elemento nasc.dia
		printf("%d/",wagner.nasc.mes);							//envia para a UART o conteúdo do elemento nasc.mes
		printf("%d\n",wagner.nasc.ano);							//envia para a UART o conteúdo do elemento nasc.ano
		while(1);												//loop infinito
 //*******************************************************************
	}
