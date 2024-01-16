#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "avisos.h"
#include "calibracion.h"
#include "constDef.h"
#include "cursores.h"
#include "fuentes.h"
#include "globales.h"
#include "math1.h"
#include "picos.h"
#include "trabajo.h"
#include "trabajoEst.h"
#include "VisAnal.h"
#include "VisIdent.h"
#include "VisGraf.h"
#include <Xm/Form.h>
#include <Xm/Frame.h>
#include <Xm/Label.h>
#include <Xm/LabelG.h>
#include <Xm/List.h>
#include <Xm/MwmUtil.h>
#include <Xm/PushB.h>
#include <Xm/RowColumn.h>
#include <Xm/Separator.h>
#include <Xm/Text.h>
#include <Xm/TextF.h>
#include <Xm/ToggleB.h>
#include <Xm/ToggleBG.h>

//Para la gráfica de dispersión
#include <Xm/DrawingA.h>
#include <Xm/ComboBox.h>

//Para manejo de ficheros
#include <fstream>
#include <iostream>
#include <string>
using namespace std;


// Widget general
Widget fondo_calibracion, label_form_calibracion;
Widget linea_calib_1, linea_calib_2;
Widget boton_precalibra, boton_calibracion;

// Widgets de precalibracion
Widget dialog_preCalib, paned_preCalib;
Widget superior_preCalib;
Widget linea_preCalib;
Widget inferior_preCalib, cerrar_preCalib;
Widget lista_preCalib;
Widget row_botones_preCalib, agnadir_preCalib, quitar_preCalib;
Widget row_textos_preCalib, canal_izq_preCalib, canal_der_preCalib, energia_preCalib;
Widget lab_canal_izq_preCalib, lab_canal_der_preCalib, lab_energia_preCalib;

// Widgets de Calibracion
Widget interv_frame_calibracion, label_interv_frame_calibracion,
		 interv_lista_calibracion, bt_add_interv_calibracion;
Widget result_frame_calibracion, label_result_frame_calibracion,
		 result_lista_calibracion, bt_form_result_calibracion,
		 bt_del_result_calibracion, bt_edit_result_calibracion;
Widget show_frame_calibracion, label_frame_calibracion, show_rowcol_calibracion;
Widget show_adjust_calibracion, show_autoZoom_calibracion, show_dispersion_calibracion;

// Widgets del menu de edicion de Calibracion
Widget dlg_edCalib;
Widget fr_adjust_edCalib, fr_energy_edCalib,
		 linea_edCalib, fm_botones_edCalib;
Widget fm_adjust_edCalib, lb_adjust_edCalib,
		 rw1_adjust_edCalib, rw2_adjust_edCalib, rw3_adjust_edCalib,
		 lbCen_adjust_edCalib, txtCen_adjust_edCalib, lbCenEn_adjust_edCalib,
		 lbSigma_adjust_edCalib, txtSigma_adjust_edCalib, lbSigmaEn_adjust_edCalib,
		 lbTau_adjust_edCalib, txtTau_adjust_edCalib, lbTauEn_adjust_edCalib,
		 lbYtau_adjust_edCalib, txtYtau_adjust_edCalib, lbYtauEn_adjust_edCalib,
		 lbGraf_adjust_edCalib, lbEcuat_adjust_edCalib,
		 lbChiSq_adjust_edCalib;
Widget fm_energy_edCalib, lb_energy_edCalib,
		 rw_Izq_energy_edCalib,
		 lbEmission_energy_edCalib,
		 lbEnergy_energy_edCalib, lbProb_energyEdCalib,
		 lbElement_energy_edCalib, lbName_energy_edCalib, lbVoid_energy_edCalib,
		 fr_DB_identify_edCalib, lb_DB_identify_edCalib,
		 fm_DB_identify_edCalib, fm_DB_identify2_edCalib,
		 bt_DB_identify_edCalib;
Widget bt_ok_edCalib, bt_cancel_edCalib, bt_adjust_edCalib;


// Funciones
Widget Crea_AreaCalibracion (Widget);
								// Precalibracion
void Crea_DialogPrecalibracion();
void ActivaPreCalibracion();
void DesactivaPreCalibracion();
void CierraDialogPrecalibracion();
void PonIntervaloPreCalibracion (int, int);
void ReconstruyeListaPrecalibracion();
tListaPrecalib* DevuelveListaPreCalibracion();
void CopiaListaPreCalibracion(tListaPrecalib);
								// Calibracion
bool IsActiveCalibration();
void ActivaCalibracion();
void DesactivaCalibracion();
tListaCalibracion* DevuelveListaCalibracion();
void CopiaListaCalibracion(tListaCalibracion);
void ReconstruyeListaIntervCalibracion();
void BorraListaIntervCalibracion();
void CreateDialogCalibracion();
void ReponeListaCalibracion();
void ActualizaIdentificacionCalibracion(unsigned short int emission);
								// Ajustes
void ActualizaVectorEnergiaCalib(double);
								// Visualizaciones
void EstableceShowsCalibracion(const tEstadoVisual);

// Datos
tListaPrecalib listaPrecalibracion = { NULL, 0 };
tListaCalibracion listaCalibracion = { NULL, 0 };
NodoPrecalibracion *nodo;
int PreCalibSeleccionado = 0;
	// para el menu de edicion de calibracion
tPicoAjuste AjusteSeguridad;
int numIntervaloEdicion;
cVisIdentify CalibIdent; // elements identification

//Creación de una gráfica de dispersión para comprobación
Widget gd;
Widget graficaFit;
Widget desplegado;
void creaGraficaDispersion();
void redraw_calib(Widget w, XtPointer data, XtPointer cbk);
void seleccionaFormatoGrafica(Widget widget, XtPointer client_data, XtPointer call_data);
int formato;

// Constantes
const int BOTON_SHOW_AJUSTE = 1,
	  BOTON_SHOW_AUTOZOOM = 2,
	  BOTON_SHOW_DISPERSION = 3;


// Callbacks
	// PreCalibracion
void pulsa_PreCalibra (Widget, XtPointer, XtPointer);
void pulsa_cerrar_preCalib (Widget, XtPointer, XtPointer);
void pulsa_agnadir_preCalib (Widget, XtPointer, XtPointer);
void pulsa_borrar_preCalib (Widget, XtPointer, XtPointer);
void pulsa_lista_preCalib (Widget, XtPointer, XtPointer);
	// Calibracion
void pulsa_calibracion (Widget, XtPointer, XtPointer);
void pulsa_lista_interv_calibracion(Widget, XtPointer, XtPointer);
void pulsa_add_interv_calibracion(Widget, XtPointer, XtPointer);
void pulsa_lista_result_calibracion(Widget, XtPointer, XtPointer);
void pulsa_del_result_calibracion (Widget, XtPointer, XtPointer);
void pulsa_edit_result_calibracion (Widget, XtPointer, XtPointer);
	// Edicion Calibracion
void pulsa_identifica_edit_calibracion (Widget, XtPointer, XtPointer);	
void pulsa_cancel_edit_calibracion (Widget, XtPointer, XtPointer);
void pulsa_ok_edit_calibracion (Widget, XtPointer, XtPointer);
void pulsa_adjust_edit_calibracion (Widget, XtPointer, XtPointer);	
	// Menu de Shows
void pulsa_show_calibracion(Widget, XtPointer, XtPointer);

//_______________________ PRUEBAS ____________________________________________
void pulsa_almacenar(Widget widget, XtPointer call_data, XtPointer user_data);
//____________________________________________________________________________


Widget Crea_AreaCalibracion(Widget parent) 
	{
   	Arg		args[7];
   	Cardinal	i = 0;
	XmString cadena;

	i = 0;
   	fondo_calibracion = XmCreateForm (parent, "fondo_calibracion", args, i);
	// Crea el titulo del form
	i = 0;
	cadena = XmStringCreate ("Calibration", "BoldNormal");
   	XtSetArg (args[i], XmNlabelString, cadena); i++;
   	XtSetArg (args[i], XmNfontList, lista_fuentes); i++;
	label_form_calibracion = XmCreateLabel(fondo_calibracion, "label_form_calibracion", args, i);
   	XmStringFree (cadena);
	XtManageChild(label_form_calibracion);

	// crea las lineas de separacion
	i = 0;
   	XtSetArg (args[i], XmNseparatorType, XmSHADOW_ETCHED_OUT); i++;
	linea_calib_1 = XmCreateSeparator(fondo_calibracion, "linea_calib_1", args, i);
	linea_calib_2 = XmCreateSeparator(fondo_calibracion, "linea_calib_2", args, i);
	XtManageChild(linea_calib_1);
	XtManageChild(linea_calib_2);
	
   // Crea seccion intervalos de ajuste
   i = 0;
	XtSetArg (args[i], XmNorientation, XmVERTICAL); i++;
	XtSetArg (args[i], XmNentryAlignment, XmALIGNMENT_CENTER); i++;
   interv_frame_calibracion = XmCreateRowColumn (fondo_calibracion, "interv_frame_calibracion", args, i);
   cadena = XmStringCreate ("Intervals (keV) ", "BoldNormal");
   i = 0;
   XtSetArg (args[i], XmNfontList, lista_fuentes); i++;
   XtSetArg (args[i], XmNlabelString, cadena); i++;
   label_interv_frame_calibracion = XmCreateLabelGadget (interv_frame_calibracion, "label_interv_frame_calibracion", args, i);
   XmStringFree (cadena);
	i = 0;
	XtSetArg (args[i], XmNlistSizePolicy, XmCONSTANT); i++;
	XtSetArg (args[i], XmNselectionPolicy, XmEXTENDED_SELECT); i++;
	XtSetArg (args[i], XmNvisibleItemCount, 9); i++;
	XtSetArg (args[i], XmNscrollBarDisplayPolicy, XmSTATIC); i++;
   XtSetArg (args[i], XmNfontList, lista_fuentes_fixed); i++;
	interv_lista_calibracion = XmCreateScrolledList(interv_frame_calibracion, "interv_lista_calibracion", args, i);
	XtAddCallback (interv_lista_calibracion, XmNextendedSelectionCallback, pulsa_lista_interv_calibracion, NULL);
	i = 0;
   cadena = XmStringCreateLocalized (" => ");
   XtSetArg (args[i], XmNlabelString, cadena); i++;
	XtSetArg (args[i], XmNalignment, XmALIGNMENT_CENTER); i++;
   XtSetArg (args[i], XmNfontList, lista_fuentes); i++;
	bt_add_interv_calibracion = XmCreatePushButton(interv_frame_calibracion, "bt_add_interv_calibracion", args, i);
	XtAddCallback (bt_add_interv_calibracion, XmNactivateCallback, pulsa_add_interv_calibracion, NULL);
   XmStringFree (cadena);

   XtVaSetValues (interv_frame_calibracion,
		XmNbottomAttachment, XmATTACH_FORM,
		XmNbottomOffset, 20,
		XmNtopAttachment, XmATTACH_FORM,
		XmNtopOffset, 10,
		XmNleftAttachment, XmATTACH_FORM, 
		XmNleftOffset, 10,
		NULL);

	XtManageChild(label_interv_frame_calibracion);
	XtManageChild(interv_lista_calibracion);
	XtManageChild(bt_add_interv_calibracion);
	XtManageChild(interv_frame_calibracion);

	// **** OPCIONES DE REPRESENTACION ****
	// crea el boton de precalibracion
	i = 0;
   cadena = XmStringCreateLocalized (" PreCalibration ");
   XtSetArg (args[i], XmNlabelString, cadena); i++;
   XtSetArg (args[i], XmNfontList, lista_fuentes); i++;
	boton_precalibra = XmCreatePushButton(fondo_calibracion, "boton_precalibra", args, i);
   XmStringFree (cadena);
	XtManageChild(boton_precalibra);
   XtAddCallback (boton_precalibra, XmNactivateCallback, pulsa_PreCalibra, NULL);
   XtVaSetValues (boton_precalibra,
		XmNrightAttachment, XmATTACH_FORM,
		XmNrightOffset, 10,
		XmNbottomAttachment, XmATTACH_FORM,
		XmNbottomOffset, 20,
		NULL);

	// crea el boton de calibracion
	i = 0;
   cadena = XmStringCreateLocalized ("Calibration");
   XtSetArg (args[i], XmNlabelString, cadena); i++;
   XtSetArg (args[i], XmNfontList, lista_fuentes); i++;
	boton_calibracion = XmCreatePushButton(fondo_calibracion, "boton_calibracion", args, i);
   XmStringFree (cadena);
   XtAddCallback (boton_calibracion, XmNactivateCallback, pulsa_calibracion, NULL);
	XtManageChild(boton_calibracion);
   XtVaSetValues (boton_calibracion,
		XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET, XmNleftWidget, boton_precalibra,
		XmNrightAttachment, XmATTACH_FORM,
		XmNrightOffset, 10,
		XmNbottomAttachment, XmATTACH_WIDGET, XmNbottomWidget, boton_precalibra,
		NULL);

	
   // Crea opciones de representacion general
   i = 0;
   show_frame_calibracion = XmCreateFrame (fondo_calibracion, "show_frame_calibracion", args, i);
   cadena = XmStringCreateLocalized ("Show");
   i = 0;
   XtSetArg (args[i], XmNchildType, XmFRAME_TITLE_CHILD); i++;
   XtSetArg (args[i], XmNchildVerticalAlignment, XmALIGNMENT_CENTER); i++;
   XtSetArg (args[i], XmNlabelString, cadena); i++;
   XtSetArg (args[i], XmNfontList, lista_fuentes); i++;
   label_frame_calibracion = XmCreateLabelGadget (show_frame_calibracion, "label_frame_calibracion", args, i);
   XmStringFree (cadena);
   i = 0;
   show_rowcol_calibracion = XmCreateRowColumn(show_frame_calibracion, "show_rowcol_calibracion", args, i);

	// Toggle buton para los ajustes
   cadena = XmStringCreateLocalized ("Fits");
   i = 0;
   XtSetArg (args[i], XmNset, false); i++;
   XtSetArg (args[i], XmNlabelString, cadena); i++;
   XtSetArg (args[i], XmNfontList, lista_fuentes); i++;
	show_adjust_calibracion = XmCreateToggleButton (show_rowcol_calibracion, "show_adjust_calibracion", args, i);
	XtAddCallback (show_adjust_calibracion, XmNvalueChangedCallback, pulsa_show_calibracion, (XtPointer) BOTON_SHOW_AJUSTE);
   XmStringFree (cadena);
   
	// Toggle button para el autozoom
   cadena = XmStringCreateLocalized ("Automatic\n  Zoom");
   i = 0;
   XtSetArg (args[i], XmNset, false); i++;
   XtSetArg (args[i], XmNlabelString, cadena); i++;
   XtSetArg (args[i], XmNfontList, lista_fuentes); i++;
	show_autoZoom_calibracion = XmCreateToggleButton (show_rowcol_calibracion, "show_autoZoom_calibracion", args, i);
	XtAddCallback (show_autoZoom_calibracion, XmNvalueChangedCallback, pulsa_show_calibracion, (XtPointer) BOTON_SHOW_AUTOZOOM);
   XmStringFree (cadena);

	// Toggle button para la gráfica de dispersión
   cadena = XmStringCreateLocalized ("Fit Graph");
   i = 0;
   XtSetArg (args[i], XmNset, false); i++;
   XtSetArg (args[i], XmNlabelString, cadena); i++;
   XtSetArg (args[i], XmNfontList, lista_fuentes); i++;
	show_dispersion_calibracion = XmCreateToggleButton (show_rowcol_calibracion, "show_dispersion_calibracion", args, i);
	XtAddCallback (show_dispersion_calibracion, XmNvalueChangedCallback, pulsa_show_calibracion, (XtPointer) BOTON_SHOW_DISPERSION);
   XmStringFree (cadena);
	//
	
   XtVaSetValues (show_frame_calibracion,
		XmNbottomAttachment, XmATTACH_WIDGET, XmNbottomWidget, boton_calibracion,
		XmNtopAttachment, XmATTACH_FORM,
		XmNtopOffset, 10,
		XmNrightAttachment, XmATTACH_FORM,
		XmNrightOffset, 10,
		XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET, XmNleftWidget, boton_precalibra,
		NULL);
		


	XtManageChild(label_frame_calibracion);
	XtManageChild(show_dispersion_calibracion);
	XtManageChild(show_autoZoom_calibracion);
	XtManageChild(show_adjust_calibracion);
	XtManageChild(show_rowcol_calibracion);
	XtManageChild(show_frame_calibracion);

	// **** FIN OPCIONES DE REPRESENTACION ***

   // Crea seccion resultados de ajuste
   i = 0;
	XtSetArg (args[i], XmNorientation, XmVERTICAL); i++;
	XtSetArg (args[i], XmNentryAlignment, XmALIGNMENT_CENTER); i++;
   result_frame_calibracion = XmCreateRowColumn (fondo_calibracion, "result_frame_calibracion", args, i);
   cadena = XmStringCreate ("Calibration List", "BoldNormal");
   i = 0;
   XtSetArg (args[i], XmNfontList, lista_fuentes); i++;
   XtSetArg (args[i], XmNlabelString, cadena); i++;
   label_result_frame_calibracion = XmCreateLabelGadget (result_frame_calibracion, "label_result_frame_calibracion", args, i);
   XmStringFree (cadena);
	i = 0;
	XtSetArg (args[i], XmNlistSizePolicy, XmCONSTANT); i++;
//	XtSetArg (args[i], XmNselectionPolicy, XmSINGLE_SELECT); i++;
	XtSetArg (args[i], XmNselectionPolicy, XmEXTENDED_SELECT); i++;
	XtSetArg (args[i], XmNvisibleItemCount, 9); i++;
	XtSetArg (args[i], XmNscrollBarDisplayPolicy, XmSTATIC); i++;
   XtSetArg (args[i], XmNfontList, lista_fuentes); i++;
	result_lista_calibracion = XmCreateScrolledList(result_frame_calibracion, "result_lista_calibracion", args, i);
//	XtAddCallback (result_lista_calibracion, XmNsingleSelectionCallback, pulsa_lista_result_calibracion, NULL);
	XtAddCallback (result_lista_calibracion, XmNextendedSelectionCallback, pulsa_lista_result_calibracion, NULL);
	XtAddCallback (result_lista_calibracion, XmNdefaultActionCallback, pulsa_edit_result_calibracion, NULL);
	i = 0;
	XtSetArg (args[i], XmNfractionBase, 2); i++;
	bt_form_result_calibracion = XmCreateForm(result_frame_calibracion, "bt_form_result_calibracion", args, i);
	i = 0;
   cadena = XmStringCreateLocalized ("Del");
   XtSetArg (args[i], XmNlabelString, cadena); i++;
	XtSetArg (args[i], XmNalignment, XmALIGNMENT_CENTER); i++;
   XtSetArg (args[i], XmNfontList, lista_fuentes); i++;
	bt_del_result_calibracion = XmCreatePushButton(bt_form_result_calibracion, "bt_del_result_calibracion", args, i);
	XtAddCallback (bt_del_result_calibracion, XmNactivateCallback, pulsa_del_result_calibracion, NULL);
   XmStringFree (cadena);
	i = 0;
   cadena = XmStringCreateLocalized ("Edit");
   XtSetArg (args[i], XmNlabelString, cadena); i++;
	XtSetArg (args[i], XmNalignment, XmALIGNMENT_CENTER); i++;
   XtSetArg (args[i], XmNfontList, lista_fuentes); i++;
	bt_edit_result_calibracion = XmCreatePushButton(bt_form_result_calibracion, "bt_edit_result_calibracion", args, i);
	XtAddCallback (bt_edit_result_calibracion, XmNactivateCallback, pulsa_edit_result_calibracion, NULL);
   XmStringFree (cadena);

   XtVaSetValues (bt_del_result_calibracion,
		XmNleftAttachment, XmATTACH_FORM,
		XmNtopAttachment, XmATTACH_FORM,
		XmNrightAttachment, XmATTACH_POSITION, XmNrightPosition, 1,
		XmNleftOffset, 10, XmNrightOffset, 10,
		XmNbottomAttachment, XmATTACH_FORM,
		NULL);
   XtVaSetValues (bt_edit_result_calibracion,
		XmNleftAttachment, XmATTACH_POSITION, XmNleftPosition, 1,
		XmNleftOffset, 10, XmNrightOffset, 10,
		XmNtopAttachment, XmATTACH_FORM,
		XmNrightAttachment, XmATTACH_FORM,
		XmNbottomAttachment, XmATTACH_FORM,
		NULL);
	XtVaSetValues (bt_form_result_calibracion,
		XmNleftAttachment, XmATTACH_FORM,
		XmNrightAttachment, XmATTACH_FORM,
		XmNbottomAttachment, XmATTACH_FORM,
		NULL);
	XtVaSetValues(result_lista_calibracion,
		XmNbottomAttachment, XmATTACH_WIDGET, XmNbottomWidget, bt_form_result_calibracion,
		NULL);
   XtVaSetValues (result_frame_calibracion,
		XmNbottomAttachment, XmATTACH_FORM,
		XmNbottomOffset, 20,
		XmNtopAttachment, XmATTACH_FORM,
		XmNtopOffset, 10,
		XmNrightAttachment, XmATTACH_WIDGET, XmNrightWidget, show_frame_calibracion,
		XmNrightOffset, 10,
		XmNleftAttachment, XmATTACH_WIDGET, XmNleftWidget, interv_frame_calibracion,
		XmNleftOffset, 10,
		NULL);

	XtManageChild(label_result_frame_calibracion);
	XtManageChild(result_lista_calibracion);
	XtManageChild(bt_del_result_calibracion);
	XtManageChild(bt_edit_result_calibracion);
	XtManageChild(bt_form_result_calibracion);
	XtManageChild(result_frame_calibracion);

	printf("Colocamos los widgets\n");

	// coloca los widgets
   XtVaSetValues (linea_calib_1,
		XmNleftAttachment, XmATTACH_FORM,
		XmNtopAttachment, XmATTACH_FORM,
		XmNrightAttachment, XmATTACH_FORM,
		NULL);
   XtVaSetValues (linea_calib_2,
		XmNleftAttachment, XmATTACH_FORM,
		XmNrightAttachment, XmATTACH_FORM,
		XmNbottomAttachment, XmATTACH_FORM,
		NULL);
   XtVaSetValues (label_form_calibracion,
		XmNleftAttachment, XmATTACH_FORM,
		XmNbottomAttachment, XmATTACH_WIDGET, XmNbottomWidget, linea_calib_2,
		XmNleftOffset, 10,
		NULL);
   XtVaSetValues (fondo_calibracion,
		XmNrightAttachment, XmATTACH_FORM,
		XmNleftAttachment, XmATTACH_FORM,
		XmNtopAttachment, XmATTACH_FORM,
		XmNbottomAttachment, XmATTACH_FORM,
		NULL);

	Crea_DialogPrecalibracion();
	
	//if(!EstadoTrabajo.Calibracion)	XtSetSensitive(show_dispersion_calibracion, false);

	return fondo_calibracion;
}

