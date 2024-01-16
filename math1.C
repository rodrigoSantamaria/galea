#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "math1.h"

#define MAXIMO_ORDEN 20

//*************** GCF ***********************

double gcf(double a, double x) 
	{
   	double gln, an, b, c, d, del, h;
   
   	gln = gamma(a);
   	b = x + 1.0 - a;
   	c = 1.0 / FPMIN;
   	d = 1.0 / b;
   	h = d;
   	for (unsigned short i = 1; i <= ITMAX; i++) 
	   	{
      	an = -i*(i-a);
      	b += 2.0;
      	d = an*d + b;
      	if (fabs(d) < FPMIN) d = FPMIN;
      	c = b + an/c;
      	if (fabs(c) < FPMIN) c = FPMIN;
      	d = 1.0 / d;
      	del = d*c;
      	h *= del;
    	if (fabs(del-1.0) < EPS) break;
   		}
   	return exp(-x+a*log(x)-gln)*h;
	}

//********************* GSER ****************
double gser(double a, double x) 
	{
   	double gln, ap, del, sum;
   
   	gln = gamma(a);
   	if (x <= 0.0) return 0.0;
   	ap = a;
   	sum = 1.0 / a;
   	del = sum;
   	for (unsigned short i = 1; i <= ITMAX; i++) 
		{
	  	ap += 1.0;
    	del *= x/ap;
      	sum += del;
      	if (fabs(del) < fabs(sum)*EPS) break;
   		}
	return sum*exp(-x+a*log(x)-gln);
	}

//************** GAMMQ ****************
//calcula de alguna forma una función gamma para la calidad Q de la recta

double gammq(double a, double x) 
	{
   	if ((x < 0.0)||(a <= 0.0)) return 0.0;
   	if (x < a+1.0) return 1.0 - gser(a, x);
   	else return gcf(a, x);
	}

//**************** GAMMP ***************
//Muy similar a gammq

double gammp(double a, double x) 
	{
   	if ((x < 0.0)||(a <= 0.0)) return 0.0;
   	if (x < a + 1.0) return gser(a, x);
   	else return 1.0 - gcf(a, x);
	}

double log0(double x) 
	{
	if (x >= 1) return log(x);
	else return 0.0;
	}

double log010(double x) 
	{
	if (x >= 1) return log10(x);
	else return 0.0;
	}

//****************** TRASPUESTA ************
/*	Calcula y devuelve la matriz traspuesta de matriz[filas][columnas]
	Es responsabilidad del código que llame a esta función la liberación de
	memoria tanto de la matriz original como de la transpuesta
	*/
/*double **traspuesta(double **matriz, int filas, int columnas)
	{
	double **traspuesta;
	printf("MATRIZ TRANSPUESTA\n");
	int columnasT=filas;
	int filasT=columnas;
//		printf("reservamos memoria\n");
	traspuesta=new double*[filasT];
	printf("Reservada la memoria para las %d filas\n", filasT);
	if(traspuesta==NULL)	{printf("ERROR: No se ha podido reservar memoria\n");return NULL;}
	for(int i=0;i<filasT;i++)	
		{
		printf("Reservando memoria para las %d columnas de la fila %d\n",columnasT, i);
		traspuesta[i]=new double[columnasT];
		if(traspuesta[i]==NULL)	{printf("ERROR: No se ha podido reservar memoria\n");return NULL;}
		}
	printf("Reservada la memoria para las columnas, haciendo cálculos\n");
	for(int i=0;i<filasT;i++)
		for(int j=0;j<columnasT;j++)	
			{
//			printf("At[%d][%d]=A[%d][%d]\n", i,j,j,i);
			traspuesta[i][j]=matriz[j][i];	
			}
	printf("Terminada la transposición, devolviendo valor\n");
	return traspuesta;
	}*/
void traspuesta(double **matriz, int filas, int columnas, double **traspuesta)
	{
	printf("MATRIZ TRANSPUESTA\n");
	int columnasT=filas;
	int filasT=columnas;
//		printf("reservamos memoria\n");
	for(int i=0;i<filasT;i++)
		for(int j=0;j<columnasT;j++)	
			{
//			printf("At[%d][%d]=A[%d][%d]\n", i,j,j,i);
			traspuesta[i][j]=matriz[j][i];	
			}
	printf("Terminada la transposición\n");
	return;
	}

