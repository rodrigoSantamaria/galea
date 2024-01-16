#ifndef __PICOS_H__
#define __PICOS_H__

#ifndef LINKEDLIST_H
#include "GA/LinkedList.h"
#endif

// ******************************
// *** TIPOS DE DATOS AJUSTES ***
// ******************************

typedef struct tPicoAjuste 
	{
	// Datos Fisicos
	double Centroide, // canal del centroide del pico
		 Anchura,	  // anchura del pico (sigma)
		 Tau, 		  // anchura de la exponencial por absorcion de electrones (tau)
		 Ytau,		  // altura de la exponencial por absorcion de electrones
		 P,			  // anchura de la exponencial por viaje fotones
		 YP;		  // altura de la exponencial por viaje fotones
	double SigCentroide, // Desviaciones de los datos anteriores
		 SigAnchura,
		 SigTau,
		 SigYtau,
		 SigP,
		 SigYP,
		 SigTauYtau, // covarianza entre Tau e Ytau
		 SigPYP; 	 // covarianza entre P e YP
	double covar[4][4];//NUEVO: Contiene las relaciones de covarianza entre los 4 parámetros
			//actuales del ajuste, aparte de que algunos se encuentren en las sig anteriores
		
	// Datos administracion
	bool inicializado;// indica si ya ha sido inicializado este pico
	bool ajustado; 	  // indica que este pico ya ha sido ajustado y que los
					// datos que hay en el ajuste son validos
	bool calibrado;	  // indica si el pico tiene asignada una energia de emision durante calibración
	bool identificado;// indica si la energia de emision ha sido identificada durante análisis o calibración
	//NOTA: En construcción. No se guarda como el resto de momento
	bool invisible;//indica si un pico, por la razón que sea, no debe salir dibujado
					//de momento se está usando para los picos que se usan para determinar equilibrios
	//
	//bool identificadoEnCalibracion;// indica si la energia de emision ha sido identificada durante la calibración
	
	bool eficienciaCalculada;//Indica si el pico tiene una eficiencia calcualda
	bool ldCalculado;//indica si el pico tiene unos límites de detección calculados
	bool analizado;  //indica si el pico tiene asignados valores a la actividad,
					 //el área restado un cierto fondo, y los factores de corrección
					 //NOTA: Ojo, ahora está indicando que tengamos una emisión identificada en análisis, para
					 //		distinguir de las emisiones identificadas en calibración
	// Datos de identificacion
	LinkedList *emisiones;//Lista enlazada con todos los elementos asociados a este pico
	
	double ld; 				//Límite de detección para este pico
	double lc; 				//Límite de decisión para este pico
	double eficiencia;		//Eficiencia para este pico
	double sigmaEficiencia;	//Varianza de la eficiencia para este pico
	double correccion;		//Corrección de la eficiencia
	double sigmaCorreccion;	//Incertidumbre de la corrección de la eficiencia
	double area;			//Área del pico
	double incerArea;		//Incertidumbre del área del pico
	double areaSinFondo;	//Área del pico sin contar el fondo del detector
	double incerAreaSinFondo;//Incertidumbre del área del pico sin contar la del fondo del detector (?)dpb
	double actividad;		//Actividad del pico
	double sigmaActividad;	//Incertidumbre de la actividad
	double mda;				//Mínima Actividad Detectable del pico
	double mdaPadre;		//Mínima Actividad Detectable del padre del pico
//	double *parametrosEficiencia;//Parámetros de la función spline de bohr para la eficiencia a esta energía
								//Su dimension es espectro.dimensionMatEfi
	//
	} tPicoAjuste;


// *****************************
// *** TIPOS DE DATOS LISTAS ***
// *****************************

//NODO EMISION
//Información de cada emisión que se asocia a este pico 
typedef struct
	{
	double Energia; 	// energia asociada a la emisión
	int codElemento;	// codigo que indica la emision asociada a este pico
	char *nombre;
	double probabilidad;
	//
	double lc; //límite de decisión asociado a cada emisión
	double actividad;
	double mda;
	//
	} NodoEmision;
	
//NODO PICO
//Se centra en las características de un solo pico como tal y sus variables de ajuste
typedef struct NodoPico 
	{
	double maximo;
	bool comprobado;//NUEVO: Para contaje en la matriz de actividad
	tPicoAjuste Ajuste;
	NodoPico *siguiente;
	} NodoPico;

typedef struct tListaDatosPicos 
	{
	NodoPico *inicio;
	int numElementos;
	} tListaDatosPicos;

//NODO PICOS
//Se refiere más bien a un intervalo que puede contener uno o más picos
typedef struct NodoPicos 
	{
	double ChiSq;  	/* una vez ajustado el intervalo, esto indica su Chi cuadrado */
	double Area, sigArea;	/* esta es el area neta y su incertidumbre de este intervalo */
   	int limInferior, limSuperior; // limites para los datos de los picos
	tListaDatosPicos picos; // puntero a lista que recoge los datos de los picos
	NodoPicos *siguiente;
	} NodoPicos;

typedef struct tListaPicos 
	{
	NodoPicos *inicio;
	int numElementos;
	} tListaPicos;


//  ******************
//  *** PROTOTIPOS ***
//  ******************

extern int agnadeNodo (tListaPicos&, NodoPicos*); // agnade un nodo
					// a una lista de picos
extern void quitaNodo(tListaPicos&, int); // quita un nodo de una lista de
					// picos (incluyendo el vector interno)
extern void borraLista(tListaPicos&); // borra todos los nodos de la lista
					// de picos incluyendo los vectores internos
extern NodoPicos* DevuelveNodo (tListaPicos&, int pos = 0); // devuelve
					// el nodo que hay en la posicion indicada
extern int InicializaNodo(NodoPicos*&); // inicializa los datos del nodo.
					// devuelve -1 si no ha conseguido crearlo

extern void iniciaNodo(NodoPico *);

extern int agnadeNodo (tListaDatosPicos&, NodoPico*); // agnade un nodo
					// a una lista de picos
extern void quitaNodo(tListaDatosPicos&, int); // quita un nodo de una lista de
					// picos (incluyendo el vector interno)
extern void borraLista(tListaDatosPicos&); // borra todos los nodos de la lista
					// de picos incluyendo los vectores internos
extern NodoPico* DevuelveNodo (tListaDatosPicos&, int pos = 0); // devuelve
					// el nodo que hay en la posicion indicada
extern void clonaNodo(NodoPico*, NodoPico*); // pone los datos del nodo
					// nodo indicado en el segundo nodo
#endif