/******************************************************************************
 ******
 ******                             CALIBRACION
 ******
 ******************************************************************************/
 
//********************** RECONSTRUYE LISTA CALIBRACIÓN **************************
 /** Construye la lista que se utiliza para mostrar todos los intervalos que se pueden calibrar
 */
void ReconstruyeListaCalibracion() 
	{
	char texto_canales[80], extra[60], aconsejado[3];
	XmString energia;

	printf("RECONSTRUYELISTACALIBRACION\n");
	printf("Borramos todos los objetos de la lista\n");
	XmListDeleteAllItems(result_lista_calibracion);
	printf("Para cada elemento\n");	
	for (int i = 1; i <= listaCalibracion.numElementos; i++) 
		{
		// coge cada elemento en la lista de calibracion y lo
		// convierte en una entrada en el widget lista de calibracion
		printf("Cogemos el pico adecuado\n");
		NodoCalibracion *nodo = DevuelveNodo(listaCalibracion, i);
		NodoPicos *intervalo = DevuelveNodo(Datos.listaPicos, nodo->numIntervalo);
		//NOTA: En la calibración sólo consideramos un pico por intervalo
		NodoPico *PicoCalibracion = DevuelveNodo(intervalo->picos, 1);
		
		if (PicoCalibracion->Ajuste.identificado) 
			{
			printf("El pico está identificado\n");
			XmString elemento;
			if (LaraBD.GetIsRunning()) 
				{
				for(int i=0;i<PicoCalibracion->Ajuste.emisiones->numElements();i++){
					printf("Tomamos el número de emisión\n");
					NodoEmision *ne=(NodoEmision *)(PicoCalibracion->Ajuste.emisiones->getNode(i));
					unsigned short int codEmision = ne->codElemento;
		//			printf("El código del elemento es %d", codEmision);
			//		printf("Y tomamos el elemento emitido\n");
					elemento = XmStringNameElement(LaraBD.GetEmissor(codEmision), "SubIndiceBold", "BoldNormal");
		//			printf("FELIPE:%s %d\n", elemento, codEmision);
					// minimize decimals of probability
					char format[50];
					strcpy(format, "  %.3f keV (%.1f%%)");
					double Prob = LaraBD.GetProbability(codEmision);
					printf("FELIPE1\n");

					int rateProb = orden(Prob)-1;
					if ((Prob > 0)&&((rateProb < -1)&&(rateProb > -10)))	
						format[14] = (unsigned char) 0x30 + abs(rateProb);
					double ener=ne->Energia;
					//printf("FELIPE20\n");
					//printf("extra:%s\n", extra);
					//printf("energia:%f\n", ener);
					//printf("probabilidad:%f\n", Prob);
					sprintf(extra, format, ener, LaraBD.GetProbability(codEmision));		
					//sprintf(extra, format, energia, Prob);		
					//printf("FELIPE200\n");
					}				
				//printf("FELIPE21\n");
				} 
			else 	
				{				elemento = XmStringCreate ("???", "BoldNormal");
				// minimize decimals of probability
				char *format = "  %.3f keV (??%%)";
					
				//sprintf(extra, format, PicoCalibracion->Ajuste.Energia);

				for(int i=0;i<PicoCalibracion->Ajuste.emisiones->numElements();i++)
					{
					NodoEmision *ne=(NodoEmision *)(PicoCalibracion->Ajuste.emisiones->getNode(i));
					double ener=ne->Energia;
					sprintf(extra, format, ener);//NOTA: Esto creo que lo sustituirá, no lo añadirá
					}
				}
			//printf("FELIPE22\n");
			XmString emision = XmStringCreate (extra, "BoldNormal");
			// concat element and emision info
			energia = XmStringConcat(elemento, emision);
			//XmStringFree(elemento); XmStringFree (emision);
			//printf("FELIPE23\n");
			}
		else 	
			{
			int minEnergia = (int) floor(Datos.DaEnergia((double)intervalo->limInferior));
			int maxEnergia = (int) floor(Datos.DaEnergia((double)intervalo->limSuperior))+1;
			sprintf(extra, "[%d-%d] keV", minEnergia, maxEnergia);
			energia = XmStringCreateLocalized (extra);
			}
		/* si el pico cumple unas condiciones de relación entre sigma y tau
			lo aconseja para calibracion señalandolo con un asterisco. Las
			condiciones de calibracion son las que se dan en la inicializacion
			del pico: tau +/- = sigma^2 / 4. Cualquier desviacion de esto indica
			la presencia de otro pico o poca estadistica para que el ajuste
			sea fiable. */
		
		double relacion = powi(PicoCalibracion->Ajuste.Anchura, 2)/4.0;
		relacion = fabs(relacion - PicoCalibracion->Ajuste.Tau)/PicoCalibracion->Ajuste.Tau;
		if (relacion < 0.1) sprintf(aconsejado, " *"); // un 10% de desvio
		else sprintf(aconsejado, "");
		printf("FELIPE3\n");
		sprintf(texto_canales, " in [%d-%d]%s", intervalo->limInferior, intervalo->limSuperior, aconsejado);
		XmString cadena = XmStringCreateLocalized (texto_canales);
		XmString final = XmStringConcat(energia, cadena);
		XmListAddItemUnselected(result_lista_calibracion, final, 0);
		//XmStringFree (cadena); XmStringFree (energia); XmStringFree (final);
			

	}
}

void BorraListaIntervCalibracion() 
	{
	// borra los intervalos que hubiese indicados
	XmListDeleteAllItems(interv_lista_calibracion);	
	}

//******************** RECONSTRUYELISTAINTERVCALIBRACION *********************
/*	Añade a la lista de intervalos todos los descubiertos en el cálculo del
	continuo
	*/
void ReconstruyeListaIntervCalibracion() 
	{
	printf("RECONSTRUYELISTAINTERVCALIBRACION\n");
	char texto_interv[20];
	XmString cadena;
	// borra los intervalos que hubiese indicados
	XmListDeleteAllItems(interv_lista_calibracion);
	// pasa por todos los intervalos e introduce los que solo contienen un pico
	printf("Número de intervalos: %d\n", Datos.listaPicos.numElementos);
	for (int i = 1; i <= Datos.listaPicos.numElementos; i++) 
		{
		NodoPicos *intervPicos = DevuelveNodo(Datos.listaPicos, i);
		if (intervPicos->picos.numElementos == 1) 
			{
			int minEnergia = (int) floor(Datos.DaEnergia((double)intervPicos->limInferior));
			int maxEnergia = (int) floor(Datos.DaEnergia((double)intervPicos->limSuperior))+1;
			sprintf(texto_interv, " %4d - %4d", minEnergia, maxEnergia);
			cadena = XmStringCreateLocalized (texto_interv);
			XmListAddItemUnselected(interv_lista_calibracion, cadena, 0);
			XmStringFree (cadena);			
			}
		}
	}


bool IsActiveCalibration() 
	{
   	return XtIsSensitive(result_frame_calibracion);
	}

void ActivaCalibracion () 
   {
   XtSetSensitive(interv_frame_calibracion, true);
   XtSetSensitive(result_frame_calibracion, true);
   XtSetSensitive(show_frame_calibracion, true);
   XtSetSensitive(boton_calibracion, true);
   }

void DesactivaCalibracion () 
   {
   XtSetSensitive(interv_frame_calibracion, false);
   XtSetSensitive(result_frame_calibracion, false);
   XtSetSensitive(show_frame_calibracion, false);
   XtSetSensitive(boton_calibracion, false);
   }

bool IsInListaCalibracion(int numIntervalo) 
    {
	bool presente = false; int i = 0;
	while ((!presente)&&(i < listaCalibracion.numElementos)) 
		{
		i++;
		NodoCalibracion *nodo = DevuelveNodo(listaCalibracion, i);
		if (nodo->numIntervalo == numIntervalo) presente = true;
	    }
	return presente;
    }

tListaCalibracion* DevuelveListaCalibracion() 
	{
	return &listaCalibracion;
	}

void CopiaListaCalibracion(tListaCalibracion lista) 
	{
	borraLista(listaCalibracion);
	listaCalibracion = lista;
	}

//************* REPONE LISTA CALIBRACION ***********
// Llama a ReconstruyeListaCalibración()
void ReponeListaCalibracion() 
	{
	// busca el elemento seleccionado
	int *posiciones, numSel, nPos;

	if (!XmListGetSelectedPos(result_lista_calibracion, &posiciones, &numSel))	nPos = 0;
	else 	
		{
		nPos = *posiciones;
		XtFree((char *)posiciones);
		}

	// Extraño error, mas de un pico seleccionado
	if (numSel > 1) nPos = 0;
	printf("Llamamos a reconstruyeListaCalibración\n");
	ReconstruyeListaCalibracion();
	
	// Vuelve a poner el elemento seleccionado
	if (nPos > 0) 
		{
		XmListSelectPos(result_lista_calibracion, nPos, false);
		XmListSetBottomPos(result_lista_calibracion, nPos);
		}
	}


/*****************************
 ******   CALLBACKS
 *****************************/


/******************************************************************************
 ******
 ******                             INTERVALOS
 ******
 ******************************************************************************/

int PosicionIntervaloPicoUnico(int posLista) 
	{
	// busca el intervalo con los datos de ese pico
	int i = 1; // contador general
	NodoPicos *intervPicos = DevuelveNodo(Datos.listaPicos, i);
	// busca el primer intervalo con un pico solo
	while (intervPicos->picos.numElementos > 1) 
		{
		i++;
		intervPicos = DevuelveNodo(Datos.listaPicos, i);
		}
	for (int k = 0; k < posLista;) 
		{
		intervPicos = DevuelveNodo(Datos.listaPicos, i);
		if (intervPicos->picos.numElementos == 1) k++;
		i++;
		}
	return --i;
	}

void pulsa_lista_interv_calibracion(Widget widget, XtPointer client_data, XtPointer call_data) 
	{
   	XmListCallbackStruct *cbs = (XmListCallbackStruct *) call_data;
	
	if (EstadoVisual.AutoZoomCalibracion) 
		{
		int limInferior, limSuperior;
		if (cbs->reason == XmCR_EXTENDED_SELECT) 
			{
			// se pueden haber seleccionado 1 o mas elementos
			// se escoge el limite inferior del primer elemento
			int posInterval = PosicionIntervaloPicoUnico(cbs->selected_item_positions[0]);
			NodoPicos *intervalo = DevuelveNodo(Datos.listaPicos, posInterval);
			limInferior = intervalo->limInferior;
			// se escoge el limite superior del ultimo elemento
			posInterval = PosicionIntervaloPicoUnico(cbs->selected_item_positions[cbs->selected_item_count-1]);
			intervalo = DevuelveNodo(Datos.listaPicos, posInterval);
			limSuperior = intervalo->limSuperior;
			}
		else 	
			{
			int posInterval = PosicionIntervaloPicoUnico(cbs->selected_item_positions[0]);
			NodoPicos *intervalo = DevuelveNodo(Datos.listaPicos, posInterval);
			limInferior = intervalo->limInferior;
			limSuperior = intervalo->limSuperior;
			}
		// muestra 3 veces el tamaño del intervalo
		// para que se aprecien las estructuras coetaneas
		int ancho = 2*(limSuperior - limInferior)/3;
		MarcaZoom(limInferior - ancho, limSuperior + ancho);
		}
	}

//**************************** PULSA ADD INTERV CALIBRACION *****************************
// Se activa cuando pulsamos el botón  "=>" para añadir un intervalo a la calibración.
// Toda esta interfaz no aparece hasta que no ha terminado el ajuste del continuo.
// Los intervalos qu aparecen en la calibración son picos, en keV (energía), no en su posición
// por canales o número de cuentas.
// NOTA: Parece que no salen todos los picos, sino sólo aquéllos que se encuentran entre dos 
//	zonas de continuo.

void pulsa_add_interv_calibracion(Widget widget, XtPointer client_data, XtPointer call_data) 
	{
	// Busca el intervalo que se quiere añadir
	printf("PULSA ADD INTERV CALIBRACIÓN\n");
	int *posiciones, numSel, nPos = 0;
	//Esta llamada nos da el número intervalos seleccionados y sus posiciones en la lista
	if (!XmListGetSelectedPos(interv_lista_calibracion, &posiciones, &numSel)) 
		{
		Aviso(widget, "You must select an interval.");
		return;
		}

	// por si la operacion dura mucho tiempo 
	// NOTA: No suele tardar mucho, igual estaría bien hacerlo en el "adjust"
	PonCursorEspera();

	// Indica cambio en el trabajo
	EstadoTrabajo.TrabajoCambios = true;
	PonTituloFichCambiado();

	// Para cada intervalo seleccionado hace el ajuste
	int posLista = 0, numIntervalo = 1;
	while (posLista < numSel) 
		{
		printf("Hacemos otro ajuste\n");
		nPos = posiciones[posLista];
		// busca el intervalo con los datos de ese pico
		// OPTIMIZAR: en vez de volver a contar desde el principio podria
		// aprovecharse del lugar al que ha llegado contando los intervalos
		numIntervalo = 1; // contador general inicializado al principio
		// listaPicos fue modificada en BuscaCanalesContinuo que ocurría durante el "Search",
		// añadiéndole todos los picos que se encontraban entre dos zonas de fondo
		NodoPicos *intervaloPicos = DevuelveNodo(Datos.listaPicos, numIntervalo);
		
		// busca el primer intervalo con pico solo
		while (intervaloPicos->picos.numElementos > 1) 
			{
			numIntervalo++;
			intervaloPicos = DevuelveNodo(Datos.listaPicos, numIntervalo);
			}
		for (int k = 0; k < nPos;) 
			{
			intervaloPicos = DevuelveNodo(Datos.listaPicos, numIntervalo);
			if (intervaloPicos->picos.numElementos == 1) k++;
			numIntervalo++;
			}
		numIntervalo--;
		printf("Terminamos los ajustes, marcamos como no inicializado\n");
		// marca el pico como no inicializado y procede a su ajuste
		intervaloPicos->picos.inicio->Ajuste.inicializado = false;
		//intervaloPicos->picos.inicio->Ajuste.Energia = 0.0;
		//intervaloPicos->picos.inicio->Ajuste.codElemento = 0;
		printf("----------------------------------Vamos a borrar emisiones\n");
		if(intervaloPicos->picos.inicio->Ajuste.identificado)	
			{	
			printf("Vamos a hacerlo\n");				
			intervaloPicos->picos.inicio->Ajuste.emisiones->deleteAll();
			printf("Lo hemos hecho\n");
			}

		printf("emisiones liberadas\n");
		intervaloPicos->picos.inicio->Ajuste.calibrado = false;
		intervaloPicos->picos.inicio->Ajuste.identificado = false;	
		
		//			
		intervaloPicos->picos.inicio->Ajuste.eficienciaCalculada = false;	
		intervaloPicos->picos.inicio->Ajuste.ldCalculado = false;	
		intervaloPicos->picos.inicio->Ajuste.analizado = false;
		//
			
		//AJUSTE DEL PICO
		printf("Ajustando el pico %d\n", numIntervalo);
		int nError = Datos.AjustePico(numIntervalo, 1, EstadoTrabajo.Calibracion, true);
		printf("Área: %f(%f)\n", intervaloPicos->picos.inicio->Ajuste.area, intervaloPicos->picos.inicio->Ajuste.incerArea);
		//
		switch (nError) 
			{
			case -1:
				PonCursorNormal();
				Aviso(fondo_calibracion, "General error");
				PonCursorEspera();
				printf(ERROR_CALIBRATION);
				printf(ERROR_GENERAL);
				break; // continua con el resto de intervalos
			case -2:
				PonCursorNormal();
				Aviso(fondo_calibracion, "No Memory Avalaible");
				printf(ERROR_CALIBRATION);
				printf(ERROR_NO_MEMORY);
				XtFree ((char *)posiciones);
				return;
			}

		// El ajuste es correcto por lo que lo añade en la lista de
		// picos para la calibracion
		NodoCalibracion *nodo = new NodoCalibracion;
		nodo->numIntervalo = numIntervalo;
		nPos = agnadeNodo(listaCalibracion, nodo);
		
		posLista++;
		}

	XtFree ((char *)posiciones); // elimina la lista de intervalos seleccionados
	ReconstruyeListaCalibracion(); // Ajusta la lista de calibracion para que 
					// represente los nuevos intervalos agnadidos

	// si solo se ha seleccionado un intervalo lo pone tambien como
	// seleccionado en la lista de resultados
	if (numSel == 1) 
		{
		XmListSelectPos(result_lista_calibracion, nPos, false);
		XmListSetBottomPos(result_lista_calibracion, nPos);
		}

	
	// Actualiza los datos de los picos en la grafica
	printf("Pulsa Add interv calib llama a CopiaAjuste a Grafica\n");
	int nError = CopiaAjustesAGrafica();
	// Quita el cursor en espera
	PonCursorNormal();
	if (nError < 0) 
		{
		Aviso(fondo_calibracion, "No Memory Avalaible");
		printf(ERROR_CALIBRATION);
		printf(ERROR_NO_MEMORY);
		return;
		}
	
	ActivaAjustesGrafica(EstadoVisual.AjusteCalibracion);
	if (EstadoVisual.AjusteCalibracion)	PintaGrafica(0);
		
	//Dejamos el foco del teclado en la lista de intervalos:
	XmProcessTraversal(interv_lista_calibracion, XmTRAVERSE_CURRENT);
	XmProcessTraversal(interv_lista_calibracion, XmTRAVERSE_CURRENT);
	}

