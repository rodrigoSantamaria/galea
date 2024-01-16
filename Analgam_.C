#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include <unistd.h>

#include "avisos.h"
#include "analisis.h"
#include "calibracion.h"
#include "continuo.h"
#include "constDef.h"
#include "cursores.h"
#include "ficheros.h"
#include "fuentes.h"
#include "globales.h"
#include "preferences.h"
#include "math1.h"
#include "picos.h"
#include "trabajo.h"
#include "trabajoEst.h"
#include "VisActiv.h"
#include "VisAnal.h"
#include "VisCalib.h"
#include "VisConti.h"
#include "VisGraf.h"
#include <Xm/Xm.h>
#include <Xm/MainW.h>
#include <Xm/MwmUtil.h>
#include <Xm/Form.h>
#include <Xm/FileSB.h>
#include <Xm/Label.h>
#include <Xm/Protocols.h>
#include <Xm/PushB.h>
#include <Xm/List.h>
#include <Xm/Text.h>
#include <Xm/TextF.h>

#include <Xm/RowColumn.h>
#include <Xm/Frame.h>
#include <Xm/Label.h>
#include <Xm/LabelG.h>

#include <Xm/ComboBox.h>
#include <Xm/ToggleB.h>
#include <Xm/ToggleBG.h>
#include <Xm/Separator.h>


//Para manejo de ficheros
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

// objeto analisis de datos
cEspectros Datos;

// Base de datos Lara
cLaraBD LaraBD;

// Fichero de preferencias
cPreferences Preferences;

char *nombreFichFBD;
char *BDIdentificaciones = NULL;

int tipoFormato=1;//Por defecto elegido el formato de Canberra

// variables generales
char *TituloPrograma       = NULL; // Texto en el titulo de la ventana principal
char *NombreFicheroTrabajo = NULL; // nombre del fichero analgam con el que
				// se trabaja actualmente
XtAppContext app_context; // contexto general
Pixmap mainIcon; // icono de la aplicacion
Display *dpy;
// Variables de "recuerdo" de ultimo directorio visitado
char *UltAnalDirOpen = NULL, *UltSpecDirOpen = NULL,
	 *UltCalibDirOpen = NULL;


// Widgets
Widget grafica_WA, menubar;
Widget main_w, tapiz, app_shell;
Widget trabajo_WA;
Widget dlg_files; // para leer/escribir ficheros
Widget dlg_files_formato; // para leer ficheros de espectro


// Funciones
Widget CreaMenu(Widget);																																																																																																																//ctq
void ReseteaDatos();
void Salida();
void PideSalida (Widget, XtPointer, XtPointer);
						// Manejo de titulos
void PonTituloFichCambiado();
void PonTituloFichSinCambios();
						
						// Analisis
void analisis_cb (Widget, XtPointer, XtPointer);
void grabar_anal(Widget, XtPointer, XtPointer);
void cargarBDFondo(Widget, XtPointer, XtPointer);
void cargarFichIdentifica(Widget, XtPointer, XtPointer);
void imprimeFondos(double *, double *, double *, int);
void GuardaAnalisis(char*, int);
						// Calibracion
void calibracion_cb (Widget, XtPointer, XtPointer);
void grabar_calib(Widget, XtPointer, XtPointer);
void GuardaCalibracion(char*, int);
void leer_calib(Widget, XtPointer, XtPointer);
						// Ficheros
void Crea_dlgFiles();
void Crea_dlgFilesConFormato();
int GuardaFichero(const char*);
void ActualizarBDFondo(Widget, XtPointer, XtPointer);
void GuardarBDFondo(Widget, XtPointer, XtPointer);
int numeroColumnas(char *);	

// Ficheros Callbacks
void file_cb (Widget, XtPointer, XtPointer);
void hide_fsb (Widget, XtPointer, XtPointer);
void grabar_espectro(Widget, XtPointer, XtPointer);
void leer_espectro(Widget, XtPointer, XtPointer);
void leer_fondo(Widget, XtPointer, XtPointer);
void leer_bdfondo(Widget, XtPointer, XtPointer);
void leer_fichero(Widget, XtPointer, XtPointer);

//Funciones de ajuste de picos
Widget dlg_options;
void creaVentanaOpciones();
void pulsa_cancel_opciones(Widget, XtPointer, XtPointer); 
void pulsa_ok_opciones(Widget, XtPointer, XtPointer); 
void pulsarOpcion(Widget, XtPointer, XtPointer);
int maxOpciones=4;
bool opciones[]			 ={true, true, true, true};
bool opcionesAnteriores[]={true, true, true, true};
//bool opciones[]		   ={false, false, false, false};
//bool opcionesAnteriores[]={false, false, false, false};

//Función para el formato
void seleccionaFormato(Widget, XtPointer, XtPointer);

void *preparaFichero(char *nombre);

Widget dlgBD;
Widget txt_element, txt_energy;
Widget listaEntries;
Widget texto_entradas;
Widget dlgModify;
char *entradas[100];

char *entry=new char[50];
void file_cb (Widget, XtPointer, XtPointer);
void creaDlgInsercionBD(char **, int);
void pulsaAddEntries(Widget, XtPointer, XtPointer);
void modificaEntrada(Widget, XtPointer, XtPointer);
void pulsaOKModificaEntrada(Widget, XtPointer, XtPointer); 

void QuitaVentanaBD(Widget, XtPointer, XtPointer);
void IgnoraBD(Widget, XtPointer, XtPointer);
void IgnoraTodasBD(Widget, XtPointer, XtPointer);
void addColumns(char *nombre);
void addNuevasEntradas(char **, int);
void addMedia(char *, int, int);
char *nombre=NULL;

float energiaMedia(LinkedList *);


/******************************************************************************
 ****                               MAIN
 ******************************************************************************/

int main (int argc, char *argv[])
{
   Arg		args[4];
   Cardinal numArgs = 0;
   Atom  	WM_DELETE_WINDOW;

	printf("========================= GALEA ==============================\n");
	printf("Establecemos lenguaje\n");
   	XtSetLanguageProc (NULL, NULL, NULL);
	printf("Abrimos ventana\n");
	app_shell=NULL;
	app_context=NULL;
	printf("Argc es %d\n", argc);
	printf("Argv[0] es %s\n", argv[0]);
   app_shell = XtVaOpenApplication (&app_context, "App_Class", NULL, 0, &argc,
   			argv, NULL, sessionShellWidgetClass, NULL);
   printf("Tomamos display\n");
	dpy = XtDisplay(app_shell);

	// Carga el fichero de configuracion personal
	printf("Cargamos preferencias\n");
	Preferences.Load(dpy, app_shell);

	printf("Dejamos preparado el fichero de naturlib con comas o puntos\n");
	char *directorio=Preferences.GetCentroidDBDir();
	string cadDB(directorio);
	delete [] directorio;
	cadDB=cadDB+"naturlib.db";
	cout << "Nuestro fichero naturlib.db es " << cadDB << endl;
//	cadDB=cadDB+"mglib.db";
	BDIdentificaciones=new char[cadDB.length()+1];
	strcpy(BDIdentificaciones, cadDB.c_str());
	preparaFichero((char *)cadDB.c_str());
	printf("Terminado todo lo de naturlib, vamos a lara\n");
	
	// Carga la base de datos de elementos y emisiones
	char *fileLaraBD = Preferences.GetLaraDBFile();
	printf("Fichero lara: %s\n", fileLaraBD);
	LaraBD.Load(fileLaraBD);
	delete [] fileLaraBD;
	
	printf("Cargada lara, tomando icono\n");
	char *file=Preferences.GetInstallDir();
	string cad(file);
	delete [] file;
	cad=cad+"logobueno.xpm";
	
	cout << "Nombre Icono: " << cad << endl;
	Screen *screen = XtScreen(app_shell);
	mainIcon = XmGetPixmap(screen, (char *)cad.c_str(),
				BlackPixelOfScreen(screen), WhitePixelOfScreen(screen));

	XtVaSetValues(app_shell,
				XmNtitle, APPLICATION_TITLE, XmNiconName, APPLICATION_TITLE,
				XmNiconPixmap, mainIcon, 
				XmNmwmDecorations, MWM_DECOR_ALL,
				XmNmwmFunctions, MWM_FUNC_ALL,
				XmNminWidth, 680, XmNminHeight, 550,
	   			XmNdeleteResponse, XmDO_NOTHING, 
				NULL);
	
	
	// aplica un callback para la salida del programa
	WM_DELETE_WINDOW = XInternAtom (dpy, "WM_DELETE_WINDOW", false);
	XmAddWMProtocolCallback (app_shell, WM_DELETE_WINDOW, PideSalida, NULL);
   
   	
	// Carga las fuentes
	CargaFuentes(dpy);

	// Crea la ventana principal
   	main_w = XmCreateMainWindow (app_shell, "main_w", args, numArgs);

	// Crea la barra de menu
	menubar = CreaMenu(main_w);
	XtManageChild (menubar);

	// Crea el cuadro de dialogo para tratar con ficheros
	Crea_dlgFiles();
	Crea_dlgFilesConFormato();
	
	// Crea el tapiz para la zona de trabajo
	tapiz = XmCreateForm (main_w, "tapiz", NULL, 0);

	// Crea el area de trabajo
	trabajo_WA = Crea_AreaTrabajo(tapiz);
	      
   // Crea el visor de graficas
   grafica_WA = Crea_VisorGrafica(tapiz);
   DesactivaGrafica();
   DesactivaTrabajo();


	// coloca las areas de trabajo y representacion
   XtVaSetValues (trabajo_WA,
      XmNrightAttachment, XmATTACH_FORM,
		XmNleftAttachment, XmATTACH_FORM,
		XmNtopAttachment, XmATTACH_FORM,
		NULL);
	printf("_______________ CAMBIANDO LOS VALORES DEL ÁREA DE TRABAJO DE LA GRÁFICA \n\n\n");
   XtVaSetValues (grafica_WA,
      XmNrightAttachment, XmATTACH_FORM,
		XmNleftAttachment, XmATTACH_FORM,
		XmNbottomAttachment, XmATTACH_FORM,
		XmNtopAttachment, XmATTACH_WIDGET, 
		XmNtopWidget, trabajo_WA,
		NULL);
	   
   XtManageChild (tapiz);
   XtManageChild (main_w);
	
	// Inicializa el cursor
	PreparaCursores();
   
   // Inicializa el estado del trabajo
	InicializaEstadoTrabajo(EstadoTrabajo);
	// Opciones de visualizacion desactivadas
	InicializaEstadoVisual(EstadoVisual);
	EstableceVisual(EstadoVisual);

   // Finaliza la representacion y comienza el programa
   XtRealizeWidget (app_shell);
   XtAppMainLoop (app_context);
}

//**************** PON TITULO FICH CAMBIADO ****************
/* Cuando se realiza algún cambio en el fichero, se le pone un * al título
*/
void PonTituloFichCambiado() 
	{
	char nombre[strlen(TituloPrograma) + 3+7];
	sprintf(nombre, "Galea: %s *", TituloPrograma);
	XtVaSetValues(app_shell, XmNtitle, nombre, NULL);
	}

void PonTituloFichSinCambios() 
	{
	char *nombre = NULL;
	
	nombre = new char[strlen(TituloPrograma) + 1+7];
	sprintf(nombre, "Galea: %s", TituloPrograma);
	
	XtVaSetValues(app_shell, XmNtitle, nombre, NULL);
//	XtFree(nombre);
	delete [] nombre;
	}

//********************** RESETEA DATOS ***************************
void ReseteaDatos() 
	{
	// Borramos datos de la interfaz de actividad
	borraVisActiv();		
	
	// borra las listas
	tListaPrecalib* listaPrecalibracion = DevuelveListaPreCalibracion();
	borraLista(*listaPrecalibracion);
	ReconstruyeListaPrecalibracion();
	
	tListaCalibracion* listaCalibracion = DevuelveListaCalibracion();
	borraLista(*listaCalibracion);
	ReconstruyeListaCalibracion();
	tListaContinuo* listaContinuo = DevuelveListaContinuo();
	borraLista(*listaContinuo);
	ReconstruyeListaContinuo();
	BorraListaIntervCalibracion();
	tListaAnalisis* listaAnalisis = DevuelveListaAnalisis();
	borraLista(*listaAnalisis);
	ReconstruyeListaAnalisis();
	printf("Tras reconstruye lista Analisis en ReseteaDatos\n");
	//
	//printf("Creamos lista actividad en reseteaDatos\n");
	//creaListaActividad();
	//

	printf("iniciamos gráficas\n");
	// inicializa datos
	ReseteaGraficas();
	printf("borramos datos del espectro\n");
	Datos.BorraDatos();
	DesactivaFormaDatos();
	DesactivaEnergiaDatos();

	printf("Borramos gráficas\n");
	BorraGrafica();
	CambiaPosicionCursor(1);

	// inicializa estructuras de trabajo y visual
	InicializaEstadoTrabajo(EstadoTrabajo);
	InicializaEstadoVisual(EstadoVisual);

	// resetea los estados visuales
	EstableceVisual(EstadoVisual);
	}

void PideSalida (Widget widget, XtPointer client_data, XtPointer call_data) {
	Salida();
}

//*********************** SALIDA ***********************
/* Lleva a cabo la salida ordenada del programa, preguntando,
	si fuera necesario, si se quieren guardar cambios.
*/
void Salida() 
	{
	printf("SALIDA DEL PROGRAMA\n");
	// Comprueba si hay cambios que no se hayan guardado
	if (EstadoTrabajo.TrabajoCambios) 
		{
		// pregunta si quiere cancelar la salida, grabar cambios o salir
		int Resp = Continuar(dlg_files, (char *)ASK_EXITING, (char *)EXITING_TITLE);
		switch(Resp) 
			{
			case CANCELAR :
				return;
			case NO :
				break;
			case SI :
				if (NombreFicheroTrabajo != NULL)
					// el fichero ya existe, lo sobreescribe con los datos
					// mas recientes
					GuardaFichero(NombreFicheroTrabajo);
				else
					// el fichero oficial no existe, cancela la salida para que
					// el usuario lo grabe
					return;
				break;
			}
		}
	XmDestroyPixmap(XtScreen(app_shell), mainIcon);
	XtDestroyWidget(app_shell);
	//Liberamos información relativa al espectro
	
	// Libera otra memoria utilizada
	delete [] UltAnalDirOpen; delete [] UltSpecDirOpen;
	delete [] UltCalibDirOpen;
	
	//Liberamos memoria utilizada en alguna de las interfaces
	printf("Borramos la memoria de VisActiv\n");
	borraVisActiv();
	printf("Memoria borrada\n");
	
	printf("Retornamos\n");
	exit(0);
}

/******************************************************************************
 ****                               ANALISIS
 ******************************************************************************/
 
/********************** ANALISIS CB *************************
	Controla y redirige todas las opciones del menú de análisis.
	Actualmente esas opciones comprenden:
		 1) Opción de menú Save Analysis ...
			Para esta opción son las siguientes 3 funciones, que si todo va bien se van llamando
			en el orden en el que están escritas: analisis_cb -> grabar_anal -> guardaAnalisis
		 2) Opción de menú Load Efficiency Parameters ...
			Esta opción llama a funciones de VisActiv.C, en concreto a:
				pulsa_efficiency
					-pulsaOKFileEficiencia
						Datos.calculaEficiencia
					-pulsaCancelFile
		3) Opción de menú Load Background DataBase
*/
void analisis_cb (Widget widget, XtPointer client_data, XtPointer call_data) {
	int item_no = (int) client_data;
	XmString titulo, filtro;
	char *UltAnalDirOpen;		
	tListaAnalisis *lista;
	XmString patron;
			
	if (Datos.numCanales <= 0) 
		{
		Aviso(main_w, "You need to load a spectrum first.");
		return;
		}
	
	switch (item_no) 
		{
		case MENU_ANALYSIS_LOAD_EFFICIENCY :
			printf("CARGA DE LA EFICIENCIA\n");
			if (!EstadoTrabajo.Calibracion) 
				{
				Aviso(main_w, "There is no calibration yet");
				return;
				}
			XtRemoveAllCallbacks (dlg_files, XmNokCallback);
			if (XtIsManaged(dlg_files))
			XtUnmanageChild(dlg_files);
			
			titulo = XmStringCreateLocalized ((char *)LOAD_EFFICIENCY_TITLE);
			filtro = XmStringCreateLocalized ((char *)FILTER_FICH_EFFICIENCY);
			//Ponemos el fichero de preferencias
			UltAnalDirOpen = Preferences.GetEfficiencyDir();
			if (UltAnalDirOpen != NULL) 
				{
				patron = XmStringCreateLocalized(UltAnalDirOpen);
				XtVaSetValues (dlg_files, XmNdirectory, patron, NULL);
				XmStringFree (patron);
				}
			XtAddCallback (dlg_files, XmNokCallback, pulsaOKFileEficiencia, (XtPointer) 1);
			
			XtVaSetValues (dlg_files, XmNdialogTitle, titulo, XmNpattern, filtro, NULL);
			XmStringFree (titulo); XmStringFree (filtro);
			
			// Pone o substituye el menu de ficheros que haya
			XtManageChild (dlg_files);
			break;
		case MENU_ANALYSIS_LOAD_BACKGROUND :
			printf("CARGA DE FONDO\n");
			if (!EstadoTrabajo.Calibracion) 
				{
				Aviso(main_w, "There is no calibration yet");
				return;
				}
			XtRemoveAllCallbacks (dlg_files, XmNokCallback);
			if (XtIsManaged(dlg_files))
			XtUnmanageChild(dlg_files);
			
			titulo = XmStringCreateLocalized ((char *)LOAD_BACKGROUND_TITLE);
			filtro = XmStringCreateLocalized ((char *)FILTER_FICH_BDBACKGROUND);
			//Ponemos el fichero de preferencias
			UltAnalDirOpen = Preferences.GetBackBDDir();
			if (UltAnalDirOpen != NULL) 
				{
				patron = XmStringCreateLocalized(UltAnalDirOpen);
				XtVaSetValues (dlg_files, XmNdirectory, patron, NULL);
				XmStringFree (patron);
				}
			XtAddCallback (dlg_files, XmNokCallback, cargarBDFondo, (XtPointer) 1);
			
			XtVaSetValues (dlg_files, XmNdialogTitle, titulo, XmNpattern, filtro, NULL);
			XmStringFree (titulo); XmStringFree (filtro);
			
			// Pone o substituye el menu de ficheros que haya
			XtManageChild (dlg_files);
			break;
		case MENU_ANALYSIS_LOAD_IDENTIFICA :
			printf("CARGA DE FICHERO DE IDENTIFICACIONES\n");
			if (!EstadoTrabajo.Calibracion) 
				{
				Aviso(main_w, "There is no calibration yet");
				return;
				}
			XtRemoveAllCallbacks (dlg_files, XmNokCallback);
			if (XtIsManaged(dlg_files))
			XtUnmanageChild(dlg_files);
			
			titulo = XmStringCreateLocalized ((char *)LOAD_IDENTIFICA_TITLE);
			filtro = XmStringCreateLocalized ((char *)FILTER_FICH_IDENTIFICA);
			//Ponemos el fichero de preferencias
			UltAnalDirOpen = Preferences.GetCentroidDBDir();
			if (UltAnalDirOpen != NULL) 
				{
				patron = XmStringCreateLocalized(UltAnalDirOpen);
				XtVaSetValues (dlg_files, XmNdirectory, patron, NULL);
				XmStringFree (patron);
				}
			XtAddCallback (dlg_files, XmNokCallback, cargarFichIdentifica, (XtPointer) 1);
			
			XtVaSetValues (dlg_files, XmNdialogTitle, titulo, XmNpattern, filtro, NULL);
			XmStringFree (titulo); XmStringFree (filtro);
			
			// Pone o substituye el menu de ficheros que haya
			XtManageChild (dlg_files);
			break;
		case MENU_ANALYSIS_SAVE :
			lista = DevuelveListaAnalisis();
			if (lista->numElementos == 0) 
				{
				Aviso(main_w, "There is no analyzed peaks yet");
				return;
				}
			XtRemoveAllCallbacks (dlg_files, XmNokCallback);
			if (XtIsManaged(dlg_files))
			XtUnmanageChild(dlg_files);
			
			titulo = XmStringCreateLocalized ((char *)SAVE_ANALISIS_TITLE);
			filtro = XmStringCreateLocalized ((char *)FILTER_FICH_ANALISIS);
			XtAddCallback (dlg_files, XmNokCallback, grabar_anal, (XtPointer) 1);
			
			XtVaSetValues (dlg_files, XmNdialogTitle, titulo, XmNpattern, filtro, NULL);
			XmStringFree (titulo); XmStringFree (filtro);
			
			// Pone o substituye el menu de ficheros que haya
			XtManageChild (dlg_files);
			break;
		case MENU_ANALYSIS_DELETE_PEAKS :
			deleteAllPeaks();
			break;
		case MENU_ANALYSIS_OPTIONS :
			creaVentanaOpciones();
			XtManageChild(dlg_options);
			break;
		}
}
//********************* CARGAR FICH IDENTIFICA *****************
/*	Carga un fichero para identificaciones automáticas. Por defecto
se encuentra cargado naturlib.db
	Los ficheros de identificación de emisiones de radionúclidos tienen
extensión .db
*/
void cargarFichIdentifica(Widget widget, XtPointer client_data, XtPointer call_data) 
	{
	XtUnmanageChild(dlg_files);
  	char *nombre = NULL, *dir = NULL;
	XmFileSelectionBoxCallbackStruct *cbs = (XmFileSelectionBoxCallbackStruct *) call_data;
	printf("PULSA OK FICH IDENTIFICA\n");
   	XmStringGetLtoR(cbs->value, XmFONTLIST_DEFAULT_TAG, &nombre);
   	XmStringGetLtoR(cbs->dir, XmFONTLIST_DEFAULT_TAG, &dir);
	printf("%s,%s\n", dir, nombre);
	//if (BDIdentificaciones!= NULL) //delete[] BDIdentificaciones;
	//	BDIdentificaciones=NULL;
	BDIdentificaciones=new char[strlen(nombre)];
	strcpy(BDIdentificaciones, nombre );
	printf("BDIdentificaciones es: %s\n", BDIdentificaciones);

	}
//********************* CARGAR BD FONDO ************************
/*	Carga una BD de fondo, calculando la media de todos sus valores y 
	añadiéndolo al continuo, ademas de calcular las áreas de los picos
	teniendo en cuenta este fondo
*/
void cargarBDFondo(Widget widget, XtPointer client_data, XtPointer call_data) 
	{
	XtUnmanageChild(dlg_files);
   	char *nombre = NULL, *dir = NULL;
	XmFileSelectionBoxCallbackStruct *cbs = (XmFileSelectionBoxCallbackStruct *) call_data;
	printf("PULSA OK BD FONDO\n");
   	XmStringGetLtoR(cbs->value, XmFONTLIST_DEFAULT_TAG, &nombre);
   	XmStringGetLtoR(cbs->dir, XmFONTLIST_DEFAULT_TAG, &dir);
	
	double *energias, *medias, *incertidumbres;
	int numEnergias=numeroFilas(nombre)-1;//La primera fila son títulos identificativos
	int numColumnas=numeroColumnas(nombre);

	medias=new double[numEnergias];
	incertidumbres=new double[numEnergias];
	energias=new double[numEnergias];
	if(medias==NULL || incertidumbres ==NULL || energias==NULL)
		{
		printf("ERROR: No hay memoria para reservar double[%d]\n", numEnergias);
		return;
		}
	preparaFichero(nombre);
	calculaMediaBDFondo(nombre, numEnergias, numColumnas, energias, medias, incertidumbres);
	Datos.calculaFondo(medias, incertidumbres, energias, numEnergias);
	//Una vez calculada la media, pasamos a calcular el área de los picos
//	calculaAreaPicos(energias, medias, incertidumbres, numEnergias);
	Datos.restaAreaFondo();
	imprimeFondos(energias, medias, incertidumbres, numEnergias);
	EstadoTrabajo.fondoCargado=true;
	nombreFichFBD=nombre;
	
	delete [] medias;
	delete [] energias;
	delete [] incertidumbres;
	}
	
//********************** IMPRIME FONDOS *********************
void imprimeFondos(double *ene, double *med, double *inc, int numEnergias)
	{
	// CREAMOS LA INTERFAZ 
	Arg args[7];
	Cardinal i=0;
	Widget listaBg, col, frameAct, f1;
	double e, a, sa;
	char texto[200];
	XmString cad;
	XmString cadena;
	Widget dlgBg;
		
	printf("MOSTRAMOS LOS DATOS OBTENIDOS PARA LOS FONDOS\n");
	printf("area para el 0 es %f(%f)\n", med[0], inc[0]);
	
	Cardinal k = 0;
	XtSetArg (args[k], XmNtitle, "Background"); k++;
	XtSetArg (args[k], XmNmwmDecorations, MWM_DECOR_BORDER|MWM_DECOR_TITLE|MWM_DECOR_MENU); k++;
	XtSetArg (args[k], XmNmwmFunctions, MWM_FUNC_MOVE|MWM_FUNC_CLOSE); k++;
	XtSetArg (args[k], XmNbaseWidth, 400); k++;
	XtSetArg (args[k], XmNbaseHeight, 300); k++;
	XtSetArg (args[k], XmNmaxWidth, 400); k++;
	XtSetArg (args[k], XmNmaxHeight, 300); k++;
	
	dlgBg = XmCreateFormDialog(app_shell, "dlgBg", args, k);
	
	k=0;
	XtSetArg (args[k], XmNbaseWidth, 400); k++;
	XtSetArg (args[k], XmNbaseHeight, 300); k++;
	XtSetArg (args[k], XmNmaxWidth, 400); k++;
	XtSetArg (args[k], XmNmaxHeight, 300); k++;
	col=XmCreateRowColumn(dlgBg, "ColumnaBg", args, k);
	
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
    listaBg = XmCreateScrolledList(f1, "lista de fondos", args, k);
	
	//Imprimimos los valores en la lista
	for(int i=0; i<numEnergias;i++)
		{
		char numero[80];
		if(i==0)	{printf("area para el %d es %f(%f)\n", i, med[i], inc[i]);}
		PresentaNumero(med[i], inc[i], numero);
		sprintf(texto, "%.2f keV has peak of area %s", ene[i], numero);
		printf("%d:%s\n", i, texto);
		cadena=XmStringCreateLocalized(texto);
		XmListAddItemUnselected(listaBg, cadena, 0); 
		XmStringFree(cadena);
		}
	
	XtManageChild(listaBg);
	XtManageChild(col);
	XtManageChild(f1);
	XtManageChild(dlgBg);
	}
	
//************************ GRABAR ANAL *****************
void grabar_anal(Widget widget, XtPointer client_data, XtPointer call_data) 
	{
   	char *nombre = NULL, *dir = NULL;
   	FILE *fDatos; // puntero al fichero donde se graba
	XmFileSelectionBoxCallbackStruct *cbs = (XmFileSelectionBoxCallbackStruct *) call_data;


   	XmStringGetLtoR(cbs->value, XmFONTLIST_DEFAULT_TAG, &nombre);
   	XmStringGetLtoR(cbs->dir, XmFONTLIST_DEFAULT_TAG, &dir);
	
	// recupera la extension
	char *ext = DaExtensionFile(nombre, dir);
	char *extOficial = (char *)EXT_FICH_ANALISIS;
	switch ((int) client_data) 
		{
		case 1: extOficial = (char *)EXT_FICH_ANALISIS; break;
		}
	
	// comprueba si hay que agnadir la extension
	if ((ext == NULL)||(strcmp(ext, extOficial) != 0)) 
		{
		// hay que agnadir la extension del fichero
		char *otro = new char[strlen(nombre) + strlen(extOficial) + 1];
		if (otro == NULL) 
			{
			XtFree(nombre); XtFree(dir); delete [] ext;
			printf(ERROR_SAVE_FILE);
			printf(ERROR_NO_MEMORY);
			return;
			}
		strcpy(otro, nombre);
		strcat(otro, extOficial);
		XtFree(nombre);
		nombre = otro;
		}
	delete [] ext;
	
	// Comprueba si el fichero ya existe
	if ((fDatos = fopen(nombre, "r")) != NULL) 
		{
		fclose(fDatos);
		// Existe el fichero, pregunta antes de sobreescribirlo
		char pregunta[80];
		char *nombreCorto = DaNombreFile(nombre, dir);
		sprintf(pregunta, ASK_OVERWRITE, nombreCorto);
		delete [] nombreCorto;
		int Resp = Continuar(dlg_files, pregunta, (char *)OVERWRITE_TITLE);
		switch(Resp) 
			{
			case CANCELAR :
				XtUnmanageChild (widget);
			case NO :
				XtFree(nombre); XtFree(dir);
				return;
			}
		}
	
	// Graba el fichero
	GuardaAnalisis(nombre, (int) client_data);

	// quita el menu de grabar archivo
	XtUnmanageChild (widget);
	XtFree(nombre); XtFree(dir);
	}

