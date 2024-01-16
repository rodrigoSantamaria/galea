#include <math.h>
#include <stdio.h>
#include "cursor.h"
#include "datos.h"
#include "globales.h"
#include "math1.h"


/***************
 *             *
 *   GENERAL   *
 *             *
 ***************/

cCursor::cCursor() { // constructor
   // inicializa los atributos
	anchoVirtual = 0;
	anchoRealD = 100; altoRealD = 100;
	anchoReal = 100; altoReal = 100;
	posMinX = 0; posMinY = 0;
	cursorPX = 0; cursor = 0;
}

cCursor::~cCursor() { // destructor
}


/*****************
 *               *
 *     DATOS     *
 *               *
 *****************/

unsigned int cCursor::posicion() {
	return cursor;
}

int cCursor::CambiaCursor(unsigned int nuevaPosicion) {
	if ((nuevaPosicion >= minimo)&&(nuevaPosicion <= maximo)) {
		cursor = nuevaPosicion;
		CambiaVentana();
	}
	
	// devuelve la nueva posicion del cursor
	return cursor;
}

int cCursor::CambiaCursorPX(unsigned int nuevaPosicion) {
	if (anchoRealD > 0) {
		// obtiene la posicion en el tapiz general
		nuevaPosicion += posMinX;
		cursorPX = nuevaPosicion;
		// obtiene el canal relacionado con esa posicion
		nuevaPosicion = (int) ((double)nuevaPosicion * (double)anchoVirtual / (double)anchoRealD);
		cursor = nuevaPosicion;
	}
	
	if (cursor < minimo)
		cursor = minimo;
	else if (cursor > maximo)
		cursor = maximo;
	
	// devuelve la nueva posicion del cursor
	return cursor;
}

/*****************************
 *                           *
 *     CONTROL DEL TAPIZ     *
 *                           *
 *****************************/


void cCursor::DefineVentanaTotal(unsigned int ancho, unsigned int alto) {
   anchoRealD = ancho;
	altoRealD = alto;
	// una vez cambiado las dimensiones vuelve a calcular la posicion X
	// del cursor en esta nueva ventana
	CambiaVentana();
}

void cCursor::DefineAnchoTotal(double ancho) {
   anchoRealD = (unsigned int)(ancho*anchoReal);
}

void cCursor::DefineAltoTotal(double alto) {
	altoRealD = (unsigned int)(alto*altoReal);
	// una vez cambiado las dimensiones vuelve a calcular la posicion Y
	// de los puntos en esta nueva ventana
	CambiaVentana();
}

void cCursor::DefineVentanaReal(unsigned int ancho, unsigned int alto) {
   anchoReal = ancho;
	altoReal = alto;
}

void cCursor::DefinePosicionVentanaReal(unsigned int valorX, unsigned int valorY) {
   posMinX =  valorX;
	posMinY = valorY;
}

void cCursor::DefinePosicionXReal(double valorX) {
 	posMinX = (unsigned int)(valorX * anchoRealD);
}

void cCursor::DefinePosicionYReal(double valorY) {
   posMinY =  (unsigned int)(valorY * altoRealD);
}

void cCursor::CambiaVentana() {
	if (anchoVirtual > 0)
	   cursorPX = (int)((cursor*anchoRealD)/anchoVirtual);
}

void cCursor::CreaGC(Display *dpy, Widget d) {
	XGCValues gcv;
	
	gcv.function = GXxor;
	gcv.line_style = LineOnOffDash;
	gcv.line_width = 2;
	gcv.background = Preferences.GetGrafBgColor().pixel;

	gcv.foreground = Preferences.GetCursorUpColor().pixel;
	gcv.foreground = gcv.foreground ^ gcv.background;
	gcUp = XCreateGC(dpy, RootWindowOfScreen(XtScreen(d)),
		GCFunction|GCForeground|GCBackground|GCLineStyle|GCLineWidth, &gcv);

	gcv.foreground = Preferences.GetCursorDownColor().pixel;
	gcv.foreground = gcv.foreground ^ gcv.background;
	gcDown = XCreateGC(dpy, RootWindowOfScreen(XtScreen(d)),
		GCFunction|GCForeground|GCBackground|GCLineStyle|GCLineWidth, &gcv);		
}

void cCursor::DibujaCursor(Display *dpy, Pixmap p, Window w, int posicY) {
	int posicX;
	unsigned int valMinX = (int)(posMinX * anchoVirtual / anchoRealD);
	unsigned int valMaxX = (int)((posMinX + anchoReal) * anchoVirtual / anchoRealD);
	
	if ((cursor >= valMinX) && (cursor <= valMaxX) && (anchoVirtual > 0)) {
		posicX = (int)((double)cursor * (double)anchoRealD / (double)anchoVirtual) - posMinX;
		if ((posicY >= posMinY)&&(posicY<=posMinY+altoReal)) {		
			posicY = altoReal - (posicY - posMinY)-1;
			XDrawLine (dpy, w, gcUp, posicX, 0, posicX, posicY);
			XDrawLine (dpy, w, gcDown, posicX, posicY, posicX, altoReal);
			XDrawLine (dpy, p, gcUp, posicX, 0, posicX, posicY);
			XDrawLine (dpy, p, gcDown, posicX, posicY, posicX, altoReal);
		}
		else if (posicY < posMinY) {
			XDrawLine (dpy, w, gcUp, posicX, 0, posicX, altoReal);
			XDrawLine (dpy, p, gcUp, posicX, 0, posicX, altoReal);
		}
		else {
			XDrawLine (dpy, w, gcDown, posicX, 0, posicX, altoReal);
			XDrawLine (dpy, p, gcDown, posicX, 0, posicX, altoReal);
		}
	}
}
