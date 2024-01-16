#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>//Para control de tiempos

#include <locale.h>

//Para manejo de ficheros
#include <fstream>
#include <iostream>
// Para el tratamiento de cadenas
#include <string>
using namespace std;

#include "VisAnal.h"
#include "VisConti.h" //para seleccionarTexto()

#include "ajustes.h"
#include "analisis.h"
#include "avisos.h"
#include "constDef.h"
#include "cursores.h"
#include "fuentes.h"
#include "ficheros.h"
#include "globales.h"
#include "VisGraf.h"
#include "math1.h"
#include "picos.h"
#include "trabajo.h"
#include "trabajoEst.h"
#include "VisCalib.h"
#include "VisIdent.h"
#include "VisActiv.h"

#include "GA/GeneticAlgorithm2.h"
#include "unistd.h"

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
#include <Xm/FileSB.h>

// Widget general
Widget fondo_analisis, label_form_analisis;
Widget linea_anal_1, linea_anal_2;

// Widgets de Analisis
Widget interv_frame_analisis, label_interv_frame_analisis,
		 interv_lista_analisis, bt_add_interv_analisis;
Widget result_frame_analisis, label_result_frame_analisis,
		 result_lista_analisis, 
		 bt_form_result_analisis,
		 bt_del_result_analisis, bt_edit_result_analisis, bt_add_result_analisis;
Widget show_frame_analisis, label_frame_analisis, show_rowcol_analisis;
Widget show_adjust_analisis, show_parcialAdjust_analisis,
		 show_autoZoom_analisis;
//Widgets de análisis para el fondo
Widget fr_FDB_identify_edAnal, fm_FDB_identify_edAnal;
	Widget lb_FDB_identify_edAnal;
	Widget txt_FDB_energia, txt_FDB_nombre, bt_FDB_identifica;

		 
//NUEVO: Botones para las distintas acciones del análisis
/*
Widget action_frame_analisis, action_label_frame_analisis, action_rowcol_analisis;
Widget action_activity_analisis, action_efficiency_analisis,
	   action_limits_analisis;
*/Widget rowcol_analisis;

// Widgets del menu de adición de Análisis
Widget dlg_adAnal;	
Widget lb_cent_adAnal, txt_cent_adAnal;
Widget fr_adAnal;
Widget bt_addc_adAnal, bt_adde_adAnal, bt_del_adAnal;
Widget bt_ok_adAnal, bt_cancel_adAnal, bt_adjust_adAnal;
Widget listEnergy_adAnal, fm_botones_adAnal, fm_DB_identify_adAnal,
		fr_DB_identify_adAnal, fm_DB_identify2_adAnal, lb_DB_identify_adAnal,
		lbEmission_energy_adAnal, fm_emi_identify_adAnal, fm_energy_adAnal;
Widget rw1_adAnal, rw2_adAnal, rw3_adAnal;

// Widgets del menu de edicion de Analisis
Widget dlg_edAnal;
Widget fr_adjust_edAnal, fr_energy_edAnal,
		 linea_edAnal, fm_botones_edAnal;
Widget fm_adjust_edAnal, lb_adjust_edAnal,
		 rw1_adjust_edAnal, rw2_adjust_edAnal,
		 lbCenEn_adjust_edAnal, txtCen_adjust_edAnal,
		 lbCen_adjust_edAnal, lbTau_adjust_edAnal,
		 lbSigma_adjust_edAnal,
		 lbSigmaEn_adjust_edAnal, txtSigma_adjust_edAnal,
		 lbTauEn_adjust_edAnal, txtTau_adjust_edAnal,
		 lbYtau_adjust_edAnal, txtYtau_adjust_edAnal,
		 lbArea_adjust_edAnal, txtArea_adjust_edAnal,
		 lbChiSq_adjust_edAnal;
Widget fm_energy_edAnal, lb_energy_edAnal,
		 rw_Izq_energy_edAnal,
		 fr_DB_energy_edAnal, lb_DB_energy_edAnal, fm_DB_energy_edAnal,
		 energyTxt_energy_edAnal,
		 rw1_DB_energy_edAnal, rw2_DB_energy_edAnal,
		 ls_DB_energy_edAnal, bt_DB_energy_edAnal,
		 tgAll_DB_energy_edAnal,
		 tgElem_DB_energy_edAnal, txtElem_DB_energy_edAnal,
		 tgMin_DB_energy_edAnal, txtMin_DB_energy_edAnal,
		 tgMax_DB_energy_edAnal, txtMax_DB_energy_edAnal,
		 listEnergy, listElement;
Widget fr_DB_identify_edAnal,lb_DB_identify_edAnal,
		bt_DB_identify_edAnal, rw3_adjust_edAnal, rw4_adjust_edAnal,
		fm_DB_identify_edAnal, fm_DB_identify2_edAnal,
		lbGraf_adjust_edAnal, lbEcuat_adjust_edAnal,
		lbVoid_energy_edAnal, lbElement_energy_edAnal;
Widget bt_DB_unidentify_edAnal;

Widget lbEmission_energy_edAnal;
Widget bt_ok_edAnal, bt_cancel_edAnal, bt_adjust_edAnal;
char mensajeError[100];
int nodoAagregar=-1;

// Funciones
Widget Crea_AreaAnalisis (Widget);
void iniciaBotones();
// Analisis
bool IsActiveAnalisis();
void ReconstruyeListaAnalisis();
void ReponeListaAnalisis();
void ReconstruyeListaIntervAnalisis();
void BorraListaIntervAnalisis();
bool IsInListaAnalisis(int);
tListaAnalisis* DevuelveListaAnalisis();
void CopiaListaAnalisis(tListaAnalisis);
void CreateDialogAnalisis();
void CreateDialogAdicion();
void ActualizaIdentificacionAnalisis(unsigned short int);
void ActualizaIdentificacionAnalisis(char *, char *);
int analisisExperto(int numIntervalo, float *, float *);
int analisisExperto(int numIntervalo, float *, float *, int *, int);

//Despliegue de listas
void desplegarLista(Widget, int, NodoPicos*, string, bool);
void determinaDesplegadas(Widget, bool *, int);
int determinaPosicion(int numIntervalo);
int determinaIntervalo(int posicion);
void borraPosicionLista(Widget w, int posicion, int, bool);
bool estaPosEnListaAbierta(int nPos);
bool esSubpico(int nPos);
//

LinkedList* construyeListaRelacionados(NodoPico *nodo);
void imprimeLista(LinkedList *lista);
void imprimeNodo(NodoPico *np);
void imprimeActividades();

// Visualizaciones
void EstableceShowsAnalisis(const tEstadoVisual);
bool todosIdentificados();
char *buscaEmisionFBD(int, int, char *);
char *buscaNombreFBD(float, char *);
LinkedList *calculaMatricesActividad();
float actividadModelo(int, float, float, char *, float &, float &);
float probabilidadBifurcaciones(int, float &);
int posicionEnCadena(int, int);
void obtener_datos_edit_analisis();

// Callbacks
	// Analisis
void pulsa_lista_interv_analisis(Widget, XtPointer, XtPointer);
void pulsa_add_interv_analisis(Widget, XtPointer, XtPointer);
void pulsa_lista_result_analisis(Widget, XtPointer, XtPointer);
void pulsa_del_result_analisis (Widget, XtPointer, XtPointer);
void pulsa_edit_result_analisis (Widget, XtPointer, XtPointer);
void pulsa_add_result_analisis(Widget, XtPointer, XtPointer);

	// Edicion Analisis
void pulsa_cancel_edit_analisis (Widget, XtPointer, XtPointer);
void pulsa_ok_edit_analisis (Widget, XtPointer, XtPointer);
void pulsa_adjust_analisis (Widget, XtPointer, XtPointer);	
void pulsa_adjust_edit_analisis (Widget, XtPointer, XtPointer);	
void pulsa_identifica_edit_analisis (Widget, XtPointer, XtPointer);	
void pulsa_quita_identifica_edit_analisis (Widget, XtPointer, XtPointer);	
void cambia_texto_FBD (Widget, XtPointer, XtPointer);	

// Adición
void pulsa_ok_adicion(Widget, XtPointer, XtPointer); 
void pulsa_cancel_adicion(Widget, XtPointer, XtPointer); 
void pulsa_adjust_adicion(Widget, XtPointer, XtPointer); 
void pulsa_addc_adAnal(Widget, XtPointer, XtPointer); 
void pulsa_adde_adAnal(Widget, XtPointer, XtPointer); 
void pulsa_quita_identifica_adAnal(Widget, XtPointer, XtPointer); 

// Borrado
void deletePeak(int nPos);
void deleteAllPeaks();

void pulsa_identifica_FDB_edit_analisis (Widget, XtPointer, XtPointer);	
	// Menu de Shows
void pulsa_show_analisis(Widget, XtPointer, XtPointer);

//Funciones relacionadas con el cuadro de diálogo para ficheros .rn
void pulsaOpenFile(Widget, XtPointer, XtPointer);
void pulsaOpenFile_2(Widget, XtPointer, XtPointer);
void pulsaCancelFile(Widget, XtPointer, XtPointer);
void pulsaOKFile(Widget, XtPointer, XtPointer);
void pulsaOKFile_2(Widget, XtPointer, XtPointer);
void pulsaOKFileEficiencia(Widget, XtPointer, XtPointer);

//Funciones relacionadas con los botones del la sección Compute
void pulsa_limits(Widget, XtPointer, XtPointer);
void pulsa_efficiency(Widget, XtPointer, XtPointer);
void pulsa_activity(Widget, XtPointer, XtPointer);
void pulsa_MDA(Widget, XtPointer, XtPointer);

//Funciones relacionadas con el cómputo de los límites de detección
void creaDialogoLD();
void QuitaVentanaLD(Widget widget, XtPointer client_data, XtPointer call_data);
void guardarLD(String);
void guardarLD(const char*, nodoCentroide *);
nodoCentroide *lecturaEmisiones(char *);
void escribeProton(Widget, XtPointer, XtPointer);
void seleccionaElemento(Widget, XtPointer, XtPointer);
void seleccionaEmision(Widget, XtPointer, XtPointer);

//Funciones relacionadas con el cálculo de los límites de decisión
void creaDialogoLC(Widget widget, XtPointer client_data, XtPointer call_data);
void QuitaVentanaLC(Widget widget, XtPointer client_data, XtPointer call_data);

//Funciones relacionadas con el cálculo de la eficiencia
int leeNumPars(String);
double leeChiEficiencia(String);
void leePars(String, int, double*);
void leeNodosEfi(String, int, double*);
void leeMatrizEfi(String, int, double**);
void QuitaVentanaEfi(Widget widget, XtPointer client_data, XtPointer call_data);

//Funciones relacionada con el cálculo de los factores de corrección de la eficiencia
void construyeListaCorreccion(char *);
void recogeFactoresCorreccion(char *);
void pulsaOKCSC(Widget, XtPointer, XtPointer);
void pulsaCancelCSC(Widget, XtPointer, XtPointer);
//void sustituyePuntoPorComa(char *);
void preparaFichero(char *);
void sustituyeCaracter(char *, char *, char *);

//Funciones relacionadas con el ajuste genético
int devuelveIntervaloSeleccionado();//NOTA: Realmente es una función genérica, que tiene más que ver con las sublistas
int determinarCentroides(cSearchLaraBD, float *);
void determinarCentroides(float, float, float *, char *);
int determinarNumCentroides(float, float, char *);
void determinarNaturales(float, LinkedList *, char *);
void determinarRestricciones(float *, int, float *, char *);
void imprimeRestricciones();
//char *cadenaRestriccion[200];
char cadenaRestriccion[200][200];
int contRestricciones=0;

NodoPico *emisionEnMultipico(NodoPicos *, int, int, char *, bool);
//float gradoSolapamiento(int, int);
float gradoSolapamiento(NodoPicos *, NodoPico *);
bool tieneMultipleEmision(NodoPico *);
void determinarEquilibrios(char *);
float actividadAsociada(int);
float sigmaActividadAsociada(int);
bool equilibrioDeterminado=false;
float **actividades;
float **sigmaActividades;

// Datos
// para el menu de edicion de analisis
tPicoAjuste AjusteSeguridadAnal;
int numIntervaloEdicionAnal;
int numPicoEdicionAnal;
tListaAnalisis listaAnalisis = {NULL, 0};
cVisIdentify AnalIdent; // elements identification

// Constantes
const int BOTON_SHOW_AJUSTE = 1,
		  BOTON_SHOW_AJUSTEPARCIAL = 2,
		  BOTON_SHOW_AUTOZOOM = 3;



Widget Crea_AreaAnalisis(Widget parent) {
    Arg		 args[7];
    Cardinal i = 0;
	XmString cadena;

	i = 0;
    fondo_analisis = XmCreateForm (parent, "fondo_analisis", args, i);
	
	// Crea el titulo del form
	i = 0;
	cadena = XmStringCreate ("Analysis", "BoldNormal");
    XtSetArg (args[i], XmNlabelString, cadena); i++;
    XtSetArg (args[i], XmNfontList, lista_fuentes); i++;
	label_form_analisis = XmCreateLabel(fondo_analisis, "label_form_analisis", args, i);
    XmStringFree (cadena);
	XtManageChild(label_form_analisis);

	// crea las lineas de separacion
	i = 0;
    XtSetArg (args[i], XmNseparatorType, XmSHADOW_ETCHED_OUT); i++;
	linea_anal_1 = XmCreateSeparator(fondo_analisis, "linea_anal_1", args, i);
	linea_anal_2 = XmCreateSeparator(fondo_analisis, "linea_anal_2", args, i);
	XtManageChild(linea_anal_1);
	XtManageChild(linea_anal_2);
	

    // Crea seccion intervalos de ajuste
    i = 0;
	XtSetArg (args[i], XmNorientation, XmVERTICAL); i++;
	XtSetArg (args[i], XmNentryAlignment, XmALIGNMENT_CENTER); i++;
    interv_frame_analisis = XmCreateRowColumn (fondo_analisis, "interv_frame_analisis", args, i);
    cadena = XmStringCreate (" Intervals (keV)  ", "BoldNormal");
    i = 0;
    XtSetArg (args[i], XmNfontList, lista_fuentes); i++;
    XtSetArg (args[i], XmNlabelString, cadena); i++;
    label_interv_frame_analisis = XmCreateLabelGadget (interv_frame_analisis, "label_interv_frame_analisis", args, i);
    XmStringFree (cadena);
	i = 0;
	XtSetArg (args[i], XmNlistSizePolicy, XmCONSTANT); i++;
	XtSetArg (args[i], XmNselectionPolicy, XmEXTENDED_SELECT); i++;
	XtSetArg (args[i], XmNvisibleItemCount, 9); i++;
	XtSetArg (args[i], XmNscrollBarDisplayPolicy, XmSTATIC); i++;
    XtSetArg (args[i], XmNfontList, lista_fuentes_fixed); i++;
	interv_lista_analisis = XmCreateScrolledList(interv_frame_analisis, "interv_lista_analisis", args, i);
	XtAddCallback (interv_lista_analisis, XmNextendedSelectionCallback, pulsa_lista_interv_analisis, NULL);
	
	i = 0;
    cadena = XmStringCreateLocalized (" => ");
    XtSetArg (args[i], XmNlabelString, cadena); i++;
	XtSetArg (args[i], XmNalignment, XmALIGNMENT_CENTER); i++;
    XtSetArg (args[i], XmNfontList, lista_fuentes); i++;
	bt_add_interv_analisis = XmCreatePushButton(interv_frame_analisis, "bt_add_interv_analisis", args, i);
	XtAddCallback (bt_add_interv_analisis, XmNactivateCallback, pulsa_add_interv_analisis, NULL);
    XmStringFree (cadena);

    XtVaSetValues (interv_frame_analisis,
		XmNbottomAttachment, XmATTACH_FORM,
		XmNbottomOffset, 20,
		XmNtopAttachment, XmATTACH_FORM,
		XmNtopOffset, 10,
		XmNleftAttachment, XmATTACH_FORM, 
		XmNleftOffset, 10,
		NULL);

	XtManageChild(label_interv_frame_analisis);
	XtManageChild(interv_lista_analisis);
	XtManageChild(bt_add_interv_analisis);
	XtManageChild(interv_frame_analisis);

	// **** OPCIONES DE REPRESENTACION ****

	//Columna a la que se añadirá tanto la parte de visualización como la de computación
    i = 0;
    rowcol_analisis = XmCreateRowColumn(fondo_analisis, "rowcol_analisis", args, i);

   // Crea opciones de representacion general
   i = 0;
   show_frame_analisis = XmCreateFrame (rowcol_analisis, "show_frame_analisis", args, i);

   cadena = XmStringCreateLocalized ("Show");
   i = 0;
   XtSetArg (args[i], XmNchildType, XmFRAME_TITLE_CHILD); i++;
   XtSetArg (args[i], XmNchildVerticalAlignment, XmALIGNMENT_CENTER); i++;
   XtSetArg (args[i], XmNlabelString, cadena); i++;
   XtSetArg (args[i], XmNfontList, lista_fuentes); i++;
   label_frame_analisis = XmCreateLabelGadget (show_frame_analisis, "label_frame_analisis", args, i);
   XmStringFree (cadena);

   i = 0;
   show_rowcol_analisis = XmCreateRowColumn(show_frame_analisis, "show_rowcol_analisis", args, i);

	// Toggle button para el ajuste total
   cadena = XmStringCreateLocalized ("Total Fit");
   i = 0;
   XtSetArg (args[i], XmNset, false); i++;
   XtSetArg (args[i], XmNlabelString, cadena); i++;
   XtSetArg (args[i], XmNfontList, lista_fuentes); i++;
	show_adjust_analisis = XmCreateToggleButton (show_rowcol_analisis, "show_adjust_analisis", args, i);
	XtAddCallback (show_adjust_analisis, XmNvalueChangedCallback, pulsa_show_analisis, (XtPointer) BOTON_SHOW_AJUSTE);
   XmStringFree (cadena);
	// Toggle buton para los ajustes
   cadena = XmStringCreateLocalized ("Partial Fit");
   i = 0;
   XtSetArg (args[i], XmNset, false); i++;
   XtSetArg (args[i], XmNlabelString, cadena); i++;
   XtSetArg (args[i], XmNfontList, lista_fuentes); i++;
	show_parcialAdjust_analisis = XmCreateToggleButton (show_rowcol_analisis, "show_parcialAdjust_analisis", args, i);
	XtAddCallback (show_parcialAdjust_analisis, XmNvalueChangedCallback, pulsa_show_analisis, (XtPointer) BOTON_SHOW_AJUSTEPARCIAL);
   XmStringFree (cadena);
	// Toggle buton para el autozoom
   cadena = XmStringCreateLocalized ("Automatic\n  Zoom");
   i = 0;
   XtSetArg (args[i], XmNset, false); i++;
   XtSetArg (args[i], XmNlabelString, cadena); i++;
   XtSetArg (args[i], XmNfontList, lista_fuentes); i++;
	show_autoZoom_analisis = XmCreateToggleButton (show_rowcol_analisis, "show_autoZoom_analisis", args, i);
	XtAddCallback (show_autoZoom_analisis, XmNvalueChangedCallback, pulsa_show_analisis, (XtPointer) BOTON_SHOW_AUTOZOOM);
   XmStringFree (cadena);

   XtVaSetValues (rowcol_analisis,
		XmNbottomAttachment, XmATTACH_FORM,
		XmNbottomOffset, 10,
		XmNtopAttachment, XmATTACH_FORM,
		XmNtopOffset, 10,
		XmNrightAttachment, XmATTACH_FORM,
		XmNrightOffset, 10,
		NULL);
	

	XtManageChild(label_frame_analisis);
	XtManageChild(show_autoZoom_analisis);
	XtManageChild(show_parcialAdjust_analisis);
	XtManageChild(show_adjust_analisis);
	XtManageChild(show_rowcol_analisis);
	XtManageChild(show_frame_analisis);
	XtManageChild(rowcol_analisis);

	// **** FIN OPCIONES DE REPRESENTACION ***

	// **** FIN OPCIONES DE ACCIÓN ***

   // Crea seccion resultados de ajuste
   i = 0;
	XtSetArg (args[i], XmNorientation, XmVERTICAL); i++;
	XtSetArg (args[i], XmNentryAlignment, XmALIGNMENT_CENTER); i++;
   result_frame_analisis = XmCreateRowColumn (fondo_analisis, "result_frame_analisis", args, i);
   cadena = XmStringCreate ("Analysis List", "BoldNormal");
   i = 0;
   XtSetArg (args[i], XmNfontList, lista_fuentes); i++;
   XtSetArg (args[i], XmNlabelString, cadena); i++;
   label_result_frame_analisis = XmCreateLabelGadget (result_frame_analisis, "label_result_frame_analisis", args, i);
   XmStringFree (cadena);
	i = 0;
	XtSetArg (args[i], XmNlistSizePolicy, XmCONSTANT); i++;
	XtSetArg (args[i], XmNselectionPolicy, XmEXTENDED_SELECT); i++;
	XtSetArg (args[i], XmNvisibleItemCount, 9); i++;
	XtSetArg (args[i], XmNscrollBarDisplayPolicy, XmSTATIC); i++;
    XtSetArg (args[i], XmNfontList, lista_fuentes); i++;
	result_lista_analisis = XmCreateScrolledList(result_frame_analisis, "result_lista_analisis", args, i);
	XtAddCallback (result_lista_analisis, XmNdefaultActionCallback, pulsa_edit_result_analisis, NULL);
	XtAddCallback (result_lista_analisis, XmNextendedSelectionCallback, pulsa_lista_result_analisis, NULL);

	i = 0;
//	XtSetArg (args[i], XmNfractionBase, 2); i++;
	XtSetArg (args[i], XmNfractionBase, 99); i++;
	bt_form_result_analisis = XmCreateForm(result_frame_analisis, "bt_form_result_analisis", args, i);
	
	//Botón para borrado de picos
	i = 0;
   	cadena = XmStringCreateLocalized ("Delete");
   	XtSetArg (args[i], XmNlabelString, cadena); i++;
	XtSetArg (args[i], XmNalignment, XmALIGNMENT_CENTER); i++;
   	XtSetArg (args[i], XmNfontList, lista_fuentes); i++;
	bt_del_result_analisis = XmCreatePushButton(bt_form_result_analisis, "bt_del_result_analisis", args, i);
	XtAddCallback (bt_del_result_analisis, XmNactivateCallback, pulsa_del_result_analisis, NULL);
   	XmStringFree (cadena);

	//Botón para edición de picos
	i = 0;
   	cadena = XmStringCreateLocalized ("Edit");
   	XtSetArg (args[i], XmNlabelString, cadena); i++;
	XtSetArg (args[i], XmNalignment, XmALIGNMENT_CENTER); i++;
   	XtSetArg (args[i], XmNfontList, lista_fuentes); i++;
	bt_edit_result_analisis = XmCreatePushButton(bt_form_result_analisis, "bt_edit_result_analisis", args, i);
	XtAddCallback (bt_edit_result_analisis, XmNactivateCallback, pulsa_edit_result_analisis, NULL);
   	XmStringFree (cadena);

	//Botón para adición de picos
	i = 0;
   	cadena = XmStringCreateLocalized ("Add");
   	XtSetArg (args[i], XmNlabelString, cadena); i++;
	XtSetArg (args[i], XmNalignment, XmALIGNMENT_CENTER); i++;
   	XtSetArg (args[i], XmNfontList, lista_fuentes); i++;
	bt_add_result_analisis = XmCreatePushButton(bt_form_result_analisis, "bt_add_result_analisis", args, i);
	XtAddCallback (bt_add_result_analisis, XmNactivateCallback, pulsa_add_result_analisis, NULL);
   	XmStringFree (cadena);
	//
	
  	XtVaSetValues (bt_del_result_analisis,
		XmNleftAttachment, XmATTACH_FORM,
		XmNtopAttachment, XmATTACH_FORM,
		XmNbottomAttachment, XmATTACH_FORM,
		XmNrightAttachment, XmATTACH_POSITION, XmNrightPosition, 33,
		XmNleftOffset, 10,
		NULL);
   XtVaSetValues (bt_edit_result_analisis,
		XmNtopAttachment, XmATTACH_FORM,
		XmNbottomAttachment, XmATTACH_FORM,
		XmNleftAttachment, XmATTACH_POSITION, XmNleftPosition, 33,
		XmNrightAttachment, XmATTACH_POSITION, XmNrightPosition, 66,
		NULL);
   XtVaSetValues (bt_add_result_analisis,
		XmNtopAttachment, XmATTACH_FORM,
		XmNrightAttachment, XmATTACH_FORM,
		XmNbottomAttachment, XmATTACH_FORM,
		XmNleftAttachment, XmATTACH_POSITION, XmNleftPosition, 66,
		XmNrightOffset, 10,
		NULL);
	
	XtVaSetValues (bt_form_result_analisis,
		XmNleftAttachment, XmATTACH_FORM,
		XmNrightAttachment, XmATTACH_FORM,
		XmNbottomAttachment, XmATTACH_FORM,
		NULL);
	XtVaSetValues(result_lista_analisis,
		XmNbottomAttachment, XmATTACH_WIDGET, XmNbottomWidget, bt_form_result_analisis,
		NULL);
   XtVaSetValues (result_frame_analisis,
		XmNbottomAttachment, XmATTACH_FORM,
		XmNbottomOffset, 20,
		XmNtopAttachment, XmATTACH_FORM,
		XmNtopOffset, 10,
		XmNrightAttachment, XmATTACH_WIDGET, XmNrightWidget, show_frame_analisis,
		XmNrightOffset, 10,
		XmNleftAttachment, XmATTACH_WIDGET, XmNleftWidget, interv_frame_analisis,
		XmNleftOffset, 10,
		NULL);

	XtManageChild(label_result_frame_analisis);
	XtManageChild(result_lista_analisis);
	XtManageChild(bt_del_result_analisis);
	XtManageChild(bt_edit_result_analisis);
	XtManageChild(bt_add_result_analisis);
	XtManageChild(bt_form_result_analisis);
	XtManageChild(result_frame_analisis);


	// coloca los widgets
   XtVaSetValues (linea_anal_1,
		XmNleftAttachment, XmATTACH_FORM,
		XmNtopAttachment, XmATTACH_FORM,
		XmNrightAttachment, XmATTACH_FORM,
		NULL);
   XtVaSetValues (linea_anal_2,
		XmNleftAttachment, XmATTACH_FORM,
		XmNrightAttachment, XmATTACH_FORM,
		XmNbottomAttachment, XmATTACH_FORM,
		NULL);
   XtVaSetValues (label_form_analisis,
		XmNleftAttachment, XmATTACH_FORM,
		XmNbottomAttachment, XmATTACH_WIDGET, XmNbottomWidget, linea_anal_2,
		XmNleftOffset, 10,
		NULL);
   XtVaSetValues (fondo_analisis,
		XmNrightAttachment, XmATTACH_FORM,
		XmNleftAttachment, XmATTACH_FORM,
		XmNtopAttachment, XmATTACH_FORM,
		XmNbottomAttachment, XmATTACH_FORM,
		NULL);

	return fondo_analisis;
	}


/******************************************************************************
 ******
 ******                             ANALISIS
 ******
 ******************************************************************************/

bool IsInListaAnalisis(int numIntervalo) 
	{
	bool presente = false; int i = 0;
	while ((!presente)&&(i < listaAnalisis.numElementos)) 
		{
		i++;
		NodoAnalisis *nodo = DevuelveNodo(listaAnalisis, i);
		if (nodo->numIntervalo == numIntervalo) presente = true;
		}
	return presente;
	}

tListaAnalisis* DevuelveListaAnalisis() 
	{
	return &listaAnalisis;
	}

void CopiaListaAnalisis(tListaAnalisis lista) 
	{
	borraLista(listaAnalisis);
	listaAnalisis = lista;
	}

//******************* REPONE LISTA ANALISIS *****************
//NOTA: Con las listas desplegables, convendría que si estamos dentro de una
	//	y, por ejemplo, tenemos seleccionado el elemento 2 de 5 nos aparezca
	//	seleccionado el 2 pero podamos ver la mayor parte de la lista
void ReponeListaAnalisis() 
	{
	// busca el elemento seleccionado
	int *posiciones, numSel, nPos;
	if (!XmListGetSelectedPos(result_lista_analisis, &posiciones, &numSel))
		nPos = 0;
	else 
		{
		nPos = *posiciones;
		XtFree((char *)posiciones);
		}
	// Extraño error, mas de un pico seleccionado
	if (numSel > 1) nPos = 0;

	ReconstruyeListaAnalisis();
	
	// Vuelve a poner el elemento seleccionado
	if (nPos > 0) 
		{
		XmListSelectPos(result_lista_analisis, nPos, false);
		XmListSetBottomPos(result_lista_analisis, nPos);
	
		if (EstadoVisual.AutoZoomAnalisis) 
			{
			int interv=0, pic=0;
			determinaPico(result_lista_analisis, nPos, &interv, &pic);
			NodoPicos *intervalo = DevuelveNodo(Datos.listaPicos, interv);

			int limInferior, limSuperior;
	
			limInferior = intervalo->limInferior;
			limSuperior = intervalo->limSuperior;
			
			// muestra 3 veces el tamaño del intervalo
			// para que se aprecien las estructuras coetaneas
			int ancho = 2*(limSuperior - limInferior)/3;
			MarcaZoom(limInferior - ancho, limSuperior + ancho);
			}
		}
	}

//********************* RECONSTRUYE LISTA ANÁLISIS **********************
/* Pone en la lista central superior todos los picos de análisis que se 
	están considerando
	*/
void ReconstruyeListaAnalisis() 
	{
	char texto_canales[80], extra[20], aconsejado[3];
	XmString cadena;
	XmString energia;
	XmString total;
	total=XmStringCreate("", "BoldNormal");						

	Arg args[6];
	Cardinal n;
	int numItems;

	printf("RECONSTRUYE LISTA ANALISIS\n");
	//1) Antes de borrar todos los nodos para reconstruir con los nuevos elementos,
	//descubrimos cuáles estaban desplegados
	//NOTA: Da bastantes problemas porque hay que tener en cuenta muchos casos, y 
	//		además en general casi preferimos que al añadir uno todos aparezcan cerrados
	//		Comento el paso 1) y algunas partes del paso 3)
	bool *desplegadas;
	int tam;
	bool nodoNuevo=false;
	bool nodoAgregado=false;
		
	tam=listaAnalisis.numElementos;
	//En este caso, ya hay un nodo más en listaAnalisis pero no todavía aquí
	if(nodoAagregar!=-1)	tam=tam-1;//NOTA: OJO, puede ser que no si ya estaba este elemento
	desplegadas=new bool[tam];
	if(desplegadas==NULL)	
		{
		printf("ERROR: No hay memoria para reservar bool[%d]\n", listaAnalisis.numElementos);
		return;
		}
	for(int i=0;i<tam;i++)	desplegadas[i]=false;	
	determinaDesplegadas(result_lista_analisis, desplegadas, tam);	
	for(int i=0;i<tam;i++)	printf("%d ", desplegadas[i]);
	printf("\n");
	int numDesplegadas=0;
	
	//2) Borramos todos los nodos
	XmListDeleteAllItems(result_lista_analisis);
	
	printf("NodoAagregar=%d\n", nodoAagregar);
	//3) Añadimos todos los nodos nuevos que haya, de forma ordenada
	printf("Bucle hasta que añadamos %d elementos principales\n", listaAnalisis.numElementos);
	for (int i = 1; i <= listaAnalisis.numElementos; i++) 
		{
//		printf("elemento %d\n", i);
		// coge cada elemento en la lista de analisis y lo
		// convierte en una entrada en el widget lista de analisis
		NodoAnalisis *nodo = DevuelveNodo(listaAnalisis, i);
		NodoPicos *intervalo = DevuelveNodo(Datos.listaPicos, nodo->numIntervalo);
		if(nodoAagregar>=0 && nodoAagregar==numDesplegadas)
			{
			//Si tenemos un nodo nuevo al reconstruir y entra en este punto,
			//no tenemos en cuenta numDesplegadas sino que directamente la ponemos como
			//cerrada (si es lista) y NO incrementamos numDesplegadas
			nodoAagregar=-1;
			nodoNuevo=true;
			}
		
		//Si tiene varios picos en el intervalo sacaría sublista	
		if(intervalo->picos.numElementos>1)
			{
			printf("Lista de varios picos\n");
			int minEnergia = (int) floor(Datos.DaEnergia((double)intervalo->limInferior));
			int maxEnergia = (int) floor(Datos.DaEnergia((double)intervalo->limSuperior))+1;
	//		if(numDesplegadas==tam  || (!desplegadas[numDesplegadas] && !nodoNuevo))
			if(nodoNuevo || !desplegadas[numDesplegadas])
				{
				//if(nodoNuevo && desplegadas[numDesplegadas])	printf("La lista está abierta pero estamos en el nodo nuevo\n");
				sprintf(extra, "+[%d-%d]", minEnergia, maxEnergia);
				printf("Lista cerrada\n");
				if(intervalo->picos.numElementos==1)	;//sprintf(texto_canales, "%s keV with 1 peak", extra);
				else									sprintf(texto_canales, "%s keV with %d peaks", extra, intervalo->picos.numElementos);
				cadena = XmStringCreateLocalized (texto_canales);
				XmListAddItemUnselected(result_lista_analisis, cadena, 0);
				XmStringFree (cadena);
				}
			else
				{
				printf("Lista abierta\n");
				sprintf(extra, "-[%d-%d]", minEnergia, maxEnergia);
				if(intervalo->picos.numElementos==1)	;//sprintf(texto_canales, "%s keV with 1 peak", extra);
				else									sprintf(texto_canales, "%s keV with %d peaks", extra, intervalo->picos.numElementos);
				cadena = XmStringCreateLocalized (texto_canales);
				XmListAddItemUnselected(result_lista_analisis, cadena, 0);
				XmStringFree (cadena);
				
				//Y ahora todos sus elementos
				string cad(texto_canales);
				
				n=0;
				XtSetArg(args[n], XmNitemCount, &numItems); n++;
				XtGetValues(result_lista_analisis, args, n);
				desplegarLista(result_lista_analisis, numItems, intervalo, cad, false);
				}				
			}
		else//Un solo pico
			{			
			NodoPico *PicoAnalisis = DevuelveNodo(intervalo->picos, 1);

			if(PicoAnalisis->Ajuste.identificado)
				{
				XmString elemento;
				//bucle de emisiones
				for(int i=0;i<PicoAnalisis->Ajuste.emisiones->numElements();i++)
					{
					NodoEmision *ne=(NodoEmision *)(PicoAnalisis->Ajuste.emisiones->getNode(i));
					int codEmision = ne->codElemento;
					printf("codEmision=%d\n", codEmision);
					if(codEmision==-1)//Es una emisión que no está en lara
						{
						printf("EMISIÓN QUE NO ESTÁ EN LARA\n");
						printf("%s, %f\n", ne->nombre, ne->Energia);
						char format[50];
//						strcpy(format, "  %.3f keV");
						//felipe. Si no está en Lara imprime también la probabilidad
						if(i!=PicoAnalisis->Ajuste.emisiones->numElements()-1)	strcpy(format, "  %.3f keV (%.1f%%), ");						else	strcpy(format, "  %.3f keV (%.1f%%)");
						//felipe. Determina probabilidad si no está en Lara.
						string cad(BDIdentificaciones);						ne->probabilidad = determinarProbabilidad(ne->Energia,(char *)cad.c_str());
						//
						sprintf(extra, format, ne->Energia, ne->probabilidad );
						elemento= XmStringCreate(ne->nombre, "BoldNormal");
						//sprintf(extra, format, ne->nombre, ne->Energia);
						}
					else
						{
						//printf("Existe la emisión\n");
						if (LaraBD.GetIsRunning()) 
							{
							elemento = XmStringNameElement(LaraBD.GetEmissor(codEmision), "SubIndiceBold", "BoldNormal");
								
							// minimize decimals of probability
							char format[50];
							if(i!=PicoAnalisis->Ajuste.emisiones->numElements()-1)	strcpy(format, "  %.3f keV (%.1f%%), ");
							else	strcpy(format, "  %.3f keV (%.1f%%)");
							double Prob = LaraBD.GetProbability(codEmision);
					
							int rateProb = orden(Prob)-1;
							if ((Prob > 0)&&((rateProb < -1)&&(rateProb > -10)))	
								format[14] = (unsigned char) 0x30 + abs(rateProb);
							sprintf(extra, format, ne->Energia, LaraBD.GetProbability(codEmision));
							} 
						else
							{
							elemento = XmStringCreate ("???", "BoldNormal");
							// minimize decimals of probability
							char format[50];
							if(i!=PicoAnalisis->Ajuste.emisiones->numElements()-1)	strcpy(format, "  %.3f keV (??%%), ");
							else	strcpy(format, "  %.3f keV (??%%)");
							sprintf(extra, format, ne->Energia);
							}
						}
			
				/*	else //En el caso de que se haya elegido un elemento de la FBD
						{
						elemento = XmStringCreate (buscaNombreFBD(ne->Energia, nombreFichFBD), "BoldNormal");
						// minimize decimals of probability
						char format[50];
						if(i!=PicoAnalisis->Ajuste.emisiones->numElements()-1)	strcpy(format, "  %.3f keV, ");
						else	strcpy(format, "  %.3f keV");
						sprintf(extra, format, ne->Energia);
						}	*/
					XmString emision = XmStringCreate (extra, "BoldNormal");
					
					// concat element and emision info
					energia = XmStringConcat(elemento, emision);
					total = XmStringConcat(total, energia);
					XmStringFree(elemento); XmStringFree (emision); 
					XmStringFree(energia);
					}//for(emisiones)
					
					// si el pico cumple unas condiciones de relación entre sigma y tau
					// lo aconseja para calibracion señalandolo con un asterisco. Las
					//condiciones de calibracion son las que se dan en la inicializacion
					//del pico: tau +/- = sigma^2 / 4. Cualquier desviacion de esto indica
					//la presencia de otro pico o poca estadistica para que el ajuste
					//sea fiable. 
					double relacion = powi(PicoAnalisis->Ajuste.Anchura, 2)/4.0;
					relacion = fabs(relacion - PicoAnalisis->Ajuste.Tau)/PicoAnalisis->Ajuste.Tau;
					if (relacion < 0.1) sprintf(aconsejado, " *"); // un 10% de desvio
					else sprintf(aconsejado, " ");
		
//					printf("Le vamos a poner el with x peaks\n");
					if(intervalo->picos.numElementos==1)	
						sprintf(texto_canales, " %s", aconsejado);
					else	sprintf(texto_canales, " with %d peaks%s", intervalo->picos.numElementos, aconsejado);
					
					XmString cadena = XmStringCreateLocalized (texto_canales);
					XmString final = XmStringConcat(total, cadena);
	
					XmListAddItemUnselected(result_lista_analisis, final, 0);
					XmStringFree (cadena); 
					XmStringFree (final);
					XmStringFree (total);
					total=XmStringCreate("", "BoldNormal");	
				//	
				}//if(identificado)
			else //Si no está identificado mostramos la información normal
				{
				int minEnergia = (int) floor(Datos.DaEnergia((double)intervalo->limInferior));
				int maxEnergia = (int) floor(Datos.DaEnergia((double)intervalo->limSuperior))+1;
				sprintf(extra, "[%d-%d]", minEnergia, maxEnergia);
				sprintf(texto_canales, "%s keV with 1 peak", extra);
				cadena = XmStringCreateLocalized (texto_canales);
				XmListAddItemUnselected(result_lista_analisis, cadena, 0);
				XmStringFree (cadena);
				}
			}//else (1 solo pico)
		
		if(!nodoNuevo)	numDesplegadas++;
		else			{nodoNuevo=false; nodoAgregado=true;}
		}//For para cada intervalo
	printf("borramos desplegadas\n");
	if(desplegadas!=NULL)	delete [] desplegadas;
	printf("trminamos RECONSTRUYE LISTA ANÁLISIS\n");
	return;
	}

