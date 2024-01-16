#include "GeneticAlgorithm2.h"
#include "math.h"

/* 2-XII Replanteamiento de la segunda parte:
		 No necesitamos un ratio que modifique la amplitud de la convolución.
		 Quitamos este segundo parámetro, sólo dejamos la sigma.
		 La amplitud de comienzo será area/tau del pico calibrado, una 
		 inicialización mucho más exacta que 1.5*amplitudGA1. Esta inicialización
		 es mala porque la convolución da una amplitud mucho más pequeña, luego
		 necesitamos area/tau para que sea mucho más alta y se adapte mejor.
		 
	13-XII Depurando el código y tratando de optimizarlo
*/

//************* DESTRUCTOR ********************
/*	//NOTA: Esto es un poco garrulo y falla, nueva, nextGeneration y población
	son Individuo[], tendríamos que borrarlos uno a uno. Tampoco vale un 
	delete [] nueva, free(nueva), free((void *)nueva); aunque compilan 
	free(nueva[i]), free((void *)nueva[i]) no compilan
	delete nueva[i].
	
	GeneticAlgorithm2::~GeneticAlgorithm2()
	{
	printf("borramos nueva\n");
	delete nueva;
	printf("borramos nextGen\n");
	delete nextGeneration;
	printf("borramos población\n");
	delete poblacion;
	}
	*/
	
/*	NOTA: Nuevo intento para liberar la memoria, las poblaciones ahora son
	de tipo **Individuo, así, podemos borrarla siguiendo el criterio de las
	cadenas de cadenas:
	1)	
		for(tam)	delete p[i];
		delete [] p;
		
		y delete [] y en *p.
	Da muchos mismatched/invalid free/delete
	Apenas baja la memoria que hay reservada.
	En ejecución sin valgrind parece que se queda frito en un determinarNaturales
*/
GeneticAlgorithm2::~GeneticAlgorithm2()
	{
	//printf("Borramos progenitores\n");
	if(progenitores!=NULL)	delete [] progenitores;
	//printf("Borramos restricciones\n");
	restricciones=NULL;
	}

void GeneticAlgorithm2::liberarMemoria()
	{
	for(int i=0;i<numeroPoblacion;i++)
		{
	//	printf("borramos población[%d]\n", i);
		poblacion[i]->liberarMemoria();
		}
	//printf("borramos nueva[]\n");
	delete [] nueva;
	//printf("borramos nextGen[]\n");
	delete [] nextGeneration;
	//printf("borramos poblacion[]\n");
	delete [] poblacion;
		
	//
	delete [] centroidesIniciales;
	}
	
//************* CONSTRUCTORES *****************
//Se basan en los constructores de GAI
GeneticAlgorithm2::GeneticAlgorithm2()
	{
	GeneticAlgorithm();
	elitismo=0;
	}

//Misma estructura que para GAI, pero los cromosomas de GAII tendrán un
//gen especializado más
GeneticAlgorithm2::GeneticAlgorithm2(int NnumeroPoblacion,  int NnumeroPadres,
								   int NnumeroMutaciones, float NfactorMutante,
								   int numeroParametros, float *Ny, 
								   int NnumeroCanales, cEspectros *Nespectro,
								   int NlimInferior)
	{
	generacion      = 0;
	numeroPoblacion	= NnumeroPoblacion;
	numeroPadres	= NnumeroPadres;
	numeroMutaciones= NnumeroMutaciones;
	factorMutante	= NfactorMutante;
	y				= Ny;
	numeroCanales	= NnumeroCanales;
	espectro		= Nespectro;
	numParametros	= numeroParametros;
	limInferior		= NlimInferior;
	
	restringirCentroides=false;
	elitismo=0;
		
	progenitores = new int[numeroPoblacion*2];
		
	nueva=new Individuo*[numeroPoblacion];
	if(nueva==NULL)	
		{
		printf("ERROR: No hay memoria para reservar LinkedList\n");
		return;
		}
		
	nextGeneration=new Individuo*[numeroPoblacion];
	if(nextGeneration==NULL)	
		{
		printf("ERROR: No hay memoria para reservar LinkedList\n");
		return;
		}	
	poblacion	= new Individuo*[numeroPoblacion];
	if(poblacion==NULL)	
		{
		printf("ERROR: No hay memoria para reservar LinkedList\n");
		return;
		}
	}

//************************* GENETIC ALGORITHM 2 ********************
//Constructor a partir del mejor individuo de la fase GA1.
//Para las amplitudes inicializaremos en función del área y tau de la calibración
//El resto son los 4 parámetros típicos del GA.
GeneticAlgorithm2::GeneticAlgorithm2(int NnumeroPoblacion,  int NnumeroPadres,
								   int NnumeroMutaciones, float NfactorMutante,
								   GeneticAlgorithm *GA1, float *taus)
	{
	generacion      = 0;
	numeroPoblacion	= NnumeroPoblacion;
	numeroPadres	= NnumeroPadres;
	numeroMutaciones= NnumeroMutaciones;
	factorMutante	= NfactorMutante;
	y				= GA1->getY();
	numeroCanales	= GA1->getNumeroCanales();
	espectro		= GA1->getEspectro();
	numParametros	= GA1->getNumParametros();
	limInferior		= GA1->getLimInferior();
	elitismo=0;
		
	restringirCentroides=false;
	progenitores = new int[numeroPoblacion*2];
	
	//Poblaciones auxiliares
	nueva=new Individuo*[numeroPoblacion];
	if(nueva==NULL)	
		{
		printf("ERROR: No hay memoria para reservar LinkedList\n");
		return;
		}		
	nextGeneration=new Individuo*[numeroPoblacion];
	if(nextGeneration==NULL)	
		{
		printf("ERROR: No hay memoria para reservar LinkedList\n");
		return;
		}	
	Individuo **preHumanos=GA1->getPoblacion();
	Individuo *adam=preHumanos[0];	
	float aleatorio;
		
	//Valores del genoma de cada individuo
	float c[numParametros];
	float a[numParametros];
	float s,t;
	
	//Valores de adam:
	float *cAdam=adam->getCentroides();
	float *aAdam=adam->getAmplitudes();
	float sAdam=adam->getSigma();	
		
	for(int i=0;i<numeroPoblacion;i++)
		{
		//La construcción del GAII a partir de los parámetros de GAI se
		//ha realizado según nuestro propio criterio por carecer de la información:
		//		Usamos variaciones del mejor individuo de GAI:
		//			c2=c1+-[0-1]*c1;	s2=s1+-[0-0.2]*s1
		//			a2=area/tau;		tau=variación(tauCalibración)
		for(int j=0;j<numParametros;j++)
			{//Creamos sus centroides y amplitudes a partir de los de adam
			aleatorio=-1 + 2.0*(float)((random()%100))/100.0;//Valor entre -1 y 1
			c[j]=cAdam[j]+aleatorio;
			// la amplitud del nuevo algoritmo genético no se puede sacar
			//  a partir de GA1 porque nos quedará muy pequeña. Lo haremos
			//  a partir del área y de la tau de la calibración.
			double area=GA1->getEspectro()->DaAreaGaussiana(aAdam[j], sAdam);
			a[j]=area/taus[(int)(c[j]-limInferior)];
			
			poblacion[i]->setCentroide(j,c[j]);
			poblacion[i]->setAmplitud(j,a[j]);				

			//Tau será una variación de la tau obtenida en la calibración				
			aleatorio=(0.5 + ((float)(random()%150)/100.0) );
			t=aleatorio*taus[(int)(c[j]-limInferior)];
			if(t<0.5)	t=0.5;
			if(t>4)		t=4;
			poblacion[i]->setTau(t);
			}	
			
		aleatorio=(-20+ (float)(random()%40))/100;//[-0.2,0.2]
		s=sAdam+sAdam*aleatorio;
		poblacion[i]->setSigma(s);
		}
	}

