#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

//Para manejo de ficheros
#include <fstream>
#include <iostream>
// Para el tratamiento de cadenas
#include <string>
using namespace std;

#include "VisConti.h"
#include "VisAnal.h"	
#include "VisActiv.h"

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

//Para el cálculo de la actividad
Widget dlgFBD;
Widget dlgCSC;
Widget dlgAct;
Widget dlgMDA;
char *nombreFichEfi;

//Funciones para el cálculo de la actividad
void calculaAreaPicos(double *, double *, double *, int);
void calculaActividad();
void calculaActividad(int,int);
void calculaActividadRadionuclidos();
void imprimePicosRN();
void imprimeActividad();
void calculaMDA();
void imprimeMDA();
LinkedList* construyeListaRelacionados(NodoPico *nodo);
void avisoSospechosos(LinkedList *);
bool avisadoSospechosos=false;
int determinaNumeroEmisiones();



double actividad(double, double, double, double, double);
double sigmaActividad(double, double, double, double, double, double, double, double, double);

//Funciones relacionadas con los botones del la sección Compute
void pulsa_limits(Widget, XtPointer, XtPointer);
void pulsa_limits_ld(Widget, XtPointer, XtPointer);
void pulsa_limits_ld_2(Widget, XtPointer, XtPointer);
void pulsa_efficiency(Widget, XtPointer, XtPointer);
void pulsa_activity(Widget, XtPointer, XtPointer);
void pulsa_MDA(Widget, XtPointer, XtPointer);
void pulsa_CSC(Widget, XtPointer, XtPointer);

void imprimeLista(LinkedList *lista);
void imprimeNodo(NodoPico *np);
void imprimirLista(LinkedList *lista);
void ordenaLista(LinkedList *lista);

int padre(int);
bool padre(int, int, int);
int enCadena(int);
bool esPadre(int);
bool enEquilibrio(int);
int familia(int);
int rango(int);

//Para NSA-IEEE
float minDev=100.0;
float maxDev=0.0;
float minDevArea=100.0;
float maxDevArea=0.0;

// Callbacks
// Manejo de las listas (básicamente para despliegues y autoZoom)
void pulsa_lista_result_actividad(Widget, XtPointer, XtPointer);
void pulsa_edit_result_actividad (Widget, XtPointer, XtPointer);
void desplegarLista(Widget, int, string);
void cerrarLista(Widget, int, string);
int determinaRadionuclido(int);
int determinaEmision(int);
int *numSublistas=NULL;
LinkedList *radionuclidos;//Lista con todos los radionúclidos presentes en alguna identificación
LinkedList *emisiones;//Lista con las emisiones correspondientes a cada radionúclido
LinkedList *actividadesRadionuclido;
LinkedList *picosRN;//Lista con los picos correspondientes a cada radionúclido (es decir, que tienen alguna emisión en ese pico identificada)
LinkedList *listaMCE;//Lista con las matrices de covarianza de la eficiencia
bool **matrizX;
double **matrizPN;//NOTA: QUizás luego necesitemos una matriz de incertidumbres asociada
				//Depende de si hacemos un ajuste lineal bivariante o no
double *vectorNE;
double *vectorIncNE;
double *vectorCT;
double *vectorIncCT;
int numPicos, numRN;
int dne;
XmString *cadenasEmisionesRadionuclidos;

void construyeMatrizX();
void construyeMatrizPN();
void construyeVectorNE();
void construyeVectorCT();
int getNumPicos();
int getNumPicosRad(LinkedList *);

//Funciones relacionadas con la base de datos de fondo
void PedirBDFondo();
void pulsaOKBDFondo(Widget, XtPointer, XtPointer);
void pulsaCancelBDFondo(Widget, XtPointer, XtPointer);
void calculaMediaBDFondo(char *, int, int, double *, double *, double *);
char *buscaEmisionFBD(int, int, char *);
char *buscaNombreFBD(float, char *);

//Funciones para el cálculo de la MDA
void QuitaVentanaMDA(Widget widget, XtPointer client_data, XtPointer call_data);

//Funciones para el cálculo de matrices de actividad
LinkedList *calculaMatricesActividad();
void QuitaVentanaAct(Widget widget, XtPointer client_data, XtPointer call_data);

//Funciones relacionadas con el cuadro de diálogo para ficheros .rn
void pulsaOpenFile(Widget, XtPointer, XtPointer);
void pulsaOpenFile_2(Widget, XtPointer, XtPointer);
void pulsaCancelFile(Widget, XtPointer, XtPointer);
void pulsaOKFile(Widget, XtPointer, XtPointer);
void pulsaOKFile_2(Widget, XtPointer, XtPointer);
void pulsaOKFileEficiencia(Widget, XtPointer, XtPointer);

//Funciones relacionadas con el cómputo de los límites de detección
void creaDialogoLD();
void creaDialogoLD_2();
void QuitaVentanaLD(Widget widget, XtPointer client_data, XtPointer call_data);
void guardarLD(String);
void guardarLD(const char*, nodoCentroide *);
nodoCentroide *lecturaEmisiones(char *);
nodoCentroide *lecturaEnergias(char *);
void escribeProton(Widget, XtPointer, XtPointer);
void seleccionaElemento(Widget, XtPointer, XtPointer);
void seleccionaEmision(Widget, XtPointer, XtPointer);

//Funciones relacionadas con el cálculo de los límites de decisión
void creaDialogoLC(Widget widget, XtPointer client_data, XtPointer call_data);
void QuitaVentanaLC(Widget widget, XtPointer client_data, XtPointer call_data);
void calculaLimitesDecision();
void PedirBDFondoLC();

//Funciones relacionadas con el cálculo de la eficiencia
//int leeNumPars(String);
int leeNumPars(char *);
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
void eliminaRepetidas(char *);
void preparaFichero(char *);
void EstableceComputeActividad();
void reseteaActividad();

//Botones para las distintas acciones de computación de actividad
Widget action_frame_actividad, action_label_frame_actividad, action_rowcol_actividad;
Widget action_activity_actividad, //NOTA: Me encanta este nombre :). Es el ejemplo perfecto de cómo una política de nombres de convierte en mierda.
	   action_efficiency_actividad,
	   action_ld_actividad,
	   //action_ld_2_actividad,
	   action_limits_actividad,
	   action_MDA_actividad, action_CSC_actividad;
Widget rowcol_actividad;

Widget dlgLD, dlgLC;//Cuadros de diálogo del cálculo de los límites de detección y decisión
Widget lista2;//Lista de emisiones con límites de detección para radionúclidos
			//leídos de un fichero
Widget lista3;//Lista de límites de detección que se hayan calculado para radionúclidos
			//elegidos 

//Para el cálculo del ld personalizado			
Widget txt_proton; //Cuadro de texto con información acerca de número de protones
Widget listaE, listaRN; //Listas con información de las energías de emisión y de los radionúclidos
// Widgets del menu de edicion de Analisis
Widget dlgEfi;
Widget dlg_filesPar;
int proton=0;
char mensajeErrorActividad[100];

// Funciones
Widget Crea_AreaActividad (Widget);
void creaListaActividad();
void reconstruyeListaActividad();
void borraVisActiv();

//Widgets
Widget label_form_actividad, fondo_actividad;
Widget linea_activ_1, linea_activ_2;
Widget result_frame_actividad, label_result_frame_actividad, result_lista_actividad;
Widget interv_frame_actividad;

Widget Crea_AreaActividad(Widget parent) 
	{
    Arg		 args[7];
    Cardinal i = 0;
	XmString cadena;

		printf("CREA AREA ACTIVIDAD\n");
	i = 0;
    fondo_actividad = XmCreateForm (parent, "fondo_actividad", args, i);
	
	// Crea el titulo del form
	i = 0;
	cadena = XmStringCreate ("Activity", "BoldNormal");
    XtSetArg (args[i], XmNlabelString, cadena); i++;
    XtSetArg (args[i], XmNfontList, lista_fuentes); i++;
	label_form_actividad = XmCreateLabel(fondo_actividad, "label_form_actividad", args, i);
    XmStringFree (cadena);
	XtManageChild(label_form_actividad);

	// crea las lineas de separación
	i = 0;
    XtSetArg (args[i], XmNseparatorType, XmSHADOW_ETCHED_OUT); i++;
	linea_activ_1 = XmCreateSeparator(fondo_actividad, "linea_activ_1", args, i);
	linea_activ_2 = XmCreateSeparator(fondo_actividad, "linea_activ_2", args, i);
	XtManageChild(linea_activ_1);
	XtManageChild(linea_activ_2);
	
	// **** OPCIONES DE REPRESENTACION ****

	//Columna a la que se añadirá tanto la parte de visualización como la de computación
    i = 0;
    rowcol_actividad = XmCreateRowColumn(fondo_actividad, "rowcol_actividad", args, i);
    XtVaSetValues (rowcol_actividad,
		XmNbottomAttachment, XmATTACH_FORM,
		XmNbottomOffset, 10,
		XmNtopAttachment, XmATTACH_FORM,
		XmNtopOffset, 10,
		XmNrightAttachment, XmATTACH_FORM,
		XmNrightOffset, 10,
		NULL);
	
   XtManageChild(rowcol_actividad);

	// **** FIN OPCIONES DE REPRESENTACION ***


   // **** ACCIONES DE ANÁLISIS ****

   // Crea opciones para las distintas acciones de cálculo que podemos tener en
   // el análisis
   i = 0;
   action_frame_actividad = XmCreateFrame (rowcol_actividad, "action_frame_actividad", args, i);

   cadena = XmStringCreateLocalized ("Compute");
   i = 0;
   XtSetArg (args[i], XmNchildType, XmFRAME_TITLE_CHILD); i++;
   XtSetArg (args[i], XmNchildVerticalAlignment, XmALIGNMENT_CENTER); i++;
   XtSetArg (args[i], XmNlabelString, cadena); i++;
   XtSetArg (args[i], XmNfontList, lista_fuentes); i++;
   action_label_frame_actividad = XmCreateLabelGadget (action_frame_actividad, "action_label_frame_actividad", args, i);
   XmStringFree (cadena);

   i = 0;
   action_rowcol_actividad = XmCreateRowColumn(action_frame_actividad, "action_rowcol_actividad", args, i);

   // Toggle buton para cálculo de actividades

   cadena = XmStringCreateLocalized ("Decision Limits");
   i = 0;
   XtSetArg (args[i], XmNset, false); i++;
   XtSetArg (args[i], XmNlabelString, cadena); i++;
   XtSetArg (args[i], XmNfontList, lista_fuentes); i++;
	action_limits_actividad = XmCreateToggleButtonGadget (action_rowcol_actividad, "action_adjust_actividad", args, i);
	XtAddCallback (action_limits_actividad, XmNvalueChangedCallback, pulsa_limits, (XtPointer) 1);
   XmStringFree (cadena);


//felipe-
   cadena = XmStringCreateLocalized ("Detection limits from file");
   i = 0;
   XtSetArg (args[i], XmNset, false); i++;
   XtSetArg (args[i], XmNlabelString, cadena); i++;
   XtSetArg (args[i], XmNfontList, lista_fuentes); i++;
	action_ld_actividad = XmCreateToggleButtonGadget (action_rowcol_actividad, "action_adjust_actividad", args, i);
	XtAddCallback (action_ld_actividad, XmNvalueChangedCallback, pulsa_limits_ld, (XtPointer) 2);
   XmStringFree (cadena);

//felipe-
   /*cadena = XmStringCreateLocalized ("Lim. de detecc.-fichero energias");
   i = 0;
   XtSetArg (args[i], XmNset, false); i++;
   XtSetArg (args[i], XmNlabelString, cadena); i++;
   XtSetArg (args[i], XmNfontList, lista_fuentes); i++;
	action_ld_2_actividad = XmCreateToggleButtonGadget (action_rowcol_actividad, "action_adjust_actividad", args, i);
	XtAddCallback (action_ld_2_actividad, XmNvalueChangedCallback, pulsa_limits_ld_2, (XtPointer) 2);
   XmStringFree (cadena);
*/
	// Toggle buton para cálculo de eficiencia
   cadena = XmStringCreateLocalized ("Efficiency");
   i = 0;
   XtSetArg (args[i], XmNset, false); i++;
   XtSetArg (args[i], XmNlabelString, cadena); i++;
   XtSetArg (args[i], XmNfontList, lista_fuentes); i++;
	action_efficiency_actividad = XmCreateToggleButtonGadget (action_rowcol_actividad, "action_rowcol_actividad", args, i);
	XtAddCallback (action_efficiency_actividad, XmNvalueChangedCallback, pulsa_efficiency, (XtPointer) 2);
   XmStringFree (cadena);

	// Toggle buton para cálculo de eficiencia
   cadena = XmStringCreateLocalized ("Efficiency\nCorrection");
   i = 0;
   XtSetArg (args[i], XmNset, false); i++;
   XtSetArg (args[i], XmNlabelString, cadena); i++;
   XtSetArg (args[i], XmNfontList, lista_fuentes); i++;
	action_CSC_actividad = XmCreateToggleButtonGadget (action_rowcol_actividad, "action_rowcol_actividad", args, i);
	XtAddCallback (action_CSC_actividad, XmNvalueChangedCallback, pulsa_CSC, (XtPointer) 2);
   XmStringFree (cadena);

	// Toggle buton para cálculo de eficiencia
   cadena = XmStringCreateLocalized ("Activity");
   i = 0;
   XtSetArg (args[i], XmNset, false); i++;
   XtSetArg (args[i], XmNlabelString, cadena); i++;
   XtSetArg (args[i], XmNfontList, lista_fuentes); i++;
 	action_activity_actividad = XmCreateToggleButtonGadget (action_rowcol_actividad, "action_rowcol_actividad", args, i);
	XtAddCallback (action_activity_actividad, XmNvalueChangedCallback, pulsa_activity, (XtPointer) 2);
   XmStringFree (cadena);

	// Toggle buton para cálculo de Actividad
   cadena = XmStringCreateLocalized ("MDA");
   i = 0;
   XtSetArg (args[i], XmNset, false); i++;
   XtSetArg (args[i], XmNlabelString, cadena); i++;
   XtSetArg (args[i], XmNfontList, lista_fuentes); i++;
   
	action_MDA_actividad = XmCreateToggleButtonGadget (action_rowcol_actividad, "action_rowcol_actividad", args, i);
  	XtAddCallback (action_MDA_actividad, XmNvalueChangedCallback, pulsa_MDA, (XtPointer) 2);
   XmStringFree (cadena);

 		
   if( !(EstadoTrabajo.AnalisisEficiencia) ) 
   	{
	printf("Lo deshabilitamos\n");
    XtSetSensitive(action_activity_actividad, false);
    XtSetSensitive(action_MDA_actividad, false);
	}
	printf("****************************************\n");
	printf("AnlisisLD es %d\n", EstadoTrabajo.AnalisisLD);
	printf("AnlisisEficiencia es %d\n", EstadoTrabajo.AnalisisEficiencia);
   if(EstadoTrabajo.AnalisisLD)	XmToggleButtonSetState(action_limits_actividad, true, false);
   if(EstadoTrabajo.AnalisisEficiencia)	XmToggleButtonSetState(action_efficiency_actividad, true, false);

	XtManageChild(action_label_frame_actividad);
	XtManageChild(action_efficiency_actividad);
	XtManageChild(action_activity_actividad);
	XtManageChild(action_CSC_actividad);
	XtManageChild(action_MDA_actividad);
	XtManageChild(action_ld_actividad);
	//XtManageChild(action_ld_2_actividad);
	XtManageChild(action_limits_actividad);
	XtManageChild(action_rowcol_actividad);
	XtManageChild(action_frame_actividad);

	// **** FIN OPCIONES DE ACCIÓN ***

   // Crea seccion resultados de ajuste
   i = 0;
	XtSetArg (args[i], XmNorientation, XmVERTICAL); i++;
	XtSetArg (args[i], XmNentryAlignment, XmALIGNMENT_CENTER); i++;
   result_frame_actividad = XmCreateRowColumn (fondo_actividad, "result_frame_actividad", args, i);
   cadena = XmStringCreate ("Activity List", "BoldNormal");
   i = 0;
   XtSetArg (args[i], XmNfontList, lista_fuentes); i++;
   XtSetArg (args[i], XmNlabelString, cadena); i++;
   label_result_frame_actividad = XmCreateLabelGadget (result_frame_actividad, "label_result_frame_actividad", args, i);
   XmStringFree (cadena);

	i = 0;
	XtSetArg (args[i], XmNlistSizePolicy, XmCONSTANT); i++;
	XtSetArg (args[i], XmNselectionPolicy, XmEXTENDED_SELECT); i++;
	XtSetArg (args[i], XmNvisibleItemCount, 9); i++;
	XtSetArg (args[i], XmNscrollBarDisplayPolicy, XmSTATIC); i++;
     	 	XtSetArg (args[i], XmNfontList, lista_fuentes); i++;
	result_lista_actividad = XmCreateScrolledList(result_frame_actividad, "result_lista_actividad", args, i);
	XtAddCallback (result_lista_actividad, XmNdefaultActionCallback, pulsa_edit_result_actividad, NULL);
	XtAddCallback (result_lista_actividad, XmNextendedSelectionCallback, pulsa_lista_result_actividad, NULL);

	XtVaSetValues(result_lista_actividad,
		XmNbottomAttachment, XmATTACH_WIDGET, XmNbottomOffset, 10,
		NULL);
	
   	XtVaSetValues (result_frame_actividad,
		XmNbottomAttachment, XmATTACH_FORM,
		XmNbottomOffset, 20,
		XmNtopAttachment, XmATTACH_FORM,
		XmNtopOffset, 10,
		XmNrightAttachment, XmATTACH_WIDGET,
		XmNrightWidget, action_frame_actividad,		
		XmNleftAttachment, XmATTACH_FORM,
		XmNleftOffset, 10,
		NULL);

	XtManageChild(label_result_frame_actividad);
	XtManageChild(result_lista_actividad);
	XtManageChild(result_frame_actividad);


	// coloca los widgets
   	XtVaSetValues (linea_activ_1,
		XmNleftAttachment, XmATTACH_FORM,
		XmNtopAttachment, XmATTACH_FORM,
		XmNrightAttachment, XmATTACH_FORM,
		NULL);
   	XtVaSetValues (linea_activ_2,
		XmNleftAttachment, XmATTACH_FORM,
		XmNrightAttachment, XmATTACH_FORM,
		XmNbottomAttachment, XmATTACH_FORM,
		NULL);
   	XtVaSetValues (label_form_actividad,
		XmNleftAttachment, XmATTACH_FORM,
		XmNbottomAttachment, XmATTACH_WIDGET, XmNbottomWidget, linea_activ_2,
		XmNleftOffset, 10,
		NULL);
   	XtVaSetValues (fondo_actividad,
		XmNrightAttachment, XmATTACH_FORM,
		XmNleftAttachment, XmATTACH_FORM,
		XmNtopAttachment, XmATTACH_FORM,
		XmNbottomAttachment, XmATTACH_FORM,
		NULL);
	
	//Ponemos los botones según los estados de trabajo
	//XmToggleButtonGadgetSetState(action_MDA_actividad, true, true);
	
//	creaListaActividad();
	return fondo_actividad;
	}

//*********** RESETEA ACTIVIDAD ****************
/*	Elimina todos el análisis de actividad. Debe llamarse cada
	vez que cambie de alguna manera el conjunto de picos identificados
	(si algún pico queda sin identificar, si cambia alguna identificación)
*/
void reseteaActividad()
	{
	EstadoTrabajo.AnalisisLD		=false;
	EstadoTrabajo.AnalisisActividad	=false;
	EstadoTrabajo.AnalisisMDA		=false;
	EstableceComputeActividad();
	}

//****************** ESTABLECE COMPUTE ACTIVIDAD ************************
/*	Configura los botones de la interfaz COMPUTE de acuerdo al estado de 
	trabajo actual.
*/
void EstableceComputeActividad()
	{
	printf("*****************INICIABOTONESCOMPUTE***********************\n");
	printf("AnalisisLD es %d\n", EstadoTrabajo.AnalisisLD);
	printf("AnalisisEficiencia es %d\n", EstadoTrabajo.AnalisisEficiencia);
	XmToggleButtonSetState(action_ld_actividad, EstadoTrabajo.AnalisisLD, false);
	//XmToggleButtonSetState(action_ld_2_actividad, EstadoTrabajo.AnalisisLD, false);
	XmToggleButtonSetState(action_limits_actividad, EstadoTrabajo.AnalisisLD, false);
	XmToggleButtonSetState(action_efficiency_actividad, EstadoTrabajo.AnalisisEficiencia, false);
   	XmToggleButtonSetState(action_activity_actividad, EstadoTrabajo.AnalisisActividad, false);
	if(EstadoTrabajo.AnalisisEficiencia)
		{
		XtSetSensitive(action_activity_actividad, true);
		XtSetSensitive(action_MDA_actividad, true);
		}
	else
		{
		XtSetSensitive(action_activity_actividad, false);
		XtSetSensitive(action_MDA_actividad, false);
		}
	XmToggleButtonSetState(action_MDA_actividad, EstadoTrabajo.AnalisisMDA, false);
	}
	
/******************************************************************************
 ******
 ******                            LISTAS
 ******
 ******************************************************************************/
/*	Funciones de manejo de la lista de radionúclidos	*/
	
	
//***************** CREA LISTA ACTIVIDAD *************************
/*	Transforma la lista intervalo(picos) a una radionúclidos(emisiones).
	Por ejemplo:
		-interval [123-134]	2 peaks
			Ra226	124,44 keV
			Th230	130,01 keV
	Pasa a ser:
		-Ra226 1 emission
			124,44 keV
		-Th230 1 emission
			130,01 keV
	*/
void creaListaActividad()
	{
	printf("CREA LISTA ACTIVIDAD\n");
	if(Datos.continuo==NULL)	{printf("Volvemos, no hay continuo\n");	return;}
	if(Datos.listaPicos.numElementos==0){printf("Volvemos, no hay picos identificados\n");	return;}
	
	bool addPico=false;
	
	if(picosRN!=NULL)	picosRN->deleteAll();
	delete picosRN;
	if(radionuclidos!=NULL)	radionuclidos->deleteAll();
	delete radionuclidos;
	if(emisiones!=NULL)	emisiones->deleteAll();
	delete emisiones;

	radionuclidos=new LinkedList();
	emisiones=new LinkedList();
	actividadesRadionuclido=new LinkedList();
	picosRN=new LinkedList();
	LinkedList *rnSospechosos=new LinkedList();
	
	//0) Si creamos la lista de nuevo, no tendremos eficiencia ni límites calculados
	// Toma los valores de energías en los que estamos trabajando del continuo
	int c=1;
	while(c<Datos.numCanales && Datos.continuo[c]==0)	c++;
	float inicio=Datos.DaEnergia(c);
	while(c<Datos.numCanales && Datos.continuo[c]!=0)	c++;
	float fin=Datos.DaEnergia(c);
	
	//1) Tomamos todos los radionúclidos distintos que hay
	for(int i=1;i<=Datos.listaPicos.numElementos;i++)
		{
		NodoPicos *intervalo = DevuelveNodo(Datos.listaPicos, i);
		for(int j=1;j<=intervalo->picos.numElementos;j++)
			{
			NodoPico *pico = DevuelveNodo(intervalo->picos, j);
			if(pico->Ajuste.identificado && ! pico->Ajuste.calibrado && !pico->Ajuste.invisible)//Si está identificado, pero sólo en la fase de análisis
				//NUEVO: If para poner sólo los que tienen un elemento identificado para la emisión
				if(pico->Ajuste.emisiones->numElements()==1)
				{
				//
				for(int k=0;k<pico->Ajuste.emisiones->numElements();k++)
					{//Tomamos la primera emisión y la introducimos en una lista de radionúclidos
					NodoEmision *ne=(NodoEmision *)pico->Ajuste.emisiones->getNode(k);
					int ce=ne->codElemento;
					int crn=LaraBD.GetEmissor(ce);
					
					if(ce!=-1)
						{
						if(!estaEmisionMasProbable(crn, inicio, fin) && !rnSospechosos->isElementInList((void *)crn))	
							{
							char name[3];
							LaraBD.GetNameElement(crn, name);
							rnSospechosos->addLast((void *)crn);
							avisadoSospechosos=false;
							}
					
						if(!radionuclidos->isElementInList((void *)crn))
							{	
							printf("Añadimos el elemento %d,%d\n", LaraBD.GetMassNumber(crn), LaraBD.GetAtomicNumber(crn));								
							radionuclidos->addLast((void *) crn);
							}
						}
					//felipe. Emisión que no está en Lara. 
					else{
						crn = -1;
						if(!radionuclidos->isElementInList((void *)crn)){
							printf("No esta en Lara. Añadimos el elemento %s, %f\n", ne->nombre, ne->probabilidad);	
							/*XmString cadena;
							char emi[3];
							sprintf(emi, "%s", ne->nombre);								
							printf("Añadimos emisión: %s\n", emi);
							cadena=XmStringCreateLocalized(emi);*/
							radionuclidos->addLast((void *) crn);
							}
						}

					}






				//del if NUEVO
				}
			}
		}
	printf("Tenemos %d radionúclidos en la lista\n", radionuclidos->numElements());
	printf("Generamos un aviso de posibles sospechosos\n");
	if(rnSospechosos->numElements()>0)	
		{
		ordenaLista(rnSospechosos);
		avisoSospechosos(rnSospechosos);
		avisadoSospechosos=true;
		}
		
	//1.b) Hacemos una comprobación de que todos los radionúclidos analizados
	//		estén realmente según el criterio de la probabilidad
	//comprobarElementosInexistentes();
		
	//2) Ordenamos los radionúclidos para que la lista quede elegante
	printf("Ordenamos e imprimimos la lista de radionúclidos\n");
	ordenaLista(radionuclidos);
	imprimirLista(radionuclidos);
	printf("Creamos numSublistas\n");
	numSublistas=new int[radionuclidos->numElements()];//NOTA: MM
	for(int i=0;i<radionuclidos->numElements();i++)	numSublistas[i]=0;
	
	printf("Borramos lo que hubiera antes\n");
	//3) Construimos la lista con las sublistas necesarias
	XmListDeleteAllItems(result_lista_actividad);
	
	XmString mas, total, cadena;
	
/*	if(cadenasEmisionesRadionuclidos!=NULL)	
		for(int i=0;i<dne;i++)	XmStringFree(cadenasEmisionesRadionuclidos[i]);
	delete [] cadenasEmisionesRadionuclidos;
	cadenasEmisionesRadionuclidos=NULL;
		
	dne=determinaNumeroEmisiones();
	cadenasEmisionesRadionuclidos=new XmString[dne];*/
	char emi[100];
	printf("Vamos a tener %d emisiones, correspondientes a %d radion\n", dne, radionuclidos->numElements());
	
	int cne=0;
	
	for(int l=0;l<radionuclidos->numElements();l++)
		{//Para cada radionúclido creamos una sublista con sus emisiones
		printf("Creamos sublista de emisiones para el radionúclido %d\n", l);
		int crn=(int)radionuclidos->getNode(l);
		if (crn != -1) cadena=XmStringNameElement(crn, "SubIndiceBold", "BoldNormal");//Añadimos entrada
		else //felipe
			cadena = XmStringCreateLocalized("otros");
			//cadena = XmStringNameElement(crn, "SubIndiceBold", "BoldNormal");
		mas=XmStringCreateLocalized("+");
		total=XmStringConcat(mas, cadena);
			
		XmListAddItemUnselected(result_lista_actividad, total, 0);
		XmStringFree(cadena); XmStringFree(mas); XmStringFree(total); 
			
		printf("Creamos lista\n");
		LinkedList *list=new LinkedList();
		printf("Nuestra lista tiene %d elementos\n", Datos.listaPicos.numElementos);
		//Buscamos todas sus emisiones
		for(int i=1;i<=Datos.listaPicos.numElementos;i++)
			{
			NodoPicos *intervalo = DevuelveNodo(Datos.listaPicos, i);
			for(int j=1;j<=intervalo->picos.numElementos;j++)
				{
//				printf("pico %d de intervalo %d con %d picos\n", j, i, intervalo->picos.numElementos);
				NodoPico *pico = DevuelveNodo(intervalo->picos, j);
				if(pico->Ajuste.identificado && ! pico->Ajuste.calibrado && !pico->Ajuste.invisible)//Si está identificado, pero sólo en la fase de análisis
					{
				//NUEVO: If para poner sólo los que tienen un elemento identificado para la emisión
				//		Luego en la siguiente fase, pondremos todos, tras calcular matrices de actividad
					if(pico->Ajuste.emisiones->numElements()==1)
						{
					//
						for(int k=0;k<pico->Ajuste.emisiones->numElements();k++)
							{//Tomamos la primera emisión y la introducimos en una lista de radionúclidos
							NodoEmision *ne=(NodoEmision *)pico->Ajuste.emisiones->getNode(k);
							int ce=ne->codElemento;
							int crnEmision=LaraBD.GetEmissor(ce);
							if(ce!=-1 && crnEmision==crn)
								{
								if(pico->Ajuste.actividad==0)	sprintf(emi, "   %.3f keV (%.1f%%)", ne->Energia, LaraBD.GetProbability(ce));
								else							
									{
									char numeroPresentado[80];
									PresentaNumero(pico->Ajuste.actividad, pico->Ajuste.sigmaActividad, numeroPresentado);
//									sprintf(emi, "   %.3f keV (%.1f%%)    %s Bq (area=%.2f)", ne->Energia, LaraBD.GetProbability(ce), numeroPresentado, pico->Ajuste.areaSinFondo);								
									sprintf(emi, "   %.3f keV (%.1f%%)    %s Bq", ne->Energia, LaraBD.GetProbability(ce), numeroPresentado);								
									}
								printf("Añadimos emisión: %s\n", emi);
								//cadenasEmisionesRadionuclidos[cne]=XmStringCreateLocalized(emi);
								//emisiones->addLast((void *)cadenasEmisionesRadionuclidos[cne]);
								cadena=XmStringCreateLocalized(emi);
								emisiones->addLast((void *)cadena);
								
								//
								actividadesRadionuclido->addLast((void *)pico);
								//
									
								numSublistas[l]++;					
								cne++;									
								addPico=true;
								}
								//felipe. Emision que no esta en lara
							if (crn == -1 && ce == -1){
								if(pico->Ajuste.actividad==0)	sprintf(emi, "   %.3f keV (%.4f%%) %s", ne->Energia, ne->probabilidad, ne->nombre);
								else							
									{
									char numeroPresentado[80];
									PresentaNumero(pico->Ajuste.actividad, pico->Ajuste.sigmaActividad, numeroPresentado);
//									sprintf(emi, "   %.3f keV (%.1f%%)    %s Bq (area=%.2f)", ne->Energia, LaraBD.GetProbability(ce), numeroPresentado, pico->Ajuste.areaSinFondo);								
									sprintf(emi, "   %.3f keV (%.4f%%)    %s Bq %s", ne->Energia, ne->probabilidad, numeroPresentado, ne->nombre);								
									}
								printf("felipe 1: Añadimos emisión: %s\n", emi);
								//cadenasEmisionesRadionuclidos[cne]=XmStringCreateLocalized(emi);
								//emisiones->addLast((void *)cadenasEmisionesRadionuclidos[cne]);
								cadena=XmStringCreateLocalized(emi);
								emisiones->addLast((void *)cadena);
								
								//
								actividadesRadionuclido->addLast((void *)pico);
								//
									
								numSublistas[l]++;					
								cne++;									
								addPico=true;
								}
							}//Para cada emisión
							
					//del if NUEVO
						}//if sólo una emisión
					}//if pico identificado, etc
				//
				if(addPico)//Nos interesa almacenar los picos relacionados con un RN a través de alguna emision
					{
					printf("Añadimos pico\n");
					addPico=false;
					list->addLast((void *)pico);						
					}
				}//Para cada pico del intervalo
			}//Para cada intervalo
		printf("Añadimos lista con %d valores\n", list->numElements());
		picosRN->addLast(list);
		}//Para cada radionúclido		
	
	//Construimos la matriz pico-radionuclido
	printf("Construimos matriz X\n");
	construyeMatrizX();
		
	XtManageChild(result_lista_actividad);
		
	//4) Calculamos las matrices de actividad.
	/*NOTA: De momento no estamos realizando el cálculo de las matrices de actividad
	if(Datos.matricesActividad==NULL)	Datos.matricesActividad=new LinkedList();
	else								Datos.matricesActividad->deleteAll();
	Datos.matricesActividad=calculaMatricesActividad();
	*/
		
	//NOTA: Provisional, para depuración (deberá ir con su propio botón de acción o automáticamente)
	//calculaActividadRadionuclidos();
	//
	printf("Liberamos sospechosos\n");
	rnSospechosos->deleteAll();
	delete rnSospechosos;

	//Si hay suficiente información para calcular actividades, lo hacemos
	//		(nos vale con tener eficiencias y áreas e identificaciones)
	if(EstadoTrabajo.AnalisisEficiencia)	calculaActividad();
	//
/*	printf("Termina creaListaActividad\n");
	printf("Hemos creado una lista con %d elementos\n", picosRN->numElements());
	for(int i=0;i<picosRN->numElements();i++)
		{
		LinkedList *lista=(LinkedList *)picosRN->getNode(i);
		printf("Lista %d tiene %d elementos\n", i, lista->numElements());
		}*/
	}	

