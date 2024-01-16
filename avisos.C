#include <Xm/Xm.h>
#include <Xm/MessageB.h>
#include "globales.h"
#include "constDef.h"

// funciones
void Aviso(Widget, char *);
int Continuar(Widget, char *, char *);



// Implementacion

//***************** AVISO **********************
/*	Crea una ventana dependiente de widget con el texto texto y 
	opción de OK
*/
void Aviso(Widget widget, char *texto) 
	{
	static Widget aviso;
	XmString t;
	void dlg_callback (Widget, XtPointer, XtPointer);

	if (!aviso) 
		{
		Arg args[5];
		int i = 0;
		XmString ok = XmStringCreateLocalized ("OK");
		XtSetArg (args[i], XmNautoUnmanage, false); i++;
		XtSetArg (args[i], XmNcancelLabelString, ok); i++;
	   XtSetArg (args[i], XmNtitle, AVISOS); i++;
	   XtSetArg (args[i], XmNdeleteResponse, XmDO_NOTHING); i++;
		aviso = XmCreateInformationDialog (widget, "aviso", args, i);
		XtAddCallback (aviso, XmNcancelCallback, dlg_callback, NULL);
		XtUnmanageChild (XtNameToWidget (aviso, "OK"));
		XtUnmanageChild (XtNameToWidget (aviso, "Help"));
		}
	t = XmStringCreateLocalized(texto);
	XtVaSetValues (aviso, XmNmessageString, t,
				XmNdialogStyle, XmDIALOG_FULL_APPLICATION_MODAL,
				NULL);
	XmStringFree (t);
	XtManageChild (aviso);
	}

//************* CARTEL ************************
/*	Como aviso, pero en este caso no hay botón de OK. Devuelve el widget
	que ha creado, y será el usuario el que tenga que quitarlo cuando lo 
	desee.
	*/
Widget Cartel(Widget widget, char *texto) 
	{
	static Widget aviso;
//	Widget aviso;
	XmString t;

	t = XmStringCreateLocalized(texto);
	if (!aviso) 
		{
		Arg args[5];
		int i = 0;
		XtSetArg (args[i], XmNautoUnmanage, false); i++;
	    XtSetArg (args[i], XmNtitle, "Computing"); i++;
	    XtSetArg (args[i], XmNdeleteResponse, XmDO_NOTHING); i++;
		aviso = XmCreateInformationDialog (widget, "aviso", args, i);
		}
	printf("%s\n", texto);
	XtVaSetValues (aviso, XmNmessageString, t,
				XmNdialogStyle, XmDIALOG_FULL_APPLICATION_MODAL,
				NULL);
	printf("Liberamos t\n");
	XmStringFree (t);
	printf("Texto puesto\n");
	XtManageChild (aviso);
	return aviso;
	}

void dlg_callback (Widget aviso, XtPointer client_data, XtPointer call_data) 
	{
	XtUnmanageChild (aviso);
	}


int Continuar(Widget parent, char *texto, char *titulo) 
	{
	static Widget dialog;
	XmString yes, no, cancel;
	static int answer;
	void response(Widget, XtPointer, XtPointer);
	
	if (!dialog) 
		{
		Arg args[5];
		int i = 0;
	    XtSetArg (args[i], XmNdeleteResponse, XmDO_NOTHING); i++;
		dialog = XmCreateWarningDialog(parent, "dialog", args, i);
		yes = XmStringCreateLocalized("Yes");
		no  = XmStringCreateLocalized("No");
		cancel = XmStringCreateLocalized("Cancel");
		XtVaSetValues (dialog,
							XmNdialogStyle, XmDIALOG_FULL_APPLICATION_MODAL,
							XmNokLabelString, yes,
							XmNcancelLabelString, no,
							XmNhelpLabelString, cancel,
							NULL);
		XtAddCallback (dialog, XmNokCallback, response, (XtPointer) &answer);
		XtAddCallback (dialog, XmNcancelCallback, response, (XtPointer) &answer);
		XtAddCallback (dialog, XmNhelpCallback, response, (XtPointer) &answer);
		XmStringFree(yes);
		XmStringFree(no);
		XmStringFree(cancel);
		}
	// le pone el titulo al shell padre
	Widget w_shell = XtParent(dialog);
	XtVaSetValues (w_shell, XmNtitle, titulo, NULL);

	answer = 0;
	XmString text = XmStringCreateLocalized (texto);
	XtVaSetValues (dialog, XmNmessageString, text, NULL);
	XmStringFree (text);
	XtManageChild (dialog);
	/* cola de eventos parada */
	while (answer == 0)
		XtAppProcessEvent (app_context, XtIMAll);
	XtUnmanageChild(dialog);
	return answer;
	}

