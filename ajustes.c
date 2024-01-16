#include <stdio.h>
#include <math.h>
#include "ajustes.h"
#include "math1.h"

// estas funciones sirven de apoyo
int gaussj(double*, int, double*);	
int gaussj2(double*, int, int, double*);

//******************* CRECTA ********************
//Constructor por defecto. Pone todos los atributos a cero
cRecta::cRecta()
	{
   	A = 0.0; B = 0.0;
   	sigmaA = 0.0; sigmaB = 0.0; sigmaAB = 0.0; Q = 0.0;
	}

//*************** AJUSTE A RECTA *************************
//Para unos determinados puntos (dx,dy), ajusta a la recta que más se aproxima a todos ellos
//NOTA: Esta puede ser una de las partes responsables de la mala precalibración, en
//		el cálculo de A y B
void cRecta::AjusteARecta(double *dX, double *dY, double *sig, unsigned short numE)
	{
   	double sxoss, t, wt;
   	double sx = 0.0, sy = 0.0, ss = 0.0; //Sumas de los pesos de los puntos
   	double st2 = 0.0;
   
   	B = 0.0;
   	// acumula sumas con pesos
   	for (unsigned short i = 0; i < numE; i++) 
	   	{
	    wt = 1.0/(sig[i]*sig[i]);
     	ss += wt;
     	sx += dX[i]*wt;
      	sy += dY[i]*wt;
   		}
   	sxoss = sx/ss;
   
   	//NOTA: ¿Cálculo de la desviación típica?
   	for (unsigned short i = 0; i < numE; i++) 
	   	{
      	t = (dX[i] - sxoss)/sig[i];
      	st2 += t*t;
      	B += t*dY[i]/sig[i];
   	   	}
   	// resuelve A, B, sigmaA, sigmaB
   	B /= st2;
   	A = (sy - sx*B)/ss;
   	sigmaA = sqrt((1.0 + sx*sx/(ss*st2))/ss);
   	sigmaB = sqrt(1.0/st2);
   	sigmaAB = 0.0;
   	for (unsigned short i = 0; i < numE; i++)
      sigmaAB += pow(((dY[i] - A - B*dX[i])/sig[i]), 2.0);
   	Q = gammq(0.5*(numE-2), 0.5*sigmaAB);
	}


/********************************************************
   	***   cAjusteLinealChi   ***
 ********************************************************/
 //Constructor por defecto
cAjusteLinealChi::cAjusteLinealChi() 
	{
	x = NULL; y = NULL; sigma = NULL; a = NULL; afunc = NULL; covar = NULL;
	numDatos = 0; ma = 0;
	ChiSq = 0.0;
	}

//*********************** AJUSTA *******************************
// Realiza el ajuste lineal chi.
int cAjusteLinealChi::Ajusta() 
	{
	// crea el vector beta
	double *beta = new double[ma+1];
	if (beta == NULL) return -1;
	
	// inicializa la matriz de covarianza y el vector beta a cero
	for (int j = 1; j <= ma; j++) 
		{
		for (int k = 1; k <= ma; k++)	covar[k+j*(ma+1)] = 0.0; //covar es una matriz hecha en plan array
		beta[j] = 0.0;
		}
			
	// acumula coeficientes para las ecuaciones
	for (int i = 0; i < numDatos; i++) 
		{
		ActualizaVector(x[i]);
		double ym = y[i];
		double sig2i = 1.0/(sigma[i]*sigma[i]);
		for (int l = 1; l <= ma; l++)
			{
			//NOTA: Este if es cosa mía. Mejora mucho el rendimiento evitarnos todas estas iteraciones
			//	cuando afunc=0, pues en ese caso wt=0, con lo que tanto beta como covar no van a 
			//	variar al sumarles cero. Este bucle segundo es el que le hace ir ralentizándose 
			//	cuantos más knots añadimos.
			//	Realizando varias pruebas de tiempos, en general este if nos da una mejora de un 50%!
			if(afunc[l]!=0)
				{
				double wt = afunc[l]*sig2i;
				for (int m = 1; m <= l; m++)	covar[m+l*(ma+1)] += wt*afunc[m];
				beta[l] += ym*wt;
				}
			}
		}
	// rellena la parte superior de la diagonal para la simetria
	for (int j = 2; j <= ma; j++)
		for (int k = 1; k <= j-1; k++)
			covar[j+k*(ma+1)] = covar[k+j*(ma+1)];

	// resuelve la matriz
	int nError = gaussj(covar, ma, beta);
	if (nError < 0) 
		{
		printf("ERROR gaussj!-------------->%d\n", nError);
		delete [] beta;
		return nError;
		}
//	printf("copiamos beta a a \n");
	// pone la solucion en el vector a
	//NOTA: Por qué no lo ha hecho todo directamente sobre a??
	for (int j = 1; j <= ma; j++)	a[j] = beta[j];
	
	// evalua Chi al cuadrado
	ChiSq = 0.0;
	for (int i = 0; i < numDatos-1; i++) 	ChiSq += powi(((y[i]-DaValor(x[i]))/sigma[i]), 2);//NOTA: El cuadrado se puede poner como multiplicación que tarda menos
	//printf("X²=%f\n", ChiSq);
	//AjustaCovarianza();
	delete [] beta;
	return 0;
	}

