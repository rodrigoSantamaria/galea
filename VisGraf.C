#include <stdio.h>

#include <math.h>



#include <string>

#include <iostream>

using namespace std;



#include "constDef.h"

#include "fuentes.h"

#include "globales.h"

#include "Graficas.h"

#include "math1.h"

#include "ficheros.h"



#include "ajustes.h"



#include "trabajoEst.h"

#include "VisCalib.h"

#include "VisConti.h"

#include "VisGraf.h"

#include <Xm/Form.h>

#include <Xm/DrawingA.h>

#include <Xm/ScrollBar.h>

#include <Xm/PushB.h>

#include <Xm/ToggleBG.h>

#include <Xm/ToggleB.h>

#include <Xm/RowColumn.h>

#include <Xm/Label.h>

#include <Xm/LabelG.h>

#include <Xm/Frame.h>

#include <Xm/Scale.h>

#include <Xm/MwmUtil.h>





// Datos de las graficas

cGrafica Graficas;



// Widget general

Widget fondo_grafica;

Window ventana_padre;



// Widgets para las graficas

Widget tapiz_grafica;

	// el tapiz de la grafica se divide en 6 cuerpos:

	//   1 -> herramientas verticales

	//   2 -> grafica en si

	//   3 -> scroll vertical

	//   4 -> cuadrado

	//   5 -> herramientas horizontales

	//   6 -> cuadrado

	//   -------

	//   |1|2|3|

	//   -------

	//   |4|5|6|

	//   -------

Widget form1_grafica, form2_grafica, form3_grafica,

		 form4_grafica, form5_grafica, form6_grafica;

Widget grafica, v_grafica, h_grafica;

Widget v_regla_grafica, h_regla_grafica;

Pixmap pix_h_regla_grafica, pix_v_regla_grafica;

Dimension ancho_pix_grafica, alto_pix_grafica;



// Widgets para las opciones

Widget opciones_grafica, resto_opciones_grafica;

Widget frame_opc_graf, label_frame_opc_graf, escala_opc_graf;

Widget lineal_opc_graf, raiz_opc_graf, log_opc_graf;

Widget row_datos_graf;

Widget row_fichero_datos_graf;

Widget auto_ajuste_graf;



// Widgets para las barras de zoom

Widget zoom_x_graf, zoom_y_graf;

Widget label_zoom_x_graf, label_zoom_y_graf;

Widget label_zoom_x, label_zoom_y;



Widget label_total_counts, counts_showed;



// Datos

unsigned char escala_graf = 1; /*	1 escala lineal

					2 escala raiz cuadrada

					3 escala logaritmica */

tEstadoVisual EstadoVisual;

int CanalCursor; // canal en el que se encuentra el cursor





// Datos sobre las barras de desplazamiento

const int h_grafica_ancho_total = 102400;

const int h_grafica_ancho_paso = 100;

const int v_grafica_alto_total = 10240;

const int v_grafica_alto_paso = 10;



// Controladores graficos

GC gcZoomX, gcZoomY, RubberZoomX, RubberGrafica;

XGCValues gcv;



// Datos tecnicos sobre la pantalla

double pixDensX, pixDensY; // densidad de pixels por milimetro





// ****

// ** FUNCIONES

// ****



	Widget Crea_VisorGrafica(Widget);

	/* Crea los widgets necesarios para la grafica */

	void ActivaGrafica ();

	/* Activa a la entrada de eventos el Widget general de la grafica */

	void DesactivaGrafica ();

	/* Desactiva a la entrada de eventos el widget general de la grafica */

	void ReseteaGraficas();

	/* Borra las graficas actuales y vuelve a generar las graficas por

		defecto, pero vacias */

	void PintaGrafica(int);

	/* Actualiza la representacion del tapiz y de las dos reglas */

	void BorraGrafica();

	/* Borra el tapiz de dibujo */

	int GetChannel(int pos_x);

	/* Devuelve el canal mas cercano a la posicion en pixels segnalada en el

		tapiz */

	void PintaReglaX ();

	/* Dibuja las marcas y los numeros de canales en la regla horizontal */

	void PintaReglaY ();

	/* Dibuja las marcas y las cuentas de canales en la regla vertical */

	void AjustaDatosGrafica ();//NOTA: Parece que no se usa!

	void CambiaPosicionCursor ();

	/* Cambia la posicion del cursor */

	void AutoAjusteY ();

	/* Calcula el zoom Y para la zona de datos que se muestra en el tapiz */

	void MarcaZoom (int, int);

	/* Calcula el zoom X necesario para poder presentar en pantalla los

		valores del intervalo indicado */

	int CopiaDatosAGrafica(unsigned int* datos, unsigned short numDatos);

	/* Pasa los datos indicados a la grafica de datos normales */



// Visualizaciones

void EstableceAutoEscalaY(bool);

void EstableceEscalaY(int);



// Callbacks

void redraw (Widget, XtPointer, XtPointer);

void Pon_Escala (Widget, XtPointer, XtPointer);

void Pon_Zoom (Widget, XtPointer, XtPointer);

void Desplaza_Grafica (Widget, XtPointer, XtPointer);

void DibujaCursor(Widget, XtPointer, XtPointer);

void CursorZoom(Widget, XtPointer, XtPointer);

void CambiaAutoEscala(Widget, XtPointer, XtPointer);




// ****

// ** VENTANA DATOS

// ****



		// Widgets

	Widget ventana_datos_graf, ventana_fichero_datos_graf, dialog_datos_graf, dialog_fichero_datos_graf;

	Widget frame_datos_graf, label_frame_datos_graf;

	Widget label_file_datos_graf;

	Widget label_canal_datos_graf, label_cuentas_datos_graf;

	Widget label_energia_datos_graf, label_FWHG_datos_graf;

	//

	Widget rowcol;

	Widget file_rowcol;

	Widget lcanalValor, lcuentasValor, lenergiaValor, lsigmaValor, ltauValor;

	

	//

	

	Widget lb_tau_datos_graf;

		// Datos

	char titulo_datos_graf[]="Data"; // titulo de la ventana de datos

		// Funciones

	void CreaVentanaDatos();

	void CreaVentanaFicheroDatos();

	void ActivaEnergiaDatos();

	void ActivaFormaDatos();

	void DesactivaEnergiaDatos();

	void DesactivaFormaDatos();

	void EstableceVentanaDatos(bool);

	void EstableceVentanaFicheroDatos(bool);

	void PonPosicionCursor();

	/* Actualiza la ventana de datos con los datos que segnala el cursor */

		// Callbacks

	void PonVentanaDatos(Widget, XtPointer, XtPointer);

	void PonVentanaFicheroDatos(Widget, XtPointer, XtPointer);

	void QuitaVentanaDatos(Widget, XtPointer, XtPointer);

	void QuitaVentanaFicheroDatos(Widget, XtPointer, XtPointer);

	void EscribeFicheroDatos();



// **********





		/************************

		 *   CREACION WIDGETS   *

		 ************************/



//***************** CREA VISOR GRÁFICA **********************

