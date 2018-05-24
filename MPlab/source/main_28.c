	/*********************************************************************
	Nome do arquivo:	main_28.c            
	Data:				21 de fevereiro de 2010          
	Versao:				1.0                              
	MPLAB IDE:			v8.20 
	Autor:				Wagner Zanco              
	*********************************************************************/
	#include <p18f4520.h>		//diretiva de compilação
	#include <stdio.h>			//diretiva de compilação
	//*******************************************************************
	//definições
	#define		AJUSTA_DECIMAL	100	//precisão de duas casas decimais
	/*
	1 	- nenhuma parte decimal
	10	- uma casa decimal
	100 - duas casas decimais
	etc...
	//*******************************************************************/
	//protótipos de funções
	int printf_message(FILE *f, const rom char *format, ...);
	char *Conv_Float_String(float float_in);
	//*******************************************************************
	//variáveis globais
	char buf[50];							//declaração de vetor
	//*******************************************************************
	void main (void)
	{
	float x = 6553.35;						//declaração de variável local
	char *p;								//declaração de variável local
		p=Conv_Float_String(x);				//chamada à função com retorno de valor
		printf_message(_H_USART,"O conteudo da variavel z eh: %s.\n",p);	//chamada à função
		while(1);							//loop infinito
	}
	//*******************************************************************
	int printf_message(FILE *f, const rom char *format, ...)
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
	/********************************************************************
	//Conversão de float para ASCII. Esta função converte valores float 
	na faixa de -65535.998 a +65535.998 em uma string. 
	//*******************************************************************/
	char *Conv_Float_String(float float_in)
	{
	unsigned int parte_inteira;				//declaração de variável local
	unsigned int parte_decimal;				//declaração de variável local
	char sinal, *pt;						//declaração de variável local
	unsigned char x, y;						//declaração de variável local
	//*******************************************************************
		if(float_in < 0)					//valor é menor que 0?
		{
			sinal = '-';					//sim, sinal negativo
			float_in = float_in * (-1);		//inverte o sinal de float_in
		}
		else sinal = ' ';					//não, sinal positivo
	//*******************************************************************
		parte_inteira = float_in;			//resgata parte interia dovalor
	//*******************************************************************
		parte_decimal =(unsigned int)((float_in - parte_inteira) * AJUSTA_DECIMAL);	//resgata parte fracionária do valor
		sprintf(buf,"%c%u.%02u",sinal,parte_inteira,parte_decimal);					//converte valor em string e armazena no vetor buf
		pt = buf;							//passa para o ponteiro pt o endereço de buf
		return (pt);						//retorna o endereço de buf
	//*******************************************************************
	}
