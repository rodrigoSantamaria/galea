#include <stdio.h>
#include "fuentes.h"

// Variables publicas
XmFontList lista_fuentes, lista_fuentes_fixed;


// Prototipos publicos
void CargaFuentes(Display*);
  
//NOTA: Poniendo los caracteres que en Courier nos dan los superíndices
//		El modo de representación es indiferente, sólo saca bien 1,2,3, los de
//		números bajos
//Unicode
//char superindices[]={'\u2070','\u00B9','\u00B2','\u00B3','\u2074',
//					 '\u2075','\u2076','\u2077','\u2078','\u2079'};
wchar_t superindices[]={'\u2070','\u00B9','\u00B2','\u00B3','\u2074',
					 '\u2075','\u2076','\u2077','\u2078','\u2079'};
/*char superindices[]={'\U00002070','\U000000B9','\U000000B2','\U000000B3','\U00002074',
					 '\U00002075','\U00002076','\U00002077','\U00002078','\U00002079'};*/
//UTF8
//char superindices[]={0xC2B9,0xC2B9,0xC2B2,0xC2B3,0xE281B4,
//					 0xE281B5,0xE281B6,0xE281B7,0xE281B8,0xE281B9};
//wchar_t superindices[]={0xC2B9,0xC2B9,0xC2B2,0xC2B3,0xE281B4,
//					 0xE281B5,0xE281B6,0xE281B7,0xE281B8,0xE281B9};
//Mapa de caracteres copiado a Anjuta
//char superindices[]={'⁰','¹','²','³','⁴','⁵','⁶','⁷','⁸','⁹'};
//char *superindices[]={"⁰","¹","²","³","⁴","⁵","⁶","⁷","⁸","⁹"};
					 
//¹²³⁴⁵⁶⁷⁸⁹⁰Ra   ²²⁶Ra

// ****************
//  Implementacion
// ****************