//********************** AJUSTA COVARIANZA *********************
// NOTA: Preguntar a Begoña acerca de qué tiene que hacer este ajuste porque
//	ahora mismo no hace absolutamente nada
//	Supongo que bien j o k deberían empezar en 0 e incrementarse
void cAjusteLinealChi::AjustaCovarianza() 
	{
//	printf("AJUSTA COVARIANZA\n");
	double swap;
	int k = ma;
	for (int j = ma; j >= 1; j--) 
		{
		for (int i = 1; i <= ma; i++) 
			{
			//NOTA: Teniendo en cuenta que j y k varían al mismo ritmo esto no parece
			//	tener ningún sentido: cambiamos un valor por sí mismo
//			if(covar[k+i*(ma+1)] != covar[j+i*(ma+1)])	printf("%f\t%f\n", covar[k+i*(ma+1)], covar[j+i*(ma+1)] );
//			printf("Cambiamos %d por %d\n", (int)(k+i*(ma+1)), (int)(j+i*(ma+1)));
			swap = covar[k+i*(ma+1)];
			covar[k+i*(ma+1)] = covar[j+i*(ma+1)];
			covar[j+i*(ma+1)] = swap;
			}
		for (int i = 1; i <= ma; i++) 
			{
//			if(covar[i+k*(ma+1)] != covar[i+j*(ma+1)])	printf("%f\t%f\n", covar[i+k*(ma+1)], covar[i+j*(ma+1)] );
//			printf("Cambiamos %d por %d\n", (int)(k+i*(ma+1)), (int)(j+i*(ma+1)));
			swap = covar[i+k*(ma+1)];
			covar[i+k*(ma+1)] = covar[i+j*(ma+1)];
			covar[i+j*(ma+1)] = swap;
			}
		k--;
		}
	}

//************************* GAUSSJ **************************
// NOTA: Cuando se llama desde ajusta se hace con a=covar (matriz de ma x ma), n=ma (que es el número de knots) y b=beta 
//	 Hace modificaciones en a y b en base a características de gauss (no sé qué quiere decir la j)
//	Exactamente no sé dónde se basa para estas operaciones
//	NOTA: OJO, aquí a es la matriz de covarianza, no la matriz de parámetros
int gaussj(double* a, int n, double* b) 
	{
	int *ipiv  = new int[n+1];
	int *indxc = new int[n+1];
	int *indxr = new int[n+1];
	if ((ipiv == NULL)||(indxc == NULL)||(indxr == NULL)) 
		{
		delete [] ipiv; delete [] indxc; delete [] indxr;
		return -1;
		}
	// inicializa ipiv a 0
	for (int j = 1; j <= n; j++)	ipiv[j] = 0;
	int irow, icol;
	for (int i = 1; i <= n; i++) 
		{
		double big = 0.0;
		for (int j = 1; j <= n; j++) 
			{//Buscamos el valor más alto de la matriz de covarianza, y su fila y columna
			if (ipiv[j] != 1) 
				{
				for (int k = 1; k <= n; k++) 
					{
					if (ipiv[k] == 0) 
						{
						if (fabs(a[k+j*(n+1)]) >= big) 
							{
							big = fabs(a[k+j*(n+1)]);
							irow = j;
							icol = k;
							}
						}
					else if (ipiv[k] > 1) 
						{
						printf("ERROR: ipiv[%d]=%f>1\n", k, ipiv[k]);
						delete [] ipiv; delete [] indxc; delete [] indxr;
						return -2;
						}
					}
				}
			}
		ipiv[icol]++;//incrementamos el valor de ipiv que está en la columna del máximo de covarianza
		if (irow != icol) //Si la fila != columna, cambiamos ambos valores en a y b
			{
			for (int l = 1; l <= n; l++) 
				{
				double swap = a[l + irow*(n+1)];
				a[l + irow*(n+1)] = a[l + icol*(n+1)];
				a[l + icol*(n+1)] = swap;
				}
			double swap = b[irow];
			b[irow] = b[icol];
			b[icol] = swap;
			}
		indxr[i] = irow;
		indxc[i] = icol;
		if (a[icol+icol*(n+1)] == 0.0) 
			{
			printf("ERROR: a[%d+%d*(%d+1)] es cero\n", icol, icol, n);
			delete [] ipiv; delete [] indxc; delete [] indxr;
			return -2;
			}
		double pivinv = 1.0/a[icol+icol*(n+1)];
		a[icol+icol*(n+1)] = 1.0;
		for (int l = 1; l <= n; l++)
			a[l + icol*(n+1)] *= pivinv;
		b[icol] *= pivinv;
		for (int ll = 1; ll <= n; ll++)
			if (ll != icol) 
				{
				double dum = a[icol + ll*(n+1)];
				a[icol + ll*(n+1)] = 0.0;
				for (int l = 1; l <= n; l++)
					a[l+ll*(n+1)] -= (a[l+icol*(n+1)]*dum);
				b[ll] -= (b[icol]*dum);
				}
		}
	
		
	for (int l = n; l >= 1; l--)
		if (indxr[l] != indxc[l])
			for (int k = 1; k <= n; k++) 
				{
				double dum = a[indxr[l]+k*(n+1)];
				a[indxr[l]+k*(n+1)] = a[indxc[l]+k*(n+1)];
				a[indxc[l]+k*(n+1)] = dum;
				}

	return 0;
	}

