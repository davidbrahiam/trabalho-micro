/*********************************************************************
Nome do arquivo:	Keyboard_HEX.c            
Data:	    		14 de mar�o de 2010          
Versao:				1.0                              
MPLAB IDE:			v8.20a 
Autor:				Wagner Zanco              
*********************************************************************/
#include <p18cxxx.h>					//diretiva de compila��o
#include "Keyboard_HEX.h"				//diretiva de compila��o
//*************************************************************
unsigned char GetKey(void)
{
	unsigned char key = '\0';			//declara��o de vari�vel local
	static unsigned char x = 0 ;		//declara��o de vari�vel local static
//configura colunas como sa�da e linhas como entrada
	x1_DIR = Out;
	x2_DIR = Out;
	x3_DIR = Out;
	x4_DIR = Out;
	y1_DIR = In;
	y2_DIR = In;
	y3_DIR = In;
	y4_DIR = In;
//	EnablePullups();					//habilita resistores de pull-up
//*************************************************************
// rastrear coluna 1
	x1 = 0;								//ativa coluna 1
	x2 = 1;								//desativa coluna 2
	x3 = 1;								//desativa coluna 3
	x4 = 1;								//desativa coluna 4
	Nop();
	if(!y1) 							//linha y1 = 0?
		if(x==1)return 'G';				//sim, retorna o caractere 'G' se tecla foi tradada e ainda est� pressionada 
		else							//tecla S1 pressionada
		{
			key = '1';					//Key = '1'
			TRISD = 0;					//configura Port B como sa�da e desabilita os resistores de pull-up
			x = 1;						//flag indica que uma tecla est� pressionada
			return(key);				//retorna valor da tecla pressionada em ASCII
		}
	if(!y2) 							//linha y2 = 0?
		if(x==1)return 'G';				//sim, retorna o caractere 'G' se tecla foi tradada e ainda est� pressionada
		else							//tecla S4 pressionada
		{

			key = '4';					//Key = '4'
			TRISD = 0;					//configura Port B como sa�da e desabilita os resistores de pull-up
			x = 1;						//flag indica que uma tecla est� pressionada
			return(key);				//retorna valor da tecla pressionada em ASCII
		}
	if(!y3) 							//linha y3 = 0?
		if(x==1)return 'G';				//sim, retorna o caractere 'G' se tecla foi tradada e ainda est� pressionada
		else							//tecla S7 pressionada
		{
			key = '7';					//Key = '7'
			TRISD = 0;					//configura Port B como sa�da e desabilita os resistores de pull-up
			x = 1;						//flag indica que uma tecla est� pressionada
			return(key);				//retorna valor da tecla pressionada em ASCII
		}
	if(!y4) 							//linha y4 = 0?
		if(x==1)return 'G';				//sim, retorna o caractere 'G' se tecla foi tradada e ainda est� pressionada
		else							//tecla SF pressionada
		{
			key = 'F';					//Key = 'F'
			TRISD = 0;					//configura Port B como sa�da e desabilita os resistores de pull-up
			x = 1;						//flag indica que uma tecla est� pressionada
			return(key);				//retorna valor da tecla pressionada em ASCII
		}
//*************************************************************
// rastrear coluna 2
	x1 = 1;								//desativa coluna 1
	x2 = 0;								//ativa coluna 2
	x3 = 1;								//desativa coluna 3
	x4 = 1;								//desativa coluna 4
	Nop();
	if(!y1) 							//linha y1 = 0?
		if(x==1)return 'G';				//sim, retorna o caractere 'G' se tecla foi tradada e ainda est� pressionada 
		else							//tecla S2 pressionada
		{
			key = '2';					//Key = '2'
			TRISD = 0;					//configura Port B como sa�da e desabilita os resistores de pull-up
			x = 1;						//flag indica que uma tecla est� pressionada
			return(key);				//retorna valor da tecla pressionada em ASCII
		}
	if(!y2) 							//linha y2 = 0?
		if(x==1)return 'G';				//sim, retorna o caractere 'G' se tecla foi tradada e ainda est� pressionada 
		else							//tecla S5 pressionada
		{
			key = '5';					//Key = '5'
			TRISD = 0;					//configura Port B como sa�da e desabilita os resistores de pull-up
			x = 1;						//flag indica que uma tecla est� pressionada
			return(key);				//retorna valor da tecla pressionada em ASCII
		}
	if(!y3) 							//linha y3 = 0?
		if(x==1)return 'G';				//sim, retorna o caractere 'G' se tecla foi tradada e ainda est� pressionada 
		else							//tecla S8 pressionada
		{
			key = '8';					//Key = '8'
			TRISD = 0;					//configura Port B como sa�da e desabilita os resistores de pull-up
			x = 1;						//flag indica que uma tecla est� pressionada
			return(key);				//retorna valor da tecla pressionada em ASCII
		}
	if(!y4) 							//linha y4 = 0?
		if(x==1)return 'G';				//sim, retorna o caractere 'G' se tecla foi tradada e ainda est� pressionada 
		else							//tecla S0 pressionada
		{
			key = '0';					//Key = '0'
			TRISD = 0;					//configura Port B como sa�da e desabilita os resistores de pull-up
			x = 1;						//flag indica que uma tecla est� pressionada
			return(key);				//retorna valor da tecla pressionada em ASCII
		}
//*************************************************************
// rastrear coluna 3
	x1 = 1;								//desativa coluna 1
	x2 = 1;								//desativa coluna 2
	x3 = 0;								//ativa coluna 3
	x4 = 1;								//desativa coluna 4
	Nop();
	if(!y1) 							//linha y1 = 0?
		if(x==1)return 'G';				//sim, retorna o caractere 'G' se tecla foi tradada e ainda est� pressionada 
		else							//tecla S3 pressionada
		{
			key = '3';					//Key = '3'
			TRISD = 0;					//configura Port B como sa�da e desabilita os resistores de pull-up
			x = 1;						//flag indica que uma tecla est� pressionada
			return(key);				//retorna valor da tecla pressionada em ASCII
		}
	if(!y2) 							//linha y2 = 0?
		if(x==1)return 'G';				//sim, retorna o caractere 'G' se tecla foi tradada e ainda est� pressionada 
		else							//tecla S6 pressionada
		{
			key = '6';					//Key = '6'
			TRISD = 0;					//configura Port B como sa�da e desabilita os resistores de pull-up
			x = 1;						//flag indica que uma tecla est� pressionada
			return(key);				//retorna valor da tecla pressionada em ASCII
		}
	if(!y3) 							//linha y3 = 0?
		if(x==1)return 'G';				//sim, retorna o caractere 'G' se tecla foi tradada e ainda est� pressionada 
		else							//tecla S9 pressionada
		{
			key = '9';					//Key = '9'
			TRISD = 0;					//configura Port B como sa�da e desabilita os resistores de pull-up
			x = 1;						//flag indica que uma tecla est� pressionada
			return(key);				//retorna valor da tecla pressionada em ASCII
		}
	if(!y4) 							//linha y4 = 0? 	
		if(x==1)return 'G';				//sim, retorna o caractere 'G' se tecla foi tradada e ainda est� pressionada 
		else							//tecla SE pressionada
		{
			key = 'E';					//Key = 'E'
			TRISD = 0;					//configura Port B como sa�da e desabilita os resistores de pull-up
			x = 1;						//flag indica que uma tecla est� pressionada
			return(key);				//retorna valor da tecla pressionada em ASCII
					}
//*************************************************************
// rastrear coluna 4
	x1 = 1;								//desativa coluna 1
	x2 = 1;								//desativa coluna 2
	x3 = 1;								//desativa coluna 3
	x4 = 0;								//ativa coluna 4
	Nop();
	if(!y1) 							//linha y1 = 0?
		if(x==1)return 'G';				//sim, retorna o caractere 'G' se tecla foi tradada e ainda est� pressionada 
		else							//tecla SA pressionada
		{
			key = 'A';					//Key = 'A'
			TRISD = 0;					//configura Port B como sa�da e desabilita os resistores de pull-up
			x = 1;						//flag indica que uma tecla est� pressionada
			return(key);				//retorna valor da tecla pressionada em ASCII
		}
	if(!y2) 							//linha y2 = 0?
		if(x==1)return 'G';				//sim, retorna o caractere 'G' se tecla foi tradada e ainda est� pressionada	
		else							//tecla SB pressionada
		{
			key = 'B';					//Key = 'B'
			TRISD = 0;					//configura Port B como sa�da e desabilita os resistores de pull-up
			x = 1;						//flag indica que uma tecla est� pressionada
			return(key);				//retorna valor da tecla pressionada em ASCII
		}
	if(!y3) 							//linha y3 = 0?
		if(x==1)return 'G';				//sim, retorna o caractere 'G' se tecla foi tradada e ainda est� pressionada
		else							//tecla SC pressionada
		{
			key = 'C';					//Key = 'C'
			TRISD = 0;					//configura Port B como sa�da e desabilita os resistores de pull-up
			x = 1;						//flag indica que uma tecla est� pressionada
			return(key);				//retorna valor da tecla pressionada em ASCII
		}
	if(!y4) 							//linha y4 = 0?
		if(x==1)return 'G';				//sim, retorna o caractere 'G' se tecla foi tradada e ainda est� pressionada
		else							//tecla SD pressionada
		{
			key = 'D';					//Key = 'D'
			TRISD = 0;					//configura Port B como sa�da e desabilita os resistores de pull-up
			x = 1;						//flag indica que uma tecla est� pressionada
			return(key);				//retorna valor da tecla pressionada em ASCII
		}
//*************************************************************
//nenhuma tecla pressionada
	TRISD = 0x00;						//configura Port B como sa�da e desabilita os resistores de pull-up
	x = 0;								//flag indica que nenhuma tecla est� pressionada
	key = '\0';							//Key = 0x00
	return(key);						//retorna 
//*************************************************************
}
