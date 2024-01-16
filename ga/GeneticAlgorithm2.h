/*		GENETIC ALGORITHM II
26-XI
	Esta clase es hija de GeneticAlgorithm, añadiendo algunos parámetros
	no contemplados por ésta.
	En particular añade todo lo correspondiente a las convoluciones con
	exponenciales: dos parámetros más, que como sigma no variarán con la energía
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

#include "GeneticAlgorithm.h"

class GeneticAlgorithm2: public GeneticAlgorithm 	
	{
	private: //Añadimos restricciones al valor de tau y sigma
		float liTau, lsTau;
		float liSigma, lsSigma;
		float *restricciones;//Restricciones al área de cada gaussiana
		float *centroidesIniciales; //centroides iniciales
		int *progenitores;
		int elitismo;
		bool restringirCentroides;
	
	public:
		GeneticAlgorithm2();
		GeneticAlgorithm2(int,int,int,float, int, float *, int, cEspectros *, int);
		~GeneticAlgorithm2();
		GeneticAlgorithm2(int,int,int,float,GeneticAlgorithm *, float *);
		GeneticAlgorithm2(int,int,int,float,int, float *,int, cEspectros *,int,float *,float *,float *);
//		GeneticAlgorithm2(int,int,int,float,int, float *,int, cEspectros *,int,float *,float ,float , float *);
		GeneticAlgorithm2(int,int,int,float,int, float *,int, cEspectros *,int,float *,float ,float , float *, int);
	
		void mutar();
		void procrear();
		void ejecutar(int, float);
		void calculaAjuste();
		void ordenar();
		void imprimeGeneracion(Individuo **);
		int *seleccionar();
		float *getFuncion(int k);
		void  getParametros(float *);
		float getMejorAjuste();
		float getAjusteMedio();

		float getPresionSelectiva();
		float getSumaAjustes();
		float getScore(int);
		float getScoreMedio();
		float getSumaScores();
		float getMejorScore();
		float getAjuste(int);
		float getDiversidad();
		float getDivergencia();
		
		void liberarMemoria();
	};
