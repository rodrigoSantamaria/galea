#include "globales.h"
#include "VisIdent.h"
#include "VisConti.h"
#include "avisos.h"
#include "fuentes.h"
#include <math.h>
#include <stdlib.h>
#include "math1.h"
#include <stdio.h>
#include <Xm/Form.h>
#include <Xm/Label.h>
#include <Xm/List.h>
#include <Xm/PushB.h>
#include <Xm/RowColumn.h>
#include <Xm/ToggleB.h>
#include <Xm/MwmUtil.h>
#include <Xm/Frame.h>
#include <Xm/Separator.h>
#include <Xm/Text.h>
#include <Xm/TextF.h>
#include <Xm/ToggleBG.h> 
#include <Xm/ToggleB.h>

#include <Xm/ComboBox.h>

	
//*** Widgets para la búsqueda avanzada:
Widget dlg_changeSearch;
Widget lb_emission_search, lb_emissor_search, lb_emin, lb_emax, lb_pmin, lb_pmax,
		lb_tipo, lb_amin, lb_amax, lb_prmin, lb_prmax, lb_meta, lb_natu, lb_name,
		lb_any, lb_gamma, lb_xa1, lb_xa2, lb_xb1, lb_xb2, lb_mnany;
Widget fr_emission_search, fr_emissor_search;
Widget linea_changeSearch;
Widget fm_botones_search, fm_emission_search, fm_emissor_search;
Widget txt_emin, txt_pmin, txt_pmax, txt_emax, txt_amin, txt_amax, txt_prmin,
	   txt_prmax, txt_name;
Widget rw1_emission_search, rw2_emission_search,
	   rw1_emissor_search, rw2_emissor_search;
Widget bt_ok_search, bt_cancel_search;
Widget cb_tipo, cb_meta, cb_natu;
Widget tb_gamma, tb_xa1, tb_xa2, tb_xb1, tb_xb2, tb_msi, tb_mno, tb_nsi, tb_nno,
	   tb_many, tb_nany, tb_any;

int tipoGamma=-1, tipoMeta=-1, tipoNatu=-1;
double cent, wide;
int intervalo, pico;//Variables para saber qué pico de qué intervalo estamos tratando de identificar

/****************** XmStringNameElement ******************************
Genera una cadena con formato para salida de bytes (no wide chars), del tipo:
	226Ra[(e)]
	Se usa un tamaño pequeño para los números para que parezca que son subíndices.
	NOTA: Realmente deberían ser superíndices, pero para ello tenemos que utilizar 
	formato wide char (ver XmStringNameElementSup)
*/
XmString XmStringNameElement(unsigned short int element, char *littleFont, char *normalFont ) 
	{
	char entry_text[100], name[3];
	XmString final;
	LaraBD.GetNameElement(element, name);

	sprintf(entry_text, "%s", name);
	XmString entry2 = XmStringCreate (entry_text, normalFont);
	sprintf(entry_text, "%d", LaraBD.GetMassNumber(element));
	XmString entry = XmStringCreate (entry_text, littleFont);

	if(LaraBD.IsEqui(element))	
		{
		sprintf(entry_text, "(e)", name);
		XmString entry3 = XmStringCreate(entry_text, normalFont);
		final = XmStringConcat(entry, entry2);
		final = XmStringConcat(final, entry3);
		XmStringFree(entry3);
		}
	else	final = XmStringConcat(entry, entry2);
	XmStringFree(entry); XmStringFree(entry2);
/*
	if(LaraBD.IsEqui(element))	
		sprintf(entry_text, "%s%d(e)", name, LaraBD.GetMassNumber(element));
	else		
		sprintf(entry_text, "%s%d", name, LaraBD.GetMassNumber(element));
		
	final=XmStringCreate(entry_text, littleFont);
	*/
	return final;
	}

/************** XmStringNameElementSup *************************
	Genera una cadena con formato en estilo wide char para poder mostrar 
	correctamente los superíndices, creando una salida similar a la siguiente:
		²²⁶Ra[(e)]
	XmStringNameElement no nos proporciona superíndices, y crea una salida tipo:
		226Ra[(e)]
*/
XmString XmStringNameElementSup(unsigned short int element, char *littleFont, char *normalFont ) 
	{
	char name[3];
	wchar_t *sup;
	XmString cadenaTemp;
	int max=10;
	wchar_t cadena[max];

	sup=superindice(LaraBD.GetMassNumber(element));
	LaraBD.GetNameElement(element, name);

	if(LaraBD.IsEqui(element))	swprintf(cadena,max,L"%s%s(e)", sup, name);
	else						swprintf(cadena,max,L"%s%s"   , sup, name);

//	XwcTextListToTextProperty(Display *display, &sup, 1,
//					XICCEncodingStyle style, XTextProperty textPropReturn);
//	XwcTextListToTextProperty(dpy, &sup, 1,	XCompoundTextStyle, &cadenaTemp);
//	cadenaTemp=XmStringGenerate(cadena, NULL, XmWIDECHAR_TEXT, NULL);
	cadenaTemp=XmStringGenerate(cadena, NULL, XmMULTIBYTE_TEXT, NULL);
	return cadenaTemp;
	}

//***************** SUPERINDICE ****************
/*	Devuelve una cadena con numero convertido en una cadea de superíndices con
dicho número
NOTA: Esta función trabaja bien, pero el problema es que o bien la representación
de los caracteres está mal en superindices, o su almacenamiento no es correcto,
o la forma de representarlos está mal.
NOTA: Sólo tiene en cuenta números de 1,2 ó 3 dígitos, que son los que manejamos
para los números másicos
*/
//char *superindice(int numero)
wchar_t *superindice(int numero)
	{
	//char *sup;
	wchar_t *sup;
	int tam=0;	
	if(numero>99)		tam=3;
	else
		{
		if(numero>9)	tam=2;
		else			tam=1;
		}
		
//	sup=new char[tam];
	sup=new wchar_t[tam];
	printf("El número es: %d, %d, %d\n", numero/100, (numero%100)/10, numero%10);
	sup[tam-1]=superindices[numero%10];
	if(numero>9)	sup[tam-2]=superindices[(numero%100)/10];
	if(numero>99)	sup[tam-3]=superindices[numero/100];
	return sup;
	}

/*************************************************************
********************** XM STRING NAME ELEMENT PLUS ***********
*************************************************************/
XmString XmStringNameElementPlus(unsigned short int element, char *littleFont, char *normalFont ) {// =""
	char entry_text[100], name[3];

	LaraBD.GetNameElement(element, name);
	sprintf(entry_text, "+%s", name);
	XmString entry = XmStringCreate (entry_text, normalFont);
	sprintf(entry_text, "%d", LaraBD.GetMassNumber(element));
	XmString entry2 = XmStringCreate (entry_text, littleFont);
	XmString final = XmStringConcat(entry, entry2);
	XmStringFree(entry); XmStringFree(entry2);
	
	return final;
}

/******************************************************
 ***												***
 ***					CALLBACKS LISTS				***
 ***												***
 *****************************************************/
void ChangeSortCB(Widget, XtPointer, XtPointer);
void ChangeSelectCB(Widget, XtPointer, XtPointer);

void ChangeSortCB(Widget widget, XtPointer client_data, XtPointer call_data) 
	{
   	XmToggleButtonCallbackStruct *state = (XmToggleButtonCallbackStruct *) call_data;
	cVisIdentify *object = (cVisIdentify *) client_data;
	
	object->ShowCandidates();
	}

//********************* CHANGE SELECT CB *********************
//NOTA: Ahora que construimos la lista en dos partes parece que le da problemas
//Cambia el elemento seleccionado en la lista de emisiones
void ChangeSelectCB(Widget widget, XtPointer client_data, XtPointer call_data) 
	{
   	XmListCallbackStruct *cbs = (XmListCallbackStruct *) call_data;
	cVisIdentify *object = (cVisIdentify *) client_data;
	printf("CHANGE SELECT CB");
	printf("La nueva emisión es la que está en la posición %d\n", cbs->item_position);
	object->ChangeSelectedEmission(cbs->item_position);
	}

void pulsa_changeSearch (Widget, XtPointer, XtPointer);
void createDialogSearch (cVisIdentify *);
void pulsa_ok_search(Widget, XtPointer, XtPointer);
void pulsa_cancel_search(Widget, XtPointer, XtPointer);
void pulsarTipo(Widget, XtPointer, XtPointer);
void pulsarMeta(Widget, XtPointer, XtPointer);
void pulsarNatu(Widget, XtPointer, XtPointer);
void seleccionarMeta(Widget, XtPointer, XtPointer); 
void seleccionarNatu(Widget, XtPointer, XtPointer); 
void seleccionarTipo(Widget, XtPointer, XtPointer); 

//void buscarCandidatos(int, int, int, int, float, float, float, float,
	//				  bool, bool, bool, bool,int, char);

/*****************************************************
 ***													***
 ***						INTERNAL LISTS				***
 ***													***
 *****************************************************/
 
 //NODE LIST
 /*Lista de nodos, cada uno con:
 	-value
	-score
 */
typedef struct NodeList 
	{
	unsigned short int value;
	double score;
	NodeList *next;
	} NodeList;

// Lista de emisiones
typedef struct 
	{
	NodeList *first;
	int numElements;
	} tListEmissions;

void AddEmissionToList (tListEmissions &list, NodeList *node);
void AddLastEmissionToList (tListEmissions &list, NodeList *node);
void DelList (tListEmissions &list);

//************** ADD EMISSION TO LIST *****************
	// Añade node a list en la posición correspondiente según orden 
	// ascendente de score
void AddEmissionToList (tListEmissions &list, NodeList *node) 
	{
	NodeList **guide;
	
	guide = &(list.first);
	while ((*guide) != NULL)
		{
		// check for sort
		if ((*guide)->score < node->score) 
			{
			node->next = *guide;
			*guide = node;
			list.numElements++;
			return;
			}
		// pass to next element
		guide = &((*guide)->next);
		}
	// put element in the last place of the list
	node->next = NULL;
	*guide = node;
	list.numElements++;
	}

//**************** ADD LAST EMISSION TO LIST *******************
void AddLastEmissionToList (tListEmissions &list, NodeList *node) 
	{
/*	NodeList *guide;
	
	guide = list.first;
	while (guide != NULL)
		{
		// pass to next element
		guide = guide->next;
		}
	
	// put element in the last place of the list
	node->next = NULL;
	guide = node;
	list.numElements++;
	*/		
	NodeList **guide;
	
	guide = &(list.first);
	while ((*guide) != NULL)
		{
		// pass to next element
		guide = &((*guide)->next);
		}
	
	// put element in the last place of the list
	node->next = NULL;
	*guide = node;
	list.numElements++;
	}

