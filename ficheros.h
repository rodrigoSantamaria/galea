#ifndef __FICHEROS_H__
#define __FICHEROS_H__

#include <stdio.h>

#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <string>
using namespace std;


#ifdef __sparc
	#ifndef platform_big_endian 
		#define platform_big_endian
	#endif
#endif

extern size_t   fileWriteBool(bool, FILE *);
extern size_t	 fileReadBool(bool&, FILE *);

	// Arquitectura Big Endian y Arquitectura Little Endian (supuesta)
extern void XCHG_BYTE(void*, size_t);
extern size_t   fileRead(void *, size_t, size_t, FILE *);
extern size_t   fileWrite(const void *, size_t, size_t, FILE *);

extern char *DaNombreFile(char*, char*);
extern char *DaExtensionFile(char*, char*);
extern char *daFichero(char *);
extern char *daFicheroSinExtension(char *);
extern char *daDirectorio(char *);

extern int numeroColumnas(char *);
extern int quitarUltimaColumna(char *);
extern int imprimirFichero(char *nombre);

extern int numeroFilas(char *);

#endif /* end of __FICHEROS_H__ */
