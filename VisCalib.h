#ifndef __VISCALIB_H__
#define __VISCALIB_H__

#include <Xm/Xm.h>
#include "calibracion.h"

// ***************
// ** VARIABLES **
// ***************


// ***************
// ** FUNCIONES **
// ***************

extern Widget Crea_AreaCalibracion (Widget);

/**********************  precalibracion  **********************/
	extern void ActivaPreCalibracion();
	/* Pone activo el boton de precalibracion */
	extern void DesactivaPreCalibracion(); 
	/* desactiva el boton de precalibracion */
	void CierraDialogPrecalibracion();
	/* cierra el dialog de precalibracion si se encuentra abierto */
	extern void ReconstruyeListaPrecalibracion();
	/* partiendo de la lista de precalibracion interna reconstruye la lista
		de precalibracion del dialog de precalibracion */
	extern tListaPrecalib* DevuelveListaPreCalibracion();
	/* Devuelve un puntero a la lista de precalibracion interna */
	extern void CopiaListaPreCalibracion(tListaPrecalib);
	/* Copia la lista pasada en la lista de precalibracion interna */
	extern void PonIntervaloPreCalibracion (int, int);
	/* pone en los cuadros de texto de canal izquierdo y derecho del dialog
		de precalibracion los datos pasados */

/**********************  calibracion  **********************/
extern bool IsActiveCalibration(); /* indica si los widgets de calibracion
					han sido activados */
extern void ActivaCalibracion();
extern void DesactivaCalibracion();
extern void ReconstruyeListaCalibracion();
extern void ReconstruyeListaIntervCalibracion(); /* reconstruye la lista
					de intervalos que solo contienen un pico, propicios para
					utilizar en la calibracion del espectro */
extern void BorraListaIntervCalibracion(); /* borra la lista de intervalos
					que solo contienen un pico para la calibracion */
	extern tListaCalibracion* DevuelveListaCalibracion();
	/* Devuelve un puntero a la lista de calibracion actual */
	extern bool IsInListaCalibracion(int);
	/* Devuelve true si el intervalo indicado se encuentra en la lista
		de calibracion */
	extern void CopiaListaCalibracion(tListaCalibracion);
	/* Copia la lista pasada en la lista de calibracion interna */

/**********************  ajustes  **********************/
	extern void ActualizaVectorEnergiaCalib(double);
	/* actualiza el vector de la cuadratica para el ajuste de la energia */
	extern void ActualizaVectorRectaCalib(double);
	/* actualiza el vector de la recta para el ajuste de las anchuras */

#endif