//******************* DEL LIST *************
void DelList (tListEmissions &list) 
	{
	NodeList *guide;
	NodeList *del;
	printf("DEL LIST (VISIDENT)\n");
		
	guide = list.first;
	while (guide != NULL) 
		{
		del = guide;
		guide = guide->next;
		delete del;
		}
	list.numElements = 0;
	list.first = NULL;
	printf("fin de DEL LIST\n");
	}

/*****************************************************
 ***																***
 ***				IDENTIFY WIDGETS PACKET					***
 ***																***
 *****************************************************/

/* ***************************
	**     General Methods   **
	***************************/
	// Constructor
cVisIdentify::cVisIdentify() 
	{
	// initialize values
	father = NULL;
	Fonts = NULL;
	BoldName = "\0";
	LittleName = "\0";
	peakList = NULL;
	listEmissions = (void*) new tListEmissions;
	tListEmissions *list = (tListEmissions *) listEmissions;
	list->numElements = 0;
	list->first = NULL;
	nInterval = 0; nPeak = 0;
	selectedEmission = 0;
	}

	// Destructor
cVisIdentify::~cVisIdentify() 
	{
	tListEmissions *list = (tListEmissions *)listEmissions;
	DelList(*list);
	delete list;
	}

/* ************************************
	**   Management Widgets Methods   **
	************************************/

void cVisIdentify::SetFontBold(char *name) {
	BoldName = name;
}

void cVisIdentify::SetFontLittle(char *name) 
	{
	LittleName = name;
	}
	
Widget cVisIdentify::getFoundsList()
	{
	return lstList;
	}

//************ CREATE WIDGETS *******************
void cVisIdentify::CreateWidgets(Widget parent) 
	{
    Arg		args[7];
    Cardinal	i = 0;
	XmString cadena;

	father = parent;

	// create background
   i = 0;
	XtSetArg (args[i], XmNorientation, XmVERTICAL); i++;
	XtSetArg (args[i], XmNentryAlignment, XmALIGNMENT_CENTER); i++;
   rcGround = XmCreateRowColumn (father, "rcGround", args, i);

	// create label widget
	i = 0;
	cadena = XmStringCreateLocalized ("        Emissions List        ");
   XtSetArg (args[i], XmNlabelString, cadena); i++;
	if (Fonts != NULL)
	   XtSetArg (args[i], XmNfontList, Fonts); i++;
	lbEmissions = XmCreateLabel(rcGround, "lbEmissions", args, i);
   XmStringFree (cadena);
	XtManageChild(lbEmissions);
	
	// Create list widget
	i = 0;
	XtSetArg (args[i], XmNlistSizePolicy, XmCONSTANT); i++;
	XtSetArg (args[i], XmNselectionPolicy, XmSINGLE_SELECT); i++;
	XtSetArg (args[i], XmNvisibleItemCount, 7); i++;
	if (Fonts != NULL)	   {XtSetArg (args[i], XmNfontList, Fonts); i++;}
	XtSetArg (args[i], XmNscrollBarDisplayPolicy, XmSTATIC); i++;
	lstList = XmCreateScrolledList(rcGround, "lstList", args, i);
	XtAddCallback (lstList, XmNsingleSelectionCallback, ChangeSelectCB, (XtPointer) this);
	XtManageChild(lstList);
	
	// Create background for buttons
	i = 0;
	XtSetArg (args[i], XmNfractionBase, 2); i++;
	frmButtons = XmCreateForm(rcGround, "frmButtons", args, i);
	// Create Buttons
	// ******** Button for search changes *************
	// Botón para realizar cambios en la búsqueda de emisiones según 
	// los criterios del usuario
	i = 0;
    cadena = XmStringCreateLocalized ("Change Search");
    XtSetArg (args[i], XmNlabelString, cadena); i++;
	if (Fonts != NULL)
	   XtSetArg (args[i], XmNfontList, Fonts); i++;
	btChangeSearch = XmCreatePushButton(frmButtons, "btChangeSearch", args, i);
   XmStringFree (cadena);
  // XtAddCallback (btChangeSearch, XmNactivateCallback, pulsa_changeSearch, NULL);
	 XtAddCallback (btChangeSearch, XmNactivateCallback, pulsa_changeSearch, (XtPointer)this);
	XtManageChild(btChangeSearch);
	// Toggle button for AutoSort
   cadena = XmStringCreateLocalized ("AutoSort");
   i = 0;
   XtSetArg (args[i], XmNset, true); i++;
   XtSetArg (args[i], XmNlabelString, cadena); i++;
	if (Fonts != NULL)
	   XtSetArg (args[i], XmNfontList, Fonts); i++;
	chkAutoSort = XmCreateToggleButton (frmButtons, "chkAutoSort", args, i);
	XtAddCallback (chkAutoSort, XmNvalueChangedCallback, ChangeSortCB, (XtPointer) this);
   XmStringFree (cadena);
	XtManageChild(chkAutoSort);


   XtVaSetValues (btChangeSearch,
		XmNleftAttachment, XmATTACH_FORM,
		XmNtopAttachment, XmATTACH_FORM,
		XmNrightAttachment, XmATTACH_POSITION, XmNrightPosition, 1,
		XmNrightOffset, 5,
		XmNbottomAttachment, XmATTACH_FORM,
		NULL);
   XtVaSetValues (chkAutoSort,
		XmNleftAttachment, XmATTACH_POSITION, XmNleftPosition, 1,
		XmNtopAttachment, XmATTACH_FORM,
		XmNrightAttachment, XmATTACH_FORM,
		XmNleftOffset, 5,
		XmNbottomAttachment, XmATTACH_FORM,
		NULL);

	XtManageChild(frmButtons);	
	XtManageChild(rcGround);
	
	// Place the widgets
   XtVaSetValues (rcGround,
		XmNtopAttachment, XmATTACH_FORM, XmNtopOffset, 5,
		XmNleftAttachment, XmATTACH_FORM, XmNleftOffset, 5,
		XmNrightAttachment, XmATTACH_FORM, XmNrightOffset, 5,
		XmNbottomAttachment, XmATTACH_FORM, XmNbottomOffset, 5,
		NULL);

	XmProcessTraversal(lstList, XmTRAVERSE_CURRENT);
	}


//*************** CHANGE SELECTED EMISSION ***************
void cVisIdentify::ChangeSelectedEmission(int newPos) 
	{
	printf("__________________ CHANGE SELECTED EMISSION _________________\n");
	//NOTA: Hay que cogerla en función de la lista, no del vector, pues el vector se
		//	modifica con cada búsqueda en el caso de que haya más de una!!
	//NOTA: La posición es correcta, pero si hemos tomado un valor antes de lista
	//		con padres y/o hijos naturales, se conservan para la siguiente si ella
	//		no los tiene!
	//selectedEmission = emisionesOrdenadas[newPos];
	tListEmissions *list = (tListEmissions *) listEmissions;
	printf("El número de elementos que supone en esta lista son: %d\n", list->numElements);
	if (newPos > list->numElements) return;

	// With emissions list sorted, position selected is equal to position in
	// internal list
	int i = 1;
	NodeList **guide = &(list->first);
	while (((*guide) != NULL)&&(i < newPos)) 
		{
		// pass to next element
		guide = &((*guide)->next);
		i++;
		}
	
	selectedEmission = (*guide)->value;
	printf("Emisión seleccionada: %d\n", selectedEmission);
	if (!XmListPosSelected(lstList, newPos))	XmListSelectPos(lstList, newPos, false);
	}


/*  ****************************
	**   Management Methods   **
	****************************/
//*************** SHOW CANDIDATES CALIBRACIÓN *****************************
/** Recorre la base de datos LARA en busca de elementos candidatos para la
	energía del pico
	Durante la calibración, para los elementos naturales se toman sólo los hijos
	de los ascendentes.
	Devuelve por tanto los elementos naturales y no naturales, 
	pero no los ascendentes últimos
*/
void cVisIdentify::ShowCandidatesCalibracion(int numInterval , int numPeak)
	{
	printf("Iniciamos candidatos para pico: %d - %d\n", numInterval, numPeak);
	if (!LaraBD.GetIsRunning()) 
		{
		printf("No database available\n");
		return; // No DB avalaible, no action
		}

	int pico=numPeak;
	int intervalo=numInterval;
	bool NewList; int newPos;

	if ((numInterval == 0)||(numPeak == 0)) 
		{
		numInterval = nInterval; numPeak = nPeak;
		NewList = false;
		}
	else 	
		{
		nInterval = numInterval; nPeak = numPeak;
		NewList = true;
		}

	//Cogemos el intervalo de picos y dentro de ese intervalo el pico a analizar
	NodoPicos *interval = DevuelveNodo(*peakList, numInterval);
	NodoPico *peak = DevuelveNodo(interval->picos, numPeak);
	
	// Calculate possible emissions candidates
	cent = Datos.DaEnergia(peak->Ajuste.Centroide);
	//NOTA: Aunque este intervalo de anchura es en principio el bueno, habría
	//		que comprobar que no es demasiado restrictivo durante las validaciones
	wide = Datos.DaFWHG(peak->Ajuste.Centroide)*sqrt(interval->ChiSq);
	if (wide < 1.5) wide = 1.5;
	
	//Buscamos en la base de datos elementos con emisiones en el rango del centroide
	//1)Buscamos en primer lugar los emisores estables naturales en sus listas de 
	//descendientes, sin contar el ascendiente último (lo llamaremos padre).
	founds =   LaraBD.SearchEmissions(cent-wide, cent+wide,-1,-1,-1,-1,-1,-1,-1,false,false,true,true,"", true, false);
	printf("Terminada búsqueda de naturales con %d encontrados\n", founds.GetNumItemsFound());
	//2)Y luego buscamos en los no naturales
	foundsNoNat = LaraBD.SearchEmissions(cent-wide, cent+wide,-1,-1,-1,-1,-1,-1,-1,false,false,true,false,"", true, false);
	printf("Terminada búsqueda de resto con %d encontrados\n", foundsNoNat.GetNumItemsFound());
	//TFileEmissionLaraBD *tfile=LaraBD.GetEmissions();	
	//foundsNoNat = foundsNoNat.deleteRepeatedEmissions(founds, tfile); //Quitamos las emisiones que sean debidas a naturales
	//foundsTotal = foundsNoNat + founds; //Sumamos sólo las de los naturales primigenios (U, Th, Ra)
	foundsTotal=foundsNoNat+founds;

	//NOTA: Para la calibración se sigue usando el método de ordenación cutre!!!
	// clean previous results
	tListEmissions *list = (tListEmissions *) listEmissions;
	DelList(*list);
	// add new results
	// Metemos los resultados de la búsqueda en una lista de emisiones
	int numEmis = foundsTotal.GetFirstResult();
	while (numEmis != -1) 
		{
		NodeList *newNode = new NodeList;
		if (newNode == NULL) break; // memory failure
		newNode->value = numEmis;
		newNode->score = GetScore(numEmis, numInterval, numPeak);
		AddEmissionToList(*list, newNode);
		numEmis = foundsTotal.GetNextResult();
		}
	printf("Emisiones añadidas a la lista\n");
	
	// With emissions list sorted, make entries in list widget
	XmString entry;
	XmListDeleteAllItems(lstList); // clean list widget
	int i = 1;
	if(foundsTotal.GetNumItemsFound()>0)
		{
		NodeList **guide = &(list->first);
		while ((*guide) != NULL) 
			{
			// make an entry list widget
			entry = GetEntryListXmString((*guide)->value);
			XmListAddItemUnselected(lstList, entry, 0);
		
			XmStringFree(entry);
		
			// search selectedEmission
			if (!NewList)	if ((*guide)->value == selectedEmission)	newPos = i;
		
			// pass to next element
			guide = &((*guide)->next);
			i++;
			}
	
		// if there is a new list, put first position as selected
		if (NewList) 
			{
			XmListSelectPos(lstList, 1, false);
			selectedEmission = list->first->value;
			}
		else	XmListSelectPos(lstList, newPos, false);
		}//del if(elementos encontrados)
	else	//warning
			Aviso(app_shell, "There is no matching emission");
	}