//************** ESTA POS EN LISTA ABIERTA **************
/*	Devuelve true si nPos está en una lista abierta.
	*/
bool estaPosEnListaAbierta(int nPos)
	{
	printf("ESTA POS EN LISTA ABIERTA %d\n", nPos);
	bool *desplegadas=new bool[listaAnalisis.numElementos];
	determinaDesplegadas(result_lista_analisis, desplegadas, listaAnalisis.numElementos);
	int k=0;
	bool estaEnListaAbierta=false;
	int cuenta=0; //Cuenta de todas las posiciones internas
	for(int i=0;i<nPos;i++)
		{
		if(desplegadas[k] && cuenta==0)	//Cogemos cuántos elementos tiene
			{
			NodoAnalisis *nodo = DevuelveNodo(listaAnalisis, i);
			NodoPicos *intervalo = DevuelveNodo(Datos.listaPicos, nodo->numIntervalo);
			cuenta=intervalo->picos.numElementos;
			estaEnListaAbierta=true;
			}
		else 
			{
			if(desplegadas[k] && cuenta !=0)
				{
				cuenta--;	
				if(cuenta==0)	k++;
				}
			else	
				{
				estaEnListaAbierta=false;
				k++;
				}
			}
		}
	delete [] desplegadas;
	return estaEnListaAbierta;
	}
	
//******************** ES SUBPICO *********************
/*	Devuelve true si nPos es un subpico (es decir, un elemento de una lista
	abierta, pero no la propia lista).
	Se diferencia de estaPosEnListaAbierta precisamente en eso, no incluye
	a la propia lista, sólo a sus elementos
	*/
bool esSubpico(int nPos)
	{
	printf("ES SUBPICO %d?\n", nPos);
	Arg args[6];
	Cardinal n;
	XmStringTable items;
	char *text=NULL;
	int numItems;
	
	n=0;
	XtSetArg(args[n], XmNitems, &items); n++;
	XtSetArg(args[n], XmNitemCount, &numItems); n++;
	XtGetValues(result_lista_analisis, args, n);
	
	for(int i=0;i<numItems;i++)
		{
		XmStringGetLtoR(items[i], XmFONTLIST_DEFAULT_TAG, &text);
		XtFree(text);
		}
	XmStringGetLtoR(items[nPos-1], XmFONTLIST_DEFAULT_TAG, &text);
	if(text[0]==' ' && text[1]==' ' && text[2]==' ')	{XtFree(text);	return true;}
	else							 					{XtFree(text);	return false;}
	}

//****************** DETERMINA DESPLEGADAS *********************
/* Pone a true los valores de listaDesplegadas correspondientes a posiciones 
en las que la lista widget tiene una cadena que empieza por '-'
*/
void determinaDesplegadas(Widget widget, bool *listaDesplegadas, int tam)
	{
	XmStringTable items;
	char *text=NULL;
	int numItems;
	
	printf("DETERMINA DESPLEGADAS\n");
	XtVaGetValues(widget, XmNitems, &items, XmNitemCount, &numItems, NULL);
	if(numItems==0)//Si aún no tenemos lista consideramos todo cero
		{
		for(int i=0;i<tam;i++)	listaDesplegadas[i]=0;
		return;
		}
		
	int k=0;
	bool listaTodas[numItems];
//	printf("Creamos la lista de todas\n");
	for(int i=0;i<numItems;i++)
		{
		XmStringGetLtoR(items[i], XmFONTLIST_DEFAULT_TAG, &text);
//		printf("|%s|\n", text);
		if(text[0]=='-')	listaTodas[i]=true;
		else				listaTodas[i]=false;
		if(text!=NULL)	{XtFree(text);text=NULL;}
		}

	k=0;
	for(int i=0;i<tam;i++)
		{
		NodoAnalisis *nodo   = DevuelveNodo(listaAnalisis, i+1);
		NodoPicos *intervalo = DevuelveNodo(Datos.listaPicos, nodo->numIntervalo);
		listaDesplegadas[i]=listaTodas[k+i];
		if(listaDesplegadas[i])	k+=intervalo->picos.numElementos;
		//if(listaDesplegadas[i])	k+=intervalo->picos.numElementos+1;//???
		}
	
	return;
	}
	
bool IsActiveAnalysis() 
	{
   	return XtIsSensitive(result_frame_analisis);
	}


/******************************************************************************
 ******
 ******                             INTERVALOS
 ******
 ******************************************************************************/

//************* PULSA ADD INTERV ANALISIS *****************
// Al pulsar "=>" en la interfaz de análisis
// Pasa el pico a la lista de la derecha, realizando previamente el ajuste para él.

void pulsa_add_interv_analisis(Widget widget, XtPointer client_data, XtPointer call_data) 
	{
	printf("_-_-_-_-_-_ PULSA ADD INTERV ANALISIS _-_-_-_-_-_\n");
	clock_t t0=0, tGA=0, tTotal=0;
	t0=clock();
	bool impRest=false;
		
	string cad(BDIdentificaciones);
	if(Datos.nodosEficiencia==NULL)
		{
		Aviso(widget, "There's no efficiency loaded. Fits may be unnacurate");
		}
	// Busca el intervalo que se quiere añadir
	int *posiciones, numSel;
	if (!XmListGetSelectedPos(interv_lista_analisis, &posiciones, &numSel))
		{
		Aviso(widget, "You must select an interval.");
		return;
		}
	//for(int i=0;i<numSel;i++)	printf("posiciones[%d]=%d\n", i, posiciones[i]);
	// por si la operacion dura mucho tiempo
	PonCursorEspera();

	// Indica cambio en el trabajo
	EstadoTrabajo.TrabajoCambios = true;
	PonTituloFichCambiado();
		
	if(!equilibrioDeterminado && opciones[3])	
		{
		//Aviso(widget, "Computing constraints, please wait ...");
		determinarEquilibrios((char *)cad.c_str());
		impRest=true;
		}
	
	// Para cada intervalo seleccionado hace el ajuste
	int posLista = 0, numIntervalo;
	while (posLista < numSel)
		{
		numIntervalo = posiciones[posLista];
		NodoPicos *intervalo = DevuelveNodo(Datos.listaPicos, numIntervalo);

		// marca los picos como no inicializados y procede a su ajuste
		for (int i = 1; i <= intervalo->picos.numElementos; i++) 
			{
			NodoPico *pico = DevuelveNodo(intervalo->picos, i);
			pico->Ajuste.inicializado = false;
			pico->Ajuste.identificado = false;
			pico->Ajuste.analizado=false;
			}

		//************** Comienza el ajuste
		int tipo=0;//0 para GAML y 1 para ML
		//printf("VAMOS A DETERMINAR EL NÚMERO DE CENTROIDES:\n");
		//printf("Entre %f y %f\n", Datos.DaEnergia(Datos.DaLimInferior(numIntervalo)), Datos.DaEnergia(Datos.DaLimSuperior(numIntervalo)));
		int numGaussianas=determinarNumCentroides(Datos.DaEnergia(Datos.DaLimInferior(numIntervalo)), Datos.DaEnergia(Datos.DaLimSuperior(numIntervalo)), (char *)cad.c_str());
		for(int i=0;i<maxOpciones;i++)	printf("%d\n", opciones[i]);
		printf("EL NÚMERO DE GAUSSIANAS QUE VAMOS A USAR PARA ACTUAR ES %d\n", numGaussianas);
		if(numGaussianas>0)
			{
			int nError=0;
			float centroides[numGaussianas];
			determinarCentroides(Datos.DaEnergia(Datos.DaLimInferior(numIntervalo)), Datos.DaEnergia(Datos.DaLimSuperior(numIntervalo)), centroides, (char *)cad.c_str());
			for(int i=0;i<numGaussianas;i++)	printf("c[%d]=%.2f (%.2f keV)\n", i, centroides[i], Datos.DaEnergia(centroides[i]) );
			
			float restricciones[numGaussianas];
			determinarRestricciones(centroides, numGaussianas, restricciones, (char *)cad.c_str());
			for(int i=0;i<numGaussianas;i++)	printf("restricción %d: %f\n", i, restricciones[i]);
			//Tomamos la función deseada con los totales de todos los intervalos
//			if(tipo==1)	Datos.AjusteIntervaloGA(numIntervalo, centroides, numGaussianas);
			if(tipo==1)	Datos.AjusteIntervaloML(numIntervalo, centroides, numGaussianas);
			else		nError=Datos.AjusteIntervaloGAML(numIntervalo, centroides, restricciones, numGaussianas,-1,-1,-1);
			if(nError==-3 || nError==-5)
				{
				printf("ERROR: %d\n", nError);
				Aviso(app_shell, "ERROR: Fitness Unaccurate, discarded");
				PonCursorNormal();
				return;
				}
			if(nError==-7)	//Problema con las calibraciones
				{
				printf("ERROR: %d\n", nError);
				Aviso(app_shell, "ERROR: Calibration is zero in form parameters. Fit undone, check calibration");
				PonCursorNormal();
				return;
				}
			tGA=clock();
			
			//Una vez terminado el ajuste, hacemos un análisis experto de los picos,
			//que luego a posteriori podrá modificar el usuario
			//Tomamos los nuevos picos ya ajustados
			float centroidesAjustados[intervalo->picos.numElementos];
			for (int i = 1; i <= intervalo->picos.numElementos; i++) 
				{
				NodoPico *pico = DevuelveNodo(intervalo->picos, i);
				pico->Ajuste.invisible=false;
				centroidesAjustados[i-1]=pico->Ajuste.Centroide;
				}
				
			//los analizamos
			if(nError!=-6)	
				{	
				float restriccionesAjustadas[intervalo->picos.numElementos];
				printf("Volvemos a determinar restricciones por si hay que reajustar\n");
				determinarRestricciones(centroidesAjustados, intervalo->picos.numElementos, restriccionesAjustadas, (char *)cad.c_str());	//NOTA: No debería hacerse con las restricciones ajustadas?
				for(int i=0;i<numGaussianas;i++)	printf("restAnt[%d]=%.2f\n", i, restricciones[i]);	
				for(int i=0;i<intervalo->picos.numElementos;i++)	printf("rest[%d]=%.2f\n", i, restriccionesAjustadas[i]);	
				analisisExperto(numIntervalo, centroidesAjustados, restriccionesAjustadas);
				//analisisExperto(numIntervalo, centroidesAjustados, NULL);
				printf("Volvemos del análisis experto\n");
				}
			else
				{
				//NOTA: De momento, en caso de que GAML no pueda resolver el ajuste
				int nError = Datos.AjusteIntervalo(numIntervalo);
				}
			}
		else	//Si no tenemos emisiones naturales, se usa el ajuste ML normal
			{
			printf("Llamamos a datos.ajustaintervalo\n");
			int nError = Datos.AjusteIntervalo(numIntervalo);
			switch (nError) 
				{
				case -1:
					PonCursorNormal();
					char texto[100];
					break; // continua con el resto de intervalos
				case -2:
					PonCursorNormal();
					Aviso(fondo_analisis, "No Memory Avalaible");
					printf(ERROR_ANALYSIS);
					printf(ERROR_NO_MEMORY);
					XtFree ((char *)posiciones);
					return;
				}
			printf("Intervalo ajustado por método normal\n");
			}
		// El ajuste es correcto por lo que lo añade en la lista de
		// picos para la calibracion
		NodoAnalisis *nodo = new NodoAnalisis;
		if(nodo==NULL)	
			{
			printf("ERROR: No hay memoria para reservar NodoAnalisis\n");
			return;
			}		
			
		nodo->numIntervalo = numIntervalo;
		if(!estaNodo(listaAnalisis, nodo))
			{
			int numIntervalo2 = agnadeNodo(listaAnalisis, nodo);//NOTA: Antes se hacía con el mismo numIntervalo
			nodoAagregar=numIntervalo2-1;
			}
		posLista++;


		ReconstruyeListaAnalisis();
		int nError = CopiaAjustesAGrafica();
		ActivaAjustesGrafica(EstadoVisual.AjusteTotalAnalisis);
		}//while(poslista<numSel)

	printf("Venimos del análisis y todo eso, terminando\n");
	//determinaDesplegadas(result_lista_analisis, desplegadas, listaAnalisis.numElementos);
	//printf("En listaAnalisis tenemos %d elementos\n", listaAnalisis.numElementos);
	XtFree ((char *)posiciones); // elimina la lista de intervalos seleccionados

	// Actualiza los datos de los picos en la grafica
	int nError = CopiaAjustesAGrafica();
	PonCursorNormal();
	if (nError < 0) 
		{
		Aviso(fondo_analisis, "No Memory Avalaible");
		printf(ERROR_ANALYSIS);
		printf(ERROR_NO_MEMORY);
		return;
		}
	
	ActivaAjustesGrafica(EstadoVisual.AjusteTotalAnalisis);

	if ((EstadoVisual.AjusteTotalAnalisis)||(EstadoVisual.AjusteParcialAnalisis))	PintaGrafica(0);

	//Recalculamos las áreas sin fondo si es necesario
	if(EstadoTrabajo.fondoCargado)	Datos.restaAreaFondo();

	//Activamos la interfaz de actividad
	ActivaActividad();
	
	// ponemos el último de los intervalos como seleccionado
	int posic=determinaPosicion(numIntervalo);
//	printf("La posición para el pico añadido (numIntervalo=%d) es %d\n", numIntervalo, posic);
	//	printf("Seleccionamos el elemento %d (add intervalo analisis)\n", posic);
	XmListSelectPos(result_lista_analisis, posic, false);
	XmListSetBottomPos(result_lista_analisis, posic);
	if (EstadoVisual.AutoZoomAnalisis) 
		{
		int interv=0, pic=0;
		determinaPico(result_lista_analisis, posic, &interv, &pic);
		NodoPicos *intervalo = DevuelveNodo(Datos.listaPicos, interv);

		int limInferior, limSuperior;

		limInferior = intervalo->limInferior;
		limSuperior = intervalo->limSuperior;
			
		// muestra 3 veces el tamaño del intervalo
		// para que se aprecien las estructuras coetaneas
		int ancho = 2*(limSuperior - limInferior)/3;
		MarcaZoom(limInferior - ancho, limSuperior + ancho);
		}
		
	EstadoTrabajo.AnalisisActividad=false;
	EstadoTrabajo.AnalisisLD=false;

	tTotal=clock();
	printf("Tiempo del AG=%f s y total=%f s.\n", 2.0*(tGA-t0)/CLOCKS_PER_SEC, 2.0*(tTotal-t0)/CLOCKS_PER_SEC);

	//Colocamos el foco del teclado en la lista de intervalos
	XmProcessTraversal(interv_lista_analisis, XmTRAVERSE_CURRENT);
	XmProcessTraversal(interv_lista_analisis, XmTRAVERSE_CURRENT);
	
	if(impRest)	imprimeRestricciones();
	printf("Terminado add_interv_analisis\n");		
	}


	
int analisisExperto(int numIntervalo, float *centroides, float *restricciones)
	{
	analisisExperto(numIntervalo, centroides, restricciones, NULL, 0);
	}
	
//******************* ANÁLISIS EXPERTO *********************
/*	Va a identificar distintas emisiones con los picos que tenemos, en función
	de distintos criterios:
	1) Naturales presentes en el rango de energías final.
		A elegir: 	- cuán largo es el rango de energías
					- si tomamos directamente las de naturlib.db				
					- si tomamos las de una búsqueda en lara.DB <- de momento vamos a implementar esta
	2) Artificiales con alta probabilidad
	NOTA: Si restricciones es NULL, si tiene que realizar ajuste no usará restricciones
	noId identifica la posición de uno de los centroides que no debe ser analizada
	(la ha metido el usuario a mano)
	*/
//int analisisExperto(int numIntervalo, float *centroides, float *restricciones)
int analisisExperto(int numIntervalo, float *centroides, float *restricciones, int *noId, int tamNoId)
	{
	printf("------------------ ANÁLISIS EXPERTO -------------------\n");
	NodoPicos *intervalo = DevuelveNodo(Datos.listaPicos, numIntervalo);
	cSearchLaraBD founds;	 //Elementos encontrados naturales
//	printf("Imprimimos los centroides, numIntervalo %d\n", numIntervalo);
//	for(int i=0;i<intervalo->picos.numElementos;i++)	printf("Centroide %d es %f\n", i, centroides[i]);
	for(int i=0;i<tamNoId;i++)
		printf("noId[%d] vale %d\n", i, noId[i]);
	int cont=0;	
	for(int i=1;i<=intervalo->picos.numElementos;i++)
		{
		NodoPico *pico = DevuelveNodo(intervalo->picos, i);
		double cent=Datos.DaEnergia(centroides[i-1]);	
		//
		if(cont>=tamNoId || (i-1)!=noId[cont])
		{
		//
		//printf("Pico %d\n", i);
		//************ análisis a partir de naturlib.db
		string cad(BDIdentificaciones);

		pico->Ajuste.emisiones=new LinkedList();
		determinarNaturales(cent,pico->Ajuste.emisiones,(char *)cad.c_str());
		printf("Tras determinarNaturales, el pico tiene %d emisiones asociadas\n", pico->Ajuste.emisiones->numElements());
		if(pico->Ajuste.emisiones->numElements()>0)	
			{
			//printf("Este pico tiene %d emisiones\n", pico->Ajuste.emisiones->numElements());
			pico->Ajuste.identificado=true;
			pico->Ajuste.analizado=true;
			}
		else	
			{
			pico->Ajuste.identificado=false;
			pico->Ajuste.analizado=true;
			}

		printf("LISTA DE EMISIONES PARA PICO %d, que tiene %d\n", i, pico->Ajuste.emisiones->numElements());
		imprimeNodo(pico);
		
		//NUEVO: Si vemos que hemos analizado 2 picos como lo mismo quitamos uno
		//		y volvemos a ajustar
		//		NOTA: Para identificacion con lara.db esta parte no está implementada
		if(pico->Ajuste.identificado && i>1)
			{
			bool igualIdentificados=true;
			NodoPico *picoAnt= DevuelveNodo(intervalo->picos, i-1);
			printf("El pico anterior tiene de emisiones: \n");
			imprimeNodo(picoAnt);
			printf("Los centroides de ambos son %f y %f\n", Datos.DaEnergia(picoAnt->Ajuste.Centroide), Datos.DaEnergia(pico->Ajuste.Centroide));
			printf("Los picos tienen %d y %d emisiones\n", pico->Ajuste.emisiones->numElements(), picoAnt->Ajuste.emisiones->numElements());
			if(pico->Ajuste.emisiones->numElements()==picoAnt->Ajuste.emisiones->numElements())
				{//Sospechamos
			    for(int j=0;j<pico->Ajuste.emisiones->numElements();j++)
					{
					NodoEmision *neAnt=(NodoEmision *)picoAnt->Ajuste.emisiones->getNode(j);
					NodoEmision *ne=(NodoEmision *)pico->Ajuste.emisiones->getNode(j);
					printf("Energía %f y %f\n", neAnt, ne);
					if(ne->Energia!=neAnt->Energia)	igualIdentificados=false;
					}
			    }
			else	igualIdentificados=false;
			
			if(igualIdentificados)
				{
				printf("OJO: MISMA IDENTIFICACIÓN, FUSIONAR O MIRAR SI HAY ALGUNO Q FALTE!\n");
				}				
			if(igualIdentificados)//Tenemos que reajustar, quitando el segundo igual (i)
				{
				printf(".-.-.-.-.Son iguales los picos %d y %d, quitamos %d\n", i, i-1, i);
				int numGaussianas=intervalo->picos.numElementos-1;
				float centroidesNuevos[numGaussianas];
				float restriccionesNuevas[numGaussianas];
				for(int j=0;j<numGaussianas;j++)	centroidesNuevos[j]=restriccionesNuevas[j]=0.0;
					
				int cont=0;
				for(int j=0;j<numGaussianas+1;j++)
					{
					if(j!=(i-1))	
						{
						centroidesNuevos[cont]=centroides[j];
						if(restricciones!=NULL)	restriccionesNuevas[cont]=restricciones[j];
						cont++;
						}
					}
				
//				for(int j=0;j<numGaussianas;j++)	printf("restAE[%d]=%.2f\n", j, restriccionesNuevas[j]);
				if(restricciones!=NULL)	Datos.AjusteIntervaloGAML(numIntervalo, centroidesNuevos, restriccionesNuevas, numGaussianas,-1,-1,-1);
				else					Datos.AjusteIntervaloGAML(numIntervalo, centroidesNuevos, numGaussianas,-1,-1,-1);

				float centroidesAjustados[intervalo->picos.numElementos];
				for (int i = 1; i <= intervalo->picos.numElementos; i++) 
					{
					NodoPico *pico = DevuelveNodo(intervalo->picos, i);
					centroidesAjustados[i-1]=pico->Ajuste.Centroide;
					}
					
				if(restricciones!=NULL)	
					{
					float restriccionesAjustadas[intervalo->picos.numElementos];
					determinarRestricciones(centroidesAjustados, intervalo->picos.numElementos, restriccionesAjustadas, (char *)cad.c_str());
					
//					printf("Llamada recursiva con centroides:\n");
//					for(int i=0;i<intervalo->picos.numElementos;i++)	printf("Centroide %d es %f\n", i, centroides[i]);
					
					analisisExperto(numIntervalo, centroidesAjustados, restriccionesAjustadas);				
					printf("Volvemos del AE recursivamen (if)\n");
					return 0;//para que no siga haciendo cosas que ya no se necesitan
					}
				else	
					{
					analisisExperto(numIntervalo, centroidesAjustados, NULL);				
					printf("Volvemos del AE recursivamen (else)\n");
					return 0;//para que no siga haciendo cosas que ya no se necesitan
					}
				//return 0;//lo he metido en if y else
				}
			}
		//
		}//del if (no idenfificar)
		else
			{
			printf("El centroide %d (%f) lo dejamos sin identificar porque lo ha añadido el usuario\n", i, cent);
			pico->Ajuste.identificado=false;
			pico->Ajuste.analizado=true;
			cont++;
			}
			
	printf("Fin de análisis experto\n");
		//
		/*	
		/**** ANÁLISIS A PARTIR DE LARA.DB	
		double wide=0.5;
		//double wide = Datos.DaFWHG(pico->Ajuste.Centroide)*sqrt(intervalo->ChiSq);
		//if (wide < 1.5) wide = 1.5;
				
		if (LaraBD.GetIsRunning()) 
			{
			// make the search
			printf("Buscamos las emisiones en el rango %f,%f\n", cent-wide, cent+wide);
			//Buscamos en la base de datos elementos con emisiones en el rango del centroide
			//NOTA: Podría ser mejor tenerlas en el rango de la resolución
			//Buscamos en primer lugar los emisores estables naturales en sus listas de descendientes
			//Emisiones del radio 226, torio 230 232 y uranio 234 235 238
			founds = LaraBD.SearchEmissions(cent-wide, cent+wide,-1,-1,-1,88,88,226,226,false,false,true,true,"", true, true);
			founds = founds + LaraBD.SearchEmissions(cent-wide, cent+wide,-1,-1,-1,90,90,232,232,false,false,true,true,"", true, true);
			founds = founds + LaraBD.SearchEmissions(cent-wide, cent+wide,-1,-1,-1,92,92,235,235,false,false,true,true,"", true, true);
			founds = founds + LaraBD.SearchEmissions(cent-wide, cent+wide,-1,-1,-1,92,92,238,238,false,false,true,true,"", true, true);
			founds = founds + LaraBD.SearchEmissions(cent-wide, cent+wide,-1,-1,-1,92,92,234,234,false,false,true,true,"");
			founds = founds + LaraBD.SearchEmissions(cent-wide, cent+wide,-1,-1,-1,90,90,230,230,false,false,true,true,"");
			
			int numFounds=founds.GetNumItemsFound();
			double energias[numFounds];
			int emisiones[numFounds];
			printf("Encontramos %d emisiones\n", numFounds);	
			if(numFounds>0)
				{
				emisiones[0] = founds.GetFirstResult();
				energias[0]= LaraBD.GetEnergy(emisiones[0]);

				pico->Ajuste.identificado=true;	
				printf("añadimos emision 0\n");
				//if(pico->Ajuste.emisiones==NULL)	pico->Ajuste.emisiones=new LinkedList();
				pico->Ajuste.emisiones=new LinkedList();
				printf("emisión creada\n");
				//if(pico->Ajuste.emisiones==NULL)	printf("EMISIONES ES NULL!!!\n");
				//else	printf("EMISIÓN debería rular perfectamente\n");
				//pico->Ajuste.emisiones->deleteAll();
				printf("Creada lista si no lo estaba\n");
				NodoEmision *ne=new NodoEmision();
				printf("emisión creada\n");
				ne->codElemento	= emisiones[0];
				printf("código creado\n");
				ne->Energia = energias[0];
				printf("energía creada\n");
				pico->Ajuste.emisiones->addLast(ne);
				printf("emisión añadida\n");
				}
		
			for(int i=1;i<numFounds; i++)
				{
				printf("preparando emisión %d\n", i);
				emisiones[i]=founds.GetNextResult();
				energias[i]=LaraBD.GetEnergy(emisiones[i]);

				printf("añadimos emisión %d\n", i);
				NodoEmision *ne=new NodoEmision();
				ne->codElemento	= emisiones[i];
				ne->Energia = energias[i];
				pico->Ajuste.emisiones->addLast(ne);
				}
			
			}//if lara running
			*/
		}
//	printf("fin de ANALISIS EXPERTO\n");
	}
	
//****************** DETERMINA POSICION ******************
//Dado un número de intervalo, determina su posición en la lista
//según las que estén desplegadas
int determinaPosicion(int numIntervalo)
	{
//	printf("DETERMINA POSICIÓN de %d\n", numIntervalo);
	int pos=0;
	bool *desplegadas=new bool[listaAnalisis.numElementos];
		
	if(desplegadas==NULL)	
		{
		printf("ERROR: No hay memoria para reservar bool[%d]\n", listaAnalisis.numElementos);
		return -1;
		}		
	
	determinaDesplegadas(result_lista_analisis, desplegadas, listaAnalisis.numElementos);
	for(int i=0;i<listaAnalisis.numElementos;i++)
		{
		pos++;	
		NodoAnalisis *nodo   = DevuelveNodo(listaAnalisis, i+1);
		NodoPicos *intervalo = DevuelveNodo(Datos.listaPicos, nodo->numIntervalo);
		//printf("nodo->ni=%d y ni=%d\n", nodo->numIntervalo, numIntervalo);
		if(nodo->numIntervalo==numIntervalo)	
			{
			delete [] desplegadas;
			//printf("Devolvemos pos=%d\n", pos);
			return pos;
			}
		if(desplegadas[i])			pos+=intervalo->picos.numElementos;
		//printf("pos es %d\n", pos);
		}
	delete [] desplegadas;
	//printf("Devolvemos pos=%d, hemos llegado al último elemento\n", pos);
	return pos;
	}

//****************** DETERMINA INTERVALO ***************************
/* El despliegue de listas hace que la posición de los intervalos
	en result lista análisis no coincida con su posición en lista análisis.
	Esta función a partir de la posición seleccionada en result lista nos
	dice qué intervalo estamos seleccionando (según la numeración de result_list,
	no de listaAnalisis
	
	*/
int determinaIntervalo(int posicion)
	{
	int numIntervalo=0;
	Arg args[6];
	Cardinal n;
	XmStringTable items;
	char *text=NULL;
	int numItems;
	
//	printf("DETERMINA INTERVALO \n");
	n=0;
	XtSetArg(args[n], XmNitems, &items); n++;
	XtSetArg(args[n], XmNitemCount, &numItems); n++;
	XtGetValues(result_lista_analisis, args, n);

	for(int i=0;i<posicion;i++)
		{
		if(text!=NULL)	XtFree(text);
		XmStringGetLtoR(items[i], XmFONTLIST_DEFAULT_TAG, &text);
		if(text[0]=='-')
			{
			NodoAnalisis *nodo   = DevuelveNodo(listaAnalisis, numIntervalo+1);
			NodoPicos *intervalo = DevuelveNodo(Datos.listaPicos, nodo->numIntervalo);
			i+=intervalo->picos.numElementos;
			}
		numIntervalo++;
		}		
	if(text!=NULL)	XtFree(text);
	//for(int i=0;i<numItems;i++)	XmStringFree(items[i]);
	//XtFree((char*)items);
	return numIntervalo;
	}

//****************** RECONSTRUYE LISTA INTERV ANALISIS **************
/*	Pone todos los intervalos de picos analizados, con su número de picos
	en la lista superior izquierda
	*/
void ReconstruyeListaIntervAnalisis() 
	{
	char texto_interv[20];
	XmString cadena;
	// borra los intervalos que hubiese indicados
	XmListDeleteAllItems(interv_lista_analisis);
	// pasa por todos los intervalos e introduce los que solo contienen un pico
	printf("Tenemos %d elementos\n", Datos.listaPicos.numElementos);
	for (int i = 1; i <= Datos.listaPicos.numElementos; i++) 
		{
		NodoPicos *intervPicos = DevuelveNodo(Datos.listaPicos, i);
		int minEnergia = (int) floor(Datos.DaEnergia((double)intervPicos->limInferior));
		int maxEnergia = (int) floor(Datos.DaEnergia((double)intervPicos->limSuperior))+1;
		sprintf(texto_interv, "%4d - %4d {%d}", minEnergia, maxEnergia, intervPicos->picos.numElementos);
		cadena = XmStringCreateLocalized (texto_interv);
		XmListAddItemUnselected(interv_lista_analisis, cadena, 0);
		XmStringFree (cadena);			
		}
}

//*************** BORRA LISTA INTERV ANALISIS ****************
void BorraListaIntervAnalisis() 
	{
	// borra los intervalos que hubiese indicados
	XmListDeleteAllItems(interv_lista_analisis);	
	}

//***************** PULSA LISTA INTERV ANALISIS *****************
/* 
	*/
void pulsa_lista_interv_analisis(Widget widget, XtPointer client_data, XtPointer call_data) {
   XmListCallbackStruct *cbs = (XmListCallbackStruct *) call_data;
	
	if (EstadoVisual.AutoZoomAnalisis) 
		{
		int limInferior, limSuperior;
		if (cbs->reason == XmCR_EXTENDED_SELECT) 
			{
			// se pueden haber seleccionado 1 o mas elementos
			// se escoge el limite inferior del primer elemento
			NodoPicos *intervalo = DevuelveNodo(Datos.listaPicos, cbs->selected_item_positions[0]);
			limInferior = intervalo->limInferior;
			// se escoge el limite superior del ultimo elemento
			intervalo = DevuelveNodo(Datos.listaPicos, cbs->selected_item_positions[cbs->selected_item_count-1]);
			limSuperior = intervalo->limSuperior;
			}
		else 
			{
			NodoPicos *intervalo = DevuelveNodo(Datos.listaPicos, cbs->selected_item_positions[0]);
			limInferior = intervalo->limInferior;
			limSuperior = intervalo->limSuperior;
			}
			
		// muestra 3 veces el tamaño del intervalo
		// para que se aprecien las estructuras coetaneas
		int ancho = 2*(limSuperior - limInferior)/3;
		MarcaZoom(limInferior - ancho, limSuperior + ancho);
	}
}


/******************************************************************************
 ******
 ******                             RESULTADOS
 ******
 ******************************************************************************/

//********************** PULSA LISTA RESULT ANALISIS ******************
/* Se activa cuando se selecciona un elemento de la lista central, para
	remarcarlo en el gráfico si la opción de zoom está activada
*/
void pulsa_lista_result_analisis(Widget widget, XtPointer client_data, XtPointer call_data) 
	{
	int interv=0, pic=0;
	int *posiciones, numSel, nPos = 0;

	if (!XmListGetSelectedPos(result_lista_analisis, &posiciones, &numSel)) 
		{
		//NOTA: Lo quito porque resulta un poco coñazo y no da información
			
		//Aviso(widget, "You must select an interval to edit.");
		return;
		}

	nPos = *posiciones;
	XtFree((char *)posiciones);
	// Extraño error, mas de un pico seleccionado
	// NOTA: No veo manera de que esto ocurra pues no deja seleccionar más que uno
	if (numSel > 1) 
		{
		Aviso(widget, "Too much intervals selected.");
		printf(ERROR_ANALYSIS);
		printf(ERROR_TOO_INTERVALS);
		return;
		}
	
	determinaPico(result_lista_analisis, nPos, &interv, &pic);

	// Rellena los campos con los datos actuales
//	NodoAnalisis *nodo = DevuelveNodo(listaAnalisis, nPos);
	NodoPicos *intervalo = DevuelveNodo(Datos.listaPicos, interv);
		
	// si la opcion de autoZoom esta activada amplia la zona
	if (EstadoVisual.AutoZoomAnalisis) 
		{
		// muestra 3 veces el tamaño del intervalo
		// para que se aprecien las estructuras coetaneas
		int ancho = 2*(intervalo->limSuperior - intervalo->limInferior)/3;
		MarcaZoom(intervalo->limInferior - ancho, intervalo->limSuperior + ancho);
		}
	
	if(intervalo->picos.numElementos>1) 
	//En este caso debemos mostrar en amarillo el pico dentro del intervalo
		{
		if (EstadoVisual.AjusteParcialAnalisis)
			{
			CopiaAjustesParcialesAGrafica(interv, pic);
			ActivaAjustesParcialesGrafica(EstadoVisual.AjusteParcialAnalisis);
			PintaGrafica(0);
			}
		}
			
	if(intervalo->picos.numElementos>1 && pic==0)	XtSetSensitive(bt_edit_result_analisis, false);
	else											XtSetSensitive(bt_edit_result_analisis, true);
	}


//*************** DETERMINA PICO *****************
/*	Recorre todos los elementos anteriores al pico en el que estamos
	para ver si hay listas abiertas, dándonos el lugar exacto del 
	pico en la lista
	*/
