#ifndef __AJUSTES_H__
#define __AJUSTES_H__

typedef struct NodoNodo 
	{
	int posic; // posicion del nodo
	NodoNodo *siguiente;
	} NodoNodo;


/******************************************************************************
 ******
 ******                       AJUSTES A UNA RECTA
 ******
 ******************************************************************************/

class cRecta {
   public:
      double A, B; // ajuste de recta: y = A + Bx
      double sigmaA, sigmaB, sigmaAB, Q; // sigmas y factor de calidad Q

   public:
      cRecta();
      void AjusteARecta(double *, double *, double *, unsigned short);
			// ajusta los datos disponibles a la forma de una recta
         // necesita un puntero a los datosX, datosY, sigmaY y el numero de
			// elementos
};


/******************************************************************************
 ******
 ******                       AJUSTES A UNA FORMA LINEAL
 ******
 ******************************************************************************/

typedef void (*tFuncion)(double pos);

class cAjusteLinealChi 
	{
	public:
		double *x, *y, *sigma;
		double *a;//Se iguala a SplineContinuo.peso
		double *afunc; //Se iguala a SplineContinuo.intervalo
		double *covar; //Matriz de covarianza
		//NOTA: El ajuste lineal chi es el ajuste lineal de cuadrados restantes (least-square) (?)
		/*  	- x e y son los valores de los puntos (x,y)
			- a es el vector con los parámetros de la función lineal que se usa para ajustar (?)
			- afunc función de ajuste ???->Supongo que no porque si es lineal será simplemente 1,x,x2,...
			- covar va a ser la matriz de covarianzas que reflejará las correlaciones entre los coeficientes
			  	de la función de ajuste, de forma que el elemento [i,j] indicará covar(i,j). La matriz
				será simétrica y en la diagonal tendrá las varianzas: [i,i]->var(i)
				La matriz se va a expresar como un array más así que el elemento i,j estará en covar[j*dim+i]
		*/
		NodoNodo *primerNodo;
		int numDatos; //Número de canales de todo el trozo de espectro a tratar
		int ma;//Número de intervalos de BSpline
		
		double ChiSq;//Valor de la chi cuadrado que nos sirve para saber la exactitud del ajuste. Debería aproximarse a 1.0
		
		tFuncion ActualizaVector; /* puntero a la funcion
					  ActualizaVector(double) que actualiza los datos
					  del vector que utiliza la clase cAjusteLinealChi */

   	public:
      	cAjusteLinealChi();
		int Ajusta(); /* ajusta los datos pasados y devuelve el vector a y
				 la matriz de covarianza del ajuste.
				 Devuelve:
				 	0 -> Ajuste sin problemas
					-1 -> Se quedo sin memoria
					-2 -> Datos pasados incorrectos */
		void Tendencia(double*, int*); /* calcula la tendencia de todos los
						 intervalos y las devuelve en la matriz
						 indicada junto con el numero de canales
						 de fondo que tiene cada intervalo*/
		void AjustaCovarianza(); /* ajusta la matriz de covarianza después
						del calculo interno de Ajusta */
		void AsociaFuncionActualiza(tFuncion);
	private:
		double DaValor(double); /* devuelve el valor que da la funcion que
						se ajusta segun los datos de ajuste 
						actuales */
	};


/******************************************************************************
 ******
 ******                     AJUSTES A UNA FORMA NO LINEAL
 ******
 ******************************************************************************/

typedef double (*tFuncionNL)(double pos, double* param, double* dParam);

class cAjusteChi {
	public:
		double *x, *y, *sigma, /* Datos a ajustar */
				 *a, /* parametros de ajuste */
				 *dyda, /* derivadas parciales de los parametros de ajuste */
				 *covar; /* matriz de covarianza de los parametros de ajuste */
		unsigned char *lista; /* lista de parametros a ajustar */
		int 	numDatos, /* numero de datos para el ajuste */
				ma, /* numero de parametros totales para el ajuste */
				mfit; /* numero de parametros para el ajuste */
		//NUEVO:restricciones---------------------------------------------------
		float tauInf, tauSup;
		float sigmaInf, sigmaSup;
		double *alturas;
		double *centroides;
		bool ajustarIncertidumbres;
		int picoSinRestricciones;
		//----------------------------------------------------------------------
		
		double ChiSq;
		tFuncionNL ActualizaVector; /* puntero a la funcion
						ActualizaVector(double) que actualiza los datos
						del vector que utiliza la clase cAjusteChi */

	private:
		double alamda, oChiSq;
		double *beta,
				 *da, /* vector con los cambios diferenciales de cada iteraccion */
				 *atry; /* vector con el calculo de los nuevos parametros */
		double *alpha; /* matriz de curvatura */
		
		
   public:
      cAjusteChi();
   	  cAjusteChi(double *, double *, float *, int, float *, int);
	  int Ajusta(); /* ajusta los datos pasados y devuelve el vector a y
							  la matriz de covarianza del ajuste.
							  Devuelve:
							   	 0 -> Ajuste sin problemas
								-1 -> Se quedo sin memoria
								-2 -> Datos pasados incorrectos */
	  void AsociaFuncionActualiza(tFuncionNL);
	private:
		int  mrqmin(); /* calculo interno */
		void mrqcof(double*, double*, double*); /* calculo interno */
		void AjustaCovarianza(); /* ajusta la matriz de covarianza después
						del calculo interno de Ajusta */
		double  calculaChi(double *);
//		double DaValor(double); 
						/* devuelve el valor que da la funcion que
						se ajusta segun los datos de ajuste 
						actuales */
};

#endif