//*************** SHOW CANDIDATES *****************************
/** Recorre la base de datos LARA en busca de elementos candidatos para la
	energía del pico
	Devuelve todos los elementos, naturales y no naturales, con todos los 
	descendientes naturales.
	
	Los candidatos se ordenan de la siguiente manera:
		1)Padres naturales (Th230 y 232, U234,235 y 238 y Ra226)
		2)Descendientes naturales
		3)Resto de elementos
	Dentro de cada grupo, los elementos se ordenan por cercanía al centroide y
	NOTA: por probabilidad (falta)
	*/
// NOTA: Comentada entera para rehacerla contando un tercer grupo que serán
//	los descendientes de los naturales, y con una ordenación un tanto distina
void cVisIdentify::ShowCandidates(int numInterval , int numPeak)
	{
	printf("SHOW CANDIDATES\n");
	printf("Iniciamos candidatos para pico: %d - %d\n", numInterval, numPeak);
	if (!LaraBD.GetIsRunning()) return; // No DB avalaible, no action
	bool NewList; int newPos;

	if ((numInterval == 0)||(numPeak == 0)) 
		{
		numInterval = nInterval; numPeak = nPeak;
		NewList = false;
		}
	else 	
		{
		nInterval = numInterval; nPeak = numPeak;
		NewList = true;
		}

	//Cogemos el intervalo de picos y dentro de ese intervalo el pico a analizar
	NodoPicos *interval = DevuelveNodo(*peakList, numInterval);
	NodoPico *peak = DevuelveNodo(interval->picos, numPeak);
	
	//NOTA: No se debería usar la chi² para determinar la sigma del centroide!
	// Calculate possible emissions candidates
	cent = Datos.DaEnergia(peak->Ajuste.Centroide);
	//NOTA: Aunque este intervalo de anchura es en principio el bueno, habría
	//		que comprobar que no es demasiado restrictivo durante las validaciones
	wide = Datos.DaFWHG(peak->Ajuste.Centroide)*sqrt(interval->ChiSq);
	if (wide < 1.5) wide = 1.5;
	
	// make the search
	//printf("Buscamos las emisiones\n");
	//Buscamos en la base de datos elementos con emisiones en el rango del centroide
	//1)
	//Buscamos en primer lugar los emisores estables naturales en sus listas de descendientes
	//Emisiones del radio 226, torio 230 232 y uranio 234 235 238
	/*
	founds = LaraBD.SearchEmissions(cent-wide, cent+wide,-1,-1,-1,88,88,226,226,false,false,true,true,"", true, true);
	founds = founds + LaraBD.SearchEmissions(cent-wide, cent+wide,-1,-1,-1,90,90,232,232,false,false,true,true,"", true, true);
	founds = founds + LaraBD.SearchEmissions(cent-wide, cent+wide,-1,-1,-1,92,92,235,235,false,false,true,true,"", true, true);
	founds = founds + LaraBD.SearchEmissions(cent-wide, cent+wide,-1,-1,-1,92,92,238,238,false,false,true,true,"", true, true);
	founds = founds + LaraBD.SearchEmissions(cent-wide, cent+wide,-1,-1,-1,92,92,234,234,false,false,true,true,"");
	founds = founds + LaraBD.SearchEmissions(cent-wide, cent+wide,-1,-1,-1,90,90,230,230,false,false,true,true,"");
	*/
	//Ahora queremos poner justo detrás de cada emisor estable natural las emisiones que 
	//		coincidan en energía y sean de alguno de sus descendientes
	/*
	founds = LaraBD.SearchEmissionsAndDescendants(cent-wide, cent+wide,-1,-1,-1,88,88,226,226,false,false,true,true,"", true, true);
	founds = founds + LaraBD.SearchEmissionsAndDescendants(cent-wide, cent+wide,-1,-1,-1,90,90,232,232,false,false,true,true,"", true, true);
	founds = founds + LaraBD.SearchEmissionsAndDescendants(cent-wide, cent+wide,-1,-1,-1,92,92,235,235,false,false,true,true,"", true, true);
	founds = founds + LaraBD.SearchEmissionsAndDescendants(cent-wide, cent+wide,-1,-1,-1,92,92,238,238,false,false,true,true,"", true, true);
	founds = founds + LaraBD.SearchEmissionsAndDescendants(cent-wide, cent+wide,-1,-1,-1,92,92,234,234,false,false,true,true,"");
	founds = founds + LaraBD.SearchEmissionsAndDescendants(cent-wide, cent+wide,-1,-1,-1,90,90,230,230,false,false,true,true,"");
	*/
	printf("Energías en [%f,%f]\n", cent-wide, cent+wide);
	founds = LaraBD.SearchEmissionsAndDescendants(cent-wide, cent+wide,-1,-1,-1,88,88,226,226,false,false,true,true,"", false, false);
	founds = founds + LaraBD.SearchEmissionsAndDescendants(cent-wide, cent+wide,-1,-1,-1,90,90,232,232,false,false,true,true,"", false, false);
	founds = founds + LaraBD.SearchEmissionsAndDescendants(cent-wide, cent+wide,-1,-1,-1,92,92,235,235,false,false,true,true,"", false, false);
	founds = founds + LaraBD.SearchEmissionsAndDescendants(cent-wide, cent+wide,-1,-1,-1,92,92,238,238,false,false,true,true,"", false, false);
	founds = founds + LaraBD.SearchEmissionsAndDescendants(cent-wide, cent+wide,-1,-1,-1,92,92,234,234,false,false,true,true,"");
	founds = founds + LaraBD.SearchEmissionsAndDescendants(cent-wide, cent+wide,-1,-1,-1,90,90,230,230,false,false,true,true,"");
	
	//Borramos la lista de llamadas anteriores y añadimos los padres naturales
	NewList=true;
	XmListDeleteAllItems(lstList); // clean list widget
	printf("encontrados %d padres+descendientes\n", founds.GetNumItemsFound());
	addLista(founds, NewList, numInterval, numPeak);
	if(founds.GetNumItemsFound()>0)	NewList=false;
	//1.2)NOTA: Deberíamos añadir el resto de entradas naturales que no están en equilibrio
	/*founds = LaraBD.SearchEmissions(cent-wide, cent+wide,-1,-1,-1,88,88,226,226,false,false,true,true,"", true, false);
	founds = founds + LaraBD.SearchEmissions(cent-wide, cent+wide,-1,-1,-1,90,90,232,232,false,false,true,true,"", true, false);
	founds = founds + LaraBD.SearchEmissions(cent-wide, cent+wide,-1,-1,-1,92,92,235,235,false,false,true,true,"", true, false);
	founds = founds + LaraBD.SearchEmissions(cent-wide, cent+wide,-1,-1,-1,92,92,238,238,false,false,true,true,"", true, false);
	founds = founds + LaraBD.SearchEmissions(cent-wide, cent+wide,-1,-1,-1,92,92,234,234,false,false,true,true,"");
	founds = founds + LaraBD.SearchEmissions(cent-wide, cent+wide,-1,-1,-1,90,90,230,230,false,false,true,true,"");
	addLista(founds, NewList, numInterval, numPeak);
	if(founds.GetNumItemsFound()>0)	NewList=false;
	*///-------------------------	
	
	//2) Añadimos las emisiones que no son debidas a los 6 naturales padres, es
	//		decir, la del resto de los naturales
	//printf("Añadimos los hijos\n");
	foundsDesc = LaraBD.SearchEmissions(cent-wide, cent+wide,-1,-1,-1,-1,-1,-1,-1,false,false,true,true,"", false,false);
	cSearchLaraBD foundsPyA = LaraBD.SearchEmissions(cent-wide, cent+wide,-1,-1,-1,-1,-1,-1,-1,false,false,true,true,"", true,true);
	cSearchLaraBD foundsDescNet = foundsDesc - foundsPyA;
	//printf("encontrados %d hijos\n", foundsDescNet.GetNumItemsFound());
	//addLista(foundsDesc - foundsPyA, NewList, numInterval, numPeak);
	addLista(foundsDesc - foundsPyA - founds, NewList, numInterval, numPeak);
	if(foundsDescNet.GetNumItemsFound()>0)	NewList=false;
		
	//3)
	// Ahora añadimos todas las demás emisiones que no son de naturales
	// Se priorizarán en menor medida
	foundsNoNat = LaraBD.SearchEmissions(cent-wide, cent+wide,-1,-1,-1,-1,-1,-1,-1,false,false,false,false,"", false,false);
	//printf("encontrados %d no naturales\n", foundsNoNat.GetNumItemsFound());
	if(foundsNoNat.GetNumItemsFound()>0)	addLista(foundsNoNat-foundsDesc, NewList, numInterval, numPeak);
	}

