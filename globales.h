// Definiciones

// Librerias
#include "preferences.h"
#include "lara/cLaraBD.H"
#include "espectros.h"

// Datos de la parte de analisis
extern cEspectros Datos;

// Base de datos Lara
extern cLaraBD LaraBD;

// Fichero de preferencias
extern cPreferences Preferences;

extern char *nombreFichFBD;
extern char *BDIdentificaciones;
// Widgets importantes
extern Widget app_shell, main_w, trabajo_WA;
extern XtAppContext app_context;
extern Widget form_calibracion, form_analisis, form_continuo, form_eficiencia;


extern int maxOpciones;
extern bool opciones[];

// Tapiz general
extern Display *dpy;

// variables de estado de trabajo
extern bool CambiandoSeccion;

// funciones
extern void PonTituloFichCambiado();
extern void PonTituloFichSinCambios();

// Colores
extern XColor colAmarillo, colVerde, colRojo, colNegro, colBlanco, colAzul;
