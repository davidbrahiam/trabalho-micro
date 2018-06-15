/**
 * @file eeprom.c
 * @author Embedded Laboratory
 * @date Feb 5, 2017
 * @brief EEPROM Functions Definition
 */

#include <p18f4550.h>
#include <string.h>
#include <EEP.h>
#include <delays.h>

void EEPROM_Write_Block( unsigned char address, unsigned char *data)
{
    unsigned char i = 0;
    while(1) {
        Write_b_eep(address+i, *data);
        Delay10TCYx(1);
        if(*data == 0x23) {
            break;
        }
        data++;  
        i++;
    }
}

unsigned char EEPROM_Read_Block( unsigned char address, unsigned char *data)
{
    unsigned char i = 0;
    while(1) {           
        *(data+i) = Read_b_eep(address+i);
        Delay10TCYx(1);
        if(*(data+i) == 0x23 || *(data+i) == 0xFF) {
            break;
        }
        i++;
    }
    
    return i;
}
