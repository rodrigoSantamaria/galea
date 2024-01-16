#ifndef __STRINGS1_H__
#define __STRINGS1_H__

char *CopyString(const char *string);
/* Hace una copia identica de la cadena pasada. Devuelve NULL si no ha
	conseguido reservar la memoria. */

void toUp(char *string);
/* Covierte la cadena pasada a mayusculas */

char *CopyInUpper(const char *string);
/* Hace una copia en mayusculas de la cadena pasada. Devuelve NULL si no ha
	conseguido reservar la memoria */

#endif