//***************** RECONSTRUYE LISTA ACTIVIDAD **********************
/*	Reconstruye la lista de actividad añadiendo los beq. de la actividad
	y de la mda si estuvieran calculados
*/
void reconstruyeListaActividad()
	{
	printf("RECONSTRUYE LISTA ACTIVIDAD\n");
	//2) Ordenamos los radionúclidos para que la lista quede elegante
	ordenaLista(radionuclidos);
	imprimirLista(radionuclidos);
	numSublistas=new int[radionuclidos->numElements()];//NOTA: MM
	for(int i=0;i<radionuclidos->numElements();i++)	numSublistas[i]=0;
	emisiones->deleteAll();
	actividadesRadionuclido->deleteAll();
	
	//3) Construimos la lista con las sublistas necesarias
	XmListDeleteAllItems(result_lista_actividad);
	/*if(cadenasEmisionesRadionuclidos!=NULL)	
		for(int i=0;i<dne;i++)	XmStringFree(cadenasEmisionesRadionuclidos[i]);
	delete [] cadenasEmisionesRadionuclidos;
	cadenasEmisionesRadionuclidos=NULL;
		
	dne=determinaNumeroEmisiones();*/
	
	XmString mas, total, cadena;
	mas=XmStringCreateLocalized("+");
	//cadenasEmisionesRadionuclidos=new XmString[dne];
	char emi[100];
		
	int cne=0;
	
	for(int l=0;l<radionuclidos->numElements();l++)
		{//Para cada radionúclido creamos una sublista con sus emisiones
		int crn=(int)radionuclidos->getNode(l);
		XmString cadena;
		
		if (crn != -1) 
			cadena=XmStringNameElement(crn, "SubIndiceBold", "BoldNormal");//Añadimos entrada
		//felipe
		else 
			cadena = XmStringCreateLocalized("otros");
			//cadena=XmStringNameElement(crn, "SubIndiceBold", "BoldNormal");//Añadimos entrada
			//crn, "SubIndiceBold", "BoldNormal"	
		total=XmStringConcat(mas, cadena);
			
		XmListAddItemUnselected(result_lista_actividad, total, 0);
		XmStringFree(cadena); XmStringFree(total); 
			
		//Buscamos todas sus emisiones
		for(int i=1;i<=Datos.listaPicos.numElementos;i++)
			{
			NodoPicos *intervalo = DevuelveNodo(Datos.listaPicos, i);
			for(int j=1;j<=intervalo->picos.numElementos;j++)
				{
				NodoPico *pico = DevuelveNodo(intervalo->picos, j);
				if(pico->Ajuste.identificado && !pico->Ajuste.calibrado && !pico->Ajuste.invisible)
				if(pico->Ajuste.emisiones->numElements()==1)
					{
					NodoEmision *ne=(NodoEmision *)pico->Ajuste.emisiones->getNode(0);
					int ce=ne->codElemento;
					int crnEmision=LaraBD.GetEmissor(ce);
					if(ce!=-1 && crnEmision==crn)
						{
						char numeroPresentado[80];
						PresentaNumero(pico->Ajuste.actividad, pico->Ajuste.sigmaActividad, numeroPresentado);
						if(pico->Ajuste.actividad==0)	sprintf(emi, "   %.3f keV (%.1f%%)", ne->Energia, LaraBD.GetProbability(ce));
						else					
							{
							if(pico->Ajuste.mda==0)	sprintf(emi, "   %.3f keV (%.1f%%)    %s Bq", ne->Energia, LaraBD.GetProbability(ce), numeroPresentado);								
							else					
								{
								char MDAr[20];
								redondeaDouble(pico->Ajuste.mda, 4, MDAr);
								sprintf(emi, "   %.3f keV (%.1f%%)    %s Bq", ne->Energia, LaraBD.GetProbability(ce), numeroPresentado);								
								}
							}
						printf("Añadimos emisión %s\n", emi);
						//cadenasEmisionesRadionuclidos[cne]=XmStringCreateLocalized(emi);
						//emisiones->addLast((void *)cadenasEmisionesRadionuclidos[cne]);
						cadena=XmStringCreateLocalized(emi);
						emisiones->addLast((void *)cadena);
						actividadesRadionuclido->addLast((void *)pico);
						//cne++;
						numSublistas[l]++;							
						}//Si es el rad. que buscamos
						//felipe, si no está en lara
						//felipe. Emision que no esta en lara
					if (crn == -1 && ce == -1)
						{
						//felipe. Determina probabilidad si no está en Lara.
						string cad(BDIdentificaciones);						ne->probabilidad = determinarProbabilidad(ne->Energia,(char *)cad.c_str());
						//


						if(pico->Ajuste.actividad==0)	sprintf(emi, "   %.3f keV (%.4f%%) %s", ne->Energia, ne->probabilidad, ne->nombre);
						else							
									{
									char numeroPresentado[80];
									PresentaNumero(pico->Ajuste.actividad, pico->Ajuste.sigmaActividad, numeroPresentado);
//									sprintf(emi, "   %.3f keV (%.1f%%)    %s Bq (area=%.2f)", ne->Energia, LaraBD.GetProbability(ce), numeroPresentado, pico->Ajuste.areaSinFondo);								
									sprintf(emi, "   %.3f keV (%.4f%%)    %s Bq %s", ne->Energia, ne->probabilidad, numeroPresentado, ne->nombre);								
									}
						printf("felipe 2: Añadimos emisión: %s\n", emi);
						//cadenasEmisionesRadionuclidos[cne]=XmStringCreateLocalized(emi);
						//emisiones->addLast((void *)cadenasEmisionesRadionuclidos[cne]);
						cadena=XmStringCreateLocalized(emi);
						emisiones->addLast((void *)cadena);
						
								//
						actividadesRadionuclido->addLast((void *)pico);
								//
									
						numSublistas[l]++;					
						}
					}//Si tiene una sola emision
				}//Para cada pico del intervalo
			}//Para cada intervalo
		}//Para cada radionúclido		
	
	XtManageChild(result_lista_actividad);
	XmStringFree(mas);	
	
	//4) Calculamos las matrices de actividad.
	if(Datos.matricesActividad==NULL)	Datos.matricesActividad=new LinkedList();
	else								Datos.matricesActividad->deleteAll();
	Datos.matricesActividad=calculaMatricesActividad();
	
	//NOTA: Provisional, para depuración (deberá ir con su propio botón de acción o automáticamente)
	//calculaActividadRadionuclidos();
	//
	}	

//*************** DETERMINA NUMERO EMISIONES ***************
int determinaNumeroEmisiones()
	{
	int cont=0;
	for(int l=0;l<radionuclidos->numElements();l++)
		{
		int crn=(int)radionuclidos->getNode(l);
		//Buscamos todas sus emisiones
		for(int i=1;i<=Datos.listaPicos.numElementos;i++)
			{
			NodoPicos *intervalo = DevuelveNodo(Datos.listaPicos, i);
			for(int j=1;j<=intervalo->picos.numElementos;j++)
				{
				NodoPico *pico = DevuelveNodo(intervalo->picos, j);
				if(pico->Ajuste.identificado && !pico->Ajuste.calibrado && !pico->Ajuste.invisible)
					if(pico->Ajuste.emisiones->numElements()==1)
						{
						NodoEmision *ne=(NodoEmision *)pico->Ajuste.emisiones->getNode(0);
						int ce=ne->codElemento;
						int crnEmision=LaraBD.GetEmissor(ce);
						if(ce!=-1 && crnEmision==crn)	cont++;
						}
				}
			}
		}
			
	}
		
	
//********************* AVISO SOSPECHOSOS ****************
/*	Crea un cuadro de diálogo con todos los radionúclidos sospechosos de 
	haber sido mal analizados ya que no aparece su emisión de mayor probabilidad
	*/
void avisoSospechosos(LinkedList *sospechosos)
	{
	Arg args[7];
	Cardinal i=0;
	Widget avisoSosp;
	Widget listaAct, col, frameAct, f1;
	Widget labelSosp;
	double e, a, sa;
	char texto[200];
	XmString cad;
	XmString cadena;

	if(avisadoSospechosos)	return;
	printf("MOSTRAMOS LOS DATOS OBTENIDOS PARA LA ACTIVIDAD\n");
	
	Cardinal k = 0;
	XtSetArg (args[k], XmNtitle, "Warning"); k++;
	XtSetArg (args[k], XmNmwmDecorations, MWM_DECOR_BORDER|MWM_DECOR_TITLE|MWM_DECOR_MENU); k++;
	XtSetArg (args[k], XmNmwmFunctions, MWM_FUNC_MOVE|MWM_FUNC_CLOSE); k++;
	XtSetArg (args[k], XmNbaseWidth, 500); k++;
	XtSetArg (args[k], XmNbaseHeight, 300); k++;
	XtSetArg (args[k], XmNmaxWidth, 500); k++;
	XtSetArg (args[k], XmNmaxHeight, 300); k++;
	
	avisoSosp = XmCreateFormDialog(app_shell, "avisoSosp", args, k);
	
	k=0;
	XtSetArg (args[k], XmNbaseWidth, 500); k++;
	XtSetArg (args[k], XmNbaseHeight, 300); k++;
	XtSetArg (args[k], XmNmaxWidth, 500); k++;
	XtSetArg (args[k], XmNmaxHeight, 300); k++;
	col=XmCreateRowColumn(avisoSosp, "ColumnaAc", args, k);
	
	k=0;
	labelSosp=XmCreateLabelGadget(col, "Most probable emissions related to these elements haven't been identified", args, k);
	
	i=0;
	XtSetArg (args[i], XmNwidth, 500); i++;
	XtSetArg (args[i], XmNheight, 300); i++;
	f1=XmCreateFrame(col, "Frame simple", args, i);
 	
	cadena = XmStringCreateLocalized ("Suspected Elements");
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
	XtSetArg (args[k], XmNbaseWidth, 500); k++;
	XtSetArg (args[k], XmNbaseHeight, 200); k++;
	XtSetArg (args[k], XmNwidth, 500); k++;
	XtSetArg (args[k], XmNheight, 200); k++;
    listaAct = XmCreateScrolledList(f1, "lista de actividades", args, k);
	
	if(Datos.continuo==NULL)	return;
	// Toma los valores de energías en los que estamos trabajando del continuo
	int c=1;
	while(c<Datos.numCanales && Datos.continuo[c]==0)	c++;
	float inicio=Datos.DaEnergia(c);
	while(c<Datos.numCanales && Datos.continuo[c]!=0)	c++;
	float fin=Datos.DaEnergia(c);
	
	for(int i=0;i<sospechosos->numElements();i++)
		{
		int rn=(int)sospechosos->getNode(i);
		char name[3];
		LaraBD.GetNameElement(rn, name);
		int m=LaraBD.GetMassNumber(rn);
		int emiMaxProb=LaraBD.daEmisionConMayorProbabilidad(rn, inicio, fin);
		if(LaraBD.IsEqui(rn))	sprintf(texto, "%s%d(e) has maximum probability at %.2f keV (%.2f%)", name, m, LaraBD.GetEnergy(emiMaxProb), LaraBD.GetProbability(emiMaxProb));
		else					sprintf(texto, "%s%d has maximum probability at %.2f keV (%.2f%)", name, m, LaraBD.GetEnergy(emiMaxProb), LaraBD.GetProbability(emiMaxProb));
		
		cadena=XmStringCreateLocalized(texto);
		XmListAddItemUnselected(listaAct, cadena, 0); 
		XmStringFree(cadena);
		}
	
	XtManageChild(labelSosp);
	XtManageChild(listaAct);
	XtManageChild(col);
	XtManageChild(f1);
	XtManageChild(avisoSosp);
	}
	
//****************** GET NUM PICOS *****************
/*	Nos dice el número de picos que tenemos identificados.
	NOTA: Quizá por los de calibración debiera decirme también los ajustados
	*/
int getNumPicos()
	{
	int np=0;
	for(int i=1;i<=Datos.listaPicos.numElementos;i++)
		{
		NodoPicos *intervalo = DevuelveNodo(Datos.listaPicos, i);

		for(int j=1;j<=intervalo->picos.numElementos;j++)
			{
			NodoPico *pico=DevuelveNodo(intervalo->picos, j);
		//	if(pico->Ajuste.identificado)	np++;
//			if(pico->Ajuste.identificado && !pico->Ajuste.calibrado)	np++;
			if(pico->Ajuste.identificado && !pico->Ajuste.calibrado && !pico->Ajuste.invisible)	np++;
			}
		}			
	return np;
	}

//**************** CONSTRUYE MATRIZ X **************
/*	Construye una matriz X que simplemente tiene un valor true si el radionúclido
	columna participa en la emisión del pico fila.
	NOTA: Va a pasar a construir además una matrizPN (Pico-Núclido) que contiene
	en esas mismas posiciones el valor Pk(Ei)*ef(Ei)
*/	
void construyeMatrizX()
	{
	//0) Determinamos dimensiones
	numPicos=getNumPicos();
	numRN=radionuclidos->numElements();
	int np=0;
	
	//1) Reservamos memoria
	matrizX=new bool*[numPicos];//Filas=picos
	matrizPN=new double*[numPicos];//Filas=picos
	for(int i=0;i<numPicos;i++)	{matrizX[i]=new bool[numRN]; matrizPN[i]=new double[numRN];}
	for(int i=0;i<numPicos;i++)		for(int j=0;j<numRN;j++)	
		{	matrizX[i][j]=false;	matrizPN[i][j]=0.0;}
		
	//2) Construimos la matrizX
	for(int i=1;i<=Datos.listaPicos.numElementos;i++)
		{
		NodoPicos *intervalo = DevuelveNodo(Datos.listaPicos, i);
		for(int j=1;j<=intervalo->picos.numElementos;j++)//Para cada pico
			{
			NodoPico *pico = DevuelveNodo(intervalo->picos, j);
//			if(pico->Ajuste.identificado)//Si está identificado
//			if(pico->Ajuste.identificado && !pico->Ajuste.calibrado)//Si está identificado
			if(pico->Ajuste.identificado && !pico->Ajuste.calibrado && !pico->Ajuste.invisible)
				{
				for(int k=0;k<picosRN->numElements();k++)//Contrastamos con todos los radionúclidos
					{
					LinkedList *lista=(LinkedList *)picosRN->getNode(k);
					for(int l=0;l<lista->numElements();l++)
						{
						NodoPico *nopi=(NodoPico *)lista->getNode(l);
						//printf("Comparando %f con %f\n", Datos.DaEnergia(nopi->Ajuste.Centroide), Datos.DaEnergia(pico->Ajuste.Centroide));
						if(Datos.DaEnergia(nopi->Ajuste.Centroide)==Datos.DaEnergia(pico->Ajuste.Centroide))	
							{
							matrizX[np][k] =true;
							//Buscamos la emisión relacionada con este pico que sea del radionúcilo en cuestión
							double prob=0.0;
							int rn=(int)radionuclidos->getNode(k);//Código del radionúclido
							for(int m=0;m<nopi->Ajuste.emisiones->numElements();m++)
								{
								NodoEmision *noemi=(NodoEmision *)nopi->Ajuste.emisiones->getNode(m);
								if(noemi->codElemento!=-1 && LaraBD.GetEmissor(noemi->codElemento)==rn)	
									{
									prob=LaraBD.GetProbability(noemi->codElemento);//->ésta es
									m=nopi->Ajuste.emisiones->numElements();
									}
								}
							//NOTA: No es el ajuste, es la corrección, pero no sé si 
							//		se refiere a la corrección de la eficiencia o a
							//		(más probablemente) la corrección por suma en coincidencia
							//		El caso es que no sé si son lo mismo, y sólo tengo la
							//		de la eficiencia, así que es la que voy a poner temporalmente
							//matrizPN[np][k]=nopi->Ajuste.eficiencia*prob;
							matrizPN[np][k]=nopi->Ajuste.correccion*prob;
							}
						}
					}
				np++;
				}
			}
		}
	//3) Imprimimos la matrizX para comprobación:
/*	printf("Por filas los picos, esta es la matriz de relaciones:\n");
	for(int j=0;j<numRN;j++)	printf("%d ", j);
	printf("\n");
	for(int i=0;i<numPicos;i++)
		{
		for(int j=0;j<numRN;j++)
			{
			//if(matrizX[i][j]==true)	printf("X ");
			if(matrizX[i][j]==true)	printf("%d ",j);
			else					printf("  ");
			}
		printf("|\n");	
		}

	printf("Por filas los picos, esta es la matriz de relaciones:\n");
	for(int j=0;j<numRN;j++)	printf("%d ", j);
	printf("\n");
	for(int i=0;i<numPicos;i++)
		{
		for(int j=0;j<numRN;j++)
			{
			printf("%.2f ", matrizPN[i][j]);
			}
		printf("\n");	
		}
	*/
	}
	
//**************** CONSTRUYE MATRIZ PN **************
/*	Construye una matriz PN (Pico-Núclido) que contiene la multiplicación de
	Pk*C, para los elementos relacionados según la matriz X, donde:
	
		Pk[i,j] es la probabilidad de emisión del radionúclido i con emisión en
				el pico j
		C		es la corrección por eficiencia del pico i 
				NOTA: Igual para todos los radionúclidos???
*/	
void construyeMatrizPN()
	{
	printf("CONSTRUYE MATRIZ PN\n");
	//0) Determinamos dimensiones
	int numPicos=getNumPicos();
	int numRN=radionuclidos->numElements();
	int np=0;
	
	//1) Reservamos memoria
	matrizX=new bool*[numPicos];//Filas=picos
	matrizPN=new double*[numPicos];//Filas=picos
	for(int i=0;i<numPicos;i++)	{matrizX[i]=new bool[numRN]; matrizPN[i]=new double[numRN];}
	for(int i=0;i<numPicos;i++)		for(int j=0;j<numRN;j++)	
		{	matrizX[i][j]=false;	matrizPN[i][j]=0.0;}
		
	//2) Construimos la matrizX
	for(int i=1;i<=Datos.listaPicos.numElementos;i++)
		{
		NodoPicos *intervalo = DevuelveNodo(Datos.listaPicos, i);
		for(int j=1;j<=intervalo->picos.numElementos;j++)//Para cada pico
			{
			NodoPico *pico = DevuelveNodo(intervalo->picos, j);
//			if(pico->Ajuste.identificado)//Si está identificado
//			if(pico->Ajuste.identificado && !pico->Ajuste.calibrado)//Si está identificado
			if(pico->Ajuste.identificado && !pico->Ajuste.calibrado && !pico->Ajuste.invisible)
				{
				for(int k=0;k<picosRN->numElements();k++)//Contrastamos con todos los radionúclidos
					{
					LinkedList *lista=(LinkedList *)picosRN->getNode(k);
					for(int l=0;l<lista->numElements();l++)
						{
						NodoPico *nopi=(NodoPico *)lista->getNode(l);
						//printf("Comparando %f con %f\n", Datos.DaEnergia(nopi->Ajuste.Centroide), Datos.DaEnergia(pico->Ajuste.Centroide));
						if(Datos.DaEnergia(nopi->Ajuste.Centroide)==Datos.DaEnergia(pico->Ajuste.Centroide))	
							{
							matrizX[np][k] =true;
							//Buscamos la emisión relacionada con este pico que sea del radionúcilo en cuestión
							double prob=0.0;
							int rn=(int)radionuclidos->getNode(k);//Código del radionúclido
							for(int m=0;m<nopi->Ajuste.emisiones->numElements();m++)
								{
								NodoEmision *noemi=(NodoEmision *)nopi->Ajuste.emisiones->getNode(m);
								if(noemi->codElemento!=-1 && LaraBD.GetEmissor(noemi->codElemento)==rn)	
									{//Si hay más de una emisión del mismo radionúclido relacionada con este pico, las sumamos
									prob+=LaraBD.GetProbability(noemi->codElemento);
									//m=nopi->Ajuste.emisiones->numElements();
									}
								}
							matrizPN[np][k]=nopi->Ajuste.correccion*prob;
							}
						}
					}
				np++;
				}
			}
		}
	//3) Imprimimos la matrizPN para comprobación:
	printf("Por filas los picos, esta es la matriz Pico-Núclido:\n");
	for(int j=0;j<numRN;j++)	printf("%d ", j);
	printf("\n");
	for(int i=0;i<numPicos;i++)
		{
		for(int j=0;j<numRN;j++)
			{
			if(matrizPN[i][j]==0)	printf("0 ");
			else					printf("X ");
			//printf("%.2f ", matrizPN[i][j]);
			}
		printf("\n");	
		}
	}
	
//*************** CONSTRUYE VECTOR NE ************************
/*	Esta función construye el vector N/e*ct, es decir un vector
	de dim=número de picos, con su vector de incertidumbres asociado.
	N=área de cada pico
	e=eficiencia de cada pico
	c(t)=tiempo de cada pico.
	
	N se saca de Ajuste.area y e de Ajuste.eficiencia. Deberían estar previamente 
	calculados.
	c(t) sale de la ecuación:
		t es Tv o c(t)						1-e^(-landa*Tv)
		t es c(t) si T1/2<350Tv -> c(t)= --------------------
										    landa
	//NOTA: Preguntar a Begoña si seguimos este mismo criterio, o si es siempre c(t)	
	//NOTA: OJO: Cambio de planes. c(t) depende de T1/2 que no varía con los picos sino
	//		con los radionúclidos, así que más bien debe estar multiplicando al vector de 
	//		actividades. Con esto construyeNEC se convierte en NE y simplemente es
	//		dividir N/e y calcular su incertidumbre
			Por otra parte creamos construyeVectorCT() que nos creará el vector c(t)
			para cada radionúclido.
	*/
void construyeVectorNE()
	{
	//0) Determinamos dimensiones
	int numPicos=getNumPicos();
	int np=0;
	
	//1) Reservamos memoria
	vectorNE=new double[numPicos];//Filas=picos
	vectorIncNE=new double[numPicos];//Filas=picos
	for(int i=0;i<numPicos;i++)		vectorIncNE[i]=vectorNE[i]=0.0;
		
	//2) Construimos ambos vectores
	for(int i=1;i<=Datos.listaPicos.numElementos;i++)
		{
		NodoPicos *intervalo = DevuelveNodo(Datos.listaPicos, i);
		for(int j=1;j<=intervalo->picos.numElementos;j++)//Para cada pico
			{
			NodoPico *pico = DevuelveNodo(intervalo->picos, j);
//			if(pico->Ajuste.identificado && !pico->Ajuste.calibrado)//Si está identificado pero no en calibración
			if(pico->Ajuste.identificado && !pico->Ajuste.calibrado && !pico->Ajuste.invisible)
				{
				double N=pico->Ajuste.area;
				double e=pico->Ajuste.eficiencia;
				double sN=pico->Ajuste.incerArea;
				double se=pico->Ajuste.sigmaEficiencia;
					
				vectorNE[np]	=N/e;
				vectorIncNE[np]=(N/e)*(N/e)*( (sN*sN)/(N*N) + (se*se)/(e*e) );
				printf("sN es %f y se es %f\n", sN, se);
				printf("N/e[%d]=%f/%f=%f(%f)\n",np, N,e,vectorNE[np],vectorIncNE[np]);
				np++;
				}
			}
		}
	printf("fin de construye vector ne\n");
	}

//*************** CONSTRUYE VECTOR CT ***********
/*	Construye un vector con los tiempos c(t) asociados a 
	cada radionúclido según su vida media
	c(t) sale de la ecuación:
		t es Tv o c(t)						1-e^(-landa*Tv)
		t es c(t) si T1/2<350Tv -> c(t)= --------------------
										    landa
	*/
void construyeVectorCT()
	{
	int numRN=radionuclidos->numElements();
	int np=0;
	
	//1) Reservamos memoria
	vectorCT=new double[numRN];//Filas=picos
	vectorIncCT=new double[numRN];//Filas=picos
		
	//2) Construimos los vectores de tiempos
	for(int i=0;i<radionuclidos->numElements();i++)
		{
		int crn=(int)radionuclidos->getNode(i);
		//
		//int emisor=LaraBD.GetEmissor(nodo->Ajuste.codElemento);
/*		int emisor=LaraBD.GetEmissor(ce);
		double tmedio =LaraBD.GetDecayTime(emisor);
		double stmedio=LaraBD.GetIncDecayTime(emisor);*/
		double tmedio =LaraBD.GetDecayTime(crn);
		double stmedio=LaraBD.GetIncDecayTime(crn);
		double tvivo=(double)Datos.tiempo;
		double ct=0.0;
		double t =0.0;
		char nom[3];
		LaraBD.GetNameElement(crn, nom);
		printf("Para el elemento %s el tmedio es %f(%f)\n", nom,tmedio,stmedio);
		printf("El tiempo vivo viene siendo %f\n", tvivo);
		printf("Comprobamos si %f (tmedio) < %f (350*tvivo)\n", tmedio, 350*tvivo);
		if(tmedio < 350*tvivo)
			{//En este caso hacemos una corrección del tiempo
			double landa=log(2.0)/tmedio;
			ct=(1-exp(-landa*tvivo))/landa;
			}
		if(ct!=0)	t=ct;
		else		t=tvivo;
		
		vectorCT[i]=ct;
		//Por último, calculamos la incertidumbre de c(t)
		double sct=fabs(1/log(2.0) - (1/log(2.0)+t/tmedio)*exp(-log(2.0*t/tmedio))  )*stmedio;
		vectorIncCT[i]=sct;
		printf("VectorCT[%d]=%f(%f)\n",i,vectorCT[i],vectorIncCT[i]);
		}
	}
	
//*************** IMPRIME PICOS RN *************
void imprimePicosRN()
	{
	for(int i=0;i<picosRN->numElements();i++)
		{	
		printf("Picos relacionados con rad%d:\n", i);	
		LinkedList *l=(LinkedList *)picosRN->getNode(i);
		for(int j=0;j<l->numElements();j++)
			{
			NodoPico *np=(NodoPico *)l->getNode(j);
			printf("	%f keV\n", Datos.DaEnergia(np->Ajuste.Centroide));
			}
		}
	}
	
//**************** IMPRIMIR LISTA **************
void imprimirLista(LinkedList *lista)
	{
	for(int i=0;i<lista->numElements();i++)
		{
		int v=(int)lista->getNode(i);
		char name[3];
		int mn=0;
		LaraBD.GetNameElement(v, name);
		mn=LaraBD.GetMassNumber(v);
		printf("Elemento %d (codigo %d) es %s%d\n", i, v, name, mn);
		}	
	printf("Termina imprimirLista\n");		
	}