//***************** MULTIPLICA *******************
/*	Realiza la multiplicación de dos matrizes m1 x m2 (en ese orden, ojo)
	Es responsabilidad del código invocador liberar m1, m2 y la matriz resultado
	m1[f1][c1] x m2[f2][c2] = r[f1][c2] (c1=f2)
	*/
double **multiplica(double **m1, int filas1, int columnas1, double **m2, int filas2, int columnas2)
	{
	printf("MULTIPLICACIóN DE MATRICES\n");
	//0) Comprobamos dimensionalidad
	if(	columnas1 != filas2 )	{ printf("ERROR: El número de columnas de la primera matriz (%d) no coincide con el número de filas de la segunda (%d). Devolviendo NULL\n", columnas1, filas2); return NULL;}
	
	//1) Reservamos memoria e iniciamos la matriz de resultados
	double **resultado;
	resultado=new double*[filas1];
	for(int i=0;i<filas1;i++)	resultado[i]=new double[columnas2];
	for(int i=0;i<filas1;i++)	for(int j=0;j<columnas2;j++)	resultado[i][j]=0.0;
	printf("%dx%d * %dx%d ---> %dx%d\n", filas1, columnas1, filas2, columnas2, filas1, columnas2);
	//2) Realizamos la multiplicación.
	for(int i=0;i<filas1;i++)
		{
		//printf("%d\n", i);
		for(int j=0;j<columnas2;j++)
			{
			//printf("%d,%d\n ", i, j);
			for(int k=0;k<columnas1;k++)
				{
		//		printf("m1\n");
		//		resultado[i][j]=m1[i][k];
		//		printf("m2\n");
		//		resultado[i][j]*=m2[k][j];
				resultado[i][j]+=m1[i][k]*m2[k][j];
				}
			//printf(" = %.2f\n", resultado[i][j]);
			}
		}
	printf("termina multiplicación\n");
	return resultado;
	}
	
//****************** ORDEN *****************
/*	Devuelve el número de veces que el número se puede
	dividir/multiplicar por 10 antes de que lleguemos a menos de 1/ más de 1.
	Vamos, el orden que se entiende matemáticamente.
	110->2
	23->1
	1->0
	0.234->
	*/
int orden(double x) 
	{
//	printf("Buscamos el orden de X=%f\n", x);
	
/*	if (x <= 0.0) 
		{
			printf("X es negativo, vale %f, mandamos un -999\n", x);
		return -999;
		}
		*/
	//NOTA: probando, lo de antes rayaba para ordenes de 1e-(89999) o parecidos
	if (x < 0.0) 
		{
		//	printf("X es negativo, vale %f, mandamos un -999\n", x);
		return -999;
		}
	else if(x==0)	return 0;
		
	int i = 0;
	bool flag = false;
	if (x >= 1.0) 
		{
	//	printf("X es mayor o igual que 1\n");
		while ((i < MAXIMO_ORDEN) && (!flag)) 
			{
			double temp = x / powi(10.0, i);
		//	printf("temp es %f, i es %d\n", temp, i);
			if (temp < 10.0)	flag = true;
			else 				i++;
			}
		}
	else 
		{
		//printf("X es menor que 1\n");
		while ((i < MAXIMO_ORDEN) && (!flag)) 
			{
			double temp = x * powi(10.0, i);
			if (temp < 1.0)		i++;
			else				flag = true;
			}
		i = -i;
		}
	//printf("El ORDEN es %d, y el flag %d\n", i, flag);
//	printf("devolvemos %d\n", i);
	if (flag) return i;
	//else return -999;
	//NOTA: Si el orden es menor que 10¯²°, devolvemos un valor que lo indique
	else 
		{
		if(x>=1.0)	return -999;
		else		return -666;
		}
	}

int inicial(double x) 
	{
	int numero = orden(x);
	if (numero != -999) return (int)floor(x/powi(10.0, numero));
	else return -1;
	}

double maxim(double x, double y) 
	{
	return (x > y) ? x : y;
	}

double signo(double x) 
	{
	return (x < 0.0) ? -1.0 : 1.0;
	}

//********************* REDONDEA ********************
// Convierte un valor real al entero más cercano.
int redondea(double x) 
	{
	double base = floor(x);
	double resto = x - base;
	if (x >= 0.0)	return (resto > 0.5)  ? (int) base + 1 : (int) base;
	else			return (resto >= 0.5) ? (int) base + 1 : (int) base;
	}