//***************** GUARDA ANÁLISIS ******************
/*	Graba toda la información del análisis en un fichero de
	texto de la siguiente manera:
		Interval N
			[x,y] Kev	
			Found peaks: m
			Quality Adjust (Chi²): q
			Spectrum Area: sa(inc)
			Adjusted Area: aa(inc)
			Peak 1
				[Identified: Ge32 -> energíaKeV(prob)]
				Energy: e(inc) KeV
				Centroid: c(inc)
				Width: w(inc)
				Tau: t(inc)
				Height: h(inc)
				Area: a(inc)
			...
			Peak m
*/
void GuardaAnalisis(char* nombre, int tipo) 
	{
    FILE *fDatos; // puntero al fichero donde se graba

	// Abre un fichero para su escritura
	if ((fDatos = fopen(nombre, "w")) == NULL) 
		{
		printf(ERROR_SAVE_FILE);
		printf(ERROR_NO_SAVE_FILE);
		return;
		}

	tListaAnalisis *lista = DevuelveListaAnalisis();

	// Cabecera identificativa de resultados
	fprintf(fDatos, "Analysis Results File: %s\n\n", TituloPrograma);
	
	fprintf(fDatos, "Intervals adjusts: %d\n", lista->numElementos);
	fprintf(fDatos, "Time: %.0f s\n\n", Datos.tiempo);

	//Información sobre los ficheros auxiliares utilizados
	if(Datos.nombreEficiencia!=NULL)	
		fprintf(fDatos, "Efficiency parameters file used: %s\n", Datos.nombreEficiencia);
	if(Datos.nombreFondo!=NULL && EstadoTrabajo.fondoCargado)	
		fprintf(fDatos, "Background DataBase file used: %s\n\n", Datos.nombreFondo);

	for (int i = 1; i <= lista->numElementos; i++) 
		{
		double temp1, temp2;
		char numero[100];
		char numero2[100];
		NodoAnalisis *nodo = DevuelveNodo(*lista, i);
		NodoPicos *intervalo = DevuelveNodo(Datos.listaPicos, nodo->numIntervalo);
		fprintf(fDatos, "Interval %d\n", i);
		temp1 = Datos.DaEnergia(intervalo->limInferior);
		temp2 = Datos.DaEnergia(intervalo->limSuperior);
		fprintf(fDatos, "\t[%4d-%4d] KeV\n", redondea(temp1), redondea(temp2));
		fprintf(fDatos, "\tFound Peaks: %d\n", intervalo->picos.numElementos);
		fprintf(fDatos, "\tQuality Adjust (Chi^2): %.5f\n", intervalo->ChiSq);
		PresentaNumero(intervalo->Area, intervalo->sigArea, numero);
		fprintf(fDatos, "\tSpectrum Area: %s\n", numero);
		PresentaNumero(Datos.DaAreaIntervalo(nodo->numIntervalo),
							Datos.DaIncerAreaIntervalo(nodo->numIntervalo),
							numero);
		fprintf(fDatos, "\tAdjusted Area: %s\n", numero);
		for (int j = 1; j <= intervalo->picos.numElementos; j++) 
			{
			NodoPico *pico = DevuelveNodo(intervalo->picos, j);
			fprintf(fDatos, "\tPeak %d\n", j);
			if (pico->Ajuste.identificado) 
				{
				for(int k=0;k<pico->Ajuste.emisiones->numElements();k++)
					{
					NodoEmision *ne=(NodoEmision *)pico->Ajuste.emisiones->getNode(k);
					unsigned short int emision = ne->codElemento;
					unsigned short int elemento = LaraBD.GetEmissor(emision);
					char name[3];
					if(ne->codElemento==-1)//No está en lara, recurrir al texto guardado
						{
						fprintf(fDatos, "\t\tIdentified %dº: %s -> %.3f keV\n", k+1, ne->nombre, ne->Energia);
						}
					else
						{
						LaraBD.GetNameElement(elemento, name);
						PresentaNumero(LaraBD.GetEnergy(emision), LaraBD.GetIncEnergy(emision), numero);
						PresentaNumero(LaraBD.GetProbability(emision), LaraBD.GetIncProbability(emision), numero2);
						fprintf(fDatos, "\t\tIdentified %dº: %s%d -> %s keV (%s%%)\n", k+1, name,
							LaraBD.GetMassNumber(elemento), numero, numero2);
						}
					}
				//
					
				if(pico->Ajuste.ldCalculado)
					{
					fprintf(fDatos, "\t\tDetection Limit: %f\t", pico->Ajuste.ld);
					fprintf(fDatos, "\t\tDecision  Limit: %f\n", pico->Ajuste.lc);
					}
				if(pico->Ajuste.eficienciaCalculada)
					{	
					PresentaNumero(pico->Ajuste.eficiencia, pico->Ajuste.sigmaEficiencia, numero);						
					//fprintf(fDatos, "\t\tEfficiency: %f(%f)\n", pico->Ajuste.eficiencia, pico->Ajuste.sigmaEficiencia);
					fprintf(fDatos, "\t\tEfficiency: %s\n", numero);
					}
				if(pico->Ajuste.analizado)
					{
					//NOTA: Esta función presentaNumero se supone que deja las cosas más elegantes, pero no sé
					//		hasta qué punto redondea bien
					PresentaNumero(pico->Ajuste.correccion, pico->Ajuste.sigmaCorreccion, numero);
					fprintf(fDatos, "\t\tCorrection of efficiency: %s\n", numero);
					PresentaNumero(pico->Ajuste.actividad, pico->Ajuste.sigmaActividad, numero);
					if(EstadoTrabajo.AnalisisActividad)
						fprintf(fDatos, "\t\tActivity: %s Bq\n", numero);
					if(EstadoTrabajo.AnalisisMDA)
						{
						if(pico->Ajuste.mdaPadre==0)
							fprintf(fDatos, "\t\tMDA: %f Bq\n", pico->Ajuste.mda);
						else
							fprintf(fDatos, "\t\tMDA: %f Bq (%f)\n", pico->Ajuste.mda, pico->Ajuste.mdaPadre);
						}
					}
				}
			PresentaNumero(Datos.DaEnergia(pico->Ajuste.Centroide),
								Datos.DaIncerEnergia(pico->Ajuste.Centroide),
								numero);
			fprintf(fDatos, "\t\tEnergy: %s KeV\n", numero);
			PresentaNumero(pico->Ajuste.Centroide, pico->Ajuste.SigCentroide,
								numero);
			fprintf(fDatos, "\t\tCentroid: %s\n", numero);
			PresentaNumero(pico->Ajuste.Anchura, pico->Ajuste.SigAnchura,
								numero);
			fprintf(fDatos, "\t\tWidth: %s\n", numero);
			PresentaNumero(pico->Ajuste.Tau, pico->Ajuste.SigTau,
								numero);
			fprintf(fDatos, "\t\tTau: %s\n", numero);
			PresentaNumero(pico->Ajuste.Ytau, pico->Ajuste.SigYtau,
								numero);
			fprintf(fDatos, "\t\tHeight: %s\n", numero);
			//printf("TAY E Y TAU VALEN %f %f para pico %d\n", pico->Ajuste.Tau, pico->Ajuste.Ytau, j);
			PresentaNumero(Datos.DaAreaPico(nodo->numIntervalo,j),
								Datos.DaIncerAreaPico(nodo->numIntervalo,j),
								numero);
			fprintf(fDatos, "\t\tArea: %s\n", numero);
			if(pico->Ajuste.areaSinFondo!=0 || pico->Ajuste.incerAreaSinFondo!=0)
				{
				PresentaNumero(pico->Ajuste.areaSinFondo, pico->Ajuste.incerAreaSinFondo, numero);
				fprintf(fDatos, "\t\tNet Area: %s\n", numero);
				}
			}

		// finaliza el intervalo
		fprintf(fDatos, "\n");
		}
	
	// Cierra el fichero
	fclose(fDatos);
	}


/******************************************************************************
 ****                               CALIBRACIONES
 ******************************************************************************/

 // Menú de calibración.
 //Las opciones de abrir calibración llaman abrir_calib mientras que las de salvar
 //llaman a grabar_calib
 
void calibracion_cb (Widget widget, XtPointer client_data, XtPointer call_data) {
	XmString titulo, filtro;
	int item_no = (int) client_data;
	
	if (Datos.numCanales <= 0) {
		Aviso(main_w, "You need to load a spectrum first.");
		return;
	}
	
	XtRemoveAllCallbacks (dlg_files, XmNokCallback);
	if (XtIsManaged(dlg_files))
		XtUnmanageChild(dlg_files);

	switch (item_no) {
      // Adapta el menu de carga de ficheros
		case MENU_CALIBRATION_OPEN :
			titulo = XmStringCreateLocalized ((char *)LOAD_CAL_TITLE);
			filtro = XmStringCreateLocalized ((char *)FILTER_FICH_CAL);
			XtAddCallback (dlg_files, XmNokCallback, leer_calib, (XtPointer) 1);
			break;
		case MENU_CALIBRATION_OPEN_ENERGY :
			titulo = XmStringCreateLocalized ((char *)LOAD_CALENERGIA_TITLE);
			filtro = XmStringCreateLocalized ((char *)FILTER_FICH_CALENERGIA);
			XtAddCallback (dlg_files, XmNokCallback, leer_calib, (XtPointer) 2);
			break;
		case MENU_CALIBRATION_OPEN_SHAPE :
			titulo = XmStringCreateLocalized ((char *)LOAD_CALFORMA_TITLE);
			filtro = XmStringCreateLocalized ((char *)FILTER_FICH_CALFORMA);
			XtAddCallback (dlg_files, XmNokCallback, leer_calib, (XtPointer) 3);
			break;
      // Adapta el menu de guardado de ficheros
		case MENU_CALIBRATION_SAVE :
			if (!EstadoTrabajo.Calibracion) {
				Aviso(main_w, "There is no complete calibration yet");
				return;
			}
			titulo = XmStringCreateLocalized ((char *)SAVE_CAL_TITLE);
			filtro = XmStringCreateLocalized ((char *)FILTER_FICH_CAL);
			XtAddCallback (dlg_files, XmNokCallback, grabar_calib, (XtPointer) 1);
			break;
		case MENU_CALIBRATION_SAVE_ENERGY :
			if (!EstadoTrabajo.CalibracionEnergia) {
				Aviso(main_w, "There is no energy calibration yet");
				return;
			}
			titulo = XmStringCreateLocalized ((char *)SAVE_CALENERGIA_TITLE);
			filtro = XmStringCreateLocalized ((char *)FILTER_FICH_CALENERGIA);
			XtAddCallback (dlg_files, XmNokCallback, grabar_calib, (XtPointer) 2);
			break;
		case MENU_CALIBRATION_SAVE_SHAPE :
			if (!EstadoTrabajo.CalibracionForma) {
				Aviso(main_w, "There is no shape calibration yet");
				return;
			}
			titulo = XmStringCreateLocalized ((char *)SAVE_CALFORMA_TITLE);
			filtro = XmStringCreateLocalized ((char *)FILTER_FICH_CALFORMA);
			XtAddCallback (dlg_files, XmNokCallback, grabar_calib, (XtPointer) 3);
			break;
	}

	// Ultimo directorio de calibracion visitado
	if (UltCalibDirOpen != NULL) 
		{
		// pone el ultimo directorio de calibracion utilizado
		XmString patron = XmStringCreateLocalized(UltCalibDirOpen);
		XtVaSetValues (dlg_files, XmNdirectory, patron, NULL);
		XmStringFree (patron);
		}
	else 
		{
		// Como no ha habido otro antes, pone el que viene por
		// defecto en preferencias
//		UltCalibDirOpen = Preferences.GetCalibDir();
		UltAnalDirOpen = getcwd(NULL,0);
		if (UltCalibDirOpen != NULL) 
			{
			XmString patron = XmStringCreateLocalized(UltCalibDirOpen);
			XtVaSetValues (dlg_files, XmNdirectory, patron, NULL);
			XmStringFree (patron);
			}
		}
	
	XtVaSetValues (dlg_files, 
				XmNdialogTitle, titulo, 
				XmNpattern, filtro, 
				NULL);
	XmStringFree (titulo); XmStringFree (filtro);

	// Pone o substituye el menu de ficheros que haya
	XtManageChild (dlg_files);
	}

void grabar_calib(Widget widget, XtPointer client_data, XtPointer call_data) 
   {
   char *nombre = NULL, *dir = NULL;
   FILE *fDatos; // puntero al fichero donde se graba
   XmFileSelectionBoxCallbackStruct *cbs = (XmFileSelectionBoxCallbackStruct *) call_data;


   XmStringGetLtoR(cbs->value, XmFONTLIST_DEFAULT_TAG, &nombre);
   XmStringGetLtoR(cbs->dir, XmFONTLIST_DEFAULT_TAG, &dir);
	
	// memoriza el ultimo directorio de calibracion visitado
	delete [] UltCalibDirOpen;
	UltCalibDirOpen = CopyString(dir);
	
	// recupera la extension
	char *ext = DaExtensionFile(nombre, dir);
	char *extOficial;
	switch ((int) client_data) 
		{
		case 1: extOficial = (char *)EXT_FICH_CAL; break;
		case 2: extOficial = (char *)EXT_FICH_CALENERGIA; break;
		case 3: extOficial = (char *)EXT_FICH_CALFORMA; break;
		}
	
	// comprueba si hay que agnadir la extension
	if ((ext == NULL)||(strcmp(ext, extOficial) != 0)) 
		{
		// hay que agnadir la extension del fichero
		char *otro = new char[strlen(nombre) + strlen(extOficial) + 1];
		if (otro == NULL) 
			{
			XtFree(nombre); XtFree(dir); delete [] ext;
			printf(ERROR_SAVE_FILE);
			printf(ERROR_NO_MEMORY);
			return;
			}
		strcpy(otro, nombre);
		strcat(otro, extOficial);
		XtFree(nombre);
		nombre = otro;
		}
	delete [] ext;
	
	// Comprueba si el fichero ya existe
	if ((fDatos = fopen(nombre, "rb")) != NULL) 
		{
		fclose(fDatos);
		// Existe el fichero, pregunta antes de sobreescribirlo
		char pregunta[80];
		char *nombreCorto = DaNombreFile(nombre, dir);
		sprintf(pregunta, (char *)ASK_OVERWRITE, nombreCorto);
		delete [] nombreCorto;
		int Resp = Continuar(dlg_files, pregunta, (char *)OVERWRITE_TITLE);
		switch(Resp) 
			{
			case CANCELAR :
				XtUnmanageChild (widget);
			case NO :
				XtFree(nombre); XtFree(dir);
				return;
			}
		}
	
	// Graba el fichero
	GuardaCalibracion(nombre, (int) client_data);

	// quita el menu de grabar archivo
	XtUnmanageChild (widget);
	XtFree(nombre); XtFree(dir);
	}

void GuardaCalibracion(char* nombre, int tipo) {
   FILE *fDatos; // puntero al fichero donde se graba
	char Indicador;

	// Abre un fichero para su escritura
	if ((fDatos = fopen(nombre, "wb")) == NULL) {
		printf(ERROR_SAVE_FILE);
		printf(ERROR_NO_SAVE_FILE);
		return;
	}
	
	// Cabecera identificativa de analgam file
	fileWrite(&CABECERA_ASF, sizeof(CABECERA_ASF[0]), sizeof(CABECERA_ASF)-1, fDatos);

	if ((tipo == 1)||(tipo == 2)) {
		// calibracion en energia
		fileWrite(&Datos.enp1, sizeof(Datos.enp1), 1, fDatos);
		fileWrite(&Datos.enp2, sizeof(Datos.enp2), 1, fDatos);
		fileWrite(&Datos.enp3, sizeof(Datos.enp3), 1, fDatos);
		for (int i = 1; i <= 3; i++)
			for (int j = 1; j <= 3; j++)
				fileWrite(&Datos.covarEnergia[i*4+j], sizeof(Datos.covarEnergia[0]), 1, fDatos);
	}

	if ((tipo == 1)||(tipo == 3)) {
		// calibracion en parametros de forma
		// Sigma
		fileWrite(&Datos.afwhg, sizeof(Datos.afwhg), 1, fDatos);
		fileWrite(&Datos.bfwhg, sizeof(Datos.bfwhg), 1, fDatos);
		for (int i = 1; i <= 2; i++)
			for (int j = 1; j <= 2; j++)
				fileWrite(&Datos.covarSigma[i*3+j], sizeof(Datos.covarSigma[0]), 1, fDatos);
		// Tau
		fileWrite(&Datos.atau, sizeof(Datos.atau), 1, fDatos);
		fileWrite(&Datos.btau, sizeof(Datos.btau), 1, fDatos);
		for (int i = 1; i <= 2; i++)
			for (int j = 1; j <= 2; j++)
				fileWrite(&Datos.covarTau[i*3+j], sizeof(Datos.covarTau[0]), 1, fDatos);
	}

	// Cierra el fichero
	Indicador = BLOQUE_CIERRE;
	fileWrite(&Indicador, sizeof(Indicador), 1, fDatos);
	fclose(fDatos);
}

//************* LEER CALIB *********************
void leer_calib(Widget widget, XtPointer client_data, XtPointer call_data) 
	{
   	char *nombre = NULL, *dir = NULL;
	char cabecera[sizeof(CABECERA_ASF)] = "";
	XmFileSelectionBoxCallbackStruct *cbs = (XmFileSelectionBoxCallbackStruct *) call_data;
   	FILE *fDatos;

   XmStringGetLtoR(cbs->value, XmFONTLIST_DEFAULT_TAG, &nombre);
   XmStringGetLtoR(cbs->dir, XmFONTLIST_DEFAULT_TAG, &dir);
	
	// memoriza el ultimo directorio de calibracion visitado
	delete [] UltCalibDirOpen;
	UltCalibDirOpen = CopyString(dir);

   if (strlen(nombre) == strlen(dir)) 
	   { // no hay nombre de fichero
		XtFree(nombre); XtFree(dir);
		Aviso(main_w, "You must to select a file");
		return;
		}

	if ((fDatos = fopen(nombre, "rb")) == NULL) 
		{ // no pudo abrir el fichero
		XtFree(nombre); XtFree(dir);
		printf(ERROR_LOAD_FILE);
		printf(ERROR_NO_LOAD_FILE, nombre);
		XtUnmanageChild (widget);
		return;
		}

	// lee la cabecera identificativa de analgam file
	fileRead(&cabecera, sizeof(CABECERA_ASF[0]), sizeof(CABECERA_ASF)-1, fDatos);
	cabecera[sizeof(CABECERA_ASF)-1] = '\0';
	if (strcmp(cabecera, CABECERA_ASF) != 0) 
		{ // cabecera incorrecta
		XtFree(nombre); XtFree(dir);
		printf(ERROR_LOAD_FILE);
		printf(ERROR_NO_ASF_FILE, nombre);
		XtUnmanageChild (widget);
		return;
		}

	// Variables necesarias
	double afwhg, bfwhg, enp1, enp2, enp3, atau, btau;
	double covarEnergia[3][3], covarSigma[2][2], covarTau[2][2];

	// Lectura de la calibracion de energia
	if (((int) client_data == 1)||((int) client_data == 2)) 
		{
		fileRead(&enp1, sizeof(enp1), 1, fDatos);
		fileRead(&enp2, sizeof(enp2), 1, fDatos);
		fileRead(&enp3, sizeof(enp3), 1, fDatos);
		for (int i = 1; i <= 3; i++)
			for (int j = 1; j <= 3; j++) 
				{
				fileRead(&covarEnergia[i-1][j-1], sizeof(covarEnergia[0][0]), 1, fDatos);
				Datos.covarEnergia[i*4+j] = covarEnergia[i-1][j-1];
				}
		Datos.enp1 = enp1; Datos.enp2 = enp2; Datos.enp3 = enp3;
		EstadoTrabajo.CalibracionEnergia = true;
		printf("************** ACTUALIZAMOS CALIB ENER AL LEERLA DE FICHERO\n");

		ActivaEnergiaDatos();
		}
	// lectura de la calibracion de parametros de forma
	if (((int) client_data == 1)||((int) client_data == 3)) 
		{
		fileRead(&afwhg, sizeof(afwhg), 1, fDatos);
		fileRead(&bfwhg, sizeof(bfwhg), 1, fDatos);
		for (int i = 1; i <= 2; i++)
			for (int j = 1; j <= 2; j++) 
				{
				fileRead(&covarSigma[i-1][j-1], sizeof(covarSigma[0][0]), 1, fDatos);
				Datos.covarSigma[i*3+j] = covarSigma[i-1][j-1];
				}
		fileRead(&atau, sizeof(atau), 1, fDatos);
		fileRead(&btau, sizeof(btau), 1, fDatos);
		for (int i = 1; i <= 2; i++)
			for (int j = 1; j <= 2; j++) 
				{
				fileRead(&covarTau[i-1][j-1], sizeof(covarTau[0][0]), 1, fDatos);
				Datos.covarTau[i*3+j] = covarTau[i-1][j-1];
				}
		Datos.afwhg = afwhg; Datos.bfwhg = bfwhg;
		Datos.atau = atau; Datos.btau = btau;
		EstadoTrabajo.CalibracionForma = true;
		ActivaFormaDatos();
		}
	
	if ((EstadoTrabajo.CalibracionForma)&&(EstadoTrabajo.CalibracionEnergia)) 
		{
		EstadoTrabajo.Calibracion = true;
		ActivaContinuo();
		if (EstadoTrabajo.Continuo) 
			{
			ActivaAnalisis();
			ReconstruyeListaIntervAnalisis();
			}
		}

	// quita el menu de leer archivo
	XtUnmanageChild (widget);
	XtFree(nombre); XtFree(dir);
	}


/******************************************************************************
 ****                               FICHEROS
 ******************************************************************************/

//********************** CREA DLG FILES **********************
void Crea_dlgFiles() 
	{
    Arg		args[5];
    Cardinal	i = 0;
	printf("CREA DLG FILES\n");

	i = 0;
	printf("Añadimos un argumento\n");
	XtSetArg (args[i], XmNtitle, SIN_FICHERO); i++;
	printf("Creamos un fileselectiondialog\n");
	dlg_files = XmCreateFileSelectionDialog (app_shell, "dlg_files", args, i);
	printf("Le añadimos comportamiento\n");
	XtAddCallback (dlg_files, XmNcancelCallback, hide_fsb, NULL);
	printf("Termina creadlgfiles\n");
	}

//********* CREA DLG FILES CON FORMATO ******************
/*	Para la opción openSpectrum, le añadimos un combo_box para elegir el
	tipo de formato que se va a usar.
*/
void Crea_dlgFilesConFormato() 
	{
    Arg		args[5];
    Cardinal	i = 0;
	printf("CREA DLG FILES\n");

	i = 0;
	printf("Añadimos un argumento\n");
	XtSetArg (args[i], XmNtitle, SIN_FICHERO); i++;
	printf("Creamos un fileselectiondialog\n");
	dlg_files_formato = XmCreateFileSelectionDialog (app_shell, "dlg_files_formato", args, i);
	printf("Le añadimos comportamiento\n");
	XtAddCallback (dlg_files_formato, XmNcancelCallback, hide_fsb, NULL);
	printf("Termina creadlgfiles\n");
	}

void hide_fsb (Widget w, XtPointer client_data, XtPointer call_data) {
	XtUnmanageChild (w);
}