/******************************************************************************
 ******
 ******                             RESULTADOS
 ******
 ******************************************************************************/

//************************ PULSA LISTA RESULT CALIBRACIÓN ************************
// Cuando pulsamos un intervalo de la lista de intervalos, si está el auto-zoom activado,
// ampliamos esa zona.
void pulsa_lista_result_calibracion(Widget widget, XtPointer client_data, XtPointer call_data) {
   XmListCallbackStruct *cbs = (XmListCallbackStruct *) call_data;
	
	if (XmListPosSelected(result_lista_calibracion, cbs->item_position)) 
		{
		// recoge el intervalo seleccionado
		NodoCalibracion *nodo = DevuelveNodo(listaCalibracion, cbs->item_position);
		NodoPicos *intervalo = DevuelveNodo(Datos.listaPicos, nodo->numIntervalo);

		// si la opcion de autoZoom esta activada amplia la zona
		if (EstadoVisual.AutoZoomCalibracion) 
			{
			// muestra 3 veces el tamaño del intervalo
			// para que se aprecien las estructuras coetaneas
			int ancho = 2*(intervalo->limSuperior - intervalo->limInferior)/3;
			MarcaZoom(intervalo->limInferior - ancho, intervalo->limSuperior + ancho);
			}
	}
}

//************************* PULSA DEL RESULT CALIBRACIÓN ********************************
// Se activa cuando se pulsa el boton "Del" para borrar un intervalo de calibración.
void pulsa_del_result_calibracion(Widget widget, XtPointer client_data, XtPointer call_data) 
	{
	// busca el elemento seleccionado
	int *posiciones, numSel, nPos = 0;
	if (!XmListGetSelectedPos(result_lista_calibracion, &posiciones, &numSel)) 
		{
		Aviso(widget, "You must select an interval to delete.");
		return;
		}
	nPos = *posiciones;
	XtFree((char *)posiciones);
	// Extraño error, mas de un pico seleccionado
	if (numSel > 1) 
		{
		Aviso(widget, "Too much intervals selected.");
		printf(ERROR_CALIBRATION);
		printf(ERROR_TOO_INTERVALS);
		return;
		}	
	
	NodoCalibracion *nodo = DevuelveNodo(listaCalibracion, nPos);
	
	// Si este pico no se encuentra en la lista de analisis borra
	// los datos de ajuste de ese pico
	if (!IsInListaAnalisis(nodo->numIntervalo))	Datos.BorraIntervalo(nodo->numIntervalo);

	// Indica cambio en el trabajo
	EstadoTrabajo.TrabajoCambios = true;
	PonTituloFichCambiado();

	// Actualiza los datos de los picos en la grafica
	printf("PulsaDelResult calibración llama a CAG\n");
	int nError = CopiaAjustesAGrafica();
	if (nError < 0) 
		{
		Aviso(fondo_calibracion, "No Memory Avalaible");
		printf(ERROR_CALIBRATION);
		printf(ERROR_NO_MEMORY);
		return;
		}

	ActivaAjustesGrafica(EstadoVisual.AjusteCalibracion);
	if (EstadoVisual.AjusteCalibracion)
		PintaGrafica(0);
	
	// recoge cual era el primer item que se veia
	int topPosition, visiblePositions;
	XtVaGetValues(result_lista_calibracion,
			XmNtopItemPosition, &topPosition,
			XmNvisibleItemCount, &visiblePositions,
			NULL);
					
	// quita el intervalo de la lista de picos de calibracion
	printf("QUITA NODO^^^^^^^^^^\n");
	quitaNodo(listaCalibracion, nPos);
	ReconstruyeListaCalibracion();
	
	
	// intenta poner el mismo elemento como seleccionado
	// si no, pone el anterior, y si no hay ninguno no hace nada
	if (listaCalibracion.numElementos > 0) 
		{
		if ((nPos == topPosition)&&(nPos > 1))
				XmListSetPos(result_lista_calibracion, nPos-1);
		else if ((topPosition + visiblePositions) > listaCalibracion.numElementos)
			XmListSetBottomPos(result_lista_calibracion, listaCalibracion.numElementos);
		else
			XmListSetPos(result_lista_calibracion, topPosition);
		if (nPos > listaCalibracion.numElementos) nPos = 0;
		XmListSelectPos(result_lista_calibracion, nPos, true);
		}

	//Dejamos el foco del teclado en la lista de picos:
	XmProcessTraversal(result_lista_calibracion, XmTRAVERSE_CURRENT);
	XmProcessTraversal(result_lista_calibracion, XmTRAVERSE_CURRENT);
	}

//******************** PULSA EDIT RESULT CALIBRACION ***********************
// Se activa cuando se pulsa el botón "Edit" de la interfaz de calibración
void pulsa_edit_result_calibracion(Widget widget, XtPointer client_data, XtPointer call_data) 
	{
	printf("PULSA EDIT RESULT CALIBRACION\n");
	// busca el elemento seleccionado
	int *posiciones, numSel, nPos = 0;
	if (!XmListGetSelectedPos(result_lista_calibracion, &posiciones, &numSel)) 
		{
		Aviso(widget, "You must select an interval to edit.");
		return;
		}

	nPos = *posiciones;
	XtFree((char *)posiciones);
	// Extraño error, mas de un pico seleccionado
	// NOTA: No veo manera de que esto ocurra pues no deja seleccionar más que uno
	if (numSel > 1) 
		{
		Aviso(widget, "Too much intervals selected.");
		printf(ERROR_CALIBRATION);
		printf(ERROR_TOO_INTERVALS);
		return;
		}
	
	printf("Lanzamos cuadro de diálogo\n");
	// una vez identificado el nodo se lanza el cuadro de dialogo
	if (!dlg_edCalib) 	CreateDialogCalibracion();
	printf("Cuadro de diálogo lanzado\n");
	
	// Rellena los campos con los datos actuales
	char texto[40];
	XmString cadena, s1, s2;
	NodoCalibracion *nodo = DevuelveNodo(listaCalibracion, nPos);
	printf("NPOS es %d\n", nPos);
	NodoPicos *intervalo = DevuelveNodo(Datos.listaPicos, nodo->numIntervalo);
	NodoPico *pico = intervalo->picos.inicio; // hay 1 solo pico
	
	Datos.ActualizaIntervaloCalibracion(nodo->numIntervalo);
	// Actualiza los datos de los picos en la grafica
	printf("Pulsa Edit Result Calibración llama a CAAG\n");
	int nError = CopiaAjustesAGrafica();
	if (nError < 0) 
		{
		Aviso(fondo_calibracion, "No Memory Avalaible");
		printf(ERROR_CALIBRATION);
		printf(ERROR_NO_MEMORY);
		return;
		}
	
	ActivaAjustesGrafica(EstadoVisual.AjusteCalibracion);
	
	// escribe ChiSq
	s1 = XmStringCreate ("Chi\262:", "BoldNormal");
	sprintf(texto, " %.5f", intervalo->ChiSq);
	s2 = XmStringCreateLocalized(texto);
	cadena = XmStringConcat (s1, s2);
	XmStringFree(s1); XmStringFree(s2);
	XtVaSetValues (lbChiSq_adjust_edCalib, XmNlabelString, cadena, NULL);
	XmStringFree(cadena);
	
	// Escribe los parametros de la gaussiana
	sprintf(texto, "%.5f", pico->Ajuste.Centroide);
	XmTextSetString(txtCen_adjust_edCalib, texto);
	sprintf(texto, "%.3fkeV", Datos.DaEnergia(pico->Ajuste.Centroide));
	cadena = XmStringCreateLocalized (texto);
	XtVaSetValues (lbCenEn_adjust_edCalib, XmNlabelString, cadena, NULL);
	XmStringFree (cadena);
	sprintf(texto, "%.6f", pico->Ajuste.Anchura);
	XmTextSetString(txtSigma_adjust_edCalib, texto);
	sprintf(texto, "%.5fkeV", Datos.DaAncho(pico->Ajuste.Centroide, pico->Ajuste.Anchura));
	cadena = XmStringCreateLocalized (texto);
	XtVaSetValues (lbSigmaEn_adjust_edCalib, XmNlabelString, cadena, NULL);
	XmStringFree (cadena);
	
	// Escribe los parametros de la primera exponencial
	sprintf(texto, "%.6f", pico->Ajuste.Tau);
	XmTextSetString(txtTau_adjust_edCalib, texto);
	sprintf(texto, "%.5fkeV", Datos.DaAncho(pico->Ajuste.Centroide, pico->Ajuste.Tau));
	cadena = XmStringCreateLocalized (texto);
	XtVaSetValues (lbTauEn_adjust_edCalib, XmNlabelString, cadena, NULL);
	XmStringFree (cadena);
	sprintf(texto, "%.4f", pico->Ajuste.Ytau);
	XmTextSetString(txtYtau_adjust_edCalib, texto);

	AjusteSeguridad = pico->Ajuste;
	numIntervaloEdicion = nodo->numIntervalo;
	printf("Vamos a escribir los datos de identificación\n");
	// Escribe los datos de identificacion si existen
	if (pico->Ajuste.identificado)
		{
		//ActualizaIdentificacionCalibracion(pico->Ajuste.codElemento);
		for(int i=0;i<pico->Ajuste.emisiones->numElements();i++)
			{
			NodoEmision *ne=(NodoEmision *)(pico->Ajuste.emisiones->getNode(i));
			int codElemento=ne->codElemento;
			//ActualizaIdentificacionCalibracion(pico->Ajuste.codElemento);//NOTA: sólo uno
			ActualizaIdentificacionCalibracion(codElemento);//NOTA: sólo uno
			}
		}
	else 
		{
		// Pone los campos en vacio
		cadena = XmStringCreateLocalized (" ");
		XtVaSetValues (lbEnergy_energy_edCalib, XmNlabelString, cadena, NULL);
		XtVaSetValues (lbProb_energyEdCalib, XmNlabelString, cadena, NULL);
		XtVaSetValues (lbName_energy_edCalib, XmNlabelString, cadena, NULL);
		XmStringFree (cadena);
		}
	printf("Datos escritos, vamos a mostrar los candidatos para %d\n", nodo->numIntervalo);
	// Saca los candidatos de ese pico
//	CalibIdent.ShowCandidatesCalibracion(nodo->numIntervalo, 1);
	printf("NODO->NUMINTERVALO ES %d\n", nodo->numIntervalo);
	CalibIdent.ShowCandidates(nodo->numIntervalo, 1);
	printf("Candidatos mostrados\n");
	XtManageChild(dlg_edCalib);
	}


// **************************** PULSA SHOW CALIBRACION *******************************
// Se activa cuando se pulsa uno de los dos botones del formulario "Show", bien para mostrar
// el ajuste o para el zoom automático.
void pulsa_show_calibracion(Widget widget, XtPointer client_data, XtPointer call_data) 
	{
   XmToggleButtonCallbackStruct *state = (XmToggleButtonCallbackStruct *) call_data;

	printf("PULSA SHOW CALIBRACION %d\n", (int)client_data);
	switch ((int) client_data) 
		{
		case BOTON_SHOW_AJUSTE:
			EstadoVisual.AjusteCalibracion = state->set;
			EstadoVisual.AjusteTotalAnalisis = state->set;
			EstableceShowsAnalisis(EstadoVisual);
			ActivaAjustesGrafica(state->set);
			PintaGrafica(0);
			break;
		case BOTON_SHOW_AUTOZOOM:
			EstadoVisual.AutoZoomCalibracion = state->set;
			break;
		case BOTON_SHOW_DISPERSION:
			if(XmToggleButtonGetState(show_dispersion_calibracion)==false)//antes false
					XtUnmanageChild(gd);
			else	creaGraficaDispersion();
			break;
		}
	}

//******************* CREA GRÁFICA DISPERSIÓN ********************
/*	Crea una ventana con una pequeña gráfica de dispersión mostrando la 
	curva de calibración y los puntos tomados para la misma
	*/
void creaGraficaDispersion()
	{
    Arg		args[7];
    Cardinal	i = 0;
    XmString cadena;
	String	translations = "<Btn1Motion>: DrawingAreaInput() ManagerGadgetButtonMotion()\n<Btn1Up>:		DrawingAreaInput() ManagerGadgetActivate()\n<Btn1Down>:	DrawingAreaInput() ManagerGadgetArm()";
	printf("CREA GRAFICA DE DISPERSIÓN\n");
	// Inicia los datos
	const int height=400;
	const int width=400;
		
   //0) Crea el form fondo para todo este "Widget"
   i = 0;
 	XtSetArg (args[i], XmNtitle, "Fit Graph"); i++;//NOTA: No sé si se dice así
//	XtSetArg (args[i], XmNmwmDecorations, MWM_DECOR_BORDER|MWM_DECOR_TITLE|MWM_DECOR_MENU); i++;
//	XtSetArg (args[i], XmNmwmFunctions, MWM_FUNC_MOVE|MWM_FUNC_CLOSE|MWM_FUNC_MAXIMIZE); i++;
	
	XtSetArg (args[i], XmNmwmDecorations, MWM_DECOR_BORDER|MWM_DECOR_TITLE|MWM_DECOR_MENU); i++;
	XtSetArg (args[i], XmNmwmFunctions, MWM_FUNC_MOVE|MWM_FUNC_CLOSE|!(MWM_FUNC_RESIZE)); i++;
	
	XtSetArg (args[i], XmNbaseWidth, width); i++;
	XtSetArg (args[i], XmNbaseHeight, height); i++;
	XtSetArg (args[i], XmNmaxWidth, width*5); i++;
	XtSetArg (args[i], XmNmaxHeight, height*5); i++;
  	gd = XmCreateFormDialog (app_shell, "grafica_dispersion", args, i);
	
	//1) Creamos el área de dibujo
    i = 0;
	XtSetArg (args[i], XmNtranslations, XtParseTranslationTable (translations)); i++;
    graficaFit = XmCreateDrawingArea (gd, "graficaFit", args, i);

	//2) Creamos colores de fondo, frente y tamaño de línea
	GC gcDraw; // graphics context para dibujar
	XGCValues gcv;
	XColor cGrafFg;
	Screen *screen=XtScreen(graficaFit);
	Colormap  cmap = DefaultColormapOfScreen(screen);
		
	XParseColor(dpy, cmap, "rgb:FF/00/00", &cGrafFg); //red
	XAllocColor(dpy, cmap, &cGrafFg);
	gcv.foreground =  cGrafFg.pixel;
	XParseColor(dpy, cmap, "rgb:FF/FF/FF", &cGrafFg); //white
	XAllocColor(dpy, cmap, &cGrafFg);
	gcv.background = cGrafFg.pixel;
	gcv.line_width = (int)Preferences.GetLimitBigPoint();
	
   	XtVaSetValues (graficaFit, XmNbackground, cGrafFg.pixel, NULL);
   	XtVaSetValues (graficaFit, XmNforeground, BlackPixelOfScreen(XtScreen(graficaFit)), NULL);
    
	gcDraw = XCreateGC(dpy, RootWindowOfScreen(screen), GCForeground, &gcv);
	XtVaSetValues(graficaFit, XmNuserData, gcDraw, NULL);
	
	//3) Con este contexto, creamos un mapa de píxels en el que dibujaremos
	// tapiz virtual para dibujar (lo inicializa)
	Pixmap tapiz; // tapiz sobre el que se dibuja
	tapiz = XCreatePixmap(dpy, RootWindowOfScreen (screen),	1, 1, DefaultDepthOfScreen(screen));

	//
	cadena=XmStringCreate("Parameter: ", "BoldNormal");
	i=0;
	XtSetArg(args[i], XmNlabelString, cadena); i++;
	Widget etiqueta = XmCreateLabel (gd, "etiqueta", args, i);
	XmStringFree (cadena);

	int count=0;
	char *formats[]={"Energy", "Sigma", "Tau"};
	if(EstadoTrabajo.Calibracion)	count=XtNumber(formats);
	else							count=XtNumber(formats)-1;
	
	XmStringTable str_list;
	str_list=(XmStringTable)XtMalloc(count*sizeof(XmString *));
	for(int k=0;k<count;k++)	str_list[k]=XmStringCreateLocalized(formats[k]);
			
	i=0;
	XtSetArg(args[i], XmNitems, str_list); i++;
	XtSetArg(args[i], XmNitemCount, count); i++;
	XtSetArg(args[i], XmNvisibleItemCount, count); i++;
	desplegado=XmCreateDropDownList(gd, "desplegada", args, i);
	XtAddCallback(desplegado, XmNselectionCallback, seleccionaFormatoGrafica, NULL);

	for(int k=0;k<count;k++)	XmStringFree(str_list[k]);
	XtFree((char *) str_list);
	//
			
	XtAddCallback (graficaFit, XmNexposeCallback, redraw_calib, NULL);
 
	//3.b) Colocamos en el gráfico
	printf("Colocando etiqueta\n");
    XtVaSetValues (etiqueta,
	XmNtopAttachment, XmATTACH_FORM,XmNtopOffset,height-30,
	XmNleftAttachment, XmATTACH_FORM,XmNleftOffset, 10,
	NULL);

    XtVaSetValues (graficaFit,
	XmNtopAttachment, XmATTACH_FORM,XmNtopOffset, 1,
	XmNleftAttachment, XmATTACH_FORM,
	XmNrightAttachment, XmATTACH_FORM, XmNrightOffset, 1,
	XmNbottomAttachment, XmATTACH_FORM, XmNbottomOffset, 40,
	NULL);
	
	XtVaSetValues (desplegado,
	XmNtopAttachment, XmATTACH_FORM,XmNtopOffset,height-40,
	XmNleftAttachment, XmATTACH_FORM,XmNleftOffset, 100,
	NULL);
	
	//4) Mostramos los resultados
	XtManageChild(desplegado);
	XtManageChild(etiqueta);
	XtManageChild (graficaFit);
	XtManageChild(gd);
	}