void determinaPico(Widget lista, int posicion, int *Nintervalo, int *Npico)
	{
	Arg args[6];
	Cardinal n;
	XmStringTable items;
	char *text=NULL;
	int numItems;
	int numPicos=0, maxPicos;
	int picosAnterior=0;
	
	bool sublista=false;
		
	printf("DETERMINA PICO en posición %d\n", posicion);
	n=0;
	XtSetArg(args[n], XmNitems, &items); n++;
	XtSetArg(args[n], XmNitemCount, &numItems); n++;
	XtGetValues(lista, args, n);
	printf("posición es %d\n", posicion);
	for(int i=0;i<posicion;i++)
		{
		XmStringGetLtoR(items[i], XmFONTLIST_DEFAULT_TAG, &text);
		printf("Texto %d es ", i);
		printf("%s\n", text);			
		if(text[0]=='-')	
			{
			string cad(text);
			cad=cad.substr(cad.find("with ")+5, cad.find(" peaks")-6);
			maxPicos=atoi((char *)cad.c_str());
				
			NodoAnalisis *nodo = DevuelveNodo(listaAnalisis, i-picosAnterior+1);//OJO!->antes sólo i
			int interval=nodo->numIntervalo;
			if(i!=0)
				{
				nodo = DevuelveNodo(listaAnalisis, i-picosAnterior);
				interval=interval-nodo->numIntervalo;
				}
			(*Nintervalo)+=interval;
			(*Npico)=0;
			sublista=true;
			numPicos=0;
			}
		else				
			{
			if(!sublista)
				{
				NodoAnalisis *nodo = DevuelveNodo(listaAnalisis, i-picosAnterior+1);
				int interval=nodo->numIntervalo;
				if(i!=0)
					{
					nodo = DevuelveNodo(listaAnalisis, i-picosAnterior);//No con respecto al anterior, si no con respecto al anterior intervalo!! (no pico!)
					interval=interval-nodo->numIntervalo;
					}
				(*Nintervalo)+=interval;
				if(text[0]=='+')	(*Npico)=0;//Ninguno seleccionado
				else				(*Npico)=1; //Si no hay sublista sólo tiene un pico
				numPicos=0;
				}
			else
				{
				(*Npico)++;
				numPicos++;
				if(numPicos>=maxPicos)	
					{
					sublista=false;
					picosAnterior+=maxPicos; //Para poder hacer bien los saltos
					maxPicos=0;
					}
				}
			}			//felipe. comento esto para que no casque al borrar un pico de una lista de varios
		//if(text!=NULL)	XtFree(text);
		}
	}
	
//**************** DELETE ALL PEAKS ******************************
/*	Borra todos los picos del análisis
	*/
void deleteAllPeaks()
	{
	if (listaAnalisis.numElementos == 0) 
		{
		Aviso(main_w, "There is no analyzed peaks yet");
		return;
		}
	else
		{
		int respuesta=Continuar(app_shell, "All analyzed peaks will be deleted. Are you sure?", "Caution"); 
		switch(respuesta)
			{
			case SI:
				//Borrar la información de todos los picos
				while(listaAnalisis.numElementos > 0)	deletePeak(1);
				break;
			case NO: //En ambos casos lo único que hacemos es volver sin más
			case CANCELAR:
				return; 
			}
		}
	}

//******************* DELETE PEAK **********************************
/*	Borra el pico identificado por nPos
	*/
void deletePeak(int nPos)
	{
	bool eraSubpico=false;
	printf("-------------Para %d la posición del nodo a borrar es %d\n", nPos, determinaIntervalo(nPos));
	int nPosReal=determinaIntervalo(nPos);
	NodoAnalisis *nodo = DevuelveNodo(listaAnalisis, nPosReal);
		
	string cad(BDIdentificaciones);

/*	if(!equilibrioDeterminado)	
		{
		//Aviso(widget, "Computing constraints, please wait ...");
		determinarEquilibrios((char *)cad.c_str());
		imprimeRestricciones();
		}*/

	// Indica cambio en el trabajo
	EstadoTrabajo.TrabajoCambios = true;
	PonTituloFichCambiado();

	// Actualiza los datos de los picos en la grafica
	int nError = CopiaAjustesAGrafica();
	if (nError < 0) 
		{
		Aviso(fondo_analisis, "No Memory Avalaible");
		printf(ERROR_ANALYSIS);
		printf(ERROR_NO_MEMORY);
		return;
		}

	ActivaAjustesGrafica(EstadoVisual.AjusteTotalAnalisis);

	if (EstadoVisual.AjusteTotalAnalisis)	PintaGrafica(0);
	

	//Tomamos la configuración actual de la lista
	int topPosition, visiblePositions, numPositions;
	XtVaGetValues(result_lista_analisis,
			XmNtopItemPosition, &topPosition,
			XmNvisibleItemCount, &visiblePositions,
			XmNitemCount, &numPositions,
			NULL);

	// quita el intervalo de la lista de picos de analisis
//	printf("Llamamos a BorraPosiciónLista\n");
	if(!esSubpico(nPos))
		{
		//Borramos los datos del intervalo 
		Datos.BorraIntervalo(nodo->numIntervalo);
		borraPosicionLista(result_lista_analisis, nPos, nodo->numIntervalo, true);
		printf("Vuelve de borraposicionlista 1\n");
		quitaNodo(listaAnalisis, nPosReal);
		ReconstruyeListaAnalisis();
		}
	else	//Si es sólo un subpico, no quitamos todo el intervalo, sólo el subpico
		{	//y reajustamos
		printf("Es un subpico, lo quitamos y reajustamos\n");
		eraSubpico=true;
		borraPosicionLista(result_lista_analisis, nPos, nodo->numIntervalo, false);
		printf("Vuelve de borraposicionlista 2\n");
		quitaNodo(listaAnalisis, nPos);
		printf("Vuelve de quitanodo\n");		//Ahora debemos rehacer el ajuste, teniendo en cuenta sólo los picos que 
		//nos hayan quedado
		NodoPicos *intervalo=DevuelveNodo(Datos.listaPicos, nodo->numIntervalo);
		printf("Vuelve de devuelve nodo\n");		int numGaussianas=intervalo->picos.numElementos-1;
		float centroides[numGaussianas];
		int interv, pic, cont=0;
		printf("Va a determinaPico nodo\n");		determinaPico(result_lista_analisis, nPos, &interv, &pic);
		printf("Vuelve de determina pico\n");		for(int i=1;i<=intervalo->picos.numElementos;i++)
			{
			if(i!=pic)
				{
				NodoPico *pico=DevuelveNodo(intervalo->picos, i);
				centroides[cont++]=pico->Ajuste.Centroide;
				printf("cent[%d]==%f\n", cont-1, pico->Ajuste.Centroide);
				}
			}
		printf("centroides %d vale %f\n", pic, centroides[pic-1]);
		if(pic==intervalo->picos.numElementos || centroides[pic-1]!=0)
			{
			//------------NUEVO: Determina las restricciones que les tenemos que poner a las áreas
			float restricciones[numGaussianas];
			determinarRestricciones(centroides, numGaussianas, restricciones, (char *)cad.c_str());	
			int nError=Datos.AjusteIntervaloGAML(nodo->numIntervalo, centroides, restricciones, numGaussianas,-1,-1,-1);
			if(nError==-3 || nError==-5)
				{
				printf("ERROR: %d\n", nError);
				Aviso(app_shell, "ERROR: Fitness Unaccurate, discarded");
				PonCursorNormal();
				return;
				}
		
			float centroidesAjustados[intervalo->picos.numElementos];
			for (int i = 1; i <= intervalo->picos.numElementos; i++) 
				{
				NodoPico *pico = DevuelveNodo(intervalo->picos, i);
				centroidesAjustados[i-1]=pico->Ajuste.Centroide;
				}
					
			float restriccionesAjustadas[intervalo->picos.numElementos];
			determinarRestricciones(centroidesAjustados, intervalo->picos.numElementos, restriccionesAjustadas, (char *)cad.c_str());
			analisisExperto(nodo->numIntervalo, centroidesAjustados, restriccionesAjustadas);
			printf("Volvemos de AE en deletePeak\n");
			}
			
		ReconstruyeListaAnalisis(); 
	
		//Pintamos			
		for(int i=1;i<=intervalo->picos.numElementos; i++)	
			CopiaAjustesParcialesAGrafica(nodo->numIntervalo, i);
		if(intervalo->picos.numElementos>1)
			ActivaAjustesParcialesGrafica(EstadoVisual.AjusteParcialAnalisis);
		CopiaAjustesAGrafica();
		ActivaAjustesGrafica(EstadoVisual.AjusteTotalAnalisis);
		}
	
	// intenta poner el mismo elemento como seleccionado
	// si no, pone el siguiente, y si no hay ninguno no hace nada
	// recoge cual era el primer item que se veia
	if (listaAnalisis.numElementos > 0) 
		{
		if ((nPos == topPosition)&&(nPos > 1))	
			{
			XmListSetPos(result_lista_analisis, nPos-1);
			}	
		else if ((topPosition + visiblePositions) > numPositions)
					XmListSetBottomPos(result_lista_analisis, numPositions-1);
			 else	XmListSetPos(result_lista_analisis, topPosition);
		
		printf("Comprobando si era o no subpico\n");
		if(eraSubpico)	//Pondremos la posición del intervalo en sí, no la de ninguno de los subpicos
			{
			printf("Era subpico\n");
			nodo = DevuelveNodo(listaAnalisis, nPosReal);
			NodoPicos *inte=DevuelveNodo(Datos.listaPicos, nodo->numIntervalo);
			if(inte->picos.numElementos==1)	XmListSelectPos(result_lista_analisis, determinaPosicion(nodo->numIntervalo), true);
			else							XmListSelectPos(result_lista_analisis, determinaPosicion(nodo->numIntervalo)+1, true);
			}
		else	
			{
			printf("No era subpico\n");
			if(nPos>=numPositions)	
				{
				printf("Seleccionamos %d\n", numPositions-1);
				XmListSelectPos(result_lista_analisis, numPositions-1,true);
				}
			else					
				{
				printf("Seleccionamos %d\n", nPos);
				XmListSelectPos(result_lista_analisis, nPos, true);
				}
			}
		}
		
	//Gestión de la activación o desactivación de la interfaz de actividad
	if(listaAnalisis.numElementos == 0)	
		{
		printf("No quedan elementos en la lista, desactivamos Actividad\n");
		EstadoTrabajo.Analisis=false; 
		DesactivaActividad();
		}
		
/*	else if(todosIdentificados())
		{
		printf("Hemos borrado un elemento sin identificar y todos los que quedan están identificados\n");
		EstadoTrabajo.Analisis=true;
		ActivaActividad();
		}*/
	//En cualquiera de ambos casos, se considera que el análisis de la actividad
	//se resetea (ha cambiado el conjunto de picos identificados)
	reseteaActividad();
	return;
	}
	
//*************** PULSA DEL RESULT ANALISIS *********************
void pulsa_del_result_analisis(Widget widget, XtPointer client_data, XtPointer call_data) 
	{
	printf("PULSA DEL RESULT ANALISIS\n");
	// busca el elemento seleccionado
	int *posiciones, numSel, nPos = 0;
//	bool eraSubpico=false;
		
	if (!XmListGetSelectedPos(result_lista_analisis, &posiciones, &numSel)) 
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
		printf(ERROR_ANALYSIS);
		printf(ERROR_TOO_INTERVALS);
		return;
		}
	
	printf("BORRAMOS PICO EN POSICión %d\n", nPos);
	deletePeak(nPos);
	printf("Pico borrado\n");

	if ((EstadoVisual.AjusteTotalAnalisis)||(EstadoVisual.AjusteParcialAnalisis))	PintaGrafica(0);
	//Dejamos el foco del teclado en la lista de picos:
	XmProcessTraversal(result_lista_analisis, XmTRAVERSE_CURRENT);
	XmProcessTraversal(result_lista_analisis, XmTRAVERSE_CURRENT);
	}

//******************** BORRA POSICION LISTA *************
/*	Borra de la lista w la posición posicion, correspondiente
	al intervalo posicionIntervalo.
	Si borra intervalo es true, borra todo el intervalo aunque
	lo que se haya seleccionado sea solo un subelemento. 
*/
void borraPosicionLista(Widget w, int posicion, int posicionIntervalo, bool borraIntervalo)
	{
	Arg args[6];
	Cardinal n;
	XmStringTable items;
	char *text=NULL;
	int numItems;
		
	printf("BORRA POSICION LISTA\n");
		
	n=0;
	XtSetArg(args[n], XmNitems, &items); n++;
	XtSetArg(args[n], XmNitemCount, &numItems); n++;
	XtGetValues(w, args, n);
	//posicion=posicion-1;
	NodoPicos *intervalo=DevuelveNodo(Datos.listaPicos, posicionIntervalo);

	//bool desplegadas[listaAnalisis.numElementos];
	bool *desplegadas;
	desplegadas=new bool[listaAnalisis.numElementos];
	if(desplegadas==NULL)	
		{
		printf("ERROR: No hay memoria para reservar bool[%d]\n", listaAnalisis.numElementos);
		return;
		}		
	printf("ListaAnalisis.numElementos es %d\n", listaAnalisis.numElementos);
	for(int i=0;i<listaAnalisis.numElementos;i++)	desplegadas[i]=false;	
	determinaDesplegadas(result_lista_analisis, desplegadas, listaAnalisis.numElementos);	
	printf("Continuamos después de determinar desplegadas\n");

	//for(int i=0;i<listaAnalisis.numElementos;i++)	printf("%d(%d) ", desplegadas[i], i);	
	//Si la lista está abierta, borramos todos sus elementos
	printf("Vamos a borrar la posición %d\n", posicion);
	
//	if(desplegadas[posicion-1])//En desplegadas contamos desde -1
	if(desplegadas[posicion-1] && borraIntervalo)//En desplegadas contamos desde -1
		{
		XmStringGetLtoR(items[posicion], XmFONTLIST_DEFAULT_TAG, &text);
		printf("La lista está abierta, borramos la línea %s\n", text);
		XmListDeletePos(w, posicion);
		for(int i=0;i<=(intervalo->picos.numElementos);i++)	
			XmListDeletePos(w, (posicion) );
		if(text!=NULL)	XtFree(text);
		}
	else	
		{
		printf("Borramos el elemento en posicion %d\n", posicion);
		XmListDeletePos(w, (posicion) );
		printf("Elemento borrado\n");		}
	return;
	}
	
/******************************************************************************
 ******
 ******                             AJUSTES
 ******
 ******************************************************************************/

//******************** ACTUALIZA VECTOR PICOS ANAL *************************
double ActualizaVectorPicosAnal(double pos, double* param, double* dParam) 
	{
	/* 0*n -> Centroide
		1*n -> anchura
		2*n -> tau
		3*n -> Ytau
		dParam -> derivada parcial */
//	printf("ACTUALIZA VECTOR PICOS ANAL\n");
	double acum = 0.0;
	for (int i = 0; i < Datos.intervaloAjuste->picos.numElementos; i++) 
		{
		int d = i*4;
//		printf("%f\t%f\t%f\t%f\n", param[d+1], param[d+2], param[d+3], param[d+4]);
		// Calcula el valor de Yn
		/* param[d+0]=centroide
		   param[d+1]=sigma
		   param[d+2]=tau
		   param[d+3]=amplitud
		*/
//		printf("Pico %d: c=%f\ts=%f\tt=%f\ta=%f\n", i,param[d+0], param[d+1], param[d+2], param[d+3]);
		double temp = param[d+3]/2.0;
		temp *= erfc(((pos - param[d+0])/param[d+1] + param[d+1]/param[d+2])*M_SQRT1_2);
		double popo = powi(param[d+1]/param[d+2], 2)/2.0 + (pos - param[d+0])/param[d+2];
			/* Para evitar problemas por exceso de desbordamiento
			   fuerza un "infinito" en 10^100 dejando de margen de los
				calculos internos hasta el 10^308 del formato double */
		if (popo > 230) temp = 1e+100;
		else
			temp *= exp(popo);
		// valor de apoyo para las derivadas
		// derivadas parciales con respecto a cada parámetro
		double temp2 = -powi((pos - param[d+0])/param[d+1], 2)/2.0;
		temp2 = -exp(temp2)*param[d+3]/sqrt(M_2PI);
	
		dParam[d+0] = -temp/param[d+2] - temp2/param[d+1];
		dParam[d+1] = param[d+1]*(temp/powi(param[d+2], 2)) +
					  temp2*(1/param[d+2] - (pos - param[d+0])/param[d+1]);
		dParam[d+2] = - ((pos - param[d+0] + powi(param[d+1], 2)/param[d+2])*temp +
					  temp2*param[d+1])/powi(param[d+2], 2);
		dParam[d+3] = temp/param[d+3];
		//printf("dph=%f/%f\n", temp, param[d+3]);
		//printf("dpc=%f\tdps=%f\tdpt=%f\tdpa=%f\n", dParam[d+0], dParam[d+1], dParam[d+2], dParam[d+3]);
		acum += temp;
		}
	
	return acum;
	}


/******************************************************************************
 ******
 ******                             SHOWS
 ******
 ******************************************************************************/

/*****************************************************************
******************* PULSA SHOW ANALISIS **************************
*****************************************************************/
void pulsa_show_analisis(Widget widget, XtPointer client_data, XtPointer call_data) 
	{
   	XmToggleButtonCallbackStruct *state = (XmToggleButtonCallbackStruct *) call_data;

	switch ((int) client_data) 
		{
		case BOTON_SHOW_AJUSTE:
			EstadoVisual.AjusteTotalAnalisis = state->set;
			EstadoVisual.AjusteCalibracion = state->set;
			EstableceShowsAnalisis(EstadoVisual);
			ActivaAjustesGrafica(state->set);
			PintaGrafica(0);
			break;
		case BOTON_SHOW_AJUSTEPARCIAL:
			//NOTA: No sé si esto está terminado
			EstadoVisual.AjusteParcialAnalisis = state->set;
			EstadoVisual.AjusteCalibracion = state->set;
			EstableceShowsAnalisis(EstadoVisual);
			ActivaAjustesParcialesGrafica(state->set);
			PintaGrafica(0);
			break;
		case BOTON_SHOW_AUTOZOOM:
			EstadoVisual.AutoZoomAnalisis = state->set;
			break;
		}
}
//***********************************************************
//				 CÁLCULOS SOBRE EL ANÁLISIS 
//***********************************************************


//********************* PULSA OPEN FILE ******************************
void pulsaOpenFile(Widget widget, XtPointer client_data, XtPointer call_data) 
	{
	Arg			args[7];
   	Cardinal	i = 0;
	XmString 	cadena;

	printf("Pulsado Open FIle \n");
	i = 0;
	XtSetArg (args[i], XmNtitle, "Open Radionuclid File ..."); i++;
	Widget dlg_files = XmCreateFileSelectionDialog (widget, "dlg_files", args, i);
	XtAddCallback (dlg_files, XmNcancelCallback, pulsaCancelFile, NULL);
	XtAddCallback (dlg_files, XmNokCallback, pulsaOKFile, NULL);
	
	cadena = XmStringCreateLocalized ("*.rn");
	XtVaSetValues (dlg_files, XmNpattern, cadena, NULL);
	XmStringFree (cadena);
	
	XtManageChild(dlg_files);
	}
//********************* PULSA OPEN FILE 2******************************
void pulsaOpenFile_2(Widget widget, XtPointer client_data, XtPointer call_data) 
	{
	Arg			args[7];
   	Cardinal	i = 0;
	XmString 	cadena;

	printf("Pulsado Open FIle \n");
	i = 0;
	XtSetArg (args[i], XmNtitle, "Open Energy File ..."); i++;
	Widget dlg_files = XmCreateFileSelectionDialog (widget, "dlg_files", args, i);
	XtAddCallback (dlg_files, XmNcancelCallback, pulsaCancelFile, NULL);
	XtAddCallback (dlg_files, XmNokCallback, pulsaOKFile_2, NULL);
	
	cadena = XmStringCreateLocalized ("*.en");
	XtVaSetValues (dlg_files, XmNpattern, cadena, NULL);
	XmStringFree (cadena);
	
	XtManageChild(dlg_files);
	}


//******************* DEVUELVE INTERVALO SELECCIONADO ******************
int devuelveIntervaloSeleccionado()
	{
	int *posiciones, numSel, nPos = 0;
	int interv=0, pic=0;

	if (!XmListGetSelectedPos(result_lista_analisis, &posiciones, &numSel)) 
		{
		printf("ERROR: No position selected\n");
		return -1;
		}

	nPos = *posiciones;
	XtFree((char *)posiciones);
	// Extraño error, mas de un pico seleccionado
	// NOTA: No veo manera de que esto ocurra pues no deja seleccionar más que uno
	if (numSel > 1) 
		{
		printf(ERROR_ANALYSIS);
		printf(ERROR_TOO_INTERVALS);
		return -1;
		}
	
	determinaPico(result_lista_analisis, nPos, &interv, &pic);

	// Rellena los campos con los datos actuales
	//printf("El intervalo es el intervalo %d,%d\n", interv, pic);
	return interv;
	}

//**************** DETERMINAR CENTROIDES ****************
/* Busca los centroides adecuados para el intervalo:
	1)Busca todas las emisiones en ese rango.
	2)Las separa en grupos de 1keV de distancia
	3)Para cada grupo hace una media ponderada según su
	probabilidad de emisión, tomándolo como un centroide
*/
int determinarCentroides(cSearchLaraBD founds, float *centroides)
	{
	//NOTA: Revisar la búsqueda que no se metan cosas raras
	//printf("Búsqueda hecha, tenemos %d elementos\n", founds.GetNumItemsFound());
	int emission=founds.GetFirstResult();
	centroides[0]=LaraBD.GetEnergy(emission);			
	for(unsigned int i=1;i<founds.GetNumItemsFound();i++)
		{
		int emission=founds.GetNextResult();
		centroides[i]=LaraBD.GetEnergy(emission);	
		}
	
	return founds.GetNumItemsFound();
	}
	

//******************* DETERMINAR NATURALES *********
/*	Busca en la base de datos de naturales (fichero) las emisiones
	que entran dentro del intervalo en el que se encuentra energía y 
	los añade a emisiones.
	La mayoría de estas emisiones se encuentran en la base de datos Lara
	también, aunque algunas reacciones extrañas, como la aniquilación a 
	511 keV, son propias de naturlib.
	*/
//NUEVO: Busca los elementos entre paréntesis (los hijos)
void determinarNaturales(float energia, LinkedList *emisiones, char *fichero)
	{
	printf("DETERMINAR NATURALES ALREDEDOR DE %.2f con %s\n", energia, fichero);	
	if(energia<=0)	{printf("ERROR: Energía no válida (cero)\n");return;}
		
//--->	preparaFichero(fichero);
	
	int filas=numeroFilas(fichero);
	int j=0;
	char *nom=NULL;
	char *tex=NULL;
	double elAnt=0;//Para volver atrás si el siguiente está más lejos que éste
	bool anteriorEsCero=true;
	for(int i=0;i<filas;i++)
		{
//		printf("fila %d\n", i);
		double el=leerElemento(fichero, i, 0);
		double elOriginal=el;
		if(el != 0 && el>=energia) //Avanzamos en el fichero hasta llegar a la zona de energías interesante		
			{
			if(fabs(elAnt-energia)<fabs(el-energia) && i>0)	
				{
				if(anteriorEsCero)	i-=2;
				else				i--;//Si hay menos distancia al anterior que a este comenzamos por el anterior
				el=elAnt;
				}
			NodoEmision *ne=new NodoEmision();
			
			float prob=leerNumeroEntreParentesis(fichero,i,1);

			printf("PROBABILIDAD DE NATURLIB: %f\n", prob);

			//exit(0);
//			if(tex!=NULL)	delete [] tex;
			tex=leerElementoEntreParentesis(fichero, i, 2);//NOTA: Eso de coger el apuntador no es demasiado buena idea (habría que reservar memoria aquí y eso)
			int p=tomarNumero(tex);
//			if(nom!=NULL)	delete [] nom;
			nom=tomarNombre(tex);
			//NOTA: Si al final esto funciona, habrá que andar con ojo porque puede haber probs mal en naturlib
			float minProb=prob-0.1;
			if(minProb<0)	minProb=0.0;
			//felipe
			//printf("Buscando [%.2f,%.2f] con probabilidad [%.2f, %.2f] y número atómico %d y nombre |%s|\n", el-0.1, el+0.1, minProb, prob+0.1, p, nom);
			printf("Buscando en LARA [%.2f,%.2f] con probabilidad [%.2f, %.2f] y número atómico %d y nombre |%s|\n", el-0.1, el+0.1, minProb, prob+0.1, p, nom);

			cSearchLaraBD founds = LaraBD.SearchEmissions(el-0.1, el+0.1,minProb,prob+0.1,-1,-1,-1,p,p,false,false,false,false,nom, false, false);
			printf("Hemos encontrado %d elementos\n", founds.GetNumItemsFound() ); 
//			if(founds.GetNumItemsFound())
//NOTA: Añadimos una pequeña restriccion para que no nos identifique con picos muy lejanos!
		if(energia>el-0.5 && energia<el+0.5)
			{
			if(founds.GetNumItemsFound())
				{
				printf("vamos a ver si es añadible\n");
				ne->codElemento=founds.GetFirstResult();
				if(founds.GetNumItemsFound()>1)	ne->codElemento=founds.GetNextResult();
				ne->Energia=el;
				//Sólo añadimos si está relativamente cerca (< 1keV)
				if(ne->Energia - energia < 1.0)		{printf("Añadimos, %f-%f\n", ne->Energia, energia);	emisiones->addLast(ne);}
				else								{printf("No añadimos, %f-%f\n", ne->Energia, energia);}
				}
			//Si no está en lara lo guardamos en plan chapucilla
			//NOTA: Para ello añadimos dos campos que no siempre van a rellenarse
			//		(de hecho en muy pocas ocasiones). Podemos aprovechar y rellenar
			//		en cualquier caso esas variables, pero sólo porque ya las estamos desaprovechando
			else
				{
				ne->Energia=el;
				ne->nombre=new char[strlen(nom)+1];
				printf("1Tamaño del nombre es %d (%s)\n", strlen(nom), nom);
				strcpy(ne->nombre, nom);
				printf("1El nombre que se asocia es %s\n", ne->nombre);
				ne->probabilidad=leerNumeroEntreParentesis(fichero,i,1);
				printf("1felipe:La probabilidad que se asocia es %f\n", ne->probabilidad);				printf("1felipe:La energia que se asocia es %f\n", ne->Energia);				ne->codElemento=-1;
				//Sólo añadimos si está relativamente cerca
				if(ne->Energia - energia < 1.0)		emisiones->addLast(ne);
				}
			
			printf("Avanzamos hacia adelante\n");
			for(int k=i+1;k<filas;k++)//Avanzamos hacia delante hasta encontrar --- o ___
				{
				el=leerElemento(fichero, k, 0);
				if(el!=0)	
					{
					NodoEmision *ne=new NodoEmision();

					float prob=leerNumeroEntreParentesis(fichero,k,1);
					tex=leerElementoEntreParentesis(fichero, k, 2);
					int p=tomarNumero(tex);
					nom=tomarNombre(tex);
					cSearchLaraBD founds = LaraBD.SearchEmissions(el-0.01, el+0.01,prob-0.1,prob+0.1,-1,-1,-1,p,p,false,false,false,false,nom, false, false);
				
					if(founds.GetNumItemsFound())
						{
						ne->codElemento=founds.GetFirstResult();
						if(founds.GetNumItemsFound()>1)	ne->codElemento=founds.GetNextResult();//NOTA: Si aparecen dos es que el mismo elemento está en equilibrio consigo mismo, lo cogemos de la lista propia, no de la de equilibrio (es decir, la segunda ocurrencia)
						ne->Energia=el;
						//Sólo añadimos si está relativamente cerca
						if(ne->Energia - energia < 1.0)	emisiones->addLast(ne);
						}
					else
						{
						ne->Energia=el;
						ne->nombre=new char[strlen(nom)+1];
						printf("Tamaño del nombre es %d (%s)\n", strlen(nom), nom);
						strcpy(ne->nombre, nom);
						printf("2El nombre que se asocia es |%s|\n", ne->nombre);
						ne->probabilidad=leerNumeroEntreParentesis(fichero,k,1);
						printf("Hemos leido el número entre paréntesis\n");
						ne->codElemento=-1;
						//Sólo añadimos si está relativamente cerca
						if(ne->Energia - energia < 1.0)
							{
							printf("Añadimos emisión\n");
							emisiones->addLast(ne);
							printf("Volvemos de añadir emisión\n");
							}
						printf("2El nombre que se asocia es |%s|\n", ne->nombre);
						}
					}
				else	k=filas;
				}
			printf("Vamos hacia atrás\n");
			for(int k=i-1;k>0;k--)//Avanzamos hacia atrás hasta encontrar --- o ___
				{
				el=leerElemento(fichero, k, 0);
				if(el!=0)
					{
					NodoEmision *ne=new NodoEmision();

					float prob=leerNumeroEntreParentesis(fichero,k,1);
					tex=leerElementoEntreParentesis(fichero, k, 2);
					int p=tomarNumero(tex);
					nom=tomarNombre(tex);
					cSearchLaraBD founds = LaraBD.SearchEmissions(el-0.01, el+0.01,prob-0.1,prob+0.1,-1,-1,-1,p,p,false,false,false,false,nom, false, false);
			
					if(founds.GetNumItemsFound())
						{//NOTA: Técnicamente SIEMPRE debería encontrar UNO y sólo uno para cada entrada
						ne->codElemento=founds.GetFirstResult();
						if(founds.GetNumItemsFound()>1)	ne->codElemento=founds.GetNextResult();
						ne->Energia=el;
						//Sólo añadimos si está relativamente cerca
						if(ne->Energia - energia < 1.0)	emisiones->addFirst(ne);
						}
					else
						{
						ne->Energia=el;
						ne->nombre=new char[strlen(nom)+1];
						printf("3 Tamaño del nombre es %d (%s)\n", strlen(nom), nom);
						strcpy(ne->nombre, nom);
						printf("3 El nombre que se asocia es %s\n", ne->nombre);
						ne->probabilidad=leerNumeroEntreParentesis(fichero,k,1);
						ne->codElemento=-1;
						//Sólo añadimos si está relativamente cerca
						if(ne->Energia - energia < 1.0)	emisiones->addFirst(ne);
						}
					}
				else	k=0;
				}
			printf("fin de determinarNaturales\n");
		//	if(nom!=NULL)	delete [] nom;
		//	if(tex!=NULL)	delete [] tex;
			return;
			}//If energía coincide exactamente	
		else	i=filas;			
			}//if estamos en su zona de energías
		if(elOriginal!=0)	{elAnt=elOriginal;	anteriorEsCero=false;}
		else				anteriorEsCero=true;
		}//for
	printf("fin de determinarNaturales\n");
//	if(nom!=NULL)	delete [] nom;
//	if(tex!=NULL)	delete [] tex;
		
	//Al final, añadimos emisiones que puedan ser debidas al fondo
//	if(fondoCargado)	determinarEmisionesFondo(energia, emisiones, nombreFichFBD);
	return;
	}

//************** DETERMINAR EMISIONES FONDO ********************
/*	Buscamos en la base de datos de fondo otras emisiones que añadamos
	a las de la base de datos de identificación
		-Si la emisión coincide con alguna de la que ya hay en emisiones,
		no la añade
	*/
void determinarEmisionesFondo(float energia, LinkedList *emisiones, char *fichero)
	{
	printf("--------- DETERMINAR EMISIONES FONDO ----------\n");
	//creado por Felipe. 15 de junio de 2010
	printf("DETERMINAR EMISIONES DE FONDO ALREDEDOR DE %.2f con %s\n", energia, fichero);	
	if(energia<=0)	{printf("ERROR: Energía no válida (cero)\n");return;}
		
//--->	preparaFichero(fichero);
	
	int filas=numeroFilas(fichero);
	int j=0;
	char *nom=NULL;
	char *tex=NULL;
	double elAnt=0;//Para volver atrás si el siguiente está más lejos que éste
	bool anteriorEsCero=true;
	for(int i=0;i<filas;i++)
		{
//		printf("fila %d\n", i);
		double el=leerElemento(fichero, i, 0);			//lee la energía que es la primera columna
		double elOriginal=el;
		if(el != 0 && el>=energia) //Avanzamos en el fichero hasta llegar a la zona de energías interesante		
			{
			if(fabs(elAnt-energia)<fabs(el-energia) && i>0)	
				{
				if(anteriorEsCero)	i-=2;
				else				i--;//Si hay menos distancia al anterior que a este comenzamos por el anterior
				el=elAnt;
				}
			NodoEmision *ne=new NodoEmision();

			tex=leerElementoEntreParentesis(fichero, i, 1);//lee el elemento de la segunda columna
			int p=tomarNumero(tex);
			nom=tomarNombre(tex);	//hay que ver si esto nos sirve porque en la base de datos de fondo está al revés. Primero el número y luego el nombre

			//recuperamos la probabilidad de LARA porque no la tenemos en la BD fondo
			cSearchLaraBD founds = LaraBD.SearchEmissions(el-0.1, el+0.1,-1,-1,-1,-1,-1,-1,-1,false,false,false,false,nom, false, false);
		//NOTA: Añadimos una pequeña restriccion para que no nos identifique con picos muy lejanos!
			if(energia>el-0.5 && energia<el+0.5)			{
			if(founds.GetNumItemsFound())				{
				printf("vamos a ver si es añadible\n");
				ne->codElemento=founds.GetFirstResult();
				if(founds.GetNumItemsFound()>1)	ne->codElemento=founds.GetNextResult();
				ne->Energia=el;

				ne->probabilidad = LaraBD.GetProbability(ne->codElemento);
				//Sólo añadimos si está relativamente cerca (< 1keV)
				if(ne->Energia - energia < 1.0)		{printf("Añadimos, %f-%f\n", ne->Energia, energia);	emisiones->addLast(ne);}
				else								{printf("No añadimos, %f-%f\n", ne->Energia, energia);}
				}				}
			}

	}
	return;	
	}

/*//NOTA: Versión que nos da los padres
void determinarNaturales(float energia, LinkedList *emisiones, char *fichero)
	{
	//printf("DETERMINAR NATURALES ALREDEDOR DE %f con %s\n", energia, fichero);	
	preparaFichero(fichero);
	//printf("fichero preparado, tomamos número de filas\n");
	int filas=numeroFilas(fichero);
	//printf("iniciamos valores\n");
	int j=0;
	double elAnt=0;//Para volver atrás si el siguiente está más lejos que éste
	bool anteriorEsCero=true;
	//printf("empezamos la búsqueda\n");	
	for(int i=0;i<filas;i++)
		{
		double el=leerElemento(fichero, i, 0);
		double elOriginal=el;
		if(el != 0 && el>=energia) //Avanzamos en el fichero hasta llegar a la zona de energías interesante		
			{
			if(abs(elAnt-energia)<abs(el-energia) && i>0)	
				{
				if(anteriorEsCero)	i-=2;
				else				i--;//Si hay menos distancia al anterior que a este comenzamos por el anterior
				el=elAnt;
				}
			NodoEmision *ne=new NodoEmision();
			
			float prob=leerElemento(fichero,i,1);
			char *nom=leerElementoCadena(fichero, i, 2);//NOTA: Eso de coger el apuntador no es demasiado buena idea (habría que reservar memoria aquí y eso
			nom=quitarParentesis(nom);
			int p=tomarNumero(nom);
			nom=tomarNombre(nom);
			//NOTA: Si al final esto funciona, habrá que andar con ojo porque puede haber probs mal en naturlib
			//printf("Buscando emisiones en [%f,%f] keV con probabilidad en [%f,%f]%% con nombre %s y nº másico %d\n", el-0.01, el+0.01, prob-0.1, prob+0.1, nom, p);
			//printf("Buscando emisiones en [%f,%f] keV con probabilidad en [%f,%f]% con nº másico %d\n", el-0.01, el+0.01, prob-0.1, prob+0.1, p);
			cSearchLaraBD founds = LaraBD.SearchEmissions(el-0.1, el+0.1,prob-0.1,prob+0.1,-1,-1,-1,p,p,false,false,false,false,nom, false, false);
			//printf("Encontradas %d emisiones para %s%d con prob %f\n", founds.GetNumItemsFound(), nom,p,prob);
			if(founds.GetNumItemsFound())
				{
				ne->codElemento=founds.GetFirstResult();
				if(founds.GetNumItemsFound()>1)	ne->codElemento=founds.GetNextResult();
				ne->Energia=el;
				//printf("Añadimos emision de %s%d (energía %f)\n", nom, p, ne->Energia);
				emisiones->addLast(ne);
				}
			//Si no está en lara lo guardamos en plan chapucilla
			//NOTA: Para ello añadimos dos campos que no siempre van a rellenarse
			//		(de hecho en muy pocas ocasiones). Podemos aprovechar y rellenar
			//		en cualquier caso esas variables, pero sólo porque ya las estamos desaprovechando
			else
				{
				ne->Energia=el;
				ne->nombre=nom;
				ne->probabilidad=leerElemento(fichero,i,1);
				ne->codElemento=-1;
				//printf("Añadimos emision de %s%d (energía %f)\n", nom, p, ne->Energia);
				emisiones->addLast(ne);
				}
			//
		
			for(int k=i+1;k<filas;k++)//Avanzamos hacia delante hasta encontrar --- o ___
				{
				el=leerElemento(fichero, k, 0);
				if(el!=0)	
					{
					NodoEmision *ne=new NodoEmision();

					float prob=leerElemento(fichero,k,1);
					char *nom=leerElementoCadena(fichero, k, 2);
					nom=quitarParentesis(nom);
					int p=tomarNumero(nom);
					nom=tomarNombre(nom);
					//NOTA: Si al final esto funciona, habrá que andar con ojo porque puede haber probs mal en naturlib
					cSearchLaraBD founds = LaraBD.SearchEmissions(el-0.01, el+0.01,prob-0.1,prob+0.1,-1,-1,-1,p,p,false,false,false,false,nom, false, false);
					//printf("Encontradas %d emisiones para %s %d con prob %f\n", founds.GetNumItemsFound(), nom,p,prob);
				
					if(founds.GetNumItemsFound())
						{
						ne->codElemento=founds.GetFirstResult();
						if(founds.GetNumItemsFound()>1)	ne->codElemento=founds.GetNextResult();//NOTA: Si aparecen dos es que el mismo elemento está en equilibrio consigo mismo, lo cogemos de la lista propia, no de la de equilibrio (es decir, la segunda ocurrencia)
						ne->Energia=el;
				//printf("Añadimos emision de %s%d (energía %f)\n", nom, p, ne->Energia);
						emisiones->addLast(ne);
						}
					else
						{
						ne->Energia=el;
						ne->nombre=new char[strlen(nom)];
						strcpy(ne->nombre, nom);
						ne->probabilidad=leerElemento(fichero,k,1);
						ne->codElemento=-1;
				//printf("Añadimos emision de %s%d (energía %f)\n", nom, p, ne->Energia);
						emisiones->addLast(ne);
						}
					}
				else	k=filas;
				}
			for(int k=i-1;k>0;k--)//Avanzamos hacia atrás hasta encontrar --- o ___
				{
				el=leerElemento(fichero, k, 0);
				if(el!=0)
					{
					NodoEmision *ne=new NodoEmision();

					float prob=leerElemento(fichero,k,1);
					char *nom=leerElementoCadena(fichero, k, 2);
					nom=quitarParentesis(nom);
					int p=tomarNumero(nom);
					nom=tomarNombre(nom);
					cSearchLaraBD founds = LaraBD.SearchEmissions(el-0.01, el+0.01,prob-0.1,prob+0.1,-1,-1,-1,p,p,false,false,false,false,nom, false, false);
					//printf("Encontradas %d emisiones para %s%d con prob %f\n", founds.GetNumItemsFound(), nom,p,prob);
			
					if(founds.GetNumItemsFound())
						{//NOTA: Técnicamente SIEMPRE debería encontrar UNO y sólo uno para cada entrada
						ne->codElemento=founds.GetFirstResult();
						if(founds.GetNumItemsFound()>1)	ne->codElemento=founds.GetNextResult();
						ne->Energia=el;
				//printf("Añadimos emision de %s%d (energía %f)\n", nom, p, ne->Energia);
						emisiones->addFirst(ne);
						}
					//
					else
						{
						ne->Energia=el;
						ne->nombre=nom;
						ne->probabilidad=leerElemento(fichero,k,1);
						ne->codElemento=-1;
				//printf("Añadimos emision de %s%d (energía %f)\n", nom, p, ne->Energia);
						emisiones->addFirst(ne);
						}
					//
					}
				else	k=0;
				}
			//printf("FIN DE DETERMINAR NATURALES\n");
			return;
			}//if energia coincide
		if(elOriginal!=0)	{	elAnt=elOriginal;	anteriorEsCero=false;}
		else				anteriorEsCero=true;
		}//for
	//printf("FIN DE DETERMINAR NATURALES\n");
	return;
	}
	*/
