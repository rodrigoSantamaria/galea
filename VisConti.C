#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "avisos.h"
#include "BSpline.h"
#include "calibracion.h"
#include "constDef.h"
#include "cursores.h"
#include "fuentes.h"
#include "globales.h"
#include "VisGraf.h"
#include "trabajo.h"
#include "trabajoEst.h"
#include "VisAnal.h"
#include "VisCalib.h"
#include "VisConti.h"
#include <Xm/Form.h>
#include <Xm/Frame.h>
#include <Xm/Label.h>
#include <Xm/LabelG.h>
#include <Xm/List.h>
#include <Xm/PushB.h>
#include <Xm/RowColumn.h>
#include <Xm/Separator.h>
#include <Xm/Text.h>
#include <Xm/TextF.h>
#include <Xm/ToggleB.h>


// Datos
int Continuo = 0;
tListaContinuo listaContinuo;
int ContinuoSeleccionado = 0;
bool AjustandoContinuo = false;
XtIntervalId TimerAjustando;

// Widget general
Widget fondo_continuo, label_form_continuo;
Widget linea_conti_1, linea_conti_2;
Widget row1_continuo, row2_continuo, row3_continuo,
       lista_continuo, bt_form_continuo, bt_ajusta_continuo, bt_busca_continuo,
       entrada_frame_continuo, label_entrada_frame_continuo,
	cn_min_continuo, cn_max_continuo,
	lb_cn_min_continuo, lb_cn_max_continuo,
       cofo_continuo, copi_continuo, manphe_continuo,
	lb_cofo_continuo, lb_copi_continuo, lb_manphe_continuo;
Widget bt_agnade_continuo, bt_quita_continuo;
Widget vacio1_continuo, vacio2_continuo;
Widget info_frame_continuo, info_label_frame_continuo, info_rowcol_continuo;
Widget info_ChiSq_continuo, info_state_continuo, info_numPicos_continuo,
	info_numNodos_continuo;
Widget info_dat_ChiSq_continuo, info_dat_state_continuo, info_dat_numPicos_continuo,
	info_dat_numNodos_continuo;
Widget show_frame_continuo, label_frame_continuo, show_rowcol_continuo;
Widget show_adjust_continuo, show_channel_continuo, show_peak_continuo,
	show_normal_continuo;

// Funciones
Widget Crea_AreaContinuo (Widget);
void PonIntervaloContinuo(int, int);
void ReconstruyeListaContinuo();
tListaContinuo* DevuelveListaContinuo();
void CopiaListaContinuo(tListaContinuo);
void EstadoBotonesContinuo();
void TutelandoAjuste(XtPointer, XtIntervalId *);
void ActualizaVectorContinuo(double);

// Visualizaciones
void EstableceShowsContinuo(bool, bool, bool);


// Callbacks
void pulsa_agnadir_continuo(Widget, XtPointer, XtPointer);
void pulsa_borrar_continuo(Widget, XtPointer, XtPointer);
void pulsa_buscar_continuo(Widget, XtPointer, XtPointer);
void pulsa_ajustar_continuo(Widget, XtPointer, XtPointer);
void pulsa_show_continuo(Widget, XtPointer, XtPointer);
void pulsa_lista_continuo(Widget, XtPointer, XtPointer);

void seleccionarTexto(Widget, XtPointer, XtPointer);

