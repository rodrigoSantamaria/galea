#include <math.h>
#include <stdio.h>
#include <time.h>

#include "espectros.h"
#include "avisos.h"

#include "globales.h"

#include "string.h"
#include "stdlib.h"
#include "fstream.h"
#include "iostream.h"

#include "math1.h"
#include "ajusteEsp.h" /* Aqui se encuentran los enlaces exteriores a funciones
				necesarias para los diversos ajustes, splines, cubicas,
				rectas, etc. debido a problemas con el enlace a objetos
				dinamicos (que todavia no he resuelto) */
#include "ajustes.h"

//PROBANDO EL PAQUETE GENÉTICO
#include "GA/GeneticAlgorithm2.h"
#include "unistd.h"
//
void imprimeMatriz(double **m, int n1, int n2);

/************************
 *                      *
 *   OTROS PROTOTIPOS   *
 *                      *
 ************************/

// ecuacion de inicializacion de centroide
double SolucionBisecCentroide(double);

/***************
 *             *
 *   GENERAL   *
 *             *
 ***************/


cEspectros::cEspectros() 
   { // constructor
   // inicializa los atributos
   espectro = NULL;//Array con el número de cuentas de cada canal
   fondo = NULL; picos = NULL;
   continuo = NULL; sigmaContinuo = NULL;
   total = NULL;
   serieCofo.numElementos = 0; serieCofo.inicio = NULL;
   serieCopi.numElementos = 0; serieCopi.inicio = NULL;
   serieManphe.numElementos = 0; serieManphe.inicio = NULL;
   listaPicos.numElementos = 0; listaPicos.inicio = NULL;
   numCanales = 0; numCanalesFondo = 0;//Nº de canales del espectro (generalmente 8192) y de ellos, los que ocupa el fondo
   afwhg = 0.0; bfwhg = 0.0;
   enp1 = 0.0; enp2 = 0.0; enp3 = 0.0;
   minInterv = 0; maxInterv = 0;//Valor del canal mínimo y máximo del intervalo de análisis en cada momento
   minTotal = 0; maxTotal = 0;//Nº mínimo y máximo de cuentas en los canales del espectro
   numPicosEncontrados = 0;//Número de picos encontrados tras realizar el análisis
   InicializaBSpline(SplineContinuo);
   nNodosSplineContinuo = 0;
   numCanales=0;
   matricesActividad=new LinkedList();
   }

cEspectros::~cEspectros() 
   { // destructor
   BorraDatos();
   }


/*****************
 *               *
 *   ESPECTROS   *
 *               *
 *****************/
/*	Datos.esFondo
    Datos.tiempo
   numCanales
   dimensionMatEfi
   matrizEficiencia
   nodosEficiencia
   parametrosEficiencia
   */
void cEspectros::BorraDatos() 
   {
   printf("************************** BORRA DATOS ***********************\n");
	   
	// matrices
	  printf("Borramos datos típicos\n");
   if (espectro != NULL)   delete [] espectro;
   if (fondo != NULL) delete [] fondo;
   if (picos != NULL) delete [] picos;
   if (continuo != NULL) delete [] continuo;
   if (total != NULL) delete [] total;
   if (sigmaContinuo != NULL) delete [] sigmaContinuo;

   //
   printf("Borramos datos del fondo\n");
   if(cuentasFondo !=NULL)	delete [] cuentasFondo;
   printf("Borramos sigmaCuentasFondo\n");
   if(sigmaCuentasFondo!=NULL)	delete [] sigmaCuentasFondo;
   //
   cuentasFondo=NULL;
   sigmaCuentasFondo=NULL;
   
   if(mediasFondo!=NULL)	delete [] mediasFondo;
   if(energiasFondo!=NULL)	delete [] energiasFondo;
   if(incertidumbresFondo!=NULL)	delete [] incertidumbresFondo;
   mediasFondo=energiasFondo=incertidumbresFondo=NULL;
   numFondos=0;
   
  printf("A null y 0 datos generales\n");
	espectro = NULL; fondo = NULL; picos = NULL;
	continuo = NULL; sigmaContinuo = NULL;
	total = NULL;
	// Datos generales
	numCanales = 0; numCanalesFondo = 0;
	numPicosEncontrados = 0;
	minInterv = 0; maxInterv = 0;
	minTotal = 0; maxTotal = 0;
   
    /* nombres
  printf("Borramos nombres de cosas\n");
    if(nombre!=NULL)	
		{
		printf("nombre vale %s\n", nombre);
		delete [] nombre;
		}
	printf("Borramos nombres de fondo\n");
  if(nombreFondo!=NULL)	delete [] nombreFondo;
	printf("Borramos nombres de eficiencia\n");
  if(nombreEficiencia!=NULL)	delete [] nombreEficiencia;
	  */

	printf("Borramos intervalos, tenemos %d elementos\n", listaPicos.numElementos);
    for(int i=0;i<listaPicos.numElementos;i++)	
		{
		printf("%d\n", i);
		BorraIntervalo(i);
		}
		
	printf("Borramos listas\n");
	// listas
	borraLista(serieCofo);
	borraLista(serieCopi); 
	borraLista(serieManphe);
	borraLista(listaPicos);
	// datos de calibraciones
   	afwhg = 0.0; bfwhg = 0.0;
  	enp1 = 0.0; enp2 = 0.0; enp3 = 0.0;
	atau = 0.0; btau = 0.0;
	// Datos del continuo
	printf("Borramos spline\n");
	borraLista(SplineContinuo);
	nNodosSplineContinuo = 0;
	//
//	delete [] ajustes;
		
	//NOTA: No hace falta borrarlo al final, pues se borra ya en el momento de
		//terminar con el continuo. QUizás estaría bien que siempre que borre algo,
		//ponga el puntero correspondiente a NULL, así luego la comprobación valdría
		//para algo y me aseguraría siempre el no borrar dos veces.
	printf("Borramos valorx, y y sigmay\n");
	if(ValorX!=NULL)	delete [] ValorX;
	if(ValorY!=NULL)	delete [] ValorY;
	if(SigmaY!=NULL)	delete [] SigmaY;
	ValorY=ValorX=SigmaY=NULL;
	//
	
	//
	printf("Borramos nodos de la eficiencia\n");
	if(nodosEficiencia!=NULL)		{delete [] nodosEficiencia;nodosEficiencia=NULL;}
	printf("Borramos parametros de la eficiencia\n");
	if(parametrosEficiencia!=NULL)	{delete [] parametrosEficiencia;parametrosEficiencia=NULL;}
	/*
	printf("Borramos matriz de la eficiencia\n");
	if(matrizEficiencia!=NULL)
		{		
		for(int i=0;i<dimensionMatEfi;i++)	
			{
			printf("matriz %d\n",i);
			if(matrizEficiencia[i]!=NULL)	delete [] matrizEficiencia[i];
			}
		//printf("Borramos la superior\n");
		//delete matrizEficiencia;
		}*/
	//
	/*
	printf("matrices actividad\n");
	if(matricesActividad!=NULL)		
		{
		while(matricesActividad->numElements()>0)
			{
			LinkedList *temp=(LinkedList *)matricesActividad->getFirst();
			temp->deleteAll();
			matricesActividad->deleteFirst();
			}
		matricesActividad->deleteAll();
		free(matricesActividad);
		}*/
	printf("fin de BORRADATOS\n");
	}

//******************** CARGA DATOS ************************
//Copia a nuestra clase cEspectros los datos que se encuentren en la matriz
// "fuente". 
// CargaDatos NO LIBERA la memoria asociada a la estructura de datos, es necesario
// llamar con anterioridad a BorraDatos().
// NOTA: No veo la utilidad del parámetro numDatos. No nos valdría con tomar el tamaño de fuente?

//NOTA: Importante! Toma una cadena de un tamaño uno mayor y empieza dede 1!!!!
int cEspectros::CargaDatos(unsigned int *fuente, unsigned short numDatos) 
	{
//    BorraDatos(); // vacia el objeto para llenarlo con los nuevos datos
	espectro = new unsigned int[numDatos + 1]; // Reserva la memoria necesaria
   
	if (espectro == NULL) // no hay memoria disponible
		{
		printf("ERROR: No hay memoria para reservar unsigned int[%d]", numDatos+1);
      	return -1;
		}

    for (int i = 0; i < numDatos; i++) // lee los datos
		espectro[i+1] = fuente[i];

	numCanales = numDatos;
	minTotal = minimo(1, numCanales);
	maxTotal = maximo(1, numCanales);
    return 0;
	}

	
//************ MÍNIMO ******************
/* Nos da el valor mínimo de los que se encuentran en la cadena entre las posiciones inicio y final */
unsigned int cEspectros::minimo(unsigned short inicio, unsigned short final) 
	{
	unsigned int min=espectro[inicio];
	if (numCanales>=final && inicio<final) 
	  {
      for (int i = inicio; i <= final; i++) 
         if (min > espectro[i]) min = espectro[i];
	  }
	return min;
   	}

//************ MÁXIMO ******************
/* Nos da el valor máximo de los que se encuentran en la cadena entre las posiciones inicio y final */
unsigned int cEspectros::maximo(unsigned short inicio, unsigned short final) 
	{
	unsigned int max=espectro[inicio];
	if(numCanales>=final && inicio<final)
		{
  	    for (int i = inicio; i <= final; i++) 
		 	{
			//printf("Max vale %d y espectro[%d] vale %d\n", max, i, espectro[i]);
       	 	if (max < espectro[i]) max = espectro[i];
			}
		}
		return max;
	}


/*******************
 *                 *
 *   CALIBRACIÓN   *
 *                 *
 *******************/

 //**************************** PRECALIBRA **********************************
 // Función que realiza todos los cálculos de la precalibración.
 // "datos" es una lista con todos los nodos de precalibración, cada uno conteniendo
 // 		ambos límites y el valor de la E.
 // "numPicos" es el número de nodos que tenemos.
int cEspectros::PreCalibra(tPrecalibracion *datos, unsigned char numPicos) 
	{

   	unsigned int temp; // variable de uso general
   	double temp2; // variable de uso general

   	double *puntosX, *puntosY, *sigmaY; // para pasar los puntos a ajustar

   	double baseX[8], baseY[8], baseS[8], baseA, baseB; // para el calculo de la linea base

   	double *gausX, *gausY, *gausS; // para ajustar los datos a una gaussiana
   	unsigned short limInf, limSup; // limites donde se encuentra la gaussiana

   	cRecta Recta; // para hacer ajustes a rectas
   
   	if (datos == NULL) return -1;
   
   	//Reserva la memoria necesaria
   	if ((puntosX = new double[numPicos]) == NULL) 
   		{
		printf("ERROR: No hay memoria para reservar double[%d]\n", numPicos);
		return -1;
		}
   	if ((puntosY = new double[numPicos]) == NULL) 
	   	{
		printf("ERROR: No hay memoria para reservar double[%d]\n", numPicos);
      	delete [] puntosX;
      	return -1; 
		}
   	if ((sigmaY = new double[numPicos]) == NULL) 
	   	{
		printf("ERROR: No hay memoria para reservar double[%d]\n", numPicos);
      	delete [] puntosX; delete [] puntosY;
      	return -1; 
		}
      

   // Busca el canal del maximo en cada intervalo y lo asocia con la energia de emision
   for (unsigned char i = 0; i < numPicos; i++) 
      {
      temp = 0;//Será el valor del número de cuentas del canal con más cuentas del espectro en los límites que busquemos
      puntosX[i] = datos[i].energia;//PuntosX contiene las energías de los picos de la lista de precalibración
      					//(los que se introducen en la ventana de precalibración)
	//Para cada uno de esos nodos de la lista de precalibración, cogemos el número de cuentas del canal con más
	//cuentas en ese intervalo y en puntosY metemos el canal con más cuentas
      for (unsigned short j = datos[i].limInferior; j <= datos[i].limSuperior; j++) 
      		{
		if (temp < espectro[j]) 
				{
				temp = espectro[j];
	    		puntosY[i] = (double) j;
	    		}
      		}
      sigmaY[i] = 1;//NOTA: SigmaY es 1 porque en esta fase no consideramos incertidumbres,
					//ya que cuando las hemos considerado (la poníamos a 0.05*puntosY) 
					//los ajustes de precalibración quedan bastante mal para energías altas
					//En concreto para el k40 teníamos un error de maś de 5 puntos!
      }
   

   /* ****
      Hace la calibracion canal-energia
      ****/
   
   Recta.AjusteARecta(puntosX, puntosY, sigmaY, numPicos);
   enp1 = Recta.A; enp2 = Recta.B; enp3 = 0.0; //Variables que se encargan de la calibración canal-energía
   //(enp3 se pone a cero porque estamos en la precalibración y enp3 se usará en la calibración
	printf("ENP1=%f\tENP2=%f\n", enp1, enp2);
   /* ****
      Hace la calibracion del canal-anchura
      ****/
   
   // primero ha de calcular la fwhg experimental de cada pico
   // para ello primero calcula la linea base del pico
   // despues lo ajusta a una gaussiana y de ahi saca su anchura
   // con los datos para los picos de la precalibracion hace el ajuste
   for (unsigned char i = 0; i < numPicos; i++) 
   {//Para cada pico

   	//Del pico se usan los valores [0, 1, 2, 3, ......... , limsup-3, -2, -1, limsup]
      // calcula la linea base del pico cogiendo 4 canales por la izquierda...
      for (unsigned char j = 0; j < 4; j++) 
      	{
        baseX[j] = datos[i].limInferior + j;//cada uno de los 4 canales más a la izquierda
		baseY[j] = espectro[(unsigned int)baseX[j]];//baseY=nº cuentas de ese canal
		baseS[j] = sqrt(baseY[j]);
		if (baseS[j] == 0) baseS[j] = 1.0;
		//printf("baseX: %u\tbaseY %e\tbaseS %e\n", (unsigned int)baseX[j], baseY[j], baseS[j]);
      	}
      // ... y 4 canales por la derecha
      for (unsigned char j = 7; j > 3; j--) 
      	{
        baseX[j] = datos[i].limSuperior - j + 4;
		baseY[j] = espectro[(int)baseX[j]];
		baseS[j] = sqrt(baseY[j]);
		if (baseS[j] == 0) baseS[j] = 1.0;
		//printf("baseX: %u\tbaseY %e\tbaseS %e\n", (unsigned int)baseX[j], baseY[j], baseS[j]);
		}
      Recta.AjusteARecta(baseX, baseY, baseS, 8);
      baseA = Recta.A; baseB = Recta.B;
      //printf("a: %e\tb: %e\n", baseA, baseB);


      // limita la anchura del pico a canales al menos un punto por encima de la linea base
      //Lo que hace es restarle a la función del espectro en cada punto la línea base. Si la diferencia entre la línea
      //base (que será una recta con valores de y pequeños) y la función del espectro (que será más o menos una gaussiana)
      //no supera 1, es que podemos desestimar como parte del pico hasta ese punto (es parte de la llanura previa / posterior)
      // luego aumentamos el límite inferior/superior hasta ahí.
      //NOTA: Quizá estaría bien dibujar la línea base para hacerse una idea
      //NOTA: No sé si podría pasar, pero, ¿y si un punto que ya se encuentra entre los del pico por lo que sea ha salido
      //	con pocas cuentas?-> nos cargamos el intervalo no?
      limInf = datos[i].limInferior;
//      for (unsigned short j = limInf; j <= (int) puntosY[i]; j++) //NOTA: cambiándolo que esto lo modifica
      for (unsigned short j = datos[i].limInferior; j <= (int) puntosY[i]; j++) 
      	{
        temp2 = espectro[j] - Recta.A - Recta.B*j;
		if (temp2 < 1.0) limInf = j + 1;
      	}
      limSup = datos[i].limSuperior;
//      for (unsigned short j = limSup; j >= (int) puntosY[i]; j--) //NOTA: Lo mesmo
      for (unsigned short j = datos[i].limSuperior; j >= (int) puntosY[i]; j--) 
      	{
        temp2 = espectro[j] - Recta.A - Recta.B*j;
		if (temp2 < 1.0) limSup = j - 1;
      	}

	//Si despues de esta limitación seguimos teniendo pico (algún canal del intervalo supera en más de un punto
	//la línea base)
	if (limSup - limInf > 0) 
		{      
	        // reserva memoria para los nuevos datos
      	if ((gausX = new double[limSup-limInf+1]) == NULL) 
			{
			printf("ERROR: No hay memoria para reservar double[%d]\n", limSup-limInf+1);
        	delete [] puntosX; delete [] puntosY; delete [] sigmaY;
         	return -1; 
			}
      	if ((gausY = new double[limSup-limInf+1]) == NULL) 
			{
			printf("ERROR: No hay memoria para reservar double[%d]\n", limSup-limInf+1);
         	delete [] puntosX; delete [] puntosY; delete [] sigmaY;
			delete [] gausX;
         	return -1; 
			}
      	if ((gausS = new double[limSup-limInf+1]) == NULL) 
			{
			printf("ERROR: No hay memoria para reservar double[%d]\n", limSup-limInf+1);
         	delete [] puntosX; delete [] puntosY; delete [] sigmaY;
			delete [] gausX; delete [] gausY;
         	return -1; 
			}

   	      // coge los canales de la gaussiana
	      //printf("max: %e\n", puntosY[i]);
		  //printf("limInf: %u\tlimSup: %u\n", limInf, limSup);
	      for (unsigned short j = 0; j <= limSup - limInf; j++) 
	      	{//Calculamos el logaritmo de lo que nos queda en el intervalo (debería de ser +- una gaussiana)
	        temp = j + limInf; //Canal del espectro
	        temp2 = espectro[temp] - Recta.A - Recta.B*temp;//Valor de la función en el canal, quitándole la línea base
			gausS[j] = sqrt(espectro[temp] + powi(Recta.sigmaA, 2) + powi(Recta.sigmaB*temp, 2))/temp2;
			gausY[j] = log(temp2);
			gausX[j] = powi((double)temp - puntosY[i], 2)/2.0;
			//printf("X: %e\tY: %e\tS: %e\n", gausX[j], gausY[j], gausS[j]);
	      	}
      
	      // ajusta el logaritmo de la gaussiana a una recta
	      Recta.AjusteARecta(gausX, gausY, gausS, limSup - limInf + 1);      
      
	      // pone los datos obtenidos en la matriz de datos a ajustar
	      puntosX[i] = puntosY[i];
	      puntosY[i] = -powi(2.35, 2) / Recta.B; // pasa la anchura de la gaussiana a su valor FWHG
	      //sigmaY[i] = powi(2.35, 2) * powi(1.0/Recta.B, 2) * Recta.sigmaB; // no utilizado
	      sigmaY[i] = puntosY[i]*0.1;
	      //printf("canal: %e\tsigma: %e\t rectab: %e\n", puntosX[i], puntosY[i], 1.0/Recta.B);
      
   	   // libera la memoria utilizada
	      delete [] gausX; delete [] gausY; delete [] gausS;
	      }//if(limSup-limInf>0)
	      
	      else //Nos hemos quedado sin pico tras el ajuste de la línea base
	      	{
			delete [] puntosX; delete [] puntosY; delete [] sigmaY;
			return -1;//NOTA: Averiguar si se hace algo en la calibración si uno de los picos realmente no existe
			}
   		}//fin del for(para cada pico)
   
   Recta.AjusteARecta(puntosX, puntosY, sigmaY, numPicos);
   afwhg = Recta.A; bfwhg = Recta.B;

   // libera memoria
   delete [] puntosX; delete [] puntosY, delete [] sigmaY;
   return 0;
	}

//******************* CALIBRA ENERGIA **********************
//Devuelve el número de picos de que se dispone para la calibración
//Para ello cuenta el número de nodos de la lista de calibración que se le pasa como
//parámetro que tienen su ajuste calibrado.
int cEspectros::CalibraEnergia(tListaCalibracion lista) 
	{
	// cuenta el numero de picos que se dispone para la calibracion
	int numCalibrados = 0;
	for (int i = 1; i <= lista.numElementos; i++) 
		{
		NodoCalibracion *nodo = DevuelveNodo(lista, i);
		NodoPicos *intervalo = DevuelveNodo(listaPicos, nodo->numIntervalo);
		NodoPico *pico = intervalo->picos.inicio;
		/* TODO: Ahora coge solo los picos que han sido asociados a una
			energia para hacer la calibracion de la forma. Debería poder
			coger segun otro metodo (por ejemplo, todos los de la lista) */
		if (pico->Ajuste.calibrado)	numCalibrados++;
		}
	
	// construye los vectores de datos para el ajuste
	double *DatosX = new double[numCalibrados+1];
	double *DatosY = new double[numCalibrados+1];
	double *SigmaY = new double[numCalibrados+1];
	if ((DatosX == NULL)||(DatosY == NULL)||(SigmaY == NULL)) 
		{
		printf("ERROR: No hay memoria para reservar double[%d]\n", numCalibrados+1);
		delete [] DatosX; delete [] DatosY; delete [] SigmaY;
		return -2;
		}
	
	// asocia las matrices al objeto que ajusta los datos
	cAjusteLinealChi Ajustes;
	Ajustes.x = DatosX; Ajustes.y = DatosY; Ajustes.sigma = SigmaY;
	Ajustes.numDatos = numCalibrados;
	Ajustes.primerNodo = NULL;
	Ajustes.AsociaFuncionActualiza((tFuncion) ActualizaVectorCuadradoCalib);


	// Energia
	//double *coeficientes = new double[4];
	double *coeficientes = new double[5];
	if (coeficientes == NULL) 
		{
		printf("ERROR: No hay memoria para reservar double[4]\n");
		delete [] coeficientes;
		delete [] DatosX; delete [] DatosY; delete [] SigmaY;
		return -2;
		}
	int k = 0, i = 1;
	while ((i <= lista.numElementos)&&(k <= numCalibrados)) 
		{
		NodoCalibracion *nodo = DevuelveNodo(lista, i);
		NodoPicos *intervalo = DevuelveNodo(listaPicos, nodo->numIntervalo);
		NodoPico *pico = intervalo->picos.inicio;
		if (pico->Ajuste.calibrado) 
			{
			//DatosX[k] = pico->Ajuste.Energia;
			double energia=0;	
			for(int i=0;i<pico->Ajuste.emisiones->numElements();i++)
					{
					printf("Tomamos el número de emisión\n");
					NodoEmision *ne=(NodoEmision *)(pico->Ajuste.emisiones->getNode(i));
					energia = ne->Energia;
					}
			DatosX[k] = energia;//NOTA: Ahora nos quedamos con la última del bucle: una media?
								//		No lo veo necesario, en calibración sólo tendremos una identificación
			DatosY[k] = pico->Ajuste.Centroide;
			SigmaY[k] = pico->Ajuste.SigCentroide;
			printf("e=%f, c=%f(%f)\n", DatosX[k], DatosY[k], SigmaY[k]);
			k++;
			}
		i++;
		}
		
	//Probando un nuevo tipo de ajuste: Quizás lo suyo es meter un pico de menos por el
	//medio para que el cambio sea menos brusco.
	//Probar eso, luego probar a quitar a vector energia el x2.
	//Mirar cuál es el problema exacto por el que falla gaussj().
	Ajustes.ma = 3; Ajustes.a = coeficientes;
	Ajustes.afunc = vectorCuadratica; Ajustes.covar = covarEnergia;
	//Ajustes.ma = 4; Ajustes.a = coeficientes;
	//Ajustes.afunc = vectorEnergia; Ajustes.covar = covarEnergia;
	int nError = Ajustes.Ajusta();
	if (nError < 0) 
		{
		delete [] coeficientes;
		delete [] DatosX; delete [] DatosY; delete [] SigmaY;
		if (nError == -1) return -2;
		else return -1;
		}
	Ajustes.AjustaCovarianza();
	// ahora actualiza los valores de enp1, enp2 y enp3
	enp3 = coeficientes[3]; enp2 = coeficientes[2]; enp1 = coeficientes[1];
	//
	//enp4 = coeficientes[4];
	//
	delete [] coeficientes;
	chiEnergia=Ajustes.ChiSq/(numCalibrados-Ajustes.ma);//Dividimos la chi entre el número de grados de libertad (número de puntos - número de parámetros a ajustar)
	printf("CHI ENERGÍA=%f\n", chiEnergia);
	//printf("Se ajusta a la ecuación y=%f*x²+%f*x+%f*%f(1/x)\n", enp3,enp2,enp1,enp4);
	printf("Se ajusta a la ecuación y=%f*x²+%f*x+%f\n", enp3,enp2,enp1);
	// elimina la memoria reservada y sale
	delete [] DatosX; delete [] DatosY; delete [] SigmaY;
	return 0;
}

//******************* CALIBRA FWHG *******************
int cEspectros::CalibraFWHG(tListaCalibracion lista) 
	{
	// cuenta el numero de picos que se dispone para la calibracion
	//int numCalibrados = lista.numElementos;
	
	int numCalibrados = 0;
	for (int i = 1; i <= lista.numElementos; i++) {
		NodoCalibracion *nodo = DevuelveNodo(lista, i);
		NodoPicos *intervalo = DevuelveNodo(listaPicos, nodo->numIntervalo);
		NodoPico *pico = intervalo->picos.inicio;
		// TODO: Ahora coge solo los picos que han sido asociados a una
		//	energia para hacer la calibracion de la forma. Debería poder
		//	coger segun otro metodo (por ejemplo, todos los de la lista)
		if (pico->Ajuste.calibrado)
			numCalibrados++;
	}
	
	// construye los vectores de datos para el ajuste
	double *DatosX = new double[numCalibrados+1];
	double *DatosY = new double[numCalibrados+1];
	double *SigmaY = new double[numCalibrados+1];
	//NOTA: Puede ser que estos datos estén mal iniciados
	if ((DatosX == NULL)||(DatosY == NULL)||(SigmaY == NULL)) 
		{
		printf("ERROR: No hay memoria para reservar double[%d]\n", numCalibrados+1);
		delete [] DatosX; delete [] DatosY; delete [] SigmaY;
		return -2;
		}
	// asocia las matrices al objeto que ajusta los datos
	cAjusteLinealChi Ajustes;
	Ajustes.x = DatosX; Ajustes.y = DatosY; Ajustes.sigma = SigmaY;
	Ajustes.numDatos = numCalibrados;
	Ajustes.primerNodo = NULL;
	Ajustes.AsociaFuncionActualiza((tFuncion) ActualizaVectorRectaCalib);


	// FWHG
	double *coeficientes = new double[3];
	if (coeficientes == NULL) 
		{
		printf("ERROR: No hay memoria para reservar double[%d]\n", 3);
		delete [] coeficientes;
		delete [] DatosX; delete [] DatosY; delete [] SigmaY;
		return -2;
		}
	// establecidas 2 variables por algoritmo antiguo (ver principio
	// de la funcion). Optimizar cuando se establezca como fijo
	int k = 0, i = 1;
	while((i <= lista.numElementos)&&(k <= numCalibrados)) 
		{
		NodoCalibracion *nodo = DevuelveNodo(lista, i);
		NodoPicos *intervalo = DevuelveNodo(listaPicos, nodo->numIntervalo);
		NodoPico *pico = intervalo->picos.inicio;
//		if (pico->Ajuste.calibrado) {
			DatosX[k] = pico->Ajuste.Centroide;
			//DatosY[k] = powi(pico->Ajuste.Anchura, 2)*2.3548;
			//SigmaY[k] = 2.0*2.3548*pico->Ajuste.Anchura*pico->Ajuste.SigAnchura;
			DatosY[k] = powi(pico->Ajuste.Anchura, 2);
			SigmaY[k] = 2.0*pico->Ajuste.Anchura*pico->Ajuste.SigAnchura;
			k++;
//		}
		i++;
		}
	Ajustes.ma = 2; Ajustes.a = coeficientes;
	Ajustes.afunc = vectorRecta; Ajustes.covar = covarSigma;
	
	int nError = Ajustes.Ajusta();
	if (nError < 0) 
		{
		delete [] coeficientes;
		delete [] DatosX; delete [] DatosY; delete [] SigmaY;
		if (nError == -1) return -2;
		else return -1;
		}
	Ajustes.AjustaCovarianza();
	// ahora actualiza los valores de afwhg, bfwhg: y=ax+b
	bfwhg = coeficientes[2]; afwhg = coeficientes[1];
	chiSigma=Ajustes.ChiSq/(numCalibrados-Ajustes.ma);
		
	delete [] coeficientes;
	// elimina la memoria reservada y sale
	delete [] DatosX; delete [] DatosY; delete [] SigmaY;
	return 0;
	}

//****************** CALIBRA TAU **********************
int cEspectros::CalibraTau(tListaCalibracion lista) 
	{
	// cuenta el numero de picos que se dispone para la calibracion
	int numCalibrados = lista.numElementos;
/*	int numCalibrados = 0;
	for (int i = 1; i <= lista.numElementos; i++) {
		NodoCalibracion *nodo = DevuelveNodo(lista, i);
		NodoPicos *intervalo = DevuelveNodo(listaPicos, nodo->numIntervalo);
		NodoPico *pico = intervalo->picos.inicio;
		// TODO: Ahora coge solo los picos que han sido asociados a una
		//	energia para hacer la calibracion de la forma. Debería poder
		//	coger segun otro metodo (por ejemplo, todos los de la lista) 
		if (pico->Ajuste.calibrado)
			numCalibrados++;
	}*/
	
	// construye los vectores de datos para el ajuste
	double *DatosX = new double[numCalibrados+1];
	double *DatosY = new double[numCalibrados+1];
	double *SigmaY = new double[numCalibrados+1];
	if ((DatosX == NULL)||(DatosY == NULL)||(SigmaY == NULL)) 
		{
		printf("ERROR: No hay memoria para reservar double[%d]\n", numCalibrados+1);
		delete [] DatosX; delete [] DatosY; delete [] SigmaY;
		return -2;
		}
	
	// asocia las matrices al objeto que ajusta los datos
	cAjusteLinealChi Ajustes;
	Ajustes.x = DatosX; Ajustes.y = DatosY; Ajustes.sigma = SigmaY;
	Ajustes.numDatos = numCalibrados;
	Ajustes.primerNodo = NULL;
	Ajustes.AsociaFuncionActualiza((tFuncion) ActualizaVectorRectaCalib);


	// Tau
	double *coeficientes = new double[3];
	if (coeficientes == NULL) 
		{
		printf("ERROR: No hay memoria para reservar double[%d]\n", 3);
		delete [] coeficientes;
		delete [] DatosX; delete [] DatosY; delete [] SigmaY;
		return -2;
		}
	// establecidas 2 variables por algoritmo antiguo (ver principio
	// de la funcion). Optimizar cuando se establezca como fijo
	int k = 0, i = 1;
	while ((i <= lista.numElementos)&&(k <= numCalibrados)) 
		{
		NodoCalibracion *nodo = DevuelveNodo(lista, i);
		NodoPicos *intervalo = DevuelveNodo(listaPicos, nodo->numIntervalo);
		NodoPico *pico = intervalo->picos.inicio;
//		if (pico->Ajuste.calibrado) {
			DatosX[k] = pico->Ajuste.Centroide;
			DatosY[k] = pico->Ajuste.Tau;
			SigmaY[k] = pico->Ajuste.SigTau;
			printf("Tau[%d] vale al calibrar %f(%f)\n", k, DatosY[k], SigmaY[k]);
			k++;
//		}
		i++;
		}
	Ajustes.ma = 2; Ajustes.a = coeficientes;
	Ajustes.afunc = vectorRecta; Ajustes.covar = covarTau;
	int nError = Ajustes.Ajusta();
	if (nError < 0) 
		{
		delete [] coeficientes;
		delete [] DatosX; delete [] DatosY; delete [] SigmaY;
		if (nError == -1) return -2;
		else return -1;
		}
	Ajustes.AjustaCovarianza();
	// ahora actualiza los valores de atau y btau
	btau = coeficientes[2]; atau = coeficientes[1];
	delete [] coeficientes;
	
	chiTau=Ajustes.ChiSq/(numCalibrados-Ajustes.ma);
	
	// elimina la memoria reservada y sale
	delete [] DatosX; delete [] DatosY; delete [] SigmaY;
	return 0;
	}

//--------------------------------------------------------
//****************** CALIBRA RESOLUCIÓN **********************
/*	Esta función nueva trata de calibrar la resolución, que es 
	sigma/altura
	NOTA: OJO, es sigma/canal !!!-->cambiándolo
	NOTA: Las calibraciones de la resolución salen muy bajas (10¯³), lo cual
		parece normal pues generalmente canal>>>sigma. Aún así la calibración
		da valores más bajos de aquellos que se usan para calibrar
		Según Debertin[pags. 89-90] la resolución es la FWHM (anchura del pico
		a la mitad de la altura)->FWHG=FWHM
	NOTA: Si usamos como resolución lo de Debertin, nos vale con la calibraFWHG
	*/
