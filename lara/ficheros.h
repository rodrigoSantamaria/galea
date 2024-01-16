#include <stdio.h>

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