// Implementacion
//******************** CREA AREA CONTINUO *******************
//Genera toda la interfaz del continuo: botones, textos, etc.
Widget Crea_AreaContinuo(Widget parent) {
   Arg		args[7], argLabel[4], argCuadro[4];
   Cardinal	i = 0, nLabel=0, nCuadro=0;
	XmString cadena;


	i = 0;
   fondo_continuo = XmCreateForm (parent, "fondo_continuo", args, i);
	
	// Crea el titulo del form
	i = 0;
   	cadena = XmStringCreate ("Continuum", "BoldNormal");
   	XtSetArg (args[i], XmNlabelString, cadena); i++;
   	XtSetArg (args[i], XmNfontList, lista_fuentes); i++;
	label_form_continuo = XmCreateLabel(fondo_continuo, "label_form_continuo", args, i);
   	XmStringFree (cadena);
	XtManageChild(label_form_continuo);

	// crea las lineas de separacion
	i = 0;
   	XtSetArg (args[i], XmNseparatorType, XmSHADOW_ETCHED_OUT); i++;
	linea_conti_1 = XmCreateSeparator(fondo_continuo, "linea_conti_1", args, i);
	linea_conti_2 = XmCreateSeparator(fondo_continuo, "linea_conti_2", args, i);
	XtManageChild(linea_conti_1);
	XtManageChild(linea_conti_2);


   // Crea los widgets de la busqueda de continuo

	// crea zona de lista de datos de busqueda de continuo
	i = 0;
	XtSetArg (args[i], XmNorientation, XmVERTICAL); i++;
	XtSetArg (args[i], XmNentryAlignment, XmALIGNMENT_CENTER); i++;
	row1_continuo = XmCreateRowColumn(fondo_continuo, "row1_continuo", args, i);
	
	i = 0;
	XtSetArg (args[i], XmNlistSizePolicy, XmCONSTANT); i++;
	XtSetArg (args[i], XmNselectionPolicy, XmSINGLE_SELECT); i++;
	XtSetArg (args[i], XmNvisibleItemCount, 9); i++;
	XtSetArg (args[i], XmNscrollBarDisplayPolicy, XmSTATIC); i++;
   	XtSetArg (args[i], XmNfontList, lista_fuentes); i++;
	lista_continuo = XmCreateScrolledList(row1_continuo, "lista_continuo", args, i);
	XtAddCallback (lista_continuo, XmNsingleSelectionCallback, pulsa_lista_continuo, NULL);
	
	i = 0;
	XtSetArg (args[i], XmNfractionBase, 2); i++;
	bt_form_continuo = XmCreateForm(row1_continuo, "bt_form_continuo", args, i);
	
	i = 0;
   	cadena = XmStringCreateLocalized ("Search");
   	XtSetArg (args[i], XmNlabelString, cadena); i++;
	XtSetArg (args[i], XmNalignment, XmALIGNMENT_CENTER); i++;
   	XtSetArg (args[i], XmNfontList, lista_fuentes); i++;
	bt_busca_continuo = XmCreatePushButton(bt_form_continuo, "bt_busca_continuo", args, i);
	XtAddCallback (bt_busca_continuo, XmNactivateCallback, pulsa_buscar_continuo, NULL);
   	XmStringFree (cadena);
	
	i = 0;
   	cadena = XmStringCreateLocalized ("Fit");
   	XtSetArg (args[i], XmNlabelString, cadena); i++;
	XtSetArg (args[i], XmNalignment, XmALIGNMENT_CENTER); i++;
	XtSetArg (args[i], XmNfontList, lista_fuentes); i++;
	bt_ajusta_continuo = XmCreatePushButton(bt_form_continuo, "bt_ajusta_continuo", args, i);
	XtAddCallback (bt_ajusta_continuo, XmNactivateCallback, pulsa_ajustar_continuo, NULL);
   	XmStringFree (cadena);

	XtManageChild(lista_continuo);
	XtManageChild(bt_busca_continuo);	
	XtManageChild(bt_ajusta_continuo);	
	XtManageChild(bt_form_continuo);	
	XtManageChild(row1_continuo);

   	XtVaSetValues (bt_busca_continuo,
	XmNleftAttachment, XmATTACH_FORM,
	XmNtopAttachment, XmATTACH_FORM,
	XmNrightAttachment, XmATTACH_POSITION, XmNrightPosition, 1,
	XmNbottomAttachment, XmATTACH_FORM, NULL);
   	XtVaSetValues (bt_ajusta_continuo,
		XmNleftAttachment, XmATTACH_POSITION, XmNleftPosition, 1,
		XmNtopAttachment, XmATTACH_FORM,
		XmNrightAttachment, XmATTACH_FORM,
		XmNbottomAttachment, XmATTACH_FORM,
		NULL);
   	XtVaSetValues (bt_form_continuo,
		XmNleftAttachment, XmATTACH_FORM,
		XmNrightAttachment, XmATTACH_FORM,
		XmNbottomAttachment, XmATTACH_FORM,
		NULL);
	XtVaSetValues(lista_continuo,
		XmNbottomAttachment, XmATTACH_WIDGET, XmNbottomWidget, bt_form_continuo,
		NULL);
	

   //********** INTRO DATA *************
   // crea zona de introduccion de datos de busqueda de continuo
   
   i = 0;
   entrada_frame_continuo = XmCreateFrame (fondo_continuo, "entrada_frame_continuo", args, i);

   cadena = XmStringCreateLocalized ("Intro Data");//NOTA: No sería mejor algo como Search Data?
   i = 0;
   XtSetArg (args[i], XmNchildType, XmFRAME_TITLE_CHILD); i++;
   XtSetArg (args[i], XmNchildVerticalAlignment, XmALIGNMENT_CENTER); i++;
   XtSetArg (args[i], XmNlabelString, cadena); i++;
   XtSetArg (args[i], XmNfontList, lista_fuentes); i++;
   label_entrada_frame_continuo = XmCreateLabelGadget (entrada_frame_continuo, "label_entrada_frame_continuo", args, i);
   XmStringFree (cadena);

   // Argumentos de las etiquetas
   nLabel = 0;
   XtSetArg (argLabel[nLabel], XmNmarginHeight, 0); nLabel++;
   XtSetArg (argLabel[nLabel], XmNmarginWidth, 0); nLabel++;
   XtSetArg (argLabel[nLabel], XmNfontList, lista_fuentes); nLabel++;
   
   // Argumentos de los cuadros
   nCuadro = 0;
   XtSetArg (argCuadro[nCuadro], XmNcolumns, 6); nCuadro++;
   XtSetArg (argCuadro[nCuadro], XmNmaxLength, 8); nCuadro++;
   XtSetArg (argCuadro[nCuadro], XmNmarginHeight, 1); nCuadro++;

   i = 0;
   XtSetArg (args[i], XmNpacking, XmPACK_COLUMN); i++;
   XtSetArg (args[i], XmNnumColumns, 2); i++;
   XtSetArg (args[i], XmNorientation, XmVERTICAL); i++;
   XtSetArg (args[i], XmNentryAlignment, XmALIGNMENT_CENTER); i++;	
   row2_continuo = XmCreateRowColumn(entrada_frame_continuo, "row2_continuo", args, i);
   
   //NOTA: NO sé donde le indica en qué posición se encuentran colocadas!
   // label
   cadena = XmStringCreateLocalized ("min channel");
   XtSetArg (argLabel[nLabel], XmNlabelString, cadena);
   lb_cn_min_continuo = XmCreateLabel(row2_continuo, "lb_cn_min_continuo", argLabel, nLabel+1);
   XmStringFree (cadena);
   
   // cuadro
   cn_min_continuo = XmCreateTextField(row2_continuo, "cn_min_continuo", argCuadro, nCuadro);
   XtAddCallback(cn_min_continuo, XmNfocusCallback, seleccionarTexto, NULL);   
   // label
   cadena = XmStringCreateLocalized ("cofo");
   XtSetArg (argLabel[nLabel], XmNlabelString, cadena);
   lb_cofo_continuo = XmCreateLabel(row2_continuo, "lb_cofo_continuo", argLabel, nLabel+1);
   XmStringFree (cadena);
   
   // cuadro
   cofo_continuo = XmCreateTextField(row2_continuo, "cofo_continuo", argCuadro, nCuadro);
   XtAddCallback(cofo_continuo, XmNfocusCallback, seleccionarTexto, NULL);   
  
   // label
   cadena = XmStringCreateLocalized ("manphe");
   XtSetArg (argLabel[nLabel], XmNlabelString, cadena);
   lb_manphe_continuo = XmCreateLabel(row2_continuo, "lb_manphe_continuo", argLabel, nLabel+1);
   XmStringFree (cadena);
   
   // cuadro
   manphe_continuo = XmCreateTextField(row2_continuo, "manphe_continuo", argCuadro, nCuadro);
   XtAddCallback(manphe_continuo, XmNfocusCallback, seleccionarTexto, NULL);   
 
   // boton
   i = 0;
   cadena = XmStringCreateLocalized ("Add");
   XtSetArg (args[i], XmNlabelString, cadena); i++;
   XtSetArg (args[i], XmNfontList, lista_fuentes); i++;
   bt_agnade_continuo = XmCreatePushButton(row2_continuo, "bt_agnade_continuo", args, i);
   XtAddCallback (bt_agnade_continuo, XmNactivateCallback, pulsa_agnadir_continuo, NULL);
   XmStringFree (cadena);
   
   // label
   cadena = XmStringCreateLocalized ("max channel");
   XtSetArg (argLabel[nLabel], XmNlabelString, cadena);
   lb_cn_max_continuo = XmCreateLabel(row2_continuo, "lb_cn_max_continuo", argLabel, nLabel+1);
   XmStringFree (cadena);
   
   // cuadro
   cn_max_continuo = XmCreateTextField(row2_continuo, "cn_max_continuo", argCuadro, nCuadro);
   XtAddCallback(cn_max_continuo, XmNfocusCallback, seleccionarTexto, NULL);   

	// label
   cadena = XmStringCreateLocalized ("copi");
   XtSetArg (argLabel[nLabel], XmNlabelString, cadena);
   lb_copi_continuo = XmCreateLabel(row2_continuo, "lb_copi_continuo", argLabel, nLabel+1);
   XmStringFree (cadena);
   
   // cuadro
   copi_continuo = XmCreateTextField(row2_continuo, "copi_continuo", argCuadro, nCuadro);
   XtAddCallback(copi_continuo, XmNfocusCallback, seleccionarTexto, NULL);   
	
   // labels
   cadena = XmStringCreateLocalized ("");
   XtSetArg (argLabel[nLabel], XmNlabelString, cadena);
   vacio1_continuo = XmCreateLabel(row2_continuo, "vacio1_continuo", argLabel, nLabel+1);
   vacio2_continuo = XmCreateLabel(row2_continuo, "vacio2_continuo", argLabel, nLabel+1);
   XmStringFree (cadena);
   i = 0;
   cadena = XmStringCreateLocalized ("Delete");
   XtSetArg (args[i], XmNlabelString, cadena); i++;
   XtSetArg (args[i], XmNfontList, lista_fuentes); i++;
   bt_quita_continuo = XmCreatePushButton(row2_continuo, "bt_quita_continuo", args, i);
   XtAddCallback (bt_quita_continuo, XmNactivateCallback, pulsa_borrar_continuo, NULL);
   XmStringFree (cadena);

   // Crea el cuadro de informacion sobre busqueda y ajuste del continuo
   i = 0;
   info_frame_continuo = XmCreateFrame (fondo_continuo, "info_frame_continuo", args, i);

   cadena = XmStringCreateLocalized ("Info");//NOTA: Quizá algo como Search Info? o Search Progress?
   i = 0;
   XtSetArg (args[i], XmNchildType, XmFRAME_TITLE_CHILD); i++;
   XtSetArg (args[i], XmNchildVerticalAlignment, XmALIGNMENT_CENTER); i++;
   XtSetArg (args[i], XmNlabelString, cadena); i++;
   XtSetArg (args[i], XmNfontList, lista_fuentes); i++;
   info_label_frame_continuo = XmCreateLabelGadget (info_frame_continuo, "info_label_frame_continuo", args, i);
   XmStringFree (cadena);
   i = 0;
   info_rowcol_continuo = XmCreateRowColumn(info_frame_continuo, "info_rowcol_continuo", args, i);
   i = 0;
   cadena = XmStringCreate ("Peaks Found", "BoldNormal");
   XtSetArg (args[i], XmNfontList, lista_fuentes); i++;
   XtSetArg (args[i], XmNlabelString, cadena); i++;
   info_numPicos_continuo = XmCreateLabel (info_rowcol_continuo, "info_numPicos_continuo", args, i);
   XmStringFree (cadena);
   i = 0;
   cadena = XmStringCreateLocalized ("  0");
   XtSetArg (args[i], XmNlabelString, cadena); i++;
   XtSetArg (args[i], XmNfontList, lista_fuentes); i++;
   info_dat_numPicos_continuo = XmCreateLabel (info_rowcol_continuo, "info_dat_numPicos_continuo", args, i);
   XmStringFree (cadena);
   i = 0;
   cadena = XmStringCreate ("Chi\262", "BoldNormal");
   XtSetArg (args[i], XmNfontList, lista_fuentes); i++;
   XtSetArg (args[i], XmNlabelString, cadena); i++;
   info_ChiSq_continuo = XmCreateLabel (info_rowcol_continuo, "info_ChiSq_continuo", args, i);
   XmStringFree (cadena);
   i = 0;
   cadena = XmStringCreateLocalized ("  0.00000");//NOTA: Igual son muchos ceros
   XtSetArg (args[i], XmNlabelString, cadena); i++;
   XtSetArg (args[i], XmNfontList, lista_fuentes); i++;
   info_dat_ChiSq_continuo = XmCreateLabel (info_rowcol_continuo, "info_dat_ChiSq_continuo", args, i);
   XmStringFree (cadena);
   i = 0;
   cadena = XmStringCreate ("BSpline knots", "BoldNormal");
   XtSetArg (args[i], XmNfontList, lista_fuentes); i++;
   XtSetArg (args[i], XmNlabelString, cadena); i++;
   info_numNodos_continuo = XmCreateLabel (info_rowcol_continuo, "info_numNodos_continuo", args, i);
   XmStringFree (cadena);
   i = 0;
   cadena = XmStringCreateLocalized ("  0");
   XtSetArg (args[i], XmNlabelString, cadena); i++;
   XtSetArg (args[i], XmNfontList, lista_fuentes); i++;
   info_dat_numNodos_continuo = XmCreateLabel (info_rowcol_continuo, "info_dat_numNodos_continuo", args, i);
   XmStringFree (cadena);
   i = 0;
   cadena = XmStringCreate ("State Continuum", "BoldNormal");
   XtSetArg (args[i], XmNfontList, lista_fuentes); i++;
   XtSetArg (args[i], XmNlabelString, cadena); i++;
   info_state_continuo = XmCreateLabelGadget (info_rowcol_continuo, "info_state_continuo", args, i);
   XmStringFree (cadena);
   i = 0;
   cadena = XmStringCreateLocalized ("  Void");
   XtSetArg (args[i], XmNlabelString, cadena); i++;
   XtSetArg (args[i], XmNfontList, lista_fuentes); i++;
   info_dat_state_continuo = XmCreateLabel (info_rowcol_continuo, "info_dat_state_continuo", args, i);
   XmStringFree (cadena);

	
	XtManageChild(info_state_continuo);			
	XtManageChild(info_numNodos_continuo);			
	XtManageChild(info_numPicos_continuo);			
	XtManageChild(info_ChiSq_continuo);			
	XtManageChild(info_dat_state_continuo);			
	XtManageChild(info_dat_numNodos_continuo);			
	XtManageChild(info_dat_numPicos_continuo);			
	XtManageChild(info_dat_ChiSq_continuo);			
	XtManageChild(info_rowcol_continuo);		
	XtManageChild(info_label_frame_continuo);
	XtManageChild(info_frame_continuo);
	
	
   // Crea opciones de representacion
   i = 0;
   show_frame_continuo = XmCreateFrame (fondo_continuo, "show_frame_continuo", args, i);

   cadena = XmStringCreateLocalized ("Show");//NOTA: Quizá mejor un View?
   i = 0;
   XtSetArg (args[i], XmNchildType, XmFRAME_TITLE_CHILD); i++;
   XtSetArg (args[i], XmNchildVerticalAlignment, XmALIGNMENT_CENTER); i++;
   XtSetArg (args[i], XmNlabelString, cadena); i++;
   XtSetArg (args[i], XmNfontList, lista_fuentes); i++;
   label_frame_continuo = XmCreateLabelGadget (show_frame_continuo, "label_frame_continuo", args, i);
   XmStringFree (cadena);

   i = 0;
   show_rowcol_continuo = XmCreateRowColumn(show_frame_continuo, "show_rowcol_continuo", args, i);

	// Toggle buton para los canales del continuo
   cadena = XmStringCreateLocalized ("Continuum\n  Channels");
   i = 0;
   XtSetArg (args[i], XmNset, false); i++;
   XtSetArg (args[i], XmNlabelString, cadena); i++;
   XtSetArg (args[i], XmNfontList, lista_fuentes); i++;
	show_channel_continuo = XmCreateToggleButton (show_rowcol_continuo, "show_channel_continuo", args, i);
	XtAddCallback (show_channel_continuo, XmNvalueChangedCallback, pulsa_show_continuo, (XtPointer) 1);
   XmStringFree (cadena);
	// Toggle buton para los canales de picos
   cadena = XmStringCreateLocalized ("Peaks\n  Channels");
   i = 0;
   XtSetArg (args[i], XmNset, false); i++;
   XtSetArg (args[i], XmNlabelString, cadena); i++;
   XtSetArg (args[i], XmNfontList, lista_fuentes); i++;
	show_peak_continuo = XmCreateToggleButton (show_rowcol_continuo, "show_peak_continuo", args, i);
	XtAddCallback (show_peak_continuo, XmNvalueChangedCallback, pulsa_show_continuo, (XtPointer) 2);
   XmStringFree (cadena);
	// Toggle button para el ajuste
   cadena = XmStringCreateLocalized ("Continuum\n  Fit");
   i = 0;
   XtSetArg (args[i], XmNset, false); i++;
   XtSetArg (args[i], XmNlabelString, cadena); i++;
   XtSetArg (args[i], XmNfontList, lista_fuentes); i++;
	show_adjust_continuo = XmCreateToggleButton (show_rowcol_continuo, "show_adjust_continuo", args, i);
	XtAddCallback (show_adjust_continuo, XmNvalueChangedCallback, pulsa_show_continuo, (XtPointer) 3);
   XmStringFree (cadena);
	// Toggle button para los canales normales
   cadena = XmStringCreateLocalized ("Not show\n  Rest");//NOTA: No entiendo por qué lo llama not show rest
   i = 0;
   XtSetArg (args[i], XmNset, false); i++;
   XtSetArg (args[i], XmNlabelString, cadena); i++;
   XtSetArg (args[i], XmNfontList, lista_fuentes); i++;
   show_normal_continuo = XmCreateToggleButton (show_rowcol_continuo, "show_normal_continuo", args, i);
   XtAddCallback (show_normal_continuo, XmNvalueChangedCallback, pulsa_show_continuo, (XtPointer) 4);
   XmStringFree (cadena);

	XtManageChild(lb_cn_min_continuo);
	XtManageChild(lb_cn_max_continuo);
	XtManageChild(lb_cofo_continuo);
	XtManageChild(lb_copi_continuo);
	XtManageChild(lb_manphe_continuo);
	XtManageChild(cn_min_continuo);
	XtManageChild(cn_max_continuo);
	XtManageChild(cofo_continuo);
	XtManageChild(copi_continuo);
	XtManageChild(manphe_continuo);
	XtManageChild(vacio1_continuo); // para que queden dos columnas bien ajustadas
	XtManageChild(vacio2_continuo);
	XtManageChild(bt_agnade_continuo);
	XtManageChild(bt_quita_continuo);		
	XtManageChild(row2_continuo);
	XtManageChild(label_entrada_frame_continuo);
	XtManageChild(entrada_frame_continuo);

	XtManageChild(label_frame_continuo);
	XtManageChild(show_normal_continuo);
	XtManageChild(show_channel_continuo);	
	XtManageChild(show_peak_continuo);
	XtManageChild(show_adjust_continuo);
	XtManageChild(show_rowcol_continuo);
	XtManageChild(show_frame_continuo);
	


   // coloca los widgets
   //Los widgets que tenemos son los 4 recuadros: la ventana de texto, la entrada de datos, el cuadro de información y 
   //el cuadro de show; más las dos líneas de separación.
   XtVaSetValues (linea_conti_1,
		XmNleftAttachment, XmATTACH_FORM,
		XmNtopAttachment, XmATTACH_FORM,
		XmNrightAttachment, XmATTACH_FORM,
		NULL);
   XtVaSetValues (linea_conti_2,
		XmNleftAttachment, XmATTACH_FORM,
		XmNrightAttachment, XmATTACH_FORM,
		XmNbottomAttachment, XmATTACH_FORM,
		NULL);
   XtVaSetValues (label_form_continuo,
		XmNleftAttachment, XmATTACH_FORM,
		XmNbottomAttachment, XmATTACH_WIDGET, XmNbottomWidget, linea_conti_2,
		XmNleftOffset, 10,
		NULL);
   XtVaSetValues (show_frame_continuo,
		XmNbottomAttachment, XmATTACH_FORM,
		XmNbottomOffset, 20,
		XmNtopAttachment, XmATTACH_FORM,
		XmNtopOffset, 10,
		XmNrightAttachment, XmATTACH_FORM,
		XmNrightOffset, 10,
		NULL);
   XtVaSetValues (info_frame_continuo,
		XmNbottomAttachment, XmATTACH_FORM,
		XmNbottomOffset, 20,
		XmNtopAttachment, XmATTACH_FORM,
		XmNtopOffset, 10,
		XmNrightAttachment, XmATTACH_WIDGET, XmNrightWidget, show_frame_continuo,
		XmNrightOffset, 10,
		NULL);
   XtVaSetValues (entrada_frame_continuo,
		XmNtopAttachment, XmATTACH_FORM,
		XmNtopOffset, 10,
		XmNbottomAttachment, XmATTACH_FORM,
		XmNbottomOffset, 20,
		XmNrightAttachment, XmATTACH_WIDGET, XmNrightWidget, info_frame_continuo,
		XmNrightOffset, 10,
		NULL);
   XtVaSetValues (row1_continuo,
		XmNtopAttachment, XmATTACH_FORM,
		XmNtopOffset, 10,
		XmNleftAttachment, XmATTACH_FORM,
		XmNleftOffset, 10,
		XmNrightAttachment, XmATTACH_WIDGET, XmNrightWidget, entrada_frame_continuo,
		XmNrightOffset, 10,
		XmNbottomAttachment, XmATTACH_FORM,
		XmNbottomOffset, 20,
		NULL);
   XtVaSetValues (fondo_continuo,
		XmNrightAttachment, XmATTACH_FORM,
		XmNleftAttachment, XmATTACH_FORM,
		XmNtopAttachment, XmATTACH_FORM,
		XmNbottomAttachment, XmATTACH_FORM,
		NULL);

   listaContinuo.numElementos = 0;

   return fondo_continuo;
}