//************************* GENETIC ALGORITHM 2 ********************
//Constructor a partir de parámetros iniciales para utilizarlo independientemente
//de GAI, con restricciones ESTE ES EL BUENO
GeneticAlgorithm2::GeneticAlgorithm2(int NnumeroPoblacion,  int NnumeroPadres,
								   int NnumeroMutaciones, float NfactorMutante,
								   int numeroParametros, float *Ny, 
								   int NnumeroCanales, cEspectros *Nespectro,
								   int NlimInferior, 
								   float *centroides, float taus, float sigmas,
								   float *Nrestricciones, int Nelitismo)
	{
	printf("GENETIC ALGORITHM\n");
	generacion      = 0;
	numeroPoblacion	= NnumeroPoblacion;
	numeroPadres	= NnumeroPadres;
	numeroMutaciones= NnumeroMutaciones;
	factorMutante	= NfactorMutante;
	y				= Ny;
	numeroCanales	= NnumeroCanales;
	espectro		= Nespectro;
	numParametros	= numeroParametros;
	limInferior		= NlimInferior;
	elitismo		= Nelitismo;
		
	progenitores = new int[numeroPoblacion*2];
	
	nueva=new Individuo*[numeroPoblacion];
	if(nueva==NULL)	
		{
		printf("ERROR: No hay memoria para reservar LinkedList\n");
		return;
		}		
	nextGeneration=new Individuo*[numeroPoblacion];
	if(nextGeneration==NULL)	
		{
		printf("ERROR: No hay memoria para reservar LinkedList\n");
		return;
		}
	
	poblacion	= new Individuo*[numeroPoblacion];
	if(poblacion==NULL)	
		{
		printf("ERROR: No hay memoria para reservar LinkedList\n");
		return;
		}	
		
	for(int i=0;i<numeroPoblacion;i++)
		{
		poblacion[i]=new Individuo(numParametros);
		nextGeneration[i]=new Individuo(numParametros);
		}
		
	float aleatorio=0;
	float s,t;
	float c[numParametros];
	float a[numParametros];
	
	//Establecemos restricciones de valores
	printf("Restricciones a los  valores\n");
	bool aniquilacion=false;
	for(int i=0;i<numeroParametros;i++)
		{
		printf("Comprobamos energía de centroides [%d]\n", i);
		double en=espectro->DaEnergia(centroides[i]);
		printf("Comprobada\n");
	
		if(en>510 && en<512)
			{
			aniquilacion=true;
			i=numeroParametros;
			}
		}
	
	//Si el pico es de aniquilación, no aplicamos restricciones a la anchura
	if(taus<=0 || sigmas<=0)
		{
		printf("Sin restricciones a tau y sigma\n");
		liTau=liSigma=-1;
		lsTau=lsSigma=-1;
		printf("Restringir centroides lo quitamos\n");
		restringirCentroides=false;
		printf("Quitado\n");
		}
	else
		{
		restringirCentroides=true;
		if(aniquilacion)
			{
			printf("-------ANIQUILACIÓN-----------\n");
			liTau=1.5*taus;
			lsTau=2.5*taus;
			liSigma=2.5*sigmas;
			lsSigma=3.5*sigmas;
			}
		else
			{
			printf("Tenemos restricciones\n");
			liTau=0.5*taus;
			lsTau=1.5*taus;
			liSigma=0.5*sigmas;
			lsSigma=1.5*sigmas;
			printf("Fin de restricciones\n");
			}
		}
		
	printf("Creamos centroides iniciales\n");
	centroidesIniciales=new float[numParametros];
	if(centroidesIniciales==NULL)	printf("ERROR: No hay memoria para reservar float[%d]\n", numParametros);
	for(int i=0;i<numParametros;i++)	centroidesIniciales[i]=centroides[i];
	restricciones=Nrestricciones;
	printf("Tau variará entre %f y %f\n", liTau, lsTau);
	printf("Sigma variará entre %f y %f\n", liSigma, lsSigma);
	for(int i=0;i<numeroPoblacion;i++)
		{
/*		if(liSigma>0 && lsSigma>0)	s= liSigma + ((float)((random()%100)/100.0)*(lsSigma-liSigma));
		else						s= 3*sigmas[0];//El pico de aniquilación es como el triple
		poblacion[i]->setSigma(s);//varía en [liSigma,lsSigma]
	
		if(liTau>0 && lsTau>0)	t=(liTau + ((float)(random()%100)/100.0)*(lsTau-liTau));
		else					t= 2*espectro->DaTau(centroides[i]);
		poblacion[i]->setTau(t);
*/		
		if(aniquilacion)				s= 3*sigmas;//El pico de aniquilación es como el triple
		else
			{
			if(liSigma>0 && lsSigma>0)	
				{
				s= liSigma + ((float)((random()%100)/100.0)*(lsSigma-liSigma));
//				printf("Hay restricción, s=%f\n",s);
				}
			else						
				{
				s= espectro->DaSigmaCh(espectro->DaEnergia(centroides[numParametros/2]) );//sigma en función de la calibracion
				s= s-0.5+((float)(random()%100)/100.0);
				if(s<0.1)	s=0.1;
//				printf("No hay restricción, s=%f\n", s);
				}
			}
		poblacion[i]->setSigma(s);
	
		if(aniquilacion)			t= 2*sigmas;
		else
			{
			if(liTau>0 && lsTau>0)	
				{
				t=(liTau + ((float)(random()%100)/100.0)*(lsTau-liTau));
//				printf("Hay restricción: t=%f\n", t);
				}
			else					
				{
				t= espectro->DaTauCh(espectro->DaEnergia(centroides[numParametros/2]) );
				t= t-0.5+((float)(random()%100)/100.0);
				if(t<0.1)	t=0.1;
//				printf("No hay restricción: t=%f (tau de centroide %f)\n", t, centroides[numParametros/2]);
				}
			}
		poblacion[i]->setTau(t);
		
		for(int j=0;j<numParametros;j++)
			{//Creamos sus centroides y amplitudes a partir de los de adam
			
			if(restringirCentroides)	aleatorio=-0.1 + 0.2*(float)((random()%100))/100.0;//Valor entre -.1 y .1
			else						aleatorio=-0.5 + (float)((random()%100))/100.0;//Valor entre -.5 y .5

			c[j]=centroides[j]+aleatorio;
			//printf("A población\n");
			poblacion[i]->setCentroide(j,c[j]);
			//printf("Seguimos\n");
/*
			//printf("Añadimos tau\n");
			//Tau será una variación de la tau obtenida en la calibración				
			t=(liTau + ((float)(random()%100)/100.0)*(lsTau-liTau));//NOTA: OJO: Varía dentro del bucle para satisfacer variaciones en la altura
			//t=aleatorio*taus[(int)(centroides[j]-limInferior)];
			if(t<liTau)	t=liTau;
			if(t>lsTau)	t=lsTau;//Antes [0.2,4]
			poblacion[i]->setTau(t);
		*/
			a[j]=0;
			if(restricciones[j]>0)	
				{
				//printf("Altura con restricciones\n");
				a[j]=restricciones[j]/(t*1.1);
		//		a[j]=restricciones[j]/(t*1.3);
				aleatorio=0.7+0.6*((float)(random()%100)/100.0);
				a[j]*=aleatorio;
//				if(j==numParametros-1)	printf("h=%.2f, a=a[j]*t=%.2f (r=%.2f)\n", a[j], a[j]*t, restricciones[j]);
				}
			else
				{
				// la altura se saca
				//  a partir del área que correspondería a una gaussiana que tuviera
				// por altura el número de cuentas que hay en el centroide; 
				//	y de la tau de la calibración.
				//printf("Altura sin restricciones\n");
				float amplitudDesplazada=y[(int)(c[j]-limInferior)];
					//-----
				double area=espectro->DaAreaGaussiana(amplitudDesplazada, s);
				//double area=amplitudDesplazada*t;
				a[j]=area/t;
					//----
					
			//	aleatorio=-0.5+1.5*((float)(random()%100)/100.0);
			//	a[j]=amplitudDesplazada*aleatorio;
				}
//			printf("área de la gaussiana %d es %f\n", j, a[j]*t);
			poblacion[i]->setAmplitud(j,a[j]);				
			}	
		}//para cada elemento de la población
	printf("GA construido\n");
	}
