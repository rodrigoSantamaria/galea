#include <stdio.h>
#include "picos.h"

// Prototipos

void iniciaNodo(NodoPico *);
int agnadeNodo (tListaPicos&, NodoPicos*);
void quitaNodo(tListaPicos&, int);
void borraLista(tListaPicos&);
NodoPicos* DevuelveNodo (tListaPicos&, int pos );//=0

int agnadeNodo (tListaDatosPicos&, NodoPico*);
void quitaNodo(tListaDatosPicos&, int);
void borraLista(tListaDatosPicos&);
NodoPico* DevuelveNodo (tListaDatosPicos&, int pos );//=0
void clonaNodo (NodoPico*, NodoPico*);

// Implementacion

int agnadeNodo (tListaPicos &lista, NodoPicos *nodo) 
	{
//	printf("AGNADE NODO\n");
	NodoPicos **puntero;
	
	puntero = &(lista.inicio);
	int i = 1;
	while ((*puntero) != NULL) 
		{
		// comprueba si hay que colocarlo antes
		if ((*puntero)->limInferior > nodo->limInferior) 
			{
			nodo->siguiente = *puntero;
			*puntero = nodo;
			lista.numElementos++;
	//		printf("Añadimos el pico %d\n",i);
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
//	printf("Añadimos el pico %d\n",i);
	return i;
	}

//************* QUITA NODO *************
void quitaNodo(tListaPicos& lista, int num) 
	{
	printf("QUITA NODO\n");
	if (lista.numElementos >= num) 
		{
		NodoPicos** puntero = &(lista.inicio);
		NodoPicos *borrar;
		for (int i = 1; i < num; i++) 
			{
			puntero = &(*puntero)->siguiente;
			}
		borrar = *puntero;
		*puntero = (*puntero)->siguiente;
		borraLista(borrar->picos);
		delete borrar;
		lista.numElementos--;
		}
	}

//*********** BORRA LISTA ****************
void borraLista(tListaPicos& lista) 
	{
	printf("BORRA LISTA\n");
	int i=0;
	while (lista.numElementos > 0)		
		{
		printf("%d\n", i++);
		quitaNodo(lista, 1);
		}
	}

//******************** DEVUELVE NODO *******************
/* Devuelve el nodo de la lista que se encuentra en la posición pos
*/
NodoPicos* DevuelveNodo (tListaPicos &lista, int pos ) 
	{
	NodoPicos **puntero;
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

int InicializaNodo(NodoPicos* &nodo) 
	{
	if ((nodo = new NodoPicos) == NULL) return -1;
	nodo->limInferior = 0;
	nodo->limSuperior = 0;
	nodo->picos.numElementos = 0;
	nodo->picos.inicio = NULL;
	return 0;
	}

//************ INICIA NODO ********
/*	Inicia las distintas variables almacenadas para cada pico, todas a 0 o NULL,
	según hablemos de variables o punteros
	*/
void iniciaNodo(NodoPico *pico)
	{
//	printf("__________________ INICIA NODO ________________\n");
	pico->Ajuste.Centroide=pico->Ajuste.Anchura=pico->Ajuste.Tau=0.0;
	pico->Ajuste.Ytau=pico->Ajuste.P=pico->Ajuste.YP=0.0;
	pico->Ajuste.SigCentroide=pico->Ajuste.SigAnchura=pico->Ajuste.SigTau=0.0;
	pico->Ajuste.SigYtau=pico->Ajuste.SigP=pico->Ajuste.SigYP=0.0;
	pico->Ajuste.SigTauYtau=pico->Ajuste.SigPYP=0.0;
		
	for(int i=0;i<4;i++)	for(int j=0;j<4;j++)	pico->Ajuste.covar[i][j]=0.0;
	pico->Ajuste.inicializado = false;
	pico->Ajuste.ajustado = false;
	pico->Ajuste.calibrado = false;
	pico->Ajuste.identificado = false;
										
	pico->Ajuste.eficienciaCalculada = false;
	pico->Ajuste.ldCalculado = false;
	pico->Ajuste.analizado = false;
	//
	pico->Ajuste.invisible = false;
	//
	//printf("Iniciamos las emisiones\n");
	//if(pico->Ajuste.emisiones!=NULL)	pico->Ajuste.emisiones->deleteAll();
	//else								pico->Ajuste.emisiones=new LinkedList();
	pico->Ajuste.emisiones=NULL;
	pico->Ajuste.emisiones=new LinkedList();
		
	pico->Ajuste.ld=pico->Ajuste.lc=pico->Ajuste.eficiencia=0.0;
	pico->Ajuste.sigmaEficiencia=pico->Ajuste.correccion=0.0;
	pico->Ajuste.sigmaCorreccion=pico->Ajuste.area=pico->Ajuste.incerArea=0.0;
	pico->Ajuste.areaSinFondo=pico->Ajuste.incerAreaSinFondo=0.0;
	pico->Ajuste.actividad=pico->Ajuste.sigmaActividad=pico->Ajuste.mda=0.0;
	pico->Ajuste.mdaPadre=0.0;
	
	//printf("Iniciamos el vector de parámetros de la eficiencia\n");
	/*if(pico->Ajuste.parametrosEficiencia!=NULL)	
		delete [] pico->Ajuste.parametrosEficiencia;*/
//	pico->Ajuste.parametrosEficiencia=NULL;
	pico->maximo=0;
	pico->comprobado=false;
	}

//************** AGNADE NODO *******************
int agnadeNodo (tListaDatosPicos &lista, NodoPico *nodo) 
	{
	//printf("AgnadeNodo\n");
	NodoPico **puntero=NULL;
	
	//printf("Apuntamos al primero\n");
	puntero = &(lista.inicio);
	int i = 1;
//	printf("numero de picos en la lista: %d\n", lista.numElementos);
//	printf("Bucle\n");
	while ((*puntero) != NULL) 
		{
//		printf("%d: comprobamso si hay que colocarlo antes\n", i);
		//if(nodo->maximo==0)	;
		//printf("es el puntero\n");
		//if((*puntero)->maximo==0)	;
		//printf("es la mezcla?\n");	
		// comprueba si hay que colocarlo antes
		if ((*puntero)->maximo > nodo->maximo) 
			{
//			printf("Hay q colocarlo antes\n");
			nodo->siguiente = *puntero;
			*puntero = nodo;
			lista.numElementos++;
			return i;
			}
//		printf("pasamos al siguiente\n");
		// pasa al siguiente elemento
		puntero = &((*puntero)->siguiente);
		i++;
		}
//	printf("Ponemos el nuevo elemento al final de la lista\n");
	// pone el elemento al final de la lista
	nodo->siguiente = NULL;
	*puntero = nodo;
	lista.numElementos++;
	return i;
	}

//************ QUITA NODO TLISTADATOSPICOS ************
void quitaNodo(tListaDatosPicos& lista, int num) 
	{
	if (lista.numElementos >= num) 
		{
		NodoPico** puntero = &(lista.inicio);
		NodoPico *borrar;
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

void borraLista(tListaDatosPicos& lista) 
	{
	while (lista.numElementos > 0)	quitaNodo(lista, 1);
	}

NodoPico* DevuelveNodo (tListaDatosPicos &lista, int pos ) 
	{
	NodoPico **puntero;
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

void clonaNodo (NodoPico* picoMadre, NodoPico* picoClon) 
	{
	picoClon->maximo = picoMadre->maximo;
    picoClon->Ajuste = picoMadre->Ajuste;
	picoClon->siguiente = picoMadre->siguiente;
	picoClon->comprobado=picoMadre->comprobado;
	}