//******************** FILE_CB ********************************
//Se llama cuando se pulsa algunas de las opciones del menu file
void file_cb (Widget widget, XtPointer client_data, XtPointer call_data) 
	{
	//---------------------------
			Arg			args[7];
			Cardinal	i = 0;
			XmString 	cadena;
			Widget formato, columnaFormato, desplegado;
			char *formats[]={"Raw", "Canberra", "Ortec", "LAP"};
			int count=XtNumber(formats);
			XmStringTable str_list;
	//---------------------------
	XmString patron;
	int item_no = (int) client_data;
	
	// si hay un fichero modificado actualmente, pregunta si quiere salvar
	// los cambios antes
	if (((item_no == MENU_FILE_OPEN)||(item_no == MENU_FILE_OPEN_SPECTRUM) || (item_no == MENU_FILE_OPEN_BACKGROUND) ) &&
		 (EstadoTrabajo.TrabajoCambios)) 
		{
		char pregunta[80];
		sprintf(pregunta, ASK_CHANGES, TituloPrograma);
		int Resp = Continuar(dlg_files, pregunta, (char *)CHANGES_TITLE);
		switch(Resp) 
			{
			case CANCELAR :
				return;
			case NO :
				break;
			case SI :
				if (NombreFicheroTrabajo != NULL) 
					{
					// el fichero ya existe, lo sobreescribe con los datos
					// mas recientes
					GuardaFichero(NombreFicheroTrabajo);
					EstadoTrabajo.TrabajoCambios = false;
					PonTituloFichSinCambios();
					}
				else
					// el fichero oficial no existe, cancela la salida para que
					// el usuario lo grabe
					return;
				break;
			}		
		}
	switch (item_no) 
		{
		case MENU_FILE_QUIT : Salida(); break;
		case MENU_FILE_OPEN :
         // Adapta el menu de carga de ficheros
			patron = XmStringCreateLocalized ((char *)LOAD_FILE_TITLE);
			XtVaSetValues (dlg_files, XmNdialogTitle, patron, NULL);
			XmStringFree (patron);
			patron = XmStringCreateLocalized ((char *)FILTER_FICH_GENERAL);
			XtVaSetValues (dlg_files, XmNpattern, patron, NULL);
			XmStringFree (patron);
			XtRemoveAllCallbacks (dlg_files, XmNokCallback);
			XtAddCallback (dlg_files, XmNokCallback, leer_fichero, NULL);
			if (UltAnalDirOpen != NULL) 
				{
				// pone el ultimo directorio de espectros utilizado
				patron = XmStringCreateLocalized(UltAnalDirOpen);
				XtVaSetValues (dlg_files, XmNdirectory, patron, NULL);
				XmStringFree (patron);
				}
			else 
				{
				// Como no ha habido otro antes, pone el que viene por
				// defecto en preferencias
				UltAnalDirOpen = getcwd(NULL,0);
				if (UltAnalDirOpen != NULL) 
					{
					patron = XmStringCreateLocalized(UltAnalDirOpen);
					XtVaSetValues (dlg_files, XmNdirectory, patron, NULL);
					XmStringFree (patron);
					}
				}

			// Pone el menu de carga de ficheros en pantalla
			// Si hay puesto otro menu de ficheros lo quita
			if (!XtIsManaged(dlg_files))
				XtManageChild (dlg_files);
			break;
		case MENU_FILE_OPEN_SPECTRUM :
         // Adapta el menu de carga de ficheros
			printf("OPEN SPECTRUM\n");
			patron = XmStringCreateLocalized ((char *)LOAD_SPECTRUM_TITLE);
			XtVaSetValues (dlg_files_formato, XmNdialogTitle, patron, NULL);
			XmStringFree (patron);
			patron = XmStringCreateLocalized ((char *)FILTER_FICH_SPECTRUM);
			XtVaSetValues (dlg_files_formato, XmNpattern, patron, NULL);
			XmStringFree (patron);
			XtRemoveAllCallbacks (dlg_files_formato, XmNokCallback);
			XtAddCallback (dlg_files_formato, XmNokCallback, leer_espectro, NULL);

			//-----------------------------------------
			//Le metemos un combo para que elija el formato:
			//NOTA: Hemos creado un dlg_files_formato para que nos salga sólo el
			//		formato en este cuadro de elección de ficheros
			XmStringFree(cadena);
			cadena = XmStringCreate ("Format", "BoldNormal");
		    i = 0;
    		XtSetArg (args[i], XmNlabelString, cadena); i++;
			formato = XmCreateLabelGadget (dlg_files_formato, "formato", args, i);
			XtManageChild(formato);
			XmStringFree(cadena);
				
			str_list=(XmStringTable)XtMalloc (count * sizeof(XmString *));
			for(int k=0;k<count;k++)	str_list[k]=XmStringCreateLocalized(formats[k]);
			i=0;
			XtSetArg(args[i], XmNitems, str_list); i++;
			XtSetArg(args[i], XmNitemCount, count); i++;
			XtSetArg(args[i], XmNvisibleItemCount, count); i++;
			desplegado=XmCreateDropDownList(dlg_files_formato, "desplegada", args, i);
			XtAddCallback(desplegado, XmNselectionCallback, seleccionaFormato, NULL);

			for(int k=0;k<count;k++)	XmStringFree(str_list[k]);
			XtFree((char *) str_list);

			XtVaSetValues(desplegado, XmNy, 250, XmNx, 160, XmNheight, 35, XmNwidth, 90, NULL);
			XtManageChild(desplegado);
	    	
			//XmStringFree (cadena);

			//XtVaSetValues(formato,
			//	XmNrightAttachment, XmATTACH_WIDGET, XmNrightWidget, desplegado,
			//	NULL);
			// 			
			//---------------------------------------------------------------------------
			
			if (UltSpecDirOpen != NULL) 
				{
				// pone el ultimo directorio de espectros utilizado
				patron = XmStringCreateLocalized(UltSpecDirOpen);
				XtVaSetValues (dlg_files, XmNdirectory, patron, NULL);
				XmStringFree (patron);
				} 
			else 
				{
				// Como no ha habido otro antes, pone el que viene por
				// defecto en preferencias
				//UltSpecDirOpen = Preferences.GetAnalSpecDir();
				UltAnalDirOpen = getcwd(NULL,0);
	
				if (UltSpecDirOpen != NULL) 
					{
					patron = XmStringCreateLocalized(UltSpecDirOpen);
					XtVaSetValues (dlg_files, XmNdirectory, patron, NULL);
					XmStringFree (patron);
					}
				}
			
			// Pone el menu de carga de ficheros en pantalla
			// Si hay puesto otro menu de ficheros lo quita
			if (!XtIsManaged(dlg_files))	XtManageChild (dlg_files_formato);
			break;
		case MENU_FILE_OPEN_BACKGROUND :
         // Adapta el menu de carga de ficheros
			patron = XmStringCreateLocalized ((char *)LOAD_BACKGROUND_TITLE);
			XtVaSetValues (dlg_files, XmNdialogTitle, patron, NULL);
			XmStringFree (patron);
			patron = XmStringCreateLocalized ((char *)FILTER_FICH_BACKGROUND);
			XtVaSetValues (dlg_files, XmNpattern, patron, NULL);
			XmStringFree (patron);
			XtRemoveAllCallbacks (dlg_files, XmNokCallback);
			//NOTA: !!!!!!!! ----> Modificando para el fondo <--------
			//XtAddCallback (dlg_files, XmNokCallback, leer_espectro, NULL);
			//XtAddCallback (dlg_files, XmNokCallback, leer_espectro, XtPointer(true));
			XtAddCallback (dlg_files, XmNokCallback, leer_fondo, NULL);
			if (UltSpecDirOpen != NULL) 
				{
				// pone el ultimo directorio de espectros utilizado
				patron = XmStringCreateLocalized(UltSpecDirOpen);
				XtVaSetValues (dlg_files, XmNdirectory, patron, NULL);
				XmStringFree (patron);
				} 
			else 
				{
				// Como no ha habido otro antes, pone el que viene por
				// defecto en preferencias
				//UltSpecDirOpen = Preferences.GetAnalSpecDir();
				UltAnalDirOpen = getcwd(NULL,0);
	
				if (UltSpecDirOpen != NULL) 
					{
					patron = XmStringCreateLocalized(UltSpecDirOpen);
					XtVaSetValues (dlg_files, XmNdirectory, patron, NULL);
					XmStringFree (patron);
					}
				}
			// Pone el menu de carga de ficheros en pantalla
			// Si hay puesto otro menu de ficheros lo quita
			if (!XtIsManaged(dlg_files))
				XtManageChild (dlg_files);
			break;
	/*	case MENU_FILE_OPEN_BDBACKGROUND :
         // Adapta el menu de carga de ficheros
			patron = XmStringCreateLocalized ((char *)LOAD_BDBACKGROUND_TITLE);
			XtVaSetValues (dlg_files, XmNdialogTitle, patron, NULL);
			XmStringFree (patron);
			patron = XmStringCreateLocalized ((char *)FILTER_FICH_BDBACKGROUND);
			XtVaSetValues (dlg_files, XmNpattern, patron, NULL);
			XmStringFree (patron);
			XtRemoveAllCallbacks (dlg_files, XmNokCallback);
			//NOTA: !!!!!!!! ---->Modificando para el fondo <---------------
			//XtAddCallback (dlg_files, XmNokCallback, leer_espectro, NULL);
			//XtAddCallback (dlg_files, XmNokCallback, leer_espectro, XtPointer(true));
			XtAddCallback (dlg_files, XmNokCallback, leer_bdfondo, NULL);
			if (UltSpecDirOpen != NULL) 
				{
				// pone el ultimo directorio de espectros utilizado
				patron = XmStringCreateLocalized(UltSpecDirOpen);
				XtVaSetValues (dlg_files, XmNdirectory, patron, NULL);
				XmStringFree (patron);
				} 
			else 
				{
				// Como no ha habido otro antes, pone el que viene por
				// defecto en preferencias
				//UltSpecDirOpen = Preferences.GetAnalSpecDir();
				UltAnalDirOpen = getcwd(NULL,0);

				if (UltSpecDirOpen != NULL) 
					{
					patron = XmStringCreateLocalized(UltSpecDirOpen);
					XtVaSetValues (dlg_files, XmNdirectory, patron, NULL);
					XmStringFree (patron);
					}
				}
			// Pone el menu de carga de ficheros en pantalla
			// Si hay puesto otro menu de ficheros lo quita
			if (!XtIsManaged(dlg_files))
				XtManageChild (dlg_files);
			break;*/
		case MENU_FILE_SAVE :
			printf("MENU FILE SAVE\n");
			if (NombreFicheroTrabajo != NULL) 
				{
			   	GuardaFichero(NombreFicheroTrabajo);
				EstadoTrabajo.TrabajoCambios = false;
				PonTituloFichSinCambios();
				}
			else if (Datos.numCanales > 0) 
				{
				patron = XmStringCreateLocalized ((char *)SAVE_FILE_TITLE);
				XtVaSetValues (dlg_files, XmNdialogTitle, patron, NULL);
				XmStringFree (patron);
				patron = XmStringCreateLocalized ((char *)FILTER_FICH_GENERAL);
				XtVaSetValues (dlg_files, XmNpattern, patron, NULL);
				XmStringFree (patron);
				XtRemoveAllCallbacks (dlg_files, XmNokCallback);
				XtAddCallback (dlg_files, XmNokCallback, grabar_espectro, NULL);
				if (UltAnalDirOpen != NULL) 
					{
					// pone el ultimo directorio de espectros utilizado
					patron = XmStringCreateLocalized(UltAnalDirOpen);
					XtVaSetValues (dlg_files, XmNdirectory, patron, NULL);
					XmStringFree (patron);
					}
				else 
					{
					// Como no ha habido otro antes, pone el que viene por
					// defecto en preferencias
					//UltAnalDirOpen = Preferences.GetAnalSpecDir();
					UltAnalDirOpen = getcwd(NULL,0);
					if (UltAnalDirOpen != NULL) 
						{
						patron = XmStringCreateLocalized(UltAnalDirOpen);
						XtVaSetValues (dlg_files, XmNdirectory, patron, NULL);
						XmStringFree (patron);
						}
					}

				// Pone el menu de grabar ficheros en pantalla
				// Si hay puesto otro menu de ficheros lo quita
				if (!XtIsManaged(dlg_files))	XtManageChild (dlg_files);
				}
			break;
		case MENU_FILE_SAVE_AS : 
			printf("MENU FILE SAVE AS\n");
		   if (Datos.numCanales > 0) {
				patron = XmStringCreateLocalized ((char *)SAVE_FILE_TITLE);
				XtVaSetValues (dlg_files, XmNdialogTitle, patron, NULL);
				XmStringFree (patron);
				patron = XmStringCreateLocalized ((char *)FILTER_FICH_GENERAL);
				XtVaSetValues (dlg_files, XmNpattern, patron, NULL);
				XmStringFree (patron);
				XtRemoveAllCallbacks (dlg_files, XmNokCallback);
				XtAddCallback (dlg_files, XmNokCallback, grabar_espectro, NULL);
				if (UltAnalDirOpen != NULL) {
					// pone el ultimo directorio de espectros utilizado
					patron = XmStringCreateLocalized(UltAnalDirOpen);
					XtVaSetValues (dlg_files, XmNdirectory, patron, NULL);
					XmStringFree (patron);
				} else {
					// Como no ha habido otro antes, pone el que viene por
					// defecto en preferencias
					//UltAnalDirOpen = Preferences.GetAnalSpecDir();
					UltAnalDirOpen = getcwd(NULL,0);

					if (UltAnalDirOpen != NULL) {
						patron = XmStringCreateLocalized(UltAnalDirOpen);
						XtVaSetValues (dlg_files, XmNdirectory, patron, NULL);
						XmStringFree (patron);
					}
				}

				// Pone el menu de grabar ficheros en pantalla
				// Si hay puesto otro menu de ficheros lo quita
				if (!XtIsManaged(dlg_files))
					XtManageChild (dlg_files);
			}
			break;
		case MENU_FILE_SAVE_TO :
			printf("MENU FILE SAVE TO\n");
			if (Datos.numCanales > 0) 
			   {
				patron = XmStringCreateLocalized ((char *)SAVE_FILE_TITLE);
				XtVaSetValues (dlg_files, XmNdialogTitle, patron, NULL);
				XmStringFree (patron);
				patron = XmStringCreateLocalized ((char *)FILTER_FICH_BDBACKGROUND);
				XtVaSetValues (dlg_files, XmNpattern, patron, NULL);
				XmStringFree (patron);
				XtRemoveAllCallbacks (dlg_files, XmNokCallback);
				XtAddCallback (dlg_files, XmNokCallback, GuardarBDFondo, NULL);
				if (UltAnalDirOpen != NULL) 
					{
					// pone el ultimo directorio de espectros utilizado
					patron = XmStringCreateLocalized(UltAnalDirOpen);
					XtVaSetValues (dlg_files, XmNdirectory, patron, NULL);
					XmStringFree (patron);
					}
				else 
					{
					// Como no ha habido otro antes, pone el que viene por
					// defecto en preferencias
					UltAnalDirOpen = Preferences.GetBackBDDir();
					if (UltAnalDirOpen != NULL) 
						{
						patron = XmStringCreateLocalized(UltAnalDirOpen);
						XtVaSetValues (dlg_files, XmNdirectory, patron, NULL);
						XmStringFree (patron);
						}
					}

				// Pone el menu de grabar ficheros en pantalla
				// Si hay puesto otro menu de ficheros lo quita
				if (!XtIsManaged(dlg_files))	XtManageChild (dlg_files);
				}
			break;
	}
}

	/***********************
	*****
	***** 	  FUNCIONES VARIAS SOBRE FICHEROS
	*****
	************************/

typedef struct tEstadoVisualComp {
	// zona del Continuo
	char CanalContinuo;
	char CanalPicos;
	char AjusteContinuo;
	char CanalNormal;
	// Zona de grafica
	unsigned char EscalaY;	// 0 -> Lineal
									// 1 -> Raiz Cuadrada
									// 2 -> Logaritmica
	char AutoEscalaY;
	char VentanaDatos;
	// Zona de calibracion
	char AutoZoomCalibracion;
	char AjusteCalibracion;
	// Zona de analisis
	char AjusteParcialAnalisis;
	char AjusteTotalAnalisis;
	char AutoZoomAnalisis;
} tEstadoVisualComp;

void lee_EstadoVisual(tEstadoVisual& Estado, FILE *stream) {
	tEstadoVisualComp flags;

	fileRead(&flags, 1, sizeof(flags), stream);
	
	// zona del Continuo
	Estado.CanalContinuo = (flags.CanalContinuo != 0) ? true : false;
	Estado.CanalPicos = (flags.CanalPicos != 0) ? true : false;
	Estado.AjusteContinuo = (flags.AjusteContinuo != 0) ? true : false;
	Estado.CanalNormal = (flags.CanalNormal != 0) ? true : false;
	// Zona de grafica
	Estado.EscalaY = flags.EscalaY;
	Estado.AutoEscalaY = (flags.AutoEscalaY != 0) ? true : false;
	Estado.VentanaDatos = (flags.VentanaDatos != 0) ? true : false;
	// Zona de calibracion
	Estado.AutoZoomCalibracion = (flags.AutoZoomCalibracion != 0) ? true : false;
	Estado.AjusteCalibracion = (flags.AjusteCalibracion != 0) ? true : false;
	// Zona de analisis
	Estado.AjusteParcialAnalisis = (flags.AjusteParcialAnalisis != 0) ? true : false;
	Estado.AjusteTotalAnalisis = (flags.AjusteTotalAnalisis != 0) ? true : false;
	Estado.AutoZoomAnalisis = (flags.AutoZoomAnalisis != 0) ? true : false;
}

void escribe_EstadoVisual(tEstadoVisual& Estado, FILE *stream) {
	tEstadoVisualComp flags;

	// zona del Continuo
	flags.CanalContinuo = (Estado.CanalContinuo) ? -1 : 0;
	flags.CanalPicos = (Estado.CanalPicos) ? -1 : 0;
	flags.AjusteContinuo = (Estado.AjusteContinuo) ? -1 : 0;
	flags.CanalNormal = (Estado.CanalNormal) ? -1 : 0;
	// Zona de grafica
	flags.EscalaY = Estado.EscalaY;
	flags.AutoEscalaY = (Estado.AutoEscalaY) ? -1 : 0;
	flags.VentanaDatos = (Estado.VentanaDatos) ? -1 : 0;
	// Zona de calibracion
	flags.AutoZoomCalibracion = (Estado.AutoZoomCalibracion) ? -1 : 0;
	flags.AjusteCalibracion = (Estado.AjusteCalibracion) ? -1 : 0;
	// Zona de analisis
	flags.AjusteParcialAnalisis = (Estado.AjusteParcialAnalisis) ? -1 : 0;
	flags.AjusteTotalAnalisis = (Estado.AjusteTotalAnalisis) ? -1 : 0;
	flags.AutoZoomAnalisis = (Estado.AutoZoomAnalisis) ? -1 : 0;

	fileWrite(&flags, 1, sizeof(flags), stream);
}

//******************* ESCRIBE AJUSTE *****************
void escribe_Ajuste(tPicoAjuste Ajuste, FILE *stream) 
	{
	// Datos Fisicos
//	printf("datos físicos\n");
	fileWrite(&Ajuste.Centroide, sizeof(Ajuste.Centroide), 1, stream);
	fileWrite(&Ajuste.SigCentroide, sizeof(Ajuste.Centroide), 1, stream);
	fileWrite(&Ajuste.Anchura, sizeof(Ajuste.Anchura), 1, stream);
	fileWrite(&Ajuste.SigAnchura, sizeof(Ajuste.SigAnchura), 1, stream);
	fileWrite(&Ajuste.Tau, sizeof(Ajuste.Tau), 1, stream);
	fileWrite(&Ajuste.SigTau, sizeof(Ajuste.SigTau), 1, stream);
	fileWrite(&Ajuste.Ytau, sizeof(Ajuste.Ytau), 1, stream);
	fileWrite(&Ajuste.SigYtau, sizeof(Ajuste.SigYtau), 1, stream);
	fileWrite(&Ajuste.P, sizeof(Ajuste.P), 1, stream);
	fileWrite(&Ajuste.SigP, sizeof(Ajuste.SigP), 1, stream);
	fileWrite(&Ajuste.YP, sizeof(Ajuste.YP), 1, stream);
	fileWrite(&Ajuste.SigYP, sizeof(Ajuste.SigYP), 1, stream);
	fileWrite(&Ajuste.SigTauYtau, sizeof(Ajuste.SigTauYtau), 1, stream);
	fileWrite(&Ajuste.SigPYP, sizeof(Ajuste.SigPYP), 1, stream);
//	printf("matriz de covarianza\n");
	for(int i=0;i<4;i++)
		for(int j=0;j<4;j++)
			fileWrite(&Ajuste.covar[i][j], sizeof(Ajuste.covar[i][j]), 1, stream);
			
	// Datos administracion
//	printf("datos de administración\n");
	fileWriteBool(Ajuste.inicializado, stream);
	fileWriteBool(Ajuste.ajustado, stream);
	fileWriteBool(Ajuste.calibrado, stream);
	fileWriteBool(Ajuste.identificado, stream);
	// Datos de identificacion
	if(Ajuste.identificado)
		{
		int num=Ajuste.emisiones->numElements();
		fileWrite(&num, sizeof(int), 1, stream);
//		printf("identificado---------------->Escribimos %d emisiones\n", num);
		for(int i=0;i<num;i++)
			{
	//		printf("escribimos emision %d\n", i);
			NodoEmision *ne=(NodoEmision *)Ajuste.emisiones->getNode(i);
	//		printf("Escribimos energía\n");
			fileWrite(&ne->Energia, sizeof(double), 1, stream);
	//		printf("Escribimos código del elemento\n");
			fileWrite(&ne->codElemento, sizeof(int), 1, stream);
			//NOTA: Tenemos que meter el nombre en caso de que exista (ie: no sea de lara)
			if(ne->codElemento==-1)//Se supone que si el código es -1 y está identificado es que tendrá nombre
				{
				int tamNombre=strlen(ne->nombre);
				fileWrite(&tamNombre, sizeof(int), 1, stream);	
				
	//			printf("Escribimos un nombre de %d caracteres: |%s|\n", tamNombre, ne->nombre);
				fileWrite(ne->nombre, sizeof(char), sizeof(ne->nombre)-1, stream);
				//felipe. tb la probabilidad
				//fileWrite(&ne->probabilidad, sizeof(double), 1, stream);
				//printf("******Escribimos la porbabilidad %f \n", ne->probabilidad);
				}
			//
			}
		}
	
	//Datos de análisis
	fileWriteBool(Ajuste.eficienciaCalculada, stream);
	//NOTa: Esto está comentado hasta que terminemos con la versión
	/*		
	if(Ajuste.eficienciaCalculada && Ajuste.parametrosEficiencia!=NULL)
		{
	//	printf("DME=%d\n", Datos.dimensionMatEfi);
		fileWrite(&Datos.dimensionMatEfi, sizeof(int), 1, stream);
		for(int i=0;i<Datos.dimensionMatEfi;i++)	
			fileWrite(&Ajuste.parametrosEficiencia[i], sizeof(Ajuste.parametrosEficiencia[i]), 1, stream);
		}
	//*/	
	fileWriteBool(Ajuste.ldCalculado, stream);
	fileWriteBool(Ajuste.analizado, stream);
		
//	printf("datos actividad\n");
	fileWrite(&Ajuste.ld, sizeof(Ajuste.ld), 1, stream);
	fileWrite(&Ajuste.lc, sizeof(Ajuste.lc), 1, stream);
	fileWrite(&Ajuste.eficiencia, sizeof(Ajuste.eficiencia), 1, stream);
	fileWrite(&Ajuste.sigmaEficiencia, sizeof(Ajuste.sigmaEficiencia), 1, stream);
	fileWrite(&Ajuste.correccion, sizeof(Ajuste.correccion), 1, stream);
	fileWrite(&Ajuste.sigmaCorreccion, sizeof(Ajuste.sigmaCorreccion), 1, stream);
	fileWrite(&Ajuste.area, sizeof(Ajuste.area), 1, stream);
	fileWrite(&Ajuste.incerArea, sizeof(Ajuste.incerArea), 1, stream);
	fileWrite(&Ajuste.areaSinFondo, sizeof(Ajuste.areaSinFondo), 1, stream);
	fileWrite(&Ajuste.incerAreaSinFondo, sizeof(Ajuste.incerAreaSinFondo), 1, stream);
	fileWrite(&Ajuste.actividad, sizeof(Ajuste.actividad), 1, stream);
	fileWrite(&Ajuste.sigmaActividad, sizeof(Ajuste.sigmaActividad), 1, stream);
	fileWrite(&Ajuste.mda, sizeof(Ajuste.mda), 1, stream);
	fileWrite(&Ajuste.mdaPadre, sizeof(Ajuste.mdaPadre), 1, stream);
	//
//	printf("fin de escribe ajuste\n");
	}

//******************* LEE AJUSTE *********************
void lee_Ajuste(tPicoAjuste& Ajuste, FILE *stream)
	{
	// Datos Fisicos
//	printf("LEE AJUSTE\n");
	fileRead(&Ajuste.Centroide, sizeof(Ajuste.Centroide), 1, stream);
	fileRead(&Ajuste.SigCentroide, sizeof(Ajuste.Centroide), 1, stream);
	fileRead(&Ajuste.Anchura, sizeof(Ajuste.Anchura), 1, stream);
	fileRead(&Ajuste.SigAnchura, sizeof(Ajuste.SigAnchura), 1, stream);
	fileRead(&Ajuste.Tau, sizeof(Ajuste.Tau), 1, stream);
	fileRead(&Ajuste.SigTau, sizeof(Ajuste.SigTau), 1, stream);
	fileRead(&Ajuste.Ytau, sizeof(Ajuste.Ytau), 1, stream);
	fileRead(&Ajuste.SigYtau, sizeof(Ajuste.SigYtau), 1, stream);
	fileRead(&Ajuste.P, sizeof(Ajuste.P), 1, stream);
	fileRead(&Ajuste.SigP, sizeof(Ajuste.SigP), 1, stream);
	fileRead(&Ajuste.YP, sizeof(Ajuste.YP), 1, stream);
	fileRead(&Ajuste.SigYP, sizeof(Ajuste.SigYP), 1, stream);
	fileRead(&Ajuste.SigTauYtau, sizeof(Ajuste.SigTauYtau), 1, stream);
	fileRead(&Ajuste.SigPYP, sizeof(Ajuste.SigPYP), 1, stream);
	for(int i=0;i<4;i++)
		for(int j=0;j<4;j++)
			fileRead(&Ajuste.covar[i][j], sizeof(Ajuste.covar[i][j]), 1, stream);
	// Datos administracion
	fileReadBool(Ajuste.inicializado, stream);
	fileReadBool(Ajuste.ajustado, stream);
	fileReadBool(Ajuste.calibrado, stream);
	fileReadBool(Ajuste.identificado, stream);
	// Datos de identificacion
//	printf("Ajuste.identificado vale %d\n", Ajuste.identificado);
	if(Ajuste.identificado)
		{
		int num=0;
		fileRead(&num, sizeof(int), 1, stream);
		Ajuste.emisiones=new LinkedList();
		for(int i=0;i<num;i++)
			{
			NodoEmision *ne=new NodoEmision();
			int ce;
			double e;
			fileRead(&e, sizeof(double), 1, stream);
			fileRead(&ce, sizeof(int), 1, stream);
			ne->codElemento=ce;
			ne->Energia=e;
			ne->nombre=NULL;
			//felipe. tb la probabilidad
			double pb;
			if(ne->codElemento==-1)
				{
				int tam=0;
				fileRead(&tam, sizeof(int), 1, stream);
				ne->nombre=new char[tam];
				//ne->nombre[0]='\0';
				//for(int j=0;j<tam;j++)	ne->nombre[j]=' ';
				printf("Vamos a leer un nombre de %d caracteres\n", tam);
				//fileRead(&nombre, sizeof(char), tam, stream);
				//fileWrite(ne->nombre, sizeof(char), sizeof(ne->nombre)-1, stream);
				fread(ne->nombre, sizeof(ne->nombre[0]), sizeof(ne->nombre)-1, stream);
				ne->nombre[sizeof(ne->nombre)-1]='\0';
				//fileRead(ne->nombre, sizeof(char)+1, tam, stream);
				printf("El nombre es |%s|\n", ne->nombre);
				//felipe. tb la probabilidad
				//fileRead(&ne->probabilidad, sizeof(double), 1, stream);
				//ne->probabilidad=pb;
				//printf("La probabilidad es %f\n", ne->probabilidad);

				}
			Ajuste.emisiones->addLast(ne);
			}
		}
	else	Ajuste.emisiones=new LinkedList(); //La creamos vacía

	
	//NUEVO: No lo guardo de momento por los problemas de versiones
	Ajuste.invisible=false;
	//
	//NUEVO
	//Datos de análisis
	fileReadBool(Ajuste.eficienciaCalculada, stream);
	/*
	if(Ajuste.eficienciaCalculada)
		{
		int num=0;
		fileRead(&num, sizeof(int), 1, stream);
		Ajuste.parametrosEficiencia=new double[num];
		for(int i=0;i<num;i++)
			fileRead(&Ajuste.parametrosEficiencia[i], sizeof(Ajuste.parametrosEficiencia[i]), 1, stream);
		}
	//*/
	
	fileReadBool(Ajuste.ldCalculado, stream);
	fileReadBool(Ajuste.analizado, stream);
	fileRead(&Ajuste.ld, sizeof(Ajuste.ld), 1, stream);
	fileRead(&Ajuste.lc, sizeof(Ajuste.lc), 1, stream);
	fileRead(&Ajuste.eficiencia, sizeof(Ajuste.eficiencia), 1, stream);
	fileRead(&Ajuste.sigmaEficiencia, sizeof(Ajuste.sigmaEficiencia), 1, stream);
	fileRead(&Ajuste.correccion, sizeof(Ajuste.correccion), 1, stream);
	fileRead(&Ajuste.sigmaCorreccion, sizeof(Ajuste.sigmaCorreccion), 1, stream);
	fileRead(&Ajuste.area, sizeof(Ajuste.area), 1, stream);
	fileRead(&Ajuste.incerArea, sizeof(Ajuste.incerArea), 1, stream);
	fileRead(&Ajuste.areaSinFondo, sizeof(Ajuste.areaSinFondo), 1, stream);
	fileRead(&Ajuste.incerAreaSinFondo, sizeof(Ajuste.incerAreaSinFondo), 1, stream);
	fileRead(&Ajuste.actividad, sizeof(Ajuste.actividad), 1, stream);
	fileRead(&Ajuste.sigmaActividad, sizeof(Ajuste.sigmaActividad), 1, stream);
	fileRead(&Ajuste.mda, sizeof(Ajuste.mda), 1, stream);
	fileRead(&Ajuste.mdaPadre, sizeof(Ajuste.mdaPadre), 1, stream);
	//
	}

	/***********************
	*****
	***** 	  LEER FICHEROS
	*****
	************************/