//************************* GENETIC ALGORITHM 2 ********************
//Constructor a partir de parámetros iniciales para utilizarlo independientemente
//de GAI 
GeneticAlgorithm2::GeneticAlgorithm2(int NnumeroPoblacion,  int NnumeroPadres,
								   int NnumeroMutaciones, float NfactorMutante,
								   int numeroParametros, float *Ny, 
								   int NnumeroCanales, cEspectros *Nespectro,
								   int NlimInferior, 
								   float *centroides, float *taus, float *sigmas)
	{
	generacion      = 0;
	numeroPoblacion	= NnumeroPoblacion;
	numeroPadres	= NnumeroPadres;
	numeroMutaciones= NnumeroMutaciones;
	factorMutante	= NfactorMutante;
	y				= Ny;
	numeroCanales	= NnumeroCanales;
	espectro		= Nespectro;
	numParametros	= numeroParametros;
	limInferior		= NlimInferior;
	elitismo=0;
	progenitores = new int[numeroPoblacion*2];
	restringirCentroides=true;
	nueva=new Individuo*[numeroPoblacion];
	if(nueva==NULL)	
		{
		printf("ERROR: No hay memoria para reservar LinkedList\n");
		return;
		}		
	nextGeneration=new Individuo*[numeroPoblacion];
	if(nextGeneration==NULL)	
		{
		printf("ERROR: No hay memoria para reservar LinkedList\n");
		return;
		}
	poblacion	= new Individuo*[numeroPoblacion];
	if(poblacion==NULL)	
		{
		printf("ERROR: No hay memoria para reservar LinkedList\n");
		return;
		}	
	float aleatorio=0;
	float s,t;
	float c[numParametros];
	float a[numParametros];
	
	//Establecemos restricciones de valores
	liTau=taus[0];
	lsTau=taus[numeroCanales-1];//NOTA: Igual son demasiado estrictas
	//liTau=0.2;
	//lsTau=4;
	
	liSigma=sigmas[0]-0.1;
	lsSigma=sigmas[numeroCanales-1]+1.5;
	//liSigma=0.5;
	//lsSigma=8;
	
	printf("Tau variará entre %f y %f (antiguo)\n", liTau, lsTau);
	printf("Sigma variará entre %f y %f (antiguo)\n", liSigma, lsSigma);
			
	for(int i=0;i<numeroPoblacion;i++)
		{
		//s= 0.5 + ((float)(random()%250)/100.0);//Varía en [0.5,3]
		s= liSigma + ((float)(random()%100)/100.0)*(lsSigma-liSigma);
		poblacion[i]->setSigma(s);//varía en [liSigma,lsSigma]
		for(int j=0;j<numParametros;j++)
			{//Creamos sus centroides y amplitudes a partir de los de adam
			aleatorio=-1 + 2.0*(float)((random()%100))/100.0;//Valor entre -1 y 1
			c[j]=centroides[j]+aleatorio;
			poblacion[i]->setCentroide(j,c[j]);


			//Tau será una variación de la tau obtenida en la calibración				
			//aleatorio=(0.5 + ((float)(random()%150)/100.0) );
			aleatorio=(liTau + ((float)(random()%100)/100.0)*(lsTau-liTau));
			t=aleatorio*taus[(int)(centroides[j]-limInferior)];
			if(t<liTau)	t=liTau;
			if(t>lsTau)	t=lsTau;//Antes [0.2,4]
			poblacion[i]->setTau(t);

			// la amplitud del nuevo algoritmo genético no se puede sacar
			//  a partir de GA1 porque nos quedará muy pequeña. Lo haremos
			//  a partir del área y de la tau de la calibración.
			float amplitudDesplazada=y[(int)(c[j]-limInferior)];
			double area=espectro->DaAreaGaussiana(amplitudDesplazada, s);
			a[j]=area/t;
			
			//NOTA: Creo que es necesario aleatorizar un poco este valor
			//aleatorio=-1+2*((float)((random()%100))/100.0);//Valor entre -1 y 1
			//a[i]=a[i]*aleatorio;
			poblacion[i]->setAmplitud(j,a[j]);				

			}	
		}
	printf("Fin del constructor\n");
	}

//************	 GET PRESION SELECTIVA ************
float GeneticAlgorithm2::getPresionSelectiva()
	{
	return numeroPoblacion*getMejorScore();
	}

//********* GET SCORE *************
float GeneticAlgorithm2::getScore(int i)
	{
	return getAjuste(i)/getSumaAjustes();
	}

//************ GET MEJOR SCORE ******************
float GeneticAlgorithm2::getMejorScore()
	{
	return getMejorAjuste()/getSumaAjustes();
	}

//************** GET AJUSTE *****************
float GeneticAlgorithm2::getAjuste(int i)
	{
	return poblacion[i]->getAjuste();
	}

//*************  GET SUMA AJUSTES **************
float GeneticAlgorithm2::getSumaAjustes()
	{
	float temp=0;
	for(int i=0;i<numeroPoblacion;i++)	temp+=poblacion[i]->getAjuste();
	return temp;
	}

//***************** GET DIVERSIDAD ************
float GeneticAlgorithm2::getDiversidad()
	{
	float temp=0;
	float resta=0;
	float media=getScoreMedio();
	for(int i=0;i<numeroPoblacion;i++)
		{
		resta=getScore(i)-media;
		temp+=resta*resta;
		}
	return sqrt(temp);
	}

//***************** GET SUMA SCORES *************
float GeneticAlgorithm2::getSumaScores()
	{
	float temp=0;
	for(int i=0;i<numeroPoblacion;i++)
		{
		temp+=getScore(i);
		}
	return temp;
	}
	
