#include <stdio.h>
#include <math.h>
#include "math1.h"
#include "BSpline.h"

/*  **********
	 PROTOTIPOS
	 ********** */
	 
int ValorNodo(tListaNodosBSpline, int);
int agnadeNodo (tListaNodosBSpline&, NodoBSpline*);
void quitaNodo(tListaNodosBSpline&, int);
void borraLista(tListaNodosBSpline&);
int InicializaBSpline(tListaIntervalosBSpline&);
int DimensionaBSpline(tListaIntervalosBSpline&);
double CalculaValorBSpline(tListaIntervalosBSpline&, double);
void CalculaIntervalosBSpline(tListaIntervalosBSpline&, double);
void borraLista(tListaIntervalosBSpline&);


/* **************
	IMPLEMENTACION
	************** */
double CalculaValorBSpline(tListaIntervalosBSpline& lista, double pos) {
	CalculaIntervalosBSpline(lista, pos);
	double acum = 0.0;
	for (int i = 1; i <= lista.numElementos; i++)
		acum += lista.peso[i]*lista.intervalo[i];
	return acum;
}

//***************** CALCULA INTERVALOS BSPLINE ****************************
// Calcula el valor que toman las B-Spline correspondientes en la posición pos
// Para una posición determinada (pos) se considera que como máximo 3 BSplines 
// se ven involucradas en el valor del ajuste. El resto tendrán valor cero para esa posición.

void CalculaIntervalosBSpline(tListaIntervalosBSpline& lista, double pos) 
	{
	double nodom1, nodo0, nodo1, nodo2;
	int i;
	
	// inicializa la estructura
	for (int i = 1; i <= lista.numElementos; i++)
		lista.intervalo[i] = 0.0;
	//NOTA: Las partes comentadas en las que se modifica el intervalo se han pasado a un código común fuera
	//	del if-else, y se han optimizado sus operaciones
	
	// si la consulta se encuentra en el mismo intervalo que la anterior no necesitamos buscar el intervalo
	if ((pos >= lista.UltNodo0) && (pos < lista.UltNodo1)) 
		{
		i = lista.UltInterv;
		nodom1= (double) lista.UltNodom1;
		nodo0 = (double) lista.UltNodo0;
		nodo1 = (double) lista.UltNodo1;
		nodo2 = (double) lista.UltNodo2;
		//NOTA: Para acelerar los procesos podemos precalcular algunas restas que se usan varias
		//	veces: nodo1-nodo0 (4) ; nodo1-nodom1 (2) ; nodo2-nodo0 (2) ; pos-nodo0 (2-3)
		//		nodo1-pos (2-3)
		//	Y también cambiar las pow2 por multiplicaciones
/*		lista.intervalo[i] = ((pos-nodom1)*(nodo1-pos))/((nodo1-nodom1)*(nodo1-nodo0)) +
				     ((nodo2-pos)*(pos-nodo0)) /((nodo2-nodo0)*(nodo1-nodo0));
		lista.intervalo[i+1] = powi((pos-nodo0), 2)/((nodo2-nodo0)*(nodo1-nodo0));
		if (i > 1)	lista.intervalo[i-1] = (powi((nodo1-pos),2))/((nodo1-nodom1)*(nodo1-nodo0));
		*/
		}
		
	// si no es igual lo hacemos si tenemos al menos los cuatro nodos con los que 
	// iniciamos en PreparaAjusteCanalesContinuo (de espectros.C)
	else if (lista.listaNodos.numElementos > 3) 
		{
		// comprueba que el valor pasado se encuentra en el intervalo total
		int extIzq = ValorNodo(lista.listaNodos, 2);
		int extDer = ValorNodo(lista.listaNodos, lista.listaNodos.numElementos - 1);
		if ((redondea(pos) >= extIzq) && (redondea(pos) < extDer)) 
			{
			// busca el intervalo en el que se encuentra la posicion pasada
			i = 2;
			extDer = ValorNodo(lista.listaNodos, i+1);
			while  (pos > extDer) 
				{
				i++;
				extDer = ValorNodo(lista.listaNodos, i+1);
				}
			//nodom1 es la posic(i-1), nodo0 es posic(i), nodo1 es posic(i+1) y nodo2 es posic(i+2)
			//pos está entre posic(i) y posic(i+1).
			lista.UltInterv = i;
			lista.UltNodom1 = ValorNodo(lista.listaNodos, i-1);
			lista.UltNodo0 = ValorNodo(lista.listaNodos, i);
			lista.UltNodo1 = extDer;
			lista.UltNodo2 = ValorNodo(lista.listaNodos, i+2);
			nodom1 = (double) lista.UltNodom1;
			nodo0 = (double) lista.UltNodo0;
			nodo1 = (double) extDer;
			nodo2 = (double) lista.UltNodo2;
		/*	lista.intervalo[i] = ((pos-nodom1)*(nodo1-pos))/((nodo1-nodom1)*(nodo1-nodo0)) +
					     ((nodo2-pos)*(pos-nodo0)) /((nodo2-nodo0)*(nodo1-nodo0));
			lista.intervalo[i+1] = powi((pos-nodo0), 2)/((nodo2-nodo0)*(nodo1-nodo0));
			if (i > 1)	lista.intervalo[i-1] = (powi((nodo1-pos),2))/((nodo1-nodom1)*(nodo1-nodo0));
		*/
			}
		}
	//Una vez calculadas las posiciones de los nodos calculamos los valores de la Bspline
	//NOTA: Parece que el funcionamiento no se ve resentido (es equivalente a lo anterior)
	//	y es sensiblemente más rápido. Entre esto y la mejora de if casi hemos ganado un 60%
	//	Todavía podríamos ganar más almacenando las restas, pero no creo que fuera un cambio
	//	demasiado notable, y ya sería un poco rancio no?
	lista.intervalo[i] = ((pos-nodom1)*(nodo1-pos))/((nodo1-nodom1)*(nodo1-nodo0)) +
			     ((nodo2-pos)*(pos-nodo0)) /((nodo2-nodo0)*(nodo1-nodo0));
	//NOTA: Quizá haya que hacer una comprobación por arriba como por abajo
	//	Ya la hemos puesto y parece que funciona bien
	if (i<lista.listaNodos.numElementos)	lista.intervalo[i+1] = ((pos-nodo0)*(pos-nodo0)) / ((nodo2-nodo0)*(nodo1-nodo0));
	if (i > 1)				lista.intervalo[i-1] = ((nodo1-pos)*(nodo1-pos)) / ((nodo1-nodom1)*(nodo1-nodo0));
	
	}