void leer_fichero(Widget widget, XtPointer client_data, XtPointer call_data)
	{
   	char *nombre = NULL, *dir = NULL;
	char cabecera[sizeof(CABECERA_ASF)] = "";
	XmFileSelectionBoxCallbackStruct *cbs = (XmFileSelectionBoxCallbackStruct *) call_data;
//	bool fbdLeido=false;
		
   	XmStringGetLtoR(cbs->value, XmFONTLIST_DEFAULT_TAG, &nombre);
   	XmStringGetLtoR(cbs->dir, XmFONTLIST_DEFAULT_TAG, &dir);
	
	// guarda el ultimo directorio visitado
	if(UltAnalDirOpen!=NULL)	delete [] UltAnalDirOpen;
	UltAnalDirOpen = CopyString(dir);

	printf("Comprobando que hay fichero\n");
   	if (nombre==NULL || dir==NULL || strlen(nombre) == strlen(dir)) 
	   	{ // no hay nombre de fichero
		XtFree(nombre); XtFree(dir);
		Aviso(main_w, "You must to select a file");
		return;
		}

	//  ***   LEE EL ESPECTRO   ***

	// Variables para cargar datos
   	FILE *fDatos;
	unsigned int* espectro;
	unsigned short numCanales;
	float tiempo;
	bool tipoFondo;
	char Indicador;
	tEstadoTrabajo EstadoLeido; InicializaEstadoTrabajo(EstadoLeido);
	tEstadoVisual EstadoVisualLeido; InicializaEstadoVisual(EstadoVisualLeido);
	
	//******************** PREPARANDO EL FICHERO ****************
	//preparaFichero(nombre);
    printf("LEEMOS NUEVO FICHERO \n");
		
    fDatos = fopen(nombre, "rb");

    if (fDatos == NULL) 
		{ // no pudo abrir el fichero
		XtFree(nombre); XtFree(dir);
		printf(ERROR_LOAD_FILE);
		printf(ERROR_NO_LOAD_FILE, nombre);
		XtUnmanageChild (widget);
		return;
		}
	printf("Leemos cabecera\n");
	Datos.BorraDatos();
	// lee la cabecera identificativa de analgam file
	fileRead(&cabecera, sizeof(CABECERA_ASF[0]), sizeof(CABECERA_ASF)-1, fDatos);
	cabecera[sizeof(CABECERA_ASF)-1] = '\0';
	printf("La cabecera es %s\n", cabecera);
	if (strcmp(cabecera, CABECERA_ASF) != 0) 
		{ // cabecera incorrecta
		XtFree(nombre); XtFree(dir);
		fclose(fDatos);
		printf(ERROR_LOAD_FILE);
		printf(ERROR_NO_ASF_FILE, nombre);
		XtUnmanageChild (widget);
		return;
		}
	printf("--- Eliminamos todos los datos anteriores ---\n");
	// Deja el programa a cero sin ningún dato. Esto libera memoria para poder
	// leer el nuevo espectro.
	ReseteaDatos();
	DesactivaGrafica();
	DesactivaTrabajo();
		
	printf("EL ESTADO INICIAL ES %d\n", ESTADO_INICIAL);
	PonActivaZona(ESTADO_INICIAL);
	XtVaSetValues(app_shell, XmNtitle, SIN_FICHERO, NULL);
		
	printf("Lectura de datos\n");
   	fileRead(&tipoFondo, sizeof(tipoFondo), 1, fDatos);
	Datos.esFondo=tipoFondo;
	printf("Datos.esFondo VALE: %d\n", Datos.esFondo);	
		//NOTA: Nuevo, no sé si es donde debe ir		
		if(Datos.esFondo)	
			{
			int tam=0;
			fileRead(&tam, sizeof(int), 1, fDatos);
			printf("El tamaño del nombre es %d \n", tam);
			char titulo[tam];
			printf("Es un fondo, leemos cuál es la base de datos\n");
			//fileRead(&titulo, sizeof(titulo[0]), sizeof(titulo), fDatos);
			fgets(titulo, sizeof(titulo)+1, fDatos);
			//titulo[sizeof(titulo)]='d';
			printf("Hemos leido |%s|\n", titulo);
					
			nombreFichFBD=new char[tam];
			sprintf(nombreFichFBD, "%s", titulo);
			printf("NombreFichFBD=|%s|\n", nombreFichFBD);	
			if(!fopen(nombreFichFBD, "r"))	
				{//No encontramos la base de datos de fondo en el fichero original
				//donde se creó. Preguntamos por la base de datos.
				printf("No se encuentra el fichero\n");
				//NOTA: Ahora, como abrimos un fondo, preguntamos a qué base de datos de
				//		fondo va a pertenecer este
				XmString patron = XmStringCreateLocalized ((char *)LOAD_BDBACKGROUND_TITLE);
				XtVaSetValues (dlg_files, XmNdialogTitle, patron, NULL);
				XmStringFree (patron);
				patron = XmStringCreateLocalized ((char *)FILTER_FICH_BDBACKGROUND);
				XtVaSetValues (dlg_files, XmNpattern, patron, NULL);
				XmStringFree (patron);
				XtRemoveAllCallbacks (dlg_files, XmNokCallback);
				XtAddCallback (dlg_files, XmNokCallback, leer_bdfondo, NULL);
		
				//Ponemos el fichero de preferencias
				//UltAnalDirOpen = Preferences.GetBackBDDir();
				UltAnalDirOpen = getcwd(NULL,0);
	
				if (UltAnalDirOpen != NULL) 
					{
					patron = XmStringCreateLocalized(UltAnalDirOpen);
					XtVaSetValues (dlg_files, XmNdirectory, patron, NULL);
					XmStringFree (patron);
					}
				printf("El dialog files está a %d\n", XtIsManaged(dlg_files));
				// Pone el menu de carga de ficheros en pantalla
				// Si hay puesto otro menu de ficheros lo quita
				if (!XtIsManaged(dlg_files))	
					{
					XtManageChild (dlg_files);
					printf("Imprimimos el cuadro de diálogo\n");
					}
				}
			else	
				{
				printf("Fichero encontrado\n");
				}
			}
		//

	fileRead(&tiempo, sizeof(tiempo), 1, fDatos);
	Datos.tiempo=tiempo;
	printf("Datos.tiempo VALE: %f\n", Datos.tiempo);	
   	fileRead(&numCanales, sizeof(numCanales), 1, fDatos);  // lee el numero de canales
	Datos.numCanales=numCanales;
   	espectro = new unsigned int[numCanales]; // Reserva la memoria necesaria
   	if (espectro == NULL) 
		{ // no hay memoria disponible
      	fclose(fDatos);
		printf(ERROR_LOAD_FILE);
		printf(ERROR_NO_MEMORY_F, nombre);
		XtFree(nombre); XtFree(dir);
		XtUnmanageChild (widget);
		return;
   		}
    fileRead(&Indicador, sizeof(Indicador), 1, fDatos);  // lee el tipo de dato de
							// los canales
	if (Indicador == 0) 
		{ // vienen en unsigned short
		unsigned short lectura;
	   	for (int i = 0; i < numCanales; i++) 
			{ // lee los datos
	   	   	fileRead(&lectura, sizeof(lectura), 1, fDatos);
			espectro[i] = (unsigned int) lectura;
			}
		}
	else
	   for (int i = 0; i < numCanales; i++) // lee los datos
   	   		fileRead(&espectro[i], sizeof(espectro[0]), 1, fDatos);

	// lee el estado actual de trabajo
	fileRead(&EstadoLeido.Actual, sizeof(EstadoLeido.Actual), 1, fDatos);

	// lee el resto de datos
	
		// variables necesarias
		// ****
				// lectura de los picos
		tListaPicos listaPicos = { NULL, 0 };
				// lectura de la precalibracion
		double afwhg, bfwhg, enp1, enp2, enp3, atau, btau, ares, bres;
		double covarEnergia[3][3], covarSigma[2][2], covarTau[2][2], covarRes[2][2];
		tListaPrecalib listaPrecalibracion = { NULL, 0 };
		NodoPrecalibracion* nodo = NULL;
				// lectura de la calibracion
		tListaCalibracion listaCalibracion = { NULL, 0 };
		NodoCalibracion *nodoCalib=NULL;//
				// lectura del continuo
		tListaContinuo listaContinuo = { NULL, 0 };
		NodoContinuo* nodoC = NULL; 
				// lectura del analisis
		tListaAnalisis listaAnalisis = { NULL, 0 };
		NodoAnalisis *nodoAnal=NULL;
		float *continuo = NULL, *sigmaContinuo = NULL;
		unsigned char *fondo = NULL, *picos = NULL;
		unsigned short minInterv, maxInterv;
		int numPicosEncontrados, nNodosSplineContinuo;
		double ChiSqContinuo;
		// ****

		printf("Leemos todos los bloques del fichero\n");
	// mientras no haya errores o se haya alcanzado el final lee un bloque
	// del fichero
	bool bParar = false; // indica si se ha de finalizar de leer
	unsigned char bloqueVisualizacion = 0,
					  bloqueContinuo = 0,
					  bloquePreCalibracion = 0,
					  bloqueCalibracion = 0,
					  bloqueAnalisis = 0,
					  bloqueActividad = 0;

	while ((!bParar)&& (fileRead(&Indicador, sizeof(Indicador), 1, fDatos) != EOF)) 
		{
		bool bError = false; // flag para comprobar que cada bloque es correcto
		int nElem = 0;
		int num=0;
		int num2=0;
		printf("----------------------------------------INDICADOR VALE %d \n", Indicador);
		switch (Indicador) 
			{
			case BLOQUE_VISUALIZACION:
//				printf("leyendo estado visualizacion\n");
				// Comprueba que este bloque no se ha leido ya
				if (++bloqueVisualizacion > 1) 
					{
					bError = true;
					break;
					}
				lee_EstadoVisual(EstadoVisualLeido, fDatos);
				break;
			case BLOQUE_ACTIVIDAD:
				printf("******************LEYENDO EL BLOQUE DE ACTIVIDAD**************\n");
				if (++bloqueActividad > 1) 
					{
					printf("Bloque de actividad vale %d\n", bloqueAnalisis);
					bError = true;
					break;
					}
				//Leemos las matrices de varianza de la eficiencia
				fileRead(&Datos.dimensionMatEfi, sizeof(Datos.dimensionMatEfi),1, fDatos);
				printf("La dimensión de la matriz de eficiencia es %d\n", Datos.dimensionMatEfi);
				if(Datos.dimensionMatEfi>0)
					{
					Datos.matrizEficiencia=new double*[Datos.dimensionMatEfi];
					for(int i=0;i<Datos.dimensionMatEfi;i++)	Datos.matrizEficiencia[i]=new double[Datos.dimensionMatEfi];
					for(int i=0;i<Datos.dimensionMatEfi;i++)
						for(int j=0;j<Datos.dimensionMatEfi;j++)
							fileRead(&Datos.matrizEficiencia[i][j], sizeof(Datos.matrizEficiencia[i][j]),1, fDatos);
					//
					printf("Leemos los parámetros y los nodos de eficiencia\n");
					for(int i=0;i<Datos.dimensionMatEfi;i++)	
						{
						for(int j=0;j<Datos.dimensionMatEfi;j++)	
							printf("%.4f ", Datos.matrizEficiencia[i][j]);
						printf("\n");
						}
					//
					Datos.parametrosEficiencia=new double[Datos.dimensionMatEfi];
					for(int i=0;i<Datos.dimensionMatEfi;i++)	
						fileRead(&Datos.parametrosEficiencia[i], sizeof(Datos.parametrosEficiencia[i]), 1, fDatos);
				
					for(int i=0;i<Datos.dimensionMatEfi;i++)	
						printf("%.2f ", Datos.parametrosEficiencia[i]);
					printf("\n");
				
					Datos.nodosEficiencia=new double[Datos.dimensionMatEfi+1];
					for(int i=0;i<Datos.dimensionMatEfi+1;i++)	
						fileRead(&Datos.nodosEficiencia[i], sizeof(Datos.nodosEficiencia[i]), 1, fDatos);
				
					for(int i=0;i<Datos.dimensionMatEfi+1;i++)	
						printf("%.2f ", Datos.nodosEficiencia[i]);
					printf("\n");
					}
				printf("Leídos datos de eficiencia, vamos a leer las matrices de actividad\n");
				//Leemos las matrices de actividad
				num=0;
				fileRead(&num, sizeof(num),1, fDatos);
				printf("La lista de matrices tendrá %d filas\n", num);
				if(Datos.matricesActividad==NULL)	Datos.matricesActividad=new LinkedList();
				else								Datos.matricesActividad->deleteAll();//NOTA: OJO: Deberíamos borrar también las sublistas
				for(int i=0;i<num;i++)
					{
					LinkedList *l=new LinkedList();
					num2=0;
					fileRead(&num2, sizeof(num2),1, fDatos);
					printf("La lista %d tiene %d columnas\n", i, num2);
					for(int j=0;j<num2;j++)
						{
						NodoPico *n=new NodoPico();
						fileRead(&n, sizeof(NodoPico *),1, fDatos);
						l->addLast(n);
						}
					Datos.matricesActividad->addLast(l);
					}
				printf("Terminada la lectura de matrices de actividad\n");
				EstableceComputeActividad();
				break;
			case BLOQUE_ANALISIS:
				printf("**********************LEYENDO BLOQUE DE ANÁLISIS********************\n");
				// Comprueba que este bloque no se ha leido ya
				if (++bloqueAnalisis > 1) 
					{
					bError = true;
					break;
					}

				fileReadBool(EstadoLeido.Analisis, fDatos);

				fileReadBool(EstadoLeido.AnalisisEficiencia, fDatos);
				fileReadBool(EstadoLeido.AnalisisActividad, fDatos);
				fileReadBool(EstadoLeido.AnalisisLD, fDatos);
				printf("**********************************\n");
					printf("AnalisisEficiencia=%d\n", EstadoLeido.AnalisisEficiencia);
					printf("AnalisisActividad=%d\n", EstadoLeido.AnalisisActividad);
					printf("AnalisisLD=%d\n", EstadoLeido.AnalisisLD);
				ActivaActividad();

				//leemos los datos de actividad para las restricciones
				fileReadBool(equilibrioDeterminado, fDatos);
				printf("Equilibrio determinado es %d\n", equilibrioDeterminado);
				if(equilibrioDeterminado)
					{
					actividades=new float*[numCadenas];
					sigmaActividades=new float*[numCadenas];
					
					for(int i=0;i<numCadenas;i++)
						{
						int minEl=cadenas[i*2];
						int maxEl=cadenas[i*2+1];
						int max=int((maxEl-minEl)/3);
						actividades[i]=new float[int((maxEl-minEl)/3)];
						sigmaActividades[i]=new float[int((maxEl-minEl)/3)];
						for(int j=0;j<max;j++)	
							{
							fileRead(&actividades[i][j], sizeof(actividades[i][j]), 1, fDatos);
							printf("Actividades[%d][%d] es %f\n", i,j, actividades[i][j]);
							fileRead(&sigmaActividades[i][j], sizeof(actividades[i][j]), 1, fDatos);
							printf("sigmaActividades[%d][%d] es %f\n", i,j, sigmaActividades[i][j]);
							}	
						}	
					}
				
				//*leemos los datos del fondo cargado
				fileReadBool(EstadoLeido.fondoCargado, fDatos);
				printf("--------------El estado del fondo es %d\n", EstadoLeido.fondoCargado);
				if(EstadoLeido.fondoCargado)
					{
					Datos.cuentasFondo=new float[Datos.numCanales];
					Datos.sigmaCuentasFondo=new float[Datos.numCanales];
					for(int i=0;i<Datos.numCanales;i++)
						{
						fileRead(&Datos.cuentasFondo[i], sizeof(Datos.cuentasFondo[i]),1,fDatos);
						fileRead(&Datos.sigmaCuentasFondo[i], sizeof(Datos.sigmaCuentasFondo[i]),1,fDatos);
						}
					fileRead(&Datos.numFondos, sizeof(Datos.numFondos),1,fDatos);
					Datos.mediasFondo			=new double[Datos.numFondos];
					Datos.incertidumbresFondo	=new double[Datos.numFondos];
					Datos.energiasFondo			=new double[Datos.numFondos];
					for(int i=0;i<Datos.numFondos;i++)
						{
						fileRead(&Datos.mediasFondo[i], sizeof(Datos.mediasFondo[i]),1,fDatos);
						fileRead(&Datos.incertidumbresFondo[i], sizeof(Datos.incertidumbresFondo[i]),1,fDatos);
						fileRead(&Datos.energiasFondo[i], sizeof(Datos.energiasFondo[i]),1,fDatos);
						printf("%.2f\t%.2f\t%.2f\n", Datos.energiasFondo[i], Datos.mediasFondo[i], Datos.incertidumbresFondo[i]);
						}
					}
				//	
				// lee la lista de los picos ajustados y sus ajustes
				printf("Leemos la lista de picos ajustados y sus ajustes\n");
				fileRead(&nElem, sizeof(nElem), 1, fDatos);
				for (int i = 0; i < nElem; i++) 
					{
					if ((nodoAnal = new NodoAnalisis) != NULL) 
						{
						// recupera el intervalo
						fileRead(&nodoAnal->numIntervalo, sizeof(nodoAnal->numIntervalo), 1, fDatos);
						agnadeNodo(listaAnalisis, nodoAnal);
						// recupera los ajustes
						NodoPicos *intervalo = DevuelveNodo(listaPicos, nodoAnal->numIntervalo);
						for (int j = 1; j <= intervalo->picos.numElementos; j++) 
							{
							NodoPico *pico = DevuelveNodo(intervalo->picos, j);
							lee_Ajuste(pico->Ajuste, fDatos);
							}
						}
					else 
						{
						bError = true;
						printf(ERROR_LOAD_FILE);
						printf(ERROR_NO_MEMORY_F, nombre);
						i = nElem;
						break;
						}
					}
				break;

			case BLOQUE_CALIBRACION:
				printf("LEYENDO BLOQUE DE CALIBRACIÓN\n");
				// Comprueba que este bloque no se ha leido ya
				if (++bloqueCalibracion > 1) 
					{
					bError = true;
					break;
					}
				
				
				printf("leemos la lista de picos ajustados y sus ajustes\n");
				// lee la lista de los picos ajustados y sus ajustes
				fileRead(&nElem, sizeof(nElem), 1, fDatos);
				for (int i = 0; i < nElem; i++) 
					{
					if ((nodoCalib = new NodoCalibracion) != NULL) 
						{
						// recupera el intervalo
						fileRead(&nodoCalib->numIntervalo, sizeof(nodoCalib->numIntervalo), 1, fDatos);
						agnadeNodo(listaCalibracion, nodoCalib);
						// recupera el ajuste
						NodoPicos *intervalo = DevuelveNodo(listaPicos, nodoCalib->numIntervalo);
						NodoPico *pico = intervalo->picos.inicio;//Sólo para un nodo
						lee_Ajuste(pico->Ajuste, fDatos);
						printf("Fin de la lectura del ajuste del pico %d\n",i);
						}
					else 
						{
						bError = true;
						printf(ERROR_LOAD_FILE);
						printf(ERROR_NO_MEMORY_F, nombre);
						i = nElem;
						break;
						}
					}
				if (bError) break; // punto de ruptura
				printf("Termina la lectura de ajustes\n");
				// Lee la calibracion en energia
				fileReadBool(EstadoLeido.CalibracionEnergia, fDatos);
				if (EstadoLeido.CalibracionEnergia) 
					{
					fileRead(&enp1, sizeof(enp1), 1, fDatos);
					fileRead(&enp2, sizeof(enp2), 1, fDatos);
					fileRead(&enp3, sizeof(enp3), 1, fDatos);
					for (int i = 1; i <= 3; i++)
						for (int j = 1; j <= 3; j++)
							fileRead(&covarEnergia[i-1][j-1], sizeof(covarEnergia[0][0]), 1, fDatos); // change to correct read
					fileRead(&Datos.chiEnergia, sizeof(Datos.chiEnergia), 1, fDatos);
					}
				// Lee la calibracion en parametros de forma
				fileReadBool(EstadoLeido.CalibracionForma, fDatos);
				if (EstadoLeido.CalibracionForma) 
					{
					//Sigma
					fileRead(&afwhg, sizeof(afwhg), 1, fDatos);
					fileRead(&bfwhg, sizeof(bfwhg), 1, fDatos);
					for (int i = 1; i <= 2; i++)
						for (int j = 1; j <= 2; j++)
							fileRead(&covarSigma[i-1][j-1], sizeof(covarSigma[0][0]), 1, fDatos); // change to correct read
					fileRead(&Datos.chiSigma, sizeof(Datos.chiSigma), 1, fDatos);
					//Tau
					fileRead(&atau, sizeof(atau), 1, fDatos);
					fileRead(&btau, sizeof(btau), 1, fDatos);
					for (int i = 1; i <= 2; i++)
						for (int j = 1; j <= 2; j++)
							fileRead(&covarTau[i-1][j-1], sizeof(covarTau[0][0]), 1, fDatos); // change to correct read
					fileRead(&Datos.chiTau, sizeof(Datos.chiTau), 1, fDatos);
					//Resolución
					fileRead(&ares, sizeof(ares), 1, fDatos);
					fileRead(&bres, sizeof(bres), 1, fDatos);
					for (int i = 1; i <= 2; i++)
						for (int j = 1; j <= 2; j++)
							fileRead(&covarRes[i-1][j-1], sizeof(covarRes[0][0]), 1, fDatos); // change to correct read
					}
				if (EstadoLeido.CalibracionEnergia && EstadoLeido.CalibracionForma)
					EstadoLeido.Calibracion = true;
				printf("TERMINA BLOQUE CALIBRACIÓN\n");
				break;

			case BLOQUE_CONTINUO:
				// Comprueba que este bloque no se ha leido ya
				if (++bloqueContinuo > 1) 
					{
					bError = true;
					break;
					}
				// lee la lista de datos de busqueda de canales del continuo/picos
				fileRead(&nElem, sizeof(nElem), 1, fDatos);
				for (int i = 0; i < nElem; i++) 
					{
					// lee el dato del continuo
					if ((nodoC = new NodoContinuo) != NULL) 
						{
						fileRead(&nodoC->cofo, sizeof(nodoC->cofo), 1, fDatos);
						fileRead(&nodoC->copi, sizeof(nodoC->copi), 1, fDatos);
						fileRead(&nodoC->manphe, sizeof(nodoC->manphe), 1, fDatos);
						fileRead(&nodoC->limInferior, sizeof(nodoC->limInferior), 1, fDatos);
						fileRead(&nodoC->limSuperior, sizeof(nodoC->limSuperior), 1, fDatos);
						agnadeNodo(listaContinuo, nodoC);
						}
					else 
						{
						bError = true;
						printf(ERROR_LOAD_FILE);
						printf(ERROR_NO_MEMORY_F, nombre);
						i = nElem;
						break;
						}
					}
				if (bError) break; // punto de ruptura
				// lee el precontinuo (si la hay)
				fileReadBool(EstadoLeido.PreContinuo, fDatos);
				if (EstadoLeido.PreContinuo) 
					{
					// lee los margenes de trabajo en la busqueda de picos y fondo
					fileRead(&minInterv, sizeof(minInterv), 1, fDatos);
					fileRead(&maxInterv, sizeof(maxInterv), 1, fDatos);
					// lee los canales de fondo y de picos
					int numCanalesFondo = numCanales/8 + 1;
					fondo = new unsigned char [numCanalesFondo];
					picos = new unsigned char [numCanalesFondo];
					if ((fondo == NULL) || (picos == NULL)) 
						{
						bError = true;
						printf(ERROR_LOAD_FILE);
						printf(ERROR_NO_MEMORY_F, nombre);
						break;
						}
					// lee los canales de fondo y de picos encontrados
					for (int i = 0; i < numCanalesFondo; i++)
						fileRead(&fondo[i], sizeof(fondo[0]), 1, fDatos);
					for (int i = 0; i < numCanalesFondo; i++)
						fileRead(&picos[i], sizeof(picos[0]), 1, fDatos);
					
					// lee el numero de picos encontrados
					fileRead(&numPicosEncontrados, sizeof(numPicosEncontrados), 1, fDatos);
					
					// lee la lista de picos formada en la busqueda de picos
					int numElementos;
					fileRead(&numElementos, sizeof(numElementos), 1, fDatos);
					for (int i = 1; i <= numElementos; i++) 
						{
						NodoPicos *nodo;
						if ((nodo = new NodoPicos) != NULL) 
							{
							nodo->picos.inicio = NULL; nodo->picos.numElementos = 0;
							
							fileRead(&nodo->limInferior, sizeof(nodo->limInferior), 1, fDatos);
							fileRead(&nodo->limSuperior, sizeof(nodo->limSuperior), 1, fDatos);
							int numElementos2;
							fileRead(&numElementos2, sizeof(numElementos2), 1, fDatos);
							for (int j = 1; j <= numElementos2; j++) 
								{
								NodoPico *nodo2 = new NodoPico;
								//
								//	iniciaNodo(nodo2);
								//
								if (nodo2 != NULL) 
									{
									fileRead(&nodo2->maximo, sizeof(nodo2->maximo), 1, fDatos);
									nodo2->Ajuste.inicializado = false;
									nodo2->Ajuste.ajustado = false;
									nodo2->Ajuste.calibrado = false;
									nodo2->Ajuste.identificado = false;
										
									nodo2->Ajuste.eficienciaCalculada = false;
									nodo2->Ajuste.ldCalculado = false;
									nodo2->Ajuste.analizado = false;
									agnadeNodo(nodo->picos, nodo2);
									}
								else 
									{
									bError = true;
					printf("delete 2\n");
					delete nodo;
									break;
									}
								}
							if (numElementos2 != nodo->picos.numElementos) bError = true;
							if (bError) break;
							agnadeNodo(listaPicos, nodo);
							}
						else 
							{
							bError = true;
							printf(ERROR_LOAD_FILE);
							printf(ERROR_NO_MEMORY_F, nombre);
							break;
							}							
						}
					if (numElementos != listaPicos.numElementos) bError = true;
					if (bError) break;
					// fin lectura de lista de picos encontrados
					
					// lee el ajuste del continuo
					fileReadBool(EstadoLeido.Continuo, fDatos);
					if (EstadoLeido.Continuo) 
						{
						fileRead(&nNodosSplineContinuo, sizeof(nNodosSplineContinuo), 1, fDatos);
						fileRead(&ChiSqContinuo, sizeof(ChiSqContinuo), 1, fDatos);
						// reserva la memoria necesaria
						continuo = new float [numCanales+1];
						sigmaContinuo = new float [numCanales+1];
						if ((continuo == NULL) || (sigmaContinuo == NULL)) 
							{
							bError = true;
							printf(ERROR_LOAD_FILE);
							printf(ERROR_NO_MEMORY_F, nombre);
							break;
							}
						// lee los canales de fondo y de picos encontrados
						for (int i = 1; i <= numCanales; i++) 
							{
							fileRead(&continuo[i], sizeof(continuo[0]), 1, fDatos);
							fileRead(&sigmaContinuo[i], sizeof(sigmaContinuo[0]), 1, fDatos);
							}
						}
					}
				break;
			case BLOQUE_PRECALIBRACION:
//				printf("leyendo datos precalibracion\n");
				// Comprueba que este bloque no se ha leido ya
				if (++bloquePreCalibracion > 1) 
					{
					bError = true;
					break;
					}
				fileRead(&nElem, sizeof(nElem), 1, fDatos);
				for (int i = 0; i < nElem; i++) 
					{
					// lee el dato de precalibracion
					if ((nodo = new NodoPrecalibracion) != NULL) 
						{
						fileRead(&nodo->energia, sizeof(nodo->energia), 1, fDatos);
						fileRead(&nodo->limInferior, sizeof(nodo->limInferior), 1, fDatos);
						fileRead(&nodo->limSuperior, sizeof(nodo->limSuperior), 1, fDatos);
						agnadeNodo(listaPrecalibracion, nodo);
						}
					else 
						{
						bError = true;
						printf(ERROR_LOAD_FILE);
						printf(ERROR_NO_MEMORY_F, nombre);
						break;
						}
					}
				if (bError) break;
				if (nElem > 2) 
					{ // lee el resultado de la precalibracion
					fileRead(&afwhg, sizeof(afwhg), 1, fDatos);
					fileRead(&bfwhg, sizeof(bfwhg), 1, fDatos);
					fileRead(&enp1, sizeof(enp1), 1, fDatos);				
					fileRead(&enp2, sizeof(enp2), 1, fDatos);				
					EstadoLeido.PreCalibracion = true;
					}
				break;
			case BLOQUE_CIERRE:
//				printf("leyendo final de fichero\n");
				bParar = true;
				break;
			default: // valor no reconocido
//				printf("valor no reconocido\n");
				bError = true;
			}
		if (bError) break;
		}
	fclose(fDatos);
//	printf("leidos todos los bloques\n");

	if (!bParar) 
		{ // salio del fichero sin la terminacion
		printf(ERROR_LOAD_FILE);
		printf(ERROR_CORRUPT_FILE, nombre);
		XtFree(nombre); XtFree(dir);
	
		delete [] espectro; delete [] fondo; delete [] picos;
		delete [] continuo; delete [] sigmaContinuo;
		borraLista(listaPicos); borraLista(listaCalibracion);
		borraLista(listaContinuo); borraLista(listaPrecalibracion);

		// vacia el programa a la espera de un espectro correcto
		ReseteaGraficas();
		DesactivaGrafica();
		DesactivaTrabajo();
		printf("El estado inicial es %d\n", ESTADO_INICIAL);
		PonActivaZona(ESTADO_INICIAL);
		printf("Quitamos widget\n");
		XtUnmanageChild (widget);
		return;
		}
	
	printf("Bloques leidos\n");
	// cargado nuevo fichero analgam
	if (NombreFicheroTrabajo != NULL) 
		{
		delete [] NombreFicheroTrabajo;
		NombreFicheroTrabajo = NULL;
		}
	NombreFicheroTrabajo = new char[strlen(nombre)+1];
	strcpy(NombreFicheroTrabajo, nombre);
	
	printf("Carga de datos\n");
	int nError = Datos.CargaDatos(espectro, numCanales);
	if (nError < 0) 
		{ // error al leer, pone todo a cero
		printf(ERROR_LOAD_FILE);
		if (nError == -1)
			printf(ERROR_NO_MEMORY_F, nombre);
		else printf(ERROR_GENERAL);
		XtFree(nombre); XtFree(dir);
		delete [] espectro; delete [] fondo; delete [] picos;
		delete [] continuo; delete [] sigmaContinuo;
		borraLista(listaPicos); borraLista(listaCalibracion);
		borraLista(listaContinuo); borraLista(listaPrecalibracion);

		// vacia el programa a la espera de un espectro correcto
		ReseteaGraficas();
		DesactivaGrafica();
		DesactivaTrabajo();
		PonActivaZona(ESTADO_INICIAL);
		}
	else 
		{
		//NUEVO
		//Datos.esFondo=false;
		//
		printf("Datos cargados, numCanales=%d\n", numCanales);
		delete [] espectro;
		if (Datos.numCanales > 0) 
			{
			EstadoTrabajo = EstadoLeido;

			// desactiva la visualizacion
			DesactivaTrabajo(); DesactivaGrafica();

			// cambia el titulo de la ventana principal al nombre del fichero
			if (TituloPrograma != NULL) 
				{
				printf("delete 16 (tituloprograma)\n");
				delete [] TituloPrograma;
				TituloPrograma = NULL;
				}		
			//sprintf(TituloPrograma, "Galea: %s", DaNombreFile(nombre, dir));
			
			TituloPrograma = DaNombreFile(nombre, dir);
			sprintf(nombre, "Galea: %s", TituloPrograma);
			XtVaSetValues(app_shell, XmNtitle, nombre, NULL);
//			XtFree(nombre); XtFree(dir);

			// Recupera los datos de precalibracion
			if (listaPrecalibracion.numElementos > 0) 
				{
				CopiaListaPreCalibracion(listaPrecalibracion);
				ReconstruyeListaPrecalibracion();
				}
			if (EstadoTrabajo.PreCalibracion) 
				{
				Datos.afwhg = afwhg; Datos.bfwhg = bfwhg;
				Datos.enp1 = enp1; Datos.enp2 = enp2;
				ActivaContinuo();
				}

			// Activa las zonas de la calibracion
			if (EstadoTrabajo.CalibracionEnergia) 
				{
				Datos.enp1 = enp1; Datos.enp2 = enp2; Datos.enp3 = enp3;
				for (int i = 1; i <= 3; i++)
					for (int j = 1; j <= 3; j++)
						Datos.covarEnergia[i*4+j] = covarEnergia[i-1][j-1];
				}
			if (EstadoTrabajo.CalibracionForma)
				{
				Datos.afwhg = afwhg; Datos.bfwhg = bfwhg;
				for (int i = 1; i <= 2; i++)
					for (int j = 1; j <= 2; j++)
						Datos.covarSigma[i*3+j] = covarSigma[i-1][j-1];
				Datos.atau = atau; Datos.btau = btau;
				for (int i = 1; i <= 2; i++)
					for (int j = 1; j <= 2; j++)
						Datos.covarTau[i*3+j] = covarTau[i-1][j-1];				
				Datos.ares = ares; Datos.bres = bres;
				for (int i = 1; i <= 2; i++)
					for (int j = 1; j <= 2; j++)
						Datos.covarRes[i*3+j] = covarRes[i-1][j-1];				
				}
			
			//Iniciamos los botones de la interfaz de actividad
			iniciaBotones();
			
			// Recupera la configuracion de busqueda de picos y fondo
			if (listaContinuo.numElementos > 0) 
				{
				CopiaListaContinuo(listaContinuo);
				ReconstruyeListaContinuo();
				}
			// Recupera los datos del precontinuo (busqueda de picos y fondo)
			if (EstadoTrabajo.PreContinuo) 
				{
				Datos.fondo = fondo;
				Datos.picos = picos;
				Datos.numCanalesFondo = Datos.numCanales/8 + 1;
				Datos.listaPicos = listaPicos;
				Datos.minInterv = minInterv;
				Datos.maxInterv = maxInterv;
				Datos.numPicosEncontrados = numPicosEncontrados;
				}
			else if (listaContinuo.numElementos > 0)
				EstadoTrabajo.SearchContinuumCambios = true;
			// Recupera los datos del ajuste del continuo
			printf("Recuperamos datos del continuo\n");
			if (EstadoTrabajo.Continuo) 
				{
				Datos.continuo = continuo;
				Datos.sigmaContinuo = sigmaContinuo;
				Datos.ChiSqContinuo = ChiSqContinuo;
				Datos.nNodosSplineContinuo = nNodosSplineContinuo;
				ActivaCalibracion();
				ReconstruyeListaIntervCalibracion();
				if (EstadoTrabajo.Calibracion/* && Datos.esFondo!=1*/) 
					{
					/* si ya esta la calibracion entonces ha de activar la
						zona de analisis */
					ActivaAnalisis();
					ReconstruyeListaIntervAnalisis();
					
					printf("VAMOS A LLAMAR A TODOS IDENTIFICADOS\n");
	//				if(todosIdentificados()/*&& EstadoTrabajo.Analisis*/ )
						ActivaActividad();
					}
				}
			else if (EstadoTrabajo.PreContinuo)
				EstadoTrabajo.AdjustContinuumCambios = true;
			// Recupera la lista de los picos para calibracion
			if (listaCalibracion.numElementos > 0) 
				{
				CopiaListaCalibracion(listaCalibracion);
				ReconstruyeListaCalibracion();
				// reconstruye el vector de ajuste de picos y continuo
					// **** ATENCION **** SIN COMPROBACION DE FALTA DE MEMORIA
				Datos.total = new float[Datos.numCanales+1];
				for (int i = 1; i <= Datos.numCanales; i++)
					Datos.total[i] = 0.0;
				for (int i = 1; i <= listaCalibracion.numElementos; i++) 
					{
					NodoCalibracion *nodo = DevuelveNodo(listaCalibracion, i);
					Datos.ActualizaIntervaloCalibracion(nodo->numIntervalo);
					}
				}
						
			if (EstadoTrabajo.Calibracion) ActivaContinuo();
			// Recupera la lista de los picos para analisis
			if (listaAnalisis.numElementos > 0) 
				{
				CopiaListaAnalisis(listaAnalisis);
				ReconstruyeListaAnalisis();
				printf("Continuamos cargando el fichero\n");	
				// reconstruye el vector de ajuste de picos
					// **** ATENCION **** SIN COMPROBACION DE FALTA DE MEMORIA
				if (Datos.total == NULL) 
					{
					Datos.total = new float[Datos.numCanales+1];
					for (int i = 1; i <= Datos.numCanales; i++)	Datos.total[i] = 0.0;
					}
				printf("Acutalizamos datos intervalos\n");
				for (int i = 1; i <= listaAnalisis.numElementos; i++) 
					{
					printf("Intervalo %d\n");
					NodoAnalisis *nodo = DevuelveNodo(listaAnalisis, i);
					Datos.ActualizaIntervaloAnalisis(nodo->numIntervalo);
					}
				printf("Terminado, seguimos\n");
				}
				
			//Pone nombre al espectro:
			Datos.nombre = new char[100];
			strcpy(Datos.nombre, NombreFicheroTrabajo);
			// Actualiza la ventana de datos
			if (EstadoTrabajo.PreCalibracion)				ActivaPreCalibDatos();
			if (EstadoTrabajo.CalibracionEnergia)			ActivaEnergiaDatos();
			if (EstadoTrabajo.CalibracionForma)				ActivaFormaDatos();
			// Inicializa las graficas
			nError = CopiaEspectroAGrafica();
			if (EstadoTrabajo.PreContinuo) 
				{
				nError = CopiaPicosChGrafica();
				nError = CopiaContinuoChGrafica();
				}
			if (EstadoTrabajo.Continuo)
				nError = CopiaContinuoAGrafica();
			if ((listaCalibracion.numElementos > 0)||(listaAnalisis.numElementos > 0))
				nError = CopiaAjustesAGrafica();
			
			ActivaTrabajo();
			ActivaGrafica();
			PonActivaZona(EstadoTrabajo.Actual);

			// pone la visualizacion
			EstadoVisual = EstadoVisualLeido;
			EstableceVisual(EstadoVisual);
	
			// por si acaso, actualiza la grafica
			if(EstadoTrabajo.fondoCargado)	
				{
				int inicio=0;
				int fin=0;
				for(int i=1;i<Datos.numCanales;i++)
					if(Datos.continuo[i]!=0)	
						{
						inicio=i;
						i=Datos.numCanales;//Para acabar
						}
				for(int i=Datos.numCanales;i>inicio;i--)
					if(Datos.continuo[i]!=0)	
						{
						fin=i;
						i=inicio;//Para que acabe el bucle
						}
				
					//PROVISIONAL: A poner
				/*BorraFondoEnGrafica();
				nError=CopiaFondoAGrafica(inicio, fin);
				ActivaFondoGrafica(true);*/
				}

			//printf("Terminada la carga, creamos la lista de actividad \n");
			//creaListaActividad();

			PonPosicionCursor();
			PintaGrafica(3);
			}
		else 
			{
			printf(ERROR_LOAD_FILE);
			printf("Error en punto 0, %d\n", numCanales);
			printf(ERROR_FICHERO_VACIO, nombre);
//			XtFree(nombre); XtFree(dir);
			delete [] espectro; delete [] fondo; delete [] picos;
			delete [] continuo; delete [] sigmaContinuo;
			borraLista(listaPicos); borraLista(listaCalibracion);
			borraLista(listaContinuo); borraLista(listaPrecalibracion);

			// vacia el programa a la espera de un espectro correcto
			DesactivaGrafica();
			DesactivaTrabajo();
			PonActivaZona(ESTADO_INICIAL);
			}
		}
	
	printf("FINAL de lectura\n");
	XtUnmanageChild (widget);

	//manejo de memoria
	printf("liberamos memoria\n");
	XtFree(nombre);		XtFree(dir);
 	}
	