//************** ADD LISTA ************************
/*	Añade, ordenados por distancia al centroide, al resto de elementos.
	*/
void cVisIdentify::addLista(cSearchLaraBD encontrados, bool NewList, int numInterval, int numPeak)
	{
//	printf("ADD LISTA, NewList=%d\n", NewList);
	int newPos;
	// Metemos los resultados de la búsqueda en una lista de emisiones para 
	// su ordenación. Este es un vector temporal, luego se añadirán a una lista
	// enlazada persistente
	int numFounds=encontrados.GetNumItemsFound();
	int emisiones[numFounds];
	int	emisionesOrdenadas[numFounds];
	float distancias[numFounds];
	float probabilidades[numFounds];
	
	for(int i=0;i<numFounds;i++)
		{
		emisiones[i]=emisionesOrdenadas[i]=0;
		distancias[i]=probabilidades[i]=0.0;
		}
	
	//1.- Tomamos todas la emisiones
	NodoPicos *interval = DevuelveNodo(*peakList, numInterval);
	NodoPico *peak = DevuelveNodo(interval->picos, numPeak);
	
//		printf("inicializamos emisiones y distancias\n");
	if(numFounds>0)
		{
		emisiones[0] = encontrados.GetFirstResult();
		distancias[0]= fabs(Datos.DaEnergia(peak->Ajuste.Centroide)-LaraBD.GetEnergy(emisiones[0]));
		}
		
	for(int i=1;i<numFounds; i++)
		{
		emisiones[i]=encontrados.GetNextResult();
		distancias[i]=fabs(Datos.DaEnergia(peak->Ajuste.Centroide)-LaraBD.GetEnergy(emisiones[i]));
		}

//		printf("Creamos la lista ordenadon por cercanía la centroide\n");
	//2.- Creamos la lista ordenando por cercanía al centroide
	//NOTA: Faltaría ordenar los que están a la misma distancia por orden de probabilidad
	float distMinima=10.0;
	int emisOptima=0;
	for(int i=0;i<numFounds;i++)
		{
		for(int j=0;j<numFounds;j++)
			{
			//printf("Distancia mínima: %f\tDistancia[%d]=%f\n", distMinima, distancias[j]);
			if(distancias[j]<distMinima)
				{
				distMinima=distancias[j];
				emisOptima=j;
				}
			}
		emisionesOrdenadas[i]=emisiones[emisOptima];
		distancias[emisOptima]=100.0;//Para que no la vuelva a coger
		distMinima=10.0;//Para que no sea la menor de nuevo
		}
//	printf("Segunda vuelta ordenando por prob\n");
	//2b.-Hacemos una segunda vuelta para ordenar por probabilidad los que 
	//	  tengan la misma distancia al centroide
	//Volvemos a coger las distancias, y también las probabilidades
	for(int i=0;i<numFounds;i++)
		{
		distancias[i]     = fabs(Datos.DaEnergia(peak->Ajuste.Centroide)-LaraBD.GetEnergy(emisionesOrdenadas[i]));
		probabilidades[i] = LaraBD.GetProbability(emisionesOrdenadas[i]);
		}
		
//		printf("empezamos a ordenar, %d\n", numFounds);
	for(int i=0;i<numFounds-1;i++)
		{
		int j=1;
//		printf("i es %d, comparamos con %d\n", i,i+j);
		while(distancias[i]==distancias[i+j])
			{//Hay que ordenar por probabilidad
//			printf("misma distancia\n");
			if(probabilidades[i]<probabilidades[i+j])
				{//Hay que cambiar su orden
//				printf("Cambiamos el orden de %d y %d\n", i+j,i);
				int temp=emisionesOrdenadas[i+j];
				emisionesOrdenadas[i+j]=emisionesOrdenadas[i];
				emisionesOrdenadas[i]=temp;
//				printf("Alternamos también probs\n");
				//
				float t=probabilidades[i+j];
				probabilidades[i+j]=probabilidades[i];
				probabilidades[i]=t;
				//
				}
			//Si las probabilidades son iguales, ordenamos por número másico
			else if(probabilidades[i]==probabilidades[i+j])
					{
					//printf("misma probabilidad\n");
					/*printf("%d\n", i);
					printf("%f\n", LaraBD.GetMassNumber(LaraBD.GetEmissor(emisionesOrdenadas[i])) );
					printf("%d\n", i+j);
					printf("%f\n", LaraBD.GetMassNumber(LaraBD.GetEmissor(emisionesOrdenadas[i+j])) );
					*/if(LaraBD.GetMassNumber(LaraBD.GetEmissor(emisionesOrdenadas[i])) < LaraBD.GetMassNumber(LaraBD.GetEmissor(emisionesOrdenadas[i+j])) )
						{
					//	printf("1\n");
						int temp=emisionesOrdenadas[i+j];
					//	printf("2\n");
						emisionesOrdenadas[i+j]=emisionesOrdenadas[i];
					//	printf("3\n");
						emisionesOrdenadas[i]=temp;
					//	printf("4\n");
						}
					}
			j++;
			}//si distancias iguales
		}
		
//	printf("construimos ahora la lista enlazada\n");
	//3.- Con las emisiones ordenadas ahora podemos construir la lista enlazada
	// With emissions list sorted, make entries in list widget
	XmString entry;
	if(encontrados.GetNumItemsFound()>0)
		{
		for(int i=0;i<numFounds;i++)
			{
			// make an entry list widget
			entry = GetEntryListXmString(emisionesOrdenadas[i]);
			XmListAddItemUnselected(lstList, entry, 0);
			// ---- search selectedEmission
			if (!NewList)	if (emisionesOrdenadas[i] == selectedEmission)	newPos = i;
			//---------
			XmStringFree(entry);
			}

		// ---- if there is a new list, put first position as selected
		if (NewList) 
			{
			XmListSelectPos(lstList, 1, false);
			selectedEmission = emisionesOrdenadas[0];
			//selectedEmission = list->first->value;
			}
		else	XmListSelectPos(lstList, newPos, false);

		//Añadimos a la lista enlazada persistente la información de los vectores
		// clean previous results
		tListEmissions *list = (tListEmissions *) listEmissions;
		if(NewList) //NOTA<=-----No se puede quitar este tal cual!!!!		
			{
			DelList(*list);
			DelList(*(tListEmissions*)listEmissions);
			//listEmissions=NULL;
			}
		for(int i=0;i<numFounds;i++)
			{
			NodeList *newNode = new NodeList;
			if (newNode == NULL) break; // memory failure
			newNode->value = emisionesOrdenadas[i];
			AddLastEmissionToList(*list, newNode);
			}
		}
//	printf("Fin\n");
	}
	
//***************** SHOW CANDIDATES ADV ********************
/* Versión extendida de la anterior, que modifica todos los parámetros,
   La versión inicial hace la búsqueda por defecto sobre los elementos
   naturales U, Th y Ra con mayor prioridad y luego sobre el resto
   NOTA: En caso de elegir elementos que puedan tener listas de descendientes
	(que son los que se buscan en la búsqueda por defecto) en este tipo de
	búsqueda sólo se dará información sobre sus descendientes directos
	*/
void cVisIdentify::ShowCandidatesAdv(int amin, int amax, float pmin, float pmax, 
								  float emin, float emax, int prmin, int prmax,
								  bool buscaNatu, bool natu, bool buscaMeta, bool meta,
								  int tipoEmision, char *nombre)
	{
	printf("Energías: [%f,%f]\n", emin, emax);
	if (!LaraBD.GetIsRunning()) 
		{
		printf("ERROR: No DataBase available\n");
		return; // No DB avalaible, no action
		}
	
	bool NewList; int newPos;
	//Buscamos en la base de datos elementos con emisiones según lo estipulado en la 
	//búsqueda avanzada
	founds = LaraBD.SearchEmissions(emin, emax,pmin,pmax,tipoEmision,amin,amax,prmin,prmax,buscaMeta,meta,buscaNatu,natu,nombre, false);
	
	// clean previous results
	tListEmissions *list = (tListEmissions *) listEmissions;
	DelList(*list);
	// add new results
	// Metemos los resultados de la búsqueda en una lista de emisiones
	int numEmis = founds.GetFirstResult();
	while (numEmis != -1) 
		{
		NodeList *newNode = new NodeList;
		if (newNode == NULL) break; // memory failure
		newNode->value = numEmis;
		newNode->score = GetScore(numEmis, intervalo, pico);
		AddEmissionToList(*list, newNode);
		numEmis = founds.GetNextResult();
		}
	
	// With emissions list sorted, make entries in list widget
	XmString entry;
	if(lstList!=NULL)		XmListDeleteAllItems(lstList); // clean list widget
	else					return;
		
	int i = 1;
	printf("Número de elementos encontrados: %d\n", founds.GetNumItemsFound() );
	if(founds.GetNumItemsFound()>0)
		{
		NodeList **guide = &(list->first);
		while ((*guide) != NULL)
			{
			// make an entry list widget
			entry = GetEntryListXmString((*guide)->value);
			XmListAddItemUnselected(lstList, entry, 0);
		
			XmStringFree(entry);
		
			// search selectedEmission
			if (!NewList)	if ((*guide)->value == selectedEmission)	newPos = i;
		
			// pass to next element
			guide = &((*guide)->next);
			i++;
			}
	
		// if there is a new list, put first position as selected
		if (NewList) 
			{
			XmListSelectPos(lstList, 1, false);
			selectedEmission = list->first->value;
			}
		else	XmListSelectPos(lstList, newPos, false);
		}//del if(elementos encontrados)
	else	//warning
			Aviso(app_shell, "There is no matching emission");
	}

//************** GET SCORE *******************
/* Devuelve un valor de prioridad de la emisión en 
	función a distintos parámetros:
	- Lo cerca que se encuentre la energía de la emisión de la energía del pico -(distancia al centroide)*150%
	- La probabilidad de emisión +sqrt(probabilidad)%
	- Si el emisor es natural	+200%
	- Si el elemento emisor ya ha aparecido en este pico +200%
	NOTA: Nos quedamos aquí: habría que añadir para que le diera el máximo de prioridad
		a los que vengan de las primeras búsquedas en listas de descendientes
	*/
