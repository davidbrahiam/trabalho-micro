/*********************************************************************
Nome do arquivo:	main_21.c            
Data:				03 de fevereiro de 2010          
Versao:				1.0                              
MPLAB IDE:			v8.10 
Autor:				Wagner Zanco              
*********************************************************************/
#include <p18f4520.h>	//diretiva de compilação
#include <stdio.h>		//diretiva de compilação
#include <string.h>		//diretiva de compilação
//*******************************************************************
//variáveis globais
char _nome[]= "jose da Silva";
char _estado_civil[]= "solteiro";
char _estado[]= "SP";
char _cidade[]= "Sao Miguel";
char _rua[]= "Lipado";
char i=0;
//*******************************************************************
//definição de estrutura
struct  Dados_Individuo
{
	char nome[30];
	unsigned char idade;
	char estado_civil [10];
	char estado [3];
	char cidade [30];
	char rua [30];
	unsigned int numero;
	long int CEP;
};
 //*******************************************************************
//rotina principal
	void main (void)
	{
	struct Dados_Individuo Jose;					//declaração de variável de estrutura 
//inicialização dos elementos da variável de estrutura Jose
		strcpy(Jose.nome,_nome);
		Jose.idade=29;
		strcpy(Jose.estado_civil,_estado_civil);
		strcpy(Jose.estado,_estado);
		strcpy(Jose.cidade,_cidade);
		strcpy(Jose.rua,_rua);
		Jose.numero=455;
		Jose.CEP=20180234;
//***************************************
//envia para UART os elementos da variável de estrutura Jose
		printf("\n%s\n",Jose.nome);
		printf("%d anos\n",Jose.idade);
		printf("%s\n",Jose.estado_civil);
		printf("%s\n",Jose.estado);
		printf("%s\n",Jose.cidade);
		printf("%s\n",Jose.rua);
		printf("%d\n",Jose.numero);
		printf("%ld\n",Jose.CEP);
//***************************************
		while(1);							//loop infinito
	}
	