Widget Crea_VisorGrafica(Widget parent) 

	{

    Arg		args[7];

    Cardinal	i = 0;

    XmString cadena;

	String	translations = "<Btn1Motion>: DrawingAreaInput() ManagerGadgetButtonMotion()\n<Btn1Up>:		DrawingAreaInput() ManagerGadgetActivate()\n<Btn1Down>:	DrawingAreaInput() ManagerGadgetArm()";

	

	// Inicializa los datos

	CanalCursor = 0;



	Screen *screen = XtScreen(parent);

	pixDensX = WidthOfScreen(screen)/(double)WidthMMOfScreen(screen);

	pixDensY = HeightOfScreen(screen)/(double)HeightMMOfScreen(screen);

	//printf("Altura de la pantalla: %d\tAltura MM: %d\n", HeightOfScreen(screen), HeightMMOfScreen(screen));



   // Crea el form fondo para todo este "Widget"

   i = 0;

   fondo_grafica = XmCreateForm (parent, "fondo_grafica", args, i);





   /****************

	 *   OPCIONES   *

	 ****************/



   // Crea las opciones asocidadas a la grafica

   i = 0;

   opciones_grafica = XmCreateForm (fondo_grafica, "opciones_grafica", args, i);

   XtVaSetValues (opciones_grafica,

			XmNtopAttachment, XmATTACH_FORM,

			XmNrightAttachment, XmATTACH_FORM,

			XmNbottomAttachment, XmATTACH_FORM,

			NULL);



   // Opciones de escala

   i = 0;

   frame_opc_graf = XmCreateFrame (opciones_grafica, "frame_opc_graf", args, i);



   cadena = XmStringCreateLocalized ("Scale Y");

   i = 0;

   XtSetArg (args[i], XmNchildType, XmFRAME_TITLE_CHILD); i++;

   XtSetArg (args[i], XmNchildVerticalAlignment, XmALIGNMENT_CENTER); i++;

   XtSetArg (args[i], XmNlabelString, cadena); i++;

   XtSetArg (args[i], XmNfontList, lista_fuentes); i++;

   label_frame_opc_graf = XmCreateLabelGadget (frame_opc_graf, "label_frame_opc_graf", args, i);

   XmStringFree (cadena);



   i = 0;

   escala_opc_graf = XmCreateRadioBox(frame_opc_graf, "escala_opc_graf", args, i);



   cadena = XmStringCreateLocalized ("Linear");

   i = 0;

   XtSetArg (args[i], XmNlabelString, cadena); i++;

   XtSetArg (args[i], XmNset, true); i++;

   XtSetArg (args[i], XmNfontList, lista_fuentes); i++;

   lineal_opc_graf = XmCreateToggleButtonGadget (escala_opc_graf, "lineal_opc_graf", args, i);

   XmStringFree (cadena);

   XtAddCallback (lineal_opc_graf, XmNvalueChangedCallback, Pon_Escala, (XtPointer) 1);



   cadena = XmStringCreateLocalized ("Root Square");

   i = 0;

   XtSetArg (args[i], XmNlabelString, cadena); i++;

   XtSetArg (args[i], XmNfontList, lista_fuentes); i++;

   raiz_opc_graf = XmCreateToggleButtonGadget (escala_opc_graf, "raiz_opc_graf", args, i);

   XmStringFree (cadena);

   XtAddCallback (raiz_opc_graf, XmNvalueChangedCallback, Pon_Escala, (XtPointer) 2);



   cadena = XmStringCreateLocalized ("Logarithmic");

   i = 0;

   XtSetArg (args[i], XmNlabelString, cadena); i++;

   XtSetArg (args[i], XmNfontList, lista_fuentes); i++;

   log_opc_graf = XmCreateToggleButtonGadget (escala_opc_graf, "log_opc_graf", args, i);

   XmStringFree (cadena);

   XtAddCallback (log_opc_graf, XmNvalueChangedCallback, Pon_Escala, (XtPointer) 3);



		

 	i = 0;

   XtSetArg (args[i], XmNpacking, XmPACK_COLUMN); i++;

   resto_opciones_grafica = XmCreateRowColumn (opciones_grafica, "resto_opciones_grafica", args, i);

	// Toggle button para la ventana de datos

   cadena = XmStringCreateLocalized ("Data");

   i = 0;

   XtSetArg (args[i], XmNset, false); i++;

   XtSetArg (args[i], XmNlabelString, cadena); i++;

   XtSetArg (args[i], XmNfontList, lista_fuentes); i++;

	ventana_datos_graf = XmCreateToggleButton (resto_opciones_grafica, "ventana_datos_graf", args, i);

   XmStringFree (cadena);


	// Toggle button para el fichero de datos

   cadena = XmStringCreateLocalized ("Data file");

   i = 0;

   XtSetArg (args[i], XmNset, false); i++;

   XtSetArg (args[i], XmNlabelString, cadena); i++;

   XtSetArg (args[i], XmNfontList, lista_fuentes); i++;

	ventana_fichero_datos_graf = XmCreateToggleButton (resto_opciones_grafica, "ventana_fichero_datos_graf", args, i);

   XmStringFree (cadena);

	// Toggle button para el auto ajuste de la escala Y

   cadena = XmStringCreateLocalized ("Auto Scale Y");

   i = 0;

   XtSetArg (args[i], XmNset, false); i++;

   XtSetArg (args[i], XmNlabelString, cadena); i++;

   XtSetArg (args[i], XmNfontList, lista_fuentes); i++;

	auto_ajuste_graf = XmCreateToggleButton (resto_opciones_grafica, "auto_ajuste_graf", args, i);

   XmStringFree (cadena);



   cadena = XmStringCreate("Counts showed:", "BoldNormal");

   i = 0;

   XtSetArg (args[i], XmNchildType, XmFRAME_TITLE_CHILD); i++;

   XtSetArg (args[i], XmNchildVerticalAlignment, XmALIGNMENT_CENTER); i++;

   XtSetArg (args[i], XmNlabelString, cadena); i++;

   XtSetArg (args[i], XmNfontList, lista_fuentes); i++;

   counts_showed = XmCreateLabelGadget (resto_opciones_grafica, "counts_showed", args, i);

   XmStringFree (cadena);



	XtVaSetValues (counts_showed,

		XmNbottomAttachment, XmATTACH_FORM,

		XmNtopAttachment, XmATTACH_FORM, 

		XmNrightAttachment, XmATTACH_FORM,

		XmNleftAttachment, XmATTACH_FORM, XmNleftOffset, 15,

		NULL);

   

   //Etiqueta con el total de cuentas seleccionadas

   string cad("0");

   cadena = XmStringCreateLocalized ((char *)cad.c_str());

   i = 0;

   XtSetArg (args[i], XmNchildType, XmFRAME_TITLE_CHILD); i++;

   XtSetArg (args[i], XmNchildVerticalAlignment, XmALIGNMENT_CENTER); i++;

   XtSetArg (args[i], XmNlabelString, cadena); i++;

   XtSetArg (args[i], XmNfontList, lista_fuentes); i++;

   label_total_counts = XmCreateLabelGadget (resto_opciones_grafica, "label_total_counts", args, i);

   XmStringFree (cadena);

	

   // Coloca los Widgets

   XtVaSetValues (resto_opciones_grafica,

			XmNtopAttachment, XmATTACH_WIDGET, XmNtopWidget, frame_opc_graf,

			XmNleftAttachment, XmATTACH_FORM, XmNleftOffset, 2,

			XmNrightAttachment, XmATTACH_FORM, XmNrightOffset, 2,

			XmNbottomAttachment, XmATTACH_FORM, XmNbottomOffset, 2,

			NULL);

   XtVaSetValues (frame_opc_graf,

			XmNtopAttachment, XmATTACH_FORM, XmNtopOffset, 5,

			XmNleftAttachment, XmATTACH_FORM, XmNleftOffset, 2,

			XmNrightAttachment, XmATTACH_FORM, XmNrightOffset, 2,

			NULL);



   XtAddCallback (auto_ajuste_graf, XmNvalueChangedCallback, CambiaAutoEscala, NULL);

	XtManageChild (auto_ajuste_graf);

	XtManageChild (counts_showed);

	XtManageChild (label_total_counts);


   XtAddCallback (ventana_datos_graf, XmNvalueChangedCallback, PonVentanaDatos, NULL);

   XtAddCallback (ventana_fichero_datos_graf, XmNvalueChangedCallback, PonVentanaFicheroDatos, NULL);
   

	XtManageChild (ventana_datos_graf);

   
   XtManageChild (ventana_fichero_datos_graf);


   XtManageChild (lineal_opc_graf);

   XtManageChild (raiz_opc_graf);

   XtManageChild (log_opc_graf);

   XtManageChild (escala_opc_graf);

   XtManageChild (label_frame_opc_graf);

   XtManageChild (frame_opc_graf);	

   XtManageChild (opciones_grafica);

   XtManageChild (resto_opciones_grafica);




   /*************

	 *   VISOR   *

	 *************/

	 

   // Crea la ventana del visor de graficas

   i = 0;

   tapiz_grafica = XmCreateForm (fondo_grafica, "grafica", args, i);

   

   XtVaSetValues (tapiz_grafica,

      XmNbottomAttachment, XmATTACH_FORM,

		XmNleftAttachment, XmATTACH_FORM,

		XmNtopAttachment, XmATTACH_FORM,

		XmNrightAttachment, XmATTACH_WIDGET, XmNrightWidget, opciones_grafica,

		NULL);

		

	// Crea los 6 formularios que contienen las herramientas de la grafica

	//   -------

	//   |1|2|3|

	//   -------

	//   |4|5|6|

	//   -------

	i = 0;

	form1_grafica = XmCreateForm (tapiz_grafica, "form1_grafica", args, i);

	i = 0;

	form2_grafica = XmCreateForm (tapiz_grafica, "form2_grafica", args, i);

	i = 0;

	form3_grafica = XmCreateForm (tapiz_grafica, "form3_grafica", args, i);

	i = 0;

	XtSetArg (args[i], XmNwidth, 90); i++;

	XtSetArg (args[i], XmNheight, 80); i++;

	form4_grafica = XmCreateForm (tapiz_grafica, "form4_grafica", args, i);

	i = 0;

	form5_grafica = XmCreateForm (tapiz_grafica, "form5_grafica", args, i);

	i = 0;

	XtSetArg (args[i], XmNwidth, 15); i++;

	form6_grafica = XmCreateForm (tapiz_grafica, "form6_grafica", args, i);



	XtVaSetValues (form1_grafica,

   	XmNleftAttachment, XmATTACH_FORM,

		XmNtopAttachment, XmATTACH_FORM,

		XmNbottomAttachment, XmATTACH_WIDGET, XmNbottomWidget, form4_grafica,

		XmNrightAttachment, XmATTACH_OPPOSITE_WIDGET, XmNrightWidget, form4_grafica,		

		NULL);

	XtVaSetValues (form2_grafica,

   	XmNleftAttachment, XmATTACH_WIDGET, XmNleftWidget, form1_grafica,

		XmNtopAttachment, XmATTACH_FORM,

		XmNbottomAttachment, XmATTACH_WIDGET, XmNbottomWidget, form5_grafica,

		XmNrightAttachment, XmATTACH_WIDGET, XmNrightWidget, form3_grafica,		

		NULL);

	XtVaSetValues (form3_grafica,

		XmNtopAttachment, XmATTACH_FORM,

		XmNbottomAttachment, XmATTACH_WIDGET, XmNbottomWidget, form6_grafica,

		XmNrightAttachment, XmATTACH_FORM,		

		XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET, XmNleftWidget, form6_grafica,		

		NULL);

	XtVaSetValues (form4_grafica,

		XmNbottomAttachment, XmATTACH_FORM,

		XmNleftAttachment, XmATTACH_FORM,		

		NULL);

	XtVaSetValues (form5_grafica,

   	XmNleftAttachment, XmATTACH_WIDGET, XmNleftWidget, form4_grafica,

		XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET, XmNtopWidget, form4_grafica,

		XmNbottomAttachment, XmATTACH_FORM,

		XmNrightAttachment, XmATTACH_WIDGET, XmNrightWidget, form6_grafica,		

		NULL);

	XtVaSetValues (form6_grafica,

		XmNbottomAttachment, XmATTACH_FORM,

		XmNrightAttachment, XmATTACH_FORM,		

		XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET, XmNtopWidget, form4_grafica,

		NULL);



	// Hace el tapiz para la grafica

   i = 0;

	XtSetArg (args[i], XmNtranslations, XtParseTranslationTable (translations)); i++;

   grafica = XmCreateDrawingArea (form2_grafica, "grafica", args, i);

   // Crea las scroll bars

   i = 0;

   XtSetArg (args[i], XmNorientation, XmVERTICAL); i++;   

   XtSetArg (args[i], XmNmaximum, v_grafica_alto_total); i++;

   XtSetArg (args[i], XmNminimum, 1); i++;

   XtSetArg (args[i], XmNsliderSize, v_grafica_alto_total-1); i++;

	XtSetArg (args[i], XmNincrement, v_grafica_alto_paso);

   XtSetArg (args[i], XmNvalue, 1); i++;  

   XtSetArg (args[i], XmNprocessingDirection, XmMAX_ON_TOP); i++; 

   v_grafica = XmCreateScrollBar (form3_grafica, "v_grafica", args, i);   

   i = 0;

   XtSetArg (args[i], XmNorientation, XmHORIZONTAL); i++;

   XtSetArg (args[i], XmNmaximum, h_grafica_ancho_total); i++;

   XtSetArg (args[i], XmNminimum, 1); i++;

   XtSetArg (args[i], XmNsliderSize, h_grafica_ancho_total-1); i++;

	XtSetArg (args[i], XmNincrement, h_grafica_ancho_paso);

   XtSetArg (args[i], XmNvalue, 1); i++;   

   h_grafica = XmCreateScrollBar (form5_grafica, "h_grafica", args, i);

   // Crea los zooms

   i = 0;

   XtSetArg (args[i], XmNmaximum, 100); i++;

   XtSetArg (args[i], XmNminimum, 0); i++;

   XtSetArg (args[i], XmNvalue, 0); i++;

   XtSetArg (args[i], XmNscaleWidth, 20); i++;

   zoom_y_graf = XmCreateScale (form1_grafica, "zoom_y_graf", args, i);   

   i = 0;

   XtSetArg (args[i], XmNmaximum, 100); i++;

   XtSetArg (args[i], XmNminimum, 0); i++;

   XtSetArg (args[i], XmNvalue, 0); i++;

   XtSetArg (args[i], XmNscaleHeight, 20); i++;

   XtSetArg (args[i], XmNorientation, XmHORIZONTAL); i++;

   zoom_x_graf = XmCreateScale (form5_grafica, "zoom_x_graf", args, i);   

   // crea las etiquetas de informacion sobre el zoom

 //  printf("Zoom Y\n");

   i = 0;

   cadena = XmStringCreate ("Zoom Y:", "BoldNormal");

   XtSetArg (args[i], XmNlabelString, cadena); i++;

   XtSetArg (args[i], XmNfontList, lista_fuentes); i++;

   label_zoom_y = XmCreateLabelGadget (form4_grafica, "label_zoom_y", args, i);

   XmStringFree (cadena);

//	printf("Zoom X\n");

   i = 0;

   cadena = XmStringCreate ("Zoom X:", "BoldNormal");

   XtSetArg (args[i], XmNlabelString, cadena); i++;

   XtSetArg (args[i], XmNfontList, lista_fuentes); i++;

   label_zoom_x = XmCreateLabelGadget (form4_grafica, "label_zoom_x", args, i);

   XmStringFree (cadena);

   i = 0;

   cadena = XmStringCreateLocalized ("1.00");

   XtSetArg (args[i], XmNlabelString, cadena); i++;

   XtSetArg (args[i], XmNrecomputeSize, false); i++;

   XtSetArg (args[i], XmNfontList, lista_fuentes); i++;

   label_zoom_y_graf = XmCreateLabel (form4_grafica, "label_zoom_y_graf", args, i);

   label_zoom_x_graf = XmCreateLabel (form4_grafica, "label_zoom_x_graf", args, i);

   XmStringFree (cadena);

   

   // Hace las reglas

   i = 0;

   XtSetArg (args[i], XmNwidth, 70); i++;

   v_regla_grafica = XmCreateDrawingArea (form1_grafica, "v_regla_grafica", args, i);

   i = 0;

   XtSetArg (args[i], XmNheight, 40); i++;

   XtSetArg (args[i], XmNtranslations, XtParseTranslationTable (translations)); i++;

   h_regla_grafica = XmCreateDrawingArea (form5_grafica, "h_regla_grafica", args, i);



   // Coloca los objetos en los formularios

	//   ***  formulario 1  ***

	XtVaSetValues (zoom_y_graf,

   	XmNleftAttachment, XmATTACH_FORM, XmNleftOffset, 1,

		XmNtopAttachment, XmATTACH_FORM, XmNtopOffset, 1,

		XmNbottomAttachment, XmATTACH_FORM,

		NULL);

	XtVaSetValues (v_regla_grafica,

   	XmNleftAttachment, XmATTACH_WIDGET, XmNleftWidget, zoom_y_graf,

		XmNtopAttachment, XmATTACH_FORM, XmNtopOffset, 1,

		XmNbottomAttachment, XmATTACH_FORM,

		XmNrightAttachment, XmATTACH_FORM,

		NULL);

	//   ***  formulario 2  ***

	XtVaSetValues (grafica,

		XmNtopAttachment,  XmATTACH_FORM, XmNtopOffset, 1,

		XmNleftAttachment, XmATTACH_FORM,

		XmNrightAttachment, XmATTACH_FORM, XmNrightOffset, 1,

		XmNbottomAttachment, XmATTACH_FORM,

		NULL);

	//   ***  formulario 3  ***

	XtVaSetValues (v_grafica,

		XmNtopAttachment, XmATTACH_FORM, XmNtopOffset, 1,

		XmNbottomAttachment, XmATTACH_FORM,

		XmNleftAttachment, XmATTACH_FORM,

		NULL);

	//   ***  formulario 4  ***

	XtVaSetValues (label_zoom_x,

   	XmNleftAttachment, XmATTACH_FORM,

		XmNbottomAttachment, XmATTACH_FORM, XmNbottomOffset, 4,

		NULL);

	XtVaSetValues (label_zoom_x_graf,

   	XmNrightAttachment, XmATTACH_FORM,

   	XmNleftAttachment, XmATTACH_WIDGET, XmNleftWidget, label_zoom_x,

		XmNbottomAttachment, XmATTACH_FORM, XmNbottomOffset, 4,

		NULL);

	XtVaSetValues (label_zoom_y,

   	XmNleftAttachment, XmATTACH_FORM,

		XmNtopAttachment, XmATTACH_FORM, XmNtopOffset, 4,

		NULL);

	XtVaSetValues (label_zoom_y_graf,

   	XmNleftAttachment, XmATTACH_WIDGET, XmNleftWidget, label_zoom_y,

   	XmNrightAttachment, XmATTACH_FORM,

		XmNtopAttachment, XmATTACH_FORM, XmNtopOffset, 4,

		NULL);

	//   ***  formulario 5  ***

   XtVaSetValues (zoom_x_graf,

      XmNbottomAttachment, XmATTACH_FORM, XmNbottomOffset, 1,

		XmNleftAttachment, XmATTACH_FORM,

		XmNrightAttachment, XmATTACH_FORM,

		NULL);	

   XtVaSetValues (h_grafica,

      XmNbottomAttachment, XmATTACH_WIDGET, XmNbottomWidget, zoom_x_graf,

		XmNleftAttachment, XmATTACH_FORM,

		XmNrightAttachment, XmATTACH_FORM,

		NULL);	

   XtVaSetValues (h_regla_grafica,

      XmNbottomAttachment, XmATTACH_WIDGET, XmNbottomWidget, h_grafica,

		XmNleftAttachment, XmATTACH_FORM,

		XmNrightAttachment, XmATTACH_FORM,

		XmNtopAttachment, XmATTACH_FORM,

		NULL);	





   XtAddCallback (v_grafica, XmNvalueChangedCallback, Desplaza_Grafica, NULL);

   XtAddCallback (h_grafica, XmNvalueChangedCallback, Desplaza_Grafica, NULL);

   XtAddCallback (v_grafica, XmNdragCallback, Desplaza_Grafica, NULL);

   XtAddCallback (h_grafica, XmNdragCallback, Desplaza_Grafica, NULL);

   XtManageChild (v_grafica);

   XtManageChild (h_grafica);

   XtAddCallback (zoom_x_graf, XmNvalueChangedCallback, Pon_Zoom, NULL);

   XtAddCallback (zoom_y_graf, XmNvalueChangedCallback, Pon_Zoom, NULL);

   XtAddCallback (zoom_x_graf, XmNdragCallback, Pon_Zoom, NULL);

   XtAddCallback (zoom_y_graf, XmNdragCallback, Pon_Zoom, NULL);

   XtManageChild (zoom_y_graf);

   XtManageChild (zoom_x_graf);

   XtManageChild (label_zoom_x);

   XtManageChild (label_zoom_y);

   XtManageChild (label_zoom_x_graf);

   XtManageChild (label_zoom_y_graf);

   XtAddCallback (grafica, XmNexposeCallback, redraw, NULL);

   XtAddCallback (grafica, XmNresizeCallback, redraw, NULL);

   XtAddCallback (grafica, XmNinputCallback, DibujaCursor, NULL);

   XtManageChild (grafica);

   XtAddCallback (v_regla_grafica, XmNexposeCallback, redraw, NULL);

   XtManageChild (v_regla_grafica);

   XtAddCallback (h_regla_grafica, XmNexposeCallback, redraw, NULL);

   XtAddCallback (h_regla_grafica, XmNinputCallback, CursorZoom, NULL);

   XtManageChild (h_regla_grafica);

	

	XtManageChild(form1_grafica);   

	XtManageChild(form2_grafica);   

	XtManageChild(form3_grafica);   

	XtManageChild(form4_grafica);   

	XtManageChild(form5_grafica);   

	XtManageChild(form6_grafica);   



   // pasa el control a Motif

   XtManageChild (tapiz_grafica);

   XtVaSetValues (fondo_grafica,

      XmNbottomAttachment, XmATTACH_FORM,

		XmNleftAttachment, XmATTACH_FORM,

		XmNtopAttachment, XmATTACH_FORM,

		XmNrightAttachment, XmATTACH_FORM,

		NULL);

   XtManageChild (fondo_grafica);



	// tapiz virtual para dibujar

	Graficas.Inicializa(grafica,

			Preferences.GetGrafFgColor().pixel, Preferences.GetGrafBgColor().pixel,

			Preferences.GetLimitBigPoint());

	Graficas.CreaCursor(Preferences.GetCursorUpColor().pixel, Preferences.GetCursorDownColor().pixel);



		//  controles para dibujar

	XGCValues gcv;



		// GC para zoom vertical

	gcv.foreground = Preferences.GetZoomFgColor().pixel;

	gcv.background = Preferences.GetZoomBgColor().pixel;

	gcZoomY = XCreateGC(dpy, RootWindowOfScreen(XtScreen(v_regla_grafica)), GCForeground|GCBackground, &gcv);

   	XtVaSetValues (v_regla_grafica, XmNbackground, gcv.background, NULL);	



		// GC para zoom horizontal

	gcZoomX = XCreateGC(dpy, RootWindowOfScreen(XtScreen(h_regla_grafica)), GCForeground|GCBackground, &gcv);

   	XtVaSetValues (h_regla_grafica, XmNbackground, gcv.background, NULL);	



		// GCs para tapiz de dibujo	

   	XtVaSetValues (grafica, XmNbackground, Preferences.GetGrafBgColor().pixel, NULL);

		

		// GC para cursor

/*	cursor.CreaGC(XtDisplay (grafica), grafica);*/



	gcv.function = GXxor; gcv.fill_style = FillSolid;

	gcv.foreground = Preferences.GetGrafBgColor().pixel;

	gcv.background = Preferences.GetGrafFgColor().pixel;

	gcv.foreground = gcv.foreground ^ gcv.background;

	RubberGrafica  = XCreateGC(XtDisplay (grafica), RootWindowOfScreen(XtScreen(grafica)), GCFunction|GCForeground|GCBackground|GCFillStyle, &gcv);

	gcv.foreground = Preferences.GetZoomFgColor().pixel;

	gcv.background = Preferences.GetZoomBgColor().pixel;

	gcv.foreground = gcv.foreground ^ gcv.background;

	RubberZoomX    = XCreateGC(XtDisplay (grafica), RootWindowOfScreen(XtScreen(h_regla_grafica)), GCFunction|GCForeground|GCBackground|GCFillStyle, &gcv);



	

	CreaVentanaDatos();

	CreaVentanaFicheroDatos();
	

	// inicializa las graficas por defecto

	ReseteaGraficas();



   return fondo_grafica;

}