//**************** ORDENA LISTA **************
void ordenaLista(LinkedList *lista)
	{
	int MAX=lista->numElements();
	int valores[MAX];
	for(int i=0;i<MAX;i++)	valores[i]=10000;
		
	for(int i=0;i<MAX;i++)
		{
		int v=(int)lista->getNode(i);
		for(int j=0;j<MAX;j++)
			if(v<valores[j])	
				{
				for(int k=MAX-1;k>=j;k--)	valores[k+1]=valores[k];
				valores[j]=v;
				j=MAX;
				}
		}		
	lista->deleteAll();
	for(int i=0;i<MAX;i++)	lista->addLast((void *)valores[i]);
	}
	
//******************** PULSA EDIT RESULT ACTIVIDAD **********************
/*	Despliega una lista si es necesario.
	Las sublistas en este caso contienen todas las emisiones correspondientes
	al radionúclido de la superlista. Esta lista no es editable más allá.
	*/
void pulsa_edit_result_actividad(Widget widget, XtPointer client_data, XtPointer call_data) 
	{
	printf("PULSA EDIT RESULT ACTIVIDAD\n");
	// busca el elemento seleccionado
	int *posiciones, numSel, nPos = 0;
	if (!XmListGetSelectedPos(result_lista_actividad, &posiciones, &numSel)) 
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
	printf("Pico seleccionado \n");
	char *selection=NULL;
	XmListCallbackStruct *cbs = (XmListCallbackStruct *) call_data;
	XmStringGetLtoR(cbs->item, XmSTRING_DEFAULT_CHARSET, &selection);
	printf("Hecha la selección\n");
	if(selection!=NULL)
		{
		string cad(selection);
		//cout << "La cadena seleccionada es " << cad << endl;
		if(cad[0]=='+')
			desplegarLista(result_lista_actividad, nPos, cad);
		else if(cad[0]=='-')
			cerrarLista(result_lista_actividad, nPos, cad);
		XtFree(selection);
		}
	else	printf("La selección es nula!\n");
	printf("Terminamos de abrir\n");
	}

//**************** CERRAR LISTA ********************
/*	Pasa de 
		-Ra226
			123,221 keV
	A
		+Ra226
	*/
void cerrarLista(Widget lista, int nPos, string cad)
	{
	XmString entry;
	XmString cadena, mas;

	//printf(" CERRAR LISTA en posición %d\n", nPos);
	//0)Determinamos radionúclido y emisión a partir de la posición en la lista
	int numRadionuclido=determinaRadionuclido(nPos);
	int numEmision=determinaEmision(nPos);
	//1)Tomamos la posición que hay que desplegar y substituimos - por +
	int crn=(int)radionuclidos->getNode(numRadionuclido);
	if (crn != -1)	cadena=XmStringNameElement(crn, "SubIndiceBold", "BoldNormal");//Añadimos entrada
	else //felipe
		cadena=XmStringCreateLocalized("otros");
		//cadena=XmStringNameElement(crn, "SubIndiceBold", "BoldNormal");//Añadimos entrada
	mas=XmStringCreateLocalized("+");
	entry=XmStringConcat(mas, cadena);

	XmListDeletePos(lista, nPos);
	XmListAddItem(lista, entry, nPos);
	XmStringFree(entry); XmStringFree(mas); XmStringFree(cadena);
	XmListSelectPos(lista, nPos, true);
	
	//2)Ahora que sabemos el radionúclido procedemos a mostrar todas sus emisiones
	for(int i=0;i<numSublistas[numRadionuclido];i++)	XmListDeletePos(lista, nPos+1); 
	}
	
//******************* DETERMINA RADIONÚCLIDO *****************
/*	Devuelve la posición del radionúclido en la lista enlazada
	correspondiente a la posición seleccionada en la lista
	*/
int determinaRadionuclido(int nPos)
	{
	//2)Miramos qué posición ocupa el radionúclido desplegado en nuestra lista de
	//de radionúclidos
	Arg args[6];
	Cardinal n;
	XmString *items;
	char *text=NULL;
	int numItems;
	char emi[100];
	
	n=0;
	XtSetArg(args[n], XmNitems, &items); n++;
	XtSetArg(args[n], XmNitemCount, &numItems); n++;
	XtGetValues(result_lista_actividad, args, n);

	int numRadionuclido=0, numEmision=0;
	
	for(int i=0;i<nPos-1;i++)//Contamos los radionúclidos hasta el que estamos
		{
		XmStringGetLtoR(items[i], XmFONTLIST_DEFAULT_TAG, &text);
		if(text[0]=='-' || text[0]=='+')	
			{
			numEmision+=numSublistas[numRadionuclido];
			numRadionuclido++;
			}
		
		if(text!=NULL)	XtFree(text);
		}
		
	return numRadionuclido;
	}
	
//******************* DETERMINA EMISIÓN *****************
/*	Devuelve la posición del radionúclido en la lista enlazada
	correspondiente a la posición seleccionada en la lista
	*/
int determinaEmision(int nPos)
	{
	//2)Miramos qué posición ocupa el radionúclido desplegado en nuestra lista de
	//de radionúclidos
	Arg args[6];
	Cardinal n;
	XmString *items;
	char *text=NULL;
	int numItems;
	char emi[100];
	
	n=0;
	XtSetArg(args[n], XmNitems, &items); n++;
	XtSetArg(args[n], XmNitemCount, &numItems); n++;
	XtGetValues(result_lista_actividad, args, n);

	int numRadionuclido=0, numEmision=0;
	
	for(int i=0;i<nPos-1;i++)//Contamos los radionúclidos hasta el que estamos
		{
		XmStringGetLtoR(items[i], XmFONTLIST_DEFAULT_TAG, &text);
		if(text[0]=='-' || text[0]=='+')	
			{
			numEmision+=numSublistas[numRadionuclido];
			numRadionuclido++;
			}
		
		if(text!=NULL)	XtFree(text);
		}
		
	return numEmision;
	}
	
//********************* DESPLEGAR LISTA **********************
void desplegarLista(Widget lista, int nPos, string cad)
	{
	XmString entry;
	XmString cadena, menos;
	printf("DESPLEGAR LISTA\n");
	//0)Determinamos radionúclido y emisión a partir de la posición en la lista
	int numRadionuclido=determinaRadionuclido(nPos);
	int numEmision=determinaEmision(nPos);
	printf("Radionúclido %d, emisión %d\n", numRadionuclido, numEmision);
	//1)Tomamos la posición que hay que desplegar y substituimos + por -
	int crn=(int)radionuclidos->getNode(numRadionuclido);
	if (crn != -1)	cadena=XmStringNameElement(crn, "SubIndiceBold", "BoldNormal");//Añadimos entrada
	else //felipe
		//cadena=XmStringNameElement(crn, "SubIndiceBold", "BoldNormal");//Añadimos entrada
		cadena=XmStringCreateLocalized("otros");
	menos=XmStringCreateLocalized("-");
	entry=XmStringConcat(menos, cadena);

	//printf("Tenemos %d emisiones\n", emisiones->numElements());
	//for(int i=0;i<emisiones->numElements();i++)	printf("%s\n", (XmString)emisiones->getNode(i));
	XmListDeletePos(lista, nPos);
	XmListAddItem(lista, entry, nPos);
	XmStringFree(entry); XmStringFree(menos); XmStringFree(cadena);
	XmListSelectPos(lista, nPos, true);
	
	printf("Cadena montada con el -\n");
	//2)Ahora que sabemos el radionúclido procedemos a mostrar todas sus emisiones
	int max=numSublistas[numRadionuclido];
	float *e=new float[max];
	float *se=new float[max];
	float m=0.0, sm=0.0;
	printf("Tenemos %d emsiiones para este radionúclido\n", max);
	for(int i=0;i<max;i++)
		{
		printf("pillamos la emisión\n");
		entry=(XmString)emisiones->getNode(numEmision+i);
		XmListAddItem(lista, entry, nPos+i+1);
		printf("pillamos la actividad\n");
		NodoPico *np=(NodoPico *)actividadesRadionuclido->getNode(numEmision+i);
		e[i] =(float)np->Ajuste.actividad;
		se[i]=(float)np->Ajuste.sigmaActividad;
		}
	media(max,e,se,m,sm);
	printf("Media es %f(%f)\n", m,sm);
	delete [] e;	delete [] se;
	}

//********************** PULSA LISTA RESULT ACTIVIDAD ******************
/* Se activa cuando se selecciona un elemento de la lista, para
	remarcarlo en el gráfico si la opción de zoom está activada
*/
void pulsa_lista_result_actividad(Widget widget, XtPointer client_data, XtPointer call_data) 
	{
	int interv=0, pic=0;
	int *posiciones, numSel, nPos = 0;

//	printf("PULSA LISTA RESULT ACTIVIDAD\n");
	if (!XmListGetSelectedPos(result_lista_actividad, &posiciones, &numSel)) 
		return;
	
	nPos = *posiciones;
	XtFree((char *)posiciones);
	
	// si la opcion de autoZoom esta activada amplia la zona
	if (EstadoVisual.AutoZoomAnalisis) 
		{
		int ic;
		XmStringTable items;
		char *text=NULL;
	
		XtVaGetValues(result_lista_actividad, XmNitems, &items, XmNitemCount, &ic,NULL);
		XmStringGetLtoR(items[nPos-1], XmFONTLIST_DEFAULT_TAG, &text);
		if(text[0]==' ')//Estamos en una emision
			{			
			NodoPicos *picos;
			NodoPico *nodo;
			NodoEmision *ne;
			int interv=0, pic=0;

			string cad(text);
			cad=cad.substr(3);
			cad=cad.substr(0,cad.find(" "));
			float energia=atof(cad.c_str());
			int canalEnergia=(int)Datos.DaCanal(energia);
				
			for(int i=1;i<=Datos.listaPicos.numElementos;i++)
				{
				picos=DevuelveNodo(Datos.listaPicos, i);
				for(int j=1;j<=picos->picos.numElementos;j++)//Con esto consigo que los muestre todos, aunque no que los calcule
					{
					nodo      = DevuelveNodo(picos->picos, j);
//					if(nodo->Ajuste.identificado)
					if(nodo->Ajuste.identificado && !nodo->Ajuste.calibrado && !nodo->Ajuste.invisible)
						{
						for(int k=0;k<nodo->Ajuste.emisiones->numElements();k++)
							{
							ne=(NodoEmision *)nodo->Ajuste.emisiones->getNode(k);
							if(ne->Energia<energia+0.001 && ne->Energia>energia-0.001)//Tenemos q dar esta pequeña holgura porque reducimos a milésimas la precisión en nuestra lista
								{
								interv=i;
								pic=j;
								j=picos->picos.numElementos+1;	
								i=Datos.listaPicos.numElementos+1;
								k=nodo->Ajuste.emisiones->numElements();
								}
							}
						}
					}//for picos
				}//for intervalos
			int ancho = 2*(picos->limSuperior - picos->limInferior)/3;
			MarcaZoom(picos->limInferior - ancho, picos->limSuperior + ancho);
			
			
			if(picos->picos.numElementos>1 && pic!=0) 
			//En este caso debemos mostrar en amarillo el pico dentro del intervalo
				{
				CopiaAjustesParcialesAGrafica(interv, pic);
				ActivaAjustesParcialesGrafica(EstadoVisual.AjusteParcialAnalisis);
				if (EstadoVisual.AjusteParcialAnalisis)	PintaGrafica(0);
				}
			}//if(text=' ')

		XtFree(text);
		}
	}

//************** PULSA MDA ********************
/* Se activa cuando se pulsa "MDA" para calcular las mínimas actividades
	detectables de los picos identificados.
	La fórmula del cálculo de la actividad es:
	
				    N
	A(e)= ---------------------
		   Psigma*Tv*Csc*Ef(e)
	
	Donde:
		-N=área del pico, calculada como Área - Fondo.
			Los valores de las áreas los tenemos calculados pero necesitamos
			hacer el cálculo del fondo.
		-Psigma
		-Tv=Tiempo de medición vivo. Es un parámetro que deberíamos obtener del
			detector a través del fichero de espectro o de una entrada del usuario.
		-Csc=Factor de corrección de la eficiencia. Se encuentra, ordenado por energías
			en ficheros cs-*.dat. Por defecto tomamos el fichero con la misma extensión
			que el que se usa para la eficiencia. Si no se encuentra, se pide por uno.
		-Ef(e)=Eficiencia, calculado al pulsar Efficiency. Su cálculo habilita este
				botón.	
	*/
void pulsa_MDA(Widget widget, XtPointer client_data, XtPointer call_data) 
	{
	if(XmToggleButtonGadgetGetState(action_MDA_actividad)==false)
		{//Si ya estaba pulsado, lo despulsamos (y quitamos la ventana)
		printf("Despulsamos MDA\n");
		XtUnmanageChild(dlgMDA);
		//XmToggleButtonGadgetSetState(action_MDA_actividad, true, true);
		}
	else
		{
		printf("Pulsamos MDA\n");
		//Ahora calculamos los coeficientes de corrección de la eficiencia
		//El fichero de eficiencias ya debería estar del cálculo de la eficiencia
		//recogeFactoresCorreccion(nombreFichEfi);
		//printf("Volvemos de todo el cálculo\n");
		
		//Por último calculamos la actividad
		//NOTA: Debería estar dentro del recogeFactoresCorreccion
		
		//Por si hubiera alguno pico ajustado tras cargar la BD de fondo	
		Datos.calculaLC();
		calculaMDA();
		}
	}

//**************** PULSA CSC ******************
/*	Saca un cuadro de diálogo para que le indiquemos el fichero que contiene
	los factores de corrección (archivo cs-*.dat), y los recoge y almacena.
	*/
void pulsa_CSC(Widget, XtPointer, XtPointer)
	{
	printf("PULSA CSC\n");
	if(XmToggleButtonGadgetGetState(action_CSC_actividad)==false)
		{//Si ya estaba pulsado, lo despulsamos (y quitamos la ventana)
		printf("Despulsamos CSC\n");
		//XmToggleButtonGadgetSetState(action_MDA_actividad, true, true);
		}
	else
		{
		printf("Pulsamos CSC\n");
		if(nombreFichEfi!=NULL)	printf("El nombre del fichero de eficiencias es %s\n", nombreFichEfi);
		recogeFactoresCorreccion(nombreFichEfi);
		}
	}
	
bool IsActiveActivity() 
	{
   	return XtIsSensitive(result_frame_actividad);
	}

/*******************************************************************************
*																			   *
*						 CÁLCULO DE LA EFICIENCIA   			               *
*																			   *
*******************************************************************************/
//************** PULSA EFFICIENCY **********************
/* 23-VIII
	Esta función realiza el cálculo de la eficiencia.
	La eficiencia es una función Ef(Energía) para la
	energía de cada pico.
	El cálculo de la actividad de cada pico identificado en la calibración
	atiende a la siguiente fórmula:
	
	A(En)= N/Psigma*Tv*Csc*Ef(En)	Donde:
		
		-N=área del pico, calculada como Área - Fondo.
			Los valores de las áreas los tenemos calculados pero necesitamos
			hacer el cálculo del fondo.
		-Psigma
		-Tv=Tiempo de medición vivo. Es un parámetro que deberíamos obtener del
			detector a través del fichero de espectro o de una entrada del usuario.
		-Csc
		-Ef(En)=Eficiencia, que es lo que calcula esta función.	
			La eficiencia está implementada en el fichero calcefa.f en fortran,
			será necesario traducirlo a C
*/
void pulsa_efficiency(Widget widget, XtPointer client_data, XtPointer call_data) 
	{
	if(XmToggleButtonGadgetGetState(action_efficiency_actividad)==false)
		{//Si ya estaba pulsado, quitamos la ventana si existiera
		if(dlgEfi!=NULL)	XtUnmanageChild(dlgEfi);
		printf("Despulsamos Efficiency\n");
		return;
		}

	//Pedimos elegir un fichero con la tabla de parámetros:
	Arg			args[7];
   	Cardinal	i = 0;

	printf("Pulsado Efficiency\n");
	i = 0;
	XtSetArg (args[i], XmNtitle, "Use efficiency parameters ..."); i++;
	dlg_filesPar = XmCreateFileSelectionDialog (app_shell, "dlg_files", args, i);
	XtAddCallback (dlg_filesPar, XmNcancelCallback, pulsaCancelFile, NULL);
	XtAddCallback (dlg_filesPar, XmNokCallback, pulsaOKFileEficiencia, NULL);
	
	//Ponemos el fichero de preferencias
	char *UltAnalDirOpen = Preferences.GetEfficiencyDir();
	XmString patron;
	printf("Directorio de eficiencias: %s\n", UltAnalDirOpen);
	if (UltAnalDirOpen != NULL) 
		{
		patron = XmStringCreateLocalized(UltAnalDirOpen);
		XtVaSetValues (dlg_filesPar, XmNdirectory, patron, NULL);
		printf("Lo ponemos como directorio\n");
		XmStringFree (patron);
		}

	patron=XmStringCreateLocalized((char *)FILTER_FICH_EFFICIENCY);
	XtVaSetValues (dlg_filesPar, XmNpattern, patron, NULL);
	XmStringFree(patron);
	
	XtManageChild(dlg_filesPar);
	
	
	/*
	if(Datos.matricesActividad==NULL)	Datos.matricesActividad=new LinkedList();
	else								Datos.matricesActividad->deleteAll();
	Datos.matricesActividad=calculaMatricesActividad();
	*/
	}

//************** PULSA OK FILE EFICIENCIA **********************
	/* Leemos toda la información del fichero de eficiencia y 
	y calculamos la eficiencia con ella, todo a través de distintas
	funciones.
	Los ficheros tienen extensión .par y la siguiente estructura:
	
	[2 líneas en blanco]
	CALIBRACION:ef-am2


	Calidad del ajuste: chisq   0.81187518959424
 	PARAMETROS DEL AJUSTE
 	NUMERO DE PARAMETROS:   N
    -3.0979676867517
    [...]
	-5.0596850795587
 	NODOS DE LA FUNCION SPLINE
     3.8185691016769
     [...]
	 7.6009024595421
 	ELEMENTOS DE LA MATRIZ DE COVARIANZA
     5.0946015514577D-04
     [...] 
	 1.5792338265901D-04
	*/
void pulsaOKFileEficiencia(Widget widget, XtPointer client_data, XtPointer call_data) 
	{
	char *dir = NULL;
	XmFileSelectionBoxCallbackStruct *cbs = (XmFileSelectionBoxCallbackStruct *) call_data;
	Arg args[7];
	Cardinal i=0;
	Widget listaEfi, col, f1;
	double e, efi, sefi;
	char texto[200];
	XmString cadena;

	printf("PULSA OK FILE EFICIENCIA\n");
	
	Cardinal k = 0;
	XtSetArg (args[k], XmNtitle, "Efficiencies"); k++;
	XtSetArg (args[k], XmNmwmDecorations, MWM_DECOR_BORDER|MWM_DECOR_TITLE|MWM_DECOR_MENU); k++;
	XtSetArg (args[k], XmNmwmFunctions, MWM_FUNC_MOVE|MWM_FUNC_CLOSE); k++;
	XtSetArg (args[k], XmNbaseWidth, 400); k++;
	XtSetArg (args[k], XmNbaseHeight, 300); k++;
	XtSetArg (args[k], XmNmaxWidth, 400); k++;
	XtSetArg (args[k], XmNmaxHeight, 300); k++;
	printf("Vamos a crear el formulario\n");
	dlgEfi = XmCreateFormDialog(app_shell, "dlgEfi", args, k);
	XtAddCallback (XtParent(dlgEfi), XmNpopdownCallback, QuitaVentanaEfi, NULL);
	
	k=0;
	XtSetArg (args[k], XmNbaseWidth, 400); k++;
	XtSetArg (args[k], XmNbaseHeight, 300); k++;
	XtSetArg (args[k], XmNmaxWidth, 400); k++;
	XtSetArg (args[k], XmNmaxHeight, 300); k++;
	col=XmCreateRowColumn(dlgEfi, "ColumnaLD", args, k);
	
	printf("Creamos el simple\n");
	i=0;
	XtSetArg (args[i], XmNwidth, 400); i++;
	XtSetArg (args[i], XmNheight, 300); i++;
	f1=XmCreateFrame(col, "Frame simple", args, i);
 	
	cadena = XmStringCreateLocalized ("Computed Efficiencies");
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
    listaEfi = XmCreateScrolledList(f1, "lista de emisiones", args, k);
	
    if(nombreFichEfi!=NULL)	delete [] nombreFichEfi;
  	XmStringGetLtoR(cbs->value, XmFONTLIST_DEFAULT_TAG, &nombreFichEfi);
    XmStringGetLtoR(cbs->dir, XmFONTLIST_DEFAULT_TAG, &dir);

	//Leemos cada una de las informaciones que tiene el fichero:
	printf("leemos numpars\n");
	printf("Poniendo el nombre del fichero de eficiencias\n");
	if(Datos.nombreEficiencia!=NULL)	delete[] Datos.nombreEficiencia;
	Datos.nombreEficiencia=new char[strlen(nombreFichEfi)+1];
	strcpy(Datos.nombreEficiencia, nombreFichEfi);
	
	int numpar=leeNumPars(nombreFichEfi);
	double *par;
	par=new double[numpar];
	if(par==NULL)
		{
		printf("ERROR: No hay memoria para reservar double[%d]\n", numpar);
		return;
		}						
		
	printf("leemos pars\n");
	for(int i=0;i<numpar;i++)	par[i]=-1;
	leePars(nombreFichEfi, numpar, par);
	double *nodos;
	nodos=new double[numpar+1];
	if(nodos==NULL)
		{
		printf("ERROR: No hay memoria para reservar double[%d]\n", numpar+1);
		return;
		}						
	for(int i=0;i<numpar+1;i++)	nodos[i]=-1;
	printf("Leemos nodos efi\n");
	leeNodosEfi(nombreFichEfi, numpar+1, nodos);
	double **mat;
	mat=new double* [numpar];
	if(par==NULL)
		{
		printf("ERROR: No hay memoria para reservar double*[%d]\n", numpar);
		return;
		}						
	
	for(int i=0;i<numpar;i++)	mat[i]=new double [numpar];
	if(mat==NULL)
		{
		printf("ERROR: No hay memoria para reservar double[%d]\n", numpar);
		return;
		}						
	printf("leemos matriz efi\n");
	leeMatrizEfi(nombreFichEfi, numpar, mat);
	printf("calculamos eficiencia\n");
	//Ya tenemos todos los datos necesarios para calcular la eficiencia
	Datos.calculaEficiencia(numpar, par, nodos, mat);


	if(EstadoTrabajo.Actual==ESTADO_ACTIVIDAD)
		{		
		printf("estado actividad. Entra a calcular matriz de covarianza \n");	
		//NOTA: Hay que decirle que solo haga el cálculo de la matriz de covarianza
		//		y el vector NE si estamos en actividad		
		listaMCE=Datos.calculaMatrizCovarianzaEficiencia();
		//------------PROVISIONAL HASTA CALCULA ACT Radionuclid
		construyeVectorNE();

		calculaActividad();
		//------------
		}
	//Liberamos todos los punteros
	printf("Liberamos punteros\n");
	for(int i=0;i<numpar;i++)	delete [] mat[i];
	delete [] mat; 	
	if(nodos!=NULL)	delete [] nodos;
	if(par!=NULL)	delete [] par;
	
	printf("Comenzamos a construir la lista\n");
	for(int i=1;i<=Datos.listaPicos.numElementos;i++)
		{
		NodoPicos *picos=DevuelveNodo(Datos.listaPicos, i);
		for(int j=1;j<=picos->picos.numElementos;j++)//Con esto consigo que los muestre todos, aunque no que los calcule
			{
			NodoPico *nodo   = DevuelveNodo(picos->picos, j);

//			if(nodo->Ajuste.identificado && !nodo->Ajuste.calibrado)
			if(nodo->Ajuste.identificado && !nodo->Ajuste.calibrado && !nodo->Ajuste.invisible)
				{
				e=Datos.DaEnergia(nodo->Ajuste.Centroide);
				efi=nodo->Ajuste.eficiencia;
				sefi=nodo->Ajuste.sigmaEficiencia;

				/*char rsefi[20];
				redondeaDouble(sefi, 2, rsefi);
				sprintf(texto, "Peak of %f keV has efficiency=%f (%s)", e, efi, rsefi);
				*/
				char presentacion[80];
				PresentaNumero(efi, sefi, presentacion);
				sprintf(texto, "Peak of %f keV has efficiency=%s", e, presentacion);
				
				cadena=XmStringCreateLocalized(texto);
				XmListAddItemUnselected(listaEfi, cadena, 0); 
				XmStringFree(cadena);
				
				}	
			}
		}//del bucle for

	//Cerramos el cuadro de diálogo de los ficheros y mostramos una ventana
	//con una lista de las eficiencias calculadas.
//	XtUnmanageChild(dlg_filesPar);
	XtUnmanageChild(widget);

	XtManageChild(listaEfi);
	XtManageChild(col);
	XtManageChild(f1);
	XtManageChild(dlgEfi);
	
	EstadoTrabajo.AnalisisEficiencia=true;
   	//Con las eficiencias calculadas, tenemos la información para calcular la
	//actividad de los picos
	XtSetSensitive(action_MDA_actividad, true);
	XtSetSensitive(action_activity_actividad, true);
		
	// Indica cambio en el trabajo
	EstadoTrabajo.TrabajoCambios = true;
	PonTituloFichCambiado();
	//----------
	if(equilibrioDeterminado)	
		{
		equilibrioDeterminado=false;
		}
	//
	printf("Trabajo Cambiado\n");
	}
	
//********************* ESCRIBE PROTON ******************************
//Se llama cada vez que se modifica el campo txt_proton para el número de
//protones del radionúclido en la búsqueda personalizada
void escribeProton(Widget widget, XtPointer client_data, XtPointer call_data) 
	{
	char *texto;
	cSearchLaraBD founds;
	int num=0;
	char nombre[3];
		
	XmString entry;
	
	proton=0;
	XmListDeleteAllItems(listaRN);
	XmListDeleteAllItems(lista3);

	if( strcmp(texto=XmTextGetString(txt_proton), "") )
		{
		proton=atoi(texto);
		printf("Buscando radionúclidos con %d protones\n", proton);
		founds=LaraBD.SearchElements(-1,-1,proton,proton,false,-1,false,-1,"");
		//founds=LaraBD.SearchElements(-1,-1,-1,-1,false,-1,false-1,"U");
		num=founds.GetFirstResult();
		printf("Hemos encontrado %d radionúclidos\n", founds.GetNumItemsFound());
		while(num!=-1)
			{
			printf("Hemos encontrado radionúclido de número atómico %d\n", LaraBD.GetAtomicNumber(num));
			// make an entry list widget
			LaraBD.GetNameElement(num, nombre); 
			printf("Su nombre es %s\n", nombre);
			
			entry = XmStringCreateLocalized(nombre);
			
			XmListAddItemUnselected(listaRN, entry, 0);
			XmStringFree(entry);
		
			num=founds.GetNextResult();
			}
		}
	}

//**************** SELECCIONA ELEMENTO ********************
//Se llama cada vez que se selecciona un elemento de listaRN 
//en la búsqueda personalizada
void seleccionaElemento(Widget widget, XtPointer client_data, XtPointer call_data) 
	{
	cSearchLaraBD founds;
	XmString entry;
	int num=0;
	String selection;
	
  	XmListCallbackStruct *cbs = (XmListCallbackStruct *) call_data;
	XmStringGetLtoR(cbs->item, XmSTRING_DEFAULT_CHARSET, &selection);
	XmListDeleteAllItems(listaE);
	XmListDeleteAllItems(lista3);
	
	if(selection!=NULL)
		{
		if(strlen(selection)>3)
			{
			string str(selection);
			int ind=0;
			//Si tiene extensión se la quitamos también
			//NOTA: Lo de (eq.) ahora es (e)
			if((ind=str.find("(eq.)"))!=-1)	
				{
				str=str.substr(0,ind-1);
				cout << str << endl;
				}
			selection=(char *)str.c_str();
			printf("Tras quitarlos nos queda |%s|\n", selection);
			//Misma búsqueda salvo que buscamos en la lista de descendientes
			founds=LaraBD.SearchEmissions(-1,-1,-1,-1,-1,-1,-1,proton,proton,false,-1,false,-1,selection,true, true);
			printf("Buscando emisiones de %d |%s|\n", proton, selection);
			}
		else
                        {
                                if(strlen(selection)==1){
                                        printf("Felipe:11-09-08; un solo caracter\n");
                                        char str1[2] = " ";
                                        strcat(str1, selection);
                                        founds=LaraBD.SearchEmissions(-1,-1,-1,-1,-1,-1,-1,proton,proton,false,-1,false,-1,(char*)str1,true,false);
                                        printf("Felipe:11-09-08.un solo caracter. Buscando emisiones de %d|%s|\n", proton, str1);
                                }
                                else{
                                        founds=LaraBD.SearchEmissions(-1,-1,-1,-1,-1,-1,-1,proton,proton,false,-1,false,-1,selection,true,false);
                                        printf("Felipe:11-09-08. Buscando emisiones de %d |%s|\n", proton,selection);
                                }
                        }
		num=founds.GetFirstResult();
		printf("Hemos encontrado %d radionúclidos\n", founds.GetNumItemsFound());
		while(num!=-1)
			{
			double energia=LaraBD.GetEnergy(num);
			double probabilidad=LaraBD.GetProbability(num);
			// make an entry list widget
			char temp[60];
			sprintf(temp, "%.3f keV %.3f%%                                     %d", energia, probabilidad, num);
			entry = XmStringCreateLocalized( temp );
			XmListAddItemUnselected(listaE, entry, 0);
			XmStringFree(entry);
			num=founds.GetNextResult();
			}
		}
	}

//felipe-funcion para trocear una cadena
char * Subcadena (const char * Cadena, int Inicio, int Cantidad){
	char * Resultado;
	int TotalCadena = strlen (Cadena);

	if (Inicio >= TotalCadena){
		Resultado = new char [1];
		Resultado [0] = NULL;
		return Resultado;
	}
	int TotalSubcadena = (Inicio + Cantidad > TotalCadena) ? TotalCadena - Inicio : Cantidad;
	Resultado = new char [TotalSubcadena + 1];
	for (int i = 0; i < TotalSubcadena; ++i)
		Resultado [i] = Cadena [i + Inicio];
	Resultado [TotalSubcadena] = NULL;
	return Resultado;
}

