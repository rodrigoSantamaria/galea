#include <stdio.h>
#include "calibracion.h"
#include "constDef.h"
#include "fuentes.h"
#include "trabajo.h"
#include "trabajoEst.h"
#include "VisActiv.h"
#include "VisAnal.h"
#include "VisCalib.h"
#include "VisConti.h"
#include "VisGraf.h"
#include <Xm/Form.h>
#include <Xm/ToggleBG.h>
#include <Xm/RowColumn.h>

// Constantes
const int 	 TOGGLE_CALIBRACION = 1,
			 TOGGLE_CONTINUO    = 2,
			 TOGGLE_ANALISIS    = 3,
			 TOGGLE_ACTIVIDAD   = 4;
			 
// variable de cambio de seccion
bool CambiandoSeccion = false;

// variable de estado de trabajo
tEstadoTrabajo EstadoTrabajo;

// Widget general
Widget fondo_trabajo;
Widget form_botones, form_secciones;
Widget row_botones, bot_calibracion, bot_continuo, bot_analisis, bot_actividad;
Widget form_calibracion, form_analisis, form_continuo, form_actividad;

// Funciones
Widget Crea_AreaTrabajo (Widget);
							// Activacion
void ActivaTrabajo();
void DesactivaTrabajo();
void ActivaContinuo();
void DesactivaContinuo();
void PonActivaZona(int);
							// Inicializacion
void InicializaEstadoTrabajo(tEstadoTrabajo&);


// Callbacks
void Cambia_Trabajo (Widget, XtPointer, XtPointer);


//******************** CREA AREA TRABAJO *******************
Widget Crea_AreaTrabajo(Widget parent) 
	{
   	Arg			args[7];
   	Cardinal	i = 0;
	XmString 	cadena;

	printf("Cremos fondo_trabajo\n");
	i = 0;
   	fondo_trabajo = XmCreateForm (parent, "fondo_trabajo", args, i);

	printf("Cremos form_botones\n");
	// formulario para los botones de cambio de seccion
	i = 0;
	form_botones = XmCreateForm (fondo_trabajo, "form_botones", args, i);
	
	printf("Añadimos lista de botones\n");
	// lista de botones
	i = 0;
   	XtSetArg (args[i], XmNorientation, XmHORIZONTAL); i++;
	row_botones = XmCreateRadioBox(form_botones, "row_botones", args, i);
	i = 0;
   	cadena = XmStringCreate ("Calibration", "BoldNormal");
   	XtSetArg (args[i], XmNlabelString, cadena); i++;
   	XtSetArg (args[i], XmNset, true); i++;
   	XtSetArg (args[i], XmNfontList, lista_fuentes); i++;
	bot_calibracion = XmCreateToggleButtonGadget(row_botones, "bot_calibracion", args, i);
   	XtAddCallback (bot_calibracion, XmNvalueChangedCallback, Cambia_Trabajo, (XtPointer) TOGGLE_CALIBRACION);
   	XmStringFree (cadena);
	i = 0;
   	cadena = XmStringCreate ("Continuum", "BoldNormal");
   	XtSetArg (args[i], XmNlabelString, cadena); i++;
   	XtSetArg (args[i], XmNsensitive, false); i++;
   	XtSetArg (args[i], XmNfontList, lista_fuentes); i++;
	bot_continuo = XmCreateToggleButtonGadget(row_botones, "bot_continuo", args, i);
   	XtAddCallback (bot_continuo, XmNvalueChangedCallback, Cambia_Trabajo, (XtPointer) TOGGLE_CONTINUO);
   	XmStringFree (cadena);
	i = 0;
   	cadena = XmStringCreate ("Peak Analysis", "BoldNormal");
   	XtSetArg (args[i], XmNlabelString, cadena); i++;
   	XtSetArg (args[i], XmNsensitive, false); i++;
   	XtSetArg (args[i], XmNfontList, lista_fuentes); i++;
	bot_analisis = XmCreateToggleButtonGadget(row_botones, "bot_analisis", args, i);
   	XtAddCallback (bot_analisis, XmNvalueChangedCallback, Cambia_Trabajo, (XtPointer) TOGGLE_ANALISIS);
   	XmStringFree (cadena);

	//
	i = 0;
   	cadena = XmStringCreate ("Activity", "BoldNormal");
   	XtSetArg (args[i], XmNlabelString, cadena); i++;
   	XtSetArg (args[i], XmNsensitive, false); i++;
   	XtSetArg (args[i], XmNfontList, lista_fuentes); i++;
	bot_actividad = XmCreateToggleButtonGadget(row_botones, "bot_actividad", args, i);
   	XtAddCallback (bot_actividad, XmNvalueChangedCallback, Cambia_Trabajo, (XtPointer) TOGGLE_ACTIVIDAD);
   	XmStringFree (cadena);
	//
	
	XtManageChild(bot_calibracion);
	XtManageChild(bot_continuo);
	XtManageChild(bot_analisis);
	XtManageChild(bot_actividad);
	
	printf("Colocamos los widgets\n");
	// coloca los widgets
   	XtVaSetValues (row_botones,
		XmNrightAttachment, XmATTACH_FORM,
		XmNleftAttachment, XmATTACH_FORM,
		XmNtopAttachment, XmATTACH_FORM,
		XmNbottomAttachment, XmATTACH_FORM,
		NULL);
   	XtManageChild (row_botones);
   	XtManageChild (form_botones);
	
	printf("creamos form_secciones\n");
	// formulario para las secciones de trabajo
	i = 0;
	
	form_secciones = XmCreateForm(fondo_trabajo, "form_secciones", args, i);
	
	printf("Creamos área calibracion\n");
   	// Crea todas las areas de trabajo
	form_calibracion = Crea_AreaCalibracion (form_secciones);
	printf("Creamos área continuo\n");
	form_continuo = Crea_AreaContinuo (form_secciones);
	printf("Creamos área análisis\n");
	form_analisis = Crea_AreaAnalisis (form_secciones);
   	printf("Creamos área actividad\n");
	form_actividad = Crea_AreaActividad (form_secciones);
   	// pone activa la primera (Calibracion)
	XtManageChild (form_calibracion);
	
	printf("Colocamos los formularios\n");
	// coloca los formularios
   	XtVaSetValues (form_botones,
      XmNrightAttachment, XmATTACH_FORM,
		XmNleftAttachment, XmATTACH_FORM,
		XmNtopAttachment, XmATTACH_FORM,
		NULL);
   	XtVaSetValues (form_secciones,
      XmNrightAttachment, XmATTACH_FORM,
		XmNleftAttachment, XmATTACH_FORM,
		XmNbottomAttachment, XmATTACH_FORM,
		XmNtopAttachment, XmATTACH_WIDGET, XmNtopWidget, form_botones,
		NULL);
	
	printf("Mostramos secciones y área\n");
   	XtManageChild (form_secciones);
   	XtManageChild (fondo_trabajo);
	
	iniciaBotones();
	printf("Terminamos crea area trabajo\n");
	return fondo_trabajo;
	}