//*************** SELECCIONA FORMATO *********************	
void seleccionaFormatoGrafica(Widget widget, XtPointer client_data, XtPointer call_data) 
	{
	printf("Redibujamos la gráfica\n");
	redraw_calib(graficaFit,NULL,NULL);
	}
	
//****************** REDRAW CALIB ***********************
/*	Dibujamos la gráfica de dispersión.
	*/
void redraw_calib(Widget w, XtPointer data, XtPointer cbk)
	{
	//0) Declaramos variables e iniciamos valores
    GC gc;
    Window win = XtWindow(w);

	//Para crear los distintos colores
	XGCValues gcv;
	XColor cGrafFg;
	Colormap  cmap = DefaultColormapOfScreen(XtScreen(w));
       
   	XtVaGetValues(w, XmNuserData, &gc, NULL);
	
	int height=300;
	int width=300;
	XtVaGetValues(w, XmNheight, &height, NULL);
	XtVaGetValues(w, XmNwidth, &width, NULL);
	
	int x=0, y=0;
	int xant=0, yant=0;
		
	//1)Limpiamos la gráfica
	XParseColor(dpy, cmap, "rgb:FF/FF/FF", &cGrafFg); //white
	XAllocColor(dpy, cmap, &cGrafFg);
	gcv.foreground =  cGrafFg.pixel;
	gc = XCreateGC(dpy, RootWindowOfScreen(XtScreen(w)), GCForeground, &gcv);
	XFillRectangle (dpy, win, gc, 0, 0, width, height);
	//
	
	//1.b) Determinamos el tipo de gráfica a generar
	//
	int seleccionado=0;
	XtVaGetValues(desplegado, XmNselectedPosition, &seleccionado, NULL);
	printf("Seleccionado vale %d\n", seleccionado);
	int nfil = Datos.numCanales;
	double datos[nfil];
	double chi;
	char etiqueta[12];
	switch(seleccionado)
		{
		case 0://Energía
			//for(int i=0;i<nfil;i++)	datos[i]=log(Datos.DaEnergia(i));
			for(int i=0;i<nfil;i++)	datos[i]=Datos.DaEnergia(i);
			chi=Datos.chiEnergia;
			strcpy(etiqueta,"E (keV)");
			break;
		case 1://Sigma
			//for(int i=0;i<nfil;i++)	datos[i]=Datos.DaFWHG(i);
			for(int i=0;i<nfil;i++)	datos[i]=Datos.DaFWHGCh(i);
			chi=Datos.chiSigma;
			//strcpy(etiqueta,"FWHG (keV)");
			strcpy(etiqueta,"FWHG (chn)");
			break;
		case 2://Tau
			//for(int i=0;i<nfil;i++)	datos[i]=Datos.DaTau(i);
			for(int i=0;i<nfil;i++)	datos[i]=Datos.DaTauCh(i);
			chi=Datos.chiTau;
			//strcpy(etiqueta, "Tau (keV)");
			strcpy(etiqueta, "Tau (chn)");
			break;
		default:
			printf("ERROR: Selección fuera del rango\n");
			Aviso(app_shell, "ERROR: Selection out of range\n");
			break;
		}
		
	printf("La chi vale %f\n", chi);
		
	//2)Determinamos dimensiones de la gráfica
	int lx = (int)width*4/5;
	int ly = (int)height*4/5;
	int hx = (int)width/10;
	int hy = (int)height/10;
  	
	int px[nfil];
	int py[nfil];
	double xmin=0;
    double ymin = datos[0];
	if(ymin<0)	ymin=0;
	double xmax = xmin, ymax = ymin;
	double tempx, tempy;
	
	// Valores maximos y minimos
	for(int i=0;i<nfil;i++)
		{
		tempx = i;
		//tempy = Datos.DaEnergia(i);
		//tempy = log(Datos.DaEnergia(i));
		tempy = datos[i];
		if(tempy<0)	tempy=0;
		xmax = tempx > xmax ? tempx : xmax;
		ymin = tempy < ymin ? tempy : ymin;
		ymax = tempy > ymax ? tempy : ymax;
		}
    ymin=xmin=0;
    printf("YMAX vale %f\n", ymax);
		
    // Factores de escala
	double fx = lx/(xmax-xmin);
	double fy = ly/(ymax-ymin);
	double dx = -xmin*fx;
	double dy = -ymin*fy;
	XPoint *puntos=new XPoint[nfil];
	
	//3)Pintamos los ejes
	XParseColor(dpy, cmap, "rgb:00/00/00", &cGrafFg); //black
	XAllocColor(dpy, cmap, &cGrafFg);
	gcv.foreground =  cGrafFg.pixel;
	gc = XCreateGC(dpy, RootWindowOfScreen(XtScreen(w)), GCForeground, &gcv);
	XDrawLine(dpy,win,gc, hx,hy, hx,hy+ly);//Línea vertical
	XDrawString(dpy,win,gc, hx-20,hy-10,etiqueta,strlen(etiqueta));
		
	//Escala en la línea vertical	
	double inc=0;
	char textoEn[5];
	int numMarcas=11;
	
	inc=ly/numMarcas;
	for(int i=1;i<numMarcas;i++)
		{
		XDrawLine(dpy,win,gc, hx-3,(int)(hy+ly-inc*i),  hx+3,(int)(hy+ly-inc*i) );//marcas horizontales
		if((i*ymax/numMarcas)<100)		sprintf(textoEn, "%.2f",(i*ymax/numMarcas));
		else							sprintf(textoEn, "%.0f",(i*ymax/numMarcas));
		XDrawString(dpy,win,gc, hx-30,(int)(hy+ly-inc*i),textoEn,strlen(textoEn));
		}

	sprintf(textoEn, "Chi² = %.3f", chi);
	XDrawString(dpy,win,gc, lx-30,hy+ly-30,textoEn,strlen(textoEn));
	XDrawLine(dpy,win,gc, hx,hy+ly, hx+lx,hy+ly);//Línea horizontal
	XDrawString(dpy,win,gc, lx,hy+ly-9,"chn (10³)",9);
	//Escala en la línea horizontal
	char texto[2];
	for(int i=1;i<9;i++)
		{
		inc=i*1000*lx/Datos.numCanales;
		XDrawLine(dpy,win,gc, (int)(hx+inc),hy+ly-3,  (int)(hx+inc),hy+ly+3);//marcas verticales
		sprintf(texto, "%d",i);
		XDrawString(dpy,win,gc, (int)(hx+inc-1),hy+ly+20,texto,1);
		}

	//4)Pintamos la curva/recta de calibración		
	XParseColor(dpy, cmap, "rgb:FF/00/00", &cGrafFg); //red
	XAllocColor(dpy, cmap, &cGrafFg);
	gcv.foreground =  cGrafFg.pixel;
	gc = XCreateGC(dpy, RootWindowOfScreen(XtScreen(w)), GCForeground, &gcv);
	// Cálculo de la tabla representable de puntos calibrados de energía
	for(int i=0;i<nfil;i++)
		{
		puntos[i].x = hx +(int)(dx + i*fx);
		puntos[i].y = hy+ly-(int)(dy + datos[i]*fy);
		if(puntos[i].y>(hy+ly-dy))	puntos[i].y=(int)(hy+ly-dy);
		}
	XDrawLines(dpy,win,gc,puntos, nfil, CoordModeOrigin);
	delete [] puntos;
		
	XParseColor(dpy, cmap, "rgb:00/00/FF", &cGrafFg); //blue
	XAllocColor(dpy, cmap, &cGrafFg);
	gcv.foreground =  cGrafFg.pixel;
	gc = XCreateGC(dpy, RootWindowOfScreen(XtScreen(w)), GCForeground, &gcv);

	// 5) Dibujo de los puntos usados para la calibración
	double calibracion;
	double sigmaCalibracion;
		
	for(int i=1;i<=listaCalibracion.numElementos;i++)
		{
		NodoCalibracion *nodo = DevuelveNodo(listaCalibracion, i);
		NodoPicos *intervalo = DevuelveNodo(Datos.listaPicos, nodo->numIntervalo);
//		NodoPico *pico = intervalo->picos.inicio;
		for(int j=1;j<=intervalo->picos.numElementos;j++)
			{
			//NodoPico *pico = intervalo->picos.inicio;
			NodoPico *pico = DevuelveNodo(intervalo->picos, j);
			printf("Pico %d tiene calibrado a %d\n", i, pico->Ajuste.calibrado);
			if (pico->Ajuste.calibrado)	
				{
				double energia=0.0;
				double incEnergia=0.0;
				switch(seleccionado)
					{
					case 0:
						calibracion = Datos.DaEnergia(pico->Ajuste.Centroide);
						sigmaCalibracion = pico->Ajuste.SigCentroide;
						//calibracion = log(Datos.DaEnergia(pico->Ajuste.Centroide));
						//sigmaCalibracion = pico->Ajuste.SigCentroide/Datos.DaEnergia(pico->Ajuste.Centroide);
						break;
					case 1:
						//calibracion=Datos.DaAncho(pico->Ajuste.Centroide, pico->Ajuste.Anchura);
						//sigmaCalibracion=Datos.DaAncho(pico->Ajuste.Centroide, pico->Ajuste.SigAnchura);
						calibracion=pico->Ajuste.Anchura;
						sigmaCalibracion=pico->Ajuste.SigAnchura;
						break;
					case 2:
						//calibracion=Datos.DaAncho(pico->Ajuste.Centroide, pico->Ajuste.Tau);
						//sigmaCalibracion=Datos.DaAncho(pico->Ajuste.Centroide, pico->Ajuste.SigTau);
						calibracion=pico->Ajuste.Tau;
						sigmaCalibracion=pico->Ajuste.SigTau;
						break;
					default:
						break;
					}				
				/*NodoEmision *ne=(NodoEmision *)(pico->Ajuste.emisiones->getFirst());//Sólo debería haber uno
				//energia = ne->Energia;
				energia = log(ne->Energia);
				incEnergia = LaraBD.GetIncEnergy(ne->codElemento);
				*/
				x = hx +(int)(dx + pico->Ajuste.Centroide*fx);
				//y = hy+ly-(int)(dy + energia*fy);
				y = hy+ly-(int)(dy + calibracion*fy);
				XFillRectangle (dpy, win, gc, x, y, 2, 2);
				//printf("Energía es %f\n", energia);
				printf("Calibracion es %f(%f)\n", calibracion, sigmaCalibracion);
				//Ahora dibujamos la barrita de error:
				//NOTA: La incertidumbre de la energía es muy pequeña!, apenas se nota en la calibración
				//printf("incEnergia vale %f\n", incEnergia);
				/*double eAlta=energia+incEnergia;
				int ya=hy+ly-(int)(dy + eAlta*fy);
				double eBaja=energia-incEnergia;
				int yb=hy+ly-(int)(dy + eBaja*fy);*/
				double eAlta=calibracion+sigmaCalibracion;
				int ya=hy+ly-(int)(dy + eAlta*fy);
				double eBaja=calibracion-sigmaCalibracion;
				int yb=hy+ly-(int)(dy + eBaja*fy);
				
				//printf("Pintamos línea de %d a %d. La incertidumbre es %f (en logarítmico %f)\n", ya,yb, incEnergia);
				XDrawLine(dpy,win,gc,x,yb,x,ya);
				XDrawLine(dpy,win,gc,x-5,ya,x+5,ya);
				XDrawLine(dpy,win,gc,x-5,yb,x+5,yb);
				}
		  	}
		}
	}
	
//******************* PULSA CALIBRACION **************************
/** Realiza las calibraciones de forma y/o de energía según usuario.
    Para la energía necesitamos tres picos calibrados
*/
void pulsa_calibracion(Widget widget, XtPointer client_data, XtPointer call_data) 
	{
	// busca si hay suficientes picos calibrados para hacer una calibracion
	// general
	int numCalibrados = 0;
	for (int i = 1; i <= listaCalibracion.numElementos; i++) 
		{
		NodoCalibracion *nodo = DevuelveNodo(listaCalibracion, i);
		NodoPicos *intervalo = DevuelveNodo(Datos.listaPicos, nodo->numIntervalo);
		NodoPico *pico = intervalo->picos.inicio;
		if (pico->Ajuste.calibrado)	numCalibrados++;
		}
	
	// si hay suficientes picos calibrados (al menos 3) procede a su ajuste

	// pregunta que calibracion quiere:
	// 	Energia y forma
	// 	Energia
	// 	Forma
	int respuesta = Elegir(fondo_calibracion, "What do you want to calibrate?", "Calibration",
							"Energy + Shape", "Energy", "Shape");
	//1=energía+forma	2=energía	3=forma	
	
	//NOTA: La calibración de la resolución: ¿La hacemos con la calibración de forma o de energía?
	// calibracion de la energia (energía + forma o energía) (se viene haciendo en la de forma)
	if ((respuesta == 1)||(respuesta == 2)) 
		{
		// si no hay suficientes picos da aviso
		if (numCalibrados < 3) 
			{
			// NOTA: Me parece más claro dar a entender que los que necesitamos son tres picos CALIBRADOS:
			// 	 Aviso(fondo_calibracion, "You need at least three --calibrated-- peaks to calibrate.");
			Aviso(fondo_calibracion, "You need at least three peaks to calibrate.");
			return;
			}
	
		int nError = Datos.CalibraEnergia(listaCalibracion);
		switch (nError) 
			{
			case -1:
				Aviso(fondo_calibracion, "General error");
				//printf("CalibraEnergia termina con
				printf(ERROR_CALIBRATION);
				printf(ERROR_GENERAL);
				return;
			case -2:
				Aviso(fondo_calibracion, "No Memory Avalaible");
				printf(ERROR_CALIBRATION);
				printf(ERROR_NO_MEMORY);
				return;
			}
		EstadoTrabajo.CalibracionEnergia = true;
		ActivaEnergiaDatos();
		
		PonPosicionCursor();
	
		// Indica cambio en el trabajo
		EstadoTrabajo.TrabajoCambios = true;
		PonTituloFichCambiado();
		printf("TERMINAMOS LA CALIBRACIÓN DE LA ENERGIA\n");
		}

	// Calibracion de los parametros de forma (energía + forma o forma)
	if ((respuesta == 1)||(respuesta == 3)) 
		{
		printf("EMPIEZA LA CALIBRACIÓN DE LA FORMA\n");
		//1) calibracion de la anchura
		int nError = Datos.CalibraFWHG(listaCalibracion);
		printf("calibración hecha con error %d\n", nError);
		switch (nError) 
			{
			case -1:
				Aviso(fondo_calibracion, "General error");
				printf(ERROR_CALIBRATION);
				printf(ERROR_GENERAL);
				return;
			case -2:
				Aviso(fondo_calibracion, "No Memory Avalaible");
				printf(ERROR_CALIBRATION);
				printf(ERROR_NO_MEMORY);
				return;
			}
	
		// Indica cambio en el trabajo
		EstadoTrabajo.TrabajoCambios = true;
		printf("Cambiando título fich\n");
		PonTituloFichCambiado();
		printf("calibramos tau\n");
		//2) calibracion de la tau
		nError = Datos.CalibraTau(listaCalibracion);
		printf("tau calibrado\n");
		switch (nError) 
			{
			case -1:
				Aviso(fondo_calibracion, "General error");
				printf(ERROR_CALIBRATION);
				printf(ERROR_GENERAL);
				return;
			case -2:
				Aviso(fondo_calibracion, "No Memory Avalaible");
				printf(ERROR_CALIBRATION);
				printf(ERROR_NO_MEMORY);
				return;
			}
			
		//----3) calibración de la resolución ------------
		printf("LLAMAMOS A DATOS.CALIBRARESOLUCION()\n");
		nError = Datos.CalibraResolucion(listaCalibracion);
		printf("RESOLUCIÓN CALIBRADA CON ERROR %d\n", nError);
		switch (nError) 
			{
			case -1:
				Aviso(fondo_calibracion, "General error");
				printf(ERROR_CALIBRATION);
				printf(ERROR_GENERAL);
				return;
			case -2:
				Aviso(fondo_calibracion, "No Memory Avalaible");
				printf(ERROR_CALIBRATION);
				printf(ERROR_NO_MEMORY);
				return;
			}
//		for(int i=0;i<8000;i+=1000)	printf("Res[%d]=%f(%f), %f(%f) en cans.\n", i, Datos.DaResolucion(i), Datos.DaIncerResolucion(i), Datos.DaResolucionCh(i), Datos.DaIncerResolucionCh(i));
		printf("FWHG para 46,54 es %f keV, %f canales\n", Datos.DaFWHG(Datos.DaCanal(46.54)), Datos.DaFWHGCh(Datos.DaCanal(46.54)));
		printf("Res[%f]=%f(%f)\n", 46.54, Datos.DaResolucionCh(Datos.DaCanal(46.54)), Datos.DaIncerResolucionCh(Datos.DaCanal(46.54)) );		
		printf("Res[%f]=%f(%f)\n", 63.29, Datos.DaResolucionCh(Datos.DaCanal(63.29)), Datos.DaIncerResolucionCh(Datos.DaCanal(63.29)) );		
		printf("Res[%f]=%f(%f)\n", 351.93, Datos.DaResolucionCh(Datos.DaCanal(351.93)), Datos.DaIncerResolucionCh(Datos.DaCanal(351.93)) );
		printf("Res[%f]=%f(%f)\n", 609.24, Datos.DaResolucionCh(Datos.DaCanal(609.24)), Datos.DaIncerResolucionCh(Datos.DaCanal(609.24)) );
		printf("Res[%f]=%f(%f)\n", 1460.82, Datos.DaResolucionCh(Datos.DaCanal(1460.82)), Datos.DaIncerResolucionCh(Datos.DaCanal(1460.82)) );
		//------------------------------------------------

		EstadoTrabajo.CalibracionForma = true;
		printf("activando forma y poniendo cursor\n");
		ActivaFormaDatos();
		PonPosicionCursor();
		printf("TERMINA la calibración de la forma\n");
		}
	
	if (EstadoTrabajo.CalibracionForma && EstadoTrabajo.CalibracionEnergia) 
		{
		EstadoTrabajo.Calibracion = true;
		//NOTA: No debemos activar el análisis si tenemos un fondo!
		printf("COMPROBANDO SI PODEMOS ACTIVAR EL ANÁLISIS, el fondo es %d\n", Datos.esFondo);
					//felipe. No se porqué tenía bloqueado que activara el análisis si era un fondo. Lo cambio para que se active en cualquier caso. 2010-05-31

		/*if(!Datos.esFondo)//Si no es fondo activamos el análisis
			{*/
			ActivaAnalisis();
			ReconstruyeListaIntervAnalisis();
			/*}
		else printf("NO activamos el análisis, estamos trabajando con un fondo\n");*/
		//
		}
		
/*	float funcion[Datos.numCanales];
//	for(int i=0;i<Datos.numCanales;i++)	funcion[i]=Datos.DaResolucion(i)*100000;//Nos da la función de gauss del mejor individuo para el parámetro j
	for(int i=0;i<Datos.numCanales;i++)	funcion[i]=Datos.DaResolucionCh(i)*100000;//Nos da la función de gauss del mejor individuo para el parámetro j
	for(int i=0;i<Datos.numCanales;i++)	funcion[i]=Datos.DaFWHG(i);//Nos da la función de gauss del mejor individuo para el parámetro j
//	for(int i=0;i<Datos.numCanales;i++)	funcion[i]=Datos.DaTau(i)*100;//Nos da la función de gauss del mejor individuo para el parámetro j
	CopiaErrorAGrafica(funcion, Datos.numCanales, 28);
	ActivaErrorGrafica(true, 28);
	PintaGrafica(0);*/
	}