//**************** SELECCIONA EMISIÓN ********************
//Se llama cada vez que se selecciona una emisión de listaE 
//en la búsqueda personalizada
void seleccionaEmision(Widget widget, XtPointer client_data, XtPointer call_data) 
	{
	char cadena[60];
	XmString entry;
	String selection;
	
  	XmListCallbackStruct *cbs = (XmListCallbackStruct *) call_data;
	XmStringGetLtoR(cbs->item, XmSTRING_DEFAULT_CHARSET, &selection);
	printf("seleccion: %s\n", selection);
	char * Alfa = Subcadena (selection, strlen(selection)-8, 8);
	int ce=atoi(Alfa);
	printf("ce: %d\n", ce);
	float amd, lc, ld;
	if(selection!=NULL)
		{
		float energia=atof(selection);
		printf("energia: %f\n", energia);
		//float ld=Datos.calculaLD(energia);
		Datos.calculaLC(energia, &lc, &ld);		
		Datos.calculaAMD(ce, energia, ld, &amd);
		printf("Para la emisión de energía %f tenemos ld=%f y amd=%f bq\n", energia, ld, amd);
		sprintf(cadena, "%f keV; lc=%f; ld=%f; amd=%f", energia, lc, ld, amd);
		entry=XmStringCreateLocalized(cadena);
		XmListAddItemUnselected(lista3, entry, 0);
		XmStringFree(entry);
		}
	}

//************** PULSA CANCEL FILE **********************
void pulsaCancelFile(Widget widget, XtPointer client_data, XtPointer call_data) 
	{
	printf("Pulsa Cancel File\n");
	XmToggleButtonGadgetSetState(action_efficiency_actividad, false, true);
	XtUnmanageChild(dlg_filesPar);
	XtUnmanageChild(widget);
	}

//************** PULSA OK FILE **********************
void pulsaOKFile(Widget widget, XtPointer client_data, XtPointer call_data) 
	{
	char *nombre = NULL, *dir = NULL;
	XmFileSelectionBoxCallbackStruct *cbs = (XmFileSelectionBoxCallbackStruct *) call_data;

    XmStringGetLtoR(cbs->value, XmFONTLIST_DEFAULT_TAG, &nombre);
    XmStringGetLtoR(cbs->dir, XmFONTLIST_DEFAULT_TAG, &dir);
		
    if (strlen(nombre) == strlen(dir)) 
		{ // no hay nombre de fichero
		XtFree(nombre); XtFree(dir);
		Aviso(main_w, "You must to select a file");
		return;
		}
		
	printf("%s, %s\n", nombre, dir);
	nodoCentroide *lista=lecturaEmisiones(nombre);//Función que determina las emisiones para las que
							//hay que calcular los límites de detección
	//Calculamos los límites de detección de las emisiones de la lista
	Datos.calculaLD(lista);

	//Ahora guardamos la información en un fichero:
	//El nombre del fichero será:
	//		/pathFicheroEspectro/nombreFichEspectroNombreFichRadionúclidos.ld
	
	//Quitamos el path del fichero de radionúclidos
	string str(nombre);
		
	cout << "string elegante: " << str << endl;
	int ind=0;
	while((ind=str.find("/"))!=-1)
		{
		str=str.substr(ind+1, str.length()-ind);
		cout << "string reduciéndose: " << str << endl;
		}
	//Si tiene extensión se la quitamos también
	if((ind=str.find("."))!=-1)	
		{
		cout << (str.length() -ind) << endl;
		//replace(a partir de, tantos caracteres, cadena de reemplace)
		str.replace(ind, str.length(), "");
		cout << "string reduciéndose: " << str << endl;
		}
	
	//NOTA: Funciona bien pero no sé por qué ya me pone la extensión !
	string fich(Datos.nombre);
	fich.insert(fich.length()-3, "_");
	fich.insert(fich.length()-3, str);
	
	cout << "nombre: " << fich << endl;
	const char *fich2=new char[fich.length()];
	fich2=fich.c_str();
	//guardarLD(fich2, lista);
	XtUnmanageChild(widget);
	//delete [] fich2;		
	//Falta meterlo en la lista2:
	nodoCentroide *temp=lista;
	
	XmListDeleteAllItems(lista2);
	while(temp!=NULL)
		{
		double e;
		double ld;
		char texto[80];
		float amd;
		e=temp->centroide;
		ld=temp->ld;
		int ce = temp->indice;
		printf("indice: %d", ce);
		
		Datos.calculaAMD(ce, e, ld, &amd);
		temp->amd = amd;
		printf("peak %d, %d, %.2f keV has ld=%.2f and amd=%.4f bq", temp->atomicnumber, temp->massnumber, e, ld, amd);
		sprintf(texto, "%d, %d, %.2f keV; ld=%.2f; amd=%.4f bq", temp->atomicnumber, temp->massnumber, e, ld, amd);
		XmString cadena = XmStringCreateLocalized(texto);
		XmListAddItemUnselected(lista2, cadena, 0);
		XmStringFree (cadena); 
	
		temp=temp->siguiente;
		}
	XtManageChild(lista2);
	guardarLD(fich2, lista);
	
	}

//************** PULSA OK FILE 2**********************
//lee ficheros de intervalos de energía
void pulsaOKFile_2(Widget widget, XtPointer client_data, XtPointer call_data) 
	{
	char *nombre = NULL, *dir = NULL;
	XmFileSelectionBoxCallbackStruct *cbs = (XmFileSelectionBoxCallbackStruct *) call_data;

    XmStringGetLtoR(cbs->value, XmFONTLIST_DEFAULT_TAG, &nombre);
    XmStringGetLtoR(cbs->dir, XmFONTLIST_DEFAULT_TAG, &dir);
		
    if (strlen(nombre) == strlen(dir)) 
		{ // no hay nombre de fichero
		XtFree(nombre); XtFree(dir);
		Aviso(main_w, "You must to select a file");
		return;
		}
		
	printf("%s, %s\n", nombre, dir);
	//nodoCentroide *lista=lecturaEmisiones(nombre);//Función que determina las emisiones para las que
							//hay que calcular los límites de detección
	//Calculamos los límites de detección de las emisiones de la lista
	float ld[1000]; //como mucho 1000
	int numlineas;
	int inicio[1000] , fin[1000];
	Datos.calculaLD(nombre, ld, &numlineas, inicio, fin);


	
	//cout <<"limd[0]="<<ld[0]<<endl;
	//cout <<"limd[1]="<<ld[1]<<endl;
	//Ahora guardamos la información en un fichero:
	//El nombre del fichero será:
	//		/pathFicheroEspectro/nombreFichEspectroNombreFichRadionúclidos.ld
	
	//Quitamos el path del fichero de radionúclidos
	string str(nombre);
		
	cout << "string elegante: " << str << endl;
	int ind=0;
	while((ind=str.find("/"))!=-1)
		{
		str=str.substr(ind+1, str.length()-ind);
		cout << "string reduciéndose: " << str << endl;
		}
	//Si tiene extensión se la quitamos también
	if((ind=str.find("."))!=-1)	
		{
		cout << (str.length() -ind) << endl;
		//replace(a partir de, tantos caracteres, cadena de reemplace)
		str.replace(ind, str.length(), "");
		cout << "string reduciéndose: " << str << endl;
		}
	
	//NOTA: Funciona bien pero no sé por qué ya me pone la extensión !
	string fich(Datos.nombre);
	fich.insert(fich.length()-3, "_");
	fich.insert(fich.length()-3, str);
	
	cout << "nombre: " << fich << endl;
	const char *fich2=new char[fich.length()];
	fich2=fich.c_str();


	//guardarLD(fich2, lista);
	XtUnmanageChild(widget);
	//delete [] fich2;		
	//Falta meterlo en la lista2:
	//nodoCentroide *temp=lista;

	//float *temp = ld;
	XmListDeleteAllItems(lista2);
	//int k = 0;

	//while(ld[k]!=0)
		cout <<"numlineas="<<numlineas<<endl;

	for (int k = 0; k < numlineas; k++)
		{
		//cout <<"ld["<<k<<"]="<<ld[k]<<endl;
		double e;
		//double ld;
		char texto[80];
		
		//e=temp->centroide;
		//ld=temp->ld;
		printf("peak of %d, %d keV has ld=%.2f", inicio[k],fin[k], ld[k]);
		sprintf(texto, "peak of %d, %d keV has ld=%.2f", inicio[k], fin[k], ld[k]);
		XmString cadena = XmStringCreateLocalized(texto);
		XmListAddItemUnselected(lista2, cadena, 0);
		XmStringFree (cadena); 
	
		//temp=temp->siguiente;
		}

	XtManageChild(lista2);
}

//***************** LECTURA EMISIONES ******************
	/*	
	Función que determina las emisiones para las que hay que calcular
	los límites de detección a partir de un fichero con información de 
	los radionúclidos (estructura "np\tna"). Devuelve una lista con las
	3 emisiones más probables para cada radionúclido dentro del intervalo
	en el que se ha calculado el continuo.
	*/
nodoCentroide *lecturaEmisiones(char *fichero)
	{
	ifstream file(fichero);
	if (! file.is_open()) 	
		{
		sprintf(mensajeErrorActividad, "Error opening file %s", fichero);
		Aviso(main_w, mensajeErrorActividad);
		cout << "Error opening file";// exit(1); 
		}
	char buffer[256];
	long int na,np;
	nodoCentroide *lista=NULL;
  	//Recorremos el fichero y para cada elemento hacemos el cálculo del LD
	int index[1000], k; //felipe. Para guardar el indice en Lara de cada nucleo
	while (! file.eof() )
  		{
	    	file.getline (buffer,100);
    		cout << buffer << endl;
		cout<<"Hemos cogido una línea, vamos a recoger los dos números del formato"<<endl;
		na=np=0;
		char *endptr=new char[1024];
			
		na=strtol(buffer, &endptr, 10);

		cout << "na: "<<na << endl;			
		int lim=*endptr;
		cout << "lim:" <<lim<< endl;
		for(int i=0;i<lim;i++)	
			{
			if(buffer[i]=='\t')	i+=4;
			buffer[i]=' ';
			}
		np=strtol(buffer, NULL, 10);
		cout << "np: "<<np << endl;		
		//delete [] endptr; me da error si le dejo el delete-felipe
		cout << "borrado puntero" << endl;	
		//Buscamos las tres emisiones más probables para este radionúclido en
		//el intervalo en el que se ha calculado el continuo
		cout << Datos.minInterv << ", " << Datos.maxInterv << endl;
		//NOTA: La búsqueda cogerá emisiones de lista de descendientes y de no descendientes
		cSearchLaraBD busqueda=LaraBD.SearchEmissions(Datos.minInterv,Datos.maxInterv,-1,-1,-1,na,na,np,np,false,false,false,false, NULL, false, false);			
		int valor2 = busqueda.GetFirstResult();
		char name[3];
		unsigned short int probs[3];
		for(int i=0;i<3;i++)	probs[i]=0;
		float prob0,prob1,prob2;
		prob0=prob1=prob2=0;//NOTA: Increible que tenga uqe hacer esto
	
		
		
		while (valor2 != -1) 
			{
	
			printf("%f Kev %f%% (%c) ", LaraBD.GetEnergy(valor2), LaraBD.GetProbability(valor2), LaraBD.GetKindEmission(valor2));
			int valor = LaraBD.GetEmissor(valor2);
			LaraBD.GetNameElement(valor, name);
			printf("%d -%s- %d\n", LaraBD.GetAtomicNumber(valor), name, LaraBD.GetMassNumber(valor));
			//Nos vamos quedando con los tres con mayor probabilidad 
			if( LaraBD.GetProbability(valor2)> prob0 )
				{
				probs[2]=probs[1];
				probs[1]=probs[0];
				probs[0]=valor2;//número de la emisión con mayor probabilidad
				cout << probs[2] << "->" << probs[1] << "->" << probs[0] << endl;
				prob2=prob1;
				prob1=prob0;
				prob0=LaraBD.GetProbability(valor2);
				}
			else
				{
				if(LaraBD.GetProbability(valor2)>prob1)
					{
					probs[2]=probs[1];
					probs[1]=valor2;
					cout << probs[2] << "->" << probs[1] << "->" << probs[0] << endl;
					prob2=prob1;
					prob1=LaraBD.GetProbability(valor2);
					}
				else if(LaraBD.GetProbability(valor2)>prob2)
					{
					probs[2]=valor2;
					cout << probs[2] << "->" << probs[1] << "->" << probs[0] << endl;
					prob2=LaraBD.GetProbability(valor2);
					}
				}

			valor2 = busqueda.GetNextResult();
			}//Bucle para todas las emisiones de un radionúclido

		printf("encontrados: %d\n", busqueda.GetNumItemsFound());
		printf("Los más probables son: \n");
		for(int i=0;i<3;i++)	
			{
			if(probs[i]>0)
				{
				if(lista==NULL)	
					{
					lista=new nodoCentroide;
					if(lista==NULL)
						{
						printf("ERROR: No hay memoria para reservar nodoCentroide\n");
						return NULL;
						}						
					lista->centroide=LaraBD.GetEnergy(probs[i]);
					lista->atomicnumber=LaraBD.GetAtomicNumber(LaraBD.GetEmissor(probs[i]));
					lista->massnumber=LaraBD.GetMassNumber(LaraBD.GetEmissor(probs[i]));
					lista->indice=probs[i];
					lista->siguiente=NULL;
					}
				else
					{
					nodoCentroide *nodo=new nodoCentroide;
					if(lista==NULL)
						{
						printf("ERROR: No hay memoria para reservar nodoCentroide\n");
						return NULL;
						}						
					nodo->centroide=LaraBD.GetEnergy(probs[i]);
					nodo->atomicnumber=LaraBD.GetAtomicNumber(LaraBD.GetEmissor(probs[i]));
					nodo->massnumber=LaraBD.GetMassNumber(LaraBD.GetEmissor(probs[i]));
					nodo->indice=probs[i];
					nodo->siguiente=lista->siguiente;
					lista->siguiente=nodo;
					}
				}
			}
		printf("se acaba el bucle for(int i=0;i<3;i++)	: \n");
		
	}//Bucle para todos los radionúclidos
	printf("se acaba el bucle de radionuclidos: \n");

	//Vamos a imprimir para comprobar que no hemos perdido ningún radionúclido
/*	nodoCentroide *temp=new nodoCentroide;
	temp=lista;
	while(temp->siguiente!=NULL)
		{
		cout << temp->centroide << endl;
		temp=temp->siguiente;
		}
	//Imprimimos el último
	cout << temp->centroide << endl;*/
	return lista;	
	}



	
//**************** LEE NUM PARS ********************
//Devuelve el número de parámetros que aparece en un fichero que tenga el
//formato .par
int leeNumPars(char *nombre)
	{
	printf("LEE NUM PARS\n");
	//char *text;
	//XmStringGetLtoR(nombre, XmFONTLIST_DEFAULT_TAG, &text);
	printf("El nombre del fichero de parámetros es %s\n", nombre);
	ifstream file(nombre);
	//ifstream file(text);
	printf("abrimos el fichero\n");
	if (! file.is_open()) 	
		{
		sprintf(mensajeErrorActividad, "Error opening file %s", nombre);
		Aviso(main_w, mensajeErrorActividad);
		cout << "Error opening file" << nombre; //exit(1); 
		return -1;
		}
	printf("Creamos buffer\n");
	char buffer[256];
	int numpar=-1;
	
	printf("buscamos la línea 8\n");
	//Recorremos el fichero hasta llegar a la línea 8
	int i=0;
	while (int (i++)<7 && !file.eof() )
	    file.getline (buffer,100);

	//Hemos llegado a la línea buena
	file.getline (buffer,100);
	printf("Hacemos el tratamiento\n");
	string cadena(buffer);
	int ind=0;
//	while((ind=cadena.find(":"))!=string::npos)
	while((ind=cadena.find(":"))!=-1)
		cadena=cadena.substr(ind+1, cadena.length());
	numpar	= atoi(cadena.c_str());
		
	file.close();
	printf("Termina leenumpars\n");
	return numpar;
	}

//**************** LEE CHI EFICIENCIA ********************
//Devuelve la calidad de la curva de eficiencia que aparece en 
//	un fichero que tenga el formato .par
double leeChiEficiencia(String nombre)
	{
	ifstream file(nombre);
	if (! file.is_open()) 	
		{ 
		sprintf(mensajeErrorActividad, "Error opening file %s", nombre);
		Aviso(main_w, mensajeErrorActividad);
		cout << "Error opening file" << nombre;
		return -1;			
		}
	char buffer[256];
	double chiefi=-1;
	
	//Recorremos el fichero y para cada elemento hacemos el cálculo del LD
	int i=0;
	while (int (i++)<5 && !file.eof() )
	    file.getline (buffer,100);

	//Hemos llegado a la línea buena
	file.getline (buffer,100);
	
	string cadena(buffer);
	int ind=0;
//	while((ind=cadena.find("shisq"))!=string::npos)
	while((ind=cadena.find("shisq"))!=-1)
		cadena=cadena.substr(ind+5, cadena.length());
	//NOTA:SE HAN SUSTITUIDO TODOS LOS NPOS POR -1 PORQUE GCC2.8 (DE PUCCINI) NO LOS COGE!
//	if( (ind=cadena.find("."))!=string::npos)	
	if( (ind=cadena.find("."))!=-1)	
		cadena.replace(ind, 1, ",");
	chiefi	= atof(cadena.c_str());
	file.close();
	return chiefi;
	}

//************************* LEE PARS ********************************
//Devuelve los valores de los parámetros que aparecen en un fichero que tenga el
//formato .par
void leePars(String nombre, int numPars, double *pars)
	{
	ifstream file(nombre);
	if (! file.is_open()) 	
		{ 
		sprintf(mensajeErrorActividad, "Error opening file %s", nombre);
		Aviso(main_w, mensajeErrorActividad);
		cout << "Error opening file" << nombre; 
		return;
		}
	char buffer[256];
	
	//Pasamos las líneas que no nos interesan
	int i=0;
	while ((i++)<8 && !file.eof() )
	    file.getline (buffer,100);

	//Hemos llegado a las líneas de los parámetros
	i=0;
	int ind=0;
		
	while (i<numPars && !file.eof() )
		{
		file.getline (buffer,100);
		//Tenemos que cambiar el "." de X.Y por X,Y para poder hacer la conversión
		string cad(buffer);
//		if((ind=cad.find("."))!=string::npos)	
		if((ind=cad.find("."))!=-1)	
			cad.replace(ind, 1, ",");
		//NOTA: Cuidado, puede parecer si imprimimos que no nos coge todas las
		//		cifras significativas, pero no es así, las coge todas lo que 
		//		pasa es que print sólo saca 6 si no le dices nada y cout sólo 5
		pars[i]=atof(cad.c_str());
		i++;
		}
	}

//************************* LEE MATRIZ EFI ********************************
//Devuelve los valores de la matriz de covarianza que aparecen 
//	en un fichero que tenga el formato .par
void leeMatrizEfi(String nombre, int numPars, double **pars)
	{
	ifstream file(nombre);
	if (! file.is_open()) 	
		{ 
		sprintf(mensajeErrorActividad, "Error opening file %s", nombre);
		Aviso(main_w, mensajeErrorActividad);
		cout << "Error opening file" << nombre; 
		return;
		}
	char buffer[256];
	
	//Pasamos las líneas que no nos interesan
	int i=0;
	while ((i++)<(8+numPars+1+numPars+2) && !file.eof() )
  	    file.getline (buffer,100);
	
	//Hemos llegado a las líneas de los parámetros
	i=0;
	int ind=0;
	
	for(i=0;i<numPars && !file.eof();i++)
		for(int j=0;j<numPars && !file.eof();j++)
			{
			file.getline (buffer,100);
			//Tenemos que cambiar el "." de X.Y por X,Y para poder hacer la conversión
			//Como está en precisión doble, fortran pone una D en vez de una E para
			//las potencias de 10: lo cambiamos tambien.
			string cad(buffer);
			if((ind=cad.find("."))!=-1)	cad.replace(ind, 1, ",");
			if((ind=cad.find("D"))!=-1)	cad.replace(ind, 1, "E");
			pars[i][j]=atof(cad.c_str());
			}			
	}
	
//************************* LEE NODOS EFI ********************************
//Devuelve los valores de los nodos de spline que aparecen en un fichero 
//	que tenga el formato .par
void leeNodosEfi(String nombre, int numNodos, double *nodos)
	{
	ifstream file(nombre);
	if (! file.is_open()) 	
		{ 
		sprintf(mensajeErrorActividad, "Error opening file %s", nombre);
		Aviso(main_w, mensajeErrorActividad);
		cout << "Error opening file" << nombre; 
		return;
		}
	char buffer[256];
	
	//Pasamos las líneas que no nos interesan
	int i=0;
	//Sumamos numNodos líneas, es decir, todas las líneas de parámetros más 1 del título
	while ((i++)<(8+numNodos) && !file.eof() )
  		{
	    file.getline (buffer,100);
		}

	//Hemos llegado a las líneas de los nodos
	i=0;
	int ind=0;
		
	while (i<numNodos && !file.eof() )
		{
		file.getline (buffer,100);
		//Tenemos que cambiar el "." de X.Y por X,Y para poder hacer la conversión
		string cad(buffer);
		if((ind=cad.find("."))!=-1)	cad.replace(ind, 1, ",");
		//NOTA: Cuidado, puede parecer si imprimimos que no nos coge todas las
		//		cifras significativas, pero no es así, las coge todas lo que 
		//		pasa es que print sólo saca 6 si no le dices nada y cout sólo 5
		nodos[i]=atof(cad.c_str());
		i++;
		}
	}


//************** PULSA ACTIVITY **********************
/* Se activa cuando se pulsa Activity para calcular las actividades
	de los picos identificados.
	La fórmula del cálculo de la actividad es:
				    N
	A(e)= ---------------------
		   Psigma*Tv*Csc*Ef(e)
	
	Donde:
		-N=área del pico, calculada como Área - Fondo.
			Los valores de las áreas los tenemos calculados pero necesitamos
			hacer el cálculo del fondo.
		-Psigma
		-Tv=Tiempo de medición vivo. Es un parámetro que deberíamos obtener del
			detector a través del fichero de espectro o de una entrada del usuario.
		-Csc=Factor de corrección de la eficiencia. Se encuentra, ordenado por energías
			en ficheros cs-*.dat. Por defecto tomamos el fichero con la misma extensión
			que el que se usa para la eficiencia. Si no se encuentra, se pide por uno.
		-Ef(e)=Eficiencia, calculado al pulsar Efficiency. Su cálculo habilita este
				botón.	
	Esta función tendrá por tanto que hacer:
	1) Calcular N. Para ello lo primero es calcular el fondo medio a partir 
		de los fondos que aparecen en la base de datos. Además, antes tendremos
		que pedirle al usuario con qué base de datos comparamos
	*/
void pulsa_activity(Widget widget, XtPointer client_data, XtPointer call_data) 
	{
	if(XmToggleButtonGadgetGetState(action_activity_actividad)==false)
		{//Si ya estaba pulsado, lo despulsamos (y quitamos la ventana)
		printf("Despulsamos MDA\n");
		XtUnmanageChild(dlgAct);
		//XmToggleButtonGadgetSetState(action_MDA_actividad, true, true);
		}
	else
		{
		printf("Pulsamos Activity\n");
		//Calculamos la actividad
		calculaActividad();
		}
	}

//**************** PEDIR BD FONDO ******************
/* Crea un pequeño cuadro de diálogo para elegir la base de datos que 
	usaremos para el cálculo de las áreas de los picos.
	Al pulsar OK pasaremos a calcular la media de sus valores.
	*/
void PedirBDFondo()
	{
    Arg		 args[5];
    Cardinal i = 0;
	XmString cadena;

	i = 0;
	printf("Vamos a ponerle título\n");
	XtSetArg (args[i], XmNtitle, "Use Background DB ..."); i++;

	dlgFBD = XmCreateFileSelectionDialog (app_shell, "dlg_files", args, i);
	XtAddCallback (dlgFBD, XmNokCallback, pulsaOKBDFondo, NULL);
	XtAddCallback (dlgFBD, XmNcancelCallback, pulsaCancelBDFondo, NULL);
		
	cadena = XmStringCreateLocalized ("*.fbd");
	XtVaSetValues (dlgFBD, XmNpattern, cadena, NULL);
	XmStringFree (cadena);
	XtManageChild(dlgFBD);
	}

//**************** PEDIR BD FONDO LC ******************
/* Crea un pequeño cuadro de diálogo para elegir la base de datos que 
	usaremos para el cálculo de las áreas de los picos.
	Al pulsar OK pasaremos a calcular la media de sus valores y después
	a calcular los límites de decisión.
	*/
void PedirBDFondoLC()
	{
    Arg		 args[5];
    Cardinal i = 0;
	XmString cadena;

	i = 0;
	XtSetArg (args[i], XmNtitle, "Use Background DB ..."); i++;

	dlgFBD = XmCreateFileSelectionDialog (app_shell, "dlg_files", args, i);
	XtAddCallback (dlgFBD, XmNokCallback, creaDialogoLC, NULL);
	XtAddCallback (dlgFBD, XmNcancelCallback, pulsaCancelBDFondo, NULL);
		
	cadena = XmStringCreateLocalized ("*.fbd");
	XtVaSetValues (dlgFBD, XmNpattern, cadena, NULL);
	XmStringFree (cadena);
	XtManageChild(dlgFBD);
	}

//********************* PULSA OK BD FONDO **********************	
/* Se activa cuando se selecciona un fichero .fbd como base de fondo
	para calcular la actividad.
	*/
void pulsaOKBDFondo(Widget widget, XtPointer client_data, XtPointer call_data)
	{
   	char *nombre = NULL, *dir = NULL;
	XmFileSelectionBoxCallbackStruct *cbs = (XmFileSelectionBoxCallbackStruct *) call_data;
	printf("PULSA OK BD FONDO\n");
   	XmStringGetLtoR(cbs->value, XmFONTLIST_DEFAULT_TAG, &nombre);
   	XmStringGetLtoR(cbs->dir, XmFONTLIST_DEFAULT_TAG, &dir);
	
	double *energias, *medias, *incertidumbres;
	int numEnergias=numeroFilas(nombre)-1;//La primera son sólo títulos
	int numColumnas=numeroColumnas(nombre);

	medias=new double[numEnergias];
	incertidumbres=new double[numEnergias];
	energias=new double[numEnergias];
	if(medias==NULL || incertidumbres ==NULL || energias==NULL)
		{
		printf("ERROR: No hay memoria para reservar double[%d]\n", numEnergias);
		return;
		}
	calculaMediaBDFondo(nombre, numEnergias, numColumnas, energias, medias, incertidumbres);
	//Una vez calculada la media, pasamos a calcular el área de los picos
	calculaAreaPicos(energias, medias, incertidumbres, numEnergias);
//	printf("volvemos de calcula area picos\n");
	//Ahora calculamos los coeficientes de corrección de la eficiencia
	if(nombreFichEfi!=NULL)	printf("El nombre del fichero de eficiencias es %s\n", nombreFichEfi);
	recogeFactoresCorreccion(nombreFichEfi);
	XtUnmanageChild(dlgFBD);
	
	delete [] medias;
	delete [] energias;
	delete [] incertidumbres;
	}
	
//*********************** PULSA CANCEL BD FONDO *****************
void pulsaCancelBDFondo(Widget widget, XtPointer client_data, XtPointer call_data)
	{
	XtUnmanageChild(dlgFBD);
	XmToggleButtonGadgetSetState(action_limits_actividad, false, true);
	XtUnmanageChild(dlgFBD);
	}

	
//*************** CALCULA MEDIA BD FONDO *********************
/* 
	A partir de la información del fichero cuyo nombre se pasa como parámetro,
	obtenemos las medias para cada energía.
	Las medias se calculan de la siguiente manera:
	_      sum(xi/s²i)
	x = ------------------
		   sum(1/s²i)
	
	y la incertidumbre es:
	__		    ___ ___
	sx=sqrt(max(s1x,s2x))
	
	donde:
	___
	s1x = inversa(sum(1/s²i))
	
	y
	
	___		sum((xi-x)²/s²i)
	s2x = -------------------
		      sum(1/s²i)
	*/