//********************* DETERMINAR CENTROIDES *****************************
/*	Devuelve en centroides todos los centroides que se encuentran en fichero
	entre las energías li y ls keV.
	*/
/*
void determinarCentroides(float li, float ls, float *centroides, char *fichero)
	{
//	printf("DETERMINAR CENTROIDES\n");
//	printf("Vamos a abrir el fichero %s\n", fichero);
	int filas=numeroFilas(fichero);
	int j=0;
	for(int i=0;i<filas;i++)
		{
		double el=leerElemento(fichero, i, 3);
		if(el != 0 && el>=li && el<=ls)		
			{
			//printf("Es centroide, para nososotros\n");
			centroides[j++]=Datos.DaCanal(el);
			}
		else if (el!=0 && el>ls)	return;
		}
	return;
	}
	*/
void determinarCentroides(float li, float ls, float *centroides, char *fichero)
	{
	printf("DETERMINAR CENTROIDES\n");
	printf("Vamos a abrir el fichero %s\n", fichero);
	int filas=numeroFilas(fichero);
	int j=0;
	for(int i=0;i<filas;i++)
		{
		double el=leerElemento(fichero, i, 3);
		if(el != 0 && el>=li && el<=ls)		
			{
			//printf("Es centroide, para nososotros\n");
			centroides[j++]=Datos.DaCanal(el);
			}
		else if (el!=0 && el>ls)	i=filas;
		}

	//Ahora añadimos los picos que pueda haber de fondo y no aparezcan solapados
	//con cosas de los centroides
	bool add=true;
	if(EstadoTrabajo.fondoCargado)
		{
		printf("Hay fondo cargado, miramos si hay que añadir más elementos: %s\n", nombreFichFBD);
		for(int i=0;i<Datos.numFondos;i++)
			{
			double elf=Datos.energiasFondo[i];
			if(elf != 0 && elf>=li && elf<=ls)		
				{
				//j++;//Añadiendo todos.
				//printf("Emisión de fondo a %f\n", elf);
				for(int k=0;k<filas;k++)
					{
					double el=leerElemento(fichero, k, 4);
					if(el != 0 && el>=li && el<=ls)
					  {
					  printf("Mirando si %.2f € [%.2f,%.2f]\n", elf, el-0.2, el+0.2);
					  if( elf<(el+0.2) && elf>(el-0.2)) 		
						{
						//En este caso, el fondo está englobado en un pico natural, 
						//no hay que añadirlo como pico adicional, será otra emisión
						//más
						//0.3 es la resolución que ponemos en naturlib.db
						//printf("Englobado en pico natural, no lo metemos\n");
						k=filas;
						add=false;
						}
					  }
					}
				if(add)	
					{
					printf("Añadimos centroide en el canal de %f\n", elf);
					centroides[j++]=Datos.DaCanal(elf);
					}
				add=true;					
				}
			}
		}
	Datos.ordenaVector(centroides, j);

	return;
	}
	

//********************* DETERMINAR NUM CENTROIDES *****************************
/*int determinarNumCentroides(float li, float ls, char *fichero)
	{
//	printf("DETERMINAR NUM CENTROIDES en [%.2f,%.2f]\n",li,ls);
//--->	preparaFichero(fichero);	
	bool
	int filas=numeroFilas(fichero);
	int j=0;
	for(int i=0;i<filas;i++)
		{
		double el=leerElemento(fichero, i, 4);
		if(el != 0 && el>=li && el<=ls)		
			{
				j++;
			}
		else if (el!=0 && el>ls)	return j;
		}
	return j;
	}*/
	
int determinarNumCentroides(float li, float ls, char *fichero)
	{
	printf("DETERMINAR NUM CENTROIDES.com\n");
	printf("%s\n", fichero);
	int filas=numeroFilas(fichero);
	int j=0;
	bool add=true;
		
	for(int i=0;i<filas;i++)
		{
		double el=leerElemento(fichero, i, 4);
//		printf("%f con respecto a [%f,%f]\n", el, li, ls);
		if(el != 0 && el>=li && el<=ls)		
			{
				j++;
			}
		else if (el!=0 && el>ls)	i=filas;
		}
	printf("Sin contar el fondo tenemos %d\n", j);
	//Ahora añadimos los picos que pueda haber de fondo y no aparezcan solapados
	//con cosas de los centroides
	if(EstadoTrabajo.fondoCargado)
		{
		int filasFBD=Datos.numFondos;
		printf("Tenemos %d picos de fondo, a buscar\n", filasFBD);
		for(int i=0;i<filasFBD;i++)
			{
			double elf=Datos.energiasFondo[i];
			if(elf != 0 && elf>=li && elf<=ls)		
				{
				//j++;//Añadiendo todos.
				//printf("Emisión de fondo a %f\n", elf);
				for(int k=0;k<filas;k++)
					{
					double el=leerElemento(fichero, k, 4);
					if(el != 0 && el>=li && el<=ls)
					  {
					  //printf("Mirando si %.2f € [%.2f,%.2f]\n", elf, el-0.3, el+0.3);
					  if( elf<(el+0.2) && elf>(el-0.2) )		
						{//En este caso, el fondo está englobado en un pico natural, 
						//no hay que añadirlo como pico adicional, será otra emisión
						//más
						//0.3 es la resolución que ponemos en naturlib.db, aquí usamos 0.2
						//printf("Englobado en pico natural, no lo metemos\n");
						k=filas;
						add=false;
						}
					  }
					}
				if(add)	j++;
				add=true;					
				}
			}
		}
	printf("Devolvemos %d\n", j);
	return j;
	}
	
//********************* DETERMINAR RESTRICCIONES ******************************
//Calculamos las restricciones a las áreas en función de las probabilidades de emisión
/*
void determinarRestricciones(float *centroides, int numGaussianas, float *restricciones, char *fichero)
	{
	printf("DETERMINAR RESTRICCIONES\n");
	//a) Toma los valores de energías en los que estamos trabajando del continuo
	int c=1;
	while(c<Datos.numCanales && Datos.continuo[c]==0)	c++;
	float inicio=Datos.DaEnergia(c);
	while(c<Datos.numCanales && Datos.continuo[c]!=0)	c++;
	float fin=Datos.DaEnergia(c);
	LinkedList *l;
	bool tieneMaxima=true;
	bool restriccionAgnadida=false;
		
	for(int i=0;i<numGaussianas;i++)
		{
		//a) Iniciamos valores, buscamos emisiones asociadas al pico
		//printf("Gaussiana %d\n", i);
		restricciones[i]=0;
		tieneMaxima=true;
		l=new LinkedList();
		determinarNaturales(Datos.DaEnergia(centroides[i]), l, fichero);
		//b) Si está solo en su intervalo, tomamos su emisión más probable
		//Para cada emisión relacionada con el pico
		for(int k=0;k<l->numElements();k++)//NOTA: Para todos las emisiones asociadas
			{
			restriccionAgnadida=false;
//			printf("Emisión %d\n", k);
			//1)Cogemos las emisiones de ese radionúclido por encima del 2%
			NodoEmision *ne=(NodoEmision *)(l->getNode(k));
						
			int *emiMax;
			int tamEmiMax=8;
			emiMax=new int[tamEmiMax];
			LaraBD.daEmisionesConMayorProbabilidad((int)LaraBD.GetEmissor(ne->codElemento),inicio,fin, emiMax, 8, false, false);//Probando a sólo coger un det. número
			//2)Para cada una (de mayor probabilidad a menor), miramos si está en pico o multipico
			for(int j=0;j<tamEmiMax;j++)
				{
//				printf("Miramos para la emisión de %.2f keV (%d) con probabilidad %f\n", LaraBD.GetEnergy(emiMax[j]), j, LaraBD.GetProbability(emiMax[j]));
				//Buscamos el intervalo que tiene esta emisión
				//a) Miramos si existe en nuestro espectro, si no existe pasamos a la siguiente
				int iMax=Datos.daNumIntervaloConEnergia(LaraBD.GetEnergy(emiMax[j]));
				if(iMax>0)
					{
					//b)Si es la que estamos ajustando, no ponemos restricciones
					if(emiMax[j]==ne->codElemento)
						{
	//					printf("El pico que intentamos restringir es el mayor, sin restricción\n");
						//NOTA: Además, pasamos de las restricciones por otras emisiones conjuntas
						restricciones[i]=0;	j=tamEmiMax; k=l->numElements();
						printf("RESTRICCIÓN %d = 0!!!!!!!!!!!, es esa emisión\n", i);
						restriccionAgnadida=true;
						}
					else
						{
						NodoPicos *intervalo = DevuelveNodo(Datos.listaPicos, iMax);
						//printf("Vamos a tomar la emisión en multipico\n");
						NodoPico *pico=emisionEnMultipico(intervalo, iMax, emiMax[j], fichero);
						//NUEVO------------------------
//						if(pico!=NULL)	printf("El pico tiene más emisiones asociadas?: %d\n", tieneMultipleEmision(pico));
//						if(pico!=NULL)	printf("grado de solapamiento: %f\n", gradoSolapamiento(intervalo, pico));
//						if(pico!=NULL)	printf("incertidumbre(%.2f)>0,1*area(%.2f)\n",	pico->Ajuste.incerArea, pico->Ajuste.area*0.1);
						if(pico!=NULL && !tieneMultipleEmision(pico) )
						//	&& gradoSolapamiento(intervalo, pico)<=0.01
						//	&& pico->Ajuste.incerArea < pico->Ajuste.area*0.1)
							{
							float p,e,am,pm,em;
							p=LaraBD.GetProbability(ne->codElemento);
							am=(pico->Ajuste.area+pico->Ajuste.incerArea)*1.1;
							printf("área+prob=%f+%f\n", pico->Ajuste.area+pico->Ajuste.incerArea);
							pm=LaraBD.GetProbability(emiMax[j]);
							e=Datos.daEficiencia(ne->Energia);
							em=Datos.daEficiencia(Datos.DaEnergia(pico->Ajuste.Centroide));
							restricciones[i]+=(p*e*am)/(pm*em);
							printf("Restricción[%d]=(%.2f/%.2f)(%.2f/%.2f)*%.2f*2=%.2f; total=%.2f\n", 
								i, p,pm, e,em, am, (p*e*am)/(pm*em), restricciones[i]);
							restriccionAgnadida=true;
							if(restricciones[i]!=0)	j=tamEmiMax;
							}
						//-------------------------------
						}
					}
				else
					{//NOTA: De momento si ocurre esto no sé si salirme, desde luego es un mal indicio
//					printf("WARNING: No hay picos en esa energía, probablemente este radionúclido no exista en la muestra\n");
					tieneMaxima=false;
					//j=tamEmiMax;
					}
				}//Para todas las emisiones más probables (8)
			
			//Si no hemos cumplido los criterios de ninguna de las emisiones, 
			//tomamos para la restricción el que menos solapamiento tenga
//				printf("Mirando si hemos tomado restricción\n");
			//++++++++++++++++ EN CONSTRUCCIÓN +++++++++++++++++++++++++++
			//NOTA: Debería funcionar pero parece que salen áreas muy pequeñas
			//		Dejándolo sin arreglar por el tema de Bego
			if(!restriccionAgnadida && k!=l->numElements())
				{//Si no hay restricción y debe haberla (no nos salimos por k=numElements())
//				printf("mmmmmm -> No hay restricción, vamos a coger el de menor solapamiento\n");
				float gsmin=10000, gs=0;
				int iMaxMin=0;
				int jMin=0;
					
				for(int j=0;j<tamEmiMax;j++)
					{
//					printf("Cogemos la emisión %d: %f keV\n", j, LaraBD.GetEnergy(emiMax[j]));
					int iMax=Datos.daNumIntervaloConEnergia(LaraBD.GetEnergy(emiMax[j]));
					if(iMax>0)//Si existe esta emisión
						{
//						printf("La emisión existe\n");
						if(emiMax[j]!=ne->codElemento)//Y no es la de nuestro intervalo (eso lo comprobamos en el bucle anterior)
							{
//							printf("Y no es la de nuestro intervalo\n");
							NodoPicos *intervalo = DevuelveNodo(Datos.listaPicos, iMax);
							NodoPico *pico=emisionEnMultipico(intervalo, iMax, emiMax[j], fichero);
							if(pico!=NULL)
								{
//								printf("incer=%f VS 0,1*area=%f\n", pico->Ajuste.incerArea, pico->Ajuste.area*0.1);
//								printf("en multipico? %d\n", tieneMultipleEmision(pico));
								}
//							else	printf("El pico no existe\n");
							if(pico!=NULL && !tieneMultipleEmision(pico) 
								&& pico->Ajuste.incerArea < pico->Ajuste.area*0.1)
								{//Si cumple los otros requisitos
								gs=gradoSolapamiento(intervalo, pico);
//								printf("grado de solapamiento: %f\n", gs);
								if(gsmin>gs)
									{
									gsmin=gs;
									iMaxMin=iMax;
									jMin=j;
//									printf("Ahora gsmin es %f, imaxmin=%d y jMin=%d\n", gsmin, iMaxMin, jMin);
									}
								}
							}
						}
					}
//				printf("La emisión con menor solapamiento es la %d, %d; con %f\n", iMaxMin, jMin, gsmin);
				//Ahora que ya tenemos el que tiene el mínimo gs, lo tomamos para la restricción
				if(iMaxMin!=0)
					{
					NodoPicos *intervalo = DevuelveNodo(Datos.listaPicos, iMaxMin);
					NodoPico *pico=emisionEnMultipico(intervalo, iMaxMin, emiMax[jMin], fichero);
					float p,e,am,pm,em;
					p=LaraBD.GetProbability(ne->codElemento);
					am=(pico->Ajuste.area+pico->Ajuste.incerArea)*1.1;
					pm=LaraBD.GetProbability(emiMax[jMin]);
					e=Datos.daEficiencia(ne->Energia);
					em=Datos.daEficiencia(Datos.DaEnergia(pico->Ajuste.Centroide));
					restricciones[i]+=(p*e*am)/(pm*em);
					printf("Restricción[%d] (mínimo GS)=(%.2f/%.2f)(%.2f/%.2f)*%.2f*2=%.2f; total=%.2f\n", 
							i, p,pm, e,em, am, (p*e*am)/(pm*em), restricciones[i]);
					}
				else
					{
//					printf("ERROR: No hay ninguna emisión para el radionúclido\n");
					}
				}//if(tal) coger el de solapamiento mínimo
				
			if(emiMax!=NULL)	delete [] emiMax;
			}//Para todas las probabilidades buscadas
		delete l;			
		}	
//	printf("FIN DE DETERMINAR RESTRICCIONES\n");
	return;
	}
*/
//NUEVO: Versión que tiene en cuenta posibles cortes en el equilibrio
//NOTA: Esta nueva versión ahora no pone los picos que no debieran estar porque
//		hemos determinado con las restricciones que no tienen las emisiones de
//		mayor probabilidad
//		OJO: No quitamos nunca ningún pico por que pensemos que no pudiera estar.
//			Dependemos en gran medida del fondo, y el fondo puede estar eliminando
//			emisiones sólo en un determinado lugar del espectro que coincida con
//			el punto de mayor probabilidad de un elemento.
void determinarRestricciones(float *centroides, int numGaussianas, float *restricciones, char *fichero)
	{
	printf("DETERMINAR RESTRICCIONES\n");
	//a) Toma los valores de energías en los que estamos trabajando del continuo
	if(!opciones[3])	
		{
		for(int i=0;i<numGaussianas;i++)
			{
			restricciones[i]=-1;
			}
		return;
		}
		
	int c=1;
	while(c<Datos.numCanales && Datos.continuo[c]==0)	c++;
	float inicio=Datos.DaEnergia(c);
	while(c<Datos.numCanales && Datos.continuo[c]!=0)	c++;
	float fin=Datos.DaEnergia(c);
	LinkedList *l;
	bool tieneMaxima=true;
	bool restriccionAgnadida=false;
		
	for(int i=0;i<numGaussianas;i++)
		{
		//a) Iniciamos valores, buscamos emisiones asociadas al pico
		printf("Gaussiana %d\n", i);
		restricciones[i]=0;
		tieneMaxima=true;
		l=new LinkedList();
		determinarNaturales(Datos.DaEnergia(centroides[i]), l, fichero);
		printf("Tenemos %d emisiones asociadas\n", l->numElements());
		//b) Si está solo en su intervalo, tomamos su emisión más probable
		//Para cada emisión relacionada con el pico
		for(int k=0;k<l->numElements();k++)//NOTA: Para todos las emisiones asociadas
			{
			restriccionAgnadida=false;
			printf("Emisión %d\n", k);
			//1)Cogemos las emisiones de ese radionúclido por encima del 2%
			NodoEmision *ne=(NodoEmision *)(l->getNode(k));
			printf("Buscamos la actividad asociada a %d\n", ne->codElemento);
			float aa=actividadAsociada(LaraBD.GetEmissor(ne->codElemento));	
			if(aa>0)
				{
				float p,e;
				p=LaraBD.GetProbability(ne->codElemento)/100;
				e=Datos.daEficiencia(ne->Energia);
//				restricciones[i]+=aa*p*e*1.1;//Ponemos un 10% de holgura
				//NOTA: Nuevo-> si la eficiencia es 1, usarla para la restricción es 
				//		malo, porque significa que la eficiencia en ese punto no está
				//		estimada y le estamos dando el máximo. Deberíamos usar algún
				//		valor, dejarlo así??.
//				if(e==1)	restricciones[i]+=aa*p*1.3;
//				else		restricciones[i]+=aa*p*e*1.3;
				if(e==1)	restricciones[i]+=aa*p*1.1;
				else		restricciones[i]+=aa*p*e*1.1;
				printf("Usamos su actividad para calcular su restricción (%d,%d)\n", LaraBD.GetAtomicNumber(LaraBD.GetEmissor(ne->codElemento)), LaraBD.GetMassNumber(LaraBD.GetEmissor(ne->codElemento)));
				printf("Que será: N=a*p*ef=%.2f*%f*%.2f === %.2f (holgura %.2f)\n", aa,p,e,restricciones[i], restricciones[i]/11);
				}
			else
				{
				printf("WARNING: No tenemos actividad natural para ese elemento %d,%d, aa vale %d\n", LaraBD.GetAtomicNumber(LaraBD.GetEmissor(ne->codElemento)), LaraBD.GetMassNumber(LaraBD.GetEmissor(ne->codElemento)), aa);
				restricciones[i]=-1;
				k=l->numElements();
				}
			}//Para cada elemento asociado a la emisión
		delete l;			
		}	
	printf("FIN DE DETERMINAR RESTRICCIONES\n");
	return;
	}

//************* ACTIVIDAD ASOCIADA ******************
/*	Devuelve la actividad que hemos estimado para picos de emisiones asociados
	al elemento de código codElemento
	*/
float actividadAsociada(int codElemento)
	{
//	printf("ACTIVIDAD ASOCIADA\n");
	//printf("Nuestro elemento es %d, %d\n", LaraBD.GetMassNumber(codElemento), LaraBD.GetAtomicNumber(codElemento));
	cSearchLaraBD founds;
		
//	imprimeActividades();
	for(int i=0;i<numCadenas;i++)
		{
		int minEl=cadenas[i*2];
		int maxEl=cadenas[i*2+1];
		for(int j=0;j<(maxEl-minEl)/3;j++)
			{
//			printf("Buscamos elemento %d,%d\n", naturales[j*3+minEl+1], naturales[j*3+minEl]);
			
			founds=LaraBD.SearchElements(naturales[j*3+minEl+1], naturales[j*3+minEl+1], naturales[j*3+minEl], 
								  naturales[j*3+minEl], false, false, false, false, NULL);
							
			int elemento = founds.GetFirstResult();
//			printf("Tenemos %d elementos\n", founds.GetNumItemsFound());
			while (elemento != -1) 
				{
//				printf("Elemento %d, comparándolo con %d, si es asociamos %d,%d\n", codElemento, elemento, i, j);
				if(elemento==codElemento)
					{
//					printf("Lo es, asociamos actividades [%d][%d]\n", i,j);
//					printf("Devolvemos act[%d][%d]=%f \n", i,j,actividades[i][j]);
					if(actividades!=NULL)		return actividades[i][j];
					else						return 0;
					}	
				elemento = founds.GetNextResult();
				}
			}
		}
	printf("WARNING: No hemos encontrado ningún elemento asociado\n");
	return -1;
	}
	
void imprimeActividades()
	{
	for(int i=0;i<numCadenas;i++)
		{
		int minEl=cadenas[i*2];
		int maxEl=cadenas[i*2+1];
		for(int j=0;j<(maxEl-minEl)/3;j++)
			{
			printf("Actividad %d,%d=", i,j);
			printf("%f\n", actividades[i][j]);
			}
		}			
	}
//************* SIGMA ACTIVIDAD ASOCIADA ******************
/*	Devuelve la incertidumbre de la actividad que hemos estimado para picos de 
	emisiones asociados	al elemento de código codElemento
	*/
float sigmaActividadAsociada(int codElemento)
	{
	printf("sigma actividad asociada\n");
	//printf("Nuestro elemento es %d, %d\n", LaraBD.GetMassNumber(codElemento), LaraBD.GetAtomicNumber(codElemento));
	cSearchLaraBD founds;
	for(int i=0;i<numCadenas;i++)
		{
		int minEl=cadenas[i*2];
		int maxEl=cadenas[i*2+1];
		for(int j=0;j<(maxEl-minEl)/3;j++)
			{
			founds=LaraBD.SearchElements(naturales[j*3+minEl+1], naturales[j*3+minEl+1], naturales[j*3+minEl], 
								  naturales[j*3+minEl], false, false, false, false, NULL);					
			int elemento = founds.GetFirstResult();
			while (elemento != -1) 
				{
				if(elemento==codElemento)
					{
					return sigmaActividades[i][j];
					}	
				elemento = founds.GetNextResult();
				}
			}
		}
	printf("WARNING: No hemos encontrado ningún elemento asociado\n");
	return -1;
	}
//**************** DETERMINAR EQUILIBRIOS ***************
/*	Guarda en tres arrays los equilibrios de las cadenas de desintegración de
	U238, U235 y Th232.
	NOTA: Podría pasarse un parámetro indicando el tiempo que se considera para
	el equilibrio
	*/
void determinarEquilibrios(char *fichero)
	{
	printf("DETERMINAR EQUILIBRIOS\n");
	cSearchLaraBD founds;	 //Elementos encontrados naturales		

	//Calculamos los límites entre los que tenemos continuo
	int c=1;
	while(c<Datos.numCanales && Datos.continuo[c]==0)	c++;
	float inicio=Datos.DaEnergia(c);
	while(c<Datos.numCanales && Datos.continuo[c]!=0)	c++;
	float fin=Datos.DaEnergia(c);

	//Calculamos el array donde irán las actividades/restricciones
	//Reservamos memoria
	actividades=new float*[numCadenas];
	sigmaActividades=new float*[numCadenas];
	float **gsActividades=new float*[numCadenas];
	float **tmedios=new float *[numCadenas];
	int **ms=new int*[numCadenas];
	for(int i=0;i<numCadenas;i++)
		{
		int minEl=cadenas[i*2];
		int maxEl=cadenas[i*2+1];
		actividades[i]=new float[int((maxEl-minEl)/3)];
		sigmaActividades[i]=new float[int((maxEl-minEl)/3)];
		gsActividades[i]=new float[int((maxEl-minEl)/3)];
		tmedios[i]=new float[int((maxEl-minEl)/3)];
		ms[i]=new int[int((maxEl-minEl)/3)];
		printf("Reservando memoria %d\n", int((maxEl-minEl)/3));
		}
	//Iniciamos a cero
	for(int i=0;i<numCadenas;i++)
		{
		int minEl=cadenas[i*2];
		int maxEl=cadenas[i*2+1];
		int max=int((maxEl-minEl)/3);
		for(int j=0;j<max;j++)	
			actividades[i][j]=sigmaActividades[i][j]=gsActividades[i][j]=tmedios[i][j]=ms[i][j]=0;
		}
	
	//Recorremos las cadenas de desintegración para ver los equilibrios
	for(int i=0;i<numCadenas;i++)
		{
		printf("CADENA DE DESINTEGRACIÓN %d\n", i);
		int minEl=cadenas[i*2];
		int maxEl=cadenas[i*2+1];
		printf("Tenemos un array de actividades de %d\n", int((maxEl-minEl)/3));
		int k=0;
		for(int j=minEl;j<maxEl;j+=3)
			{
			//Calculamos el área de la emisión más probable de cada elemento de
			//la familia, si tiene un tiempo de vida suficiente para sospechar
			//que puede haber ruptura (> 1 año)
			//printf("buscando elemento %d,%d\n", naturales[j+1], naturales[j]);
			founds=LaraBD.SearchElements(naturales[j+1], naturales[j+1], naturales[j], 
								  naturales[j], false, false, true, true, NULL);
			
			if(naturales[j+1]==92 && naturales[j]==235)	printf("URANIO235 está en %d,%d\n", i,k);				
			if(naturales[j+1]==92 && naturales[j]==238)	printf("URANIO238 está en %d,%d\n", i,k);				
			if(naturales[j+1]==88 && naturales[j]==226)	printf("RADIO\n", i,k);				
			
			//printf("Hemos encontrado %d elementos para %d, %d\n", founds.GetNumItemsFound(), naturales[j], naturales[j+1]);
			if(founds.GetNumItemsFound()!=0)
				{
				int elemento = founds.GetFirstResult();
				actividades[i][k]=actividadModelo(elemento, inicio, fin, fichero, sigmaActividades[i][k], gsActividades[i][k]);
				tmedios[i][k]=LaraBD.GetDecayTime(elemento)/(3600*24*365);
				ms[i][k]=LaraBD.GetMassNumber(elemento);
				
				//printf("sigmaActividades[%d][%d] vale %f\n", i,k, sigmaActividades[i][k]);
				if(naturales[j+1]==92 && naturales[j]==238)
					{
					//El U235 debe ser calculado a la vez que el U238, para que se tenga
					//en cuenta más abajo en la cadena del U238, concretamente en el 226Ra,
					//con el que está solapado.
					printf("URANIO 238 es %d\n", elemento);
					founds=LaraBD.SearchElements(92, 92, 235,235, false, false, true, true, NULL);
					printf("Hemos encontrado %d elementos para %d, %d\n", founds.GetNumItemsFound(), naturales[j], naturales[j+1]);
					if(founds.GetNumItemsFound()!=0)
						{
						int elemento235 = founds.GetFirstResult();
						printf("URANIO 235 ES %d\n", elemento235);
						actividades[1][0]=actividadModelo(elemento235, inicio, fin, fichero, sigmaActividades[i][k], gsActividades[i][k]);
						tmedios[1][0]=LaraBD.GetDecayTime(elemento235)/(3600*24*365);
						ms[1][0]=LaraBD.GetMassNumber(elemento235);
						}
					}
				}
			else
				{
				//printf("%d: Ponemos su actividad a -10\n", k);
				actividades[i][k]=-10;
				sigmaActividades[i][k]=0;
				gsActividades[i][k]=0;
				tmedios[i][k]=0;
				ms[i][k]=0;
				}
			k++;					
			}
		}
		
	//Comprobaciones
	for(int i=0;i<numCadenas;i++)
		{
		int minEl=cadenas[i*2];
		int maxEl=cadenas[i*2+1];
		for(int j=0;j<(maxEl-minEl)/3;j++)
			printf("actividad[%d][%d]=%f(%f) (es el %d,%d)\n", i, j, actividades[i][j], sigmaActividades[i][j], naturales[j*3+minEl], naturales[j*3+minEl+1]);
		}
	printf("\n\n\n\n\n\n\n\n");	
	
	//Controlamos si los valores de actividad en los que hemos podido calcular son o no
	//equiparables, y si lo son igualamos al del padre:
	float t=0;
	for(int i=0;i<numCadenas;i++)
		{
		int minEl=cadenas[i*2];
		int maxEl=cadenas[i*2+1];
		float x1, x2, s1, s2;
		for(int j=0;j<(maxEl-minEl)/3;j++)
			{
			x1=actividades[i][j];
			s1=sigmaActividades[i][j];
			if(x1>0)//Recorremos hacia adelante la cadena
				{
				for(int k=j+1;k<(maxEl-minEl)/3;k++)
					{
					printf("Comprobando equilibrio en %d;%d\n", i,k);
					x2=actividades[i][k];
					s2=sigmaActividades[i][k];
					//NOTA: La comparación, teniendo en cuenta que no le ponemos
					//		tv, es entre dos valores muy altos, así que generalmente
					//		el test da que son distintos
					if(x2>0)
						{
						//Hacemos el test de student:
//						t=fabs(x1-x2)/sqrt(s1+s2);
						t=fabs(x1-x2)/sqrt(s1*s1+s2*s2);
						printf("s1=%e y s2=%e\n", s1, s2);
//						printf("comparando %f con %f -> t=%f=%f/%f\n", x1, x2, t, fabs(x1-x2), sqrt(s1+s2));
						printf("comparando %f con %f -> t=%f=%f/%f\n", x1, x2, t, fabs(x1-x2), sqrt(s1*s1+s2*s2));
						if(t<1.960)	actividades[i][k]=actividades[i][j];//Los valores no difieren
						}
					}
				}
			}
		}

	//Comprobaciones
	for(int i=0;i<numCadenas;i++)
		{
		int minEl=cadenas[i*2];
		int maxEl=cadenas[i*2+1];
		for(int j=0;j<(maxEl-minEl)/3;j++)
			{
			printf("actividad[%d][%d]=%f(%f) (es el %d,%d)\n", i, j, actividades[i][j], sigmaActividades[i][j], naturales[j*3+minEl], naturales[j*3+minEl+1]);
			}
		}
	printf("\n\n\n\n\n\n\n\n");	
			
	//
	//Ahora que tenemos las actividades, propagamos a los que estén en eq.
	//Tal y como vamos a hacerlo, todos los hijos cogen la del padre superior, aunque
	//la suya inicalmente fuera distinta en T!.
	//Si algún padre no tenía propia, la toma de su hijo más cercano con actividad
	for(int i=0;i<numCadenas;i++)
		{
		int minEl=cadenas[i*2];
		int maxEl=cadenas[i*2+1];
		for(int j=0;j<(maxEl-minEl)/3;j++)
			{
			int cod=j*3+minEl;
			//founds=LaraBD.SearchElements(naturales[cod+1], naturales[cod+1], naturales[cod], 
			//					  naturales[cod], false, false, true, true, NULL);					
			if(founds.GetNumItemsFound()!=0)
				{
				//Si para un elemento no hemos encontrado actividad				
				//vamos para adelante o para atraś hasta encontrar uno que tenga
				
				if(actividades[i][j]<0 && tmedios[i][j]>1)//Si no tiene buscamos en sus descendientes
					{
					for(int k=j+1;k<(maxEl-minEl)/3;k++)
						{
						if(actividades[i][k]>0 && (tmedios[i][k]<1 && ms[i][k]!=222) )	//NOTA: IMPORTANTE: No miramos si tienen actividad en el caso de que sean menores a 1 año, directametne tomamos la del padre
						//Sólo le igualamos la actividad si lo consideramos en eq
							{
							actividades[i][j]=actividades[i][k];
							k=(maxEl-minEl)/3;							
							}
//						else if(tmedio2>1)	k=(maxEl-minEl)/3;
						else if(tmedios[i][k]>1)	k=(maxEl-minEl)/3;
						}
					}
//				else if(actividades[i][j]<0 && (tmedio<1 && LaraBD.GetMassNumber(elemento)!=222) )//Si no tiene buscamos en su padre -> Para evitar forzar equilibrios con elementos intermedios que aunque con Tv pequeños, rompan la cadena, por ej: gases (Rn)
				else if(actividades[i][j]<0 && (tmedios[i][j]<1 && ms[i][j]!=222) )//Si no tiene buscamos en su padre -> Para evitar forzar equilibrios con elementos intermedios que aunque con Tv pequeños, rompan la cadena, por ej: gases (Rn)
					{
//					printf("Buscando actividad para %d,%d\n", naturales[cod+1], naturales[cod]);
					for(int k=j-1;k>=0;k--)
						{
//						printf("Su actividad es %f\n", 	actividades[i][k]);							
						if(actividades[i][k]>0)
						//Sólo le asumimos la misma actividad si no es de los que rompen
						//la cadena o, si lo son, la que le copiamos es de alguien con quien
						//está en equilibrio							
							{
							actividades[i][j]=actividades[i][k];
							k=0;
							}
						//if(tmedio2>1 || LaraBD.GetMassNumber(elemento2)==222)	k=0;//Hemos llegado a punto de ruptura, no podemos copiar act.
						if(tmedios[i][k]>1 || ms[i][k]==222)	k=0;//Hemos llegado a punto de ruptura, no podemos copiar act.
						}
					}
				}//Si hay elemento
			}//para cada elemento de la cadena
		}//para cada cadena
	
	//Comprobaciones
	for(int i=0;i<numCadenas;i++)
		{
		int minEl=cadenas[i*2];
		int maxEl=cadenas[i*2+1];
		for(int j=0;j<(maxEl-minEl)/3;j++)
			{
			printf("actividad[%d][%d]=%f (es el %d,%d)\n", i, j, actividades[i][j], naturales[j*3+minEl], naturales[j*3+minEl+1]);
			}
		}
	printf("\n\n\n\n\n\n\n\n");	
	//
	
	//NOTA: No me convence en absoluto esta última parte, es una decisión demasiado
	//		fuerte, y casi nunca funciona bien
	/*
	printf("Por subgrupos según gs\n");
	//Por último normalizamos grupos: puede haber equilibrios mal calculados o lo que sea.
	for(int i=0;i<numCadenas;i++)
		{
		int minEl=cadenas[i*2];
		int maxEl=cadenas[i*2+1];
		for(int j=0;j<(maxEl-minEl)/3;j++)
			{
			printf("j es %d\n",j);
			if(actividades[i][j]!=-10 && actividades[i][j]!=-111)
				{
				if(tmedios[i][j]>1 || ms[i][j]==222)
					{//Recorremos para coger el menor solapamiento del subgrupo
					printf("Le damos valores a gs, act, sact\n");
					float gs=gsActividades[i][j];
					printf("dando valor a act: %f\n", actividades[i][j]);
					float act=actividades[i][j];
					printf("dando valor a sact\n");
					float sact=sigmaActividades[i][j];
					printf("elemento %d,%d, gs=%f\n", i, j, gs);
					for(int k=j+1;k<(maxEl-minEl)/3;k++)
						{
						printf("elemento %d,%d\n", i,k);
						int cod2=k*3+minEl;
						if(actividades[i][k]!=-10 && actividades[i][k]!=-111)
							{
							printf("\tgs=%.2f\n", gsActividades[i][k]);
							if(tmedios[i][k]>1 || ms[i][k]==222)	
								{
								k=(maxEl-minEl)/3;
								printf("Con este se acaba la cadena (es el primero de la siguiente)\n");
								}
							else if(gsActividades[i][k]<gs && gsActividades[i][k]>=0)	
								{
								printf("gs cambia en el subgrupo a %f\n", gsActividades[i][k]),
								gs=gsActividades[i][k];
								act=actividades[i][k];
								sact=sigmaActividades[i][k];
								}
							}
						}
										
					//Una vez determinado el menor solapamiento del subgrupo, ponemos su actividad en todos
					printf("Extendemos a todo el subgrupo la actividad con menos solapamiento\n");
					for(int k=j;k<(maxEl-minEl)/3;k++)
						{
						printf("Elemento %d,%d\n", i,k);
						if(actividades[i][k]!=-10 && actividades[i][k]!=-111)
							{
							if(k>j && (tmedios[i][k]>1 || ms[i][k]==222))	{j=k;	k=(maxEl-minEl)/3;}
							else
								{		
								printf("Ponemos nueva actividad\n");
								actividades[i][k]=act;
								sigmaActividades[i][k]=sact;
								}
							}
						}
					}//if(otro subgrupo)
				}
			}//bucle interno
		}//bucle de subgrupos
	printf("Terminamos la propagación\n");
	//
		
	//Comprobaciones
	for(int i=0;i<numCadenas;i++)
		{
		int minEl=cadenas[i*2];
		int maxEl=cadenas[i*2+1];
		for(int j=0;j<(maxEl-minEl)/3;j++)
			{
			printf("actividad[%d][%d]=%f (es el %d,%d)\n", i, j, actividades[i][j], naturales[j*3+minEl], naturales[j*3+minEl+1]);
			}
		}
	*/
	printf("\n\n\n\n\n\n\n\n");	
	//
	equilibrioDeterminado=true;

	for(int i=0;i<numCadenas;i++)	
		{
		delete [] gsActividades[i];
		delete [] tmedios[i];
		delete [] ms[i];
		}
	delete [] gsActividades;
	delete [] tmedios;
	delete [] ms;
	
	return;
	}

