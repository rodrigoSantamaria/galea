#ifndef __GRAFICA_H__
#define __GRAFICA_H__

#include <Xm/Xm.h>

extern Widget Crea_VisorGrafica (Widget);
extern void PintaGrafica (int);
extern void DesactivaGrafica ();
extern void ActivaGrafica ();
extern void MarcaZoom (int, int); /* ajusta el zoom horizontal y el desplazamiento
					para que se vea el intervalo indicado en la grafica */
extern void CambiaPosicionCursor (unsigned int);
extern void EstableceAutoEscalaY(bool);
extern void EstableceEscalaY(int);

	extern void BorraGrafica();
	/* */
	extern void ReseteaGraficas();
	/* Borra las graficas actuales y vuelve a generar las graficas por
		defecto, pero vacias */
	int CopiaEspectroAGrafica();
	int CopiaContinuoAGrafica();
	int CopiaAjustesAGrafica();
	int CopiaAjustesParcialesAGrafica(int, int);
	int CopiaAjustesDeconvolucionAGrafica(int,int,int);
	//
	int CopiaFondoAGrafica(int, int);
	int CopiaFuncionAGrafica(float*, int, int);
	int CopiaGeneticAGrafica(float*, int, int, int);
	int CopiaErrorAGrafica(float*, int, int);
	//
	int CopiaPicosChGrafica();
	int CopiaContinuoChGrafica();
	
	/* Pasa los datos indicados a la grafica asociada. Devuelve -1 si no lo
		consigue. */
	int BorraPicosChEnGrafica();
	int BorraContinuoChEnGrafica();
	int BorraContinuoEnGrafica();
	int BorraAjustesEnGrafica();
	int BorraAjustesParcialesEnGrafica();
	int BorraAjustesDeconvolucionEnGrafica();
	int BorraFondoEnGrafica();
	int BorraGeneticEnGrafica();
	int BorraGeneticEnGrafica(int numGraf);
	int BorraErrorEnGrafica(int numGraf);
	
	/* Borra la grafica indicada */
	void ActivaDatosGrafica(bool state);
	void ActivaContinuoChGrafica(bool state);
	void ActivaPicosChGrafica(bool state);
	void ActivaContinuoGrafica(bool state);
	void ActivaAjustesGrafica(bool state);
	void ActivaAjustesParcialesGrafica(bool state);
	void ActivaAjustesDeconvolucionGrafica(bool state);
	void ActivaAjustesDeconvolucionGrafica(bool state, int numGraf);
	//-------- NUEVO -----------
	void ActivaFondoGrafica(bool state);
	void ActivaGeneticGrafica(bool state, int numGraf);
	void ActivaErrorGrafica(bool state, int numGraf);
	//--------------------------
	/* Pone el estado de activo o inactivo en las graficas */

// ***************************  Ventana Datos  ************************
	extern void ActivaEnergiaDatos();
	/* Activa informacion sobre energia en la ventana de datos */
	extern void ActivaFormaDatos();
	/* Activa informacion sobre parametros de forma en la ventana de datos */
	extern void ActivaPreCalibDatos();
	/* Activa informacion sobre energia y anchura gaussiana en la ventana de
		datos */
	extern void DesactivaEnergiaDatos();
	/* Desactiva informacion sobre energia en la ventana de datos */
	extern void DesactivaFormaDatos();
	/* Desactiva informacion sobre parametros de forma en la ventana de datos */
	extern void DesactivaPreCalibDatos();
	/* Desactiva informacion sobre energia y anchura gaussiana en la ventana de
		datos */
	extern void PonPosicionCursor();
	/* Actualiza la informacion de la ventana de datos */
	extern void EstableceVentanaDatos(bool);
	/* Muestra (true) o quita (false) la ventana de datos */

#endif