/************************************************************************

 ***

 ***			              CONTROL DE GRAFICAS

 ***

 ************************************************************************/



void BorraCursor() 

	{

	Graficas.ShowTapiz(); // Borra el cursor reescribiendo el tapiz

	}



void DibujaCursor() 

	{

	BorraCursor();

	// Coge la ultima posicion del cursor y lo dibuja sobre la ventana

	if (Datos.numCanales > 0) Graficas.DibujaCursor((double)CanalCursor+1, (double)Datos.espectro[CanalCursor+1]);

	}	



void ReseteaGraficas() 

	{

	// elimina las graficas actuales

//	printf("Graficas.Resetea\n");

	Graficas.Resetea();

//	printf("Graficas.resetea terminado\n");

	// vuelve a crear las que hay por defecto

	// espectro normal

	Graficas.CreateNewGraphic(0, "Normal", 1);

	Graficas.SetApparience(Preferences.GetGrafFgColor().pixel,

				Preferences.GetGrafBgColor().pixel, Preferences.GetLineWidth(), 0);

	// espectro de canales del continuo

	Graficas.CreateNewGraphic(5, "Fondo", 2);

	Graficas.SetApparience(Preferences.GetChContColor().pixel,

				Preferences.GetGrafBgColor().pixel, Preferences.GetLineWidth(), 0);

	// espectro de canales de picos

	Graficas.CreateNewGraphic(5, "Picos", 3);

	Graficas.SetApparience(Preferences.GetChPeaksColor().pixel,

				Preferences.GetGrafBgColor().pixel, Preferences.GetLineWidth(), 0);

	// ajuste del continuo

	Graficas.CreateNewGraphic(10, "Continuo", 4);

	Graficas.SetApparience(Preferences.GetAdjustContColor().pixel,

				Preferences.GetGrafBgColor().pixel, Preferences.GetLineWidth(), 1);

	// Ajuste de los picos

	Graficas.CreateNewGraphic(-5, "Ajuste", 5);

	Graficas.SetApparience(Preferences.GetAdjustPeakColor().pixel,

				Preferences.GetGrafBgColor().pixel, Preferences.GetLineWidth(), 1);



	Graficas.CreateNewGraphic(10, "Ajuste Parcial", 6);

	Graficas.SetApparience(Preferences.GetAdjustSelectedPeakColor().pixel,

				Preferences.GetGrafBgColor().pixel, Preferences.GetLineWidth(), 1);

	//-------------- NUEVO ---------------

	Graficas.CreateNewGraphic(-6, "Genetic", 7);

	Graficas.SetApparience(Preferences.GetGeneticPeakColor().pixel,

				Preferences.GetGrafBgColor().pixel, Preferences.GetLineWidth(), 1);

	Graficas.CreateNewGraphic(-6, "Genetic", 8);

	Graficas.SetApparience(Preferences.GetGeneticPeakColor().pixel,

				Preferences.GetGrafBgColor().pixel, Preferences.GetLineWidth(), 1);

	Graficas.CreateNewGraphic(-6, "Genetic", 9);

	Graficas.SetApparience(Preferences.GetGeneticPeakColor().pixel,

				Preferences.GetGrafBgColor().pixel, Preferences.GetLineWidth(), 1);

	Graficas.CreateNewGraphic(-6, "Genetic", 10);

	Graficas.SetApparience(Preferences.GetGeneticPeakColor().pixel,

				Preferences.GetGrafBgColor().pixel, Preferences.GetLineWidth(), 1);

	Graficas.CreateNewGraphic(-6, "Genetic", 11);

	Graficas.SetApparience(Preferences.GetGeneticPeakColor().pixel,

				Preferences.GetGrafBgColor().pixel, Preferences.GetLineWidth(), 1);

	Graficas.CreateNewGraphic(-6, "Genetic", 12);

	Graficas.SetApparience(Preferences.GetGeneticPeakColor().pixel,

				Preferences.GetGrafBgColor().pixel, Preferences.GetLineWidth(), 1);

	Graficas.CreateNewGraphic(-6, "Genetic", 13);

	Graficas.SetApparience(Preferences.GetGeneticPeakColor().pixel,

				Preferences.GetGrafBgColor().pixel, Preferences.GetLineWidth(), 1);

	Graficas.CreateNewGraphic(-6, "Genetic", 14);

	Graficas.SetApparience(Preferences.GetGeneticPeakColor().pixel,

				Preferences.GetGrafBgColor().pixel, Preferences.GetLineWidth(), 1);

	Graficas.CreateNewGraphic(-6, "Genetic", 15);

	Graficas.SetApparience(Preferences.GetGeneticPeakColor().pixel,

				Preferences.GetGrafBgColor().pixel, Preferences.GetLineWidth(), 1);

	Graficas.CreateNewGraphic(-6, "Genetic", 16);

	Graficas.SetApparience(Preferences.GetGeneticPeakColor().pixel,

				Preferences.GetGrafBgColor().pixel, Preferences.GetLineWidth(), 1);

	Graficas.CreateNewGraphic(-6, "Genetic", 17);

	Graficas.SetApparience(Preferences.GetGeneticPeakColor().pixel,

				Preferences.GetGrafBgColor().pixel, Preferences.GetLineWidth(), 1);

	Graficas.CreateNewGraphic(-6, "Genetic", 18);

	Graficas.SetApparience(Preferences.GetGeneticPeakColor().pixel,

				Preferences.GetGrafBgColor().pixel, Preferences.GetLineWidth(), 1);

	Graficas.CreateNewGraphic(-6, "Genetic", 19);

	Graficas.SetApparience(Preferences.GetGeneticPeakColor().pixel,

				Preferences.GetGrafBgColor().pixel, Preferences.GetLineWidth(), 1);

	Graficas.CreateNewGraphic(-6, "Genetic", 20);

	Graficas.SetApparience(Preferences.GetGeneticPeakColor().pixel,

				Preferences.GetGrafBgColor().pixel, Preferences.GetLineWidth(), 1);

	Graficas.CreateNewGraphic(-6, "Genetic", 21);

	Graficas.SetApparience(Preferences.GetGeneticPeakColor().pixel,

				Preferences.GetGrafBgColor().pixel, Preferences.GetLineWidth(), 1);

	Graficas.CreateNewGraphic(-6, "Genetic", 22);

	Graficas.SetApparience(Preferences.GetGeneticPeakColor().pixel,

				Preferences.GetGrafBgColor().pixel, Preferences.GetLineWidth(), 1);

	Graficas.CreateNewGraphic(-6, "Genetic", 23);

	Graficas.SetApparience(Preferences.GetGeneticPeakColor().pixel,

				Preferences.GetGrafBgColor().pixel, Preferences.GetLineWidth(), 1);

	Graficas.CreateNewGraphic(-6, "Genetic", 24);

	Graficas.SetApparience(Preferences.GetGeneticPeakColor().pixel,

				Preferences.GetGrafBgColor().pixel, Preferences.GetLineWidth(), 1);

	Graficas.CreateNewGraphic(-6, "Genetic", 25);

	Graficas.SetApparience(Preferences.GetGeneticPeakColor().pixel,

				Preferences.GetGrafBgColor().pixel, Preferences.GetLineWidth(), 1);

	Graficas.CreateNewGraphic(-6, "Genetic", 26);

	Graficas.SetApparience(Preferences.GetGeneticPeakColor().pixel,

				Preferences.GetGrafBgColor().pixel, Preferences.GetLineWidth(), 1);

	Graficas.CreateNewGraphic(-6, "Genetic", 27);

	Graficas.SetApparience(Preferences.GetGeneticPeakColor().pixel,

				Preferences.GetGrafBgColor().pixel, Preferences.GetLineWidth(), 1);

	//------------------------------------

	Graficas.CreateNewGraphic(11, "Error", 28);

	Graficas.SetApparience(Preferences.GetGeneticPeakColor().pixel,

				Preferences.GetGrafBgColor().pixel, Preferences.GetLineWidth(), 1);

	Graficas.CreateNewGraphic(11, "FBD", 29);

	Graficas.SetApparience(Preferences.GetBackgroundColor().pixel,

				Preferences.GetGrafBgColor().pixel, Preferences.GetLineWidth(), 1);



	Graficas.CreateNewGraphic(12, "Ultima-Vacía", 30);

	Graficas.SetApparience(Preferences.GetBackgroundColor().pixel,

				Preferences.GetGrafBgColor().pixel, Preferences.GetLineWidth(), 1);

				

	// Cursor

	Graficas.CreaCursor(Preferences.GetCursorUpColor().pixel, Preferences.GetCursorDownColor().pixel);

	}



void CambiaPosicionCursor(unsigned int nuevoValor) 

	{

	CanalCursor = nuevoValor-1;

//	printf("llamado en CAMBIAPOSICIÖNCURSOR\n");

	PonPosicionCursor();

	DibujaCursor();

	}



void BorraGrafica() {

	XClearWindow (XtDisplay(grafica), XtWindow(grafica));

}



int GetChannel(int pos_x) {

	// Recoge los datos de la grafica y del desplazamiento de

	// la grafica

	Dimension ancho;

	int posicion, anchura;

	XtVaGetValues(grafica, XmNwidth, &ancho, NULL);

	XtVaGetValues (h_grafica, XmNvalue, &posicion,

				XmNsliderSize, &anchura, NULL);

	// Calcula en base a esto la correspondencia pixel<->canal

	double a = (double)pos_x*(double)anchura/(double)ancho;

	a = (a + (double)posicion)*(double)Datos.numCanales/(double)h_grafica_ancho_total;

	return redondea(a);

}



//************ MARCA ZOOM **************

void MarcaZoom(int posMin, int posMax) 

	{

//	printf("MARCA ZOOM\n");

	posMin--; posMax--;

	if (posMin < 0) posMin = 0;

	if (posMax >= Datos.numCanales) posMax = Datos.numCanales - 1;



	// hace el calculo de autozoom

	double minimo = (double)posMin+1;

	double maximo = (double)posMax+1;

	// Calcula el zoom necesario para esta ampliacion

	double zoom = (double) Datos.numCanales-1;

	if (maximo > minimo)

		zoom /= (maximo - minimo);

	else 

		{

		zoom = 1.0;

		minimo = 1.0; maximo = (double)Datos.numCanales;

		}



	// ajusta el zoom para que no se pase de los margenes

	if (zoom > 1024.0) 

		{

		zoom = 1024.0;

		// estira de manera simetrica los dos extremos

		double centro = (maximo+minimo)/2.0;

		maximo = centro + (double)Datos.numCanales/2048.0;

		minimo = centro - (double)Datos.numCanales/2048.0;

		if (maximo > (double)Datos.numCanales) 

			{

			minimo = Datos.numCanales*(1023.0/1024.0);

			maximo = Datos.numCanales;

			}

		else if (minimo < 1.0) 

			{

			minimo = 1.0;

			maximo = Datos.numCanales/1024.0;

			}

		}



	// actualiza la barra de zoom

	double temp = 10.0*log(zoom)/log(2.0);

	int ancho = (int) floor(temp);

   	zoom = pow(2.0, (double)ancho/10.0);

	XtVaSetValues (zoom_x_graf, XmNvalue, ancho, NULL);



	// pone el nuevo zoom en las graficas

	Graficas.SetZoomX(zoom);



	// Calcula el nuevo tamagno de la barra de scroll

	ancho = (int)  (h_grafica_ancho_total / zoom);

	// limita este valor por problemas con la scroll bar

	if (ancho >= h_grafica_ancho_total) ancho = h_grafica_ancho_total - 1; 



	// Calcula la nueva posicion de la barra de scroll

	temp = (minimo-1) / (double)Datos.numCanales;

	int posicion = (int)(temp * h_grafica_ancho_total);

	if ((posicion + ancho) > h_grafica_ancho_total) posicion = h_grafica_ancho_total - ancho;

	if (posicion < 1) posicion = 1;

	

	// Pone la nueva marca de minimo en el tapiz

	double CalcMin = (posicion - 1)/(double)(h_grafica_ancho_total -1);

	CalcMin *= (double)Datos.numCanales;

	Graficas.SetMinX(CalcMin+1);



	// Pone los nuevos datos del scroll

	int salto = (int) ancho / 100; // incremento para el paso a paso

	if (salto < 1) salto = 1;

	XtVaSetValues (h_grafica, XmNsliderSize, ancho,

      	XmNvalue, posicion, XmNpageIncrement, ancho,

			XmNincrement, salto, NULL);



	// Imprime el nuevo valor del zoom

	XmString cadena;

	char base[] = "xxxx";

	if (zoom < 10)

		sprintf(base, "%2.2f", zoom);

	else if (zoom < 100)

		sprintf(base, "%2.1f", zoom);

	else

		sprintf(base, "%4.0f", zoom);

	cadena = XmStringCreateLocalized (base);

	XtVaSetValues (label_zoom_x_graf, XmNlabelString, cadena, NULL);

	XmStringFree (cadena);



	char buffer[100];

	sprintf(buffer, "%d counts", Datos.DaTotalCuentas(posMin, posMax));

    cadena = XmStringCreateLocalized (buffer);

	XtVaSetValues (label_total_counts, XmNlabelString, cadena, NULL);

	XmStringFree (cadena);



	// Actualiza la regla

	if (XmToggleButtonGetState(auto_ajuste_graf))

		{

		AutoAjusteY();

		PintaGrafica(3);

		}

	else PintaGrafica(1);

	}