//******************************* TENDENCIA **************************
// Calcula las tendencias de cada intervalo BSpline, almacenándolas en el array 
// tendencias. NumCanales es un array que contiene las posiciones de los knots (?)
void cAjusteLinealChi::Tendencia(double *tendencias, int *numCanales) 
	{
	NodoNodo *puntero = primerNodo;
	int intervActual = 1;
	double tendencia;

	int j = 1; int k = 1;

	while (intervActual < ma-1) //Recorremos todos los intervalos, calculando la tendencia de cada uno
		{
		tendencia = 0.0;

		// recoge los extremos del intervalo
		int extIzq = puntero->posic;
		int extDer = puntero->siguiente->posic;

		// optimizacion: los intervalos van en secuencia ordenada
		// por lo que la busqueda del primer canal se puede
		// empezar desde el ultimo canal que pertenecia al otro
		// intervalo

		// busca el primer canal de fondo que esta en el intervalo
		j = k;
		while (redondea((double) x[j]) < extIzq) j++;//x contiene los canales que son del fondo, basta con buscar el primero que está en el intervalo

		// comprueba si este intervalo tiene al menos un punto
		if ((redondea((double) x[j])) < extDer) 
			{
			// busca el ultimo canal de fondo que esta en el intervalo
			k = j;
			while (redondea((double) x[k+1]) < extDer) k++;

			// calcula los valores S y R del intervalo de puntos
			double S = 0.0; double R = 0.0;
			for (int i = j; i <= k; i++) 
				{
				//  *** NUEVO ***
				// evalua la funcion en cada canal de fondo
				// pero solo si son dos canales contiguos
//				if (redondea(x[i+1]-x[i]) == 1) {
				//NOTA: Con respecto al papel que trata sobre esto, R y S están cambiados
				//	(S corresponde a R y R a S)
				//	RS corresponde a ri. RS1 debería ser ri-1 pero es ri+1!
					double RS = (y[i] - DaValor(x[i]))/sigma[i];
//					double RS1 = (y[i+1] - DaValor(x[i+1]))/sigma[i+1];
					double RS1 = (y[i-1] - DaValor(x[i-1]))/sigma[i-1];
					S += RS*RS1;
					R += RS*RS;
//				}
				}
			// calcula la tendencia
			if (R != 0.0)	tendencia = sqrt(k-j+1)*S/R;
			else tendencia = 0.0;
			numCanales[intervActual-1] = k - j + 1;
			}
		else	numCanales[intervActual-1] = 0;
		// guarda la tendencia calculada en la matriz
		tendencias[intervActual-1] = tendencia;

		// pasa al siguiente intervalo
		puntero = puntero->siguiente;
		intervActual++;
		j = k;
		}
	}