int cEspectros::CalibraResolucion(tListaCalibracion lista) 
	{
	// cuenta el numero de picos que se dispone para la calibracion
	int numCalibrados = lista.numElementos;
	
	// construye los vectores de datos para el ajuste
	double *DatosX = new double[numCalibrados+1];
	double *DatosY = new double[numCalibrados+1];
	double *SigmaY = new double[numCalibrados+1];
	if ((DatosX == NULL)||(DatosY == NULL)||(SigmaY == NULL)) 
		{
		printf("ERROR: No hay memoria para reservar double[%d]\n", numCalibrados+1);
		delete [] DatosX; delete [] DatosY; delete [] SigmaY;
		return -2;
		}
	
	// asocia las matrices al objeto que ajusta los datos
	cAjusteLinealChi Ajustes;
	Ajustes.x = DatosX; Ajustes.y = DatosY; Ajustes.sigma = SigmaY;
	Ajustes.numDatos = numCalibrados;
	Ajustes.primerNodo = NULL;
	Ajustes.AsociaFuncionActualiza((tFuncion) ActualizaVectorRectaCalib);

	// Tau
	double *coeficientes = new double[3];
	if (coeficientes == NULL) 
		{
		printf("ERROR: No hay memoria para reservar double[%d]\n", 3);
		delete [] coeficientes;
		delete [] DatosX; delete [] DatosY; delete [] SigmaY;
		return -2;
		}
		
	// establecidas 2 variables por algoritmo antiguo (ver principio
	// de la funcion). Optimizar cuando se establezca como fijo
	int k = 0, i = 1;
	while ((i <= lista.numElementos)&&(k <= numCalibrados)) 
		{
		NodoCalibracion *nodo = DevuelveNodo(lista, i);
		NodoPicos *intervalo = DevuelveNodo(listaPicos, nodo->numIntervalo);
		NodoPico *pico = intervalo->picos.inicio;

			DatosX[k] = pico->Ajuste.Centroide;//Centroide en canales
			double s=pico->Ajuste.Anchura;//Anchura en canales
//			double h=pico->Ajuste.Ytau;
			double c=pico->Ajuste.Centroide;
			double ss=pico->Ajuste.SigAnchura;
//			double sh=pico->Ajuste.SigYtau;
			double sc=pico->Ajuste.SigCentroide;
			
			printf("Resolución [%f]=%f/%f=%f ---- %f\n", DaEnergia(pico->Ajuste.Centroide), s, c, s/c, DaFWHGCh(pico->Ajuste.Centroide) );
			/*printf("FWHM[%f]=%f\n",pico->Ajuste.Centroide, DaFWHM(nodo->numIntervalo));
			printf("FWTM[%f]=%f\n",pico->Ajuste.Centroide, DaFWTM(nodo->numIntervalo));
			printf("FWFM[%f]=%f\n",pico->Ajuste.Centroide, DaFWFM(nodo->numIntervalo));
			printf("FWTM/FWHM(1.9)=%f\tFWFM/FWHM(2.65)=%f\n", DaFWTM(nodo->numIntervalo)/DaFWHM(nodo->numIntervalo), DaFWFM(nodo->numIntervalo)/DaFWHM(nodo->numIntervalo));
			*/
			//Ahora Y y sY serán s/h y su incertidumbre
//			DatosY[k] = s/h;
//			SigmaY[k] = (s/h)*sqrt((ss/s)*(ss/s)+(sh/h)*(sh/h));

			//NOTA: NO está mal???, no sería s/c*s/c abajo???
			//DatosY[k] = (s/c)*(s/c);
			//SigmaY[k] = (s/c)*sqrt((ss/s)*(ss/s)+(sc/c)*(sc/c));
			DatosY[k] = (s/c);
			SigmaY[k] = (s/c)*(s/c)*sqrt((ss/s)*(ss/s)+(sc/c)*(sc/c));
			k++;
		i++;
		}
	Ajustes.ma = 2; Ajustes.a = coeficientes;
	Ajustes.afunc = vectorRecta; Ajustes.covar = covarRes;
	int nError = Ajustes.Ajusta();
	if (nError < 0) 
		{
		delete [] coeficientes;
		delete [] DatosX; delete [] DatosY; delete [] SigmaY;
		if (nError == -1) return -2;
		else return -1;
		}
	Ajustes.AjustaCovarianza();
	// ahora actualiza los valores de atau y btau
	bres = coeficientes[2]; ares = coeficientes[1];
	delete [] coeficientes;
	printf("La recta queda: y²=%f*x+%f\n", bres, ares);
	// elimina la memoria reservada y sale
	delete [] DatosX; delete [] DatosY; delete [] SigmaY;
	return 0;
	}
//--------------------------------------------------------
	
//********************** DA ENERGÍA *******************
// Devuelve la energía correspondiente al canal "channel"
// Si aún no se ha realizado la precalibración devuelve cero
// Si se ha realizado, las variables enp1-3 tienen valores que se
// usan para determinar cuál es la energía de un canal cualquiera
// NOTA: enp1-3 se calculan a partir del ajuste de rectas.
// NOTA: Parece ser que esa fórmula es: E=(-enp2 + sqrt( enp2²-4*enp3*(enp1-canal) )) / (2*enp3);
//	Yo diría que nos devuelve la solución de una ecuación de 2º grado (si hay calibración) o de
//	1º grado si no la hay.

double cEspectros::DaEnergia(double channel) 
	{
	if(channel < 0) {printf("ERROR: Canal negativo\n"); return -1;}
	double temp;
	
	if (enp3 == 0) 
		{ // no hay calibracion
		if (enp2 == 0) // no hay precalibracion
			return 0.0;
		//Sólo tenemos la precalibración
		temp = (channel - enp1) / enp2;
		if (temp < 0) return 0.0;
		return temp;
		}
	//Si tenemos la calibración completa:
	temp = enp2*enp2 - 4*enp3*(enp1-channel);
	if (temp < 0) return 0.0;
	temp = sqrt(temp);
	temp = (-enp2 + temp)/(2*enp3);
	
	/*
	double e, eant;
	double precision=0.001;
	eant=temp;
	e=0;
	
	while(e-eant>precision*e)
		{
		if(e!=0)	eant=e;
		e=(channel-enp1)/enp2 + (enp3*eant*eant)/enp2 + enp4/(enp2*eant);
		printf("e vale %f y eant es %f\n", e, eant);
		}
	
	temp=e;*/
	return (temp > 0.0) ? temp : 0.0;
	}

//*********** DA TOTAL CUENTAS *********
//Devuelve la suma de todas las cuentas en [c1,c2) 
int cEspectros::DaTotalCuentas(int c1, int c2)
	{
	int sum=0;
	for(int i=c1; i<c2;i++)		sum+=espectro[i];
	return sum;
	}
	
//Devuelve la suma total de cuentas de todo el espectro
int cEspectros::DaTotalCuentas()
	{
	return DaTotalCuentas(1,numCanales);	
	}
	
double cEspectros::DaCanal(double energia) 
	{
	double temp;
	
	if (enp3 == 0) 
		{ // no hay calibracion
		if (enp2 == 0) // no hay precalibracion
			return 0.0;
		temp = enp2*energia + enp1;
		if (temp < 0) return 0.0;
		return temp;
		}
	//temp = enp3*powi(energia,2) + enp2*energia + enp1;
	temp = enp3*powi(energia,2) + enp2*energia + enp1+(1.0/energia)*enp4;
	return (temp > 0.0) ? temp : 0.0;
	}

//******************* DA INCER ENERGIA ******************
double cEspectros::DaIncerEnergia(double channel) 
	{
	double temp;
	
	temp = enp2*enp2 - 4*enp3*(enp1-channel);
	if (temp < 0) return 0.0;
	temp = sqrt(temp);

	double Ea = (enp1-channel)/(temp*enp3) + (-enp2+temp)/(2.0*enp3*enp3);
	Ea = powi(Ea*covarEnergia[15],2);
	double Eb = (-1.0 + enp2/temp)/(2.0*enp3);
	Eb = powi(Eb*covarEnergia[10],2);
	double Ec = 1/temp;
	Ec = powi(Ec*covarEnergia[5],2);
	temp = Ea + Eb + Ec;
	
	if (temp < 0) return 0.0;

	return sqrt(temp);	
	}

//***************** DA FWHG CH ****************
// Nos devuelve el FWHG de un determinado canal.
// Para ello usa las variables afwhg y bfwhg que se calculan durante la precalibración (PreCalibra())
// FWHG(x)=sqrt(afwhg+bfwhg*x)
double cEspectros::DaFWHGCh(double channel) 
	{
	double temp = afwhg + bfwhg*channel;
	
	if (temp < 0) return 0.0;
	return sqrt(temp);
	}

//***************** DA FWHG *******************
double cEspectros::DaFWHG(double channel) 
	{
	// obtiene la anchura en canales
	double temp = DaFWHGCh(channel);
	// la pasa a energia
	temp = (DaEnergia(channel + temp) - DaEnergia(channel - temp))/2.0;
	
	return temp;
	}

//*********** DA INCER FWHG CH ****************
double cEspectros::DaIncerFWHGCh(double channel) 
	{
	double temp = afwhg + bfwhg*channel;
	
	if (temp <= 0) return 0.0;
	temp = (powi(covarSigma[4], 2) + powi(covarSigma[8], 2)*channel*channel)/(4.0*temp);
	if (temp < 0) return 0.0;
	return sqrt(temp);
	}

//**************** DA INCER FWHG ****************
double cEspectros::DaIncerFWHG(double channel) 
	{
	// obtiene la incertiumbre de la anchura en canales
	double temp = DaIncerFWHGCh(channel);
	// la pasa a energia
	temp = (DaEnergia(channel + temp) - DaEnergia(channel - temp))/2.0;
	
	return temp;
	}

/*	************************** DA FWHM ************************
	Indica el número de canales en [c1,c2] que tienen más cuentas que la mitad 
	del canal que tenga el máximo de cuentas en [c1,c2]
	//NOTA: De momento estamos contando valores enteros, no reales, es decir,
			no estamos teniendo en cuenta el ajuste, sólo las cuentas
	*/
double cEspectros::DaFWHM(int numIntervalo)
	{
	NodoPicos *intervalo = DevuelveNodo(listaPicos, numIntervalo);
	NodoPico *pico=DevuelveNodo(intervalo->picos,1);
	if(pico->Ajuste.ajustado)
		{
		//double max=DaEnergia(pico->Ajuste.Centroide);
		double max=espectro[(int)pico->Ajuste.Centroide];
		int li=intervalo->limInferior;
		int ls=intervalo->limSuperior;
		double em=max/2;
		int fwhm=0;
		for(int i=li;i<ls;i++)	if(espectro[i]>em)	fwhm++;
		return fwhm;
		}
	else	
		{
		printf("ERROR: Pico no ajustado\n");
		return -1;
		}
	}
	
/*	************************** DA FWTM ************************
	Indica el número de canales en [c1,c2] que tienen más cuentas que la mitad 
	del canal que tenga el máximo de cuentas en [c1,c2]
	//NOTA: De momento estamos contando valores enteros, no reales, es decir,
			no estamos teniendo en cuenta el ajuste, sólo las cuentas
	*/
double cEspectros::DaFWTM(int numIntervalo)
	{
	NodoPicos *intervalo = DevuelveNodo(listaPicos, numIntervalo);
	NodoPico *pico=DevuelveNodo(intervalo->picos,1);
	if(pico->Ajuste.ajustado)
		{
		//double max=DaEnergia(pico->Ajuste.Centroide);
		double max=espectro[(int)pico->Ajuste.Centroide];
		int li=intervalo->limInferior;
		int ls=intervalo->limSuperior;
		double em=max/10;
		int fwhm=0;
		for(int i=li;i<ls;i++)	if(espectro[i]>em)	fwhm++;
		return fwhm;
		}
	else	
		{
		printf("ERROR: Pico no ajustado\n");
		return -1;
		}
	}
/*	************************** DA FWFM ************************
	Indica el número de canales en [c1,c2] que tienen más cuentas que la mitad 
	del canal que tenga el máximo de cuentas en [c1,c2]
	//NOTA: De momento estamos contando valores enteros, no reales, es decir,
			no estamos teniendo en cuenta el ajuste, sólo las cuentas
	*/
double cEspectros::DaFWFM(int numIntervalo)
	{
	NodoPicos *intervalo = DevuelveNodo(listaPicos, numIntervalo);
	NodoPico *pico=DevuelveNodo(intervalo->picos,1);
	if(pico->Ajuste.ajustado)
		{
		//double max=DaEnergia(pico->Ajuste.Centroide);
		double max=espectro[(int)pico->Ajuste.Centroide];
		int li=intervalo->limInferior;
		int ls=intervalo->limSuperior;
		double em=max/50;
		int fwhm=0;
		for(int i=li;i<ls;i++)	if(espectro[i]>em)	fwhm++;
		return fwhm;
		}
	else	
		{
		printf("ERROR: Pico no ajustado\n");
		return -1;
		}
	}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//***************** DA RESOLUCIÓN ****************
// Nos devuelve el tanto por uno de resolución tolerado para un determinado canal.
// Para ello usa las variables afwhg y bfwhg que se calculan durante la precalibración (PreCalibra())
// FWHG(x)=sqrt(afwhg+bfwhg*x)
//NOTA: Fase de pruebas	
double cEspectros::DaResolucionCh(double channel) 
	{
	double temp = ares + bres*channel;
	if (temp < 0) return 0.0;
	return sqrt(temp);
	//printf("enp2: %f, enp3: %f\n", enp2, enp3);
	//return DaFWHG(channel)*(enp2+enp3*(2*channel-1));
	//return DaFWHGCh(channel)*(enp2+enp1*(2*channel-1)-enp3*channel*channel);
	//printf("resolución=%f*%f\n", DaFWHG(channel), enp2);
	//return DaFWHGCh(channel)*enp2;
	return DaSigmaCh(channel)*2.35/channel;
	}

double cEspectros::DaResolucion(double channel) 
	{
	// obtiene la anchura en canales
/*	double temp = DaResolucionCh(channel);
	// la pasa a energia
	temp = (DaEnergia(channel + temp) - DaEnergia(channel - temp))/2.0;
		return temp;
		*/
	return DaEnergia(DaResolucionCh(channel));
	//return DaSigma(channel)*2.35/channel;
	}

double cEspectros::DaIncerResolucionCh(double channel) 
	{
	double temp = ares + bres*channel;
	
	if (temp <= 0) return 0.0;
	temp = (powi(covarRes[4], 2) + powi(covarRes[8], 2)*channel*channel)/(4.0*temp);
	if (temp < 0) return 0.0;
	return sqrt(temp);
	}

double cEspectros::DaIncerResolucion(double channel) 
	{
	// obtiene la incertiumbre de la anchura en canales
	double temp = DaIncerResolucionCh(channel);
	// la pasa a energia
	temp = (DaEnergia(channel + temp) - DaEnergia(channel - temp))/2.0;
	
	return temp;
	}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	
//*************** DA SIGMA CH ********************
//Nos devuelve la sigma asociada a un canal en canales
//NOTA: la fórmula exacta (sobre todo la constante) no sé de dónde salen
double cEspectros::DaSigmaCh(double channel) 
	{
	double temp = (afwhg + bfwhg*channel)/2.3548;
	
	if (temp < 0) return 0.0;
	return sqrt(temp);
	}

//************ DA SIGMA **************
/* Devuelve la sigma en energía, a partir de
	un determinado canal
	*/
double cEspectros::DaSigma(double channel) 
	{
	// obtiene la sigma en canales
	double temp = DaSigmaCh(channel);
	// la pasa a energia
	temp = (DaEnergia(channel + temp) - DaEnergia(channel - temp))/2.0;
	return temp;
	}

//********************* DA ANCHO ****************************
/** Calcula la distancia desde canal hasta ancho, en keV
*/
double cEspectros::DaAncho(double channel, double ancho) 
	{
	// pasa la anchura en canales a energia
	double temp = (DaEnergia(channel + ancho) - DaEnergia(channel - ancho))/2.0;
	return temp;
	}

double cEspectros::DaIncerSigmaCh(double channel) 
	{
	double temp = DaIncerFWHGCh(channel);
	return temp/sqrt(2.3548);
	}

double cEspectros::DaIncerSigma(double channel) {
	// obtiene la incertidumbre de la sigma en canales
	double temp = DaIncerSigmaCh(channel);
	// la pasa a energia
	temp = (DaEnergia(channel + temp) - DaEnergia(channel - temp))/2.0;
	
	return temp;
}

double cEspectros::DaTauCh(double channel) 
	{
	double temp = (atau + btau*channel);
	
	if (temp < 0) return 0.0;
	return temp;
	}

//********** DA TAU **************
/*	Devuelve la tau correspondiente al canal, según la calibración realizada
	y en forma de energía
*/
double cEspectros::DaTau(double channel) 
	{
	// obtiene el tau en canales
	double temp = DaTauCh(channel);
	// lo pasa a energia
	temp = (DaEnergia(channel + temp) - DaEnergia(channel - temp))/2.0;
	
	return temp;
	}

double cEspectros::DaIncerTauCh(double channel) 
	{
//	double temp = covarTau[4] + channel*channel*covarTau[8];//NOTA: Creemos que falta el término cruzado
	double temp = covarTau[4] + channel*channel*covarTau[8] + 2*channel*covarTau[5];
//	printf("Valor nuevo vale %f\n", 2*channel*covarTau[5]);
	if (temp < 0) return 0.0;
	return sqrt(temp);
	}

double cEspectros::DaIncerTau(double channel) 
	{
	// obtiene la incertidumbre de tau en canales
	double temp = DaIncerTauCh(channel);
	// la pasa a energia
	temp = (DaEnergia(channel + temp) - DaEnergia(channel - temp))/2.0;

	return temp;
	}


/********************************************************************************
 ********************************************************************************
 ******************************    CONTINUO     *********************************
 ********************************************************************************
 ********************************************************************************/
 
 //********************* FILTRO ************************
//Realiza un filtrado para un determinado canal (nPos).
double cEspectros::Filtro(unsigned short nPos, bool CalibracionForma) 
	{
	double Correc = (CalibracionForma) ? 1.88 : 1.00;

	// ancho en canales de un pico para el canal indicado
	// si esta definido manphe se queda con el valor de Manphe
	int nAncho = (int) DevuelveValorListaInterv(serieManphe, nPos);
	// si no está definido vamos a hacer una estimación en función a los datos obtenidos durante la precalibración.
	if (nAncho < 0) nAncho = redondea(DaFWHGCh((double)nPos)*Correc);
	if (nAncho == 0) nAncho = 1; // establece 1 canal como minimo
	
	// *** NUEVO ***
	// desplaza el centro del pico para hacer el filtro simetrico
	nPos -= (int) (nAncho+1)/2 - 1;
	// *****


	// pasa el filtro por el espectro
	//NOTA: ¿Qué pasa si nPos es un valor cercano a cero y luego alguna i nos queda negativa?
	//	que esta función Filtro devolvería un null o algo así
	//NOTA: Efectivamente para nPos muy bajo tenemos valores negativos pero parece que
	//	espectros[-x] da cero, con lo que simplemente esos valores tendrían menos valores
	//	a un lado para calcular su filtro
	//NOTA: Preguntar a Begoña si esto sería válido
//	printf("Límite a partir del que hacemos el filtrado: %d", nPos-nAncho);
	
	
	double suavizado = 0.0, varianza = 0.0;
	for (int i = nPos - nAncho; i <= nPos - 1; i++) 
		{
		suavizado -= (double) espectro[i];
		suavizado +=  (double) (2*espectro[i + nAncho]);
		suavizado -= (double) espectro[i + 2*nAncho];
		varianza += (double) espectro[i];
		varianza +=  (double) (4*espectro[i + nAncho]);
		varianza += (double) espectro[i + 2*nAncho];
		}

	// si los canales valen 0 devuelve un valor muy alto
	if (varianza == 0.0) suavizado = HUGE*signo(suavizado);
	else		     suavizado = suavizado/sqrt(varianza);
	
	return suavizado;
	}

//****************************** BUSCA CANALES CONTINUO ***********************
//Busca todos los canales del espectro que forman parte del continuo según la información
//que tiene almacenadas en sus listas serieCofo, serieCopi y serieManphe.
//El parámetro lógico CalibracionForma que se le pasa indica si es necesario realizar un ajuste
//de corrección de 1.88 o no (la anchura de una gaussiana suele ser 1.88 veces mayor que la gaussiana
//convolucionada con una exponencial)
//
int cEspectros::BuscaCanalesContinuo(bool CalibracionForma) 
	{
	//printf("BUSCA CANALES CONTINUO +++++++++++++++++++\n");
	// cuando se ajusta a una gaussiana, la anchura suele ser 1.88 veces
	// mayor que la gaussiana convolucionada con la exponencial
	double Correc = (CalibracionForma) ? 1.88 : 1.00;
	if (!completitudListaInterv(serieCofo)) return -2;
	if (!completitudListaInterv(serieCopi)) return -2;
	
	//Calculamos los límites inferior y superior de los intervalos de los canales en cuanto
	//a Cofo y Copi.
	int ia1, ib1, ia2, ib2;
	DevuelveAlcanceListaInterv(serieCofo, ia1, ib1);
	DevuelveAlcanceListaInterv(serieCopi, ia2, ib2);
	if ((ia1 != ia2)||(ib1 != ib2)) return -3;
	minInterv = ia1; maxInterv = ib1;
	//printf("minInterv=%d, maxInterv=%d\n", minInterv, maxInterv);
	// reserva la memoria necesaria para poner los canales de fondo
	// y los canales de los picos
	borraLista(listaPicos);
	numPicosEncontrados = 0;
	delete [] fondo; delete [] picos;
	numCanalesFondo = numCanales/8 + 1;//NOTA: Lo hace para ahorrar memoria pues va a meter en los array de caracteres
						// de picos y de fondos, cada canal como un bit
						// Yo creo que es mucho armar para conseguir poca eficiencia, pero no lo
						// voy a cambiar ahora que está hecho
	if ((fondo = new unsigned char[numCanalesFondo]) == NULL)
		{
		printf("ERROR: No hay memoria para reservar unsigned char[%d]\n", numCanalesFondo);
		return -4;
		}
	if ((picos = new unsigned char[numCanalesFondo]) == NULL) {
		{
		printf("ERROR: No hay memoria para reservar unsigned char[%d]\n", numCanalesFondo);
		delete [] fondo; fondo = NULL;
		return -4;
		}
	}
	// inicializa los canales de fondo y de picos
	for (int i = 0; i < numCanalesFondo; i++) 
		{
		fondo[i] = 0; picos[i] = 0;
		}
	
	// mueve los intervalos de busqueda hasta que haya al menos 2 canales
	// de fondo seguidos
	//NOTA: OJO: minInterv puede ser un valor bastante bajo (1,10,20...) con lo que el filtro, que necesita bajar
	//	hasta minInterv-nAncho/2 puede llegar a ser menor que cero. Aunque no da error porque parece que los
	//	elementos negativos de un array nos los coge a cero, sí es cierto que para algunos valores bajos quizá
	//	el filtrado no sea demasiado correcto
	
	//Mientras los valores de filtrado sean mayores en dos valores consecutivos que los de cofo, avanzamos.
	while (((fabs(Filtro(minInterv, CalibracionForma)) > DevuelveValorListaInterv(serieCofo, minInterv))||
		(fabs(Filtro(minInterv+1, CalibracionForma)) > DevuelveValorListaInterv(serieCofo, minInterv+1)))
		&&(minInterv < maxInterv)) 
		{
		minInterv++;
		}
		
	if (minInterv == maxInterv) 
		{//No hemos encontrado continuo
		maxInterv = minInterv;
		return -5;
		}
		
	//Hacemos la pasada en sentido inverso
	while (((fabs(Filtro(maxInterv-1, CalibracionForma)) > DevuelveValorListaInterv(serieCofo, maxInterv-1))||
		(fabs(Filtro(maxInterv, CalibracionForma)) > DevuelveValorListaInterv(serieCofo, maxInterv)))
		&&(maxInterv > minInterv)) 
		{
		maxInterv--;
		}
	if (maxInterv == minInterv) 
		{
		minInterv = maxInterv;
		return -5;
		}
	
	// Pasa el filtro por el intervalo de busqueda, seleccionando como
	// fondo los que esten por debajo de Cofo y como picos los que esten
	// por encima de Copi
	for (int i = minInterv; i <= maxInterv; i++) 
		{
		double filtrado = Filtro(i, CalibracionForma);
		if (filtrado >= DevuelveValorListaInterv(serieCopi, i))
			SetChannel(picos, i);
		else if (fabs(filtrado) <= DevuelveValorListaInterv(serieCofo, i))
			SetChannel(fondo, i);
		}
	
	// Borra los canales del continuo que esten aislados
	for (int i = minInterv + 1; i < maxInterv; i++)
		if (IsChannelContinuum(i))
			if ((!IsChannelContinuum(i-1))&&(!IsChannelContinuum(i+1)))
				UnsetChannel(fondo, i);

	//  *** NUEVO ***
	// Si queda un canal aislado entre dos intervalos con canales de pico
	// añade el canal a los canales de pico
	for (int i = minInterv+1; i < maxInterv; i++)
		if ((!IsChannelContinuum(i))&&(!IsChannelPeaks(i)))
			if (IsChannelPeaks(i-1)&&IsChannelPeaks(i+1))
				SetChannel(picos, i);

	// busca y guarda todos los picos encontrados en la siguiente lista temporal
	// hasta que pueda organizarlos en su listado correspondiente mas
	// adelante
	tListaDatosPicos PicosTemp;
	PicosTemp.numElementos = 0; PicosTemp.inicio = NULL;
	if (BuscaPicos(PicosTemp, CalibracionForma) < 0) 
		{
		delete [] fondo; delete [] picos;
		fondo = NULL; picos = NULL;
		return -4;
		}
	
		
	// Se asegura que no haya canales de fondo metidos en las estructuras
	// de los picos eliminandolos en una distancia sigma*1.5 a cada lado
	for (int i = 1; i <= PicosTemp.numElementos; i++) {
		// recoge los datos del pico y su anchura
		NodoPico *puntero = DevuelveNodo(PicosTemp, i);
		int centro = redondea(puntero->maximo);
		int nAncho = redondea(1.5*DaSigmaCh(puntero->maximo)*Correc);
		if (nAncho == 0) nAncho = 1;
		
		// quita los canales de fondo por la izquierda (sin pasarse de los límites del intervalo)
		for (int j = 0; j <= nAncho; j++)
			if ((centro-j) > minInterv+1)
				UnsetChannel(fondo, centro - j);
		
 		// *** NUEVO ***
		// quita los canales de fondo por la derecha (sin pasarse de los límites del intervalo)
		for (int j = 1; j <= nAncho; j++)
			if ((centro + j) < maxInterv-1)
				UnsetChannel(fondo, centro + j);
			
 		// *** NUEVO ***
		// Vuelve a borrar los canales del continuo que esten aislados (puede que nos haya salido
		// alguno nuevo después de quitarlos de la zona de los picos).
		for (int i = minInterv+1; i < maxInterv; i++)
			if (IsChannelContinuum(i))
				if ((!IsChannelContinuum(i-1))&&(!IsChannelContinuum(i+1)))
					UnsetChannel(fondo, i);

		//NOTA: Esto de abajo estaba comentado pero igual no es mala idea.
/*		// *** NUEVO ***
		// rodea el maximo del pico con canales de picos
		nAncho = redondea(DaFWHGCh(puntero->maximo));
		if (nAncho == 0) nAncho = 1;
		for (int j = 1; j < nAncho; j++) {
			SetChannel(picos, centro - j);
			SetChannel(picos, centro + j);
		}
*/
		// tambien se podrian añadir estos canales como canales de pico
		// y volver a hacer una busqueda de los maximos (para añadir picos
		// en estructuras anchas)
		

	}
	
	if (PicosTemp.numElementos == 0) //No hemos encontrado ningún pico
		{ 
		delete [] fondo; delete [] picos;
		fondo = NULL; picos = NULL;
		return -6;
		}
	
	// Una vez encontrados todos los intervalos que contienen picos
	// se reconstruyen los intervalos para que guarden los picos
	// que hay entre canal y canal de continuo

	// Busca los canales de fondo
	int nPic = 1;
	int i = minInterv;
	while (!IsChannelContinuum(i)) i++;

	while (i <= maxInterv) 
		{
		//printf("Intervalo %d\n", i);
		// para cada intervalo busca el ultimo canal de continuo
		while (IsChannelContinuum(i)) i++;
		if (i <= maxInterv) 
			{
			// entre este canal y el siguiente de continuo hay un
			// intervalo que puede tener picos
			NodoPicos *nuevo;
			if (InicializaNodo(nuevo) < 0) 
				{
				delete [] fondo; delete [] picos; borraLista(PicosTemp);
				fondo = NULL; picos = NULL;
				borraLista(listaPicos);
				return -4;
				}
			nuevo->limInferior = i;
			//printf("nuevo->limInferior vale %d\n", i);
			// busca el final del intervalo de trabajo
			while (!IsChannelContinuum(i)) i++;
			nuevo->limSuperior = i-1;
			
			// busca los picos que hay encontrados que estén en ese intervalo
			// y los pone en la lista
			//printf("Buscamos el pico q caiga en el intervalo: %d\n", nPic);
			NodoPico *picoTemp = DevuelveNodo(PicosTemp, nPic);
			//printf("%d está en %d < %d\n", nPic, nuevo->limInferior, redondea(picoTemp->maximo));
			while ((nuevo->limInferior > redondea(picoTemp->maximo)) && (nPic < PicosTemp.numElementos)) 
				{//Pasamos todos los picos anteriores al intervalo en el que estamos
				nPic++;
				picoTemp = DevuelveNodo(PicosTemp, nPic);
				//printf("%d está en %d < %d\n", nPic, nuevo->limInferior, redondea(picoTemp->maximo));
				}
			int contPicos = 0;
			//printf("%d <= %.2f <= %d\n", nuevo->limInferior, redondea(picoTemp->maximo), nuevo->limSuperior);
			while ((nuevo->limSuperior >= redondea(picoTemp->maximo)) && (nPic <= PicosTemp.numElementos)) 
				{//Ahora cogemos todos los que se encuentren en nuestro intervalo
				//printf("%.2f <= %.2f <= %.2f\n", nuevo->limInferior, redondea(picoTemp->maximo), nuevo->limSuperior);
				//printf("%d <= %.2f <= %d\n", nuevo->limInferior, redondea(picoTemp->maximo), nuevo->limSuperior);
				if ((nuevo->limSuperior >= redondea(picoTemp->maximo)) && (nuevo->limInferior <= redondea(picoTemp->maximo))) 
				  	{
					// este pico se encuentra en el intervalo de trabajo
					// crea un clon y lo añade
					NodoPico *PicoNuevo = new NodoPico;
					if (PicoNuevo == NULL) 
						{
						printf("ERROR: No hay memoria para reservar NodoPico\n");
						delete [] fondo; delete [] picos; borraLista(PicosTemp);
						fondo = NULL; picos = NULL;
						borraLista(nuevo->picos); borraLista(listaPicos);
						return -4;
						}
					iniciaNodo(PicoNuevo);
					clonaNodo(picoTemp, PicoNuevo);
						//
					PicoNuevo->Ajuste.identificado=false;
						//
					agnadeNodo(nuevo->picos, PicoNuevo);
					contPicos++;
					}
				nPic++;
				picoTemp = DevuelveNodo(PicosTemp, nPic);
				}
			// si hay al menos 1 pico añadido entonces este intervalo de
			// de trabajo es util, se añade a la lista
			if (contPicos > 0)	
				{
				//printf("Añadimos si al menos hay uno, para no perder el intervalo\n");
				agnadeNodo(listaPicos, nuevo);
				}	
			else
				{
				//printf("Borramos el nuevo, no nos vale\n");
				delete nuevo;
				}
			}
		}//while hasta el final de los intervalos
	
	numPicosEncontrados = PicosTemp.numElementos;

	// limpia la memoria de las variables temporales
	borraLista(PicosTemp);
	borraLista(serieCofo); borraLista(serieCopi); borraLista(serieManphe);
	
	// por si acaso hay problemas en el final de la busqueda de canales del
	// continuo ajusta el maximo del intervalo a la posicion del ultimo canal
	// de continuo encontrado
	i = maxInterv;
	while (!IsChannelContinuum(i)) i--;
	maxInterv = i;
//	printf("FIN BUSCACANALES CONTINUO +++++++++++++++++++++++++++++\n");
	return 0;
	}

//********************* DEVUELVE LIMITES CONTINUO **************
/* Básicamente hace una llamada a DevuelveAlcanceListaInterv() de 
	continuo.c que por alguna extraña razón me da error si la llamo
	desde fuera de espectros.c (me dice que le paso un &lista en vez
	de una lista cuando estoy seguro de que no es así)
*/
void cEspectros::devuelveLimitesContinuo(int inicio, int fin) 
	{
	DevuelveAlcanceListaInterv(serieCofo, inicio, fin);
	}
	
//************************** BUSCA PICOS ********************************
//En función de los canales que pertenecen a picos que se han buscado con BuscaCanalesContinuo
//y se almacenan en la matriz picos, determinamos la identidad de cada pico y lo introducimos en 
//la lista que se pasa como parámetro.
int cEspectros::BuscaPicos(tListaDatosPicos& lista, bool CalibracionForma) {
	double Correc = (CalibracionForma) ? 1.88 : 1.00;

	// inicializa la lista
	borraLista(lista);

	int i = minInterv;
	int iTemp = 0;
	while (i < maxInterv) 
		{
		// busca el siguiente canal que es señalado como parte de un pico
		while ((!IsChannelPeaks(i))&&(i <= maxInterv)) i++;

		if (i <= maxInterv) 
			{
			// busca un maximo local del filtro sobre el espectro
			bool maxEncontrado = false;
			if ((!IsChannelPeaks(i-1))&&(!IsChannelPeaks(i+1))) //Si no son picos ninguno de sus vecinos es que él es el máximo
				maxEncontrado = true;
			else if ( (!IsChannelPeaks(i-1)) && (Filtro(i,CalibracionForma) >= Filtro(i+1, CalibracionForma)) )//si el de atrás no es pico y el de adelante es más pequeño
				maxEncontrado = true;
			else if ((!IsChannelPeaks(i+1))&&(Filtro(i,CalibracionForma) >= Filtro(i-1, CalibracionForma)))//si el de adelante no es pico y el de atrás es más pequeño
				maxEncontrado = true;
			else if (IsChannelPeaks(i-1)&&IsChannelPeaks(i+1)&&
				 (Filtro(i, CalibracionForma) >= Filtro(i-1, CalibracionForma))&&(Filtro(i, CalibracionForma) >= Filtro(i+1, CalibracionForma)))//si ambos son picos pero son más pequeños
				maxEncontrado = true;
				
			if (maxEncontrado) 
				{
				// busca el maximo real en el espectro para añadirlo a la lista
				// rastreando una estructura de ancho tipico de pico
				//Es decir, busca en un intervalo de i+-ancho un canal con valor
				//mayor que el máximo encontrado para llamarlo máximo
				int nAnchoTemp = redondea(DaFWHGCh((double)i)*Correc/2.0);
				if (nAnchoTemp <= 0) nAnchoTemp = 1;

				int MaximoTemp = espectro[i];//Empezamos cogiendo como máximo el que teníamos
				int canalTemp = 0;
				// por la izquierda y por la derecha
				for (int j = -nAnchoTemp+1; j <= nAnchoTemp-1; j++)
					if (espectro[i+j] > (unsigned int)MaximoTemp) 
						{
						canalTemp = j;
						MaximoTemp = espectro[i+j];
						}

/*				// si el canal encontrado es el mismo que el canal anterior
				// busca el segundo maximo local
				if (iTemp == i + canalTemp) {
					int MaximoTemp2 = espectro[iTemp-1]; int canalTemp2 = canalTemp-1;
					// por la izquierda y por la derecha
					for (int j = -nAnchoTemp+1; j <= nAnchoTemp-1; j++)
						if ((espectro[i+j] > MaximoTemp2)&&
							 (espectro[i+j] < MaximoTemp)) {
							canalTemp = j;
							MaximoTemp = espectro[i+j];
						}
					iTemp = i + canalTemp2;
				}
				else
					iTemp = i + canalTemp;*/
				iTemp = i + canalTemp;
				
				// ahora crea el nodo, introduce los datos y lo añade a la lista
				NodoPico *nuevo = new NodoPico;
				if (nuevo == NULL) 
					{
					printf("ERROR: No hay memoria para reservar NodoPico\n");
					delete [] fondo; delete [] picos;
					fondo = NULL; picos = NULL;
					borraLista(lista);
					return -1;
					}
				
				iniciaNodo(nuevo);
				nuevo->maximo = (double) iTemp;
				agnadeNodo(lista, nuevo);
				if (iTemp > i) i = iTemp;//para evitar la búsqueda de otros máximos en el mismo trozo
				i++;
				}//del if
			else 	i++; // no es maximo local, pasa al siguiente
			}
		}
	return 0;
}

