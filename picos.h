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
	double covar[4][4];//NUEVO: Contiene las relaciones de covarianza entre los 4 par�metros
			//actuales del ajuste, aparte de que algunos se encuentren en las sig anteriores
		
	// Datos administracion
	bool inicializado;// indica si ya ha sido inicializado este pico
	bool ajustado; 	  // indica que este pico ya ha sido ajustado y que los
					// datos que hay en el ajuste son validos
	bool calibrado;	  // indica si el pico tiene asignada una energia de emision durante calibraci�n
	bool identificado;// indica si la energia de emision ha sido identificada durante an�lisis o calibraci�n
	//NOTA: En construcci�n. No se guarda como el resto de momento
	bool invisible;//indica si un pico, por la raz�n que sea, no debe salir dibujado
					//de momento se est� usando para los picos que se usan para determinar equilibrios
	//
	//bool identificadoEnCalibracion;// indica si la energia de emision ha sido identificada durante la calibraci�n
	
	bool eficienciaCalculada;//Indica si el pico tiene una eficiencia calcualda
	bool ldCalculado;//indica si el pico tiene unos l�mites de detecci�n calculados
	bool analizado;  //indica si el pico tiene asignados valores a la actividad,
					 //el �rea restado un cierto fondo, y los factores de correcci�n
					 //NOTA: Ojo, ahora est� indicando que tengamos una emisi�n identificada en an�lisis, para
					 //		distinguir de las emisiones identificadas en calibraci�n
	// Datos de identificacion
	LinkedList *emisiones;//Lista enlazada con todos los elementos asociados a este pico
	
	double ld; 				//L�mite de detecci�n para este pico
	double lc; 				//L�mite de decisi�n para este pico
	double eficiencia;		//Eficiencia para este pico
	double sigmaEficiencia;	//Varianza de la eficiencia para este pico
	double correccion;		//Correcci�n de la eficiencia
	double sigmaCorreccion;	//Incertidumbre de la correcci�n de la eficiencia
	double area;			//�rea del pico
	double incerArea;		//Incertidumbre del �rea del pico
	double areaSinFondo;	//�rea del pico sin contar el fondo del detector
	double incerAreaSinFondo;//Incertidumbre del �rea del pico sin contar la del fondo del detector (?)dpb
	double actividad;		//Actividad del pico
	double sigmaActividad;	//Incertidumbre de la actividad
	double mda;				//M�nima Actividad Detectable del pico
	double mdaPadre;		//M�nima Actividad Detectable del padre del pico
//	double *parametrosEficiencia;//Par�metros de la funci�n spline de bohr para la eficiencia a esta energ�a
								//Su dimension es espectro.dimensionMatEfi
	//
	} tPicoAjuste;


// *****************************
// *** TIPOS DE DATOS LISTAS ***
// *****************************

//NODO EMISION
//Informaci�n de cada emisi�n que se asocia a este pico 
typedef struct
	{
	double Energia; 	// energia asociada a la emisi�n
	int codElemento;	// codigo que indica la emision asociada a este pico
	char *nombre;
	double probabilidad;
	//
	double lc; //l�mite de decisi�n asociado a cada emisi�n
	double actividad;
	double mda;
	//
	} NodoEmision;
	
//NODO PICO
//Se centra en las caracter�sticas de un solo pico como tal y sus variables de ajuste
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
//Se refiere m�s bien a un intervalo que puede contener uno o m�s picos
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
