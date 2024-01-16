#ifndef __ESPECTROS_H__
#define __ESPECTROS_H__

#include <time.h>
#include "calibracion.h"
#include "continuo.h"
#include "picos.h"
#include "analisis.h"
#include "BSpline.h"

typedef struct
	{
	double energia;
	char *nombre;
	}BDBGentry;
// esta esctructura guarda los espectros necesarios para un analisis
// completo
class cEspectros  
	{
   /*************
    * ESPECTROS *
    *************/
   	public:
   	//Atributos y funciones básicas presentes desde la carga del espectro:
   
      char *nombre; // nombre identificativo del espectro
	  char *nombreFondo;//nombre del fichero de fondo utilizado
	  char *nombreEficiencia;//nombre del fichero de eficiencias utilizado
   	  //
   	  bool esFondo; //Indica si el fichero es un espectro de fondo (true) o de materia
   	  //
	
	  float tiempo;//tiempo de medida del espectro en segundos
	
   	  unsigned int *espectro; // puntero a matriz de canales de espectro
      unsigned short numCanales; // numero de canales del espectro
      unsigned short minInterv, maxInterv; /* definen el minimo y el maximo
					del intervalo de trabajo para busqueda y analisis de picos
					y continuo */
      unsigned int minTotal, maxTotal; /* indican el valor minimo y el maximo
					en cuentas del espectro cargado */

   	public:
      int CargaDatos(unsigned int*, unsigned short); /* carga el espectro
				indicado de una matriz */
      unsigned int minimo(unsigned short, unsigned short); /* da el valor minimo
				del espectro en el intervalo de canales indicado (incluye
				los extremos) */
      unsigned int maximo(unsigned short, unsigned short); /* da el valor maximo
				del espectro en el intervalo de canales indicado (incluye
				los extremos) */
      void BorraDatos();  // borra la memoria reservada, dejando el objeto vacio

   /********* **
    * CONTINUO *
    ************/
	public:
		tListaContinuoInterv serieCofo, serieCopi, serieManphe; /* listado
					contiene los intervalos con los valores asignados para
					cofo, copi y manphe */
		tListaIntervalosBSpline SplineContinuo; /* listado que contiene los
					nodos, los pesos y los valores de las splines que sirven
					para calcular el continuo */
		int nNodosSplineContinuo; /* numero de nodos que tiene la spline
					de ajuste */
		float *continuo; /* vector que recoge el valor del continuo para
					cada canal */
		float *sigmaContinuo; /* vector que recoge la desviacion estandar para
					cada canal del continuo */
		double ChiSqContinuo;
      	unsigned char *fondo; // puntero a matriz de canales de fondo
		unsigned short numCanalesFondo; /* numero de canales en la matriz de
					fondo = numCanales/8 + 1;  simplifica calculos */

	private:
		/* variables varias para el ciclo de ajuste de canales a una BSpline */
		void *ajustes;
		double *ValorX, *ValorY, *SigmaY;
		int numFondo;
		clock_t principioTiempo;
		
	public:
		int BuscaCanalesContinuo(bool); /* busca los canales
					pertenecientes al continuo utilizando las configuraciones
					indicadas en las series de intervalos. Si hay una calibracion
					de los parametros de forma, "convierte" los picos en gaussianas.
					Devuelve:
							0 -> busqueda de canales correcta
						  -1 -> error
						  -2 -> intervalos de cofo y copi no completos
						  -3 -> intervalos de cofo y copi no coincidentes
						  -4 -> memoria insuficiente
						  -5 -> continuo no encontrado
						  -6 -> picos no encontrados */
		void BorraAjusteCanalesContinuo(); /* borra las estructuras de
					datos que se obtienen como resultado de hacer un ajuste
					del continuo. */
		int PreparaAjusteCanalesContinuo(); /* prepara los datos para el
					ajuste de los canales pertenecientes al continuo a una
					BSpline.
					Devuelve:
							0 -> preparacion correcta
						  -1 -> error
						  -2 -> memoria insuficiente */
		int CicloAjusteCanalesContinuo(); /* hace un ciclo de ajuste a los
					canales pertenecientes al continuo a una BSpline. El
					proceso ha de continuarse hasta que devuelva 0.
					Devuelve:
						  >0 -> nodos añadidos en este ciclo
						   0 -> ciclo ajuste de canales correcto
						  -1 -> error
						  -2 -> memoria insuficiente */
		int calculaAncho(int, tListaNodosBSpline);//Calcula el ancho en canales de un intervaloBSpline
		int intervaloTendencia(int, double *, int *, tListaNodosBSpline);//Calcula cuál es el intervalo en el que
										//introducir el próximo nudo
		void devuelveLimitesContinuo(int, int);
		int FinalAjusteCanalesContinuo(); /* finaliza el proceso de ajuste
					de los canales pertenecientes al continuo a una BSpline.
					Libera memoria ocupada y prepara los datos.
					Devuelve:
						   0 -> final de ajuste de canales correcto
						  -1 -> error
						  -2 -> memoria insuficiente */
		void CancelaAjusteCanalesContinuo(); /* cancela el proceso de ajuste
					de los canales pertenecientes al continuo a una BSpline.
					Libera memoria ocupada.*/
		bool IsChannelContinuum(unsigned short); /* devuelve true si el canal
					indicado pertenece al continuo */
	private:
		double Filtro(unsigned short, bool); /* calcula el valor del paso del
					filtro sobre la zona indicada del espectro utilizando como
					ancho una gaussiana de anchura equivalente a la que deberian
					tener los picos en esa zona del espectro. El segundo parametro
					indica si hay una calibracion de forma. */
		void SetChannel(unsigned char*, unsigned short); /* señala el canal
					indicado como que pertenece al continuo */
		void UnsetChannel(unsigned char*, unsigned short); /* señala el canal
					indicado como que no pertenece al continuo */

   
   /*************
    *   PICOS   *
    *************/
	public:
		tListaPicos listaPicos; /* lista que contiene los intervalos donde se
					se encuentran los picos y sus datos */
		unsigned char *picos; // puntero a matriz de canales de picos
		int numPicosEncontrados; /* numero de picos encontrados */
		float *total; /* vector que recoge el valor del espectro general
					(picos + continuo) para cada canal en los interavalos
					ajustados */
		NodoPicos *intervaloAjuste; /* intervalo cuyos picos se estan ajustando */

	public:
		bool IsChannelPeaks(unsigned short); /* devuelve true si el canal
					indicado pertenece al conjunto de picos encontrados */
		int BuscaPicos(tListaDatosPicos&, bool); /* busca todos los maximos de
					los picos que se pueden encontrar en el espectro y los pone
					en la lista pasada. El segundo parametro indica si hay una
					calibracion de forma. */
		float calculaChiCuadrado(float *, int, int, int, float *);
		float calculaChiCuadrado(float *, int, int, int, float *, float *);
		//float *gridSearch(float *, int, int, int, float *,int);//Sin restricciones
		//float *gridSearch(float *, int, int, int, float *,int, float *);//Con restricciones
		void gridSearch(float *, int, int, int, float *,int, float *, float *);//Sin rest, paso por ref.
		void gridSearch(float *, int, int, int, float *,int, float *, float *, float *);//Con rest, paso por ref.
		float *quitarGaussianasNulas(float *, int, int, float);
		float *quitarGaussianasNulas(float *, int, int, int);
		int AjusteIntervaloGA(int, float *, int); 
		int AjusteIntervaloML(int, float *, int); 
		int AjusteIntervaloGAML(int, float *, int, float, float, int); //sin restricciones
//		int AjusteIntervaloGAML(int, float *, float *, int); //con restricciones
		int AjusteIntervaloGAML(int, float *, float *, int, float , float, int); //con restricciones
		int daNumIntervaloConEnergia(float);
		
		int AjusteIntervalo(int); /* ajusta los picos que se encuentran
					en el intervalo indicado.
					Devuelve:
							0 -> final de ajuste de pico correcto
						  -1 -> error
						  -2 -> memoria insuficiente */
		int AjustePico(int, int, bool, bool); /* ajusta el pico indicado que se
					encuentra en el intervalo señalado. Si el penultimo campo es
					true entonces lo inicializa para calibracion. Si el ultimo campo
					es true entonces le hace inicializacion.
					Devuelve:
							0 -> final de ajuste de pico correcto
						  -1 -> error
						  -2 -> memoria insuficiente */
		void BorraIntervalo(int); /* borra los datos del ajuste del intervalo
					indicado. */
		void ActualizaIntervaloCalibracion(int);		
		void ActualizaIntervaloAnalisis(int); 
//		void ActualizaIntervaloAnalisis(int, int); 
		/* actualiza la ChiSq y el vector total
					en el intervalo indicado. 
			Durante la calibración la información será distinta a
			como es durante el análisis
			*/
		double DaAreaPico(int, int); /* devuelve el area del pico indicado
					(el primer parametro es el intervalo y el segundo el pico).
					Si no esta ajustado devuelve 0. */
		double DaAreaPico(int, int, double); //NUEVO: Contando con el fondo
		double DaIncerAreaPico(int, int); /* devuelve la incertidumbre del 
					area del pico indicado (el primer parametro es el intervalo y
					el segundo el pico). Si no esta ajustado devuelve 0. */
		double DaAreaIntervalo(int); /* devuelve el area del intervalo
					indicado. Si no hay ningun pico ajustado en el intervalo 
					devuelve 0. */
		double DaIncerAreaIntervalo(int); /* devuelve la incertidumbre del 
					area del intervalo indicado. Si no hay ningun pico ajustado
					en el intervalo devuelve 0. */
		//NUEVO:
		float *DaVectorIntervalo(int); /* devuelve un vector con todos los valores
				del espectro sin el continuo en el intervalo*/
		int DaAnchoIntervalo(int);/* devuelve la anchura en canales de un intervalo*/
		int DaLimInferior(int);//devuelve el límite inferior del intervalo en canales
		int DaLimSuperior(int);//devuelve el límite superior del intervalo en canales
		double DaValorPico(int, int, double); /* da el valor que tienen uno de los
					picos ajustados en el intervalo señalado en el canal
					indicado */
		double DaValorPico(NodoPicos *, NodoPico *, double);
		double DaValorPico(double, double, double, int); //da el valor de gauss+exp para estos parámetros de anchura, centroide y sigma, en ese canal
		double DaValorGaussiana(double, double, double, double); //da el valor de gauss para estos parámetros de anchura, centroide y sigma, en ese canal
		double DaValorConvolucion(double, double, double, double, double); //da el valor de la convolución entre la campana de gauss y una exponencial para estos parámetros de anchura, centroide(cogemos el mismo para ambas funciones) y sigma, en ese canal
		double DaIncerConvolucion(double, double, double, double, double, double[4][4], double);
		double dpCentroide(double, double, double, double, double, double);
		double dpAmplitud(double, double);
		double dpSigma(double, double, double, double, double, double);
		double dpTau(double, double, double, double, double, double);
		double DaValorExponencial(double, double, double, double); //da el valor de la exponencial para estos parámetros de anchura, centroide y tau, en ese canal
		double DaAreaGaussiana(double, double);
		double DaAreaConvolucion(double, double, double);
		int DaTotalCuentas(int, int);
		int DaTotalCuentas();
		
		double DaValorIntervalo(int, double); /* da el valor total que tienen los
					picos ajustados en el intervalo señalado en el canal
					indicado */
		double DaValorIntervalo(NodoPicos *, double);
		

   /***************
    * CALIBRACION *
    ***************/
   public:

							/* Datos de la calibracion */
      double afwhg, bfwhg; /* calibracion de la anchura de la forma
      		Raiz(afwhg + bfwhg*canal) */
      double enp1, enp2, enp3; /* calibracion de la energia de la forma
      				canal = forma cuadratica de la energia: enp3*x²+enp2*x+enp1 */
   	  double enp4; //Cuarto parámetro para mejorar la calibración de la energía: enp4*1/x
	  double atau, btau; /* calibracion de la anchura de la primera
				exponencial de la forma Raiz(atau + btau*canal) */
   	  double ares, bres; //calibración de la resolución: raiz(ares+bres*canal)
//	  double covarEnergia[16];//Matriz de covarianza para la energia
  	  double covarEnergia[25];//Matriz de covarianza para la energia
   	  double covarSigma[9], covarTau[9], covarRes[9]; /* matrices de
				covarianza para el ajuste de comportamiento de la energia, sigma
				y tau frente al canal */
      double vectorCuadratica[4], vectorRecta[3]; /* vectores para la
				calibracion de formas cuadraticas y rectas */
   	
   	//
   	  double vectorEnergia[5]; //Vector para la mejora de la calibración de la energía
   	//

	  double chiEnergia, chiSigma, chiTau; //Errores de la calibración de la 
	  										//energía, la sigma y la tau
								
	/* Datos del ajuste */
	  double param[4], dParam[4], covarParam[16];

		
   public:
		void InicializaPico(tPicoAjuste&, double, bool); /* inicializa el pico
				pasado utilizando una inicializacion sin calibracion a base
				de conjeturas (si el ultimo parametro es false) o con la
				calibracion actual (si el ultimo parametro es true). El segundo 
				parametro es el canal donde esta el maximo (sin estimar) */
      int PreCalibra(tPrecalibracion *, unsigned char); /* precalibra la
				anchura (FWHG) frente al canal del maximo mediante la funcion
				FWHG**2 = a + b*max; 
				Precalibra la relacion canal-energia
				El segundo parametro indica el numero de picos utilizados para la
				precalibracion*/
		int CalibraEnergia(tListaCalibracion); /* calibra la energia frente al
				canal utilizando los picos calibrados calibrados de la lista pasada.
				Devuelve: 
							0 -> final de calibracion correcto
						  -1 -> error
						  -2 -> memoria insuficiente */
		int CalibraFWHG(tListaCalibracion); /* calibra la FWHM frente al
				canal utilizando los picos calibrados de la lista pasada.
				Devuelve: 
							0 -> final de calibracion correcto
						  -1 -> error
						  -2 -> memoria insuficiente 
				Modifica: afwhg, bfwhg -> coeficientes de la recta de calibración
				*/
		int CalibraTau(tListaCalibracion); /* calibra la Tau frente al
				canal utilizando los picos calibrados de la lista pasada.
				Devuelve: 
							0 -> final de calibracion correcto
						  -1 -> error
						  -2 -> memoria insuficiente 
				Modifica: atau, btau -> coeficientes de la recta de calibración*/
		int CalibraResolucion(tListaCalibracion lista); /* calibra la resolución frente al
				canal utilizando los picos calibrados de la lista pasada.
				Devuelve: 
						   0 -> final de calibracion correcto
						  -1 -> error
						  -2 -> memoria insuficiente 
						  
				Modifica: ares, bres -> coeficientes de la recta de calibración
				*/
		
		double DaEnergia(double); /* Devuelve la energia correspondiente al
				canal indicado. Si no hay precalibracion o calibracion, devuelve
				cero. */
		double DaCanal(double); /* Devuelve el canal correspondiente a la
				energia indicada. Si no hay precalibracion o calibracion, devuelve
				cero. */
		double DaFWHG(double); /* Devuelve el FWHG correspondiente al
				canal indicado. Si no hay precalibracion o calibracion, devuelve
				cero. */
		double DaFWHM(int);//Devuelve el nº de canales que hay por 
				//encima de la mitad del máximo en el intervalo
		double DaFWTM(int);//Devuelve el nº de canales que hay por 
				//encima de 1/10 del máximo en el intervalo
		double DaFWFM(int);//Devuelve el nº de canales que hay por 
				//encima de 1/50 del máximo en el intervalo
		double DaFWHGCh(double); /* Devuelve el FWHG correspondiente al
				canal indicado en canales. Si no hay precalibracion o calibracion, devuelve
				cero. */
		double DaResolucion(double); /* Devuelve la resolución correspondiente al
				canal indicado. Si no hay precalibracion o calibracion, devuelve
				cero. */
		double DaResolucionCh(double); /* Devuelve la resolución correspondiente al
				canal indicado en canales. Si no hay precalibracion o calibracion, devuelve
				cero. */
		double DaSigma(double); /* Devuelve la sigma correspondiente al
				canal indicado. Si no hay precalibracion o calibracion, devuelve
				cero. */
		double DaSigmaCh(double); /* Devuelve la sigma correspondiente al
				canal indicado en canales. Si no hay precalibracion o calibracion,
				devuelve cero. */
		double DaTau(double); /* Devuelve el tau correspondiente al
				canal indicado. Si no hay calibracion, devuelve cero. */
		double DaTauCh(double); /* Devuelve el tau correspondiente al
				canal indicado en canales. Si no hay calibracion, devuelve cero. */
		double DaAncho(double, double); /* Devuelve en energia el ancho pasado
				en canales correspondiente al canal central indicado en el primer
				parametros. Si no hay precalibracon o calibracion, devuelve cero. */
		
									/* INCERTIDUMBRES */
		double DaIncerEnergia(double);
		double DaIncerFWHG(double);
		double DaIncerFWHGCh(double); /* en canales */
		double DaIncerSigma(double);
		double DaIncerSigmaCh(double); /* en canales */
		double DaIncerTau(double);
		double DaIncerTauCh(double); /* en canales */
		//
		double DaIncerResolucion(double);
		double DaIncerResolucionCh(double); /* en canales */

	/***********
	* ANÁLISIS *
	***********/
	
   public:
//	    void  calculaLC(int, double *, double *, double *);
  	    void  calculaLC();
   	    void  calculaLC(int);
   	    void  calculaLC(float, float*, float*);
   	    void  calculaLD();
   	    float calculaLD(float);
	    void  calculaLD(nodoCentroide *);
   	    void  calculaLD(char *, float *, int*, int*, int*);
	    void  calculaAMD(int , float, float, float *);
   	    void  calculaEficiencia(int, double *, double *, double **);
	    void  bohr(double, double *, int, double *, int);
   	    void  calculaCorreccionesEficiencia(double *, double *, double *, double);
		LinkedList *calculaMatrizCovarianzaEficiencia();
		double **calculaMatrizCovarianzaEficiencia(int);
   
   		bool calibracionCorrecta(int, int);
   		//ordenación de centroides
		void ordenaCentroides(double *, int);
		void ordenaVector(float *, int);

   /**************
    * EFICIENCIA *
    **************/
   		int dimensionMatEfi;
		double **matrizEficiencia;
		double *nodosEficiencia;
		double *parametrosEficiencia;
   		LinkedList *matricesActividad;
   
   		double daEficiencia(double);
		double daSigmaEficiencia(double);
   
   /***********
    ** FONDO **
    ***********/
		float *cuentasFondo;
		float *sigmaCuentasFondo;
		double *mediasFondo;
		double *incertidumbresFondo;
		double *energiasFondo;
		int numFondos;
		void calculaFondo(double *, double *, double *, int);
		void restaAreaFondo();
		void determinarFondos(float *, int *, int);
		int determinarNumFondos(float *, int);
		void restriccionesFondo(float *, int *, float *, int, int);
		
   /***********
    * GENERAL *
    ***********/
   public:
      cEspectros();
      ~cEspectros();
      
};

#endif