//******************* AUTO AJUSTE Y **********************

void AutoAjusteY() 

	{

	// Busca la ventana actual de trabajo

	int posicion=0, anchura=0;

	XtVaGetValues (h_grafica, XmNvalue, &posicion,

				XmNsliderSize, &anchura, NULL);

	// Calcula el valor minimo y maximo que se muestran en la ventana actual

	double minimoX = (double) posicion * (double)Datos.numCanales / (double)h_grafica_ancho_total;

	double maximoX = (double) (posicion+anchura) * (double)Datos.numCanales / (double)h_grafica_ancho_total;

	// busca los valores maximo y minimo que toman los datos en ese intervalo

	// Este es el punto mas lento de este algoritmo

	//NOTA: No sé por qué usa double si los parámetros son posiciones de la lista.

	//	Hago una conversion para evitar warnings

	double minimo = (double) Datos.minimo((int)minimoX+1, (int)maximoX);//NOTA: Antes sin el +1

	double maximo = (double) Datos.maximo((int)minimoX+1, (int)maximoX);

	// lo adapta a la escala utilizada

	double minTotal = (double) Datos.minTotal;

	double maxTotal = (double) Datos.maxTotal;

	if (escala_graf == 2) 

		{

		minimo = sqrt(minimo);

		maximo = sqrt(maximo);

		minTotal = sqrt(minTotal);

		maxTotal = sqrt(maxTotal);

		}

	else if (escala_graf == 3) 

		{

		minimo = log010(minimo);

		maximo = log010(maximo);

		minTotal = log010(minTotal);

		maxTotal = log010(maxTotal);

		}



	// TODO: dejar un 10% de margen en el eje vertical para la comodidad

	// de vision

	minimo -= minimo * 0.05;

	maximo += maximo * 0.05;

	if (minimo < minTotal) minimo = minTotal;

	if (maximo > maxTotal) maximo = maxTotal;

	

	// Calcula el zoom vertical necesario para esta ampliacion

	double zoom;

	if (minimo >= maximo)

		zoom = 1025.0; // to force readjust

	else

		zoom = (maxTotal - minTotal)/(maximo - minimo);



	// ajusta el zoom para que no se pase de los margenes

	if (zoom > 1024.0) 

		{

		zoom = 1024.0;

		// estira de manera simetrica los dos extremos

		double centro = (maximo+minimo)/2.0;

		maximo = centro + (maxTotal - minTotal)/2048.0;

		minimo = centro - (maxTotal - minTotal)/2048.0;

		if (maximo > maxTotal) 

			{

			minimo = (maxTotal*1023.0 + minTotal)/1024.0;

			maximo = maxTotal;

			}

		else if (minimo < minTotal) 

			{

			minimo = minTotal;

			maximo = (minTotal*1023.0 + maxTotal)/1024.0;

			}

		}



	// actualiza la barra de zoom

	double temp = 10.0*log(zoom)/log(2.0);

	int alto = (int) floor(temp);

   	zoom = pow(2.0, (double)alto/10.0);

	XtVaSetValues (zoom_y_graf, XmNvalue, alto, NULL);



	// pone el nuevo zoom

	Graficas.SetZoomY(zoom);





	// Calcula el nuevo tamagno de la barra de scroll

	alto = (int)  (v_grafica_alto_total / zoom);

	// limita este valor por problemas con la scroll bar

	if (alto == v_grafica_alto_total) alto = v_grafica_alto_total - 1; 



	// Calcula la nueva posicion de la barra de scroll

	temp = (minimo - minTotal) / (maxTotal - minTotal);

	posicion = (int)(temp * v_grafica_alto_total);

	if ((posicion + alto) > v_grafica_alto_total) posicion = v_grafica_alto_total - alto;

	if (posicion < 1) posicion = 1;

	

	// Indica el nuevo minimo del tapiz

	if (escala_graf == 2)

		minimo *= minimo;

	else if (escala_graf == 3)

		minimo = pow(10, minimo);

	Graficas.SetMinY(minimo);



	// Pone los nuevos datos del scroll

	XtVaSetValues (v_grafica, XmNsliderSize, alto,

      	XmNvalue, posicion, XmNpageIncrement, alto, NULL);



	// Imprime el nuevo valor del zoom

	XmString cadena;

	char base[] = "xxxx";

	if (zoom < 10)

		sprintf(base, "%2.2f", zoom);

	else if (zoom < 100)

		sprintf(base, "%2.1f", zoom);

	else

		sprintf(base, "%4.0f", zoom);

	cadena = XmStringCreateLocalized (base);

	XtVaSetValues (label_zoom_y_graf, XmNlabelString, cadena, NULL);

	XmStringFree (cadena);

	}



//********** ACTIVA GRÁFICA **********

void ActivaGrafica() 

	{

	XmString cadena;

	char buffer[100];

	sprintf(buffer, "%d counts", Datos.DaTotalCuentas());

    cadena = XmStringCreateLocalized (buffer);

	XtVaSetValues (label_total_counts, XmNlabelString, cadena, NULL);

	XmStringFree (cadena);

   	XtSetSensitive(fondo_grafica, true);

	}



void DesactivaGrafica () 

	{

   	XtSetSensitive(fondo_grafica, false);

	}



//************** PINTA GRÁFICA *******************

// Actualiza la gráfica repintando en uno de los dos modos

// modo 1: actualiza horizontal

// modo 2: actualiza vertical

void PintaGrafica(int modo) 

	{

	int temp = (int) XtWindow(grafica);

	if (temp != 0) 

		{

	   	if (modo & 1)	PintaReglaX();

	   	if (modo & 2)	PintaReglaY();



	   	if (Datos.numCanales > 0) 

	   		{

			Graficas.UpdateTapiz();

			Graficas.ShowTapiz();

			Graficas.DibujaCursor((double)CanalCursor+1, (double)Datos.espectro[CanalCursor+1]);

			}

	  	else	XClearWindow (XtDisplay(grafica), XtWindow(grafica));

		}

	}



void PintaReglaX() 

	{

	Dimension ancho, alto;

	unsigned short marcas;

   	char base[] = "xxxxxx";

	

   	// borra la regla y pone el rectangulo blanco	

	XClearWindow (XtDisplay(h_regla_grafica), XtWindow(h_regla_grafica));

   	XtVaGetValues(h_regla_grafica, XmNwidth, &ancho, XmNheight, &alto, NULL);

   	XDrawRectangle (dpy, XtWindow(h_regla_grafica), gcZoomX, 0, 0, ancho-1, alto-1);

	

	// Dibuja las marcas

	if (Datos.numCanales > 0) 

		{

		// Son necesarios 45 pixels para poner una marca

		// sin "aturullar" la informacion. Como los numeros de las marcas

		// se dibujan segun los pixels y no los mm, el siguiente calculo

		// es suficiente

		marcas = (ancho / 45) + 1; // marcas que entran por pantalla



		// Datos tecnicos de la zona de representacion

		int posicion, anchura;

		XtVaGetValues (h_grafica, XmNvalue, &posicion, XmNsliderSize, &anchura, NULL);



		// canales que hay en la ventana

		int minimo = (int)(Datos.numCanales*(posicion/(double)h_grafica_ancho_total));

		int maximo = (int)(Datos.numCanales*((posicion+anchura)/(double)h_grafica_ancho_total));

		int total = maximo - minimo;

		// estos parametros se calculan para acelerar el proceso de dibujo

		// en vez de utilizar la funcion GetChannel

		double a = (double)h_grafica_ancho_total * (double)ancho / ((double)Datos.numCanales * (double)anchura);

		double b = (double)posicion * (double)ancho / (double)anchura;



		// calcula la separacion en canales entre las marcas

		float separacion = total/marcas;

		// fuerza a que esa separacion sea de 1, 2 o 5 *10^x canales

		// primero saca el orden de valor de separacion

		minimo = (int)powi(10.0,orden(separacion));

		maximo = inicial(separacion);

		if ((minimo == -1)||(maximo == -1)) 

			{

			maximo = 1;

			minimo = 1;

			}

		else if (maximo > 5 ) 

			{

			maximo = 1;

			minimo *= 10;

			}

		else if (maximo > 2) maximo = 5;

		else maximo = 2;

		maximo *= minimo;

		// calcula el nuevo numero de marcas

		marcas = total/maximo + 1;

		

		// ahora tiene que dibujar las marcas que toque

		// busca la primera linea que tiene que dibujar

		minimo = (int)(Datos.numCanales*(posicion/(double)h_grafica_ancho_total));

		minimo = minimo - (minimo % maximo) + maximo;

		for (int i = 0; i < marcas; i++) 

			{

		   	int posicX = (int)((minimo + i*maximo - 1)*a - b);

			XDrawLine (dpy, XtWindow(h_regla_grafica), gcZoomX, posicX, 0, posicX, 15);

			sprintf(base, "%d", minimo + i*maximo);

			int longit = strlen(base);

			XDrawString(dpy, XtWindow(h_regla_grafica), gcZoomX, posicX - (longit*6/2), 27, base, longit);

			}

		// semimarcas

		if (maximo >= 10.0) 

			for (int i = 0; i < marcas+1; i++) 

				{

			   	int posicX = (int)((minimo + i*maximo - maximo/2 - 1)*a - b);

				XDrawLine (dpy, XtWindow(h_regla_grafica), gcZoomX, posicX, 0, posicX, 7);

				}

		}

	}



void PintaReglaY() 

	{

   	Dimension ancho, alto;

	unsigned short marcas;

   	char base[] = "xxxxxxx";

	

	XClearWindow (dpy, XtWindow(v_regla_grafica));

   	XtVaGetValues(v_regla_grafica, XmNwidth, &ancho, XmNheight, &alto, NULL);

   	XDrawRectangle (dpy, XtWindow(v_regla_grafica), gcZoomY, 0, 0, ancho-1, alto-1);



	// TODO: escribir una funcion decente que dibuje las lineas de cuentas

	// segun la escala elegida. Esto que hay hecho es un parche

	// cutre

	if (Datos.numCanales > 0) 

		{

		marcas = (alto / 30) + 1; // marcas que entran por pantalla



		// cuentas que hay en la ventana

		double temp;

		switch(escala_graf) 

			{

			case 2:

				temp = sqrt((double)Datos.maxTotal) - sqrt((double)Datos.minTotal);

				break;

			case 3:

				temp = log010((double)Datos.maxTotal) - log010((double)Datos.minTotal);

				break;

			default:

				temp = Datos.maxTotal - Datos.minTotal;

				break;

			}



		// Datos tecnicos de la zona de representacion

		int posicion, altura;

		XtVaGetValues (v_grafica, XmNvalue, &posicion, XmNsliderSize, &altura, NULL);



		double minimo = (temp*(posicion/(double)v_grafica_alto_total));

		double maximo = (temp*((posicion+altura)/(double)v_grafica_alto_total));

		double total = maximo - minimo;

		// estos parametros se calculan para acelerar el proceso de dibujo

		double a = (double)v_grafica_alto_total * (double)alto / (temp * (double)altura);

		double b = (double)posicion * (double)alto / (double)altura;



			

		// salto representara ahora la distancia entre las marcas

		double salto, baseN;

		double separacion;

		switch (escala_graf) 

			{

			case 3:

			case 2:

				// calculamos el orden de las marcas

				salto = (total/marcas);

				baseN = minimo;

				break;

			default: // escala lineal

				// calculamos el orden de las marcas

				separacion = total/marcas;

				// pasamos el valor de separacion a multiplos de 1, 2 o 5 *10 x canales

				minimo = powi(10.0,orden(separacion));

				maximo = (double) inicial(separacion);

				if ((minimo == -1.0)||(maximo == -1.0)) 

					{

					maximo = 1;

					minimo = 1;

					}

				else if (maximo > 5 ) 

					{

					maximo = 1;

					minimo *= 10;

					}

				else if (maximo > 2) maximo = 5;

				else maximo = 2;

				salto = (maximo * minimo);

				// calcula el nuevo numero de marcas

				marcas = (int)(total/salto) + 1;

				baseN = (temp*(posicion/(double)v_grafica_alto_total));

				baseN = baseN - (redondea(baseN) % redondea(salto)) + redondea(salto);;

				break;

			}

		// ahora tiene que dibujar las marcas que toque

		// busca la primera linea que tiene que dibujar

		for (int i = 0; i < marcas; i++) 

			{

			double pos = (baseN + i*salto);

		   	int posicY = alto - redondea(pos*a - b);

			XDrawLine (dpy, XtWindow(v_regla_grafica), gcZoomY, ancho-20, posicY, ancho, posicY);

			switch(escala_graf) 	

				{

				case 2:

					pos *= pos;

					break;

				case 3:

					pos = pow(10.0, pos);

					break;

				}

			sprintf(base, "%d", redondea(pos));

			int longit = strlen(base);

			XDrawString(dpy, XtWindow(v_regla_grafica), gcZoomY, ancho - longit*6 - 25, posicY + 4, base, longit);

			}

		}

	}



//****************** PON ZOOM X *************

void Pon_Zoom_X(int valor) 

	{

   	XmString cadena;

   	double graf_zoom_x = 1.0;

   	char base[10];



   	graf_zoom_x = pow(2.0, (double)valor/10.0);



   	// Calcula el nuevo tamagno de la barra de scroll

	int ancho = (int) (h_grafica_ancho_total / graf_zoom_x);

	// limita este valor por problemas con la scroll bar

	if (ancho == h_grafica_ancho_total) ancho = h_grafica_ancho_total-1;



	// Calcula la nueva posicion de la barra de scroll

	// centra el zoom en el cursor

	int posicion = (int)((double)CanalCursor*h_grafica_ancho_total/(double)Datos.numCanales - ancho/2.0);

	if ((posicion + ancho) > h_grafica_ancho_total) posicion = h_grafica_ancho_total - ancho;

	if (posicion < 1) posicion = 1;



	// Pone los nuevos datos

	int salto = (int) ancho / 100; // incremento para el paso a paso

	if (salto < 1) salto = 1;

	XtVaSetValues (h_grafica, XmNsliderSize, ancho,

      	XmNvalue, posicion, XmNpageIncrement, ancho, 

			XmNincrement, salto, NULL);

			

	// Indica el cambio a las graficas, tanto el zoom como la nueva

	// posicion minima

	double minimo = (posicion*(double)Datos.numCanales/(double)h_grafica_ancho_total);

	Graficas.SetZoomX(graf_zoom_x);

	Graficas.SetMinX(minimo+1);



	//

	double maximo = (double) (posicion+ancho) * (double)Datos.numCanales / (double)h_grafica_ancho_total;

	char buffer[100];

	sprintf(buffer, "%d counts", Datos.DaTotalCuentas((int)(minimo+1), (int)(maximo)));//+1?

	cadena = XmStringCreateLocalized (buffer);

	XtVaSetValues (label_total_counts, XmNlabelString, cadena, NULL);

	XmStringFree (cadena);

	//



	// Imprime el nuevo valor del zoom

   	if (graf_zoom_x < 10)     sprintf(base, "%2.2f", graf_zoom_x);

   	else 	if (graf_zoom_x < 100)     sprintf(base, "%2.1f", graf_zoom_x);

   		else			   sprintf(base, "%4.0f", graf_zoom_x);

   	cadena = XmStringCreateLocalized (base);

   	XtVaSetValues (label_zoom_x_graf, XmNlabelString, cadena, NULL);

   	XmStringFree (cadena);

	}