double cVisIdentify::GetScore(unsigned short int emission, int numInterval, int numPeak) 
	{
	NodoPicos *interval = DevuelveNodo(*peakList, numInterval);
	NodoPico *peak = DevuelveNodo(interval->picos, numPeak);
	double acum = 0.0;
	
//	printf("Hemos tomado pico e intervalo\n");
	if (XmToggleButtonGetState(chkAutoSort)) 
		{
		int element = LaraBD.GetEmissor(emission);
		//printf("Pillado elemento emisor (%d) de la emisión (%d)\n", element, emission);
		//NOTA: Revisar esta expresión para priorizar porque quizá da demasiada
		//	importancia a la distancia del centroide (*1.5) y demasiada poca a su probabilidad
		//  de emisión (sqrt). Durante las validaciones se verá su correctitud
		
		
		// Score for emission probability
		//acum = sqrt(LaraBD.GetProbability(emission));
		acum = 100*(LaraBD.GetProbability(emission));
		// Score for "farness" of centroid
		acum -= fabs(Datos.DaEnergia(peak->Ajuste.Centroide) - LaraBD.GetEnergy(emission))*1.5;
		
	//	printf("Ponderando naturales\n");
		int na=LaraBD.GetAtomicNumber(element);
		int np=LaraBD.GetMassNumber(element);
	//	printf("na=%d, np=%d\n", na, np);
		if( (na== 88 && np == 226)   ||(na== 90 && np == 230)
			||(na== 90 && np == 232) ||(na== 92 && np == 234)
			||(na== 92 && np == 235) ||(na== 92 && np == 238) )		
			{ 
			if (acum<=0)	acum=0.01;
			acum*=10000.0; 
//			printf("Un natural: (%d, %d), máxima prioridad\n", na, np);	
			} 
		/*
		if (LaraBD.IsNatural(element))		{ acum *= 2.0; printf("+200\t"); }// 200% of score for natural
		//NOTA: Mirar en todos los intervalos
		if (IsElementInList(element, numInterval, numPeak)) 
			{
			acum *= 2.0; // 200% of score if there are more emissions of this peak
			//printf("ya encontrada emision: %d %d\n", LaraBD.GetAtomicNumber(element), LaraBD.GetMassNumber(element));
			printf("+200\t");	
			}
		*/
		if (acum < 0) {/*printf("Acumulador negativo=%f\n");*/ acum = 0.0;}
		
/*		char *nombre=new char[10];
		LaraBD.GetNameElement(element, nombre);
		printf("\nEmisión de %s %d acumulador de %f % \n", nombre, LaraBD.GetMassNumber(element), acum);
	*/	}
	else
		acum =-(double) emission;
	
	return acum;
	}

//************* GET ENTRY LIST XM STRING *********************
/* Genera una cadena con información de la emisión:
	elemento[(e)]	energía(incertidumbre) keV probabilidad%(incertidumbre)
*/
/*
XmString cVisIdentify::GetEntryListXmString(unsigned short int emission) 
	{
	XmString entry, entry2, final;
	char entry_text[100], name[3], format[50];
	
	int element = LaraBD.GetEmissor(emission);
	final = XmStringNameElement(element, LittleName);
	strcpy(format, " -> %.2f (%.2f)keV %.1f%% (%.2f)");
	double Prob = LaraBD.GetProbability(emission);
	int rateProb = orden(Prob)-1;
	if ((Prob > 0)&&((rateProb < -1)&&(rateProb > -10)))
		format[21] = (unsigned char) 0x30 + abs(rateProb);
	else 
		format[21]= '1';
	
	sprintf(entry_text, format, LaraBD.GetEnergy(emission), 
				LaraBD.GetIncEnergy(emission), Prob, LaraBD.GetIncProbability(emission));
	
	entry2 = XmStringCreateLocalized (entry_text);
	entry = XmStringConcat(final, entry2);
	XmStringFree(final); XmStringFree(entry2);
	
	return entry;
	}
*/
XmString cVisIdentify::GetEntryListXmString(unsigned short int emission) 
	{
	XmString entry, entry2, final;
	char entry_text[100], name[3], format[50];
	char cadena1[80];
	char cadena2[80];		
	
	int element = LaraBD.GetEmissor(emission);
	final = XmStringNameElement(element, LittleName);
//	final = XmStringNameElementSup(element, LittleName);
	strcpy(format, " -> %s keV %s%%");
	double Prob = LaraBD.GetProbability(emission);
	int rateProb = orden(Prob)-1;
	if ((Prob > 0)&&((rateProb < -1)&&(rateProb > -10)))
			format[21] = (unsigned char) 0x30 + abs(rateProb);
	else 	format[21]= '1';
	
	PresentaNumero(LaraBD.GetEnergy(emission), LaraBD.GetIncEnergy(emission), cadena1);
	PresentaNumero(Prob, LaraBD.GetIncProbability(emission), cadena2);
	sprintf(entry_text, format, cadena1, cadena2);
	
	entry2 = XmStringCreateLocalized (entry_text);
	entry = XmStringConcat(final, entry2);
	XmStringFree(final); XmStringFree(entry2);
	
	return entry;
	}

/************** GET ENTRY LIST XMSTRING SUP *******************************
	Crea una entrada en la lista con formato y estilo para los superíndices
	La salida es igual a la de GetEntryListXmString(), pero en XTextProperty
**********************************************************************/
/*XTextProperty cVisIdentify::GetEntryListXmStringSup(unsigned short int emission) 
	{
	XmString entry, entry2, final;
	char entry_text[100], name[3], format[50];
	char cadena1[80];
	char cadena2[80];		
	
	int element = LaraBD.GetEmissor(emission);
	final = XmStringNameElement(element, LittleName);
	strcpy(format, " -> %s keV %s%%");
	double Prob = LaraBD.GetProbability(emission);
	int rateProb = orden(Prob)-1;
	if ((Prob > 0)&&((rateProb < -1)&&(rateProb > -10)))
			format[21] = (unsigned char) 0x30 + abs(rateProb);
	else 	format[21]= '1';
	
	PresentaNumero(LaraBD.GetEnergy(emission), LaraBD.GetIncEnergy(emission), cadena1);
	PresentaNumero(Prob, LaraBD.GetIncProbability(emission), cadena2);
	sprintf(entry_text, format, cadena1, cadena2);
	
	entry2 = XmStringCreateLocalized (entry_text);
	entry = XmStringConcat(final, entry2);
	XmStringFree(final); XmStringFree(entry2);
	
	return entry;
	}*/
//******************* IS ELEMENT IN LIST **********************
/* Nos da verdadero si el elemento element está en 
*/
bool cVisIdentify::IsElementInList(unsigned short int element, int numInterval, int numPeak) 
	{
	if (!LaraBD.GetIsRunning()) return false; // No DB, No information avalaible

	// "travel" through the list search emission identified
	NodoPicos **guide = &(peakList->inicio);
	int nInterval = 1;
	while ((*guide) != NULL) 
		{
		// travel through internal peak list
		NodoPico **guide2 = &((*guide)->picos.inicio);
		int nPeak = 1;
		while ((*guide2) != NULL) 
			{
			if (((numInterval != nInterval)||(numPeak != nPeak))&& 
				((*guide2)->Ajuste.identificado))
				{
				//if (LaraBD.GetEmissor((*guide2)->Ajuste.codElemento) == element)
				//	return true;
				for(int i=0;i<(*guide2)->Ajuste.emisiones->numElements();i++)
					{
					NodoEmision *ne=(NodoEmision *)((*guide2)->Ajuste.emisiones->getNode(i));
					if (LaraBD.GetEmissor(ne->codElemento) == element)
						return true;
					}					
				}
			guide2 = &((*guide2)->siguiente);
			nPeak++;
			}
		guide = &((*guide)->siguiente);
		nInterval++;
		}

	return false;
	}

bool cVisIdentify::IsEmissionInList(unsigned short int emission, int &numInterval, int &numPeak) 
	{
	if (!LaraBD.GetIsRunning()) return false; // No DB, No information avalaible

	// "travel" through the list search emission identified
	NodoPicos **guide = &(peakList->inicio);
	int nInterval = 1;
	while ((*guide) != NULL) 
		{
		// travel through internal peak list
		NodoPico **guide2 = &((*guide)->picos.inicio);
		int nPeak = 1;
		while ((*guide2) != NULL) 
			{
			if ((*guide2)->Ajuste.identificado)
				{
				for(int i=0;i<(*guide2)->Ajuste.emisiones->numElements();i++)
					{
					NodoEmision *ne=(NodoEmision *)((*guide2)->Ajuste.emisiones->getNode(i));
					if ( ne->codElemento == emission)
						{
						numInterval = nInterval; numPeak = nPeak;
						return true;
						}
					}					
				}
			guide2 = &((*guide2)->siguiente);
			nPeak++;
			}
		guide = &((*guide)->siguiente);
		nInterval++;
		}

	return false;
	}

//****************** MUESTRA IDENTIFICADOS *************
/*	Función de depuración que muestra todos los elementos identificados
*/	
void cVisIdentify::MuestraIdentificados() 
	{
	// "travel" through the list search emission identified
	NodoPicos **guide = &(peakList->inicio);
	while ((*guide) != NULL) 
		{
		// travel through internal peak list
		NodoPico **guide2 = &((*guide)->picos.inicio);
		while ((*guide2) != NULL) 
			{
			if ((*guide2)->Ajuste.identificado) 
				{
				for(int i=0;i<(*guide2)->Ajuste.emisiones->numElements();i++)
					{
					NodoEmision *ne=(NodoEmision *)((*guide2)->Ajuste.emisiones->getNode(i));
					unsigned short int emision = ne->codElemento;
					unsigned short int element = LaraBD.GetEmissor(emision);
					}					
				}
			guide2 = &((*guide2)->siguiente);
			}
		guide = &((*guide)->siguiente);
		}
	}

//******************* PULSA CHANGE SEARCH *****************************
/**
Se activa cuando se pulsa el botón addSearch de edición de intervalos,
mostrándonos todas las opciones avanzadas de búsqueda de forma que el 
usuario pueda adaptar a sus necesidades la búsqueda de posibles emisores
*/
void pulsa_changeSearch(Widget widget, XtPointer client_data, XtPointer call_data) 
	{
	// una vez identificado el nodo se lanza el cuadro de dialogo
	if (!dlg_changeSearch) 	
		{
		//createDialogSearch();
	//   	XmListCallbackStruct *cbs = (XmListCallbackStruct *) call_data;
		cVisIdentify *object = (cVisIdentify *) client_data;

		createDialogSearch(object);
		}

	//Le ponemos los valores que tenemos por defecto
	char texto[40];
	sprintf(texto, "%.2f", cent-wide);
	XmTextSetString(txt_emin, texto);

	sprintf(texto, "%.2f", cent+wide);
	XmTextSetString(txt_emax, texto);
	
	XtManageChild(dlg_changeSearch);
	}
	
	/****************************************************
	****************** CREATE DIALOG SEARCH *************
	****************************************************/
	
