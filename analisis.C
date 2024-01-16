#include <stdio.h>
#include "analisis.h"

// ******************
// *** prototipos ***
// ******************

//NOTA: No sé por qué a veces pone como argumentos cosas como int pos=0, que generalmente (y como es lógico)
//	dan error en los compiladores. Yo la opción que tomo es directamente quitar el =0
NodoAnalisis* DevuelveNodo (tListaAnalisis&, int pos);
int agnadeNodo (tListaAnalisis&, NodoAnalisis*);
void quitaNodo(tListaAnalisis&, int);
void borraLista(tListaAnalisis&);
bool estaNodo (tListaAnalisis&, NodoAnalisis*);


// ************************
// ***  implementacion  ***
// ************************

//*************** ESTÁ NODO ****************
//Devuelve true si nodo está en lista y false en caso contrario
bool estaNodo(tListaAnalisis &lista, NodoAnalisis *nodo)
	{
	NodoAnalisis **puntero;
	
	puntero = &(lista.inicio);
	while ((*puntero) != NULL) 
		{
		// comprueba si ya existia este intervalo
		if ((*puntero)->numIntervalo == nodo->numIntervalo)		return true;
		// pasa al siguiente elemento
		puntero = &((*puntero)->siguiente);
		}

	return false;
	}
	
//************** AGNADE NODO ********************
int agnadeNodo (tListaAnalisis &lista, NodoAnalisis *nodo) 
	{
	NodoAnalisis **puntero;
	int pos = 1;
	
	puntero = &(lista.inicio);
	while ((*puntero) != NULL) 
		{
		// comprueba si ya existia este intervalo
		if ((*puntero)->numIntervalo == nodo->numIntervalo)
			return pos;
		// comprueba si hay que colocarlo antes
			
		else 
			
			//printf("Nodo actual vale %d y el que vamos a añadir %d\n", (*puntero)->numIntervalo, nodo->numIntervalo);
		if ((*puntero)->numIntervalo >= nodo->numIntervalo) 
			{
			nodo->siguiente = *puntero;
			*puntero = nodo;
			lista.numElementos++;
			return pos;
			}
		// pasa al siguiente elemento
		puntero = &((*puntero)->siguiente);
		pos++;
		}

	// pone el elemento al final de la lista
	nodo->siguiente = NULL;
	*puntero = nodo;
	lista.numElementos++;
	return pos;
	}

void quitaNodo(tListaAnalisis& lista, int num) {
	if (lista.numElementos >= num) {
		NodoAnalisis** puntero = &(lista.inicio);
		NodoAnalisis *borrar;
		for (int i = 1; i < num; i++) {
			puntero = &(*puntero)->siguiente;
		}
		borrar = *puntero;
		*puntero = (*puntero)->siguiente;
		delete borrar;
		lista.numElementos--;
	}
}

void borraLista(tListaAnalisis& lista) {
	while (lista.numElementos > 0)
		quitaNodo(lista, 1);
}

//NOTA: quitado un =0
NodoAnalisis* DevuelveNodo (tListaAnalisis &lista, int pos) {
	NodoAnalisis **puntero;
	
	if ((pos == 0)||(pos > lista.numElementos)) pos = lista.numElementos;
	puntero = &(lista.inicio);
	int i = 1;
	while (((*puntero) != NULL)&&(i < pos)) {
		// pasa al siguiente elemento
		puntero = &((*puntero)->siguiente);
		i++;
	}
	return *puntero;
}