// ***********
// **  	ACTIVACIONES
// *********

void ActivaTrabajo () {
	ActivaPreCalibracion();
}

void DesactivaTrabajo () {
	DesactivaPreCalibracion();
	DesactivaCalibracion();
	DesactivaContinuo();
	DesactivaAnalisis();
	DesactivaActividad();
}

void ActivaContinuo () {
   XtSetSensitive(bot_continuo, true);
   XtSetSensitive(form_continuo, true);
}

void DesactivaContinuo () {
   XtSetSensitive(bot_continuo, false);
   XtSetSensitive(form_continuo, false);
}

void ActivaAnalisis () {
   XtSetSensitive(bot_analisis, true);
   XtSetSensitive(form_continuo, true);//NOTA: continuo????
}

void DesactivaAnalisis () 
	{
   	XtSetSensitive(bot_analisis, false);
	}

void ActivaActividad () 
	{
	printf("-----------------------------------------ACTIVA ACTIVIDAD\n");
   	XtSetSensitive(bot_actividad, true);
   	XtSetSensitive(form_actividad, true);
	}
	
//***************** DESACTIVA ACTIVIDAD ************
/*	Inhabilita la interfaz de actividad.
	NOTA: Debería resetear toda la lista de actividad y los botones
*/
void DesactivaActividad () 
	{
	printf("DESACTIVA ACTIVIDAD\n"); 
   	XtSetSensitive(bot_actividad, false);
  // 	XtSetSensitive(form_actividad, false);
	XtSetSensitive(form_continuo, false);//NOTA: continuo??
	}


void Cambia_Trabajo (Widget widget, XtPointer client_data, XtPointer call_data) {
   XmToggleButtonCallbackStruct *state = (XmToggleButtonCallbackStruct *) call_data;
	int Zona_activada = (int) client_data;

	CambiandoSeccion = true;
	
   if (state->set == 0) 
	   { // se desactiva una zona de trabajo
	   switch (Zona_activada) 
		   {
		   case TOGGLE_CALIBRACION:
				printf("CREANDO AREA DE CALIBRACIÓN\n");
				XtUnmanageChild (form_calibracion);
		   		CierraDialogPrecalibracion();
			    break;
			case TOGGLE_CONTINUO:
				printf("CREANDO AREA DE CONTINUO\n");
				XtUnmanageChild (form_continuo);
				break;
			case TOGGLE_ANALISIS:
				XtUnmanageChild (form_analisis);
				break;
			case TOGGLE_ACTIVIDAD:
				XtUnmanageChild (form_actividad);
				break;
			}
		} 
	else 
	   { // se activa una zona de trabajo
	   switch (Zona_activada) 
		   {
		   case TOGGLE_CALIBRACION:
				XtManageChild (form_calibracion);
				if (EstadoTrabajo.Actual > ESTADO_PRECALIBRACION) 
					{
					EstadoTrabajo.Actual = ESTADO_CALIBRACION;
					DesactivaPreCalibracion();
					}
				else 
					{
					EstadoTrabajo.Actual = ESTADO_PRECALIBRACION;
					ActivaPreCalibracion();
					}
		      break;
			case TOGGLE_CONTINUO:
				EstadoTrabajo.Actual = ESTADO_CONTINUO;
				EstadoBotonesContinuo();
				XtManageChild (form_continuo);
				break;
			case TOGGLE_ANALISIS:
				iniciaBotones();
				XtManageChild (form_analisis);
				EstadoTrabajo.Actual = ESTADO_ANALISIS;
				break;
			case TOGGLE_ACTIVIDAD:
				printf("Poniendo activa zona de Actividad\n");
				EstadoTrabajo.Actual = ESTADO_ACTIVIDAD;
				creaListaActividad();
				XtManageChild(form_actividad);
				break;
		 	}	
		}
	}

