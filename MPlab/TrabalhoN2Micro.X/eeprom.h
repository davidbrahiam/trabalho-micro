/**
 * @file eeprom.c
 * @author Embedded Laboratory
 * @date Feb 5, 2017
 * @brief EEPROM Functions Definition
 */

#include <p18f4550.h>
#include <string.h>
#include <EEP.h>

void EEPROM_Write_Block( unsigned char address, unsigned char *data, unsigned char length)
{
    unsigned char i = 0;
    for(i=0; i<length; i++)
    {
      Write_b_eep(address+i, *(data+i));
      Delay10TCYx(1);
    }
}

void EEPROM_Read_Block( unsigned char address, unsigned char *data, unsigned char length )
{
    unsigned char i = 0;
    for( i=0; i< length; i++ )
    {
      *(data+i) = Read_b_eep(address+i);
      Delay10TCYx(1);
    }
}