//******************** DA VALOR ***********************
//Devuelve el valor asociado a la posición pos.
double cAjusteLinealChi::DaValor(double pos) {
	ActualizaVector(pos);//Devuelve el valor de la BSpline correspondiente para esta posición 
				//(es una llamada a la función CalculaIntervalosBSpline de BSpline.c)
	double temp = 0.0;
	for (int i = 1; i <= ma; i++)
		temp += a[i]*afunc[i];
	return temp;
}

void cAjusteLinealChi::AsociaFuncionActualiza(tFuncion funcion) {
	ActualizaVector = funcion;
}



/* ****************************
   ***							 ***
	***      cAjusteChi      ***
   ***					 		 ***
	**************************** */



cAjusteChi::cAjusteChi() 
	{
	x = NULL; y = NULL; sigma = NULL; a = NULL; alpha = NULL; covar = NULL;
	numDatos = 0; ma = 0; mfit = 0;
	ChiSq = 0.0;
	tauInf=tauSup=sigmaInf=sigmaSup=0.0;
	alturas = NULL;
	ajustarIncertidumbres=false;
	picoSinRestricciones=-1;
	}

//********************** CONSTRUCTOR PARA AJUSTE CONCRETO ****************
/*	Inicializa la clase con la función a ajustar y los parámetros de ajuste:
	x, y, sigmaY: valores de la función a ajustar. numCanales es el número de puntos (longitud de esos valores)
	a, numParametros: parámetros y longitud que se pueden ajustar.
	lista, numLista: parámetros y longitud de los que vamos a ajustar del total.
	*/
cAjusteChi::cAjusteChi(double *xNuevo, double *sigmaYNuevo, float *yNuevo, int numCanales, float *aNuevo, int numParams)
	{
	x=xNuevo;
	sigma=sigmaYNuevo;
	y=(double *)yNuevo;
	numDatos=numCanales;
	a=(double *)aNuevo;
	ma=numParams;
	lista=new unsigned char[ma];
	for(int i=0;i<ma;i++)	
		{
		lista[i]=(char)(i+1);
		}
	alamda=-1;
	ChiSq=0.0;
	ajustarIncertidumbres=false;
	picoSinRestricciones=-1;
	}
	

void cAjusteChi::AsociaFuncionActualiza(tFuncionNL funcion) 
	{
	ActualizaVector = funcion;
	}

//*********************** AJUSTA ********************************

int cAjusteChi::Ajusta() 
	{
//	printf("AJUSTA\n");
//	for(int i=0;i<ma;i++)	printf("a[%d]=%f\n", i, a[i]);
	// crea las matrices para el calculo interno
	beta = new double[ma]; alpha = new double[ma*ma];
	atry = new double[ma]; da = new double[ma];
	if ((beta == NULL)||(alpha == NULL)||(da == NULL)||(atry == NULL)) 
		{
		delete [] beta; delete [] alpha; delete [] atry; delete [] da;
		return -1;
		}
	// TODO: realiza los calculos hasta que se obtiene una buena aproximacion
	//	De momento hace 300 iteraciones, haya llegado antes a una buena aproximación
	//	o no haya llegado al finalizar las 300.
//	printf("SigmaInf %f sigmaSup %f TauInf %f TauSup: %f\n", sigmaInf, sigmaSup, tauInf, tauSup);
	alamda = -1.0;
//	printf("Chi inicial: %f\n", ChiSq);
	bool bParar = false; int i = 0; int bajadas = 0;
	do 	{
//		printf("+++ ITERACIÓN %d +++\n", i);
		double OldChiSq = ChiSq;
		
	//	printf("alamda=%e\n", alamda);
		double oldAlamda = alamda;
		int nError = mrqmin();
//		printf("Volvemos con error: %d\n", nError);
		if(nError==123)
			{
//			printf("El error es mayor que el que teníamos en la anterior iteración o que la entrada inicial\n");
			bParar=true;
			return 10;	
			}
		if (nError < 0) 
			{
			delete [] beta; delete [] alpha; delete [] atry; delete [] da;
			return -2;
			}
		//printf("almda(%e)<oldAlamda(%e)?\n", alamda, oldAlamda);
		if (alamda < oldAlamda) 
			{
			double comparacion = fabs(ChiSq - OldChiSq)/ChiSq;
			if (comparacion < 1e-4) 
				{
			//	printf("Salimos a las %d iteraciones\n", i);
				bParar = true;
				}
/*			// En este codigo se comprueba que CHiSq baja 2 veces
			// seguidas y que lo que baja es menos de 0.01 del valor absoluto
			double comparacion = fabs(ChiSq - OldChiSq);
			if (comparacion < 0.01) bajadas++;
			if (bajadas == 2) bParar = true;*/
			}
		else bajadas = 0;
		i++;
		if (i > 300)
			bParar = true;
		//printf("X²=%f\n", ChiSq);
		} while (!bParar);

	// finaliza el ajuste suponiendo que ya esta muy cerca de la solucion
	alamda = 0.0;
	int nError=0;
	if(nError!=123)	
		{
//		printf("Pasamos una última vez\n");
		nError = mrqmin();
		}
//	printf("ERROR CUADRADO: %f\n", ChiSq);
	if (nError < 0) 
		{
		delete [] beta; delete [] alpha; delete [] atry; delete [] da;
		return nError;
		}

		// libera la memoria
	delete [] beta; delete [] alpha; delete [] atry; delete [] da;
	
	// y vuelve sin errores (error>=0)
	if(i<=300)	return 1;//Vuelve por haber convergido
	else		return 0;//Vuelve por terminar sus iteraciones
	}

