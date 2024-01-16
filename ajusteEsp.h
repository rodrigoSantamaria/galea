#ifndef __AJUSTEESP_H__
#define __AJUSTEESP_H__

extern void ActualizaVectorRectaCalib(double);
extern void ActualizaVectorCuadradoCalib(double);
extern void ActualizaVectorContinuo(double);
extern double ActualizaVectorPicoCalib(double pos, double *param, double* dParam);
extern double ActualizaVectorPicosAnal(double pos, double *param, double* dParam);

#endif