void createDialogSearch(cVisIdentify *object)
	{
	Arg		args[7];
   	Cardinal	i = 0;
	XmString cadena;
	
	// *********************
	// * cuadro de dialogo *
	// *********************
	
	i = 0;
	XtSetArg (args[i], XmNtitle, "Advanced Search"); i++;
	XtSetArg (args[i], XmNdialogStyle, XmDIALOG_FULL_APPLICATION_MODAL); i++;
	XtSetArg (args[i], XmNmwmDecorations, MWM_DECOR_BORDER|MWM_DECOR_TITLE|MWM_DECOR_MENU); i++;
	XtSetArg (args[i], XmNmwmFunctions, MWM_FUNC_MOVE); i++;
	dlg_changeSearch = XmCreateFormDialog(app_shell, "dlg_changeSearch", args, i);

	/*******************
	El cuadro de diálogo tendrá campos para todas estas opciones de búsqueda:
		-número máximo y mínimo de protones del emisor
		-número atómico máximo y mínimo del emisor
		-energía máxima y mínima de emisión
		-probabilidad mínima y máxima de emisión
		-emisor natural y/o metaestable
		-nombre del emisor.
		-tipo de emisión
	Los parámetros que aparecerán por defecto son los de la búsqueda que se hace
	por defecto: energía máxima y mínima según el centroide y elementos naturales.
	*******************/
		
	i = 0;
	fr_emission_search = XmCreateFrame (dlg_changeSearch, "fr_emission_search", args, i);
	linea_changeSearch = XmCreateSeparator(dlg_changeSearch, "linea_edCalib", args, i);
	fr_emissor_search = XmCreateFrame (dlg_changeSearch, "fr_emissor_search", args, i);
   	
	i = 0;
	XtSetArg (args[i], XmNfractionBase, 3); i++;
	fm_botones_search = XmCreateForm (dlg_changeSearch, "fm_botones_search", args, i);	
    XtVaSetValues (fm_botones_search,
		XmNbottomAttachment, XmATTACH_FORM,
		XmNbottomOffset, 10,
		XmNrightAttachment, XmATTACH_FORM,
		XmNrightOffset, 10,
		XmNleftAttachment, XmATTACH_FORM,
		XmNleftOffset, 10,
		NULL);
   	XtVaSetValues (linea_changeSearch,
		XmNbottomAttachment, XmATTACH_WIDGET, XmNbottomWidget, fm_botones_search,
		XmNbottomOffset, 10,
		XmNrightAttachment, XmATTACH_FORM,
		XmNleftAttachment, XmATTACH_FORM,
		NULL);
   	XtVaSetValues (fr_emission_search,
		XmNbottomAttachment, XmATTACH_WIDGET, XmNbottomWidget, linea_changeSearch,
		XmNbottomOffset, 10,
		XmNrightAttachment, XmATTACH_FORM,
		XmNrightOffset, 10,
		XmNleftAttachment, XmATTACH_FORM,
		XmNleftOffset, 10,
		NULL);
   	XtVaSetValues (fr_emissor_search,
		XmNbottomAttachment, XmATTACH_WIDGET, XmNbottomWidget, fr_emission_search,
		XmNtopAttachment, XmATTACH_FORM,
		XmNtopOffset, 10,
		XmNrightAttachment, XmATTACH_FORM,
		XmNrightOffset, 10,
		XmNleftAttachment, XmATTACH_FORM,
		XmNleftOffset, 10,
		NULL);

	XtManageChild(fr_emission_search);
	XtManageChild(fr_emissor_search);
	XtManageChild(linea_changeSearch);
	XtManageChild(fm_botones_search);
	
	//********* Parámetros de búsqueda referentes a la emisión
	//Título
	cadena = XmStringCreate ("Emission", "BoldNormal");
   	i = 0;
   	XtSetArg (args[i], XmNchildType, XmFRAME_TITLE_CHILD); i++;
   	XtSetArg (args[i], XmNchildVerticalAlignment, XmALIGNMENT_CENTER); i++;
   	XtSetArg (args[i], XmNlabelString, cadena); i++;
	
   	lb_emission_search = XmCreateLabel (fr_emission_search, "lb_emission_search", args, i);
	XmStringFree (cadena);
	XtManageChild(lb_emission_search);
	
	//Manejador del frame
	i = 0;
	fm_emission_search = XmCreateForm(fr_emission_search, "fm_emission_search", args, i);

	
	printf("Columnas de separación\n");
	//Columnas de separación
	i = 0;
	XtSetArg (args[i], XmNorientation, XmVERTICAL); i++;
//	XtSetArg (args[i], XmNradioBehavior, True); i++;
//	XtSetArg (args[i], XmNradioAlwaysOne, True); i++;
	rw1_emission_search = XmCreateRowColumn (fm_emission_search, "rw1_emission_search", args, i);

	printf("Columna 2\n");
	i = 0;
	XtSetArg (args[i], XmNorientation, XmVERTICAL); i++;
//	XtSetArg (args[i], XmNradioBehavior, True); i++;
//	XtSetArg (args[i], XmNradioAlwaysOne, True); i++;
	rw2_emission_search = XmCreateRowColumn (fm_emission_search, "rw2_emission_search", args, i);
	
	printf("mínimo de energía\n");
	//**** Mínimo de energía
	cadena = XmStringCreate ("Energy minimum (keV)", "BoldNormal");
   	i = 0;
   	XtSetArg (args[i], XmNlabelString, cadena); i++;
   	lb_emin = XmCreateLabel (rw1_emission_search, "lb_emin", args, i);
   	XmStringFree (cadena);
	XtManageChild(lb_emin);
	i = 0;
 //  	XtSetArg (args[i], XmNcolumns, 10); i++;
   	XtSetArg (args[i], XmNmaxLength, 10); i++;
	txt_emin = XmCreateTextField(rw1_emission_search, "txt_emin", args, i);
   	XtAddCallback(txt_emin, XmNfocusCallback, seleccionarTexto, NULL);   
	XtManageChild(txt_emin);

	// **** Máximo de energía
	printf("Máx e\n");
	cadena = XmStringCreateLocalized ("Energy maximum (keV)");
	i = 0;
   	XtSetArg (args[i], XmNlabelString, cadena); i++;
   	lb_emax = XmCreateLabel (rw1_emission_search, "lb_emax", args, i);
   	XmStringFree (cadena);
	XtManageChild(lb_emax);
	i = 0;
 //  	XtSetArg (args[i], XmNcolumns, 10); i++;
   	XtSetArg (args[i], XmNmaxLength, 10); i++;
	txt_emax = XmCreateTextField(rw1_emission_search, "txt_emax", args, i);
	XtAddCallback(txt_emax, XmNfocusCallback, seleccionarTexto, NULL);   
	XtManageChild(txt_emax);
	cadena = XmStringCreateLocalized ("");
 
	//**** Mínimo de probabilidad
		printf("minp\n");

	cadena = XmStringCreate ("Probability minimum", "BoldNormal");
   	i = 0;
   	XtSetArg (args[i], XmNlabelString, cadena); i++;
   	lb_pmin = XmCreateLabel (rw2_emission_search, "lb_pmin", args, i);
   	XmStringFree (cadena);
	
	XtManageChild(lb_pmin);
	i = 0;
//   	XtSetArg (args[i], XmNcolumns, 10); i++;
   	XtSetArg (args[i], XmNmaxLength, 10); i++;
	txt_pmin = XmCreateTextField(rw2_emission_search, "txt_pmin", args, i);
   	XtAddCallback(txt_pmin, XmNfocusCallback, seleccionarTexto, NULL);   
	XtManageChild(txt_pmin);
	cadena = XmStringCreateLocalized ("");

	printf("maxp\n");
	//**** Máximo de probabilidad
	cadena = XmStringCreateLocalized ("Probability maximum");
   	i = 0;
   	XtSetArg (args[i], XmNlabelString, cadena); i++;
   	lb_pmax = XmCreateLabel (rw2_emission_search, "lb_pmax", args, i);
   	XmStringFree (cadena);
	XtManageChild(lb_pmax);
	i = 0;
  // 	XtSetArg (args[i], XmNcolumns, 10); i++;
   	XtSetArg (args[i], XmNmaxLength, 10); i++;
	txt_pmax = XmCreateTextField(rw2_emission_search, "txt_pmax", args, i);
   	XtAddCallback(txt_pmax, XmNfocusCallback, seleccionarTexto, NULL);   
	XtManageChild(txt_pmax);

	printf("tipoe\n");
	//**** Tipo de emisión
	cadena = XmStringCreateLocalized ("Type: ");
   	i = 0;
   	XtSetArg (args[i], XmNlabelString, cadena); i++;
   	lb_tipo = XmCreateLabel (rw1_emission_search, "lb_tipo", args, i);
   	XmStringFree (cadena);
	XtManageChild(lb_tipo);
	
	int count=6;
	char *tipos[]={"Any", "Gamma", "X (Ka1)", "X (Ka2)", "X (Kb1)", "X (Kb2)"};
	XmStringTable str_list=(XmStringTable) XtMalloc(count * sizeof(XmString *));
	for(int i=0;i<count;i++)	str_list[i]=XmStringCreateLocalized(tipos[i]);
	
	i=0;
	XtSetArg(args[i],XmNitems, str_list); i++;
	XtSetArg(args[i], XmNitemCount, count); i++;
	XtSetArg(args[i], XmNvisibleItemCount, count); i++;
	cb_tipo=XmCreateDropDownList(rw1_emission_search, "cb_tipo", args, i);

	printf("Creado el cuadro de diálogo, borramos str_list\n");
	for(i=0;i<count;i++)	XmStringFree(str_list[i]);
	XtFree((char *)str_list);
	
	XtAddCallback(cb_tipo, XmNselectionCallback, seleccionarTipo, NULL);
	XtManageChild(cb_tipo);

	printf("Seguimos creando cosillas\n");
	
	XtVaSetValues (rw1_emission_search,	
					XmNtopAttachment, XmATTACH_FORM,
					XmNleftAttachment, XmATTACH_FORM, NULL);
	XtManageChild(rw1_emission_search);

   	XtVaSetValues (rw2_emission_search,
		XmNtopAttachment, XmATTACH_FORM,
		XmNleftAttachment, XmATTACH_WIDGET, XmNleftWidget, rw1_emission_search,
		NULL);
	XtManageChild(rw2_emission_search);


  	XtVaSetValues (fm_emission_search,
		XmNbottomAttachment, XmATTACH_FORM,
		XmNtopAttachment, XmATTACH_FORM,
		XmNrightAttachment, XmATTACH_FORM,
		XmNleftAttachment, XmATTACH_FORM,
		NULL);
	XtManageChild(fm_emission_search);

  	//*******************************************************************
	//********* Parámetros de búsqueda referentes al emisor *************
	cadena = XmStringCreate ("Emissor", "BoldNormal");
   	i = 0;
   	XtSetArg (args[i], XmNchildType, XmFRAME_TITLE_CHILD); i++;
   	XtSetArg (args[i], XmNchildVerticalAlignment, XmALIGNMENT_CENTER); i++;
   	XtSetArg (args[i], XmNlabelString, cadena); i++;
	
   	lb_emissor_search = XmCreateLabel (fr_emissor_search, "lb_emissor_search", args, i);
	XmStringFree (cadena);
	XtManageChild(lb_emissor_search);
	
	//Manejador del frame
	i = 0;
	fm_emissor_search = XmCreateForm(fr_emissor_search, "fm_emissor_search", args, i);

	
	//**** Mínimo número atómico
	i = 0;
	XtSetArg (args[i], XmNorientation, XmVERTICAL); i++;
//	XtSetArg (args[i], XmNradioBehavior, True); i++;
//	XtSetArg (args[i], XmNradioAlwaysOne, True); i++;
	rw1_emissor_search = XmCreateRowColumn (fm_emissor_search, "rw1_emissor_search", args, i);
	cadena = XmStringCreate ("Minimum Atomic Number", "BoldNormal");
   	i = 0;
   	XtSetArg (args[i], XmNlabelString, cadena); i++;
   	lb_amin = XmCreateLabel (rw1_emissor_search, "lb_amin", args, i);
   	XmStringFree (cadena);
	XtManageChild(lb_amin);
	i = 0;
   	XtSetArg (args[i], XmNcolumns, 3); i++;
   	XtSetArg (args[i], XmNmaxLength, 3); i++;
	XtSetArg (args[i], XmNwidth, 3); i++;
	txt_amin = XmCreateTextField(rw1_emissor_search, "txt_amin", args, i);
   	XtAddCallback(txt_amin, XmNfocusCallback, seleccionarTexto, NULL);   
	XtManageChild(txt_amin);
	
	//**** Mínimo número de Protones
	cadena = XmStringCreate ("Minimum Mass Number", "BoldNormal");
   	i = 0;
   	XtSetArg (args[i], XmNlabelString, cadena); i++;
   	lb_prmin = XmCreateLabel (rw1_emissor_search, "lb_prmin", args, i);
   	XmStringFree (cadena);
	XtManageChild(lb_prmin);
	i = 0;
   	XtSetArg (args[i], XmNcolumns, 3); i++;
   	XtSetArg (args[i], XmNmaxLength, 3); i++;
	XtSetArg (args[i], XmNwidth, 3); i++;
	txt_prmin = XmCreateTextField(rw1_emissor_search, "txt_prmin", args, i);
   	XtAddCallback(txt_prmin, XmNfocusCallback, seleccionarTexto, NULL);   
	XtManageChild(txt_prmin);
	cadena = XmStringCreateLocalized ("");
	
	//**** Metaestabilidad
	cadena = XmStringCreate ("Metaestable?", "BoldNormal");
   	i = 0;
   	XtSetArg (args[i], XmNlabelString, cadena); i++;
   	lb_meta = XmCreateLabel (rw1_emissor_search, "lb_meta", args, i);
   	XmStringFree (cadena);
	XtManageChild(lb_meta);
	
	count=3;
	char *tiposM[]={"Doesn't Matter", "Yes", "No"};
	str_list=(XmStringTable) XtMalloc(count * sizeof(XmString *));
	for(int i=0;i<count;i++)	str_list[i]=XmStringCreateLocalized(tiposM[i]);
	
	i=0;
	XtSetArg(args[i],XmNitems, str_list); i++;
	XtSetArg(args[i], XmNitemCount, count); i++;
	XtSetArg(args[i], XmNvisibleItemCount, count); i++;
	cb_meta=XmCreateDropDownList(rw1_emissor_search, "cb_meta", args, i);
	
	XtAddCallback(cb_meta, XmNselectionCallback, seleccionarMeta, NULL);
	XtManageChild(cb_meta);
	//**** fin metaestable
	
	//**** Nombre Emisor 
	cadena = XmStringCreate ("Emissor Name (i.e. Cs)", "BoldNormal");
   	i = 0;
   	XtSetArg (args[i], XmNlabelString, cadena); i++;
   	lb_name = XmCreateLabel (rw1_emissor_search, "lb_name", args, i);
   	XmStringFree (cadena);
	XtManageChild(lb_name);
	i = 0;
   	XtSetArg (args[i], XmNcolumns, 2); i++;
   	XtSetArg (args[i], XmNmaxLength, 2); i++;
	XtSetArg (args[i], XmNwidth, 2); i++;
	
	txt_name = XmCreateTextField(rw1_emissor_search, "txt_name", args, i);
   	XtAddCallback(txt_name, XmNfocusCallback, seleccionarTexto, NULL);   
	XtManageChild(txt_name);
	cadena = XmStringCreateLocalized ("");
	
	//manejo de una de las columnas	
	XtVaSetValues (rw1_emissor_search,
		XmNtopAttachment, XmATTACH_FORM,
		XmNleftAttachment, XmATTACH_FORM,
		NULL);
	XtManageChild(rw1_emissor_search);


	// **** Máximo número atómico
	i = 0;
	XtSetArg (args[i], XmNorientation, XmVERTICAL); i++;
//	XtSetArg (args[i], XmNradioBehavior, True); i++;
//	XtSetArg (args[i], XmNradioAlwaysOne, True); i++;
	rw2_emissor_search = XmCreateRowColumn (fm_emissor_search, "rw2_emissor_search", args, i);
	cadena = XmStringCreateLocalized ("Maximum Atomic Number");
	i = 0;
   	XtSetArg (args[i], XmNlabelString, cadena); i++;
   	lb_amax = XmCreateLabel (rw2_emissor_search, "lb_amax", args, i);
   	XmStringFree (cadena);
	XtManageChild(lb_amax);
	i = 0;
   	XtSetArg (args[i], XmNcolumns, 3); i++;
   	XtSetArg (args[i], XmNmaxLength, 3); i++;
	XtSetArg (args[i], XmNwidth, 3); i++;
	txt_amax = XmCreateTextField(rw2_emissor_search, "txt_amax", args, i);
   	XtAddCallback(txt_amax, XmNfocusCallback, seleccionarTexto, NULL);   
	XtManageChild(txt_amax);
	cadena = XmStringCreateLocalized ("");
 
 	//**** Máximo Número de protones
	cadena = XmStringCreateLocalized ("Maximum Mass Number");
   	i = 0;
   	XtSetArg (args[i], XmNlabelString, cadena); i++;
   	lb_prmax = XmCreateLabel (rw2_emissor_search, "lb_prmax", args, i);
   	XmStringFree (cadena);
	XtManageChild(lb_prmax);
	i = 0;
   	XtSetArg (args[i], XmNcolumns, 3); i++;
   	XtSetArg (args[i], XmNmaxLength, 3); i++;
	XtSetArg (args[i], XmNwidth, 3); i++;
	txt_prmax = XmCreateTextField(rw2_emissor_search, "txt_prmax", args, i);
   	XtAddCallback(txt_prmax, XmNfocusCallback, seleccionarTexto, NULL);   
	XtManageChild(txt_prmax);
	
	printf("Creamos natural\n");
	//**** Natural
	cadena = XmStringCreate ("Natural?", "BoldNormal");
   	i = 0;
   	XtSetArg (args[i], XmNlabelString, cadena); i++;
   	lb_natu = XmCreateLabel (rw2_emissor_search, "lb_natu", args, i);
   	XmStringFree (cadena);
	XtManageChild(lb_natu);

	printf("Creamos la lista\n");
	i=0;
	XtSetArg(args[i],XmNitems, str_list); i++;
	XtSetArg(args[i], XmNitemCount, count); i++;
	XtSetArg(args[i], XmNvisibleItemCount, count); i++;
	cb_natu=XmCreateDropDownList(rw2_emissor_search, "cb_natu", args, i);
	
	XtAddCallback(cb_natu, XmNselectionCallback, seleccionarNatu, NULL);
	XtManageChild(cb_natu);
	
	printf("lista creada, borramos\n");
	for(i=0;i<count;i++)	XmStringFree(str_list[i]);
	XtFree((char *)str_list);

	XtVaSetValues (rw2_emissor_search,
		XmNtopAttachment, XmATTACH_FORM,
		XmNleftAttachment, XmATTACH_WIDGET, XmNleftWidget, rw1_emissor_search,
//		XmNleftOffset, 10,
		NULL);
	XtManageChild(rw2_emissor_search);


  	XtVaSetValues (fm_emissor_search,
		XmNbottomAttachment, XmATTACH_FORM,
		XmNtopAttachment, XmATTACH_FORM,
		XmNrightAttachment, XmATTACH_FORM,
		XmNleftAttachment, XmATTACH_FORM,
		NULL);
	XtManageChild(fm_emissor_search);
	
	// **********************
	// * seccion de botones *
	// **********************
	i = 0;
   	cadena = XmStringCreate ("OK", "BoldNormal");
   	XtSetArg (args[i], XmNlabelString, cadena); i++;
	XtSetArg (args[i], XmNalignment, XmALIGNMENT_CENTER); i++;
	bt_ok_search = XmCreatePushButton(fm_botones_search, "bt_ok_search", args, i);
  	XtAddCallback (bt_ok_search, XmNactivateCallback, pulsa_ok_search, object);
   	XmStringFree (cadena);
	i = 0;
   	cadena = XmStringCreate ("Cancel", "BoldNormal");
   	XtSetArg (args[i], XmNlabelString, cadena); i++;
	XtSetArg (args[i], XmNalignment, XmALIGNMENT_CENTER); i++;
	bt_cancel_search = XmCreatePushButton(fm_botones_search, "bt_cancel_search", args, i);
	XtAddCallback (bt_cancel_search, XmNactivateCallback, pulsa_cancel_search, NULL);
   	XmStringFree (cadena);
   	XtVaSetValues (bt_ok_search,
		XmNbottomAttachment, XmATTACH_FORM,
		XmNtopAttachment, XmATTACH_FORM,
		XmNleftAttachment, XmATTACH_FORM,
		XmNrightAttachment, XmATTACH_POSITION, XmNrightPosition, 1,
		XmNrightOffset, 5, 
		NULL);
   	XtVaSetValues (bt_cancel_search,
		XmNbottomAttachment, XmATTACH_FORM,
		XmNtopAttachment, XmATTACH_FORM,
		XmNleftAttachment, XmATTACH_POSITION, XmNleftPosition, 1,
		XmNrightAttachment, XmATTACH_POSITION, XmNrightPosition, 2,
		XmNrightOffset, 5, XmNleftOffset, 5,
		NULL);
	XtManageChild(bt_ok_search);
	XtManageChild(bt_cancel_search);
	}

