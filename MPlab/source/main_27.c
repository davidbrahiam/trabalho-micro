	/*********************************************************************
	Nome do arquivo:	main_27.c            
	Data:				21 de fevereiro de 2010          
	Versao:				1.0                              
	MPLAB IDE:			v8.20 
	Autor:				Wagner Zanco              
	*********************************************************************/
	#include <p18f4520.h>		//diretiva de compilação
	#include <stdio.h>			//diretiva de compilação
	//*******************************************************************
	int printf_message(FILE *f, const rom char *format, ...);
	//*******************************************************************
	void main (void)			//função main
	{
	int x=31;																//declaração de variável local
		printf_message(_H_USART,"O conteudo da variavel z eh: %d.\n",x);	//chamada à função
		while(1);															//loop infinito
	}
	//*******************************************************************
	int printf_message(FILE *f, const rom char *format, ...)	//função printf_message
	{
		int x=0;							//declaração de variável local
		va_list ap;							//cria um ponteiro do tipo va_list
		va_start(ap, format);				//inicializa o ponteiro
		x = vfprintf(_H_USART, format, ap);	//chamada à função
		va_end(ap);							//finaliza processo
		return x;							/*retorna um valor negativo em caso 
											de ocorrência de algum erro na execução
											da função, caso contrário retorna o npumero
											de caracteres escritos.*/
	//*******************************************************************
	}