//****************** LEER FONDO ********************
/* Interpreta la información que se encuentra en el fichero del fondo
	e inicia en consecuencia el programa
	*/
void leer_fondo(Widget widget, XtPointer client_data, XtPointer call_data) 
	{
	leer_espectro(widget, client_data, call_data);
	printf("SETEAMOS A TRUE EL FONDO\n");
	Datos.esFondo=true;
	printf("Datos.esfondo=%d\n", Datos.esFondo);

	//NOTA: Ahora, como abrimos un fondo, preguntamos a qué base de datos de
	//		fondo va a pertenecer este
	XmString patron = XmStringCreateLocalized ((char *)LOAD_BDBACKGROUND_TITLE);
	XtVaSetValues (dlg_files, XmNdialogTitle, patron, NULL);
	XmStringFree (patron);
	patron = XmStringCreateLocalized ((char *)FILTER_FICH_BDBACKGROUND);
	XtVaSetValues (dlg_files, XmNpattern, patron, NULL);
	XmStringFree (patron);
	XtRemoveAllCallbacks (dlg_files, XmNokCallback);
	XtAddCallback (dlg_files, XmNokCallback, leer_bdfondo, NULL);
		
	//Ponemos el fichero de preferencias
	//UltAnalDirOpen = Preferences.GetBackBDDir();
	UltAnalDirOpen = getcwd(NULL,0);
		
	if (UltAnalDirOpen != NULL) 
		{
		patron = XmStringCreateLocalized(UltAnalDirOpen);
		XtVaSetValues (dlg_files, XmNdirectory, patron, NULL);
		XmStringFree (patron);
		}
	printf("EL dialog files está a %d\n", XtIsManaged(dlg_files));
	// Pone el menu de carga de ficheros en pantalla
	// Si hay puesto otro menu de ficheros lo quita
	if (!XtIsManaged(dlg_files))	
		{
		XtManageChild (dlg_files);
		printf("Imprimimos el cuadro de diálogo\n");
		}
	}
	
//****************** LEER BDFONDO ********************
/* Guarda el nombre de la base de datos de fondo para usarlo en el futuro
	*/
void leer_bdfondo(Widget widget, XtPointer client_data, XtPointer call_data) 
	{
	printf("Leyendo la base de datos del fondo\n");
    char *nombre = NULL, *dir = NULL;
	XmFileSelectionBoxCallbackStruct *cbs = (XmFileSelectionBoxCallbackStruct *) call_data;
	
	//1.- Abrimos el fichero
    XmStringGetLtoR(cbs->value, XmFONTLIST_DEFAULT_TAG, &nombre); // nombre de fichero a abrir
    XmStringGetLtoR(cbs->dir, XmFONTLIST_DEFAULT_TAG, &dir); // nombre de directorio
	nombreFichFBD=nombre;
	printf("NombreFichFBD=%s\n", nombreFichFBD);
	XtUnmanageChild(widget);//Quitamos el cuadro de diálogo de apertura
	//manejo de memoria
	XtFree(nombre); XtFree(dir);
	}
 	
//****************** LEER ESPECTRO ********************
/* Interpreta la información que se encuentra en el fichero del espectro
	e inicia en consecuencia el programa
	NOTA: Queremos que el propio programa a partir del tipo de fichero, 
	interprete según el formato.
	*/
void leer_espectro(Widget widget, XtPointer client_data, XtPointer call_data) 
	{
    char *nombre = NULL, *dir = NULL;
	XmFileSelectionBoxCallbackStruct *cbs = (XmFileSelectionBoxCallbackStruct *) call_data;

	//1.- Abrimos el fichero
    XmStringGetLtoR(cbs->value, XmFONTLIST_DEFAULT_TAG, &nombre); // nombre de fichero a abrir
    XmStringGetLtoR(cbs->dir, XmFONTLIST_DEFAULT_TAG, &dir); // nombre de directorio

	// guarda el ultimo directorio visitado
	delete [] UltSpecDirOpen;
	UltSpecDirOpen = CopyString(dir);

   if (strlen(nombre) == strlen(dir)) 
	   { // no hay nombre de fichero
		XtFree(nombre); XtFree(dir);
		Aviso(main_w, "You must to select a file");
		return;
		}

	//  ***   CONSTRUIMOS SEGÚN EL FORMATO  ***
	// Mostramos un cuadro de diálogo con los tipos de formato disponibles
	int nc=0;
	int tvivo=0;
	int *canales;
	char buffer[256];
	int linea=0;
	int pos=0;
	string stemp, cadena;
			
	if(tipoFormato==2)//------------- 	CANBERRA
		{
		printf("Elegido Canberra: %s\n", nombre);
		//Para Canberra tenemos que adaptar el programa formage
		ifstream file(nombre);
		if (! file.is_open()) 	{ cout << "Error opening file"; exit(1); }
  		//Buscamos el tiempo vivo
	//	printf("Buscamos tiempo vivo\n");
		while (! file.eof() && linea<9)
  			{
	    	file.getline (buffer,100);
			linea++;
			}
		cadena=string(buffer);
		if( (pos=cadena.find("\"Elp Live\"", 0))!=string::npos)
		cadena=cadena.substr(pos+10);
		tvivo=atoi(cadena.c_str());
		printf("Elp Live=%d\n", tvivo);
		
		//Buscamos el número de canales
		while (! file.eof() && linea<16)
  			{
	    	file.getline (buffer,100);
			linea++;
			}
		cadena=string(buffer);
		if( (pos=cadena.find("\"Channels:\"", 0))!=string::npos)
		cadena=cadena.substr(pos+11);
		nc=atoi(cadena.c_str());
		printf("Channels: %d\n", nc);
		//Buscamos los valores de los canales
		while (! file.eof() && linea<54)
  			{
	    	file.getline (buffer,100);
			linea++;
			}
			
		canales=new int[nc];
		int i=0;
		while(i<nc)
			{
			if(!file.eof())
				{
		    	file.getline (buffer,100);//Cada línea tiene 8 canales
				cadena=string(buffer);
				for(int j=0;j<8;j++)
					{
					pos=cadena.find("  ", 0);
					stemp=cadena.substr(0,pos);
					cadena=cadena.substr(pos+2);
					canales[i]=atoi(stemp.c_str());
					i++;
					}
				}
			else	{Aviso(main_w, "ERROR: Corrupt file or wrong format"); return;}
			}
		file.close();
			
		//Construimos nuestro fichero a partir de los datos extraidos de este formato
		//Mismo nombre con extensión .dat
		cadena=string(nombre);
		pos=cadena.find(".");
		cadena=cadena.substr(0,pos)+".dat";
		ofstream of(cadena.c_str(), ofstream :: trunc);
		of << tvivo << endl;
		of << nc << endl;
		for(int i=0;i<nc;i++)	of << canales[i] << endl;
		of << endl;
		of.close();	
		strcpy(nombre, cadena.c_str());		
		}
	else
		{
		if(tipoFormato==3)//----------------------------ORTEC
			{
			//NOTA: De momento comentado, se usa como formato interno .dat
			//NOTA: No está probado ni terminado de integrar porque no tenemos
			//		un fichero con ese tipo .chn con el que probar que rula
			//NOTA: Copiado del manual Maestro II de Ortec, pág 223
			int f_type; //CHN file type
			char acq_time[32];	//buffer for time and date
			unsigned int chan_offset,	//beginning channel number of ata
						 count,		//general purpose loop counter
						mca_num,	// 1-4 are valid
						num_chans,	//number of data channels
						num_writ,	//number of bytes written out
						segment;	//segment number
			long int	livetime,	//20ms tics of livetime
						realtime,	//20ms tics of realtime
						chan_data;	//stores channel data
			FILE *f_pointer;
			
			f_pointer = fopen(nombre, "rb");
				
			//*********** CABECERA ****************
			fread(&f_type,sizeof(int),1 ,f_pointer);
			if(f_type != -1)
				{
				Aviso(main_w, "ERROR: Not a valid file");
				//exit(1);
				}
			fread(&mca_num,sizeof(int),1,f_pointer);
			fread(&segment,sizeof(int),1 ,f_pointer);
			printf("Tomado el segmento\n");
			fread(acq_time+12,sizeof(char),2 ,f_pointer);
			printf("Tomado el tiempo\n");
			fread(&realtime,sizeof(long),1 ,f_pointer);
			fread(&livetime,sizeof(long),1 ,f_pointer);
			fread(acq_time,sizeof(char),2 ,f_pointer);
			fread(acq_time+2,sizeof(char),3 ,f_pointer);
			fread(acq_time+5,sizeof(char),3 ,f_pointer);
			fread(acq_time+8,sizeof(char),2 ,f_pointer);
			fread(acq_time+10,sizeof(char),2 ,f_pointer);
			fread(&chan_offset,sizeof(int),1 ,f_pointer);
			fread(&num_chans,sizeof(int),1 ,f_pointer);
			
			printf("TYPE = %4i MCA # %2i SEGMENT # %3i\n", f_type, mca_num, segment);
			printf("REALTIME = %10li SECONDS, LIVETIME = %10li SECONDS\n", realtime/50, livetime/50);
			
			printf("DATA COLLECTED AT ");
			fwrite(acq_time+8, sizeof(char), 2, stdout);				
			putchar(':');
			fwrite(acq_time+10, sizeof(char), 2, stdout);				
			putchar(':');
			fwrite(acq_time+12, sizeof(char), 2, stdout);				

			printf(" ON ");
			fwrite(acq_time, sizeof(char), 2, stdout);				
			putchar('-');
			fwrite(acq_time+2, sizeof(char), 3, stdout);				
			putchar('-');
			fwrite(acq_time+5, sizeof(char), 2, stdout);				
			
			printf("\nSTARTING CHANNEL = %6i, NUMBER OF CHANNELS = %6i\n\n", chan_offset, num_chans);
			
			//******************** CANALES
			printf("CHANNEL	    DATA\n");
			for(count =0; count<num_chans;count++)
				{
		/*		if((count%6)==0)	//Cada 6 canales salto de línea y
					{
					printf("\n%7i", count);//imprimir el número de canal
					}*/
				fread(&chan_data, sizeof(long),1,f_pointer);
//				printf("%1li", chan_data);
				}
			fcloseall();
			}
		else
			{
			if(tipoFormato==4)//-------------------- 	LAP
				{
				printf("Elegido LAP\n");
				//En LAP sólo tenemos que quitar la primera línea y pasar a 
				//segundos los milisegundos
				nc=numeroFilas(nombre);
				nc=nc-3;
				canales=new int[nc];

				ifstream file(nombre);
				if (! file.is_open()) 	{ cout << "Error opening file"; exit(1); }
		  		//Buscamos el tiempo vivo
				while (! file.eof() && linea<2)
		  			{
	    			file.getline (buffer,100);
					linea++;
					}
				printf("Dividimos %d entre 1000\n", atoi(buffer));
				tvivo=atoi(buffer)/1000;
				for(int i=0;i<nc;i++)
					{
					if(!file.eof())
						{
						file.getline(buffer,100);
						canales[i]=atoi(buffer);
						cout << canales [i] << " ";
						}
					else			Aviso(main_w, "ERROR: Corrupt file or wrong format");
					}
				file.close();
				//Construimos nuestro fichero a partir de los datos extraidos de este formato
				//Mismo nombre con extensión .dat
				cadena=string(nombre);
				pos=cadena.find(".");
				cadena=cadena.substr(0,pos)+".dat";
				ofstream of(cadena.c_str(), ofstream :: trunc);
				of << tvivo << endl;
				of << nc << endl;
				for(int i=0;i<nc;i++)	of << canales[i] << endl;
				of << endl;
				of.close();	
				strcpy(nombre, cadena.c_str());		
				}
			else
				{
				if(tipoFormato == 1)	;
				else
					{
					printf("ERROR: Tipo de Formato Desconocido\n");
					//manejo de memoria
					XtFree(nombre); XtFree(dir);
					return;
					}
				}
			}
		}
//----------------------fin formato
		
	//  ***   LEE EL ESPECTRO   ***

	// Carga los datos en una matriz
    FILE *fDatos;
	unsigned int* espectro;
	unsigned short numCanales;
   
   fDatos = fopen(nombre, "r");
	
   if (fDatos == NULL) 
	   { // no pudo abrir el fichero
		XtFree(nombre); XtFree(dir);
		printf(ERROR_LOAD_SPECTRUM);
		printf(ERROR_NO_LOAD_SPECTRUM, nombre);
		XtUnmanageChild (widget);
		return;
		}
	float tiempo;
	cout << "LEEMOS TIEMPO DE MEDIDA" << endl;
    fscanf(fDatos, "%f", &tiempo);  // lee el tiempo de medida
	cout << "Tiempo de medida leido, es " << tiempo << endl;

	int temp;
    fscanf(fDatos, "%u", &temp);  // lee el numero de canales
	numCanales = temp; // *** por compatibilidad con formatos de lectura ***
	espectro = new unsigned int[numCanales]; // Reserva la memoria necesaria
    if (espectro == NULL) 
		{ // no hay memoria disponible
      	fclose(fDatos);
		XtFree(nombre); XtFree(dir);
		printf(ERROR_LOAD_SPECTRUM);
		printf(ERROR_NO_MEMORY_F, nombre);
		XtUnmanageChild (widget);
		return;
   		}
	int nError;
    for (int i = 0; i < numCanales; i++) // lee los datos
		{
		nError = fscanf(fDatos, "%u", &espectro[i]);
		}
	
	if (nError == EOF) 
		{
		XtFree(nombre); XtFree(dir);
	    fclose(fDatos);
		printf(ERROR_LOAD_SPECTRUM);
		printf(ERROR_CORRUPT_SPECTRUM, nombre);
		delete [] espectro;
		XtUnmanageChild (widget);
		return;
		}

   fclose(fDatos);

	// cargado fichero de espectros, no hay fichero analgam
	if (NombreFicheroTrabajo != NULL) 
		{
		delete [] NombreFicheroTrabajo;
		NombreFicheroTrabajo = NULL;
		}

	// Vacia el contenido anterior
	ReseteaDatos();
	XtVaSetValues(app_shell, XmNtitle, SIN_FICHERO, NULL);
	
	// Carga los datos
	Datos.BorraDatos();
	nError = Datos.CargaDatos(espectro, numCanales);
	Datos.tiempo=tiempo;
	Datos.esFondo=false;
	Datos.nombre = new char[100];
	printf("EL TIEMPO DE MEDICIÓN ES %f, Y EL NÚMERO DE CANALES %d\n", Datos.tiempo, numCanales);
	strcpy(Datos.nombre, nombre);
	printf("EL NOMBRE ES %s\n", Datos.nombre);
	if (nError < 0) 
	    { // error al leer, pone todo a cero
		printf(ERROR_LOAD_SPECTRUM);
		if (nError == -1)		printf(ERROR_NO_MEMORY_F, nombre);
		else 					printf(ERROR_GENERAL);
		delete [] espectro;
		XtFree(nombre); XtFree(dir);

		// vacia el programa a la espera de un espectro correcto
		DesactivaGrafica();
		DesactivaTrabajo();
		PonActivaZona(ESTADO_INICIAL);
		}
	else  //No ha habido error en la carga
		{
		delete [] espectro;
		if (Datos.numCanales > 0) 
			{
			// cambia el titulo de la ventana principal al nombre del fichero
			if (TituloPrograma != NULL) 
				{
				delete [] TituloPrograma;
				TituloPrograma = NULL;
				}
	
			TituloPrograma = DaNombreFile(nombre, dir);
			sprintf(nombre, "Galea: %s", TituloPrograma);
	
			XtVaSetValues(app_shell, XmNtitle, nombre, NULL);
			
			nError = CopiaEspectroAGrafica();
			EstadoTrabajo.Actual = ESTADO_PRECALIBRACION;
			PonActivaZona(EstadoTrabajo.Actual);
			ActivaGrafica();
			DesactivaTrabajo();
			ActivaTrabajo();
			DesactivaEnergiaDatos();
			DesactivaContinuo();
			PintaGrafica(3);
			PonPosicionCursor();
			}
		else 
			{
			printf(ERROR_LOAD_SPECTRUM);
			printf(ERROR_FICHERO_VACIO, nombre);

			// deja todo desactivado a la espera de un espectro correcto
			DesactivaGrafica();
			DesactivaTrabajo();
			PonActivaZona(EstadoTrabajo.Actual);
			}
		}
	EstadoTrabajo.TrabajoCambios = false;
	XtUnmanageChild (widget);
	//manejo de memoria
	XtFree(nombre); XtFree(dir);
	printf("Terminamos la carga del espectro\n");
}


	/***********************
	*****
	***** 	SALVAR FICHEROS
	*****
	************************/
//********************** GRABAR ESPECTRO ****************************
/* Graba toda la información de un espectro en un fichero.asf (Analgam Spectrum File)
*/
void grabar_espectro(Widget widget, XtPointer client_data, XtPointer call_data) 
	{
   	char *nombre = NULL, *dir = NULL;
   	FILE *fDatos; // puntero al fichero donde se graba
	XmFileSelectionBoxCallbackStruct *cbs = (XmFileSelectionBoxCallbackStruct *) call_data;

   	XmStringGetLtoR(cbs->value, XmFONTLIST_DEFAULT_TAG, &nombre);
   	XmStringGetLtoR(cbs->dir, XmFONTLIST_DEFAULT_TAG, &dir);
	
	// guarda el ultimo directorio visitado
	delete [] UltAnalDirOpen;
	UltAnalDirOpen = CopyString(dir);

	// recupera la extension
	char *ext = DaExtensionFile(nombre, dir);
	
	// comprueba si hay que agnadir la extension
	if ((ext == NULL)||(strcmp(ext, EXT_FICH_GENERAL) != 0)) 
		{
		// hay que agnadir la extension del fichero
		char *otro = new char[strlen(nombre) + strlen(EXT_FICH_GENERAL) + 1];
		if (otro == NULL) 
			{
			XtFree(nombre); XtFree(dir); delete [] ext;
			printf(ERROR_SAVE_FILE);
			printf(ERROR_NO_MEMORY);
			return;
			}
		strcpy(otro, nombre);
		strcat(otro, EXT_FICH_GENERAL);
		XtFree(nombre);
		nombre = otro;
		}
	delete [] ext;
	
	// Comprueba si el fichero ya existe en un Save As ...
	if ((fDatos = fopen(nombre, "rb")) != NULL) 
		{
		fclose(fDatos);
		// Existe el fichero, pregunta antes de sobreescribirlo
		char pregunta[80];
		char *nombreCorto = DaNombreFile(nombre, dir);
		sprintf(pregunta, ASK_OVERWRITE, nombreCorto);
		delete [] nombreCorto;
		int Resp = Continuar(dlg_files, pregunta, (char *)OVERWRITE_TITLE);
		switch(Resp) 
			{
			case CANCELAR :
				XtUnmanageChild (widget);
			case NO :
				XtFree(nombre); XtFree(dir);
				return;
			}
		}

	// Graba el fichero
	GuardaFichero(nombre);
	EstadoTrabajo.TrabajoCambios = false;
	
	// cambia el nombre del fichero con el que se trabaja
	if (NombreFicheroTrabajo != NULL) 
		{
		delete [] NombreFicheroTrabajo;
		NombreFicheroTrabajo = NULL;
		}
	int numLetras = strlen(nombre);
	if ((NombreFicheroTrabajo = new char[numLetras + 1]) == NULL) 
		{
		XtFree(nombre); XtFree(dir);
		printf(ERROR_SAVE_FILE);
		printf(ERROR_NO_MEMORY);
		return;
		}
	strcpy(NombreFicheroTrabajo, nombre);

	// cambia el titulo de la ventana principal al nombre del fichero
	if (TituloPrograma != NULL) 
		{
		delete [] TituloPrograma;
		TituloPrograma = NULL;
		}
	
	TituloPrograma = DaNombreFile(nombre, dir);
	XtVaSetValues(app_shell, XmNtitle, TituloPrograma, NULL);
	XtFree(nombre); XtFree(dir);

	// quita el menu de grabar archivo
	XtUnmanageChild (widget);
	}

//****************** GUARDA FICHERO ********************
/*	Salvamos fichero .asf
	*/
