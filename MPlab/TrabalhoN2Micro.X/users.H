/* 
 * File:   users.H
 * Author: �lefe Costa Macedo
 *
 * Created on 14 de Junho de 2018, 18:05
 */

#ifndef USERS_H
#define	USERS_H
//*************************************************************
// prot�tipos de fun��es
unsigned char eepromEmptyAddress(void);
int authenticateUser(static unsigned char *id, static unsigned char *password);
void saveNewUser(static unsigned char *id, static unsigned char *password);
void saveRoot(void);
int updateRoot(unsigned char *newPassword);
#endif
