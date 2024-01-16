/*			INDIVIDUO 
26-X Representa un individuo de la población para el algoritmo genético
	El algoritmo genético puede incluir cualquier tipo de individuo.
	Esta clase implementa el caso concreto de un individuo de 37 parámetros
	que intenta solucionar la detección de elementos naturales en el multiplete
	de energías bajas.
	Estos 37 parámetros son:
		c1...c18 Centroides propuestos para los 18 picos considerados en el multiplete
		a1...a18 Amplitudes propuestas para los 18 picos considerados en el multiplete
		sigma	 Considerada la misma para los 18 picos.
	Las razones para considerar 18 picos se pueden encontrar en la tesis de Marta;
	y se basan principalmente en experimentos empíricos
*/
#ifndef INDIVIDUO_H
#define INDIVIDUO_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

//Para manejo de ficheros
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

#define PARAMS 17


class Individuo
	{
	protected:
		//************** GENOMA ******************
		//Cada uno de los parámetros o cromosomas del individuo, separados
		//según su tipo (su evolución no discrimina por tipo)
		int numeroParametros;//indica el número de centroides que hay.
							//Habrá el mismo número de amplitudes, con lo cual la cantidad de 
							//material genómico será 2*numeroParametros+1
		float sigma;	  //Valor de sigma.
		float tau;		//Valor de tau
	
		float ajuste;  //Grado de calidad del genoma de este individuo
	public:
		float *centroide; //Centroides de todos los picos considerados
		float *amplitud;  //Amplitudes de todos los picos considerados

		Individuo();
		Individuo(int);
		Individuo(int, float*,float*,float);
		Individuo(int, float*,float*,float, float);
		~Individuo();
	
		//Funciones de obtención de parámetros
		float *getCentroides();
		float getCentroide(int);
		float *getAmplitudes();
		float getAmplitud(int);
		float getSigma();
		float getTau();
		float getAjuste();
		int	  getNumeroParametros();

		//Funciones de escritura de parámetros
		void setCentroides(float *);
		void setCentroide(int, float);
		void setAmplitudes(float *);
		void setAmplitud(int, float);
		void setSigma(float);
		void setAjuste(float);
		void setNumeroParametros(int);
		void setTau(float);
		
		void liberarMemoria();
	};

#endif //INDIVIDUO_H