//************************** SET CHANNEL *******************************
// Mete el canal en una matriz. La matriz hemos dicho que tiene una capacidad
// de 1/8 con respecto al espectro completo. 
// Como 8 bits es un byte, lo que hacemos es activar este bit en concreto de la posición 
// donde le toque
// NOTA: Me parece que esto es mucho comerse la cabeza para una ganancia pequeña de espacio
// 	al fin y al cabo sólo dividimos entre 8 el espacio y preparamos un jaleo de para qué
//	contarte, cuando con una matriz de booleanos más grande pero de menos espacio cada elemento
//	tendríamos lo mismo.
void cEspectros::SetChannel(unsigned char *matriz, unsigned short nPos) {
	// recoge el byte asociado a ese canal
	unsigned char valor = matriz[nPos/8];
	unsigned char nBit = nPos % 8;
	unsigned char mascara = 0x1 << nBit;
	matriz[nPos/8] = valor | mascara; //matriz[nPos/8]=
}

//************************** UNSET CHANNEL *******************************
// Lo mismo pero para quitarlos
void cEspectros::UnsetChannel(unsigned char *matriz, unsigned short nPos) {
	// recoge el byte asociado a ese canal
	unsigned char valor = matriz[nPos/8];
	unsigned char nBit = nPos % 8;
	unsigned char mascara = 0x1 << nBit;
	mascara = mascara ^ 0xFF;
	fondo[nPos/8] = valor & mascara;
}

//*************** IS CHANNEL CONINUUM *******************
//Indica si un determinado canal está en el array asociado al fondo.
bool cEspectros::IsChannelContinuum(unsigned short nPos) {
	// recoge el byte asociado a ese canal
	unsigned char valor = fondo[nPos/8];
	unsigned char nBit = nPos % 8;
	unsigned char mascara = 0x1 << nBit;
	valor = valor & mascara;
	if (valor != 0) return true;
	return false;
}

//*************** IS CHANNEL CONINUUM *******************
//Indica si un determinado canal está en el array asociado al fondo.
bool cEspectros::IsChannelPeaks(unsigned short nPos) {
	// recoge el byte asociado a ese canal
	unsigned char valor = picos[nPos/8];
	unsigned char nBit = nPos % 8;
	unsigned char mascara = 0x1 << nBit;
	valor = valor & mascara;
	if (valor != 0) return true;
	return false;
}


//*****************************************************************************
//******************** PREPARA AJUSTE CANALES CONTINUO ************************
// Realiza el ajuste de continuo cuando se le llama desde pulsa_ajustar_continuo() de 
// VisConti.C, al pulsar el botón Adjust tras haber realizado la búsqueda del continuo

int cEspectros::PreparaAjusteCanalesContinuo() {
	// limpia la estructura de la spline del continuo
	borraLista(SplineContinuo);
	
	// Pone 4 nodos, dos a cada extremo del intervalo en el que se lleva
	//a cabo el ajuste.
	NodoBSpline *nuevo = new NodoBSpline;
	if(nuevo==NULL)
		{
		printf("ERROR: No hay memoria para reservar NodoBSpline\n");
		return -2;
		}
	nuevo->posic = minInterv;
	agnadeNodo(SplineContinuo.listaNodos, nuevo);
	nuevo = new NodoBSpline;
	nuevo->posic = minInterv;
	agnadeNodo(SplineContinuo.listaNodos, nuevo);
	nuevo = new NodoBSpline;
	nuevo->posic = maxInterv;
	agnadeNodo(SplineContinuo.listaNodos, nuevo);
	nuevo = new NodoBSpline;
	nuevo->posic = maxInterv;
	agnadeNodo(SplineContinuo.listaNodos, nuevo);
	
	// Ajusta la estructura del spline
	if (DimensionaBSpline(SplineContinuo) < 0) 
		{
		borraLista(SplineContinuo);
		return -2;
		}
	
	// crea los datos que hay que ajustar, que es el logaritmo del
	// espectro de los canales de fondo
	// Cuenta los canales de fondo
	numFondo = 0;
	for (int i = 1; i <= numCanales; i++)	if (IsChannelContinuum(i))	numFondo++;

	// crea los vectores que contendran los datos X, Y y la sigma de la Y del fondo
	// a los que hay que ajustar la Spline
	ValorY = new double[numFondo];
	if (ValorY == NULL) 
		{
		printf("ERROR: No hay memoria para reservar double[%d]\n", numFondo);
		borraLista(SplineContinuo);
		return -2;
		}
	ValorX = new double[numFondo];
	if (ValorX == NULL) 
		{
		printf("ERROR: No hay memoria para reservar double[%d]\n", numFondo);
		delete [] ValorY;
		borraLista(SplineContinuo);
		return -2;
		}
	SigmaY = new double[numFondo];
	if (SigmaY == NULL) 
		{
		printf("ERROR: No hay memoria para reservar double[%d]\n", numFondo);
		delete [] ValorY; delete [] ValorX;
		ValorY=ValorX=NULL;
		borraLista(SplineContinuo);
		return -2;
		}
	// En el vector ValorX pone los canales que son de fondo 
	int j = 0;
	for (int i = 1; i <= numCanales; i++)
		if (IsChannelContinuum(i)) 
			{
			ValorX[j] = (double) i;
			j++;
			}

	// En el vector ValorY pone el logaritmo del valor del canal indicado
	// en el vector ValorX. Si el canal tiene 0 cuentas le asocia el valor
	// 0.01 y le pone como sigma 1
	for (int i = 0; i < numFondo; i++) 
		{
		int temp = redondea(ValorX[i]);
		if (espectro[temp] == 0) 
			{
			ValorY[i] = log(0.01);
			//NOTA: Este log se puede cambiar por su valor pues va a ser siempre constante (-2 si es en base 10,
			//	 o -4.60517 si es neperiano)
			SigmaY[i] = 1.0;
			}
		else 	
			{
			ValorY[i] = log((double) espectro[temp]);
			SigmaY[i] = 1/sqrt((double) espectro[temp]);//La varianza es 1/raizcuadrada del valor para N>>.
			}
		}
	
	// Control de gasto en tiempo de ejecucion
	principioTiempo = clock();
	
	// asocia las matrices al objeto que ajusta los datos
	cAjusteLinealChi* Spline = new cAjusteLinealChi;
	if(Spline==NULL)
		{
		printf("ERROR: No hay memoria para reservar cAjusteLinealChi\n");
		return -2;
		}
	ajustes = (void*) Spline;
	
	Spline->x = ValorX; Spline->y = ValorY; Spline->sigma = SigmaY;
	Spline->numDatos = numFondo;
	Spline->primerNodo = (NodoNodo*) SplineContinuo.listaNodos.inicio->siguiente;
	Spline->AsociaFuncionActualiza((tFuncion) ActualizaVectorContinuo);
	//NOTA: Parece ser algo del ajuste lo que hace que el cálculo aumente bastante
	//	De momento parece que no es nada de esta función !!!
	
	return 0;
}

//********************* CICLO AJUSTE CANALES CONTINUO *********************
// Realiza los ajustes del continuo. Cada llamada corresponderá a un ciclo o iteración
//
int cEspectros::CicloAjusteCanalesContinuo() 
	{
	// Objeto encargado de hacer los ajustes
	cAjusteLinealChi *Spline = (cAjusteLinealChi *) ajustes;

	// esta variable indica si es necesario incluir mas nodos en este ciclo
	int MasNodos = 0;

	// crea el vector de tendencias. Partiendo de los nodos que hay crea
	// los intervalos entre ellos y a cada intervalo le asigna su tendencia
	// tendenciasCh guarda el numero de canales de fondo que hay en cada
	// intervalo
	double *tendencias = new double[SplineContinuo.numElementos];
	//NOTA: En principio el número de elementos de SplineContinuo será de 4 si se le llamó desde PreparaAjusteCanalesContinuo
	int *tendenciasCh = new int[SplineContinuo.numElementos];
	if ((tendencias == NULL)||(tendenciasCh == NULL)) 
		{
		printf("ERROR: No hay memoria para reservar int[%d]\n", SplineContinuo.numElementos);
		delete [] tendencias; delete [] tendenciasCh;
		delete [] ValorY; delete [] ValorX; delete [] SigmaY;
		ValorY=ValorX=SigmaY=NULL;
		borraLista(SplineContinuo);
		return -2;
		}
		
	// asocia los datos que varian en cada ciclo al objeto que hace
	// el ajuste. Estos datos varian ya que en cada ciclo, al añadirse
	// nuevos nodos, las matrices y vectores se redimensionan.
//	printf("SplineContinuo.covar = %f\n", SplineContinuo.covar[0]); 
	Spline->ma = SplineContinuo.numElementos;
	Spline->a  = SplineContinuo.peso; 
	Spline->afunc = SplineContinuo.intervalo;
	Spline->covar = SplineContinuo.covar;

	// *** AJUSTA LOS DATOS A LA SPLINE ***
	int nError = Spline->Ajusta();
	if (nError < 0) 
		{
		delete [] tendencias; delete [] tendenciasCh;
		delete [] ValorY; delete [] ValorX; delete [] SigmaY;
		ValorY=ValorX=SigmaY=NULL;
		borraLista(SplineContinuo);
		if (nError == -1) return -2;
		else return -1;
		}
		
	//Modificamos los datos de la chi del continuo a partir de la chi cuadrada
	ChiSqContinuo = Spline->ChiSq/(numFondo-SplineContinuo.numElementos);

	// calcula la tendencia del nuevo ajuste
	Spline->Tendencia(tendencias, tendenciasCh);
	
	// ajusta el vector de tendencias: si un intervalo tiene menos puntos
	// entre nodo y nodo que la fwhm, entonces anula su tendencia para
	// evitar que se sobrecargue de nodos
	//NOTA: El método teórico (Morton, 88) dice que si el número de nodos es menor que el umbral,
	//	se divide un intervalo adyacente, en vez de simplemente ignorar la tendencia.
	//	Eso es lo que vamos a hacer en lugar de esto para ver si mejora la representación del
	//	continuo.
	
	/****** ANTIGUO ***
	for (int i = 1; i < SplineContinuo.numElementos - 1; i++) 
		{
		int extIzq = ValorNodo(SplineContinuo.listaNodos, i+1);
		int extDer = ValorNodo(SplineContinuo.listaNodos, i+2);
		int nAncho = redondea((DaFWHGCh((double)extIzq)+DaFWHGCh((double)extDer))/2.0);
		if (tendenciasCh[i-1] < nAncho)		tendencias[i-1] = 1.0;
		}
		
	// Busca el maximo del vector de tendencias
	double tendencia = tendencias[0];
	int intervMax = 1;
	for (int i = 2; i < SplineContinuo.numElementos - 1; i++)
		if (tendencias[i-1] > tendencia) 
			{
			tendencia = tendencias[i-1];
			intervMax = i;
			}

	
	***/
	//************* NUEVO (MORTON) *********************
	// Busca el maximo del vector de tendencias
	// NOTA: Este bucle inicial también se puede meter en la función de más abajo.
	double tendencia = tendencias[0];
	int intervMax = 1;
	for (int i = 2; i < SplineContinuo.numElementos - 1; i++)
		if (tendencias[i-1] > tendencia) 
			{
			//Si la tendencia es mayor, la ponemos como la tendencia máxima
			tendencia = tendencias[i-1];
			intervMax = i;
			}
	
	//Vamos a ver ahora si el intervalo en el que estamos es suficientemente
	//grande para albergar otro nudo. Si no lo es, elegimos uno adyacente
//	printf("intervalo a modificar: %d\n", intervMax);
	intervMax = intervaloTendencia(intervMax, tendencias, tendenciasCh, SplineContinuo.listaNodos);
//	printf("intervalo tras comprobación: %d\n", intervMax);
	//OJO: Por tanto, puede ser que la tendencia no se refiera al mismo intervalo en el que se calculó.
	//NOTA: Tal y como está ahora, si el intervalo es muy pequeño y los adyacentes también, nos devuelve
	//	-1, con lo que acaba la función de ajuste si el intervalo con mayor tendencia no es divisible
	//	ni sus adyacentes tampoco (parece un buen momento para parar no?)
	//*******
	
	
	// crea un nodo y lo coloca en mitad del intervalo
	if (tendencia > M_SQRT2/*1*/ && intervMax>0) 
		{
		MasNodos++;
		NodoBSpline *nuevo = new NodoBSpline;
		if (nuevo == NULL) 
			{
			printf("ERROR: No hay memoria para reservar NodoBSpline\n");
			delete nuevo;
			delete [] tendencias; delete [] tendenciasCh;
			delete [] ValorY; delete [] ValorX; delete [] SigmaY;
			ValorY=ValorX=SigmaY=NULL;
			borraLista(SplineContinuo);
			return -2;
			}

		int extIzq = ValorNodo(SplineContinuo.listaNodos, intervMax+1);
		int extDer = ValorNodo(SplineContinuo.listaNodos, intervMax+2);

		// Proyecto: busca el lugar con una tendencia mayor para introducir
		// el nuevo nodo. Evalua la tendencia por cada grupo
		// contiguo de dos canales de fondo

		// intenta poner el nodo en mitad del intervalo
		nuevo->posic = (extIzq + extDer)/2;

		// si la nueva posicion no esta en una zona de canal de fondo
		// busca el canal de fondo mas cercano y pone ahi el punto
		// de control
		int sep = 0;
		while ((!IsChannelContinuum(nuevo->posic+sep))&&
				 (!IsChannelContinuum(nuevo->posic-sep)))
			sep++;
		if (IsChannelContinuum(nuevo->posic-sep)) nuevo->posic -= sep;
		else nuevo->posic += sep;
		
		agnadeNodo(SplineContinuo.listaNodos, nuevo);
		// Ajusta la estructura del spline
		if (DimensionaBSpline(SplineContinuo) < 0) 
			{
			delete [] ValorY; delete [] ValorX; delete [] SigmaY;
			ValorY=ValorX=SigmaY=NULL;
			borraLista(SplineContinuo);
			return -2;
			}
		}
	//NOTA: Investigar qué pasa si se acaban los intervalos y no necesitamos más nodos por causa no
	//	de que todos los intervalos tengan tendencias bajas si no porque todos sean menores que
	//	sus fwhm
	else
		{
		//printf("No tenemos ningún intervalo que supere la tendencia de raiz de 2\n");
		}
	delete [] tendencias; delete [] tendenciasCh;

	nNodosSplineContinuo = SplineContinuo.listaNodos.numElementos;
	return MasNodos;
}		


//****************** INTERVALO TENDENCIA *****************************
/** Devuelve el intervalo que hay que dividir en orden a la tendencia 
	máxima. Generalmente será el intervalo en el que se da la 
	tendencia máxima, pero si este intervalo fuera demasiado 
	pequeño para albergar otro nudo, se buscaría en los dos nodos
	adyacentes
	Parámetros:
	- i intervalo en el que se encuentra una tendencia alta
	- tendencias array con las tendencias calculadas para cada intervalo
	- tendenciasCh array con el número de canales en cada intervalo de BSpline
	- lista lista enlazada con todos los nodos de BSpline que tenemos.
	NOTA: Falta decidir qué hacer si los dos nodos adyacentes son muy pequeños.
		¿Qué nodo devolvemos?
*/	
int cEspectros::intervaloTendencia(int i, double *tendencias, int *tendenciasCh, tListaNodosBSpline lista)
	{	
	int nAncho = calculaAncho(i, lista);
	
	if (tendenciasCh[i-1] < nAncho) //El intervalo es demasiado pequeño, seleccionamos entre los adyacentes
		{
		//Si tenemos intervalo a la izquierda con tamaño suficiente, ése será el intervalo a dividir
		if( (i-2>0) && (tendenciasCh[i-2] >= calculaAncho(i-1, lista)) )	return i-1;
		else	//Si no probamos con el de la derecha
			{
			if(i<lista.numElementos && tendenciasCh[i] >= calculaAncho(i+1, lista))	return i+1;
			else	//NOTA: Si no hay a ningún lado habrá que inventar algo
				//	A veces se da este caso, y sí que se ve que es en puntos donde el continuo
				//	se "arruga"
				{
				printf("No hay intervalos más pequeños a los lados. El intervalo está en: %d\n", ValorNodo(lista, i)); 
				return -1;
				}
			}
		}
	else	//Si el intervalo es suficientemente grande, él mismo vale
		return i;
	
	//Lo mismo de arriba pero para una vecindad de 3 (o de lo que queramos)
	//Parece que el tema de la vecindad no mejora nada respecto a mirar sólo los dos adyacentes
/*	if (tendenciasCh[i-1] < nAncho)  //El intervalo es demasiado pequeño, seleccionamos entre los adyacentes
		{ 
		int k=1;
		while(k<3)
			{
			//Si tenemos intervalo a la izquierda con tamaño suficiente, ése será el intervalo a dividir
			if( (i-(k+1)>0) && (tendenciasCh[i-(k+1)] >= calculaAncho(i-k, lista)) )	return i-k;
			else	//Si no probamos con el de la derecha
				{
				if((i+(k-1))<lista.numElementos && tendenciasCh[i+(k-1)] >= calculaAncho(i+k, lista))	return i+k;
				else	//NOTA: Si no hay a ningún lado habrá que inventar algo
					//	A veces se da este caso, y sí que se ve que es en puntos donde el continuo
					//	se "arruga"
					k++;
					//printf("No hay intervalos más pequeños a los lados de vecindad %d", k);
				}
			}
		}
	else	//Si el intervalo es suficientemente grande, él mismo vale
		return i;
		
	//NOTA: Faltaría ver qué hacemos si hemos terminado y ningún intervalo es suficientemente grande.
	//	de momento mandamos el intervalo inicial
	printf("Devolviendo el inicial por no haber más, canal %d\n", ValorNodo(lista, i));
	return i;
*/	
	
	
	}

//******************** CALCULA ANCHO ***********************
/** Nos devuelve el ancho en canales de un intervalo de Spline
	- intervalo 	Intervalo del que queremos saber el ancho
	- lista		lista con las posiciones de los nudos de BSpline
*/
int cEspectros::calculaAncho(int intervalo, tListaNodosBSpline lista)
	{
	int extIzq = ValorNodo(lista, intervalo+1);
	int extDer = ValorNodo(lista, intervalo+2);
	int nAncho = redondea((DaFWHGCh((double)extIzq)+DaFWHGCh((double)extDer))/2.0);
	return nAncho;	
	}	
	
//****************** FINAL AJUSTE CANALES CONTINUO *************************
int cEspectros::FinalAjusteCanalesContinuo() 
	{
	cAjusteLinealChi *Spline = (cAjusteLinealChi *) ajustes;
	Spline->AjustaCovarianza();
	
	// Finalizado el calculo se libera la memoria
	delete Spline; // ya no es necesario el objeto de ajustes
	delete [] ValorX; delete [] ValorY; delete [] SigmaY;//NOTA: Aquí borramos agusto, sin comprobar si se ha resevado?
	ValorY=ValorX=SigmaY=NULL;
	
	// Mete la nueva informacion en el espectro del continuo
	// crea los vectores
	delete [] continuo; delete [] sigmaContinuo;
	continuo = new float[numCanales+1];
	sigmaContinuo = new float[numCanales+1];
	if ((continuo == NULL)||(sigmaContinuo == NULL)) 
		{
		printf("ERROR: No hay memoria para reservar float[%d]\n", numCanales+1);
		delete [] continuo; delete [] sigmaContinuo;
		continuo = NULL; sigmaContinuo = NULL;
		borraLista(SplineContinuo);
		return -2;
		}
	
	//NOTA: Esto es lo que no mola. En la zona donde no tenemos datos del continuo
	//	porque no están dentro del intervalo que calculamos, no deberíamos sacar
	//	nada en la gráfica.
	for (int i = 1; i < minInterv; i++) 
		{
		continuo[i] = 0.0;
		sigmaContinuo[i] = 0.0;
		}
	for (int i = minInterv; i <= maxInterv; i++) 
		{
		continuo[i] = (float) exp(CalculaValorBSpline(SplineContinuo, (double) i));
		double acov = 0.0;
		for (int j = 1; j <= SplineContinuo.numElementos; j++)
			for (int k = 1; k <= SplineContinuo.numElementos; k++)
				acov += (SplineContinuo.intervalo[j]*SplineContinuo.intervalo[k]*SplineContinuo.covar[k+j*(SplineContinuo.numElementos+1)]);
		sigmaContinuo[i] = (float) sqrt(acov)*continuo[i];
			
		}
	for (int i = maxInterv +1 ; i <= numCanales; i++) 
		{
		continuo[i] = 0.0;
		sigmaContinuo[i] = 0.0;
		}
	
	nNodosSplineContinuo = SplineContinuo.listaNodos.numElementos;;
	// elimina lo que queda de memoria reservada
	borraLista(SplineContinuo);
	
	return 0;
	}

//***************** CANCELA AJUSTE CANALES CONTINUO **************
void cEspectros::CancelaAjusteCanalesContinuo() 
	{
	cAjusteLinealChi *Spline = (cAjusteLinealChi *) ajustes;
	
	// Cancelado el calculo se libera la memoria
	delete Spline;
	delete [] ValorX; delete [] ValorY; delete [] SigmaY;
	delete [] continuo; delete [] sigmaContinuo;
	continuo = NULL; sigmaContinuo = NULL;
	ValorY=ValorX=SigmaY=NULL;

	borraLista(SplineContinuo);
	nNodosSplineContinuo = 0;
	ChiSqContinuo = 0.0;
	}

void cEspectros::BorraAjusteCanalesContinuo() 
	{
	delete [] continuo; delete [] sigmaContinuo;
	continuo = NULL; sigmaContinuo = NULL;
	ChiSqContinuo = 0.0; nNodosSplineContinuo = 0;
	borraLista(SplineContinuo);
	}

/*************
 *           *
 *   PICOS   *
 *           *
 *************/

//*********************** INICIALIZA PICO **********************************
/**  Calcula los distintos parámetros de ajuste para el pico "Pico"
     - Pico: Estructura con los datos de ajuste de un pico (ver picos.h) Información a completar acerca de
       su anchura, exponenciales de ajuste, centroide...
     - maximo: valor en nº de cuentas del canal más alto. Servirá para el cálculo del centroide
     - calibración: si es verdadera el cálculo de la anchura y de tau (exp por absorción de e-) se hace de manera
     			distinta.  
	
	De los distintos parámetros que se inicializan no conozco de dónde salen
	las ecuaciones. 
	Comparándolas con el papel "Gamma ray Spectral Analysis with the COSPAJ continuum Fitting Routine,
	no encuentro muchas similitudes.
	Además la funcion de momento deja la exponencial por viaje de fotones sin inicializar
	
*/
void cEspectros::InicializaPico(tPicoAjuste& Pico, double maximo, bool calibracion) 
	{
	// busqueda del maximo precisa por primera derivada
	//NOTA: maximo me parece que no está restado al continuo (tampoco sé si es muy necesario).
	//printf("INICIALIZA PICOS\n");
	int centro = redondea(maximo);
	//printf("maximo=%f, centro=%d\n", maximo, centro);
	if(centro<2)	centro=2;
	double Y1 = espectro[centro-1] - continuo[centro-1];
	double Y2 = espectro[centro] - continuo[centro];
	double Y3 = espectro[centro+1] - continuo[centro+1];
	//NOTA: El -0.5 no sé si no sería mejor continuo[centro-1], aunque sería un valor bastante mayor.
	double Emax = maximo - 0.5 + (Y2-Y1)/(2.0*Y2-Y1-Y3);
	
	/* Si el maximo aproximado se aleja mucho del maximo (mas de 10 canales),
	   debido por ejemplo a una mala conformacion del pico, deja la energia
	   del maximo en el mismo lugar que el maximo */
	if (fabs(Emax-maximo) > 10.0) Emax = maximo;
				
	// inicializacion sigma y tau en valores tipicos partiendo de gaussiana
	if (!calibracion)
		{
		Pico.Anchura = DaSigmaCh(Emax)/1.88;
		Pico.Tau = powi(Pico.Anchura, 2) / 4.0;
		}
	else 	
		{
		Pico.Anchura = DaSigmaCh(Emax);
		Pico.Tau = DaTauCh(Emax);
		}
	
	// TODO: hacer una proteccion decente
	// proteccion contra calibraciones pesimas
	// NOTA: Parece que por ahora no hay calibraciones malas
	if (Pico.Anchura  <= 0.05) 		
		{
		printf("Mala calibración de la anchura: %f\n", Pico.Anchura);
		Pico.Anchura = 0.05;
		printf("Ajustada a %f\n", Pico.Anchura);
		}
	if ((Pico.Tau <= 0.05)||(Pico.Anchura/Pico.Tau > 10/Pico.Anchura))	
		{
		printf("Mala calibración de la anchura de la exponencial por la absorción de e-: %f\n", Pico.Tau);
		printf("Se le va a ajustar el valor a pow(%f,2)/4\n", Pico.Anchura);
		//NOTA: Antes se ponía simplemente la ecuación del if.
		//Para valores de anchura entre 0.05 y 0.45 nos quedaba un Tau menor que 0.05
		//con lo que el ajuste daba erróneo para el pico. Por eso hemos puesto este
		//else con una corrección alternativa en esos casos.
		//No obstante, anchuras tan pequeñas suelen dar un ajuste pésimo, sea la corrección
		//que sea
		if(Pico.Anchura>0.45)	Pico.Tau = powi(Pico.Anchura, 2) / 4.0;
		else					Pico.Tau=0.05;
		
		printf("Ajustada a %f\n", Pico.Tau);
		}

	// inicializacion de centroide a partir de sigma/tau y la posicion
	// del maximo 
	Pico.Centroide = M_SQRT2*SolucionBisecCentroide(Pico.Anchura/Pico.Tau);
	Pico.Centroide -= Pico.Anchura/Pico.Tau;
	Pico.Centroide = Emax - Pico.Anchura*Pico.Centroide;

	// inicializacion de Ytau por canal del maximo
	double m = Y3 + Y1 - 2*Y2;
	double n = Y2 - Y1 - m*(maximo-0.5);
	double Ymax = Y2 + m/2*(Emax*Emax - maximo*maximo) + n*(Emax- maximo);

	//NOTA: Esta ecuación parece que en general coincide con la de los papeles, pero no exactamente:
	//anchura=deltac	tau=deltat	Emax=c		Centroide=Cgamma
	//Con esto, es todo igual salvo que la exponencial debería estar multiplicando en vez de dividiendo
	//y no debería elevarse al cuadrado.
	//en cuanto al erfc, es igual, simplificado, salvo por el parámetro M_SQRT1_2 que no se donde está 
	//definido pero aún así lo suyo es que estuviera fuera, al menos si se refiere a la sqrt(2pi)
	//SOLUCIÓN: Había un pequeño fallo en los papeles. De todas maneras aquí está dividiendo, pues se
	//			trata de una especie de normalización entre la inicialización máxima (Ymax) y el pico en sí
	Pico.Ytau = 2.0*Ymax;
	Pico.Ytau /= exp(powi(Pico.Anchura/Pico.Tau, 2)/2.0 +
						  (Emax-Pico.Centroide)/Pico.Tau);
	Pico.Ytau /= erfc(M_SQRT1_2*((Emax-Pico.Centroide)/Pico.Anchura +
							Pico.Anchura/Pico.Tau));

/*							
	//NOTA: Esta es la ecuación de la fórmula de tau tal y como creemos que debería ser
	//	Los cambios en el ajuste de picos son inexistentes se calcule como se calcule,
	//	pero esta ecuación más cercana a la teoría hace cosas raras para algunos picos.
	printf("%f\n", M_SQRT1_2);
	Pico.Ytau =  2*sqrt(2*3.14)*Pico.Anchura*Ymax*exp((Pico.Anchura/Pico.Tau)*0.5 +(Emax-Pico.Centroide)/Pico.Tau);
	Pico.Ytau /= erfc((Emax-Pico.Centroide)/Pico.Anchura + Pico.Anchura/Pico.Tau);
*/	
	// resto de parametros a cero
	// NOTA: La exponencial por viaje de fotones no se usa!
	Pico.P = 0.0;
	Pico.YP = 0.0;
	
	//printf("Inicializando el resto de parámetros\n");
	for(int i=0;i<4;i++)
		for(int j=0;j<4;j++)
			Pico.covar[i][j]=0.0;
	Pico.ajustado=false;
					
	Pico.calibrado=false;	  // indica si el pico tiene asignada una energia de emision
	Pico.identificado=false;// indica si la energia de emision ha sido identificada
	
	Pico.eficienciaCalculada=false;//Indica si el pico tiene una eficiencia calcualda
	Pico.ldCalculado=false;//indica si el pico tiene unos límites de detección calculados
	Pico.analizado=false;  //indica si el pico tiene asignados valores a la actividad,
					 //el área restado un cierto fondo, y los facores de corrección
	Pico.invisible=false;
		
	// Datos de identificacion
	Pico.emisiones=new LinkedList();
	
	Pico.ld=0.0; 				//Límite de detección para este pico
	Pico.lc=0.0; 				//Límite de decisión para este pico
	Pico.eficiencia=0.0;		//Eficiencia para este pico
	Pico.sigmaEficiencia=0.0;	//Varianza de la eficiencia para este pico
	Pico.correccion=0.0;		//Corrección de la eficiencia
	Pico.sigmaCorreccion=0.0;	//Incertidumbre de la corrección de la eficiencia
	Pico.area=0.0;			//Área del pico
	Pico.incerArea=0.0;		//Incertidumbre del área del pico
	Pico.areaSinFondo=0.0;	//Área del pico sin contar el fondo del detector
	Pico.incerAreaSinFondo=0.0;//Incertidumbre del área del pico sin contar la del fondo del detector (?)dpb
	Pico.actividad=0.0;		//Actividad del pico
	Pico.sigmaActividad=0.0;	//Incertidumbre de la actividad
	Pico.mda=0.0;				//Mínima Actividad Detectable del pico
	Pico.mdaPadre=0.0;		//Mínima Actividad Detectable del padre del pico

	/*if(Pico.parametrosEficiencia!=NULL)	
		{
		//printf("parametros eficiencia no es null, hay que borrarlo\n");
		//delete [] Pico.parametrosEficiencia;//NOTA: Peta si lo intento borrar así"!!!!!
		}
	Pico.parametrosEficiencia = new double[dimensionMatEfi];
	for(int i=0;i<dimensionMatEfi;i++)	Pico.parametrosEficiencia[i]=0.0;*/
	//
	}

//******************** AJUSTE INTERVALO ******************
/*	Realiza un ajuste de un intervalo mediante Marquardt Levenberg
	*/