//***************** PULSA OK SEARCH ***********************
//Recoge todos los parámetros y realiza una búsqueda en base a dichos
//parámetros
void pulsa_ok_search(Widget widget, XtPointer client_data, XtPointer call_data)
	{
	char *texto="";
	float emin, emax, pmin, pmax; 
	int   amin, amax, prmin, prmax;
	char *nombre="";
	bool natu, meta, buscarMeta, buscarNatu;

	//Tomamos la información de los campos de texto
	amin=amax=prmin=prmax=-1;
	emin=emax=pmin=pmax=-1.0;
	natu=meta=buscarMeta=buscarNatu=false;
		
	if( strcmp(texto=XmTextGetString(txt_amin), "") )
		{
		amin=atoi(texto);
		XtFree(texto);
		}
	if( strcmp(texto=XmTextGetString(txt_amax), "") )
		{
		amax=atoi(texto);
		XtFree(texto);
		}
	if( strcmp(texto=XmTextGetString(txt_emin), "") )
		{
		emin=atof(texto);
		XtFree(texto);
		}
	if( strcmp(texto=XmTextGetString(txt_emax), "") )
		{
		emax=atof(texto);
		XtFree(texto);
		}
	if( strcmp(texto=XmTextGetString(txt_pmin), "") )
		{
		pmin=atof(texto);
		XtFree(texto);
		}
	if( strcmp(texto=XmTextGetString(txt_pmax), "") )
		{
		pmax=atof(texto);
		XtFree(texto);
		}
	if( strcmp(texto=XmTextGetString(txt_prmin), "") )
		{
		prmin=atoi(texto);
		XtFree(texto);
		}
	if( strcmp(texto=XmTextGetString(txt_prmax), "") )
		{
		prmax=atoi(texto);
		XtFree(texto);
		}
	if( strcmp(texto=XmTextGetString(txt_name), "") )
		{
		if(strlen(texto)==1)	
			{
			nombre=new char[strlen(texto)+1];
			strcpy(nombre, " ");
			strcat(nombre, texto);
			printf("Un sólo carácter: |%s|\n", nombre);
			}
		else
			{
			nombre=new char[strlen(texto)];
			strcpy(nombre, texto);
			printf("nombre: |%s|\n", nombre);
			}
		XtFree(texto);
		}	
	
	//Convertimos la información de los combobox a bool:
	switch(tipoNatu)
		{
		case -1://Lo dejamos todo a falso
			buscarNatu=false;
			natu=false;
			break;
		case 0:
			buscarNatu=true;
			natu=true;
			break;
		case 1:
			buscarNatu=true;
			natu=false;
			break;
		default://Si pasara algo raro, se pasa de la búsqueda por natural
			break;
		}
	switch(tipoMeta)
		{
		case -1://Lo dejamos todo a falso
			buscarMeta=false;
			meta=false;
			break;
		case 0:
			buscarMeta=true;
			meta=true;
			break;
		case 1:
			buscarMeta=true;
			meta=false;
			break;
		default://Si pasara algo raro, se pasa de la búsqueda por metaestable
			break;
		}
	
	printf("Realizamos una nueva búsqueda\n");
	cVisIdentify *object = (cVisIdentify *) client_data;
	object->ShowCandidatesAdv(amin, amax, pmin, pmax, emin, emax, prmin, prmax, buscarNatu, natu, buscarMeta, meta, tipoGamma, nombre);	

	XtUnmanageChild(dlg_changeSearch);
	if(strcmp(nombre,""))	delete [] nombre;//Si el nombre no es vacío lo borramos
	}
	