int GuardaFichero(const char* nombre) 
	{
	printf("GUARDAR FICHERO\n");
   	FILE *fDatos; // puntero al fichero donde se graba
	char Indicador;
	unsigned short Canales; // indica numero de canales
			// y los propios canales si todos son menores de este tamagno
	unsigned int Canales2; // indica los canales si no se pueden indicar
			// con el formato anterior
	tPrecalibracion ElementoPrecalibracion;
	tListaPrecalib* listaPrecalibracion;
		
	// Graba los datos actuales en el fichero indicado
	if ((fDatos = fopen(nombre, "wb")) == NULL) 
		{
		printf(ERROR_SAVE_FILE);
		printf(ERROR_NO_SAVE_FILE);
		return -1;
		}

	// *** GRABA EL FICHERO ***
	// Sigue el formato indicado en ExtensionAsf.txt
	
	// Cabecera identificativa de analgam file
	fileWrite(&CABECERA_ASF, sizeof(CABECERA_ASF[0]), sizeof(CABECERA_ASF)-1, fDatos);

	printf("GUARDAMOS SI ES FONDO %d\n", Datos.esFondo);
	// Guardamos si es fondo o no:
	fileWrite(&Datos.esFondo, sizeof(Datos.esFondo), 1, fDatos);
	//
	printf("Datos.esFondo vale %d y Estado.CalibraciónEnergía: %d\n", Datos.esFondo, EstadoTrabajo.CalibracionEnergia);
	if(Datos.esFondo)
		{
		int tam=strlen(nombreFichFBD);
		fileWrite(&tam, sizeof(int), 1, fDatos);
		fileWrite(nombreFichFBD, sizeof(char)*tam, 1, fDatos);
		}
	
	printf("Guardamos el tiempo de medición: %f\n", Datos.tiempo);
	fileWrite(&Datos.tiempo, sizeof(Datos.tiempo), 1, fDatos);
	
	// Numero de canales
	Canales = Datos.numCanales;
	printf("Guardamos número de canales %d\n", Canales);
	fileWrite(&Canales, sizeof(Canales), 1, fDatos);
	// Canales
	printf("Guardamos los canales\n");
	if (Datos.maximo(1, Datos.numCanales) < 65536) 
		{
		Indicador = 0;
		fileWrite(&Indicador, sizeof(Indicador), 1, fDatos);
		for (int i = 1; i <= Datos.numCanales; i++) 
			{
			Canales = (unsigned short) Datos.espectro[i];
			fileWrite(&Canales, sizeof(Canales), 1, fDatos);
			}
		}
	else
		{
		Indicador = 1;
		fileWrite(&Indicador, sizeof(Indicador), 1, fDatos);
		for (int i = 1; i <= Datos.numCanales; i++) 
			{
			Canales2 = (unsigned int) Datos.espectro[i];
			fileWrite(&Canales2, sizeof(Canales2), 1, fDatos);
			}
		}
	
	// Escribe el modo actual de trabajo
	fileWrite(&EstadoTrabajo.Actual, sizeof(EstadoTrabajo.Actual), 1, fDatos);
	
	// Escribe la precalibracion
	printf("Guardamos la precalibración\n");
	listaPrecalibracion = DevuelveListaPreCalibracion();
	if (listaPrecalibracion->numElementos > 0) 
		{
		Indicador = BLOQUE_PRECALIBRACION;
		fileWrite(&Indicador, sizeof(Indicador), 1, fDatos);
		fileWrite(&listaPrecalibracion->numElementos, sizeof(listaPrecalibracion->numElementos), 1, fDatos);
		for (int i = 1; i <= listaPrecalibracion->numElementos; i++) 
			{
			DevuelveDatoPreCalibracion(*listaPrecalibracion, i, ElementoPrecalibracion);
			// Escribe el dato de la lista en el fichero
			fileWrite(&ElementoPrecalibracion.energia, sizeof(ElementoPrecalibracion.energia), 1, fDatos);
			fileWrite(&ElementoPrecalibracion.limInferior, sizeof(ElementoPrecalibracion.limInferior), 1, fDatos);
			fileWrite(&ElementoPrecalibracion.limSuperior, sizeof(ElementoPrecalibracion.limSuperior), 1, fDatos);				
			}
		if (EstadoTrabajo.PreCalibracion) 
			{ // Escribe el resultado de la precalibracion
			fileWrite(&Datos.afwhg, sizeof(Datos.afwhg), 1, fDatos);
			fileWrite(&Datos.bfwhg, sizeof(Datos.bfwhg), 1, fDatos);
			fileWrite(&Datos.enp1, sizeof(Datos.enp1), 1, fDatos);				
			fileWrite(&Datos.enp2, sizeof(Datos.enp2), 1, fDatos);				
			}
		}
	
	// Escribe el continuo
	printf("Guardamos el continuo\n");
	tListaContinuo* listaContinuo = DevuelveListaContinuo();
	if (listaContinuo->numElementos > 0) 
		{
		// Indica bloque de datos de continuo
		Indicador = BLOQUE_CONTINUO;
		fileWrite(&Indicador, sizeof(Indicador), 1, fDatos);

		// escribe la lista de datos busqueda de picos/continuo
		fileWrite(&listaContinuo->numElementos, sizeof(listaContinuo->numElementos), 1, fDatos);
		for (int i = 1; i <= listaContinuo->numElementos; i++) 
			{
			NodoContinuo *nodo = DevuelveNodo(*listaContinuo, i);
			// Escribe el dato de la lista en el fichero
			fileWrite(&(nodo->cofo), sizeof(nodo->cofo), 1, fDatos);
			fileWrite(&(nodo->copi), sizeof(nodo->copi), 1, fDatos);
			fileWrite(&(nodo->manphe), sizeof(nodo->manphe), 1, fDatos);
			fileWrite(&(nodo->limInferior), sizeof(nodo->limInferior), 1, fDatos);
			fileWrite(&(nodo->limSuperior), sizeof(nodo->limSuperior), 1, fDatos);
			}

		// Escribe los datos del precontinuo (si lo hay y esta actualizado)
		if ((!EstadoTrabajo.SearchContinuumCambios)&&(EstadoTrabajo.PreContinuo)) 
			{
			fileWriteBool(EstadoTrabajo.PreContinuo, fDatos);
			fileWrite(&Datos.minInterv, sizeof(Datos.minInterv), 1, fDatos);
			fileWrite(&Datos.maxInterv, sizeof(Datos.maxInterv), 1, fDatos);

			// escribe los datos de los canales de fondo y de picos
			for (int i = 0; i < Datos.numCanalesFondo; i++)
				fileWrite(&Datos.fondo[i], sizeof(Datos.fondo[0]), 1, fDatos);
			for (int i = 0; i < Datos.numCanalesFondo; i++)
				fileWrite(&Datos.picos[i], sizeof(Datos.fondo[0]), 1, fDatos);

			// escribe el numero de picos encontrados
			fileWrite(&Datos.numPicosEncontrados, sizeof(Datos.numPicosEncontrados), 1, fDatos);

			// escribe la lista de picos encontrados
			fileWrite(&Datos.listaPicos.numElementos, sizeof(Datos.listaPicos.numElementos), 1, fDatos);
			for (int i = 1; i <= Datos.listaPicos.numElementos; i++) 
				{
				NodoPicos *nodo = DevuelveNodo(Datos.listaPicos, i);
				fileWrite(&nodo->limInferior, sizeof(nodo->limInferior), 1, fDatos);
				fileWrite(&nodo->limSuperior, sizeof(nodo->limSuperior), 1, fDatos);
				fileWrite(&nodo->picos.numElementos, sizeof(nodo->picos.numElementos), 1, fDatos);
				for (int j = 1; j <= nodo->picos.numElementos; j++) 
					{
					NodoPico *nodo2 = DevuelveNodo(nodo->picos, j);
					fileWrite(&nodo2->maximo, sizeof(nodo2->maximo), 1, fDatos);
					}
				}
			
			// Escribe los datos del ajuste del continuo (si lo hay y esta actualizado)
			if ((!EstadoTrabajo.AdjustContinuumCambios)&&(EstadoTrabajo.Continuo)) 
				{
				fileWriteBool(EstadoTrabajo.Continuo, fDatos);
				fileWrite(&Datos.nNodosSplineContinuo, sizeof(Datos.nNodosSplineContinuo), 1, fDatos);
				fileWrite(&Datos.ChiSqContinuo, sizeof(Datos.ChiSqContinuo), 1, fDatos);
				
				// escribe los datos del continuo ajustado y su sigma
				for (int i = 1; i <= Datos.numCanales; i++) 
					{
					fileWrite(&Datos.continuo[i], sizeof(Datos.continuo[0]), 1, fDatos);
					fileWrite(&Datos.sigmaContinuo[i], sizeof(Datos.sigmaContinuo[0]), 1, fDatos);
					}
				}
			else 
				{
				bool temp = false;
				fileWriteBool(temp, fDatos);
				}
			}
		else 
			{
			bool temp = false;
			fileWriteBool(temp, fDatos);
			}
		}
	
	// Escribe la calibracion
	printf("Guardamos la calibración\n");
	tListaCalibracion* listaCalibracion = DevuelveListaCalibracion();
	if ((listaCalibracion->numElementos > 0)||
		(EstadoTrabajo.CalibracionEnergia)||(EstadoTrabajo.CalibracionForma)) 
		{
		// Indica bloque de datos de calibracion
		printf("Guardamos bloque de calibración\n");
		Indicador = BLOQUE_CALIBRACION;
		fileWrite(&Indicador, sizeof(Indicador), 1, fDatos);
		
		printf("Escribimos lista calibración\n");
		// escribe la lista intervalos ajustados y los ajustes de los picos
		fileWrite(&listaCalibracion->numElementos, sizeof(listaCalibracion->numElementos), 1, fDatos);
		for (int i = 1; i <= listaCalibracion->numElementos; i++) 
			{
			NodoCalibracion *nodo = DevuelveNodo(*listaCalibracion, i);
			fileWrite(&(nodo->numIntervalo), sizeof(nodo->numIntervalo), 1, fDatos);
			// escribe el ajuste del pico
			NodoPicos *intervalo = DevuelveNodo(Datos.listaPicos, nodo->numIntervalo);
			NodoPico *pico = intervalo->picos.inicio;
			printf("Escribimos el ajuste de este pico\n");
			escribe_Ajuste(pico->Ajuste, fDatos);
			printf("Ajuste escrito, pasamos al siguiente\n");
			}
		
		// Escribe la calibracion de energia
		fileWriteBool(EstadoTrabajo.CalibracionEnergia, fDatos);
		if (EstadoTrabajo.CalibracionEnergia) 
			{
			fileWrite(&Datos.enp1, sizeof(Datos.enp1), 1, fDatos);
			fileWrite(&Datos.enp2, sizeof(Datos.enp2), 1, fDatos);
			fileWrite(&Datos.enp3, sizeof(Datos.enp3), 1, fDatos);
			for (int i = 1; i <= 3; i++)
				for (int j = 1; j <= 3; j++)
					fileWrite(&Datos.covarEnergia[i*4+j], sizeof(Datos.covarEnergia[0]), 1, fDatos);
			fileWrite(&Datos.chiEnergia, sizeof(Datos.chiEnergia), 1, fDatos);
			}
		// Escribe la calibracion de forma
		fileWriteBool(EstadoTrabajo.CalibracionForma, fDatos);
		if (EstadoTrabajo.CalibracionForma) 
			{
			// Sigma
			fileWrite(&Datos.afwhg, sizeof(Datos.afwhg), 1, fDatos);
			fileWrite(&Datos.bfwhg, sizeof(Datos.bfwhg), 1, fDatos);
			for (int i = 1; i <= 2; i++)
				for (int j = 1; j <= 2; j++)
					fileWrite(&Datos.covarSigma[i*3+j], sizeof(Datos.covarSigma[0]), 1, fDatos);
			fileWrite(&Datos.chiSigma, sizeof(Datos.chiSigma), 1, fDatos);
			// Tau
			fileWrite(&Datos.atau, sizeof(Datos.atau), 1, fDatos);
			fileWrite(&Datos.btau, sizeof(Datos.btau), 1, fDatos);
			for (int i = 1; i <= 2; i++)
				for (int j = 1; j <= 2; j++)
					fileWrite(&Datos.covarTau[i*3+j], sizeof(Datos.covarTau[0]), 1, fDatos);
			fileWrite(&Datos.chiTau, sizeof(Datos.chiTau), 1, fDatos);
			// ---Resolución (a quitar?)
			fileWrite(&Datos.ares, sizeof(Datos.ares), 1, fDatos);
			fileWrite(&Datos.bres, sizeof(Datos.bres), 1, fDatos);
			for (int i = 1; i <= 2; i++)
				for (int j = 1; j <= 2; j++)
					fileWrite(&Datos.covarRes[i*3+j], sizeof(Datos.covarRes[0]), 1, fDatos);
			}
		}

	// Escribe el analisis
	printf("Guardamos el análisis\n");
	tListaAnalisis* listaAnalisis = DevuelveListaAnalisis();
	if (listaAnalisis->numElementos > 0) 
		{
		// Indica bloque de datos de analisis
		Indicador = BLOQUE_ANALISIS;
		fileWrite(&Indicador, sizeof(Indicador), 1, fDatos);
		printf("*************************** ESCRIBIMOS BLOQUE ANALISIS ************************\n");

		fileWriteBool(EstadoTrabajo.Analisis, fDatos);

		printf("Guardamos el resto de datos del estado de análisis\n");
		fileWriteBool(EstadoTrabajo.AnalisisEficiencia, fDatos);
		fileWriteBool(EstadoTrabajo.AnalisisActividad, fDatos);
		fileWriteBool(EstadoTrabajo.AnalisisLD, fDatos);
		printf("ANALISIS EFICIENCIA=%d, ANALISIS ACTIVIDAD=%d, ANALISIS LD=%d\n", EstadoTrabajo.AnalisisEficiencia, EstadoTrabajo.AnalisisActividad, EstadoTrabajo.AnalisisLD);
		
		//escribimos los datos de actividad para las restricciones
		fileWriteBool(equilibrioDeterminado, fDatos);
		if(equilibrioDeterminado)
			{
			for(int i=0;i<numCadenas;i++)
				{
				int minEl=cadenas[i*2];
				int maxEl=cadenas[i*2+1];
				int max=int((maxEl-minEl)/3);
				for(int j=0;j<max;j++)	
					{
					fileWrite(&actividades[i][j], sizeof(actividades[i][j]), 1, fDatos);
					fileWrite(&sigmaActividades[i][j], sizeof(actividades[i][j]), 1, fDatos);
					}	
				}	
			}
		//
			
		//escribimos los datos del fondo cargado
		fileWriteBool(EstadoTrabajo.fondoCargado, fDatos);
		if(EstadoTrabajo.fondoCargado)
			{
			for(int i=0;i<Datos.numCanales;i++)
				{
				fileWrite(&Datos.cuentasFondo[i], sizeof(Datos.cuentasFondo[i]),1,fDatos);
				fileWrite(&Datos.sigmaCuentasFondo[i], sizeof(Datos.sigmaCuentasFondo[i]),1,fDatos);
				}
			
			printf("Datos.numFondos vale %d\n", Datos.numFondos);//No me está guardando estoooo!!!
			fileWrite(&Datos.numFondos, sizeof(Datos.numFondos),1,fDatos);
			for(int i=0;i<Datos.numFondos;i++)
				{
				fileWrite(&Datos.mediasFondo[i], sizeof(Datos.mediasFondo[i]),1,fDatos);
				fileWrite(&Datos.incertidumbresFondo[i], sizeof(Datos.incertidumbresFondo[i]),1,fDatos);
				fileWrite(&Datos.energiasFondo[i], sizeof(Datos.energiasFondo[i]),1,fDatos);
				}
			}
		//
			
		// escribe la lista intervalos ajustados y los ajustes de los picos
		fileWrite(&listaAnalisis->numElementos, sizeof(listaAnalisis->numElementos), 1, fDatos);
		printf("------------------------- NUMERO DE ELEMENTOS ES %d\n", listaAnalisis->numElementos);
		for (int i = 1; i <= listaAnalisis->numElementos; i++) 
			{
			printf("Escribimos nodo %d\n", i);
			NodoAnalisis *nodo = DevuelveNodo(*listaAnalisis, i);
			fileWrite(&(nodo->numIntervalo), sizeof(nodo->numIntervalo), 1, fDatos);
			// escribe los datos de ajuste del intervalo
			NodoPicos *intervalo = DevuelveNodo(Datos.listaPicos, nodo->numIntervalo);
			for (int j = 1; j <= intervalo->picos.numElementos; j++) 
				{
				printf("escribimos pico %d\n", j);
				// escribe el ajuste del pico
				NodoPico *pico = DevuelveNodo(intervalo->picos, j);
				escribe_Ajuste(pico->Ajuste, fDatos);
				}
			}
		}

	//NOTA: Habrá que ir viendo qué estados son válidos para guardar la actividad
	if(EstadoTrabajo.AnalisisActividad || EstadoTrabajo.AnalisisEficiencia || EstadoTrabajo.AnalisisLD)
		{
		// Indica bloque de datos de actividad
		Indicador = BLOQUE_ACTIVIDAD;
		fileWrite(&Indicador, sizeof(Indicador), 1, fDatos);
		printf("**************** ESCRIBIMOS BLOQUE ACTIVIDAD (%d) ***************\n", Indicador);
			
		//Escribimos las matrices de varianza de la eficiencia, los parámetros y los nodos de eficiencia
		fileWrite(&Datos.dimensionMatEfi, sizeof(Datos.dimensionMatEfi),1, fDatos);
		printf("Escrita dim de la matriz de eficiencias: %d de tamaño %d\n", Datos.dimensionMatEfi, sizeof(Datos.dimensionMatEfi));
		for(int i=0;i<Datos.dimensionMatEfi;i++)
			for(int j=0;j<Datos.dimensionMatEfi;j++)
				{
				printf("Escrito el dato [%d][%d] de tam %d con valor %f\n", i,j,sizeof(Datos.matrizEficiencia[i][j]),Datos.matrizEficiencia[i][j]);
				fileWrite(&Datos.matrizEficiencia[i][j], sizeof(Datos.matrizEficiencia[i][j]),1, fDatos);
				}
		if(Datos.parametrosEficiencia!=NULL)
			{
			for(int i=0;i<Datos.dimensionMatEfi;i++)	
				fileWrite(&Datos.parametrosEficiencia[i], sizeof(Datos.parametrosEficiencia[i]), 1, fDatos);
			for(int i=0;i<Datos.dimensionMatEfi+1;i++)	
				fileWrite(&Datos.nodosEficiencia[i], sizeof(Datos.nodosEficiencia[i]), 1, fDatos);
			}
		
		//Escribimos las matrices de actividad
		int num=Datos.matricesActividad->numElements();		
		fileWrite(&num, sizeof(num),1, fDatos);
		printf("Escribimos %d filas de la matriz de actividad\n", num);
		for(int i=0;i<num;i++)
			{
			LinkedList *l=(LinkedList *)Datos.matricesActividad->getNode(i);
			int num2=l->numElements();
			fileWrite(&num2, sizeof(num2),1, fDatos);
			printf("La fila %d tiene %d columnas\n", i, num2);
			for(int j=0;j<num2;j++)
				{
				NodoPico *n=(NodoPico *)l->getNode(j);
				fileWrite(&n, sizeof(NodoPico *),1, fDatos);
				}
			}			
		}

	printf("Guardamos el estado de visualización\n");
	// Escribe el estado de la visualizacion
	Indicador = BLOQUE_VISUALIZACION;
	fileWrite(&Indicador, sizeof(Indicador), 1, fDatos);
	escribe_EstadoVisual(EstadoVisual, fDatos);
	

	printf("Cerramos con el bloque de cierre\n");
	// Cierra el fichero
	Indicador = BLOQUE_CIERRE;
	fileWrite(&Indicador, sizeof(Indicador), 1, fDatos);
	
	fclose(fDatos);

	//Si es un fichero de fondo y se ha hecho la calibración, debemos
	//preguntar si se quieren guardar sus valores en la base de datos del fondo
	if(Datos.esFondo && EstadoTrabajo.CalibracionEnergia)
		{
		int respuesta=Elegir(app_shell, "Do you want to save this background\nin background data base?", "Saving Background", NULL, NULL, NULL);
		if(respuesta==1)	
			{
			printf("Tenemos que guardar en la base de datos del fondo\n");
			//Debemos sacar un cuadro de diálogo para que elija en qué base de datos
			//lo guarda.
			// Adapta el menu de carga de ficheros
			XmString patron = XmStringCreateLocalized ((char *)SAVE_BDBACKGROUND_TITLE);
			XtVaSetValues (dlg_files, XmNdialogTitle, patron, NULL);
			//XmStringFree (patron);
			patron = XmStringCreateLocalized ((char *)FILTER_FICH_BDBACKGROUND);
			XtVaSetValues (dlg_files, XmNpattern, patron, NULL);
			//XmStringFree (patron);
			XtRemoveAllCallbacks (dlg_files, XmNokCallback);
			XtAddCallback (dlg_files, XmNokCallback, GuardarBDFondo, NULL);
			//De momento, para las pruebas, lo hacemos con ActualizarBDFondo
		//		XtAddCallback (dlg_files, XmNokCallback, ActualizarBDFondo, NULL);
		
			//Ponemos el fichero de preferencias
			//UltAnalDirOpen = Preferences.GetBackBDDir();
			UltAnalDirOpen = getcwd(NULL,0);

			if (UltAnalDirOpen != NULL) 
				{
				patron = XmStringCreateLocalized(UltAnalDirOpen);
				XtVaSetValues (dlg_files, XmNdirectory, patron, NULL);
				//XmStringFree (patron);
				}
			printf("EL dialog files está a %d\n", XtIsManaged(dlg_files));
			// Pone el menu de carga de ficheros en pantalla
			// Si hay puesto otro menu de ficheros lo quita
			if (!XtIsManaged(dlg_files))	
				{
				XtManageChild (dlg_files);
				printf("Imprimimos el cuadro de diálogo\n");
				}
			}
		}
	return 0; //Terminamos sin errores
	}

//********************** GUARDAR BD FONDO ********************
/* Modifica la BD del fondo añadiendo las cuentas del nuevo fondo que se va a 
	guardar.
	Se ejecuta al seleccionar un fichero *.fbd tras guardar un espectro de 
	fondo (si se eligió la opción de guardarlo también en una fbd)
*/
void GuardarBDFondo(Widget widget, XtPointer client_data, XtPointer call_data) 
	{
	// -- Recogemos el nombre del fichero .fbd donde vamos a guardar
	nombre=NULL;
	char *dir = NULL;
	XmFileSelectionBoxCallbackStruct *cbs = (XmFileSelectionBoxCallbackStruct *) call_data;

	XtUnmanageChild(dlg_files);
    XmStringGetLtoR(cbs->value, XmFONTLIST_DEFAULT_TAG, &nombre);
    XmStringGetLtoR(cbs->dir, XmFONTLIST_DEFAULT_TAG, &dir);

	// recupera la extension
	char *ext = DaExtensionFile(nombre, dir);
	char *extOficial = (char *)EXT_FICH_BDBACKGROUND;
	switch ((int) client_data) 
		{
		case 1: extOficial = (char *)EXT_FICH_BDBACKGROUND; break;
		}
	
	// comprueba si hay que agnadir la extension
	if ((ext == NULL)||(strcmp(ext, extOficial) != 0)) 
		{
		// hay que agnadir la extension del fichero
		char *otro = new char[strlen(nombre) + strlen(extOficial) + 1];
		if (otro == NULL) 
			{
			XtFree(nombre); XtFree(dir); delete [] ext;
			printf(ERROR_SAVE_FILE);
			printf(ERROR_NO_MEMORY);
			return;
			}
		strcpy(otro, nombre);
		strcat(otro, extOficial);
		XtFree(nombre);
		nombre = otro;
		}
	delete [] ext;
		
	// guarda el ultimo directorio visitado
	delete [] UltAnalDirOpen;
	UltAnalDirOpen = CopyString(dir);

    if (strlen(nombre) == strlen(dir)) 
	    { // no hay nombre de fichero
		XtFree(nombre); XtFree(dir);
		Aviso(main_w, "You must to select a file");
		return;
		}

	char buffer[1024];
	double ef;
	int ind;
	double area, incertidumbre;
	int indice=0;
	printf("PULSA ALMACENAR\n");
	
	// --Abrimos el fichero y tomamos su número de columnas	
	ifstream ifile(nombre);
	if (!ifile.is_open()) 	
		{
		cout << "Error opening files";
		int respuesta=Elegir(app_shell, "DataBase does not exist. Create?\n", "Save to DataBase", NULL, NULL, NULL);
		if(respuesta==SI)//Lo creamos vacío y volvemos a abrirlo
			{
			ofstream ofile(nombre, ios::trunc);
			ofile << "Energy(keV)\tReaction\t\t\t\t\t" << endl;
			ofile << "EOF" << endl;
			ofile.close();
			ifstream ifile(nombre);
			}
		else
			{
			XtFree(nombre); XtFree(dir);
			return;
			}
		}	

	int numFilas=numeroFilas(nombre)-1;//La primera son sólo títulos
	int numColumnas=numeroColumnas(nombre);//NOTA: Luego habrá que restar una para no contar la media
	printf("El número de columnas del fichero es %d\n", numColumnas);
	printf("El número de filas del fichero es %d\n", numFilas);

	//NOTA: CREAMOS LAS FILAS NECESARIAS
	// --Comprobamos si está abierto y creamos cadenas auxiliares.
	string finfich("EOF");
	string vacia("");
	printf("El número de elementos que tenemos es %d\n", Datos.listaPicos.numElementos);
		

	/* Para cada pico identificado en el espectro de fondo, miramos si su energía
	de emisión coincide con alguna de las que aparecen en la fbd seleccionada.
	Si no coincide, ese pico se almacena en una lista de "nuevas entradas"
	*/

	double minEnergia, maxEnergia;

	//for(int i=1;i<=67;i++)
	printf("Datos.listaPicos.numElementos %d\n", Datos.listaPicos.numElementos);
	for(int i=1;i<=Datos.listaPicos.numElementos;i++){
		NodoPicos *nodoPicos=DevuelveNodo(Datos.listaPicos, i);
		for(int j=1;j<=nodoPicos->picos.numElementos;j++)
			{
			NodoPico  *nodo   = DevuelveNodo(nodoPicos->picos, j);
			minEnergia = Datos.DaEnergia((double)nodoPicos->limInferior);//Por intervalos completos
			maxEnergia = Datos.DaEnergia((double)nodoPicos->limSuperior)+1;
			area			  = Datos.DaAreaPico(i,j);
			incertidumbre	  = Datos.DaIncerAreaPico(i,j);
			printf("Nodo (%d,%d): [%d,%d], identificado=%d, ajustado=%d, calibrado=%d\n", i, j, (int)minEnergia, (int)maxEnergia, nodo->Ajuste.identificado, nodo->Ajuste.ajustado, nodo->Ajuste.calibrado);
						
			//Tenemos la información del pico. Recorremos el fichero para ver si está su energía
			//NOTA: Aquí habrá que añadir un & !nodo->Ajuste.ignorado) que se pondrá a true si
			//se pulsa Del en Análisis
//			if(nodo->Ajuste.identificado || nodo->Ajuste.ajustado || nodo->Ajuste.calibrado)
			if(nodo->Ajuste.ajustado && !nodo->Ajuste.invisible)
				{
				printf("Recorremos el fichero hasta llegar a la energía correspondiente\n");
				if(numFilas<=1)//En este caso añadimos sin más, no va a haber nada en fich
					{
					printf("fichero vacío\n");
					printf("Vamos a grabar un nodo identificado\n");
					int emisor=0;
					string cadena("");
					double energia=0.0;
					char nom[100];
					sprintf(nom, "");
					printf("Tomamos energía\n");
					energia=energiaMedia(nodo->Ajuste.emisiones);
					printf("Energía es %f, tenemos %d emisiones\n", energia, nodo->Ajuste.emisiones->numElements());
					for(int j=0;j<nodo->Ajuste.emisiones->numElements();j++)
						{
						NodoEmision *ne=(NodoEmision *)nodo->Ajuste.emisiones->getNode(j);
						emisor=LaraBD.GetEmissor(ne->codElemento);
						char nombr[3];
						LaraBD.GetNameElement(emisor, nombr);
						int mn=LaraBD.GetMassNumber(emisor);
						char texto[10]="";									
						if(j!=0)		sprintf(texto, ", %d%s", mn, nombr);
						else 			sprintf(texto, "%d%s", mn, nombr);
						strcat(nom, texto);
						printf("Emisión %d: %s,%d\n", j, nombr, mn);
						}
					if(nodo->Ajuste.emisiones->numElements()==0)
						{
						sprintf(nom, "?");
						energia=Datos.DaEnergia(nodo->Ajuste.Centroide);
						}
					printf("Reservando memoria %d\n", indice);
					entradas[indice]=new char[150];
					printf("Memoria reservada, asignando valor\n");
					sprintf(entradas[indice], "%.3f %s", energia, nom);
					printf("Entradas %d va a valer |%s|\n", indice, entradas[indice]);
					indice++;	
					}//fin(si fichero vacío)
				ifile.getline(buffer,1024); //La primera fila contiene títulos
				/*for(int k=1;k<=numFilas;k++)//Ahora recorremos el fichero
			  		{
					//Recorremos el fichero hasta llegar a la línea correspondiente
					
					ifile.getline (buffer,1024);
					string cadena(buffer);
			
					if(cadena!=finfich && cadena!=vacia)
						{
						if((ind=cadena.find("\t"))!=-1)	
							{
							string cadenaEnergia(cadena.substr(0,ind));
							ef=atof((char *)cadenaEnergia.c_str());
							}
						}
					printf("ef vale: %f<%f<%f, y k=%d, nF=%d\n", minEnergia, ef, maxEnergia, k, numFilas);
					//Si la energía está en alguno de los límites
					if(ef>=minEnergia && ef<=maxEnergia)
						{
						//No hay fila que añadir, en el próximo bucle añadiremos la columna
						printf("Este nodo está en la BD: %f<=%f<=%f\n", minEnergia, ef, maxEnergia);
						break;
						}
					else if((ef>maxEnergia || k>=numFilas-1 || numFilas<=1))
						{
						printf("Esta energía ya no va a aparecer, añadimos\n");
						//Esta energía no ha aparecido y ya no aparecerá porque 
						//ya estamos por energías más altas en un fichero de 
						//orden creciente en energías
						//Tenemos que añadir una fila con esta energía.
						//La guardamos en un array de filas nuevas a añadir
						double energia=0.0;
						char nom[100];
						sprintf(nom, "");
						for(int i=0;i<10;i++)	nom[i]='\0';
						if (nom == NULL) 
							{
							XtFree(nombre); XtFree(dir);
							printf(ERROR_NO_MEMORY);
							return;
							}
						if(nodo->Ajuste.identificado || nodo->Ajuste.calibrado)
							{
							printf("Vamos a grabar un nodo identificado\n");
							int emisor=0;
							string cadena("");
							energia=energiaMedia(nodo->Ajuste.emisiones);
							for(int j=0;j<nodo->Ajuste.emisiones->numElements();j++)
								{
								NodoEmision *ne=(NodoEmision *)nodo->Ajuste.emisiones->getNode(j);
								emisor=LaraBD.GetEmissor(ne->codElemento);
								char nombre[3];
								LaraBD.GetNameElement(emisor, nombre);
								int mn=LaraBD.GetMassNumber(emisor);
								char texto[10];									
								if(j!=0)		sprintf(texto, ", %d%s", mn, nombre);
								else 			sprintf(texto, "%d%s", mn, nombre);
								strcat(nom, texto);
								}
							printf("Hemos cogido los valores del nodo, el emisor es %d\n", emisor);
							}
						else if(nodo->Ajuste.ajustado || nodo->Ajuste.calibrado)//antes ajustado, deja algunos valores fuera
								{
								energia=Datos.DaEnergia(nodo->Ajuste.Centroide);
								printf("Energía del Centroide es: %f\n", energia);
								sprintf(nom, "?");
								}
						
						entradas[indice]=new char[150];
						sprintf(entradas[indice], "%.3f %s", energia, nom);
						printf("Entradas %d va a valer %s\n", indice, entradas[indice]);
						indice++;	
						ifile.seekg(ios::beg);//Para buscar los otros
						break;
						}
					printf("Pasamos al siguiente\n");
					}//while(!EOF)*/
				}//if identificado
			}	//for(j)
		}//for(i)

	//NOTA: Al contrario que en actualizar fondo, todas las entradas que no estén
	//		se meten sin más, se consideran verificadas/modificadas/ignoradas
	//		durante el análisis
	addNuevasEntradas(entradas, indice);
	numFilas=numeroFilas(nombre)-1;//El número de filas ha cambiado
	addMedia(nombre, numFilas, numColumnas);//Ya hay q tener en cuenta la columna nueva, agregada
	ifile.close();
	XtFree(nombre); XtFree(dir);
	return;
	}
	