void borraLista(tListaIntervalosBSpline& lista) {
	borraLista(lista.listaNodos);
	delete [] lista.intervalo; lista.intervalo = NULL;
	delete [] lista.peso; lista.peso = NULL;
	delete [] lista.covar; lista.covar = NULL;
	lista.UltNodo0 = 0; lista.UltNodo1 = 0; lista.UltNodo2 = 0;
	lista.UltNodom1 = 0; lista.UltInterv = 0;
}


//********************* DIMENSIONA BSPLINE ******************
// Lo único que hace es inicializar la lista de intervalos de spline
// (Da dimensiones a las variables que contiene)
//Pone las variables numéricas a cero.
//-Si tenemos algún nodo en la lista de splines y ese número de nodos no coincide con 
//el número de elementos que se supone que tiene la lista, borramos los tres array (intervalo, peso y covar)
//y los rehacemos con el número de nodos que haya en la lista de Splines (el el caso de la covarianza con su cuadrado,
// pues aunque sea un array va a representar una matriz de numElementos*numElementos)
//por último corregimos el número de elementos de la estructura.
int DimensionaBSpline(tListaIntervalosBSpline& lista) 
	{
	lista.UltNodo0 = 0; lista.UltNodo1 = 0;
	lista.UltNodo2 = 0; lista.UltNodom1 = 0;
	lista.UltInterv = 0;
	if ((lista.numElementos != lista.listaNodos.numElementos + 1) && (lista.listaNodos.numElementos > 1)) 
		{
		delete [] lista.intervalo;
		if ((lista.intervalo = new double[lista.listaNodos.numElementos]) == NULL) 
			{
			lista.numElementos = 0;
			return -1;
			}
		delete [] lista.peso;
		if ((lista.peso = new double[lista.listaNodos.numElementos]) == NULL) 
			{
			delete [] lista.intervalo; lista.intervalo = NULL;
			lista.numElementos = 0;
			return -1;
			}
		delete [] lista.covar;
		int temp = (lista.listaNodos.numElementos)*(lista.listaNodos.numElementos);
		if ((lista.covar = new double[temp]) == NULL) 
			{
			delete [] lista.intervalo; lista.intervalo = NULL;
			delete [] lista.peso; lista.peso = NULL;
			lista.numElementos = 0;
			return -1;
			}
		lista.numElementos = lista.listaNodos.numElementos - 1;
		}
	}

int InicializaBSpline(tListaIntervalosBSpline& lista) 
	{
	lista.listaNodos.numElementos = 0;
	lista.listaNodos.inicio = NULL;
	lista.numElementos = 0;
	lista.intervalo = NULL;
	lista.peso = NULL;
	lista.covar = NULL;
	lista.UltNodo0 = 0; lista.UltNodo1 = 0; lista.UltNodo2 = 0; lista.UltNodom1 = 0;
	lista.UltInterv = 0;
	}

//******************** VALOR NODO ***********************
// Nos devuelve el valor de posic del nodo la lista que está en la posición pos
int ValorNodo(tListaNodosBSpline lista, int pos) 
	{
	NodoBSpline **puntero;
	
	if ((pos == 0)||(pos > lista.numElementos)) pos = lista.numElementos;
	puntero = &(lista.inicio);
	int i = 1;
	while (((*puntero) != NULL) && (i < pos)) 
		{
		// pasa al siguiente elemento
		puntero = &((*puntero)->siguiente);
		i++;
		}
	return (*puntero)->posic;
	}

int agnadeNodo (tListaNodosBSpline& lista, NodoBSpline* nodo) {
	NodoBSpline **puntero;
	
	puntero = &(lista.inicio);
	int i = 1;
	while ((*puntero) != NULL) {
		// comprueba si hay que colocarlo antes
		if ((*puntero)->posic > nodo->posic) {
			nodo->siguiente = *puntero;
			*puntero = nodo;
			lista.numElementos++;
			return i;
		}

		// pasa al siguiente elemento
		puntero = &((*puntero)->siguiente);
		i++;
	}
	// pone el elemento al final de la lista
	nodo->siguiente = NULL;
	*puntero = nodo;
	lista.numElementos++;
	return i;
}

void quitaNodo(tListaNodosBSpline& lista, int num) {
	if (lista.numElementos >= num) {
		NodoBSpline** puntero = &(lista.inicio);
		NodoBSpline *borrar;
		for (int i = 1; i < num; i++) {
			puntero = &(*puntero)->siguiente;
		}
		borrar = *puntero;
		*puntero = (*puntero)->siguiente;
		delete borrar;
		lista.numElementos--;
	}
}

void borraLista(tListaNodosBSpline& lista) {
	while (lista.numElementos > 0)
		quitaNodo(lista, 1);
}
	
