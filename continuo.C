#include <stdio.h>
#include "continuo.h"

// tListaContinuo y tListaContinuoInterv se encuentran en continuo.h.
//tListaContinuo contiene los límites de un intervalo y los valores de cofo, copi y manphe
//tListaInterv contiene los límites de un intervalo y un valor asociado (cofo, copi o manphe)

// Prototipos

int agnadeNodo (tListaContinuo&, NodoContinuo*);
void quitaNodo(tListaContinuo&, int);
void borraLista(tListaContinuo&);
NodoContinuo* DevuelveNodo (tListaContinuo&, int pos);
int agnadeNodo (tListaContinuoInterv&, NodoContinuoInterv*);
void quitaNodo(tListaContinuoInterv&, int);
void borraLista(tListaContinuoInterv&);
void ajustaListaInterv(tListaContinuoInterv&);
int construyeLista (tListaContinuo, tListaContinuoInterv&);
bool completitudListaInterv (tListaContinuoInterv);
float DevuelveValorListaInterv(tListaContinuoInterv&, int);



// Implementacion

//********************* DEVUELVE NODO **************************
/* Nos devuelve el nodo en pos de la lista de tipo tListaContinuo */
NodoContinuo* DevuelveNodo (tListaContinuo &lista, int pos) {
	NodoContinuo **puntero;
	
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

//******************************* AÑADE NODO *********************
/* Añade un nodo de tipo NodoContinuo a una lista de tipo tListaContinuo*/
int agnadeNodo (tListaContinuo &lista, NodoContinuo *nodo) {
	NodoContinuo **puntero;
	
	puntero = &(lista.inicio);
	int i = 1;
	while ((*puntero) != NULL) {
		// comprueba si ha de superponerlos
		if (((*puntero)->limInferior == nodo->limInferior)&&
			 		((*puntero)->limSuperior == nodo->limSuperior)) {
			if (nodo->cofo == -1.0) nodo->cofo = (*puntero)->cofo;
			if (nodo->copi == -1.0) nodo->copi = (*puntero)->copi;
			if (nodo->manphe == -1.0) nodo->manphe = (*puntero)->manphe;
			
			nodo->siguiente = (*puntero)->siguiente;
			delete (*puntero);
			*puntero = nodo;
			return i;
		}
		// comprueba si hay que colocarlo antes
		else if (((*puntero)->limInferior > nodo->limInferior)||
			 (((*puntero)->limInferior == nodo->limInferior)&&
			  ((*puntero)->limSuperior < nodo->limSuperior))){
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

//*********************** QUITA NODO ********************
/* Quita un nodo de una lista de tipo tListaContinuo */

void quitaNodo(tListaContinuo& lista, int num) {
	if (lista.numElementos >= num) {
		NodoContinuo** puntero = &(lista.inicio);
		NodoContinuo *borrar;
		for (int i = 1; i < num; i++) {
			puntero = &(*puntero)->siguiente;
		}
		borrar = *puntero;
		*puntero = (*puntero)->siguiente;
		delete borrar;
		lista.numElementos--;
	}
}

//********************** BORRA LISTA **********************
/* Elimina todos los nodos de una lista de tipo tListaContinuo */
void borraLista(tListaContinuo& lista) {
	while (lista.numElementos > 0)
		quitaNodo(lista, 1);
}

//************************** AÑADE NODO *************************
/* Añade un nodo a la lista de intervalos, cuidando de introducirlo
	en el orden que le corresponde según sus límites.
	Si se encuentra dentro de los límites de algún otro, se divide
	el intervalo y se pone el nodo nuevo entre medias
 */
int agnadeNodo (tListaContinuoInterv &lista, NodoContinuoInterv *nodo) {
	NodoContinuoInterv **puntero;
	
	puntero = &(lista.inicio);
	lista.numElementos++;
	int i = 1;
	while ((*puntero) != NULL) {
		// compara los dos intervalos
		if ((*puntero)->limSuperior < nodo->limInferior) {
			// pasa al siguiente elemento
			puntero = &((*puntero)->siguiente);
			i++;
		}
		else if (((*puntero)->limInferior < nodo->limInferior)&&
			  ((*puntero)->limSuperior > nodo->limSuperior)) {
			// intervalo nuevo englobado en anterior
			// hay que dividir el intervalo anterior y poner el nuevo
			// entre medias
			NodoContinuoInterv *nuevo;
			if ((nuevo = new NodoContinuoInterv)== NULL)
				return -1;
			nuevo->limInferior = nodo->limSuperior + 1;
			nuevo->limSuperior = (*puntero)->limSuperior;
			nuevo->valor = (*puntero)->valor;
			(*puntero)->limSuperior = nodo->limInferior - 1;
			nuevo->siguiente = (*puntero)->siguiente;
			(*puntero)->siguiente = nodo;
			nodo->siguiente = nuevo;
			lista.numElementos++;
			return i+1;
		}
		else {
			// no hay que crear ningun nodo nuevo, solo ajustar/destruir
			// los existentes
			
			if ((*puntero)->limInferior < nodo->limInferior) {
				// necesita ajustar la parte izquierda
				(*puntero)->limSuperior = nodo->limInferior - 1;
				//indica el siguiente nodo para ser ajustado
				nodo->siguiente = (*puntero)->siguiente;
				(*puntero)->siguiente = nodo;
			}
			else {
				nodo->siguiente = (*puntero);
				(*puntero) = nodo;
			}
			
			puntero = &(nodo->siguiente);
			
			// comprueba si hace falta borrar los siguientes elementos
			while ((*puntero) != NULL) {
				if ((*puntero)->limSuperior <= nodo->limSuperior) {
					// este nodo hay que borrarlo
					NodoContinuoInterv *borrar = (*puntero);
					puntero = &((*puntero)->siguiente);
					delete borrar;
					lista.numElementos--;
				}
				else {
					// necesita ajustar la parte derecha
					(*puntero)->limInferior = nodo->limSuperior + 1;
					nodo->siguiente = (*puntero);
					return i+1;
				}
			}
			return i+1;
		}
	}
	// pone el elemento al final de la lista
	nodo->siguiente = NULL;
	(*puntero) = nodo;
	return i;
}

//************************** QUITA NODO *****************
/* Elimina un nodo de una lista manteniendo la estructura */
void quitaNodo(tListaContinuoInterv& lista, int num) {
	if (lista.numElementos >= num) {
		NodoContinuoInterv** puntero = &(lista.inicio);
		NodoContinuoInterv *borrar;
		for (int i = 1; i < num; i++) {
			puntero = &(*puntero)->siguiente;
		}
		borrar = *puntero;
		*puntero = (*puntero)->siguiente;
		delete borrar;
		lista.numElementos--;
	}
}

//************************* BORRA LISTA *****************
/* Elimina todos los nodos de la lista */

void borraLista(tListaContinuoInterv& lista) {
	while (lista.numElementos > 0)
		quitaNodo(lista, 1);
}

//***********************  AJUSTA LISTA INTERV *******************
/* Reúne nodos que formen intervalos iguales. Si un nodo tiene el mismo valor
	que el nodo siguente y su límites están contiguos, se pueden fundir en un
	mismo intervalo
	*/
void ajustaListaInterv(tListaContinuoInterv& lista) 
	{
	NodoContinuoInterv **puntero = &(lista.inicio);
	
	while ((*puntero) != NULL) 
		{
		if ((*puntero)->siguiente != NULL) 
			{
			if (((*puntero)->valor == (*puntero)->siguiente->valor)&&
				((*puntero)->limSuperior == (*puntero)->siguiente->limInferior - 1)) 
				{
				// este nodo sobra
				NodoContinuoInterv *borrar = (*puntero)->siguiente;
				(*puntero)->limSuperior = (*puntero)->siguiente->limSuperior;
				(*puntero)->siguiente = (*puntero)->siguiente->siguiente;
				delete borrar;
				lista.numElementos--;
				}
			else 
				puntero = &((*puntero)->siguiente);
			}
		else
			puntero = &((*puntero)->siguiente);
		}
	}
	

//**************************** CONSTRUYE LISTA ***************************
/* Construye una lista que tendrá nodos con la información (límite inferior, límite superior, valor)
	Cada uno de esos valores puede ser cofo, copi o manphe según el tipo de dato que se pase como parámetro.
	Esta lista se construye a partir de la información en lista y se crea en listaInterv
*/
int construyeLista (tListaContinuo lista, tListaContinuoInterv& listaInterv, const int  tipoDato) {	
	// comprueba el tipo de dato
	if ((tipoDato < 1) && (tipoDato > 3))
		return -3; // tipo de dato desconocido

	// inicializa la lista
	borraLista(listaInterv);

	for (int i = 1; i <= lista.numElementos; i++) 
		{//Para cada nodo que tengamos con información del continuo
		NodoContinuo *puntero = DevuelveNodo(lista, i);
		
		//Capturamos en valor la información del tipo de dato que estemos cogiendo
		float valor = -1.0;
		switch (tipoDato) 
			{
			case 1:
				valor = puntero->cofo;
				break;
			case 2:
				valor = puntero->copi;
				break;
			case 3:
				valor = puntero->manphe;
				break;
			}
		
		//Si el valor es válido (debería serlo según cómo se dejan introducir los datos	
		if (valor != -1.0) 
			{
			//Creamos un nuevo nodo para listaInterv
			NodoContinuoInterv *nuevo = new NodoContinuoInterv;
			if (nuevo == NULL) // error de falta de memoria
				return -2;

			nuevo->limInferior = puntero->limInferior;
			nuevo->limSuperior = puntero->limSuperior;
			nuevo->valor = valor;

			if (agnadeNodo(listaInterv, nuevo) == -1)
				return -2;
			}
		//NOTA: Aunque a priori no debería ocurrir nunca, no consideramos el caso de que valor sea erróneo!
		}
	
	// elimina los intervalos innecesarios
	ajustaListaInterv(listaInterv);

	// pone el primer elemento en la zona de consulta
	listaInterv.UltimaConsulta = listaInterv.inicio;
	
	//Comprueba que entre todos los intervalos no haya discontinuidades
	//(que a un intervalo le siga justo el siguiente sin canales entre medias).
	if (completitudListaInterv (listaInterv))	return 0;
	else						return 1;
}

//******************** COMPLETITUD LISTA INTERV ********************************
/** Comprueba si el límite superior de cada nodo coincide con el inferior del siguiente,
	de forma que esté cubierto todo el continuo
	*/
bool completitudListaInterv (tListaContinuoInterv lista) {
	NodoContinuoInterv **puntero = &(lista.inicio);
	
	if ((*puntero) == NULL)
		return false;
	while ((*puntero)->siguiente != NULL) {
		if ((*puntero)->limSuperior != (*puntero)->siguiente->limInferior - 1)
			// no hay continuidad en la lista
			return false;
		puntero = &((*puntero)->siguiente);
	}
	return true;
}

// ********************* DEVUELVE VALOR LISTA INTERV ************************
/* Devuelve el valor del nodo en posición de una lista de tipo tListaCOntinuoInterv */
float DevuelveValorListaInterv(tListaContinuoInterv& lista, int posicion) {
	if ((lista.UltimaConsulta == NULL)||(posicion < lista.UltimaConsulta->limInferior))
		lista.UltimaConsulta = lista.inicio;

	if (lista.UltimaConsulta == NULL) // lista vacia
		return -1.0;
		
	while ((posicion > lista.UltimaConsulta->limSuperior)&&
			 (lista.UltimaConsulta->siguiente != NULL))
		lista.UltimaConsulta = lista.UltimaConsulta->siguiente;

	if ((posicion >= lista.UltimaConsulta->limInferior)&&
		 (posicion <= lista.UltimaConsulta->limSuperior))
		return lista.UltimaConsulta->valor;
	
	return -1.0;
}

//************************ DEVUELVE ALCANCE LISTA INTERV *************************
/* Almacena en min y max el límite inferior y superior, respectivamente, del nodo inicial y del final, 
   es decir, el límite inferior y superior de la lista en su conjunto.
   Si todo ocurre como debe, devuelve 0.
   En caso de que el límite final sea menor que el inicial devuelve -2
   Si la lista está vacía, devuelve -1
   */
int DevuelveAlcanceListaInterv(tListaContinuoInterv lista, int& min, int& max) 
	{
	NodoContinuoInterv *puntero = lista.inicio;

	if (puntero != NULL) 
		{
		printf("La lista existe, buscando límites\n");
		min = puntero->limInferior;
		while (puntero->siguiente != NULL)
			puntero = puntero->siguiente;
		max = puntero->limSuperior;
		if (max >= min)		return 0;
		return -2;
		}
	printf("La lista no existe\n");
	return -1;
	}
/*
int DevuelveAlcanceListaInterv(tListaContinuoInterv &lista, int& min, int& max) 
	{
	NodoContinuoInterv *puntero = lista->inicio;

	if (puntero != NULL) 
		{
		printf("La lista existe, buscando límites\n");
		min = puntero->limInferior;
		while (puntero->siguiente != NULL)
			puntero = puntero->siguiente;
		max = puntero->limSuperior;
		if (max >= min)		return 0;
		return -2;
		}
	printf("La lista no existe\n");
	return -1;
	}
*/
