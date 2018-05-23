/*********************************************************************
Nome do arquivo:	main_17.c            
Data:				02 de fevereiro de 2009          
Versao:				1.0                              
MPLAB IDE:			v8.20a 
Autor:				Wagner Zanco              
*********************************************************************/
#include <p18f4520.h>	//diretiva de compilação
#include <stdio.h>		//diretiva de compilação
//*******************************************************************
//protótipos de funções
//*******************************************************************
void main(void)					//função main					
{	
char alfabeto[30]={"Wagner"};	//declaração de vetor inicializado
unsigned char i=0;				//declaração de variável local inicializada
unsigned char x=0x61;			//declaração de variável local inicializada

	for(i=0;i<=25;i++)			//laço de iteração
	{
	alfabeto [i]=x++;			//carrega x no índice apontado por i e em seguida incrementa x 
	}
	printf("%s\n", alfabeto);	//envia para a UART todo o alfabeto
	while (1);					//loop infinito
//*******************************************************************
}