//**************** GET SCORE MEDIO *********
float GeneticAlgorithm2::getScoreMedio()
	{
	return getSumaScores()/numeroPoblacion;
	}

//**************** GET DIVERGENCIA *********
/*	Devuelve la diferencia entre el valor de ajuste
	del mejor individuo y la media de ajuste de la población
	Funciona como un método de terminación del algoritmo genetico
	quizás menos estricto que el de la mera frontera generacional
	*/
float GeneticAlgorithm2::getDivergencia()
	{
	return getAjusteMedio()-getMejorAjuste();
	}

//***************** SELECCIONAR ******************
/*	Crea una piscina de padres listos para procrear.
	En la piscina hay 2 veces el número de individuos,
	de forma que cada pareja pueda generar un individuo
	para la siguiente generación.
	Los padres se eligen de entre los numPadres mejor
	ajustados, con más probabilidades de aparecer cuanto
	mejor ajuste tengan.
*/
int *GeneticAlgorithm2::seleccionar()
	{
	float ruleta[numeroPadres];
	float maxRuleta=0;
//	printf("SELECCIONAR\n");

	ordenar();
//		printf("seguimos seleccionando\n");
	//Tomamos la suma de los ajustes de todos los padres
	maxAjuste=0;
	for(int i=0;i<numeroPadres;i++)		maxAjuste += 1/poblacion[i]->getAjuste();			
	for(int i=0;i<numeroPadres;i++)
		{
		ruleta[i] = (1/poblacion[i]->getAjuste())/maxAjuste;
		maxRuleta+=ruleta[i];
		}		
	for(int i=0;i<numeroPoblacion*2;i++)	progenitores[i]=0;
		
	//Calculamos los progenitores en función de la ruleta de la vida
	//(muestreo estocástico simple)
	//Mejor ajuste -> más probabilidad de procrear
	
	
	for(int i=0;i<numeroPoblacion*2;i++)
		{
		//printf("progenitor %d\n", i);
		//Cogemos progenitor.
		float aleatorio=(float)((random()%100))/100.0;
		int j=0;
		float temp=0.0;
		while(temp<=aleatorio && j<numeroPadres)	temp+=ruleta[j++];
		progenitores[i]=j-1;	
		}
	///---fin m.e.s.
	
	
	// Ruleta con muestreo estocástico universal
	//			NOTA: Un poco difícil de hacer porque tal y como estamos haciéndolo
	//			haríamos que se reprodujera cada uno consigo mismo la mayoría de las veces
	/*
	int muestreo[numeroPoblacion*2];
	float aleatorio=(float) ((random()%100))/100.0;
	float offset=(float)(1.0/(numeroPoblacion*2));
	for(int i=0;i<numeroPoblacion*2;i++)
		{
		float temp=0.0;
		//Cogemos progenitor.
		int j=0;
		float cuenta=(float)(aleatorio+offset*i);
		if(cuenta>1)	cuenta=cuenta-1;
		while(cuenta>temp && j<numeroPadres)	temp+=ruleta[j++];
		muestreo[i]=j;
		}
	
	//Ya tenemos a los padres elegidos por un muestreo estocástico, pero ordenados
	//secuencialmente. Los ordenamos aleatoriamente para que no se reproduzcan consigo
	//mismos
	for(int i=0;i<numeroPoblacion*2;i++)	
		{
		aleatorio=(random()%(numeroPoblacion*2-i));
		progenitores[i]=muestreo[(int)aleatorio];
		for(int j=(int)aleatorio;j<(numeroPoblacion*2-i);j++)
			muestreo[j]=muestreo[j+1]; //Pisamos el que ya está pasado
		}
	*///---fin m.e.u.
		
//	printf("FIn seleccionar\n");
	return progenitores;
	}

//*********** ORDENAR **************
/*	Ordena la lista enlazada de población en función del
	ajuste, siendo el primer nodo el correspondiente al
	individuo mejor ajustado.

	La selección es un proceso más complicado. Cada individuo tiene
	una probabilidad de procrear en función de la bondad de su ajuste.
	A mejor ajuste (más cercano a 1) más probabilidades. La barrera numPadres
	simplemente indica dónde la probabilidad pasa a ser cero
*/
void GeneticAlgorithm2::ordenar()
	{
	Individuo *actual;
	int posMinimo=0;
	float valMinimo;

//	bool *ordenado=new bool[numeroPoblacion];
	bool ordenado[numeroPoblacion];
	for(int i=0;i<numeroPoblacion;i++)	ordenado[i]=false;

//	printf("ORDENAR\n");
	for(int i=0;i<numeroPoblacion;i++)
		{
//		printf("ordenando %d\n", i);
		posMinimo=0;
//		while(poblacion[posMinimo]->getAjuste()==-1)	posMinimo++;
		while(ordenado[posMinimo])	{posMinimo++;/* printf("Empezamos en %d\n", posMinimo);*/ }
		valMinimo=poblacion[posMinimo]->getAjuste();
		
		for(int j=posMinimo+1;j<numeroPoblacion;j++)		
			{
//			if(poblacion[j]->getAjuste()!=-1 && poblacion[j]->getAjuste() < valMinimo)
			if(!ordenado[j] && poblacion[j]->getAjuste() < valMinimo)
				{
				valMinimo=poblacion[j]->getAjuste();
				posMinimo=j;
				}
			}
		nueva[i]=poblacion[posMinimo];

		//poblacion[posMinimo]->setAjuste(-1);//OJOO!->tras ordenar vamos a tener los ajustes a-1!!!!
		ordenado[posMinimo]=true;
		}
	//printf("Convertimos nueva en la nueva poblacion\n");
	for(int i=0;i<numeroPoblacion;i++)	poblacion[i]=nueva[i];
	//for(int i=0;i<numeroPoblacion;i++)	printf("%.2f ", poblacion[i]->getAjuste());
//	delete [] ordenado;
//	printf("Fin de ordenar\n");
	}
	