int cEspectros::AjusteIntervalo(int numIntervalo) 
	{
	// selecciona el intervalo que hay que ajustar
	NodoPicos *intervalo = DevuelveNodo(listaPicos, numIntervalo);
	intervaloAjuste = intervalo;
	printf("El intervalo de ajuste es el %d\n", numIntervalo);
	// si no hay vector para recoger los datos totales lo intenta crear
	if (total == NULL)
		{
		if ((total = new float[numCanales+1]) == NULL) 
			{
			printf("ERROR: No hay memoria para reservar float[%d]\n", numCanales+1);
			return -2;
			}
		// pone a cero todo el vector
		for (int i = 1; i <= numCanales; i++)	total[i] = 0.0;
		}
	printf("Construimos vectores\n");
	// Construye los vectores necesarios para hacer un ajuste de intervalo
	int numDatos = intervalo->limSuperior - intervalo->limInferior + 1;
	double *DatosX = NULL, *DatosY = NULL, *SigmaY = NULL;
			// reserva la memoria
	DatosX = new double [numDatos];
	DatosY = new double [numDatos];
	SigmaY = new double [numDatos];
	if ((DatosX == NULL)||(DatosY == NULL)||(SigmaY == NULL)) 
		{
		printf("ERROR: No hay memoria para reservar  tres double[%d]\n", numDatos);
		delete [] DatosX; delete [] DatosY; delete [] SigmaY;
		return -2;
		}
	printf("llenamos vectores\n");
	// llena los vectores
	for (int i = 0; i < numDatos; i++) 
		{
		DatosX[i] = (double) i + intervalo->limInferior;
		DatosY[i] = (double) espectro[i + intervalo->limInferior] -
						(double) continuo[i + intervalo->limInferior];
		SigmaY[i] = sqrt(powi((double) sigmaContinuo[i + intervalo->limInferior], 2) +
						espectro[i + intervalo->limInferior]);
		}

	printf("Inicializamos picos\n");
	// Crea el vector de parametros de ajuste
	// TODO: dejamos los parametros de la segunda exponencial a cero y sin ajustar
	int topeIteraciones = 1;
	for (int i = 1; i <= intervalo->picos.numElementos; i++) 
		{
		NodoPico *pico = DevuelveNodo(intervalo->picos, i);
		if (!pico->Ajuste.inicializado)	InicializaPico(pico->Ajuste, pico->maximo, true);
		}

	printf("Preparamos parámetros de ajuste\n");
	// Prepara los vectores de los datos a ajustar
	int numParam = intervalo->picos.numElementos*4;
	// reserva la memoria
	unsigned char *lista = new unsigned char[numParam];
	double *paramM = new double [numParam];
	double *dParamM = new double [numParam];
	double *covarParamM = new double [numParam*numParam];
	if ((paramM == NULL)||(dParamM == NULL)||(covarParamM == NULL)||(lista == NULL)) 
		{
		printf("ERROR: No hay memoria para reservar double[%d]\n", numParam);
		delete [] DatosX; delete [] DatosY; delete [] SigmaY;
		delete [] paramM; delete [] dParamM; delete [] covarParamM;
		delete [] lista;
		return -2;
		}
		
	printf("Ajustamos todos los parámetros\n");
	// Se ajustan todos los parametros
	for (int i = 0; i < numParam; i++) lista[i] = 1;
	printf("Número de pico: %d\n", intervalo->picos.numElementos);
	for (int i = 0; i < intervalo->picos.numElementos; i++) 
		{
		NodoPico *Pico = DevuelveNodo(intervalo->picos, i+1);
		paramM[i*4+0] = Pico->Ajuste.Centroide;
		paramM[i*4+1] = Pico->Ajuste.Anchura;
		paramM[i*4+2] = Pico->Ajuste.Tau;
		paramM[i*4+3] = Pico->Ajuste.Ytau;
		printf("c=%.2f s=%.2f, t=%.2f, h=%.2f\n", Pico->Ajuste.Centroide, Pico->Ajuste.Anchura, Pico->Ajuste.Tau, Pico->Ajuste.Ytau);
		}

	printf("Asociamos matrices de ajuste\n");
	// asocia las matrices al objeto que ajusta los datos
	cAjusteChi Ajuste;
	Ajuste.x = DatosX; Ajuste.y = DatosY; Ajuste.sigma = SigmaY;
	Ajuste.a = paramM; Ajuste.dyda = dParamM;
	Ajuste.lista = lista; Ajuste.covar = covarParamM;
	Ajuste.numDatos = numDatos; Ajuste.ma = numParam; Ajuste.mfit = numParam; 
	Ajuste.AsociaFuncionActualiza((tFuncionNL) ActualizaVectorPicosAnal);
	Ajuste.sigmaInf=Ajuste.sigmaSup=Ajuste.tauInf=Ajuste.tauSup=0;
	Ajuste.centroides=NULL;
	Ajuste.alturas=NULL;
		
	printf("Realizamos el ajuste\n");
	// hace el ajuste repetidas veces ya que cada vez utiliza los datos
	// conseguidos como una exploracion mas cercana de los minimos locales
	int nError;
	topeIteraciones=10;
	for (int i = 0; i < topeIteraciones; i++) 
		{
		nError = Ajuste.Ajusta();
		if (nError == -1) 
			{
			delete [] DatosX; delete [] DatosY; delete [] SigmaY;
			delete [] paramM; delete [] dParamM; delete [] covarParamM;
			delete [] lista;
			return -2;
			}
		}

	printf("Terminamos\n");
	// si el error ha sido -2 (fallo en el calculo) deja la inicializacion
	if (nError == -2) 
		{
		printf("FALLO EN EL CÁLCULO DEL AJUSTE\n");
		for (int i = 1; i <= intervalo->picos.numElementos; i++) 
			{
			NodoPico *pico = DevuelveNodo(intervalo->picos, i);
			InicializaPico(pico->Ajuste, pico->maximo, true);
			pico->Ajuste.SigCentroide = 1.0;
			pico->Ajuste.SigAnchura   = 1.0;
			pico->Ajuste.SigTau  	  = 1.0;
			pico->Ajuste.SigYtau 	  = 1.0;
			pico->Ajuste.SigTauYtau   = 0.0;
			}
		nError = -1;
		}
	else
		{
		printf("Tras ML en solitario\n");
		// aqui deberia actualizar los datos del ajuste en el pico
		for (int i = 0; i < intervalo->picos.numElementos; i++) 
			{
			NodoPico *Pico = DevuelveNodo(intervalo->picos, i+1);
			Pico->Ajuste.Centroide = paramM[i*4+0];
			Pico->Ajuste.Anchura   = paramM[i*4+1];
			Pico->Ajuste.Tau  	  = paramM[i*4+2];
			Pico->Ajuste.Ytau 	  = paramM[i*4+3];
				// y sus incertidumbres (matriz covarParaM)
			Pico->Ajuste.SigCentroide = sqrt(covarParamM[(4*i+0)*(numParam+1)]);
			Pico->Ajuste.SigAnchura   = sqrt(covarParamM[(4*i+1)*(numParam+1)]);
			Pico->Ajuste.SigTau  	  = sqrt(covarParamM[(4*i+2)*(numParam+1)]);
			Pico->Ajuste.SigYtau 	  = sqrt(covarParamM[(4*i+3)*(numParam+1)]);
			Pico->Ajuste.SigTauYtau   = covarParamM[(4*i+2)*(numParam+1)+1];
			}
		}

	for (int i = 1; i <= intervalo->picos.numElementos; i++) 
		{
		NodoPico *pico = DevuelveNodo(intervalo->picos, i);
		pico->Ajuste.ajustado = true;
		pico->Ajuste.area=DaAreaPico(numIntervalo, i);
		pico->Ajuste.incerArea=DaIncerAreaPico(numIntervalo, i);
		}
	
	ActualizaIntervaloAnalisis(numIntervalo); // actualiza los datos con el nuevo ajuste
//	ActualizaIntervaloAnalisis(numIntervalo, 4); // actualiza los datos con el nuevo ajuste
	
	// Limpia la memoria utilizada
	delete [] DatosX; delete [] DatosY; delete [] SigmaY;
	delete [] paramM; delete [] dParamM; delete [] covarParamM;
	delete [] lista;

	return nError;
	}

/*********************** AJUSTE INTERVALOS GAML **************************
	Ajuste sin restricciones
	*/
int cEspectros::AjusteIntervaloGAML(int numIntervalo, float *centroides, int numGaussianas, float taus, float sigmas, int nea)
	{
	float restricciones[numGaussianas];
	for(int i=0;i<numGaussianas;i++)	restricciones[i]=0;
	return AjusteIntervaloGAML(numIntervalo, centroides, restricciones, numGaussianas, taus, sigmas, nea);
	}
	
//************************* AJUSTE INTERVALO GA ML ************************
/*	Ajuste de intervalos mediante Algoritmos Genéticos + Marquadt-Levenberg
	Este es un proceso bastante importante para el análisis.
	También es bastante complejo. El proceso sería este:
		1)Iniciamos un algoritmo genético con parámetros de un grupo de picos
	según su energía y las emisiones guardadas en naturlib.db
		
		2) Corremos el algoritmo genético
		3) Corremos un algoritmo de búsqueda Grid Search con los parámetros 
			obtenidos por el algoritmo genético.
		4) Corremos un algoritmo de Marquardt-Levenberg con los parámetros 
			obtenidos con el Grid Search. Técnicamente es un poco bobada 
			correr dos algoritmos de optimización tradicional. Se hace porque
			GS es más rápido que ML, pero ML calcula incertidumbres; por tanto
			la carga del ajuste la lleva GS y ML apenas hace alguna corrección
			y calcula las incertidumbres.
		5) Comprobaciones finales: comprobamos que los centroides no estén 
		    demasiado cerca unos de otros y que las áreas estén por encima 
			de los límites de detección. Si alguna de estas dos cosas no 
			ocurre, reiniciamos el proceso eliminando uno de los dos picos 
			cercanos o el pico con poca estadística.
		6) El algoritmo ajusta bien los picos, con un error mínimo. Pero ahora
			estamos intentando ir más allá y que sus áreas se ajusten perfectamente
			a las actividades esperadas para esos radionúclidos.
			Para ello, como no estamos contemplando probabilidades de emisión
			ni intercomparación de emisiones del mismo radionúclido, existen casos
			en que obtenemos actividades disparatadas (generalmente para emisiones
			con muy poca probabilidad, <0,1%).
			Para ello vamos a añadir una última condición de ajuste:
				Para cada pico que emita en el intervalo:
					a) Lo identificamos mediante naturlib.db
					b) Si está solo en su intervalo, identificamos el intervalo
						donde está su emisión más probable (NOTA: A la larga igual hay que detallar más esto y coger la más probable pero que no esté solapada, o si todas lo están la que lo esté menos, y que no tenga más picos en naturlib).
						Si dicho intervalo no existe -> su emisión más probable
						no existe -> el radionúclido no emite en este espectro ->
						eliminamos el pico
					   Si no está solo, no hacemos nada más
					c) Calculamos el área de la emisión más probable. Implica hacer
						otro ajuste, en este caso no hacemos uno tan complicado si
						no directamente un ML o GS.
					d) Comparamos ese área con el que hemos obtenido, teniendo en
						cuenta que su área teórica debería ser:
							Ap=(Ep*Pp*Am)/(Em*Pm)
							(m-pico de prob máxima)
							(p-pico en cuestión)
						Si el área real difiere más de una determinada holgura del
						área teórica, rehacemos el ajuste teniendo en cuenta esta 
						restricción.
						NOTA: Como la eficiencia es un parámetro que se calcula
						durante la actividad, y aunque puede ser determinante
						(pues tiene una variación más o menos considerable, por 
						ejemplo de 0,008 a 0,08, un orden lo más.); de momento
						lo eliminamos considerándolo igual en todo el espectro
						Si vemos que aún salen cosas raras, lo añadiremos
						->Parece que es necesario, por ejemplo, para estimar el
						área de 226Ra a 46,54 frente al de 609,32. De todos modos
						todavía no lo añadimos
	*/
int cEspectros::AjusteIntervaloGAML(int numIntervalo, float *centroides, float *restricciones, int numGaussianas, float tauEsp, float sigmaEsp, int nea)
	{
	printf("AJUSTE INTERVALO GAML\n");	
	
	for(int i=0;i<maxOpciones;i++)	printf("Opciones %d=%d\n", i, opciones[i]);
	// selecciona el intervalo que hay que ajustar
//	printf("********************* AJUSTE INTERVALO GAML ************************\n");
	NodoPicos *intervalo = DevuelveNodo(listaPicos, numIntervalo);
	intervaloAjuste = intervalo;
	int nError=0;
	
	printf("%d\n", nea);
	// si no hay vector para recoger los datos totales lo intenta crear
	if (total == NULL)
		{
		if ((total = new float[numCanales+1]) == NULL) 
			{
			printf("ERROR: AjusteIntervaloGAML: No hay memoria para reservar float[%d]\n", numCanales+1);
			return -2;
			}
		// pone a cero todo el vector
		for (int i = 1; i <= numCanales; i++)
			total[i] = 0.0;
		}
	
	int numDatos = intervalo->limSuperior - intervalo->limInferior + 1;
	double *DatosX = NULL, *DatosY = NULL, *SigmaY = NULL;
	// reserva la memoria
	DatosX = new double [numDatos];
	DatosY = new double [numDatos];
	SigmaY = new double [numDatos];
	if ((DatosX == NULL)||(DatosY == NULL)||(SigmaY == NULL)) 
		{
		printf("ERROR: No hay memoria para reservar double[%d]\n", numDatos);
		delete [] DatosX; delete [] DatosY; delete [] SigmaY;
		return -2;
		}
	
	// comprueba que se le hayan pasado datos lógicos
	for(int i=0;i<numGaussianas;i++)
		{
	//	printf("cent[%d]=%f\n", i, centroides[i]);
		if(DaEnergia(centroides[i])<=0)	//OJO: Con esto se mete en bucle infinito
			{
			printf("ERROR: centroide[%d] <= 0\n", centroides[i]);
			return -1;
			}
		}
	// llena los vectores
	for (int i = 0; i < numDatos; i++) 
		{
		DatosX[i] = (double) i + intervalo->limInferior;
		DatosY[i] = (double) espectro[i + intervalo->limInferior] -
					(double) continuo[i + intervalo->limInferior];
		SigmaY[i] = sqrt(powi((double) sigmaContinuo[i + intervalo->limInferior], 2) +
							  espectro[i + intervalo->limInferior]);
		if(SigmaY[i]<1)	SigmaY[i]=1;
		/*Importantísimo este if: si un canal no tiene cuentas, este valor puede
					ser <1 en ese canal. Lo que indicará mínimo un O(-1) o generalmente
					un O(-2). Un O(-2) que luego cuando se calcula X2 en ajustes.C va
					al cuadrado y dividiendo, es decir, afecta como un O(4) cuando otros
					canales (O(1) ó O(2)) afectan como mucho en O(-2)!; luego el error
					del ajuste depende única y exclusivamente de lo cerca de cero que 
					quede este canal; lo cual claramente no es que esté muy bien
		*/
		}

	// Crea el vector de parametros de ajuste
	// TODO: dejamos los parametros de la segunda exponencial a cero y sin ajustar
	for (int i = 1; i <= intervalo->picos.numElementos; i++) 
		{
		NodoPico *pico=DevuelveNodo(intervalo->picos, i);
		if(!pico->Ajuste.inicializado)
			InicializaPico(pico->Ajuste, pico->maximo, true);
		}

	BorraAjustesParcialesEnGrafica();//Estas gráficas son las del mejor individuo

		
	int li=DaLimInferior(numIntervalo);
	int ls=DaLimSuperior(numIntervalo);
	int ancho=DaAnchoIntervalo(numIntervalo);
	float *v=DaVectorIntervalo(numIntervalo);
	
		
	//********************** PARTE I: ALGORITMO GENÉTICO *****************		
	//COMIENZA EL ALGORITMO GENÉTICO	
	//Preparamos los datos de los centroides
	int numGeneraciones=numGaussianas*5;//-->la buena
//	int numGeneraciones=numGaussianas*100;
//	int numGeneraciones=200;

	float error[numGeneraciones];
	float funcionTotal[ancho];
	clock_t finalTiempo=0;
	clock_t inicioIteracion=0;
		
	int j=0;
	float taus=-1;
	float sigmas=-1;
	if(opciones[0])		taus=DaTauCh(ancho/2+li);	
	if(opciones[1])		sigmas=DaSigmaCh(ancho/2+li);	
	
	printf("^^^ ANTES DE GA: Tau está en %.2f\n", taus);
	if(!calibracionCorrecta(li, ls+ancho))	return -7;
	//numGaussianas=1;
	GeneticAlgorithm2 *GA2;
//	for(int i=0;i<numGaussianas;i++)	printf("REST[%d]=%f\n", i, restricciones[i]);

//	if(numGaussianas>5)	GA2=new GeneticAlgorithm2(numPob,(int)(.3*numPob),(int)(.45*numPob),1,numGaussianas,v,ancho,this,li,centroides,taus, sigmas, restricciones);
//	GA2=new GeneticAlgorithm2(100,60,90,1,numGaussianas,v,ancho,this,li,centroides,taus, sigmas, restricciones);
	GA2=new GeneticAlgorithm2(100,40,20,0.8,numGaussianas,v,ancho,this,li,centroides,taus, sigmas, restricciones,0);
//	GA2=new GeneticAlgorithm2(100,40,90,1,numGaussianas,v,ancho,this,li,centroides,taus, sigmas, restricciones,0);
//	GA2=new GeneticAlgorithm2(1,1,1,1,numGaussianas,v,ancho,this,li,centroides,taus, sigmas, restricciones);
	
	if(GA2==NULL)
		{
		printf("ERROR: No hay memoria para reservar GeneticAlgorithm2\n");
		return -2;
		}
//printf("calculamos ajuste\n");
	GA2->calculaAjuste();	
	//	printf("ordenamos\n");
	GA2->ordenar();

		//printf("pintamos\n");
	for(j=0;j<numGaussianas;j++)
		{
		float *funcion=GA2->getFuncion(j);//Nos da la función de gauss del mejor individuo para el parámetro j
		BorraGeneticEnGrafica(j+7);//Estas gráficas son las del mejor individuo
		CopiaGeneticAGrafica(funcion, li, ls, j+7);
		ActivaGeneticGrafica(true, j+7);
		PintaGrafica(0);
		delete [] funcion;
		}

//	printf("vamos a ejecutar\n");
	float *temp=new float[numGaussianas*2+2];
	int i=0;
	double err=10;
//	while(i<numGeneraciones)
	while(i<numGeneraciones && err>1)
		{
		inicioIteracion=clock();
		GA2->ejecutar(1,0.001);
		GA2->getParametros(temp);

		err=GA2->getMejorAjuste();
//		printf("It %d: ajuste: %f\n", i, err);
		i++;
		}

	GA2->calculaAjuste();
	GA2->ordenar();
	err=GA2->getMejorAjuste();
	finalTiempo=clock();
	printf("tiempo empleado(GA): %f s (vel: %ld).\n", 2.0*(finalTiempo-principioTiempo)/CLOCKS_PER_SEC, CLOCKS_PER_SEC);

	GA2->getParametros(temp);
	
	printf("^^^ TRAS GA: Tau es %.2f\n", temp[numGaussianas*2+1]);
	for(int i=0;i<numGaussianas;i++)
//		printf("c:%.2f (keV)\ta:%.2f\ts:%.2f (keV)\tt:%.2f (keV)\tar=%.2f\n", DaEnergia(temp[i]), temp[i+numGaussianas], DaEnergia(temp[numGaussianas*2]), DaEnergia(temp[numGaussianas*2+1]), temp[i+numGaussianas]*temp[numGaussianas*2+1]);
		printf("c:%.2f (keV)\ta:%.2f\ts:%.2f (keV)\tt:%.2f (keV)\tar=%.2f\n", DaEnergia(temp[i]), temp[i+numGaussianas], temp[numGaussianas*2], temp[numGaussianas*2+1], temp[i+numGaussianas]*temp[numGaussianas*2+1]);
	printf("El error es: %f\n", calculaChiCuadrado(temp, numGaussianas*2+2, li, ancho, v));	

	float *parametros=new float[numGaussianas*2+2];//NOTA: Creo que ahora mismo no se usa pa na
	if(parametros==NULL)
		{
		printf("ERROR: No hay memoria para reservar float[%d]\n", numGaussianas*2+2);
		return -2;
		}
	
	//******************* 2ª PARTE (BÚSQUEDA LOCAL)-MARQUARDT *******************
	// Construye los vectores necesarios para hacer un ajuste de intervalo
	printf("TENEMOS %d GAUSSIANAS Y %d PICOS\n", numGaussianas, intervalo->picos.numElementos);
	if(numGaussianas>intervalo->picos.numElementos)
		{
		int necesarias=numGaussianas-intervalo->picos.numElementos;
		for(int i=0;i<necesarias;i++)
			{
			NodoPico *PicoNuevo = new NodoPico;
			if (PicoNuevo == NULL) 
				{
				printf("ERROR: No hay memoria para reservar NodoPico\n");
				delete [] fondo; delete [] picos; 
				fondo = NULL; picos = NULL;
				return -4;
				}
			iniciaNodo(PicoNuevo);
			agnadeNodo(intervalo->picos, PicoNuevo);
			}
		}
	else if(numGaussianas<intervalo->picos.numElementos)
			{
			int inservibles=intervalo->picos.numElementos-numGaussianas;
			for(int i=0;i<inservibles;i++)		quitaNodo(intervalo->picos,0);
			}
	
	int numParam = numGaussianas*4;

	// reserva la memoria
	unsigned char *lista = new unsigned char[numParam];
	double *paramM = new double [numParam];
	double *dParamM = new double [numParam];
	double *covarParamM = new double [numParam*numParam];
	
	if ((paramM == NULL)||(dParamM == NULL)||(covarParamM == NULL)||(lista == NULL)) 
		{
		printf("ERROR: No hay memoria para reservar double[%d]\n", numParam);
		delete [] DatosX; delete [] DatosY; delete [] SigmaY;
		delete [] paramM; delete [] dParamM; delete [] covarParamM;
		delete [] lista;
		return -2;
		}

	for(int i=0;i<numParam;i++)
		{
		paramM[i]=dParamM[i]=0.0;
		for(int j=0;j<numParam;j++)		covarParamM[i*numParam+j]=0.0;
		}
		
	// Se ajustan todos los parametros
	int k=0;
	for (int i = 0; i < numParam; i++) 	//lista[i]=1;//Antes
		{
		//NOTA: Probando a dejar los centroides y las taus fijas.
		if(i%2==0)	lista[i] = 0;//Están a offset 0 (centroides) y 2 (taus)
		//NOTA: Quiero probar a dejar la sigma fija para los picos gemelos:
		//		Allí las restricciones funcionan bien, pero tenemos el problema
		//		de que ML para hacer el gracioso coge y ensancha con sigma, 
		//		saltándose la restricción de la altura
		else		
			{
			lista[i] = 1;
			}
		}
	int numAjustados=numParam/2; //OJO: Variar según los parámetros que ajustemos

	printf("Num gaussianas es %d, creando numParam\n", numGaussianas);
	for (int i = 0; i < numGaussianas; i++) 
		{
		paramM[i*4+0] = temp[i];//Centroide
		paramM[i*4+1] = temp[numGaussianas*2];//Sigma
		paramM[i*4+2] = temp[numGaussianas*2+1];//Tau
		paramM[i*4+3] = temp[numGaussianas+i];//Amplitud
		printf("Amplitud %d es %f\n", i, temp[i+1]);
		}
	
	// asocia las matrices al objeto que ajusta los datos
	cAjusteChi Ajuste;
	Ajuste.x = DatosX; Ajuste.y = DatosY; Ajuste.sigma = SigmaY;
	Ajuste.a = paramM; Ajuste.dyda = dParamM;
	Ajuste.lista = lista; Ajuste.covar = covarParamM;
	Ajuste.numDatos = numDatos; Ajuste.ma = numParam; Ajuste.mfit = numAjustados; 
	Ajuste.AsociaFuncionActualiza((tFuncionNL) ActualizaVectorPicosAnal);
	
	//Restricciones----------------------------
	//Si es el pico de aniquilación, no se aplican restricciones a la anchura
	Ajuste.picoSinRestricciones=-1;
	
	if(opciones[0])
		{
		Ajuste.tauInf=0.5*temp[numGaussianas*2+1];		
		Ajuste.tauSup=1.5*temp[numGaussianas*2+1];		
		for(int i=0;i<numGaussianas;i++)
			{
			if(DaEnergia(paramM[i*4])>510 && DaEnergia(paramM[i*4])<512)
				{
				Ajuste.picoSinRestricciones=i*4;
				Ajuste.tauInf=0.5*taus;		
				Ajuste.tauSup=1.5*taus;		
				i=numGaussianas;
				}
			}
		}
	else	Ajuste.tauInf=Ajuste.tauSup=-1;
		
	if(opciones[1])
		{
		Ajuste.sigmaInf=0.5*temp[numGaussianas*2];		
		Ajuste.sigmaSup=1.5*temp[numGaussianas*2];
		for(int i=0;i<numGaussianas;i++)
			{
			if(DaEnergia(paramM[i*4])>510 && DaEnergia(paramM[i*4])<512)
				{
				Ajuste.picoSinRestricciones=i*4;
				Ajuste.sigmaInf=0.5*sigmas;		
				Ajuste.sigmaSup=1.5*sigmas;
				i=numGaussianas;
				}
			}
		}
	else	Ajuste.sigmaInf=Ajuste.sigmaSup=-1;

	printf("Pico Sin Restricciones vale %d\n", Ajuste.picoSinRestricciones);
	printf("^^^ANTES ML: tau en [%f,%f] según la tau ajustada en GAGS, %f\n", Ajuste.tauInf, Ajuste.tauSup, temp[numGaussianas*2+1]);
	printf("sigma en [%f,%f] según la sigma ajustada en GAGS, %f\n", Ajuste.sigmaInf, Ajuste.sigmaSup, temp[numGaussianas*2]);

//	printf("Ajustadas restricciones para ML a t€[%.2f,%.2f] y s€[%.2f,%.2f]\n", Ajuste.tauInf, Ajuste.tauSup, Ajuste.sigmaInf, Ajuste.sigmaSup);
	/*NOTA: Si le ponemos la restricción a ML, efectivamente cumple las restricciones,
	pero claro, no tiene en cuenta rotura de equilibrios. Con lo cual todos los ajustes
	quedan coherentes, con todas las actividades muy cerca unas de otras, pero no por
	ello todos los ajustes son buenos, pues hay picos con más estadistica que la del 
	natural padre, y por tanto las actividades no contemplarían esto.
	Lo suyo es comprobar si hay equilibrio y dependiendo de si lo hay o no aplicar la
	restricción de uno o del otro. -> es la siguiente fase, q tengo q hablar con Bego.
	De momento está sin esta restricción, a pesar de que eso también puede ser un problema,
	pues puede darle cancha a emisiones q realmente no son significativas.
	*/
	double *alturas = new double[numGaussianas];
	double *centros = new double[numGaussianas];
	printf("numGaussianas es %d\n", numGaussianas);
	for(int i=0;i<numGaussianas;i++)	
		{
		centros[i]=temp[i];
		if(restricciones[i]>0)//Si había restricción, le ponemos restricción a la altura
			{
			alturas[i]=restricciones[i]/temp[numGaussianas*2+1];
		//	printf("%d: %.2f\n", i, alturas[i]);
			}
		else	
			{
		//	printf("%d: No tiene restricción\n", i); 
			alturas[i]=0;
			}
		}
	
	Ajuste.alturas=alturas;
	Ajuste.centroides=centros;
	//------------------------------------------------
	for(int i=0;i<numGaussianas;i++)
		printf("Restricciones a la altura y el centroide: a[%d]=%f, c[%d]=%f\n", i,alturas[i], i,centros[i]);
	principioTiempo=clock();
	nError=Ajuste.Ajusta();//Pasamos una sola vez
	finalTiempo=clock();
	printf("tiempo empleado(ML): %f s (vel: %ld)\n", 2.0*(finalTiempo-principioTiempo)/CLOCKS_PER_SEC, CLOCKS_PER_SEC);
	printf("ERROR TRAS MARQUARDT: %f\n", Ajuste.ChiSq);
	
	//************ ACTUALIZACIÓN DE RESULTADOS *****************
	// aqui deberia actualizar los datos del ajuste en el pico (En temp)
	// deberíamos además ver si tenemos más gaussianas de las que en un 
	// principio obtuvimos con el ajuste inicial para en ese caso modificar
	// el número de picos estimados en el intervalo.
	
	//paramM=ordenaCentroides(paramM, numGaussianas);
	/*printf("Valores antes de ordenaCentroides\n");
	for(int i=0;i<intervalo->picos.numElementos;i++)
			printf("c[%d]=%.2f\n", paramM[i*4]);*/
	//ordenaCentroides(paramM, numGaussianas);
//	printf("^^^TRAS ML: tau es %f\n", paramM[i*4+2]);
	printf("Valores tras Marquardt:\n");

	for (int i = 0; i < intervalo->picos.numElementos; i++) 
		{
		NodoPico *Pico = DevuelveNodo(intervalo->picos, i+1);
		Pico->Ajuste.ajustado = true;
		Pico->Ajuste.identificado = true;
		Pico->Ajuste.Centroide = paramM[i*4];
		Pico->Ajuste.Anchura   = paramM[i*4+1];
		Pico->Ajuste.Tau  	   = paramM[i*4+2];
		Pico->Ajuste.Ytau 	   = paramM[i*4+3];//Para ML
		printf("c:%.2f a:%.2f s=%.2f t=%.2f ar=%.2f\n", DaEnergia(Pico->Ajuste.Centroide), Pico->Ajuste.Ytau, Pico->Ajuste.Anchura, Pico->Ajuste.Tau, Pico->Ajuste.Ytau*Pico->Ajuste.Tau);
//		printf("c:%.2f a:%.2f s=%.2f t=%.2f ar=%.2f\n", DaEnergia(Pico->Ajuste.Centroide), Pico->Ajuste.Ytau, DaEnergia(Pico->Ajuste.Anchura), DaEnergia(Pico->Ajuste.Tau), Pico->Ajuste.Ytau*Pico->Ajuste.Tau);
		// y sus incertidumbres (matriz covarParaM)
//		printf("Accediendo a %d\n", (4*i+0)*(numParam+1) );
		Pico->Ajuste.SigCentroide = sqrt(covarParamM[(4*i+0)*(numParam+1)]);
		//printf("Accediendo a %d\n", (4*i+1)*(numParam+1) );
		Pico->Ajuste.SigAnchura   = sqrt(covarParamM[(4*i+1)*(numParam+1)]);
		//printf("Accediendo a %d\n", (4*i+2)*(numParam+1) );
		Pico->Ajuste.SigTau  	  = sqrt(covarParamM[(4*i+2)*(numParam+1)]);
		//printf("Accediendo a %d\n", (4*i+3)*(numParam+1) );
		Pico->Ajuste.SigYtau 	  = sqrt(covarParamM[(4*i+3)*(numParam+1)]);
		Pico->Ajuste.SigTauYtau   = covarParamM[(4*i+2)*(numParam+1)+1];
//		printf("incerTau=%f y covarTauAltura=%f\n", Pico->Ajuste.SigTau, Pico->Ajuste.SigTauYtau);}		//NOTA: OJO: creo que se hace necesario tener la matriz covarParam ENTERA
		//		almacenada, por si queremos, por ejemplo, calcular la incertidumbre
		//		de la convolución. O si no, por lo menos, crear SigCentroideAnchura, etc.
		for(int j=0;j<4;j++)//columna
			for(int k=0;k<4;k++)//fila
				Pico->Ajuste.covar[j][k]=covarParamM[(i*4+j)*(numParam) + (4*i)+k];
			
		Pico->Ajuste.area=DaAreaPico(numIntervalo, i+1);
		Pico->Ajuste.incerArea=DaIncerAreaPico(numIntervalo, i+1);
//		printf("Área del pico %d es %f(%f)\n", i, Pico->Ajuste.area, Pico->Ajuste.incerArea);
		}
	
	//Borramos estas gráficas provisionales
	for(j=0;j<numGaussianas;j++)	BorraGeneticEnGrafica(j+7);
	BorraGeneticEnGrafica(j+7);

	//******************* actualiza los datos con el nuevo ajuste
	ActualizaIntervaloAnalisis(numIntervalo); 
//	ActualizaIntervaloAnalisis(numIntervalo,2); 

	// Limpia la memoria utilizada
	delete [] DatosX; delete [] DatosY; delete [] SigmaY;
	delete [] paramM; delete [] dParamM; delete [] covarParamM;
	delete [] lista;
	
	//*************** descarte de picos con mala estadística **********
	//NOTA: Según Bego, mejor hacer el descarte entre GA y ML
	
	calculaLC(numIntervalo);
	int numPicosValidos=0;
	float centroidesValidos[numGaussianas];
//	printf("Número de picos en el intervalo: %d y numero de Gaussianas: %d\n", intervalo->picos.numElementos, numGaussianas);
	bool picoValido=true;
	for (int i = 1; i <= intervalo->picos.numElementos; i++) 
		{
		NodoPico *pico = DevuelveNodo(intervalo->picos, i);
		picoValido=true;
		//1)Descarte por altura pequeña (límites de decisión)
		if(DaAreaPico(numIntervalo, i)>pico->Ajuste.lc)	
			{
			centroidesValidos[i-1]=centroides[i-1];
			}
		else	
			{
			picoValido=false;
			printf("AREA %f MENOR QUE LC (%f)\n", DaAreaPico(numIntervalo, i), pico->Ajuste.lc); 
			centroidesValidos[i-1]=-1; //No es válido
			}
		//2)Descarte por centroides más cercanos que la resolución
		if(i>1)
			{	
			NodoPico *picoAnt = DevuelveNodo(intervalo->picos, i-1);
			if(centroidesValidos[i-2]!=-1 && centroidesValidos[i-1]!=-1 && fabs(pico->Ajuste.Centroide-picoAnt->Ajuste.Centroide)<DaResolucionCh(pico->Ajuste.Centroide))	
				{//NOTA: Funciona mal esto!!!!
				printf("---Los centroides %d y %d están en %f y %f, con distancia de %f<%f\n", i-2, i-1, picoAnt->Ajuste.Centroide, pico->Ajuste.Centroide, fabs(picoAnt->Ajuste.Centroide-pico->Ajuste.Centroide), DaFWHGCh(pico->Ajuste.Centroide));
				picoValido=false;
				centroidesValidos[i-1]=-1;//Eliminamos el primero, para evitar que luego borremos un tercero que hacía falta
				
				if(i-1==nea)	nea=tauEsp=sigmaEsp=-1;//Si hemos quitado el pico especial pues nada
				if(i-1<nea)		nea--;//Si hemos quitado un pico cualquiera anterior nea es uno menos
				}
			}
		if(picoValido)	numPicosValidos++;
		}
	
	if(numPicosValidos<numGaussianas)
		{
		//Alguno de los picos no es necesario para el ajuste, lo eliminamos y lo repetimos
		float nuevosCentroides[numPicosValidos];
		float nuevasRestricciones[numPicosValidos];
			
		int j=0;
		for(int i=0;i<numGaussianas;i++)
			{
			if(centroidesValidos[i]!=-1)	
				{
				nuevosCentroides[j]=centroidesValidos[i];
				nuevasRestricciones[j]=restricciones[i];
				j++;
				}
			}
		if(numPicosValidos>0)	
			{
			printf("Tenemos que reajustar \n");
//			nError=AjusteIntervaloGAML(numIntervalo, nuevosCentroides, nuevasRestricciones, numPicosValidos);
			nError=AjusteIntervaloGAML(numIntervalo, nuevosCentroides, nuevasRestricciones, numPicosValidos, tauEsp, sigmaEsp, nea);
			}
		else		
			{		
			printf("Se nos han acabado los picos mediante GA, realizamos sólo un LM\n");				
		//	AjusteIntervalo(numIntervalo);
			printf("Volvemos del LM\n");
			nError=-6;//Para que sepa que GAML no ha podido hacer nada
			}
		}
		
	//Gestión de memoria	
	delete [] parametros;	delete [] temp; 
//	delete [] temp2;		delete [] sigmaTemp2;
//	delete [] taus;	delete [] sigmas;
	delete [] v;
	delete GA2;
	delete [] alturas;
	delete [] centros;
	
	return nError;
	}