//***************** F REDONDEA **************
/* Redondea un valor double a un entero, pero lo 
	almacena en una variable double
	*/
double fredondea(double x) 
	{
	double base = floor(x);
	double resto = x - base;
	if (x >= 0.0)
		return (resto > 0.5) ? base + 1.0 : base;
	else
		return (resto >= 0.5) ? base + 1.0 : base;
	}

//************ REDONDEA DOUBLE ***************
/* Redondea las cifras decimales significativas de un
	double según el siguiente criterio:
	-Si la primera cifra significativa es un 1, redondeamos
	a numCS cifras significativas. Si es otro número, redondeamos
	a (numCS-1) cifras.
	Las cifras que quedan a la derecha de la última cifra significativa
	deberán desaparecer, por ello tendremos que introducir el valor redondeado
	en una cadena en vez de en un double normal
		x - valor a redondear
		numCs - número de Cifras Significativas que queremos contemplar
		xRedondeado - cadena en la que se introducirá el número redondeado
	*/
void redondeaDouble(double x, int numCS, char *xRedondeado)
	{	//x=X.0000YZQ
	double base  = floor(x); // X
	double resto = x - base; // 0.0000YZQ
	bool   fin	 = false;
	int    numCifras = 0;
	double fraccion  = 0;
	double restoFraccion = 0;
	double enteroResto = 0;
//	printf("VALOR A REDONDEAR: %f, base %f, resto %f\n", x, base, resto);
	//Para el resto, vamos a quedarnos sólo con las cifras significativas:
//	while(!fin)
	int i=0;
	while(i<10 && fin==false)
		{
		resto=10*resto; //0.000YZQ, 0.00YZQ, 0.0YZQ, 0.YZQ, Y.ZQ
	//	printf("resto=%f\n", resto);
		enteroResto=floor(resto);
	//	printf("enteroResto=%f\n", enteroResto);
		if(enteroResto>0)//Ya sabemos cuál es el primer caracter
			{
			numCifras++;
			fraccion += floor(enteroResto)*pow(10, -i-1);
	//		printf("La primera cifra significativa es un %f\n", enteroResto);
			resto=resto-enteroResto;
			fin=true;
			}
		i++;
		}
	//Ya sabemos hasta qué número de cifras decimales tenemos que coger:
	int sumar=0;
	if(enteroResto==1)	sumar=numCS;
	else				sumar=numCS-1;
//	printf("Tenemos que sumar otras %d cifras\n", sumar);
	fin=false;
	int j=0;
	while(j<sumar)
		{
		resto=10*resto; //0.000YZQ, 0.00YZQ, 0.0YZQ, 0.YZQ, Y.ZQ
		enteroResto=floor(resto);
		resto=resto-enteroResto;
		if(j==sumar-1) //Si es la última cifra redondeamos
			{//Cogemos la siguiente cifra para ver si hay que redondear
			resto=10*resto; //0.000YZQ, 0.00YZQ, 0.0YZQ, 0.YZQ, Y.ZQ
//			printf("Última cifra, mirando si hay que redondear\n");
			double enteroRestoSiguiente=floor(resto);
//			printf("La siguiente es %f\n", enteroRestoSiguiente);
			if(enteroRestoSiguiente>4)	enteroResto++;
			}
//		printf("La siguiente cifra a coger es %f\n", enteroResto);
//		printf("Luego le sumamos: %f\n", enteroResto*pow(10, -i-j-1));
		fraccion +=enteroResto*pow(10, -i-j-1);
//		printf("Fracción=%f\n", fraccion);
		j++;
		}		
		
	double num=base+fraccion;
	switch(i+j)
		{
		case 1:
			sprintf(xRedondeado, "%.1f", num);
			break;		
		case 2:
			sprintf(xRedondeado, "%.2f", num);
			break;		
		case 3:
			sprintf(xRedondeado, "%.3f", num);
			break;		
		case 4:
			sprintf(xRedondeado, "%.4f", num);
			break;		
		case 5:
			sprintf(xRedondeado, "%.5f", num);
			break;		
		case 6:
			sprintf(xRedondeado, "%.6f", num);
			break;		
		case 7:
			sprintf(xRedondeado, "%.7f", num);
			break;		
		case 8:
			sprintf(xRedondeado, "%.8f", num);
			break;		
		case 9:
			sprintf(xRedondeado, "%.9f", num);
			break;		
		case 10:
			sprintf(xRedondeado, "%.10f", num);
			break;		
		default:
			sprintf(xRedondeado, "%.6f", num);
			break;		
		}
//	printf("Devolvemos %s\n", xRedondeado);
//	return base+fraccion;
	}