//******************** MUTAR *****************
//Función similar a la de GAI pero contando los nuevos parámetros
/*
	Hay varias formas de realizar la mutación:
		Mutación estacionaria a lo bruto: variar aleatoriamente entre sus límites
		Mutación estacionaria local: multiplicarlo por un número aleatorio
		Mutación no estacionaria: formulita de la pág. 122 de los apuntes
*/
void GeneticAlgorithm2::mutar()
	{
	int mutante=0;
	int cromosomaMutante=0;
	float valorMutado=0.0;
	float valorMutacion=0.0;
		
//	printf("MUTAR\n");
	//Nuestra forma de mutación no es excluyente: un individuo que haya sufrido
	//una mutación puede sufrir mutaciones adicionales, en el mismo o en otros
	//cromosomas
	for(int i=0;i<numeroMutaciones;i++)
		{
		mutante=(int)(random()%numeroPoblacion);
		cromosomaMutante=(int)(random()%(2*numParametros+2+1));//Los dos de más son sigma y tau
		valorMutacion      =(1-2*((float)(random()%100)/100.0))*factorMutante;//Fluctuará en [-fM,fM]
		
		if(cromosomaMutante<numParametros)
			{//El cromosoma mutante corresponde a un centroide
			float c=poblacion[mutante]->getCentroide(cromosomaMutante);
			
			if(restringirCentroides)	valorMutado=c+0.2*valorMutacion;
			else						valorMutado=c+0.5*valorMutacion;
			if(restringirCentroides && valorMutado>c+.1)	valorMutado=c+.1;
			if(restringirCentroides && valorMutado<c-.1)	valorMutado=c-.1;
			
			if(valorMutado<limInferior)	valorMutado=limInferior; //No dejamos ajustar centroides fuera del intervalo
																//Como mucho dejamos que tengan su centroide en el centro				
			if(valorMutado>limInferior+numeroCanales)	valorMutado=limInferior+numeroCanales; //No dejamos ajustar centroides fuera del intervalo
//			printf("mutación centroide (restringir=%d) %f -> %f\n", restringirCentroides, poblacion[mutante]->getCentroide(cromosomaMutante), valorMutado);
			poblacion[mutante]->setCentroide(cromosomaMutante, valorMutado);
			}
		else
			{
			if(cromosomaMutante>=numParametros && cromosomaMutante<2*numParametros)
				{//El cromosoma poblacion[mutante] corresponde a una amplitud
				valorMutado=poblacion[mutante]->getAmplitud(cromosomaMutante/2)*(2*valorMutacion);
				if(valorMutado<0)	valorMutado=0;
				poblacion[mutante]->setAmplitud(cromosomaMutante/2, valorMutado);
				}
			else
				{				
				if(cromosomaMutante==2*numParametros+1)
					{//El cromosoma poblacion[mutante] es la sigma
					valorMutacion+=1;//Para que la fluctuación sea en [0,2] en vez de [-1,1]
					valorMutado=poblacion[mutante]->getSigma()*(valorMutacion);
					if(liSigma>0 && valorMutado<liSigma)		valorMutado=liSigma;
					else if(lsSigma>0 && valorMutado>lsSigma)	valorMutado=lsSigma;
					//printf("mutar sigma: %f --> %f, li y ls=%f,%f\n", poblacion[mutante]->getSigma(), valorMutado, liSigma, lsSigma);
					poblacion[mutante]->setSigma(valorMutado);
					}
				else if(cromosomaMutante==2*numParametros+2)
						{//Es la tau
						valorMutacion+=1;//Para que la fluctuación sea en [0,2] en vez de [-1,1]
						valorMutado=poblacion[mutante]->getTau()*(valorMutacion);
//						printf("valor mutado=%f=%f*%f\n", valorMutado, poblacion[mutante]->getTau(), valorMutacion);
						if(liTau>0 && valorMutado<liTau)		valorMutado=liTau;
						else if(lsTau>0 && valorMutado>lsTau)	valorMutado=lsTau;
						//printf("mutar tau: %f --> %f, li y ls=%f,%f\n", poblacion[mutante]->getTau(), valorMutado, liTau, lsTau);
						poblacion[mutante]->setTau(valorMutado);
						}
				}
			}
		}			
//	printf("Termino de mutar\n");
	}
	
//NOTA: Variación de la mutación. Con la mutación de arriba, tau y sigma, aunque
//		sean parámetros iguales para todos los picos, afectan a todos los picos,
//		y sin embargo no mutan con la misma facilidad. En esta versión, tau y
//		sigma mutan en la misma tasa que los centroides o amplitudes (en conjunto)
/*
void GeneticAlgorithm2::mutar()
	{
	int mutante=0;
	int cromosomaMutante=0;
	float valorMutado=0.0;
	float valorMutacion=0.0;
		
//	printf("MUTAR\n");

	for(int i=0;i<numeroMutaciones;i++)
		{
		mutante=(int)(random()%numeroPoblacion);
		cromosomaMutante=(int)(random()%(4*numParametros+1));//Los dos de más son sigma y tau
		valorMutacion      =(1-2*((float)(random()%100)/100.0))*factorMutante;//Fluctuará en [-fM,fM]
		
		if(cromosomaMutante<numParametros)
			{//El cromosoma mutante corresponde a un centroide
			float c=poblacion[mutante]->getCentroide(cromosomaMutante);
			//valorMutado=c+valorMutacion;
			//if(valorMutado>c+1)	valorMutado=c+1;
			//if(valorMutado<c-1)	valorMutado=c-1;
			
			valorMutado=c+0.5*valorMutacion;
			if(valorMutado>c+.5)	valorMutado=c+.5;
			if(valorMutado<c-.5)	valorMutado=c-.5;
			
			if(valorMutado<limInferior)	valorMutado=limInferior; //No dejamos ajustar centroides fuera del intervalo
																//Como mucho dejamos que tengan su centroide en el centro				
			if(valorMutado>limInferior+numeroCanales)	valorMutado=limInferior+numeroCanales; //No dejamos ajustar centroides fuera del intervalo
			//printf("mutación centroide %f -> %f\n", poblacion[mutante]->getCentroide(cromosomaMutante), valorMutado);
			poblacion[mutante]->setCentroide(cromosomaMutante, valorMutado);
			}
		else
			{
			if(cromosomaMutante>=numParametros && cromosomaMutante<2*numParametros)
				{//El cromosoma poblacion[mutante] corresponde a una amplitud
//				valorMutado=poblacion[mutante]->getAmplitud(cromosomaMutante/2)*(1.5+valorMutacion);
//				printf("Mutación amplitud\n");
				valorMutado=poblacion[mutante]->getAmplitud(cromosomaMutante/2)*(2*valorMutacion);
				//if(cromosomaMutante/2 == 2) printf("Altura2 mutante con valor %.2f\n", poblacion[mutante]->getAmplitud(2));
				if(valorMutado<0)	valorMutado=0;
				poblacion[mutante]->setAmplitud(cromosomaMutante/2, valorMutado);
				}
			else
				{				
				if(cromosomaMutante>=2*numParametros && cromosomaMutante<3*numParametros)
					{//El cromosoma poblacion[mutante] es la sigma
//					valorMutado=poblacion[mutante]->getSigma()*(1+valorMutacion);
//					printf("Mutación sigma\n");
					valorMutado=poblacion[mutante]->getSigma()*(valorMutacion);
					if(valorMutado<liSigma)	valorMutado=liSigma;
					else if(valorMutado>lsSigma)	valorMutado=lsSigma;
					poblacion[mutante]->setSigma(valorMutado);
					}
				else if(cromosomaMutante>=3*numParametros)
						{//Es la tau
//						valorMutado=poblacion[mutante]->getTau()*(1+valorMutacion);
						valorMutado=poblacion[mutante]->getTau()*(valorMutacion);
//						printf("Mutación tau a %f\n", valorMutado);
						if(valorMutado<liTau)	valorMutado=liTau;
						else if(valorMutado>lsTau)	valorMutado=lsTau;
						poblacion[mutante]->setTau(valorMutado);
						}
				}
			}
		}			
	}
	*/

