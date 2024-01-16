#include <stdio.h>
#include <math.h>
#include "math1.h"

#define MAXIMO_ORDEN 20

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

double gammq(double a, double x) 
	{
   	if ((x < 0.0)||(a <= 0.0)) return 0.0;
   	if (x < a+1.0) return 1.0 - gser(a, x);
   	else return gcf(a, x);
	}

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

int orden(double x) 
	{
	if (x <= 0.0) return -999;
	
	int i = 0;
	bool flag = false;
	if (x >= 1.0) 
		{
		while ((i < MAXIMO_ORDEN)&&(!flag)) 
			{
			double temp = x / powi(10.0, i);
			if (temp < 10.0)
			   flag = true;
			else i++;
			}
		}
	else 
		{
		while ((i < MAXIMO_ORDEN)&&(!flag)) 
			{
			double temp = x * powi(10.0, i);
			if (temp < 1.0)
				i++;
			else 
				{
			    flag = true;
				i = -i;
				}
			}
		}
	if (flag) return i;
	else return -999;
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

int redondea(double x) 
	{
	double base = floor(x);
	double resto = x - base;
	if (x >= 0.0)	return (resto > 0.5) ? (int) base + 1 : (int) base;
	else			return (resto >= 0.5) ? (int) base + 1 : (int) base;
	}

double fredondea(double x) 
	{
	double base = floor(x);
	double resto = x - base;
	if (x >= 0.0)		return (resto > 0.5) ? base + 1.0 : base;
	else				return (resto >= 0.5) ? base + 1.0 : base;
	}

//*****************	PRESENTA NÚMERO *************************
/*  Dado un número num1 con incertidumbre num2 presenta este número
	como una cadena.
	Si la incertidumbre es 0 la cadena sólo contendrá num1.
*/
void PresentaNumero(double num1, double num2, char *cadena) 
	{
	if (num2 <= 0.0) 
		{
		sprintf(cadena, "%f", num1);
		return;
		}

	// obtiene el numero de cifras de la incertidumbre
	int ordenNum2 = orden(num2);
	if (ordenNum2 == -999) 
		{
		sprintf(cadena, "***%f (%e)***", num1, num2);
		return;
		}
	if (inicial(num2) == 1) ordenNum2--;
	
	// obtiene las cifras de la incertidumbre
	int cifrasNum2 = redondea(num2/powi(10.0, ordenNum2));
	
	// obtiene el orden del valor
	int ordenNum1 = orden(num1);
	if ((ordenNum1 == -1)&&(ordenNum2 > ordenNum1)) 
		{
		ordenNum2--;
		cifrasNum2 = redondea(num2/powi(10.0, ordenNum2));
		num2 = ((double)cifrasNum2) * powi(10.0, ordenNum2);
		sprintf(cadena, "%.1f(%.0f)", num1, num2);
		return;
		}

	// deja los numeros de la incertidumbre
	num2 = ((double)cifrasNum2) * powi(10.0, ordenNum2);
	
	if (ordenNum2 >= 0)		sprintf(cadena, "%.0f(%.0f)", num1, num2);
	else 
		{
		char formato[] = "%.00f(%d)";
		// pone el numero de digitos a representar
		formato[2] = (unsigned char) 0x30 + abs(ordenNum2/10);
		formato[3] = (unsigned char) 0x30 + abs(ordenNum2%10);
		sprintf(cadena, formato, num1, cifrasNum2);
		}
	}

#ifndef powi
double powi(double valor, int potencia) 
	{
	return pow(valor, (double) potencia);
	}
#endif

#ifndef abs
int abs(int numero) 
	{
	return (numero >= 0) ? numero : -numero;
	}
#endif