//************************ ACTUALIZA DATOS CONTINUO ******************
//Actualiza los datos que aparecen en el cuadro de información con los datos de la
//variable principal de tipo cEspectros que se llama Datos.
//NOTA: Falta ver cómo se modifican esos campos en Datos (ver cEspectros.c)

void ActualizaDatosContinuo() {
   XmString cadena;
   char dato[] = "xxxxxxxxxxx";
	int valor;
	double valor2;

	// numero de picos
	if (EstadoTrabajo.PreContinuo) valor = Datos.numPicosEncontrados;
	else valor = 0;
	sprintf(dato, "  %d", valor);
	cadena = XmStringCreateLocalized (dato);
	XtVaSetValues (info_dat_numPicos_continuo, XmNlabelString, cadena, NULL);
	XmStringFree (cadena);

	// Valor de ChiSq
	if ((AjustandoContinuo)||(EstadoTrabajo.Continuo)) valor2 = Datos.ChiSqContinuo;
	else valor2 = 0.0;
	sprintf(dato, "  %.5f", valor2);
	cadena = XmStringCreateLocalized (dato);
	XtVaSetValues (info_dat_ChiSq_continuo, XmNlabelString, cadena, NULL);
	XmStringFree (cadena);
	
	// Numero de nodos
	if ((AjustandoContinuo)||(EstadoTrabajo.Continuo))	valor = Datos.nNodosSplineContinuo;
	else valor = 0;
	sprintf(dato, "  %d", valor);
	cadena = XmStringCreateLocalized (dato);
	XtVaSetValues (info_dat_numNodos_continuo, XmNlabelString, cadena, NULL);
	XmStringFree (cadena);

	// estado del continuo
	if (AjustandoContinuo)
	//NOTA: Daba el error de "invalid conversion from const char * to char *". He hecho una conversión típica a (char *) 
	//      pero aún no he localizado dónde están definidas esas macros.
		cadena = XmStringCreateLocalized ((char *)CONTINUO_AJUSTANDO);
	else 
		{
		printf("PONIENDO POSICIÓN DEL CURSOR\n");
		PonPosicionCursor();
		printf("PUESTA POSICIÖN CURSOR\n");
		if (!EstadoTrabajo.PreContinuo)
			{
			cadena = XmStringCreateLocalized ((char *)CONTINUO_VACIO);
			}
		else 
			{
			if (!EstadoTrabajo.Continuo)
				cadena = XmStringCreateLocalized ((char *)CONTINUO_BUSCADO);
			else
				cadena = XmStringCreateLocalized ((char *)CONTINUO_AJUSTADO);
			}
		}
	XtVaSetValues (info_dat_state_continuo, XmNlabelString, cadena, NULL);
	XmStringFree (cadena);
	
	// debido a un problema desconocido en la actualizacion de la
	// presentacion, las cadenas introducidas han de ser cortas
	// o bien forzar la presentacion de nuevo del frame. Para evitar una saturacion
	// de procesos graficos debido al proceso de fondo del ajuste al continuo
	// lo dejamos en cadenas cortas (este problema está relacionado con
	// el tamagno fijo del form; cuando la ventana principal tiene permitido
	// el autoajuste del tamagno esta incidencia no ocurre)
	printf("Termina actualiza datos continuo\n");
	printf("Tenemos %d intervalos en la lista\n", Datos.listaPicos.numElementos);//Aquí ya hay 0!!!
}