//********************* PROCREAR **************
//Función similar a la de GAI pero contando los nuevos parámetros
void GeneticAlgorithm2::procrear()
	{
//	printf("PROCREAR\n");
		
	float centroidesHijo[numParametros];
	float amplitudesHijo[numParametros];
	float sigmaHijo, tauHijo;
	int numPadre, numMadre;
	float ajustePadre, ajusteMadre;
	
	seleccionar();//Actualizamos la lista de progenitores con los mejores según ruleta
//	printf("Seguimos con procrear\n");
		
	//ELITISMO
	//Primero ponemos tantos como elitismo.
	//printf("Elitismo=%d\n", elitismo);
	for(int i=0;i<elitismo;i++)
		{
		printf("Elemento de élite %d tiene error %f\n", i, poblacion[i]->getAjuste());
		nextGeneration[i]->setCentroides(poblacion[i]->getCentroides());
		nextGeneration[i]->setAmplitudes(poblacion[i]->getAmplitudes());
		nextGeneration[i]->setSigma(poblacion[i]->getSigma());
		nextGeneration[i]->setTau(poblacion[i]->getTau());
		}
		
	//CRUCE ARITMÉTICO PONDERADO (CON ELITISMO)
	for(int i=elitismo;i<numeroPoblacion;i++)
		{
		//printf("Creando hijo %d\n", i);
		numPadre=progenitores[(i-elitismo)*2];
		numMadre=progenitores[(i-elitismo)*2+1];

		ajustePadre=poblacion[numPadre]->getAjuste();
		ajusteMadre=poblacion[numMadre]->getAjuste();
		
		//NOTA: Podría darse el caso de que un individuo generara él sólo un individuo sin la comprobación anterior(onanismo fértil!!!)
		for(int j=0;j<numParametros;j++)
			{
			centroidesHijo[j]=(poblacion[numMadre]->getCentroide(j)/ajusteMadre+poblacion[numPadre]->getCentroide(j)/ajustePadre)/(1/ajustePadre+1/ajusteMadre);
			amplitudesHijo[j]=(poblacion[numMadre]->getAmplitud(j)/ajusteMadre+poblacion[numPadre]->getAmplitud(j)/ajustePadre)/(1/ajustePadre+1/ajusteMadre);
			if(amplitudesHijo[j]<0)	amplitudesHijo[j]=0;	
			if(centroidesHijo[j]<limInferior)	centroidesHijo[j]=limInferior; //No dejamos ajustar centroides fuera del intervalo
			if(centroidesHijo[j]>limInferior+numeroCanales)	centroidesHijo[j]=limInferior+numeroCanales; //No dejamos ajustar centroides fuera del intervalo
			if(restringirCentroides && centroidesHijo[j]>centroidesIniciales[j]+0.1)	centroidesHijo[j]=centroidesIniciales[j]+0.1;
			if(restringirCentroides && centroidesHijo[j]<centroidesIniciales[j]-0.1)	centroidesHijo[j]=centroidesIniciales[j]-0.1;
			}
		sigmaHijo=(poblacion[numMadre]->getSigma()/ajusteMadre+poblacion[numPadre]->getSigma()/ajustePadre)/(1/ajustePadre+1/ajusteMadre);
		tauHijo=(poblacion[numMadre]->getTau()/ajusteMadre+poblacion[numPadre]->getTau()/ajustePadre)/(1/ajustePadre+1/ajusteMadre);
	
		//Condiciones para mantenerse en el ROI
		if(liSigma>0 && sigmaHijo<liSigma)		sigmaHijo=liSigma;
		else if(lsSigma>0 && sigmaHijo>lsSigma)	sigmaHijo=lsSigma;
		if(liTau>0 && tauHijo<liTau)			tauHijo=liTau;
		else if(lsTau>0 && tauHijo>lsTau)		tauHijo=lsTau;
		
		nextGeneration[i]->setCentroides(centroidesHijo);
		nextGeneration[i]->setAmplitudes(amplitudesHijo);
		nextGeneration[i]->setSigma(sigmaHijo);
		nextGeneration[i]->setTau(tauHijo);
		}
	//---- fin elitismo
		
	/*PROCREACIÓN SIN ELITISMO
	for(int i=0;i<numeroPoblacion;i++)
		{
		//printf("Creando hijo %d\n", i);
		numPadre=progenitores[i*2];
		numMadre=progenitores[i*2+1];

		ajustePadre=poblacion[numPadre]->getAjuste();
		ajusteMadre=poblacion[numMadre]->getAjuste();
		
		//NOTA: Podría darse el caso de que un individuo generara él sólo un individuo sin la comprobación anterior(onanismo fértil!!!)
		for(int j=0;j<numParametros;j++)
			{
			centroidesHijo[j]=(poblacion[numMadre]->getCentroide(j)/ajusteMadre+poblacion[numPadre]->getCentroide(j)/ajustePadre)/(1/ajustePadre+1/ajusteMadre);
			amplitudesHijo[j]=(poblacion[numMadre]->getAmplitud(j)/ajusteMadre+poblacion[numPadre]->getAmplitud(j)/ajustePadre)/(1/ajustePadre+1/ajusteMadre);
			if(amplitudesHijo[j]<0)	amplitudesHijo[j]=0;	
			if(centroidesHijo[j]<limInferior)	centroidesHijo[j]=limInferior; //No dejamos ajustar centroides fuera del intervalo
			if(centroidesHijo[j]>limInferior+numeroCanales)	centroidesHijo[j]=limInferior+numeroCanales; //No dejamos ajustar centroides fuera del intervalo
			if(restringirCentroides && centroidesHijo[j]>centroidesIniciales[j]+0.1)	centroidesHijo[j]=centroidesIniciales[j]+0.1;
			if(restringirCentroides && centroidesHijo[j]<centroidesIniciales[j]-0.1)	centroidesHijo[j]=centroidesIniciales[j]-0.1;
			}
		sigmaHijo=(poblacion[numMadre]->getSigma()/ajusteMadre+poblacion[numPadre]->getSigma()/ajustePadre)/(1/ajustePadre+1/ajusteMadre);
		tauHijo=(poblacion[numMadre]->getTau()/ajusteMadre+poblacion[numPadre]->getTau()/ajustePadre)/(1/ajustePadre+1/ajusteMadre);
	
		//Condiciones para mantenerse en el ROI
		if(liSigma>0 && sigmaHijo<liSigma)		sigmaHijo=liSigma;
		else if(lsSigma>0 && sigmaHijo>lsSigma)	sigmaHijo=lsSigma;
		if(liTau>0 && tauHijo<liTau)			tauHijo=liTau;
		else if(lsTau>0 && tauHijo>lsTau)		tauHijo=lsTau;
		
		nextGeneration[i]->setCentroides(centroidesHijo);
		nextGeneration[i]->setAmplitudes(amplitudesHijo);
		nextGeneration[i]->setSigma(sigmaHijo);
		nextGeneration[i]->setTau(tauHijo);
		}
	*/
	for(int i=0;i<numeroPoblacion;i++)	nueva[i]		 =poblacion[i];		
	for(int i=0;i<numeroPoblacion;i++)	poblacion[i]	 =nextGeneration[i];		
	for(int i=0;i<numeroPoblacion;i++)	nextGeneration[i]=nueva[i];		
	generacion++;
	}

//*************** IMPRIME GENERACION ********************
//void GeneticAlgorithm2::imprimeGeneracion(LinkedList *l)
void GeneticAlgorithm2::imprimeGeneracion(Individuo **l)
	{
	if(l==NULL)	{printf("Lista nula\n"); return;}
	for(int i=0;i<numeroPoblacion;i++)
		{
		for(int j=0;j<numParametros;j++)	printf("c(%d):%f\t", j, l[i]->getCentroide(j));
		printf("\t");
		for(int j=0;j<numParametros;j++)	printf("a(%d):%f\t", j, l[i]->getAmplitud(j));
		printf("\t");
		printf("s:%f\tt:%f\n", l[i]->getSigma(), l[i]->getTau());
		}
	}
