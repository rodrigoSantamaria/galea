#ifndef __COLOR_H__
#define __COLOR_H__

#include <Xm/Xm.h>

extern XColor color[];

enum colores {
	Negro, Rojo, Verde, Azul, Blanco, Navy, Naranja, Amarillo, Rosa, Magenta,
	Ciano, Marron, Gris, Lima, Turquesa, Violeta, Wheat, Purpura
};

extern void CargaColores(Display*, Widget);

#endif