/******************************************************************************
 ******
 ******                            EDICION CALIBRACION
 ******
 ******************************************************************************/

#include "Ecuation.xbm"
#include "HelpIcon.xbm"

	//********************** CREATE DIALOG CALIBRACIÓN **********************
/* Crea todo el cuadro de diálogo de la calibración, con 3 secciones:
	- Ajuste: todas las variables del ajuste y la chi cuadrado
	- Energías: relación de elementos a los que se puede referir la energía del pico
	- Botones:  ajustar, cancelar y aceptar
	
NOTA: Importante: se han suprimido todas las referencias a lista_fuentes porque nos dan un
	segmentation fault al realizar la creación de las etiquetas que lo usan.
	La culpa no es de la propia lista_fuentes si no más bien del tipo de fuente.
	Con el tipo interface user que usaba david no había ningún problema, pero este
	tipo no lo encuentra en Linux (para Unix funcionaba perfectamente)
	Lo cambié a times que sí lo encuentra, pero parece que luego no lo aceptan estas funciones	
	
	El resultado: los números de los elementos no salen como subíndices.
*/
void CreateDialogCalibracion() 
	{
   	Arg		args[7];
   	Cardinal	i = 0;
	XmString cadena;
	
	// *********************
	// * cuadro de dialogo *
	// *********************
	
	i = 0;
	XtSetArg (args[i], XmNtitle, CALIBRATION_EDIT_TITLE); i++;
	XtSetArg (args[i], XmNdialogStyle, XmDIALOG_FULL_APPLICATION_MODAL); i++;
	XtSetArg (args[i], XmNmwmDecorations, MWM_DECOR_BORDER|MWM_DECOR_TITLE|MWM_DECOR_MENU); i++;
	XtSetArg (args[i], XmNmwmFunctions, MWM_FUNC_MOVE); i++;
	dlg_edCalib = XmCreateFormDialog(app_shell, "dlg_edCalib", args, i);

	/*******************
	El cuadro de diálogo se separa en tres zonas básicas:
		-fr_adjust_edCalib: Datos del ajuste, con la gráfica y la fórmula para guiar
		-fr_energy_edCalib: Datos de la energía y elementos químicos asociados
		-fm_botones_edCalib: Botones de ok, cancel y adjust
	Todos tendrán como padre a dlg_edCalib, el cuadro de diálogo.
	*******************/
	i = 0;
	fr_adjust_edCalib = XmCreateFrame (dlg_edCalib, "fr_adjust_edCalib", args, i);
	fr_energy_edCalib = XmCreateFrame (dlg_edCalib, "fr_energy_edCalib", args, i);
	linea_edCalib = XmCreateSeparator(dlg_edCalib, "linea_edCalib", args, i);
	i = 0;
	XtSetArg (args[i], XmNfractionBase, 3); i++;
	fm_botones_edCalib = XmCreateForm (dlg_edCalib, "fm_botones_edCalib", args, i);	
   	XtVaSetValues (fm_botones_edCalib,
		XmNbottomAttachment, XmATTACH_FORM,
		XmNbottomOffset, 10,
		XmNrightAttachment, XmATTACH_FORM,
		XmNrightOffset, 10,
		XmNleftAttachment, XmATTACH_FORM,
		XmNleftOffset, 10,
		NULL);
   	XtVaSetValues (linea_edCalib,
		XmNbottomAttachment, XmATTACH_WIDGET, XmNbottomWidget, fm_botones_edCalib,
		XmNbottomOffset, 10,
		XmNrightAttachment, XmATTACH_FORM,
		XmNleftAttachment, XmATTACH_FORM,
		NULL);
   	XtVaSetValues (fr_energy_edCalib,
		XmNbottomAttachment, XmATTACH_WIDGET, XmNbottomWidget, linea_edCalib,
		XmNbottomOffset, 10,
		XmNrightAttachment, XmATTACH_FORM,
		XmNrightOffset, 10,
		XmNleftAttachment, XmATTACH_FORM,
		XmNleftOffset, 10,
		NULL);
   	XtVaSetValues (fr_adjust_edCalib,
		XmNbottomAttachment, XmATTACH_WIDGET, XmNbottomWidget, fr_energy_edCalib,
		XmNtopAttachment, XmATTACH_FORM,
		XmNtopOffset, 10,
		XmNrightAttachment, XmATTACH_FORM,
		XmNrightOffset, 10,
		XmNleftAttachment, XmATTACH_FORM,
		XmNleftOffset, 10,
		NULL);
	XtManageChild(fr_adjust_edCalib);
	XtManageChild(fr_energy_edCalib);
	XtManageChild(linea_edCalib);
	XtManageChild(fm_botones_edCalib);
	
	// *********************
	// * seccion de ajuste *
	// *********************
	cadena = XmStringCreate ("Fit Data", "BoldNormal");
   	i = 0;
   	XtSetArg (args[i], XmNchildType, XmFRAME_TITLE_CHILD); i++;
   	XtSetArg (args[i], XmNchildVerticalAlignment, XmALIGNMENT_CENTER); i++;
 //  	XtSetArg (args[i], XmNfontList, lista_fuentes); i++;
   	XtSetArg (args[i], XmNlabelString, cadena); i++;
	
   	lb_adjust_edCalib = XmCreateLabel (fr_adjust_edCalib, "lb_adjust_edCalib", args, i);
	XmStringFree (cadena);
	XtManageChild(lb_adjust_edCalib);
	i = 0;
	fm_adjust_edCalib = XmCreateForm(fr_adjust_edCalib, "fm_adjust_edCalib", args, i);

	// indicadores de Gaussiana
	//NOTA: Parece que aquí no le da los valores a los campos
	i = 0;
	XtSetArg (args[i], XmNorientation, XmVERTICAL); i++;
		
	rw1_adjust_edCalib = XmCreateRowColumn (fm_adjust_edCalib, "rw1_adjust_edCalib", args, i);
	cadena = XmStringCreate ("Centroid", "BoldNormal");
   	i = 0;
   	XtSetArg (args[i], XmNlabelString, cadena); i++;
   //	XtSetArg (args[i], XmNfontList, lista_fuentes); i++;
   	lbCen_adjust_edCalib = XmCreateLabel (rw1_adjust_edCalib, "lbCen_adjust_edCalib", args, i);
   	XmStringFree (cadena);
	XtManageChild(lbCen_adjust_edCalib);
	i = 0;
   	XtSetArg (args[i], XmNcolumns, 10); i++;
   	XtSetArg (args[i], XmNmaxLength, 20); i++;
   //	XtSetArg (args[i], XmNfontList, lista_fuentes); i++;
	txtCen_adjust_edCalib = XmCreateTextField(rw1_adjust_edCalib, "txtCen_adjust_edCalib", args, i);
	XtManageChild(txtCen_adjust_edCalib);
	cadena = XmStringCreateLocalized ("");
   	i = 0;
   	XtSetArg (args[i], XmNlabelString, cadena); i++;
   	XtSetArg (args[i], XmNalignment, XmALIGNMENT_END); i++;
   //	XtSetArg (args[i], XmNfontList, lista_fuentes); i++;
   	lbCenEn_adjust_edCalib = XmCreateLabel (rw1_adjust_edCalib, "lbCenEn_adjust_edCalib", args, i);
   	XmStringFree (cadena);
	XtManageChild(lbCenEn_adjust_edCalib); // No se coloca a la derecha porque sería necesario ponerlo en una base form
	
	cadena = XmStringCreate ("Sigma", "BoldNormal");
   	i = 0;
   	XtSetArg (args[i], XmNlabelString, cadena); i++;
   //	XtSetArg (args[i], XmNfontList, lista_fuentes); i++;
   	lbSigma_adjust_edCalib = XmCreateLabel (rw1_adjust_edCalib, "lbSigma_adjust_edCalib", args, i);
   	XmStringFree (cadena);
	XtManageChild(lbSigma_adjust_edCalib);
	i = 0;
   	XtSetArg (args[i], XmNcolumns, 10); i++;
   	XtSetArg (args[i], XmNmaxLength, 20); i++;
   //	XtSetArg (args[i], XmNfontList, lista_fuentes); i++;
	txtSigma_adjust_edCalib = XmCreateTextField(rw1_adjust_edCalib, "txtSigma_adjust_edCalib", args, i);
	XtManageChild(txtSigma_adjust_edCalib);
	cadena = XmStringCreateLocalized ("");
   	i = 0;
   	XtSetArg (args[i], XmNlabelString, cadena); i++;
   	XtSetArg (args[i], XmNalignment, XmALIGNMENT_END); i++;
   //	XtSetArg (args[i], XmNfontList, lista_fuentes); i++;
   	lbSigmaEn_adjust_edCalib = XmCreateLabel (rw1_adjust_edCalib, "lbSigmaEn_adjust_edCalib", args, i);
   	XmStringFree (cadena);
	XtManageChild(lbSigmaEn_adjust_edCalib);
	
   	XtVaSetValues (rw1_adjust_edCalib,
		XmNtopAttachment, XmATTACH_FORM,
		XmNleftAttachment, XmATTACH_FORM,
		NULL);
	XtManageChild(rw1_adjust_edCalib);

	// indicadores de primera exponencial
	i = 0;
	XtSetArg (args[i], XmNorientation, XmVERTICAL); i++;
	rw2_adjust_edCalib = XmCreateRowColumn (fm_adjust_edCalib, "rw2_adjust_edCalib", args, i);
	cadena = XmStringCreateLocalized ("Tau");
	i = 0;
   	XtSetArg (args[i], XmNlabelString, cadena); i++;
//   XtSetArg (args[i], XmNfontList, lista_fuentes); i++;
   	lbTau_adjust_edCalib = XmCreateLabel (rw2_adjust_edCalib, "lbTau_adjust_edCalib", args, i);
   	XmStringFree (cadena);
	XtManageChild(lbTau_adjust_edCalib);
	i = 0;
   	XtSetArg (args[i], XmNcolumns, 10); i++;
   	XtSetArg (args[i], XmNmaxLength, 20); i++;
//   XtSetArg (args[i], XmNfontList, lista_fuentes); i++;
	txtTau_adjust_edCalib = XmCreateTextField(rw2_adjust_edCalib, "txtTau_adjust_edCalib", args, i);
	XtManageChild(txtTau_adjust_edCalib);
	cadena = XmStringCreateLocalized ("");
   	i = 0;
   	XtSetArg (args[i], XmNlabelString, cadena); i++;
   	XtSetArg (args[i], XmNalignment, XmALIGNMENT_END); i++;
//   XtSetArg (args[i], XmNfontList, lista_fuentes); i++;
   	lbTauEn_adjust_edCalib = XmCreateLabel (rw2_adjust_edCalib, "lbTauEn_adjust_edCalib", args, i);
   	XmStringFree (cadena);
	XtManageChild(lbTauEn_adjust_edCalib);
	cadena = XmStringCreateLocalized ("Ytau");
   	i = 0;
   	XtSetArg (args[i], XmNlabelString, cadena); i++;
//   XtSetArg (args[i], XmNfontList, lista_fuentes); i++;
   	lbYtau_adjust_edCalib = XmCreateLabel (rw2_adjust_edCalib, "lbYtau_adjust_edCalib", args, i);
   	XmStringFree (cadena);
	XtManageChild(lbYtau_adjust_edCalib);
	i = 0;
   	XtSetArg (args[i], XmNcolumns, 10); i++;
   	XtSetArg (args[i], XmNmaxLength, 20); i++;
//   XtSetArg (args[i], XmNfontList, lista_fuentes); i++;
	txtYtau_adjust_edCalib = XmCreateTextField(rw2_adjust_edCalib, "txtYtau_adjust_edCalib", args, i);
	XtManageChild(txtYtau_adjust_edCalib);
	
   	XtVaSetValues (rw2_adjust_edCalib,
		XmNtopAttachment, XmATTACH_FORM,
		XmNleftAttachment, XmATTACH_WIDGET, XmNleftWidget, rw1_adjust_edCalib,
		XmNleftOffset, 10,
		NULL);
	XtManageChild(rw2_adjust_edCalib);

	// Dibujo de ayuda
	Pixel fg, bg;
	XtVaGetValues (dlg_edCalib,
			XmNforeground, &fg,
			XmNbackground, &bg,
			NULL);
	i = 0;
	XtSetArg (args[i], XmNorientation, XmVERTICAL); i++;
	XtSetArg (args[i], XmNentryAlignment, XmALIGNMENT_CENTER); i++;
	rw3_adjust_edCalib = XmCreateRowColumn (fm_adjust_edCalib, "rw3_adjust_edCalib", args, i);
		// carga el icono de ayuda sobre parametros
	Pixmap helpIcon = XCreatePixmapFromBitmapData (dpy,
			RootWindowOfScreen(XtScreen (dlg_edCalib)),
			(char *) HelpIcon_bits, HelpIcon_width, HelpIcon_height,
			fg, bg,
			DefaultDepthOfScreen (XtScreen(dlg_edCalib)));
   	i = 0;
   	XtSetArg (args[i], XmNlabelType, XmPIXMAP); i++;
   	XtSetArg (args[i], XmNlabelPixmap, helpIcon); i++;
   	lbGraf_adjust_edCalib = XmCreateLabel (rw3_adjust_edCalib, "lbGraf_adjust_edCalib", args, i);
	XtManageChild(lbGraf_adjust_edCalib);

	XtManageChild(rw3_adjust_edCalib);

	// indicador de calidad de ajuste
	cadena = XmStringCreate ("Chi\262:", "BoldNormal");
   	i = 0;
   	XtSetArg (args[i], XmNlabelString, cadena); i++;
   	lbChiSq_adjust_edCalib = XmCreateLabel (fm_adjust_edCalib, "lbChiSq_adjust_edCalib", args, i);
   	XmStringFree (cadena);
   	XtVaSetValues (lbChiSq_adjust_edCalib,
		XmNtopAttachment, XmATTACH_WIDGET, XmNtopWidget, rw1_adjust_edCalib,
		XmNtopOffset, 10,
		XmNleftAttachment, XmATTACH_FORM, XmNleftOffset, 10, 
		XmNbottomAttachment, XmATTACH_FORM, XmNbottomOffset, 10,
		NULL);
	XtManageChild(lbChiSq_adjust_edCalib);

	//Fórmula de ayuda
	Pixmap formula = XCreatePixmapFromBitmapData (dpy,
			RootWindowOfScreen(XtScreen (dlg_edCalib)),
			(char *) ecuation_bits, ecuation_width, ecuation_height,
			fg, bg,
			DefaultDepthOfScreen (XtScreen(dlg_edCalib)));
   	i = 0;
   	XtSetArg (args[i], XmNlabelType, XmPIXMAP); i++;
   	XtSetArg (args[i], XmNlabelPixmap, formula); i++;
   	lbEcuat_adjust_edCalib = XmCreateLabel (fm_adjust_edCalib, "lbEcuat_adjust_edCalib", args, i);
	XtManageChild(lbEcuat_adjust_edCalib);
   	XtVaSetValues (lbEcuat_adjust_edCalib,
		XmNtopAttachment, XmATTACH_WIDGET, XmNtopWidget, rw1_adjust_edCalib,
		XmNtopOffset, 10,
		XmNbottomAttachment, XmATTACH_FORM, XmNbottomOffset, 10,
		XmNleftAttachment, XmATTACH_WIDGET, XmNleftWidget, lbChiSq_adjust_edCalib,
		NULL);

   	XtVaSetValues (rw3_adjust_edCalib,
		XmNtopAttachment, XmATTACH_FORM, XmNtopOffset, 20,
		XmNleftAttachment, XmATTACH_WIDGET, XmNleftWidget, rw2_adjust_edCalib,
		XmNleftOffset, 10,
		XmNrightAttachment, XmATTACH_FORM,
		XmNbottomAttachment, XmATTACH_WIDGET, XmNbottomWidget, lbEcuat_adjust_edCalib,
		NULL);
	// ajustes del formulario contenedor
	
   	XtVaSetValues (fm_adjust_edCalib,
		XmNbottomAttachment, XmATTACH_FORM,
		XmNtopAttachment, XmATTACH_FORM,
		XmNrightAttachment, XmATTACH_FORM,
		XmNleftAttachment, XmATTACH_FORM,
		NULL);
	XtManageChild(fm_adjust_edCalib);

	printf("Creamos sección de ajuste\n");
	// **********************
	// * seccion de energia *
	// **********************
	printf("Sección de energía\n");	
	cadena = XmStringCreate ("Energy Data", "BoldNormal");
	i = 0;
   	XtSetArg (args[i], XmNchildType, XmFRAME_TITLE_CHILD); i++;
   	XtSetArg (args[i], XmNchildVerticalAlignment, XmALIGNMENT_CENTER); i++;
//   XtSetArg (args[i], XmNfontList, lista_fuentes); i++;
   	XtSetArg (args[i], XmNlabelString, cadena); i++;
   	lb_energy_edCalib = XmCreateLabel (fr_energy_edCalib, "lb_energy_edCalib", args, i);
   	XmStringFree (cadena);
	XtManageChild(lb_energy_edCalib);
	i = 0;
	fm_energy_edCalib = XmCreateForm(fr_energy_edCalib, "fm_energy_edCalib", args, i);

	// buscador de elementos
	i = 0;
	fr_DB_identify_edCalib = XmCreateFrame (fm_energy_edCalib, "fr_DB_identify_edCalib", args, i);
	cadena = XmStringCreate ("Elements DB", "BoldNormal");
   	i = 0;
   	XtSetArg (args[i], XmNchildType, XmFRAME_TITLE_CHILD); i++;
   	XtSetArg (args[i], XmNchildVerticalAlignment, XmALIGNMENT_CENTER); i++;
   	XtSetArg (args[i], XmNlabelString, cadena); i++;
//   XtSetArg (args[i], XmNfontList, lista_fuentes); i++;
   	lb_DB_identify_edCalib = XmCreateLabelGadget (fr_DB_identify_edCalib, "lb_DB_identify_edCalib", args, i);
   	XmStringFree (cadena);
	XtManageChild(lb_DB_identify_edCalib);
	// lista de seleccion
	i = 0;
	fm_DB_identify_edCalib = XmCreateForm(fr_DB_identify_edCalib, "fm_DB_identify_edCalib", args, i);
	fm_DB_identify2_edCalib = XmCreateForm(fm_DB_identify_edCalib, "fm_DB_identify2_edCalib", args, i);

	// lista de identificacion
	CalibIdent.SetPeakList(&Datos.listaPicos);
//	CalibIdent.SetFonts(lista_fuentes);
//	CalibIdent.SetFontBold("BoldNormal"); CalibIdent.SetFontLittle("SubIndice");
	CalibIdent.CreateWidgets(fm_DB_identify2_edCalib);
	XtManageChild(fm_DB_identify2_edCalib);
	// Boton de utlizar identificacion
	i = 0;
   	cadena = XmStringCreateLocalized ("<=");
   	XtSetArg (args[i], XmNlabelString, cadena); i++;
	XtSetArg (args[i], XmNalignment, XmALIGNMENT_CENTER); i++;
//   XtSetArg (args[i], XmNfontList, lista_fuentes); i++;
	bt_DB_identify_edCalib = XmCreatePushButton(fm_DB_identify_edCalib, "bt_DB_identify_edCalib", args, i);
   	XmStringFree (cadena);
   	XtAddCallback (bt_DB_identify_edCalib, XmNactivateCallback, pulsa_identifica_edit_calibracion, NULL);
	XtManageChild(bt_DB_identify_edCalib);
   	XtVaSetValues (bt_DB_identify_edCalib,
		XmNtopAttachment, XmATTACH_WIDGET, XmNtopWidget, fm_DB_identify2_edCalib,
		XmNbottomAttachment, XmATTACH_FORM, XmNbottomOffset, 5,
		XmNleftAttachment, XmATTACH_FORM, XmNleftOffset, 5, 
		XmNrightAttachment, XmATTACH_FORM, XmNrightOffset, 5,
		NULL);
   	XtVaSetValues (fm_DB_identify2_edCalib,
		XmNleftAttachment, XmATTACH_FORM,
		XmNrightAttachment, XmATTACH_FORM,
		NULL);
		

	
	XtManageChild(fm_DB_identify_edCalib);
		

	//indicador de energia
	i = 0;
	rw_Izq_energy_edCalib = XmCreateRowColumn (fm_energy_edCalib, "rw_Izq_energy_edCalib", args, i);
	cadena = XmStringCreate ("Emission:      ", "BoldNormal");
   	i = 0;
   	XtSetArg (args[i], XmNlabelString, cadena); i++;
//   XtSetArg (args[i], XmNfontList, lista_fuentes); i++;
   	lbEmission_energy_edCalib = XmCreateLabel (rw_Izq_energy_edCalib, "lbEmission_energy_edCalib", args, i);
   	XmStringFree (cadena);
	XtManageChild(lbEmission_energy_edCalib);
	cadena = XmStringCreateLocalized (" ");
   	i = 0;
   	XtSetArg (args[i], XmNlabelString, cadena); i++;
//   XtSetArg (args[i], XmNfontList, lista_fuentes); i++;
   	lbEnergy_energy_edCalib = XmCreateLabel (rw_Izq_energy_edCalib, "lbEnergy_energy_edCalib", args, i);
   	XmStringFree (cadena);
	XtManageChild(lbEnergy_energy_edCalib);
	cadena = XmStringCreateLocalized (" ");
   	i = 0;
   	XtSetArg (args[i], XmNlabelString, cadena); i++;
//   XtSetArg (args[i], XmNfontList, lista_fuentes); i++;
   	lbProb_energyEdCalib = XmCreateLabel (rw_Izq_energy_edCalib, "lbProb_energyEdCalib", args, i);
   	XmStringFree (cadena);
	XtManageChild(lbProb_energyEdCalib);
	cadena = XmStringCreateLocalized (" ");
   	i = 0;
   	XtSetArg (args[i], XmNlabelString, cadena); i++;
//   XtSetArg (args[i], XmNfontList, lista_fuentes); i++;
   	lbVoid_energy_edCalib = XmCreateLabelGadget (rw_Izq_energy_edCalib, "lbVoid_energy_edCalib", args, i);
   	XmStringFree (cadena);
	XtManageChild(lbVoid_energy_edCalib);
	cadena = XmStringCreate ("Element:", "BoldNormal");
   	i = 0;
   	XtSetArg (args[i], XmNlabelString, cadena); i++;
//   XtSetArg (args[i], XmNfontList, lista_fuentes); i++;
   	lbElement_energy_edCalib = XmCreateLabel (rw_Izq_energy_edCalib, "lbElement_energy_edCalib", args, i);
   	XmStringFree (cadena);
	XtManageChild(lbElement_energy_edCalib);
	cadena = XmStringCreateLocalized (" ");
   	i = 0;
   	XtSetArg (args[i], XmNlabelString, cadena); i++;
//   XtSetArg (args[i], XmNfontList, lista_fuentes); i++;
   	lbName_energy_edCalib = XmCreateLabel (rw_Izq_energy_edCalib, "lbName_energy_edCalib", args, i);
   	XmStringFree (cadena);
	XtManageChild(lbName_energy_edCalib);


   	XtVaSetValues (fm_energy_edCalib,
		XmNbottomAttachment, XmATTACH_FORM,
		XmNtopAttachment, XmATTACH_FORM,
		XmNrightAttachment, XmATTACH_FORM,
		XmNleftAttachment, XmATTACH_FORM,
		NULL);
   	XtVaSetValues (rw_Izq_energy_edCalib,
		XmNbottomAttachment, XmATTACH_FORM,
		XmNtopAttachment, XmATTACH_FORM, XmNtopOffset, 15,
		XmNleftAttachment, XmATTACH_FORM,
//		XmNrightAttachment, XmATTACH_WIDGET, XmNrightWidget, fr_DB_identify_edCalib,
		XmNrightOffset, 5, XmNleftOffset, 5,
		NULL);
   	XtVaSetValues (fr_DB_identify_edCalib,
		XmNbottomAttachment, XmATTACH_FORM, XmNbottomOffset, 5,
		XmNtopAttachment, XmATTACH_FORM,
		XmNleftAttachment, XmATTACH_WIDGET, XmNleftWidget, rw_Izq_energy_edCalib,
		XmNrightAttachment, XmATTACH_FORM, XmNrightOffset, 5,
		NULL);

	XtManageChild(fr_DB_identify_edCalib);
	XtManageChild(rw_Izq_energy_edCalib);
	XtManageChild(fm_energy_edCalib);
	


	// **********************
	// * seccion de botones *
	// **********************
	i = 0;
   	cadena = XmStringCreate ("OK", "BoldNormal");
   	XtSetArg (args[i], XmNlabelString, cadena); i++;
	XtSetArg (args[i], XmNalignment, XmALIGNMENT_CENTER); i++;
//   XtSetArg (args[i], XmNfontList, lista_fuentes); i++;
	bt_ok_edCalib = XmCreatePushButton(fm_botones_edCalib, "bt_ok_edCalib", args, i);
	XtAddCallback (bt_ok_edCalib, XmNactivateCallback, pulsa_ok_edit_calibracion, NULL);
   	XmStringFree (cadena);
	i = 0;
   	cadena = XmStringCreate ("Cancel", "BoldNormal");
   	XtSetArg (args[i], XmNlabelString, cadena); i++;
	XtSetArg (args[i], XmNalignment, XmALIGNMENT_CENTER); i++;
//   XtSetArg (args[i], XmNfontList, lista_fuentes); i++;
	bt_cancel_edCalib = XmCreatePushButton(fm_botones_edCalib, "bt_cancel_edCalib", args, i);
	XtAddCallback (bt_cancel_edCalib, XmNactivateCallback, pulsa_cancel_edit_calibracion, NULL);
   	XmStringFree (cadena);
	i = 0;
   	cadena = XmStringCreate ("Fit", "BoldNormal");
   	XtSetArg (args[i], XmNlabelString, cadena); i++;
	XtSetArg (args[i], XmNalignment, XmALIGNMENT_CENTER); i++;
//   XtSetArg (args[i], XmNfontList, lista_fuentes); i++;
	bt_adjust_edCalib = XmCreatePushButton(fm_botones_edCalib, "bt_adjust_edCalib", args, i);
	XtAddCallback (bt_adjust_edCalib, XmNactivateCallback, pulsa_adjust_edit_calibracion, NULL);
   	XmStringFree (cadena);
   	XtVaSetValues (bt_ok_edCalib,
		XmNbottomAttachment, XmATTACH_FORM,
		XmNtopAttachment, XmATTACH_FORM,
		XmNleftAttachment, XmATTACH_FORM,
		XmNrightAttachment, XmATTACH_POSITION, XmNrightPosition, 1,
		XmNrightOffset, 5, 
		NULL);
   	XtVaSetValues (bt_cancel_edCalib,
		XmNbottomAttachment, XmATTACH_FORM,
		XmNtopAttachment, XmATTACH_FORM,
		XmNleftAttachment, XmATTACH_POSITION, XmNleftPosition, 1,
		XmNrightAttachment, XmATTACH_POSITION, XmNrightPosition, 2,
		XmNrightOffset, 5, XmNleftOffset, 5,
		NULL);
   	XtVaSetValues (bt_adjust_edCalib,
		XmNbottomAttachment, XmATTACH_FORM,
		XmNtopAttachment, XmATTACH_FORM,
		XmNleftAttachment, XmATTACH_POSITION, XmNleftPosition, 2,
		XmNrightAttachment, XmATTACH_FORM,
		XmNleftOffset, 5,
		NULL);
	XtManageChild(bt_ok_edCalib);
	XtManageChild(bt_cancel_edCalib);
	XtManageChild(bt_adjust_edCalib);	
	}