//*************** RESPONSE *******************
/* Función correspondiente a las callback que vengan
	de los cuadros de continuar. Lo único que hará es
	devolvernos cuál es la respuesta del usuario para
	actuar en consecuencia
*/
void response(Widget widget, XtPointer client_data, XtPointer call_data) {
	int *answer = (int *) client_data;
	XmAnyCallbackStruct *cbs = (XmAnyCallbackStruct *) call_data;
	
	switch (cbs->reason) {
		case XmCR_OK		: *answer = SI; break;
		case XmCR_CANCEL	: *answer = NO; break;
		case XmCR_HELP 	: *answer = CANCELAR; break;
		default				: return;
	}
}

//************************ ELEGIR ***********************
/*
	Crea una ventana de aviso que nos permite elegir hasta tres opciones
	Parámetros:
		-Parent: widget padre
		-texto:  texto de la ventana
		-titulo: título de la ventana
		-primera, segunda, tercera: texto de los tres botones de las opciones.
									si alguna aparece a null esa opcion no aparece
*/
int Elegir(Widget parent, char *texto, char *titulo,
				char *primera = NULL, char *segunda = NULL, char *tercera = NULL) {
	static Widget dialog;
	XmString yes, no, cancel;
	static int answer;
	void response(Widget, XtPointer, XtPointer);
	
//	if (!dialog) {
		Arg args[5];
		int i = 0;
	    XtSetArg (args[i], XmNdeleteResponse, XmDO_NOTHING); i++;
		dialog = XmCreateQuestionDialog(parent, "dialog", args, i);
		if (primera == NULL) yes = XmStringCreateLocalized("Yes");
		else yes = XmStringCreateLocalized(primera);
		if (segunda == NULL) no  = XmStringCreateLocalized("No");
		else no  = XmStringCreateLocalized(segunda);

		cancel = XmStringCreateLocalized("Cancel");
		XtVaSetValues (dialog,
							XmNdialogStyle, XmDIALOG_FULL_APPLICATION_MODAL,
							XmNokLabelString, yes,
							XmNcancelLabelString, no,
							NULL);
		XtAddCallback (dialog, XmNokCallback, response, (XtPointer) &answer);
		XtAddCallback (dialog, XmNcancelCallback, response, (XtPointer) &answer);
		XmStringFree(yes);
		XmStringFree(no);
		if (tercera == NULL)
			XtUnmanageChild (XtNameToWidget (dialog, "Help"));
		else {
			cancel  = XmStringCreateLocalized(tercera);
			XtVaSetValues (dialog,
								XmNhelpLabelString, cancel,
								NULL);
			XtAddCallback (dialog, XmNhelpCallback, response, (XtPointer) &answer);
			XmStringFree(cancel);
		}
//	}//Del if(!dialog)
	// le pone el titulo al shell padre
	Widget w_shell = XtParent(dialog);
	XtVaSetValues (w_shell, XmNtitle, titulo, NULL);

	answer = 0;
	XmString text = XmStringCreateLocalized (texto);
	XtVaSetValues (dialog, XmNmessageString, text, NULL);
	XmStringFree (text);
	XtManageChild (dialog);
	/* cola de eventos parada */
	while (answer == 0)
		XtAppProcessEvent (app_context, XtIMAll);
	XtUnmanageChild(dialog);
	return answer;
}
