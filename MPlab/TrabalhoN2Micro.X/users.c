/*********************************************************************
Nome do arquivo:	users.c            
Data:	    		14 de junho de 2018          
Versao:				1.0                              
MPLAB X IDE:		v4.05
Autor:				Álefe Macedo              
*********************************************************************/
#include "eeprom.h"
#include <string.h>
#include <stdio.h>

// Funções
/*******************************************************************/
// Busca o próximo endereço vazio na eeprom
unsigned char eepromEmptyAddress() {
    unsigned char i = 0;
    for( i=0; i< 256; i++ )
    {
      if(Read_b_eep(i) == 0xFF){
            return i; 
            break;
      }
      Delay10TCYx(1);
    }
}

// Recebe um id e uma senha para salvar um novo usuário 
void saveNewUser(unsigned char *id, unsigned char *password) {
    unsigned char *bufferID = "U";
    unsigned char *bufferPASS = "P";
    
    strcat(bufferID, id);
    strcat(bufferPASS, password);
    
    //escreve na eeprom o id do novo usuário
    EEPROM_Write_Block(
            0x00,
            bufferID, 
            sizeof(bufferID)
    ); 
    
    //escreve na eeprom a senha do novo usuário
//    EEPROM_Write_Block(
//            eepromEmptyAddress(),
//            bufferPASS,
//            sizeof(bufferPASS)
//    );
}

// Autentica os dados de um usuário de acordo com os dados salvos na eeprom
int authenticateUser(unsigned char *id, unsigned char *password) {
    unsigned char *read;                     //buffer para leitura da eeprom
    unsigned char bufferID[8] = "U";
    unsigned char bufferPASS[8] = "P";
    unsigned char address = 0x00;
    
    strcat(bufferID, id);
    strcat(bufferPASS, password);    
    
    //le da eeprom
    while (1) {
        EEPROM_Read_Block(address, read, 8);
        if(memcmp(read, bufferID, 1) == 0) {
            EEPROM_Read_Block(address+8, read, 8);
            
            if(memcmp(read, bufferPASS, 1) == 0) return 1;
            else return 0;
            
            break;
        }
        address +=8;
    }
}
/*******************************************************************/