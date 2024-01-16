#ifndef __TRABAJOEST_H__
#define __TRABAJOEST_H_

/* *********************
	**      TIPOS      **
	********************* */

// estructura para estados completos del analisis
typedef struct tEstadoTrabajo {
	// estado del analisis (se guarda en el fichero)
	bool PreCalibracion;
	bool Calibracion;
	bool Continuo;
	bool PreContinuo;
	//NOTA: No entiendo para qué los bool si tiene luego un entero para el estado actual
	int Actual;
	// estado de analisis parcial
	bool CalibracionEnergia;
	bool CalibracionForma;
	//NUEVO: Estados del análisis
	bool Analisis;//Indica que se ha realizado el análisis de todos los picos
	//Estados de la actividad
	bool AnalisisEficiencia;
	bool AnalisisLD;
	bool AnalisisActividad;
	bool AnalisisMDA;
	//
	bool fondoCargado;
	
	// cambios en el trabajo (no se guarda en el fichero)
	bool TrabajoCambios;
	bool SearchContinuumCambios;
	bool AdjustContinuumCambios;
} tEstadoTrabajo;

// estructura para estado visual de las representaciones
typedef struct tEstadoVisual {
	// zona del Continuo
	bool CanalContinuo;
	bool CanalPicos;
	bool AjusteContinuo;
	bool CanalNormal;
	// Zona de grafica
	unsigned char EscalaY;	// 0 -> Lineal
							// 1 -> Raiz Cuadrada
							// 2 -> Logaritmica
	bool AutoEscalaY;
	bool VentanaDatos;
	bool VentanaFicheroDatos;
	// Zona de calibracion
	bool AutoZoomCalibracion;
	bool AjusteCalibracion;
	// Zona de analisis
	bool AjusteParcialAnalisis;
	bool AjusteTotalAnalisis;
	bool AutoZoomAnalisis;
} tEstadoVisual;


/* **************************
	**      CONSTANTES      **
	************************** */

// valores para el estado actual del analisis
const int 	ESTADO_INICIAL = 0, 
				ESTADO_PRECALIBRACION = 1,
				ESTADO_CONTINUO = 2,
				ESTADO_CALIBRACION = 3,
				ESTADO_ANALISIS = 4,
				ESTADO_ACTIVIDAD = 5;

// valores para la visualizacion
const int 	VISUAL_LINEAL = 0, 
				VISUAL_RAIZ = 1,
				VISUAL_LOGARITMO = 2;


/* *************************
	**      VARIABLES      **
	************************* */

extern tEstadoTrabajo EstadoTrabajo;
extern tEstadoVisual EstadoVisual;



/* *************************
	**      FUNCIONES      **
	************************* */

							// Inicializacion
extern void InicializaEstadoTrabajo(tEstadoTrabajo&);
extern void InicializaEstadoVisual(tEstadoVisual&);
extern void EstableceVisual(const tEstadoVisual);
/**********************  visualizaciones  **********************/
	extern void EstableceShowsContinuo(const tEstadoVisual);
	/* establece los valores de los Show del continuo */
	extern void EstableceShowsCalibracion(const tEstadoVisual);
	/* establece los valores de las opciones de representacion de Calibracion */
	extern void EstableceShowsAnalisis(const tEstadoVisual);
	/* establece los valores de las opciones de representacion de Analisis */

#endif