//************** COMPROBAR CALIBRACIONES ******************
/*	Comprueba que la calibración de tau y sigma en [li,ls]
	(li y ls deben pasarse en canales) no toma valores erróneos 
	para nuestros cálculos (<=0)
	Devuelve:
		0 si todo está correcto
		1 si hay fallo en la calibración
	*/
bool cEspectros::calibracionCorrecta(int li, int ls)
	{
	float tauli=DaTauCh(li);
	float tauls=DaSigmaCh(ls);
	float sigmali=DaSigmaCh(li);
	float sigmals=DaSigmaCh(ls);
	
	if(tauli<=0 || tauls<=0 || sigmali<=0 || sigmals<=0)	return false;
	return true;
	}
	
//******************* ORDENA CENTROIDES ********************
void cEspectros::ordenaCentroides(double *params, int tam)
	{
	double *nuevo=new double[tam*4];
	if(nuevo==NULL)
		{
		printf("ERROR: No hay memoria para reservar double[%d]\n", tam*4);
		return;
		}
	
	double cmin=0;
	int pmin=0;
	for(int i=0;i<tam;i++)
		{
		pmin=0;
		cmin=1000000000;
		for(int j=0;j<tam;j++)
			{
			if(cmin>params[j*4] && params[j*4]!=-1)	
				{
				cmin=params[j*4]; 
				pmin=j;
				}
			}
		nuevo[i*4]=params[pmin*4];
		nuevo[i*4+1]=params[pmin*4+1];
		nuevo[i*4+2]=params[pmin*4+2];
		nuevo[i*4+3]=params[pmin*4+3];
		
		params[pmin*4]=-1;//Para que no lo vuelva a escoger
		}
	//params=nuevo;
	for(int i=0;i<tam*4;i++)	params[i]=nuevo[i];
	delete [] nuevo;
	return;
	//return nuevo;	
	}
	
//******************* ORDENA VECTOR ********************
void cEspectros::ordenaVector(float *params, int tam)
	{
	printf("ORDENA VECTOR DE %d elementos\n", tam);
	for(int i=0;i<tam;i++)	printf("%.2f\n", params[i]);
		
	float *nuevo=new float[tam];
	if(nuevo==NULL)
		{
		printf("ERROR: No hay memoria para reservar double[%d]\n", tam*4);
		return;
		}
	
	float cmin=0;
	int pmin  =0;
	for(int i=0;i<tam;i++)
		{
		pmin=0;
		cmin=1000000000;
		for(int j=0;j<tam;j++)
			{
			if(cmin>params[j] && params[j]!=-1)	
				{
				cmin=params[j]; 
				pmin=j;
				}
			}
		nuevo[i]=params[pmin];
		params[pmin]=-1;//Para que no lo vuelva a escoger
		}
	//params=nuevo;
	
	for(int i=0;i<tam;i++)	printf("%.2f ", nuevo[i]);
	printf("\n");
	
	for(int i=0;i<tam;i++)	params[i]=nuevo[i];
	delete [] nuevo;
	return;
	}

//*************** AJUSTE INTERVALO ML ***************	
int cEspectros::AjusteIntervaloML(int numIntervalo, float *centroides, int numGaussianas)
	{
	printf("AJUSTE INTERVALO GAML\n");	
	
	NodoPicos *intervalo = DevuelveNodo(listaPicos, numIntervalo);
	intervaloAjuste = intervalo;
	int nError=0;
	
	// si no hay vector para recoger los datos totales lo intenta crear
	if (total == NULL)
		{
		if ((total = new float[numCanales+1]) == NULL) 
			{
			printf("ERROR: AjusteIntervaloGAML: No hay memoria para reservar float[%d]\n", numCanales+1);
			return -2;
			}
		// pone a cero todo el vector
		for (int i = 1; i <= numCanales; i++)
			total[i] = 0.0;
		}
	
	int numDatos = intervalo->limSuperior - intervalo->limInferior + 1;
	double *DatosX = NULL, *DatosY = NULL, *SigmaY = NULL;
	// reserva la memoria
	DatosX = new double [numDatos];
	DatosY = new double [numDatos];
	SigmaY = new double [numDatos];
	if ((DatosX == NULL)||(DatosY == NULL)||(SigmaY == NULL)) 
		{
		printf("ERROR: No hay memoria para reservar double[%d]\n", numDatos);
		delete [] DatosX; delete [] DatosY; delete [] SigmaY;
		return -2;
		}
	
	// comprueba que se le hayan pasado datos lógicos
	for(int i=0;i<numGaussianas;i++)
		{
	//	printf("cent[%d]=%f\n", i, centroides[i]);
		if(DaEnergia(centroides[i])<=0)	//OJO: Con esto se mete en bucle infinito
			{
			printf("ERROR: centroide[%d] <= 0\n", centroides[i]);
			return -1;
			}
		}
	// llena los vectores
	for (int i = 0; i < numDatos; i++) 
		{
		DatosX[i] = (double) i + intervalo->limInferior;
		DatosY[i] = (double) espectro[i + intervalo->limInferior] -
					(double) continuo[i + intervalo->limInferior];
		SigmaY[i] = sqrt(powi((double) sigmaContinuo[i + intervalo->limInferior], 2) +
							  espectro[i + intervalo->limInferior]);
		if(SigmaY[i]<1)	SigmaY[i]=1;
		/*Importantísimo este if: si un canal no tiene cuentas, este valor puede
					ser <1 en ese canal. Lo que indicará mínimo un O(-1) o generalmente
					un O(-2). Un O(-2) que luego cuando se calcula X2 en ajustes.C va
					al cuadrado y dividiendo, es decir, afecta como un O(4) cuando otros
					canales (O(1) ó O(2)) afectan como mucho en O(-2)!; luego el error
					del ajuste depende única y exclusivamente de lo cerca de cero que 
					quede este canal; lo cual claramente no es que esté muy bien
		*/
		}

	// Crea el vector de parametros de ajuste
	// TODO: dejamos los parametros de la segunda exponencial a cero y sin ajustar
	for (int i = 1; i <= intervalo->picos.numElementos; i++) 
		{
		NodoPico *pico=DevuelveNodo(intervalo->picos, i);
		if(!pico->Ajuste.inicializado)
			InicializaPico(pico->Ajuste, pico->maximo, true);
		}

	BorraAjustesParcialesEnGrafica();//Estas gráficas son las del mejor individuo

	int li=DaLimInferior(numIntervalo);
	int ls=DaLimSuperior(numIntervalo);
	int ancho=DaAnchoIntervalo(numIntervalo);
	float *v=DaVectorIntervalo(numIntervalo);
	
	float funcionTotal[ancho];
	clock_t finalTiempo=0;
	clock_t inicioIteracion=0;
	int j=0;
		
	float taus=-1;
	float sigmas=-1;
	taus=DaTauCh(ancho/2+li);	
	sigmas=DaSigmaCh(ancho/2+li);	
	
	printf("^^^ ANTES DE ML: Tau está en %.2f\n", taus);
	if(!calibracionCorrecta(li, ls+ancho))	return -7;
	
	//******************* MARQUARDT *******************
	// Construye los vectores necesarios para hacer un ajuste de intervalo
	printf("TENEMOS %d GAUSSIANAS Y %d PICOS\n", numGaussianas, intervalo->picos.numElementos);
	if(numGaussianas>intervalo->picos.numElementos)
		{
		int necesarias=numGaussianas-intervalo->picos.numElementos;
		for(int i=0;i<necesarias;i++)
			{
			NodoPico *PicoNuevo = new NodoPico;
			if (PicoNuevo == NULL) 
				{
				printf("ERROR: No hay memoria para reservar NodoPico\n");
				delete [] fondo; delete [] picos; 
				fondo = NULL; picos = NULL;
				return -4;
				}
			iniciaNodo(PicoNuevo);
			agnadeNodo(intervalo->picos, PicoNuevo);
			}
		}
	else if(numGaussianas<intervalo->picos.numElementos)
			{
			int inservibles=intervalo->picos.numElementos-numGaussianas;
			for(int i=0;i<inservibles;i++)		quitaNodo(intervalo->picos,0);
			}
	
	int numParam = numGaussianas*4;

	// reserva la memoria
	unsigned char *lista = new unsigned char[numParam];
	double *paramM = new double [numParam];
	double *dParamM = new double [numParam];
	double *covarParamM = new double [numParam*numParam];
	
	if ((paramM == NULL)||(dParamM == NULL)||(covarParamM == NULL)||(lista == NULL)) 
		{
		printf("ERROR: No hay memoria para reservar double[%d]\n", numParam);
		delete [] DatosX; delete [] DatosY; delete [] SigmaY;
		delete [] paramM; delete [] dParamM; delete [] covarParamM;
		delete [] lista;
		return -2;
		}

	for(int i=0;i<numParam;i++)
		{
		paramM[i]=dParamM[i]=0.0;
		for(int j=0;j<numParam;j++)		covarParamM[i*numParam+j]=0.0;
		}
		
	// Se ajustan todos los parametros
	int k=0;
	for (int i = 0; i < numParam; i++) 	lista[i]=1;//Antes
	int numAjustados=numParam;

	printf("Num gaussianas es %d, creando numParam\n", numGaussianas);
	for (int i = 0; i < numGaussianas; i++) 
		{
		paramM[i*4+0] = centroides[i];//Centroide
		printf("centroides[%d] vale %f\n", i, centroides[i]);
		paramM[i*4+1] = sigmas;//Sigma
		printf("sigmas[%d] vale %f\n", i, sigmas);
		paramM[i*4+2] = taus;//Tau
		printf("taus[%d] vale %f\n", i, taus);
		paramM[i*4+3] = espectro[(int)centroides[i]];//Amplitud
		printf("alturas[%d] vale %f\n", i, paramM[i*4+3]);
		}
	
	// asocia las matrices al objeto que ajusta los datos
	cAjusteChi Ajuste;
	Ajuste.x = DatosX; Ajuste.y = DatosY; Ajuste.sigma = SigmaY;
	Ajuste.a = paramM; Ajuste.dyda = dParamM;
	Ajuste.lista = lista; Ajuste.covar = covarParamM;
	Ajuste.numDatos = numDatos; Ajuste.ma = numParam; Ajuste.mfit = numAjustados; 
	Ajuste.AsociaFuncionActualiza((tFuncionNL) ActualizaVectorPicosAnal);
	
	Ajuste.sigmaInf=Ajuste.sigmaSup=-1;

	/*double *alturas = new double[numGaussianas];
	double *centros = new double[numGaussianas];
	printf("numGaussianas es %d\n", numGaussianas);
	for(int i=0;i<numGaussianas;i++)	
		{
		//centros[i]=centroides[i];
		centros[i]=0;
		alturas[i]=0;
		}
	
	Ajuste.alturas=alturas;
	Ajuste.centroides=centros;*/
	Ajuste.alturas=Ajuste.centroides=NULL;
	
	principioTiempo=clock();
	nError=Ajuste.Ajusta();//Pasamos una sola vez
	finalTiempo=clock();
	printf("tiempo empleado(ML): %f s (vel: %ld)\n", 2.0*(finalTiempo-principioTiempo)/CLOCKS_PER_SEC, CLOCKS_PER_SEC);
	printf("ERROR TRAS MARQUARDT: %f\n", Ajuste.ChiSq);
	
	//************ ACTUALIZACIÓN DE RESULTADOS *****************
	// aqui deberia actualizar los datos del ajuste en el pico (En temp)
	// deberíamos además ver si tenemos más gaussianas de las que en un 
	// principio obtuvimos con el ajuste inicial para en ese caso modificar
	// el número de picos estimados en el intervalo.
	
	printf("Valores tras Marquardt:\n");

	for (int i = 0; i < intervalo->picos.numElementos; i++) 
		{
		NodoPico *Pico = DevuelveNodo(intervalo->picos, i+1);
		Pico->Ajuste.ajustado = true;
		Pico->Ajuste.identificado = true;
		Pico->Ajuste.Centroide = paramM[i*4];
		Pico->Ajuste.Anchura   = paramM[i*4+1];
		Pico->Ajuste.Tau  	   = paramM[i*4+2];
		Pico->Ajuste.Ytau 	   = paramM[i*4+3];//Para ML
		printf("c:%.2f a:%.2f s=%.2f t=%.2f ar=%.2f\n", DaEnergia(Pico->Ajuste.Centroide), Pico->Ajuste.Ytau, Pico->Ajuste.Anchura, Pico->Ajuste.Tau, Pico->Ajuste.Ytau*Pico->Ajuste.Tau);
//		printf("c:%.2f a:%.2f s=%.2f t=%.2f ar=%.2f\n", DaEnergia(Pico->Ajuste.Centroide), Pico->Ajuste.Ytau, DaEnergia(Pico->Ajuste.Anchura), DaEnergia(Pico->Ajuste.Tau), Pico->Ajuste.Ytau*Pico->Ajuste.Tau);
		// y sus incertidumbres (matriz covarParaM)
//		printf("Accediendo a %d\n", (4*i+0)*(numParam+1) );
		Pico->Ajuste.SigCentroide = sqrt(covarParamM[(4*i+0)*(numParam+1)]);
		//printf("Accediendo a %d\n", (4*i+1)*(numParam+1) );
		Pico->Ajuste.SigAnchura   = sqrt(covarParamM[(4*i+1)*(numParam+1)]);
		//printf("Accediendo a %d\n", (4*i+2)*(numParam+1) );
		Pico->Ajuste.SigTau  	  = sqrt(covarParamM[(4*i+2)*(numParam+1)]);
		//printf("Accediendo a %d\n", (4*i+3)*(numParam+1) );
		Pico->Ajuste.SigYtau 	  = sqrt(covarParamM[(4*i+3)*(numParam+1)]);
		Pico->Ajuste.SigTauYtau   = covarParamM[(4*i+2)*(numParam+1)+1];
//		printf("incerTau=%f y covarTauAltura=%f\n", Pico->Ajuste.SigTau, Pico->Ajuste.SigTauYtau);}		//NOTA: OJO: creo que se hace necesario tener la matriz covarParam ENTERA
		//		almacenada, por si queremos, por ejemplo, calcular la incertidumbre
		//		de la convolución. O si no, por lo menos, crear SigCentroideAnchura, etc.
		for(int j=0;j<4;j++)//columna
			for(int k=0;k<4;k++)//fila
				Pico->Ajuste.covar[j][k]=covarParamM[(i*4+j)*(numParam) + (4*i)+k];
			
		Pico->Ajuste.area=DaAreaPico(numIntervalo, i+1);
		Pico->Ajuste.incerArea=DaIncerAreaPico(numIntervalo, i+1);
//		printf("Área del pico %d es %f(%f)\n", i, Pico->Ajuste.area, Pico->Ajuste.incerArea);
		}
	
		
		//Borramos estas gráficas provisionales
	for(j=0;j<numGaussianas;j++)	BorraGeneticEnGrafica(j+7);
	BorraGeneticEnGrafica(j+7);

	//******************* actualiza los datos con el nuevo ajuste
	ActualizaIntervaloAnalisis(numIntervalo); 

	// Limpia la memoria utilizada
	delete [] DatosX; delete [] DatosY; delete [] SigmaY;
	delete [] paramM; delete [] dParamM; delete [] covarParamM;
	delete [] lista;
	
	//*************** descarte de picos con mala estadística **********
	//NOTA: Según Bego, mejor hacer el descarte entre GA y ML
	
	calculaLC(numIntervalo);
	int numPicosValidos=0;
	float centroidesValidos[numGaussianas];
//	printf("Número de picos en el intervalo: %d y numero de Gaussianas: %d\n", intervalo->picos.numElementos, numGaussianas);
	bool picoValido=true;
	for (int i = 1; i <= intervalo->picos.numElementos; i++) 
		{
		NodoPico *pico = DevuelveNodo(intervalo->picos, i);
		picoValido=true;
		//1)Descarte por altura pequeña (límites de decisión)
		if(DaAreaPico(numIntervalo, i)>pico->Ajuste.lc)	
			{
			centroidesValidos[i-1]=centroides[i-1];
			}
		else	
			{
			picoValido=false;
			printf("AREA %f MENOR QUE LC (%f)\n", DaAreaPico(numIntervalo, i), pico->Ajuste.lc); 
			centroidesValidos[i-1]=-1; //No es válido
			}
		//2)Descarte por centroides más cercanos que la resolución
		if(i>1)
			{	
			NodoPico *picoAnt = DevuelveNodo(intervalo->picos, i-1);
			if(centroidesValidos[i-2]!=-1 && centroidesValidos[i-1]!=-1 && fabs(pico->Ajuste.Centroide-picoAnt->Ajuste.Centroide)<DaResolucionCh(pico->Ajuste.Centroide))	
				{//NOTA: Funciona mal esto!!!!
				printf("---Los centroides %d y %d están en %f y %f, con distancia de %f<%f\n", i-2, i-1, picoAnt->Ajuste.Centroide, pico->Ajuste.Centroide, fabs(picoAnt->Ajuste.Centroide-pico->Ajuste.Centroide), DaFWHGCh(pico->Ajuste.Centroide));
				picoValido=false;
				centroidesValidos[i-1]=-1;//Eliminamos el primero, para evitar que luego borremos un tercero que hacía falta
				}
			}
		if(picoValido)	numPicosValidos++;
		}
	
	if(numPicosValidos<numGaussianas)
		{
		//Alguno de los picos no es necesario para el ajuste, lo eliminamos y lo repetimos
		float nuevosCentroides[numPicosValidos];
		float nuevasRestricciones[numPicosValidos];
			
		int j=0;
		for(int i=0;i<numGaussianas;i++)
			{
			if(centroidesValidos[i]!=-1)	
				{
				nuevosCentroides[j]=centroidesValidos[i];
				j++;
				}
			}
		if(numPicosValidos>0)	
			{
			printf("Tenemos que reajustar \n");
			nError=AjusteIntervaloML(numIntervalo, nuevosCentroides, numPicosValidos);
			}
		else		
			{		
			printf("Se nos han acabado los picos mediante GA, realizamos sólo un LM\n");				
		//	AjusteIntervalo(numIntervalo);
			printf("Volvemos del LM\n");
			nError=-6;//Para que sepa que GAML no ha podido hacer nada
			}
		}
		
	//Gestión de memoria	
	delete [] v;
	
	return nError;
		
	}
//FIN AJUSTE INTERVALO ML	
	
//*********** AJUSTE INTERVALO GA *******************************
/*	Realiza el ajuste de un intervalo mediante algoritmos genéticos
	NOTA: 10-I-05 Decicido a la vista de los resultados eliminar la primera 
				  fase GAI. Su necesidad es nula. Frente al mismo ejemplo y
				la misma configuración la comparación es:
		GA1 + GA2 + GS	t=67.36=5.06+11.76+93.96	e=18.42->17.21->3.42
		GA2 + GS		t=93.96=     5.24+62.12		e=       13.32->2.86
		Tenemos una ganancia de casi medio minuto en el caso más complicado
		y una mejora de alrededor de medio punto.
		
		Queda la clase GeneticAlgorithm por si la necesitáramos, aunque usaremos
		GeneticAlgorithm2.
	
	Por tanto el ajuste se realiza mediante un algoritmo genético que ajusta
	los centroides que se pasan como parámetro en el array, de longitur numGaussianas
	numIntervalo indica la posición del intervalo dentro de todo el espectro
*/
//NOTA: Gestión de la memoria no fiable
int cEspectros::AjusteIntervaloGA(int numIntervalo, float *centroides, int numGaussianas)
	{
	// selecciona el intervalo que hay que ajustar
	NodoPicos *intervalo = DevuelveNodo(listaPicos, numIntervalo);
	intervaloAjuste = intervalo;
	int nError=0;

	// Crea el vector de parametros de ajuste
	// TODO: dejamos los parametros de la segunda exponencial a cero y sin ajustar
//	int topeIteraciones = 1;

	for (int i = 1; i <= intervalo->picos.numElementos; i++) 
		{
		NodoPico *pico = DevuelveNodo(intervalo->picos, i);
		if(!pico->Ajuste.inicializado)
			InicializaPico(pico->Ajuste, pico->maximo, true);
		}
		
	//COMIENZA EL ALGORITMO GENÉTICO	
	//Preparamos los datos de los centroides
	int numGeneraciones=20;//10 generaciones se muestran suficientes
	int li=DaLimInferior(numIntervalo);
	int ls=DaLimSuperior(numIntervalo);
	int ancho=DaAnchoIntervalo(numIntervalo);
	float *v=DaVectorIntervalo(numIntervalo);
	float error[numGeneraciones];
	float funcionTotal[ancho];
	clock_t finalTiempo;
	int j;

	//********************** PARTE I: ALGORITMO GENÉTICO *****************		
	//Calculamos las taus para elegir buenas amplitudes (a=area/tau)
	float *taus=new float[ancho];
	float *sigmas=new float[ancho];
	for(int i=0;i<ancho;i++)	taus[i]=DaTauCh(i+li);
	for(int i=0;i<ancho;i++)	sigmas[i]=DaSigmaCh(i+li);
		
	GeneticAlgorithm2 *GA2=new GeneticAlgorithm2(100,10,10,0.8,numGaussianas,v,ancho,this,li,centroides,taus, sigmas);

	GA2->calculaAjuste();	
	GA2->ordenar();
	printf("El mejor ajuste nada más construir GA es %f\n", GA2->getMejorAjuste());
	float *ini=new float[numGaussianas*2+2];
	//float *ini=GA2->getParametros();
	GA2->getParametros(ini);
	for(int i=0;i<numGaussianas;i++)	printf("Altura %d tras GA es %f\n", i, ini[i+numGaussianas]);
	
	for(j=0;j<numGaussianas;j++)
		{
		//float *funcion=GA2->getFuncion(j);//Nos da la función de gauss del mejor individuo para el parámetro j
		float *funcion=new float[ancho];
		funcion=GA2->getFuncion(j);
		BorraGeneticEnGrafica(j+7);//Estas gráficas son las del mejor individuo
		CopiaGeneticAGrafica(funcion, li, ls, j+7);
		ActivaGeneticGrafica(true, j+7);
		PintaGrafica(0);
		delete [] funcion;
		}
		
	for(int i=0;i<numGeneraciones;i++)
	//for(int i=0;GA2->getDivergencia()>0.0000001;i++)
		{
		GA2->ejecutar(1,0.001);
	
		//Imprimimos cada una de sus gaussianas
		for(j=0;j<numGaussianas;j++)
			{
			float *funcion=new float[ancho];
			funcion=GA2->getFuncion(j);//Nos da la función de gauss del mejor individuo para el parámetro j
			BorraGeneticEnGrafica(j+7);//Estas gráficas son las del mejor individuo
			CopiaGeneticAGrafica(funcion, li, ls, j+7);
			ActivaGeneticGrafica(true, j+7);
			PintaGrafica(0);
			for(int k=0;k<ancho;k++)	funcionTotal[k]+=funcion[k];
			delete [] funcion;
			}
		//Imprimimos la gaussiana total
		BorraGeneticEnGrafica(j+7);//Estas gráficas son las del mejor individuo
		CopiaGeneticAGrafica(funcionTotal, li, ls, j+7);
		ActivaGeneticGrafica(true, j+7);
		PintaGrafica(0);
			
		for(int k=0;k<ancho;k++)	funcionTotal[k]=0;
		//Imprimimos el error
		error[i]=GA2->getMejorAjuste();
//		cout << "DIVERGENCIA: " << GA2->getDivergencia() << endl;
		}
	
	printf("FIN GA con error %f\n", GA2->getMejorAjuste());
	finalTiempo=clock();
	printf("tiempo empleado(GA): %f s (vel: %d)\n", 2.0*(finalTiempo-principioTiempo)/CLOCKS_PER_SEC, (int)CLOCKS_PER_SEC);
		
	//********************* 2ª PARTE (BÚSQUEDA LOCAL) - GRID SEARCH *************		
	float *temp=new float[numGaussianas*2+2];
	GA2->getParametros(temp);
	float *parametros=new float[numGaussianas*2+2];
	//for(int i=0;i<(numGaussianas*2+2);i++)	printf("parametro %d es %f\n", i, temp[i]);
	for(int i=0;i<numGaussianas;i++)	printf("Altura %d tras GA es %f\n", i, temp[i+numGaussianas]);
	principioTiempo=clock();
	float *parametros2=new float[numGaussianas*2+2];
	float *sigmaparam2=new float[numGaussianas*2+2];
	
	gridSearch(temp, numGaussianas*2+2, li, ancho, v, numIntervalo, parametros2, sigmaparam2);
	finalTiempo=clock();

	printf("tiempo empleado(GS): %f s (vel: %d)\n", 2.0*(finalTiempo-principioTiempo)/CLOCKS_PER_SEC, (int)CLOCKS_PER_SEC);

	for(j=0;j<numGaussianas;j++)
		{
		float *funcion=new float[ancho];
		float sigma=parametros[numGaussianas*2];
		float tau=parametros[numGaussianas*2+1];
		float c=parametros[j];
		float a=parametros[j+numGaussianas];
		printf("Altura %d terminado GS es %f\n", j, a);
		for(int i=0;i<ancho;i++)
			funcion[i]=DaValorConvolucion(c, a, sigma, tau, (double)(i+li) );
			
		BorraGeneticEnGrafica(j+7);
		CopiaGeneticAGrafica(funcion, li, ls, j+7);
		ActivaGeneticGrafica(true, j+7);
		PintaGrafica(0);
		for(int k=0;k<ancho;k++)	funcionTotal[k]+=funcion[k];
		}

	BorraGeneticEnGrafica(j+7);
	CopiaGeneticAGrafica(funcionTotal, li, ls, j+7);
	ActivaGeneticGrafica(true, j+7);
	PintaGrafica(0);
	for(int k=0;k<ancho;k++)	funcionTotal[k]=0;

	//************* Selección de emisiones ***************
	//Este trozo de código es un poco idiota, sobre todo poner una función 
	//para hacer lo mismo que estamos haciendo aquí
	int numPicos=0;
	float amplitudUmbral=10;
	printf("INICIALMENTE TENEMOS %d GAUSSIANAS\n", numGaussianas);
	for(j=0;j<numGaussianas;j++)
		{
		float c=parametros[j];
		float a=parametros[j+numGaussianas];
		if(a>amplitudUmbral)	
			{
			printf("La gaussiana %d debe aparecer tiene a=%f y c=%f\n",j,a,c);
			numPicos++;
			}
		else	printf("La gaussiana %d NO debe aparecer tiene a=%f\n", j, a);
		}
	
	float *nuevosParametros=quitarGaussianasNulas(parametros, numGaussianas, numPicos, amplitudUmbral);
	//
	printf("Tenemos que añadir (%d-%d)=%d picos nuevos\n", numPicos, intervalo->picos.numElementos, numPicos-intervalo->picos.numElementos);
	if(numPicos>intervalo->picos.numElementos)
		{
		int nuevos=numPicos-intervalo->picos.numElementos;
		for(int i=0;i<nuevos;i++)
			{
			NodoPico *PicoNuevo = new NodoPico;
			if (PicoNuevo == NULL) 
				{
				delete [] fondo; delete [] picos; 
				fondo = NULL; picos = NULL;
				return -4;
				}
			iniciaNodo(PicoNuevo);
			agnadeNodo(intervalo->picos, PicoNuevo);
			}
		}
	else if(numPicos<intervalo->picos.numElementos)
		{//El preajuste se ha equivocado y metió picos de más
		int inservibles=intervalo->picos.numElementos-numPicos;
		for(int i=0;i<inservibles;i++)		quitaNodo(intervalo->picos,0);
		}
	printf("Con lo que tenemos %d picos\n", intervalo->picos.numElementos);
	
	//************FIN DE GA + GRID SEARCH *****************
	// aqui deberia actualizar los datos del ajuste en el pico (En temp)
	// deberíamos además ver si tenemos más gaussianas de las que en un 
	// principio obtuvimos con el ajuste inicial para en ese caso modificar
	// el número de picos estimados en el intervalo.
	
	for (int i = 0; i < intervalo->picos.numElementos; i++) 
		{
		NodoPico *Pico = DevuelveNodo(intervalo->picos, i+1);
		Pico->Ajuste.Centroide = nuevosParametros[i];
		Pico->Ajuste.Anchura   = nuevosParametros[numPicos*2];
		Pico->Ajuste.Tau  	   = nuevosParametros[numPicos*2+1];
		Pico->Ajuste.Ytau 	   = nuevosParametros[i+numPicos];
		//printf("TAU E Y TAU VALEN %f %f para pico %d\n", Pico->Ajuste.Tau, Pico->Ajuste.Ytau, i);
			// y sus incertidumbres (matriz covarParaM)
		Pico->Ajuste.SigCentroide = nuevosParametros[numPicos*2+1 +i];
		Pico->Ajuste.SigAnchura   = nuevosParametros[numPicos*4+1];
		Pico->Ajuste.SigTau  	  = nuevosParametros[numPicos*4+2];
		Pico->Ajuste.SigYtau 	  = nuevosParametros[numPicos*2+1+i+numPicos];
		//Pico->Ajuste.SigTauYtau   = covarParamM[(4*i+2)*(numParam+1)+1];
		Pico->Ajuste.SigTauYtau   = 0;	//NOTA: De momento
		printf("SigTauYtau vale %f\n", Pico->Ajuste.SigTauYtau);
		//NOTA: Acualizamos el valor de areaSinFondo aunque de momento no contemos el fondo
		//Pico->Ajuste.area		  = DaAreaGaussiana(Pico->Ajuste.Ytau);
		}

	for (int i = 1; i <= intervalo->picos.numElementos; i++) 
		{
		NodoPico *pico = DevuelveNodo(intervalo->picos, i);
		if(DaIncerAreaPico(numIntervalo, i)<DaAreaPico(numIntervalo,i))
				pico->Ajuste.ajustado = true;
		else	quitaNodo(intervalo->picos, i);//No nos vale, tiene una incertidumbre demasiado alta
			
		}
//	ActualizaIntervaloAnalisis(numIntervalo, 4); // actualiza los datos con el nuevo ajuste
	ActualizaIntervaloAnalisis(numIntervalo); // actualiza los datos con el nuevo ajuste

	return nError;
	}

//********************** QUITAR GAUSSIANAS NULAS *******************
/*	Función que elimina de parámetros los valores que tengan amplitud
	menor que amplitud.
	La manera de decidir si la gaussiana aparece o no debe hacerse con
	un criterio estadístico más serio que el valor de la amplidud
	*/
float *cEspectros::quitarGaussianasNulas(float *parametros, int longitud, int numPicos, float amplitud)
	{
	float *nuevos=new float[numPicos*4+4];
	if(nuevos==NULL)
		{
		printf("ERROR: No hay memoria para reservar float[%d]\n", numPicos*4+4);
		return NULL;
		}
	int ind=0;
		
	for(int i=0;i<longitud;i++)
		{
		printf("Mirando el pico %d\n", i);
		if(parametros[i+longitud]>amplitud)
			{
			printf("metiendo elemento %d: %f,%f\n", ind, parametros[i], parametros[i+longitud]);
			nuevos[ind]=parametros[i];
			nuevos[numPicos+ind]=parametros[i+longitud];
			nuevos[numPicos*2+1+ind]=parametros[longitud*2+i+1];
			nuevos[numPicos*3+1+ind]=parametros[longitud*2+i+1];
			ind++;
			}
		}
	nuevos[numPicos*2]=parametros[longitud*2];
	nuevos[numPicos*2+1]=parametros[longitud*2+1];
		
	nuevos[numPicos*4+1]=parametros[longitud*4+1];
	nuevos[numPicos*4+2]=parametros[longitud*4+2];
	for(int i=0;i<numPicos*2+2;i++)	printf("nuevos[%d]=%f\n", i, nuevos[i]);
	return nuevos;
	}	

//********************** QUITAR GAUSSIANAS NULAS ******************
/*	Elimina las gaussianas que tengan una incertidumbre en el área 
mayor que su propio área.  
*/	
float *cEspectros::quitarGaussianasNulas(float *parametros, int numIntervalo, int longitud, int numPicos)
	{
	float *nuevos=new float[numPicos*4+4];
	if(nuevos==NULL)
		{
		printf("ERROR: No hay memoria para reservar float[%d]\n", numPicos*4+4);
		return NULL;
		}
	int ind=0;
	NodoPicos *intervalo = DevuelveNodo(listaPicos, numIntervalo);

	for(int i=0;i<intervalo->picos.numElementos;i++)
		{
		NodoPico *pico = DevuelveNodo(intervalo->picos, i);
		if (pico->Ajuste.ajustado) 
			{
			printf("Mirando el pico %d\n", i);
			if(DaIncerAreaPico(numIntervalo, i)<DaAreaPico(numIntervalo,i))
				{//Metemos un nuevo pico si tenemos más área que incertidumbre
				printf("metiendo elemento %d: %f,%f\n", ind, parametros[i], parametros[i+longitud]);
				nuevos[ind]=parametros[i];
				nuevos[numPicos+ind]=parametros[i+longitud];
				nuevos[numPicos*2+1+ind]=parametros[longitud*2+i+1];
				nuevos[numPicos*3+1+ind]=parametros[longitud*2+i+1];
				ind++;
				}
			}
		}
	nuevos[numPicos*2]=parametros[longitud*2];
	nuevos[numPicos*2+1]=parametros[longitud*2+1];
		
	nuevos[numPicos*4+1]=parametros[longitud*4+1];
	nuevos[numPicos*4+2]=parametros[longitud*4+2];
	
	for(int i=0;i<numPicos*2+2;i++)	printf("nuevos[%d]=%f\n", i, nuevos[i]);
	return nuevos;
	}	