//******** REDONDEA DOUBLE
/* Redondea un valor double a un determinado número de cifras,
	y lo devuelve*/
double redondeaDoubleF(double x, int numCS)
	{	//x=X.0000YZQ
	double base  = floor(x); // X
	double resto = x - base; // 0.0000YZQ
	bool   fin	 = false;
	int    numCifras = 0;
	double fraccion  = 0;
	double restoFraccion = 0;
	double enteroResto = 0;
	char xRedondeado[100];
//	printf("VALOR A REDONDEAR: %f, base %f, resto %f\n", x, base, resto);
	//Para el resto, vamos a quedarnos sólo con las cifras significativas:
//	while(!fin)
	int i=0;
	while(i<10 && fin==false)
		{
		resto=10*resto; //0.000YZQ, 0.00YZQ, 0.0YZQ, 0.YZQ, Y.ZQ
	//	printf("resto=%f\n", resto);
		enteroResto=floor(resto);
	//	printf("enteroResto=%f\n", enteroResto);
		if(enteroResto>0)//Ya sabemos cuál es el primer caracter
			{
			numCifras++;
			fraccion += floor(enteroResto)*pow(10, -i-1);
	//		printf("La primera cifra significativa es un %f\n", enteroResto);
			resto=resto-enteroResto;
			fin=true;
			}
		i++;
		}
	//Ya sabemos hasta qué número de cifras decimales tenemos que coger:
	int sumar=0;
	if(enteroResto==1)	sumar=numCS;
	else				sumar=numCS-1;
//	printf("Tenemos que sumar otras %d cifras\n", sumar);
	fin=false;
	int j=0;
	while(j<sumar)
		{
		resto=10*resto; //0.000YZQ, 0.00YZQ, 0.0YZQ, 0.YZQ, Y.ZQ
		enteroResto=floor(resto);
		resto=resto-enteroResto;
		if(j==sumar-1) //Si es la última cifra redondeamos
			{//Cogemos la siguiente cifra para ver si hay que redondear
			resto=10*resto; //0.000YZQ, 0.00YZQ, 0.0YZQ, 0.YZQ, Y.ZQ
//			printf("Última cifra, mirando si hay que redondear\n");
			double enteroRestoSiguiente=floor(resto);
//			printf("La siguiente es %f\n", enteroRestoSiguiente);
			if(enteroRestoSiguiente>4)	enteroResto++;
			}
//		printf("La siguiente cifra a coger es %f\n", enteroResto);
//		printf("Luego le sumamos: %f\n", enteroResto*pow(10, -i-j-1));
		fraccion +=enteroResto*pow(10, -i-j-1);
//		printf("Fracción=%f\n", fraccion);
		j++;
		}		
		
	double num=base+fraccion;
	switch(i+j)
		{
		case 1:
			sprintf(xRedondeado, "%.1f", num);
			break;		
		case 2:
			sprintf(xRedondeado, "%.2f", num);
			break;		
		case 3:
			sprintf(xRedondeado, "%.3f", num);
			break;		
		case 4:
			sprintf(xRedondeado, "%.4f", num);
			break;		
		case 5:
			sprintf(xRedondeado, "%.5f", num);
			break;		
		case 6:
			sprintf(xRedondeado, "%.6f", num);
			break;		
		case 7:
			sprintf(xRedondeado, "%.7f", num);
			break;		
		case 8:
			sprintf(xRedondeado, "%.8f", num);
			break;		
		case 9:
			sprintf(xRedondeado, "%.9f", num);
			break;		
		case 10:
			sprintf(xRedondeado, "%.10f", num);
			break;		
		default:
			sprintf(xRedondeado, "%.6f", num);
			break;		
		}
//	printf("Devolvemos %s\n", xRedondeado);
	return atof(xRedondeado);
//	return base+fraccion;
	}

//**************** PRESENTA NUMERO ********************
/*	Dado un número num1+-num2, nos devuelve una cadena
	con ese número en notación valor(incertidumbre)
	*/