//************** PON ZOOM Y ************

void Pon_Zoom_Y(int valor) 

	{

   	XmString cadena;

   	double graf_zoom_y = 1.0;

   	char base[10];



   	graf_zoom_y = pow(2.0, (float)valor/10.0);



   	// Calcula el nuevo tamagno de la barra de scroll

	int alto = (int)(v_grafica_alto_total / graf_zoom_y);

	// limita este valor por problemas con la scroll bar

	if (alto == v_grafica_alto_total) alto = v_grafica_alto_total-1; 



	double minTotal = (double) Datos.minTotal;

	double maxTotal = (double) Datos.maxTotal;

	double centro = Datos.espectro[CanalCursor+1];

	if (escala_graf == 2) 

		{

		minTotal = sqrt(minTotal);

		maxTotal = sqrt(maxTotal);

		centro = sqrt(centro);

		}

	else if (escala_graf == 3) 

		{

		minTotal = log010(minTotal);

		maxTotal = log010(maxTotal);

		centro = log010(centro);

		}

	// Calcula la nueva posicion de la barra de scroll

	// centrando el zoom en el cursor

	double temp = (centro - minTotal)/maxTotal;

	int posicion = redondea(temp*v_grafica_alto_total - alto/2.0);

	if ((posicion + alto) > v_grafica_alto_total) posicion = v_grafica_alto_total - alto;

	if (posicion < 1) posicion = 1;



	// Pone los nuevos datos

	int salto = (int) alto / 100;

	if (salto < 1) salto = 1;

	XtVaSetValues (v_grafica, XmNsliderSize, alto,

      	XmNvalue, posicion, XmNpageIncrement, alto, XmNincrement, salto, NULL);

			

	// Indica el cambio a las graficas, tanto el zoom como la nueva

	// posicion minima

	double minimo = (int)(posicion*(double)(maxTotal - minTotal)/(double)h_grafica_ancho_total) + (double) minTotal;

	Graficas.SetZoomY(graf_zoom_y);

	if (escala_graf == 2)		minimo *= minimo;

	else if (escala_graf == 3)	minimo = pow(10, minimo);

	Graficas.SetMinY(minimo);



	// Imprime el nuevo valor del zoom

	if (graf_zoom_y < 10)  		sprintf(base, "%2.2f", graf_zoom_y);

	else 	if (graf_zoom_y < 100)	sprintf(base, "%2.1f", graf_zoom_y);

		else			sprintf(base, "%4.0f", graf_zoom_y);

   	cadena = XmStringCreateLocalized (base);

   	XtVaSetValues (label_zoom_y_graf, XmNlabelString, cadena, NULL);

   	XmStringFree (cadena);

	}



//***************** COPIA ESPECTRO A GRÁFICA *****************

	/* 

	Copia la información que tenemos del espectro a la gráfica

	*/

int CopiaEspectroAGrafica() 

	{

//	printf("CopiaEspectroAGrafica\n");

	if (!Graficas.SeleccionaGrafica(1)) // Selecciona la grafica "Normal"

		return -1;

	if (!Graficas.PreparaDatos(Datos.numCanales)) // prepara la memoria para la copia

		return -1;

	

	double *DatosX=Graficas.pDatosX();

	double *DatosY=Graficas.pDatosY();



	for (int i = 0; i < Datos.numCanales; i++) 

		{

		DatosX[i] = (double) i+1;

		DatosY[i] = (double) Datos.espectro[i+1];

		}

	Graficas.SetGraphX(1.0, (double) Datos.numCanales);

	Graficas.SetGraphY((double) Datos.minTotal, (double) Datos.maxTotal);

	Graficas.CopiaTerminada();

	Graficas.SetActive(true);

	Graficas.SetLimitsCursor(1.0, (double)Datos.numCanales,

				(double)Datos.minTotal, (double)Datos.maxTotal);

	return 0;

	}



//********************* COPIA CONTINUO A GRÁFICA **********************

// Copia los datos del continuo calculado a la gráfica.

// NOTA: Pinta todo el gráfico aunque hayamos calculado el continuo en un

//	 intervalo menor al número total de canales. En los canales en los

//	 que no se ha calculado pone ceros para el continuo. 

int CopiaContinuoAGrafica() 

	{

	printf("CopiaContinuo a gráfica-INICIO\n");

	 // Selecciona la grafica "Continuo"

	if (!Graficas.SeleccionaGrafica(4))		return -1;



	//Dibujamos sólo en la parte donde hay continuo

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

	

	// prepara la memoria para la copia

			//NOTA: Hay que preparar los canales con el mismo número de siempre o peta

//	if (!Graficas.PreparaDatos(Datos.numCanales)) 	{printf("ERROR: Preparación de los datos errónea\n"); return -1;}

	if (!Graficas.PreparaDatos(fin-inicio)) 	{printf("ERROR: Preparación de los datos errónea\n"); return -1;}//<--ésta es la que parece que da el fallo

	

	//NOTA: El problema a borrar es que aquí las creamos con el tamaño tradicional,

	//		con lo que los puntos donde no calculamos el espectro quedan a null.

	double *DatosX=Graficas.pDatosX();

	double *DatosY=Graficas.pDatosY();

	//Vamos a probar a inicializarlos antes:

	for (int i = 0; i < fin-inicio; i++) 

		{

		DatosX[i] = (double) i+1+inicio;

		DatosY[i] = (double) Datos.continuo[i+1+inicio];

		}

	

	Graficas.setInicio(inicio);

	Graficas.SetGraphX(1.0, (double) Datos.numCanales);//<--esto no parece que haga falta sustituirlo.

//	Graficas.SetGraphX(inicio, fin);

	Graficas.SetGraphY((double) Datos.minTotal, (double) Datos.maxTotal);

	Graficas.CopiaTerminada();//Escala según la opción que tengamos

	return 0;

	}



//***************** COPIA AJUSTES A GRAFICA ******************

// Copia a la gráfica los ajustes de los picos como convolución + continuo.

int CopiaAjustesAGrafica() 

	{

	printf("Copiando ajustes a gráfica\n");

	if (!Graficas.SeleccionaGrafica(5)) // Selecciona la grafica "Ajuste"

		return -1;

	// Cuenta los valores que hay distintos de cero

	int j = 0;

	for (int i = 1; i <= Datos.numCanales; i++)

		if (Datos.total[i] > 0.0) j++;

	if (!Graficas.PreparaDatos(j)) // prepara la memoria para la copia

		return -1;

//	printf("Datos preparados\n");

	double *DatosX=Graficas.pDatosX();

	double *DatosY=Graficas.pDatosY();

//	printf("Datos reservados\n");

	j = 0;

	for (int i = 1; i <= Datos.numCanales; i++)

		if (Datos.total[i] > 0.0) 

			{

			DatosX[j] = (double) i;

			DatosY[j] = (double) Datos.total[i];

			j++;

			}

//	printf("Datos inicializados\n");	

	Graficas.SetGraphX(1.0, (double) Datos.numCanales);

	Graficas.SetGraphY((double) Datos.minTotal, (double) Datos.maxTotal);

//	printf("Datos representados\n");

	Graficas.CopiaTerminada();

	return 0;

	}



//***************** COPIA AJUSTES PARCIALES A GRAFICA ******************

// Copia a la gráfica todos los picos deconvolucionados que ajustan numIntervalo,

//	destacando numPico.

int CopiaAjustesParcialesAGrafica(int numIntervalo, int numPico) 

	{

//	printf("COPIA AJUSTES PARCIALES A GRAFICA %d,%d\n", numIntervalo, numPico);

	

	//Copiamos la opción seleccionada

	NodoPicos *intervalo = DevuelveNodo(Datos.listaPicos, numIntervalo);



	//Copiamos la deconvolución de todos los picos del intervalo

	BorraGeneticEnGrafica();

	for(int i=1;i<=intervalo->picos.numElementos;i++)

		{

		CopiaAjustesDeconvolucionAGrafica(numIntervalo, i,7+i);

		ActivaAjustesDeconvolucionGrafica(true, 7+i);

		PintaGrafica(0);

		}

	//

	

	if(numPico>0)

		{

		//Tomamos el centroide y la anchura en canales

		int li=intervalo->limInferior;

		int ls=intervalo->limSuperior;

		

		if (!Graficas.SeleccionaGrafica(6)) return -1;

		// Cuenta los valores que hay distintos de cero

		// Los valores distintos de cero serán los mismos en un ajuste parcial que

		// en uno total.

		int j = 0;

		for (int i = li; i <= ls; i++)

			if (Datos.total[i] > 0.0) j++;



		if (!Graficas.PreparaDatos(j)) return -1;

	

		double *DatosX=Graficas.pDatosX();

		double *DatosY=Graficas.pDatosY();

	

		j=0;

		for (int i = li; i <= ls; i++)

			if (Datos.total[i] > 0.0) 

				{

				DatosX[j] = (double) i;

				DatosY[j] = (double) Datos.DaValorPico(numIntervalo, numPico, i);

				j++;

				}

			

		Graficas.SetGraphX(1.0, (double) Datos.numCanales);

		Graficas.SetGraphY((double) Datos.minTotal, (double) Datos.maxTotal);

		Graficas.CopiaTerminada();

		}		

	return 0;

	}



//***************** COPIA AJUSTES DECONVOLUCION A GRAFICA ******************

// Copia a la gráfica los ajustes de los picos deconvolucionados.

int CopiaAjustesDeconvolucionAGrafica(int numIntervalo, int numPico, int numGraf) 

	{

	NodoPicos *intervalo = DevuelveNodo(Datos.listaPicos, numIntervalo);

	//Tomamos el centroide y la anchura en canales

	int li=intervalo->limInferior;

	int ls=intervalo->limSuperior;

		

	if (!Graficas.SeleccionaGrafica(numGraf)) // Selecciona la grafica "AjusteParcial"

		{

		printf("ERROR: Cannot select graph %d\n", numGraf);

		return -1;

		}

	// Cuenta los valores que hay distintos de cero

	// Los valores distintos de cero serán los mismos en un ajuste parcial que

	// en uno total.

	int j = 0;

	for (int i = li; i <= ls; i++)

		if (Datos.total[i] > 0.0) j++;



	if (!Graficas.PreparaDatos(j)) // prepara la memoria para la copia

		return -1;

	

	double *DatosX=Graficas.pDatosX();

	double *DatosY=Graficas.pDatosY();

	

	j=0;

	for (int i = li; i <= ls; i++)

		if (Datos.total[i] > 0.0) 

			{

			DatosX[j] = (double) i;

			DatosY[j] = (double) Datos.DaValorPico(numIntervalo, numPico, i);

			//printf("y[%d]=%f ", j, DatosY[j]);

			j++;

			}

			

	Graficas.SetGraphX(1.0, (double) Datos.numCanales);

	Graficas.SetGraphY((double) Datos.minTotal, (double) Datos.maxTotal);

	Graficas.CopiaTerminada();

	return 0;

	}



//----------------------------- NUEVO --------------------------

//***************** COPIA GENETIC A GRAFICA ******************

/* Copia a la gráfica la función *funcion entre los canales li y ls,

	para la gráfica numGraf

	

//NOTA: NumIntervalo no lo usa para nada

*/

int CopiaGeneticAGrafica(float *funcion, int li, int ls, int numGraf) 

	{

//	printf("COPIA GENETIC A GRAFICA--------------------------------\n");

	int j=0;

//	printf("Vamos a seleccionar la gráfica\n");

	if (!Graficas.SeleccionaGrafica(numGraf)) // Selecciona la grafica "Genetic"

		return -1;

	// Cuenta los valores que hay distintos de cero

	// Los valores distintos de cero serán los mismos en un ajuste parcial que

	// en uno total.

//	printf("Gráfica seleccionada\n");

	j=ls-li+1;

	if (!Graficas.PreparaDatos(j)) // prepara la memoria para la copia

		return -1;

//	printf("Datos Preparados\n");

/*	double *DatosX=new double[j];

	double *DatosY=new double[j];

	DatosX=Graficas.pDatosX();

	DatosY=Graficas.pDatosY();*/

	double *DatosX=Graficas.pDatosX();

	double *DatosY=Graficas.pDatosY();

			

//	printf("Datos inicializados\n");

	j=0;

	for (int i = li; i <= ls; i++)

		{

		DatosX[j] = (double) i;

		DatosY[j]= Datos.continuo[i];

		DatosY[j] += funcion[j];

		j++;

		}

		

	Graficas.SetGraphX(1.0, (double) Datos.numCanales);

	Graficas.SetGraphY((double) Datos.minTotal, (double) Datos.maxTotal);

	Graficas.CopiaTerminada();

	return 0;

	}



//***************** COPIA FONDO A GRAFICA ******************

/* Copia a la gráfica las cuentas sumadas del continuo y cuentasFondo 

	entre los canales li y ls para la gráfica numGraf

*/

int CopiaFondoAGrafica(int li, int ls) 

	{

//	printf("COPIA FONDO A GRAFICA de %d a %d--------------------------------\n", li, ls);

	int j=0;

//	printf("Seleccionamos gráfica\n");

	if (!Graficas.SeleccionaGrafica(29)) // Selecciona la grafica "FBD"

		return -1;

	// Cuenta los valores que hay distintos de cero

	// Los valores distintos de cero serán los mismos en un ajuste parcial que

	// en uno total.

	j=ls-li+1;

	//printf("Preparamos datos\n");

	if (!Graficas.PreparaDatos(j)) // prepara la memoria para la copia

		return -1;

	//printf("pdatosX\n");

	double *DatosX=Graficas.pDatosX();

	//printf("pdatosY\n");

	double *DatosY=Graficas.pDatosY();

			

	//printf("Metemos datosx e y entre \n");

	j=0;

	for (int i = li; i <= ls; i++)

		{

		DatosX[j] = (double) i;

		//printf("A ver el continuo\n");

		DatosY[j]= Datos.continuo[i];

		//printf("A ver las cuentas de fondo\n");

		DatosY[j] += Datos.cuentasFondo[i];

//		printf("Datos Y es %f\n", DatosY[j]);

		j++;

		}

		

	//printf("setGraphXY y copiaterminada\n");

	Graficas.SetGraphX(1.0, (double) Datos.numCanales);

	Graficas.SetGraphY((double) Datos.minTotal, (double) Datos.maxTotal);

	Graficas.CopiaTerminada();

	//printf("Fin de copia fondo a grafica\n");	

	return 0;

	}



//***************** COPIA FUNCION A GRAFICA ******************

/* Copia a la gráfica la función *funcion entre los canales li y ls,

	para la gráfica numGraf

*/

