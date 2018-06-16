/*********************************************************************
Nome do arquivo:	users.c            
Data:	    		14 de junho de 2018          
Versao:				1.0                              
MPLAB X IDE:		v4.05
Autor:				Álefe Macedo              
*********************************************************************/
#include "eeprom.h"
#include "users.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

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

int compareArray(unsigned char *a,unsigned char *b)	{
	int i = 0;
    while(1) {        
        if(*(a+i) != *(b+i)) {
            return 0;
        }
        
        if((*(a+i) == 0x23 || *(a+i) == 0x00) || (*(b+i) == 0x00 || *(b+i) == 0x23)) {
            break;
        }
        i++;
    }
	return 1;
}

// Recebe um id e uma senha para salvar um novo usuário 
void saveNewUser(unsigned char *id, unsigned char *password) {
    unsigned char bufferID[50];
    unsigned char bufferPASS[50];
    unsigned char i = 1;
    unsigned char address = eepromEmptyAddress();
    
    *(bufferID+0) = 0x55;
    *(bufferPASS+0) = 0x50;
    
    while (1) {
        if (*id == 0x00 || i > 3) {
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
        if (*password == 0x00 || i > 6) {
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
    unsigned char bufferID[25];
    unsigned char bufferPASS[35];
    unsigned char read[50];                      //buffer para leitura da eeprom
    unsigned char address = 0x00;
    unsigned char i = 1;
    *(bufferID+0) = 0x55;
    *(bufferPASS+0) = 0x50;
    
    while (1) {
        if (*id == 0x00 || i > 3) {
            *(bufferID+i) = 0x23;
            break;
        }
        *(bufferID+i) = *id; 
        id++;
        i++;
    }  
    
    i = 1;
    while (1) {
        if (*password == 0x00 || i > 6) {
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
        // caso a eeprom esteja vazia
        if(address == 0x01) address--;
        
        if(compareArray(read, bufferID)) {
            address += (EEPROM_Read_Block(address, read) + 1);
            
            if(compareArray(read, bufferPASS)) return 1;
            else return 0;
            
            break;
        } else if(address == eepromEmptyAddress()) {
            return 0;
        }
    }
}

// Autentica os dados de um usuário de acordo com os dados salvos na eeprom
void saveRoot() {
    unsigned char read[50];                     //buffer para leitura da eeprom
    unsigned char user[5] = "U999#";
    unsigned char password[8] = "P123456#";
    unsigned char address = 0x00;

    // busca pelo root na eeprom
    while (1) {
        address += (EEPROM_Read_Block(address, read) + 1);
        // caso a eeprom esteja vazia
        if(address == 0x01) address--;
        
        if(compareArray(read, user)) {
            break;
        } else if(address == eepromEmptyAddress()) {
            EEPROM_Write_Block(eepromEmptyAddress(), user);
            EEPROM_Write_Block(eepromEmptyAddress(), password);
            break;
        }
    }
}

int updateRoot(unsigned char *newPassword) {
    unsigned char read[50];                     //buffer para leitura da eeprom
    unsigned char user[5] = "U999#";
    unsigned char bufferPASS[50];
    unsigned char address = 0x00;
    unsigned char i = 1;
    *(bufferPASS+0) = 0x50;
    
    while (1) {
        if (*newPassword == 0x00 || i > 6) {
            *(bufferPASS+i) = 0x23;
            break;
        }
        *(bufferPASS+i) = *newPassword; 
        newPassword++;
        i++;
    }
    
    // busca pelo root na eeprom
    while (1) {
        address += (EEPROM_Read_Block(address, read) + 1);
        // caso a eeprom esteja vazia
        if(address == 0x01) address--;
        
        if(compareArray(read, user)) {
            EEPROM_Write_Block(address, bufferPASS);
            EEPROM_Read_Block(address, read);
            
            if(compareArray(read, bufferPASS)) return 1;
            else return 0;            
            break;
        
        } else if(address == eepromEmptyAddress()) {
            return 0;
        }
    }
}
/*******************************************************************/