//*********************** MRQMIN *************************
/* Función de ajuste por mínimos cuadrados
	Usa el método de ajuste de Marquardt-Levenberg, según lo 
	que me ha explicado Begoña.
	Básicamente es un método de búsqueda local, que salta a otras
	zonas si cree que se está metiendo en un mínimo local, con lo
	que la búsqueda se generaliza.
	De los parámetros de la clase, usa:
	alamda->Deberíamos dárselo (lo que vamos a hacer es iniciarlo directamente a -1)
	alpha, beta
	ChiSq->escribe, no hay que pasarlo

	ma->Necesita que se lo pasemos
	a ->Necesita que se lo pasemos
	lista->Necesita que se lo pasemos

	atry->no hay que pasarlo
	covar, da->no hay que pasarlos (la matriz de covarianza es para si no ajustamos todos los parámetros!
	
_________________________________________________________________________
	La función está explicada en el Numerical Recipes Example Book,
	Capítulo 14, pág 155 y sigs.
	Traduzco las partes más interesantes de su explicación:
	mrqmin() se usa junto a mrqcof para realizar ajustes no lineales de mínimos
	cuadrados con el método de Marquardt-Levenberg.
	Los datos artificiales utilizados para provarlos en el programa de ejemplo
	D14R8A se computan como la suma de dos gaussianas más ruido:
		Y(I)=A(1)exp{-[(X(I)-A(2))/A(3)]²} + A(4)exp{-[X(I)-A(5)/A(6)]²}+ruido
		
	Las A(I) se inicializan en DATA, como los parámetros iniciales 
	[NOTA: Nosotros deberemos inicializar la matriz de aes con nuestros parámetros 
	del GA!]
	También hay que inicializar LISTA(I)=I para indicar que todos los parámetros
	van a ajustarse
	[NOTA: Nosotros tendremos que indicar una lista de (2*numGaussianas +2), que
	serán los valores que aparezcan en A]
	
	En la primera llamada a mrqmin ALAMDA debe ser -1.
	Luego se entra en un bucle en el que mrqmin itera mientras chequea los valores
	de CHISQ. Cuando CHISQ cambia menos de 0.1 en dos iteraciones consecutivas se
	considera que el ajuste se ha completado, y MRQMIN se llama una última vez con
	ALAMDA=0 para que COVAR devuelva la matriz de covarianza. 
	Las incertidumbres se derivan de las raices cuadradas de los elementos de la
	diagonal de COVAR (NOTA: que son las varianzas)
	
// Función de error
	*/
