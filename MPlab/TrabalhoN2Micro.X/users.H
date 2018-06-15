/* 
 * File:   users.H
 * Author: Álefe Costa Macedo
 *
 * Created on 14 de Junho de 2018, 18:05
 */

#ifndef USERS_H
#define	USERS_H
//*************************************************************
// protótipos de funções
unsigned char eepromEmptyAddress(void);
int authenticateUser(unsigned char *id, unsigned char *password);
void saveNewUser(unsigned char *id, unsigned char *password);
void saveRoot(void);
int updateRoot(unsigned char *newPassword);
#endif
