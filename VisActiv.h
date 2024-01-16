#ifndef __VISACTIV_H__
#define __VISACTIV_H__

#include <Xm/Xm.h>
#include "analisis.h"
#include "picos.h"

// ***************
// ** VARIABLES **
// ***************

// ***************
// ** FUNCIONES **
// ***************


extern Widget Crea_AreaActividad (Widget);
extern void creaListaActividad();
void comprobarElementosInexistentes();
bool estaEmisionMasProbable(int);
bool estaEmisionMasProbable(int, float, float);

extern void calculaMediaBDFondo(char *, int, int, double *, double *, double *); 
extern void calculaAreaPicos(double *, double *, double *, int);
extern void pulsaOKFileEficiencia(Widget, XtPointer, XtPointer);
extern void EstableceComputeActividad();
extern void reseteaActividad();
extern double actividad(double, double, double, double, double);
extern double sigmaActividad(double, double, double, double, double, double, double, double, double);

extern void borraVisActiv();

extern float determinarProbabilidad(float, char *);

#endif