//********************** ACTUALIZAR BD FONDO *****************
/* Modifica la BD del fondo añadiendo las cuentas del nuevo fondo que se va a 
	guardar.
	Se ejecuta al seleccionar un fichero *.fbd tras guardar un espectro de 
	fondo (si se eligió la opción de guardarlo también en una fbd)
*/
void ActualizarBDFondo(Widget widget, XtPointer client_data, XtPointer call_data) 
	{
	//Vemos cuál es el fichero seleccionado
	nombre=NULL;
//	char *titulo = NULL;
	char *dir = NULL;
//	char cabecera[sizeof(CABECERA_ASF)] = "";
	XmFileSelectionBoxCallbackStruct *cbs = (XmFileSelectionBoxCallbackStruct *) call_data;

	XtUnmanageChild(dlg_files);
    XmStringGetLtoR(cbs->value, XmFONTLIST_DEFAULT_TAG, &nombre);
    XmStringGetLtoR(cbs->dir, XmFONTLIST_DEFAULT_TAG, &dir);
	
	// guarda el ultimo directorio visitado
	delete [] UltAnalDirOpen;
	UltAnalDirOpen = CopyString(dir);


    if (strlen(nombre) == strlen(dir)) 
	    { // no hay nombre de fichero
		XtFree(nombre); XtFree(dir);
		Aviso(main_w, "You must to select a file");
		return;
		}

	char buffer[1024];
//	double e;
	double ef;
	int ind;
	double area, incertidumbre;
//	bool estaEnergia=false;
	int indice=0;
	printf("PULSA ALMACENAR\n");
	
	ifstream ifile(nombre);
	ofstream ofile("temp.fbd", ios::trunc);
	int numColumnas=numeroColumnas(nombre);
	printf("El número de columnas del fichero es %d\n", numColumnas);

	//NOTA: CREAMOS LAS FILAS NECESARIAS
	if (!ifile.is_open() || !ofile.is_open() ) 	
		{ 
		cout << "Error opening files"; 
		XtFree(nombre); XtFree(dir);
		exit(1); 
		}
	string finfich("EOF");
	string vacia("");
	printf("El número de elementos que tenemos es %d\n", Datos.listaPicos.numElementos);
	/* Para cada pico identificado en el espectro de fondo, miramos si su energía
	de emisión coincide con alguna de las que aparecen en la fbd seleccionada.
	Si no coincide, ese pico se almacena en una lista de "nuevas entradas"
	*/
	for(int i=1;i<=Datos.listaPicos.numElementos;i++)
		{
		NodoPicos *nodoPicos=DevuelveNodo(Datos.listaPicos, i);
		printf("intervalo %d\n", i);
//		for(int j=0;j<nodoPicos->picos.numElementos;j++)
		//NOTA: Importante: aunque programado para funcionar con grupos de picos,
		//el programa sólo calcula centroides para el primer pico de cada intervalo,
		//así que no tenemos en cuenta de momento más que el primero
		for(int j=0;j<1;j++)
			{
			NodoPico  *nodo   = DevuelveNodo(nodoPicos->picos, j);
			double minEnergia = Datos.DaEnergia((double)nodoPicos->limInferior);
			double maxEnergia = Datos.DaEnergia((double)nodoPicos->limSuperior)+1;
			area			  = Datos.DaAreaPico(i,j);
			incertidumbre	  = Datos.DaIncerAreaPico(i,j);
			printf("Nodo (%d,%d): [%d][%d], identificado=%d, ajustado=%d\n", i, j, (int)minEnergia, (int)maxEnergia, nodo->Ajuste.identificado, nodo->Ajuste.ajustado);
						
			//Tenemos la información del pico. Recorremos el fichero para ver si está su energía
			if(nodo->Ajuste.identificado || nodo->Ajuste.ajustado)
				{
				while (! ifile.eof() )
			  		{
					//Recorremos el fichero hasta llegar a la línea correspondiente
					ifile.getline (buffer,1024);
					string cadena(buffer);
			
					if(cadena!=finfich && cadena!=vacia)
						{
						if((ind=cadena.find("\t"))!=-1)	
							{
							string cadenaEnergia(cadena.substr(0,ind));
							ef=atof((char *)cadenaEnergia.c_str());
							}
						}
					
					//Si la energía está en alguno de los límites
					if(ef>=minEnergia && ef<=maxEnergia)
						{
						//No hay fila que añadir, en el próximo bucle añadiremos la columna
						printf("Este nodo está en la BD: %f<=%f<=%f\n", minEnergia, ef, maxEnergia);
						break;
						}
					else if(ef>maxEnergia)
						{
						//Esta energía no ha aparecido y ya no aparecerá porque 
						//ya estamos por energías más altas en un fichero de 
					 	//	orden creciente en energías
					 	//Tenemos que añadir una fila con esta energía.
						//La guardamos en un array de filas nuevas a añadir
						double energia=0.0;
						char *nom=new char[10];
						for(int i=0;i<10;i++)	nom[i]='\0';
						if (nom == NULL) 
							{
							XtFree(nombre); XtFree(dir);
							printf(ERROR_NO_MEMORY);
							return;
							}
	
						printf("Esta energía no la tenemos (%f), la añadimos: identificado=%d\n", nodo->Ajuste.identificado);
						if(nodo->Ajuste.identificado)
							{
							int emisor=0;
							for(int j=0;j<nodo->Ajuste.emisiones->numElements();j++)
								{
								NodoEmision *ne=(NodoEmision *)nodo->Ajuste.emisiones->getNode(j);
								energia=ne->Energia;
								emisor=ne->codElemento;
								}
							char nombr[3];
							LaraBD.GetNameElement(emisor, nombr);
							int mn=LaraBD.GetMassNumber(emisor);
							sprintf(nom, "%d%s", mn, nombr);	
							printf("Energía es: %f\n", energia);
							}
						else if(nodo->Ajuste.ajustado)//antes ajustado, deja algunos valores fuera
							{
							energia=Datos.DaEnergia(nodo->Ajuste.Centroide);
							//energia=nodo->Ajuste.Energia;
							printf("Energía del Centroide es: %f\n", energia);
							sprintf(nom, "?");
							}
						//printf("Añadimos la entrada: Energía=%f\tnombre=|%s|\n", energia, nom);
						//Una vez calculada la información que debe salir, sacamos el
						//cuadro  de diálogo
						entradas[indice]=new char[50];
						sprintf(entradas[indice], "%.3f %s", energia, nom);
						printf("Entradas %d va a valer %s\n", indice, entradas[indice]);
						indice++;	
						ifile.seekg(ios::beg);//Para buscar los otros 							
						break;
						}
					}//while(!EOF)
				}//if(identificado)
			}	//for(j)
		}//for(i)
	
	if(indice>0)	creaDlgInsercionBD(entradas, indice);
	else			addColumns(nombre);					
	ifile.close();
	ofile.close();
	XtFree(nombre);	XtFree(dir);
	return;
	}


//******************** CREA DLG INSERCION BD ************************
/* Crea un cuadro de diálogo antes de insertar una nueva entrada en la 
	base de datos de fondo. En este cuadro de diálogo aparecerán todas
	las entradas nuevas que no se corresponden con ninguna de las que 
	aparecen en la base de datos del fondo. 
	Estas entradas se le pasa como parámetro en entries, siendo numEntries
	el número de entradas.
	NOTA: La variable global entradas contiene la misma información que entries!
	*/
void creaDlgInsercionBD(char **entries, int numEntries)
	{
	Arg			args[7];
   	Cardinal	i = 0;
	XmString 	cadena;

	printf("CREA DIALOGO INSERCION BD\n");
	for(int i=0;i<20;i++)
		printf("entrada %d = %s\n", i, entradas[i]);
	for(int i=0;i<numEntries;i++)
		printf("entries %d = %s\n", i, entries[i]);

	i = 0;
	XtSetArg (args[i], XmNtitle, "Add to Background DB"); i++;
	XtSetArg (args[i], XmNmwmDecorations, MWM_DECOR_BORDER|MWM_DECOR_TITLE|MWM_DECOR_MENU); i++;
	XtSetArg (args[i], XmNmwmFunctions, MWM_FUNC_MOVE|MWM_FUNC_CLOSE); i++;
	XtSetArg (args[i], XmNbaseWidth, 200); i++;
	XtSetArg (args[i], XmNbaseHeight, 250); i++;
	XtSetArg (args[i], XmNmaxWidth, 200); i++;
	XtSetArg (args[i], XmNmaxHeight, 250); i++;
	printf("Creados argumentos\n");
	dlgBD = XmCreateFormDialog(app_shell, "dlgBD", args, i);

	XtAddCallback (XtParent(dlgBD), XmNpopdownCallback, QuitaVentanaBD, NULL);
	
	i=0;
	Widget col1=XmCreateRowColumn(dlgBD, "ColumnaBD", args, i);
	

	cadena = XmStringCreateLocalized ("List of new entries:");
    i = 0;
   	XtSetArg (args[i], XmNlabelString, cadena); i++;
    Widget l1f1 = XmCreateLabelGadget (col1, "l1f1", args, i);
    XmStringFree (cadena);
	XtManageChild(l1f1);
	i = 0;
	XtSetArg (args[i], XmNlistSizePolicy, XmCONSTANT); i++;
	XtSetArg (args[i], XmNselectionPolicy, XmSINGLE_SELECT); i++;
	XtSetArg (args[i], XmNvisibleItemCount, 2); i++;
	XtSetArg (args[i], XmNscrollBarDisplayPolicy, XmAS_NEEDED); i++;
    listaEntries = XmCreateScrolledList(col1, "lista de entradas", args, i);
	XtAddCallback (listaEntries, XmNextendedSelectionCallback, modificaEntrada, NULL);
	
	//Añadimos las entradas nuevas(les quitamos el \n):
	printf("Añadiendo entradas a la lista\n");
	for(i=0;i<(unsigned int)numEntries;i++)
		{
		printf("Vamos a añadir la entrada %d, de valor %s\n", i, entries[i]);
/*		string cad(entries[i]);
		int ind=0;
//		if((ind=cad.find("\n"))!=string::npos)	cad=cad.substr(0,ind-1);
		if((ind=cad.find("\n"))!=-1)	cad=cad.substr(0,ind-1);
		entries[i]=(char *)cad.c_str();
		cadena = XmStringCreateLocalized(entries[i]);
		printf("Esto nos da la cadena %s\n", entries[i]);
		XmListAddItemUnselected(listaEntries, cadena, 0);*/
		cadena = XmStringCreateLocalized(entradas[i]);
		printf("Esto nos da la cadena %s\n", entradas[i]);
		XmListAddItemUnselected(listaEntries, cadena, 0);
		
		}

	XtManageChild(listaEntries);
	
	i = 0;
	cadena = XmStringCreateLocalized (" Add ");
    XtSetArg (args[i], XmNlabelString, cadena); i++;
   	//
//	XtSetArg (args[i], XmNchildVerticalAlignment, XmALIGNMENT_CENTER); i++;
	XtSetArg (args[i], XmNwidth, 100); i++;
	XtSetArg (args[i], XmNheight, 20); i++;
	//
	Widget botonAdd = XmCreatePushButton(col1, "Botón para añadir entrada", args, i);
	XtAddCallback (botonAdd, XmNactivateCallback, pulsaAddEntries, NULL);
    XmStringFree (cadena);
	XtManageChild(botonAdd);
	
	i = 0;
	cadena = XmStringCreateLocalized (" Edit ");
    XtSetArg (args[i], XmNlabelString, cadena); i++;
   	//
//	XtSetArg (args[i], XmNchildVerticalAlignment, XmALIGNMENT_CENTER); i++;
	XtSetArg (args[i], XmNwidth, 100); i++;
	XtSetArg (args[i], XmNheight, 20); i++;
	//
	Widget botonModify = XmCreatePushButton(col1, "Botón para modificar entrada", args, i);
	XtAddCallback (botonModify, XmNactivateCallback, modificaEntrada, NULL);
    XmStringFree (cadena);
	XtManageChild(botonModify);

	i=0;
	cadena = XmStringCreateLocalized (" Ignore ");
    XtSetArg (args[i], XmNlabelString, cadena); i++;
   	//
//	XtSetArg (args[i], XmNchildVerticalAlignment, XmALIGNMENT_CENTER); i++;
	XtSetArg (args[i], XmNwidth, 100); i++;
	XtSetArg (args[i], XmNheight, 20); i++;
	//
	Widget botonIgnore = XmCreatePushButton(col1, "Botón para ignorar entrada", args, i);
	XtAddCallback (botonIgnore, XmNactivateCallback, IgnoraBD, NULL);
    XmStringFree (cadena);
	XtManageChild(botonIgnore);
	
	i=0;
	cadena = XmStringCreateLocalized (" Ignore All");
    XtSetArg (args[i], XmNlabelString, cadena); i++;
	XtSetArg (args[i], XmNwidth, 100); i++;
	XtSetArg (args[i], XmNheight, 20); i++;

	Widget botonIgnoreAll = XmCreatePushButton(col1, "Botón para ignorar entrada", args, i);
	XtAddCallback (botonIgnoreAll, XmNactivateCallback, IgnoraTodasBD, NULL);
    XmStringFree (cadena);
	XtManageChild(botonIgnore);
	XtManageChild(botonIgnoreAll);
	XtManageChild(botonModify);
	XtManageChild(botonAdd);

	XtManageChild(col1);
	XtManageChild(dlgBD);
	}

//*********************** PULSA ADD ENTRIES ****************************
/*	Al pulsar AddEntries en la interfaz que lista las nuevas entradas, añadimos
	todas las entradas que aún estén en la lista (con las modificaciones que se
	les haya hecho) a la base de datos del fondo que se haya seleccionado.
	De esta forma, añadiremos una fila en su orden de energía correspondiente
	para cada reacción de dicha lista.
	*/
void pulsaAddEntries(Widget widget, XtPointer client_data, XtPointer call_data) 
	{
	printf("PULSA ADDENTRIES\n");
	Arg			args[7];
   	Cardinal	i = 0;
	
	char buffer[1024];
	double ef;
	int ind;
	XmString *entradasCad;
	int numEntradas=0;
		
	//Abrimos los ficheros
	ifstream ifile(nombre);
	ofstream ofile("temp.fbd", ios::trunc);
	if (!ifile.is_open() || !ofile.is_open() ) 	{ cout << "Error opening files"; exit(1); }
	string finfich("EOF");
	string vacia("");
	int numColumnas=numeroColumnas(nombre);
	printf("El número de columnas del fichero es %d\n", numColumnas);

	//NOTA: CREAMOS LAS FILAS NECESARIAS
	//Cogemos las nuevas entradas de la lista de entradas:
	i=0;
	XtSetArg(args[i], XmNitems, &entradasCad);i++; 
	XtSetArg(args[i], XmNitemCount, &numEntradas);i++; 
	XtGetValues(listaEntries, args, i);
	printf("El número de entradas es %d\n", numEntradas);
	
	//Entradas de la lista
	char *entradas[numEntradas];
	//Energías correspondientes a cada una de las entradas
	double energias[numEntradas];

	//Cogemos las energías de la lista de entradas
	for(int i=0;i<numEntradas;i++)
		{
		XmStringGetLtoR(entradasCad[i], XmSTRING_DEFAULT_CHARSET, &entradas[i]);
		printf("%s\n", entradas[i]);
		string aux(entradas[i]);
		ind=aux.find(" ");
		string cadenaEnergia=aux.substr(0,ind);
		energias[i]=atof((char *)cadenaEnergia.c_str());
		}
	
	int ultimo=-1;//Último valor de la lista de entradas a añadir que tenemos que falta por añadir
		
	while(!ifile.eof())
		{
		//Recorremos el fichero hasta llegar a la línea correspondiente
		ifile.getline (buffer,1024);
		string cadena(buffer);
			
		if(cadena!=finfich && cadena!=vacia)
			{
			if((ind=cadena.find("\t"))!=-1)	
				{
				string cadenaEnergia(cadena.substr(0,ind));
				ef=atof((char *)cadenaEnergia.c_str());
				}
			}
		else if(cadena==finfich)	ofile << "EOF" << endl;

		for(int i=ultimo+1;i<numEntradas;i++)
			{
			if(ef>=energias[i])
				{//Tenemos que meter aquí una fila
				ultimo=i;//Para no repetir filas que ya se han añadido
				//Construimos la nueva fila:
				string nuevaEntrada(entradas[i]);	
				//Cogemos el trozo final, con la reaccion y el inicial, con la energía
				ind=nuevaEntrada.find(" ");
				string ener    =nuevaEntrada.substr(0, ind);
				string reaccion=nuevaEntrada.substr(ind, nuevaEntrada.length());
				cout << "energia: " << ener << "reaccion: " << reaccion << endl;
				//Cambiamos el espacio por los tabuladores necesarios
				if(ener.length()>7.0)	nuevaEntrada.replace(ind, 1, "\t");
				else					nuevaEntrada.replace(ind, 1, "\t\t");
				//Añadimos los tabuladores necesarios al final
				for(unsigned int j=0;j<=(28-reaccion.length())/4; j++)	
					nuevaEntrada.insert(nuevaEntrada.length(), "\t");
				//Añadimos ---- para todas las columnas anteriores.
				for(int j=0;j<numColumnas;j++)
					nuevaEntrada.insert(nuevaEntrada.length(), "\t----\t\t");
				//Insertamos la cadena en el fichero:
				ofile << nuevaEntrada << endl;				
				}
			else	
				{
				//No tenemos energias menores que por la que vamos de las antiguas
				//Metemos la antigua
				ofile << cadena << endl;					
				break;
				}
			}
		//Para las que ya estén más allá de las energías nuevas, metemos la fila sin más
		if(ultimo+1>=numEntradas)		ofile << cadena << endl;
		}//while(!eof)

	ifstream ifile2("temp.fbd");
	ofstream ofile2(nombre, ios::trunc);
	
	//Pasamos del fichero temporal al fichero final
	if (!ifile2.is_open() || !ofile2.is_open() ) 	{ cout << "Error opening files"; exit(1); }
	while (! ifile2.eof() )
		{
		ifile2.getline (buffer,1024);
		ofile2 << buffer << "\n";
		}
	ofile2.close();
	ifile2.close();
	remove("temp.fbd");
		
	ifile.close();
	ofile.close();

	//Ahora hacemos las columnas necesarias
	quitarUltimaColumna(nombre);
		
	printf("Fichero sin media:\n");
	imprimirFichero(nombre);
		
	addColumns(nombre);		
		
	printf("Fichero con nueva columna:\n");
	imprimirFichero(nombre);
	XtUnmanageChild(dlgBD);		
	printf("Liberamos entradas\n");
	for(int i=0;i<numEntradas;i++)	XtFree(entradas[i]);
	}

//********************** ADD COLUMNS **********************
/*	En función a la calibración que se haya hecho del fondo, a los picos
	identificados y a las posibles modificaciones del usuario, añadimos a la
	base de datos del fondo las cuentas de los picos apropiados.
	*/
void addColumns(char *nombre)
	{
	//NOTA: CREA LAS COLUMNAS NECESARIAS
  	//Recorremos la lista de picos analizados en el espectro y vamos añadiéndolos
	//a fbege.fbd en una nueva columna
	char buffer[1024];
	double ef;
	int ind;
	double area, incertidumbre;
	bool estaEnergia=false;
	bool ultimaEnergia=false;
	//Este es el tiempo por el que vamos a dividir, considerado en millonésimas de segundo
	double tiempo = Datos.tiempo/1000000.0;
	printf("Tiempo para dividir: %f\n", tiempo);
		
	ifstream ifile(nombre);
	ofstream ofile("temp.fbd", ios::trunc);
		
	if (!ifile.is_open() || !ofile.is_open() ) 	{ cout << "Error opening files"; exit(1); }

	string finfich("EOF");
	string vacia("");

	//Tomamos la primera línea, que tiene los títulos, y le añadimos el
	//nombre de este fichero de fondo	
	ifile.getline (buffer,1024);
	string cadena(buffer);
	sprintf(buffer, "%s\t%s", cadena.c_str(), daFicheroSinExtension(Datos.nombre) );				
	ofile << buffer << endl;
	
	while (! ifile.eof() )
  		{
		//Recorremos el fichero hasta llegar a la línea correspondiente
		ifile.getline (buffer,1024);
		string cadena(buffer);
			
		if(cadena!=finfich && cadena!=vacia)
		{
		if((ind=cadena.find("\t"))!=-1)	
			{
			string cadenaEnergia(cadena.substr(0,ind));
			ef=atof((char *)cadenaEnergia.c_str());
			}
	
		double iarea=0;
		double iincertidumbre=0;
		for(int i=1;i<=Datos.listaPicos.numElementos;i++)
			{
			NodoPicos *nodoPicos=DevuelveNodo(Datos.listaPicos, i);
			for(int j=1;j<=nodoPicos->picos.numElementos;j++)
				{
				NodoPico  *nodo   = DevuelveNodo(nodoPicos->picos, j);
				double minEnergia = Datos.DaEnergia((double)nodo->Ajuste.Centroide-3);
				double maxEnergia = Datos.DaEnergia((double)nodo->Ajuste.Centroide+3);
				area=nodo->Ajuste.area;
				incertidumbre=nodo->Ajuste.incerArea;
				if(nodo->Ajuste.identificado)
				  for(int k=0;k<nodo->Ajuste.emisiones->numElements();k++)
					{
					NodoEmision *ne=(NodoEmision *)nodo->Ajuste.emisiones->getNode(k);
					if( (nodo->Ajuste.ajustado && !nodo->Ajuste.invisible) && ne->Energia>(ef-0.1) && ne->Energia<(ef+0.1) )
						{
						iarea=(area/tiempo);
					//	printf("incertidumbre=%f/%f\n", (incertidumbre*incertidumbre), tiempo);
						iincertidumbre=incertidumbre/tiempo;
					//	printf("area=%f, incertidumbre=%f\n", iarea, iincertidumbre);
						estaEnergia=true;
						if((iarea<=99 && iincertidumbre<=99) || (iarea>99 && iarea <=999 && iincertidumbre<=99) || (iarea<=99 && iincertidumbre>99 && iincertidumbre<=999))
							sprintf(buffer, "%s\t%d(%d)\t", cadena.c_str(), (int)iarea, (int)iincertidumbre);
						else
							sprintf(buffer, "%s\t%d(%d)", cadena.c_str(), (int)iarea, (int)iincertidumbre);
						j=nodoPicos->picos.numElementos;
						i=Datos.listaPicos.numElementos;
						}						
					}
				else 
				  {
					if( (nodo->Ajuste.ajustado && !nodo->Ajuste.invisible) && ef>=minEnergia && ef<=maxEnergia)
					{
				//	printf("Nodo entre %f<=%f<=%f\n", minEnergia, ef, maxEnergia);
					iarea+=(area/tiempo);
				//	printf("incertidumbre=%f/%f\n", (incertidumbre*incertidumbre), tiempo);
					iincertidumbre+=(incertidumbre*incertidumbre)/tiempo;
				//	printf("area=%f, incertidumbre=%f\n", iarea, iincertidumbre);
					estaEnergia=true;
					}
				  if(estaEnergia && (ef>maxEnergia || j==nodoPicos->picos.numElementos-1))	ultimaEnergia=true;
				  if(estaEnergia && ultimaEnergia)
					{
					iincertidumbre=sqrt(iincertidumbre);
					iarea=iarea;
				//	printf("%f->%f(%f)\n", ef, iarea, iincertidumbre);
					if((iarea<=99 && iincertidumbre<=99) || (iarea>99 && iarea <=999 && iincertidumbre<=99) || (iarea<=99 && iincertidumbre>99 && iincertidumbre<=999))
						sprintf(buffer, "%s\t%d(%d)\t", cadena.c_str(), (int)iarea, (int)iincertidumbre);
					else
						sprintf(buffer, "%s\t%d(%d)", cadena.c_str(), (int)iarea, (int)iincertidumbre);
					ultimaEnergia=false;
					j=nodoPicos->picos.numElementos;
					i=Datos.listaPicos.numElementos;
					}
				  }
				}	//for(j)
			}//for(i)
		if(!estaEnergia)	sprintf(buffer, "%s\t----\t", cadena.c_str());				
		else				estaEnergia=false;
		//Una vez hemos recorrido todos los intervalos para esta energía, 
		//decidimos qué es lo que hay que escribir a la salida
		ofile << buffer << endl;
		}//if compare con linea final
		else if(cadena==finfich)	ofile << "EOF" << endl;
		}//while(fich)
	//Ahora sólo tenemos que volcar el contenido del fichero temporal en 
	//la base de datos
	//NOTA: Todo este rodeo de guardar primero en un archivo temporal y luego
		//	volcarlo se debe a la dificultad para añadir directamente en fbege.fbd
		//	sin borrar nada.
	ofile.close();
	ifile.close();
		
	//Pasamos la información del fichero temporal al fichero permanente
	ifstream ifile2("temp.fbd");
	ofstream ofile2(nombre, ios::trunc);
		
	if (!ifile2.is_open() || !ofile2.is_open() ) 	{ cout << "Error opening files"; exit(1); }
	while (! ifile2.eof() )
		{
		ifile2.getline (buffer,1024);
		ofile2 << buffer << "\n";
		}
	ofile2.close();
	ifile2.close();
	remove("temp.fbd");
	}	
	
//********************** ADD MEDIA **********************
/*	En función a la calibración que se haya hecho del fondo, a los picos
	identificados y a las posibles modificaciones del usuario, añadimos a la
	base de datos del fondo las cuentas de los picos apropiados.
	*/
void addMedia(char *nombre, int numEnergias, int numColumnas)
	{
	//NOTA: CREA LAS COLUMNAS NECESARIAS
  	//Recorremos la lista de picos analizados en el espectro y vamos añadiéndolos
	//a fbege.fbd en una nueva columna
	char buffer[1024];
	double ef;
	int ind;
	double area, incertidumbre;
	bool estaEnergia=false;
	bool ultimaEnergia=false;
		
	ifstream ifile(nombre);
	ofstream ofile("temp.fbd", ios::trunc);
		
	if (!ifile.is_open() || !ofile.is_open() ) 	{ cout << "Error opening files"; exit(1); }

	string finfich("EOF");
	string vacia("");

	ifile.getline (buffer,1024);
	string cadena(buffer);
	sprintf(buffer, "%s\taverage", cadena.c_str());
	ofile << buffer << endl;
	
	double *energias, *medias, *incertidumbres;
	medias=new double[numEnergias];
	incertidumbres=new double[numEnergias];
	energias=new double[numEnergias];
	if(medias==NULL || incertidumbres ==NULL || energias==NULL)
		{
		printf("ERROR: No hay memoria para reservar double[%d]\n", numEnergias);
		return;
		}
		
	calculaMediaBDFondo(nombre, numEnergias, numColumnas+1, energias, medias, incertidumbres);	//Tiene una columna más de las iniciales (la nueva)	
	for(int i=0;i<numEnergias;i++)
  		{
		//Recorremos el fichero hasta llegar a la línea correspondiente
		ifile.getline (buffer,1024);
		string cadena(buffer);
			
		if(cadena!=finfich && cadena!=vacia)
			{
			if((ind=cadena.find("\t"))!=-1)	string cadenaEnergia(cadena.substr(0,ind));
	
			sprintf(buffer, "%s\t%.0f(%.0f)\t", cadena.c_str(), medias[i]*(1000000/Datos.tiempo), incertidumbres[i]*(1000000/Datos.tiempo) );
			ofile << buffer << endl;
			}//if compare con linea final
			
		}//while(fich)
	ofile << "EOF" << endl;
	//Ahora sólo tenemos que volcar el contenido del fichero temporal en 
	//la base de datos
	//NOTA: Todo este rodeo de guardar primero en un archivo temporal y luego
	//		volcarlo se debe a la dificultad para añadir directamente en fbege.fbd
	//		sin borrar nada.
	ofile.close();
	ifile.close();
		
	//Pasamos la información del fichero temporal al fichero permanente
	ifstream ifile2("temp.fbd");
	ofstream ofile2(nombre, ios::trunc);
		
	if (!ifile2.is_open() || !ofile2.is_open() ) 	{ cout << "Error opening files"; exit(1); }
	while (! ifile2.eof() )
		{
		ifile2.getline (buffer,1024);
		ofile2 << buffer << endl;
		}
	ofile2.close();
	ifile2.close();
	remove("temp.fbd");
		
	printf("Fichero con la media\n");
	imprimirFichero(nombre);
	}	

//**************** MODIFICA ENTRADA ************************
/*	Se ejecuta cuando se pulsa Edit en la interfaz para añadir entradas
	nuevas a la base de datos del fondo.
	Genera una ventana con un cuadro de texto para introducir el nombre
	de la nueva reacción y un botón para aplicar la modificación.
	*/
