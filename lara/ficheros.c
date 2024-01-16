#include <stdio.h>
#include "ficheros.h"

size_t fileWriteBool(bool valor, FILE *stream) {
	char flag = (valor) ? -1 : 0;
	size_t temp = fwrite(&flag, sizeof(flag), 1, stream);
	return temp;
}

size_t fileReadBool(bool& valor, FILE *stream) {
	char flag;
	size_t temp = fread(&flag, sizeof(flag), 1, stream);
	valor = (flag != 0) ? true : false;
	return temp;
}

#ifdef platform_big_endian
	// Arquitectura Big Endian

void XCHG_BYTE(void *ptr, size_t size) {
	unsigned char *principio = (unsigned char*) ptr;
	unsigned char *final = (unsigned char*) ptr + size - 1;
   for (int i = 1; i <= size/2; i++) {
	   unsigned char swap = *principio;
		*(principio++) = *final;
		*(final--) = swap;
	}
}

size_t fileRead(void *ptr, size_t size, size_t nmemb, FILE *stream) {
	// emula la lectura de un fichero en notacion little endian
	// para ello lee cada byte y lo coloca en la celda de memoria
	// correspondiente:
	// ***** ATENCION **** sin deteccion de errores
	unsigned char *puntero = (unsigned char*) ptr;
	for (int i = 1; i <= nmemb; i++) {
		// lee cada bloque
   	fread(puntero, size, 1, stream);
		// y recoloca los bytes (todos los tamagnos disponibles en size
		// seran siempre una potencia de 2)
		if (size > 1) XCHG_BYTE(puntero, size);
		puntero += size;
	}
	return nmemb;
}


size_t fileWrite(const void *ptr, size_t size, size_t nmemb, FILE *stream) {
	// emula la escritura de un fichero en notacion little endian
	// para ello escribe cada byte en la posicion correspondiente:
	// ***** ATENCION **** sin deteccion de errores
	unsigned char *puntero = (unsigned char*) ptr;
	for (int i = 1; i <= nmemb; i++) {
		// escribe cada byte de cada bloque
		unsigned char *final = puntero + size - 1;
		while (final >= puntero)
			fwrite((final--), 1, 1, stream);
		puntero += size;
	}
	return nmemb;
}

#else
	// Arquitectura Little Endian (supuesta)

void XCHG_BYTE(void *ptr, size_t size) {
}

size_t fileRead(void *ptr, size_t size, size_t nmemb, FILE *stream) {
   return fread(ptr, size, nmemb, stream);
}

size_t fileWrite(const void *ptr, size_t size, size_t nmemb, FILE *stream) {
   return fwrite(ptr, size, nmemb, stream);
}
#endif