void PonIntervaloContinuo(int minimo, int maximo) 
	{
	char result1[]="xxxxxxxx";

	// pone el canal izquierdo
	if (minimo < 1) minimo = 1;
	sprintf(result1, "%d", minimo);
	XmTextSetString(cn_min_continuo, result1);
	// pone el canal derecho
	if (maximo > Datos.numCanales) maximo = Datos.numCanales;
	sprintf(result1, "%d", maximo);
	XmTextSetString(cn_max_continuo, result1);
	}

//*************************** PULSA AÑADIR CONTINUO *******************************
//Añade la información sobre otro intervalo para poder calcular el continuo.
//Primero hace una comprobación exhaustiva de que todos los parámetros se hayan introducido
//de forma correcta.
//Despues añade un nodo con dicha información a la lista "listaContinuo"
void pulsa_agnadir_continuo(Widget widget, XtPointer client_data, XtPointer call_data){
	char result1[]="xxxxxxxx", texto_canales[80];
	char *cofoText, *copiText, *manpheText, *canIzqText, *canDerText, *seguridad;
	double cofo, copi, manphe;
	int canIzq, canDer;
	XmString t;
	bool correcto = true;

	printf("Añadir Continuo --- INICIO\n");
	tListaCalibracion* lista = DevuelveListaCalibracion();
	if (IsActiveCalibration()&&(lista->numElementos > 0)) 
		{
		// TODO: 
		// Si se esta haciendo calculo de calibracion, avisa de que se perderan los datos
		// al cambiar el continuo
		int respuesta=Continuar(widget, "Calibration data will be lost if continuum fit is recalculated. Continue?", "Caution");
		switch(respuesta)
			{
			case SI:
				//Borrar la información de la calibración y cambiar el estado
				DesactivaCalibracion();
				printf("Borrar y cambiar\n");
				break;
			case NO: //En ambos casos lo único que hacemos es volver sin más
			case CANCELAR:
				return; 
			}
		}

	// Comprueba el valor del canal izquierdo
	canIzqText = XmTextGetString(cn_min_continuo);
	canIzq = (int)strtol(canIzqText, &seguridad, 10);
	if (seguridad == canIzqText) 
		{
		Aviso(widget, "Left channel format incorrect. Please, re-type.");
		sprintf(result1, "");
		XmTextSetString(cn_min_continuo, result1);
		correcto = false;
		XtFree(canIzqText);
		return;
		}
	else
		{
		if ((canIzq > 0)&&(canIzq < Datos.numCanales)) 
			{
			sprintf(result1, "%d", canIzq);
			XmTextSetString(cn_min_continuo, result1);
			canIzq = atol(result1);
			XtFree(canIzqText);
			}
		else 	
			{
			if (canIzq <= 0)	sprintf(texto_canales, "Minimum channel value must be greater than 0.");
			else			sprintf(texto_canales, "Minimum channel value must be lesser than %d.", Datos.numCanales);
			Aviso(widget, texto_canales);
			sprintf(result1, "");
			XmTextSetString(cn_min_continuo, result1);
			correcto = false;
			XtFree(canIzqText);
			return;
			}		
		}

	// Comprueba el valor del canal derecho
	canDerText = XmTextGetString(cn_max_continuo);
	canDer = (int)strtol(canDerText, &seguridad, 10);
	if (seguridad == canDerText) 
		{
		Aviso(widget, "Maximum channel format incorrect. Please, re-type.");
		sprintf(result1, "");
		XmTextSetString(cn_max_continuo, result1);
		correcto = false;
		XtFree(canDerText);
		return;
		}
	else 	
		{
		if ((canDer > 0)&&(canDer <= Datos.numCanales)&&(canDer > canIzq)) 
			{
			sprintf(result1, "%d", canDer);
			XmTextSetString(cn_max_continuo, result1);
			canDer = atol(result1);
			XtFree(canDerText);
			}
		else 	
			{
			if (canDer <= 0)	sprintf(texto_canales, "Maximum channel value must be greater than 0.");
			else if (canDer > Datos.numCanales)
				sprintf(texto_canales, "Maximum channel value must be lesser than %d.", Datos.numCanales);
			else
				sprintf(texto_canales, "Maximum channel value must be greater than minimum channel value.");
			Aviso(widget, texto_canales);
			sprintf(result1, "");
			XmTextSetString(cn_max_continuo, result1);
			correcto = false;
			XtFree(canDerText);
			return;
			}
		}

	printf("Comprobaciones de textos\n");
	// Comprueba cofo
	cofoText = XmTextGetString(cofo_continuo);
	cofo = strtod(cofoText, &seguridad);
	if ((strlen(cofoText) == 0)||(cofo < 0.0)) 
		{
		cofo = -1.0;
		sprintf(result1, "%3.1f", cofo);
		XmTextSetString(cofo_continuo, result1);
		XtFree(cofoText);
		}   
	else if (seguridad == cofoText) 
		{
		Aviso(widget, "Cofo format incorrect. Please, re-type.");
		sprintf(result1, "");
		XmTextSetString(cofo_continuo, result1);
		correcto = false;
		XtFree(cofoText);
		return;
		}
	else 	{
		sprintf(result1, "%.2f", cofo);
		XmTextSetString(cofo_continuo, result1);
		cofo = atof(result1);
		XtFree(cofoText);
		}

	// Comprueba copi
	copiText = XmTextGetString(copi_continuo);
	copi = strtod(copiText, &seguridad);
	if ((strlen(copiText) == 0)||(copi < 0.0)) 
		{
		copi = -1.0;
		sprintf(result1, "%3.1f", copi);
		XmTextSetString(copi_continuo, result1);
		XtFree(copiText);
		}   
	else if (seguridad == copiText) 
		{
		Aviso(widget, "Copi format incorrect. Please, re-type.");
		sprintf(result1, "");
		XmTextSetString(copi_continuo, result1);
		correcto = false;
		XtFree(copiText);
		return;
		}
	else 	{
		sprintf(result1, "%.2f", copi);
		XmTextSetString(copi_continuo, result1);
		copi = atof(result1);
		XtFree(copiText);
		}

	// Comprueba manphe
	manpheText = XmTextGetString(manphe_continuo);
	manphe = strtod(manpheText, &seguridad);
	if ((strlen(manpheText) == 0)||(manphe < 0.0)) 
		{
		manphe = -1.0;
		sprintf(result1, "%3.1f", manphe);
		XmTextSetString(manphe_continuo, result1);
		XtFree(manpheText);
		}   
	else if (seguridad == manpheText) 
		{
		Aviso(widget, "Manphe format incorrect. Please, re-type.");
		sprintf(result1, "");
		XmTextSetString(manphe_continuo, result1);
		correcto = false;
		XtFree(manpheText);
		return;
		}
	else 	{
		sprintf(result1, "%.2f", manphe);
		XmTextSetString(manphe_continuo, result1);
		manphe = atof(result1);
		XtFree(manpheText);
		}

	// Comprueba si los 3 parametros estan "desactivados"
	if ((cofo == -1.0)&&(copi == -1.0)&&(manphe == -1.0)) 
		{
		Aviso(widget, "Cofo, Copi or Manphe must be larger than 0.");
		correcto = false;
		return;
		}

	printf("Añadiendo intervalo\n");
	
	//********************************************fin comprobaciones
	// Todo correcto, añade intervalo a la lista "listaContinuo"
	NodoContinuo *nodo;
	if ((nodo = new NodoContinuo) != NULL) 
		{
		if (IsActiveCalibration())	DesactivaCalibracion();
	
		nodo->cofo = cofo;
		nodo->copi = copi;
		nodo->manphe = manphe;
		nodo->limInferior = canIzq;
		nodo->limSuperior = canDer;
		ContinuoSeleccionado = agnadeNodo(listaContinuo, nodo);

		// reconstruye la lista que sale en el widget
		XmListDeleteAllItems(lista_continuo);
		for (int i = 1; i <= listaContinuo.numElementos; i++) 
			{
			// coge cada elemento en la lista del continuo y lo
			// convierte en una entrada en el widget que saca un listado de los intervalos que se
			// usan para el continuo
			nodo = DevuelveNodo(listaContinuo, i);
			if (nodo != NULL) 
				{
				sprintf(texto_canales, "[%d, %d]: ", nodo->limInferior, nodo->limSuperior);
				if (nodo->cofo >= 0.0) sprintf(texto_canales, "%s %.1f cf", texto_canales, nodo->cofo);
				if (nodo->copi >= 0.0) sprintf(texto_canales, "%s %.1f cp", texto_canales, nodo->copi);
				if (nodo->manphe >= 0.0) sprintf(texto_canales, "%s %.1f mp", texto_canales, nodo->manphe);
				t = XmStringCreateLocalized (texto_canales);
				XmListAddItemUnselected(lista_continuo, t, 0);
			   	XmStringFree (t);
				}
			}
		XmListSelectPos(lista_continuo, ContinuoSeleccionado, false);
		EstadoTrabajo.TrabajoCambios = true;
		PonTituloFichCambiado();
		EstadoTrabajo.SearchContinuumCambios = true;
		EstadoTrabajo.AdjustContinuumCambios = true;
		
		printf("Vamos a borrar datos de los canales de picos\n");
		BorraPicosChEnGrafica();
		printf("Vamos a borrar datos de los canales de continuo\n");
		printf("%d\n", BorraContinuoChEnGrafica());
		printf("Si hemos ajustado el continuo\n");
		if (EstadoTrabajo.Continuo) 
			{
			// borra todos los datos de ajuste del continuo anterior
			printf("Vamos a borrar los datos del continuo de la gráfica\n");
			BorraContinuoEnGrafica();
			printf("Datos borrados\n");
			Datos.BorraAjusteCanalesContinuo();
			EstadoTrabajo.Continuo = false;
			PintaGrafica(0);
			DesactivaCalibracion();
			}
		printf("Terminamos\n");

		EstadoBotonesContinuo();
		}
	}