//***************************** AJUSTE PICO ***********************************
/** Realiza el ajuste de los picos en base a las pérdidas por viaje de fotones y 
	de absorción de e-; además de la línea base según el cálculo del centroide, etc.
    NOTA: De momento el viaje de fotones no está implementado
	Es un ajuste tradicional por mínimos cuadrados.
	AjustePico quita cualquier restricción que pudiera haber en el ajuste
*/
int cEspectros::AjustePico(int numIntervalo, int numPico, bool HayCalibracion, bool ConInicializacion) 
	{
	// selecciona el pico que hay que ajustar (selecciona el intervalo, y de ese intervalo el pico)
	// (en principio sólo habrá un pico en el intervalo y numPico=1)
	NodoPicos *intervalo = DevuelveNodo(listaPicos, numIntervalo);
	NodoPico *Pico = DevuelveNodo(intervalo->picos, numPico);

	// si no hay vector para recoger los datos totales lo intenta crear
	if (total == NULL) 
		{
		if ((total = new float[numCanales+1]) == NULL) 
			{
			printf("ERROR: No hay memoria para reservar float[%d]\n", numCanales+1);
			return -2;
			}
		// pone a cero todo el vector
		for (int i = 1; i <= numCanales; i++)		total[i] = 0.0;
		}
	
	// Construye los vectores necesarios para hacer un ajuste de pico
	// a los datos de este intervalo	
	// Cogemos el número de canales del intervalo (que no del pico)
	int numDatos = intervalo->limSuperior - intervalo->limInferior + 1;
	double *DatosX = NULL, *DatosY = NULL, *SigmaY = NULL;
	// reserva la memoria
	DatosX = new double [numDatos];
	DatosY = new double [numDatos];
	SigmaY = new double [numDatos];
	if ((DatosX == NULL)||(DatosY == NULL)||(SigmaY == NULL)) 
		{
		printf("ERROR: No hay memoria para reservar double[%d]\n", numDatos);
		delete [] DatosX; delete [] DatosY; delete [] SigmaY;
		return -2;
		}
	// llena los vectores
	for (int i = 0; i < numDatos; i++) 
		{
		DatosX[i] = (double) i + intervalo->limInferior;
		DatosY[i] = (double) espectro[i + intervalo->limInferior] -
						(double) continuo[i + intervalo->limInferior];
		SigmaY[i] = sqrt(powi((double) sigmaContinuo[i + intervalo->limInferior], 2) +
							  espectro[i + intervalo->limInferior]);
		}
	
	// Crea el vector de parametros de ajuste
	// TODO: dejamos los parametros de la segunda exponencial a cero y sin ajustar
	int topeIteraciones = 5;
	// ConInicializacion quiere decir que queremos que el ajuste lleve una inicialización, no
	// que ya esté hecha.
	if (ConInicializacion) 
		{
		printf("Inicializamos pico\n");
		InicializaPico(Pico->Ajuste, Pico->maximo, HayCalibracion);
		topeIteraciones = 1;
		}
	
	//NOTA: Supongo que cuando tengamos el ajuste de los fotones tendremos que añadir otros dos parámetros
	param[0] = Pico->Ajuste.Centroide;
	param[1] = Pico->Ajuste.Anchura;
	param[2] = Pico->Ajuste.Tau;
	param[3] = Pico->Ajuste.Ytau;
	
	// Se ajustan todos los parametros
	unsigned char *lista = new unsigned char[4];
	if (lista == NULL) 
		{
		printf("ERROR: No hay memoria para reservar unsigned char[%d]\n", 4);
		delete [] DatosX; delete [] DatosY; delete [] SigmaY;
		return -2;
		}
	for (int i = 0; i < 4; i++) lista[i] = 1;
		
	// asocia las matrices al objeto que ajusta los datos
	cAjusteChi Ajuste;
	Ajuste.x = DatosX; Ajuste.y = DatosY; Ajuste.sigma = SigmaY;
	Ajuste.a = param; Ajuste.dyda = dParam;
	Ajuste.lista = lista; Ajuste.covar = covarParam;
	Ajuste.numDatos = numDatos; Ajuste.ma = 4; Ajuste.mfit = 4; 
	Ajuste.AsociaFuncionActualiza((tFuncionNL) ActualizaVectorPicoCalib);
	//No usamos restricciones:
	Ajuste.centroides=NULL;
	Ajuste.alturas=NULL;
	Ajuste.tauInf=Ajuste.tauSup=Ajuste.sigmaInf=Ajuste.tauSup=0;
	
	// hace el ajuste repetidas veces ya que cada vez utiliza los datos
	// conseguidos como una exploracion mas cercana de los minimos locales
	// Haremos una sola iteración en caso de tener la inicialización. Si no haremos 5
	for (int i = 0; i < topeIteraciones; i++) 
		{
		int nError = Ajuste.Ajusta();
		printf("Error es %d\n", nError);
		if (nError < 0) 
			{
			delete [] DatosX; delete [] DatosY; delete [] SigmaY;
			delete [] lista;
			if (nError == -1) return -2;
			else return -1;
			}
		}
	
	// aqui deberia actualizar los datos del ajuste en el pico
	// NOTA: creo que los param están asociados a ajuste.a por lo tanto han cambiado en Ajusta()
	//	 comprobarlo
	Pico->Ajuste.Centroide = param[0];
	Pico->Ajuste.Anchura = param[1];
	Pico->Ajuste.Tau = param[2];
	Pico->Ajuste.Ytau = param[3];
		// y sus incertidumbres (matriz covarParam 4*4)
		//(las varianzas están en la diagonal)
	Pico->Ajuste.SigCentroide = sqrt(covarParam[0]);
	Pico->Ajuste.SigAnchura = sqrt(covarParam[1*4+1]);
	Pico->Ajuste.SigTau = sqrt(covarParam[2*4+2]);
	Pico->Ajuste.SigYtau = sqrt(covarParam[3*4+3]);
	Pico->Ajuste.SigTauYtau = covarParam[2*4+2+1];
	
	/* Las ChiSq dan diferentes del ajuste anterior a la actualizacion posterior
	   porque una se hace sobre el espectro neto y la otra sobre el espectro
	   total */
	Pico->Ajuste.ajustado = true;

	Pico->Ajuste.area=DaAreaPico(numIntervalo, numPico);
	Pico->Ajuste.incerArea=DaIncerAreaPico(numIntervalo, numPico);
	
	ActualizaIntervaloAnalisis(numIntervalo); // actualiza los datos con el nuevo ajuste
//	ActualizaIntervaloAnalisis(numIntervalo,4); // actualiza los datos con el nuevo ajuste
	
	// Limpia la memoria utilizada
	delete [] DatosX; delete [] DatosY; delete [] SigmaY;
	delete [] lista;

	return 0;
	}

//******************* ACTUALIZA INTERVALO CALIBRACIÓN **********************
// 	Calcula el valor de la función de ajuste para cada canal de cada pico,
//	además del área y su desviación y la chi cuadrado asociada.
//  Se llama en calibración, con lo que para los grados de libertad sólo
//	utilizamos 4 parámetros (una gaussiana).
//  NOTA: Aparte, yo no la he tocado aunque hay cosas que igual deberían corregir
void cEspectros::ActualizaIntervaloCalibracion(int numIntervalo) 
	{
	//printf("ACTUALIZA INTERVALO CALIBRACION\n");
	// recoge los datos del intervalo
	NodoPicos *intervalo = DevuelveNodo(listaPicos, numIntervalo);

	// calcula el ChiSq de este ajuste
	double temp = 0.0, acum = 0.0, acumArea = 0.0, sigAcumArea = 0.0;
	// Calcula el chi cuadrado de la forma vista; aparte de las áreas.
	for (int i = intervalo->limInferior; i <= intervalo->limSuperior; i++) 
		{
		// Guarda la forma de los picos del intervalo
		total[i] = DaValorIntervalo(numIntervalo, (double) i);
		// Guarda su area neta y calcula la desviacion de cada canal
		acumArea += (double)espectro[i] - (double)continuo[i];
		sigAcumArea += sigmaContinuo[i]*sigmaContinuo[i];
		
		temp = total[i] - espectro[i];
		if (espectro[i] > 0) 
			{
			temp /= sqrt((double)espectro[i]);
			sigAcumArea += espectro[i];
			}
		
		temp = powi(temp, 2);
		acum += temp;
		}
	intervalo->ChiSq = acum / (intervalo->limSuperior-intervalo->limInferior+1-4); // se ajustan 4 parametros por cada emisión
//	intervalo->ChiSq = acum / (intervalo->limSuperior-intervalo->limInferior+3); // se ajustan 4 parametros por cada emisión
	intervalo->Area = acumArea;
	intervalo->sigArea = sqrt(sigAcumArea);

	total[intervalo->limInferior-2] = DaValorIntervalo(numIntervalo, (double) intervalo->limInferior-2);
	total[intervalo->limInferior-1] = DaValorIntervalo(numIntervalo, (double) intervalo->limInferior-1);
	total[intervalo->limSuperior+1] = DaValorIntervalo(numIntervalo, (double) intervalo->limSuperior+1);
	total[intervalo->limSuperior+2] = DaValorIntervalo(numIntervalo, (double) intervalo->limSuperior+2);
	}

//********************* ACTUALIZA INTERVALO ANÁLISIS ****************
/*	Función similar a la de calibración, pero con un ajuste un tanto distinto
	*/
void cEspectros::ActualizaIntervaloAnalisis(int numIntervalo) 
	{
//	printf("ACTUALIZA INTERVALO ANÁLISIS\n");
	// recoge los datos del intervalo
	NodoPicos *intervalo = DevuelveNodo(listaPicos, numIntervalo);

	// calcula el ChiSq de este ajuste
	double temp = 0.0, acum = 0.0, acumArea = 0.0, sigAcumArea = 0.0;
	// Calcula el chi cuadrado de la forma vista; aparte de las áreas.
	for (int i = intervalo->limInferior; i <= intervalo->limSuperior; i++) 
		{
		// Guarda la forma de los picos del intervalo
		total[i] = DaValorIntervalo(numIntervalo, (double) i);
		// Guarda su area neta y calcula la desviacion de cada canal
		acumArea += (double)espectro[i] - (double)continuo[i];
		sigAcumArea += sigmaContinuo[i]*sigmaContinuo[i];//El espectro no tiene incertidumbre
		
		temp = total[i] - espectro[i];
		if (espectro[i] > 0) 
			{
			temp /= sqrt((double)espectro[i]);
			sigAcumArea += espectro[i];
			}
		temp = powi(temp, 2);
		acum += temp;
		//printf("acum suma %f (+%f)\n", acum, temp);
		}
		
	intervalo->Area = acumArea;
	intervalo->sigArea = sqrt(sigAcumArea);
	
	//NOTA: Dependiendo de si al final nos quedamos con ML o con GS el número de parámetros ajustados es distinto
	//		Porque GS considera iguales para todas las energías tau y sigma.
//	printf("X² sin contar grados de libertad: %f\n", acum);
//	printf("Dividiendo X2 entre %d-%d\n", (intervalo->limSuperior-intervalo->limInferior+1), (intervalo->picos.numElementos*2+2) );
	int n=(intervalo->limSuperior-intervalo->limInferior+1);
	int m=(intervalo->picos.numElementos*2+2);
	int gl=n-m;
	printf("----------------------\n\n\n\n-------------------------------Grados de libertad: %d-%d=%d\n", n,m, gl);
	intervalo->ChiSq = acum/( (intervalo->limSuperior-intervalo->limInferior+1)-(intervalo->picos.numElementos*2+2) );//Antes *4, ahora dejamos fijo centroide y tau
//	printf("X² ahora: %f\n", intervalo->ChiSq);

	total[intervalo->limInferior-2] = DaValorIntervalo(numIntervalo, (double) intervalo->limInferior-2);
	total[intervalo->limInferior-1] = DaValorIntervalo(numIntervalo, (double) intervalo->limInferior-1);
	total[intervalo->limSuperior+1] = DaValorIntervalo(numIntervalo, (double) intervalo->limSuperior+1);
	total[intervalo->limSuperior+2] = DaValorIntervalo(numIntervalo, (double) intervalo->limSuperior+2);
	
	acumArea=0;
	sigAcumArea=0;
	for (int i = 1; i <= intervalo->picos.numElementos; i++) 
		{
		NodoPico *pico = DevuelveNodo(intervalo->picos, i);
		pico->Ajuste.area=DaAreaPico(numIntervalo, i);
		pico->Ajuste.incerArea=DaIncerAreaPico(numIntervalo, i);
		acumArea+=pico->Ajuste.area;
		sigAcumArea+=pow(pico->Ajuste.incerArea, 2);
		}
	
	printf("Área: %f y sigÁrea: %f\n", intervalo->Area, intervalo->sigArea);
	printf("Área: %f y sigÁrea ajuste: %f\n", acumArea, sqrt(sigAcumArea));
	}

//******************** BORRA INTERVALO *******************
void cEspectros::BorraIntervalo(int numIntervalo)
	{
	// recoge los datos del intervalo
	printf("BORRA INTERVALO %d\n", numIntervalo);
	NodoPicos *intervalo = DevuelveNodo(listaPicos, numIntervalo);
	//printf("Tomamos todos sus picos y los borramos\n");
	// repasa todos los picos y les quita el flag de ajustados
	for (int i = 1; i <= intervalo->picos.numElementos; i++) 
		{
		//printf("Borramos pico %d\n",i);
		NodoPico *Pico = DevuelveNodo(intervalo->picos, i);
		Pico->Ajuste.ajustado = false;
		Pico->Ajuste.analizado= false;
		
		if(Pico->Ajuste.identificado==true)
			{
			Pico->Ajuste.identificado = false;
			printf("Borramos sus emisiones, que son %d\n", Pico->Ajuste.emisiones->numElements());
			while(Pico->Ajuste.emisiones->numElements()>0)
				{
				printf("Borramos el nodo\n");
				NodoEmision *ne=(NodoEmision *)Pico->Ajuste.emisiones->getFirst();
				printf("Borramos su nombre si hay\n");
				if(ne->nombre!=NULL)	delete [] ne->nombre;
				printf("free de ne\n");
				//free((void *)ne);
				delete ne;
				printf("Borramos del ajuste\n");
				Pico->Ajuste.emisiones->deleteFirst();
				printf("Borrado\n");
				}
			printf("Borramos en general\n");
			//free(Pico->Ajuste.emisiones);
			delete Pico->Ajuste.emisiones;
			printf("Borrado\n");
			}
		/*printf("Borramos parámetros eficiencia\n");
		if(Pico->Ajuste.parametrosEficiencia!=NULL)	delete [] Pico->Ajuste.parametrosEficiencia;
		printf("borrada la eficiencia\n");*/
		}
		
	intervalo->ChiSq 	= 0.0;
	intervalo->Area 	= 0.0; 
	intervalo->sigArea 	= 0.0;
	
	//printf("Borra todos los datos del vector total\n");		
	// borra los datos de ajuste en el vector total
	int li=intervalo->limInferior-2;
	if(li<0)	li=0;
	int ls=intervalo->limSuperior+2;
	if(ls>numCanales-1)	ls=numCanales-1;
	for (int i = li; i <= ls; i++)		total[i] = 0.0;
	printf("termina borraIntervalo\n");
	}

//******************** DA VALOR INTERVALO *********************
// Devuelve la suma de todas las convoluciones de gauss+Ytau de los picos
// que afecten al canal más el continuo en dicho punto.
double cEspectros::DaValorIntervalo(int numIntervalo, double canal) 
	{
	double Acum = 0.0;
	NodoPicos *intervaloPico = DevuelveNodo(listaPicos, numIntervalo);
	//Si el canal está en el intervalo que se pasa como parámetro ...
//	printf("1\n");
	if ((canal >= intervaloPico->limInferior-2) && (canal <= intervaloPico->limSuperior+2)) 
		{
//		printf("2\n");
		//Se calcula el valor que se le da al continuo en ese punto
		Acum = (double) continuo[redondea(canal)];
		for (int i = 1; i <= intervaloPico->picos.numElementos; i++) 
			{
			//Le añadimos al acum los valores de los picos en ese intervalo que afectan al canal, según 
			//la fórmula derivada de la convolución de la gaussiana con la exponencial tau
			NodoPico *Pico = DevuelveNodo(intervaloPico->picos, i);
//			printf("3\n");
			if (Pico->Ajuste.ajustado && !Pico->Ajuste.invisible) 
				{
				// pasa el canal a energia
				double temp = Pico->Ajuste.Ytau / 2.0;
//				printf("4\n");
				if(temp!=0)
					{
					temp *= erfc(((canal-Pico->Ajuste.Centroide)/Pico->Ajuste.Anchura + (Pico->Ajuste.Anchura/Pico->Ajuste.Tau))*M_SQRT1_2);
//					printf("5\n");
					if(temp!=0)
						{
						temp *= exp((powi(Pico->Ajuste.Anchura/Pico->Ajuste.Tau, 2)/2.0) +
					    ((canal-Pico->Ajuste.Centroide)/Pico->Ajuste.Tau));
						Acum += temp;
						}
					}
				}
			}
		}
//	printf("terminamos\n");
	return Acum;
	}

//******************* DA VALOR INTERVALO *************************
/*	Devuelve el 
	*/
double cEspectros::DaValorIntervalo(NodoPicos *intervaloPico, double canal) 
	{
	double Acum = 0.0;
	
	//Si el canal está en el intervalo que se pasa como parámetro ...
	if ((canal >= intervaloPico->limInferior-2) && (canal <= intervaloPico->limSuperior+2)) 
		{
		//Se calcula el valor que se le da al continuo en ese punto
		Acum = (double) continuo[redondea(canal)];
		for (int i = 1; i <= intervaloPico->picos.numElementos; i++) 
			{
			//Le añadimos al acum los valores de los picos en ese intervalo que afectan al canal, según 
			//la fórmula derivada de la convolución de la gaussiana con la exponencial tau
			NodoPico *Pico = DevuelveNodo(intervaloPico->picos, i);
			if (Pico->Ajuste.ajustado) 
//			printf("Este pico es invisible?: %d\n", Pico->Ajuste.invisible);
//			if (Pico->Ajuste.ajustado && !Pico->Ajuste.invisible) 
				{
				// pasa el canal a energia
				double temp = Pico->Ajuste.Ytau / 2.0;
				if(temp!=0)
					{
					temp *= erfc(((canal-Pico->Ajuste.Centroide)/Pico->Ajuste.Anchura + (Pico->Ajuste.Anchura/Pico->Ajuste.Tau))*M_SQRT1_2);
					if(temp!=0)
						{
						temp *= exp((powi(Pico->Ajuste.Anchura/Pico->Ajuste.Tau, 2)/2.0) +
					    ((canal-Pico->Ajuste.Centroide)/Pico->Ajuste.Tau));
						Acum += temp;
						}
					}
				}
			}
		}
	return Acum;
	}

//******************** DA VALOR PICO *********************
// Devuelve la convolución gauss+itau para este pico
double cEspectros::DaValorPico(int numIntervalo, int numPico, double canal) 
	{
	double Acum = 0.0;
	NodoPicos *intervaloPico = DevuelveNodo(listaPicos, numIntervalo);
	NodoPico *Pico=DevuelveNodo(intervaloPico->picos, numPico);
		
	//Si el canal está en el intervalo que se pasa como parámetro ...
	if ((canal >= intervaloPico->limInferior-2) && (canal <= intervaloPico->limSuperior+2)) 
		{
		//Se calcula el valor que se le da al continuo en ese punto
		Acum = (double) continuo[redondea(canal)];
		//Le añadimos al acum los valores de los picos en ese intervalo que afectan al canal, según 
		//la fórmula derivada de la convolución de la gaussiana con la exponencial tau
		if (Pico->Ajuste.ajustado) 
			{
			// pasa el canal a energia
			//El modelo de la forma del pico se supone que se corresponde a la fórmula:
			//1/(2*anchura*sqrt(2pi)) * Ytau * exp(0.5*(anchura/tau)) + (canal-centroide)/tau) * 
			//		erfc((canal-centroide)/anchura+anchura/tau)
			// Según esto, no sé a qué viene: - dividir entre 2 Ytau (suple el 1/2 del primer multiplicador)
			//				  --> multiplicar lo de erfc por sqrt(0.5)
			//				  - Por qué no está el multiplicador primero (en pág 158 de tesis está tachado)
			double temp = Pico->Ajuste.Ytau / 2.0;
			temp *= erfc(((canal-Pico->Ajuste.Centroide)/Pico->Ajuste.Anchura + (Pico->Ajuste.Anchura/Pico->Ajuste.Tau))*M_SQRT1_2);
			temp *= exp((powi(Pico->Ajuste.Anchura/Pico->Ajuste.Tau, 2)/2.0) +
				    ((canal-Pico->Ajuste.Centroide)/Pico->Ajuste.Tau));
			Acum += temp;
			}
		}
	return Acum;
	}

double cEspectros::DaValorPico(NodoPicos *intervaloPico, NodoPico *Pico, double canal) 
	{
	double Acum = 0.0;
		
	//Si el canal está en el intervalo que se pasa como parámetro ...
	if ((canal >= intervaloPico->limInferior-2) && (canal <= intervaloPico->limSuperior+2)) 
		{
		//Se calcula el valor que se le da al continuo en ese punto
		Acum = (double) continuo[redondea(canal)];
		//Le añadimos al acum los valores de los picos en ese intervalo que afectan al canal, según 
		//la fórmula derivada de la convolución de la gaussiana con la exponencial tau
		if (Pico->Ajuste.ajustado) 
			{
			// pasa el canal a energia
			//El modelo de la forma del pico se supone que se corresponde a la fórmula:
			//1/(2*anchura*sqrt(2pi)) * Ytau * exp(0.5*(anchura/tau)) + (canal-centroide)/tau) * 
			//		erfc((canal-centroide)/anchura+anchura/tau)
			// Según esto, no sé a qué viene: - dividir entre 2 Ytau (suple el 1/2 del primer multiplicador)
			//				  --> multiplicar lo de erfc por sqrt(0.5)
			//				  - Por qué no está el multiplicador primero (en pág 158 de tesis está tachado)
			double temp = Pico->Ajuste.Ytau / 2.0;
			temp *= erfc(((canal-Pico->Ajuste.Centroide)/Pico->Ajuste.Anchura + (Pico->Ajuste.Anchura/Pico->Ajuste.Tau))*M_SQRT1_2);
			temp *= exp((powi(Pico->Ajuste.Anchura/Pico->Ajuste.Tau, 2)/2.0) +
				    ((canal-Pico->Ajuste.Centroide)/Pico->Ajuste.Tau));
			Acum += temp;
			}
		}
	return Acum;
	}

//******************** DA VALOR GAUSSIANA *********************
// Devuelve el valor de una función gaussiana con valor máximo de amplitud en x=centroide, 
//	anchura definida por sigma; en el punto x=canal.	
double cEspectros::DaValorGaussiana(double centroide, double amplitud, double sigma,  double canal) 
	{
	double g = 0.0;
		
//	g =	amplitud*exp( (-pow(canal-centroide+1,2))/(2*sigma*sigma) );//NOTA: Probando a sumar o restar un +1 al centroide
	g =	amplitud*exp( (-pow(canal-centroide,2))/(2*sigma*sigma) );//NOTA: Probando a sumar o restar un +1 al centroide
	//NOTA: Para GA.calculaAjuste viene mejor quitarle el continuo
	return g;
	}

//******************* DA AREA GAUSSIANA ******************	
double cEspectros::DaAreaGaussiana(double amplitud, double sigma)
	{
	return amplitud*sigma*sqrt(6.28);	
	}
/*//******************* DA AREA GAUSSIANA ******************	
double cEspectros::DaAreaGaussiana(double amplitud, double tau)
	{
	return amplitud*tau;	
	}*/
	
//******************** DA VALOR CONVOLUCIÓN *********************
// Devuelve la gaussiana convolucionada con la exponencial para este canal
/* La fórmula de la convolución parece que es:
			  (0.5*(s/t)² + (e-e0)/t)	
	T(E)=At*e						 *0.5*erfc(s/sqrt(2)*t + (e-e0)/sqrt(2)*s)
	Con:
		At=Amplitud de la exponencial
		s=sigma
		t=tau
		e=canal (variable x)
		e0=centroide
		
	NOTA: La amplitud de la gaussiana no aparece por ninguna parte?!
			-->El parámetro de amplitud va a pasar a depender del área y la
				tau de la calibración
*/
double cEspectros::DaValorConvolucion(double centroide, double amplitud, double sigma,  double tau, double canal) 
	{
	double canalMenosCentroide=canal-centroide;
	double sigmaPartidoTau=sigma/tau;
	double temp = amplitud*0.5;	
	temp *= erfc(((canalMenosCentroide)/sigma + (sigmaPartidoTau))*M_SQRT1_2);
	if(temp==0)	return 0;
	double exponencial=exp ((sigmaPartidoTau*sigmaPartidoTau*0.5) + ((canalMenosCentroide)/tau));
	temp *= exponencial;
	return temp;	
	}

//******************** DA INCER VALOR CONVOLUCIÓN *********************
/*	Devuelve la incertidumbre correspondiente a una convolución
	*//*EN CONSTRUCCIÓN
double cEspectros::DaIncerValorConvolucion(double centroide, double amplitud, double sigma,  double tau, double canal) 
	{
	double canalMenosCentroide=canal-centroide;
	double sigmaPartidoTau=sigma/tau;
	double temp = amplitud*0.5;	
	temp *= erfc(((canalMenosCentroide)/sigma + (sigmaPartidoTau))*M_SQRT1_2);
	if(temp==0)	return 0;
	double exponencial=exp ((sigmaPartidoTau*sigmaPartidoTau*0.5) + ((canalMenosCentroide)/tau));
	temp *= exponencial;
	return temp;	
	}*/
//************** DA INCER CONVOLUCIÓN ******************
/*	La incertidumbre de la convolución corresponde a la fórmula:
	sumi (sumj (df/dxi * df/dxj * covar(i,j) ))
	*/
double cEspectros::DaIncerConvolucion(double centroide, double amplitud, double sigma,  double tau, double canal, double covarParam[4][4], double convolucion) 
	{
	double temp=0;
	double funcConvolucion=convolucion;
	double dpc=dpCentroide(centroide, amplitud, sigma, tau, canal, funcConvolucion);
	double dpa=dpAmplitud(amplitud, funcConvolucion);
	double dps=dpSigma(centroide, amplitud, sigma, tau, canal, funcConvolucion);
	double dpt=dpTau(centroide, amplitud, sigma, tau, canal, funcConvolucion);

	temp+=dpc*dpc*covarParam[0][0];
	temp+=dpc*dps*covarParam[0][1];
	temp+=dpc*dpt*covarParam[0][2];
	temp+=dpc*dpa*covarParam[0][3];
			
	temp+=dps*dps*covarParam[1][1];
	temp+=dps*dpc*covarParam[1][0];
	temp+=dps*dpt*covarParam[1][2];
	temp+=dps*dpa*covarParam[1][3];
		
	temp+=dpt*dpt*covarParam[2][2];
	temp+=dpt*dpc*covarParam[2][0];
	temp+=dpt*dps*covarParam[2][1];
	temp+=dpt*dpa*covarParam[2][3];
	
	temp+=dpa*dpa*covarParam[3][3];
	temp+=dpa*dpc*covarParam[3][0];
	temp+=dpa*dps*covarParam[3][1];
	temp+=dpa*dpt*covarParam[3][2];
	return temp;
	}

//****************** DP CENTROIDE *****************
/*	Devuelve la derivada parcial de la convolución de una gaussiana con una
	exponencial con respecto al parámetro centroide
	NOTA: Todas estas derivadas parciales se sacan de la fórmula que está en
	ActualizaVectorPicosAnal() de VisAnal.C
*/	
double cEspectros::dpCentroide(double centroide, double amplitud, double sigma,  double tau, double canal, double convolucion)
	{
	double temp2 = -powi((canal - centroide)/sigma, 2)/2.0;
	temp2 = -exp(temp2)*amplitud/sqrt(M_2PI);
	double derivada=0;
		
	derivada = -convolucion/tau - temp2/sigma;
	return derivada;
	}
		
//****************** DP AMPLITUD *****************
/*	Devuelve la derivada parcial de la convolución de una gaussiana con una
	exponencial con respecto al parámetro amplitud
	*/
double cEspectros::dpAmplitud(double amplitud, double convolucion)
	{
	double derivada=0;
	derivada = convolucion/amplitud;
	return derivada;
	}
	
//****************** DP SIGMA *****************
/*	Devuelve la derivada parcial de la convolución de una gaussiana con una
	exponencial con respecto al parámetro sigma
	*/
double cEspectros::dpSigma(double centroide, double amplitud, double sigma,  double tau, double canal, double convolucion)
	{
	double temp2 = -powi((canal - centroide)/sigma, 2)/2.0;
	temp2 = -exp(temp2)*amplitud/sqrt(M_2PI);
	double derivada=0;
		
	derivada = sigma*(convolucion/(tau*tau)) + 
				temp2*(1/tau - (canal-centroide)/sigma);
	return derivada;
	}

//****************** DP TAU *****************
/*	Devuelve la derivada parcial de la convolución de una gaussiana con una
	exponencial con respecto al parámetro sigma
	*/
double cEspectros::dpTau(double centroide, double amplitud, double sigma,  double tau, double canal, double convolucion)
	{
	double temp2 = -powi((canal - centroide)/sigma, 2)/2.0;
	temp2 = -exp(temp2)*amplitud/sqrt(M_2PI);
	double derivada=0;
		
	derivada = -((canal-centroide+(sigma*sigma)/tau)*convolucion + 
				temp2*sigma)/(tau*tau);
	return derivada;
	}
	
	
	
	
	
//********************** DA VALOR EXPONENCIAL ******************************
/*	Devuelve el valor que toma una función exponencial asintótica en x=centroide,
	con altura de amplitud y anchura de tau, para el valor x=canal
	*/
double cEspectros::DaValorExponencial(double centroide, double amplitud, double tau, double canal)
	{
	if(canal>centroide)	return 0.0;
	else				return (amplitud*exp((canal-centroide+1)/tau));		
	}	

//***************** DA AREA PICO ****************	
/*	Devuelve el área del pico tal y como la asume la tesis de Begoña:
	Ap=Yp*Acp (papel Quintana, Fernandez, 1236)
*/	
double cEspectros::DaAreaPico(int numIntervalo, int numPico) 
	{
	NodoPicos *intervalo = DevuelveNodo(listaPicos, numIntervalo);
	NodoPico  *pico		 = DevuelveNodo(intervalo->picos, numPico);
//	printf("%f*%f\n", pico->Ajuste.Tau, pico->Ajuste.Ytau);
	if (pico->Ajuste.ajustado)	return pico->Ajuste.Tau*pico->Ajuste.Ytau;
	else 						return 0.0;
	}
	
//**************** DA AREA PICO *******************
//Igual que el anterior, pero quita el área del fondo si existiera
double cEspectros::DaAreaPico(int numIntervalo, int numPico, double areaFondo) 
	{
	NodoPicos *intervalo = DevuelveNodo(listaPicos, numIntervalo);
	NodoPico  *pico		 = DevuelveNodo(intervalo->picos, numPico);
	if (pico->Ajuste.ajustado)	return (pico->Ajuste.Tau*pico->Ajuste.Ytau-areaFondo);
	else 						return 0.0;
	}

//******************** DA INCER AREA PICO *************
/* 
	sqrt[(tau*Ysigma)²+(Ytau*sigma)²+2*(Ytau*tau*sigmaYTau)]
*/
double cEspectros::DaIncerAreaPico(int numIntervalo, int numPico) 
	{
	NodoPicos *intervalo = DevuelveNodo(listaPicos, numIntervalo);
	NodoPico *pico = DevuelveNodo(intervalo->picos, numPico);
	if (pico->Ajuste.ajustado) 
		{
		double temp = powi(pico->Ajuste.Tau*pico->Ajuste.SigYtau, 2);
		temp += powi(pico->Ajuste.Ytau*pico->Ajuste.SigTau, 2);
		temp += 2.0 * pico->Ajuste.SigTauYtau *
				pico->Ajuste.Ytau * pico->Ajuste.Tau;
		if (temp >= 0.0) return sqrt(temp);
		}
	return 0.0;
	}
//NOTA: Prueba
/*double cEspectros::DaIncerAreaPico(int numIntervalo, int numPico) 
	{
	NodoPicos *intervalo = DevuelveNodo(listaPicos, numIntervalo);
	NodoPico *pico = DevuelveNodo(intervalo->picos, numPico);
	if (pico->Ajuste.ajustado) 
		{
		double t=pico->Ajuste.Tau;
		double st=pico->Ajuste.SigTau;
		double h=pico->Ajuste.Ytau;
		double sh=pico->Ajuste.SigYtau;
			
		double temp = DaAreaPico(numIntervalo, numPico)*
					  sqrt( (st*st)/(t*t) + (sh*sh)/(h*h) );
		return temp;
		}
	return 0.0;
	}
*/
//************** DA AREA INTERVALO ****************
double cEspectros::DaAreaIntervalo(int numIntervalo) 
	{
	NodoPicos *intervalo = DevuelveNodo(listaPicos, numIntervalo);
	double acum = 0.0;
	for (int i = 1; i <= intervalo->picos.numElementos; i++)
		acum += DaAreaPico(numIntervalo, i);
	return acum;
	}

//******************* DA VECTOR INTERVALO ****************
/*	Devuelve el valor neto sin contar el continuo de cuentas del intervalo
	*/