int cAjusteChi::mrqmin() 
	{
	if (alamda < 0.0) 
		{
		alamda = 0.001;
	//	printf("Llamamos a mrqcof\n");
		mrqcof(a, alpha, beta);
	//	printf("Volvemos de mrqcof\n");
//		printf("alamda es <0, ponemos como viejo chi (%f) el nuevo (%f)\n", oChiSq, ChiSq);
		oChiSq = ChiSq;
		for (int j = 0; j < ma; j++)	atry[j] = a[j];
		}
	
	int j = -1;
	for (int l = 0; l < ma; l++) 
		{
		if (lista[l] != 0) 
			{
			j++;
			int k = -1;
			for (int m = 0; m < ma; m++)
				if (lista[m] != 0) 
					{
					k++;
					covar[j*ma+k] = alpha[j*ma+k];
					}
			covar[j*ma+j] = alpha[j*ma+j]*(1.0+alamda);
			da[j] = beta[j];
			}
		}
		
	// resuelve la matriz
/*	printf("Antes de gauss\n");
	for(int i=0;i<ma;i++)	printf("a[%d]=%f ", i, a[i]);
	for(int i=0;i<ma;i++)	printf("da[%d]=%f ", i, da[i]);*/
	int nError = gaussj2(covar, mfit, ma, da);
//	printf("gauss realizada\n");
//	for(int i=0;i<ma;i++)	printf("a[%d]=%f ", i, a[i]);
	if (nError < 0) 	return nError;

/*
// ************ DEBUG
	printf("alamda %e  cambios: ", alamda);
	for (int l = 0; l < ma; l++) {
		printf("%e ", da[l]);
	}
	printf("\n");
// *************
*/

	// si ya hay convergencia, evalua la matriz de convergencia
/*	printf("--------------------------El alamda vale %e\n", alamda);
		printf("Matriz de covarianza antes de mrqcof\n");
		for (int i = 0; i < ma; i++) 
			{
			for (int j = 0; j < ma; j++)
				printf("%e ", covar[i*ma+j]);
			printf("\n");
			}
		printf("\n");
	*/
/*	printf("Ajustar covarianza\n");
	for(int i=0;i<ma;i++)	printf("a[%d]=%f ", i, a[i]);*/
	
	if (alamda == 0.0) 
		{
		AjustaCovarianza();
		// escribe matriz de covarianza
		return 0;
		}
	
/*	printf("Covarianza ajustada\n");
	for(int i=0;i<ma;i++)	printf("a[%d]=%f ", i, a[i]);*/
		
	// comprueba si la iteracion fue buena
	j = -1;
	for (int l = 0; l < ma; l++) 
		{
		if (lista[l] != 0) 
			{
			j++;
			atry[l] = a[l] + da[j];
			}
		}
		
/*	printf("antes de mrqcof(atry, covar, da)\n");
	for(int i=0;i<ma;i++)	printf("a[%d]=%f ", i, a[i]);
	for(int i=0;i<ma;i++)	printf("da[%d]=%f ", i, da[i]);
	for(int i=0;i<ma;i++)	printf("atry[%d]=%f ", i, atry[i]);*/
		
	//NUEVO--------------------
	/*if(calculaChi(atry)>oChiSq)
		{
		//printf("X² vale %e y el anterior %e\n", calculaChi(atry), oChiSq);
		printf("Altura vale %f y pretendía valer %f\n", a[3], atry[3]);
		mrqcof(a,covar,da);//Calculamos del último no del nuevo, que es peor
		return 123;
		}
	*/
	mrqcof(atry, covar, da);
	
//	printf("X² vale %e y el anterior %e\n", ChiSq, oChiSq);
/*	printf("despues de mrqcof(atry, covar, da)\n");
	for(int i=0;i<ma;i++)	printf("a[%d]=%f ", i, a[i]);*/
		
	if (ChiSq < oChiSq)
		{
		alamda *= 0.1;//Alamda se divide por 10 si tenemos un chi menor
//		printf("Chi nuevo (%f) mejor que el viejo (%f), ponemos en el viejo el nuevo\n", ChiSq, oChiSq); 
		oChiSq = ChiSq;
		j = -1;
		for (int l = 0; l < ma; l++) 
			{
			if (lista[l] != 0) 
				{
				j++;
				int k = -1;
				for (int m = 0; m < ma; m++)
					if (lista[m] != 0) 
						{
						k++;
						alpha[j*ma+k] = covar[j*ma+k];
						}
				beta[j] = da[j];
//				printf("a[%d] vale %f y atry[%d]=%f\n", l, a[l], l, atry[l]);
						
				a[l]=atry[l];
						
				
				printf("Pico sin restricciones es %d\n", picoSinRestricciones);
				printf("Mirando %d de valor %f\n", l, a[l]);
				//Limitación de ROI
				if(l%4==2 && tauSup>0 && a[l]>tauSup)	
					{
					printf("comparando %d con %d\n", l, picoSinRestricciones+2);
					if(l!=picoSinRestricciones+2)	a[l]=tauSup;	//Limitación holgada de tau
					else	printf("tau sin restricciones: %d\n", l );
					}
				if(l%4==2 && tauInf>0 && a[l]<tauInf)	
					{
					printf("comparando %d con %d\n", l, picoSinRestricciones+2);
					if(l!=picoSinRestricciones+2)	a[l]=tauInf;
					else	printf("tau sin restricciones: %d\n", l );
					}
				if(l%4==1 && sigmaSup>0 && a[l]>sigmaSup)	
					{
					printf("comparando %d con %d\n", l, picoSinRestricciones+1);
					if(l!=picoSinRestricciones+1)	a[l]=sigmaSup;	//Limitación holgada de sigma
					else	printf("sigma sin restricciones: %d\n", l );
					}
				if(l%4==1 && sigmaInf>0 && a[l]<sigmaInf)	
					{
					printf("comparando %d con %d\n", l, picoSinRestricciones+1);
					if(l!=picoSinRestricciones+1)	a[l]=sigmaInf;
					else	printf("sigma sin restricciones: %d\n", l);
					}
					
				if(l%4==0 && centroides!=NULL && a[l]<centroides[l/4]-0.5)	
					{
	//				printf("limitación centroide por abajo quería ser %f\n", a[l]);
					a[l]=centroides[l/4]-0.5;	//Limitación holgada de sigma
					}
				if(l%4==0 && centroides!=NULL && a[l]>centroides[l/4]+0.5)	
					{
	//				printf("limitación centroide por arriba quería ser %f\n", a[l]);
					a[l]=centroides[l/4]+0.5;
					}
				if(l%4==3 && a[l]<0)
					{
	//				printf("limitación altura por abajo quería ser %f\n", a[l]);
					a[l]=0;	//La altura no puede ser negativa
					}
				if(l%4==3 && alturas!=NULL && alturas[l/4]>0 && a[l]>alturas[l/4])	
					{
	//				printf("limitación altura por encima quería ser %f, la pasamos a %f\n", a[l], alturas[l/4]);
					a[l]=alturas[l/4];	
					}
				}
			}
		}
	else 
		{
		alamda *= 10.0;
		ChiSq = oChiSq;
		}
	return 0;
	}