void PonActivaZona(int zona) 
	{
	// busca la zona activa
	printf("PON ACTIVA ZONA %d\n", zona);
	int nEstado=0;
	if (XmToggleButtonGadgetGetState(bot_calibracion)) XmToggleButtonGadgetSetState(bot_calibracion, false, true);
	else if (XmToggleButtonGadgetGetState(bot_continuo)) XmToggleButtonGadgetSetState(bot_continuo, false, true);
	else if (XmToggleButtonGadgetGetState(bot_analisis)) XmToggleButtonGadgetSetState(bot_analisis, false, true);
	else if (XmToggleButtonGadgetGetState(bot_actividad)) XmToggleButtonGadgetSetState(bot_actividad, false, true);
	switch (zona) 
		{
		case ESTADO_INICIAL:
		case ESTADO_PRECALIBRACION:
		case ESTADO_CALIBRACION:
			CierraDialogPrecalibracion();
			XmToggleButtonGadgetSetState(bot_calibracion, true, true);
			break;
		case ESTADO_CONTINUO:
			XmToggleButtonGadgetSetState(bot_continuo, true, true);
			break;
		case ESTADO_ANALISIS:
			XmToggleButtonGadgetSetState(bot_analisis, true, true);
			break;
		case ESTADO_ACTIVIDAD:
			printf("Poniendo activa zona de Actividad (PonActivaZona)\n");
			
			XmToggleButtonGadgetSetState(bot_actividad, true, true);
			creaListaActividad();
			break;
 		}
	}


// ***********
// **  		INICIALIZACIONES
// *********

void InicializaEstadoTrabajo(tEstadoTrabajo& Estado) 
	{
	printf("INICIALIZA ESTADO TRABAJO \n");
	Estado.PreCalibracion 	  = false;
	Estado.Calibracion 		  = false;
	Estado.PreContinuo 		  = false;
	Estado.Continuo 		  = false;
	Estado.CalibracionEnergia = false;
	Estado.CalibracionForma   = false;
	Estado.AnalisisLD         = false;
	Estado.AnalisisEficiencia = false;
	Estado.AnalisisActividad  = false;
	Estado.TrabajoCambios 	  = false;
	Estado.SearchContinuumCambios = false;
	Estado.AdjustContinuumCambios = false;
	Estado.Actual = ESTADO_INICIAL;
		
	Estado.fondoCargado=false;
	}

void InicializaEstadoVisual(tEstadoVisual& Estado) {
					// Zona de continuo
	Estado.CanalContinuo = false;
	Estado.CanalPicos = false;
	Estado.AjusteContinuo = false;
	Estado.CanalNormal = false;
					// Zona de grafica
	Estado.AutoEscalaY = false;
	Estado.VentanaDatos = false;
	Estado.EscalaY = VISUAL_LINEAL;
					// Zona de calibracion
	Estado.AutoZoomCalibracion = false;
	Estado.AjusteCalibracion = false;
					// Zona de analisis
	Estado.AjusteParcialAnalisis = false;
	Estado.AjusteTotalAnalisis = false;
	Estado.AutoZoomAnalisis = false;

		// Desactiva las graficas
	ActivaDatosGrafica(true);
	ActivaContinuoChGrafica(false);
	ActivaPicosChGrafica(false);
	ActivaContinuoGrafica(false);
	ActivaAjustesGrafica(false);
}

// ***********
// **  		VISUALIZACIONES
// *********

//************** ESTABLECE VISUAL ****************
/*	Inicia de la manera adecuada según el fichero que hayamos
cargado todas las variables de la interfaz: zooms, escalas, estados
de los botones, etc.
Cualquier función que maneje este tipo de eventos debería ser llamada
desde aquí para la correcta inicialización del fichero cargado.
(para los estados de botones que no tengan que ver con la gráfica, la gestión
se hace desde su bloque de carga en Analgam.c, al final del bloque.
*/
void EstableceVisual(const tEstadoVisual Estado) 
	{
	EstableceShowsContinuo(Estado);
	EstableceShowsCalibracion(Estado);
	EstableceShowsAnalisis(Estado);
	EstableceAutoEscalaY(Estado.AutoEscalaY);
	EstableceEscalaY(Estado.EscalaY);
	EstableceVentanaDatos(Estado.VentanaDatos);
	}
