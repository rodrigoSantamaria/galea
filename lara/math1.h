const unsigned short ITMAX = 100;
const double EPS = 3.0e-7;
const double FPMIN = 1.0e-30;

extern double gcf(double, double);
extern double gser(double, double);
extern double gammq(double, double);
extern double gammp(double, double);
extern double log0(double);
extern double log010(double);
extern int orden(double); /* devuelve el orden del numero indicado. Optimizado
							para numeros pequegnos */
extern int inicial(double); /* devuelve el valor del primer digito del numero */

extern double maxim(double, double); /* devuelve el valor maximo de los dos
							indicados */

extern double signo(double); /* devuelve el signo del valor pasado */

extern int redondea(double); /* devuelve el valor redondeado del numero pasado */
extern double fredondea(double); /* devuelve el valor redondeado del numero pasado */

extern void PresentaNumero(double, double, char *); /* presenta en la cadena
						pasada los numeros pasados como valor(incertidumbre). La
						cadena ha de tener espacio suficiente para la representacion. */

/* Necesarios por si las siguientes constantes/funciones no se encuentran
	definidas en la libreria <math.h> */
#ifndef M_2PI
#define M_2PI     6.2831853071795864769E0
#endif

#ifndef powi
extern double powi(double, int);
#endif

#ifndef abs
extern int abs(int);
#endif
