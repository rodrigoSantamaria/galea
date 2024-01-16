#ifndef __AVISOS_H__
#define __AVISOS_H__

#include <Xm/Xm.h>

// funciones 
	extern void Aviso(Widget, char *);
	/* Pone una ventana de aviso con el texto pasado como modal de aplicacion
		y un boton de OK. Es labor del programa liberar la memoria de la cadena. */
	extern int Continuar(Widget, char *, char *);
	/* Pone una ventana que pregunta si se quiere continuar o no en modo modal
		impidiendo la continuacion del programa con el texto pasado. Es labor
		del programa liberar la memoria de la cadena. */
	extern int Elegir(Widget, char *, char *, char *, char *, char *);
	/* Pone una ventana que permite elegir hasta 3 opciones. Las opciones
		marcadas como NULL no apareceran. */
	
	extern Widget Cartel(Widget, char *);

#endif