void calculaMediaBDFondo(char *nombre, int numEnergias, int numColumnas, double *energias, double *medias, double *incertidumbres)
	{
	double s1=0;
	double s2=0;
	double elemento=0;
	double incertidumbre=0;
	double numerador=0;
	double denominador=0;
	
//	printf("Poniendo el nombre del fichero de fondo\n");
	if(Datos.nombreFondo != NULL)	delete [] Datos.nombreFondo;
	Datos.nombreFondo=new char [strlen(nombre)+1];
	strcpy(Datos.nombreFondo, nombre);
		
	printf("CALCULA MEDIA BD FONDO de %d columnas\n", numColumnas);
	for(int i=0;i<numEnergias;i++)//La primera línea tiene títulos
		{
		medias[i]=0.0;
		numerador=denominador=0;
		energias[i]=leerElemento(nombre, i+1, 0);//La primera línea tiene títulos
		printf("Energía %d es %f\n", i, energias[i]);
		for(int j=2;j<numColumnas+2;j++)
			{
			incertidumbre=pow(leerIncertidumbre(nombre, i+1, j),2);
			elemento=leerElemento(nombre, i+1, j);
		//	printf("Valor en %d,%d es %f(%f)\n", i+1, j, elemento, incertidumbre);
			if(incertidumbre>0 && elemento>0)
				{
				numerador+=elemento/incertidumbre;
				denominador+=1/incertidumbre;
				}
			}
		//Calculamos la media
		if(denominador!=0)	medias[i]=numerador/denominador;
		else				medias[i]=0;//Por si hay alguna reacción de la que no se registra actividad
		//Ahora que sabemos la media calculamos la incertidumbre
		numerador=denominador=0;
		s1=s2=0;
		for(int j=2;j<numColumnas+2;j++)
			{
			double inc=leerIncertidumbre(nombre, i+1, j);
			if(inc==-1)	inc=0;
			incertidumbre=pow(inc,2);
			
			elemento=leerElemento(nombre, i+1, j);
//			printf("Elemento %f(%f)\n", elemento, incertidumbre);
			if(incertidumbre>0 && elemento>0)
				{
	//			printf("sumamos 1/%f\n", incertidumbre);
				numerador   += pow(elemento-medias[i],2)/incertidumbre;
				denominador += 1/incertidumbre;
				}
			}
		if(denominador!=0)	
			{
			s2=numerador/denominador;	
			s1=1/denominador;
			}
		incertidumbres[i]=sqrt(maxim(s1,s2));
		if(s1==1 && s2==0)	incertidumbres[i]=0;//Si no hay ningún valor pues es 0, no 1!
//		printf("Incertidumbre es sqrt del máximo entre %f y %f -> %f\n", s1,s2, incertidumbres[i]);
//		printf("media %d es %f(%f)\n", i, medias[i], incertidumbres[i]);
		}
		
	for(int i=0;i<numEnergias;i++)
		{
		incertidumbres[i]*=(Datos.tiempo/1000000.0);
		medias[i]*=(Datos.tiempo/1000000.0);
		}
		
	//Dibujo en gráfica (ayuda a saber cómo es el fondo):
	Datos.calculaFondo(medias, incertidumbres, energias, numEnergias);
	calculaAreaPicos(energias, medias, incertidumbres, numEnergias);
	//NOTA: decidir si dejamos calcula areaPicos o vamos a restaAreaFondo.
	//		Ver si el resultado es el mismo
	//Copiamos sólo el fondo donde haya continuo
	int inicio=0;
	int fin=0;
	for(int i=1;i < Datos.numCanales;i++)
		if(Datos.continuo[i]!=0)	
			{
			inicio=i;
			i=Datos.numCanales;//Para acabar
			}
	for(int i=Datos.numCanales;i > inicio;i--)
		if(Datos.continuo[i]!=0)	
			{
			fin=i;
			i=inicio;//Para que acabe el bucle
			}
	
	BorraFondoEnGrafica();
	CopiaFondoAGrafica(inicio, fin);
	ActivaFondoGrafica(true);
	PintaGrafica(0);
	}
	
//************** ELIMINA REPETIDAS ******************
void eliminaRepetidas(char *nombre)
	{
	printf("ELIMINA REPETIDAS\n");
	ifstream file(nombre);
	if (! file.is_open()) 	
		{ 
		sprintf(mensajeErrorActividad, "Error opening file %s", nombre);
		Aviso(main_w, mensajeErrorActividad);
		cout << "Error opening file" << nombre; 
		return;
		}
	char buffer[256];
	char bufferAnt[256]="";
	ofstream ofile("temp");

	int filas=numeroFilas(nombre);
	int cont=0;
	//Tomamos de cada línea la energía y el factor de corrección
/*	while (!file.eof())
  		{
	    file.getline (buffer,100);
		string cadena(buffer);
		printf("Comparamos |%s| con |%s|\n", bufferAnt, buffer);
		printf("%d vs %d\n", cont, filas);
		if(strcmp(buffer, bufferAnt))//Si son distintos añadimos la línea
			{
			if(cont<filas)	ofile << cadena << endl;
			else			ofile << cadena;
			}
		strcpy(bufferAnt, buffer);		
		cont++;
		}
		*/
	printf("filas es %d\n", filas);
	for(int i=0;i<filas;i++)
		{
		file.getline (buffer,100);
		string cadena(buffer);
		if(strcmp(buffer, bufferAnt))//Si son distintos añadimos la línea
			{
			if(i<filas-1)	ofile << cadena << endl;
			else			ofile << cadena;
			}
		strcpy(bufferAnt, buffer);		
		}
	ofile.close();
	
	//Pasamos del fichero temporal al fichero final
	ifstream ifile2("temp");
	ofstream ofile2(nombre, ios::trunc);
	if (!ifile2.is_open() || !ofile2.is_open() ) 	
		{ cout << "Error opening files"; exit(1); }
	
	cont=0;
	filas=numeroFilas("temp");
	/*while (! ifile2.eof() )
		{
		ifile2.getline (buffer,1024);
		
		if(cont<filas)	ofile2 << buffer << "\n";
		else			ofile2 << buffer;
		
		cont++;
		}*/
	for(int i=0;i<filas;i++)
		{
		ifile2.getline (buffer,1024);
		
		if(i<filas-1)	ofile2 << buffer << "\n";
		else			ofile2 << buffer;
		
		}

	ofile2.close();
	ifile2.close();
	remove("temp.fbd");
		
	file.close();
	ofile.close();
	}

//******************* CONSTRUYE LISTA CORRECCION ******************
/*	Construye todas las cadenas necesarias para introducir en Datos
	las correcciones de la eficiencia a partir del nombre del fichero
	en el que se encuentran
	*/
void construyeListaCorreccion(char *nombre)
	{
	printf("CONSTRUYE LISTA CORRECCIÓN: %s\n", nombre);
	ifstream file(nombre);
	if (! file.is_open()) 	
		{ 
		sprintf(mensajeErrorActividad, "Error opening file %s", nombre);
		Aviso(main_w, mensajeErrorActividad);
		cout << "Error opening file" << nombre; 
		return;
		}
	char buffer[256];
	
	int i=0;
	
	//Eliminamos todas las filas que aparezcan repetidas en el fichero
	printf("El fichero tiene %d filas\n", numeroFilas(nombre));
	eliminaRepetidas(nombre);
//	sustituyePuntoPorComa(nombre);
//	printf("El fichero tiene %d filas\n", numeroFilas(nombre));
	preparaFichero(nombre);
	printf("El fichero tiene %d filas\n", numeroFilas(nombre));
		
	int MAX=numeroFilas(nombre);
	double energias[MAX];
	double factores[MAX];
	double sigmaFactores[MAX];
		
	//Tomamos de cada línea la energía, factor de corrección y su incertidumbre
	while (!file.eof())
  		{
	    file.getline (buffer,100);
		string cadena(buffer);
		//cout << cadena << endl;
		if(cadena=="")	break;
		//Quitamos los cuatro espacios primeros
		for(int j=0;j<2;j++)	cadena=cadena.substr(1,cadena.length());
		//cout << cadena << endl;
		energias[i]=atof((char *)cadena.c_str());
		cadena=cadena.substr(cadena.find("    ")+4,cadena.length());
		//cout << cadena << endl;
		factores[i]=atof((char *)cadena.c_str());
		cadena=cadena.substr(cadena.find("    ")+4,cadena.length());
		//cout << cadena << endl;
		sigmaFactores[i]=atof((char *)cadena.c_str());
		printf("energia[%d]=%f\tfactor=%f, sigma=%f\n", i, energias[i], factores[i], sigmaFactores[i]);
		i++;			
		}
	file.close();
	printf("Construimos la información en Datos\n");
	for(int i=0;i<MAX;i++)	printf("FACTORES[%d]=%f\n", i, factores[i]);
	Datos.calculaCorreccionesEficiencia(energias, factores, sigmaFactores, 0.01);
	printf("Listas de corrección construidas\n");
	
	//NOTA: Provisional
	construyeMatrizPN();
	//calculaActividadRadionuclidos();//Para comprobar cómo hace las divisiones
	//
	}


//****************** CALCULA MATRICES ACTIVIDAD *****************
/*	Cálculo de matrices de actividad según Debertin, pág 304.
	*/
LinkedList *calculaMatricesActividad()
	{
	LinkedList *matrices=new LinkedList();//NOTA: MM

	printf("CALCULA MATRICES ACTIVIDAD\n");
	for(int i=1;i<=Datos.listaPicos.numElementos;i++)	//Para cada intervalo de la lista
		{
		NodoPicos *picos=DevuelveNodo(Datos.listaPicos, i);
		for(int j=0;j<picos->picos.numElementos;j++)//Para cada pico del intervalo
			{
			NodoPico *nodo = DevuelveNodo(picos->picos, j);
			nodo->comprobado=false;
			}
		}
	
	for(int i=1;i<=Datos.listaPicos.numElementos;i++)	//Para cada intervalo de la lista
		{
		NodoPicos *picos=DevuelveNodo(Datos.listaPicos, i);
		for(int j=0;j<picos->picos.numElementos;j++)//Para cada pico del intervalo
			{
			NodoPico  *nodo      = DevuelveNodo(picos->picos, j);
//			if(nodo->Ajuste.identificado && !nodo->comprobado)	//Si está identificado y no está ya en alguna lista
			if(nodo->Ajuste.identificado && !nodo->Ajuste.calibrado && !nodo->comprobado)	//Si está identificado y no está ya en alguna lista
				{//Tomamos sus radionúclidos asociados
				LinkedList *lista=construyeListaRelacionados(nodo);
				//Imprimimos para comprobación de lista
				//imprimeLista(lista);
				
				matrices->addLast(lista);
				}
			}
		}
	
	//printf("FIN DE CALCULA MATRICES ACTIVIDAD\n");
	return matrices;
	}
	
//****************** IMPRIME LISTA **************	
void imprimeLista(LinkedList *lista)
	{
	printf("IMPRIME LISTA de %d els\n", lista->numElements());
	for(int i=0;i<lista->numElements();i++)
		{
		NodoPico *np=(NodoPico *)lista->getNode(i);
		imprimeNodo(np);
		printf("\n");
		}
	printf("-------------------------------\n");
	}

//************** IMPRIME NODO ***************
void imprimeNodo(NodoPico *np)
	{
	for(int j=0;j<np->Ajuste.emisiones->numElements();j++)
		{
		NodoEmision *ne=(NodoEmision *)np->Ajuste.emisiones->getNode(j);
		if(ne->codElemento!=-1)
			{
			int emisor=LaraBD.GetEmissor(ne->codElemento);
			int mn=LaraBD.GetMassNumber(emisor);
			char nom[3];
			LaraBD.GetNameElement(emisor, nom);
			printf("%s%d %f keV, ", nom, mn, ne->Energia);
			}
		else
			{
			printf("%s %f keV, ", ne->nombre, ne->Energia);
			}
		}
	printf("\n");
	}
	
//********************* CONSTRUYE LISTA RELACIONADOS ***************************
LinkedList* construyeListaRelacionados(NodoPico *nodo)
	{
	//1) Tomamos todos los radionúclidos relacionados con este pico
	LinkedList *listaRadionuclidos=new LinkedList();
	int emisor=0;
	printf("CONSTRUYE LISTA RELACIONADOS\n");	
	printf("El nodo tiene %d emisiones asociadas\n", nodo->Ajuste.emisiones->numElements());	
	for(int j=0;j<nodo->Ajuste.emisiones->numElements();j++)
		{
		NodoEmision *ne=(NodoEmision *)nodo->Ajuste.emisiones->getNode(j);
		//emisor[i]=LaraBD.GetEmissor(nodo->Ajuste.codElemento);//Este es el radionúclido asociado
		int cod=LaraBD.GetEmissor(ne->codElemento);
	//	printf("Añadimos el radionúclido %d\n", cod);
		if(ne->codElemento!=-1)	listaRadionuclidos->addLast((void *)cod);
		//Tenemos todos los emisores relacionados con este radionúclido
		}
		
	//2) Añadimos nodo a la lista que vamos a devolver
//	printf("2\n");
	LinkedList *lista = new LinkedList();
	lista->addLast((void *)nodo);
	nodo->comprobado=true;
		
	//3) Ahora vamos a ver qué otros picos se corresponden con éste, y los añadimos
//	printf("3\n");
	for(int i=1;i<=Datos.listaPicos.numElementos;i++)	//Para cada intervalo de la lista
		{
//		printf("%d\n", i);
		NodoPicos *picos=DevuelveNodo(Datos.listaPicos, i);
		for(int j=1;j<=picos->picos.numElementos;j++)//Para cada pico del intervalo
			{
	//		printf("%d,%d\n", i,j);
			NodoPico *nodo = DevuelveNodo(picos->picos, j);
//			printf("comprobado: %d\n", nodo->comprobado);
			if(nodo->Ajuste.identificado && !nodo->Ajuste.calibrado && !nodo->comprobado)	//Si está identificado y no está ya en alguna lista
				{
				int radionuclidosAqui[nodo->Ajuste.emisiones->numElements()];
				for(int k=0;k<nodo->Ajuste.emisiones->numElements();k++)//Tomamos todos los radionuclidos de este pico
					{
					NodoEmision *ne=(NodoEmision *)nodo->Ajuste.emisiones->getNode(k);
					if(ne->codElemento!=-1) radionuclidosAqui[k]=LaraBD.GetEmissor(ne->codElemento);//Este es el radionúclido asociado
					}
				
				for(int k=0;k<listaRadionuclidos->numElements();k++)//Comprobamos si alguno está en nuestra lista
					{
					for(int l=0;l<nodo->Ajuste.emisiones->numElements();l++)
						{
						if(radionuclidosAqui[l]==(int)listaRadionuclidos->getNode(k))
							{
							//Este pico debe entrar en la matriz
				//			printf("				El radionúclido %d está en ambos!, añadimos\n", radionuclidosAqui[l]);
							lista->addLast((void *)nodo);
							nodo->comprobado=true;
							//No hay que buscar más: salimos del bucle
							l=nodo->Ajuste.emisiones->numElements();
							k=listaRadionuclidos->numElements();
							}
						}
					}
				
				if(nodo->comprobado==true)//Si lo hemos añadido
					{
					//Añadimos posibles radionúclidos de este pico que no estuvieran en la matriz
					for(int l=0;l<nodo->Ajuste.emisiones->numElements();l++)
						{
						bool agnadirla=true;
						for(int k=0;k<listaRadionuclidos->numElements();k++)
							{
							if(radionuclidosAqui[l]==(int)listaRadionuclidos->getNode(k))
								{//Si coincide, no hay que añadirla
								agnadirla=false;
								}
							}
						if(agnadirla)	
							{
							listaRadionuclidos->addLast((void *)radionuclidosAqui[l]);
							i=0;//Tenemos que rechequear todos los anteriores por si tenían
								//este radionúclido!!!
							}
							
						}
					}
				}//if nodo comprobado e identificado
			}
		}
	
	printf("fin de CONSTRUYE LISTA RELACIONADOS\n");
	return lista;
	}
	
/*******************************************************************************
*																			   *
*					CÁLCULO DE LÍMITES DE DECISIÓN							   *
*	
*
*******************************************************************************/
//*********************** PULSA LIMITS ***********************
/*	Calcula los límites de decisión. Pide la base de datos de fondo y a partir 
	de ella calcula dichos límites
	NOTA: Falta cambiar el estado de trabajo y plasmar los ld en el fichero .asf
	que se genera en caso de uqe se quiera guardar
*/
void pulsa_limits(Widget widget, XtPointer client_data, XtPointer call_data) 
	{
	printf("PULSA LIMITS\n");
	if(XmToggleButtonGadgetGetState(action_limits_actividad)==false)
		{//Si ya estaba pulsado, quitamos la ventana si existiera
		XtUnmanageChild(dlgLC);
		}
	else	
		{
//		if(EstadoTrabajo.fondoCargado)	recogeFactoresCorreccion(nombreFichEfi);
/*		if(EstadoTrabajo.fondoCargado)	creaDialogoLC(app_shell, NULL, NULL);
		else							PedirBDFondoLC();*/

		creaDialogoLC(app_shell, NULL, NULL);
		
		}
	}
	
void pulsa_limits_ld(Widget widget, XtPointer client_data, XtPointer call_data) 
	{
	printf("PULSA LIMITS\n");
	if(XmToggleButtonGadgetGetState(action_ld_actividad)==false)
		{//Si ya estaba pulsado, quitamos la ventana si existiera
		XtUnmanageChild(dlgLC);
		}
	else	
		{
//		if(EstadoTrabajo.fondoCargado)	recogeFactoresCorreccion(nombreFichEfi);
/*		if(EstadoTrabajo.fondoCargado)	creaDialogoLC(app_shell, NULL, NULL);
		else							PedirBDFondoLC();*/

		creaDialogoLD();
		
		}
	}
/*	
void pulsa_limits_ld_2(Widget widget, XtPointer client_data, XtPointer call_data) 
	{
	printf("PULSA LIMITS\n");
	if(XmToggleButtonGadgetGetState(action_ld_2_actividad)==false)
		{//Si ya estaba pulsado, quitamos la ventana si existiera
		XtUnmanageChild(dlgLC);
		}
	else	
		{

		creaDialogoLD_2();
		
		}
	}
*/
//************** CREA DIALOGO LD ***********************
/* Nos crea un diálogo para los distintos cálculos de los límites
	de detección. Constará de tres apartados, según el tipo de cálculo
	que realicemos:
	1)Cálculo sobre las emisiones identificadas en la calibración:
		Sólo tendrá un botón para realizar el ajustes
	2)Cálculo sobre  las emisiones que estén en un fichero de radionúclidos
		Abrirá un cuadro de exploración de ficheros de la extensión que le 
		pongamos.
	3)Cálculo personalizado sobre un determinado radionúclido:
		Permitirá elegir número de protones, tipo de radionúclido y energía
	*/
void creaDialogoLD()
	{
	Arg			args[7];
   	Cardinal	i = 0;
	XmString 	cadena;
	
	// *********************
	// * cuadro de dialogo *
	// *********************
	printf("CREA DIALOGO LD\n");
	i = 0;
	XtSetArg (args[i], XmNtitle, "Detection Limits"); i++;
	XtSetArg (args[i], XmNmwmDecorations, MWM_DECOR_BORDER|MWM_DECOR_TITLE|MWM_DECOR_MENU); i++;
	XtSetArg (args[i], XmNmwmFunctions, MWM_FUNC_MOVE|MWM_FUNC_CLOSE); i++;
	XtSetArg (args[i], XmNbaseWidth, 350); i++;
	XtSetArg (args[i], XmNbaseHeight, 550); i++;
	XtSetArg (args[i], XmNmaxWidth, 350); i++;
	XtSetArg (args[i], XmNmaxHeight, 200); i++;
	dlgLD = XmCreateFormDialog(app_shell, "dlgLD", args, i);

	XtAddCallback (XtParent(dlgLD), XmNpopdownCallback, QuitaVentanaLD, NULL);
	
	i=0;
	Widget col=XmCreateRowColumn(dlgLD, "ColumnaLD", args, i);
	
	//Los tres frames para las tres opciones de cálculo de límites de detección.
	Widget f1, f2, f3;

	//********** Frame sencillo ****
	i=0;
	XtSetArg (args[i], XmNwidth, 350); i++;
	XtSetArg (args[i], XmNheight,150); i++;
	f1=XmCreateFrame(col, "Frame simple", args, i);
 	
	cadena = XmStringCreateLocalized ("From peaks identified");
    i = 0;
	XtSetArg (args[i], XmNchildType, XmFRAME_TITLE_CHILD); i++;
   	XtSetArg (args[i], XmNchildVerticalAlignment, XmALIGNMENT_CENTER); i++;
   	XtSetArg (args[i], XmNlabelString, cadena); i++;
    Widget titulof1 = XmCreateLabelGadget (f1, "label_frame_analisis", args, i);
    XmStringFree (cadena);
	XtManageChild(titulof1);
	
	//******************* Frame que lee de fichero ****************
	i=0;
	XtSetArg (args[i], XmNwidth, 350); i++;
	XtSetArg (args[i], XmNheight, 150); i++;
	f2=XmCreateFrame(col, "Frame archivo", args, i);
	
	cadena = XmStringCreateLocalized ("From emissions in file");
    i = 0;
	XtSetArg (args[i], XmNchildType, XmFRAME_TITLE_CHILD); i++;
   	XtSetArg (args[i], XmNchildVerticalAlignment, XmALIGNMENT_CENTER); i++;
   	XtSetArg (args[i], XmNlabelString, cadena); i++;
    Widget titulof2 = XmCreateLabelGadget (f2, "label_frame_analisis", args, i);
    XmStringFree (cadena);
	XtManageChild(titulof2);

	//Columna para colocar los frames de fichero
	i=0;
	Widget colf=XmCreateRowColumn(f2, "ColumnaLD", args, i);

	i = 0;
    cadena = XmStringCreateLocalized (" Open File ... ");
    XtSetArg (args[i], XmNlabelString, cadena); i++;
   	//
	XtSetArg (args[i], XmNchildVerticalAlignment, XmALIGNMENT_CENTER); i++;
	XtSetArg (args[i], XmNwidth, 100); i++;
	XtSetArg (args[i], XmNheight, 20); i++;
	//
	Widget botonOpen = XmCreatePushButton(colf, "Botón para buscar fichero", args, i);
	XtAddCallback (botonOpen, XmNactivateCallback, pulsaOpenFile, NULL);
	//printf("PULSA OPEN FILE\n");
    XmStringFree (cadena);
	XtManageChild(botonOpen);
		
	//******************* frame "personalizado" ****************
	i=0;
	XtSetArg (args[i], XmNwidth, 350); i++;
	XtSetArg (args[i], XmNheight, 250); i++;
	f3=XmCreateFrame(col, "Frame personalizado", args, i);

	cadena = XmStringCreateLocalized ("From an specific radionuclid");
    i = 0;
	XtSetArg (args[i], XmNchildType, XmFRAME_TITLE_CHILD); i++;
   	XtSetArg (args[i], XmNchildVerticalAlignment, XmALIGNMENT_CENTER); i++;
   	XtSetArg (args[i], XmNlabelString, cadena); i++;
    Widget titulof3 = XmCreateLabelGadget (f3, "label_frame_analisis", args, i);
    XmStringFree (cadena);
	XtManageChild(titulof3);

	i=0;
	Widget columna1f3=XmCreateRowColumn(f3, "columna1f3", args, i);

	cadena = XmStringCreateLocalized ("Atomic mass:");
    i = 0;
	XtSetArg (args[i], XmNchildType, XmFRAME_TITLE_CHILD); i++;
   	XtSetArg (args[i], XmNchildVerticalAlignment, XmALIGNMENT_CENTER); i++;
   	XtSetArg (args[i], XmNlabelString, cadena); i++;
    Widget l1f3 = XmCreateLabelGadget (columna1f3, "l1f3", args, i);
    XmStringFree (cadena);
	XtManageChild(l1f3);
	
	i = 0;
   	XtSetArg (args[i], XmNcolumns, 10); i++;
   	XtSetArg (args[i], XmNmaxLength, 30); i++;
	txt_proton = XmCreateTextField(columna1f3, "txt_proton", args, i);
	XtAddCallback (txt_proton, XmNmotionVerifyCallback, escribeProton, NULL);
	XtManageChild(txt_proton);

	cadena = XmStringCreateLocalized ("Radionuclid:");
    i = 0;
	XtSetArg (args[i], XmNchildType, XmFRAME_TITLE_CHILD); i++;
   	XtSetArg (args[i], XmNchildVerticalAlignment, XmALIGNMENT_CENTER); i++;
   	XtSetArg (args[i], XmNlabelString, cadena); i++;
    Widget l2f3 = XmCreateLabelGadget (columna1f3, "l2f3", args, i);
    XmStringFree (cadena);
	XtManageChild(l2f3);

	i = 0;
	XtSetArg (args[i], XmNlistSizePolicy, XmCONSTANT); i++;
	XtSetArg (args[i], XmNselectionPolicy, XmSINGLE_SELECT); i++;
	XtSetArg (args[i], XmNvisibleItemCount, 8); i++;
	XtSetArg (args[i], XmNscrollBarDisplayPolicy, XmAS_NEEDED); i++;
	listaRN = XmCreateScrolledList(columna1f3, "lista de radionúclidos", args, i);
	XtAddCallback (listaRN, XmNsingleSelectionCallback, seleccionaElemento, NULL);
	XtManageChild(listaRN);

	cadena = XmStringCreateLocalized ("Emission:");
    i = 0;
	XtSetArg (args[i], XmNchildType, XmFRAME_TITLE_CHILD); i++;
   	XtSetArg (args[i], XmNchildVerticalAlignment, XmALIGNMENT_CENTER); i++;
   	XtSetArg (args[i], XmNlabelString, cadena); i++;
    Widget l3f3 = XmCreateLabelGadget (columna1f3, "l3f3", args, i);
    XmStringFree (cadena);
	XtManageChild(l3f3);

	i = 0;
	XtSetArg (args[i], XmNlistSizePolicy, XmCONSTANT); i++;
	XtSetArg (args[i], XmNselectionPolicy, XmSINGLE_SELECT); i++;
	XtSetArg (args[i], XmNvisibleItemCount, 5); i++;
	XtSetArg (args[i], XmNscrollBarDisplayPolicy, XmAS_NEEDED); i++;
    listaE = XmCreateScrolledList(columna1f3, "lista de emisiones", args, i);
	XtAddCallback (listaE, XmNsingleSelectionCallback, seleccionaEmision, NULL);
	XtManageChild(listaE);

	//Creamos listas con los límites de detección
	Widget lista;
	i = 0;
	XtSetArg (args[i], XmNlistSizePolicy, XmCONSTANT); i++;
	XtSetArg (args[i], XmNselectionPolicy, XmSINGLE_SELECT); i++;
	XtSetArg (args[i], XmNvisibleItemCount, 6); i++;
	XtSetArg (args[i], XmNscrollBarDisplayPolicy, XmAS_NEEDED); i++;
    lista = XmCreateScrolledList(f1, "lista de límites de detección", args, i);
	
	i = 0;
	XtSetArg (args[i], XmNlistSizePolicy, XmCONSTANT); i++;
	XtSetArg (args[i], XmNselectionPolicy, XmSINGLE_SELECT); i++;
	XtSetArg (args[i], XmNvisibleItemCount, 6); i++;
	XtSetArg (args[i], XmNscrollBarDisplayPolicy, XmAS_NEEDED); i++;
    lista2 = XmCreateScrolledList(colf, "lista de límites de detección para readionúclidos", args, i);
	XtManageChild(lista2);
	
	i = 0;
	XtSetArg (args[i], XmNlistSizePolicy, XmCONSTANT); i++;
	XtSetArg (args[i], XmNselectionPolicy, XmSINGLE_SELECT); i++;
	XtSetArg (args[i], XmNvisibleItemCount, 6); i++;
	XtSetArg (args[i], XmNscrollBarDisplayPolicy, XmAS_NEEDED); i++;
    lista3 = XmCreateScrolledList(columna1f3, "lista de ld personalizada", args, i);
	XtManageChild(lista3);

	//*** Cálculo de los límites de detección para los picos identificados
	Datos.calculaLD(); 
	
	double e;
	double ld;
	char texto[80];
	for(int i=1;i<=Datos.listaPicos.numElementos;i++)
		{
		NodoPicos *picos=DevuelveNodo(Datos.listaPicos, i);
		//NOTA: Como en otros sitios, sólo estamos cogiendo un pico de cada intervalo
		//		de picos (consideramos intervalos simples)
		if(picos==NULL)	{printf("ERROR: Lista de picos a NULL\n"); return;}
		NodoPico  *nodo =DevuelveNodo(picos->picos, 1);
		if(nodo==NULL)	{printf("ERROR: Lista de picos a NULL\n"); return;}
//		if(nodo->Ajuste.identificado && !nodo->Ajuste.calibrado)
		if(nodo->Ajuste.identificado && !nodo->Ajuste.calibrado && !nodo->Ajuste.invisible)
			{
			//
			for(int j=0;j<nodo->Ajuste.emisiones->numElements();j++)//NOTA: Todos estos bucles de momento sólo se quedan con el último valor y fuera
				{
				NodoEmision *ne=(NodoEmision *)nodo->Ajuste.emisiones->getNode(j);
				e=ne->Energia;
				}
			ld=nodo->Ajuste.ld;
			sprintf(texto, "Peak of %.2f keV has ld=%.2f", e, ld);
			cadena = XmStringCreateLocalized(texto);
			XmListAddItemUnselected(lista, cadena, 0);
			XmStringFree (cadena); 
			//
			
			//e=nodo->Ajuste.Energia;
			//ld=nodo->Ajuste.ld;
			//sprintf(texto, "Peak of %.2f keV has ld=%.2f", e, ld);
			//cadena = XmStringCreateLocalized(texto);
			//XmListAddItemUnselected(lista, cadena, 0);
			//XmStringFree (cadena); 
				
			}	
		}
	
	XtManageChild(lista);
	XtManageChild(colf);
	XtManageChild(col);
	XtManageChild(columna1f3);
	XtManageChild(f1);
	XtManageChild(f2);
	XtManageChild(f3);
	XtManageChild(dlgLD);
	//NOTA: Guardamos los resultados de ld en un fichero provisionalmente para
	//que puedan usar los datos los programas fortran
	string nombre(Datos.nombre);
	int ind=nombre.find(".");
	if(ind!=-1)	nombre.replace(ind, nombre.length()-ind, ".ld");
	cout << nombre << endl;
	//*** Guardamos un fichero con el nombre del archivo abierto y extensión .ld
	guardarLD((char *)nombre.c_str());
	//Reflejamos en el estado de trabajo que ya se ha realizado el cálculo de los
	//limites de detección
	EstadoTrabajo.AnalisisLD=true;
	}


//************** CREA DIALOGO LD ***********************
/* Nos crea un diálogo para los distintos cálculos de los límites
	de detección. Constará de tres apartados, según el tipo de cálculo
	que realicemos:
	1)Cálculo sobre las emisiones identificadas en la calibración:
		Sólo tendrá un botón para realizar el ajustes
	2)Cálculo sobre  las emisiones que estén en un fichero de radionúclidos
		Abrirá un cuadro de exploración de ficheros de la extensión que le 
		pongamos.
	3)Cálculo personalizado sobre un determinado radionúclido:
		Permitirá elegir número de protones, tipo de radionúclido y energía
	*/