//****************** ACTUALIZA IDENTIFICACIÓN CALIBRACIÓN ******************
void ActualizaIdentificacionCalibracion(unsigned short int emission) 
	{
	XmString cadena, s1, s2;
	char buffer[80], nombre[3];

	//NOTA: Hecho con un sólo pico
	NodoPicos *intervalo = DevuelveNodo(Datos.listaPicos, numIntervaloEdicion);
	NodoPico *pico = intervalo->picos.inicio; // hay 1 solo pico

	// Ahora solo muestra los datos
	if (LaraBD.GetIsRunning())	sprintf(buffer, "  %.3fkeV", LaraBD.GetEnergy(emission));
	//else 						sprintf(buffer, "  %.3fkeV", pico->Ajuste.Energia);
	else 						
		{
		for(int i=0;i<pico->Ajuste.emisiones->numElements();i++)
			{
			NodoEmision *ne=(NodoEmision *)(pico->Ajuste.emisiones->getNode(i));
			double e=ne->Energia;	
			sprintf(buffer, "  %.3fkeV", e);
			}
		}
	cadena = XmStringCreateLocalized (buffer);
	XtVaSetValues (lbEnergy_energy_edCalib, XmNlabelString, cadena, NULL);
	XmStringFree (cadena);

	if (LaraBD.GetIsRunning())	sprintf(buffer, "  %f%%", LaraBD.GetProbability(emission));
	else 						sprintf(buffer, "  ??%%");
	cadena = XmStringCreateLocalized (buffer);
	XtVaSetValues (lbProb_energyEdCalib, XmNlabelString, cadena, NULL);
	XmStringFree (cadena);
	
	if (LaraBD.GetIsRunning()) 
		{
		unsigned short int element = LaraBD.GetEmissor(emission);
		s1 = XmStringCreateLocalized ("  ");
		s2 = XmStringNameElement(element, "SubIndice");
		cadena = XmStringConcat(s1, s2);
		XmStringFree(s1); XmStringFree(s2);
		}
	else cadena = XmStringCreateLocalized ("  ???");
	XtVaSetValues (lbName_energy_edCalib, XmNlabelString, cadena, NULL);
	XmStringFree (cadena);
	}

//******************* PULSA IDENTIFICA EDIT CALIBRACIÓN *******************
// Se activa cuando se pulsa el botón "<=" de la interfaz de identificación de picos
// Al identificar el pico con un determinado elemento, el resultado es que se
// pone como identificado y se le pone como código de elemento getSelectedEmission() 
// y como energía la que nos da la base de datos para esa getSelectedEmission().
// Con esto el pico queda calibrado.
void pulsa_identifica_edit_calibracion(Widget widget, XtPointer client_data, XtPointer call_data) 
	{
	ActualizaIdentificacionCalibracion(CalibIdent.GetSelectedEmission());

	if (LaraBD.GetIsRunning()) 
		{
		//NOTA: Sólo se considera un pico por intervalo
		NodoPicos *intervalo = DevuelveNodo(Datos.listaPicos, numIntervaloEdicion);
		NodoPico *pico = intervalo->picos.inicio; // hay 1 solo pico
	
		//	
		if(pico->Ajuste.identificado==false)	pico->Ajuste.emisiones=new LinkedList();
		//
		pico->Ajuste.identificado = true;
		pico->Ajuste.calibrado = true;
		
		//NUEVO: Prueba a suerte
		pico->Ajuste.eficienciaCalculada=false;
		//
		/*pico->Ajuste.codElemento = CalibIdent.GetSelectedEmission();//Devuelve selected emission, que es value que es el número de emisión
		printf("Código de elemento: %f y la función %f\n", pico->Ajuste.codElemento, CalibIdent.GetSelectedEmission());
		pico->Ajuste.Energia = LaraBD.GetEnergy(CalibIdent.GetSelectedEmission());
		printf("Energía según la BD: %f\n", pico->Ajuste.Energia);*/
		
		//
		printf("Creamos un nodo emisión\n");
		NodoEmision *emision=new NodoEmision();
		emision->codElemento= CalibIdent.GetSelectedEmission();//Devuelve selected emission, que es value que es el número de emisión
		emision->Energia = LaraBD.GetEnergy(CalibIdent.GetSelectedEmission());
		//printf("Añadimos (%d,%f) a las otras emisiones que tenemos\n", emision.codElemento, emision.Energia);
		
		pico->Ajuste.emisiones->deleteAll();
		//NOTA: Esto si permitiéramos más de una emisión, pero creo que lo suyo es
		//		permitir sólo una en la calibración (por eso añado la línea deleteAll()
		pico->Ajuste.emisiones->addLast(emision);
		//free(emision);
		
		/*
		NodoEmision emision;
		printf("Le ponemos inicialización\n");
		emision.codElemento= CalibIdent.GetSelectedEmission();//Devuelve selected emission, que es value que es el número de emisión
		emision.Energia = LaraBD.GetEnergy(CalibIdent.GetSelectedEmission());
		printf("Añadimos (%d,%f) a las otras emisiones que tenemos\n", emision.codElemento, emision.Energia);
		pico->Ajuste.emisiones->addLast(&emision);
		NodoEmision *ne;
		ne=(NodoEmision *)pico->Ajuste.emisiones->getFirst();
		printf("Que quedan como (%d,%f)\n", ne->codElemento, ne->Energia);
		*/
		//

		pico->Ajuste.calibrado = true;
		}
	else	Aviso(fondo_calibracion, "No Database for identification avalaible");
}

//********************** PULSA CANCEL EDIT CALIBRACION *************************
// Cancela la calibración y la adaptación a una emisión de elemento conocido, manteniendo
// la calibracion previa
void pulsa_cancel_edit_calibracion(Widget widget, XtPointer client_data, XtPointer call_data) 
	{
	NodoPicos *intervalo = DevuelveNodo(Datos.listaPicos, numIntervaloEdicion);
	NodoPico *pico = intervalo->picos.inicio; // hay 1 solo pico
	
	// devuelve los datos iniciales
	pico->Ajuste = AjusteSeguridad;
	
	// rehace el ajuste
	Datos.ActualizaIntervaloCalibracion(numIntervaloEdicion);
	
	// Actualiza los datos de los picos en la grafica
	printf("PulsaCancelEditCalib llama a CAAG\n");
	int nError = CopiaAjustesAGrafica();
	if (nError < 0) 
		{
		Aviso(fondo_calibracion, "No Memory Avalaible");
		printf(ERROR_CALIBRATION);
		printf(ERROR_NO_MEMORY);
		return;
		}

	ActivaAjustesGrafica(EstadoVisual.AjusteCalibracion);
	
	if (EstadoVisual.AjusteCalibracion)	PintaGrafica(0);

	XtUnmanageChild(dlg_edCalib);
	}

//******************* PULSA OK EDIT CALIBRACION ***********************************
/* Cuando pulsamos el botón de ok en la interfaz de edición de calibraciones
	NOTA: A veces, si se elige una emisión y se da a OK sin haber ajustado previamente
		da violación de segmento. Puede que sea igualmete cosa de la dichosa gráfica

	*/	
