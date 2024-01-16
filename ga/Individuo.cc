#include "Individuo.h"

//**************************************
//*************** CONSTRUCTORES ********
//**************************************

//*************** INDIVIDUO ************
// Constructor por defecto. 
// Da el tamaño predeterminado de 17 a los vectores de parámetros
Individuo::Individuo()
	{
	sigma=0;
	numeroParametros=PARAMS;
	centroide=new float[PARAMS];
	amplitud=new float[PARAMS];
	for(int i=0;i<PARAMS;i++)
		{//NOTA: Probablemente convenga darles límites a los valores aleatorios.
		centroide[i]= random();
		amplitud[i] = random();
		}
	//ajuste=random();
	}
	
//*************** INDIVIDUO ************
// Permite especificar el tamaño y dar valores a todos sus parámetros
Individuo::Individuo(int tam, float *Ncentroide, float *Namplitud, float Nsigma)
	{
	sigma=Nsigma;
	numeroParametros=tam;
	centroide=new float[tam];
	amplitud=new float[tam];
	for(int i=0;i<tam;i++)
		{
		centroide[i]=Ncentroide[i];
		amplitud[i]=Namplitud[i];
		}
	}

Individuo::Individuo(int tam)
	{
	sigma=0;
	numeroParametros=tam;
	centroide=new float[tam];
	amplitud =new float[tam];
	for(int i=0;i<tam;i++)
		{//NOTA: Probablemente convenga darles límites a los valores aleatorios.
		centroide[i]= random();
		amplitud[i] = random();
		}
	}

Individuo::Individuo(int tam, float *Ncentroide, float *Namplitud, float Nsigma, float Ntau)
	{
	sigma=Nsigma;
	tau=Ntau;
	numeroParametros=tam;
	centroide=new float[tam];
	amplitud=new float[tam];
	for(int i=0;i<tam;i++)
		{
		centroide[i]=Ncentroide[i];
		amplitud[i]=Namplitud[i];
		}
	}
	
Individuo::~Individuo()
	{
	if(centroide != NULL)	delete [] centroide;
	if(amplitud != NULL)	delete [] amplitud;
	}
	
void Individuo::liberarMemoria()
	{
	//printf("Borramos centroides\n");
	if(centroide != NULL)	delete [] centroide;
	//printf("Borramos amplitudes\n");
	if(amplitud != NULL)	delete [] amplitud;	
	}

//**************************************
//***** OBTENCIÓN DE PARÁMETROS ********
//**************************************
//************** GET CENTROIDES ******************
float *Individuo::getCentroides()
	{
	return centroide;
	}
	
//************** GET CENTROIDE ******************
float Individuo::getCentroide(int pos)
	{
	return centroide[pos];
	}
	
//************** GET AMPLITUDES ******************
float *Individuo::getAmplitudes()
	{
	return amplitud;	
	}
	
//************** GET AMPLITUD ******************
float Individuo::getAmplitud(int pos)
	{
	return amplitud[pos];	
	}
	
//************** GET SIGMA ******************
float Individuo::getSigma()
	{
	return sigma;	
	}

//************** GET AJUSTE ******************
float Individuo::getAjuste()
	{
	return ajuste;	
	}

//************** GET NUMERO PARAMETROS ******************
int Individuo::getNumeroParametros()
	{
	return numeroParametros;	
	}

	
//**************************************
//***** ESCRITURA DE PARÁMETROS ********
//**************************************

//************** SET AJUSTE ******************
void Individuo::setAjuste(float Najuste)
	{
	ajuste=Najuste;	
	}
	
//************** SET CENTROIDE *****************
void Individuo::setCentroide(int pos, float valor)
	{
	centroide[pos]=valor;	
	}

//************** SET CENTROIDES *****************
void Individuo::setCentroides(float *c)
	{
	//delete [] centroide;
	//centroide = new float[numeroParametros];
	//centroide=c;
	if(centroide == NULL)	centroide=new float[numeroParametros];
	for(int i=0;i<numeroParametros;i++)		centroide[i]=c[i];	
	}


//************** SET AMPLITUD *****************
void Individuo::setAmplitud(int pos, float valor)
	{
	amplitud[pos]=valor;	
	}

//************** SET AMPLITUDES *****************
void Individuo::setAmplitudes(float *a)
	{
	//amplitud=a;
	if(amplitud == NULL)	amplitud=new float[numeroParametros];
	for(int i=0;i<numeroParametros;i++)		amplitud[i]=a[i];	
	}

//************** SET SIGMA *****************
void Individuo::setSigma(float valor)
	{
	sigma=valor;	
	}
	
//**************** SET NUMERO PARAMETROS ***************
void Individuo::setNumeroParametros(int valor)
	{
	numeroParametros=valor;	
	}

//Funciones de obtención de parámetros
float Individuo::getTau()
	{
	return tau;	
	}

//Funciones de escritura de parámetros
void Individuo::setTau(float t)
	{
	tau=t;	
	}
