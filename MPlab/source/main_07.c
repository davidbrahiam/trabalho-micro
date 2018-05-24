/*********************************************************************
Nome do arquivo:	main_07.c            
Data:	    		20 de novembro de 2008          
Versao:				1.0                              
MPLAB IDE:			v8.20a 
Autor:				Wagner Zanco              
*********************************************************************/
//delaração de variável global com endereço fixo na memória RAM
#pragma udata my_section=0x100		//cria seção de dados na memória RAM
float f1;							//declaração de varíavel global não inicializada
float f2;							//declaração de varíavel global não inicializada
#pragma udata						//termina seção de dados
//*******************************************************************
void main(void)						//função main					
{	
	f1 = 3.59375;					//atribui o valor 3,59375 à variável f1
	f2 = 465.85;					//atribui o valor 465.85 à variável f2
	while (1);						//loop infinito	
//*******************************************************************
}
