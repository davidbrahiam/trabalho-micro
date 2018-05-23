/*********************************************************************
Nome do arquivo:	main_10.c            
Data:	    		28 de novembro de 2008          
Versao:				1.0                              
MPLAB IDE:			v8.20a 
Autor:				Wagner Zanco              
*********************************************************************/
#include <stdio.h>					//diretiva de compilação
//*******************************************************************
void main(void)						//função main					
{	
int *p;								//declaração de variável de ponteiro	
int contador;						//declaração de variável não inicializada
int x;								//declaração de variável não inicializada

	contador = 100;					//contador recebe o valor 100
	p = &contador;					//p recebe endereço da variável contador
	x = *p;							//x recebe o conteúdo do endereço apontado por p

	printf("O endereco da variavel contador eh = %#x\n",p);		//envia para a USART o endereço de contador
	printf("O conteudo da variavel contador eh = %d\n",x);		//envia para a USART o conteúdo de contador
	while (1);						//loop infinito	
//*******************************************************************
}