int CopiaFuncionAGrafica(float *funcion, int li, int ls) 

	{

	printf("COPIA FUNCION A GRAFICA de %d a %d---------------------\n", li, ls);

	int j=0;

	printf("Seleccionamos gráfica\n");

	if (!Graficas.SeleccionaGrafica(29)) // Selecciona la grafica "FBD"

		return -1;

	// Cuenta los valores que hay distintos de cero

	// Los valores distintos de cero serán los mismos en un ajuste parcial que

	// en uno total.

	j=ls-li+1;

	printf("Preparamos datos\n");

	if (!Graficas.PreparaDatos(j)) // prepara la memoria para la copia

		return -1;

	printf("pdatosX\n");

	double *DatosX=Graficas.pDatosX();

	printf("pdatosY\n");

	double *DatosY=Graficas.pDatosY();

			

	printf("Metemos datosx e y entre \n");

	j=0;

	for (int i = li; i <= ls; i++)

		{

		DatosX[j] = (double) i;

		DatosY[j]= Datos.continuo[i];

		DatosY[j] += funcion[j];

		j++;

		}

		

	printf("setGraphXY y copiaterminada\n");

	Graficas.SetGraphX(1.0, (double) Datos.numCanales);

	Graficas.SetGraphY((double) Datos.minTotal, (double) Datos.maxTotal);

	Graficas.CopiaTerminada();

	printf("Fin de copia fondo a grafica\n");	

	return 0;

	}



//---------------------------------------------------------

//************ COPIA ERROR A GRAFICA *****************

int CopiaErrorAGrafica(float *funcion, int num, int numGraf) 

	{

	int j=num;

	if (!Graficas.SeleccionaGrafica(numGraf)) // Selecciona la grafica "Genetic"

		return -1;

	// Cuenta los valores que hay distintos de cero

	// Los valores distintos de cero serán los mismos en un ajuste parcial que

	// en uno total.

	if (!Graficas.PreparaDatos(j)) // prepara la memoria para la copia

		return -1;

	double *DatosX=Graficas.pDatosX();

	double *DatosY=Graficas.pDatosY();

			

	j=0;

	for (int i = 1; i <= num; i++)

		{

		DatosX[j] = (double) i;

		DatosY[j] = funcion[j];

		j++;

		}

		

	Graficas.SetGraphX(1.0, (double) Datos.numCanales);

	Graficas.SetGraphY((double) Datos.minTotal, (double) Datos.maxTotal);

	Graficas.CopiaTerminada();

	return 0;

	}

	

//*********************** COPIA PICOS CH GRAFICA **********************

//Copia a la variable Graficas la información de gráfica correspondiente a los canales de pico

//Graficas es una variable de tipo cGrafica definida en el fichero Graficas.h

int CopiaPicosChGrafica() 

	{

	if (!Graficas.SeleccionaGrafica(3)) // Selecciona la grafica "Picos"

		return -1;

	// Cuenta los valores que hay distintos de cero

	int j = 0;

	for (int i = 1; i <= Datos.numCanales; i++)

		if (Datos.IsChannelPeaks(i)) j++;



	if (!Graficas.PreparaDatos(j)) // prepara la memoria para la copia

		return -1;

	

	double *DatosX=Graficas.pDatosX();

	double *DatosY=Graficas.pDatosY();

	j = 0;

	for (int i = 1; i <= Datos.numCanales; i++)

		if (Datos.IsChannelPeaks(i)) 

			{

			DatosX[j] = (double) i;

			DatosY[j] = (double) Datos.espectro[i];

			j++;

			}

	

	Graficas.SetGraphX(1.0, (double) Datos.numCanales);

	Graficas.SetGraphY((double) Datos.minTotal, (double) Datos.maxTotal);

	Graficas.CopiaTerminada();

	return 0;

	}



//*********************** COPIA CONTINUO CH GRAFICA **********************

//Copia a la variable Graficas la información de gráfica correspondiente a los canales de continuo 

//Graficas es una variable de tipo cGrafica definida en el fichero Graficas.h

int CopiaContinuoChGrafica() 

	{

	if (!Graficas.SeleccionaGrafica(2)) // Selecciona la grafica "Fondo"

		return -1;

	// Cuenta los valores que hay distintos de cero

	int j = 0;

	for (int i = 1; i <= Datos.numCanales; i++)

		if (Datos.IsChannelContinuum(i)) j++;



	if (!Graficas.PreparaDatos(j)) // prepara la memoria para la copia

		return -1;

	

	double *DatosX=Graficas.pDatosX();

	double *DatosY=Graficas.pDatosY();

	

	j = 0;

	for (int i = 1; i <= Datos.numCanales; i++)

		if (Datos.IsChannelContinuum(i)) 

			{

			DatosX[j] = (double) i;

			DatosY[j] = (double) Datos.espectro[i];

			j++;

			}

	

	Graficas.SetGraphX(1.0, (double) Datos.numCanales);

	Graficas.SetGraphY((double) Datos.minTotal, (double) Datos.maxTotal);

	Graficas.CopiaTerminada();

	return 0;

	}



//***************** BORRAR FUNCIONES ******************

// Selecciona una de las gráficas y llama a preparaDatos(0) que elimina toda la información de esa función

//NOTA: Quizá sería mejor una sola función a la que se le pasara el número de gráfica

int BorraContinuoEnGrafica() 

	{

	if (!Graficas.SeleccionaGrafica(4)) // Selecciona la grafica "Continuo"

		return -1;

	Graficas.PreparaDatos(0);

	return 0;

	}



int BorraContinuoChEnGrafica() 

	{

	if (!Graficas.SeleccionaGrafica(2)) // Selecciona la grafica "Fondo"

		return -1;

	Graficas.PreparaDatos(0);

	

	return 0;

	}



int BorraPicosChEnGrafica() 

	{

	if (!Graficas.SeleccionaGrafica(3)) // Selecciona la grafica "Picos"

		return -1;

	Graficas.PreparaDatos(0);



	return 0;

	}



int BorraAjustesEnGrafica() 

	{

	if (!Graficas.SeleccionaGrafica(5)) // Selecciona la grafica "Ajustes"

		return -1;

	Graficas.PreparaDatos(0);



	return 0;

	}

	

int BorraAjustesParcialesEnGrafica() 

	{

	if (!Graficas.SeleccionaGrafica(6)) // Selecciona la grafica "Parciales"

		return -1;

	Graficas.PreparaDatos(0);



	return 0;

	}

	

int BorraAjustesDeconvolucionEnGrafica(int numGraf) 

	{

	if (!Graficas.SeleccionaGrafica(numGraf)) // Selecciona la grafica "Parciales"

		return -1;

	Graficas.PreparaDatos(0);



	return 0;

	}



int BorraGeneticEnGrafica()

	{

	for(int i=7;i<28;i++)

		{

		if (!Graficas.SeleccionaGrafica(i))		return -1;

		Graficas.PreparaDatos(0);

		}

	}	

	

int BorraGeneticEnGrafica(int numGraf) 

	{

	if (!Graficas.SeleccionaGrafica(numGraf)) // Selecciona la grafica de picos parciales

		return -1;

	Graficas.PreparaDatos(0);



	return 0;

	}	



int BorraFondoEnGrafica() 

	{

	if (!Graficas.SeleccionaGrafica(29)) // Selecciona la grafica de fondo

		return -1;

	Graficas.PreparaDatos(0);

	return 0;

	}	

	

//******************* ACTIVA GRAFICA ****************

//Selecciona una gráfica y pone su estado a activo



void ActivaDatosGrafica(bool state) 

	{

	if (Graficas.SeleccionaGrafica(1)) // Selecciona la grafica "Normal"

		Graficas.SetActive(state);

	}



void ActivaContinuoChGrafica(bool state) 

	{

	if (Graficas.SeleccionaGrafica(2)) // Selecciona la grafica "Fondo"

		Graficas.SetActive(state);

	printf("ActivaContinuoChGrafica fin\n");

	}



void ActivaPicosChGrafica(bool state) 

	{

	if (Graficas.SeleccionaGrafica(3)) // Selecciona la grafica "Picos"

		Graficas.SetActive(state);

	}



void ActivaContinuoGrafica(bool state) 

	{

	if (Graficas.SeleccionaGrafica(4)) // Selecciona la grafica "Continuo"

		Graficas.SetActive(state);

	}



void ActivaAjustesGrafica(bool state) 

	{

//	printf("ActivaAjustesGrafica inicio\n");

	if (Graficas.SeleccionaGrafica(5)) // Selecciona la grafica "Ajuste"

		Graficas.SetActive(state);

	}



void ActivaAjustesParcialesGrafica(bool state) 

	{

//	printf("ActivaAjustesParcialesGrafica inicio\n");

	if (Graficas.SeleccionaGrafica(6)) Graficas.SetActive(state);

	for(int i=7;i<28;i++)	

		if (Graficas.SeleccionaGrafica(i)) Graficas.SetActive(state);

	}



void ActivaAjustesDeconvolucionGrafica(bool state, int numGraf) 

	{

//	printf("ActivaAjustesParcialesGrafica inicio\n");

	if (Graficas.SeleccionaGrafica(numGraf)) // Selecciona la grafica "Deconvolucion"

		Graficas.SetActive(state);

//	printf("ActivaAjustesParcialesGrafica fin\n");

	}



//---------------- NUEVO -----------------	

void ActivaGeneticGrafica(bool state, int numGraf)

	{

//	printf("ActivaGeneticGrafica\n");

	if(Graficas.SeleccionaGrafica(numGraf))	Graficas.SetActive(state);

	}



//*********** ACTIVA FONDO GRÁFICA ***************

void ActivaFondoGrafica(bool state)

	{

	if(Graficas.SeleccionaGrafica(29))	

		{

//		printf("\n\n\n\n\n\n\nPoniendo activa la gráfica 29\n");

		Graficas.SetActive(state);

		}

	}



void ActivaErrorGrafica(bool state, int numGraf)

	{

//	printf("ActivaGeneticGrafica\n");

	if(Graficas.SeleccionaGrafica(numGraf))	Graficas.SetActive(state);

	}

//-------------------------------------------

	

//******************************************



/************************************************************************

 ***

 ***			              CALLBACKS

 ***

 ************************************************************************/



void redraw (Widget widget, XtPointer client_data, XtPointer call_data) {

	Dimension ancho, alto;

	XmDrawingAreaCallbackStruct *dwc = (XmDrawingAreaCallbackStruct *) call_data;



	if (dwc->reason == XmCR_RESIZE) {

		// Cambio de tamagno de las ventanas

		// recoge el nuevo tamagno de visualizacion

		XtVaGetValues(grafica, XmNwidth, &ancho, XmNheight, &alto, NULL);

		Graficas.Resize(ancho, alto);

		if (XtIsRealized(grafica))

			PintaGrafica(3);

	}

	else { // es un evento Expose

		if (CambiandoSeccion) 

			{ 

			// hay cambios por un resize debido a un cambio de seccion,

			// hay que actualizar datos

			PintaGrafica(3);

			CambiandoSeccion = false;

			}

		else { // solo es un evento expose, hay que refrescar datos

			if (widget == grafica) 

				{

				XExposeEvent *evento = (XExposeEvent *) dwc->event;

				Graficas.ShowTapiz(evento->x, evento->y,

								evento->width, evento->height);

				if (Datos.numCanales > 0) Graficas.DibujaCursor((double)CanalCursor+1, (double)Datos.espectro[CanalCursor+1]);

				}

			else { // es en las reglas

				if (widget == h_regla_grafica)

					PintaReglaX();

				else PintaReglaY();

			}

		}

	}

}



void DibujaCursor(Widget widget, XtPointer client_data, XtPointer call_data) {

	Dimension ancho, alto;

	static short modo = 0;

	static int min_x, max_x, temp_x;

	XmDrawingAreaCallbackStruct *cbs = (XmDrawingAreaCallbackStruct *) call_data;

	XEvent *event = cbs->event;



	if (event->xany.type == MotionNotify) { // Seleccion de zona

	   XtVaGetValues(grafica, XmNwidth, &ancho, XmNheight, &alto, NULL);

		// Borra la anterior zona seleccionada

		int temp_val_min = min_x;

		if (min_x > temp_x) temp_val_min = temp_x;

	   XFillRectangle (dpy, XtWindow(grafica), RubberGrafica, temp_val_min, 0, abs(temp_x-min_x), alto);

		// Pone la siguiente zona seleccionada

		temp_val_min = min_x;

		if (min_x > event->xbutton.x) temp_val_min = event->xbutton.x;

	   XFillRectangle (dpy, XtWindow(grafica), RubberGrafica, temp_val_min, 0, abs(event->xbutton.x-min_x), alto);

		temp_x = event->xbutton.x;

	}

	else if (event->xany.type == ButtonPress) { // se pulso el boton del raton

		if ((modo == 0)&&(event->xbutton.button == Button1)) {

			modo = 1; // pone el modo arrastrar

			min_x = event->xbutton.x;

			temp_x = min_x;

		}

	}

	else if (event->xany.type == ButtonRelease) { // se solto el boton del raton

		if ((event->xbutton.button == Button1)&&(modo == 1)) {

			modo = 0; // termina el modo arrastrar

			max_x = event->xbutton.x;

				

			// Borra la seleccion anterior

			XtVaGetValues(grafica, XmNwidth, &ancho, XmNheight, &alto, NULL);

			int temp_val_min = min_x;

			if (min_x > temp_x) temp_val_min = temp_x;

			XFillRectangle (dpy, XtWindow(grafica), RubberGrafica, temp_val_min, 0, abs(temp_x-min_x), alto);



			if (abs(max_x - min_x)/pixDensX > 5) {

				// arrastre minimo de 5 mm para ser una seleccion

				int pos_min = GetChannel(min_x);

				int pos_max = GetChannel(max_x);

				if (pos_min > pos_max) {

					int temp = pos_min;

					pos_min = pos_max;

					pos_max = temp;

				}

				switch (EstadoTrabajo.Actual) {

				   case ESTADO_PRECALIBRACION:

					   PonIntervaloPreCalibracion(pos_min+1, pos_max+1);

						break;

				   case ESTADO_CONTINUO:

					   PonIntervaloContinuo(pos_min+1, pos_max+1);

						break;

				}

			}

			else {

				// Se ha pulsado sobre el tapiz para mover el cursor

				// Calcula la nueva posicion del cursor en canales

				CanalCursor = GetChannel(min_x);

				if (CanalCursor < 0) CanalCursor = 0;

				else if (CanalCursor >= Datos.numCanales) CanalCursor = Datos.numCanales-1;



				// Actualiza la ventana de datos

				PonPosicionCursor();

				DibujaCursor();

			}

		}

	}

}



//***************** CURSOR ZOOM ***********