//******************* IMPRIME RESTRICCIONES ********************	
/*	Crea un cuadro de diálogo en el que imprime las actividades con entradas:
	Peak of XXX keV has activity XXX (XXX)
	*/
void imprimeRestricciones()
	{
	Arg args[7];
	Cardinal i=0;
	Widget col, frameAct, f1;
	double e, a, sa;
	char texto[200];
	XmString cad;
	XmString cadena;
	Widget dlgAct;

	printf("MOSTRAMOS LOS DATOS OBTENIDOS PARA LAS RESTRICCIONES\n");
	
	Cardinal k = 0;
	XtSetArg (args[k], XmNtitle, "Constraints"); k++;
	XtSetArg (args[k], XmNmwmDecorations, MWM_DECOR_BORDER|MWM_DECOR_TITLE|MWM_DECOR_MENU); k++;
	XtSetArg (args[k], XmNmwmFunctions, MWM_FUNC_MOVE|MWM_FUNC_CLOSE); k++;
	XtSetArg (args[k], XmNbaseWidth, 400); k++;
	XtSetArg (args[k], XmNbaseHeight, 300); k++;
	XtSetArg (args[k], XmNmaxWidth, 400); k++;
	XtSetArg (args[k], XmNmaxHeight, 300); k++;
	
	dlgAct = XmCreateFormDialog(app_shell, "dlgAct", args, k);
//	XtAddCallback (XtParent(dlgAct), XmNpopdownCallback, QuitaVentanaAct, NULL);
	
	k=0;
	XtSetArg (args[k], XmNbaseWidth, 400); k++;
	XtSetArg (args[k], XmNbaseHeight, 300); k++;
	XtSetArg (args[k], XmNmaxWidth, 400); k++;
	XtSetArg (args[k], XmNmaxHeight, 300); k++;
	col=XmCreateRowColumn(dlgAct, "ColumnaAc", args, k);
	
	i=0;
	XtSetArg (args[i], XmNwidth, 400); i++;
	XtSetArg (args[i], XmNheight, 300); i++;
	f1=XmCreateFrame(col, "Frame simple", args, i);
 	
	cadena = XmStringCreateLocalized ("Computed Activities");
    i = 0;
	XtSetArg (args[i], XmNchildType, XmFRAME_TITLE_CHILD); i++;
   	XtSetArg (args[i], XmNchildVerticalAlignment, XmALIGNMENT_CENTER); i++;
   	XtSetArg (args[i], XmNlabelString, cadena); i++;
    Widget titulof1 = XmCreateLabelGadget (f1, "label_frame_analisis", args, i);
    XmStringFree (cadena);
	XtManageChild(titulof1);

	k = 0;
	XtSetArg (args[k], XmNselectionPolicy, XmSINGLE_SELECT); k++;
	XtSetArg (args[k], XmNvisibleItemCount, 10); k++;
	XtSetArg (args[k], XmNscrollBarDisplayPolicy, XmAS_NEEDED); k++;
	XtSetArg (args[k], XmNbaseWidth, 400); k++;
	XtSetArg (args[k], XmNbaseHeight, 300); k++;
	XtSetArg (args[k], XmNwidth, 400); k++;
	XtSetArg (args[k], XmNheight, 300); k++;
    Widget listaAct = XmCreateScrolledList(f1, "lista de restricciones", args, k);
	
	for(int i=0;i<contRestricciones;i++)
		{
		cadena=XmStringCreateLocalized(cadenaRestriccion[i]);
		XmListAddItemUnselected(listaAct, cadena, 0); 
		XmStringFree(cadena);
		}
	
	XtManageChild(listaAct);
	XtManageChild(col);
	XtManageChild(f1);
	XtManageChild(dlgAct);
	}

//********************** ACTIVIDAD MODELO ***************
/*	Devuelve la actividad del elemento según su emisión más
	clara en el espectro.
	Si no tiene emisiones suficientemente claras (NOTA: ajustar los criterios)
	o si directamente no tiene emisiones, devolvera un valor negativo.
	El criterio es el siguiente:
		1)Tomamos las 8 emisiones más probables en el tramo del continuo
		2)De la más probable a la menos: miramos si está solapada y sola
		(sola implica que sólo se le asocia una emisión)
		3)Si hay alguna no solapada sola de esas ocho, la tomamos
		4)Si no, pero hay alguna sola, cogemos la menos solapada
		5)Si no, miramos si es U235. En ese caso, tomamos como actividad
		la del U238 multiplicada por el factor de equilibrio isotópico 0.042
*/
//NOTA: Añadiendo para que 
float actividadModelo(int elemento, float inicio, float fin, char *fichero, float &sigma, float &gradoS)
	{
	printf("ACTIVIDAD MODELO de %d,%d\n", LaraBD.GetAtomicNumber(elemento), LaraBD.GetMassNumber(elemento));
		
	//1)Tomamos las emisiones con mayor probabilidad del elemento
	char nom[3];
	LaraBD.GetNameElement(elemento, nom);
	int an=LaraBD.GetAtomicNumber(elemento);
	int mn=LaraBD.GetMassNumber(elemento);
	int *emiMax;
	int tamEmiMax=8;
	emiMax=new int[tamEmiMax];
	LaraBD.daEmisionesConMayorProbabilidad(elemento,inicio,fin, emiMax, 8, true, false);//Probando a sólo coger un det. número
	
	//----------NOTA: Peculiaridad provisional para el U235: equilibrio isotópico
	if(an==92 && mn==235)//Si es U235, le ponemos el equilibrio isotópico
		{
//		cSearchLaraBD founds=LaraBD.SearchElements(an, an, 238,238, false, false, true, true, NULL);					
		cSearchLaraBD founds=LaraBD.SearchElements(90, 90, 234,234, false, false, true, true, NULL);					
		int uranio238 = founds.GetFirstResult();//A partir del Th234
		
		double a=0.042*actividadModelo(uranio238, inicio, fin, fichero, sigma, gradoS); 
		sigma=0.042*sigma;
		printf("------------->El U235 va a tener de restricción 0.042*%.2f=%.2f(%.2f)\n", a/0.042, a, sigma);
		if(a<0)	{a=0;sigma=0;}
		
		//-----Listado de actividades	
		printf("Añadiendo la cadena %d\n", contRestricciones);
		contRestricciones--;
		sprintf(cadenaRestriccion[contRestricciones++], "Model activity for %s%d based on emission of 234Th", nom, mn);
		//-------------				
		if(emiMax!=NULL)	delete [] emiMax;
		
		return a;
		}		
	//-----------------

	sigma=0;
	//2)Para cada una (de mayor probabilidad a menor), miramos si está en pico o multipico
	for(int j=0;j<tamEmiMax;j++)
		{
		if(emiMax[j]>0)
			{			
//			printf("Miramos para la emisión de %.2f keV (%d) con probabilidad %f\n", LaraBD.GetEnergy(emiMax[j]), j, LaraBD.GetProbability(emiMax[j]));
			//Buscamos el intervalo que tiene esta emisión
			//a) Miramos si existe en nuestro espectro, si no existe pasamos a la siguiente
			int iMax=Datos.daNumIntervaloConEnergia(LaraBD.GetEnergy(emiMax[j]));
			if(iMax>0)
				{
				NodoPicos *intervalo = DevuelveNodo(Datos.listaPicos, iMax);
				NodoPico *pico=emisionEnMultipico(intervalo, iMax, emiMax[j], fichero, true);
				
				if(pico!=NULL)	printf("El pico tiene más emisiones asociadas?: %d\n", tieneMultipleEmision(pico));
				if(pico!=NULL)	printf("grado de solapamiento: %f\n", gradoSolapamiento(intervalo, pico));
				if(pico!=NULL)	printf("incertidumbre(%.2f)>0,1*area(%.2f)\n",	pico->Ajuste.incerArea, pico->Ajuste.area*0.1);
				if(pico!=NULL && !tieneMultipleEmision(pico) 
					&& gradoSolapamiento(intervalo, pico)<=0.1//Menos de un 5% de solapamiento
//					&& pico->Ajuste.incerArea < pico->Ajuste.area*0.1)
					&& pico->Ajuste.incerArea < pico->Ajuste.area)//NOTA: estaba al 10%
					{
					float ps=0.0;
					float p=probabilidadBifurcaciones(emiMax[j], ps);
					float ef=Datos.daEficiencia(Datos.DaEnergia(pico->Ajuste.Centroide));
					float sef=Datos.daSigmaEficiencia(Datos.DaEnergia(pico->Ajuste.Centroide));
					double a=actividad(pico->Ajuste.area, p, ef, 1, 1);
					sigma=sigmaActividad(a, pico->Ajuste.area, pico->Ajuste.incerArea, 
										p, ps, ef, sef, 1, 0);
					printf("actividad de restricción será %.2f=%.2f(%.2f)/%f(%f)*%.2f(%.2f)\n", 
						a, pico->Ajuste.area, pico->Ajuste.incerArea, p, ps, ef, sef);
					gradoS=gradoSolapamiento(intervalo, pico);
					
					//-----Listado de actividades	
					float energia=LaraBD.GetEnergy(emiMax[j]);
					sprintf(cadenaRestriccion[contRestricciones++], "Model activity for %s%d based on emission of %fkeV", nom, mn, energia);
					//-------------				
						
					if(emiMax!=NULL)	delete [] emiMax;
					return a;
					}
				}
			else
				{//NOTA: De momento si ocurre esto no sé si salirme, desde luego es un mal indicio
				printf("WARNING: No hay picos en esa energía, probablemente este radionúclido no exista en la muestra\n");
				gradoS=-1;
				sigma=0;
				return -111;//La emisión de mayor probabilidad no está en la muestra
				}
			}
		}//Para todas las emisiones más probables (8)
			
	//Si no hemos cumplido los criterios de ninguna de las emisiones, 
	//tomamos para la restricción el que menos solapamiento tenga
//	printf("No hemos obtenido una actividad limpia, probando con la de menor solapamiento\n");
	float gsmin=10000, gs=0;
	int iMaxMin=0;
	int jMin=0;
		
	for(int j=0;j<tamEmiMax;j++)
		{
		int iMax=Datos.daNumIntervaloConEnergia(LaraBD.GetEnergy(emiMax[j]));
		if(iMax>0)//Si existe esta emisión
			{
			NodoPicos *intervalo = DevuelveNodo(Datos.listaPicos, iMax);
			NodoPico *pico=emisionEnMultipico(intervalo, iMax, emiMax[j], fichero, true);
			if(pico!=NULL && !tieneMultipleEmision(pico) 
//				&& pico->Ajuste.incerArea < pico->Ajuste.area*0.1)
				&& pico->Ajuste.incerArea < pico->Ajuste.area)
				{//Si cumple los otros requisitos
				gs=gradoSolapamiento(intervalo, pico);
				if(gsmin>gs)
					{
					gsmin=gs;
					iMaxMin=iMax;
					jMin=j;
					}
				}
			}
		}
		//Ahora que ya tenemos el que tiene el mínimo gs, lo tomamos para la restricción
		if(iMaxMin!=0)
			{
			NodoPicos *intervalo = DevuelveNodo(Datos.listaPicos, iMaxMin);
			NodoPico *pico=emisionEnMultipico(intervalo, iMaxMin, emiMax[jMin], fichero, true);
			float ps=0;
			float p=probabilidadBifurcaciones(emiMax[jMin], ps);
			float ef=Datos.daEficiencia(Datos.DaEnergia(pico->Ajuste.Centroide));
			float sef=Datos.daSigmaEficiencia(Datos.DaEnergia(pico->Ajuste.Centroide));
			double a=actividad(pico->Ajuste.area, p,ef, 1, 1);
		//	printf("actividad que le damos es %f\n", a);
			sigma=sigmaActividad(a, pico->Ajuste.area, pico->Ajuste.incerArea,
				p, ps, ef, sef, 1, 0);
			gradoS=gsmin;
		//	printf("y sigma: %f\n", sigma);
					//-----Listado de actividades	
					float energia=LaraBD.GetEnergy(emiMax[jMin]);
			//		printf("Añadiendo la cadena (MENOR SOLAP.) %d: MOdel activity for %s%d based on emission of %.2f keV\n", contRestricciones, nom, mn, energia);
					sprintf(cadenaRestriccion[contRestricciones++], "Model activity for %s%d based on emission of %.2f keV", nom, mn, energia);
					//-------------				
			if(emiMax!=NULL)	delete [] emiMax;
			return a;
			}
		else
			{
			printf("ERROR: No hay ninguna emisión para el radionúclido\n");
			if(emiMax!=NULL)	delete [] emiMax;
			sigma=0;
			return -111;//Todas las emisiones de mayor probabilidad no están en la muestra
			}
//		}//for(tal) en caso de coger el de solapamiento mínimo
	
	if(emiMax!=NULL)	delete [] emiMax;
	sigma=0;
	return -111;//No hay emisiones de este radionúclido
	}
	
//**************** PROBABILIDAD BIFURCACIONES *******************
/*	Para un determinado elemento natural que provoca la emision que se pasa como
	parámetro, consulta las cadenas de desintegración
	para determinar si hay alguna bifurcación entre él y el elemento inmediatamente
	superior (incluido ese mismo elemento) que tenga un tiempo de desintegración > 1 año.
	Si hay bifurcación, devuelve la probabilidad del padre inicial para esa 
	emisión, la única que tenemos en Lara (la del U238, U235 o Th232, no tenemos
	la de otros elementos con t>1a). En otro caso, devuelve la del mismo elemento.
	Si hay algún error devuelve -1.
	*/
float probabilidadBifurcaciones(int emision, float &sigma)
	{
	printf("PROBABILIDAD BIFURCACIONES\n");
	cSearchLaraBD founds;
	int elemento=LaraBD.GetEmissor(emision);
	//Buscamos el hijo en la cadena de desintegración
	int m=LaraBD.GetMassNumber(elemento);
	int a=LaraBD.GetAtomicNumber(elemento);
	float e=LaraBD.GetEnergy(emision);
	int pos=posicionEnCadena(m,a);
	float prob=LaraBD.GetProbability(emision)/100;
	sigma=LaraBD.GetIncProbability(emision)/100;
	for(int i=pos-3;i>0;i-=3)
		{
		if(bifurcaciones[pos/3])
			{//la probabilidad que se usa es la del padre inicial
			printf("Tenemos bifurcación: la probabilidad es la del elemento padre!!!\n");
			founds=LaraBD.SearchEmissions(e-0.03, e+0.03, -1,-1,-1, 90,90,232,232,
				false, false, false, false, "", false, false);
			founds=founds+LaraBD.SearchEmissions(e-0.03, e+0.03, -1,-1,-1, 92,92,235,235,
				false, false, false, false, "", false, false);
			founds=founds+LaraBD.SearchEmissions(e-0.03, e+0.03, -1,-1,-1, 92,92,238,238,
				false, false, false, false, "", false, false);
			if(founds.GetNumItemsFound()>0)
				{
				int emipadre=founds.GetFirstResult();
				printf("Devolvemos la emisión del padre que es %.2f en vez de %.2f\n", LaraBD.GetProbability(emipadre)/100, prob);
				sigma=LaraBD.GetIncProbability(emipadre)/100;
				return LaraBD.GetProbability(emipadre)/100;
				}
			else
				{
				printf("WARNING: No hemos encontrado emisión del padre para %d,%d en energía %f\n", a,m, e);
				return -1;
				}
			}
		
		founds=LaraBD.SearchElements(naturales[i+1], naturales[i+1], naturales[i], 
								naturales[i], false, false, true, true, NULL);					
		int elemento = founds.GetFirstResult();
		float tmedio = LaraBD.GetDecayTime(elemento)/(3600*24*365);
		if(tmedio>1)	
			{
			return prob;
			printf("No hay bifurcación o no nos afecta, cogemos la prob del hijo(%d,%d): %f\n", a,m,prob);
			}	
		}
	return -1;
	}

//******************* POSICIÓN EN CADENA ****************
/*	Devuelve la posición del elemento m,a en las cadenas de desintegración, o
	-1 si no lo encuentra.
*/	
int posicionEnCadena(int m, int a)
	{
	for(int i=0;i<numCadenas;i++)
		{
		int minEl=cadenas[i*2];
		int maxEl=cadenas[i*2+1];
		for(int j=0;j<(maxEl-minEl)/3;j++)
			if(m==naturales[j*3+minEl] && naturales[j*3+minEl+1])	return (minEl+j*3);				
		}
	return -1;
	}

//**************** TIENE MULTIPLE EMISIÓN ***************
/*	Devuelve true si tiene más de una emisión asociada al pico
	Devuelve false si tiene una sola emisión o no está identificado
	*/
bool tieneMultipleEmision(NodoPico *pico)
	{
	if(pico==NULL)	{printf("ERROR: Pico inexistente\n"); return false;}
	if(pico->Ajuste.emisiones->numElements()>1)	return true;
	else
		{
		if(pico->Ajuste.emisiones->numElements()==0)	printf("Error: no está identificado\n");
		return false;		
		}
	}
	
//**************** GRADO SOLAPAMIENTO ***********************
/*	Devuelve el porcentaje del pico que se encuentra solapado
	con otros picos, medido como:
	
	nº de canales del pico en que están otros picos
	------------------------------------------------
			nº total de canales del pico
	
	nº de cuentas de otros picos en su intervalo
	----------------------------------------------
		nº total de cuentas del pico
	
	NOTA: Es un poco burdo, no tiene en cuenta el nº de cuentas que están i
	*/
//float gradoSolapamiento(int numIntervalo, int numPico)
float gradoSolapamiento(NodoPicos *intervalo, NodoPico *pico)
	{
//	printf("GRADO DE SOLAPAMIENTO\n");
//	int cInf=intervalo->limInferior+1;
//	int cSup=intervalo->limSuperior+1;
	int cInf=(int)(pico->Ajuste.Centroide-4*pico->Ajuste.Anchura);
	int cSup=(int)(pico->Ajuste.Centroide+4*pico->Ajuste.Anchura);
		
	float sumIntervalo=0.0, sumPico=0.0;
//	printf("calculando de %d a %d\n", cInf, cSup);
		
	//Cálculo de solapamiento por partes.
	int f1=intervalo->limInferior+1;
	int f2=(int)(pico->Ajuste.Centroide-2*pico->Ajuste.Anchura);
	int f3=(int)(pico->Ajuste.Centroide-pico->Ajuste.Anchura);
	int f4=(int)(pico->Ajuste.Centroide);
	int f5=(int)(pico->Ajuste.Centroide+pico->Ajuste.Anchura);
	int f6=(int)(pico->Ajuste.Centroide+2*pico->Ajuste.Anchura);
	int f7=intervalo->limSuperior+1;
		
	float sol1, sol2, sol3, sol;
	sol1=sol2=sol3=sol=0;
	float si1,sp1,si2,sp2,si3,sp3;
	si1=sp1=si2=sp2=si3=sp3=0;
	float sum1,sum2,sum3;
	sum1=sum2=sum3=0;
	
	for(int j=f1;j<f2;j++)	
		{
		if( (Datos.DaValorPico(intervalo,pico,j)-Datos.continuo[j])*0.08>1)
			{
			si1+=Datos.DaValorIntervalo(intervalo,j);
			sp1+=Datos.DaValorPico(intervalo,pico,j);
			}
		}			
	for(int j=f2+1;j<f3;j++)	
		{
		if( (Datos.DaValorPico(intervalo,pico,j)-Datos.continuo[j])*0.58>1)
			{
			si2+=Datos.DaValorIntervalo(intervalo,j);
			sp2+=Datos.DaValorPico(intervalo,pico,j);
			}
		}			
	for(int j=f3+1;j<f5;j++)	
		{
		if( (Datos.DaValorPico(intervalo,pico,j)-Datos.continuo[j])>1)
			{
			si3+=Datos.DaValorIntervalo(intervalo,j);
			sp3+=Datos.DaValorPico(intervalo,pico,j);
			}
		}			
	for(int j=f5+1;j<f6;j++)	
		{
		if( (Datos.DaValorPico(intervalo,pico,j)-Datos.continuo[j])*0.58>1)
			{
			si2+=Datos.DaValorIntervalo(intervalo,j);
			sp2+=Datos.DaValorPico(intervalo,pico,j);
			}
		}			
	for(int j=f6+1;j<f7;j++)	
		{
		if( (Datos.DaValorPico(intervalo,pico,j)-Datos.continuo[j])*0.08>1)
			{
			si1+=Datos.DaValorIntervalo(intervalo,j);
			sp1+=Datos.DaValorPico(intervalo,pico,j);
			}
		}
		
	if(si1>0)	sol1=0.08*(si1-sp1)/si1;
	if(si2>0)	sol2=0.58*(si2-sp2)/si2;
	if(si3>0)	sol3=(si3-sp3)/si3;
	sol=sol1+sol2+sol3;
	printf("Grado de solapamiento será %.2f+%.2f+%.2f=%.2f\n", sol1,sol2,sol3, sol);
	return sol;
	//		
		
	//Cálculo de solapamiento de manera global.
/*	for(int j=cInf;j<cSup;j++)
		{
//		if( Datos.DaValorPico(intervalo, pico, j)>0)//Si hay presencia del pico
		if( Datos.DaValorPico(intervalo, pico, j) > Datos.continuo[j]+1)//Si hay presencia del pico
			{
			printf("%d (%f)", j, (Datos.DaValorPico(intervalo, pico, j) - Datos.continuo[j]) );
			sumIntervalo+=Datos.DaValorIntervalo(intervalo, j);
			sumPico		+=Datos.DaValorPico(intervalo, pico, j);
			}
		}
	printf("\n");
		
	printf("Canales del intervalo total son %.2f y del pico es %.2f\n", sumIntervalo, sumPico);
	float sumExterna=sumIntervalo-sumPico;
	if(sumExterna<0)	sumExterna=0;
	if(sumPico>0)	
		{
		printf("Grado de solapamiento=%f, de otro modo sería %f\n", sumExterna/sumPico, sumExterna/sumIntervalo);
		return sumExterna/sumIntervalo;	
		}
	else
		{
		printf("ERROR: Suma del área del pico<1\n");
		return -1;	
		}
		*/
	}
	
//*************** AREA EMISION EN MULTIPICO ******************
/*	Devuelve el pico que contiene la emisión emi. Lo ajusta si aún no existe
	RETURNS:
		NodoPico * pico en el que está la emisión
	OUT:
		NodoPicos *intervalo Intervalo en el que está el pico correspondiente a la emisión
	IN:
		int numIntervalo
		int emi				Emisión a buscar
		char *ficheros		Fichero de emisiones
		bool volverInvisible	Indica si, en caso de tener que ajustar el pico, debemos 
								dibujarlo o no (usado para los picos que se usan para 
								determinar eq., que no los añade el usuario)
		
*/
NodoPico *emisionEnMultipico(NodoPicos *intervalo, int numIntervalo, int emi, char *fichero, bool volverInvisible)
	{
	printf("EMISIONENMULTIPICO----------------------------\n");
	float eInf=Datos.DaEnergia(Datos.DaLimInferior(numIntervalo));
	printf("LÍMITE inferior: %f\n", eInf);
	float eSup=Datos.DaEnergia(Datos.DaLimSuperior(numIntervalo));
	printf("LÍMITE superior: %f\n", eSup);
	if(intervalo==NULL)	printf("Intervalo es null!!!\n");
	NodoPico *pico = DevuelveNodo(intervalo->picos, 1);
	if(pico==NULL)	{printf("pico es NULL!!!!\n"); return NULL;}
	printf("Hemos tomado el intervalo %d,1, que tiene %d picos\n", numIntervalo, intervalo->picos.numElementos);
	if(!pico->Ajuste.ajustado)
		{
		printf("El pico no está ajustado\n");
		int numGaussianas=determinarNumCentroides(eInf, eSup, fichero);
		if(numGaussianas>0)
			{
			int nError=0;
			float centroides[numGaussianas];
			printf("Ajustamos intervalo\n");
			determinarCentroides(eInf, eSup, centroides, fichero);
//			nError=Datos.AjusteIntervaloGAML(numIntervalo, centroides, numGaussianas);//sin restricciones
			
			//con restricciones
			//Determinamos las restricciones que les tenemos que poner a las áreas
			string cad(BDIdentificaciones);
	
			float restricciones[numGaussianas];
			determinarRestricciones(centroides, numGaussianas, restricciones, (char *)cad.c_str());	
			nError=Datos.AjusteIntervaloGAML(numIntervalo, centroides, restricciones, numGaussianas,-1,-1,-1);
			//
				
			float centroidesAjustados[intervalo->picos.numElementos];
			for (int i = 1; i <= intervalo->picos.numElementos; i++) 
				{
				NodoPico *pico = DevuelveNodo(intervalo->picos, i);
				centroidesAjustados[i-1]=pico->Ajuste.Centroide;
				printf("Área del pico %d es %f(%f)\n", i, pico->Ajuste.area, pico->Ajuste.incerArea);
				}
			//los analizamos
			analisisExperto(numIntervalo, centroidesAjustados,NULL);
			printf("Volvemos del AE en emisionEnMultipico\n");
			//	
			if(volverInvisible)
				{
				for (int i = 1; i <= intervalo->picos.numElementos; i++) 
					{
					NodoPico *pico = DevuelveNodo(intervalo->picos, i);
					pico->Ajuste.invisible=true;
					}
				Datos.ActualizaIntervaloAnalisis(numIntervalo); 
				}
			//
			}
		else	//No hay centroides en su intervalo
			{
			printf("Llamamos a datos.ajustaintervalo\n");
			int nError = Datos.AjusteIntervalo(numIntervalo);
			printf("Volvemos con error %d\n", nError);
			switch (nError) 
				{
				case -1:
					/*PonCursorNormal();
					char texto[100];
					sprintf(texto, "Fitting in [%d-%d] may not be accurate", (int)(Datos.DaEnergia(intervalo->limInferior)), (int)(Datos.DaEnergia(intervalo->limSuperior)+1) );
					Aviso(fondo_analisis, texto);
					PonCursorEspera();
					printf(ERROR_ANALYSIS);
					printf(ERROR_GENERAL);*/
					break; // continua con el resto de intervalos
				case -2:
					PonCursorNormal();
					Aviso(fondo_analisis, "No Memory Avalaible");
					printf(ERROR_ANALYSIS);
					printf(ERROR_NO_MEMORY);
					return NULL;
				}
			//	
			if(volverInvisible)
				{
				for (int i = 1; i <= intervalo->picos.numElementos; i++) 
					{
					NodoPico *pico = DevuelveNodo(intervalo->picos, i);
					pico->Ajuste.invisible=true;
					Datos.ActualizaIntervaloAnalisis(numIntervalo); 
					}
				}
			//
			printf("Intervalo ajustado por método normal\n");
			}
		}
	else
		{ 
		printf("Pico identificado\n");
//		if(!pico->Ajuste.identificado)	//Puede que esté ajustado pero no identificado
			{
			printf("El pico está ajustado pero no está identificado\n");
			float centroidesAjustados[intervalo->picos.numElementos];
			for (int i = 1; i <= intervalo->picos.numElementos; i++) 
				{
				NodoPico *pico = DevuelveNodo(intervalo->picos, i);
				centroidesAjustados[i-1]=pico->Ajuste.Centroide;
				}
			//los analizamos
			analisisExperto(numIntervalo, centroidesAjustados, NULL);
			printf("Volvemos del AE en emisionMultipico 2\n");
			}
		}
		
	printf("Cogemos el pico del intervalo, que tiene %d picos\n", intervalo->picos.numElementos);
	//printf("Hola?\n");
	for(int k=1;k<=intervalo->picos.numElementos;k++)
		{//Buscamos cuál de ellos es el que necesitamos
		NodoPico *pico=DevuelveNodo(intervalo->picos, k);
		for(int m=0;m<pico->Ajuste.emisiones->numElements();m++)
			{//Y cuando lo encontramos, lo usamos para el área (NOTA: Así tal cual, sin mirar solapamientos o emisiones adjuntas)
			NodoEmision *nodoe=(NodoEmision *)pico->Ajuste.emisiones->getNode(m);
			if(nodoe->codElemento==emi)		return pico;
			}
		}
	printf("fin de EMISIONENMULTIPICO---------------------------------------\n");
	return NULL;
	}

//************** PREPARA FICHERO ****************************
/*	Sustituye puntos por comas o comas por puntos dependiendo del 
	sistema operativo en el que ejecutemos:
		Puccini -> cambia todos los puntos decimales por comas
		Resto	-> cambia todos las comas decimales por puntos
		(NOTA: Realmente, todos los puntos o comas, no sólo los decimales)
	
	*/
void preparaFichero(char *nombre)
	{/*
//	printf("PREPARA FICHERO\n");
	char *os=getenv("HOSTNAME");
//	printf("EL SISTEMA OPERATIVO ES %s\n", getenv("HOSTNAME"));
	char *disp=getenv("DISPLAY");

	if(strcmp(disp,":0.0") && strcmp(disp,":0") )	
		{
		printf("Estamos ejecutando en remoto, coma por punto\n");
		sustituyeCaracter(nombre, ",",".");
		}
	else	
		{
		printf("Estamos ejecutando en local, punto por coma\n");
		sustituyeCaracter(nombre, ".", ",");
		}
//	printf("fin de prepara fic\n");*/
	return;
	}
	
//************** SUSTITUYE CARACTER *************************
/*	Sustituye todas las ocurrencias de la secuencia a por la secuencia b
	en el fichero nombre.
	*/
void sustituyeCaracter(char *nombre, char *a, char *b)
	{
//	printf("SUSTITUYE CARACTER %s por %s en |%s|\n",a,b,nombre);
	ifstream file(nombre);
//	printf("abierto archivo de entrada\n");
	if (! file.is_open()) 	
		{ 
		sprintf(mensajeError, "Error opening file %s", nombre);
		Aviso(main_w, mensajeError);
		cout << "Error opening file" << nombre; 
		return;
		}
//	printf("abriendo archivo de salida\n");
	char buffer[256];
	ofstream ofile("temp");
	int ind=0;
//	printf("fichero abierto\n");
	//Tomamos de cada línea la energía y el factor de corrección
	while (!file.eof())
  		{
//		printf(".\n");
	    file.getline (buffer,100);
		
		string cadena(buffer);
		while( (ind=cadena.find(a))!=-1)	cadena.replace(ind, 1, b);

		//para evitar que añada \n al final sin necesidad
		if(!strcmp(buffer,"EOF"))	ofile << cadena;
		else						ofile << cadena << endl;
		}
	ofile.close();
//	printf("Pasamos del temporal al final\n");
	//Pasamos del fichero temporal al fichero final
	ifstream ifile2("temp");
	ofstream ofile2(nombre);
		
	if (!ifile2.is_open() || !ofile2.is_open() ) 	
		{
		printf("ERROR opening file\n");			
		exit(1); 
		}
	while (! ifile2.eof() )
		{
		ifile2.getline (buffer,1024);

		//para evitar que añada \n al final sin necesidad
		if(!strcmp(buffer,"EOF"))	ofile2 << buffer;
		else						ofile2 << buffer << "\n";
		}
	ofile2.close();
	ifile2.close();
	remove("temp");
		
	file.close();
	ofile.close();
	}

//*************** LEER ELEMENTO ****************
/* Devuelve el valor double contenido en la posición i,j del fichero tabulado
	llamado nombre
*/	
double leerElemento(char *nombre, int fila, int columna)
	{
	char buffer[1024];
	fstream file(nombre, ios::in);
	int ind=0;
//	printf("LEER ELEMENTO %d,%d de %s\n", fila, columna, nombre);	
	for(int i=0;i<fila;i++)		file.getline (buffer,1024);
	
	//Cogemos la fila que queríamos
	file.getline (buffer,1024);
	string cadena(buffer);
	//cout << cadena << endl;
	//Quitamos las columnas anteriores
	for(int j=0;j<columna;j++)
		{
		if(cadena.length()<=0)	return -1;
		ind=cadena.find("\t");
		cadena=cadena.substr(ind+1, cadena.length()-ind-1);
		for(int k=0;k<10;k++)
			{
			if(cadena[0]=='\t')	cadena=cadena.substr(1, cadena.length()-1);
			else 				break;
			}
		}
	//Queda quitar las columnas posteriores
	ind=cadena.find("\t");
	cadena=cadena.substr(0,ind);
	//cout << cadena << endl;
	file.close();
	//printf("Devolvemos el valor %f\n", atof((char *)cadena.c_str()) );
	return (atof((char *)cadena.c_str()));
	}
	
//********************** LEER NÚMERO ENTRE PARÉNTESIS ********************
/*	Lee el elemento en (fila,columna). Si hay algo entre paréntesis, toma el
	número entre paréntesis, si no el que haya.
	*/
double leerNumeroEntreParentesis(char *nombre, int fila, int columna)
	{
	char buffer[1024];
	fstream file(nombre, ios::in);
	int ind=0;
//	printf("LEER NÚMERO ENTRE PARÉNTESIS %d,%d de %s\n", fila, columna, nombre);	
	for(int i=0;i<fila;i++)		file.getline (buffer,1024);
	
	//Cogemos la fila que queríamos
	file.getline (buffer,1024);
	string cadena(buffer);
	//cout << cadena << endl;
	//Quitamos las columnas anteriores
	for(int j=0;j<columna;j++)
		{
		if(cadena.length()<=0)	return -1;
		ind=cadena.find("\t");
		cadena=cadena.substr(ind+1, cadena.length()-ind-1);
		for(int k=0;k<10;k++)
			{
			if(cadena[0]=='\t')	cadena=cadena.substr(1, cadena.length()-1);
			else 				break;
			}
		}
	//Queda quitar las columnas posteriores
	ind=cadena.find("\t");
	cadena=cadena.substr(0,ind);
	//cout << cadena << endl;
	file.close();
	//printf("Devolvemos el valor %f\n", atof((char *)cadena.c_str()) );

	//Nos quedamos sólo con lo que está entre paréntesis:
	ind=cadena.find("(");
	if(ind==-1)	return (atof((char *)cadena.c_str()));
	int ind2=cadena.find(")");
	if(ind2!=-1)
		{
		cadena=cadena.substr(ind+1, ind2-1);
//		cout << "cadena entre paréntesis es: " << cadena << endl;
		return (atof((char *)cadena.c_str()));		
		}
	else
		{
		cout << "ERROR: Paréntesis abierto pero no cerrado" << endl;
		return (atof((char *)cadena.c_str()));		
		}
	printf("Terminamos de leer número entre paréntesis\n");
	return (atof((char *)cadena.c_str()));
	}

