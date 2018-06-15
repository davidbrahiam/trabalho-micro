/*********************************************************************
Nome do arquivo:	users.c            
Data:	    		14 de junho de 2018          
Versao:				1.0                              
MPLAB X IDE:		v4.05
Autor:				Álefe Macedo              
*********************************************************************/
#include "eeprom.h"
#include "users.H"
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
    unsigned char *bufferID;
    unsigned char *bufferPASS;
    unsigned char i = 1;
    unsigned char address = eepromEmptyAddress();
    
    *(bufferID+0) = 0x55;
    *(bufferPASS+0) = 0x50;
    
    while (1) {
        if (*id == 0x00) {
            *(bufferID+i) = 0x23;
            break;
        }
        *(bufferID+i) = *id; 
        id++;
        i++;
    }
    
    //escreve na eeprom o id do novo usuário
    EEPROM_Write_Block(
            address,
            bufferID
    );
    
    address = address + (i+1);
    
    i = 1;
    while (1) {
        if (*password == 0x00) {
            *(bufferPASS+i) = 0x23;
            break;
        }
        *(bufferPASS+i) = *password; 
        password++;
        i++;
    }     
    
    //escreve na eeprom a senha do novo usuário
    EEPROM_Write_Block(
            address,
            bufferPASS
    );
}

// Autentica os dados de um usuário de acordo com os dados salvos na eeprom
int authenticateUser(unsigned char *id, unsigned char *password) {
    unsigned char *read;                     //buffer para leitura da eeprom
    unsigned char *bufferID;
    unsigned char *bufferPASS;
    unsigned char address = 0x00;
    unsigned char i = 1;
    *(bufferID+0) = 0x55;
    *(bufferPASS+0) = 0x50;
    
    while (1) {
        if (*id == 0x00) {
            *(bufferID+i) = 0x23;
            break;
        }
        *(bufferID+i) = *id; 
        id++;
        i++;
    }  
    
    i = 1;
    while (1) {
        if (*password == 0x00) {
            *(bufferPASS+i) = 0x23;
            break;
        }
        *(bufferPASS+i) = *password; 
        password++;
        i++;
    }
    
    //le da eeprom
    while (1) {
        address += (EEPROM_Read_Block(address, read) + 1);
        if(memcmp(read, bufferID, 1) == 0) {
            address += (EEPROM_Read_Block(address, read) + 1);
            
            if(memcmp(read, bufferPASS, 1) == 0) return 1;
            else return 0;
            
            break;
        }
    }
}
/*******************************************************************/