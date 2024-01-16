#ifndef __VISCONTI_H__
#define __VISCONTI_H__

#include <Xm/Xm.h>
#include "continuo.h"

extern int Continuo;

// ***************
// ** FUNCIONES **
// ***************

extern Widget Crea_AreaContinuo (Widget);

	extern void PonIntervaloContinuo(int, int);
	/* pone en los cuadros de texto de canal izquierdo y derecho del dialog
		de parametros de busqueda del continuo los datos pasados */
	extern tListaContinuo* DevuelveListaContinuo();
	/* Devuelve un puntero a la lista de parametros de busqueda interna */
	extern void ReconstruyeListaContinuo();
	/* partiendo de la lista de parametros de busqueda interna reconstruye la
		lista de parametros del dialog de busqueda del continuo */
	extern void CopiaListaContinuo(tListaContinuo);
	/* Copia la lista pasada en la lista de parametros de busqueda interna */

	extern void EstadoBotonesContinuo();
	/* actualiza la representacion de los botones de busqueda y ajuste del
		continuo */
	extern void ActualizaVectorContinuo(double);
	/* actualiza el vector de datos del spline del continuo para el ajuste */
	
	extern void seleccionarTexto(Widget, XtPointer, XtPointer);

#endif
