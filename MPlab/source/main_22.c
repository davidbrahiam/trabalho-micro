/*********************************************************************
Nome do arquivo:	main_22.c            
Data:				03 de fevereiro de 2010          
Versao:				1.0                              
MPLAB IDE:			v8.20a 
Autor:				Wagner Zanco              
*********************************************************************/
#include <p18f4520.h>		//diretiva de compilação
#include <stdio.h>			//diretiva de compilação
#include <string.h>			//diretiva de compilação
//*******************************************************************
//declaração de estrutura
struct frame
{
	char x;
	char y;	
	char w[3];
};
//*******************************************************************
//protótipo de função
void Verif_soma (struct frame);
//*******************************************************************
 /rotina principal
	void main (void)								//função main
	{
		struct frame soma= {10,20,'A','B','C'};		//declaração de variável de estrutura inicializada 
		Verif_nota (soma);							//chamada a função com passagem de parâmetro
		while(1);									//loop infinito
	}
//***************************************
void Verif_soma (struct frame _soma)				//função Verif_nota
	{
		printf("O conteudo de x eh: %d\n", _soma.x);		//envia para a UART o conteudo de x
		printf("O conteudo de y eh: %d\n", _soma.y);		//envia para a UART o conteudo de y
		printf("O conteudo w[0] eh: %d\n", _soma.w[0]);		//envia para a UART o conteudo de w[0]
		printf("O conteudo de w[1] eh: %d\n", _soma[1]);	//envia para a UART o conteudo de w[1]
		printf("O conteudo de w[2] eh: %d\n", _soma[2]);	//envia para a UART o conteudo de w[2]
//***************************************
	}