float *cEspectros::DaVectorIntervalo(int numIntervalo) 
	{
	NodoPicos *intervalo = DevuelveNodo(listaPicos, numIntervalo);
	int anchoIntervalo=intervalo->limSuperior-intervalo->limInferior+1;
	int inf=intervalo->limInferior;
	float *vector=new float[anchoIntervalo];
	if(vector==NULL)
		{
		printf("ERROR: No hay memoria para reservar float[%d]\n", anchoIntervalo);
		return NULL;
		}
	
	for(int i=0;i<anchoIntervalo;i++)
		{
		int j=i+inf;
		vector[i]=espectro[j]-continuo[j];
		//printf("vectorEspectro[%d]=%f\n", i, vector[i]);
		if(vector[i]<0)	vector[i]=0;
		}
	return vector;
	}

//**************** DA ANCHO INTERVALO ******************
int cEspectros::DaAnchoIntervalo(int numIntervalo)
	{
	NodoPicos *intervalo = DevuelveNodo(listaPicos, numIntervalo);
	return (intervalo->limSuperior-intervalo->limInferior+1);//NOTA: Intervalo cerrado: +1!
	}

//***************** DA LIM INFERIOR *********************
int cEspectros::DaLimInferior(int numIntervalo)
	{
	NodoPicos *intervalo = DevuelveNodo(listaPicos, numIntervalo);
	if(intervalo==NULL)	{printf("ERROR: DaLimInferior(): intervalo nulo\n"); return -1;}
	return intervalo->limInferior+1;//NOTA: El +1 es una prueba (creo q es mejor)
	}

int cEspectros::DaLimSuperior(int numIntervalo)
	{
	NodoPicos *intervalo = DevuelveNodo(listaPicos, numIntervalo);
	if(intervalo==NULL)	{printf("ERROR: DaLimSuperior(): intervalo nulo\n"); return -1;}
	return intervalo->limSuperior+1;//NOTA: El +1 es una prueba (creo q es mejor)
	}

//***************** DA INCER AREA INTERVALO ********************	
double cEspectros::DaIncerAreaIntervalo(int numIntervalo) 
	{
	NodoPicos *intervalo = DevuelveNodo(listaPicos, numIntervalo);
	double acum = 0.0;
	/* Cuando fallo el ajuste y se tuvo que dejar la inicializacion, para
		calcular la incertidumbre del area se utiliza la estimacion
		indicada en el Bevington, p 254: s^2 = sigma^2 * Chi^2
		donde la sigma es la incertidumbre del area neta del intervalo
		del espectro */

	//Si la incertidumbre del área está ya calculada la mandamos sin más
	//if(intervalo->sigArea!=0)	return intervalo->sigArea;

	/*if (intervalo->picos.inicio->Ajuste.SigTauYtau == 0.0) 
		{
		printf("SigTauYTau es cero para el intervalo %d\n", numIntervalo);
		acum = fabs(intervalo->ChiSq*intervalo->sigArea);
		return acum;
		}
	
	for (int i = 1; i <= intervalo->picos.numElementos; i++)
		{
		acum += powi(DaIncerAreaPico(numIntervalo, i), 2);
		}
	return sqrt(acum);
	*///NOTA: Antes estaba esto por extrañas razones
	for (int i = 1; i <= intervalo->picos.numElementos; i++)
		{
		acum += pow(DaIncerAreaPico(numIntervalo, i),2);
		}
	return sqrt(acum);
	}

//********************* F CEROS *****************************
// erfc() error de función complementaria, es de la librería math.h
double fCeros(double x, double relacion) 
	{
	double temp = erfc(x)*relacion*1.253314137 - exp(-x*x);
	return temp;
	}

//***************** SOLUCION BISEC CENTROIDE *******************
double SolucionBisecCentroide(double relacion) 
	{
	//NOTA: xmas no debería ser el 1.0 y xmenos el -1.0???
	double xmas = -1.0;
	double xmenos = 1.0;
	double ymas = fCeros(xmas,relacion);
	double ymenos = fCeros(xmenos, relacion);
	double x0;
	//Hacemos 250 iteraciones. En cada una, x/ymas o x/ymenos van acercándose(?) hasta llegar
	//al punto medio donde se supone que está el centroide (generalmente tras las 250 iteraciones
	//llega un punto en el que xmas=xmenos=x0, pero no siempre. Algunas otras veces uno de los dos 
	//no llega a moverse nunca)
	for (int i = 1; i <= 250; i++) 
		{
		x0 = xmenos - ymenos*(xmas-xmenos)/(ymas-ymenos);
		double y0 = fCeros(x0, relacion);
		if (y0 > 0.0) 
			{
      			xmas = x0; ymas = y0;
			}
		else 	
			{
			xmenos = x0; ymenos = y0;
			}
		//NOTA: Si tenemos problemas de rendimiento se puede comprobar si xmas=xmenos para salir del bucle
//		printf("xM=%f	xm=%f\n", xmas, xmenos);
		}
//	printf("\n");
	return x0;
	}

//************** CALCULA LD *******************
/* Función que calcula los límites de detección para ciertas emisiones 
	que se le pasan como parámetros. Así podremos calcular los límites 
	para emisiones que no han aparecido durante la calibración pero que
	pueden resultar interesantes para el usuario.
	*/
void cEspectros::calculaLD(nodoCentroide *lista)
	{
	printf("CALCULA LD LISTA+++++++++++++++++++++++++++++++++++++++++++\n");
	nodoCentroide *temp=lista; //Puntero para recorrer la lista
	int inicio, final;		//Inicio y final de los intervalos de cálculo
	float cent;
	//Recorremos la lista de emisiones y vamos calculando sus límites
	while(temp!=NULL)
		{
		float lc, ld;
		Datos.calculaLC(temp->centroide, &lc, &ld);	
		//modificado por felipe para que de el canal por consistencia con el calculo para los picos del espectro
		/*cent=DaCanal(temp->centroide);
		printf("centroide %f\n", cent);
		//inicio=(int)(cent -0.43*DaFWHG(cent));
		//final=(int) (cent + 0.4*DaFWHG(cent));
		//modificado por felipe para que de lo mismo que el ajuste del espectro
		inicio=(int)(cent - 0.43*DaFWHGCh(cent));
		final=(int) (cent + 0.43*DaFWHGCh(cent));
		//inicio=(int)(cent - 2);
		//final=(int) (cent + 2);
		printf("inicio %d\n", inicio);	
		printf("final %d\n", final);	
		float Nc=0;//Número de cuentas del intervalo
		float Sc=0;//Incertidumbre de las cuentas del intervalo
			
		for(int j=inicio;j<=final;j++)
			{
			Nc+=continuo[j];
			Sc+=sigmaContinuo[j]*sigmaContinuo[j];//En la fórmula de debertin se usa sigma cuadrado
			}
			
		float ld=5.4+3.3*sqrt(Nc+Sc);
		*/
		temp->ld=ld;
		printf("LD de centroide %f vale %f\n", cent, ld);
			
		temp=temp->siguiente;	
		}
	}
//************** CALCULA LD *******************
/* Función que calcula los límites de detección para un fichero de energias
	*/
void cEspectros::calculaLD(char *fichero, float* ld, int* numlineas, int *ene1, int *ene2){	
	//float ld[100];
	ifstream file(fichero);
	if (! file.is_open()) 	
		{
		//sprintf(mensajeErrorActividad, "Error opening file %s", fichero);
		//Aviso(main_w, mensajeErrorActividad);
		//cout << "Error opening file";// exit(1); 
		}
	char buffer[256];
	int na,np;
	nodoCentroide *lista=NULL;
  	//Recorremos el fichero y para cada elemento hacemos el cálculo del LD
	int k= 0;
	int inicio, final;
	
	while (! file.eof() ){
	    	file.getline (buffer,100);
    		cout << buffer << endl;
		cout<<"Hemos cogido una línea, vamos a recoger los dos números del formato"<<endl;
		na=np=0;
		char *endptr=new char[1024];
			
		na=strtol(buffer, &endptr, 10);

		cout << "na: "<<na << endl;			
		int lim=*endptr;
		cout << "lim:" <<lim<< endl;
		for(int i=0;i<lim;i++)	
			{
			if(buffer[i]=='\t')	i+=5;
			buffer[i]=' ';
			}

		np=strtol(buffer, NULL, 10);
		cout << "np: "<<np << endl;	


		inicio=(int) DaCanal(na);	
		final=(int) DaCanal(np);	
		cout << "inicio: "<<inicio << endl;	
		cout << "final: "<<final << endl;	

		float Nc=0;//Número de cuentas del intervalo
		float Sc=0;//Incertidumbre de las cuentas del intervalo
	
		for(int j=inicio;j<=final;j++)
			{
			Nc+=continuo[j];
			Sc+=sigmaContinuo[j]*sigmaContinuo[j];//En la fórmula de debertin se usa sigma cuadrado
			}
		
		ld[k]=5.4+3.3*sqrt(Nc+Sc);
		printf("LD de intervalo %d, %d vale %f\n", inicio, final, ld[k]);
		ene1[k]=na;
		ene2[k]=np;
		k++;
		}	
	*numlineas = k-1;
	//return ld;		
	
}

//************** CALCULA LD *******************
/* Función que calcula el límite de detección de una ciertas emisión 
	que se le pasa como parámetro. Permite calcular emisiones especiales
	que no han aparecido en el análisis de picos pero que puedan resultar
	interesantes para el usuario.
	*/
float cEspectros::calculaLD(float cent)
	{
	//modificado por felipe. Cambia energia a canal para ser coherente con CalculaLD de los picos del espectro
	cent=DaCanal(cent);
	//int inicio=(int)(cent -0.43*DaFWHG(cent));
	//int final=(int) (cent + 0.4*DaFWHG(cent));
	//modificado por felipe para que de lo mismo que el ajuste del espectro
	int inicio=(int)(cent - 0.43*DaFWHGCh(cent));
	int final=(int) (cent + 0.4*DaFWHGCh(cent));
	float Nc=0;//Número de cuentas del intervalo
	float Sc=0;//Incertidumbre de las cuentas del intervalo
	
	for(int j=inicio;j<=final;j++)
		{
		Nc+=continuo[j];
		Sc+=sigmaContinuo[j]*sigmaContinuo[j];//En la fórmula de debertin se usa sigma cuadrado
		}
		
	float ld=5.4+3.3*sqrt(Nc+Sc);
	printf("LD de centroide %f vale %f\n", cent, ld);
	return ld;			
	}
//***************** CALCULA LD ******************
/*	Función polimórfica a la anterior que usa como lista de emisiones
	las calculadas durante la calibración, que se encuentran almacenadas
	en listaPicos
	*/
void cEspectros::calculaLD()
	{
	//Recorremos lista picos y para cada uno de sus picos calculamos el ld
	printf("CALCULA LD () +++++++++++++++++++++++++++++\n");
	printf("LIMITES DE DETECCIÓN: (tenemos %d elementos)\n", listaPicos.numElementos);
	for (int i = 1; i <= listaPicos.numElementos; i++)
		{
		//printf("LIMITES DE DETECCIÓN: dentro del bucle\n");
		NodoPicos *intervalo = DevuelveNodo(listaPicos, i);
		//NOTA: Parece que de momento cada intervalo sólo tiene un pico, si no
		//		habría que realizar aquí otro bucle.
		if(intervalo==NULL)			return;
		NodoPico *PicoCalibracion=DevuelveNodo(intervalo->picos, 1);
		if(PicoCalibracion==NULL)	return;
		
		if(PicoCalibracion->Ajuste.identificado)
			{
			//NOTA: El inicio y el final de forma estricta deberían calcularse como se 
			//hace en guardaAnalisis() de Analgam.C
			//Por el momento no se hace así porque para cálculos posteriores en rege nos
			//vienen mejor estos límites.
			float anchura=PicoCalibracion->Ajuste.Anchura;
			float centroide=PicoCalibracion->Ajuste.Centroide;
			int inicio =(int)(centroide-anchura);
			int final  =(int)(centroide+anchura);
			printf("Ajuste.inicio %d\n", inicio);	
			printf("Ajuste.final %d\n", final);	
			float Nc=0;//Número de cuentas del intervalo
			float Sc=0;//Incertidumbre de las cuentas del intervalo
			for(int j=inicio;j<=final;j++)
				{
				Nc+=continuo[j];
				Sc+=sigmaContinuo[j]*sigmaContinuo[j];//En la fórmula de debertin se usa sigma cuadrado
				}
			
			//Según la fórmula adaptada de Debertin[88] (pág 53)
			PicoCalibracion->Ajuste.ld=5.4+3.3*sqrt(Nc+Sc);
			PicoCalibracion->Ajuste.ldCalculado=true;
			printf("Ajuste-LD de centroide %f vale %f\n", centroide, PicoCalibracion->Ajuste.ld);
			}//del if(identificado)
		}
	}

//***************** CALCULA AMD  ******************
/*	para cualquier energia, aunque no esté en la lista de picos
	*/
void cEspectros::calculaAMD(int ce, float energia, float ld, float *amd){
	//eficiencia	
	//double energia=LaraBD.GetEnergy(ce);
	double eficiencia=daEficiencia(energia);
	//tiempo
	int emisor=LaraBD.GetEmissor(ce);
	double tmedio =LaraBD.GetDecayTime(emisor);
	double tvivo=(double)tiempo;
	double t;
	double ct=0.0;
	if (tmedio < 350*tvivo){//En este caso hacemos una corrección del tiempo
		double landa=log(2.0)/tmedio;
		ct=(1-exp(-landa*tvivo))/landa;
	}
	if(ct!=0)	
		t=ct;
	else		
		t=tvivo;
	//probabilidad de emision
	double p=LaraBD.GetProbability(ce)/100.0;
	//calculo de amd
	double c = 1.0;	//!!!!!tomo como uno el coeficiente de corrección
	*amd = ld/(eficiencia*t*p*c);
}	
//*********** DA LC ***************
/*	Devuelve el límite de decisión que hay en una determinada energía,
	según los cálculos realizados con anterioridad.
	*/
/*double cEspectros::daLC(float energia)
	{
	if(cuentasFondo!=NULL)
	else		
		{
		printf("ERROR: There are no background computed\n");
		return -1;
		}
	}*/
	
//***************** CALCULA LC ******************
/*	Calcula los límites de decisión para los picos que estén 
	identificados a partir de la información de la base de datos
	de fondo
	Tiene en cuenta el posible solapamiento de picos
	El solapamiento sigue el siguiente criterio: si la parte que solapa está
	dentro de [centroide+-anchura] del pico solapado se usa el 100% del pico
	solapador. Si se encuentra en [centroide+-anchura*2] usamos el 58% (si al
	0.6 le corresponde el 100%, al 0.35 le coresponde el 58). La misma regla de
	tres se usa para el último intervalo en [c+-3*a].
	NOTA: Ahora mismo calcula el límite de decisión para picos, pero lo estamos
		retocando para que use emisiones. Pero en ese caso, cómo tómo el solapamiento?
		Yo de momento lo estoy haciendo restando el solapamiento de picos, que creo
		que es lo lógico
		
	NOT: NUEVO: Haciéndolo para que lo calcule tal cual, si hay fondo con fondo y
		si no pues sin él, así separo LC de cálculo de fondo, que se hace en calculaFondo
		y nos lo almacena en cuentasFondo[] y sigmaCuentasFondo[]
*/
//void cEspectros::calculaLC(int numEnergia, double *energias, double *medias, double *incertidumbres)
void cEspectros::calculaLC()
	{
	//Recorremos lista picos y para cada uno de sus picos calculamos el ld
	printf("LIMITES DE DECISIÓN: (tenemos %d elementos)\n", listaPicos.numElementos);
//	for(int i=0;i<numEnergia;i++)	printf("%f ", incertidumbres[i]);
	float Nc, Sc, Nb, Sb;
	float Np, Sp, Na, Sa;
	NodoPico *NodoAnterior, *NodoPosterior;
	NodoPicos *intervaloAnt, *intervaloPost;
	float centroideAnt=0, centroidePost=9000, anchuraAnt=0, anchuraPost=0, inicioPost=0, finalAnt=0;
	bool solapaAnt=false, solapaPost=false;
	float anchura, centroide, inicio, final;
		
	for (int i = 1; i <= listaPicos.numElementos; i++)
		{
		NodoPicos *intervalo = DevuelveNodo(listaPicos, i);
		if(intervalo==NULL)			return;
			
		for(int k=1;k<=intervalo->picos.numElementos;k++)
			{
			NodoPico *PicoAnalisis=DevuelveNodo(intervalo->picos, k);
			if(PicoAnalisis==NULL)	return;

			if(PicoAnalisis->Ajuste.analizado)
				{
				anchura=PicoAnalisis->Ajuste.Anchura;
				centroide=PicoAnalisis->Ajuste.Centroide;
				inicio =centroide-anchura;
				final  =centroide+anchura;

				//Comprobación de solapamiento
				//Tomamos pico anterior y posterior
				centroideAnt=0;
				centroidePost=9000;
				anchuraAnt=anchuraPost=0; 
				inicioPost=finalAnt=0;
				solapaAnt=solapaPost=false;
				NodoAnterior=NodoPosterior=NULL;
					
				if(k>1) NodoAnterior=DevuelveNodo(intervalo->picos, k-1);
				else	
					{
					if(i==1)	{intervaloAnt=NULL; NodoAnterior=NULL;}
					else
						{
						intervaloAnt = DevuelveNodo(listaPicos, i-1);
						//printf("Pico anterior es %d,%d\n", i-1, intervaloAnt->picos.numElementos);
						NodoAnterior = DevuelveNodo(intervaloAnt->picos, intervaloAnt->picos.numElementos);
						}
					}
				if(k<intervalo->picos.numElementos)	NodoPosterior=DevuelveNodo(intervalo->picos, k+1);
				else
					{
					if(i==listaPicos.numElementos)	 {intervaloPost=NULL; NodoPosterior=NULL;}
					else
						{
						intervaloPost = DevuelveNodo(listaPicos, i+1);
						//printf("Pico siguiente es %d,%d\n", i+1, 0);
						NodoPosterior = DevuelveNodo(intervaloPost->picos, 0);
						}
					}
				
				if(NodoAnterior!=NULL && NodoAnterior->Ajuste.ajustado)
					{//Si el que consideramos anterior no está ajsutado, no entramos aquí
					//NOTA: Puede ser un poco destroyer, porque qué pasa si el anterior del
					//anterior sí que está ajustado y se está solapando?->sería muy jevi, porque
					//sólo estamos considerando ahora mismo al inmediatamente anterior/posterior
					anchuraAnt=NodoAnterior->Ajuste.Anchura;
					centroideAnt=NodoAnterior->Ajuste.Centroide;
					}
				
				if(NodoPosterior!=NULL && NodoPosterior->Ajuste.ajustado)
					{
					anchuraPost=NodoPosterior->Ajuste.Anchura;
					centroidePost=NodoPosterior->Ajuste.Centroide;
					}

				inicioPost =centroidePost-anchuraPost;
				finalAnt  =centroideAnt+anchuraAnt;
				if(inicio<finalAnt)		{solapaAnt=true;	printf("Se solapa con el pico anterior: %f<%f\n",inicio, finalAnt);}
				if(final>inicioPost)	{solapaPost=true; 	printf("Se solapa con el pico posterior: %f>%f\n", final, inicioPost);}
				//-----------------------
					
					
				Nc=0;//Número de cuentas del intervalo
				Sc=0;//Incertidumbre de las cuentas del intervalo
				for(int j=(int)inicio;j<=(int)final;j++)
					{
					Nc+=continuo[j];
					Sc+=sigmaContinuo[j]*sigmaContinuo[j];//En la fórmula de debertin se usa sigma cuadrado
					}
		
				Nb=0;//Número de cuentas del fondo para ese intervalo
				Sb=0;//Incertidumbre de las cuentas para el fondo
				//Miramos si hay algún fondo en las inmediaciones de este pico
				//1)Calculamos sumándole el fondo de continuo donde haya fondo de continuo
				//	Problema: la incertidumbre del fondo en ese punto no está calculada,
				//				se iguala a la incertidumbre del continuo
				/*
				if(cuentasFondo!=NULL)
					{
					for(int j=(int)inicio;j<=(int)final;j++)
						{
						Nb+=cuentasFondo[j];
						Sb+=sigmaCuentasFondo[j]*sigmaCuentasFondo[j];//OJO: Parece que esto introduce mucho error! (40K, 210Pb), mirar para 214Bi, Pb
						}
					}
				*/
				//Para el fondo tomamos el mismo criterio que para un solapamiento
				//2)Mismo problema que para 1, aunque un poco suavizado
				/*
				Nb=0;//Número de cuentas del fondo para ese intervalo
				Sb=0;//Incertidumbre de las cuentas para el fondo
				//printf("numFondos vale %d\n", numFondos);
				if(cuentasFondo!=NULL)
					{
					for(int j=(int)(inicio-anchura);j<=(int)(final+anchura);j++)
						{
						if(cuentasFondo[j]>0)
							{
					//		printf("Cuentas fondo %d vale %d\n", cuentasFondo[j]);
							float dif=10000000;
							int emision=0;
							for(int k=0;k<numFondos;k++)	
								{
								float canalfondo=Datos.DaCanal(energiasFondo[k]);
						//		printf("canal fondo es %f y canal en cuestion=%d\n", canalfondo, j);
								if( (canalfondo-j)<dif) 	
									{
									dif=fabs(canalfondo-j);									
									emision=k;
									}
								}
						//	printf("Nos quedamos con la emisión %d de energía %f\n", emision, energiasFondo[emision]);
							//emision es la más cercana, a la que corresponde cuentas Fondo[j]
							if(dif>3*DaSigmaCh(energiasFondo[emision]))	
								{
								Nb+=cuentasFondo[j]*0.08;
								Sb+=sigmaCuentasFondo[j]*sigmaCuentasFondo[j]*0.08;
								}
							else 
								{
								if(dif>2*DaSigmaCh(energiasFondo[emision]))	
									{
									Nb+=cuentasFondo[j]*0.58;
									Sb+=sigmaCuentasFondo[j]*sigmaCuentasFondo[j]*0.58;
									}
								else
									{
									Nb+=cuentasFondo[j];
									Sb+=sigmaCuentasFondo[j]*sigmaCuentasFondo[j];
									}
								}
							}
						}
					}
				*/
				//3)Tomamos todo el área del pico si está dentro del intervalo su
				//	centroide+-s. 
				// Si está en +-2s o +-3s se hace proporcionalmente, según las
				//predicciones de áreas recogidas en esas zonas en [Sachs, 1.3.4, fig 1.12]
				for(int k=0;k<numFondos;k++)
					{
					float canalfondo=Datos.DaCanal(energiasFondo[k]);
					float distancia=fabs(canalfondo-centroide);
					
					if(distancia < anchura)				
						{
						Nb+=mediasFondo[k];	
						Sb+=incertidumbresFondo[k]*incertidumbresFondo[k];
						printf("Está entera, distancia es %f, sumamos %f\n", distancia, mediasFondo[k]);
						}
					else 
						{
						if(distancia < 2*anchura)		
							{
							Nb+=mediasFondo[k]*0.32;	
							Sb+=incertidumbresFondo[k]*incertidumbresFondo[k]*0.32;
							printf("Está entera, distancia es %f, sumamos %f\n", distancia, mediasFondo[k]);
							}
						else if (distancia < 3*anchura)	
							{
							Nb+=mediasFondo[k]*0.04;	
							Sb+=incertidumbresFondo[k]*incertidumbresFondo[k]*0.04;
							}
						}
					}
				
				//Por último, si hay solapamiento, también añadimos las cuentas del
				//pico con el que solapa 
				Na=Sa=0;
				Np=Sp=0;
				
				if(solapaAnt)
					{
					for(int j=(int)inicio;j<=(int)final;j++)
						{
						//NOTA: Ponderamos el número de cuentas en función de la distribución
						//normal, bien en intervalos o mediante la integral de la gaussiana.
						//(se multiplicará por ese valor que irá entre 0 y 1)
						double conv=DaValorConvolucion(NodoAnterior->Ajuste.Centroide, NodoAnterior->Ajuste.Ytau, NodoAnterior->Ajuste.Anchura, NodoAnterior->Ajuste.Tau, j);
						double sconv=DaIncerConvolucion(NodoAnterior->Ajuste.Centroide, NodoAnterior->Ajuste.Ytau, NodoAnterior->Ajuste.Anchura, NodoAnterior->Ajuste.Tau, j, NodoAnterior->Ajuste.covar, conv);
						//NOTA: OJO! -> Me da a mí que me da un valor muy alto de convolución en el error al menos!
						//
						//printf("Suma de Sa vale %f\n", Sa);
						//printf("Sumamos ant[j]=%f\n", conv);
						//Ponderación por distancia:
						if(j<centroide-2*DaSigmaCh(j) || j>centroide+2*DaSigmaCh(j))	{conv=0.08*conv;	sconv*=0.08;}
						else
							{
							if(j<centroide-DaSigmaCh(j) || j>centroide+DaSigmaCh(j))	{conv=0.58*conv;	sconv*=0.58;}
							//else	tomamos el valor entero, no modificamos conv=1*conv, sconv=1*sconv
							}
						Na+=conv;
						Sa+=sconv;
						}
					}
				
				if(solapaPost)
					{
					//printf("Sumamos el pico posterior allá donde solape\n");
					for(int j=(int)inicio;j<=(int)final;j++)
						{
						double conv=DaValorConvolucion(NodoPosterior->Ajuste.Centroide, NodoPosterior->Ajuste.Ytau, NodoPosterior->Ajuste.Anchura, NodoPosterior->Ajuste.Tau, j);
						double sconv=DaIncerConvolucion(NodoPosterior->Ajuste.Centroide, NodoPosterior->Ajuste.Ytau, NodoPosterior->Ajuste.Anchura, NodoPosterior->Ajuste.Tau, j, NodoPosterior->Ajuste.covar, conv);
						
						//printf("Sumamos post[%d]=%f\n", j, conv);
						//Ponderación por distancia:
						if(j<centroide-2*DaSigmaCh(j) || j>centroide+2*DaSigmaCh(j))	{printf("Muy lejos, multiplicamos por 0.08\n"); conv=0.08*conv;	sconv*=0.08;}
						else if(j<centroide-DaSigmaCh(j) || j>centroide+DaSigmaCh(j))	{printf("Lejos, multiplicamos por 0.58\n"); conv=0.58*conv;	sconv*=0.58;}
						Np+=conv;
						Sp+=sconv;
						}
					}
				
				printf("Pico %f keV\n", DaEnergia(centroide));
				printf("NC=%f(%f)\n", Nc, Sc);
				printf("NB=%f(%f)\n", Nb, Sb);
				printf("NA=%f(%f)\n", Na, Sa);
				printf("NP=%f(%f)\n", Np, Sp);
				PicoAnalisis->Ajuste.lc=1.65*sqrt(Nc+Sc+Nb+Sb+Na+Sa+Np+Sp);
					
				//De paso calculamos los límites de detección, que están relacionados
				PicoAnalisis->Ajuste.ld=5.4+2*PicoAnalisis->Ajuste.lc;//NOTA: Antes se tomaba 2.71
				PicoAnalisis->Ajuste.ldCalculado=true;
				
				}//del if(identificado)
			}//for(picos del intervalo)
		}
	}



//***************** FELIPE-CALCULA LC ******************
/*	Calcula los límites de decisión para centroides genéricos

	No tiene en cuenta el posible solapamiento de picos
*/
//void cEspectros::calculaLC(int numEnergia, double *energias, double *medias, double *incertidumbres)
void cEspectros::calculaLC(float cent_energia, float* lc, float* ld)
	{
	//Recorremos lista picos y para cada uno de sus picos calculamos el ld
	printf("LIMITES DE DECISIÓN: (tenemos %d elementos)\n", listaPicos.numElementos);
//	for(int i=0;i<numEnergia;i++)	printf("%f ", incertidumbres[i]);
	float Nc, Sc, Nb, Sb;
	float Np, Sp, Na, Sa;
	NodoPico *NodoAnterior, *NodoPosterior;
	NodoPicos *intervaloAnt, *intervaloPost;
	float centroideAnt=0, centroidePost=9000, anchuraAnt=0, anchuraPost=0, inicioPost=0, finalAnt=0;
	bool solapaAnt=false, solapaPost=false;
	float anchura, centroide, inicio, final;
		

				//anchura=PicoAnalisis->Ajuste.Anchura;
				centroide=DaCanal(cent_energia);
					
				anchura=1.5*DaSigmaCh(centroide);				
				inicio =centroide-anchura;
				final  =centroide+anchura;
					
				Nc=0;//Número de cuentas del intervalo
				Sc=0;//Incertidumbre de las cuentas del intervalo
				for(int j=(int)inicio;j<=(int)final;j++)
					{
					Nc+=continuo[j];
					Sc+=sigmaContinuo[j]*sigmaContinuo[j];//En la fórmula de debertin se usa sigma cuadrado
					}
		
				Nb=0;//Número de cuentas del fondo para ese intervalo
				Sb=0;//Incertidumbre de las cuentas para el fondo
				//3)Tomamos todo el área del pico si está dentro del intervalo su
				//	centroide+-s. 
				// Si está en +-2s o +-3s se hace proporcionalmente, según las
				//predicciones de áreas recogidas en esas zonas en [Sachs, 1.3.4, fig 1.12]
				for(int k=0;k<numFondos;k++)
					{
					float canalfondo=Datos.DaCanal(energiasFondo[k]);
					float distancia=fabs(canalfondo-centroide);
					
					if(distancia < anchura)				
						{
						Nb+=mediasFondo[k];	
						Sb+=incertidumbresFondo[k]*incertidumbresFondo[k];
						printf("Está entera, distancia es %f, sumamos %f\n", distancia, mediasFondo[k]);
						}
					else 
						{
						if(distancia < 2*anchura)		
							{
							Nb+=mediasFondo[k]*0.32;	
							Sb+=incertidumbresFondo[k]*incertidumbresFondo[k]*0.32;
							printf("Está entera, distancia es %f, sumamos %f\n", distancia, mediasFondo[k]);
							}
						else if (distancia < 3*anchura)	
							{
							Nb+=mediasFondo[k]*0.04;	
							Sb+=incertidumbresFondo[k]*incertidumbresFondo[k]*0.04;
							}
						}
					}
				
				printf("Pico %f keV\n", DaEnergia(centroide));
				printf("NC=%f(%f)\n", Nc, Sc);
				printf("NB=%f(%f)\n", Nb, Sb);
				*lc=1.65*sqrt(Nc+Sc+Nb+Sb);
					
				//De paso calculamos los límites de detección, que están relacionados
				*ld=5.4+2*(*lc);//NOTA: Antes se tomaba 2.71
				
	}


//***************** CALCULA LC *****************
/*	Versión de cálculo para el límite de decisión tomando 
	el continuo de este espectro como fondo.
	Sólo calcula los límites de decisión de los picos del
	intervalo indicado
*/
void cEspectros::calculaLC(int numIntervalo)
	{
	//printf("calculaLC %d\n", numIntervalo);
	NodoPicos *intervalo = DevuelveNodo(listaPicos, numIntervalo);
	for(int i=1;i<=intervalo->picos.numElementos;i++)
		{
		NodoPico *PicoCalibracion=DevuelveNodo(intervalo->picos, i);
		float anchura=PicoCalibracion->Ajuste.Anchura;
		float centroide=PicoCalibracion->Ajuste.Centroide;
		int inicio =(int)(centroide-anchura);
		int final  =(int)(centroide+anchura);
		//Comprobaciones por si el ajustes salió raro para este pico
		if(inicio<1)	inicio=1;
		if(final<1)		final=1;
		if(inicio>numCanales)	inicio=numCanales;
		if(final>numCanales)	final=numCanales;
		//
		float Nc=0;//Número de cuentas del intervalo
		float Sc=0;//Incertidumbre de las cuentas del intervalo
		for(int j=inicio;j<=final;j++)
			{
			Nc+=continuo[j];
			Sc+=sigmaContinuo[j]*sigmaContinuo[j];//En la fórmula de debertin se usa sigma cuadrado
			}
		//Según la fórmula adaptada de Debertin[88] (pág 53 y sigs)
		PicoCalibracion->Ajuste.lc=1.65*sqrt(Nc+Sc);
				
		//De paso calculamos los límites de detección, que están relacionados
		PicoCalibracion->Ajuste.ld=5.4+2*PicoCalibracion->Ajuste.lc;
		PicoCalibracion->Ajuste.ldCalculado=true;
		//NOTA: Uso del fondo para el ld???
		}
	}
	
//************************ DA EFICIENCIA ********************
/*	Devuelve la eficiencia para una cierta energía, según los parámetros que
	tenga cargados (extraidos de algún fichero .par).
	Si no tiene parámetros cargados, devuelve -1
	*/
double cEspectros::daEficiencia(double energia)
	{
	double yefi=0.0;
	if(nodosEficiencia==NULL)	
		{
		printf("nodosEficiencia es null!!! \n");
		return 1;
		}
	double *afunc=new double[dimensionMatEfi];
	for(int i=0;i<dimensionMatEfi;i++)	afunc[i]=0.0;
	
	bohr(log(energia), afunc, dimensionMatEfi, nodosEficiencia, dimensionMatEfi+1);
	for(int i=0;i<dimensionMatEfi;i++)
		yefi+=parametrosEficiencia[i]*afunc[i];
	yefi=exp(yefi);
	delete [] afunc;
	return yefi;
	}
	
//************************ DA SIGMA EFICIENCIA ********************
/*	Devuelve la incertidumbre de la eficiencia para una cierta energía, 
	según los parámetros que tenga cargados (extraidos de algún fichero .par).
	Si no tiene parámetros cargados, devuelve -1
	*/