void pulsa_ok_edit_calibracion(Widget widget, XtPointer client_data, XtPointer call_data) 
	{
	char *cenText, *sigmaText, *tauText, *YtauText, *energiaText, *elemText, *seguridad;
	double cen, sigma, tau, Ytau, energia;

	NodoPicos *intervalo = DevuelveNodo(Datos.listaPicos, numIntervaloEdicion);
	NodoPico *pico = intervalo->picos.inicio; // hay 1 solo pico

	printf("Pulsado OK\n");
	// Actualiza la lista de calibracion si ha habido identificacion
	if (pico->Ajuste.identificado)	ReponeListaCalibracion();
	printf("Pico identificado\n");
			
	// Indica cambio en el trabajo
	EstadoTrabajo.TrabajoCambios = true;
	PonTituloFichCambiado();
	printf("Trabajo Cambiado\n");

	// recoge los datos introducidos
	cenText = XmTextGetString(txtCen_adjust_edCalib);
	sigmaText = XmTextGetString(txtSigma_adjust_edCalib);
	tauText = XmTextGetString(txtTau_adjust_edCalib);
	YtauText = XmTextGetString(txtYtau_adjust_edCalib);
	printf("Nuevos datos recogidos\n");

	// pone los valores en el ajuste
	
	// centroide
	cen = strtod(cenText, &seguridad);
	if ((strlen(cenText) == 0)||(cen <= 0.0)||(seguridad == cenText)) 
		{
		Aviso(widget, "Centroid format incorrect. Please, re-type.");
		XtFree(cenText); XtFree(sigmaText); XtFree(tauText); XtFree(YtauText);
		return;
		}
	else 	
		{
		XtFree(cenText);
		pico->Ajuste.Centroide = cen;
		}
	// sigma
	sigma = strtod(sigmaText, &seguridad);
	if ((strlen(sigmaText) == 0)||(sigma <= 0.0)||(seguridad == sigmaText)) 
		{
		Aviso(widget, "Sigma format incorrect. Please, re-type.");
		XtFree(sigmaText); XtFree(tauText); XtFree(YtauText);
		return;
		}
	else 	
		{
		XtFree(sigmaText);
		pico->Ajuste.Anchura = sigma;
		}
	// tau
	tau = strtod(tauText, &seguridad);
	if ((strlen(tauText) == 0)||(tau <= 0.0)||(seguridad == tauText)) 
		{
		Aviso(widget, "Tau format incorrect. Please, re-type.");
		XtFree(tauText); XtFree(YtauText);
		return;
		}
	else 	
		{
		XtFree(tauText);
		pico->Ajuste.Tau = tau;
		}
		// Ytau
	Ytau = strtod(YtauText, &seguridad);
	if ((strlen(YtauText) == 0)||(Ytau <= 0.0)||(seguridad == YtauText)) 
		{
		Aviso(widget, "Ytau format incorrect. Please, re-type.");
		XtFree(YtauText);
		return;
		}
	else 	
		{
		XtFree(YtauText);
		pico->Ajuste.Ytau = Ytau;
		}
	printf("Valores del ajuste cambiados\n");

	// rehace el ajuste
	//NOTA: Por recomendación de Begoña, mejor no hacemos el ajuste a no ser que
		//	explícitamente nos lo pidan
	//Datos.AjustePico(numIntervaloEdicion, 1, EstadoTrabajo.Calibracion, false);
	//printf("Ajuste rehecho\n");

	Datos.ActualizaIntervaloCalibracion(numIntervaloEdicion);
	// Actualiza los datos de los picos en la grafica
	int nError = CopiaAjustesAGrafica();
	if (nError < 0) 
		{
		Aviso(fondo_calibracion, "No Memory Avalaible");
		printf(ERROR_CALIBRATION);
		printf(ERROR_NO_MEMORY);
		return;
		}
	printf("Datos en gráfica actualizados\n");

	ActivaAjustesGrafica(EstadoVisual.AjusteCalibracion);
	printf("Gráfica dibujada\n");

	if (EstadoVisual.AjusteCalibracion)	PintaGrafica(0);

	XtUnmanageChild(dlg_edCalib);
	}
	
//********************* PULSA ADJUST EDIT CALIBRACIÓN *************************

void pulsa_adjust_edit_calibracion(Widget widget, XtPointer client_data, XtPointer call_data) 
	{
	char *cenText, *sigmaText, *tauText, *YtauText, *seguridad;
	double cen, sigma, tau, Ytau;

	NodoPicos *intervalo = DevuelveNodo(Datos.listaPicos, numIntervaloEdicion);
	NodoPico *pico = intervalo->picos.inicio; // hay 1 solo pico
	
	// recoge los datos introducidos
	cenText = XmTextGetString(txtCen_adjust_edCalib);
	sigmaText = XmTextGetString(txtSigma_adjust_edCalib);
	tauText = XmTextGetString(txtTau_adjust_edCalib);
	YtauText = XmTextGetString(txtYtau_adjust_edCalib);
	
	// pone los valores en el ajuste
		// centroide
	cen = strtod(cenText, &seguridad);
	if ((strlen(cenText) == 0)||(cen <= 0.0)||(seguridad == cenText)) 
		{
		Aviso(widget, "Centroid format incorrect. Please, re-type.");
		XtFree(cenText); XtFree(sigmaText); XtFree(tauText); XtFree(YtauText);
		return;
		}
	else 	
		{
		XtFree(cenText);
		pico->Ajuste.Centroide = cen;
		}
		// sigma
	sigma = strtod(sigmaText, &seguridad);
	if ((strlen(sigmaText) == 0)||(sigma <= 0.0)||(seguridad == sigmaText)) 
		{
		Aviso(widget, "Sigma format incorrect. Please, re-type.");
		XtFree(sigmaText); XtFree(tauText); XtFree(YtauText);
		return;
		}
	else 	
		{
		XtFree(sigmaText);
		pico->Ajuste.Anchura = sigma;
		}
		// tau
	tau = strtod(tauText, &seguridad);
	if ((strlen(tauText) == 0)||(tau <= 0.0)||(seguridad == tauText)) 
		{
		Aviso(widget, "Tau format incorrect. Please, re-type.");
		XtFree(tauText); XtFree(YtauText);
		return;
		}
	else 	
		{
		XtFree(tauText);
		pico->Ajuste.Tau = tau;
		}
		// Ytau
	Ytau = strtod(YtauText, &seguridad);
	if ((strlen(YtauText) == 0)||(Ytau <= 0.0)||(seguridad == YtauText)) 
		{
		Aviso(widget, "Ytau format incorrect. Please, re-type.");
		XtFree(YtauText);
		return;
		}
	else 	
		{
		XtFree(YtauText);
		pico->Ajuste.Ytau = Ytau;
		}
	
	// rehace el ajuste
	Datos.AjustePico(numIntervaloEdicion, 1, EstadoTrabajo.CalibracionForma, false);
	
	// actualiza los datos del nuevo ajuste
	XmString s1, s2, cadena;
	char texto[40];
	// escribe ChiSq
	s1 = XmStringCreate ("Chi\262:", "BoldNormal");
	sprintf(texto, " %.5f", intervalo->ChiSq);
	s2 = XmStringCreateLocalized(texto);
	cadena = XmStringConcat (s1, s2);
	XmStringFree(s1); XmStringFree(s2);
	XtVaSetValues (lbChiSq_adjust_edCalib, XmNlabelString, cadena, NULL);
	XmStringFree(cadena);
	
	// Escribe los parametros de la gaussiana
	sprintf(texto, "%.5f", pico->Ajuste.Centroide);
	XmTextSetString(txtCen_adjust_edCalib, texto);
	sprintf(texto, "%.6f", pico->Ajuste.Anchura);
	XmTextSetString(txtSigma_adjust_edCalib, texto);
	sprintf(texto, "%.3fkeV", Datos.DaEnergia(pico->Ajuste.Centroide));
	cadena = XmStringCreateLocalized (texto);
	XtVaSetValues (lbCenEn_adjust_edCalib, XmNlabelString, cadena, NULL);
	XmStringFree (cadena);
	
	// Escribe los parametros de la primera exponencial
	sprintf(texto, "%.6f", pico->Ajuste.Tau);
	XmTextSetString(txtTau_adjust_edCalib, texto);
	sprintf(texto, "%.4f", pico->Ajuste.Ytau);
	XmTextSetString(txtYtau_adjust_edCalib, texto);

	// Actualiza los datos de los picos en la grafica
	printf("Pulsa Adjust Edit Calib (VisCalib) llamando a CopiaAjustesAGrafica\n");
	int nError = CopiaAjustesAGrafica();
	if (nError < 0) 
		{
		Aviso(fondo_calibracion, "No Memory Avalaible");
		printf(ERROR_CALIBRATION);
		printf(ERROR_NO_MEMORY);
		return;
		}
	
	ActivaAjustesGrafica(EstadoVisual.AjusteCalibracion);

	if (EstadoVisual.AjusteCalibracion)	PintaGrafica(0);
	}


/******************************************************************************
 ******
 ******                             PRECALIBRACION
 ******
 ******************************************************************************/

void Crea_DialogPrecalibracion() 
	{
   	Arg		args[10];
   	Cardinal	i = 0;
	XmString cadena;

	i = 0;
	XtSetArg (args[i], XmNtitle, PRECALIBRATION_TITLE); i++;
	XtSetArg (args[i], XmNallowShellResize, true); i++;
	dialog_preCalib = XmCreateFormDialog(app_shell, "dialog_preCalib", args, i);

	// Crea la zona superior
	i = 0;
	paned_preCalib = XmCreateForm(dialog_preCalib, "paned_preCalib", args, i);

	i = 0;
	superior_preCalib = XmCreateForm(paned_preCalib, "superior_preCalib", args, i);
	i = 0;
	XtSetArg (args[i], XmNvisibleItemCount, 5), i++;
	XtSetArg (args[i], XmNselectionPolicy, XmSINGLE_SELECT), i++;
	XtSetArg (args[i], XmNlistSizePolicy, XmRESIZE_IF_POSSIBLE), i++;
	XtSetArg (args[i], XmNlistMarginHeight, 10), i++;
	XtSetArg (args[i], XmNlistMarginWidth, 10), i++;
	lista_preCalib = XmCreateList(superior_preCalib, "lista_preCalib", args, i);
	XtAddCallback (lista_preCalib, XmNsingleSelectionCallback, pulsa_lista_preCalib, NULL);
	XtManageChild(lista_preCalib);
	i = 0;
//	   XtSetArg (args[i], XmNhorizontalSpacing, 10); i++;
	XtSetArg (args[i], XmNfractionBase, 2); i++;
	row_botones_preCalib = XmCreateForm(superior_preCalib, "row_botones_preCalib", args, i);
	i = 0;
	cadena = XmStringCreateLocalized ("  Add  ");
	XtSetArg (args[i], XmNlabelString, cadena); i++;
	agnadir_preCalib = XmCreatePushButton(row_botones_preCalib, "agnadir_preCalib", args, i);
	XtVaSetValues (agnadir_preCalib,
		XmNleftAttachment, XmATTACH_FORM, XmNrightOffset, 10,
		XmNrightAttachment, XmATTACH_POSITION, XmNrightPosition, 1,
		NULL);
	XmStringFree (cadena);
	XtManageChild(agnadir_preCalib);
	XtAddCallback (agnadir_preCalib, XmNactivateCallback, pulsa_agnadir_preCalib, NULL);
	i = 0;
	cadena = XmStringCreateLocalized ("  Delete  ");
	XtSetArg (args[i], XmNlabelString, cadena); i++;
	quitar_preCalib = XmCreatePushButton(row_botones_preCalib, "quitarr_preCalib", args, i);
	XtVaSetValues (quitar_preCalib,
		XmNleftAttachment, XmATTACH_POSITION, XmNleftPosition, 1,
		XmNrightAttachment, XmATTACH_FORM, XmNleftOffset, 10,
		NULL);
	XmStringFree (cadena);
	XtManageChild(quitar_preCalib);
	XtAddCallback (quitar_preCalib, XmNactivateCallback, pulsa_borrar_preCalib, NULL);
	XtManageChild(row_botones_preCalib);

	// Crea los widget para introducir los datos
	i = 0;
	XtSetArg (args[i], XmNfractionBase, 3); i++;
	row_textos_preCalib = XmCreateForm(superior_preCalib, "row_textos_preCalib", args, i);
	i = 0;
	cadena = XmStringCreateLocalized ("Energy");
	XtSetArg (args[i], XmNlabelString, cadena); i++;
	lab_energia_preCalib = XmCreateLabel(row_textos_preCalib, "lab_energia_preCalib", args, i);
	XtVaSetValues (lab_energia_preCalib,
		XmNleftAttachment, XmATTACH_FORM,
		XmNrightAttachment, XmATTACH_POSITION, XmNrightPosition, 1,
		NULL);
	XmStringFree (cadena);
	XtManageChild(lab_energia_preCalib);
	i = 0;
	XtSetArg (args[i], XmNcolumns, 6); i++;
	XtSetArg (args[i], XmNmaxLength, 9); i++;
	energia_preCalib = XmCreateTextField(row_textos_preCalib, "energia_preCalib", args, i);
	XtVaSetValues (energia_preCalib,
		XmNleftAttachment, XmATTACH_FORM,
		XmNrightAttachment, XmATTACH_POSITION, XmNrightPosition, 1,
		XmNtopAttachment, XmATTACH_WIDGET, XmNtopWidget, lab_energia_preCalib,
		NULL);
	XtManageChild(energia_preCalib);
	i = 0;
	cadena = XmStringCreateLocalized ("Min Ch");
	XtSetArg (args[i], XmNlabelString, cadena); i++;
	lab_canal_izq_preCalib = XmCreateLabel(row_textos_preCalib, "lab_canal_izq_preCalib", args, i);
	XtVaSetValues (lab_canal_izq_preCalib,
		XmNleftAttachment, XmATTACH_POSITION, XmNleftPosition, 1,
		XmNrightAttachment, XmATTACH_POSITION, XmNrightPosition, 2,
		NULL);
	XmStringFree (cadena);
	XtManageChild(lab_canal_izq_preCalib);
	i = 0;
	XtSetArg (args[i], XmNcolumns, 6); i++;
	XtSetArg (args[i], XmNmaxLength, 8); i++;
	canal_izq_preCalib = XmCreateTextField(row_textos_preCalib, "canal_izq_preCalib", args, i);
	XtVaSetValues (canal_izq_preCalib,
		XmNleftAttachment, XmATTACH_POSITION, XmNleftPosition, 1,
		XmNrightAttachment, XmATTACH_POSITION, XmNrightPosition, 2,
		XmNtopAttachment, XmATTACH_WIDGET, XmNtopWidget, lab_canal_izq_preCalib,
		NULL);
	XtManageChild(canal_izq_preCalib);
	i = 0;
	cadena = XmStringCreateLocalized ("Max Ch");
	XtSetArg (args[i], XmNlabelString, cadena); i++;
	lab_canal_der_preCalib = XmCreateLabel(row_textos_preCalib, "lab_canal_der_preCalib", args, i);
	XtVaSetValues (lab_canal_der_preCalib,
		XmNleftAttachment, XmATTACH_POSITION, XmNleftPosition, 2,
		XmNrightAttachment, XmATTACH_FORM,
		NULL);
	XmStringFree (cadena);
	XtManageChild(lab_canal_der_preCalib);
	i = 0;
	XtSetArg (args[i], XmNcolumns, 6); i++;
	XtSetArg (args[i], XmNmaxLength, 8); i++;
	canal_der_preCalib = XmCreateTextField(row_textos_preCalib, "canal_der_preCalib", args, i);
	XtVaSetValues (canal_der_preCalib,
		XmNleftAttachment, XmATTACH_POSITION, XmNleftPosition, 2,
		XmNrightAttachment, XmATTACH_FORM,
		XmNtopAttachment, XmATTACH_WIDGET, XmNtopWidget, lab_canal_der_preCalib,
		NULL);
	XtManageChild(canal_der_preCalib);
	XtManageChild(row_textos_preCalib);
	XtManageChild(superior_preCalib);

	i = 0;
	linea_preCalib = XmCreateSeparator(paned_preCalib, "linea_preCalib", args, i);
	XtManageChild(linea_preCalib);

	// Crea la zona inferior
	i = 0;
	XtSetArg (args[i], XmNfractionBase, 3); i++;
	inferior_preCalib = XmCreateForm(paned_preCalib, "inferior_preCalib", args, i);
	i = 0;
	cadena = XmStringCreateLocalized ("Close");
	XtSetArg (args[i], XmNlabelString, cadena); i++;
	XtSetArg (args[i], XmNdefaultButtonShadowThickness, 1); i++;
	XtSetArg (args[i], XmNshowAsDefault, true); i++;
	cerrar_preCalib = XmCreatePushButton(inferior_preCalib, "cerrar_preCalib", args, i);
	XmStringFree (cadena);
	XtManageChild(cerrar_preCalib);
	XtAddCallback (cerrar_preCalib, XmNactivateCallback, pulsa_cerrar_preCalib, NULL);
	XtManageChild(inferior_preCalib);

	XtManageChild(paned_preCalib);

	// coloca los widgets
	XtVaSetValues (paned_preCalib,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			XmNtopAttachment, XmATTACH_FORM,
			XmNbottomAttachment, XmATTACH_FORM,
			NULL);
	XtVaSetValues (linea_preCalib,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNbottomOffset, 40,
			NULL);
	XtVaSetValues (inferior_preCalib,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNtopAttachment, XmATTACH_WIDGET, XmNtopWidget, linea_preCalib,
			NULL);
	XtVaSetValues (cerrar_preCalib,
			XmNrightAttachment, XmATTACH_POSITION,
			XmNrightPosition, 2,
			XmNleftAttachment, XmATTACH_POSITION,
			XmNleftPosition, 1,
			XmNtopAttachment, XmATTACH_FORM,
			XmNbottomAttachment, XmATTACH_FORM,
			NULL);
	XtVaSetValues (superior_preCalib,
			XmNrightAttachment, XmATTACH_FORM,
			XmNleftAttachment, XmATTACH_FORM,
			XmNtopAttachment, XmATTACH_FORM,
			XmNbottomAttachment, XmATTACH_WIDGET, XmNbottomWidget, linea_preCalib,
			NULL);
	XtVaSetValues (lista_preCalib,
			XmNrightAttachment, XmATTACH_FORM,
			XmNrightOffset, 20,
			XmNleftAttachment, XmATTACH_FORM,
			XmNleftOffset, 20,
			XmNtopAttachment, XmATTACH_FORM,
			XmNtopOffset, 10,
			XmNbottomAttachment, XmATTACH_WIDGET, XmNbottomWidget, row_textos_preCalib,
			XmNbottomOffset, 10,
			NULL);
	XtVaSetValues (row_textos_preCalib,
			XmNrightAttachment, XmATTACH_FORM,
			XmNrightOffset, 10,
			XmNleftAttachment, XmATTACH_FORM,
			XmNleftOffset, 10,
			XmNbottomAttachment, XmATTACH_WIDGET, XmNbottomWidget, row_botones_preCalib,
			XmNbottomOffset, 10,
			NULL);
	XtVaSetValues (row_botones_preCalib,
			XmNrightAttachment, XmATTACH_FORM,
			XmNrightOffset, 10,
			XmNleftAttachment, XmATTACH_FORM,
			XmNleftOffset, 10,
			XmNbottomAttachment, XmATTACH_FORM,
			XmNbottomOffset, 10,
			NULL);
	}


//********************* PULSA PRECALIBRA **************************
//Acción al pulsar el botón "PreCalibration"
//Si el programa se encuentra en una fase de trabajo posterior a la precalibración
//(cálculo de la calibración, forma o energía o cálculo del continuo o dentro de poco análisis)
//no hace nada.
// NOTA: Igual lo suyo es que el botón aparezca deshabilitado si está en fases posteriores
//Si comprueba que efectivamente puede pasar a la fase de precalibración, pone como estado de trabajo
//la precalibración y llama a ReconstruyeListaPrecalibración()

void pulsa_PreCalibra(Widget widget, XtPointer client_data, XtPointer call_data) 
	{
	if ((EstadoTrabajo.Continuo)||(EstadoTrabajo.PreContinuo)||
		 (EstadoTrabajo.Calibracion)||
		 (EstadoTrabajo.CalibracionEnergia)||(EstadoTrabajo.CalibracionForma)) 
		{
		/* TODO: preguntar si quiere seguir adelante y borrar todo lo calculado
			previamente en las anteriores secciones */
		
		/* De manera provisional, ante cualquier calibracion no permite
			la pecalibracion */
		return;

		/* De manera provisional, solo comprueba si la lista de parametros de
			busqueda del continuo esta vacia para permitir la precalibracion
		tListaContinuo *lista = DevuelveListaContinuo();
		if (lista->numElementos != 0)
			return;*/
		}
	EstadoTrabajo.Actual = ESTADO_PRECALIBRACION;
	XtManageChild (dialog_preCalib);
	ReconstruyeListaPrecalibracion();
	}

