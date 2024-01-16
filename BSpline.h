#ifndef __BSPLINE_H__
#define __BSPLINE_H__

// *** TIPOS DE DATOS ***

typedef struct NodoBSpline 
	{
	int posic; // posicion del nodo
	NodoBSpline *siguiente;
	} NodoBSpline;

//************** T LISTA NODOS BSPLINE ************
//Lista con cada uno de los parámetros de la spline?
typedef struct tListaNodosBSpline 
	{
	NodoBSpline *inicio;
	int numElementos;
	} tListaNodosBSpline;

//*********** T LISTA INTERVALOS B SPLINE ************
//Esta lista contiene una lista con nodos para el spline
//y punteros a sus intervalos, pesos y covarianzas.
//Tiene otros muchos valores pero de momento no sé muy bien
//cómo funciona
typedef struct tListaIntervalosBSpline 
	{
	//Cada uno de los intervalos de la spline. Cada uno de esos nodos tiene un sólo valor posic
	//NOTA: Este valor posic no sé exactamente a qué se refiere, parece que es el último valor,
	//	el de la derecha.
	tListaNodosBSpline listaNodos;
	double *intervalo;
	double *peso;
	double *covar;
	int numElementos;
	//NOTA: Estos nodos sirven para llevar una cuenta de los últimos nodos que se han procesado (?)
	int UltNodo0, UltNodo1, UltNodo2, UltNodom1, UltInterv;
	} tListaIntervalosBSpline;


//  *** PROTOTIPOS ***

extern int ValorNodo(tListaNodosBSpline, int); /* devuelve el valor del
					nodo indicado */
extern int agnadeNodo (tListaNodosBSpline&, NodoBSpline*); /* añade un nodo
					a la lista, poniendolo en orden */
extern void quitaNodo(tListaNodosBSpline&, int); /* quita el nodo indicado */
extern void borraLista(tListaNodosBSpline&); /* elimina todos los nodos de
					la lista */

extern int InicializaBSpline(tListaIntervalosBSpline&); /* limpia la
					estructura pasada (supuesta recien creada) */
extern int DimensionaBSpline(tListaIntervalosBSpline&); /* adapta el numero de
					intervalos al numero de nodos.
					Devuelve:
						-1 -> No consiguió crear el vector */
extern void borraLista(tListaIntervalosBSpline&); /* elimina los nodos y los
					intervalos de la lista pasada */
extern void CalculaIntervalosBSpline(tListaIntervalosBSpline&, double); /* calcula
					el valor de todos los splines para la posicion indicada */
extern double CalculaValorBSpline(tListaIntervalosBSpline&, double); /* calcula
					el valor de la funcion simulada por el spline indicado */

#endif
