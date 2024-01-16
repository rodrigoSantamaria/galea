#ifndef __ANALISIS_H__
#define __ANALISIS_H__

/* *************************
	**  TIPOS ANALISIS  **
	************************* */

typedef struct NodoAnalisis {
	int numIntervalo; // numero de intervalo de picos
	NodoAnalisis *siguiente;
} NodoAnalisis;

typedef struct tListaAnalisis {
	NodoAnalisis *inicio;
	int numElementos;
} tListaAnalisis;

//Estructura para almacenar información de límites de detección con respecto
//a emisiones de radionúclidos extraidas de un fichero, que no tienen por qué
//existir en los picos que hemos detectado.
typedef struct nodoCentroide
	{
	float centroide;
	int atomicnumber;
	int massnumber;		
	float ld;
	float amd;   //felipe para almacenar el amd	
	int indice; //felipe para el calculo del amd en listas
	nodoCentroide *siguiente;
	}nodoCentroide;



/* ******************************
	**   PROTOTIPOS ANÁLISIS   **
	****************************** */

extern NodoAnalisis* DevuelveNodo (tListaAnalisis&, int pos = 0);
					// devuelve el nodo que hay en la posicion indicada
extern int agnadeNodo (tListaAnalisis&, NodoAnalisis*); // agnade un
					// nodo a una lista de analisis y devuelve la posicion
					// en la que lo añadio
extern void quitaNodo(tListaAnalisis&, int); // quita un nodo de una lista
					// de analisis
extern void borraLista(tListaAnalisis&); // borra todos los nodos de la
					// lista de analisis
extern bool estaNodo (tListaAnalisis&, NodoAnalisis*);

#endif