void PresentaNumero(double num1, double num2, char *cadena) 
	{
//	printf("PRESENTA NUMERO %f,%f\n", num1, num2);
	if (num2 <= 0.0) 
		{
		redondeaDouble(num1, 2, cadena);
		return;
		}

//	printf("Obtenemos el número de cifras de la incertidumbre\n");
	// obtiene el numero de cifras de la incertidumbre
//	printf("num2=%f\n", num2);
	int ordenNum2 = orden(num2);
	if (ordenNum2 == -999) 
		{
		printf("ERROR: ordenNum2 vale %f\n", ordenNum2);
		sprintf(cadena, "*error*%f (%e)***", num1, num2);
		return;
		}
	//NUEVO
	if(ordenNum2 == -666)	//El orden de la incertidumbre es muy bajo, se puede asemejar a cero
		{
		sprintf(cadena, "%.f", num1);
		return;
		}
	//-------
		
//	if (inicial(num2) == 1) ordenNum2--;
	
	if (num2<0 && inicial(num2) == 1) ordenNum2--;
	if(num2>1)	ordenNum2=0;
	
	//NUEVO: Me ha dicho Begoña que mejor una cifra significativa más.
	ordenNum2--;
	//
//	printf("Número de cifras de la incertidumbre: %d\n", ordenNum2);
	// obtiene las cifras de la incertidumbre
//	printf("Obtenemos las cifras de la incertidumbre\n");
	int cifrasNum2 = redondea(num2/powi(10.0, ordenNum2));
//	printf("Cifras de la incertidumbre: %d\n", cifrasNum2);
	// obtiene el orden del valor
//	printf("Obtenemos el orden del valor\n");
	int ordenNum1 = orden(num1);
//	printf("Orden del valor: %d\n", ordenNum1);
	
	if ((ordenNum1 == -1)&&(ordenNum2 > ordenNum1)) 
		{
		ordenNum2--;
		cifrasNum2 = redondea(num2/powi(10.0, ordenNum2));
		num2 = ((double)cifrasNum2) * powi(10.0, ordenNum2);
		sprintf(cadena, "%.1f(%.0f)", num1, num2);
		return;
		}
	
		
	// deja los numeros de la incertidumbre
//	printf("Dejamos los números de la incertidumbre\n");
	num2 = ((double)cifrasNum2) * powi(10.0, ordenNum2);
	
//	printf("Dejamos los numeros de la incertidumbre, num2=%f\n", num2);
		
	if (ordenNum2 >= 0)		
		{
		sprintf(cadena, "%.0f(%.0f)", num1, num2);
	//	printf("Nos queda de cadena: %.0f(%.0f)\n", num1, num2);
		}
	else
		{
		char formato[] = "%.00f(%d)";
		// pone el numero de digitos a representar
		formato[2] = (unsigned char) 0x30 + abs(ordenNum2/10);
		formato[3] = (unsigned char) 0x30 + abs(ordenNum2%10);
		sprintf(cadena, formato, num1, cifrasNum2);
		}
//	printf("Termina PRESENTANUMERO\n");
	}

//********************* MEDIA *******************
/*	Calcula la media ponderada por incertidumbres de e(se), de tamaño max,
	y la almacena en media(smedia)
	*/
void media(int max, float *e, float *se, float media, float smedia)
	{
	printf("MEDIA--------------------------\n");
	media=0;
	float s1=0;
	float s2=0;
	smedia=0;
	float temp=0;
	int i=0;
	
	for(i=0;i<max;i++)
		{
		printf("media+=%f/%f²\n", e[i], se[i]);
		media+=e[i]/(se[i]*se[i]);
		temp+=1.0/(se[i]*se[i]);
		}
	media/=temp;
	for(i=0;i<max;i++)	s1+=((e[i]-media)*(e[i]-media))/(se[i]*se[i]);	
	printf("smedia(%f) se divide entre %f y entre %d\n", smedia, temp, max-1);
	s1/=temp;
	s2=1/temp;
	
	printf("Media fluctuaba entre ser %f y %f\n", s1, s2);
	smedia=sqrt(maxim(s1,s2));
	printf("%f (%f)\n", media, smedia);
	char texto[20];
	PresentaNumero(media, smedia, texto);
	printf("%s\n", texto);
	}	
	
#ifndef powi
double powi(double valor, int potencia) 
	{
	return pow(valor, (double) potencia);
	}
#endif

#ifndef abs
int abs(int numero) {
	return (numero >= 0) ? numero : -numero;
}
#endif
