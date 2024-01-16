#ifndef __TRABAJO_H__
#define __TRABAJO_H__

#include <Xm/Xm.h>

// funciones
extern Widget Crea_AreaTrabajo (Widget);
							// Activacion
extern void ActivaTrabajo();
extern void DesactivaTrabajo();
extern void ActivaContinuo();
extern void DesactivaContinuo();
extern void ActivaAnalisis();
extern void DesactivaAnalisis();
extern void ActivaActividad();
extern void DesactivaActividad();
extern void PonActivaZona(int);

#endif