//*************** LEER ELEMENTO CADENA****************
/* Devuelve el valor char* contenido en la posición i,j del fichero tabulado
	llamado nombre
*/	
char *leerElementoCadena(char *nombre, int fila, int columna)
	{
	char buffer[1024];
	fstream file(nombre, ios::in);
	int ind=0;
		
	for(int i=0;i<fila;i++)		file.getline (buffer,1024);
	
	//Cogemos la fila que queríamos
	file.getline (buffer,1024);
	string cadena(buffer);
	
	//Quitamos las columnas anteriores
	for(int j=0;j<columna;j++)
		{
		ind=cadena.find("\t");
		cadena=cadena.substr(ind+1, cadena.length()-ind-1);
		for(int k=0;k<10;k++)
			{
			if(cadena[0]=='\t')	cadena=cadena.substr(1, cadena.length()-1);
			else 				break;
			}
		}
	//Queda quitar las columnas posteriores
	ind=cadena.find("\t");
	cadena=cadena.substr(0,ind);
	file.close();
	return ((char *)cadena.c_str());
	}

//*************** LEER ELEMENTO ENTRE PARÉNTESIS ****************
/* Devuelve el valor char* contenido en la posición i,j del fichero tabulado
	llamado nombre
*/	
char *leerElementoEntreParentesis(char *nombre, int fila, int columna)
	{
	char buffer[1024];
	fstream file(nombre, ios::in);
	int ind=0;
		
	for(int i=0;i<fila;i++)		file.getline (buffer,1024);
	
	//Cogemos la fila que queríamos
	file.getline (buffer,1024);
	string cadena(buffer);
	file.close();
	
	//Quitamos las columnas anteriores
	for(int j=0;j<columna;j++)
		{
		ind=cadena.find("\t");
		cadena=cadena.substr(ind+1, cadena.length()-ind-1);
		for(int k=0;k<10;k++)
			{
			if(cadena[0]=='\t')	cadena=cadena.substr(1, cadena.length()-1);
			else 				break;
			}
		}
	//Queda quitar las columnas posteriores
	ind=cadena.find("\t");
	cadena=cadena.substr(0,ind);
	
	//Nos quedamos sólo con lo que está entre paréntesis:
	ind=cadena.find("(");
	if(ind==-1)	return ((char *)cadena.c_str());
	int ind2=cadena.find(")");
	if(ind2!=-1)
		{
		cadena=cadena.substr(ind+1, ind2-1);
//		cout << "cadena entre paréntesis es: " << cadena << endl;
		return ((char *)cadena.c_str());		
		}
	else
		{
		cout << "ERROR: Paréntesis abierto pero no cerrado" << endl;
		return ((char *)cadena.c_str());		
		}
	}
	
//**************** QUITAR PARÉNTESIS
/*	Quita la información de más correspondiente a (...)
	*/
char *quitarParentesis(char *cad)
	{
	//printf("creamos cadena\n");//NOTA: Valgrind da un source and destination overlap aquí
	string cadena(cad);
	int ind=0;
	//printf("buscamos paréntesis\n");
	ind=cadena.find("(");
	//printf("recortamos cadena\n");
	if(ind!=string::npos)		cadena=cadena.substr(0,ind);
	//printf("devolvemos convertida\n");
	return ((char *)cadena.c_str());
	}	
/*
//***************** TOMAR NOMBRE *******************	
//Coge el nombre de un radionúclido con la forma Th232 ---> Th
char *tomarNombre(char *cad)
	{
	string cadena(cad);
	int ind=0;
//	if(isdigit(cadena[1]))	cadena=" "+cadena.substr(0,1);	
//	else					cadena=cadena.substr(0,2);	
	while(!isdigit(cadena[ind]))	ind++;
	cadena=cadena.substr(0, ind);
	
	if(cadena.length()==1)	cadena=" "+cadena;
		
	return ((char *)cadena.c_str());
	}

//**************** TOMAR NÚMERO ***********************
//Coge el numero de un radionúclido con la forma Th232 ---> 232
int tomarNumero(char *cad)
	{
	string cadena(cad);
	int ind=0;
	if(isdigit(cadena[1]))	cadena=cadena.substr(1, cadena.length()-1);	
	else					cadena=cadena.substr(2, cadena.length()-2);	
	//printf("La cadena del número es %s\n", cad);
	return atoi((char *)cadena.c_str());
	}*/
	
//***************** TOMAR NOMBRE *******************	
//Coge el nombre de un radionúclido con la forma Th232 o 232Th	---> Th
char *tomarNombre(char *cad)
	{
	string cadena(cad);
	int ind=0;

	if(!isdigit(cadena[0]))	//Formato nombreNumero
		{		
		while(!isdigit(cadena[ind]))	ind++;
		cadena=cadena.substr(0, ind);
		if(cadena.length()==1)	cadena=" "+cadena;
		return ((char *)cadena.c_str());
		}
	else			//Formato numeroNombre
		{
		while(isdigit(cadena[ind]))		ind++;
		cadena=cadena.substr(ind);
		if(cadena.length()==1)	cadena=" "+cadena;
		return ((char *)cadena.c_str());
		}
	}

//**************** TOMAR NÚMERO ***********************
//Coge el numero de un radionúclido con la forma Th232 o 232Th---> 232
int tomarNumero(char *cad)
	{
	string cadena(cad);
	int ind=0;
		
	if(!isdigit(cadena[0]))	//Formato nombreNumero
		{
		if(isdigit(cadena[1]))	cadena=cadena.substr(1, cadena.length()-1);	
		else					cadena=cadena.substr(2, cadena.length()-2);	
		//printf("La cadena del número es %s\n", cad);
		return atoi((char *)cadena.c_str());
		}
	else					//Formato numeroNombre
		{
		while(isdigit(ind))	ind++;
		cadena=cadena.substr(0,ind);
		return atoi((char *)cadena.c_str());
		}
	}
	
//*************** LEER INCERTIDUMBRE ****************
/* Devuelve el valor double contenido entre paréntesis en la posición i,j del 
	fichero tabulado llamado "nombre"
*/	
double leerIncertidumbre(char *nombre, int fila, int columna)
	{
	char buffer[1024];
	fstream file(nombre, ios::in);
	int ind=0;
		
	for(int i=0;i<fila;i++)		file.getline (buffer,1024);
	
	//Cogemos la fila que queríamos
	file.getline (buffer,1024);
	string cadena(buffer);
	
	//Quitamos las columnas anteriores
	for(int j=0;j<columna;j++)
		{
		ind=cadena.find("\t");
		cadena=cadena.substr(ind+1, cadena.length()-ind-1);
		for(int k=0;k<10;k++)
			{
			if(cadena[0]=='\t')	cadena=cadena.substr(1, cadena.length()-1);
			else 				break;
			}
		}
	//Cogemos lo que hay entre paréntesis
	int inf=cadena.find("(");
//	if(inf==string::npos)	
	if(inf==-1)	
		{
		if(cadena[0]=='-')	{ file.close(); return 0;}
		else
			{
			printf("ERROR: Formato de fichero incorrecto -> (\n"); 
			file.close();
			return -1;
			}
		}
	int sup=cadena.find(")");
//	if(sup==string::npos)	
	if(sup==-1)	
		{
		printf("ERROR: Formato de fichero incorrecto -> )\n"); 
		file.close();
		return -1;
		}
	cadena=cadena.substr(inf+1, sup-1);
		
	//Queda quitar las columnas posteriores
	ind=cadena.find("\t");
	cadena=cadena.substr(0,ind);
	file.close();
	return (atof((char *)cadena.c_str()));
	}
	


//********************* INICIA BOTONES ***********************
	/*	
	Configura los botones de cómputo en función del fichero que se cargue
	*/
void iniciaBotones()
	{
	printf("Inicia action_activity a %d\n", EstadoTrabajo.AnalisisEficiencia );
    //XtSetSensitive(action_activity_analisis, (EstadoTrabajo.AnalisisEficiencia && EstadoTrabajo.AnalisisLD) );
	printf("Vamos a poner los botones a %d %d %d\n", EstadoTrabajo.AnalisisEficiencia, EstadoTrabajo.AnalisisLD, EstadoTrabajo.AnalisisActividad); 
	if(EstadoTrabajo.AnalisisEficiencia)
		{
		//XtSetSensitive(action_activity_analisis, true);
		printf("Llamamos a ActivaActividad\n");
		ActivaActividad();
		}
	printf("Fin de inicia botones\n");
	}

//********************* DESPLEGAR LISTA **********************
void desplegarLista(Widget lista, int nPos, NodoPicos *intervalo, string cad, bool seleccionar)
	{
	char cadena[60];
	XmString entry;

//	printf(" DESPLEGAR LISTA en posición %d\n", nPos);
//	printf("Abrir sublista con %d elementos\n", intervalo->picos.numElementos);
	
	cad="-" +cad.substr(1,cad.length());
	sprintf(cadena, "%s", (char *)cad.c_str());
	entry=XmStringCreateLocalized(cadena);
	XmListDeletePos(lista, nPos);
	XmListAddItem(lista, entry, nPos);
	XmStringFree(entry);

	for(int i=(intervalo->picos.numElementos)-1;i>=0;i--)//NOTA: No sé por qué lo hice hacia atrás
		{//Desplegamos cada elemento contenido en el intervalo
		NodoPico *PicoAnalisis = DevuelveNodo(intervalo->picos, intervalo->picos.numElementos-i);
		char extra[100];
		XmString elemento, energia; 
		char texto_canales[50];
		
		//NOTA: Tenemos que añadir a lista análisis tantos nodos como nuevos picos 
		//		desplegados haya.
		NodoAnalisis *nodo = DevuelveNodo(listaAnalisis, nPos);
		
	//	printf("El pico %d tiene identificado a %d\n", i, PicoAnalisis->Ajuste.identificado);
		XmString total;
		total=XmStringCreate("", "BoldNormal");						
		if(PicoAnalisis->Ajuste.identificado)
			{
			XmString elemento;
			
//			printf("Este pico tiene %d identificaciones\n", PicoAnalisis->Ajuste.emisiones->numElements());
			//bucle de emisiones
			for(int j=0;j<PicoAnalisis->Ajuste.emisiones->numElements();j++)
				{
			//	printf("Tomamos el número de emisión %d\n", j);
				NodoEmision *ne=(NodoEmision *)(PicoAnalisis->Ajuste.emisiones->getNode(j));
			//	printf("Tomado\n");
				int codEmision = ne->codElemento;
						
				if(codEmision>0)
					{
					if (LaraBD.GetIsRunning()) 
						{
						elemento = XmStringNameElement(LaraBD.GetEmissor(codEmision), "SubIndiceBold", "BoldNormal");
						
						// minimize decimals of probability
						char format[50];
						if(j!=PicoAnalisis->Ajuste.emisiones->numElements()-1)	strcpy(format, "  %.3f keV (%.1f%%), ");
						else	strcpy(format, "  %.3f keV (%.1f%%)");
						double Prob = LaraBD.GetProbability(codEmision);
				
						int rateProb = orden(Prob)-1;
						if ((Prob > 0)&&((rateProb < -1)&&(rateProb > -10)))	
							format[14] = (unsigned char) 0x30 + abs(rateProb);
						sprintf(extra, format, ne->Energia, LaraBD.GetProbability(codEmision));
						} 
					else 	
						{
						elemento = XmStringCreate ("???", "BoldNormal");
						// minimize decimals of probability
						char *format;
						if(j!=PicoAnalisis->Ajuste.emisiones->numElements()-1)	format = "  %.3f keV (??%%), ";
						else	format = "  %.3f keV (??%%)";
						sprintf(extra, format, ne->Energia);
						delete [] format;
						}
					}
				else
					{
					if(codEmision==-1)//Es una emisión que no está en lara, sólo en naturlib.db
						{
						printf("EMISIÓN QUE NO ESTÁ EN LARA: |%s|, %f, %f\n", ne->nombre, ne->probabilidad, ne->Energia);
						char format[50];
						if(j!=PicoAnalisis->Ajuste.emisiones->numElements()-1)	strcpy(format, "  %.3f keV (%.4f%%), ");
						else	strcpy(format, "  %.3f keV (%.4f%%)");
						elemento=XmStringCreate(ne->nombre, "BoldNormal");
						//felipe. Determina probabilidad si no está en Lara.
						string cad(BDIdentificaciones);						ne->probabilidad = determinarProbabilidad(ne->Energia,(char *)cad.c_str());
						//
 						sprintf(extra, format, ne->Energia, ne->probabilidad);						printf("EMISIÓN QUE NO ESTÁ EN LARA-de nuevo: |%s|, %f, %f\n", ne->nombre, ne->probabilidad, ne->Energia);
						}
				
					else //En el caso de que se haya elegido un elemento de la FBD
						{
						elemento = XmStringCreate (buscaNombreFBD(ne->Energia, nombreFichFBD), "BoldNormal");
						// minimize decimals of probability
						char *format;
						if(j!=PicoAnalisis->Ajuste.emisiones->numElements()-1)	format = "  %.3f keV, ";
						else	format = "  %.3f keV";
						sprintf(extra, format, ne->Energia);
						delete [] format; format=NULL;
						}	
					}
				//printf("Tenemos creada la emisión\n");
				XmString emision = XmStringCreate (extra, "BoldNormal");
				// concat element and emision info
				//printf("Creamos mezcla elemento-emision\n");
				energia = XmStringConcat(elemento, emision);
				total = XmStringConcat(total, energia);
				XmStringFree(elemento); XmStringFree (emision);
				}//for(emisiones)
		
	
				XmString espacios=XmStringCreateLocalized("   ");
				XmString final=XmStringConcat(espacios, total);
//				printf("Añadimos a la base de datos el %d en %d\n", i, nPos+(intervalo->picos.numElementos-i));
				XmListAddItemUnselected(lista, final, nPos+(intervalo->picos.numElementos-i));
				XmStringFree (energia); 
				XmStringFree (final);
				XmStringFree (total); 
				XmStringFree (espacios);
			}//if(identificado)
		else //Si no está identificado mostramos la información normal
			{
			sprintf(texto_canales, "   Peak %d", intervalo->picos.numElementos-i);
			XmString cadena = XmStringCreateLocalized (texto_canales);
			XmListAddItemUnselected(lista, cadena, nPos+(intervalo->picos.numElementos-i));
			XmStringFree (cadena);
			}
		}//for (num elementos)

	if(seleccionar)	XmListSelectPos(lista, nPos, true);//NOTA: Eliminado por el tema de que hace Zoom
										//En ese caso al desplegar lo hace mal, habrá que pasarle un parámetro
	
	int top=0, num=0, bottom=0;
	XtVaGetValues(lista,XmNtopItemPosition, &top, XmNvisibleItemCount, &num, NULL);
	bottom=top+num;
	if(bottom<nPos+intervalo->picos.numElementos)	
		XmListSetBottomPos(lista, nPos+intervalo->picos.numElementos);
	}
	
//******************** PULSA EDIT RESULT ANÁLISIS ***********************
// Se activa cuando se pulsa el botón "Edit" de la interfaz de análisis, o 
// cuando se hace un doble click sobre un elemento de la lista.
// NOTA: Cambiándolo para que, en el caso de que haya más de un pico en ese
//		intervalo, despliegue una lista con todos
// NOTA: Hay que cambiarlo para que:
//		-En caso de un espectro normal, permita elegir más de una emisión para
//		el pico.
void pulsa_edit_result_analisis(Widget widget, XtPointer client_data, XtPointer call_data) 
	{
	printf("PULSA EDIT RESULT ANÁLISIS\n");
	//printf("El fichero de FBD es %s\n", nombreFichFBD);
	// busca el elemento seleccionado
	int *posiciones, numSel, nPos = 0;
	if (!XmListGetSelectedPos(result_lista_analisis, &posiciones, &numSel)) 
		{
		//Aviso(widget, "You must select an interval to edit.");
		return;
		}

	nPos = *posiciones;
	XtFree((char *)posiciones);
	// Extraño error, mas de un pico seleccionado
	if (numSel > 1) 
		{
		Aviso(widget, "Too much intervals selected.");
		printf(ERROR_ANALYSIS);
		printf(ERROR_TOO_INTERVALS);
		return;
		}
	
	int interv=0, pic=0;
	determinaPico(result_lista_analisis, nPos, &interv, &pic);

	// Rellena los campos con los datos actuales
	char texto[40];
	XmString cadena, s1, s2;
	NodoAnalisis *nodo = DevuelveNodo(listaAnalisis, nPos);
	
	NodoPicos *intervalo = DevuelveNodo(Datos.listaPicos, interv);
	NodoPico *pico = DevuelveNodo(intervalo->picos, pic);
	printf("Interv %d y pic %d\n", interv, pic);
		
	if(intervalo->picos.numElementos>1 && pic==0)
		{
		char cadena[60];
		XmString entry;
		String selection;
		
	  	XmListCallbackStruct *cbs = (XmListCallbackStruct *) call_data;
		XmStringGetLtoR(cbs->item, XmSTRING_DEFAULT_CHARSET, &selection);
		printf("Pinchamos en multiplete\n");
		if(selection!=NULL)
			{
			string cad(selection);
			if(cad[0]=='+')
				{
				desplegarLista(result_lista_analisis, nPos, intervalo, cad, true);
				}//if (+)
			else //cad[0] es '-'
				{
				cad="+" +cad.substr(1,cad.length());
				sprintf(cadena, "%s", (char *)cad.c_str());
				entry=XmStringCreateLocalized(cadena);
				XmListDeletePos(result_lista_analisis, nPos);
				XmListAddItem(result_lista_analisis, entry, nPos);
				for(int i=0;i<intervalo->picos.numElementos;i++)
					XmListDeletePos(result_lista_analisis, nPos+1);
				printf("Seleccionamos la posicion (al cerrar lista) %d\n", nPos);
				XmListSelectPos(result_lista_analisis, nPos, false);//nuevo
				
				XmStringFree(entry);
				}
			}
		if(selection!=NULL)	XtFree(selection);
		return;	
		}
	//
	printf("Vamos a crear el cuadro de diálogo\n");
	// una vez identificado el nodo se lanza el cuadro de dialogo
	if (!dlg_edAnal) 	CreateDialogAnalisis();
	printf("Hemos creado el cuadro de diálogo\n");
	Datos.ActualizaIntervaloAnalisis(interv);
	// Actualiza los datos de los picos en la grafica
	printf("Pulsa Edit Result Calibración llama a CAAG\n");
	int nError = CopiaAjustesAGrafica();
	if (nError < 0) 
		{
		Aviso(fondo_analisis, "No Memory Avalaible");
		printf(ERROR_ANALYSIS);
		printf(ERROR_NO_MEMORY);
		return;
		}
	
	ActivaAjustesGrafica(EstadoVisual.AjusteCalibracion);
	
	// escribe ChiSq
	wchar_t test= '\u2070';
	 if (!setlocale(LC_ALL, "")) 
		{
    	printf("Couldn't set locale()");
	    }	
  	//printf("------------- 0x03B1 es: |%lc|\n", 0x03B1);		
	printf("ESCRIBIMOS SUPERINDICES: |%lc|\n", test);
	s1 = XmStringCreate ("Chi\262:", "BoldNormal");
	sprintf(texto, " %.5f", intervalo->ChiSq);
	s2 = XmStringCreateLocalized(texto);
	cadena = XmStringConcat (s1, s2);
	XmStringFree(s1); XmStringFree(s2);
	XtVaSetValues (lbChiSq_adjust_edAnal, XmNlabelString, cadena, NULL);
	XmStringFree(cadena);
	
	// escribe Area
	s1 = XmStringCreate ("Area:", "BoldNormal");
	char numeroPresentado[80];
//	sprintf(texto, " %.2f (%.2f)", pico->Ajuste.area, pico->Ajuste.incerArea);
	printf("Área: %f(%f)\n", pico->Ajuste.area, pico->Ajuste.incerArea);
	PresentaNumero(pico->Ajuste.area, pico->Ajuste.incerArea,numeroPresentado);
	sprintf(texto, " %s", numeroPresentado);
	s2 = XmStringCreateLocalized(texto);
	cadena = XmStringConcat (s1, s2);
	XmStringFree(s1); XmStringFree(s2);
	XtVaSetValues (lbArea_adjust_edAnal, XmNlabelString, cadena, NULL);
	XmStringFree(cadena);

	// Escribe los parametros de la gaussiana
	sprintf(texto, "%.2f", redondeaDoubleF(pico->Ajuste.Centroide,2));
	XmTextSetString(txtCen_adjust_edAnal, texto);
	
	//wchar_t *wcs="\307";
	//XmTextSetStringWcs(txtCen_adjust_edAnal, wcs);
	//XmTextSetStringWcs(txtCen_adjust_edAnal, L"ho\307la");
	
	sprintf(texto, "%.2fkeV", redondeaDoubleF(Datos.DaEnergia(pico->Ajuste.Centroide),2));
	cadena = XmStringCreateLocalized (texto);
	XtVaSetValues (lbCenEn_adjust_edAnal, XmNlabelString, cadena, NULL);
	XmStringFree (cadena);
	sprintf(texto, "%.2f", redondeaDoubleF(pico->Ajuste.Anchura,2));
	XmTextSetString(txtSigma_adjust_edAnal, texto);
	sprintf(texto, "%.2f keV", redondeaDoubleF(Datos.DaAncho(pico->Ajuste.Centroide, pico->Ajuste.Anchura),2) );
	cadena = XmStringCreateLocalized (texto);
	XtVaSetValues (lbSigmaEn_adjust_edAnal, XmNlabelString, cadena, NULL);
	XmStringFree (cadena);
	
	// Escribe los parametros de la primera exponencial
	sprintf(texto, "%.2f", redondeaDoubleF(pico->Ajuste.Tau,2));
	XmTextSetString(txtTau_adjust_edAnal, texto);
	sprintf(texto, "%.2f keV", redondeaDoubleF(Datos.DaAncho(pico->Ajuste.Centroide, pico->Ajuste.Tau),2));
	cadena = XmStringCreateLocalized (texto);
	XtVaSetValues (lbTauEn_adjust_edAnal, XmNlabelString, cadena, NULL);
	XmStringFree (cadena);
	sprintf(texto, "%.2f", redondeaDoubleF(pico->Ajuste.Ytau,2));
	XmTextSetString(txtYtau_adjust_edAnal, texto);

	AjusteSeguridadAnal = pico->Ajuste;
	numIntervaloEdicionAnal = interv;
	if(pic!=0)	numPicoEdicionAnal = pic;
	else		numPicoEdicionAnal = 1;
		
	XmListDeleteAllItems(listEnergy);
	XmListDeleteAllItems(listElement);

	if (pico->Ajuste.identificado && pico->Ajuste.emisiones->numElements()!=0)
		{
		for(int i=0;i<pico->Ajuste.emisiones->numElements();i++)
			{
			NodoEmision *ne=(NodoEmision *)pico->Ajuste.emisiones->getNode(i);
			printf("código del elemento a editar: %d\n", ne->codElemento);
			if(ne->codElemento==-1)	
				{
				//printf("ELEMENTO DESCONOCIDO EN LARA!!!!--<-<-<<-<-\n");
				char *e=new char[10];
				sprintf(e, "%.3f", ne->Energia);
				strcat(e, " keV");		
				ActualizaIdentificacionAnalisis(ne->nombre, e);
				}
			else	ActualizaIdentificacionAnalisis(ne->codElemento);
			}//NOTA: No sé cómo lo hará, supongo que la buena actualización será la última
		}
	else 
		{
//		if(pico->Ajuste.identificado && pico->Ajuste.codElemento==0 && Datos.esFondo)
		if(pico->Ajuste.identificado && pico->Ajuste.emisiones->emptyList() && Datos.esFondo)
			{ //En este caso la identificación vino de la BD de fondo y no de Lara
			//Tomamos los valores de la FBD
			char *emisFBD;
			emisFBD=buscaEmisionFBD(interv, pic, nombreFichFBD);
			sprintf(texto, "%s", emisFBD);
//			printf("El texto es %s\n", emisFBD);
			string nombre(texto);
			string energia(texto);

			nombre=nombre.substr(0,nombre.find(" -> "));		
			energia=energia.substr(energia.find(" -> ")+4, energia.find(" keV"));		
			ActualizaIdentificacionAnalisis((char *)nombre.c_str(), (char *)energia.c_str());
			delete emisFBD;
			}
		else 
			{
			// Pone los campos en vacio
			cadena = XmStringCreateLocalized (" ");
			XmListDeleteAllItems(listEnergy); // clean list widget
			XmListDeleteAllItems(listElement); // clean list widget

			XmStringFree (cadena);
			}
		}
	// Saca los candidatos de ese pico
	printf("Show candidates %d,%d\n", interv, pic);
	AnalIdent.ShowCandidates(interv, pic);
	
	//Si es un pico de fondo, saca el candidato de la base de datos de fondo
	if(Datos.esFondo)
		{
		char *emisFBD;
		emisFBD=buscaEmisionFBD(interv, pic, nombreFichFBD);
		sprintf(texto, "%s", emisFBD);
		if(!strcmp(emisFBD, "No matching emission"))
			//Deshabilitamos el botón de selección del texto
			XtSetSensitive(bt_FDB_identifica, false);				
		else	//lo habilitamos por si estuviera deshabilitado
			XtSetSensitive(bt_FDB_identifica, true);				
		XmTextSetString(txt_FDB_nombre, texto);
		delete emisFBD;
		}

	XtManageChild(dlg_edAnal);

	//Dejamos el foco en la lista de picos ajustados->no funciona, se va al edit
	//XmProcessTraversal(result_lista_analisis, XmTRAVERSE_CURRENT);
	//XmProcessTraversal(result_lista_analisis, XmTRAVERSE_CURRENT);
	}

//****************** PULSA ADD RESULT ANÁLISIS *****************************
/*	Añadido con posterioridad a del y edit
	Permite añadir un pico entorno a la localización del pico seleccionado.
	El programa realizará el ajuste teniendo en cuenta una gaussiana más, 
	pudiendo llegar a convertir picos aislados en multipletes o aumentando el
	número de picos contemplados en otros multipletes.
	Genera un cuadro de diálogo en el que podemos añadir un pico bien directamente
	dando la energía del centroide que consideramos (NOTA: debemos introducirla
	bien o comprobar que está dentro de los límites del intervalo)
	El algoritmo de ajuste puede que decida que el nuevo pico no sirva para 
	nada y no lo llegue a añadir, o puede que el ajuste cambie y lo tenga en
	cuenta. En todo caso, esta adición se hará sin tener en cuenta restricciones
	*/
void pulsa_add_result_analisis(Widget widget, XtPointer client_data, XtPointer call_data) 
	{
	printf("PULSA ADD RESULT ANÁLISIS\n");
	printf("Calculando la incertidumbre máxima\n");
	//Calculillos para estándares, a boorar luego
	cSearchLaraBD founds=LaraBD.SearchEmissions(-1,-1,1.0,-1,-1,92,92,238,238,false,false,false,false,
								NULL,false, false);
	int numFounds=founds.GetNumItemsFound();
	float maxInc, minInc;
	maxInc=0.0;
	minInc=100.0;
	float porcentaje=0;
	int codigo=0;
	printf("Número de emisiones en lara: %d\n", numFounds);
	for(int i=1;i<numFounds; i++)
		{
		codigo=founds.GetNextResult();
		
		porcentaje=100*(LaraBD.GetIncProbability(codigo)/LaraBD.GetProbability(codigo));
		if(!LaraBD.IsEqui(LaraBD.GetEmissor(codigo)))
			{
			if(porcentaje>maxInc)	
				{
				printf("Cambiando porcentaje máximo a %f/%f*100\n", LaraBD.GetIncProbability(codigo), LaraBD.GetProbability(codigo));
				printf("Es %d,%d a %f\n", LaraBD.GetAtomicNumber(LaraBD.GetEmissor(codigo)), LaraBD.GetMassNumber(LaraBD.GetEmissor(codigo)), LaraBD.GetEnergy(codigo)); 
				maxInc=porcentaje;
				}
			if(porcentaje<minInc)	
				{
				minInc=porcentaje;
				printf("Cambiando porcentaje mínimo a %f/%f*100\n", LaraBD.GetIncProbability(codigo), LaraBD.GetProbability(codigo));
				}			
			}
		}
	printf("La incertidumbre está en [%f,%f]\n", minInc, maxInc);
	//printf("El fichero de FBD es %s\n", nombreFichFBD);
	// busca el elemento seleccionado
	int *posiciones, numSel, nPos = 0;
	if (!XmListGetSelectedPos(result_lista_analisis, &posiciones, &numSel)) 
		{
		//Aviso(widget, "You must select an interval to edit.");
		return;
		}

	nPos = *posiciones;
	XtFree((char *)posiciones);
	// Extraño error, mas de un pico seleccionado
	if (numSel > 1) 
		{
		Aviso(widget, "Too much intervals selected.");
		printf(ERROR_ANALYSIS);
		printf(ERROR_TOO_INTERVALS);
		return;
		}
	
	int interv=0, pic=0;
	determinaPico(result_lista_analisis, nPos, &interv, &pic);

	// Rellena los campos con los datos actuales
	char texto[40];
	XmString cadena, s1, s2;
	NodoAnalisis *nodo = DevuelveNodo(listaAnalisis, nPos);
	
	NodoPicos *intervalo = DevuelveNodo(Datos.listaPicos, interv);
	NodoPico *pico = DevuelveNodo(intervalo->picos, pic);
	printf("Interv %d y pic %d\n", interv, pic);
	if(pic==0)	pic=1;
		
	if(intervalo->picos.numElementos>1 && pic==0)
		{
		char cadena[60];
		XmString entry;
		String selection;
	
	  	XmListCallbackStruct *cbs = (XmListCallbackStruct *) call_data;
		XmStringGetLtoR(cbs->item, XmSTRING_DEFAULT_CHARSET, &selection);
	
		if(selection!=NULL)
			{
			string cad(selection);
			if(cad[0]=='+')
				{
				desplegarLista(result_lista_analisis, nPos, intervalo, cad, true);
			
				//imprimimos cómo queda la lista
				int numItems=0;
				XmStringTable items=NULL;
				char *text=NULL;
				XtVaGetValues(result_lista_analisis, XmNitems, &items, XmNitemCount, &numItems, NULL);
				printf("Tomada la información del widget\n");
				for(int i=0;i<numItems;i++)
					{
					XmStringGetLtoR(items[i], XmFONTLIST_DEFAULT_TAG, &text);
					printf("|%s|\n", text);
					if(text!=NULL)	{XtFree(text);text=NULL;}
					}
				//
				}//if (+)
			else //cad[0] es '-'
				{
				cad="+" +cad.substr(1,cad.length());
				sprintf(cadena, "%s", (char *)cad.c_str());
				entry=XmStringCreateLocalized(cadena);
				XmListDeletePos(result_lista_analisis, nPos);
				XmListAddItem(result_lista_analisis, entry, nPos);
				for(int i=0;i<intervalo->picos.numElementos;i++)
					XmListDeletePos(result_lista_analisis, nPos+1);
				printf("Seleccionamos la posicion (al cerrar lista) %d\n", nPos);
				XmListSelectPos(result_lista_analisis, nPos, false);//nuevo
				
				XmStringFree(entry);
				}
			}
		if(selection!=NULL)	XtFree(selection);
		return;	
		}
	
	// una vez identificado el nodo se lanza el cuadro de dialogo
	if (!dlg_adAnal) 	CreateDialogAdicion();

	Datos.ActualizaIntervaloAnalisis(interv);
	
	// Escribe los parametros de la gaussiana (Centroide)
	//NOTA: Igual es mejor no poner nada
/*	printf("Ponemos el texto del centroide\n");
	sprintf(texto, "%.2f", pico->Ajuste.Centroide);
	XmTextSetString(txt_cent_adAnal, texto);*/
	printf("Ponemos el texto del centroide\n");
	sprintf(texto, "");
	XmTextSetString(txt_cent_adAnal, texto);
	
	AjusteSeguridadAnal = pico->Ajuste;
	numIntervaloEdicionAnal = interv;//Usamos las mismas variables que para la edición
	if(pic!=0)	numPicoEdicionAnal = pic;
	else		numPicoEdicionAnal = 1;
	
	XmListDeleteAllItems(listEnergy_adAnal);
	// Saca los candidatos de ese pico
//	AnalIdent.ShowCandidates(interv, pic);
	
	XtManageChild(dlg_adAnal);
	
	
	}
	
//******************* BUSCA EMISION FBD ********************
char *buscaEmisionFBD(int numInterval, int numPeak, char *nombre)
	{
	printf("BUSCA EMISION FBD\n");
	//Cogemos el intervalo de picos y dentro de ese intervalo el pico a analizar
	NodoPicos *interval = DevuelveNodo(Datos.listaPicos, numInterval);
	NodoPico *peak = DevuelveNodo(interval->picos, numPeak);
	char *buffer=new char[1024];
	if(buffer==NULL)
		{
		printf("ERROR: No hay memoria para reservar char[1024]\n");
		return NULL;
		}		
		
	double minEnergia = Datos.DaEnergia((double)interval->limInferior);
	double maxEnergia = Datos.DaEnergia((double)interval->limSuperior)+1;
	double ef;
	int ind;
		
	int numEnergias=numeroFilas(nombre);	

	for(int i=0;i<numEnergias;i++)
		{
		ef=leerElemento(nombre, i, 0);
		//Si la energía está en alguno de los límites, la devolvemos
		if(ef>=minEnergia && ef<=maxEnergia)	
			{
			sprintf(buffer, "%s -> %.3f keV", leerElementoCadena(nombre, i, 1), ef);		
			return buffer;
			}
		}
	return "No matching emission";
	}

	
/******************************************************************************
 ******
 ******                            EDICION ANÁLISIS
 ******
 ******************************************************************************/

#include "Ecuation.xbm"
#include "HelpIcon.xbm"

	//********************** CREATE DIALOG ANÁLISIS **********************