void modificaEntrada(Widget widget, XtPointer client_data, XtPointer call_data) 
	{
	Arg			args[7];
   	Cardinal	i = 0;
	XmString 	cadena;
    
	
	printf("CREA DIALOGO INSERCION BD\n");
	i = 0;
	XtSetArg (args[i], XmNtitle, "Modify entry to BDB"); i++;
	XtSetArg (args[i], XmNmwmDecorations, MWM_DECOR_BORDER|MWM_DECOR_TITLE|MWM_DECOR_MENU); i++;
	XtSetArg (args[i], XmNmwmFunctions, MWM_FUNC_MOVE|MWM_FUNC_CLOSE); i++;
	XtSetArg (args[i], XmNbaseWidth, 200); i++;
	XtSetArg (args[i], XmNbaseHeight, 250); i++;
	XtSetArg (args[i], XmNmaxWidth, 200); i++;
	XtSetArg (args[i], XmNmaxHeight, 250); i++;
	dlgModify = XmCreateFormDialog(app_shell, "dlgModify", args, i);

	XtAddCallback (XtParent(dlgBD), XmNpopdownCallback, QuitaVentanaBD, NULL);
	
	i=0;
	Widget col1=XmCreateRowColumn(dlgModify, "ColumnaBD", args, i);
	cadena = XmStringCreateLocalized ("Enter the name of the reaction:");
    
	i = 0;
   	XtSetArg (args[i], XmNlabelString, cadena); i++;
    Widget l1f1 = XmCreateLabelGadget (col1, "etiqueta", args, i);
    XmStringFree (cadena);

	i = 0;
	XtSetArg (args[i], XmNbaseWidth, 100); i++;
	XtSetArg (args[i], XmNbaseHeight, 20); i++;
	XtSetArg (args[i], XmNwidth, 100); i++;
	XtSetArg (args[i], XmNheight, 20); i++;
	texto_entradas = XmCreateTextField(col1, "texto entradas", args, i);
	
	i=0;
	cadena = XmStringCreateLocalized ("Modify");
    XtSetArg (args[i], XmNlabelString, cadena); i++;
 	XtSetArg (args[i], XmNwidth, 100); i++;
	XtSetArg (args[i], XmNheight, 20); i++;

	Widget botonOK = XmCreatePushButton(col1, "Botón para modificar entrada", args, i);
	XtAddCallback (botonOK, XmNactivateCallback, pulsaOKModificaEntrada, NULL);	
	
	XtManageChild(col1);
	XtManageChild(texto_entradas);
	XtManageChild(l1f1);
	XtManageChild(botonOK);
	XtManageChild(dlgModify);
	}

//**************** ADD NUEVAS ENTRADAS *************************
/* Añade todas las entradas que no estuvieran ya en la base de datos.
	*/
void addNuevasEntradas(char **entradas, int numEntradas)
	{
	double energias[numEntradas];
	char buffer[1024];
	double ef=-300;
	int ind;
	int ultimo=-1;

	
	printf("ADD NUEVAS ENTRADAS: tenemos que añadir %d nuevas entradas\n", numEntradas);
	//Abrimos los ficheros
	ifstream ifile(nombre);
	ofstream ofile("temp.fbd", ios::trunc);
	if (!ifile.is_open() || !ofile.is_open() )	cout << "Error opening files" << endl; 
	string finfich("EOF");
	string vacia("");
	int numColumnas=numeroColumnas(nombre);
	printf("El número de columnas del fichero es %d\n", numColumnas);
		
	//Cogemos la energía de cada entrada
	for(int i=0;i<numEntradas;i++)
		{
		string aux(entradas[i]);
		ind=aux.find(" ");
		string cadenaEnergia=aux.substr(0,ind);
		cout << "cadenaEnergia vale " << cadenaEnergia << endl;
		energias[i]=atof((char *)cadenaEnergia.c_str());
		}
	int cont=0;
	while(!ifile.eof())
		{
//		printf("Línea %d\n", cont++);
	
		//Recorremos el fichero hasta llegar a la línea correspondiente
		ifile.getline (buffer,1024);
		string cadena(buffer);
		cout << cadena << endl;
		if(cadena!=finfich && cadena!=vacia)
			{
			if((ind=cadena.find("\t"))!=-1)	
				{
				string cadenaEnergia(cadena.substr(0,ind));
				ef=atof((char *)cadenaEnergia.c_str());
				}
			}
		else	ef=-300;
//		printf("EF vale %f\n", ef);
		for(int i=ultimo+1;i<numEntradas;i++)
			{
			if(ef>=energias[i] || ef==-300)
				{//Tenemos que meter aquí una fila
//				printf("Tenemos que meter aquí una fila\n");
				ultimo=i;//Para no repetir filas que ya se han añadido
				//Construimos la nueva fila:
				string nuevaEntrada(entradas[i]);	
				//Cogemos el trozo final, con la reaccion y el inicial, con la energía
				ind=nuevaEntrada.find(" ");
				string ener    =nuevaEntrada.substr(0, ind);
				string reaccion=nuevaEntrada.substr(ind, nuevaEntrada.length());
//				cout << "energía " << ener << "(" << ener.length() << ")" << endl;
//				cout << "reaccion " << reaccion << "(" << reaccion.length() << ")" << endl;
				//Cambiamos el espacio por los tabuladores necesarios
				if(ener.length()>7.0)	nuevaEntrada.replace(ind, 1, "\t");
				else					nuevaEntrada.replace(ind, 1, "\t\t");
				//Añadimos los tabuladores necesarios al final
				
				int numTab=(int)((28.0-reaccion.length())/4.0);
				cout << "añado " << numTab << " tabuladores a " << nuevaEntrada << endl;
				for(int j=0;j<=numTab; j++)
					nuevaEntrada.insert(nuevaEntrada.length(), "\t");
			    if(numTab<=0)	//Aunque no haya hueco, metemos un tabulador
					nuevaEntrada.insert(nuevaEntrada.length(), "\t");

				//Añadimos ---- para todas las columnas anteriores.
//				for(int j=0;j<numColumnas+1;j++)//Me daba 0 teniendo ya una columna?
				for(int j=0;j<numColumnas;j++)//Me daba 0 teniendo ya una columna?
					nuevaEntrada.insert(nuevaEntrada.length(), "\t----\t");
				if(numeroColumnas(nombre)>0)	nuevaEntrada.insert(nuevaEntrada.length(), "\t0(0)\t");//Añadimos al final un dummy mean
				//Insertamos la cadena en el fichero:
				ofile << nuevaEntrada << endl;
				cout << "Entradas " << i << " vale |" << nuevaEntrada << "|" << endl;;
				}
			else	
				{
				//No tenemos energias menores que por la que vamos de las antiguas
				//Metemos la antigua
				ofile << cadena << endl;					
			//	cout <<"Entradas " << i << " vale |" << cadena << "|" << endl;;
				break;
				}
			}
		//Para las que ya estén más allá de las energías nuevas, metemos la fila sin más
		if(ultimo+1>=numEntradas)		ofile << cadena << endl;
		}//while(!eof)

	ifstream ifile2("temp.fbd");
	ofstream ofile2(nombre, ios::trunc);
	
	//Pasamos del fichero temporal al fichero final
	if (!ifile2.is_open() || !ofile2.is_open() ) 	{ cout << "Error opening files"; exit(1); }
	while (! ifile2.eof() )
		{
		ifile2.getline (buffer,1024);
		ofile2 << buffer << "\n";
		}
	ofile2.close();
	ifile2.close();
	remove("temp.fbd");
		
	ifile.close();
	ofile.close();

	//Ahora hacemos las columnas necesarias
	quitarUltimaColumna(nombre);
	printf("Fichero sin la media\n");
	imprimirFichero(nombre);
	addColumns(nombre);	
	printf("Fichero con la nueva columna y la media\n");
	imprimirFichero(nombre);
	}
	
//**************** PULSA OK MODIFICA ENTRADA ***********************************
/* Efectúa en la lista de nuevas entradas la modificación realizada en el nombre
*/	
void pulsaOKModificaEntrada(Widget widget, XtPointer client_data, XtPointer call_data) 
	{
	printf("PULSA OK MODIFICA ENTRADA\n");
	int *posiciones, numSel, pos;
	if (!XmListGetSelectedPos(listaEntries, &posiciones, &numSel))
		pos = 0;
	else 
		{
		pos = *posiciones;
		XtFree((char *)posiciones);
		}
	
	//Crear nueva entrada con información de modificación
	string cad(entradas[pos-1]);

	int ind=0;
	char *texto=new char[50];
		
	//Quitamos el antiguo nombre para la reacción
	//Y ponemos el nuevo (si lo hay)
	if( strcmp(texto=XmTextGetString(texto_entradas), "") )
		{
		if((ind=cad.find(" "))!=-1)	cad=cad.substr(0,ind+1);
		string cad2(texto);
		cad=cad+cad2;
		entradas[pos]=(char *)cad.c_str();
		XmString cadena = XmStringCreateLocalized(entradas[pos]);
		
		//Borrar posición de la lista
		XmListDeletePos(listaEntries, pos);
		//añadir entrada en la misma posición
		XmListAddItem(listaEntries, cadena, pos);
		XmStringFree(cadena);
		}

	XtUnmanageChild(dlgModify);
	}
	
//****************** IGNORA TODAS BD ***********************
/*	Ignora todas las nuevas entradas para la base de datos del fondo, de forma
	que sólo se añadirán las que ya aparezcan en la base de datos.
	*/
void IgnoraTodasBD(Widget widget, XtPointer client_data, XtPointer call_data) 
	{
	printf("Ignora todas BD\n");
	XmListDeleteAllItems(listaEntries);
	XtUnmanageChild(dlgBD);		
	}

//************* QUITA VENTANA BD ****************
void QuitaVentanaBD(Widget widget, XtPointer client_data, XtPointer call_data) 
	{
	printf("Pulsa QuitaVentanaBD\n");
	XmListDeleteAllItems(listaEntries);
	XtUnmanageChild(dlgBD);
	}

//**************** IGNORA BD **************************
/*	Ignora la entrada seleccionada, con lo que no se añadirá a la base de 
	datos
	*/
void IgnoraBD(Widget widget, XtPointer client_data, XtPointer call_data) 
	{
	int *posiciones, numSel, nPos;
	if (!XmListGetSelectedPos(listaEntries, &posiciones, &numSel))
		nPos = 0;
	else 
		{
		nPos = *posiciones;
		XtFree((char *)posiciones);
		}
	// Extraño error, más de un pico seleccionado
	if (numSel > 1) nPos = 0;

	XmListDeletePos(listaEntries, nPos);
	}

//*************************** SELECCIONA FORMATO ***************
/* Guardamos el formato seleccionado para este fichero en la variable global tipoFormato
	Donde:
		1=Canberra
		2=Ortec
		3=LAP
*/

void seleccionaFormato(Widget widget, XtPointer client_data, XtPointer call_data) 
	{
	XmComboBoxCallbackStruct *cb=(XmComboBoxCallbackStruct *) call_data;
	XmString patron;

	char *choice=(char *) XmStringUnparse (cb->item_or_text, 
											XmFONTLIST_DEFAULT_TAG,
											XmCHARSET_TEXT, XmCHARSET_TEXT,
											NULL, 0, XmOUTPUT_ALL);
		
	if(cb->event == NULL)	printf("ERROR: Posible elección: %s\n", choice);
	
	if(!strcmp(choice, "Raw"))
		{
		tipoFormato=1;
		patron = XmStringCreateLocalized ("*.dat");//Formato interno de Galea
		XtVaSetValues (dlg_files_formato, XmNpattern, patron, NULL);
		XmStringFree (patron);
		}
	else if(!strcmp(choice, "Canberra"))	//Formato de los archivos bege tras lotcnv	
		{
		tipoFormato=2;
		patron = XmStringCreateLocalized ("*.ASC");
		XtVaSetValues (dlg_files_formato, XmNpattern, patron, NULL);
		XmStringFree (patron);
		}
	else if(!strcmp(choice, "Ortec"))	
		{
		tipoFormato=3;
		patron = XmStringCreateLocalized ("*.chn");//Formato no utilizado de momento
		XtVaSetValues (dlg_files_formato, XmNpattern, patron, NULL);
		XmStringFree (patron);
		}
		 else if(!strcmp(choice, "LAP"))
		 	{
			patron = XmStringCreateLocalized ("*.asc");//Formato de los archivos rege
			XtVaSetValues (dlg_files_formato, XmNpattern, patron, NULL);
			XmStringFree (patron);
			tipoFormato=4;
			}
		 else							tipoFormato=-1;			 
			 
	return;
	}

/******************************************************************************
 ****                               MENUES
 ******************************************************************************/

Widget CreaMenu(Widget Padre) 
	{
	Widget Menu;
	
	XmString sFile 			= XmStringCreateLocalized (" File ");
	XmString sCalibracion	= XmStringCreateLocalized(" Calibration ");
	XmString sAnalisis 		= XmStringCreateLocalized(" Analysis ");
	Menu = XmVaCreateSimpleMenuBar (Padre, "menubar",
				XmVaCASCADEBUTTON, sFile, 'F',
				XmVaCASCADEBUTTON, sCalibracion, 'C',
				XmVaCASCADEBUTTON, sAnalisis, 'A',
				NULL);
	XmStringFree(sFile); XmStringFree (sCalibracion); XmStringFree (sAnalisis);

	// crea las opciones del menu File
	XmString sOpen 		= XmStringCreateLocalized ((char *)MENU_FILE_OPEN_NAME);
	XmString sOpenSpec 	= XmStringCreateLocalized ((char *)MENU_FILE_OPEN_SPECTRUM_NAME);
	XmString sOpenBack 	= XmStringCreateLocalized ((char *)MENU_FILE_OPEN_BACKGROUND_NAME);
	XmString sSave 		= XmStringCreateLocalized ((char *)MENU_FILE_SAVE_NAME);
	XmString sSaveAs 	= XmStringCreateLocalized ((char *)MENU_FILE_SAVE_AS_NAME);
	XmString sSaveTo	= XmStringCreateLocalized ((char *)MENU_FILE_SAVE_TO_NAME);
	XmString sQuit 		= XmStringCreateLocalized ((char *)MENU_FILE_QUIT_NAME);
	XmVaCreateSimplePulldownMenu (Menu, "file_menu", 0, file_cb,
				XmVaPUSHBUTTON, sOpen,	   'O', NULL, NULL,
				XmVaPUSHBUTTON, sOpenSpec, 'u', NULL, NULL,
				XmVaPUSHBUTTON, sOpenBack, 'B', NULL, NULL,
				XmVaSEPARATOR,  XmVaSEPARATOR,
				XmVaPUSHBUTTON, sSave,  	'S', NULL, NULL,
				XmVaPUSHBUTTON, sSaveAs, 	'A', NULL, NULL,
				XmVaPUSHBUTTON, sSaveTo, 	'g', NULL, NULL,
				XmVaSEPARATOR,  XmVaSEPARATOR,
				XmVaPUSHBUTTON, sQuit,  	'Q', NULL, NULL,
				NULL);
	XmStringFree (sOpen); XmStringFree (sOpenSpec);
	XmStringFree (sSave); XmStringFree (sSaveAs); XmStringFree(sSaveTo);
	XmStringFree (sQuit);

	// crea las opciones del menu Calibration
	XmString sOpenC      = XmStringCreateLocalized ((char *)MENU_CALIBRATION_OPEN_NAME);
	XmString sOpenEnergy = XmStringCreateLocalized ((char *)MENU_CALIBRATION_OPEN_ENERGY_NAME);
	XmString sOpenShape  = XmStringCreateLocalized ((char *)MENU_CALIBRATION_OPEN_SHAPE_NAME);
	XmString sSaveC      = XmStringCreateLocalized ((char *)MENU_CALIBRATION_SAVE_NAME);
	XmString sSaveEnergy = XmStringCreateLocalized ((char *)MENU_CALIBRATION_SAVE_ENERGY_NAME);
	XmString sSaveShape  = XmStringCreateLocalized ((char *)MENU_CALIBRATION_SAVE_SHAPE_NAME);
	XmVaCreateSimplePulldownMenu (Menu, "calibration_menu", 1, calibracion_cb,
				XmVaPUSHBUTTON, sOpenC,      'O', NULL, NULL,
				XmVaPUSHBUTTON, sOpenEnergy, 'E', NULL, NULL,
				XmVaPUSHBUTTON, sOpenShape,  'h', NULL, NULL,
				XmVaSEPARATOR, XmVaSEPARATOR,
				XmVaPUSHBUTTON, sSaveC,      'S', NULL, NULL,
				XmVaPUSHBUTTON, sSaveEnergy, 'n', NULL, NULL,
				XmVaPUSHBUTTON, sSaveShape,  'p', NULL, NULL,
				NULL);
	XmStringFree (sOpenC); XmStringFree (sOpenEnergy); XmStringFree (sOpenShape);
	XmStringFree (sSaveC); XmStringFree (sSaveEnergy); XmStringFree (sSaveShape);
	
	// crea las opciones del menu Analisis
	XmString sLoadF      = XmStringCreateLocalized ((char *)MENU_ANALYSIS_LOAD_EFFICIENCY_NAME);
	XmString sLoadB      = XmStringCreateLocalized ((char *)MENU_ANALYSIS_LOAD_BACKGROUND_NAME);
	XmString sLoadI      = XmStringCreateLocalized ((char *)MENU_ANALYSIS_LOAD_IDENTIFICA_NAME);
	XmString sSaveA      = XmStringCreateLocalized ((char *)MENU_ANALYSIS_SAVE_NAME);
	XmString sDeleteP    = XmStringCreateLocalized ((char *)MENU_ANALYSIS_DELETE_PEAKS_NAME);
	XmString sOptions    = XmStringCreateLocalized ((char *)MENU_ANALYSIS_OPTIONS_NAME);
	XmVaCreateSimplePulldownMenu (Menu, "analysis_menu", 2, analisis_cb,
				XmVaPUSHBUTTON, sLoadF,      'E', NULL, NULL,
				XmVaPUSHBUTTON, sLoadB,      'B', NULL, NULL,
				XmVaPUSHBUTTON, sLoadI,      'I', NULL, NULL,
				XmVaSEPARATOR, XmVaSEPARATOR,
				XmVaPUSHBUTTON, sSaveA,      'S', NULL, NULL,
				XmVaSEPARATOR, XmVaSEPARATOR,
				XmVaPUSHBUTTON, sDeleteP,    'D', NULL, NULL,
				XmVaPUSHBUTTON, sOptions,    'O', NULL, NULL,
				NULL);
	XmStringFree (sSaveA);
	XmStringFree (sLoadF);
	XmStringFree (sLoadB);
	XmStringFree (sDeleteP);
	XmStringFree (sOptions);

	return Menu;
	}

	
//****************** ENERGIA MEDIA ****************
/*		Devuelve la energía media ponderada para el 
	conjunto de emisiones determinadas por la lista
	enlazada que se pasa como parámetro
	*/
float energiaMedia(LinkedList *emisiones)
	{
	float sum=0.0;
	float den=0.0;

	printf("ENERGIA MEDIA, %d emisiones \n", emisiones->numElements());
	for(int i=0;i<emisiones->numElements();i++)
		{
		NodoEmision *ne=(NodoEmision *)emisiones->getNode(i);
		int codEmisor=LaraBD.GetEmissor(ne->codElemento);
		int mn=LaraBD.GetMassNumber(codEmisor);
		int a=LaraBD.GetAtomicNumber(codEmisor);
		printf("buscando emisiones para (%f,%f) keV del elemento %d, %d\n", ne->Energia-0.1, ne->Energia+0.1, a,mn);
		cSearchLaraBD founds=
			LaraBD.SearchEmissions(ne->Energia-0.1, ne->Energia+0.1,-1,-1,-1,
				a,a,mn,mn);
		printf("emisión %d tiene %d asociadas\n", i, founds.GetNumItemsFound());
		if(founds.GetNumItemsFound()>0)
			{
			printf("Encontrados para energia media %d emisiones\n", founds.GetNumItemsFound());
			int codigo=founds.GetFirstResult();
			float prob=LaraBD.GetProbability(codigo);
			if(padre(LaraBD.GetEmissor(codigo), 92,235))	
				{
				printf("De la familia del U235\n");
				prob*=0.042;
				}
			printf("Sumamos a den %.2f y a sum %.2f*%.2f=%.2f\n", prob, prob, ne->Energia, prob*ne->Energia);
			den+=prob;
			sum+=ne->Energia*prob;
			}
		else
			{
			printf("ERROR: No tenemos esta emisión para hacer la media\n");
			}
		}
	printf("La media de energías ponderada es %f\n", sum);
	sum/=den;
	return sum;
	}

//*************** CREA VENTANA OPCIONES ****************
/*	Crea una ventana con las distintas opciones de ajuste:
		-Aplicar las distintas restricciones o no
		-Holgura en las restricciones
	*/
void creaVentanaOpciones()
	{
   	Arg		args[7];
   	Cardinal	i = 0;
	XmString cadena;
	
	// *********************
	// * cuadro de dialogo *
	// *********************
	i = 0;
	printf("Empezamos a crear\n");
	XtSetArg (args[i], XmNtitle, "Adjust Options"); i++;
	XtSetArg (args[i], XmNmwmDecorations, MWM_DECOR_BORDER|MWM_DECOR_TITLE|MWM_DECOR_MENU); i++;
	XtSetArg (args[i], XmNmwmFunctions, MWM_FUNC_MOVE); i++;
	dlg_options = XmCreateFormDialog(app_shell, "dlg_options", args, i);

	/*******************
	El cuadro de diálogo se separa en tres zonas básicas:
		-fr_adjust_edAnal: Datos del ajuste, con la gráfica y la fórmula para guiar
		-fr_energy_edAnal: Datos de la energía y elementos químicos asociados
		-fm_botones_edAnal: Botones de ok, cancel y adjust
	Todos tendrán como padre a dlg_edAnal, el cuadro de diálogo.
	*******************/
	printf("Creamos secciones\n");
	i = 0;
	Widget fr_opciones = XmCreateForm (dlg_options, "fr_adjust_edAnal", args, i);
	Widget linea = XmCreateSeparator(dlg_options, "linea", args, i);
	Widget fr_botones = XmCreateForm (dlg_options, "fr_energy_edAnal", args, i);

   	XtVaSetValues (fr_botones,
		XmNbottomAttachment, XmATTACH_FORM,
		XmNrightAttachment, XmATTACH_FORM,
		XmNrightOffset, 10,
		XmNleftAttachment, XmATTACH_FORM,
		XmNleftOffset, 10,
		NULL);
   	XtVaSetValues (linea,
		XmNbottomAttachment, XmATTACH_WIDGET, XmNbottomWidget, fr_botones,
		XmNbottomOffset, 10,
		XmNrightAttachment, XmATTACH_FORM,
		XmNleftAttachment, XmATTACH_FORM,
		NULL);
   	XtVaSetValues (fr_opciones,
		XmNbottomAttachment, XmATTACH_WIDGET, XmNbottomWidget, linea,
		XmNbottomOffset, 10,
		XmNrightAttachment, XmATTACH_FORM,
		XmNrightOffset, 10,
		XmNleftAttachment, XmATTACH_FORM,
		XmNleftOffset, 10,
		NULL);
	XtManageChild(fr_opciones);
	XtManageChild(linea);
	XtManageChild(fr_botones);
		
	//*** Columnas para colocación
	i = 0;
	XtSetArg (args[i], XmNorientation, XmVERTICAL); i++;
	Widget rw1 = XmCreateRowColumn (fr_opciones, "rw1", args, i);
	
	i = 0;
	XtSetArg (args[i], XmNorientation, XmVERTICAL); i++;
	Widget rw2 = XmCreateRowColumn (fr_opciones, "rw2", args, i);

	XtManageChild(rw1);	
	XtManageChild(rw2);	
	
	// **********************
	// * seccion de opciones*
	// **********************
	
	int count=4;
	char *tipos[]={"Restrict Tau (by Shape Calibration)", "Restrict Sigma (by Shape Calibration)", 
		"Restrict Centroid (by Energy Calibration)", "Restrict Area (by internal computing)"};
	XmStringTable str_list=(XmStringTable) XtMalloc(count * sizeof(XmString *));
	for(int i=0;i<count;i++)	str_list[i]=XmStringCreateLocalized(tipos[i]);
	
   	i = 0;
   	XtSetArg (args[i], XmNlabelString, str_list[0]); i++;
	XtSetArg (args[i], XmNset, opciones[0]); i++;
	Widget tb_tau = XmCreateToggleButton(rw1, "0", args, i);
   	i = 0;
   	XtSetArg (args[i], XmNlabelString, str_list[1]); i++;
	XtSetArg (args[i], XmNset, opciones[1]); i++;
	Widget tb_sigma = XmCreateToggleButton(rw1, "1", args, i);
   	i = 0;
   	XtSetArg (args[i], XmNlabelString, str_list[2]); i++;
	XtSetArg (args[i], XmNset, opciones[2]); i++;
	Widget tb_centroid = XmCreateToggleButton(rw1, "2", args, i);
   	i = 0;
   	XtSetArg (args[i], XmNlabelString, str_list[3]); i++;
	XtSetArg (args[i], XmNset, opciones[3]); i++;
	Widget tb_area = XmCreateToggleButton(rw1, "3", args, i);
	
	XtAddCallback (tb_tau, XmNvalueChangedCallback, pulsarOpcion, (XtPointer)0);
	XtAddCallback (tb_sigma, XmNvalueChangedCallback, pulsarOpcion, (XtPointer)1);
	XtAddCallback (tb_centroid, XmNvalueChangedCallback, pulsarOpcion, (XtPointer)2);
	XtAddCallback (tb_area, XmNvalueChangedCallback, pulsarOpcion, (XtPointer)3);
	
	XtManageChild(tb_tau);
	XtManageChild(tb_sigma);
	XtManageChild(tb_centroid);
	XtManageChild(tb_area);
	
	// **********************
	// * seccion de botones *
	// **********************
	i = 0;
   	cadena = XmStringCreate ("OK", "BoldNormal");
   	XtSetArg (args[i], XmNlabelString, cadena); i++;
	XtSetArg (args[i], XmNalignment, XmALIGNMENT_CENTER); i++;
	XtSetArg (args[i], XmNwidth, 100); i++;
	Widget bt_ok_options = XmCreatePushButton(fr_botones, "bt_ok_options", args, i);
	XtAddCallback (bt_ok_options, XmNactivateCallback, pulsa_ok_opciones, NULL);
   	XmStringFree (cadena);
	
	i = 0;
   	cadena = XmStringCreate ("Cancel", "BoldNormal");
   	XtSetArg (args[i], XmNlabelString, cadena); i++;
	XtSetArg (args[i], XmNalignment, XmALIGNMENT_CENTER); i++;
	XtSetArg (args[i], XmNwidth, 100); i++;
	Widget bt_cancel_options = XmCreatePushButton(fr_botones, "bt_cancel_options", args, i);
	XtAddCallback (bt_cancel_options, XmNactivateCallback, pulsa_cancel_opciones, NULL);
   	XmStringFree (cadena);

   	XtVaSetValues (bt_ok_options,
		XmNbottomAttachment, XmATTACH_FORM,
		XmNtopAttachment, XmATTACH_FORM,
		XmNleftAttachment, XmATTACH_FORM,
//		XmNrightAttachment, XmATTACH_POSITION, XmNrightPosition, 1,
		XmNrightAttachment, XmATTACH_WIDGET, XmNrightWidget, bt_cancel_options,
		XmNrightOffset, 5, 
		NULL);
   	XtVaSetValues (bt_cancel_options,
		XmNbottomAttachment, XmATTACH_FORM,
		XmNtopAttachment, XmATTACH_FORM,
//		XmNleftAttachment, XmATTACH_POSITION, XmNleftPosition, 1,
//		XmNrightAttachment, XmATTACH_POSITION, XmNrightPosition, 2,
		XmNrightAttachment, XmATTACH_FORM,
		XmNrightOffset, 5, XmNleftOffset, 5,
		NULL);

	XtManageChild(bt_ok_options);
	XtManageChild(bt_cancel_options);
	}
	
//************ PULSA CANCEL OPCIONES ***********
void pulsa_ok_opciones(Widget widget, XtPointer client_data, XtPointer call_data) 
	{
	for(int i=0;i<maxOpciones;i++)	opcionesAnteriores[i]=opciones[i];
	XtUnmanageChild(dlg_options);	
	}

//************ PULSA CANCEL OPCIONES ***********
void pulsa_cancel_opciones(Widget widget, XtPointer client_data, XtPointer call_data) 
	{
	for(int i=0;i<maxOpciones;i++)	opciones[i]=opcionesAnteriores[i];
	XtUnmanageChild(dlg_options);	
	}

//***************** PULSAR OPCION ********************
void pulsarOpcion(Widget widget, XtPointer client_data, XtPointer call_data) 
	{
	XmToggleButtonCallbackStruct *state=(XmToggleButtonCallbackStruct *) call_data;
	
	printf("%s: %s\n", XtName(widget),
					state->set == 1 ? "on" :
					state->set == 0 ? "off" : "indeterminate");
		
	if(state->set)	opciones[(int) client_data]=true;
	else			opciones[(int) client_data]=false;
		
	}