//******************* PULSA BORRAR CONTINUO ***********************
//Se activa cuando se pulsa el botón "Delete" del widget que lista los intervalos
//Borra de la lista del widget y de listaContinuo toda la información de ese intervalo.
void pulsa_borrar_continuo(Widget widget, XtPointer client_data, XtPointer call_data)
	{
	tListaCalibracion* lista = DevuelveListaCalibracion();
	if (IsActiveCalibration()&&(lista->numElementos > 0)) 
		{
		// TODO: 
		// Si se esta haciendo calculo de calibracion, avisa de que se perderan los datos
		// al cambiar el continuo
		
		return;
		}

	// busca el elemento seleccionado
	int j = 0;
	
	for (int i = 1; i <= listaContinuo.numElementos; i++)
		if (XmListPosSelected (lista_continuo, i)) j = i;

	if (j > 0) 
		{
		if (IsActiveCalibration())		DesactivaCalibracion();
	
		BorraPicosChEnGrafica();
		BorraContinuoChEnGrafica();

		// Borra el elemento en la lista y en el widget
		quitaNodo(listaContinuo, j);
		XmListDeletePos (lista_continuo, j);
	
		if (listaContinuo.numElementos > 0) 
			{
			if (listaContinuo.numElementos >= j)	ContinuoSeleccionado = j;
			else									ContinuoSeleccionado = listaContinuo.numElementos;
			XmListSelectPos(lista_continuo, ContinuoSeleccionado, false);
			}
	
		EstadoTrabajo.TrabajoCambios = true;
		EstadoTrabajo.SearchContinuumCambios = true;
		EstadoTrabajo.AdjustContinuumCambios = true;
		if (EstadoTrabajo.Continuo) 
			{
			// borra todos los datos de ajuste del continuo anterior
			BorraContinuoEnGrafica();
			Datos.BorraAjusteCanalesContinuo();
			EstadoTrabajo.Continuo = false;
			PintaGrafica(0);
//			DesactivaCalibracion();
			}
		EstadoBotonesContinuo();
		PonTituloFichCambiado();
		}
	}