void CursorZoom(Widget widget, XtPointer client_data, XtPointer call_data) 

	{

	printf("CURSOR ZOOM\n");	

	

	Dimension ancho, alto;

	static short modo = 0;

	static int min_x, max_x, temp_x;

	int temp;

	XmDrawingAreaCallbackStruct *cbs = (XmDrawingAreaCallbackStruct *) call_data;



	XEvent *event = cbs->event;

	

	if (event->xany.type == MotionNotify) 

		{

	  	XtVaGetValues(h_regla_grafica, XmNwidth, &ancho, XmNheight, &alto, NULL);

		// Borra el anterior

		int temp_val_min = min_x;

		if (min_x > temp_x) temp_val_min = temp_x;

	   	XFillRectangle (dpy, XtWindow(h_regla_grafica), RubberZoomX, temp_val_min, 1, abs(temp_x-min_x), alto-2);

		// Pone el siguiente

		temp_val_min = min_x;

		if (min_x > event->xbutton.x) temp_val_min = event->xbutton.x;

	   	XFillRectangle (dpy, XtWindow(h_regla_grafica), RubberZoomX, temp_val_min, 1, abs(event->xbutton.x-min_x), alto-2);

		temp_x = event->xbutton.x;

		}

	else if (event->xany.type == ButtonPress) 

		{

		if ((modo == 0)&&(event->xbutton.button == Button1)) 

			{

			modo = 1; // pone el modo arrastrar

			min_x = event->xbutton.x;

			temp_x = min_x;

			}

		}

	else if (event->xany.type == ButtonRelease) 

		{

		if ((event->xbutton.button == Button1)&&(modo == 1)) 

			{

			modo = 0; // termina el modo arrastrar

			max_x = event->xbutton.x;

			// Calcula el canal asociado a ese pixel

			int pos_min = GetChannel(min_x);

			int pos_max = GetChannel(max_x);

			if (pos_min < 0) pos_min = 0;

			if (pos_max >= Datos.numCanales) pos_max = Datos.numCanales - 1;

		

			

			if (abs(max_x - min_x)/pixDensX > 5) 

				{

				// arrastre minimo de 5mm para ser un zoom

				if (pos_min > pos_max) 

					{

					temp = pos_min;

					pos_min = pos_max;

					pos_max = temp;

					}



				/*

				XmString cadena;

				char buffer[100];

				sprintf(buffer, "%d counts", Datos.DaTotalCuentas(pos_min, pos_max+1));//+1?

   				cadena = XmStringCreateLocalized (buffer);

				XtVaSetValues (label_total_counts, XmNlabelString, cadena, NULL);

				XmStringFree (cadena);

				*/

					

				int nuevoCursor = (pos_max+pos_min)/2;

				if ((nuevoCursor >= 0)&&(nuevoCursor < Datos.numCanales))

					CambiaPosicionCursor(nuevoCursor + 1);

				MarcaZoom(pos_min+1, pos_max+1);

				// No se borra la seleccion ya que se debera volver a redibujar

				// la barra horizontal al aplicar el nuevo zoom

				}

			else 

				{

			   XtVaGetValues(h_regla_grafica, XmNwidth, &ancho, XmNheight, &alto, NULL);

				// Borra el anterior

				int temp_val_min = min_x;

				if (min_x > temp_x) temp_val_min = temp_x;

			   XFillRectangle (dpy, XtWindow(h_regla_grafica), RubberZoomX, temp_val_min, 1, abs(temp_x-min_x), alto-2);

				}

			}

		}

	}





		/******************************

		 *   ZOOMS/ESCALA CALLBACKs   *

		 ******************************/



//*********************** CAMBIA AUTO ESCALA *****************

void CambiaAutoEscala(Widget widget, XtPointer client_data, XtPointer call_data) 

   	{

   	XmToggleButtonCallbackStruct *state = (XmToggleButtonCallbackStruct *) call_data;

	printf("CAMBIA AUTO ESCALA\n");

   	if (state->set == 1) 

	   	{

		EstadoVisual.AutoEscalaY = true;

		// desactiva el zoom Y

	   	XtSetSensitive(zoom_y_graf, false);

		// desactiva el scroll Y

		XtSetSensitive(v_grafica, false);

		AutoAjusteY();

		PintaGrafica(2);

		}

	else 

		{

		EstadoVisual.AutoEscalaY = false;

		// activa el zoom Y

   		XtSetSensitive(zoom_y_graf, true);

		// activa el scroll Y

	   	XtSetSensitive(v_grafica, true);

		}

	}



void Pon_Escala(Widget widget, XtPointer client_data, XtPointer call_data) {

   XmToggleButtonCallbackStruct *state = (XmToggleButtonCallbackStruct *) call_data;



	printf("PON ESCALA\n");

   if (state->set) {

	   escala_graf = (int) client_data;

		Graficas.ChangeScale(escala_graf);

		EstadoVisual.EscalaY = escala_graf - 1;

		if (XmToggleButtonGetState(auto_ajuste_graf))

			AutoAjusteY();

		PintaGrafica(2);

	}

}





void Pon_Zoom(Widget widget, XtPointer client_data, XtPointer call_data) {

   XmScaleCallbackStruct *cbs = (XmScaleCallbackStruct *) call_data;

//   printf("PON ZOOM\n");

   // pone en la etiqueta el nuevo zoom seleccionado

   if (widget == zoom_x_graf) 

	   {

	   Pon_Zoom_X(cbs->value);

		// Si esta activo el autozoom, hace el ajuste del zoom Y

		if (XmToggleButtonGetState(auto_ajuste_graf)) 

			{

			AutoAjusteY();

			// Actualiza los datos

			PintaGrafica(3);

			}

		else

			PintaGrafica(1);

		}

   else

	   {

	   Pon_Zoom_Y(cbs->value);

	   PintaGrafica(2);

	   }

}



//********************* DESPLAZA GRÁFICA ********************

// Se activa cuando se arrastra la barra de desplazamiento de las x

void Desplaza_Grafica(Widget widget, XtPointer client_data, XtPointer call_data) 

	{

    XmScrollBarCallbackStruct *cbs = (XmScrollBarCallbackStruct *) call_data;

    int valor = cbs->value;

	

   	if (widget == h_grafica) 

		{

		// calcula el canal mas a la izquierda que se ve

		double temp = (double)(valor-1)/(double)(h_grafica_ancho_total-1);

		temp *= (double)Datos.numCanales;

		

		// Actualiza el valor del tapiz

		Graficas.SetMinX(temp+1);

		if (cbs->reason != XmCR_DRAG) 

			{

			// Si no es un arrastre de barra comprueba si esta activa la

			// autoescala

			if (XmToggleButtonGetState(auto_ajuste_graf)) 

				{

				AutoAjusteY();

				PintaGrafica(3);

				}

			else

				PintaGrafica(1);

			}

		else PintaGrafica(1);



		int ancho = (int) (h_grafica_ancho_total / Graficas.GetZoomX());

		if (ancho == h_grafica_ancho_total) ancho = h_grafica_ancho_total-1;

		double minimo = valor * (double)Datos.numCanales/(double)h_grafica_ancho_total;

		double maximo = (double)(valor+ancho)*(double)Datos.numCanales / (double)h_grafica_ancho_total;

		char buffer[100];

		sprintf(buffer, "%d counts", Datos.DaTotalCuentas((int)(minimo+1), (int)(maximo)));

		XmString cadena = XmStringCreateLocalized (buffer);

		XtVaSetValues (label_total_counts, XmNlabelString, cadena, NULL);

		XtManageChild(label_total_counts);

		XmStringFree (cadena);

   		}

   else 

	   {

		// calcula el valor mas bajo que se ve

		double temp = (double)(valor-1)/(double)(v_grafica_alto_total-1);

		switch (escala_graf) 

			{

			case 2:

				temp *= sqrt((double)Datos.maxTotal)-sqrt((double)Datos.minTotal);

				temp += sqrt((double)Datos.minTotal);

				temp *= temp;

				break;

			case 3:

				temp *= log010((double)Datos.maxTotal)-log010((double)Datos.minTotal);

				temp += log010((double)Datos.minTotal);

				temp = pow(10.0, temp);

				break;

			default:

				temp *= (double)(Datos.maxTotal-Datos.minTotal);

				temp += (double)Datos.minTotal;

				break;

			}

		// Actualiza el valor del tapiz

		Graficas.SetMinY(temp);

		PintaGrafica(2);

   		}

	}







/************************************************************************

 ***

 ***			              VENTANA DE DATOS

 ***

 ************************************************************************/

// ***** VENTANA FICHERO DATOS*****

void CreaVentanaFicheroDatos() {
	Arg args[5];

	int i;

	XmString cadena;

	if (!dialog_fichero_datos_graf) {

		i = 0;

	   	XtSetArg (args[i], XmNtitle, "Spectrum data file"); i++;

	   	XtSetArg (args[i], XmNallowShellResize, true); i++;

	   	XtSetArg (args[i], XmNmwmDecorations, MWM_DECOR_BORDER|MWM_DECOR_TITLE|MWM_DECOR_MENU); i++;

	   	XtSetArg (args[i], XmNmwmFunctions, MWM_FUNC_MOVE|MWM_FUNC_CLOSE); i++;

		dialog_fichero_datos_graf = XmCreateFormDialog(app_shell, "dialog_fichero_datos_graf", args, i);

		
		i = 0;

		row_fichero_datos_graf = XmCreateRowColumn (dialog_fichero_datos_graf, "row_fichero_datos_graf", NULL, 0);

		XtVaSetValues (row_fichero_datos_graf, XmNpacking, XmPACK_COLUMN,

					XmNorientation, XmVERTICAL,

					NULL);

	   	
		i = 0;

		file_rowcol = XmCreateRowColumn (dialog_fichero_datos_graf, "file_rowcol", NULL, 0);

		XtVaSetValues (file_rowcol, XmNpacking, XmPACK_COLUMN,

					XmNorientation, XmVERTICAL,

					NULL);

	
		i = 0;

		cadena = XmStringCreate ("Spectrum data file created", "BoldNormal");

	   	XtSetArg (args[i], XmNlabelString, cadena); i++;

	   	//XtSetArg (args[i], XmNfontList, lista_fuentes); i++;

	   	label_file_datos_graf = XmCreateLabel (row_fichero_datos_graf, "label_file_canal_datos_graf", args, i);

	   	//XmStringFree (cadena);

	   	XtManageChild (label_file_datos_graf);
	

		XtVaSetValues (row_fichero_datos_graf,

			XmNtopAttachment, XmATTACH_FORM,

			XmNleftAttachment, XmATTACH_FORM,

			NULL);

		XtManageChild(row_fichero_datos_graf);



		XtVaSetValues (file_rowcol,

			XmNtopAttachment, XmATTACH_FORM,

			XmNleftAttachment, XmATTACH_WIDGET, XmNleftWidget, row_fichero_datos_graf,

			XmNleftOffset, 10,

			NULL);

		XtManageChild(file_rowcol);

	

		
		
	  	XtAddCallback (XtParent(dialog_datos_graf), XmNpopdownCallback, QuitaVentanaFicheroDatos, NULL);
		//XmFontListFree (lista_fuentes);


}


}

// *****  CREA VENTANA DATOS  *****

//Crea la ventana con los datos que aparecen según el punto en el que pinchemos de la gráfica,

//si está seleccionada la opción "Data" de la derecha.

//dialog_datos_graf es el widget que se encarga de dicha ventana.

void CreaVentanaDatos() {

	Arg args[5];

	int i;

	XmString cadena;



	if (!dialog_datos_graf) {

		i = 0;

	   XtSetArg (args[i], XmNtitle, titulo_datos_graf); i++;

	   XtSetArg (args[i], XmNallowShellResize, true); i++;

	   XtSetArg (args[i], XmNmwmDecorations, MWM_DECOR_BORDER|MWM_DECOR_TITLE|MWM_DECOR_MENU); i++;

	   XtSetArg (args[i], XmNmwmFunctions, MWM_FUNC_MOVE|MWM_FUNC_CLOSE); i++;

		dialog_datos_graf = XmCreateFormDialog(app_shell, "dialog_datos_graf", args, i);



		i = 0;

		row_datos_graf = XmCreateRowColumn (dialog_datos_graf, "row_datos_graf", NULL, 0);

		XtVaSetValues (row_datos_graf, XmNpacking, XmPACK_COLUMN,

					XmNorientation, XmVERTICAL,

					NULL);

		

		//Otra columna para los valores.

		i = 0;

		rowcol = XmCreateRowColumn (dialog_datos_graf, "rowcol", NULL, 0);

		XtVaSetValues (rowcol, XmNpacking, XmPACK_COLUMN,

					XmNorientation, XmVERTICAL,

					NULL);



	   cadena = XmStringCreate ("Ch:", "BoldNormal");

	   i = 0;

	   XtSetArg (args[i], XmNlabelString, cadena); i++;

	   XtSetArg (args[i], XmNfontList, lista_fuentes); i++;

	   label_canal_datos_graf = XmCreateLabel (row_datos_graf, "label_canal_datos_graf", args, i);

	   XmStringFree (cadena);

	   XtManageChild (label_canal_datos_graf);



	   cadena = XmStringCreate ("Ct:", "BoldNormal");

	   i = 0;

	   XtSetArg (args[i], XmNlabelString, cadena); i++;

	   XtSetArg (args[i], XmNfontList, lista_fuentes); i++;

	   label_cuentas_datos_graf = XmCreateLabel (row_datos_graf, "label_cuentas_datos_graf", args, i);

	   XmStringFree (cadena);

	   XtManageChild (label_cuentas_datos_graf);



	   cadena = XmStringCreate ("Energy:", "BoldNormal");

	   i = 0;

	   XtSetArg (args[i], XmNlabelString, cadena); i++;

	   XtSetArg (args[i], XmNfontList, lista_fuentes); i++;

	   label_energia_datos_graf = XmCreateLabel (row_datos_graf, "label_energia_datos_graf", args, i);

	   XmStringFree (cadena);

	   XtManageChild (label_energia_datos_graf);



	   cadena = XmStringCreate ("Sigma:", "BoldNormal");

	   i = 0;

	   XtSetArg (args[i], XmNlabelString, cadena); i++;

	   XtSetArg (args[i], XmNfontList, lista_fuentes); i++;

	   label_FWHG_datos_graf = XmCreateLabel (row_datos_graf, "label_FWHG_datos_graf", args, i);

	   XmStringFree (cadena);

	   XtManageChild (label_FWHG_datos_graf);



	   cadena = XmStringCreate ("Tau:", "BoldNormal");

	   i = 0;

	   XtSetArg (args[i], XmNlabelString, cadena); i++;

	   XtSetArg (args[i], XmNfontList, lista_fuentes); i++;

	   lb_tau_datos_graf = XmCreateLabel (row_datos_graf, "lb_tau_datos_graf", args, i);

	   XmStringFree (cadena);

	   XtManageChild (lb_tau_datos_graf);



//

	   cadena = XmStringCreateLocalized ("");

	   i = 0;

	   XtSetArg (args[i], XmNlabelString, cadena); i++;

	   XtSetArg (args[i], XmNfontList, lista_fuentes); i++;

	   lcanalValor = XmCreateLabel (rowcol, "l", args, i);

	   XmStringFree (cadena);

	   XtManageChild (lcanalValor);



	   cadena = XmStringCreateLocalized ("");

	   i = 0;

	   XtSetArg (args[i], XmNlabelString, cadena); i++;

	   XtSetArg (args[i], XmNfontList, lista_fuentes); i++;

	   lcuentasValor = XmCreateLabel (rowcol, "l", args, i);

	   XmStringFree (cadena);

	   cadena = XmStringCreateLocalized ("");

	   XtManageChild (lcuentasValor);

	   

	   i = 0;

	   XtSetArg (args[i], XmNlabelString, cadena); i++;

	   XtSetArg (args[i], XmNfontList, lista_fuentes); i++;

	   lenergiaValor = XmCreateLabel (rowcol, "l", args, i);

	   XmStringFree (cadena);

	   cadena = XmStringCreateLocalized ("");

	   XtManageChild (lenergiaValor);

	   

	   i = 0;

	   XtSetArg (args[i], XmNlabelString, cadena); i++;

	   XtSetArg (args[i], XmNfontList, lista_fuentes); i++;

	   lsigmaValor = XmCreateLabel (rowcol, "l", args, i);

	   XmStringFree (cadena);

	   cadena = XmStringCreateLocalized ("");

	   XtManageChild (lsigmaValor);

	   

	   i = 0;

	   XtSetArg (args[i], XmNlabelString, cadena); i++;

	   XtSetArg (args[i], XmNfontList, lista_fuentes); i++;

	   ltauValor = XmCreateLabel (rowcol, "l", args, i);

	   XmStringFree (cadena);

	   XtManageChild (ltauValor);



	//Colocamos las columnas de datos

	XtVaSetValues (row_datos_graf,

		XmNtopAttachment, XmATTACH_FORM,

		XmNleftAttachment, XmATTACH_FORM,

		NULL);

	XtManageChild(row_datos_graf);



	XtVaSetValues (rowcol,

		XmNtopAttachment, XmATTACH_FORM,

		XmNleftAttachment, XmATTACH_WIDGET, XmNleftWidget, row_datos_graf,

		XmNleftOffset, 10,

		NULL);

	XtManageChild(rowcol);

	

	XtAddCallback (XtParent(dialog_datos_graf), XmNpopdownCallback, QuitaVentanaDatos, NULL);

	XmFontListFree (lista_fuentes);

	}

}