void creaDialogoLD_2()
	{
	Arg			args[7];
   	Cardinal	i = 0;
	XmString 	cadena;
	
	// *********************
	// * cuadro de dialogo *
	// *********************
	printf("CREA DIALOGO LD\n");
	i = 0;
	XtSetArg (args[i], XmNtitle, "Detection Limits"); i++;
	XtSetArg (args[i], XmNmwmDecorations, MWM_DECOR_BORDER|MWM_DECOR_TITLE|MWM_DECOR_MENU); i++;
	XtSetArg (args[i], XmNmwmFunctions, MWM_FUNC_MOVE|MWM_FUNC_CLOSE); i++;
	XtSetArg (args[i], XmNbaseWidth, 350); i++;
	XtSetArg (args[i], XmNbaseHeight, 550); i++;
	XtSetArg (args[i], XmNmaxWidth, 350); i++;
	XtSetArg (args[i], XmNmaxHeight, 200); i++;
	dlgLD = XmCreateFormDialog(app_shell, "dlgLD", args, i);

	XtAddCallback (XtParent(dlgLD), XmNpopdownCallback, QuitaVentanaLD, NULL);
	
	i=0;
	Widget col=XmCreateRowColumn(dlgLD, "ColumnaLD", args, i);
	
	//Los tres frames para las tres opciones de cálculo de límites de detección.
	Widget f1, f2, f3;

	//********** Frame sencillo ****
	i=0;
	XtSetArg (args[i], XmNwidth, 350); i++;
	XtSetArg (args[i], XmNheight,150); i++;
	f1=XmCreateFrame(col, "Frame simple", args, i);
 	
	cadena = XmStringCreateLocalized ("From peaks identified");
    i = 0;
	XtSetArg (args[i], XmNchildType, XmFRAME_TITLE_CHILD); i++;
   	XtSetArg (args[i], XmNchildVerticalAlignment, XmALIGNMENT_CENTER); i++;
   	XtSetArg (args[i], XmNlabelString, cadena); i++;
    Widget titulof1 = XmCreateLabelGadget (f1, "label_frame_analisis", args, i);
    XmStringFree (cadena);
	XtManageChild(titulof1);
	
	//******************* Frame que lee de fichero ****************
	i=0;
	XtSetArg (args[i], XmNwidth, 350); i++;
	XtSetArg (args[i], XmNheight, 150); i++;
	f2=XmCreateFrame(col, "Frame archivo", args, i);
	
	cadena = XmStringCreateLocalized ("From emissions in file");
    i = 0;
	XtSetArg (args[i], XmNchildType, XmFRAME_TITLE_CHILD); i++;
   	XtSetArg (args[i], XmNchildVerticalAlignment, XmALIGNMENT_CENTER); i++;
   	XtSetArg (args[i], XmNlabelString, cadena); i++;
    Widget titulof2 = XmCreateLabelGadget (f2, "label_frame_analisis", args, i);
    XmStringFree (cadena);
	XtManageChild(titulof2);

	//Columna para colocar los frames de fichero
	i=0;
	Widget colf=XmCreateRowColumn(f2, "ColumnaLD", args, i);

	i = 0;
    cadena = XmStringCreateLocalized (" Open File ... ");
    XtSetArg (args[i], XmNlabelString, cadena); i++;
   	//
	XtSetArg (args[i], XmNchildVerticalAlignment, XmALIGNMENT_CENTER); i++;
	XtSetArg (args[i], XmNwidth, 100); i++;
	XtSetArg (args[i], XmNheight, 20); i++;
	//
	Widget botonOpen = XmCreatePushButton(colf, "Botón para buscar fichero", args, i);
	XtAddCallback (botonOpen, XmNactivateCallback, pulsaOpenFile_2, NULL);
	//printf("PULSA OPEN FILE\n");
    XmStringFree (cadena);
	XtManageChild(botonOpen);
		
	//******************* frame "personalizado" ****************
	i=0;
	XtSetArg (args[i], XmNwidth, 350); i++;
	XtSetArg (args[i], XmNheight, 250); i++;
	f3=XmCreateFrame(col, "Frame personalizado", args, i);

	cadena = XmStringCreateLocalized ("From an specific radionuclid");
    i = 0;
	XtSetArg (args[i], XmNchildType, XmFRAME_TITLE_CHILD); i++;
   	XtSetArg (args[i], XmNchildVerticalAlignment, XmALIGNMENT_CENTER); i++;
   	XtSetArg (args[i], XmNlabelString, cadena); i++;
    Widget titulof3 = XmCreateLabelGadget (f3, "label_frame_analisis", args, i);
    XmStringFree (cadena);
	XtManageChild(titulof3);

	i=0;
	Widget columna1f3=XmCreateRowColumn(f3, "columna1f3", args, i);

	cadena = XmStringCreateLocalized ("Atomic mass:");
    i = 0;
	XtSetArg (args[i], XmNchildType, XmFRAME_TITLE_CHILD); i++;
   	XtSetArg (args[i], XmNchildVerticalAlignment, XmALIGNMENT_CENTER); i++;
   	XtSetArg (args[i], XmNlabelString, cadena); i++;
    Widget l1f3 = XmCreateLabelGadget (columna1f3, "l1f3", args, i);
    XmStringFree (cadena);
	XtManageChild(l1f3);
	
	i = 0;
   	XtSetArg (args[i], XmNcolumns, 10); i++;
   	XtSetArg (args[i], XmNmaxLength, 30); i++;
	txt_proton = XmCreateTextField(columna1f3, "txt_proton", args, i);
	XtAddCallback (txt_proton, XmNmotionVerifyCallback, escribeProton, NULL);
	XtManageChild(txt_proton);

	cadena = XmStringCreateLocalized ("Radionuclid:");
    i = 0;
	XtSetArg (args[i], XmNchildType, XmFRAME_TITLE_CHILD); i++;
   	XtSetArg (args[i], XmNchildVerticalAlignment, XmALIGNMENT_CENTER); i++;
   	XtSetArg (args[i], XmNlabelString, cadena); i++;
    Widget l2f3 = XmCreateLabelGadget (columna1f3, "l2f3", args, i);
    XmStringFree (cadena);
	XtManageChild(l2f3);

	i = 0;
	XtSetArg (args[i], XmNlistSizePolicy, XmCONSTANT); i++;
	XtSetArg (args[i], XmNselectionPolicy, XmSINGLE_SELECT); i++;
	XtSetArg (args[i], XmNvisibleItemCount, 8); i++;
	XtSetArg (args[i], XmNscrollBarDisplayPolicy, XmAS_NEEDED); i++;
	listaRN = XmCreateScrolledList(columna1f3, "lista de radionúclidos", args, i);
	XtAddCallback (listaRN, XmNsingleSelectionCallback, seleccionaElemento, NULL);
	XtManageChild(listaRN);

	cadena = XmStringCreateLocalized ("Emission:");
    i = 0;
	XtSetArg (args[i], XmNchildType, XmFRAME_TITLE_CHILD); i++;
   	XtSetArg (args[i], XmNchildVerticalAlignment, XmALIGNMENT_CENTER); i++;
   	XtSetArg (args[i], XmNlabelString, cadena); i++;
    Widget l3f3 = XmCreateLabelGadget (columna1f3, "l3f3", args, i);
    XmStringFree (cadena);
	XtManageChild(l3f3);

	i = 0;
	XtSetArg (args[i], XmNlistSizePolicy, XmCONSTANT); i++;
	XtSetArg (args[i], XmNselectionPolicy, XmSINGLE_SELECT); i++;
	XtSetArg (args[i], XmNvisibleItemCount, 5); i++;
	XtSetArg (args[i], XmNscrollBarDisplayPolicy, XmAS_NEEDED); i++;
    listaE = XmCreateScrolledList(columna1f3, "lista de emisiones", args, i);
	XtAddCallback (listaE, XmNsingleSelectionCallback, seleccionaEmision, NULL);
	XtManageChild(listaE);

	//Creamos listas con los límites de detección
	Widget lista;
	i = 0;
	XtSetArg (args[i], XmNlistSizePolicy, XmCONSTANT); i++;
	XtSetArg (args[i], XmNselectionPolicy, XmSINGLE_SELECT); i++;
	XtSetArg (args[i], XmNvisibleItemCount, 6); i++;
	XtSetArg (args[i], XmNscrollBarDisplayPolicy, XmAS_NEEDED); i++;
    lista = XmCreateScrolledList(f1, "lista de límites de detección", args, i);
	
	i = 0;
	XtSetArg (args[i], XmNlistSizePolicy, XmCONSTANT); i++;
	XtSetArg (args[i], XmNselectionPolicy, XmSINGLE_SELECT); i++;
	XtSetArg (args[i], XmNvisibleItemCount, 6); i++;
	XtSetArg (args[i], XmNscrollBarDisplayPolicy, XmAS_NEEDED); i++;
    lista2 = XmCreateScrolledList(colf, "lista de límites de detección para readionúclidos", args, i);
	XtManageChild(lista2);
	
	i = 0;
	XtSetArg (args[i], XmNlistSizePolicy, XmCONSTANT); i++;
	XtSetArg (args[i], XmNselectionPolicy, XmSINGLE_SELECT); i++;
	XtSetArg (args[i], XmNvisibleItemCount, 6); i++;
	XtSetArg (args[i], XmNscrollBarDisplayPolicy, XmAS_NEEDED); i++;
    lista3 = XmCreateScrolledList(columna1f3, "lista de ld personalizada", args, i);
	XtManageChild(lista3);

	//*** Cálculo de los límites de detección para los picos identificados
	Datos.calculaLD(); 
	
	double e;
	double ld;
	char texto[80];
	for(int i=1;i<=Datos.listaPicos.numElementos;i++)
		{
		NodoPicos *picos=DevuelveNodo(Datos.listaPicos, i);
		//NOTA: Como en otros sitios, sólo estamos cogiendo un pico de cada intervalo
		//		de picos (consideramos intervalos simples)
		if(picos==NULL)	{printf("ERROR: Lista de picos a NULL\n"); return;}
		NodoPico  *nodo =DevuelveNodo(picos->picos, 1);
		if(nodo==NULL)	{printf("ERROR: Lista de picos a NULL\n"); return;}
//		if(nodo->Ajuste.identificado && !nodo->Ajuste.calibrado)
		if(nodo->Ajuste.identificado && !nodo->Ajuste.calibrado && !nodo->Ajuste.invisible)
			{
			//
			for(int j=0;j<nodo->Ajuste.emisiones->numElements();j++)//NOTA: Todos estos bucles de momento sólo se quedan con el último valor y fuera
				{
				NodoEmision *ne=(NodoEmision *)nodo->Ajuste.emisiones->getNode(j);
				e=ne->Energia;
				}
			ld=nodo->Ajuste.ld;
			sprintf(texto, "Peak of %.2f keV has ld=%.2f", e, ld);
			cadena = XmStringCreateLocalized(texto);
			XmListAddItemUnselected(lista, cadena, 0);
			XmStringFree (cadena); 
			//
			
			//e=nodo->Ajuste.Energia;
			//ld=nodo->Ajuste.ld;
			//sprintf(texto, "Peak of %.2f keV has ld=%.2f", e, ld);
			//cadena = XmStringCreateLocalized(texto);
			//XmListAddItemUnselected(lista, cadena, 0);
			//XmStringFree (cadena); 
				
			}	
		}
	
	XtManageChild(lista);
	XtManageChild(colf);
	XtManageChild(col);
	XtManageChild(columna1f3);
	XtManageChild(f1);
	XtManageChild(f2);
	XtManageChild(f3);
	XtManageChild(dlgLD);
	//NOTA: Guardamos los resultados de ld en un fichero provisionalmente para
	//que puedan usar los datos los programas fortran
	string nombre(Datos.nombre);
	int ind=nombre.find(".");
	if(ind!=-1)	nombre.replace(ind, nombre.length()-ind, ".ld");
	cout << nombre << endl;
	//*** Guardamos un fichero con el nombre del archivo abierto y extensión .ld
	guardarLD((char *)nombre.c_str());
	//Reflejamos en el estado de trabajo que ya se ha realizado el cálculo de los
	//limites de detección
	EstadoTrabajo.AnalisisLD=true;
	}


//************** CREA DIALOGO LC ***********************
/* Nos crea un diálogo para el cálculo del límite de decisión. 
	Pregunta por la base de datos que se usará para los fondos
	Muestra los límites de decisión calculados para los picos
	identificados.
	*/
void creaDialogoLC(Widget widget, XtPointer client_data, XtPointer call_data)
	{
	Arg			args[7];
   	Cardinal	i = 0;
	XmString 	cadena;
	
	// *********************
	// * cuadro de dialogo *
	// *********************
	if(dlgFBD!=NULL)	XtUnmanageChild(dlgFBD);
	printf("CREA DIALOGO LC\n");
	i = 0;
	XtSetArg (args[i], XmNtitle, "Decision Limits"); i++;
	XtSetArg (args[i], XmNmwmDecorations, MWM_DECOR_BORDER|MWM_DECOR_TITLE|MWM_DECOR_MENU); i++;
	XtSetArg (args[i], XmNmwmFunctions, MWM_FUNC_MOVE|MWM_FUNC_CLOSE); i++;
	XtSetArg (args[i], XmNbaseWidth, 350); i++;
	XtSetArg (args[i], XmNbaseHeight, 200); i++;
	XtSetArg (args[i], XmNmaxWidth, 350); i++;
	XtSetArg (args[i], XmNmaxHeight, 200); i++;
	dlgLC = XmCreateFormDialog(app_shell, "dlgLC", args, i);

	XtAddCallback (XtParent(dlgLC), XmNpopdownCallback, QuitaVentanaLC, NULL);
	
	i=0;
	Widget col=XmCreateRowColumn(dlgLC, "ColumnaLC", args, i);
	
	Widget f1;
	printf("Creamos frame sencillo\n");
	//********** Frame sencillo ****
	i=0;
	XtSetArg (args[i], XmNwidth, 350); i++;
	XtSetArg (args[i], XmNheight,200); i++;
	f1=XmCreateFrame(col, "Frame simple", args, i);
 	
	cadena = XmStringCreateLocalized ("From peaks identified");
    i = 0;
	XtSetArg (args[i], XmNchildType, XmFRAME_TITLE_CHILD); i++;
   	XtSetArg (args[i], XmNchildVerticalAlignment, XmALIGNMENT_CENTER); i++;
   	XtSetArg (args[i], XmNlabelString, cadena); i++;
    Widget titulof1 = XmCreateLabelGadget (f1, "label_frame_analisis", args, i);
    XmStringFree (cadena);
	XtManageChild(titulof1);
	
	printf("Creamos lista con LC\n");
	//Creamos la lista con los límites de decisión
	Widget lista;
	i = 0;
	XtSetArg (args[i], XmNlistSizePolicy, XmCONSTANT); i++;
	XtSetArg (args[i], XmNselectionPolicy, XmSINGLE_SELECT); i++;
	XtSetArg (args[i], XmNvisibleItemCount, 6); i++;
	XtSetArg (args[i], XmNscrollBarDisplayPolicy, XmAS_NEEDED); i++;
    lista = XmCreateScrolledList(f1, "lista de límites de decisión", args, i);
	
	Datos.calculaLC();//Calculamos el lc para todos los picos identificados hasta ahora
	double e;
	double lc;
	char texto[80];
	for(int i=1;i<=Datos.listaPicos.numElementos;i++)
		{
		NodoPicos *picos=DevuelveNodo(Datos.listaPicos, i);
		if(picos==NULL)	{printf("ERROR: Lista de picos a NULL\n"); return;}
		for(int k=1;k<=picos->picos.numElementos;k++)
			{
			NodoPico *nodo =DevuelveNodo(picos->picos, k);
			if(nodo==NULL)	{printf("ERROR: Lista de picos a NULL\n"); return;}
			if(nodo->Ajuste.analizado)	
				{
				e=Datos.DaEnergia(nodo->Ajuste.Centroide);
				lc=nodo->Ajuste.lc;
				printf("LC=%f\tAREA=%f\n", nodo->Ajuste.lc, nodo->Ajuste.area);	
				if(nodo->Ajuste.lc>nodo->Ajuste.area)	
					{
					printf("PICO A ELIMINAR, ÁREA DESPRECIABLE\n");
					sprintf(texto, "Peak of %.2f keV has lc=%.2f * (%.2f) ld=%.2f", e, lc, nodo->Ajuste.area, nodo->Ajuste.ld);
					}
				else	sprintf(texto, "Peak of %.2f keV has lc=%.2f (%.2f) ld=%.2f", e, lc, nodo->Ajuste.area, nodo->Ajuste.ld);
				cadena = XmStringCreateLocalized(texto);
				XmListAddItemUnselected(lista, cadena, 0);
				XmStringFree (cadena); 
				}
			}
		}
	XtManageChild(lista);
	XtManageChild(f1);
	XtManageChild(col);
	XtManageChild(dlgLC);
		
	//NOTA: Para los límites de decisión esta opción de guardar los límites no
	//		la contemplamos de momento.
	/*string nombre(Datos.nombre);
	int ind=nombre.find(".");
	if(ind!=-1)	nombre.replace(ind, nombre.length()-ind, ".ld");
	cout << nombre << endl;
	//+++ Guardamos un fichero con el nombre del archivo abierto y extensión .ld
	guardarLC((char *)nombre.c_str());*/
	//Reflejamos en el estado de trabajo que ya se ha realizado el cálculo de los
	//limites de detección
	//NOTA: De momento no contamos el análisisLC como un estado de trabajo
	EstadoTrabajo.AnalisisLD=true;
	printf("El AnálisisLD está, el de la eficiencia es: %d\n", EstadoTrabajo.AnalisisEficiencia);
	if(EstadoTrabajo.AnalisisEficiencia)	
		{
		printf("Activamos MDA");
		XtSetSensitive(action_activity_actividad, true);
		XtSetSensitive(action_MDA_actividad, true);
		}
		
	// Indica cambio en el trabajo
	EstadoTrabajo.TrabajoCambios = true;
	PonTituloFichCambiado();
	}

//**************** QUITA VENTANA LD ****************
// Pone el botón correspondiente a no pulsado
void QuitaVentanaLD(Widget widget, XtPointer client_data, XtPointer call_data) 
	{
	XtUnmanageChild(dlgLD);
	XmToggleButtonGadgetSetState(action_efficiency_actividad, false, true);
	}

//**************** QUITA VENTANA LC ****************
// Pone el botón correspondiente a no pulsado
void QuitaVentanaLC(Widget widget, XtPointer client_data, XtPointer call_data) 
	{
	printf("Quita ventalan LC\n");
	XtUnmanageChild(dlgLC);
	XmToggleButtonGadgetSetState(action_limits_actividad, false, true);
	}
	
//********************** GUARDAR LD *********************
/* 	Función provisional para guardar los resultados de los límites
	de detección. Terminará añadiéndose a guardarAnálisis() de Analgam.C
	NOTA: Eliminar
*/
void guardarLD(String nombre)
	{
	FILE *fDatos;
	// Abre un fichero para su escritura
	if ((fDatos = fopen(nombre, "w")) == NULL) 
		{
		printf(ERROR_SAVE_FILE);
		printf(ERROR_NO_SAVE_FILE);
		return;
		}
	double e;
	double ld;

	for(int i=1;i<=Datos.listaPicos.numElementos;i++)
		{
		NodoPicos *picos=DevuelveNodo(Datos.listaPicos, i);
		//NOTA: Como en otros sitios, sólo estamos cogiendo un pico de cada intervalo
		//		de picos (consideramos intervalos simples)
		if(picos==NULL)	{printf("ERROR: Lista de picos a NULL\n"); return;}
		NodoPico  *nodo =DevuelveNodo(picos->picos, 1);
		if(nodo==NULL)	{printf("ERROR: Lista de picos a NULL\n"); return;}
//		if(nodo->Ajuste.identificado && !nodo->Ajuste.calibrado)	
		if(nodo->Ajuste.identificado && !nodo->Ajuste.calibrado && !nodo->Ajuste.invisible)	
			{
			for(int j=0;j<nodo->Ajuste.emisiones->numElements();j++)
				{
				NodoEmision *ne=(NodoEmision *)nodo->Ajuste.emisiones->getNode(j);
				e=ne->Energia;
				}
//			e=nodo->Ajuste.Energia;
			ld=nodo->Ajuste.ld;
			printf("Interval of %.2f keV has ld=%.2f\n", e, ld);
			fprintf(fDatos, "%.2f\t%.2f\n", e, ld);
			}	
		}
	fclose(fDatos);
	}

//********************** GUARDAR LD (2) *********************
/* 	Función provisional para guardar los resultados de los límites
	de detección. En este caso guarda los límites de detección de 
	emisiones correspondientes a radionúclidos tomados de un fichero,
	no a las emisiones detectadas
*/
void guardarLD(const char *nombre, nodoCentroide *lista)
	{
	FILE *fDatos;
	// Abre un fichero para su escritura
	cout << "Salvando en " << nombre << endl;
	if ((fDatos = fopen(nombre, "w")) == NULL) 
		{
		printf(ERROR_SAVE_FILE);
		printf(ERROR_NO_SAVE_FILE);
		return;
		}
		
	float e, ld, amd;
	nodoCentroide *temp=lista;
	while(temp!=NULL)
		{
		e=temp->centroide;
		ld=temp->ld;
		amd=temp->amd;	//felipe
		int atomicnumber=temp->atomicnumber;
		int massnumber =temp->massnumber;
		printf("Interval of %.2f keV has ld=%.2f\n", e, ld);
		fprintf(fDatos, "%d\t%d\t%.4f\t\t%.4f\t\t%.4f\n", atomicnumber, massnumber, e, ld, amd);
		temp=temp->siguiente;
		}

	fclose(fDatos);
	cout << "Sale de guardarLD " << nombre << endl;
	}
	
//**************** QUITA VENTANA EFI ****************
// Pone el botón correspondiente a no pulsado
void QuitaVentanaEfi(Widget widget, XtPointer client_data, XtPointer call_data) 
	{
	XmToggleButtonGadgetSetState(action_efficiency_actividad, false, true);
	XtUnmanageChild(dlgEfi);
	XtUnmanageChild(dlg_filesPar);
	}
	
//**************** QUITA VENTANA ACT ****************
// Pone el botón correspondiente a no pulsado
void QuitaVentanaAct(Widget widget, XtPointer client_data, XtPointer call_data) 
	{
	XmToggleButtonGadgetSetState(action_activity_actividad, false, true);
	XtUnmanageChild(dlgAct);
	XtUnmanageChild(dlgFBD);
	}

//**************** QUITA VENTANA MDA ****************
// Pone el botón correspondiente a no pulsado
void QuitaVentanaMDA(Widget widget, XtPointer client_data, XtPointer call_data) 
	{
	XtUnmanageChild(dlgMDA);
	XmToggleButtonGadgetSetState(action_MDA_actividad, false, true);
	}

//***************** CALCULA ÁREA PICOS *****************************
/* Calcula el área de los picos restando a cada pico el área del fondo si 
	existiera algún pico de fondo en la energía de dicho pico.
	*/
void calculaAreaPicos(double *energias, double *medias, double *incertidumbres, int numEnergias)
	{
	printf("CALCULA ÁREA PICOS\n");
	for(int i=1;i<=Datos.listaPicos.numElementos;i++)
		{
		NodoPicos *nodoPicos=DevuelveNodo(Datos.listaPicos, i);
		for(int j=1;j<=nodoPicos->picos.numElementos;j++)
			{
			NodoPico  *nodo      = DevuelveNodo(nodoPicos->picos, j);
			if(nodo->Ajuste.identificado)
				{
				double minEnergia    = Datos.DaEnergia((double)nodoPicos->limInferior);
				double maxEnergia    = Datos.DaEnergia((double)nodoPicos->limSuperior)+1;
				double area		     = Datos.DaAreaPico(i,j);
				//printf("Área del pico %d,%d (%f keV) es %f\n", i,j,Datos.DaEnergia(nodo->Ajuste.Centroide), area);
				double incertidumbre = Datos.DaIncerAreaPico(i,j);
				int k;
				for(k=0;k<numEnergias;k++)
					{
					if(energias[k]>=minEnergia && energias[k]<=maxEnergia)
						{//Hay que restarle este pico del fondo
						//Multiplicamos la media del fondo (que va por unidad de tiempo)
						//por el tiempo que medimos esta muestra
						//El número de cuentas está medido como 10¯6, luego debemos dividir
						//dicho número entre un millón.
					//	nodo->Ajuste.areaSinFondo=area-(medias[k]*Datos.tiempo/1000000.0);
						nodo->Ajuste.areaSinFondo=area-medias[k];
						printf("Area de %f keV es %f=%f -%f(%f*%f/10^6)\n", Datos.DaEnergia(nodo->Ajuste.Centroide), nodo->Ajuste.areaSinFondo, area, medias[k], medias[k], Datos.tiempo);
						if(nodo->Ajuste.areaSinFondo<0)	nodo->Ajuste.areaSinFondo=0;//
					//	nodo->Ajuste.incerAreaSinFondo=sqrt(incertidumbre*incertidumbre+pow((incertidumbres[k]*Datos.tiempo/1000000),2));
						nodo->Ajuste.incerAreaSinFondo=sqrt(incertidumbre*incertidumbre+pow(incertidumbres[k],2));
						printf("Incertidumbre es %f+%f=%f\n", incertidumbre*incertidumbre, pow(incertidumbres[k],2), nodo->Ajuste.incerAreaSinFondo);			
						break;
						}
					}//for(fondos)
				if(k==numEnergias)	
					{
					nodo->Ajuste.areaSinFondo=area;
					nodo->Ajuste.incerAreaSinFondo=incertidumbre;
					//printf("Área del pico %d,%d (%f keV) almacenada es %f\n", i,j,Datos.DaEnergia(nodo->Ajuste.Centroide), area);
					}
				}//if(identificado)	
			}//For(picos)
		}//for(intervalos)
	}


//********************** RECOGE FACTORES CORRECCIÓN ********************
// Coge el nombre donde se encuentra el fichero con los factores de corrección
// Por defecto tomará uno con la misma extensión que el fichero de eficiencias.
// Si no lo encontrara, preguntará al usuario.
void recogeFactoresCorreccion(char *nombre)
	{
	printf("RECOGE FACTORES CORRECCIÓN \n");
	//Nos quedamos con la extensión del nombre del fichero de eficiencias:
	string cadf("");
	if(nombre!=NULL)
		{
		string cad(nombre);
		//cadena=cadena.substr(cadena.find("-")+1, cadena.length());
		cad.replace(cad.find("-")-2,2,"cs");
		cad.replace(cad.find(".")+1,3,"dat");
		cout << cad << endl;
		cadf=cad;
//		cout << "la cadena final queda: " << cadf << endl;
		}
	
	ifstream file(cadf.c_str());
	if (! file.is_open()) 	
		{//Si no encontramos fichero en el nombre por defecto preguntamos por uno 
	    Arg		args[5];
    	Cardinal	i = 0;

		i = 0;
		XtSetArg (args[i], XmNtitle, "Use efficiency correction file ..."); i++;
		dlgCSC = XmCreateFileSelectionDialog (app_shell, "csc_files", args, i);
		XtAddCallback (dlgCSC, XmNcancelCallback, pulsaCancelCSC, NULL);
		XtAddCallback (dlgCSC, XmNokCallback, pulsaOKCSC, NULL);

		//Ponemos el fichero de preferencias
		char *UltAnalDirOpen = Preferences.GetEfficiencyDir();
		XmString patron;
		
		if (UltAnalDirOpen != NULL) 
			{
			patron = XmStringCreateLocalized(UltAnalDirOpen);
			XtVaSetValues (dlgCSC, XmNdirectory, patron, NULL);
			XmStringFree (patron);
			}

		patron=XmStringCreateLocalized((char *)FILTER_FICH_BACKGROUND);
		XtVaSetValues (dlgCSC, XmNpattern, patron, NULL);
		XmStringFree(patron);

		XtManageChild(dlgCSC);
		}
	else
		{
		file.close();
//		printf("Construimos lista de corrección con %s\n", (char *)cadf.c_str());
		construyeListaCorreccion((char *)cadf.c_str());
		construyeMatrizPN();
		
		bool pulsadoActividad=XmToggleButtonGadgetGetState(action_activity_actividad);
		bool pulsadoMDA=	  XmToggleButtonGadgetGetState(action_MDA_actividad);
		printf("Actividad %d y MDA %d\n", pulsadoActividad, pulsadoMDA);
		if(pulsadoActividad && !pulsadoMDA)	calculaActividad();
		else if(pulsadoMDA)	calculaMDA();
		}
	}


//*********************** PULSA OK CSC ************************
void pulsaOKCSC(Widget widget, XtPointer client_data, XtPointer call_data)
	{
  	char *nombre = NULL, *dir = NULL;
	XmFileSelectionBoxCallbackStruct *cbs = (XmFileSelectionBoxCallbackStruct *) call_data;

   	XmStringGetLtoR(cbs->value, XmFONTLIST_DEFAULT_TAG, &nombre);
   	XmStringGetLtoR(cbs->dir, XmFONTLIST_DEFAULT_TAG, &dir);

	printf("Construimos lista de corrección con %s\n", cbs->value);
	construyeListaCorreccion(nombre);	
	construyeMatrizPN();		
	//NOTA: En este caso debemos recalcular la actividad porque se va a haber
	//		calculado previamente ya que no espera por la respuesta de este 
	//		cuadro de dialogo. No es que sea lo más elegante pero sí lo más sencillo
	//NOTA: Distinguimos entre calcular MDA y Actividad según qué botón esté pulsado
	if(XmToggleButtonGadgetGetState(action_activity_actividad))	calculaActividad();
	else if(XmToggleButtonGadgetGetState(action_MDA_actividad))	calculaMDA();
	XtUnmanageChild(dlgCSC);
	printf("Termina OKCSC\n");
	}

//**************** PULSA CANCEL CSC *************	
void pulsaCancelCSC(Widget widget, XtPointer client_data, XtPointer call_data)
	{
	printf("pulsaCancelCSC\n");
	XmToggleButtonGadgetSetState(action_activity_actividad, false, true);
	XmToggleButtonGadgetSetState(action_MDA_actividad, false, true);
	XtUnmanageChild(dlgCSC);
	}

