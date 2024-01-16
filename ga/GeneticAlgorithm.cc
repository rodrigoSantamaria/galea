
#include "GeneticAlgorithm.h"

//**************** GENETIC ALGORITHM ***************
// Constructor por defecto
GeneticAlgorithm::GeneticAlgorithm()
	{
	generacion      =0;
	numeroPoblacion	=0;
	numeroPadres	=0;
	numeroMutaciones=0;
	factorMutante	=0.0;
	}
	
//**************** GENETIC ALGORITHM ***************
// Constructor aleatorio. Inicializa los parámetros de configuración
// y genera una población aleatoria de individuos
GeneticAlgorithm::GeneticAlgorithm(int NnumeroPoblacion,  int NnumeroPadres,
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

	/*nueva=new LinkedList();
	nextGeneration=new LinkedList();
	poblacion	= new LinkedList();*/
	/*nueva=new Individuo[numeroPoblacion];
	nextGeneration=new Individuo[numeroPoblacion];
	poblacion=new Individuo[numeroPoblacion];
	*/
	nueva=new Individuo*[numeroPoblacion];
	nextGeneration=new Individuo*[numeroPoblacion];
	poblacion=new Individuo*[numeroPoblacion];
		
	for(int i=0;i<numeroPoblacion;i++)
		{
		poblacion[i]->setSigma( 0.5 + ((float)(random()%450)/100.0) );//varía en [0.5,5]
		}
	}


//******************** DESTRUCTOR ***************************
/*	Destructor por defecto.
	*/
GeneticAlgorithm::~GeneticAlgorithm()
	{
	//printf("Borramos poblaciones\n");
	for(int i=0;i<numeroPoblacion;i++)
		{
	//	printf("borramos población %d\n", i);
		if(poblacion[i]!=NULL)			delete poblacion[i];
	//	printf("nulificamos nueva %d\n", i);
		nueva[i]=NULL;
	//	printf("borramos nextGen %d\n", i);
		if(nextGeneration[i]!=NULL)		delete nextGeneration[i];
		}
	//printf("Borramos nueva\n");
	delete [] nueva;
	//printf("Borramos nextGen\n");
	delete [] nextGeneration;
	//printf("Borramos poblacion\n");
	delete [] poblacion;
	//printf("Borramos y\n");
	y=NULL;
	//printf("Nulificamos espectro\n");
	espectro=NULL;
	//printf("Terminamos borra GA\n");
	}

//**************************************************************
//******* MÉTODOS DE OBTENCIÓN DE PARÁMETROS *******************
//**************************************************************

//**************** GET POBLACIÓN ******************
Individuo **GeneticAlgorithm::getPoblacion()
	{
	return poblacion;
	}

//************ GET NUMERO POBLACIÓN *******************
int GeneticAlgorithm::getNumeroPoblacion()
	{
	return numeroPoblacion;
	}

//************ GET NUMERO MUTACIONES *******************
int GeneticAlgorithm::getNumeroMutaciones()
	{
	return numeroMutaciones;
	}

//************ GET FACTOR MUTANTE *******************
float GeneticAlgorithm::getFactorMutante()
	{
	return factorMutante;
	}
	
//************ GET Y *********************
float *GeneticAlgorithm::getY()
	{
	return y;	
	}

//************ GET Yi *********************
float GeneticAlgorithm::getY(int i)
	{
	return y[i];	
	}

//************ GET NUMERO CANALES *********************
int GeneticAlgorithm::getNumeroCanales()
	{
	return numeroCanales;	
	}

//************ GET NUMERO PARAMETROS *********************
int GeneticAlgorithm::getNumParametros()
	{
	return numParametros;	
	}
	
//************* GET PARAMETROS *********************
float *GeneticAlgorithm::getParametros()
	{
	float *parametros=new float[numParametros*2+1];
	for(int i=0;i<numParametros;i++)	parametros[i]  = poblacion[0]->getCentroide(i);
	for(int i=0;i<numParametros;i++)	parametros[i+numParametros]  = poblacion[0]->getAmplitud(i);
	parametros[2*numParametros]=poblacion[0]->getSigma();
	
	return parametros;
	}
	
//************ GET ESPECTRO *********************
cEspectros *GeneticAlgorithm::getEspectro()
	{
	return espectro;	
	}
	
