#include <stdio.h>
#include "calibracion.h"

// ******************
// *** prototipos ***
// ******************

NodoPrecalibracion* DevuelveNodo (tListaPrecalib&, int pos );//=0
void agnadeNodo (tListaPrecalib&, NodoPrecalibracion *);
void quitaNodo(tListaPrecalib&, int);
void borraLista(tListaPrecalib&);
void DevuelvePreCalibracion(tListaPrecalib, tPrecalibracion* &);
void DevuelveDatoPreCalibracion(tListaPrecalib, int, tPrecalibracion&);

NodoCalibracion* DevuelveNodo (tListaCalibracion&, int pos);//=0
int agnadeNodo (tListaCalibracion&, NodoCalibracion*);
void quitaNodo(tListaCalibracion&, int);
void borraLista(tListaCalibracion&);


// ************************
// ***  implementacion  ***
// ************************

//*********************** A�ADE NODO ******************************
//A�ade un nodo tipo NodoPrecalibracion a una lista tipo tListaPrecalib
//La posici�n en la que se coloca el nodo es aqu�lla en la que sus l�mites est�n ordenados con respecto al resto
//NOTA: Esta funci�n al contrario que otras por el estilo no contempla que puede que los l�mites est�n incluidos
//	en otro intervalo. Habr�a que mirar si es que se ha olvidado o es innecesario por alguna raz�n.

void agnadeNodo (tListaPrecalib &lista, NodoPrecalibracion *nodo) {
	NodoPrecalibracion **puntero;
	
	puntero = &(lista.inicio);
	while ((*puntero) != NULL) {
		// comprueba si el elemento ha de cambiarse por el actual
		if (((*puntero)->limInferior < nodo->limSuperior)&&
			 ((*puntero)->limSuperior > nodo->limInferior)) {
			 nodo->siguiente = (*puntero)->siguiente;
			 delete (*puntero);
			 *puntero = nodo;
			 return;
		}
		// comprueba si hay que colocarlo antes
		else if ((*puntero)->limInferior > nodo->limInferior) {
			nodo->siguiente = *puntero;
			*puntero = nodo;
			lista.numElementos++;
			return;
		}
		// pasa al siguiente elemento
		puntero = &((*puntero)->siguiente);
	}
	// pone el elemento al final de la lista
	nodo->siguiente = NULL;
	*puntero = nodo;
	lista.numElementos++;
}

//**************************** DEVUELVE DATO PRECALIBRACI�N ***********************
//Devuelve en "dato" el "nodo" de lista que est� en la posici�n "num"
//Los nodos del tipo tPrecalibraci�n tienen como informaci�n: l�mite inferior, l�mite superior y energ�a.

void DevuelveDatoPreCalibracion(tListaPrecalib lista, int num, tPrecalibracion& dato) {
	if (lista.numElementos >= num) {
		NodoPrecalibracion *puntero = lista.inicio;
		for (int i = 1; i < num; i++) {
			puntero = puntero->siguiente;
		}
		dato.energia = puntero->energia;
		dato.limInferior = puntero->limInferior;
		dato.limSuperior = puntero->limSuperior;
	}
}

//*************************** QUITA NODO ***************************************
//Quita el nodo de la posici�n "num" de la lista
//NOTA: Estas funciones todas devuelven void. QUiz� ser�a conveniente que devolvieran un c�digo num�rico 
//	por los casos de fallo (p.ej. num es mayor que el n�mero de elementos de la lista.

void quitaNodo(tListaPrecalib& lista, int num) 
	{
	if (lista.numElementos >= num) 
		{
		NodoPrecalibracion** puntero = &(lista.inicio);
		NodoPrecalibracion *borrar;
		for (int i = 1; i < num; i++) 
			{
			puntero = &(*puntero)->siguiente;
			}
		borrar = *puntero;
		*puntero = (*puntero)->siguiente;
		delete borrar;
		lista.numElementos--;
		}
	}

//********************** BORRA LISTA *********************
//Elimina todos los nodos de "lista"
void borraLista(tListaPrecalib& lista) {
	while (lista.numElementos > 0)
		quitaNodo(lista, 1);
}

//*********************** DEVUELVE PRECALIBRACI�N ****************
//Copia en "puntero" toda la informaci�n de "lista". 
//NOTA: Puntero al contrario que lista parece que es un array, con lo que esta funci�n puede que tenga sentido
void DevuelvePreCalibracion(tListaPrecalib lista, tPrecalibracion* &puntero) {
	if (lista.numElementos > 0) {
		tPrecalibracion dato;
		if ((puntero = new tPrecalibracion[lista.numElementos]) != NULL) {
			for (int i = 1; i <= lista.numElementos; i++) {
				DevuelveDatoPreCalibracion(lista, i, dato);
				puntero[i-1].energia = dato.energia;
				puntero[i-1].limInferior = dato.limInferior-1;
				puntero[i-1].limSuperior = dato.limSuperior-1;				
			}
		}
	}
}

//************************ DEVUELVE NODO **************************
//Devuelve el nodo que se encuentra en la posici�n "pos" de "lista"
//Si no lo encontrara devolver�a null.
NodoPrecalibracion* DevuelveNodo (tListaPrecalib &lista, int pos) {//=0
	NodoPrecalibracion **puntero;
	
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

//**************************** A�ADE NODO ***************************
//A�ade "nodo" a "lista". Los nodosCalibraci�n s�lo contienen la informaci�n de numIntervalo.
//El nodo se coloca en la posici�n correspondiente a su num Intervalo. Si ya existiera ese numIntervalo
//no se a�ade.
//En cualquier caso la funci�n devuelve la posici�n en la que se ha insertado el nodo, o en la que est�
//el nodo equivalente.
int agnadeNodo (tListaCalibracion &lista, NodoCalibracion *nodo) 
	{
	NodoCalibracion **puntero;
	int pos = 1;
	
	puntero = &(lista.inicio);
	while ((*puntero) != NULL) 
		{
		// comprueba si ya existia este intervalo
		if ((*puntero)->numIntervalo == nodo->numIntervalo)
			return pos;
		// comprueba si hay que colocarlo antes
		else if ((*puntero)->numIntervalo > nodo->numIntervalo) 
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

//********************* QUITA NODO ********************
//Elimina el nodo que se encuentra en la posici�n "num" de "lista"
void quitaNodo(tListaCalibracion& lista, int num) 
	{
	if (lista.numElementos >= num) 
		{
		NodoCalibracion** puntero = &(lista.inicio);
		NodoCalibracion *borrar;
		for (int i = 1; i < num; i++) 
			{
			puntero = &(*puntero)->siguiente;
			}
		borrar = *puntero;
		*puntero = (*puntero)->siguiente;
		delete borrar;
		lista.numElementos--;
		}
	}

//********************** BORRA LISTA ***********************
//Elimina todos los nodos de "lista"
void borraLista(tListaCalibracion& lista) 
	{
	while (lista.numElementos > 0)	quitaNodo(lista, 1);
	}

//*********************** DEVUELVE NODO *************************
//Devuelve el nodo que se encuentra en la posici�n "pos" de "lista"
NodoCalibracion* DevuelveNodo (tListaCalibracion &lista, int pos) 
	{//=0
	NodoCalibracion **puntero;
	
	if ((pos == 0)||(pos > lista.numElementos)) pos = lista.numElementos;
	puntero = &(lista.inicio);
	int i = 1;
	while (((*puntero) != NULL)&&(i < pos)) 
		{
		// pasa al siguiente elemento
		puntero = &((*puntero)->siguiente);
		i++;
		}
	return *puntero;
	}
