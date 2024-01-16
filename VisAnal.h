#ifndef __VISANAL_H__
#define __VISANAL_H__

#include <Xm/Xm.h>
#include "analisis.h"
#include "picos.h"

// ***************
// ** VARIABLES **
// ***************

// ***************
// ** FUNCIONES **
// ***************

extern Widget result_lista_analisis;
extern Widget Crea_AreaAnalisis (Widget);
extern void iniciaBotones();
extern bool todosIdentificados();

/**********************  analisis  **********************/
extern bool IsActiveAnalysis(); /* indica si los widgets de analisis
					han sido activados */
extern void ReconstruyeListaIntervAnalisis(); /* reconstruye la lista
					de intervalos para el analisis del espectro */
extern void BorraListaIntervAnalisis(); /* borra la lista de intervalos
					de analisis */
extern void ReconstruyeListaAnalisis();
extern void ReponeListaAnalisis();
extern tListaAnalisis* DevuelveListaAnalisis();
	/* Devuelve un puntero a la lista de analisis actual */
extern bool IsInListaAnalisis(int);
	/* Devuelve true si el intervalo indicado se encuentra en la lista
		de analisis */
extern void CopiaListaAnalisis(tListaAnalisis);
	/* Copia la lista pasada en la lista de analisis interna */

//Funcions de tratamiento de ficheros
//extern int  numeroFilas(char *);//Devuelve el número de filas de un fichero
//extern int numeroColumnas(char *);
extern double leerElemento(char *, int, int);
extern double leerNumeroEntreParentesis(char *, int, int);
extern char * leerElementoCadena(char *, int, int);
extern char * leerElementoEntreParentesis(char *, int, int);
extern double leerIncertidumbre(char *, int, int);

//Funciones de tratamiento de cadenas.
extern char *quitarParentesis(char *);
extern char *tomarNombre(char *);
extern int tomarNumero(char *);

extern float **actividades;
extern float **sigmaActividades;
extern bool equilibrioDeterminado;

extern bool padre(int,int,int);

extern bool todosIdentificados();
extern void deleteAllPeaks();
//Despliegue de listas
void determinaPico(Widget, int, int *,int *);

extern void pulsa_edit_result_analisis (Widget, XtPointer, XtPointer);

#endif
