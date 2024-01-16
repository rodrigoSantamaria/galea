// Codigo de cursores
#include "globales.h"
#include <X11/cursorfont.h>

Cursor CursorEspera;

void PreparaCursores() {
	CursorEspera = XCreateFontCursor(dpy, XC_watch);
}

void PonCursorNormal() {
	XUndefineCursor(dpy, XtWindow(main_w));
	XmUpdateDisplay(main_w);
}

void PonCursorEspera() {
	XDefineCursor(dpy, XtWindow(main_w), CursorEspera);
	XmUpdateDisplay(main_w);
}
