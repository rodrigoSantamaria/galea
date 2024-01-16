#ifndef __CONTINUO_H__
#define __CONTINUO_H__

// *** TIPOS DE DATOS ***

typedef struct NodoContinuo {
   int limInferior, limSuperior; // limites para los datos del continuo
   double cofo, copi, manphe; // datos asociados a ese intervalo
	NodoContinuo *siguiente;
} NodoContinuo;

typedef struct tListaContinuo {
	NodoContinuo *inicio;
	int numElementos;
} tListaContinuo;

typedef struct NodoContinuoInterv {
   int limInferior, limSuperior; // limites para los datos del continuo
   double valor; // datos asociados a ese intervalo
	NodoContinuoInterv *siguiente;
} NodoContinuoInterv;

typedef struct tListaContinuoInterv {
	NodoContinuoInterv *inicio;
	int numElementos;
	NodoContinuoInterv *UltimaConsulta;
} tListaContinuoInterv;



//  *** PROTOTIPOS ***

extern NodoContinuo* DevuelveNodo (tListaContinuo&, int pos = 0); // devuelve
					// el nodo que hay en la posicion indicada
extern int agnadeNodo (tListaContinuo&, NodoContinuo*); // agnade un nodo
					// a una lista de datos del continuo
extern void quitaNodo(tListaContinuo&, int); // quita un nodo de una lista de
					// datos del continuo
extern void borraLista(tListaContinuo&); // borra todos los nodos de la lista
					// de datos del continuo
extern void borraLista(tListaContinuoInterv&); // borra todos los nodos de la
					// lista de datos de series de intervalos

extern int construyeLista (tListaContinuo, tListaContinuoInterv&, const int); // construye
					// una lista en formato intervalos partiendo de una lista
					// del continuo. El dato que se pasa es el indicado en el tercer
					// parametro:
					// 		1 -> Cofo
					// 		2 -> Copi
					// 		3 -> Manphe
					// Devuelve:
					// 		0  -> conseguido y lista completa
					// 		1  -> conseguido pero lista de completa
					// 	  -2  -> Fallo de memoria
					// 	  -3  -> tipo de dato no conocido
extern bool completitudListaInterv (tListaContinuoInterv); // indica si la lista de
					// intervalos pasada es completa (es decir, entre el menor limite
					// inferior y el mayor limite superior estan cubiertos todos los
					// intervalos
float DevuelveValorListaInterv(tListaContinuoInterv&, int); // devuelve el valor que
					// almacenado en la lista para la posicion indicada en el segundo 
					// parametro. Si no hay valor guardado se devuelve -1.
int DevuelveAlcanceListaInterv(tListaContinuoInterv, int&, int&); // devuelve la
					// posicion menor y mayor de todos los intervalos.
					// Devuelve:
					// 	   0  -> accion realizada
					// 	  -1  -> lista vacia
					// 	  -2  -> lista desordenada

#endif