double cEspectros::daSigmaEficiencia(double energia)
	{
	double sigmayefi=0.0;
	double yefi=0.0;
	if(nodosEficiencia==NULL)	return 0;
	double *afunc=new double[dimensionMatEfi];
	for(int i=0;i<dimensionMatEfi;i++)	afunc[i]=0.0;
		
	bohr(log(energia), afunc, dimensionMatEfi, nodosEficiencia, dimensionMatEfi+1);
	for(int i=0;i<dimensionMatEfi;i++)
		{
		yefi+=parametrosEficiencia[i]*afunc[i];
		for(int j=0;j<dimensionMatEfi;j++)	sigmayefi+=afunc[i]*afunc[j]*matrizEficiencia[i][j];
		}
	yefi=exp(yefi);
	sigmayefi=sqrt(sigmayefi)*yefi;				
		
	delete [] afunc;
	return sigmayefi;
	}

//****************** CALCULA FONDO *******************
/*
	Calcula el número de cuentas de fondo (sumado al continuo) a partir de
	los valores de energías y cuentas medias de las emisiones del fondo.
	El continuo+fondo se almacena en cuentasFondo/sigmaCuentasFondo
	*/
void cEspectros::calculaFondo(double *medias, double *incertidumbres, double *energias,int numEnergias)
	{
	if(cuentasFondo!=NULL)		delete [] cuentasFondo;
	if(sigmaCuentasFondo!=NULL)	delete [] sigmaCuentasFondo;
	cuentasFondo=new float[numCanales];
	sigmaCuentasFondo=new float[numCanales];
		
	if(cuentasFondo==NULL)
		{
		printf("ERROR: No hay memoria para reservar float[%d]\n", numCanales);
		return;
		}
	if(sigmaCuentasFondo==NULL)
		{
		printf("ERROR: No hay memoria para reservar float[%d]\n", numCanales);
		return;
		}
		
	printf("DATOS.CALCULA FONDO\n");
	
	//Guardamos la información de los valores de fondo
	numFondos=numEnergias;
	if(mediasFondo!=NULL)			delete [] mediasFondo;
	if(incertidumbresFondo!=NULL)	delete [] incertidumbresFondo;
	if(energiasFondo!=NULL)			delete [] energiasFondo;
	mediasFondo=new double[numFondos];
	incertidumbresFondo=new double[numFondos];
	energiasFondo=new double[numFondos];
	for(int i=0;i<numFondos;i++)
		{
		mediasFondo[i]=medias[i];
		incertidumbresFondo[i]=incertidumbres[i];
		energiasFondo[i]=energias[i];
		}
	//		
	
	for(int j=0;j<numCanales;j++)
		{
		cuentasFondo[j]=0;
		sigmaCuentasFondo[j]=0;
		for(int i=0;i<numEnergias;i++)
			{
			float cent=DaCanal(energias[i]);
			double tau=DaTauCh(cent);
			double y=medias[i]/tau;//h=a/t
				//NOTA: El área total de todas las cuentas de tau puede variar debido
				//a que la tau aquí es distinta a la de los espectros de fondo, etc.
			if(medias[i]!=0)	
				if(cent>(j-200) && cent<(j+200) ) //Condición para hacer menos cuentas (más allá de 200 canales de diferencia ningún pico afecta
					{
					if(DaEnergia(j)>506 && DaEnergia(j)<515)
						cuentasFondo[j]+=DaValorConvolucion(cent, y/3, DaFWHGCh(cent), 3*tau, j);
					else	
						cuentasFondo[j]+=DaValorConvolucion(cent, y, DaFWHGCh(cent), tau, j);
					}
			}
		}
	
	//NOTA: Esto pone la incertidumbre de fondo del fichero a todos los canales
	//		en el rango -> no es muy apropiado, pero no sé cómo calcular si no
	//		la incertidumbre de la convolución (cómo calcular la incertidumbre de erfc)
	int cont=0;
	for(int i=0;i<numCanales;i++)
		{
		if( (cont+1)<numEnergias && 
			(fabs(DaEnergia(i)-energias[cont]) > fabs(DaEnergia(i)-energias[cont+1])) )
			{
			cont++;//Estamos más cerca de la siguiente
			}
		if(cuentasFondo[i]!=0)
			{
			sigmaCuentasFondo[i]=incertidumbres[cont];
			}
		else	sigmaCuentasFondo[i]=0;
//		printf("fondo[%d]=%f\tsigma[%d]=%f\n", i, cuentasFondo[i], i, sigmaCuentasFondo[i]);
		}
	return;
	}
	
//*************** CALCULA EFICIENCIA ********************
/* 	Calcula la eficiencia tal y como está calculada en calcefa.f, 
	pero para todas las energías de los picos detectados en la calibración
*/
void cEspectros::calculaEficiencia(int numpar, double *pars, double *nodos, double **matriz)
	{
	double yefi, sigmayefi;
	//Recorremos lista picos y para cada uno de sus picos calculamos la eficiencia
	printf("CALCULA EFICIENCIA: \n");
	
	/*double *afunc=new double[numpar];
	if(afunc==NULL)
		{
		printf("ERROR: No hay memoria para reservar float[%d]\n", numpar);
		return;
		}*/
		
	//Guardamos la nueva matriz de eficiencia
	/*if(matrizEficiencia!=NULL)	
		{
		for(int i=0;i<dimensionMatEfi;i++)
			delete [] matrizEficiencia[i];
		delete [] matrizEficiencia;
		}*/
	matrizEficiencia=new double*[numpar];
	dimensionMatEfi=numpar;

	for(int i=0;i<numpar;i++)
		{
		matrizEficiencia[i]=new double[numpar];
		for(int j=0;j<numpar;j++)	matrizEficiencia[i][j]=matriz[i][j];
		}

	//Borramos los vectores con los nodos y parámetros de eficiencia.
	if(nodosEficiencia!=NULL)		{delete [] nodosEficiencia;nodosEficiencia=NULL;}
	if(parametrosEficiencia!=NULL)	{delete [] parametrosEficiencia;parametrosEficiencia=NULL;}
		
	nodosEficiencia=new double[numpar+1];
	for(int i=0;i<numpar+1;i++)	nodosEficiencia[i]=nodos[i];
	parametrosEficiencia=new double[numpar];
	for(int i=0;i<numpar;i++)	parametrosEficiencia[i]=pars[i];
	//
	
	for (int i = 1; i <= listaPicos.numElementos; i++) 
		{
		NodoPicos *intervalo = DevuelveNodo(listaPicos, i);
		for(int j=1;j<=intervalo->picos.numElementos;j++)
			{	
			NodoPico *PicoCalibracion = DevuelveNodo(intervalo->picos, j);
			if(PicoCalibracion->Ajuste.identificado)
				{
				double energia=DaEnergia(PicoCalibracion->Ajuste.Centroide);
				PicoCalibracion->Ajuste.eficiencia=daEficiencia(energia);
				PicoCalibracion->Ajuste.sigmaEficiencia=daSigmaEficiencia(energia);
				PicoCalibracion->Ajuste.eficienciaCalculada=true;
			
//				PicoCalibracion->Ajuste.parametrosEficiencia=new double[dimensionMatEfi];
//				for(int k=0;k<dimensionMatEfi;k++)	PicoCalibracion->Ajuste.parametrosEficiencia[k]=afunc[k];
				
				//printf("Pico de energía %f tiene eficiencia %f (%f)\n", energia, yefi, sigmayefi);
				}//del if(identificado)
			}
		}
	}

//************* CALCULA MATRIZ COVARIANZA EFICIENCIA **********************
/*	La matriz de covarianza de la eficiencia no es sencilla de calcular.
	Tenemos que dar unas cuantas vueltas:
	1)Calcula Vz=At*Vp*A donde:
		-A es una matriz que por filas tiene los parámetros de la spline de bohr
		para el ajuste de eficiencias de cada pico.
		-At es traspuesta(A)
		-Vp es la matriz de covarianza de las eficiencias, obtenida a partir del
		fichero de eficiencias utilizado.
	
	Esta versión la calcula para la matriz de actividades situada en "pos"
		
	*/
double **cEspectros::calculaMatrizCovarianzaEficiencia(int pos)
	{
	LinkedList *matriz=(LinkedList *)matricesActividad->getNode(pos);
	printf("Matriz de actividad %d, de dim %dx%d\n", pos, matriz->numElements(), matriz->numElements());
	int DIM=matriz->numElements();
	double eficiencias[DIM];
		
	double **A=new double* [dimensionMatEfi];
	for(int j=0;j<dimensionMatEfi;j++)	A[j]=new double[DIM];
	for(int j=0;j<dimensionMatEfi;j++)	for(int k=0;k<DIM;k++)	A[j][k]=0;
	//printf("Construimos A, de dimensión %dx%d\n", dimensionMatEfi, DIM);
	//1) Construimos A
	for(int j=0;j<DIM;j++)
		{//Para cada pico de esta matriz de actividad
		NodoPico *nodo=(NodoPico *)matriz->getNode(j);
		eficiencias[j]=nodo->Ajuste.eficiencia;
		for(int k=0;k<dimensionMatEfi;k++)//Construimos columna de A
			//A[k][j]=nodo->Ajuste.parametrosEficiencia[k];
			A[k][j]=parametrosEficiencia[k];
		}
	//printf("Matriz A es (%dx%d)\n", dimensionMatEfi, DIM);
	//imprimeMatriz(A, dimensionMatEfi, DIM);
	//printf("Vector de eficiencias e[%d]:\n", DIM);
	//for(int j=0;j<DIM;j++)	printf("%f ", eficiencias[j]); 
	//2) Vz=At*Vp*A
	//double **At;
	int columnasT=dimensionMatEfi;
	int filasT=DIM;
	double **At=new double*[filasT];
	if(At==NULL)	printf("ERROR: No hay suficiente memoria para At\n");
	printf("Reservada la memoria para las %d filas\n", filasT);
	for(int i=0;i<filasT;i++)	
		{
		At[i]=new double[columnasT];
		if(At[i]==NULL)	printf("ERROR: No hay suficiente memoria para At[%d]\n", i);
		}
	for(int i=0;i<filasT;i++)	for(int j=0;j<columnasT;j++)	At[i][j]=0.0;
		
	printf("Haciendo transpuesta\n");
	//At=traspuesta(A, dimensionMatEfi, DIM);
	traspuesta(A, dimensionMatEfi, DIM, At);
	
	printf("Transpuesta terminada\n");
	//printf("Transpuesta de A (dim %dx%d):\n", DIM, dimensionMatEfi);
	//imprimeMatriz(At, DIM, dimensionMatEfi);//Parece que hasta aquí la cosa va
	//NOTA:Comprobar lo que vale matrizEficiencia y luego las multiplicaciones	
	//printf("Matriz de covarianzas de la eficiencia (Vp):\n");
	//imprimeMatriz(matrizEficiencia, dimensionMatEfi, dimensionMatEfi);
	
	double **temp;
	printf("multiplicando At x matrizEficiencia\n");
	temp=multiplica(At, DIM, dimensionMatEfi, matrizEficiencia, dimensionMatEfi, dimensionMatEfi);
	//printf("AtxVp=temp: \n");
	//imprimeMatriz(temp, DIM, dimensionMatEfi);	
	
	double **Vz;
	printf("multiplicando la anterior por A\n");
	Vz=multiplica(temp, DIM, dimensionMatEfi, A, dimensionMatEfi, DIM);	
	//3) Comprobación intermedia de resultados
	//printf("MATRIZ Vz: \n");
	//imprimeMatriz(Vz, DIM, DIM);
	//printf("Liberamos memoria\n");
	//4) Cálculo de Cz a partir de Vz
	double Cz[DIM][DIM];
	//printf("MATRIZ Cz:\n");
	for(int j=0;j<DIM;j++)
		{
		for(int k=0;k<DIM;k++)
			{
			Cz[j][k]=Vz[j][k]/(sqrt(Vz[j][j]*Vz[k][k]));
			//printf("%f ", Cz[j][k]);
			}
		//printf("\n");
		}
	//5)Cálculo de Ve: matriz de covarianza de las eficiencias
	double **Ve;
	Ve=new double *[DIM];
	for(int i=0;i<DIM;i++)	Ve[i]=new double[DIM];
	//printf("MATRIZ Ve FINAL:\n");
	for(int j=0;j<DIM;j++)
		{
		for(int k=0;k<DIM;k++)
			{
			Ve[j][k]= eficiencias[j]*sqrt(Vz[j][j])*Cz[j][k]*eficiencias[k]*sqrt(Vz[k][k]);
			//printf("%f ", Ve[j][k]);
			}
		//printf("\n");
		}
	
		
	//Liberación de memoria
	for(int j=0;j<DIM;j++)	
		{
		//printf("borramos At[%d]\n", j);
		delete[] At[j];	
		//printf("borramos temp[%d]\n", j);
		delete[] temp[j];	
		//printf("borramos Vz[%d]\n", j);
		delete Vz[j];
		}
	//printf("borramos temp\n");
	delete[] temp;	
	//printf("borramos At\n");
	delete[] At;	
	//printf("borramos Vz\n");
	delete [] Vz;
	
	//printf("Borramos A\n");
	for(int j=0;j<dimensionMatEfi;j++)	delete[] A[j];
	delete[] A;
	//printf("____________________________________________\n");
	printf("Termina CalculaMatrizCovarianzaEficiencia\n");
	return Ve;
	return NULL;
	}
//************* CALCULA MATRIZ COVARIANZA EFICIENCIA **********************
/*	Devuelve una lista enlazada con todas las matrices de covarianza	
	*/
LinkedList *cEspectros::calculaMatrizCovarianzaEficiencia()
	{
	LinkedList *lista=new LinkedList();
	printf("*************** CALCULA MATRIZ COVARIANZA EFICIENCIA *****************\n");
	for(int i=0;i<matricesActividad->numElements();i++)
		{//Para cada matriz de actividad distinta
		double **res=calculaMatrizCovarianzaEficiencia(i);
		lista->addLast(res);
		}
	printf("Fin de Calcula Matrices covarianza eficiencia\n");
	return lista;
	}

//************ IMPRIME MATRIZ **************	
void imprimeMatriz(double **m, int n1, int n2)
	{
	for(int i=0; i<n1;i++)
		{
		for(int j=0;j<n2;j++)
			{
			printf("%.2f ", m[i][j]);
			//printf("%f ", m[i][j]);
			}
		printf("\n");
		}
	}
//************* BOHR ************************
/*La salida de esta subrutina es el vector formado por las funciones 
 spline cubicas, base del espacio de la funcion ln(ef), para un valor
 dado de la energia.
	-energia: valor dado de energía
	-afunc: función de salida donde devolverá los parámetros de la spline
	-numpar: dimensión de afunc
	-nodos:
	-numnodos:dimensión de nodos
*/
void cEspectros::bohr(double energia, double *afunc, int numpar, double *nodos, int numnodos)
	{
	double e0,e1,e2,e3;
	e0=e1=e2=e3=0.0;
	int j=0;
		
	for(int i=0;i<numpar;i++)	afunc[i]=0.0;
	for(int i=1;i<numnodos-2;i++)
		{
		if(energia<nodos[i+1] && energia>=nodos[i])
			{
			e0=nodos[i-1];	
			e1=nodos[i];	
			e2=nodos[i+1];	
			e3=nodos[i+2];
			j=i+1;
				
			afunc[j]=((energia-e1)*(energia-e1)) / ((e3-e1)*(e2-e1));
			afunc[j-1]=( ((energia-e0)*(e2-energia)) / ((e2-e0)*(e2-e1)) ) +
					   ( ((e3-energia)*(energia-e1)) / ((e3-e1)*(e2-e1)) );
			afunc[j-2]=((e2-energia)*(e2-energia)) / ((e2-e1)*(e2-e0));
	     	}
		}
	}

//********* CALCULA CORRECCIONES EFICIENCIA ********************
/*	Para cada pico identificado, busca una energía coincidente con la suya
	en la lista de energías que se pasa como parámetro.
	Si la encuentra, establece como factor de corrección e incertidumbre
	los correspondientes de las otras dos cadenas que se pasan como parámetros.
	Como puede ser que los valores obtenidos para la energía del pico en la bd
	lara sean ligeramente distintos a los que se encuentran en los ficheros
	cs-*.dat que se usan para las correcciones, se añade un parámetro holgura
	para contar con esas ligeras variaciones, que deberían estar en el orden
	de la centésima
	*/
void cEspectros::calculaCorreccionesEficiencia(double *energias, double *factores, double *sigmaFactores, double holgura)
	{
//	double yefi, sigmayefi;
	//Recorremos lista picos y para cada uno de sus picos calculamos la eficiencia
	printf("CALCULA CORRECCIONES EFICIENCIA: \n");
			
	for (int i = 1; i <= listaPicos.numElementos; i++) 
		{
		NodoPicos *intervalo = DevuelveNodo(listaPicos, i);
		for(int k=1;k<=intervalo->picos.numElementos;k++)
			{
			NodoPico *PicoCalibracion = DevuelveNodo(intervalo->picos, k);
			if(PicoCalibracion->Ajuste.identificado)
				{
				bool encontrado=false;
				double energia=DaEnergia(PicoCalibracion->Ajuste.Centroide);
				printf("energia=%f\n", energia);
				for(unsigned int j=0;j<sizeof(energias);j++)
					{
					if(energia<=energias[j]+holgura && energia>=energias[j]-holgura)
						{//Si la energía está en los límites de holgura
						printf("La energía está en [%f,%f]\n", energias[j]+holgura, energias[j]-holgura);
						//NOTA: OJO: Es posible que el fichero no tenga ninguna energía que caiga en la
						//holgura o que directamente el fichero del que se trajeran las correcciones estuviera
						//vacío, en cuyo caso estamos metiendo un factor = 0 que va a dar problemas al calcular el MDA
						PicoCalibracion->Ajuste.correccion=factores[j];
						PicoCalibracion->Ajuste.sigmaCorreccion=sigmaFactores[j];
						printf("Va a tener %f(%f) de corrección\n", PicoCalibracion->Ajuste.correccion, PicoCalibracion->Ajuste.sigmaCorreccion);
						encontrado=true;
						}
					}
				//Si no había ninguna energía, ponemos 1 como corrección y 0 como incertidumbre
				if(!encontrado)
					{
					PicoCalibracion->Ajuste.correccion=1.0;
					PicoCalibracion->Ajuste.sigmaCorreccion=0.0;
					}
				printf("****Va a tener %f(%f) de corrección\n", PicoCalibracion->Ajuste.correccion, PicoCalibracion->Ajuste.sigmaCorreccion);
				}//if(picoIdentificado)
			}
		}
	}

//***************** CALCULA CHI CUADRADO *********************
/*	
	Devuelve el valor de chi cuadrado para la convolución de las
	distintas gaussianas sumadas que se encuentran en nuevosParámetros, que 
	contiene: [c1,a1,c2,a2,...,cn,an,s,t] y tiene longitud length.
	
	La calcula en el intervalo [limInferior, limInferior+numeroCanales], y siendo
	y la funcion de puntos que tratan de ajustar las gaussianas de numParametros
	
	En este caso se añaden unas restricciones a las áreas de las gaussianas que 
	penalizarán al chi² si las superan
*/	
float cEspectros::calculaChiCuadrado(float *nuevosParametros, int length, int limInferior, int numeroCanales, float *y, float *restricciones)
	{
	float chi=0;
	float errorCuadrado=0;
	float Fx[numeroCanales];
	int li=limInferior;
	float c=0, a=0, g=0, total=0;
	int cont;
	float penalizacion=0, area=0;
			
	float sigma	= nuevosParametros[length-2];
	float t	= nuevosParametros[length-1];
	int numParametros=(length-1)/2;

	for(int j=0;j<numeroCanales;j++)	
		{
		Fx[j]=0;
		total+=espectro[j+li-1]+sigmaContinuo[j+li-1]*sigmaContinuo[j+li-1];//Ni+sci²
		}
	
	penalizacion=0;
	for(int k=0;k<numParametros;k++)//Para cada parámetro calculamos su convolución y la restamos
		{
		c=nuevosParametros[k];
		a=nuevosParametros[k+numParametros];
			
		int holgura=5;//Menos sube el error, más sube el error y el tiempo
		int limInf=(int)(c-holgura*sigma);
		int limSup=(int)(c+holgura*sigma);
		if(limInf<li)	limInf=li;
		if(limSup>li+numeroCanales)	limSup=li+numeroCanales;
		
		cont=(int)(c-li);
		if(c-li>numeroCanales-1)	cont=numeroCanales-1;
		for(int j=(int)c;j>limInf;j--)//Recorremos para atrás hasta que se acabe la gaussiana
			{
			g=DaValorConvolucion(c, a, sigma, t, (double)j-1);
			if(g<=0)	j=limInf;
			else		Fx[cont--]+=g;
			}		
		cont=(int)(c-li)+1;
		for(int j=(int)c+1;j<limSup;j++)//Recorremos para adelante hasta que se acabe la gaussiana
			{
			g=DaValorConvolucion(c, a, sigma, t, (double)j-1);
			if(g==0)	j=limSup;
			else		Fx[cont++]+=g;
			}		
		area=a*t;
		if(restricciones!=NULL && restricciones[k]>0 && area>restricciones[k])	
			{
			//printf("area=%.2f y restricción: %.2f; penalización=%.2f\n", area, restricciones[k], area-restricciones[k]);				
			penalizacion+=area-restricciones[k];
			//printf("%d se pasa de la restricción de %d a %.2f a %.2f, penalización=%.2f (altura=%.2f)\n", i,k,restricciones[k], area, area-restricciones[k], a);
			}
		}//para cada parámetro
			
	for(int j=0;j<numeroCanales;j++)
		{
		errorCuadrado+=(y[j]-Fx[j])*(y[j]-Fx[j]);
		Fx[j]=0;
		}

//	printf("chi² es %f/%f\n", errorCuadrado, total);
	chi=errorCuadrado / (total);
	//printf("%.1f ", penalizacion);
	return chi+penalizacion;
	}
		
//***************** CALCULA CHI CUADRADO *********************
/*	
	Devuelve el valor de chi cuadrado para la convolución de las
	distintas gaussianas sumadas que se encuentran en nuevosParámetros, que 
	contiene: [c1,a1,c2,a2,...,cn,an,s,t] y tiene longitud length.
	
	La calcula en el intervalo [limInferior, limInferior+numeroCanales], y siendo
	y la funcion de puntos que tratan de ajustar las gaussianas de numParametros
*/	
float cEspectros::calculaChiCuadrado(float *nuevosParametros, int length, int limInferior, int numeroCanales, float *y)
	{
	float chi=0;
	float errorCuadrado=0;
	float Fx[numeroCanales];
	int li=limInferior;
	float c=0, a=0, g=0, total=0;
	int cont;
			
	float sigma	= nuevosParametros[length-2];
	float t	= nuevosParametros[length-1];
	int numParametros=(length-1)/2;

	for(int j=0;j<numeroCanales;j++)	
		{
		Fx[j]=0;
		total+=espectro[j+li-1]+sigmaContinuo[j+li-1]*sigmaContinuo[j+li-1];//Ni+sci²
		}
	
	for(int k=0;k<numParametros;k++)//Para cada parámetro calculamos su convolución y la restamos
		{
		c=nuevosParametros[k];
		a=nuevosParametros[k+numParametros];
			
		int holgura=5;//Menos sube el error, más sube el error y el tiempo
		int limInf=(int)(c-holgura*sigma);
		int limSup=(int)(c+holgura*sigma);
		if(limInf<li)	limInf=li;
		if(limSup>li+numeroCanales)	limSup=li+numeroCanales;
		
		cont=(int)(c-li);
		if(c-li>numeroCanales-1)	cont=numeroCanales-1;
		for(int j=(int)c;j>limInf;j--)//Recorremos para atrás hasta que se acabe la gaussiana
			{
			g=DaValorConvolucion(c, a, sigma, t, (double)j-1);
			if(g<=0)	j=limInf;
			else		Fx[cont--]+=g;
			}		
		cont=(int)(c-li)+1;
		for(int j=(int)c+1;j<limSup;j++)//Recorremos para adelante hasta que se acabe la gaussiana
			{
			g=DaValorConvolucion(c, a, sigma, t, (double)j-1);
			if(g==0)	j=limSup;
			else		Fx[cont++]+=g;
			}		
		}//para cada parámetro
			
	for(int j=0;j<numeroCanales;j++)
		{
		errorCuadrado+=(y[j]-Fx[j])*(y[j]-Fx[j]);
		Fx[j]=0;
		}
//	printf("chi² es %f/%f\n", errorCuadrado, total);
	chi=errorCuadrado / (total);
	return chi;
	}
	
//******************* GRID SEARCH ***********************
/*	Grid Search sin restricciones
	*/
void cEspectros::gridSearch(float *a, int length, int limInferior, int numeroCanales, float *y, int numIntervalo, float *nuevosParametros, float *sigmaNP)
	{
	gridSearch(a,length,limInferior, numeroCanales, y, numIntervalo, NULL, nuevosParametros, sigmaNP);
	}
	
//******************* GRID SEARCH ***********************
/*	Búsqueda reticular según [Bevington 69]
	Parámetros de entrada:
		y 				- vector con el espectro
		limInferior 	- primer canal de y
		numeroCanales 	- múmero de canales del espectro
	
		a				- vector con los parámetros a ajustar
		length			- longitud de a
	
//NOTA: En esta parte no estoy manteniendo el ROI!->Podría ser muy importante
		para considerar los datos como válidos
	*/
//NOTA: Tiene un último parámetro que sólo lo necesitamos para representarlo en gráfica
//float *cEspectros::gridSearch(float *a, int length, int limInferior, int numeroCanales, float *y, int numIntervalo, float *restricciones)
void cEspectros::gridSearch(float *a, int length, int limInferior, int numeroCanales, float *y, int numIntervalo, float *restricciones, float *nuevosParametros, float *sigmaNP)
	{
//	printf("GRID SEARCH CON RESTRICCIONES: \n");
	
	if(nuevosParametros==NULL || sigmaNP==NULL)
		{
		printf("ERROR: Parámetros a devolver nulos\n");
		return;
		}
		
	int nfree=numeroCanales-length;//Número de grados de libertad
	double x1, x2, x3, temp;
	
	double fn;
	bool stop=false;
	int ng=(length-2)/2;	
	for(int i=0;i<length;i++)		nuevosParametros[i]=a[i];
	
	double delta=0.1;
	clock_t ti, tf;
	
	for(int i=0;i<length;i++)
		{
//		printf("Cambiando parámetro %d: %.2f\n", i, a[i]);
		fn=0;
		ti=clock();
		x1=calculaChiCuadrado(nuevosParametros, length, limInferior, numeroCanales, y, restricciones);
		nuevosParametros[i]+=delta;
		x2=calculaChiCuadrado(nuevosParametros, length, limInferior, numeroCanales, y, restricciones);
		tf=clock();
		if(x2>x1)//Si x2 tiene mayor error que x1 es que hay que tirar pal otro lado
			{
			delta=-delta;
			nuevosParametros[i]+=delta;//Se queda en el medio
			temp=x1;
			x1=x2;//x1 pasa a ser la más alta y x2 la mediana
			x2=temp;
			}	
//		printf("Yendo hacia %f\n", delta);
		ti=clock();
		//NOTA: Voy a hacer el bucle un poco más inteligente para que no vaya pasín a pasín:
		if(i>=ng && i<ng*2)	delta=1000*delta;//vamos de 100 en 100 en el caso de las alturas
		float difDelta=0;
	//	printf("Empezamos, delta=%f, npinicial=%.2f\n", delta, nuevosParametros[i]);
		while(!stop)
			{
			nuevosParametros[i]+=delta;//Incrementamos para el tercer punto
			if(nuevosParametros[i]<0)	
				{
				difDelta=nuevosParametros[i];
				nuevosParametros[i]=0;
				}
			//NOTA: Faltarían las limitaciones a sigma y tau
			x3=calculaChiCuadrado(nuevosParametros,length, limInferior, numeroCanales, y, restricciones);//x3 debería ser la menor
	//		printf("x1=%.2f\tx2=%.2f\tx3=%.2f, nP=%.2f\n",x1,x2,x3, nuevosParametros[i]);
			if(x2>x3)
				{
				x1=x2;
				x2=x3;
				}
			else	
				{
				if(delta==0.1)
					{
					stop=true; //Ya no baja más el error
					float k=(x3-2*x2+x1);
					if(k!=0)	
						{
						nuevosParametros[i]=nuevosParametros[i]-delta*( 0.5+(x3-x2)/k );
						sigmaNP[i]=delta*sqrt( 2.0/(nfree*(k)) );
					//	printf("npfinal es %.2f\n", nuevosParametros[i]);
						}
					}
				else
					{
					if(difDelta!=0)	nuevosParametros[i]-=difDelta;
					else			nuevosParametros[i]-=delta;//Deshacemos lo que hicimos
					//nuevosParametros[i]-=delta;//Deshacemos lo que hicimos
					if(nuevosParametros[i]<0)	nuevosParametros[i]=0;
					delta=delta/2.0;//refinamos la búsqueda
					if(fabs(delta)<=0.1)	delta=0.1;
				//	printf("Nos hemos pasado, reducimos delta a %f\n", delta);
					}
				}
			}
		//-----------------------------------------
			
			tf=clock();
//			printf("Tiempo de iteración %d es %f\n", i, 2.0*(tf-ti)/CLOCKS_PER_SEC);
			
		stop=false;//Para la siguiente it.
//		printf("Parámetro %d termina siendo %f\n", i, nuevosParametros[i]);
		}
	printf("Error final del GS: %f\n", calculaChiCuadrado(nuevosParametros, length, limInferior, numeroCanales, y, restricciones));	
	
	return;
	}
	
//***************** DA NUM INTERVALO CON ENERGIA **************
/*	Devuelve el intervalo con la energía indicada.
	Si ese intervalo no aparece en la lista de intervalos, devuelve -1
	*/
int cEspectros::daNumIntervaloConEnergia(float energia)
	{
	for(int i=1;i<=listaPicos.numElementos;i++)
		{
		NodoPicos *intervPicos = DevuelveNodo(listaPicos, i);
		int minEnergia = (int) floor(DaEnergia((double)intervPicos->limInferior));
		int maxEnergia = (int) floor(DaEnergia((double)intervPicos->limSuperior))+1;
		//printf("Intervalo [%.0f, %.0f]\n", minEnergia, maxEnergia);
		if(energia>=minEnergia && energia<=maxEnergia)	return i;
		}
	return -1;
	}

//******************** RESTA AREA FONDO ****************
/*	Restamos el área del fondo a las áreas de nuestros picos.
	Si no hay area de fondo calculada, dejamos la normal.
	En otro caso restamos las cuentas correspondientes
	*/
void cEspectros::restaAreaFondo()
	{
	//En el caso de que no haya fondo cargado, el área sin fondo es el área normal
	if(cuentasFondo==NULL)
	  {	
	  printf("No hay fondo cargado, las áreas permanecen iguales\n");
	  for(int i=1;i<=listaPicos.numElementos;i++)
		{
		NodoPicos *nodoPicos=DevuelveNodo(listaPicos, i);
		for(int j=1;j<=nodoPicos->picos.numElementos;j++)
			{
			NodoPico  *nodo      = DevuelveNodo(nodoPicos->picos, j);
			if(nodo->Ajuste.identificado)
				{
				nodo->Ajuste.areaSinFondo=nodo->Ajuste.area;
				nodo->Ajuste.incerAreaSinFondo=nodo->Ajuste.incerArea;
				}
			}
		}
	  }
	 //En el caso de que tengamos fondo cargado, restamos todas las cuentas de ese intervalo
	else
	  {
	  for(int i=1;i<=listaPicos.numElementos;i++)
		{
		NodoPicos *nodoPicos=DevuelveNodo(listaPicos, i);
		for(int j=1;j<=nodoPicos->picos.numElementos;j++)
			{
			NodoPico  *nodo      = DevuelveNodo(nodoPicos->picos, j);
			//NOTA: Revisar si están bien calculadas las incertidumbres->Parece que está bien
			if(nodo->Ajuste.ajustado)
				{
				bool hayFondo=false;
				int li=nodoPicos->limInferior;
				int ls=nodoPicos->limSuperior;

				for(int k=li;k<ls;k++)
					{
					if(DaValorPico(nodoPicos, nodo, k)>continuo[k])//Si el pico está presente en ese canal
						{
						if(cuentasFondo[k]>0)	hayFondo=true;
						}
					}
				if(!hayFondo)
					{
					nodo->Ajuste.areaSinFondo=DaAreaPico(i,j);
					nodo->Ajuste.incerAreaSinFondo=DaIncerAreaPico(i,j);
					}
				else
					{
					nodo->Ajuste.areaSinFondo=0.0;
					nodo->Ajuste.incerAreaSinFondo=0;
				//	printf("Pico (%d,%d), limites en [%f,%f] keV, con área %f(%f)\n", i,j, DaEnergia(li), DaEnergia(ls), nodo->Ajuste.area, nodo->Ajuste.incerAreaSinFondo );
					for(int k=li;k<ls;k++)
						{
						double sum=DaValorPico(nodoPicos,nodo,k)-continuo[k]-cuentasFondo[k];
						if(sum<0)	sum=0;
				//		printf("Sumando para %d %f-%f-%f=%f\n",k, DaValorPico(nodoPicos,nodo,k), continuo[k],cuentasFondo[k], sum); 
						if(sum>0)
							{
							nodo->Ajuste.areaSinFondo+=sum;
							if(nodo->Ajuste.incerAreaSinFondo==0 && sigmaCuentasFondo[k]>0)
								{
								nodo->Ajuste.incerAreaSinFondo=sigmaCuentasFondo[k]*sigmaCuentasFondo[k];
								}
							}
						}
					if(nodo->Ajuste.areaSinFondo<0)	nodo->Ajuste.areaSinFondo=0;
					nodo->Ajuste.incerAreaSinFondo+=DaIncerAreaPico(i,j)*DaIncerAreaPico(i,j);
					nodo->Ajuste.incerAreaSinFondo=sqrt(nodo->Ajuste.incerAreaSinFondo);
				//	printf("Área (%d,%d) queda %f(%f)\n", i,j, nodo->Ajuste.areaSinFondo, nodo->Ajuste.incerAreaSinFondo); 
					}
				}	
			}
		}
	  }
  	}