//******************** CALCULACHI ************************
/*	Devuelve el valor de ChiSq tal y como lo calcularía mrqcof
	*/
double cAjusteChi::calculaChi(double *soluc)
	{
	double devolver = 0.0;
	for (int i = 0; i < numDatos; i++) 
		{
		double ymod = ActualizaVector(x[i], soluc, dyda);
		double sig2i = 1.0/powi(sigma[i], 2);
		double dy = y[i] - ymod;
		int j = -1;
		// y calcula Chi
		devolver += dy*dy*sig2i;
		}
	return devolver;
	}
		
//******************** MRQCOF ****************************
/*	NOTA: No sé si es exactamente la función del numerical recipes
	Comprobándolo
	Texto aclaratorio del numerical recipes:
		"El método de ajuste no lineal usa un vector beta (gradiente de X²) y
	otro alpha (la hessiana de X²). Estas cantidades las produce mrqcof.
	La función es una suma de dos gaussianas con ruido, usada dos veces.
	En la primera pasada se ajustan todos los parámetros y en la segunda sólo
	los tres últimos."
*/
void cAjusteChi::mrqcof(double* soluc, double* matriz, double* vector) 
	{
	//printf("MRQCOF\n");
	// inicializa alpha y beta
//	printf("inicializamos a y b\n");
	for (int j = 0; j < mfit; j++) 
		{
		for (int k = 0; k <= j; k++)	matriz[j*ma+k] = 0.0;
		vector[j] = 0.0;
		}
	ChiSq = 0.0;
//		printf("Bucle pa sumar todos los datos\n");
	// bucle para sumar todos los datos
	for (int i = 0; i < numDatos; i++) 
		{
//		printf("Llamamos a actualizavector\n");
		double ymod = ActualizaVector(x[i], soluc, dyda);
//		printf("Hacemos división por sigma\n");
		double sig2i = 1.0/powi(sigma[i], 2);
//		printf("sig2i=1/%f²\n", sigma[i]);
		double dy = y[i] - ymod;
		int j = -1;
//		printf("Bucle con la lista de parámetros\n");
		for (int l = 0; l < ma; l++)
			if (lista[l] != 0) 
				{
				j++;
				double wt = dyda[l]*sig2i;
				int k = -1;
				for (int m = 0; m <= l; m++)
					if (lista[m] != 0) 
						{
						k++;
						matriz[j*ma+k] += (wt*dyda[m]);
						}
				vector[j] += (dy*wt);
				}
		// y calcula Chi
		ChiSq += dy*dy*sig2i;
//		printf("x2+=%f²*%f=%f\n", dy*dy, sig2i, ChiSq);
		}
//	printf("El nuevo chi vale %f para los paámetros:\n", ChiSq);
//	for(int i=0;i<ma;i++)	printf("a[%d]=%f ", i, soluc[i]); 
	// rellena la parte simetrica
	for (int j = 1; j < mfit; j++)
		for (int k = 0; k <= j - 1; k++)
			matriz[k*ma+j] = matriz[j*ma+k];
	}