//********************* CALCULA AJUSTE **************
//Función similar a la de GAI pero contando con la exponencial
void GeneticAlgorithm2::calculaAjuste()
	{
/*	maxAjuste=0;
	int li=limInferior;
	float total=0.0;	
	float chi, errorCuadrado,c,a,g,sigma,t;
	//Para las restricciones de área
	float penalizacion=0;
	float area=0;
	int holgura=5;//Menos: sube el error, más: sube el error y el tiempo
	//
		
//	printf("CALCULA AJUSTE, Fx tiene tamaño %d\n", numeroCanales);
		
	for(int j=0;j<numeroCanales;j++)	total+=espectro->espectro[j+li-1]+espectro->sigmaContinuo[j+li-1]*espectro->sigmaContinuo[j+li-1];//Ni+sci²
	
	for(int i=0;i<numeroPoblacion;i++)//Para cada elemento de la población
		{
		chi=errorCuadrado=penalizacion=c=a=g=0;
		int cont=0;
		sigma=poblacion[i]->getSigma();
		t=poblacion[i]->getTau();
		int maximo=numeroCanales+(int)(holgura*sigma*2);//Tenemos en cuenta canales fuera del intervalo si el centroide está en los bordes
		float Fx[maximo];
		for(int j=0;j<maximo;j++)	Fx[j]=0;
			
		for(int k=0;k<poblacion[i]->getNumeroParametros();k++)//Para cada parámetro calculamos su convolución y la restamos
			{
			c=poblacion[i]->getCentroide(k);
			a=poblacion[i]->getAmplitud(k);
			area=0;
			int lia=(int)(li-holgura*sigma);//Límite Inferior Ancho
				
			if(sigma<0)		printf("ERROR: calculaAjuste: sigma < 0\n");
			int limInf=(int)(c-holgura*sigma);
			int limSup=(int)(c+holgura*sigma);
			if(limInf<lia)	limInf=(int)(lia);	
			if(limSup>lia+numeroCanales)	limSup=(int)(lia+numeroCanales);
			cont=(int)(c-lia);
		
	//		printf("li=%d, limInf=%d, limSup=%d, c=%.2f, cont=%d (c-limInf)=%d\n", li, limInf, limSup, c, cont, c-limInf);
			for(int j=(int)c;j>limInf;j--)//Recorremos para atrás hasta que se acabe la gaussiana
				{
				g=espectro->DaValorConvolucion(c, a, sigma, t, (double)j-1);
				area+=g;
			
				if(g<=0)	j=limInf;
				else		
					{
					if(cont<0)	{printf("ERROR, calculaAjuste(): cont < 0\n");	cont=0;}
					Fx[cont--]+=g;
					}
				}
			cont=(int)(c-lia)+1;
	
//			printf("[%d...|%d...>%.2f<...%d|...%d] (%d), cont=%d\n", lia, limInf, c, limSup, lia+maximo, maximo, cont);
				
			for(int j=(int)c+1;j<limSup;j++)//Recorremos para adelante hasta que se acabe la gaussiana
				{
				g=espectro->DaValorConvolucion(c, a, sigma, t, (double)j-1);
				area+=g;
				if(g==0)	j=limSup;	//terminamos, ya no va a haber más
				else		
					{
					if(cont>=numeroCanales)	printf("ERROR: calculaAjuste(): cont >= %d\n", numeroCanales);
					Fx[cont++]+=g;
					}
				}
			//float areaSuma=area;
			area=a*t;//NUEVO: Comparación con la fórmula en vez de con la suma de cosas	
			//printf("AreaSuma=%.2f y area=%.2f\n", areaSuma, area);				
			if(restricciones[k]!=0 && area>restricciones[k])	
				{
				penalizacion+=area-restricciones[k];
			//	printf("%d se pasa de la restricción de %d a %.2f a %.2f, penalización=%.2f (altura=%.2f)\n", i,k,restricciones[k], area, area-restricciones[k], a);
				}
			//if(k==0)	printf("area=%f para el elemento %d (rest. %f)\n", area, i, restricciones[k]);
			}//Para cada parámetro
		
		for(int j=0;j<numeroCanales;j++)
			{
			errorCuadrado+=(y[j]-Fx[j])*(y[j]-Fx[j]);
			Fx[j]=0;
			}
		chi=(errorCuadrado/total);
		//NOTA: Probando a suavizar un poco la penalización
		penalizacion=penalizacion/2;
		//--
		printf("chi+penalización=%.2f+%.2f (a2=%.2f)\n", chi, penalizacion, a);
		poblacion[i]->setAjuste(chi+penalizacion);
		//printf("%d: penalización=%f y chi=%f\n", i, penalizacion, chi);
		//printf("%d tiene ajuste %.2f+%.2f=%.2f. ALTURA=%.2f, TAU=%.2f, SIGMA=%.2f\n", i,chi,penalizacion,(chi+penalizacion), a,t,c);
		maxAjuste+=chi;
		}
		//printf("\n");
//		printf("FIN CALCULA AJUSTE\n");
	*/	
	maxAjuste=0;
	int li=limInferior;
	float total=0.0;	
	float Fx[numeroCanales];
	float chi, errorCuadrado,c,a,g,sigma,t;
	//Para las restricciones de área
	float penalizacion=0;
	float area=0;
	//
		
//	printf("CALCULA AJUSTE, Fx tiene tamaño %d\n", numeroCanales);
		
	for(int j=0;j<numeroCanales;j++)	Fx[j]=0;
	for(int j=0;j<numeroCanales;j++)	total+=espectro->espectro[j+li-1]+espectro->sigmaContinuo[j+li-1]*espectro->sigmaContinuo[j+li-1];//Ni+sci²
	
	for(int i=0;i<numeroPoblacion;i++)//Para cada elemento de la población
		{
		chi=errorCuadrado=penalizacion=c=a=g=0;
		int cont=0;
		sigma=poblacion[i]->getSigma();
		t=poblacion[i]->getTau();
		for(int k=0;k<poblacion[i]->getNumeroParametros();k++)//Para cada parámetro calculamos su convolución y la restamos
			{
			c=poblacion[i]->getCentroide(k);
			a=poblacion[i]->getAmplitud(k);
			area=0;
			int holgura=5;//Menos: sube el error, más: sube el error y el tiempo
			if(sigma<0)		printf("ERROR: calculaAjuste: sigma < 0\n");
			int limInf=(int)(c-holgura*sigma);
			int limSup=(int)(c+holgura*sigma);
			if(limInf<li)	limInf=li;
			if(limSup>li+numeroCanales)	limSup=li+numeroCanales;
			cont=(int)(c-li);
		
			if(c-li>numeroCanales-1)	cont=numeroCanales-1;
			for(int j=(int)c;j>limInf;j--)//Recorremos para atrás hasta que se acabe la gaussiana
				{
				//printf("c: %f, a:%f, s:%f, t:%f\n", c, a, sigma, t);
				g=espectro->DaValorConvolucion(c, a, sigma, t, (double)j-1);
				area+=g;
			
				if(g<=0)	j=limInf;
				else		
					{
					if(cont<0)	{printf("ERROR, calculaAjuste(): cont < 0\n");	cont=0;}
					Fx[cont--]+=g;
					}
				}
			cont=(int)(c-li)+1;
	
			for(int j=(int)c+1;j<limSup;j++)//Recorremos para adelante hasta que se acabe la gaussiana
				{
				g=espectro->DaValorConvolucion(c, a, sigma, t, (double)j-1);
				area+=g;
				if(g==0)	j=limSup;	//terminamos, ya no va a haber más
				else		
					{
					if(cont>=numeroCanales)	printf("ERROR: calculaAjuste(): con >= %d\n", numeroCanales);
					Fx[cont++]+=g;
					}
				}
			//area=a*t;//NUEVO: Comparación con la fórmula en vez de con la suma de cosas	<- Ojo, tau varía mucho
			//printf("AreaSuma=%.2f y area=%.2f\n", areaSuma, area);				
			if(restricciones[k]>0 && area>restricciones[k])	
				{
				penalizacion+=area-restricciones[k];
			//	printf("%d se pasa de la restricción de %d (%.2f) subiendo hasta %.2f, penalización=%.2f (altura=%.2f)\n", i,k,restricciones[k], area, area-restricciones[k], a);
				}
			//NOTA: Parece útil añadir la penalización también por debajo
			if(restricciones[k]>0 && area<restricciones[k]/1.2)	
				{
				penalizacion+=restricciones[k]-area;
				}
			//if(k==0)	printf("area=%f para el elemento %d (rest. %f)\n", area, i, restricciones[k]);
			}//Para cada parámetro
		
		for(int j=0;j<numeroCanales;j++)
			{
			errorCuadrado+=(y[j]-Fx[j])*(y[j]-Fx[j]);
			Fx[j]=0;
			}
		chi=(errorCuadrado/total);
		//NOTA: Probando a suavizar un poco la penalización
		//penalizacion=penalizacion/2;
		//--
		poblacion[i]->setAjuste(chi+penalizacion);
		//printf("%d: penalización=%f y chi=%f\n", i, penalizacion, chi);
//		printf("%d tiene ajuste %.2f+%.2f=%.2f. ALTURA=%.2f, TAU=%.2f, SIGMA=%.2f\n", i,chi,penalizacion,(chi+penalizacion), a,t,c);
//		printf("%f\n", poblacion[i]->getAjuste());
		/*printf("%.2f (%.2f+%.2f)-> ", chi+penalizacion, chi, penalizacion);
		for(int j=0;j<numParametros;j++)
			{
			printf("%.2f ", poblacion[i]->getAmplitud(j)*poblacion[i]->getTau());
			}
		printf("\n");*/
		maxAjuste+=chi;
		}
		//printf("\n");
//		printf("FIN CALCULA AJUSTE\n");
	}
	