//****************** PULSA CANCEL SEARCH ***********************
void pulsa_cancel_search(Widget widget, XtPointer client_data, XtPointer call_data)
	{
	XtUnmanageChild(dlg_changeSearch);
	}

//************************** PULSAR TIPO ********************************
/** Cambia el valor de tipoGamma según la entrada en los botones de radio
	Para mantener la compatibilidad con las funciones cLaraBD.searchEmissions
	los botones dan estos valores a tipoGamma:
	0->Gamma	1->X (Ka1)	2->X (Ka2)	3->X (Kb1)	4->X (Xb2)
	-1->Any o por defecto
	NOTA: En las bibliotecas de puccini (versión muy antigua del gcc, la 1.8),
		XmSET y XmOFF no están definidas.
		XmSET es 1 y XmOFF es 0, así que por eso se usan esos valores en vez
		de las constantes, que sería lo suyo	
	*/
void pulsarTipo(Widget widget, XtPointer client_data, XtPointer call_data)
	{
	XmToggleButtonCallbackStruct *state=(XmToggleButtonCallbackStruct *) call_data;
	
	printf("%s: %s\n", XtName(widget),
					state->set == 1 ? "on" :
					state->set == 0 ? "off" : "indeterminate");
		
	if(state->set)	tipoGamma= (int) client_data;
	else			tipoGamma= -1;
	printf("Tipo gamma: %d\n", tipoGamma);		
	}

//************************** PULSAR META ********************************
/** Cambia el valor de tipoGamma según la entrada en los botones de radio
	Para mantener la compatibilidad con las funciones cLaraBD.searchEmissions
	los botones dan estos valores a tipoGamma:
	0->	1->
	-1->Any o por defecto
	*/
void pulsarMeta(Widget widget, XtPointer client_data, XtPointer call_data)
	{
	XmToggleButtonCallbackStruct *state=(XmToggleButtonCallbackStruct *) call_data;
	printf("%s: %s\n", XtName(widget),
					state->set == 1 ? "on" :
					state->set == 0 ? "off" : "indeterminate");
	if(state->set)	tipoMeta= (int) client_data;
	else			tipoMeta= -1;
	printf("Tipo meta: %d\n", tipoMeta);		
	}

//************************** PULSAR NATU ********************************
/** Cambia el valor de tipoNatu según la entrada en los botones de radio
	Para mantener la compatibilidad con las funciones cLaraBD.searchEmissions
	los botones dan estos valores a tipoGamma:
	0->	1->
	-1->Any o por defecto
	*/
void pulsarNatu(Widget widget, XtPointer client_data, XtPointer call_data)
	{
	XmToggleButtonCallbackStruct *state=(XmToggleButtonCallbackStruct *) call_data;
	printf("%s: %s\n", XtName(widget),
					state->set == 1 ? "on" :
					state->set == 0 ? "off" : "indeterminate");
	if(state->set)	tipoNatu= (int) client_data;
	else			tipoNatu= -1;
	printf("Tipo natural: %d\n", tipoNatu);		
	}

//*************************** SELECCIONAR META ***************
/* Guardamos la condicion de metaestabilidad por la que queremos buscar
*/
void seleccionarMeta(Widget widget, XtPointer client_data, XtPointer call_data) 
	{
	printf("SELECCIONAR META\n");
	XmComboBoxCallbackStruct *cb=(XmComboBoxCallbackStruct *) call_data;
	XmString patron;

	char *choice=(char *) XmStringUnparse (cb->item_or_text, 
											XmFONTLIST_DEFAULT_TAG,
											XmCHARSET_TEXT, XmCHARSET_TEXT,
											NULL, 0, XmOUTPUT_ALL);
		
	if(cb->event == NULL)	printf("ERROR: Posible elección: %s\n", choice);
	printf("Seleccionamos: |%s|\n", choice);
	if(!strcmp(choice, "Doesn't Matter"))
		tipoMeta=-1;
	if(!strcmp(choice, "Yes"))	
		tipoMeta=1;
	if(!strcmp(choice, "No"))	
		tipoMeta=0;
			 
	return;
	}
	
//*************************** SELECCIONAR NATU ***************
/* Guardamos la condicion de naturalidad por la que queremos buscar
*/
void seleccionarNatu(Widget widget, XtPointer client_data, XtPointer call_data) 
	{
	XmComboBoxCallbackStruct *cb=(XmComboBoxCallbackStruct *) call_data;
	XmString patron;

	char *choice=(char *) XmStringUnparse (cb->item_or_text, 
											XmFONTLIST_DEFAULT_TAG,
											XmCHARSET_TEXT, XmCHARSET_TEXT,
											NULL, 0, XmOUTPUT_ALL);
		
	if(cb->event == NULL)	printf("ERROR: Posible elección: %s\n", choice);
	if(!strcmp(choice, "Doesn't Matter"))
		tipoNatu=-1;
	if(!strcmp(choice, "Yes"))	
		tipoNatu=0;
	if(!strcmp(choice, "No"))	
		tipoNatu=1;
			 
	return;
	}

//*************************** SELECCIONAR TIPO ***************
/* Guardamos el tipo de rayo que queremos buscar
*/
void seleccionarTipo(Widget widget, XtPointer client_data, XtPointer call_data) 
	{
	XmComboBoxCallbackStruct *cb=(XmComboBoxCallbackStruct *) call_data;
	XmString patron;

	char *choice=(char *) XmStringUnparse (cb->item_or_text, 
											XmFONTLIST_DEFAULT_TAG,
											XmCHARSET_TEXT, XmCHARSET_TEXT,
											NULL, 0, XmOUTPUT_ALL);
		
	if(cb->event == NULL)	printf("ERROR: Posible elección: %s\n", choice);
	if(!strcmp(choice, "Any"))
		tipoGamma=-1;
	if(!strcmp(choice, "Gamma"))	
		tipoGamma=0;
	if(!strcmp(choice, "X (Ka1)"))	
		tipoGamma=1;
	if(!strcmp(choice, "X (Ka2)"))	
		tipoGamma=2;
	if(!strcmp(choice, "X (Kb1)"))	
		tipoGamma=3;
	if(!strcmp(choice, "X (Kb2)"))	
		tipoGamma=4;
			 
	return;
	}