//*********************** PULSA BUSCAR CONTINUO ***************************
//Comienza la búsqueda del continuo correspondiente al espectro analizado.
//Para que se pueda realizar la búsqueda del continuo tenemos que tener información 
//acerca del cofo, copi y manphe en al menos tres intervalos.
void pulsa_buscar_continuo(Widget widget, XtPointer client_data, XtPointer call_data) 
	{
	int error;
	//Comprobamos que tengamos información sobre algún intervalo
	if (listaContinuo.numElementos < 1) 
		{
		Aviso(fondo_continuo, "The parameters of continuum and peaks searching is empty.");
		return;
		}
	//Ahora construye una lista por intervalos con cada parámetro (cofo, copi y manphe)
	//Para ello llama a construyeLista (está en continuo.c). listaContinuo es la lista que hemos construido
	//según se le iba dando a añadir. Datos.serieCofo/Copi/Manphe son campos de la megaestructura cEspectros
	//cada uno es una lista de tipo tListaContinuoInterv, con nodos que llevan limSup, LimInf y valor.
	//Valor sea cofo, copi o manphe según la lista que usemos (serieCofo/Copi/Manphe)
	error = construyeLista(listaContinuo, Datos.serieCofo, 1);
	if (error < 0) 
		{
		Aviso(fondo_continuo, "No Memory Avalaible.");
		//NOTA: Esto no es del todo cierto. construyeLista() puede devolver distintos valores negativos
		//y no todos se deben a la falta de memoria:
		//-3 indica que el tipo de dato que se indica en el tercer parámetro es desconocido
		//-2 es falta de memoria
		//-1 no se devuelve nunca
		printf(ERROR_CONTINUUM);
		printf(ERROR_NO_MEMORY);
		return;
		}
	//construyeLista devuelve un uno si no hay completitud en la lista, es decir, si el límite superior
	//referente al intervalo contenido en un nodo coincide con el inferior del nodo siguiente
	//NOTA: de algún modo, eso no es exactamente lo mismo que decir que no hay un valor de cofo/copi/manphe en
	//	todos los intervalos, como dice el mensaje de aviso!
	//	Quizá sería más adecuado decir que "The intervals must be continuous one to the next"
	//	Hemos optado de momento por la frase "Each interval must finish exactly where the next interval starts"
	else if (error == 1) 
		{
		Aviso(fondo_continuo, "Each interval must finish exactly where the next interval starts");
		return;
		}

	error = construyeLista(listaContinuo, Datos.serieCopi, 2);
	if (error < 0) 
		{
		Aviso(fondo_continuo, "No Memory Avalaible.");
		printf(ERROR_CONTINUUM);
		printf(ERROR_NO_MEMORY);
		return;
		}
	else if (error == 1) 
		{
		Aviso(fondo_continuo, "Each interval must finish exactly where the next interval starts");
		return;
		}
	
	error = construyeLista(listaContinuo, Datos.serieManphe, 3);
	if (error < 0) 
		{
		Aviso(fondo_continuo, "No Memory Avalaible.");
		printf(ERROR_CONTINUUM);
		printf(ERROR_NO_MEMORY);
		return;
		}
	//	No obligamos como en los casos anteriores que manphe esté definida en todos los intervalos, pues
	//	manphe es un valor opcional que nos indica la anchura que deberían tener los picos en cada intervalo.
	//	Si no la indicamos, se realiza una estimación en función del FHWG calculado durante la precalibración
	
		
	BorraPicosChEnGrafica();
	BorraContinuoChEnGrafica();
	
	//Ahora, con todos los datos de los intervalos correctos, llamamos a una función de la megaestructura,
	//que realiza la búsqueda del continuo/picos
	error = Datos.BuscaCanalesContinuo(EstadoTrabajo.CalibracionForma);
	//Actualizamos los datos que salen en el widget llamado Info
	//NOTA: Realmente en este paso sólo nos va a cambiar el número de picos encontrados y el estado que lo pondrá a 
	//	searched. Los BSpline y la Chi2 siguen sin tener un valor definido hasta el ajuste
	ActualizaDatosContinuo();
	printf("Volvemos en pulsa buscar continuo\n");
	switch (error) 
		{
		case -1:
			Aviso(fondo_continuo, "General error");
			printf(ERROR_CONTINUUM);
			printf(ERROR_GENERAL);
			return;
		case -2://NOTA: Lo mismo de antes, el error parece que sería mejor decir que los intervalos no son contiguos
			Aviso(fondo_continuo, "Each interval must finish exactly where the next interval starts");
			return;
		case -3:
			Aviso(fondo_continuo, "Cofo and Copi not defined in same range.");
			return;
		case -4:
			Aviso(fondo_continuo, "No Memory Avalaible");
			printf(ERROR_CONTINUUM);
			printf(ERROR_NO_MEMORY);
			return;
		case -5:
			Aviso(fondo_continuo, "Continuum not found with this parameters.");
			return;			
		case -6:
			Aviso(fondo_continuo, "Peaks not found with this parameters.");
			return;			
		}
	
	//Llamamos a varias funciones de gestión de gráficos para que muestren el continuo y los picos que hemos 
	//encontrado. Estas funciones se encuentran en el fichero VisGraf.C
	CopiaPicosChGrafica();
	CopiaContinuoChGrafica();
	ActivaPicosChGrafica(EstadoVisual.CanalPicos);
	ActivaContinuoChGrafica(EstadoVisual.CanalContinuo);
	
		// TODO: cuando no hay al menos 3 intervalos que tengan un solo pico
		// no se podra hacer calibracion. Asi pues, no dar como buena una busqueda
		// de picos que no de al menos 3 intervalos si no esta todavia la calibracion
		
	// Indica cambio en el trabajo
	EstadoTrabajo.TrabajoCambios = true;
	PonTituloFichCambiado();

	EstadoTrabajo.PreContinuo = true;
	if (EstadoTrabajo.Continuo) 
		{//Si ya estaba realizado el continuo calculado con la calibración hecha, borramos 
		//toda esa información porque ahora se recalculará
		// borra todos los datos de ajuste del continuo anterior
		BorraContinuoEnGrafica();
		Datos.BorraAjusteCanalesContinuo();
		EstadoTrabajo.Continuo = false;
		}
	EstadoTrabajo.AdjustContinuumCambios = true;
	EstadoTrabajo.SearchContinuumCambios = false;
	EstadoBotonesContinuo();
	
	ActualizaDatosContinuo();
	printf("Volvemos en pulsa buscar continuo II\n");
	PintaGrafica(0);
	}