//********************** CALCULA ACTIVIDAD RADIONÚCLIDOS ***********************
/*	Mientras en la función posterior calcularemos la actividad para un pico
	aislado, aquí queremos calcular la actividad para los radionúclidos.
	Cada matriz de actividad es realmente una lista de picos que tienen emisiones
	de radionúclidos relacionados, directa o indirectamente (es decir, si tenemos
	p1 con emisión de r1 y p2 con emisiones de r1 y r2; p1 estaría relacionado con
	un p3 con emisión en r2 (indirectamente a través de p2), así como con p2
	(directamente por r1).

	Este cálculo se realiza mediante ecuaciones matriciales. Si antes la formula
	era A=N/P*T*C*E; ahora N será un vector con las áreas de todos los picos y
	A será la actividad para las áreas de todos los picos; etc.

	Con lo cual tenemos:
	N=E*Pk*C*A; donde:

	N=vector de áreas de los picos. Dimensión (numPicosx1)	
	A=actividades de los radionúclidos (numRadx1)
	E=eficiencia de cada picos (numPicosx1). La vamos a pasar a dividir, quedando:
	
	N/E=Pk*C*A.
	
	Por último:
	Pk = matriz de probabilidades de cada emisión de cada pico->cada pico tiene un det número de emisiones, no parece que sea esto
			o la probabilidad de cada pico para cada radionúclido???->no puede ser porque no tenemos probabilidades para picos
			o la probabilidad de todas las emisiones relacionadas con un pico
			para cada radionúclido?????->Esto tiene bastante sentido
			
		Considerando lo tercero, su dim es (numRadxnumPicos)
		Problema con este tercero: no tenemos probabilidades de picos, sólo de 
		emisiones (en el caso muy raro de que haya más de una emisión del mismo
		radionúclido para un mismo pico, qué hacemos???)
	
	C = correcciones de la eficiencia para cada pico. Puede ser numPicox1 o si
		lo extendemos según emisiones numRadxnumPicos, con lo que se pueda mezclar
		con Pk
		
	En cuanto a las incertidumbres tenemos:
		Tenemos la incertidumbre de las áreas de los picos sin fondo.
		Tenemos la incertidumbre de las probabilidades(lara).
		Tenemos la incertidumbre de las eficiencias y su matriz de covarianza.
		La correcciones de la eficiencia también tienen su incertidumbre.
		
	Al ser matricial, usamos el ajuste de calefnu.
	
	Datos que necesitamos:
	
	AREA		Ajuste.area 		Ajuste.incerArea	(AjusteIntervaloGAML())
	EFICIENCIA	Ajuste.eficiencia	Ajuste.sigmaEficiencia	(calculaEficiencia())
	CORRECCION	Ajuste.correccion	Ajuste.sigmaCorreccion	(calculaCorreccionesEficiencia())
	PROBABILIDAD LaraBD.GetProbability() LaraBD.GetIncProbability()	-> Tomamos las que necesitemos
	ACTIVIDAD	A calcular aquí
	*/
void calculaActividadRadionuclidos()
	{
	printf("CALCULA ACTIVIDAD RADIONÚCLIDOS \n");
	int MAXMAT=Datos.matricesActividad->numElements();
	
	//1) Construimos la matriz de probabilidades.
	//construyeMatrizX();
	//construyeMatrizPN();	//Aquí estarán  al final las construcciones de estas
							//matrices, pero de momento se construyen en construyeListaCorreccion y
							//en pulsaOKFileEficiencia()
	//construyeVectorNE();
	//	Tendremos que resolver una ecuación por cada matriz de actividades, que
	//	nos dará las actividades relacionadas para cada radionúclido de esa matriz.
	//	Para cada una, tomamos 
	//	Cada fila serán las probabilidades correspondientes a un radionúclido
	
	//2) Creamos una lista de radionúclidos relacionados a partir de la matriz de 
	//	relaciones matrizX
	bool comprobado[numRN];
	LinkedList *rnRelacionados=new LinkedList();
		
	for(int i=0;i<numRN;i++)	comprobado[i]=false;
	for(int i=0;i<numRN;i++)//Recorremos los radionúclidos
		{
		if(!comprobado[i])
			{
			LinkedList *lista=new LinkedList();
			lista->addLast((void *)i);
			comprobado[i]=true;//Marcamos como comprobado
		
			for(int j=i+1;j<numRN;j++)//Miramos si alguno de los siguientes tiene relación
				{
				if(!comprobado[j])
					{
					for(int l=0;l<lista->numElements();l++)
						{
						int rad=(int)lista->getNode(l);
						for(int k=0;k<numPicos;k++)
							{
							if(matrizX[k][rad] && matrizX[k][j])//Si ambos están a true
								{
								//printf("(%d,%d) es %d y (%d,%d) es %d\n", k,i,matrizX[k][i],k,j,matrizX[k][j]);
								comprobado[j]=true;
								lista->addLast((void *)j);
								j=0;//Tenemos que rechequear todos!
								}
							}
						}
					}
				}
			rnRelacionados->addLast(lista);
			}
		}
		
	printf("Lista de radionúclidos relacionados:\n");
	for(int i=0;i<rnRelacionados->numElements();i++)
		{
		LinkedList *l=(LinkedList *)rnRelacionados->getNode(i);
		ordenaLista(l);
		for(int j=0;j<l->numElements();j++)
			{
			printf("%d ", (int)l->getNode(j));
			}
		printf("\n");
		}
	
	//2.1) Almacenamos los picos correspondientes a cada uno de los grupos de radionúclidos
	LinkedList *pRelacionados=new LinkedList();
	LinkedList *pLista=new LinkedList(); //lista de picos con rn comunes
	bool picoAgnadido[numPicos];
	for(int i=0;i<numPicos;i++)	picoAgnadido[i]=false;
		
	for(int i=0;i<rnRelacionados->numElements();i++)
		{
		LinkedList *listaPic=new LinkedList();
		LinkedList *listaRad=(LinkedList *)rnRelacionados->getNode(i);
			
		for(int j=0;j<listaRad->numElements();j++)
			{
			int rad=(int)listaRad->getNode(j);
			for(int k=0;k<numPicos;k++)
				{
				if(matrizX[k][rad] && !picoAgnadido[k])	
					{
					listaPic->addLast((void *)k);
					picoAgnadido[k]=true;
					}
				}
			}
		pLista->addLast(listaPic);			
		}		
		
	printf("Lista de picos relacionados:\n");
	for(int i=0;i<pLista->numElements();i++)
		{
		LinkedList *l=(LinkedList *)pLista->getNode(i);
		ordenaLista(l);
		for(int j=0;j<l->numElements();j++)
			{
			printf("%d ", (int)l->getNode(j));
			}
		printf("\n");
		}
	
	//2.2)
	// Para cada grupo asociado de radionúclidos, construimos su matriz PN a partir
	// de la matriz PN general
	for(int i=0;i<rnRelacionados->numElements();i++)
		{
		LinkedList *lista=(LinkedList *)rnRelacionados->getNode(i);
		LinkedList *listaPic=(LinkedList *)pLista->getNode(i);
		int n=lista->numElements();
		int p=getNumPicosRad(lista);//Devuelve el número de picos que tienen relación con los
									//radionúclidos existentes en lista
		int contP=0;//Contador de picos que llevamos.
		double mpn[p][n];
		double ne[p], sne[p];
		for(int j=0;j<p;j++)	
			{
			ne[j]=sne[j]=0.0;
			for(int k=0;k<n;k++)	mpn[j][k]=0.0;
			}
		
		printf("La matriz PN %d tiene dimensiones %dx%d:\n", i, p, n);
		for(int j=0;j<n;j++)	//Para cada radionúclido (recorremos por columnas)
			for(int k=0;k<p;k++)
				mpn[k][j]=matrizPN[(int)listaPic->getNode(k)][(int)lista->getNode(j)];
		
		for(int j=0;j<p;j++)	//Para cada pico
			{
			for(int k=0;k<n;k++)	//Para cada rad.
				printf("%.2f ", mpn[j][k]);
			printf("\n");
			}
			
		printf("El vector N/E %d tiene dim %d\n", i, p);
		for(int k=0;k<p;k++)	
			{
			ne[k]=vectorNE[(int)listaPic->getNode(k)];
			sne[k]=vectorIncNE[(int)listaPic->getNode(k)];
			printf("%.2f(%.2f) ", ne[k], sne[k]);
			}
		printf("\n");
	
		//3) Con esta matriz y vector hechos, empezamos a calcular actividades:
		//Ajuste a partir de mpn y ne
		}		
	}

//************** GET NUM PICOS RAD *******************
/*	Devuelve el número de picos para los que alguno de los
	redionúclidos de lista contiene emisiones
	*/
int getNumPicosRad(LinkedList *lista)
	{
	int numPicosRad=0;
	bool picoAgnadido[numPicos];
	for(int i=0;i<numPicos;i++)	picoAgnadido[i]=false;
		
	for(int i=0;i<lista->numElements();i++)
		{
		int r=(int)lista->getNode(i);
		for(int j=0;j<numPicos;j++)
			{
			if(matrizX[j][r] && !picoAgnadido[j])	
				{
				numPicosRad++;
				picoAgnadido[j]=true;
				}
			}
		}
	return numPicosRad;		
	}

//********************** ACTIVIDAD *******************************
/*	Devuelve el valor de actividad para los parámetros que le indiquemos
	*/
double actividad(double N, double p, double ef, double tv, double csc)
	{
	printf("Actividad del pico %f=%f/%f(p)*%f(ef)*%f(tv)*%f(csc)\n", N/(p*tv*ef*csc),N,p,ef,tv,csc);
	return 	N/(p*tv*ef*csc);
	}
double sigmaActividad(double a, double n, double sn, double p, double sp, double e, double se, double c, double sc)
	{
	return a*sqrt(pow(sn/n,2)+pow(se/e,2)+pow(sp/p,2)+pow(sc/c,2));
	}
	
//************** ACTIVIDAD SIN TIEMPO **********************
/*	Calcula la actividad sin tener en cuenta tiempso de desintegración
	*//*
double actividadSinTiempo(NodoPico *nodo, float &sigma)
	{
	if(nodo->Ajuste.identificado && !nodo->Ajuste.calibrado)
		{//NOTA: Sólo calculamos la actividad para los picos identificados
		double n= nodo->Ajuste.area;
		double e= nodo->Ajuste.eficiencia;
		double sn=nodo->Ajuste.incerAreaSinFondo;
		double se=nodo->Ajuste.sigmaEficiencia;
		//Tomamos el factor de corrección de la eficiencia
		double c= nodo->Ajuste.correccion;
		double sc=nodo->Ajuste.sigmaCorreccion;
		if(c==0)	{printf("WARNING: Calculando actividad sin suma en coincidencia\n"); c=1;}
		if(e==0)	{printf("WARNING: Calculando actividad sin valor de eficiencia\n");e=1;}//Por si acaso se calcula sin las eficiencias.
		int ce;
		NodoEmision *ne;
		double p,sp;
		sp=0; p=0; ne=NULL;		
		for(int k=0;k<nodo->Ajuste.emisiones->numElements();k++)
			{
			ne=(NodoEmision *)nodo->Ajuste.emisiones->getNode(k);
			ce=ne->codElemento;
			int emisor=LaraBD.GetEmissor(ce);
			p+=LaraBD.GetProbability(ce)/100.0;
			sp+=LaraBD.GetIncProbability(ce)/100.0;
			}
		
		printf("Área         = %f(%f)\n", n,sn);
		printf("Probabilidad = %f(%f)\n", p,sp);
		printf("Eficiencia	 = %f(%f)\n", e,se);
		printf("Csc			 = %f(%f)\n", c, sc);
		if(n==0)	nodo->Ajuste.actividad=nodo->Ajuste.sigmaActividad=0;
		else
			{
			nodo->Ajuste.actividad		= n/(e*p*c);
			nodo->Ajuste.sigmaActividad	= nodo->Ajuste.actividad*sqrt(pow(sn/n,2)+pow(se/e,2)+pow(sp/p,2)+pow(sc/c,2));
			}
		printf("Actividad del pico %f=%f/%f*%f*%f=%f\n", Datos.DaEnergia(nodo->Ajuste.Centroide),n,e,p,c, nodo->Ajuste.actividad);
		sigma=nodo->Ajuste.sigmaActividad;
		return nodo->Ajuste.actividad;	
		}
	sigma=0;
	return -1;
	}
	*/
//********************** ACTIVIDAD *******************************
/*	Devuelve el valor de actividad según el pico indicado
	*//*
double actividad(NodoPico *nodo, float &sigma)
	{
	printf("ACTIVIDAD\n");
	if(nodo->Ajuste.identificado && !nodo->Ajuste.calibrado)
		{//NOTA: Sólo calculamos la actividad para los picos identificados
		double n= nodo->Ajuste.area;
		double e= nodo->Ajuste.eficiencia;
		double sn=nodo->Ajuste.incerAreaSinFondo;
		double se=nodo->Ajuste.sigmaEficiencia;
		//Tomamos el factor de corrección de la eficiencia
		double c= nodo->Ajuste.correccion;
		double sc=nodo->Ajuste.sigmaCorreccion;
		if(c==0)	{printf("WARNING: Calculando actividad sin suma en coincidencia\n"); c=1;}
		if(e==0)	{printf("WARNING: Calculando actividad sin valor de eficiencia\n");e=1;}//Por si acaso se calcula sin las eficiencias.
		//Tomamos el periodo para ver qué tenemos que tomar como tiempo
		//
		int ce;
		NodoEmision *ne;
		double tmedio,tvivo, stmedio;
		double p,sp;
		tmedio=tvivo=stmedio=p=sp=0.0;
				
		//-------------------------------------------
		//NOTA: Sustitución por lo comentado abajo, para contar todas las emisiones
		tvivo=(double)Datos.tiempo;
		double ct=0.0;
		double t =0.0;
		for(int k=0;k<nodo->Ajuste.emisiones->numElements();k++)
			{
			ne=(NodoEmision *)nodo->Ajuste.emisiones->getNode(k);
			ce=ne->codElemento;
			int emisor=LaraBD.GetEmissor(ce);
			tmedio +=LaraBD.GetDecayTime(emisor);
			stmedio+=LaraBD.GetIncDecayTime(emisor);//No sé si las incertidumbres se pueden sumar así como así
			//char nom[3];
			//LaraBD.GetNameElement(emisor, nom);
			p+=LaraBD.GetProbability(ce)/100.0;
			sp+=LaraBD.GetIncProbability(ce)/100.0;
			}
		if(tmedio < 350*tvivo)
			{//En este caso hacemos una corrección del tiempo
			double landa=log(2.0)/tmedio;
			ct=(1-exp(-landa*tvivo))/landa;
//					printf("ct=1-e^(%f*%f)/%f=%f\n", -landa, tvivo, landa, ct); 
			}
		if(ct!=0)	t=ct;
		else		t=tvivo;

		printf("Área         = %f(%f)\n", n,sn);
		printf("Probabilidad = %f(%f)\n", p,sp);
		printf("Tiempo       = %f\n", t);
		printf("Eficiencia	 = %f(%f)\n", e,se);
		printf("Csc			 = %f(%f)\n", c, sc);
		if(n==0)	nodo->Ajuste.actividad=nodo->Ajuste.sigmaActividad=0;
		else
			{
			nodo->Ajuste.actividad		= n/(e*t*p*c);
			//nodo->Ajuste.sigmaActividad	= nodo->Ajuste.actividad*sqrt(pow(sn/n,2)+pow(se/e,2)+pow(sp/p,2)+pow(sc/c,2));
			nodo->Ajuste.sigmaActividad	= nodo->Ajuste.actividad*sqrt(pow(sn/n,2)+pow(se/e,2)+pow(sp/p,2)+pow(sc/c,2));
			}
		printf("Actividad del pico %f=%f/%f*%f*%f*%f=%f\n", Datos.DaEnergia(nodo->Ajuste.Centroide),n,e,t,p,c, nodo->Ajuste.actividad);
		sigma=nodo->Ajuste.sigmaActividad;
		return nodo->Ajuste.actividad;	
		}
	sigma=0;
	return -1;
	}
	*/
	
	
//********************** CALCULA ACTIVIDAD ***********************
/* Calculamos la actividad de cada pico en función a la fórmula conocida:
					N
	A(e)= _____________________
		     Pe*Tv*Csc*Ef(e)

	t es Tv o c(t)						1-e^(-landa*Tv)
	t es c(t) si T1/2<350Tv -> c(t)= --------------------
										    landa
	
	donde landa=(ln2)/(T1/2)
	
	Pe es la probabilidad de emisión.
	
	NOTA: De momento sólo haremos N/E por falta del resto de parámetros
	NOTA: Sólo tenemos eficiencias para algunos valores (los picos ajustados)
	
	bool reconstruir	A true, se reconstruye la lista de actividad
	*/
void calculaActividad()
	{
	printf("CALCULA ACTIVIDAD\n");
	//Recalculamos áreas por si se ha ajustado alguna después de cargar fondos
	Datos.restaAreaFondo();

	for(int i=1;i<=Datos.listaPicos.numElementos;i++)
		{
		NodoPicos *nodoPicos=DevuelveNodo(Datos.listaPicos, i);
		for(int j=1;j<=nodoPicos->picos.numElementos;j++)
			calculaActividad(i,j);
		}
		
//	imprimeActividad();	//Es un poco rollo que se te abra siempre esta ventana, 
						//y no da información nueva, lo único que hace es ordenar 
						//por energía, que no lo necesitamos
	printf("Las desviaciones de las actividades están en [%f,%f]\n", minDev, maxDev);
	printf("Las desviaciones de las areas están en [%f,%f]\n", minDevArea, maxDevArea);
	EstadoTrabajo.AnalisisActividad=true;
	printf("Fin de calcula actividad\n");
	reconstruyeListaActividad(); //Para que nos saque las actividades en la lista
	}
	
//***************** CALCULA ACTIVIDAD ********************
/*	Calcula la actividad de las emisiones asociadas al pico
	j del intervalo i.
	*/
void calculaActividad(int i, int j)	{
	//if (actividades != NULL){
	
	NodoPicos *nodoPicos=DevuelveNodo(Datos.listaPicos, i);
	NodoPico *nodo = DevuelveNodo(nodoPicos->picos, j);
	if(nodo->Ajuste.identificado && !nodo->Ajuste.calibrado && !nodo->Ajuste.invisible)
		{
		double cent=Datos.DaEnergia(nodo->Ajuste.Centroide);
		double n= nodo->Ajuste.areaSinFondo;
		double sn=nodo->Ajuste.incerAreaSinFondo;
		
		nodo->Ajuste.eficiencia= Datos.daEficiencia(cent);
		double e= nodo->Ajuste.eficiencia;
		nodo->Ajuste.sigmaEficiencia=Datos.daSigmaEficiencia(cent);
		double se=nodo->Ajuste.sigmaEficiencia;
				
		//Tomamos el factor de corrección de la eficiencia
		if(nodo->Ajuste.correccion==0)	
			{
			nodo->Ajuste.correccion=1;
			nodo->Ajuste.sigmaCorreccion=0;
			}
		double c= nodo->Ajuste.correccion;
		double sc=nodo->Ajuste.sigmaCorreccion;
		
		//Tomamos el periodo para ver qué tenemos que tomar como tiempo
		//
		int ce;
		NodoEmision *ne;
		double tmedio,tvivo, stmedio;
		double p,sp;
		tmedio=tvivo=stmedio=p=sp=0.0;
				
		tvivo=(double)Datos.tiempo;
		double ct=0.0;
		double t =0.0;
		int cep=-1;//Código de la emisión del padre
		double tmedioPadre=0.0;
					
		for(int k=0;k<nodo->Ajuste.emisiones->numElements();k++)
			{
			ne=(NodoEmision *)nodo->Ajuste.emisiones->getNode(k);
			ce=ne->codElemento;
			if(ce!=-1)
				{	
				int emisor=LaraBD.GetEmissor(ce);
				if(enCadena(emisor) && padre(emisor)!=-1)	
					{
					tmedioPadre+=LaraBD.GetDecayTime(padre(emisor));
					}
							
				//printf("Equilibrio con el padre es %d\n", enEquilibrio(emisor));
				if(enCadena(emisor)!=-1 && padre(emisor)!=-1 && enEquilibrio(emisor))
					{
				//	printf("Tomamos el tiempo del padre, que es %d\n", padre(emisor));
					tmedio+=LaraBD.GetDecayTime(padre(emisor));
					stmedio+=pow(LaraBD.GetIncDecayTime(padre(emisor)),2);//No sé si las incertidumbres se pueden sumar así como así
					}
				else		
					{
					tmedio +=LaraBD.GetDecayTime(emisor);
					stmedio+=pow(LaraBD.GetIncDecayTime(emisor),2);//No sé si las incertidumbres se pueden sumar así como así
					}
							
				//Hay que tomar la prob. del padre en el caso de que sean distintas
				//(Tomamos siempre la del padre y así nos curamos en salud
				if(padre(emisor)!=-1)
					{
				//	printf("El padre es %d,%d\n", LaraBD.GetAtomicNumber(padre(emisor)), LaraBD.GetMassNumber(padre(emisor)));
					//Ahora buscamos la emisión del padre para esa energía
					cSearchLaraBD foundsPater=LaraBD.SearchEmissionsByCode(ne->Energia-0.01, ne->Energia+0.01, -1.0,-1.0,padre(emisor));
					if(foundsPater.GetNumItemsFound()>0)
						{
						cep=foundsPater.GetFirstResult();
						p+=LaraBD.GetProbability(cep)/100;
						sp+=pow(LaraBD.GetIncProbability(cep)/100,2);
					//	printf("La probabilidad del padre es %.2f (%.2f)\n", p, sp);
						}
					else
						{
						p+=LaraBD.GetProbability(ce)/100;
						sp+=pow(LaraBD.GetIncProbability(ce)/100,2);
						}
					}
				else
					{
					p+=LaraBD.GetProbability(ce)/100;
					sp+=pow(LaraBD.GetIncProbability(ce)/100,2);
					}
				}
//felipe
			else
					//float p=leerNumeroEntreParentesis(fichero,i,1);
				{			
				string cad(BDIdentificaciones);				p = determinarProbabilidad(cent,(char *)cad.c_str())/100;
				sp = 0.0;
				}
			}
//fin felipe					
		if(tmedio < 350*tvivo)
			{//En este caso hacemos una corrección del tiempo
			//La corrección por tiempo debería hacerse teniendo en cuenta
			//un tiempo de desintegración que no sería ni el del padre ni
			//el del hijo, ya que los dos contribuyen. Para simplificar,
			//tomamos no obstante el del padre como aproximación
			
			double landa=log(2.0)/tmedioPadre;
			printf("landa=%.2f\ttmedioPadre=%.2f\ttmedio=%.2f\n", log(2.0), tmedioPadre, tmedio);
			ct=(1-exp(-landa*tvivo))/landa;
			printf("ct=1-e^(-%e*%e)/%e=%e\n", landa, tvivo, landa, ct); 
			}
		if(ct!=0)	t=ct;
		else		t=tvivo;
		printf("tmedio=%f < 350*tvivo(%f)=%f\n", tmedio, tvivo, 350*tvivo);
		printf("ct=%f, t=%f\n", ct, t);
				
		sp=sqrt(sp);
		stmedio=sqrt(tmedio);//NOTA: NO estamos contando el tiempo para las incertidumbres nunca! -> debería contarse si se usa el tmedio
				
		printf("Área         = %f(%f)\n", n,sn);
		printf("Probabilidad = %f(%f)\n", p,sp);
		printf("Tiempo       = %f\n", t);
		printf("Eficiencia	 = %f(%f)\n", e,se);
		printf("Csc			 = %f(%f)\n", c, sc);
		if(n==0)	nodo->Ajuste.actividad=nodo->Ajuste.sigmaActividad=0;
		else
			{
			nodo->Ajuste.actividad		= n/(e*t*p*c);
			nodo->Ajuste.sigmaActividad	= nodo->Ajuste.actividad*sqrt(pow(sn/n,2)+pow(se/e,2)+pow(sp/p,2)+pow(sc/c,2));
			//NSA-IEEE
			float dev=100*(nodo->Ajuste.sigmaActividad/nodo->Ajuste.actividad);
			float devArea=100*(sn/n);
			if(dev>maxDev)	maxDev=dev;
			if(dev<minDev)	minDev=dev;
			if(devArea>maxDevArea)	
				{
				printf("Nuevo máximo para el área: %f/%f\n\n\n\n\n\n", sn,n);
				printf("Sigma de la actividad:\n");
				printf("%f*sqrt((%f/%f)²+(%f/%f)²+(%f/%f)²+(%f/%f)²)\n", nodo->Ajuste.actividad, sn,n,se,e,sp,p,sc,c);
				maxDevArea=devArea;
				}
			if(devArea<minDevArea)	
				{
				minDevArea=devArea;
				}
			//
			}
		printf("Actividad del pico %f=%f/%f*%f*%f*%f=%f(%f)\n", Datos.DaEnergia(nodo->Ajuste.Centroide),n,e,t,p,c, nodo->Ajuste.actividad, nodo->Ajuste.sigmaActividad);
		}
	EstadoTrabajo.AnalisisActividad=true;
	printf("Fin de calcula actividad\n");
	//}
	}
	
//******************* EN EQUILIBRIO ******************
/*	Devuelve true si el hijo está en equilibrio con su padre superior
	En caso contrario devuelve false.
	Para determinarlo, usa los valores calculados en determinarRestricciones de
	VisAnal.C
	*/
bool enEquilibrio(int elemento)
	{
//felipe. violacion de segmento EN EQUILIBRIO??	
if (actividades != NULL){
	printf("EN EQUILIBRIO %d?\n", elemento);
	int x=familia(elemento);
	printf("EN EQUILIBRIO 2 %d?\n", elemento);
	int y=rango(elemento);
	printf("EN EQUILIBRIO 3 %d?\n", elemento);
	int p=padre(elemento);
	printf("EN EQUILIBRIO 4 %d?\n", elemento);
	int xp=familia(p);
	printf("EN EQUILIBRIO 5 %d?\n", elemento);
	int yp=rango(p);
	printf("EN EQUILIBRIO 6 %d?\n", elemento);
	printf("comparando %d,%d \n",  xp,yp);
	printf("comparando %d,%d \n",  x,y);
	printf("comparando (%f)\n",  actividades[xp][yp]);
	printf("comparando (%f)\n",  actividades[x][y]);
	printf("comparando %d,%d (%f) con %d,%d (%f)\n", x,y,actividades[x][y], xp,yp,actividades[xp][yp]);
	if(actividades[x][y]==actividades[xp][yp])
			return true;		
	else	return false;
}
}


//******************* FAMILIA ************************
/*	Devuelve el número de la familia de radionúclidos a la que pertenece
	el elemento con ese código:
		-1: No pertenece a ninguna familia de radionúclidos naturales
		0: Pertenece a la familia del Th232
		1: Pertenece a la familia del U235
		2: Pertenece a la familia del U238
	*/
int familia(int codigo)
	{
	int a=LaraBD.GetAtomicNumber(codigo);
	int m=LaraBD.GetMassNumber(codigo);

	for(int i=0;i<numCadenas;i++)
		{
	//	printf("CADENA DE DESINTEGRACIÓN %d\n", i);
		int minEl=cadenas[i*2];
		int maxEl=cadenas[i*2+1];
	//	printf("Tenemos un array de actividades de %d\n", int((maxEl-minEl)/3));
		for(int j=minEl;j<maxEl;j+=3)
			{
	//		printf("Comparando %d,%d con %d,%d\n", m,a, naturales[j], naturales[j+1]);
			if(naturales[j]==m && naturales[j+1]==a)	
				{
	//			printf("El %d,%d está en cadena\n", a,m);
				return i;
				}
			}
		}
	return -1;
	}
	
//******************* RANGO ************************
/*	Devuelve la posición dentro de una familia de radionúclidos a la que pertenece
	el elemento con ese código:
	*/
int rango(int codigo)
	{
	int a=LaraBD.GetAtomicNumber(codigo);
	int m=LaraBD.GetMassNumber(codigo);

	for(int i=0;i<numCadenas;i++)
		{
	//	printf("CADENA DE DESINTEGRACIÓN %d\n", i);
		int minEl=cadenas[i*2];
		int maxEl=cadenas[i*2+1];
	//	printf("Tenemos un array de actividades de %d\n", int((maxEl-minEl)/3));
		int k=0;
		for(int j=minEl;j<maxEl;j+=3)
			{
	//		printf("Comparando %d,%d con %d,%d\n", m,a, naturales[j], naturales[j+1]);
			if(naturales[j]==m && naturales[j+1]==a)	
				{
	//			printf("El %d,%d está en cadena\n", a,m);
				return k;
				}
			k++;
			}
		}
	return -1;
	}

//******************* EN CADENA **********************
/*	Devuelve la posición de un determinado elemento en la cadena de naturales,
	o -1 si no está
	*/
int enCadena(int codigo)
	{
	int a=LaraBD.GetAtomicNumber(codigo);
	int m=LaraBD.GetMassNumber(codigo);

	for(int i=0;i<numCadenas;i++)
		{
	//	printf("CADENA DE DESINTEGRACIÓN %d\n", i);
		int minEl=cadenas[i*2];
		int maxEl=cadenas[i*2+1];
	//	printf("Tenemos un array de actividades de %d\n", int((maxEl-minEl)/3));
		for(int j=minEl;j<maxEl;j+=3)
			{
	//		printf("Comparando %d,%d con %d,%d\n", m,a, naturales[j], naturales[j+1]);
			if(naturales[j]==m && naturales[j+1]==a)	
				{
	//			printf("El %d,%d está en cadena\n", a,m);
				return j;
				}
			}
		}
	return -1;
	}
	
//****************** PADRE ***********************
/*	Devuelve el código del padre inmediatamente superior al elemento identificado
	por código (considerando padres sólo a Th232, Th230, U234, U235, U238, Ra226)
	*/