// *****************



// *****  ACTIVACION ZONAS  *****

void ActivaEnergiaDatos() 

	{

	if (dialog_datos_graf) 

		{

		XtManageChild (label_energia_datos_graf);

		printf("llamado en ACTIVAENERGIADATOS\n");

		if (Datos.numCanales > 0) PonPosicionCursor();

		}

	}



void DesactivaEnergiaDatos() 

	{

	if (dialog_datos_graf)	XtUnmanageChild (label_energia_datos_graf);

	}



void ActivaPreCalibDatos() 

	{

	if (dialog_datos_graf) 

		{

		XtManageChild (label_energia_datos_graf);

		XtManageChild (label_FWHG_datos_graf);

		printf("llamado en ACTIVAPRECALIBDATOS\n");

		if (Datos.numCanales > 0) PonPosicionCursor();

		}

	}



void DesactivaPreCalibDatos() 

	{

	if (dialog_datos_graf) 

		{

		XtUnmanageChild (label_energia_datos_graf);

		XtUnmanageChild (label_FWHG_datos_graf);

		}

	}



void ActivaFormaDatos() 

	{

	if (dialog_datos_graf) 

		{

		XtManageChild (label_FWHG_datos_graf);

		XtManageChild (lb_tau_datos_graf);

		if (Datos.numCanales > 0) PonPosicionCursor();

		}

	}



void DesactivaFormaDatos() 

	{

	if (dialog_datos_graf) 

		{

		XtUnmanageChild (label_FWHG_datos_graf);

		XtUnmanageChild (lb_tau_datos_graf);

		}

	}

// ****************



void QuitaVentanaDatos(Widget widget, XtPointer client_data, XtPointer call_data) 

	{

	XtVaSetValues(ventana_datos_graf, XmNset, false, NULL);

	EstadoVisual.VentanaDatos = false;

	}

//****************
void QuitaVentanaFicheroDatos(Widget widget, XtPointer client_data, XtPointer call_data) 

	{

	XtVaSetValues(ventana_fichero_datos_graf, XmNset, false, NULL);

	EstadoVisual.VentanaFicheroDatos = false;

	}

//**************** PON VENTANA FICHERO DATOS ***********************

void PonVentanaFicheroDatos(Widget widget, XtPointer client_data, XtPointer call_data) 

	{

   	XmToggleButtonCallbackStruct *state = (XmToggleButtonCallbackStruct *) call_data;

   	if (state->set == 1) 

		{

		// Pone la ventana de datos

		XtManageChild (dialog_fichero_datos_graf);

		EstadoVisual.VentanaFicheroDatos = true;

		

		if (Datos.numCanales > 0) 
			EscribeFicheroDatos();

		printf("EscribeFicheroDatos 3\n");

		}

	else 

		{

		// Quita la ventana de datos

		XtUnmanageChild (dialog_fichero_datos_graf);

		EstadoVisual.VentanaFicheroDatos = false;

		}

	}



//**************** PON VENTANA DATOS ***********************

void PonVentanaDatos(Widget widget, XtPointer client_data, XtPointer call_data) 

	{

   	XmToggleButtonCallbackStruct *state = (XmToggleButtonCallbackStruct *) call_data;

	

   	if (state->set == 1) 

		{

		// Pone la ventana de datos

		XtManageChild (dialog_datos_graf);

		EstadoVisual.VentanaDatos = true;

		

		if (Datos.numCanales > 0) PonPosicionCursor();

		}

	else 

		{

		// Quita la ventana de datos

		XtUnmanageChild (dialog_datos_graf);

		EstadoVisual.VentanaDatos = false;

		}

	}

//*********************** ESCRIBE FICHERO DE DATOS**********************

void EscribeFicheroDatos() 

   {

   //XmString cadena, s1, s2, s3;
   
   if (Datos.numCanales > 0)		{
	FILE * pFile;

	   int cuentas, cuentasfondo;
	   double valorcontinuo, incertcontinuo;
	   double valorsigma, incertsigma, valortau, incertau, valorenergia, incertenergia;
	   double valortotal;	
	   char str[80];
  	   strcpy (str, daFichero(Datos.nombre));

	//pFile = fopen ("datosespectro.txt", "w");
	pFile = fopen (strcat(str, ".txt"), "w");
	printf("fichero creado: %s\n", str);
	//pFile = fopen (Datos.nombre, "w");
	
	fprintf (pFile, "canal; cuentas; cuentasfondo; valorcontinuo;  incertcontinuo;  valorsigma; incertsigma; valortau; incertau; valorenergia; incertenergia; valortotal  \n");

	for (int canal=1; canal < Datos.numCanales + 1; canal++){

		cuentas = Datos.espectro[canal];
		cuentasfondo = 0;
		valorcontinuo = 0.0;
		incertcontinuo = 0.0;
		valorsigma = 0.0;
		incertsigma = 0.0;
		valortau = 0.0;
		incertau = 0.0;
		valorenergia = 0.0;
		incertenergia = 0.0;
		valortotal = 0.0;


		if (EstadoTrabajo.Continuo) 

			{ // datos del continuo

			valorcontinuo = Datos.continuo[canal];
			incertcontinuo = Datos.sigmaContinuo[canal];

			if (EstadoTrabajo.fondoCargado && Datos.cuentasFondo[canal]>0)
				cuentasfondo = Datos.cuentasFondo[canal];

			if (!EstadoTrabajo.CalibracionForma) {
				if (EstadoTrabajo.PreCalibracion)
					valorsigma = Datos.DaSigma((double)canal);
			}
			else {
				valorsigma = Datos.DaSigma((double)canal);
				incertsigma = Datos.DaIncerSigma((double) canal);
				valortau = Datos.DaTau((double)canal);
				incertau = Datos.DaIncerTau((double) canal);
			}
			if (!EstadoTrabajo.CalibracionEnergia)  {
				if (EstadoTrabajo.PreCalibracion) 
					valorenergia = Datos.DaEnergia((double)canal);
			}
			else  {
				valorenergia = Datos.DaEnergia((double)canal);
				incertenergia = Datos.DaIncerEnergia((double) canal);
				valortotal = Datos.total[canal];
			}

		} //del estado continuo

		fprintf (pFile, "%d; %d; %d; %.4f; %.4f; %.4f; %.4f; %.4f; %.4f; %.4f; %.4f; %.4f  \n", canal, cuentas, cuentasfondo, valorcontinuo,  incertcontinuo,  valorsigma, incertsigma, valortau, incertau, valorenergia, incertenergia, valortotal);

		} //del for
		printf("ha escrito el fichero \n");
		fclose (pFile);

	} //del if	
	printf("sale de EscribeFicheroDatos \n");
	printf("sale de EscribeFicheroDatos 2\n");
}

//************************************************************************

//*********************** PON POSICIÓN CURSOR **************************

// Se llama cuando se pincha en algún punto de la gráfica.

void PonPosicionCursor() 

   {

   XmString cadena, s1, s2, s3;

   char dato[80];

	   

   if ((dialog_datos_graf)&&(Datos.numCanales > 0)) 

		{

		//printf("Canal\n");

		int valor = CanalCursor+1;

//		int valor = CanalCursor;

	    sprintf(dato, "%d", valor);

		cadena = XmStringCreateLocalized (dato);

		XtVaSetValues (lcanalValor, XmNlabelString, cadena, NULL);

		XmStringFree(cadena);

		

		int cont= Datos.espectro[valor];

	    sprintf(dato, "%d", cont);

		cadena = XmStringCreateLocalized (dato);

			

		if (EstadoTrabajo.Continuo) 

			{ // datos del continuo

			double valor2 = Datos.continuo[valor];

			double incert = Datos.sigmaContinuo[valor];

			char numero[80];

			PresentaNumero(valor2, incert, numero);

//			sprintf(dato, " [%s]", numero);

			if(EstadoTrabajo.fondoCargado && Datos.cuentasFondo[valor]>0)	

				{

				sprintf(dato, "[%s + %.0f]", numero, Datos.cuentasFondo[valor]);

				printf("Cuentas fondo vale %f\n", Datos.cuentasFondo[valor]);

				}

			else			sprintf(dato, "[%s]", numero);

			s3 = XmStringCreateLocalized(dato);

			s1 = cadena;

			cadena = XmStringConcat(s1, s3);

			XmStringFree(s3);

			XmStringFree(s1);

			}

	   XtVaSetValues (lcuentasValor, XmNlabelString, cadena, NULL);

	   XmStringFree (cadena);

	

		// Parametros de forma en energia

		int canal = CanalCursor+1;

		double val_ener, val_incer;

		// Sigma en energia

		//printf("Sigma\n");

	   			

		sprintf(dato, "");

		s1 = XmStringCreateLocalized (" keV");

		if (!EstadoTrabajo.CalibracionForma) 

			{

			if (EstadoTrabajo.PreCalibracion) 

				{

				val_ener = Datos.DaSigma((double)canal);

				sprintf(dato, "%.4f", val_ener);

				}

			else	s1=XmStringCreateLocalized("");

			}

		else

			{

			val_ener = Datos.DaSigma((double)canal);

			val_incer = Datos.DaIncerSigma((double) canal);

			if (val_incer == 0.0) sprintf(dato, "%.4f", val_ener);

			else PresentaNumero(val_ener, val_incer, dato);

			}

		s2 = XmStringCreateLocalized (dato);

		cadena = XmStringConcat (s2,s1);

		XtVaSetValues (lsigmaValor, XmNlabelString, cadena, NULL);

	    XmStringFree (cadena); XmStringFree(s1); XmStringFree(s2);



		// Tau

		//printf("Tau\n");

		sprintf(dato, "");

		if (EstadoTrabajo.CalibracionForma) 

			{

			val_ener = Datos.DaTau((double)canal);

			val_incer = Datos.DaIncerTau((double) canal);

			if (val_incer == 0.0) sprintf(dato, "%.4f", val_ener);

			else PresentaNumero(val_ener, val_incer, dato);

			s1 = XmStringCreateLocalized (" keV");

			}

		else	s1=XmStringCreateLocalized("");

		s2 = XmStringCreateLocalized (dato);

		//XmStringFree(s1);

		cadena = XmStringConcat (s2,s1);

		XtVaSetValues (ltauValor, XmNlabelString, cadena, NULL);

	    XmStringFree (cadena); XmStringFree(s1); XmStringFree(s2);



   		// Energia

		//printf("Energía calculada sobre el canal %d, con %d cuentas\n", canal, valor);

		sprintf(dato, "");

		s1 = XmStringCreateLocalized (" keV");

		if (!EstadoTrabajo.CalibracionEnergia) 

			{

			if (EstadoTrabajo.PreCalibracion) 

				{

				val_ener = Datos.DaEnergia((double)canal);

				sprintf(dato, "%.3f", val_ener);

				}

			else s1 = XmStringCreateLocalized ("");

			}

		else 

			{

			val_ener = Datos.DaEnergia((double)canal);

			val_incer = Datos.DaIncerEnergia((double) canal);

			if (val_incer == 0.0) sprintf(dato, "%.3f", val_ener);

			else PresentaNumero(val_ener, val_incer, dato);

//			printf("VAL INCER: %f\n", val_incer);

			}

		s2 = XmStringCreateLocalized (dato);

		cadena = XmStringConcat (s2, s1);

		XtVaSetValues (lenergiaValor, XmNlabelString, cadena, NULL);

	    XmStringFree (cadena); XmStringFree(s1); XmStringFree(s2);



		}//Del if

	//printf("Fin de PonPosicionCursor\n");

	}



void EstableceVentanaDatos(bool valor) 

	{

	XmToggleButtonSetState(ventana_datos_graf, valor, true);

	}


void EstableceVentanaFicheroDatos(bool valor) 

	{

	XmToggleButtonSetState(ventana_fichero_datos_graf, valor, true);

	}



/******************************************************************************

 ******

 ******                             VISUALIZACIONES

 ******

 ******************************************************************************/



void EstableceAutoEscalaY(bool valor) 

	{

	XmToggleButtonSetState(auto_ajuste_graf, valor, true);

	}



void EstableceEscalaY(int valor) 

	{

	if ((valor >= VISUAL_LINEAL)&&(valor <= VISUAL_LOGARITMO)) 

		{

		switch (escala_graf-1) 

			{

			case 0:				

				XmToggleButtonSetState(lineal_opc_graf, false, false);

				break;

			case 1:				

				XmToggleButtonSetState(raiz_opc_graf, false, false);

				break;

			case 2:				

				XmToggleButtonSetState(log_opc_graf, false, false);

				break;

			}

		switch (valor) 

			{

			case 0:				

				XmToggleButtonSetState(lineal_opc_graf, true, true);

				break;

			case 1:				

				XmToggleButtonSetState(raiz_opc_graf, true, true);

				break;

			case 2:				

				XmToggleButtonSetState(log_opc_graf, true, true);

				break;

			}

		}

	}