//********* EJECUTAR ************************
/* Realiza el siguiente proceso:
	1) Calcula los ajustes
	1b) Ordena los ajustes
	2) Procrea y recalcula los ajustes
	3) Muta y recalcula los ajustes
   Mientras el error calculado sea mayor que minError y el número de generaciones
	no haya llegado al máximo estipulado.
*/

void GeneticAlgorithm2::ejecutar(int maxGeneraciones, float minError)
	{
	double t1, t2;
//	printf("Ejecutar\n");
	for(int i=0;i<maxGeneraciones;i++)
		{
		//t1=clock();
		calculaAjuste();
					
/*		printf("Alturas: ");
		for(int k=0;k<numParametros;k++)	printf("%.2f ", poblacion[0]->getAmplitud(k));
		printf("\n");*/
		
		if(poblacion[0]->getAjuste()<=minError)	
			{
			cout << "Se ha minimizado el error: " << poblacion[0]->getAjuste() << endl;
			return;
			}
		
		//t1=clock();
		procrear();
		//printf("Tiempo tardado en procrear: %f\n", 2.0*(clock()-t1)/CLOCKS_PER_SEC);
		//t1=clock();
		mutar();
		//printf("Tiempo tardado en mutar: %f\n", 2.0*(clock()-t1)/CLOCKS_PER_SEC);
		}
	//cout << "Se ha llegado al máximo de generaciones, el ajuste logrado es " << adan->getAjuste() << endl;		 
	}


//********************* GET FUNCIÓN **************
//Función similar a la de GAI pero contando con la exponencial
float *GeneticAlgorithm2::getFuncion(int k)
	{
	float *funcion=new float[numeroCanales];
	if(funcion==NULL)	
		{
		printf("ERROR: No hay memoria para reservar float[%d]\n", numeroCanales);
		return NULL;
		}		
	int li=limInferior;
		
	float sigma=poblacion[0]->getSigma();
	float tau=poblacion[0]->getTau();
	float c=poblacion[0]->getCentroide(k);
	float a=poblacion[0]->getAmplitud(k);

//	printf("c: %f, a:%f, s:%f, t:%f\n", c, a, sigma, tau);
	for(int i=0;i<numeroCanales;i++)
		funcion[i]=espectro->DaValorConvolucion(c, a, sigma, tau,(double)(i+li) );
	return funcion;
	}
	
	
//************* GET MEJOR AJUSTE ***************	
float GeneticAlgorithm2::getMejorAjuste()
	{
//	printf("%f\n", poblacion[0]->getAjuste());
	ordenar();
	calculaAjuste();
//	printf("%f\n", poblacion[0]->getAjuste());
//	printf("%f\n", poblacion[0]->getAjuste());
	return poblacion[0]->getAjuste();
	}

//************** GET AJUSTE MEDIO ****************
float GeneticAlgorithm2::getAjusteMedio()
	{
	calculaAjuste();
	float media;
	for(int i=0;i<numParametros;i++)
		media+=poblacion[i]->getAjuste();
	media/=numParametros;
	return media;
	}
	
//****************** GET PARAMETROS ***********************
/*	Nos da los parámetros del primer elemento de la población
	Si queremos al mejor individuo, tendremos que llamar antes a calculaAjuste
	para que nos los ordene.
	*/
void GeneticAlgorithm2::getParametros(float *parametros)
	{
//	printf("GET PARAMETROS\n");
	//float *parametros=new float[numParametros*2+2];
	if(parametros==NULL)	
		{
		printf("ERROR: No hay memoria en el argumento\n");
		return;
		}		
	
	for(int i=0;i<numParametros;i++)	
		{
		parametros[i]  = poblacion[0]->getCentroide(i);
	//	printf("c[%d]=%.2f\n", i, poblacion[0]->getCentroide(i));
		}
	for(int i=0;i<numParametros;i++)	
		{
		parametros[i+numParametros]  = poblacion[0]->getAmplitud(i);
	//	printf("a[%d]=%.2f\n", i, poblacion[0]->getAmplitud(i));
		}
	parametros[2*numParametros]=poblacion[0]->getSigma();
//	printf("Sigma=%.2f\n", poblacion[0]->getSigma());
	parametros[2*numParametros+1]=poblacion[0]->getTau();
//	printf("Tau=%.2f\n", poblacion[0]->getTau());
//	printf("Fin de getParametros\n");
//	for(int j=0;j<numeroPoblacion;j++)	printf("%.2f(%.2f) ", poblacion[j]->getAmplitud(2), poblacion[j]->getAjuste());
	}
