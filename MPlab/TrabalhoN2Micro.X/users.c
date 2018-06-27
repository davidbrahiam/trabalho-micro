/*********************************************************************
Nome do arquivo:	users.c            
Data:	    		14 de junho de 2018          
Versao:				1.0                              
MPLAB X IDE:		v4.05
Autor:				�lefe Macedo              
*********************************************************************/
#include "eeprom.h"
#include "users.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "criptografia.h"

static unsigned long *passwordL;
static unsigned long keyL[4] = {
    0x00112233,
    0x44556677,
    0x8899aabb,
    0xccddeeff
};
static unsigned char buffer[sizeof(unsigned long)*8];

// Fun��es
/*******************************************************************/
// Busca o pr�ximo endere�o vazio na eeprom
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

void concatID(unsigned char *id, unsigned char *bufferID)
{
    unsigned char initID[1] = "U";
    unsigned char endID[1] = "#";
    
    memcpy(bufferID, initID, 1);
    memcpy(bufferID+1, id, 3);
    memcpy(bufferID+strlen(bufferID), endID, 1);
    
}

void concatPASS(unsigned char *password, unsigned char *bufferPASS)
{
    unsigned char initPASS[1] = "P";
    unsigned char endPASS[1] = "#";
        
    memcpy(bufferPASS, initPASS, 1);
    memcpy(bufferPASS+1, password, strlen(password));
    memcpy(bufferPASS+strlen(bufferPASS), endPASS, 1);
    
}

void extractData(unsigned char *buffer) {
    int i = 1;
    while(1) { 
        if(*(buffer+i) == 0x23) {
            *(buffer+(i-1)) = 0;
            *(buffer+i) = 0;
            break;
        } else {
            *(buffer+(i-1)) = *(buffer+i);  
        }
        
        i++;
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

// Recebe um id e uma senha para salvar um novo usu�rio 
void saveNewUser(static unsigned char *id, static unsigned char *password) {
    unsigned char bufferID[50]; 
    unsigned char bufferPASS[50];
    unsigned char address = eepromEmptyAddress();
    memset(bufferID, 0, 50);    
    memset(bufferPASS, 0, 50);
    memset(buffer, 0, sizeof(unsigned long)*8);
    
//    strncpy(buffer, password, 6);
    *passwordL = atoul(password);
//    memset(buffer, 0, sizeof(unsigned long)*8);
    
    encrypt(passwordL, keyL);
    ultoa(*passwordL, buffer);
    
    concatID(id, bufferID);    
    concatPASS(buffer, bufferPASS);

    //escreve na eeprom o id do novo usu�rio
    EEPROM_Write_Block(
            address,
            bufferID
    );
    
    address = address + strlen(bufferID);
    //escreve na eeprom a senha do novo usu�rio
    EEPROM_Write_Block(
            address,
            bufferPASS
    );
}

// Autentica os dados de um usu�rio de acordo com os dados salvos na eeprom
int authenticateUser(static unsigned char *id, static unsigned char *password) {
    unsigned char bufferID[25];
    unsigned char bufferPASS[35];
    unsigned char read[50];                      //buffer para leitura da eeprom
    unsigned char address = 0x00;
    
    memset(bufferID, 0, 25);    
    memset(bufferPASS, 0, 35);
    memset(read, 0, 50);
    memset(buffer, 0, sizeof(unsigned long)*8);
    
    strncpy(buffer, password, 6);
    
    *passwordL = atoul(buffer);
    encrypt(passwordL, keyL);
    ultoa(*passwordL, buffer);
    
    concatID(id, bufferID);    
//    concatPASS(password, bufferPASS);
  
    
    //le da eeprom
    while (1) {
        address += (EEPROM_Read_Block(address, read) + 1);
        // caso a eeprom esteja vazia
        if(address == 0x01) address--;
        
        if(compareArray(read, bufferID)) {
            address += (EEPROM_Read_Block(address, read) + 1);
            extractData(read);           
            
            if(compareArray(buffer, read)) return 1;
            else return 0;
            
            break;
        } else if(address == eepromEmptyAddress()) {
            return 0;
        }
    }
}

// Autentica os dados de um usu�rio de acordo com os dados salvos na eeprom
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