int padre(int codigo)
	{
	int pos=enCadena(codigo);
	if(pos<0)	
		{
		printf("ERROR: Código %d (%d,%d) no está en la cadena\n", codigo, LaraBD.GetAtomicNumber(codigo), LaraBD.GetMassNumber(codigo) );
		return -1;
		}
		
	int ap=LaraBD.GetAtomicNumber(codigo);
	int mp=LaraBD.GetMassNumber(codigo);
//	printf("Buscando el padre de %d,%d\n", ap,mp);
	cSearchLaraBD founds;	 //Elementos encontrados naturales		
	for(int i=pos;i>0;i-=3)
		{
	//	printf("i+1 es %d e i es %d\n", naturales[i+1], naturales[i]);
		int a=naturales[i+1];
		int m=naturales[i];
	//	printf("Buscando para %d,%d\n", a,m);
		if( (a==92 && (m==234 || m==235 || m==238)) || (a==90 && (m==230 || m==232))
			|| (a==88 && m==226) )
			{
			founds=LaraBD.SearchElements(naturales[i+1], naturales[i+1], naturales[i], 
					  naturales[i], false, false, true, true, NULL);	
//			printf("PADRE: hemos encontrado %d\n", 	founds.GetNumItemsFound());			
			if(founds.GetNumItemsFound()!=0)	
				{
//				printf("Padre es %d,%d\n", naturales[i+1], naturales[i]);				
				return founds.GetFirstResult();
				}
			else
				{
				printf("ERROR: padre %d,%d no se encuentra en Lara\n", a,m);
				return -1;
				}
			}
		}
	printf("ERROR: Elemento no tiene padre en cadena de naturales\n");
	return -1;
	}

	
//********************* HIJOS **********************
/*	Si elemento es 234U, 230Th, 232Th, 235U, 238U o 226Ra, devuelve una lista
	con los códigos de todos sus hijos. En caso contrario devuelve NULL
	*/
int hijos(int elemento, int *desc)
	{
	//printf("------------------HIJOS de %d,%d------------------------\n", LaraBD.GetAtomicNumber(elemento), LaraBD.GetMassNumber(elemento) );
	int descendientes[numNaturales];
	int numDesc;
	int cont=0;
	if(esPadre(elemento))
		{
		int pos=enCadena(elemento);
		if(pos<0)	{printf("ERROR: El elemento no está en ninguna cadena\n"); return -1;}
		cSearchLaraBD founds;	 //Elementos encontrados naturales		
		for(int i=pos+3;i<numNaturales*3;i+=3)
			{
			int a=LaraBD.GetAtomicNumber(naturales[i+1]);
			int m=LaraBD.GetMassNumber(naturales[i]);
			founds=LaraBD.SearchElements(naturales[i+1], naturales[i+1], naturales[i], 
					  naturales[i], false, false, true, true, NULL);					
			if(founds.GetNumItemsFound()!=0)	
				{
				int cod=founds.GetFirstResult();
				if(!esPadre(cod))	
					{
					descendientes[cont++]=cod;
					}
				else				
					{
					i=numNaturales;
					}
				}
			}
			
//		desc=new int[cont];
		for(int i=0;i<cont;i++)
			desc[i]=descendientes[i];
//		printf("El elemento %d,%d tiene %d hijos\n", LaraBD.GetMassNumber(elemento), LaraBD.GetAtomicNumber(elemento), cont);
		return cont;
		}
	else
		{
//		printf("ERROR: Elemento no tiene padre en cadena de naturales\n");
		return -1;
		}
	}

//**************** PADRE ***************
/*	Devuelve true si el código del elemento es un descendiente natural del
	elemento de número atómico apadre y número másico mnpadre.
	*/
bool padre(int codigoHijo, int apadre, int mnpadre)
	{
	bool devolver=false;
	int ahijo=LaraBD.GetAtomicNumber(codigoHijo);
	int mnhijo=LaraBD.GetMassNumber(codigoHijo);
//	printf("COMPROBANDO SI %d,%d es hijo de %d,%d\n", ahijo, mnhijo, apadre, mnpadre);
	if(ahijo==apadre && mnhijo==mnpadre)
		{
//		printf("Es él mismo!\n");
		return true;
		}
				
	cSearchLaraBD founds=LaraBD.SearchElements(apadre,apadre,mnpadre,mnpadre);
	if(founds.GetNumItemsFound()>0)
		{
		int codPadre=founds.GetFirstResult();
		
		int *codHijos=new int[numNaturales];
		int numHijos=hijos(codPadre, codHijos);
		for(int i=0;i<numHijos;i++)
			if(codHijos[i]==codigoHijo)	
				{
//				printf("Es un hijo suyo\n");
				delete [] codHijos;
				return true;
				}
		delete [] codHijos;
		}
		
	return false;			
	}

//**************** ES PADRE ******************
/*	Devuelve true si elemento es 234,235,238U, 230,232Th o 226Ra
	*/
bool esPadre(int elemento)
	{
	int m=LaraBD.GetMassNumber(elemento);
	int a=LaraBD.GetAtomicNumber(elemento);
	if( (a==92 && (m==235 || m==234 || m==238)) || (a==88 && m==226) ||
		(a==90 && (m==230 || m==232)) )	return true;
	else								return false;
	}
	
//********************** CALCULA MDA ***********************
/* Calculamos la actividad de cada pico en función a la fórmula conocida:
					Ld
	A(e)= _____________________
		     Pe*Tv*Csc*Ef(e)

	t es Tv o c(t)						1-e^(-landa*Tv)
	t es c(t) si T1/2<350Tv -> c(t)= --------------------
										    landa
	
	donde landa=(ln2)/(T1/2)
	
	Pe es la probabilidad de emisión.
	
	//NOTA: Para los elementos naturales, debemos calcular el MDA dos veces,
	una con el tiempo de vida del padre y otra con la del hijo!!!!
	//Ahora mismo sólo aparece la del hijo
	*/
void calculaMDA()
	{
	printf("CALCULA MDA\n");
	for(int i=1;i<=Datos.listaPicos.numElementos;i++)
		{
		NodoPicos *nodoPicos=DevuelveNodo(Datos.listaPicos, i);
		for(int j=1;j<=nodoPicos->picos.numElementos;j++)
			{
			NodoPico *nodo = DevuelveNodo(nodoPicos->picos, j);
//			if(nodo->Ajuste.identificado && !nodo->Ajuste.calibrado)
			if(nodo->Ajuste.identificado && !nodo->Ajuste.calibrado && !nodo->Ajuste.invisible)
				{
				//double n= nodo->Ajuste.areaSinFondo;
				double cent=Datos.DaEnergia(nodo->Ajuste.Centroide);
				double lc=nodo->Ajuste.lc;
				double ld=nodo->Ajuste.ld;
				nodo->Ajuste.eficiencia=Datos.daEficiencia(cent);
				double e= nodo->Ajuste.eficiencia;
				double sn=nodo->Ajuste.incerAreaSinFondo;
				nodo->Ajuste.sigmaEficiencia=Datos.daSigmaEficiencia(cent);
				double se=nodo->Ajuste.sigmaEficiencia;
				//Tomamos el factor de corrección de la eficiencia
				if(nodo->Ajuste.correccion==0)	
					{
					nodo->Ajuste.correccion=1;
					nodo->Ajuste.sigmaCorreccion=0;
					}
				double c= nodo->Ajuste.correccion;
				double sc=nodo->Ajuste.sigmaCorreccion;
				
				printf("El pico tiene %d emisiones\n", nodo->Ajuste.emisiones->numElements());
			//	for(int k=0;k<nodo->Ajuste.emisiones->numElements();k++)
			  	if(nodo->Ajuste.emisiones->numElements()>0)
				  for(int k=0;k<1;k++)
					{
					printf("emisión %d\n", k);
					int ce;
					NodoEmision *ne;
					double p, sp, tmedio, stmedio;
					ne=(NodoEmision *)nodo->Ajuste.emisiones->getNode(k);
					ce=ne->codElemento;
					if(ce!=-1)
						{
						//El t medio es para emisiones individuales, así que o calculamos
						//MDA para emisiones en vez de picos o no sé.
						//La energía, que viene después, sí que se puede coger para el pico
						int emisor=LaraBD.GetEmissor(ce);
						tmedio =LaraBD.GetDecayTime(emisor);
						stmedio=LaraBD.GetIncDecayTime(emisor);
						double tvivo=(double)Datos.tiempo;
						double ct=0.0;
						double t =0.0;
						char nom[3];
						LaraBD.GetNameElement(emisor, nom);

						//En el caso de un elemento con padre natural, se calcula el 
						//MDA también para el padre (tiempo de vida del padre)
						//double energia=LaraBD.GetEnergy(nodo->Ajuste.codElemento);
						double energia=LaraBD.GetEnergy(ce);
						cSearchLaraBD founds = LaraBD.SearchEmissions(energia, energia,-1,-1,-1,88,88,226,226,false,false,true,true,"", true, true);
						founds = founds + LaraBD.SearchEmissions(energia, energia,-1,-1,-1,90,90,232,232,false,false,true,true,"", true, true);
						founds = founds + LaraBD.SearchEmissions(energia, energia,-1,-1,-1,92,92,235,235,false,false,true,true,"", true, true);
						founds = founds + LaraBD.SearchEmissions(energia, energia,-1,-1,-1,92,92,238,238,false,false,true,true,"", true, true);
						founds = founds + LaraBD.SearchEmissions(energia, energia,-1,-1,-1,92,92,234,234,false,false,true,true,"");
						founds = founds + LaraBD.SearchEmissions(energia, energia,-1,-1,-1,90,90,230,230,false,false,true,true,"");

						if(tmedio < 350*tvivo)
							{//En este caso hacemos una corrección del tiempo
							double landa=log(2.0)/tmedio;
							ct=(1-exp(-landa*tvivo))/landa;
							}
						if(ct!=0)	t=ct;
						else		t=tvivo;
						
						//Buscamos la probabilidad de emisión de la emisión asociada a este pico
						p=LaraBD.GetProbability(ce)/100.0;
						sp=LaraBD.GetIncProbability(ce);
											
						nodo->Ajuste.mda	 = ld/(e*t*p*c);
						printf("Ld           = %f\n", ld);
						printf("Lc           = %f\n", lc);
						printf("Probabilidad = %f(%f)\n", p,sp);
						printf("Tiempo       = %f\n", t);
						printf("Eficiencia	 = %f(%f)\n", e,se);
						printf("Csc			 = %f(%f)\n", c, sc);
						printf("MDA del pico=%f/%f*%f*%f*%f=%f\n", ld,e,t,p,c, nodo->Ajuste.mda);
					
						if(founds.GetNumItemsFound()==1) //Si tiene padre natural
							{//Si tiene padre natural calculamos también ese mda
							int emisionPadre=founds.GetFirstResult();
							int padre=LaraBD.GetEmissor(emisionPadre);
							printf("Tiene padre: %d,%d\n", LaraBD.GetAtomicNumber(padre), LaraBD.GetMassNumber(padre));
							double tmedioPadre =LaraBD.GetDecayTime(padre);
			//				double stmedioPadre=LaraBD.GetIncDecayTime(padre);
							double tPadre=0.0;
							ct=0;
							if(tmedioPadre < 350*tvivo)
								{//En este caso hacemos una corrección del tiempo
								double landa=log(2.0)/tmedioPadre;
								ct=(1-exp(-landa*tvivo))/landa;
								}
							if(ct!=0)	tPadre=ct;
							else		tPadre=tvivo;
								
							p=LaraBD.GetProbability(emisionPadre)/100;
							sp=pow(LaraBD.GetIncProbability(emisionPadre)/100,2);
							printf("La probabilidad del padre es %.2f (%.2f)\n", p, sp);
							
							//printf("Tiempo medio padre ajustado=%f\n", tPadre);
							nodo->Ajuste.mdaPadre=ld/(e*tPadre*p*c);	//NOTA: Antes como arriba, ld
							printf("MDA del pico padre=%f/%f*%f*%f*%f=%f\n", ld,e,tPadre,p,c, nodo->Ajuste.mdaPadre);
							}
						}
					}
				}
				//Tomamos el periodo para ver qué tenemos que tomar como tiempo
			}//For(picos)
		}//for(intervalos)
	imprimeMDA();
	EstadoTrabajo.AnalisisMDA=true;
	reconstruyeListaActividad(); //Para que nos saque las MDA en la lista
	}

//******************* IMPRIME ACTIVIDAD ********************	
/*	Crea un cuadro de diálogo en el que imprime las actividades con entradas:
	Peak of XXX keV has activity XXX (XXX)
	*/
void imprimeActividad()
	{
	Arg args[7];
	Cardinal i=0;
	Widget listaAct, col, frameAct, f1;
	double e, a, sa;
	char texto[200];
	XmString cad;
	XmString cadena;

	printf("MOSTRAMOS LOS DATOS OBTENIDOS PARA LA ACTIVIDAD\n");
	
	Cardinal k = 0;
	XtSetArg (args[k], XmNtitle, "Activities"); k++;
	XtSetArg (args[k], XmNmwmDecorations, MWM_DECOR_BORDER|MWM_DECOR_TITLE|MWM_DECOR_MENU); k++;
	XtSetArg (args[k], XmNmwmFunctions, MWM_FUNC_MOVE|MWM_FUNC_CLOSE); k++;
	XtSetArg (args[k], XmNbaseWidth, 400); k++;
	XtSetArg (args[k], XmNbaseHeight, 300); k++;
	XtSetArg (args[k], XmNmaxWidth, 400); k++;
	XtSetArg (args[k], XmNmaxHeight, 300); k++;
	
	dlgAct = XmCreateFormDialog(app_shell, "dlgAct", args, k);
	XtAddCallback (XtParent(dlgAct), XmNpopdownCallback, QuitaVentanaAct, NULL);
	
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
    listaAct = XmCreateScrolledList(f1, "lista de actividades", args, k);
	
	for(int i=1;i<=Datos.listaPicos.numElementos;i++)
		{
		NodoPicos *picos=DevuelveNodo(Datos.listaPicos, i);
		for(int j=1;j<=picos->picos.numElementos;j++)
			{
			NodoPico  *nodo      = DevuelveNodo(picos->picos, j);
//			if(nodo->Ajuste.identificado && !nodo->Ajuste.calibrado)
			if(nodo->Ajuste.identificado && !nodo->Ajuste.calibrado && !nodo->Ajuste.invisible)
				{
				e=Datos.DaEnergia(nodo->Ajuste.Centroide);
				a=nodo->Ajuste.actividad;
				sa=nodo->Ajuste.sigmaActividad;
				char presentacion[80];
				PresentaNumero(a, sa, presentacion);
				sprintf(texto, "Peak of %f keV has activity=%s", e, presentacion);
				
				cadena=XmStringCreateLocalized(texto);
				XmListAddItemUnselected(listaAct, cadena, 0); 
				XmStringFree(cadena);
				}	
			}
		}//del bucle for
	
	XtManageChild(listaAct);
	XtManageChild(col);
	XtManageChild(f1);
	XtManageChild(dlgAct);
	}

//******************* IMPRIME MDA ********************	
/*	Imprimimos la mínima actividad detectable para cada 
	pico
	*/
void imprimeMDA()
	{
	Arg args[7];
	Cardinal i=0;
	Widget listaAct, col, frameAct, f1;
	double e, mda, mdaPadre;
	char texto[200];
	XmString cad;
	XmString cadena;
	
	printf("IMPRIME MDA\n");
	
	Cardinal k = 0;
	XtSetArg (args[k], XmNtitle, "Minimum Detectable Activities"); k++;
	XtSetArg (args[k], XmNmwmDecorations, MWM_DECOR_BORDER|MWM_DECOR_TITLE|MWM_DECOR_MENU); k++;
	XtSetArg (args[k], XmNmwmFunctions, MWM_FUNC_MOVE|MWM_FUNC_CLOSE); k++;
	XtSetArg (args[k], XmNbaseWidth, 400); k++;
	XtSetArg (args[k], XmNbaseHeight, 300); k++;
	XtSetArg (args[k], XmNmaxWidth, 400); k++;
	XtSetArg (args[k], XmNmaxHeight, 300); k++;
	dlgMDA = XmCreateFormDialog(app_shell, "dlgMDA", args, k);
	XtAddCallback (XtParent(dlgMDA), XmNpopdownCallback, QuitaVentanaMDA, NULL);
	
	k=0;
	XtSetArg (args[k], XmNbaseWidth, 400); k++;
	XtSetArg (args[k], XmNbaseHeight, 300); k++;
	XtSetArg (args[k], XmNmaxWidth, 400); k++;
	XtSetArg (args[k], XmNmaxHeight, 300); k++;
	col=XmCreateRowColumn(dlgMDA, "ColumnaAc", args, k);
	
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
    listaAct = XmCreateScrolledList(f1, "lista de actividades", args, k);
	
	
	//------temporal: imprimimos a un ficherillo los datos
	string nombreFA("activ");
	string cad2(daFichero(Datos.nombre));
	cout << cad2 << endl;
	nombreFA+=cad2;
	cout << nombreFA << endl;
	char *pathFA;
	pathFA=DaNombreFile((char *)nombreFA.c_str(), daDirectorio(Datos.nombre));
	printf("Abriendo fichero %s\n", pathFA);
//	printf("Abriendo fichero %s\n", nombreFA);
	ofstream fichA(pathFA, ios_base::trunc);
	fichA << "ENERGÍA(keV)\tACTIVIDAD(Bq)\tMDA(Bq)" << endl;
	//--------------
	printf("Una vez construidos los widgets, empezamos a escribir la lista\n");

	for(int i=1;i<=Datos.listaPicos.numElementos;i++)
		{
		NodoPicos *picos=DevuelveNodo(Datos.listaPicos, i);
		for(int j=1;j<=picos->picos.numElementos;j++)
			{
			NodoPico  *nodo      = DevuelveNodo(picos->picos, j);
//			if(nodo->Ajuste.identificado && !nodo->Ajuste.calibrado)
			if(nodo->Ajuste.identificado && !nodo->Ajuste.calibrado && !nodo->Ajuste.invisible)
				{
				for(int k=0;k<nodo->Ajuste.emisiones->numElements();k++)
					{					
					e=Datos.DaEnergia(nodo->Ajuste.Centroide);
					mda=nodo->Ajuste.mda;
					mdaPadre=nodo->Ajuste.mdaPadre;
					char act[20];
					char mdar[20], mdapr[20];
					redondeaDouble(nodo->Ajuste.mda, 3, mdar);
					redondeaDouble(nodo->Ajuste.mdaPadre, 3, mdapr);
					PresentaNumero(nodo->Ajuste.actividad, nodo->Ajuste.sigmaActividad, act);
					NodoEmision *ne=(NodoEmision *)nodo->Ajuste.emisiones->getNode(k);
					double en=LaraBD.GetEnergy(ne->codElemento);
						
					if(nodo->Ajuste.mdaPadre!=0)
						{
						cout << en << "\t\t" << act << "\t" << mdar << "(" << mdapr << ")" << endl;
						fichA << en << "\t\t" << act << "\t" << mdar << "(" << mdapr << ")" << endl;
						}
					else
						{
						cout << en << "\t\t" << act << "\t" << mdar << endl;
						fichA << en << "\t\t" << act << "\t" << mdar << endl;
						}
					
					if(nodo->Ajuste.mdaPadre==0)
						{
						sprintf(texto, "Peak of %f keV has MDA=%f Bq", e, mda);
						cadena=XmStringCreateLocalized(texto);
						}
					else
						{
						sprintf(texto, "Peak of %f keV has MDA=%f Bq (%f eq.)", e, mda, mdaPadre);
						cadena=XmStringCreateLocalized(texto);
						}
					XmListAddItemUnselected(listaAct, cadena, 0); 
					XmStringFree(cadena);
					}//if(sólo tiene asociada una emision
				}	//if(identificado)
			}//for(picos del intervalo)
		}//for(intervalos del espectro)
	
	fichA.close();
	
	XtManageChild(listaAct);
	XtManageChild(col);
	XtManageChild(f1);
	XtManageChild(dlgMDA);
	}
	

//************* ESTÁ EMISIÓN MÁS PROBABLE ***************
/*	Realiza una búsqueda en lara y en nuestra lista de análisis para ver si la
	emisión más probable de lara para ese elemento la tenemos nosotros identificada
	*/
bool estaEmisionMasProbable(int elemento)
	{
	return estaEmisionMasProbable(elemento,-1,-1);
	}
	
//************* ESTÁ EMISIÓN MÁS PROBABLE ***************
/*	Realiza una búsqueda en lara y en nuestra lista de análisis para ver si la
	emisión más probable de lara para ese elemento la tenemos nosotros identificada
	Versión de la anterior que sólo toma la emisión más probable en un determinado
	rango de energías, que seguramente cuando se la llame será el rango en el que
	tenemos picos identificados
	*/
bool estaEmisionMasProbable(int elemento, float eInf, float eSup)
	{
	//buscamos la emisión más probable para el elemento
	int emiMaxProb=LaraBD.daEmisionConMayorProbabilidad(elemento, eInf, eSup);
	int emisorMax=LaraBD.GetEmissor(emiMaxProb);
	int mMax=LaraBD.GetMassNumber(emisorMax);
	int aMax=LaraBD.GetAtomicNumber(emisorMax);
	float e=LaraBD.GetEnergy(emiMaxProb);
	float prob=LaraBD.GetProbability(emiMaxProb);
	
	int codPadre=-1;
	int mMaxPadre, aMaxPadre, probPadre;
//	if(enCadena(elemento))	
	if(enCadena(elemento)!=-1)	
		{
		codPadre=padre(elemento);//Tenemos en cuenta que pueda estar identificada como emisión de su padre
		mMaxPadre=LaraBD.GetMassNumber(codPadre);
		aMaxPadre=LaraBD.GetAtomicNumber(codPadre);
		}
		
	int *codHijos=new int[numNaturales];
	int numHijos=hijos(elemento, codHijos);//Tenemos en cuenta que pueda estar identificada como emisión de algún hijo
	
	//printf("La emisión más probable para %s%d es %f, para la energía %f\n", name,m,prob, LaraBD.GetEnergy(emiMaxProb) );
	//Ya tenemos la emisión más probable, ahora vamos a ver si está en nuestra lista
	for(int i=1;i<=Datos.listaPicos.numElementos;i++)
		{
		NodoPicos *intervalo = DevuelveNodo(Datos.listaPicos, i);
		for(int j=1;j<=intervalo->picos.numElementos;j++)
			{
			NodoPico *pico = DevuelveNodo(intervalo->picos, j);
//			if(pico->Ajuste.identificado && ! pico->Ajuste.calibrado)//Si está identificado, pero sólo en la fase de análisis
			if(pico->Ajuste.identificado && !pico->Ajuste.calibrado && !pico->Ajuste.invisible)
				for(int k=0;k<pico->Ajuste.emisiones->numElements();k++)
					{//Tomamos la primera emisión y la introducimos en una lista de radionúclidos
					NodoEmision *ne=(NodoEmision *)pico->Ajuste.emisiones->getNode(k);
					int ce=ne->codElemento;
					if(ce!=-1)
						{
						int emisorAct=LaraBD.GetEmissor(ce);
						int mAct=LaraBD.GetMassNumber(emisorAct);
						int aAct=LaraBD.GetAtomicNumber(emisorAct);
						float eAct=LaraBD.GetEnergy(ce);
						float pAct=LaraBD.GetProbability(ce);
		
						if(mAct==mMax && aAct==aMax && eAct==e && pAct==prob)	
							{
							delete [] codHijos;
							return true;
							}
						if(codPadre>0 && eAct==e && aAct==aMaxPadre && mAct==mMaxPadre)	
							{
							delete [] codHijos;
							return true;//NOTA: Faltaría la probabilidad, pero no creo q sea necesaria
							}
						for(int l=0;l<numHijos;l++)
							{//procesar los hijos para no tenerlos en cuenta
							int mHijo=LaraBD.GetMassNumber(codHijos[l]);
							int aHijo=LaraBD.GetAtomicNumber(codHijos[l]);
							if(mAct==mHijo && aAct==aHijo && eAct==e)	
								{
								delete [] codHijos;
								return true;
								}
							}
						}
					}
			}
		}
		
	delete [] codHijos;
	return false;
	}
	
//************** BORRA VIS ACTIV ********************
/*	Elimina todas las variables globales que puedan haber quedado reservadas
	*/
void borraVisActiv()
	{
	printf("BORRA VIS ACTIV\n");
	printf("borramos nombreFichEfi\n");
	if(nombreFichEfi!=NULL)	delete [] nombreFichEfi;
	printf("borramos numSublistas\n");
	if(numSublistas!=NULL)	delete [] numSublistas;
	printf("borramos radionuclidos\n");
	
	if(radionuclidos!=NULL)	{radionuclidos->deleteAll();	/*delete radionuclidos;/*free(radionuclidos);*/}
	printf("borramos emisiones\n");
	if(emisiones!=NULL)		{emisiones->deleteAll();		/*delete emisiones;/*free(emisiones);*/}
	printf("borramos picosRN\n");
	if(picosRN!=NULL)
		{
		while(picosRN->numElements()>0)
			{
			LinkedList *temp=(LinkedList *)picosRN->getFirst();
			temp->deleteAll();
			picosRN->deleteFirst();
			}
		picosRN->deleteAll();
		//free(picosRN);
		//delete picosRN;
		}
	printf("borramos listaMCE\n");
	if(listaMCE!=NULL)	{listaMCE->deleteAll();	/*delete listaMCE;/*free(listaMCE);*/}
	
	printf("borramos vectorNE\n");
	if(vectorNE!=NULL)		delete [] vectorNE;	
	printf("borramos vectorIncNE\n");
	if(vectorIncNE!=NULL)	delete [] vectorIncNE;
	printf("borramos vectorCT\n");
	if(vectorCT!=NULL)		delete [] vectorCT;	
	printf("borramos vectorIncCT\n");
	if(vectorIncCT!=NULL)	delete [] vectorIncCT;
	return;
	}

//************** Felipe:determinarProbabilidad ********************
//lee la probabilidad de emisión de un fichero.
//Evita recuperar la probabilidad de Lara para elementos que no están allí.
//**************************************************************
float determinarProbabilidad(float energia, char *fichero)
	{
	printf("DETERMINAR PROBABILIDAD ALREDEDOR DE %.2f con %s\n", energia, fichero);	
	if(energia<=0)	{printf("ERROR: Energía no válida (cero)\n");return 0.0;}
		
//--->	preparaFichero(fichero);
	
	int filas=numeroFilas(fichero);
	int j=0;
	char *nom=NULL;
	char *tex=NULL;
	double elAnt=0;//Para volver atrás si el siguiente está más lejos que éste
	bool anteriorEsCero=true;
	NodoEmision *ne=new NodoEmision();
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
			
			float prob=leerNumeroEntreParentesis(fichero,i,1);
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
			//printf("Buscando en LARA [%.2f,%.2f] con probabilidad [%.2f, %.2f] y número atómico %d y nombre |%s|\n", el-0.1, el+0.1, minProb, prob+0.1, p, nom);

			//cSearchLaraBD founds = LaraBD.SearchEmissions(el-0.1, el+0.1,minProb,prob+0.1,-1,-1,-1,p,p,false,false,false,false,nom, false, false);
			//printf("Hemos encontrado %d elementos\n", founds.GetNumItemsFound() ); 
//			if(founds.GetNumItemsFound())
//NOTA: Añadimos una pequeña restriccion para que no nos identifique con picos muy lejanos!
		if(energia>el-2.0 && energia<el+2.0) //felipe. Cambiar intervalo de 0.5 a 1.0
			{
			ne->Energia=el;
			ne->nombre=new char[strlen(nom)+1];
			printf("1Tamaño del nombre es %d (%s)\n", strlen(nom), nom);
			strcpy(ne->nombre, nom);
			printf("1El nombre que se asocia es %s\n", ne->nombre);
			ne->probabilidad=leerNumeroEntreParentesis(fichero,i,1);
			printf("1felipe:La probabilidad que se asocia es %f\n", ne->probabilidad);
				printf("1felipe:La energia que se asocia es %f\n", ne->Energia);
				ne->codElemento=-1;
			//Sólo añadimos si está relativamente cerca
			//if(ne->Energia - energia < 1.0)		emisiones->addLast(ne);
			
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
					ne->nombre=new char[strlen(nom)+1];
					printf("Tamaño del nombre es %d (%s)\n", strlen(nom), nom);
					strcpy(ne->nombre, nom);
					printf("2El nombre que se asocia es |%s|\n", ne->nombre);
					ne->probabilidad=leerNumeroEntreParentesis(fichero,k,1);
					printf("Hemos leido el número entre paréntesis\n");
					ne->codElemento=-1;
					//Sólo añadimos si está relativamente cerca
					/*if(ne->Energia - energia < 1.0)
						{
						printf("Añadimos emisión\n");
						emisiones->addLast(ne);
						printf("Volvemos de añadir emisión\n");
						}
*/
					printf("2El nombre que se asocia es |%s|\n", ne->nombre);
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
					ne->Energia=el;
					ne->nombre=new char[strlen(nom)+1];
					printf("3 Tamaño del nombre es %d (%s)\n", strlen(nom), nom);
					strcpy(ne->nombre, nom);
					printf("3 El nombre que se asocia es %s\n", ne->nombre);
					ne->probabilidad=leerNumeroEntreParentesis(fichero,k,1);
					ne->codElemento=-1;
					//Sólo añadimos si está relativamente cerca
					//if(ne->Energia - energia < 1.0)	emisiones->addFirst(ne);
					}
				else	k=0;
				}
			printf("fin de determinarProbabilidad\n");
		//	if(nom!=NULL)	delete [] nom;
		//	if(tex!=NULL)	delete [] tex;
			return (float) ne->probabilidad;
			}//If energía coincide exactamente	
		else	i=filas;			
			}//if estamos en su zona de energías
		if(elOriginal!=0)	{elAnt=elOriginal;	anteriorEsCero=false;}
		else				anteriorEsCero=true;
		}//for
	printf("fin de determinarProbabilidad. La probabilidad es %f \n", (float) ne->probabilidad);
//	if(nom!=NULL)	delete [] nom;
//	if(tex!=NULL)	delete [] tex;
		
	//Al final, añadimos emisiones que puedan ser debidas al fondo
//	if(fondoCargado)	determinarEmisionesFondo(energia, emisiones, nombreFichFBD);
	return (float) ne->probabilidad;
	}