//************ GET LIM INFERIOR *********************
int GeneticAlgorithm::getLimInferior()
	{
	return limInferior;	
	}
	
		
	
//**************************************************
//***** MÉTODOS DE CONFIGURACIÓN DE PARÁMETROS *****
//**************************************************
//****************** SET NUMERO POBLACIÓN *****************
void GeneticAlgorithm::setNumeroPoblacion(int NnumeroPoblacion)
	{
	numeroPoblacion=NnumeroPoblacion;
	}

//****************** SET NUMERO PADRES *****************
void GeneticAlgorithm::setNumeroPadres(int NnumeroPadres)
	{
	numeroPadres=NnumeroPadres;
	}

//****************** SET NUMERO MUTACIONES *****************
void GeneticAlgorithm::setNumeroMutaciones(int NnumeroMutaciones)
	{
	numeroMutaciones=NnumeroMutaciones;
	}

//****************** SET FACTOR MUTANTE *****************
void GeneticAlgorithm::setFactorMutante(float NfactorMutante)
	{
	factorMutante=NfactorMutante;
	}
	
//******************** SET Y ********************
void GeneticAlgorithm::setY(float *Ny)
	{
	y=Ny;	
	}

//******************** SET Yi *******************
void GeneticAlgorithm::setY(float Ny, int i)
	{
	y[i]=Ny;	
	}
	
//************** SET POBLACIÓN **********************
/*	Función polimórfica con la anterior, que establece también las 
	amplitudes.
	A cada individuo se le darán esos valores de centroides variando en 
	un intervalo de +-1; con una probabilidad uniforme de que ocurra
	cualquiera de los valores.
	El elegir del intervalo de +-1 es debido a que la propia resolución
	de nuestro detector (y por tanto la forma en que se establecen los centroides)
	es de 1keV.
	En cuanto a las amplitudes, tendrán un valor variable entre 0 y 1.5*Yl, siendo
	l el entero más cercano a ese centroide.
	*/
void GeneticAlgorithm::setPoblacion(float *centroides)
	{
	int li=limInferior;
	float aleatorio=0;

	for(int i=0;i<numeroPoblacion;i++)
		{
		for(int j=0;j<poblacion[i]->getNumeroParametros();j++)
			{
			aleatorio=-1 + 2.0*(float)((random()%100))/100.0;//Valor entre -1 y 1
			float centroideDesplazado=centroides[j]+aleatorio;
			poblacion[i]->setCentroide(j, centroideDesplazado);

			aleatorio=(float)((random()%150))/100.0;//Probar a poner la altura como area/tau
			float amplitudDesplazada=aleatorio*y[(int)(centroideDesplazado-li)];
			poblacion[i]->setAmplitud(j, amplitudDesplazada);
			}
		}
	}
	

//********************************************
//*********** MÉTODOS DE IMPRESIÓN ***********
//********************************************
		
//*********** PRINT POBLACION***********
void GeneticAlgorithm::printPoblacion()
	{
	for(int i=0;i<numeroPoblacion;i++)
		{
		Individuo *individuo=poblacion[i];
		cout << "individuo " << i << " (s=" << individuo->getSigma() << "):" << endl;
		cout << "\tcentroides: ";
		for(int j=0;j<numParametros;j++)	cout << individuo->getCentroide(j) << " ";
		cout << endl << "\tamplitudes: ";
		for(int j=0;j<numParametros;j++)	cout << individuo->getAmplitud(j) << " ";
		cout << endl;
		}
	}

//*********** PRINT AJUSTES ***********
void GeneticAlgorithm::printAjustes()
	{
	for(int i=0;i<numeroPoblacion;i++)
		{
		Individuo *individuo=poblacion[i];
		cout << "individuo " << i << ": " << individuo->getAjuste() << endl;
		}
	}

//*************** GET FUNCION ***************
//Devuelve el vector con los valores de la gaussiana calculada para el 
//centroide k-ésimo para el mejor elemento de la población
float *GeneticAlgorithm::getFuncion(int k)
	{
	Individuo *adan=poblacion[0];
	float *funcion=new float[numeroCanales];
	int li=limInferior;
		
	float sigma=adan->getSigma();
	float c=adan->getCentroide(k);
	float a=adan->getAmplitud(k);
	for(int i=0;i<numeroCanales;i++)
		funcion[i]=espectro->DaValorGaussiana(c, a, sigma, (double)(i+li) );
	return funcion;
	}