//*************** AJUSTE CHI: AJUSTA COVARIANZA **************
void cAjusteChi::AjustaCovarianza() 
	{
//	printf("AJUSTA COVARIANZA\n");
	double swap;
	for (int i = mfit; i < ma; i++)
		for (int j = 0; j <= i; j++) 
			{
			covar[i*ma + j] = 0.0;
			covar[j*ma + i] = 0.0;
			}

	int k = mfit-1;
	for (int j = ma-1; j >= 0; j--) 
		{
		if (lista[j] != 0) 
			{
			for (int i = 0; i < ma; i++) 
				{
	//			printf("Cambiamos %d por %d\n", (int)(k+i*(ma+1)), (int)(j+i*(ma+1)));
				swap = covar[i*ma+k];
				covar[i*ma+k] = covar[i*ma+j];
				covar[i*ma+j] = swap;
				}
			for (int i = 0; i < ma; i++) 
				{
		//		printf("Cambiamos %d por %d\n", (int)(k+i*(ma+1)), (int)(j+i*(ma+1)));
				swap = covar[k*ma+i];
				covar[k*ma+i] = covar[j*ma+i];
				covar[j*ma+i] = swap;
				}
			k--;
			}
		}
	}

//********************* GAUSS J2 ********************
/* Resuelve la función de Gauss correspondiente
	a	- matriz np x np (matriz de covarianzas)
	np 	- dimensión de la matriz
	b	- vector
	*/
int gaussj2(double* a, int n, int np, double* b) 
	{
	//printf("\nGAUSSJ2\n");
	int *ipiv = new int[n];
	int *indxc = new int[n];
	int *indxr = new int[n];
		
	if ((ipiv == NULL)||(indxc == NULL)||(indxr == NULL)) 
		{
		printf("Falta de memoria para gaussj2\n");
		delete [] ipiv; delete [] indxc; delete [] indxr;
		return -1;
		}
	// inicializa ipiv
	for (int j = 0; j < n; j++)		ipiv[j] = 0;
	int irow, icol;
	for (int i = 0; i < n; i++) 
		{
		double big = 0.0;
		for (int j = 0; j < n; j++) 
			{
			if (ipiv[j] != 1) 
				{
				for (int k = 0; k < n; k++) 
					{
					if (ipiv[k] == 0) 
						{
						if (fabs(a[k+j*np]) >= big) 
							{
							big = fabs(a[k+j*np]);
							irow = j;
							icol = k;
							}
						}
					else if (ipiv[k] > 1) 
						{
						printf("ERROR: ipiv[%d]=%d > 1\n", k, ipiv[k]);
						delete [] ipiv; delete [] indxc; delete [] indxr;
						return -2;
						}
					}
				}
			}
		ipiv[icol]++;
		if (irow != icol) 
			{
			for (int l = 0; l < n; l++) 
				{
				double swap = a[l + irow*np];
				a[l + irow*np] = a[l + icol*np];
				a[l + icol*np] = swap;
				}
			double swap = b[irow];
			b[irow] = b[icol];
			b[icol] = swap;
			}
		indxr[i] = irow;
		indxc[i] = icol;
		if (a[icol+icol*np] == 0.0) 
			{
			printf("ERROR: a[%d+%d*%d]=0\n", icol, icol, np);
			delete [] ipiv; delete [] indxc; delete [] indxr;
			return -2;
			}
		double pivinv = 1.0/a[icol+icol*np];
		a[icol+icol*np] = 1.0;
		for (int l = 0; l < n; l++)
			a[l + icol*np] *= pivinv;
		b[icol] *= pivinv;
		for (int ll = 0; ll < n; ll++)
			if (ll != icol) 
				{
				double dum = a[icol + ll*np];
				a[icol + ll*np] = 0.0;
				for (int l = 0; l < n; l++)
					a[l+ll*np] = a[l+ll*np] - (a[l+icol*np]*dum);
				b[ll] -= (b[icol]*dum);
				}
		}
	
	for (int l = n-1; l >= 0; l--)
		if (indxr[l] != indxc[l])
			for (int k = 0; k < n; k++) 
				{
				double dum = a[indxr[l]+k*np];
				a[indxr[l]+k*np] = a[indxc[l]+k*np];
				a[indxc[l]+k*np] = dum;
				}
				
	delete [] ipiv;
	delete [] indxc;
	delete [] indxr;
	
	return 0;
	}