//************************* PULSA AJUSTAR CONTINUO *****************************
// Se activa cuando pulsamos el botón "Adjust" en la interfaz de continuum
// Realiza el ajuste del continuo.
void pulsa_ajustar_continuo(Widget widget, XtPointer client_data, XtPointer call_data) 
	{
	// aviso si no hay busqueda
	if (!EstadoTrabajo.PreContinuo) 
		{
		Aviso(widget, "You must do a channel searching before.");
		return;
		}
	// aviso si no hay busqueda actualizada
	if (EstadoTrabajo.SearchContinuumCambios) 
		{
		Aviso(widget, "You must do a new channel searching before.");
		return;
		}
	// cancela el ajuste si esta en medio de uno
	if (AjustandoContinuo) 
		{
		XtRemoveTimeOut(TimerAjustando);
		AjustandoContinuo = false;
		EstadoTrabajo.AdjustContinuumCambios = true;
		EstadoTrabajo.Continuo = false;
		Datos.CancelaAjusteCanalesContinuo();
		EstadoBotonesContinuo();
		BorraContinuoEnGrafica();
		return;
		}

	// Indica cambio en el trabajo
	EstadoTrabajo.TrabajoCambios = true;
	PonTituloFichCambiado();
	printf("Cambia el estado de trabajo\n");

	// prepara los datos para hacer el ajuste
	BorraContinuoEnGrafica();
	int error;
	
	// AJUSTAMOS EL CONTINUO
	error = Datos.PreparaAjusteCanalesContinuo();
	switch (error) 
		{
		case -1:
			Aviso(fondo_continuo, "General error");
			printf(ERROR_CONTINUUM);
			printf(ERROR_GENERAL);
			return;
		case -2:
			Aviso(fondo_continuo, "No Memory Avalaible");
			printf(ERROR_CONTINUUM);
			printf(ERROR_NO_MEMORY);
			return;
		}
	
	AjustandoContinuo = true;
	EstadoBotonesContinuo();
	printf("Llamamos a tuteLando Ajuste\n");
	//XtAppAddTimeOut(x,intervalo,funcion,x) inicia función despues de pasado un tiempo intervalo (ms)
	//Ahora mismo, con intervalo=0, es como llamar directamente a TutelandoAjuste.
	TimerAjustando = XtAppAddTimeOut(app_context, (unsigned long) 0, TutelandoAjuste, NULL);
	}

//**************** ACTUALIZA VECTOR CONTINUO ********************
void ActualizaVectorContinuo(double pos) 
	{
	CalculaIntervalosBSpline(Datos.SplineContinuo, pos);
	}

//****************** TUTELANDO AJUSTE ****************************
//NOTA: Se llama desde pulsar ajustar continuo una vez que se ha preparado
//	el ajuste de los canales de continuo. No se llama directamente
//	desde allí sino a través de XtAppAddTimeOut(), que no le especifica los
//	parámetros por lo que parece, pero de todos modos no se usan
//	En resumen, creo que simplemente lo puso para hacer una medición de tiempos,
//	así que quizá en un futuro se pueda hacer una llamada normal y quitarle sus parámetros.

void TutelandoAjuste(XtPointer SinUsar1, XtIntervalId *SinUsar2) 
	{
	int error = Datos.CicloAjusteCanalesContinuo();
	ActualizaDatosContinuo();
	printf("Volvemos en tutelando ajuste\n");
	switch (error) 
		{
		case -1:
			AjustandoContinuo = false;
			EstadoBotonesContinuo();
			Aviso(fondo_continuo, "General error");
			printf(ERROR_CONTINUUM);
			printf(ERROR_GENERAL);
			return;
		case -2:
			AjustandoContinuo = false;
			EstadoBotonesContinuo();
			Aviso(fondo_continuo, "No Memory Avalaible");
			printf(ERROR_CONTINUUM);
			printf(ERROR_NO_MEMORY);
			return;
		}
	// si no hay errores y quedan nodos por introducir se vuelve a
	// llamar a si misma. Si el proceso ha terminado lo cierra
	if (error == 0) 
		{
		// por si la operacion dura mucho tiempo
		PonCursorEspera();
		
		AjustandoContinuo = false;
		EstadoBotonesContinuo();

		int error = Datos.FinalAjusteCanalesContinuo();
		XBell(dpy, 0); // da aviso sonoro de proceso terminado
		switch (error) 
			{
			case -1:
				PonCursorNormal();
				Aviso(fondo_continuo, "General error");
				printf(ERROR_CONTINUUM);
				printf(ERROR_GENERAL);
				return;
			case -2:
				PonCursorNormal();
				Aviso(fondo_continuo, "No Memory Avalaible");
				printf(ERROR_CONTINUUM);
				printf(ERROR_NO_MEMORY);
				return;
			}


		// asocia los datos del continuo a la grafica que los representa
		printf("Copiamos continuo a gráfica\n");
		error = CopiaContinuoAGrafica();
		
//		EstadoBotonesContinuo();
		if (error < 0) 
			{
			PonCursorNormal();
			Aviso(fondo_continuo, "No Memory Avalaible");
			printf(ERROR_CONTINUUM);
			printf(ERROR_NO_MEMORY);
			return;
			}
		printf("Activamos continuo a gráfica\n");
		ActivaContinuoGrafica(EstadoVisual.AjusteContinuo);
		PonCursorNormal();
		
		// Indica un ajuste al continuo finalizado y quita los cambios
		EstadoTrabajo.Continuo = true;
		EstadoTrabajo.AdjustContinuumCambios = false;
		EstadoBotonesContinuo();

		// Una vez terminado el ajuste, se activa la zona de calibracion y
		// se rellenan los datos, aunque ya exista una calibracion cargada
		printf("Activamos calibración\n");
		ActivaCalibracion();
		printf("Reconstruimos lista de itnervalos de calibracón\n");
		ReconstruyeListaIntervCalibracion();
		
		// Si ya hay calibracion cargada entonces pone disponible la zona
		// de analisis
		printf("Activamos el análisis si se puede\n");
		if (EstadoTrabajo.Calibracion) 
			{
			ActivaAnalisis();
			ReconstruyeListaIntervAnalisis();
			}
		
		if (EstadoVisual.AjusteContinuo)	PintaGrafica(0);
		}//del if(error==0)
	else 	
		{
		//NOTA: Estas llamadas son las que según avanzan las iteraciones cada vez son más lentas.
		XmUpdateDisplay(main_w);
		TimerAjustando = XtAppAddTimeOut(app_context, (unsigned long) 10, TutelandoAjuste, NULL);
		}
	}

