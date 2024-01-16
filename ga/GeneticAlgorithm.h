/*		GENETIC ALGORITHM
18-X
Esta clase implementa un algoritmo genético con sus distintos 
atributos y operaciones. En concreto va a contener:
	Un vector con todos los parámetros de cada individuo de la población
	Métodos para la recombinación y la mutación de la población
26-X Añadimos los primeros atributos y métodos a esta clase
*/
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

//Para manejo de ficheros
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

#ifndef LINKEDLIST_H
#include "LinkedList.h"
#endif

#include "../espectros.h"
#include "../VisGraf.h"

#include "../picos.h"
#include "../Graficas.h"

#ifndef INDIVIDUO_H
#include "Individuo.h"
#endif

class GeneticAlgorithm
	{
	protected:
		//Propiedades genéticas
		int generacion; //Número de generación en la que estamos
		int numeroPoblacion;//Nº de individuos de la población
		int numeroPadres;   //Nº de padres que se seleccionarán para generar la siguiente población
		int numeroMutaciones;//Nº de nuevos individuos que sufrirán mutaciones
		float factorMutante;  //Factor mutante.
		Individuo **poblacion;	//Cadenas de punteros a individuos
		Individuo **nextGeneration;
		Individuo **nueva;
	
		//Propiedades características de nuestro problema
		float *y; //Función que se desea ajustar
		int numeroCanales;//Número de canales que tiene la función a ajustar
		cEspectros *espectro;//Datos del espectro en cuestión
		int numParametros;//Nº de gaussianas que ajusta cada individuo
		int limInferior;//Canal en el que empieza el intervalo
		float maxAjuste;
	
	public:	
		//Constructores 
		GeneticAlgorithm();
		GeneticAlgorithm(int,int,int,float, int, float *, int, cEspectros *, int);
		~GeneticAlgorithm();
		//Funciones GET
		int getNumeroPoblacion();
		int getNumeroPadres();
		int getNumeroMutaciones();
		float getFactorMutante();
		Individuo **getPoblacion();
		Individuo *getIndividuo(int);
		float *getY();
		float getY(int);
		int getNumeroCanales();
		int getNumParametros();
		float *getParametros();
		int getLimInferior();
		cEspectros *getEspectro();
	
		//
		float *getFuncion(int);
		//
	
		//Funciones SET
		void setNumeroPoblacion(int);
		void setNumeroPadres(int);
		void setNumeroMutaciones(int);
		void setFactorMutante(float);
		void setPoblacion(LinkedList *);
		void setPoblacion(float *);
		void setY(float *);
		void setY(float, int);
		
		//Funciones de depuración
		int main();	
		void printPoblacion();
		void printAjustes();
		void dibujaGenetic(Individuo *);
	
	};