/* Crea todo el cuadro de diálogo de la análisis, con 3 secciones:
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
void CreateDialogAnalisis() 
	{
   	Arg		args[7];
   	Cardinal	i = 0;
	XmString cadena;
	
	// *********************
	// * cuadro de dialogo *
	// *********************
	i = 0;
	printf("Empezamos a crear\n");
	XtSetArg (args[i], XmNtitle, ANALYSIS_EDIT_TITLE); i++;
	XtSetArg (args[i], XmNmwmDecorations, MWM_DECOR_BORDER|MWM_DECOR_TITLE|MWM_DECOR_MENU); i++;
	XtSetArg (args[i], XmNmwmFunctions, MWM_FUNC_MOVE); i++;
	dlg_edAnal = XmCreateFormDialog(app_shell, "dlg_edAnal", args, i);

	/*******************
	El cuadro de diálogo se separa en tres zonas básicas:
		-fr_adjust_edAnal: Datos del ajuste, con la gráfica y la fórmula para guiar
		-fr_energy_edAnal: Datos de la energía y elementos químicos asociados
		-fm_botones_edAnal: Botones de ok, cancel y adjust
	Todos tendrán como padre a dlg_edAnal, el cuadro de diálogo.
	*******************/
	printf("Creamos secciones\n");
	i = 0;
	fr_adjust_edAnal = XmCreateFrame (dlg_edAnal, "fr_adjust_edAnal", args, i);
	fr_energy_edAnal = XmCreateFrame (dlg_edAnal, "fr_energy_edAnal", args, i);
	linea_edAnal = XmCreateSeparator(dlg_edAnal, "linea_edAnal", args, i);
	i = 0;
	XtSetArg (args[i], XmNfractionBase, 3); i++;
	fm_botones_edAnal = XmCreateForm (dlg_edAnal, "fm_botones_edAnal", args, i);	
   	XtVaSetValues (fm_botones_edAnal,
		XmNbottomAttachment, XmATTACH_FORM,
		XmNbottomOffset, 10,
		XmNrightAttachment, XmATTACH_FORM,
		XmNrightOffset, 10,
		XmNleftAttachment, XmATTACH_FORM,
		XmNleftOffset, 10,
		NULL);
   	XtVaSetValues (linea_edAnal,
		XmNbottomAttachment, XmATTACH_WIDGET, XmNbottomWidget, fm_botones_edAnal,
		XmNbottomOffset, 10,
		XmNrightAttachment, XmATTACH_FORM,
		XmNleftAttachment, XmATTACH_FORM,
		NULL);
   	XtVaSetValues (fr_energy_edAnal,
		XmNbottomAttachment, XmATTACH_WIDGET, XmNbottomWidget, linea_edAnal,
		XmNbottomOffset, 10,
		XmNrightAttachment, XmATTACH_FORM,
		XmNrightOffset, 10,
		XmNleftAttachment, XmATTACH_FORM,
		XmNleftOffset, 10,
		NULL);
   	XtVaSetValues (fr_adjust_edAnal,
		XmNbottomAttachment, XmATTACH_WIDGET, XmNbottomWidget, fr_energy_edAnal,
		XmNtopAttachment, XmATTACH_FORM,
		XmNtopOffset, 10,
		XmNrightAttachment, XmATTACH_FORM,
		XmNrightOffset, 10,
		XmNleftAttachment, XmATTACH_FORM,
		XmNleftOffset, 10,
		NULL);
	XtManageChild(fr_adjust_edAnal);
	XtManageChild(fr_energy_edAnal);
	XtManageChild(linea_edAnal);
	XtManageChild(fm_botones_edAnal);
	
	printf("seccion de ajuste\n");
	// *********************
	// * seccion de ajuste *
	// *********************
	cadena = XmStringCreate ("Fit Data", "BoldNormal");
   	i = 0;
   	XtSetArg (args[i], XmNchildType, XmFRAME_TITLE_CHILD); i++;
   	XtSetArg (args[i], XmNchildVerticalAlignment, XmALIGNMENT_CENTER); i++;
 //  	XtSetArg (args[i], XmNfontList, lista_fuentes); i++;
   	XtSetArg (args[i], XmNlabelString, cadena); i++;
	lb_adjust_edAnal = XmCreateLabel (fr_adjust_edAnal, "lb_adjust_edAnal", args, i);
	XmStringFree (cadena);
	XtManageChild(lb_adjust_edAnal);
	i = 0;
	fm_adjust_edAnal = XmCreateForm(fr_adjust_edAnal, "fm_adjust_edAnal", args, i);

	printf("centroide\n");
	// indicadores de Gaussiana
	//*** Columnas para colocación
	i = 0;
	XtSetArg (args[i], XmNorientation, XmVERTICAL); i++;
	rw1_adjust_edAnal = XmCreateRowColumn (fm_adjust_edAnal, "rw1_adjust_edAnal", args, i);
	
	i = 0;
	XtSetArg (args[i], XmNorientation, XmVERTICAL); i++;
	rw2_adjust_edAnal = XmCreateRowColumn (fm_adjust_edAnal, "rw2_adjust_edAnal", args, i);

	i=0;
	XtSetArg (args[i], XmNorientation, XmVERTICAL); i++;
	rw3_adjust_edAnal = XmCreateRowColumn (fm_adjust_edAnal, "rw3_adjust_edAnal", args, i);

	i=0;
	XtSetArg (args[i], XmNorientation, XmVERTICAL); i++;
	rw4_adjust_edAnal = XmCreateRowColumn (fm_adjust_edAnal, "rw4_adjust_edAnal", args, i);

	//******** CENTROIDE
	cadena = XmStringCreate ("Centroid", "BoldNormal");
   	i = 0;
   	XtSetArg (args[i], XmNlabelString, cadena); i++;
   	lbCen_adjust_edAnal = XmCreateLabel (rw1_adjust_edAnal, "lbCen_adjust_edAnal", args, i);
   	XmStringFree (cadena);
	XtManageChild(lbCen_adjust_edAnal);
	i = 0;
   	XtSetArg (args[i], XmNcolumns, 10); i++;
   	XtSetArg (args[i], XmNmaxLength, 20); i++;
	txtCen_adjust_edAnal = XmCreateTextField(rw1_adjust_edAnal, "txtCen_adjust_edAnal", args, i);
   	XtAddCallback(txtCen_adjust_edAnal, XmNfocusCallback, seleccionarTexto, NULL);   
	XtManageChild(txtCen_adjust_edAnal);
	cadena = XmStringCreateLocalized ("");
   	i = 0;
   	XtSetArg (args[i], XmNlabelString, cadena); i++;
   	XtSetArg (args[i], XmNalignment, XmALIGNMENT_END); i++;
   	lbCenEn_adjust_edAnal = XmCreateLabel (rw1_adjust_edAnal, "lbCenEn_adjust_edAnal", args, i);
   	XmStringFree (cadena);
	XtManageChild(lbCenEn_adjust_edAnal); // No se coloca a la derecha porque sería necesario ponerlo en una base form
	
	//******* Height
	cadena = XmStringCreateLocalized ("Height");
	i = 0;
   	XtSetArg (args[i], XmNlabelString, cadena); i++;
   	lbYtau_adjust_edAnal = XmCreateLabel (rw2_adjust_edAnal, "lbYtau_adjust_edAnal", args, i);
   	XmStringFree (cadena);
	XtManageChild(lbYtau_adjust_edAnal);

	i = 0;
   	XtSetArg (args[i], XmNcolumns, 10); i++;
   	XtSetArg (args[i], XmNmaxLength, 10); i++;
	txtYtau_adjust_edAnal = XmCreateTextField(rw2_adjust_edAnal, "txtYtau_adjust_edAnal", args, i);
	XtAddCallback(txtYtau_adjust_edAnal, XmNfocusCallback, seleccionarTexto, NULL);   
	XtManageChild(txtYtau_adjust_edAnal);

	//*** Sigma
	cadena = XmStringCreate("Sigma", "BoldNormal");
   i = 0;
   	XtSetArg (args[i], XmNlabelString, cadena); i++;
   	lbSigma_adjust_edAnal = XmCreateLabel (rw3_adjust_edAnal, "lbSigma_adjust_edAnal", args, i);
   	XmStringFree (cadena);
	XtManageChild(lbSigma_adjust_edAnal);
   i = 0;
   	XtSetArg (args[i], XmNcolumns, 10); i++;
   	XtSetArg (args[i], XmNmaxLength, 20); i++;
   	txtSigma_adjust_edAnal = XmCreateTextField(rw3_adjust_edAnal, "txtSigma_adjust_edAnal", args, i);
	XtAddCallback(txtSigma_adjust_edAnal, XmNfocusCallback, seleccionarTexto, NULL);   
	XtManageChild(txtSigma_adjust_edAnal);
	cadena = XmStringCreateLocalized ("");
   i = 0;
   	XtSetArg (args[i], XmNlabelString, cadena); i++;
   	XtSetArg (args[i], XmNalignment, XmALIGNMENT_END); i++;
   	lbSigmaEn_adjust_edAnal = XmCreateLabel (rw3_adjust_edAnal, "lbSigmaEn_adjust_edAnal", args, i);
   	XmStringFree (cadena);
	XtManageChild(lbSigmaEn_adjust_edAnal);
	
	//****Tau
	cadena = XmStringCreateLocalized ("Tau");
	i = 0;
   	XtSetArg (args[i], XmNlabelString, cadena); i++;
   	lbTau_adjust_edAnal = XmCreateLabel (rw4_adjust_edAnal, "lbTau_adjust_edAnal", args, i);
   	XmStringFree (cadena);
	XtManageChild(lbTau_adjust_edAnal);

	i = 0;
   	XtSetArg (args[i], XmNcolumns, 10); i++;
   	XtSetArg (args[i], XmNmaxLength, 10); i++;
	txtTau_adjust_edAnal = XmCreateTextField(rw4_adjust_edAnal, "txtTau_adjust_edAnal", args, i);
	XtAddCallback(txtTau_adjust_edAnal, XmNfocusCallback, seleccionarTexto, NULL);   
	XtManageChild(txtTau_adjust_edAnal);
	cadena = XmStringCreateLocalized ("");
   	
	i = 0;
   	XtSetArg (args[i], XmNlabelString, cadena); i++;
   	XtSetArg (args[i], XmNalignment, XmALIGNMENT_END); i++;
   	lbTauEn_adjust_edAnal = XmCreateLabel (rw4_adjust_edAnal, "lbTauEn_adjust_edAnal", args, i);
   	XmStringFree (cadena);
	XtManageChild(lbTauEn_adjust_edAnal);

	//Colocación de las columnas
	XtVaSetValues (rw1_adjust_edAnal,
		XmNtopAttachment, XmATTACH_FORM,
		XmNleftAttachment, XmATTACH_FORM,
		NULL);
	XtManageChild(rw1_adjust_edAnal);

   	XtVaSetValues (rw2_adjust_edAnal,
		XmNtopAttachment, XmATTACH_FORM,
		XmNleftAttachment, XmATTACH_WIDGET, XmNleftWidget, rw1_adjust_edAnal,
		XmNleftOffset, 10,
		NULL);
	XtManageChild(rw2_adjust_edAnal);

   	XtVaSetValues (rw3_adjust_edAnal,
		XmNtopAttachment, XmATTACH_FORM,
		XmNleftAttachment, XmATTACH_WIDGET, XmNleftWidget, rw2_adjust_edAnal,
		XmNleftOffset, 10,
		NULL);
	XtManageChild(rw3_adjust_edAnal);
	
	XtVaSetValues (rw4_adjust_edAnal,
		XmNtopAttachment, XmATTACH_FORM,
		XmNleftAttachment, XmATTACH_WIDGET, XmNleftWidget, rw3_adjust_edAnal,
		XmNleftOffset, 10,
//		XmNrightAttachment, XmATTACH_FORM,
		NULL);
	XtManageChild(rw4_adjust_edAnal);
	// Dibujo de ayuda
/*	Pixel fg, bg;
	XtVaGetValues (dlg_edAnal, XmNforeground, &fg, XmNbackground, &bg, NULL);
	i = 0;
	// carga el icono de ayuda sobre parametros
	Pixmap helpIcon = XCreatePixmapFromBitmapData (dpy,
			RootWindowOfScreen(XtScreen (dlg_edAnal)),
			(char *) HelpIcon_bits, HelpIcon_width, HelpIcon_height,
			fg, bg,
			DefaultDepthOfScreen (XtScreen(dlg_edAnal)));
   	i = 0;
   	XtSetArg (args[i], XmNlabelType, XmPIXMAP); i++;
   	XtSetArg (args[i], XmNlabelPixmap, helpIcon); i++;
   	lbGraf_adjust_edAnal = XmCreateLabel (rw3_adjust_edAnal, "lbGraf_adjust_edAnal", args, i);
	XtManageChild(lbGraf_adjust_edAnal);
*/

printf("Chi cuadrado\n");
	// indicador de calidad de ajuste
	cadena = XmStringCreate ("Chi\262:", "BoldNormal");
   	i = 0;
   	XtSetArg (args[i], XmNlabelString, cadena); i++;
   	lbChiSq_adjust_edAnal = XmCreateLabel (fm_adjust_edAnal, "lbChiSq_adjust_edAnal", args, i);
 //  	lbChiSq_adjust_edAnal = XmCreateLabel (rw3_adjust_edAnal, "lbChiSq_adjust_edAnal", args, i);
   	XmStringFree (cadena);
   	XtVaSetValues (lbChiSq_adjust_edAnal,
		XmNtopAttachment, XmATTACH_WIDGET, XmNtopWidget, rw1_adjust_edAnal,
		XmNtopOffset, 10,
		XmNleftAttachment, XmATTACH_FORM, XmNleftOffset, 10, 
		//XmNbottomAttachment, XmATTACH_FORM, XmNbottomOffset, 10,
		//XmNbottomAttachment, XmATTACH_WIDGET, XmNbottomWidget, lbArea_adjust_edAnal,
		NULL);
	XtManageChild(lbChiSq_adjust_edAnal);
	
printf("Área\n");
	// indicador del área del pico
	cadena = XmStringCreate ("Area:", "BoldNormal");
   	i = 0;
   	XtSetArg (args[i], XmNlabelString, cadena); i++;
   	lbArea_adjust_edAnal = XmCreateLabel (fm_adjust_edAnal, "lbArea_adjust_edAnal", args, i);
//   	lbArea_adjust_edAnal = XmCreateLabel (rw3_adjust_edAnal, "lbArea_adjust_edAnal", args, i);
   	XmStringFree (cadena);
   	XtVaSetValues (lbArea_adjust_edAnal,
		XmNtopAttachment, XmATTACH_WIDGET, XmNtopWidget, lbChiSq_adjust_edAnal,
		XmNtopOffset, 10,
		XmNleftAttachment, XmATTACH_FORM, XmNleftOffset, 10, 
		XmNbottomAttachment, XmATTACH_FORM, XmNbottomOffset, 10,
		NULL);
	XtManageChild(lbArea_adjust_edAnal);


	//Fórmula de ayuda
	/*Pixmap formula = XCreatePixmapFromBitmapData (dpy,
			RootWindowOfScreen(XtScreen (dlg_edAnal)),
			(char *) ecuation_bits, ecuation_width, ecuation_height,
			fg, bg, DefaultDepthOfScreen (XtScreen(dlg_edAnal)));
   	i = 0;
   	XtSetArg (args[i], XmNlabelType, XmPIXMAP); i++;
   	XtSetArg (args[i], XmNlabelPixmap, formula); i++;
   	lbEcuat_adjust_edAnal = XmCreateLabel (fm_adjust_edAnal, "lbEcuat_adjust_edAnal", args, i);
	XtManageChild(lbEcuat_adjust_edAnal);
   	XtVaSetValues (lbEcuat_adjust_edAnal,
		XmNtopAttachment, XmATTACH_WIDGET, XmNtopWidget, rw1_adjust_edAnal,
		XmNtopOffset, 10,
		XmNbottomAttachment, XmATTACH_FORM, XmNbottomOffset, 10,
		XmNleftAttachment, XmATTACH_WIDGET, XmNleftWidget, lbChiSq_adjust_edAnal,
		XmNleftOffset, 20, 
		NULL);

   	XtVaSetValues (rw3_adjust_edAnal,
		XmNtopAttachment, XmATTACH_FORM, XmNtopOffset, 20,
		XmNleftAttachment, XmATTACH_WIDGET, XmNleftWidget, rw2_adjust_edAnal,
		XmNleftOffset, 10,
		XmNrightAttachment, XmATTACH_FORM,
		XmNbottomAttachment, XmATTACH_WIDGET, XmNbottomWidget, lbEcuat_adjust_edAnal,
		NULL);
*/	// ajustes del formulario contenedor
	
   	XtVaSetValues (fm_adjust_edAnal,
		XmNbottomAttachment, XmATTACH_FORM,
		XmNtopAttachment, XmATTACH_FORM,
		XmNrightAttachment, XmATTACH_FORM,
		XmNleftAttachment, XmATTACH_FORM,
		NULL);
	XtManageChild(fm_adjust_edAnal);

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
   	lb_energy_edAnal = XmCreateLabel (fr_energy_edAnal, "lb_energy_edAnal", args, i);
   	XmStringFree (cadena);
	XtManageChild(lb_energy_edAnal);
	i = 0;
	fm_energy_edAnal = XmCreateForm(fr_energy_edAnal, "fm_energy_edAnal", args, i);

	//Elemento de la base de datos de fondo, si procede
	i = 0;
	fr_FDB_identify_edAnal = XmCreateFrame (fm_energy_edAnal, "fr_FDB_identify_edAnal", args, i);
	
	cadena = XmStringCreate ("Background DB", "BoldNormal");
   	i = 0;
   	XtSetArg (args[i], XmNchildType, XmFRAME_TITLE_CHILD); i++;
   	XtSetArg (args[i], XmNchildVerticalAlignment, XmALIGNMENT_CENTER); i++;
   	XtSetArg (args[i], XmNlabelString, cadena); i++;
//   XtSetArg (args[i], XmNfontList, lista_fuentes); i++;
   	lb_FDB_identify_edAnal = XmCreateLabelGadget (fr_FDB_identify_edAnal, "lb_FDB_identify_edAnal", args, i);
   	XmStringFree (cadena);
	XtManageChild(lb_FDB_identify_edAnal);

	i = 0;
	fm_FDB_identify_edAnal = XmCreateForm(fr_FDB_identify_edAnal, "fm_FDB_identify_edAnal", args, i);

	i = 0;
   	cadena = XmStringCreateLocalized ("<=");
   	XtSetArg (args[i], XmNlabelString, cadena); i++;
	XtSetArg (args[i], XmNalignment, XmALIGNMENT_CENTER); i++;
	bt_FDB_identifica = XmCreatePushButton(fm_FDB_identify_edAnal, "bt_FDB_identifica", args, i);
   	XmStringFree (cadena);
   	XtAddCallback (bt_FDB_identifica, XmNactivateCallback, pulsa_identifica_FDB_edit_analisis, NULL);
	XtManageChild(bt_FDB_identifica);

	i = 0;
   	XtSetArg (args[i], XmNcolumns, 10); i++;
   	XtSetArg (args[i], XmNmaxLength, 20); i++;
//   XtSetArg (args[i], XmNfontList, lista_fuentes); i++;
	txt_FDB_nombre = XmCreateTextField(fm_FDB_identify_edAnal, "txt_FDB_nombre", args, i);
	XtAddCallback(txt_FDB_nombre, XmNfocusCallback, seleccionarTexto, NULL);   
	XtManageChild(txt_FDB_nombre);
   	XtAddCallback (txt_FDB_nombre, XmNvalueChangedCallback, cambia_texto_FBD, NULL);
   	XtVaSetValues (txt_FDB_nombre,
		XmNtopAttachment, XmATTACH_FORM,
		XmNbottomAttachment, XmATTACH_FORM, XmNbottomOffset, 5,
		XmNleftAttachment, XmATTACH_FORM, XmNleftOffset, 30, 
		XmNrightAttachment, XmATTACH_FORM, XmNrightOffset, 50, 
		NULL);


	// buscador de elementos
	i = 0;
	fr_DB_identify_edAnal = XmCreateFrame (fm_energy_edAnal, "fr_DB_identify_edAnal", args, i);
	cadena = XmStringCreate ("Elements DB", "BoldNormal");
   	i = 0;
   	XtSetArg (args[i], XmNchildType, XmFRAME_TITLE_CHILD); i++;
   	XtSetArg (args[i], XmNchildVerticalAlignment, XmALIGNMENT_CENTER); i++;
   	XtSetArg (args[i], XmNlabelString, cadena); i++;
//   XtSetArg (args[i], XmNfontList, lista_fuentes); i++;
   	lb_DB_identify_edAnal = XmCreateLabelGadget (fr_DB_identify_edAnal, "lb_DB_identify_edAnal", args, i);
   	XmStringFree (cadena);
	XtManageChild(lb_DB_identify_edAnal);
	// lista de seleccion
	i = 0;
	fm_DB_identify_edAnal = XmCreateForm(fr_DB_identify_edAnal, "fm_DB_identify_edAnal", args, i);
	fm_DB_identify2_edAnal = XmCreateForm(fm_DB_identify_edAnal, "fm_DB_identify2_edAnal", args, i);

	// lista de identificacion
	AnalIdent.SetPeakList(&Datos.listaPicos);
	AnalIdent.CreateWidgets(fm_DB_identify2_edAnal);
	XtManageChild(fm_DB_identify2_edAnal);
	
	// Boton de hacer identificacion
	i = 0;
   	cadena = XmStringCreateLocalized ("<=");
   	XtSetArg (args[i], XmNlabelString, cadena); i++;
	XtSetArg (args[i], XmNalignment, XmALIGNMENT_CENTER); i++;
	bt_DB_identify_edAnal = XmCreatePushButton(fm_DB_identify_edAnal, "bt_DB_identify_edAnal", args, i);
   	XmStringFree (cadena);
   	XtAddCallback (bt_DB_identify_edAnal, XmNactivateCallback, pulsa_identifica_edit_analisis, NULL);
	XtManageChild(bt_DB_identify_edAnal);
   	XtVaSetValues (bt_DB_identify_edAnal,
		XmNtopAttachment, XmATTACH_WIDGET, XmNtopWidget, fm_DB_identify2_edAnal,
		XmNbottomAttachment, XmATTACH_FORM, XmNbottomOffset, 5,
		XmNleftAttachment, XmATTACH_FORM, XmNleftOffset, 5, 
		XmNrightAttachment, XmATTACH_FORM, XmNrightOffset, 40,
		NULL);
		
	// Boton de borrar identificacion
	i = 0;
   	cadena = XmStringCreateLocalized ("X");
   	XtSetArg (args[i], XmNlabelString, cadena); i++;
	XtSetArg (args[i], XmNalignment, XmALIGNMENT_CENTER); i++;
	bt_DB_unidentify_edAnal = XmCreatePushButton(fm_DB_identify_edAnal, "bt_DB_identify_edAnal", args, i);
   	XmStringFree (cadena);
   	XtAddCallback (bt_DB_unidentify_edAnal, XmNactivateCallback, pulsa_quita_identifica_edit_analisis, NULL);
	XtManageChild(bt_DB_unidentify_edAnal);
   	XtVaSetValues (bt_DB_unidentify_edAnal,
		XmNtopAttachment, XmATTACH_WIDGET, XmNtopWidget, fm_DB_identify2_edAnal,
		XmNbottomAttachment, XmATTACH_FORM, XmNbottomOffset, 5,
		XmNleftAttachment, XmATTACH_WIDGET, XmNleftWidget, bt_DB_identify_edAnal,
		XmNrightAttachment, XmATTACH_FORM, XmNrightOffset, 5,
		NULL);
	
   	XtVaSetValues (fm_FDB_identify_edAnal,
		XmNleftAttachment, XmATTACH_FORM,
		XmNrightAttachment, XmATTACH_FORM,
		NULL);

	XtVaSetValues (fm_DB_identify2_edAnal,
		XmNleftAttachment, XmATTACH_FORM,
		XmNrightAttachment, XmATTACH_FORM,
		NULL);
	
	XtManageChild(fm_FDB_identify_edAnal);
	XtManageChild(fm_DB_identify_edAnal);

	//indicador de energia
	i = 0;
	rw_Izq_energy_edAnal = XmCreateRowColumn (fm_energy_edAnal, "rw_Izq_energy_edAnal", args, i);
	cadena = XmStringCreate ("Emission:      ", "BoldNormal");
   	i = 0;
   	XtSetArg (args[i], XmNlabelString, cadena); i++;
   	lbEmission_energy_edAnal = XmCreateLabel (rw_Izq_energy_edAnal, "lbEmission_energy_edAnal", args, i);
   	XmStringFree (cadena);
	XtManageChild(lbEmission_energy_edAnal);
	
	//Lista de energías/probabilidad
	// Create list widget
	i = 0;
	XtSetArg (args[i], XmNlistSizePolicy, XmCONSTANT); i++;
	XtSetArg (args[i], XmNselectionPolicy, XmSINGLE_SELECT); i++;
	XtSetArg (args[i], XmNvisibleItemCount, 7); i++;
	XtSetArg (args[i], XmNscrollBarDisplayPolicy, XmSTATIC); i++;
	listEnergy = XmCreateScrolledList(rw_Izq_energy_edAnal, "listEnergy", args, i);
	XtManageChild(listEnergy);
		
	cadena = XmStringCreate ("Element:", "BoldNormal");
   	i = 0;
   	XtSetArg (args[i], XmNlabelString, cadena); i++;
   	lbElement_energy_edAnal = XmCreateLabel (rw_Izq_energy_edAnal, "lbElement_energy_edAnal", args, i);
   	XmStringFree (cadena);
	XtManageChild(lbElement_energy_edAnal);
	
	i = 0;
	XtSetArg (args[i], XmNlistSizePolicy, XmCONSTANT); i++;
	XtSetArg (args[i], XmNselectionPolicy, XmSINGLE_SELECT); i++;
	XtSetArg (args[i], XmNvisibleItemCount, 7); i++;
	XtSetArg (args[i], XmNscrollBarDisplayPolicy, XmSTATIC); i++;
	listElement = XmCreateScrolledList(rw_Izq_energy_edAnal, "listElement", args, i);
	XtManageChild(listElement);

   	XtVaSetValues (fm_energy_edAnal,
		XmNbottomAttachment, XmATTACH_FORM,
		XmNtopAttachment, XmATTACH_FORM,
		XmNrightAttachment, XmATTACH_FORM,
		XmNleftAttachment, XmATTACH_FORM,
		NULL);
   	XtVaSetValues (rw_Izq_energy_edAnal,
		XmNbottomAttachment, XmATTACH_FORM,
		XmNtopAttachment, XmATTACH_FORM, XmNtopOffset, 15,
		XmNleftAttachment, XmATTACH_FORM,
		XmNrightOffset, 5, XmNleftOffset, 5,
		NULL);

   	XtVaSetValues (fr_FDB_identify_edAnal,
		//XmNbottomAttachment, XmATTACH_FORM, XmNbottomOffset, 5,
		//XmNbottomAttachment, XmATTACH_WIDGET, XmNbottomWidget, fr_DB_identify_edAnal,
		XmNtopAttachment, XmATTACH_FORM, XmNtopOffset, 5,
		XmNleftAttachment, XmATTACH_WIDGET, XmNleftWidget, rw_Izq_energy_edAnal,
		XmNrightAttachment, XmATTACH_FORM, XmNrightOffset, 5,
		NULL);


	XtVaSetValues (fr_DB_identify_edAnal,
		XmNbottomAttachment, XmATTACH_FORM, XmNbottomOffset, 5,
		XmNtopAttachment, XmATTACH_FORM, XmNtopOffset, 80, 
		//XmNtopAttachment, XmATTACH_WIDGET, XmNtopWidget, fr_FDB_identify_edAnal,
		XmNleftAttachment, XmATTACH_WIDGET, XmNleftWidget, rw_Izq_energy_edAnal,
		XmNrightAttachment, XmATTACH_FORM, XmNrightOffset, 5,
		NULL);

	XtManageChild(fr_FDB_identify_edAnal);


	XtManageChild(fr_DB_identify_edAnal);
	XtManageChild(rw_Izq_energy_edAnal);
	XtManageChild(fm_energy_edAnal);
	
	if(!Datos.esFondo)
		   XtSetSensitive(fr_FDB_identify_edAnal, false);


	// **********************
	// * seccion de botones *
	// **********************
	i = 0;
   	cadena = XmStringCreate ("OK", "BoldNormal");
   	XtSetArg (args[i], XmNlabelString, cadena); i++;
	XtSetArg (args[i], XmNalignment, XmALIGNMENT_CENTER); i++;
//   XtSetArg (args[i], XmNfontList, lista_fuentes); i++;
	bt_ok_edAnal = XmCreatePushButton(fm_botones_edAnal, "bt_ok_edAnal", args, i);
	XtAddCallback (bt_ok_edAnal, XmNactivateCallback, pulsa_ok_edit_analisis, NULL);
   	XmStringFree (cadena);
	i = 0;
   	cadena = XmStringCreate ("Cancel", "BoldNormal");
   	XtSetArg (args[i], XmNlabelString, cadena); i++;
	XtSetArg (args[i], XmNalignment, XmALIGNMENT_CENTER); i++;
//   XtSetArg (args[i], XmNfontList, lista_fuentes); i++;
	bt_cancel_edAnal = XmCreatePushButton(fm_botones_edAnal, "bt_cancel_edAnal", args, i);
	XtAddCallback (bt_cancel_edAnal, XmNactivateCallback, pulsa_cancel_edit_analisis, NULL);
   	XmStringFree (cadena);
	i = 0;
   	cadena = XmStringCreate ("Fit", "BoldNormal");
   	XtSetArg (args[i], XmNlabelString, cadena); i++;
	XtSetArg (args[i], XmNalignment, XmALIGNMENT_CENTER); i++;
//   XtSetArg (args[i], XmNfontList, lista_fuentes); i++;
	bt_adjust_edAnal = XmCreatePushButton(fm_botones_edAnal, "bt_adjust_edAnal", args, i);
	XtAddCallback (bt_adjust_edAnal, XmNactivateCallback, pulsa_adjust_edit_analisis, NULL);
   	XmStringFree (cadena);
   	XtVaSetValues (bt_ok_edAnal,
		XmNbottomAttachment, XmATTACH_FORM,
		XmNtopAttachment, XmATTACH_FORM,
		XmNleftAttachment, XmATTACH_FORM,
		XmNrightAttachment, XmATTACH_POSITION, XmNrightPosition, 1,
		XmNrightOffset, 5, 
		NULL);
   	XtVaSetValues (bt_cancel_edAnal,
		XmNbottomAttachment, XmATTACH_FORM,
		XmNtopAttachment, XmATTACH_FORM,
		XmNleftAttachment, XmATTACH_POSITION, XmNleftPosition, 1,
		XmNrightAttachment, XmATTACH_POSITION, XmNrightPosition, 2,
		XmNrightOffset, 5, XmNleftOffset, 5,
		NULL);
   	XtVaSetValues (bt_adjust_edAnal,
		XmNbottomAttachment, XmATTACH_FORM,
		XmNtopAttachment, XmATTACH_FORM,
		XmNleftAttachment, XmATTACH_POSITION, XmNleftPosition, 2,
		XmNrightAttachment, XmATTACH_FORM,	
		XmNleftOffset, 5,
		NULL);
	XtManageChild(bt_ok_edAnal);
	XtManageChild(bt_cancel_edAnal);
	XtManageChild(bt_adjust_edAnal);	
	}


//********************** CREATE DIALOG ADICIÓN **********************
/* Crea el pequeño cuadro de adición de picos:
	Un textbox para añadir centroides de cualquier energía
	Una lista con las emisiones cercanas
	Botones para añadir por cualquiera de los dos métodos o para quitarlos
	Lista con los picos añadidos
	Botones de OK/Cancel
*/
void CreateDialogAdicion() 
	{
   	Arg		args[7];
   	Cardinal	i = 0;
	XmString cadena;
	
	// *********************
	// * cuadro de dialogo *
	// *********************
	printf("CREATE DIALOG ADICIÓN\n");
	i = 0;
		
	XtSetArg (args[i], XmNtitle, "Add new peak(s)"); i++;
	XtSetArg (args[i], XmNmwmDecorations, MWM_DECOR_BORDER|MWM_DECOR_TITLE|MWM_DECOR_MENU); i++;
	XtSetArg (args[i], XmNmwmFunctions, MWM_FUNC_MOVE); i++;
	XtSetArg (args[i], XmNwidth, 300); i++;
	XtSetArg (args[i], XmNheight, 250); i++;
	dlg_adAnal = XmCreateFormDialog(app_shell, "dlg_edAnal", args, i);
	
	//Creamos 2 columnas en las que distribuiremos los widgets
	i = 0;
	rw1_adAnal = XmCreateRowColumn (dlg_adAnal, "rw1_adAnal", args, i);
	i = 0;
	rw2_adAnal = XmCreateRowColumn (dlg_adAnal, "rw2_adAnal", args, i);

	XtVaSetValues (rw1_adAnal,
		XmNbottomAttachment, XmATTACH_FORM,
		XmNtopAttachment, XmATTACH_FORM, XmNtopOffset, 15,
		XmNleftAttachment, XmATTACH_FORM,
		XmNrightAttachment, XmATTACH_POSITION,XmNrightPosition,32,	
		NULL);
	XtVaSetValues (rw2_adAnal,
		XmNbottomAttachment, XmATTACH_FORM,
		XmNtopAttachment, XmATTACH_FORM, XmNtopOffset, 15,
		XmNleftAttachment, XmATTACH_POSITION, XmNleftPosition, 32,
		XmNrightAttachment, XmATTACH_FORM,
		NULL);
	XtManageChild(rw1_adAnal);
	XtManageChild(rw2_adAnal);
	
	i = 0;
	fm_emi_identify_adAnal = XmCreateForm(rw2_adAnal, "fm_emi_identify_adAnal", args, i);
	XtManageChild(fm_emi_identify_adAnal);
	i = 0;
	fm_energy_adAnal = XmCreateForm(rw1_adAnal, "fm_energy_adAnal", args, i);
	XtManageChild(fm_energy_adAnal);
	
   	i = 0;
	cadena = XmStringCreate ("Centroid (keV): ", "BoldNormal");
   	XtSetArg (args[i], XmNlabelString, cadena); i++;
 //  	lb_cent_adAnal = XmCreateLabel (rw2_adAnal, "lb_cent_adAnal", args, i);
    lb_cent_adAnal = XmCreateLabel (fm_emi_identify_adAnal, "lb_cent_adAnal", args, i);
   	XmStringFree (cadena);
	XtManageChild(lb_cent_adAnal);
   	XtVaSetValues (lb_cent_adAnal,
		XmNtopAttachment, XmATTACH_FORM, XmNtopOffset, 5,
		//XmNbottomAttachment, XmATTACH_FORM, XmNbottomOffset, 5,
		//XmNbottomAttachment, XmATTACH_WIDGET, XmNbottomWidget, txt_cent_adAnal,
		XmNleftAttachment, XmATTACH_FORM, XmNleftOffset, 20, 
		XmNrightAttachment, XmATTACH_FORM, XmNrightOffset, 20,
		NULL);

	i = 0;
   	XtSetArg (args[i], XmNcolumns, 10); i++;
   	XtSetArg (args[i], XmNmaxLength, 20); i++;
//   	txt_cent_adAnal = XmCreateTextField(rw2_adAnal, "txt_cent_adAnal", args, i);
	txt_cent_adAnal = XmCreateTextField(fm_emi_identify_adAnal, "txt_cent_adAnal", args, i);
	XtAddCallback(txt_cent_adAnal, XmNfocusCallback, seleccionarTexto, NULL);   
	XtManageChild(txt_cent_adAnal);
	cadena = XmStringCreateLocalized ("");
   	XtVaSetValues (txt_cent_adAnal,
		XmNtopAttachment, XmATTACH_FORM, XmNtopOffset, 25,
		//XmNbottomAttachment, XmATTACH_WIDGET, XmNbottomWidget, bt_addc_adAnal,
		XmNleftAttachment, XmATTACH_FORM, XmNleftOffset, 40, 
		XmNrightAttachment, XmATTACH_FORM, XmNrightOffset, 40,
		NULL);
	
	//Botón de añadir pico a partir de centroide
	i = 0;
   	cadena = XmStringCreateLocalized ("<=");
   	XtSetArg (args[i], XmNlabelString, cadena); i++;
	XtSetArg (args[i], XmNalignment, XmALIGNMENT_CENTER); i++;
//	bt_addc_adAnal = XmCreatePushButton(rw2_adAnal, "bt_addc_adAnal", args, i);
  	bt_addc_adAnal = XmCreatePushButton(fm_emi_identify_adAnal, "bt_addc_adAnal", args, i);
   	XmStringFree (cadena);
   	XtAddCallback (bt_addc_adAnal, XmNactivateCallback, pulsa_addc_adAnal, NULL);
	XtManageChild(bt_addc_adAnal);
   	XtVaSetValues (bt_addc_adAnal,
		XmNtopAttachment, XmATTACH_FORM, XmNtopOffset, 55,
		XmNbottomAttachment, XmATTACH_FORM, XmNbottomOffset, 5,
		XmNleftAttachment, XmATTACH_FORM, XmNleftOffset, 40, 
		XmNrightAttachment, XmATTACH_FORM, XmNrightOffset, 40,
		NULL);
	/*
	//Frame con la lista de emisiones
	cadena = XmStringCreate ("", "BoldNormal");
   	i = 0;
   	XtSetArg (args[i], XmNchildType, XmFRAME_TITLE_CHILD); i++;
   	XtSetArg (args[i], XmNchildVerticalAlignment, XmALIGNMENT_CENTER); i++;
   	XtSetArg (args[i], XmNlabelString, cadena); i++;
   	lb_DB_identify_adAnal = XmCreateLabelGadget (rw2_adAnal, "lb_DB_identify_adAnal", args, i);
   	XmStringFree (cadena);
	XtManageChild(lb_DB_identify_adAnal);
	// lista de seleccion
	i = 0;
	fm_DB_identify_adAnal = XmCreateForm(rw2_adAnal, "fm_DB_identify_adAnal", args, i);
	fm_DB_identify2_adAnal = XmCreateForm(fm_DB_identify_adAnal, "fm_DB_identify2_adAnal", args, i);

	// lista de identificacion
	AnalIdent.SetPeakList(&Datos.listaPicos);
	AnalIdent.CreateWidgets(fm_DB_identify2_adAnal);
	XtManageChild(fm_DB_identify2_adAnal);
	
	// Boton de añadir pico a partir de emision
	i = 0;
   	cadena = XmStringCreateLocalized ("<=");
   	XtSetArg (args[i], XmNlabelString, cadena); i++;
	XtSetArg (args[i], XmNalignment, XmALIGNMENT_CENTER); i++;
	bt_adde_adAnal = XmCreatePushButton(fm_DB_identify_adAnal, "bt_adde_adAnal", args, i);
   	XmStringFree (cadena);
   	XtAddCallback (bt_adde_adAnal, XmNactivateCallback, pulsa_adde_adAnal, NULL);
	XtManageChild(bt_adde_adAnal);
   	XtVaSetValues (bt_adde_adAnal,
		XmNtopAttachment, XmATTACH_WIDGET, XmNtopWidget, fm_DB_identify2_adAnal,
		XmNbottomAttachment, XmATTACH_FORM, XmNbottomOffset, 5,
		XmNleftAttachment, XmATTACH_FORM, XmNleftOffset, 40, 
		XmNrightAttachment, XmATTACH_FORM, XmNrightOffset, 40,
		NULL);
		
	
	XtVaSetValues (fm_DB_identify2_adAnal,
		XmNleftAttachment, XmATTACH_FORM,
		XmNrightAttachment, XmATTACH_FORM,
		NULL);
	
	XtManageChild(fm_DB_identify_adAnal);
	*/
	i=0;
	Widget row_aux= XmCreateRowColumn (fm_energy_adAnal, "row_aux", args, i);
	XtManageChild(row_aux);

	//indicador de energia
	cadena = XmStringCreate ("Energy (keV):", "BoldNormal");
   	i = 0;
   	XtSetArg (args[i], XmNlabelString, cadena); i++;
   	lbEmission_energy_adAnal = XmCreateLabel (row_aux, "lbEmission_energy_adAnal", args, i);
   	XmStringFree (cadena);
	XtManageChild(lbEmission_energy_adAnal);
 
	printf("lista de energías seleccionadas\n");
	//Lista de energías/probabilidad
	// Create list widget
	i = 0;
	XtSetArg (args[i], XmNlistSizePolicy, XmCONSTANT); i++;
	XtSetArg (args[i], XmNselectionPolicy, XmSINGLE_SELECT); i++;
	XtSetArg (args[i], XmNvisibleItemCount, 5); i++;
	listEnergy_adAnal = XmCreateScrolledList(row_aux, "listEnergy_adAnal", args, i);
	XtManageChild(listEnergy_adAnal);
	printf("Boton de borrar adición\n");
	// Boton de borrar adición
	i = 0;
   	cadena = XmStringCreateLocalized ("Delete");
   	XtSetArg (args[i], XmNlabelString, cadena); i++;
	XtSetArg (args[i], XmNalignment, XmALIGNMENT_CENTER); i++;
  	bt_del_adAnal = XmCreatePushButton(fm_energy_adAnal, "bt_del_adAnal", args, i);
   	XmStringFree (cadena);
   	XtAddCallback (bt_del_adAnal, XmNactivateCallback, pulsa_quita_identifica_adAnal, NULL);
	XtManageChild(bt_del_adAnal);
   	XtVaSetValues (bt_del_adAnal,
		XmNtopAttachment, XmATTACH_FORM, XmNtopOffset, 120,
		XmNleftAttachment, XmATTACH_FORM, XmNleftOffset, 5, 
		XmNrightAttachment, XmATTACH_FORM, XmNrightOffset, 5,
		NULL);
   	
	// **********************
	// * seccion de botones *
	// **********************
	
	printf("sección de botones\n");
	i = 0;
   	cadena = XmStringCreate ("OK", "BoldNormal");
   	XtSetArg (args[i], XmNlabelString, cadena); i++;
	XtSetArg (args[i], XmNalignment, XmALIGNMENT_CENTER); i++;
	bt_ok_adAnal = XmCreatePushButton(fm_energy_adAnal, "bt_ok_adAnal", args, i);
	XtAddCallback (bt_ok_adAnal, XmNactivateCallback, pulsa_ok_adicion, NULL);
   	XmStringFree (cadena);
	
	i = 0;
   	cadena = XmStringCreate ("Cancel", "BoldNormal");
   	XtSetArg (args[i], XmNlabelString, cadena); i++;
	XtSetArg (args[i], XmNalignment, XmALIGNMENT_CENTER); i++;
	bt_cancel_adAnal = XmCreatePushButton(fm_energy_adAnal, "bt_cancel_adAnal", args, i);
	XtAddCallback (bt_cancel_adAnal, XmNactivateCallback, pulsa_cancel_adicion, NULL);
   	XmStringFree (cadena);
   	XtVaSetValues (bt_ok_adAnal,
		XmNtopAttachment, XmATTACH_FORM, XmNtopOffset, 170,
		XmNleftAttachment, XmATTACH_FORM, XmNleftOffset, 5, 
		XmNrightAttachment, XmATTACH_FORM, XmNrightOffset, 5,
		NULL);
   	XtVaSetValues (bt_cancel_adAnal,
		XmNtopAttachment, XmATTACH_FORM, XmNtopOffset, 200,
		XmNleftAttachment, XmATTACH_FORM, XmNleftOffset, 5, 
		XmNrightAttachment, XmATTACH_FORM, XmNrightOffset, 5,
		NULL);
	XtManageChild(bt_ok_adAnal);
	XtManageChild(bt_cancel_adAnal);
	printf("Termina creación del cuadro de diálogo de adición\n");
	}
	
	
