	/*********************************************************************
	Nome do arquivo:	main_24.c            
	Data:				14 de fevereiro de 2010          
	Versao:				1.0                              
	MPLAB IDE:			v8.20a 
	Autor:				Wagner Zanco              
	*********************************************************************/
	#include <p18f452.h>	//diretiva de compilação
	#include <stdio.h>		//diretiva de compilação
	//*******************************************************************
	//definições
	#define max 50			//diretiva decompilaçao
	//*******************************************************************
	//variáveis globais
	char buffer[max]="";
	//*******************************************************************
	void main (void)
	{
	stdout = _H_USER;					//ativa stream definida pelo usuário como saída padrão
	printf("Quando penso em voce.\n");	//envia string para stdout
	while(1);							//loop infinito
	}
	//*******************************************************************
	int _user_putc (char c)
	{
	char i;							//declaração de variável local incializada	
	static char x=0;				//declaração de variável local estática incializada
		if(c!=0 && c!=0x0A)			//executa bloco de código se c!=0 e c!=0Ah
		{
			buffer[x]=c;			//carrega caractere no índice do buffer apontado por x
			x++;					//incrementa a variável x 
		}
		else 	if(c==0x0A)			//se não e se c==0Ah executa bloco de código
				{
					buffer[x]=c;	//carrega o caractere recebido no índice apontado por x
					x++;			//incrementa x
					buffer[x]='\0';	//carrega caractere null no indice apontado por x
					x=0;			//x=0
				}
	//*******************************************************************
	}
