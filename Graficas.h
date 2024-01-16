#ifndef __GRAFICAS_H__
#define __GRAFICAS_H__

#include <Xm/Xm.h>

/*****************************************************
 ***																***
 ***						INTERNAL LISTS						***
 ***																***
 *****************************************************/

typedef struct NodoGrafica 
	{
	char *keyName; // nombre alternativo para identificar esta grafica
	char keyNumber; // numero alternativo para identificar esta grafica
	int Priority; // prioridad en la posicion de dibujo (mas bajo, se dibuja antes)
	GC gcDraw; // graphics context para dibujar
	char estilo; /* estilo para la grafica:
			0 -> puntos
			1 -> lineas */
	double *DatosX, *DatosY, *DatosEscY; /* Valores de los datos:
						X -> Posiciones horizontales
						Y -> Posiciones verticales
						EscY -> Posiciones verticales ya escaladas */
	double minX, maxX; /* Valores minimo y maximo de referencia en la
				escala X. */
	double minY, maxY; /* Valores minimo y maximo de referencia en la
				escala Y. */
	double minEscY, maxEscY; /* Valores minimo y maximo ya escalados */
	int nDatos; /* numero de datos */
	bool Activa; /* false indica desactivada, por lo que no se dibujaria */
	
	int inicioX; /* indica el primer dato de esta grafica que aparece en el
						tapiz */
	
	NodoGrafica *Next;
	} NodoGrafica;

typedef struct tListGraficas {
	unsigned int numElements;
	NodoGrafica *First;
} tListGraficas;


/*****************************************************
 ***																***
 ***						GRAFICAS								***
 ***																***
 *****************************************************/

class cGrafica {
   /***********
    * GENERAL *
    ***********/
	public:
      cGrafica();
      ~cGrafica();
		void Inicializa(Widget padre, unsigned long colorFG,
					unsigned long colorBg, double limitPG);
		/* Inicializa el objeto y lo asocia a una superficie de trabajo */
		void CreaCursor(unsigned long colorSup, unsigned long colorInf);
		/* Crea un cursor con los colores pasados */
		void SetLimitsCursor(double cMiniX, double cMaxiX, double cMiniY, double cMaxiY);
		/* Marca los limites del tapiz en el que se dibuja el cursor */
		void DibujaCursor(double posX, double posY);
		/* Dibuja el cursor en la posicion X segnalada tomando a posY como
			el valor Y medio del canal */
		void Resetea();
		/* Borra todo el contenido de las graficas */
		void Resize(Dimension ancho, Dimension alto);
		
		//NUEVO
		void setInicio(int inicio);
		
		/* Adapta el nuevo tamagno del tapiz al indicado */
		void SetZoomX(double newZoomX);
		void SetZoomY(double newZoomY);
		
		double GetZoomX();
		
		/* Indica un cambio de zoom en el tapiz */
		void SetMinX(double newMinX);
		void SetMinY(double newMinY);
		/* Indica un movimiento en el tapiz. Se actualizan los nuevos
			minimos */
		void ShowTapiz();
		/* Copia el contenido del tapiz al widget padre */
		void ShowTapiz(int src_x, int src_y, int width, int height);
		/* Copia el rectangulo indicado del contenido del tapiz al widget padre */
		void UpdateTapiz();
		/* Redibuja los datos de las graficas en el tapiz */
		void ChangeScale(int newScale);
		/* Cambia la escala actual por otra nueva:
				1 -> Escala lineal
				2 -> Escala Raiz cuadrada
				3 -> Escala logaritmica */

	   /***************************
   	 * CONTROL DE LAS GRAFICAS *
	    ***************************/

		bool CreateNewGraphic(int priority, char *name, char keyNumber);
		/* Crea una nueva grafica con los datos pasados. Esta queda vacia e
			inactiva. Devuelve false si no ha conseguido crearla. */
		bool SeleccionaGrafica(char *name);
		bool SeleccionaGrafica(char number);
		/* Pone disponible la grafica identificada, bien mediante un nombre,
			bien mediante su numero clave */
		void SetApparience(unsigned long colorFg, unsigned long colorBg, int anchoLinea, char tipo);
		/* Establece la apariencia de la grafica. El ultimo parametro indica
			si es una grafica de puntos (0), o de lineas (1) */
		void SetActive(bool state);
		/* Activa (true) o desactiva (false) la grafica indicada */
		void SetGraphX(double graphMinX, double graphMaxX);
		/* Establece los valores maximos y minimos en la escala X en la
			que se englobara la grafica */
		void SetGraphY(double graphMinY, double graphMaxY);
		/* Establece los valores maximos y minimos en la escala Y en la que
			se englobara la grafica */
		bool PreparaDatos(int numDatos);
		/* Limpia los datos preexistentes y reserva memoria para los nuevos
			datos */
		double *pDatosX();
		/* Devuelve un puntero a los datos X */
		double *pDatosY();
		/* Devuelve un puntero a los datos Y */
		double *pDatosEscY();
		/* Devuelve un puntero a los datos Y escalados */
		void CopiaTerminada();
		/* Una vez se ha terminado de transvasar datos a la grafica, esta
			funcion se encarga de actualizar los datos para la presente
			representacion */

	private:
			// Geometria del tapiz
		double limitePG; /* valor limite para el punto gordo */
		double WidthDensity, HeightDensity; /* mm que ocupa un pixel */
		double zoomX, zoomY; /* zoom aplicado a la ventana de representacion */
		double minX, minY; /* valores minimos de X e Y que se representan
					en la ventana */
		double minEscY; /* Valor minimo de Y escalado */
		unsigned char escala; /*	1  ->  escala lineal
											2  ->  escala raiz cuadrada
											3  ->  escala logaritmica */
		int sizeX, sizeY; /* tamagno horizontal y vertical en pixels del tapiz */
		
			// Controladores Motif para el tapiz
		GC gcErase; // Graphic Context para borrar el tapiz
		GC gcCopy; // Graphic Context para copiar el tapiz
		XColor ColorFondo; // Color de fondo del tapiz
		Pixmap tapiz; // tapiz sobre el que se dibuja
		Widget Padre; // widget padre (util para la creacion de contextos */
		Display *dpy;
		
			// Graficas en si
		tListGraficas ListaGraficas;
		NodoGrafica *Seleccionado; // nodo actualmente seleccionado
		
			// Cursor
		GC gcCursorUp, gcCursorDown; // Graphic Context para el cursor
		double cursorMinX, cursorMaxX;
		double cursorMinY, cursorMaxY;
		bool hayCursor; // indica la existencia de cursor
		bool hayCursorLimits; // indica la existencia de limites para el cursor
			
		void BorraTodo(); // limpia todo lo que hay en el objeto
};

#endif