void pulsa_show_continuo(Widget widget, XtPointer client_data, XtPointer call_data) {
   XmToggleButtonCallbackStruct *state = (XmToggleButtonCallbackStruct *) call_data;

	switch ((int) client_data) 
		{
		case 1://Mostrar los canales que forman parte del continuo
			EstadoVisual.CanalContinuo = state->set;
			ActivaContinuoChGrafica(state->set);
			break;
		case 2://Mostrar los canales que forman parte de algún pico
			EstadoVisual.CanalPicos = state->set;
			ActivaPicosChGrafica(state->set);
			break;
		case 3://Mostrar el continuo ajustado
			EstadoVisual.AjusteContinuo = state->set;
			ActivaContinuoGrafica(state->set);
			break;
		case 4://Mostrar los canales que no son continuo ni picos
			EstadoVisual.CanalNormal = state->set;
			ActivaDatosGrafica(!state->set);
			break;
		}
	
//	if ((EstadoTrabajo.Continuo)||(EstadoTrabajo.PreContinuo))
		PintaGrafica(0);
	}


void pulsa_lista_continuo(Widget widget, XtPointer client_data, XtPointer call_data) {
   XmListCallbackStruct *cbs = (XmListCallbackStruct *) call_data;
	
	if (XmListPosSelected(lista_continuo, cbs->item_position)) {
		// saca los datos de esa posicion en las ventanas de datos
		NodoContinuo *nodo = DevuelveNodo(listaContinuo, cbs->item_position);
		if (nodo != NULL) {
			char result1[]="xxxxxxxx";
			sprintf(result1, "%d", nodo->limInferior);
			XmTextSetString(cn_min_continuo, result1);
			sprintf(result1, "%d", nodo->limSuperior);
			XmTextSetString(cn_max_continuo, result1);
			if (nodo->cofo == -1.0) sprintf(result1, "%3.1f", nodo->cofo);
			else sprintf(result1, "%.2f", nodo->cofo);
			XmTextSetString(cofo_continuo, result1);
			if (nodo->copi == -1.0) sprintf(result1, "%3.1f", nodo->copi);
			else sprintf(result1, "%.2f", nodo->copi);
			XmTextSetString(copi_continuo, result1);
			if (nodo->manphe == -1.0) sprintf(result1, "%3.1f", nodo->manphe);
			else sprintf(result1, "%.2f", nodo->manphe);
			XmTextSetString(manphe_continuo, result1);
		}
	}
}

void EstadoBotonesContinuo() 
	{
	XmString cadena;

	ActualizaDatosContinuo();
	printf("Volvemos en estado botones continuo\n");
	// debido a un problema desconocido de actualizacion de presentacion
	// fuerza un cambio en la página para que se cambie correctamente el
	// tamaño de los botones
	XtUnmanageChild(bt_form_continuo);


	// pone el estado del boton de busqueda
	if (EstadoTrabajo.SearchContinuumCambios) 
		{
		XtSetSensitive (bt_ajusta_continuo, false);
		XtSetSensitive (bt_busca_continuo, true);		
		}
	else 
		{
		XtSetSensitive (bt_busca_continuo, false);
		if (EstadoTrabajo.AdjustContinuumCambios)	XtSetSensitive (bt_ajusta_continuo, true);
		else 										XtSetSensitive (bt_ajusta_continuo, false);
		}

	// pone el estado del boton de ajuste
	if (AjustandoContinuo) 
		{
		XtSetSensitive (entrada_frame_continuo, false);
		XtSetSensitive (bt_busca_continuo, false);
	   	cadena = XmStringCreateLocalized ("Cancel");
		XtVaSetValues (bt_ajusta_continuo, XmNlabelString, cadena, NULL);
		XmStringFree(cadena);
		}
	else 
		{
		XtSetSensitive (entrada_frame_continuo, true);
		cadena = XmStringCreateLocalized ("Fit");
		XtVaSetValues (bt_ajusta_continuo, XmNlabelString, cadena, NULL);
		XmStringFree(cadena);
		}
	
//	XtVaSetValues (lista_continuo, XmNvisibleItemCount, 9, NULL);

	// debido a un problema desconocido de actualizacion de presentacion
	// fuerza un cambio en la página para que se cambie correctamente el
	// tamaño de los botones
	XtManageChild(bt_form_continuo);
	printf("Termina estado botones continuo\n");
}


/******************************************************************************
 ******
 ******                       ADMINISTRACION LISTAS DEL CONTINUO
 ******
 ******************************************************************************/

void ReconstruyeListaContinuo() {
	tPrecalibracion datosPre;
	char texto_canales[80];
	XmString t;

	XmListDeleteAllItems(lista_continuo);
	for (int i = 1; i <= listaContinuo.numElementos; i++) {
		// coge cada elemento en la lista del continuo y lo
		// convierte en una entrada en el widget lista del continuo
		NodoContinuo *nodo = DevuelveNodo(listaContinuo, i);
		if (nodo != NULL) {
			sprintf(texto_canales, "[%d, %d]: ", nodo->limInferior, nodo->limSuperior);
			if (nodo->cofo >= 0.0) sprintf(texto_canales, "%s %.1f cf", texto_canales, nodo->cofo);
			if (nodo->copi >= 0.0) sprintf(texto_canales, "%s %.1f cp", texto_canales, nodo->copi);
			if (nodo->manphe >= 0.0) sprintf(texto_canales, "%s %.1f mp", texto_canales, nodo->manphe);
			t = XmStringCreateLocalized (texto_canales);
			XmListAddItemUnselected(lista_continuo, t, 0);
		   XmStringFree (t);
		}
	}
}

tListaContinuo* DevuelveListaContinuo() {
	return &listaContinuo;
}

void CopiaListaContinuo(tListaContinuo lista) {
	borraLista(listaContinuo);
	listaContinuo = lista;
}

//**************** SELECCIONAR TEXTO **********************
/*	Pone el texto de un cuadro de diálogo como seleccionado
*/
void seleccionarTexto(Widget widget, XtPointer client_data, XtPointer call_data)
	{
	XmTextFieldSetSelection(widget, 0, 100, 0);
	}

/*******************************************************************************
 ******
 ******                             VISUALIZACIONES
 ******
 ******************************************************************************/

void EstableceShowsContinuo(const tEstadoVisual Estado) 
	{
	XmToggleButtonSetState(show_channel_continuo, Estado.CanalContinuo, false);
	XmToggleButtonSetState(show_peak_continuo, Estado.CanalPicos, false);
	XmToggleButtonSetState(show_adjust_continuo, Estado.AjusteContinuo, false);
	// Activa o desactiva las graficas segun los valores
	ActivaDatosGrafica(!Estado.CanalNormal);
	ActivaContinuoChGrafica(Estado.CanalContinuo);
	ActivaPicosChGrafica(Estado.CanalPicos);
	ActivaContinuoGrafica(Estado.AjusteContinuo);
	}