//****************** ACTUALIZA IDENTIFICACIÓN ANALISIS ******************
/*	Añade a la lista de emisiones de edición las emisiones asociadas cuando
	se edita el pico, o las que añadamos luego
	*/
void ActualizaIdentificacionAnalisis(unsigned short int emission) 
	{
	XmString cadena, s1, s2;
	char buffer[80], nombre[3];
		
	printf("ACTUALIZA IDENTIFICACIÓN ANALISIS\n");
	printf("La emisión es %d\n", emission);

	NodoPicos *intervalo = DevuelveNodo(Datos.listaPicos, numIntervaloEdicionAnal);
	NodoPico *pico = DevuelveNodo(intervalo->picos, numPicoEdicionAnal);

	// Ahora solo muestra los datos
	if (LaraBD.GetIsRunning())	sprintf(buffer, "  %.3fkeV", LaraBD.GetEnergy(emission));
	else 						
		{
		for(int i=0;i<pico->Ajuste.emisiones->numElements();i++)
			{
			NodoEmision *ne=(NodoEmision *)pico->Ajuste.emisiones->getNode(i);
			sprintf(buffer, "  %.3fkeV", ne->Energia);
			}//NOTA: Esto sólo pondrá la energía del último
		}
	XmString entry;
	entry = XmStringCreateLocalized(buffer);
	XmListAddItemUnselected(listEnergy, entry, 0);
	XmStringFree(entry);
		
	if (LaraBD.GetIsRunning())	sprintf(buffer, "  %f%%", LaraBD.GetProbability(emission));
	else 						sprintf(buffer, "  ???%%");
	entry = XmStringCreateLocalized(buffer);
	XmListAddItemUnselected(listEnergy, entry, 0);
	XmStringFree(entry);
	
	if (LaraBD.GetIsRunning()) 
		{
		unsigned short int element = LaraBD.GetEmissor(emission);
		s1 = XmStringCreateLocalized ("  ");
		s2 = XmStringNameElement(element, "SubIndice");
		cadena = XmStringConcat(s1, s2);
		XmStringFree(s1); 
		XmStringFree(s2);
//		cadena=XmStringNameElement(element, "SubIndice");
//		cadena=XmStringNameElement(XmStringCreateLocalized("  "), XmStringNameElement(element, "SubIndice"));
		}
	else cadena = XmStringCreateLocalized ("  ???");
	/*XtVaSetValues (lbName_energy_edAnal, XmNlabelString, cadena, NULL);
	XmStringFree (cadena);*/
	//entry = GetEntryListXmString(cadena);
	XmListAddItemUnselected(listElement,cadena, 0);
	XmStringFree(cadena);
	printf("Fin de identificación análisis\n");
	}

//****************** ACTUALIZA IDENTIFICACIÓN ANALISIS ******************
/*	Variación de la función que lo hace según un nombre y una energía sin más
	*/
void ActualizaIdentificacionAnalisis(char *nombre, char *energia) 
	{
	printf("ACTUALIZA IDENTIFICACIÓN ANÁLISIS %s, %s\n", nombre, energia);
	XmString cadena, s1, s2;
	char buffer[80];

	sprintf(buffer, "  %s", energia);
//	cadena = XmStringCreateLocalized (energia);
	cadena = XmStringCreateLocalized (buffer);
	XmListAddItem(listEnergy, cadena, 0);
	//XmStringFree (cadena);

	cadena = XmStringCreateLocalized ("  -");//No le ponemos probabilidad
	XmListAddItemUnselected(listEnergy, cadena, 0);
	//XmStringFree (cadena);

	sprintf(buffer, "  %s", nombre);
	cadena = XmStringCreateLocalized (buffer);
	XmListAddItem(listElement, cadena, 0);
	//XmStringFree (cadena);
	printf("FIN ACTUALIZA IDENTIFICACIÓN ANÁLISIS\n");
	}

//**************** CAMBIA TEXTO FBD **********************
void cambia_texto_FBD(Widget widget, XtPointer client_data, XtPointer call_data)
	{
	if(!strcmp(XmTextGetString(txt_FDB_nombre), "No matching emission"))
		//Deshabilitamos el botón de selección del texto
		XtSetSensitive(bt_FDB_identifica, false);				
	else	//lo habilitamos por si estuviera deshabilitado
		XtSetSensitive(bt_FDB_identifica, true);				
	}	

//******************* PULSA IDENTIFICA EDIT ANALISIS *******************
// Se activa cuando se pulsa el botón "<=" de la interfaz de identificación de picos
// Al identificar el pico con un determinado elemento, el resultado es que se
// pone como identificado y se le pone como código de elemento getSelectedEmission() 
// y como energía la que nos da la base de datos para esa getSelectedEmission().
// Con esto el pico queda calibrado.
void pulsa_identifica_edit_analisis(Widget widget, XtPointer client_data, XtPointer call_data) 
	{
	printf("PULSA IDENTIFICA EDIT ANÁLISIS\n");
	ActualizaIdentificacionAnalisis(AnalIdent.GetSelectedEmission());

	if (LaraBD.GetIsRunning()) 
		{
		//NOTA: Sólo se considera un pico por intervalo
		NodoPicos *intervalo = DevuelveNodo(Datos.listaPicos, numIntervaloEdicionAnal);
		NodoPico *pico = DevuelveNodo(intervalo->picos, numPicoEdicionAnal); 
		printf("Está identificado el pico %d,%d\n", numIntervaloEdicionAnal, numPicoEdicionAnal);	
		if(pico->Ajuste.identificado==false)	pico->Ajuste.emisiones=new LinkedList();
		
		pico->Ajuste.identificado = true;
		pico->Ajuste.analizado = true;
	
		NodoEmision *ne=new NodoEmision();
		ne->codElemento	= AnalIdent.GetSelectedEmission();//Devuelve selected emission, que es value que es el número de emisión
		ne->Energia = LaraBD.GetEnergy(AnalIdent.GetSelectedEmission());
		pico->Ajuste.emisiones->addLast(ne);
		//pico->Ajuste.calibrado = true;//NOTA: Ponemos sólo a calibrado los que se identifican en
										//		calibración, para poder distinguirlos
//		if(todosIdentificados())	{EstadoTrabajo.Analisis=true; ActivaActividad();}
		}
	else	Aviso(fondo_analisis, "No Database for identification avalaible");
	}

//*********************** TODOS IDENTIFICADOS ********************
/*	Devuelve true si todos los picos contemplados están identificados con al
	menos una emisión.
	*/
bool todosIdentificados()
	{
	printf("TODOS IDENTIFICADOS\n");
	//if(Datos.listaPicos==NULL)	return false;
	if(Datos.listaPicos.numElementos<=0)	return false;
	for(int i=1;i<=Datos.listaPicos.numElementos;i++)
		{
		NodoPicos *intervalo = DevuelveNodo(Datos.listaPicos, i);
		for(int j=1;j<=intervalo->picos.numElementos;j++)
			{
			NodoPico *pico = DevuelveNodo(intervalo->picos, j);
			if(pico->Ajuste.ajustado && !pico->Ajuste.identificado)	
				{
				//printf("El pico %d,%d está ajustado pero no identificado\n", i, j);
				if(!pico->Ajuste.calibrado)	return false;
//				else	printf("El pico tiene una identificación faltante debida al proceso de la calibración\n");
				}
			}
		}
	printf("Todos los picos identificados\n");
	return true;
	}
	
//************* PULSA QUITA IDENTIFICA EDIT ANALISIS *************
/*	Estamos realizando una mejora para que podamos asociar más de una identificación
	a una emisión. Eso implica que podamos también quitar alguna si en un momento dado
	el usuario decide que no le conviene
*/
void pulsa_quita_identifica_edit_analisis(Widget widget, XtPointer client_data, XtPointer call_data) 
	{
	printf("PULSA QUITA IDENTIFICA ANALISIS\n");
	int numpos;
	int *posiciones;
	XmListGetSelectedPos(listElement, &posiciones, &numpos);
	//for(int i=0;i<numpos;i++)		printf("%d\n", posiciones[i]);
		
	int posicionesEnergy[numpos*2];
	for(int i=0;i<numpos;i++)
		{
		posicionesEnergy[i]=2*posiciones[i]-1;
		posicionesEnergy[i+1]=2*posiciones[i];
		}
	XmListDeletePositions(listElement, posiciones, numpos);
	XmListDeletePositions(listEnergy, posicionesEnergy, numpos*2);
		
	NodoPicos *intervalo = DevuelveNodo(Datos.listaPicos, numIntervaloEdicionAnal);
	NodoPico *pico = DevuelveNodo(intervalo->picos, numPicoEdicionAnal); 
	for(int i=0;i<numpos;i++)	pico->Ajuste.emisiones->deleteNode(posiciones[i]-1);
	printf("Hemos quitado la emisión, nos quedan %d emisiones para el pico\n", pico->Ajuste.emisiones->numElements());
	if(pico->Ajuste.emisiones->numElements()==0)	
		{
		pico->Ajuste.identificado=false;
		pico->Ajuste.analizado=false;///
		}
		
	XtManageChild(listElement);
	XtManageChild(listEnergy);
	delete [] posiciones;
	
	//
	reseteaActividad();
	//
/*	if(!todosIdentificados())	
		{
		EstadoTrabajo.Analisis=false; 	
		DesactivaActividad();
		}*/
	}

	
//************** BUSCA NOMBRE FBD *******************
char *buscaNombreFBD(float energia, char *nombre)
	{
	int numFilas=numeroFilas(nombre);
	for(int i=0;i<numFilas;i++)
		{
		float e=leerElemento(nombre, i, 0);
		if(e==energia)	
			return leerElementoCadena(nombre, i, 1);
		}
	}

/*********************************************************
********** FUNCIONES DEL DIÁLOGO DE ADICIÓN **************
*********************************************************/
/*	Realiza el ajuste con el nuevo pico.
	OJO: No usa restricciones, al igual que la edición. Con restricciones, los picos
	que se están añadiendo no deberían existir.
	*/
void pulsa_ok_adicion(Widget widget, XtPointer client_data, XtPointer call_data) 
	{
	int numPositions=0;
	XmStringTable items;
	char *text;
	int interv, pic, cont=0;

	printf("PULSA OK ADICIÓN\n");

	// Indica cambio en el trabajo
	EstadoTrabajo.TrabajoCambios = true;
	PonTituloFichCambiado();

	//rehace el ajuste
	PonCursorEspera();

	XtVaGetValues(listEnergy_adAnal, XmNitemCount, &numPositions, 
					XmNitems, &items, NULL);

	NodoPicos *intervalo = DevuelveNodo(Datos.listaPicos, numIntervaloEdicionAnal);
	NodoPico *pico = DevuelveNodo(intervalo->picos, numPicoEdicionAnal);
	
	int numGaussianas=intervalo->picos.numElementos+numPositions;
	float centroides[numGaussianas];
	for(int i=0;i<intervalo->picos.numElementos;i++)//Tomamos todos los picos (el centroide de este ya es el nuevo)
		{
		NodoPico *pico2=DevuelveNodo(intervalo->picos, i);
		centroides[i]=pico2->Ajuste.Centroide;
		pico2->Ajuste.identificado=false;
		pico2->Ajuste.emisiones->deleteAll();
		}

	int posicionesNoIdentificables[numPositions];
	float centroidesNoIdentificables[numPositions];
	int k=0;
	for(int i=intervalo->picos.numElementos; i<(intervalo->picos.numElementos+numPositions); i++)
		{
		XmStringGetLtoR(items[i-intervalo->picos.numElementos], XmFONTLIST_DEFAULT_TAG, &text);
		centroides[i]=Datos.DaCanal(atof(text));
		centroidesNoIdentificables[k++]=centroides[i];
		XtFree(text);
		}
	
	float *centord=new float[intervalo->picos.numElementos+numPositions];
	for(int i=0;i<intervalo->picos.numElementos+numPositions;i++)
		centord[i]=centroides[i];
	
	Datos.ordenaVector(centord, intervalo->picos.numElementos+numPositions);
	for(int i=0;i<intervalo->picos.numElementos+numPositions;i++)	centroides[i]=centord[i];
	for(int i=0;i<numPositions;i++)
		{
		for(int j=0;j<intervalo->picos.numElementos+numPositions;j++)
			{
			if(centord[j]==centroidesNoIdentificables[i])
				{
				posicionesNoIdentificables[i]=j;
				}				
			}
		}
	delete [] centord;

	int nError2=Datos.AjusteIntervaloGAML(numIntervaloEdicionAnal, centroides, numGaussianas,-1,-1,-1);
	if(nError2==-3 || nError2==-5)
		{
		printf("ERROR: %d\n", nError2);
		Aviso(app_shell, "ERROR: Fitness Unaccurate, discarded");
		PonCursorNormal();
		return;
		}

	//Análisis experto
	float centroidesAjustados[intervalo->picos.numElementos];
	for (int i = 1; i <= intervalo->picos.numElementos; i++) 
		{
		NodoPico *pico = DevuelveNodo(intervalo->picos, i);
		centroidesAjustados[i-1]=pico->Ajuste.Centroide;
//		printf("centroides[%d] vale %f\n", i-1, centroidesAjustados[i-1]);
		}
	analisisExperto(numIntervaloEdicionAnal, centroidesAjustados, NULL, posicionesNoIdentificables, numPositions);//Ponerle un noId
		
	ReconstruyeListaAnalisis();
		
	// Actualiza los datos de los picos en la grafica
	int nError = CopiaAjustesAGrafica();
	if (nError < 0) 
		{
		Aviso(fondo_analisis, "No Memory Avalaible");
		printf(ERROR_ANALYSIS);
		printf(ERROR_NO_MEMORY);
		return;
		}
	
	ActivaAjustesGrafica(EstadoVisual.AjusteCalibracion);
	PonCursorNormal();

	// ponemos el último de los intervalos como seleccionado
	int posic=determinaPosicion(numIntervaloEdicionAnal);
	XmListSelectPos(result_lista_analisis, posic, false);
	XmListSetBottomPos(result_lista_analisis, posic);
	if (EstadoVisual.AutoZoomAnalisis) 
		{
		int interv=0, pic=0;
		determinaPico(result_lista_analisis, posic, &interv, &pic);
		NodoPicos *intervalo = DevuelveNodo(Datos.listaPicos, interv);

		int limInferior, limSuperior;

		limInferior = intervalo->limInferior;
		limSuperior = intervalo->limSuperior;
			
		// muestra 3 veces el tamaño del intervalo
		// para que se aprecien las estructuras coetaneas
		int ancho = 2*(limSuperior - limInferior)/3;
		MarcaZoom(limInferior - ancho, limSuperior + ancho);
		}
		
	EstadoTrabajo.AnalisisActividad=false;
	EstadoTrabajo.AnalisisLD=false;
	
	if (EstadoVisual.AjusteCalibracion)	PintaGrafica(0);
	if(dlg_adAnal!=NULL)	XtUnmanageChild(dlg_adAnal);
	}
	
//*********** PULSA CANCEL ADICIÓN **********
void pulsa_cancel_adicion(Widget widget, XtPointer client_data, XtPointer call_data) 
	{
	printf("PULSA CANCEL ADICION\n");
	if(dlg_adAnal!=NULL)	XtUnmanageChild(dlg_adAnal);
	}
	
//******** PULSA ADJUST ADICIÓN ************
void pulsa_adjust_adicion(Widget widget, XtPointer client_data, XtPointer call_data) 
	{
	//Operación de ajuste
	}
	
//********* PULSA ADDC ADANAL ****************
void pulsa_addc_adAnal(Widget widget, XtPointer client_data, XtPointer call_data) 
	{
	//NOTA: Quizás sea bueno cerciorarse de que la energía no es disparatada
	char *texto = XmTextGetString(txt_cent_adAnal);
	XmString cadena=XmStringCreateLocalized(texto);
	XmListAddItem(listEnergy_adAnal, cadena, 0);
	}

//********* PULSA ADDE ADANAL ****************
void pulsa_adde_adAnal(Widget widget, XtPointer client_data, XtPointer call_data) 
	{
	double Energia = LaraBD.GetEnergy(AnalIdent.GetSelectedEmission());
	char texto[20];
	sprintf(texto, "%.2f", Energia);
	XmString cadena=XmStringCreateLocalized(texto);
	XmListAddItem(listEnergy_adAnal, cadena, 0);
	}

//*********** PULS QUITA IDENTIFICA ADANAL *********
void pulsa_quita_identifica_adAnal(Widget widget, XtPointer client_data, XtPointer call_data) 
	{
	int numpos;
	int *posiciones;

	XmListGetSelectedPos(listEnergy_adAnal, &posiciones, &numpos);
	XmListDeletePositions(listEnergy_adAnal, posiciones, numpos);
	XtManageChild(listEnergy_adAnal);
	}
	
//******************* PULSA IDENTIFICA FDB EDIT ANALISIS *******************
// Se activa cuando se pulsa el botón "<=" de la interfaz de identificación de picos
// Al identificar el pico con un determinado elemento, el resultado es que se
// pone como identificado y se le pone como código de elemento getSelectedEmission() 
// y como energía la que nos da la base de datos para esa getSelectedEmission().
// Con esto el pico queda calibrado.
void pulsa_identifica_FDB_edit_analisis(Widget widget, XtPointer client_data, XtPointer call_data) 
	{
	printf("PULSA IDENTIFICA FDB EDIT ANALISIS\n");
	char *texto=XmTextGetString(txt_FDB_nombre);
	string nombre(texto);
	string energia(texto);

	nombre=nombre.substr(0,nombre.find(" -> "));		
	energia=energia.substr(energia.find(" -> ")+4, energia.find(" keV"));		
	
	cout << "energia: " << energia << endl;
	cout << "nombre: " << nombre << endl;
		
	ActualizaIdentificacionAnalisis((char *)nombre.c_str(), (char *)energia.c_str());

	if (LaraBD.GetIsRunning()) 
		{
		//NOTA: Sólo se considera un pico por intervalo
		NodoPicos *intervalo = DevuelveNodo(Datos.listaPicos, numIntervaloEdicionAnal);
//		NodoPico *pico = intervalo->picos.inicio; // hay 1 solo pico
		NodoPico *pico = DevuelveNodo(intervalo->picos, numPicoEdicionAnal);
	
		pico->Ajuste.identificado = true;
		pico->Ajuste.analizado = true;///
	//	printf("Código de elemento: %f y la función %f\n", pico->Ajuste.codElemento, AnalIdent.GetSelectedEmission());
		//pico->Ajuste.Energia = atof((char *)energia.c_str());
		//
		NodoEmision *ne;
		ne->Energia=atof((char *)energia.c_str());
		pico->Ajuste.emisiones->addLast(ne);
		//
		
		//pico->Ajuste.calibrado = true;//NOTA:Estamos poniendo a calibrado sólo los que se
										//identifican en calibración, para poder distinguirlos
		}
	else	Aviso(fondo_analisis, "No Database for identification avalaible");
//	if(todosIdentificados())	{EstadoTrabajo.Analisis=true;	ActivaActividad();}
}

	
//********************** PULSA CANCEL EDIT ANÁLISIS *************************
// Cancela el análisis y la adaptación a una emisión de elemento conocido, manteniendo
// el análisis previo
void pulsa_cancel_edit_analisis(Widget widget, XtPointer client_data, XtPointer call_data) 
	{
	NodoPicos *intervalo = DevuelveNodo(Datos.listaPicos, numIntervaloEdicionAnal);
	NodoPico *pico = DevuelveNodo(intervalo->picos, numPicoEdicionAnal);
	
	// devuelve los datos iniciales
	pico->Ajuste = AjusteSeguridadAnal;
	
	// rehace el ajuste
	Datos.ActualizaIntervaloAnalisis(numIntervaloEdicionAnal);
	
	// Actualiza los datos de los picos en la grafica
	int nError = CopiaAjustesAGrafica();
	if (nError < 0) 
		{
		Aviso(fondo_analisis, "No Memory Avalaible");
		printf(ERROR_ANALYSIS);
		printf(ERROR_NO_MEMORY);
		return;
		}

	ActivaAjustesGrafica(EstadoVisual.AjusteCalibracion);
	
	if (EstadoVisual.AjusteCalibracion)	PintaGrafica(0);

	XtUnmanageChild(dlg_edAnal);
	}

//******************* PULSA OK EDIT ANÁLISIS ***********************************
/* Cuando pulsamos el botón de ok en la interfaz de edición de analisis
	Traspasa al ajuste los cambios que se hayan realizado:
		Cambios en las emisiones asociadas al pico
		Cambios en los parámetros de forma
	NOTA: Refrescar el dibujo
	*/	
void pulsa_ok_edit_analisis(Widget widget, XtPointer client_data, XtPointer call_data) 
	{
	char *cenText, *sigmaText, *tauText, *YtauText, *energiaText, *elemText, *seguridad;
	double cen, sigma, tau, Ytau, energia;
	printf("********************** PULSA OK EDIT ANALISIS *******************\n");
		
	printf("Cambiando los ajustes para [%d,%d]\n", numIntervaloEdicionAnal, numPicoEdicionAnal);
	NodoPicos *intervalo = DevuelveNodo(Datos.listaPicos, numIntervaloEdicionAnal);
	NodoPico *pico = DevuelveNodo(intervalo->picos, numPicoEdicionAnal);

	printf("El pico que hemos modificado tiene centroide en %f y tiene identificación a %d\n", pico->Ajuste.Centroide, pico->Ajuste.identificado);
	// Actualiza la lista de analisis si ha habido identificacion
	//if (pico->Ajuste.identificado)	ReponeListaAnalisis();
	ReponeListaAnalisis();//Siempre, pues puede que haya pasado de identificado a no identificado!
			
	// Indica cambio en el trabajo
	EstadoTrabajo.TrabajoCambios = true;
	PonTituloFichCambiado();
	printf("Trabajo Cambiado\n");

	// recoge los datos introducidos
	cenText = XmTextGetString(txtCen_adjust_edAnal);
	sigmaText = XmTextGetString(txtSigma_adjust_edAnal);
	tauText = XmTextGetString(txtTau_adjust_edAnal);
	YtauText = XmTextGetString(txtYtau_adjust_edAnal);
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
		//La incertidumbre la modificamos proporcionalmente
		double sc=pico->Ajuste.SigCentroide;
		pico->Ajuste.SigCentroide=(sc*cen)/pico->Ajuste.Centroide;
		//
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
		//La incertidumbre la modificamos proporcionalmente
		double sa=pico->Ajuste.SigAnchura;
		pico->Ajuste.SigAnchura=(sa*sigma)/pico->Ajuste.Anchura;
		//
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

		//La incertidumbre la modificamos proporcionalmente
		double st=pico->Ajuste.SigTau;
		pico->Ajuste.SigTau=(st*tau)/pico->Ajuste.Tau;
		//
			
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
			
		//La incertidumbre la modificamos proporcionalmente
		double syt=pico->Ajuste.SigYtau;
		pico->Ajuste.SigYtau=(syt*Ytau)/pico->Ajuste.Ytau;
		//
			
		pico->Ajuste.Ytau = Ytau;
		}
	printf("Valores del ajuste cambiados\n");

	// rehace el ajuste
	//NOTA: Cambia la política: ahora el botón OK sólo hace la identificación pero
	//		sin reajustar el pico. A veces la identificación no es todo lo buena que
	//		debería y reajustar implica perder la precisión en el dibujo del pico
    //		El reajuste debe hacerse explícitamente con el botón "Adjust"
//	Datos.AjustePico(numIntervaloEdicionAnal, 1, EstadoTrabajo.Calibracion, false);
//	Datos.AjustePico(numIntervaloEdicionAnal, numPicoEdicionAnal, EstadoTrabajo.Calibracion, false);

	Datos.ActualizaIntervaloAnalisis(numIntervaloEdicionAnal);
	printf("c=%f, a=%f, s=%f, t=%f\n", pico->Ajuste.Centroide, pico->Ajuste.Ytau, pico->Ajuste.Anchura, pico->Ajuste.Tau);
	// Actualiza los datos de los picos en la grafica
	int nError = CopiaAjustesAGrafica();
	if (nError < 0)
		{
		Aviso(fondo_analisis, "No Memory Avalaible");
		printf(ERROR_ANALYSIS);
		printf(ERROR_NO_MEMORY);
		return;
		}
	printf("Datos en gráfica actualizados\n");

	ActivaAjustesGrafica(EstadoVisual.AjusteCalibracion);
	printf("Gráfica dibujada\n");

	if (EstadoVisual.AjusteCalibracion)	PintaGrafica(0);
		
	XtUnmanageChild(dlg_edAnal);
	}

//******************* OBTENER DATOS EDIT ANALISIS *****************
/* Recoge los datos de análisis que haya en la interfaz de edición
*/	
void obtener_datos_edit_analisis(Widget widget)
	{	
	char *cenText, *sigmaText, *tauText, *YtauText, *seguridad;
	double cen, sigma, tau, Ytau;

	NodoPicos *intervalo = DevuelveNodo(Datos.listaPicos, numIntervaloEdicionAnal);
	NodoPico *pico = DevuelveNodo(intervalo->picos, numPicoEdicionAnal);
	
	// recoge los datos introducidos
	cenText = XmTextGetString(txtCen_adjust_edAnal);
	sigmaText = XmTextGetString(txtSigma_adjust_edAnal);
	tauText = XmTextGetString(txtTau_adjust_edAnal);
	YtauText = XmTextGetString(txtYtau_adjust_edAnal);
	
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
	}
	
//********************* PULSA ADJUST EDIT ANÁLISIS *************************
/*	Se llama cuando se pulsa el botón "Adjust" de la interfaz de edición
	NOTA: Este ajuste no tiene en cuenta restricciones!
		Lo dejamos así para dar la oportunidad de mejorar el ajuste, pero 
		hay que contar que quizás empeore las actividades al no estar tan 
		restringido.
		En general se muestra útil en espectros mal calibrados (la mejor opción
		es calibrar bien, por supuesto), pero en algunos como los suelos, que
		son imposibles de calibrar bien, es un buen método en lo que encontramos
		uno mejor.
	*/
void pulsa_adjust_edit_analisis(Widget widget, XtPointer client_data, XtPointer call_data) 
	{
		
	printf("++++++++++++++++++PULSA ADJUST EDIT ANALISIS ++++++++++++\n");
	NodoPicos *intervalo = DevuelveNodo(Datos.listaPicos, numIntervaloEdicionAnal);
	NodoPico *pico = DevuelveNodo(intervalo->picos, numPicoEdicionAnal);

	obtener_datos_edit_analisis(widget);
	
	//rehace el ajuste
	PonCursorEspera();
	
	int numGaussianas=intervalo->picos.numElementos;
	float centroides[numGaussianas];
	//NUEVO: Tomamos la t, s y h actuales como inicio para el ajuste
	float taus=-1;
	float sigmas=-1;
	int interv, pic, cont=0;
		
	//Guardamos las emisiones antiguas
	printf("Guardamos emisiones antiguas de %d elementos\n", intervalo->picos.numElementos);
	LinkedList **listaEmisiones=new LinkedList*[intervalo->picos.numElementos];
	for(int i=0;i<intervalo->picos.numElementos;i++)//Tomamos todos los picos (el centroide de este ya es el nuevo)
		{
		NodoPico *pico=DevuelveNodo(intervalo->picos, i);
		listaEmisiones[i]=new LinkedList();
		if(pico->Ajuste.emisiones->numElements()>0)	
			{
			for(int j=0;j<pico->Ajuste.emisiones->numElements();j++)
				listaEmisiones[i]->addLast((NodoEmision *)pico->Ajuste.emisiones->getNode(j));		
			//printf("Guardamos la lista de emisiones del %d\n");
			}
		}
	//
		
	//
	taus=pico->Ajuste.Tau;
	sigmas=pico->Ajuste.Anchura;
	printf("tauEsp=%f; sigmaEsp=%f\n", taus, sigmas);
	//
	
	printf("Ajustamos\n");
	for(int i=0;i<intervalo->picos.numElementos;i++)//Tomamos todos los picos (el centroide de este ya es el nuevo)
		{
		NodoPico *pico2=DevuelveNodo(intervalo->picos, i);
		centroides[i]=pico2->Ajuste.Centroide;
		printf("centroide[%d]=%f\n", i, centroides[i]);
			
		//Cogemos el centroide o, si identificado, la media de las emisiones
		//identificadas para el centroide
		//NOTA: De momento begoña ha dicho que mejor coger sólo el del ajuste,
		//		pasar de las identificaciones
		/*if(pico2->Ajuste.identificado)
			{
			centroides[i]=0;
			for(int j=0;j<pico2->Ajuste.emisiones->numElements();j++)
				{
				centroides[i]+=((NodoEmision *)pico2->Ajuste.emisiones->getNode(j))->Energia;
				}
			centroides[i]/=pico2->Ajuste.emisiones->numElements();
			printf("El centroide vale %f\n", centroides[i]);
			}
		*/
		}
		
	Datos.ordenaVector(centroides, numGaussianas);
	int nError2=Datos.AjusteIntervaloGAML(numIntervaloEdicionAnal, centroides, numGaussianas, taus, sigmas, numPicoEdicionAnal);
	if(nError2==-3 || nError2==-5)
		{
		printf("ERROR: %d\n", nError2);
		Aviso(app_shell, "ERROR: Fitness Unaccurate, discarded");
		PonCursorNormal();
		return;
		}
	//Aquí al terminar no tiene que hacer el análisis experto sino incluir los
	//picos que están puestos sin más
	printf("Reponemos las emisiones antiguas\n");
	for(int i=0;i<intervalo->picos.numElementos;i++)
		{
		NodoPico *pico=DevuelveNodo(intervalo->picos, i);
		pico->Ajuste.emisiones=listaEmisiones[i];
		if(listaEmisiones[i]->numElements()>0)	
			{
			pico->Ajuste.identificado=true;
			pico->Ajuste.analizado=true;
			}
		}
	
	float centroidesAjustados[intervalo->picos.numElementos];
	for (int i = 1; i <= intervalo->picos.numElementos; i++) 
		{
		NodoPico *pico = DevuelveNodo(intervalo->picos, i);
		centroidesAjustados[i-1]=pico->Ajuste.Centroide;
		}
		
	// actualiza los datos del nuevo ajuste
	XmString s1, s2, cadena;
	char texto[40];
	// escribe ChiSq
	//NOTA: Quizás todos estos .5f, .2f, etc deberían ir con la función de redondeo
	s1 = XmStringCreate ("Chi\262:", "BoldNormal");
	sprintf(texto, " %.5f", intervalo->ChiSq);
	s2 = XmStringCreateLocalized(texto);
	cadena = XmStringConcat (s1, s2);
	XmStringFree(s1); XmStringFree(s2);
	XtVaSetValues (lbChiSq_adjust_edAnal, XmNlabelString, cadena, NULL);
	XmStringFree(cadena);

	// escribe Area
	s1 = XmStringCreate ("Area:", "BoldNormal");
	char numeroPresentado[80];
	printf("Área: %f(%f)\n", pico->Ajuste.area, pico->Ajuste.incerArea);
	PresentaNumero(pico->Ajuste.area, pico->Ajuste.incerArea,numeroPresentado);
	sprintf(texto, " %s", numeroPresentado);
	s2 = XmStringCreateLocalized(texto);
	cadena = XmStringConcat (s1, s2);
	XmStringFree(s1); XmStringFree(s2);
	XtVaSetValues (lbArea_adjust_edAnal, XmNlabelString, cadena, NULL);
	XmStringFree(cadena);
	
	// Escribe los parametros de la gaussiana
	sprintf(texto, "%.2f", pico->Ajuste.Centroide);
	XmTextSetString(txtCen_adjust_edAnal, texto);
	sprintf(texto, "%.2f", pico->Ajuste.Anchura);
	XmTextSetString(txtSigma_adjust_edAnal, texto);
	sprintf(texto, "%.2fkeV", Datos.DaEnergia(pico->Ajuste.Centroide));
	cadena = XmStringCreateLocalized (texto);
	XtVaSetValues (lbCenEn_adjust_edAnal, XmNlabelString, cadena, NULL);
	XmStringFree (cadena);
	
	// Escribe los parametros de la primera exponencial
	sprintf(texto, "%.2f", pico->Ajuste.Tau);
	XmTextSetString(txtTau_adjust_edAnal, texto);
	sprintf(texto, "%.2f", pico->Ajuste.Ytau);
	XmTextSetString(txtYtau_adjust_edAnal, texto);

	// Actualiza los datos de los picos en la grafica
	printf("Pulsa Adjust Edit Calib (VisCalib) llamando a CopiaAjustesAGrafica\n");
	int nError = CopiaAjustesAGrafica();
	if (nError < 0) 
		{
		Aviso(fondo_analisis, "No Memory Avalaible");
		printf(ERROR_ANALYSIS);
		printf(ERROR_NO_MEMORY);
		return;
		}
	
	ActivaAjustesGrafica(EstadoVisual.AjusteCalibracion);
	PonCursorNormal();

	if (EstadoVisual.AjusteCalibracion)	PintaGrafica(0);
	}

/******************************************************************************
 ******
 ******                             VISUALIZACIONES
 ******
 ******************************************************************************/

void EstableceShowsAnalisis(const tEstadoVisual Estado) 
	{
	XmToggleButtonSetState(show_adjust_analisis, Estado.AjusteTotalAnalisis, false);
	XmToggleButtonSetState(show_parcialAdjust_analisis, Estado.AjusteParcialAnalisis, false);
	XmToggleButtonSetState(show_autoZoom_analisis, Estado.AutoZoomAnalisis, false);
	// Activa o desactiva las graficas segun los valores
	ActivaAjustesGrafica(Estado.AjusteTotalAnalisis);
	ActivaAjustesParcialesGrafica(Estado.AjusteParcialAnalisis);
	}