void CargaFuentes(Display* dpy) {
	XmFontListEntry fuente_normal	    , fuente_bold,     fuente_pequegna,
			fuente_normal_bold  , fuente_subIndice,
			fuente_subIndiceBold, fuente_fixed,    fuente_especial;
	
	// fuentes normales
	//NOTA: Para todas las fuentes tenemos en jsbach un warning del tipo
	//Cannot convert string "XXX" to type FontStruct
	//Habrá que observar si este error se reproduce en wagner/puccini o si es más bien
	//un error de cómo está escrito
	
//Fuentes válidas en Wagner/Puccini (Unix)
/*
	char char_fuente_subIndice[]     ="-*-interface user-medium-r-normal-*-8-*-*-*-*-*-*-*";
	char char_fuente_subIndiceBold[] ="-*-interface user-bold-r-normal-*-8-*-*-*-*-*-*-*";
	char char_fuente_pequegna[] 	 ="-*-interface system-medium-r-normal-*-10-*-*-*-*-*-*-*";
	char char_fuente_normal[] 	 ="-*-interface user-medium-r-normal-*-12-*-*-*-*-*-*-*";
	char char_fuente_bold[] 	 ="-*-interface user-bold-r-normal-*-14-*-*-*-*-*-*-*";
	char char_fuente_normal_bold[]   ="-*-interface user-bold-r-normal-*-12-*-*-*-*-*-*-*";
	char char_fuente_especial[] 	 ="-*-interface user-bold-r-normal-*-18-*-*-*-*-*-*-*";
	// fuentes fixed
	char char_fuente_fixed[] 	 ="-*-interface user-medium-r-normal-*-12-*-*-*-M-*-*-*";
*/
//Fuentes válidas en Schumann (Linux)

	char char_fuente_subIndice[]     ="-*-courier-medium-r-normal-*-8-*-*-*-*-*-*-*";
	char char_fuente_subIndiceBold[] ="-*-courier-bold-r-normal-*-8-*-*-*-*-*-*-*";
	char char_fuente_pequegna[] 	 ="-*-courier-medium-r-normal-*-10-*-*-*-*-*-*-*";
	char char_fuente_normal[] 	 ="-*-courier-medium-r-normal-*-12-*-*-*-*-*-*-*";
	char char_fuente_bold[] 	 ="-*-courier-bold-r-normal-*-14-*-*-*-*-*-*-*";
	char char_fuente_normal_bold[]   ="-*-courier-bold-r-normal-*-12-*-*-*-*-*-*-*";
	char char_fuente_especial[] 	 ="-*-courier-bold-r-normal-*-18-*-*-*-*-*-*-*";
	// fuentes fixed
	char char_fuente_fixed[] 	 ="-*-courier-medium-r-normal-*-12-*-*-*-M-*-*-*";

/*
	char char_fuente_subIndice[]     ="-*-courier-medium-r-normal-*-8-*-*-*-*-*-*-*";
	char char_fuente_subIndiceBold[] ="-*-courier-bold-r-normal-*-8-*-*-*-*-*-*-*";
	char char_fuente_pequegna[] 	 ="-*-courier-medium-r-normal-*-10-*-*-*-*-*-*-*";
	char char_fuente_normal[] 	 ="-*-courier-medium-r-normal-*-12-*-*-*-*-*-iso10646-*";
	char char_fuente_bold[] 	 ="-*-courier-bold-r-normal-*-14-*-*-*-*-*-iso10646-*";
	char char_fuente_normal_bold[]   ="-*-courier-bold-r-normal-*-12-*-*-*-*-*-iso10646-*";
	char char_fuente_especial[] 	 ="-*-courier-bold-r-normal-*-18-*-*-*-*-*-*-*";
	// fuentes fixed
	char char_fuente_fixed[] 	 ="-*-courier-medium-r-normal-*-12-*-*-*-M-*-*-*";
*/

/*	char char_fuente_subIndice[]     ="-*-courier-medium-r-normal-*-8-*-*-*-*-*-iso10646-*";
	char char_fuente_subIndiceBold[] ="-*-courier-bold-r-normal-*-8-*-*-*-*-*-iso10646-*";
	char char_fuente_pequegna[] 	 ="-*-courier-medium-r-normal-*-10-*-*-*-*-*-iso10646-*";
	char char_fuente_normal[] 	 ="-*-courier-medium-r-normal-*-12-*-*-*-*-*-iso10646-*";
	char char_fuente_bold[] 	 ="-*-courier-bold-r-normal-*-14-*-*-*-*-*-iso10646-*";
	char char_fuente_normal_bold[]   ="-*-courier-bold-r-normal-*-12-*-*-*-*-*-iso10646-*";
	char char_fuente_especial[] 	 ="-*-courier-bold-r-normal-*-18-*-*-*-*-*-iso10646-*";
	// fuentes fixed
	char char_fuente_fixed[] 	 ="-*-courier-medium-r-normal-*-12-*-*-*-M-*-iso10646-*";
	*/			
	//Fixed, no muestra más que cuadrados, tiene tipo misc. Si le quito iso10646 me saca kanjis
/*	char char_fuente_subIndice[]     ="-*-fixed-medium-r-normal-*-8-*-*-*-*-*-*-*";
	char char_fuente_subIndiceBold[] ="-*-fixed-bold-r-normal-*-8-*-*-*-*-*-*-*";
	char char_fuente_pequegna[] 	 ="-*-fixed-medium-r-normal-*-10-*-*-*-*-*-*-*";
	char char_fuente_normal[] 	 ="-*-fixed-medium-r-normal-*-12-*-*-*-*-*-*-*";
	char char_fuente_bold[] 	 ="-*-fixed-bold-r-normal-*-14-*-*-*-*-*-*-*";
	char char_fuente_normal_bold[]   ="-*-fixed-bold-r-normal-*-12-*-*-*-*-*-*-*";
	char char_fuente_especial[] 	 ="-*-fixed-bold-r-normal-*-18-*-*-*-*-*-*-*";
	// fuentes fixed
	char char_fuente_fixed[] 	 ="-*-fixed-medium-r-normal-*-12-*-*-*-*-*-*-*";
	*/	
	//courier forzada a bitstream (no tiene estos tipos)
/*	char char_fuente_subIndice[]     ="-bitstream-fixed-medium-r-normal-*-8-*-*-*-*-*-iso10646-*";
	char char_fuente_subIndiceBold[] ="-bitstream-courier-bold-r-normal-*-8-*-*-*-*-*-iso10646-*";
	char char_fuente_pequegna[] 	 ="-bitstream-courier-medium-r-normal-*-10-*-*-*-*-*-iso10646-*";
	char char_fuente_normal[] 	 ="-bitstream-courier-medium-r-normal-*-12-*-*-*-*-*-iso10646-*";
	char char_fuente_bold[] 	 ="-bitstream-courier-bold-r-normal-*-14-*-*-*-*-*-iso10646-*";
	char char_fuente_normal_bold[]   ="-bitstream-courier-bold-r-normal-*-12-*-*-*-*-*-iso10646-*";
	char char_fuente_especial[] 	 ="-bitstream-courier-bold-r-normal-*-18-*-*-*-*-*-iso10646-*";
	// fuentes fixed
	char char_fuente_fixed[] 	 ="-bitstream-courier-medium-r-normal-*-12-*-*-*-*-*-iso10646-*";
*/	
	
	// Carga las fuentes
	printf("Llamadas a EntryLoad\n");
	fuente_subIndiceBold = XmFontListEntryLoad (dpy, char_fuente_subIndiceBold, XmFONT_IS_FONT, "SubIndiceBold" );//13
	fuente_subIndice     = XmFontListEntryLoad (dpy, char_fuente_subIndice, XmFONT_IS_FONT, "SubIndice" );		//9
	fuente_pequegna	     = XmFontListEntryLoad (dpy, char_fuente_pequegna, XmFONT_IS_FONT, "Pequegna" );	//8
	fuente_normal	     = XmFontListEntryLoad (dpy, char_fuente_normal, XmFONT_IS_FONT, XmFONTLIST_DEFAULT_TAG );
	fuente_bold 	     = XmFontListEntryLoad (dpy, char_fuente_bold, XmFONT_IS_FONT, "Bold" );	//4
	fuente_normal_bold   = XmFontListEntryLoad (dpy, char_fuente_normal_bold, XmFONT_IS_FONT, "BoldNormal" );	
	fuente_especial      = XmFontListEntryLoad (dpy, char_fuente_especial, XmFONT_IS_FONT, "Especial" );
	fuente_fixed	     = XmFontListEntryLoad (dpy, char_fuente_fixed, XmFONT_IS_FONT, XmFONTLIST_DEFAULT_TAG );

	// Añade las fuentes a una lista
	printf("Llamadas a AppendEntry\n");
	lista_fuentes = XmFontListAppendEntry (NULL, fuente_normal);
	printf("Bold\n");
	lista_fuentes = XmFontListAppendEntry (lista_fuentes, fuente_bold);
	printf("Pequeña\n");
	lista_fuentes = XmFontListAppendEntry (lista_fuentes, fuente_pequegna);
	printf("BoldNormal\n");
	lista_fuentes = XmFontListAppendEntry (lista_fuentes, fuente_normal_bold);
	printf("Subindice\n");
	lista_fuentes = XmFontListAppendEntry (lista_fuentes, fuente_subIndice);
	printf("SubindiceBold\n");
	lista_fuentes = XmFontListAppendEntry (lista_fuentes, fuente_subIndiceBold);
	// Añade las fuentes a la lista de fixed
	printf("Añadiendo las fuentes\n");
	lista_fuentes_fixed = XmFontListAppendEntry (NULL, fuente_fixed);
	printf("Liberando memoria\n");

	// libera la memoria
	
	#if ((XmVERSION == 1) && (XmREVISION == 2) && (XmUPDATE_LEVEL == 1))
		XtFree (fuente_normal);
		XtFree (fuente_bold);
		XtFree (fuente_pequegna);
		XtFree (fuente_normal_bold);
		XtFree (fuente_subIndice);
		XtFree (fuente_subIndiceBold);
		XtFree (fuente_fixed);
		XtFree (fuente_especial);
	#else
		XmFontListEntryFree (&fuente_normal);
		XmFontListEntryFree (&fuente_bold);
		XmFontListEntryFree (&fuente_pequegna);
		XmFontListEntryFree (&fuente_normal_bold);
		XmFontListEntryFree (&fuente_subIndice);
		XmFontListEntryFree (&fuente_subIndiceBold);
		XmFontListEntryFree (&fuente_fixed);
		XmFontListEntryFree (&fuente_especial);
	#endif
	
	printf("Cargamos otras fuentes\n");
	// Carga las fuentes que requieren modificacion
	XFontStruct *especial = XLoadQueryFont(dpy, char_fuente_especial);
	if (especial != NULL) {
/*		if (especial->n_properties != 0) {
			printf("Propiedades %d: \n", especial->n_properties);
			for (int i = 0; i < especial->n_properties; i++) {
				char *nombre = XGetAtomName(dpy, especial->properties[i].name);
				printf("Atom: %s  ", nombre);
				printf("valor: %s\n", XGetAtomName(dpy, especial->properties[i].card32));
				XFree(nombre);
			}
		}*/
		XmFontListEntry fuente_especial = XmFontListEntryCreate("Especial", 
				XmFONT_IS_FONT, (XtPointer) especial);
		lista_fuentes = XmFontListAppendEntry (lista_fuentes, fuente_especial);
		#if ((XmVERSION == 1) && (XmREVISION == 2) && (XmUPDATE_LEVEL == 1))
			XtFree (fuente_especial);
		#else
			XmFontListEntryFree (&fuente_especial);
		#endif
	}
	else XBell(dpy, 0);
}