void pulsa_cerrar_preCalib(Widget widget, XtPointer client_data, XtPointer call_data)
	{
	XtUnmanageChild (dialog_preCalib);
	}

void CierraDialogPrecalibracion() 
	{
	if (dialog_preCalib)
		XtUnmanageChild (dialog_preCalib);
	}

//****************************** PULSA AÑADIR PRECALIB ********************************
//Evento: pulsación del botón "Add" de la ventana Precalibration Window.
//Añadimos un nuevo nodo de precalibración a la lista, comprobando que todos los formatos son correctos.
//Después, si con este nuevo nodo tenemos suficientes (3) para realizar la precalibración o para recalcularla
//(si ya teníamos tres o más), hacemos la precalibración (llamamos a Datos.PreCalibra() de espectros.C)
//Además, en el caso de que podamos llevar a cabo el cálculo de la precalibración, pues cambiamos el estado de trabajo
//(podemos comenzar con el continuo)
/*	NOTA: Un tema un poco rollete: al añadir un nodo y ponerle la energía, en algunos
	sistemas coge la "," como separación de decimales y en otros ".". Si no se pone
	la adecuada, no nos pone ninguno de los decimales que escribamos!.
	*/
void pulsa_agnadir_preCalib(Widget widget, XtPointer client_data, XtPointer call_data)
	{
	char result1[]="xxxxxxxx", texto_canales[80], label_energia_preCalib[] = "a: 0.0  b: 0.0  ";
	char *energiaText, *canIzqText, *canDerText, *seguridad;
	double energia;
	int canIzq, canDer;
	XmString t;
	bool correcto = true;
	
	if (!EstadoTrabajo.Continuo) 
		{
		energiaText = XmTextGetString(energia_preCalib);
		canIzqText = XmTextGetString(canal_izq_preCalib);
		canDerText = XmTextGetString(canal_der_preCalib);
		
		// Comprueba que el valor de la energía se haya introducido correctamente
		// (que no tenga un formato erróneo ni sea negativa.
		energia = strtod(energiaText, &seguridad);
		if (seguridad == energiaText) 
			{
			printf("Formato erróneo\n");
			Aviso(widget, "Energy format incorrect. Please, re-type.");
			sprintf(result1, "");
			XmTextSetString(energia_preCalib, result1);
			correcto = false;
			XtFree(energiaText);
			XtFree(canIzqText);
			XtFree(canDerText);
			return;
			}
		else 
			{
			if (energia < 0.0) 
				{
				printf("Energía menor que cero\n");
				sprintf(texto_canales, "Energy value must be greater or equal than 0.");
				Aviso(widget, texto_canales);
				sprintf(result1, "");
				XmTextSetString(energia_preCalib, result1);
				correcto = false;
				XtFree(energiaText);
				XtFree(canIzqText);
				XtFree(canDerText);
				return;
				}
			else 
				{
				//if(seguridad!=NULL)
				if(strcmp(seguridad,""))
					{
					 //En este caso nos hemos comido los decimales, se los
					//		añadimos sin molestar al usuario
					printf("Añadiendo si nos hemos comido decimales, seguridad vale: |%s|\n", seguridad);
					char *temp;
					
					string cad(seguridad);
					cad=cad.substr(1, strlen(seguridad));
					strcpy(seguridad, cad.c_str());
					
					double decimas=strtod(seguridad, &temp)*pow(10.0,(int)-strlen(seguridad) );
					sprintf(result1, "%.4f", (energia+decimas));
					XmTextSetString(energia_preCalib, result1);
					energia = atof(result1);
					}
				else
					{
					//La energía es correcta, se añade a la cadena de resultado
					printf("Cadena correcta, añadiendo energía\n");
					sprintf(result1, "%.4f", energia);
					XmTextSetString(energia_preCalib, result1);
					energia = atof(result1);
					}
				}
			}

		// Comprueba el valor del canal izquierdo
		printf("Comprobamos el valor del canal izquierdo\n");
		canIzq = (int)strtol(canIzqText, &seguridad, 10);
		if (seguridad == canIzqText) 
			{
			Aviso(widget, "Left channel format incorrect. Please, re-type.");
			sprintf(result1, "");
			XmTextSetString(canal_izq_preCalib, result1);
			correcto = false;
			XtFree(energiaText);
			XtFree(canIzqText);
			XtFree(canDerText);
			return;
			}
		else 
			{
			if ((canIzq > 0)&&(canIzq < Datos.numCanales)) 
				{
				sprintf(result1, "%d", canIzq);
				XmTextSetString(canal_izq_preCalib, result1);
				canIzq = atol(result1);
				}
			else 
				{
				if (canIzq <= 0)
					sprintf(texto_canales, "Minimum channel value must be greater than 0.");
				else
					sprintf(texto_canales, "Minimum channel value must be lesser than %d.", Datos.numCanales);
				Aviso(widget, texto_canales);
				sprintf(result1, "");
				XmTextSetString(canal_izq_preCalib, result1);
				correcto = false;
				XtFree(energiaText);
				XtFree(canIzqText);
				XtFree(canDerText);
				return;
				}			
			}

		// Comprueba el valor del canal derecho
		printf("Comprobamos el valor del canal derecho\n");
		canDer = (int)strtol(canDerText, &seguridad, 10);
		if (seguridad == canDerText) 
			{
			Aviso(widget, "Maximum channel format incorrect. Please, re-type.");
			sprintf(result1, "");
			XmTextSetString(canal_der_preCalib, result1);
			correcto = false;
			XtFree(energiaText);
			XtFree(canIzqText);
			XtFree(canDerText);
			return;
			}
		else 
			{
			if ((canDer > 0)&&(canDer <= Datos.numCanales)&&(canDer > canIzq)) 
				{
				sprintf(result1, "%d", canDer);
				XmTextSetString(canal_der_preCalib, result1);
				canDer = atol(result1);
				}
			else 
				{
				if (canDer <= 0)
					sprintf(texto_canales, "Maximum channel value must be greater than 0.");
				else if (canDer > Datos.numCanales)
					sprintf(texto_canales, "Maximum channel value must be lesser than %d.", Datos.numCanales);
				else
					sprintf(texto_canales, "Maximum channel value must be greater than mimimum channel value.");
				Aviso(widget, texto_canales);
				sprintf(result1, "");
				XmTextSetString(canal_der_preCalib, result1);
				correcto = false;
				XtFree(energiaText);
				XtFree(canIzqText);
				XtFree(canDerText);
				return;
				}			
			}
		printf("Añadimos pico a la lista\n");
		// Todo correcto, agnade pico a la lista
		if ((nodo = new NodoPrecalibracion) != NULL) 
			{
			tPrecalibracion datosPre;

			nodo->energia = energia;
			nodo->limInferior = canIzq;
			nodo->limSuperior = canDer;
			agnadeNodo(listaPrecalibracion, nodo);

			// hace la lista del tamagno necesario para que quepan los picos
			if (listaPrecalibracion.numElementos > 5) 
				{
				XtVaSetValues (lista_preCalib, XmNvisibleItemCount, listaPrecalibracion.numElementos, NULL);
				}
			else
				XtVaSetValues (lista_preCalib, XmNvisibleItemCount, 5, NULL);

			// reconstruye la lista
			XmListDeleteAllItems(lista_preCalib);
			for (int i = 1; i <= listaPrecalibracion.numElementos; i++) 
				{
				// coge cada elemento en la lista de precalibracion y lo
				// convierte en una entrada en el widget lista de precalibracion
				DevuelveDatoPreCalibracion(listaPrecalibracion, i, datosPre);
				if (datosPre.limSuperior == canDer) PreCalibSeleccionado = i;
				sprintf(texto_canales, "%.2f keV from %d to %d", datosPre.energia, datosPre.limInferior, datosPre.limSuperior);
				t = XmStringCreateLocalized (texto_canales);
				XmListAddItemUnselected(lista_preCalib, t, 0);
			   	XmStringFree (t);
				}
			XmListSelectPos(lista_preCalib, PreCalibSeleccionado, false);
			if (listaPrecalibracion.numElementos > 2) 
				{ // suficientes datos para precalibracion
				tPrecalibracion *puntero;
				DevuelvePreCalibracion(listaPrecalibracion, puntero);
				int nError=0;
				if ((nError=Datos.PreCalibra(puntero, listaPrecalibracion.numElementos))==0) 
					{
					if (!EstadoTrabajo.PreCalibracion) 
						{
						EstadoTrabajo.PreCalibracion = true;
						ActivaPreCalibDatos();
						printf("activamos continuo\n");
						ActivaContinuo();
						}
					}
				else 
					{
					printf("La función de precalibración devolvió %d\n", nError);
					if (EstadoTrabajo.PreCalibracion) 
						{
						EstadoTrabajo.PreCalibracion = false;
						DesactivaPreCalibDatos();
						DesactivaContinuo();
						Datos.afwhg = 0.0; Datos.bfwhg = 0.0;
						Datos.enp1 = 0.0; Datos.enp2 = 0.0; Datos.enp3 = 0.0;
						}
					}
				delete [] puntero;
				}
			EstadoTrabajo.TrabajoCambios = true;
			PonTituloFichCambiado();
			}

		XtFree(energiaText);
		XtFree(canIzqText);
		XtFree(canDerText);
		}
	printf("Termina la adición de precalibración\n");
	}

//******************** PULSA BORRAR PRECALIB ***************************************
//Evento: pulsar Delete en la Precalibration Window que aparece al pulsar Precalibrate
//Borrará el elemento seleccionado; recalculando el resultado de la precalibración si procede
//NOTA: No veo dónde hace la precalibración. Quizá se haga dinámicamente cuando se pulsa en uno
//	de los canales en la gráfica
void pulsa_borrar_preCalib(Widget widget, XtPointer client_data, XtPointer call_data)
	{
	int j = 0;//Será la posición del elemento seleccionado
	
	// busca el elemento seleccionado
	for (int i = 1; i <= listaPrecalibracion.numElementos; i++)
		if (XmListPosSelected (lista_preCalib, i)) j = i;

	if (j > 0) 
		{
		// Borra el elemento en la lista y en el widget
		quitaNodo(listaPrecalibracion, j);
		XmListDeletePos (lista_preCalib, j);
	
		// hace la lista del tamagno necesario para que quepan los picos
		//NOTA: este redimensionamiento sería simplemente mirar si hemos bajado de 5
		if (listaPrecalibracion.numElementos > 5)
			XtVaSetValues (lista_preCalib, XmNvisibleItemCount, listaPrecalibracion.numElementos, NULL);
		else
			XtVaSetValues (lista_preCalib, XmNvisibleItemCount, 5, NULL);

		//Dejamos seleccionado el elemento que viene después del que se ha borrado
		if (listaPrecalibracion.numElementos > 0) 
			{
			if (listaPrecalibracion.numElementos >= j)
				PreCalibSeleccionado = j;
			else
				PreCalibSeleccionado = listaPrecalibracion.numElementos;
			XmListSelectPos(lista_preCalib, PreCalibSeleccionado, false);
			}
	
		//Si tenemos suficientes datos tras eliminar este, recalculamos la precalibración.
		//3 datos se consideran suficientes.
		if (listaPrecalibracion.numElementos > 2) 
			{ // suficientes datos para precalibracion
			tPrecalibracion *puntero;

			DevuelvePreCalibracion(listaPrecalibracion, puntero);
			
			if (Datos.PreCalibra(puntero, listaPrecalibracion.numElementos)==0) 
				{
				if (!EstadoTrabajo.PreCalibracion) 
					{
					EstadoTrabajo.PreCalibracion = true;
					ActivaPreCalibDatos();
					printf("Activamos continuo2\n");
					ActivaContinuo();
					}
				}
			else if (EstadoTrabajo.PreCalibracion) 
				{
				EstadoTrabajo.PreCalibracion = false;
				DesactivaPreCalibDatos();
				DesactivaContinuo();
				Datos.afwhg = 0.0; Datos.bfwhg = 0.0;
				Datos.enp1 = 0.0; Datos.enp2 = 0.0; Datos.enp3 = 0.0;
				}

			delete [] puntero;
			}
		else if (EstadoTrabajo.PreCalibracion) 
			{
			EstadoTrabajo.PreCalibracion = false;
			DesactivaPreCalibDatos();
			DesactivaContinuo();
			Datos.afwhg = 0.0; Datos.bfwhg = 0.0;
			Datos.enp1 = 0.0; Datos.enp2 = 0.0; Datos.enp3 = 0.0;
			}
		
		EstadoTrabajo.TrabajoCambios = true;
		PonTituloFichCambiado();
		}
	}

void pulsa_lista_preCalib(Widget widget, XtPointer client_data, XtPointer call_data) 
	{
    XmListCallbackStruct *cbs = (XmListCallbackStruct *) call_data;
	
	if (XmListPosSelected(lista_preCalib, cbs->item_position)) 
		{
		// saca los datos de esa posicion en las ventanas de datos
		NodoPrecalibracion *nodo = DevuelveNodo(listaPrecalibracion, cbs->item_position);
		if (nodo != NULL) 
			{
			char result1[]="xxxxxxxx";
			sprintf(result1, "%d", nodo->limInferior);
			XmTextSetString(canal_izq_preCalib, result1);
			sprintf(result1, "%d", nodo->limSuperior);
			XmTextSetString(canal_der_preCalib, result1);
			sprintf(result1, "%.4f", nodo->energia);
			XmTextSetString(energia_preCalib, result1);
			}
		}
	}

//Activa el botón PreCalibrate
void ActivaPreCalibracion () 
	{
   	XtSetSensitive(boton_precalibra, true);
	}

//Desactiva el botón PreCalibrate
void DesactivaPreCalibracion () 
	{
   	XtSetSensitive(boton_precalibra, false);
	}

//************************* RECONSTRUYE LISTA PRECALIBRACION *******************
//Se llama después de pulsar pulsa_precalibración y crea la ventana del tamaño apropiado
//y la rellena con la información de todos los nodos que se encuentren en listaPrecalibracion

void ReconstruyeListaPrecalibracion() 
	{
	tPrecalibracion datosPre;
	char texto_canales[80];
	XmString t;

	if (dialog_preCalib) 
		{
		// hace la lista del tamagno necesario para que quepan los picos
		if (listaPrecalibracion.numElementos > 5)
			XtVaSetValues (lista_preCalib, XmNvisibleItemCount, listaPrecalibracion.numElementos, NULL);
		else
			XtVaSetValues (lista_preCalib, XmNvisibleItemCount, 5, NULL);
		XmListDeleteAllItems(lista_preCalib);
		for (int i = 1; i <= listaPrecalibracion.numElementos; i++) 
			{
			// coge cada elemento en la lista de precalibracion y lo
			// convierte en una entrada en el widget lista de precalibracion
			DevuelveDatoPreCalibracion(listaPrecalibracion, i, datosPre);
			sprintf(texto_canales, "%.2f keV from %d to %d", datosPre.energia, datosPre.limInferior, datosPre.limSuperior);
			t = XmStringCreateLocalized (texto_canales);
			XmListAddItemUnselected(lista_preCalib, t, 0);
			XmStringFree (t);
			}
		}
	}

void PonIntervaloPreCalibracion(int minimo, int maximo) 
	{
	char result1[]="xxxxxxxx";

	if (dialog_preCalib) 
		{
		// pone el canal izquierdo
		if (minimo < 1) minimo = 1;
		sprintf(result1, "%d", minimo);
		XmTextSetString(canal_izq_preCalib, result1);
		// pone el canal derecho
		if (maximo > Datos.numCanales) maximo = Datos.numCanales;
		sprintf(result1, "%d", maximo);
		XmTextSetString(canal_der_preCalib, result1);
		}
	}

tListaPrecalib* DevuelveListaPreCalibracion() 
	{
	return &listaPrecalibracion;
	}

void CopiaListaPreCalibracion(tListaPrecalib lista) 
	{
	borraLista(listaPrecalibracion);
	listaPrecalibracion = lista;
	}


/******************************************************************************
 ******
 ******                             AJUSTES
 ******
 ******************************************************************************/

void ActualizaVectorCuadradoCalib(double pos) 
	{
	Datos.vectorCuadratica[1] = 1.0;
	Datos.vectorCuadratica[2] = pos;
	Datos.vectorCuadratica[3] = pos*pos;
	}

//************ ACTUALIZA VECTOR ENERGIA CALIB ***********
/*
	Añadimos un término 1/x a la función vectorCuadratica para ver si
	la calibración sale mejor para el caso del detector rege.
*/
void ActualizaVectorEnergiaCalib(double pos) 
	{
	Datos.vectorEnergia[1] = 1.0;
	Datos.vectorEnergia[2] = pos;
	Datos.vectorEnergia[3] = pos*pos;
	Datos.vectorEnergia[4] = 1.0/pos;
	}

void ActualizaVectorRectaCalib(double pos) 
	{
	Datos.vectorRecta[1] = 1.0;
	Datos.vectorRecta[2] = pos;
	}

double ActualizaVectorPicoCalib(double pos, double* param, double* dParam) 
	{
	/* 0 -> Centroide
		1 -> anchura
		2 -> tau
		3 -> Ytau
		dParam -> derivada parcial */

	// Calcula el valor de Y
	double temp = param[3]/2.0;
	temp *= erfc(((pos - param[0])/param[1] + param[1]/param[2])*M_SQRT1_2);
	double popo = powi(param[1]/param[2], 2)/2.0 + (pos - param[0])/param[2];
		/* Para evitar problemas por exceso de desbordamiento
		   fuerza un "infinito" en 10^100 dejando de margen de los
			calculos internos hasta el 10^308 del formato double */
	if (popo > 230) temp = 1e+100;
	else
		temp *= exp(popo);
	
	// valor de apoyo para las derivadas
	double temp2 = -powi((pos - param[0])/param[1], 2)/2.0;
	temp2 = -exp(temp2)*param[3]/sqrt(M_2PI);
	
	dParam[0] = -temp/param[2] - temp2/param[1];
	dParam[1] = param[1]*(temp/powi(param[2], 2)) +
							temp2*(1/param[2] - (pos - param[0])/param[1]);
	dParam[2] = - ((pos - param[0] + powi(param[1], 2)/param[2])*temp +
								temp2*param[1])/powi(param[2], 2);
	dParam[3] = temp/param[3];
	return temp;
	}

/******************************************************************************
 ******
 ******                             VISUALIZACIONES
 ******
 ******************************************************************************/

void EstableceShowsCalibracion(const tEstadoVisual Estado) 
	{
	XmToggleButtonSetState(show_adjust_calibracion, Estado.AjusteCalibracion, false);
	XmToggleButtonSetState(show_autoZoom_calibracion, Estado.AutoZoomCalibracion, false);
	// Activa o desactiva las graficas segun los valores
	ActivaAjustesGrafica(Estado.AjusteCalibracion);
	}
