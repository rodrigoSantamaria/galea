#ifndef __CALIBRACION_H__
#define __CALIBRACION_H__

/* ****************************
	**  TIPOS PRECALIBRACION  **
	**************************** */
	
//Estos tipos de nodo contienen información sobre los picos: su E (energía) y su W en función de sus dos límites
typedef struct tPrecalibracion { // conjunto de datos para hacer precalibracion
   double energia; // energia asociada al pico
   int limInferior, limSuperior; // limites en los que se encuentra el pico
} tPrecalibracion;

typedef struct NodoPrecalibracion {
   double energia; // energia asociada al pico
   int limInferior, limSuperior; // limites en los que se encuentra el pico
	NodoPrecalibracion *siguiente;
} NodoPrecalibracion;

typedef struct tListaPrecalib {
	NodoPrecalibracion *inicio;
	int numElementos;
} tListaPrecalib;

/* *************************
	**  TIPOS CALIBRACION  **
	************************* */

typedef struct NodoCalibracion {
	int numIntervalo; // numero de intervalo de picos que sirve de pico de
							// calibracion 
							//NOTA: ?
	NodoCalibracion *siguiente;
} NodoCalibracion;

typedef struct tListaCalibracion {
	NodoCalibracion *inicio;
	int numElementos;
} tListaCalibracion;



/* *********************************
	**  PROTOTIPOS PRECALIBRACION  **
	********************************* */

extern NodoPrecalibracion* DevuelveNodo (tListaPrecalib&, int pos = 0); // devuelve
					// el nodo que hay en la posicion indicada
extern void agnadeNodo (tListaPrecalib&, NodoPrecalibracion*); // agnade un nodo
					// a una lista de precalibracion
extern void quitaNodo(tListaPrecalib&, int); // quita un nodo de una lista de
					// precalibracion
extern void borraLista(tListaPrecalib&); // borra todos los nodos de la lista
					// de precalibracion
extern void DevuelvePreCalibracion(tListaPrecalib, tPrecalibracion* &); 
					// Devuelve una matriz con los datos de precalibracion compatible
					// con el algoritmo de precalibracion
extern void DevuelveDatoPreCalibracion(tListaPrecalib, int, tPrecalibracion&);
					// Devuelve un elemento de la lista indicada en el formato de un
					// elemento de calibracion. El primer elemento es el 1.


/* ******************************
	**  PROTOTIPOS CALIBRACION  **
	****************************** */

extern NodoCalibracion* DevuelveNodo (tListaCalibracion&, int pos = 0);
					// devuelve el nodo que hay en la posicion indicada
extern int agnadeNodo (tListaCalibracion&, NodoCalibracion*); // agnade un
					// nodo a una lista de calibracion y devuelve la posicion
					// en la que lo añadio
extern void quitaNodo(tListaCalibracion&, int); // quita un nodo de una lista
					// de calibracion
extern void borraLista(tListaCalibracion&); // borra todos los nodos de la
					// lista de calibracion

#endif
