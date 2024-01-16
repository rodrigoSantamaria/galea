#include <stdio.h>
#include <math.h>
#include "Graficas.h"
#include "strings1.h"
#include "math1.h"

/*****************************************************
 ***												***
 ***						INTERNAL LISTS			***
 ***												***
 *****************************************************/

//********************** ADD NODO GRÁFICA *******************
NodoGrafica *AddNodoGrafica(tListGraficas *list, int Priority) 
	{
	NodoGrafica *nuevo = new NodoGrafica;
	if (nuevo == NULL) 
		{
		printf("ERROR: No hay memoria para reservar NodoGrafica\n");
		return NULL; // memory failure
		}
	
	// initialize dates
	nuevo->Priority = Priority;
	nuevo->keyName = NULL; nuevo->keyNumber = 0;
	nuevo->DatosX = NULL; nuevo->DatosY = NULL;
	nuevo->DatosEscY = NULL; nuevo->nDatos = 0;
	nuevo->estilo = 0;
	nuevo->Activa = false;
	nuevo->gcDraw = NULL; nuevo->inicioX = 0;
	nuevo->minX = 0.0; nuevo->maxX = 0.0;
	nuevo->minY = 0.0; nuevo->maxY = 0.0;
	nuevo->minEscY = 0.0;
	nuevo->minEscY = 0.0; nuevo->maxEscY = 0.0;
	
	NodoGrafica **guide = &(list->First);
	while ((*guide) != NULL) 
		{
		if ((*guide)->Priority > nuevo->Priority) 
			{
			// Priority Orden
			nuevo->Next = *guide;
			*guide = nuevo;
			list->numElements++;
			return nuevo;
			}
		// pass to next element
		guide = &((*guide)->Next);
		}
	// put element in the last place of the list
	nuevo->Next = NULL;
	*guide = nuevo;
	list->numElements++;
	return nuevo;
	}

//************** GET NODO GRAFICA ******************
/** Devuelve el nodo que tiene el número o nombre especificado dentro de list
*/
NodoGrafica *GetNodoGrafica(tListGraficas *list, char number) 
	{
	NodoGrafica **guide = &(list->First);
	while ((*guide) != NULL) 
		{
		if ((*guide)->keyNumber == number)
			return (*guide);
		// pass to next element
		guide = &((*guide)->Next);
		}
	return NULL;
	}

NodoGrafica *GetNodoGrafica(tListGraficas *list, char *name) 
	{
	NodoGrafica **guide = &(list->First);
	while ((*guide) != NULL) 
		{
		if (strcmp((*guide)->keyName, name) == 0)	return (*guide);
		// pass to next element
		guide = &((*guide)->Next);
		}
	return NULL;
	}

//*************** DELETE LIST **********************
void DeleteList(tListGraficas *list, Display *dsp) 
	{
	printf("DELETE LIST\n");
/*	NodoGrafica **guide = &(list->First);
	while ((*guide) != NULL) 
		{
		NodoGrafica *delNode = (*guide);
		
		// pass to next element
		guide = &((*guide)->Next);
		// clean internal data
		if (delNode->gcDraw != NULL) XFreeGC(dsp, delNode->gcDraw);
		
		delete [] delNode->keyName;
		delete [] delNode->DatosX; 
		delete [] delNode->DatosY;
		delete [] delNode->DatosEscY;
		delete delNode;
		}		
	list->First = NULL;
	list->numElements = 0;
		*/
	//NOTA: No sé por qué lo complicaba tanto ahí arriba, 
	//		así parece que funciona bien, sin punteros dobles
	NodoGrafica *guide=list->First;
	while(guide!=NULL)
		{
		NodoGrafica *delNode=guide;
		guide=guide->Next;
		if (delNode->gcDraw != NULL) XFreeGC(dsp, delNode->gcDraw);
		
		delete [] delNode->keyName;
		delete [] delNode->DatosX; 
		delete [] delNode->DatosY;
		delete [] delNode->DatosEscY;
		delete delNode;
		}		
	list->First = NULL;
	list->numElements = 0;
	}

/*****************************************************
 ***																***
 ***						GRAFICAS								***
 ***																***
 *****************************************************/
/* ***************************
	**     General Methods   **
	***************************/
	// Constructors
cGrafica::cGrafica() {
	limitePG = 1.0;
	WidthDensity = 0.38; // 17" monitor with 800x600 resolution
	HeightDensity = 0.38; // 17" monitor with 800x600 resolution
	zoomX = 1.0; zoomY = 1.0;
	minX = 0.0; minY = 0.0;
	sizeX = 0; sizeY = 0;
	
	ListaGraficas.numElements = 0;
	ListaGraficas.First = NULL;
	Seleccionado = NULL;
	
	hayCursor = false;
}

	// Destructor
cGrafica::~cGrafica() 
	{
	BorraTodo();
	}

//************* BORRA TODO *********
void cGrafica::BorraTodo() 
	{
	DeleteList(&ListaGraficas, dpy);
	Seleccionado = NULL;
	if (hayCursor) 
		{
		XFreeGC(dpy, gcCursorUp);
		XFreeGC(dpy, gcCursorDown);
		hayCursor = false;
		}
	hayCursorLimits = false;
	}

//*********** RESETEA *************
void cGrafica::Resetea() 
	{
	BorraTodo();
	}

//***************** INICIALIZA **********************
/*	Pone color de fondo y de frente de la gráfica y establece
	el tamaño de los puntos.
	NOTA: Con el limitPG no hace absolutamente nada!
	*/
void cGrafica::Inicializa(Widget padre, unsigned long colorFg,
			unsigned long colorBg, double limitPG) 
	{
	Padre = padre;
	dpy = XtDisplay(padre);
	Screen *screen = XtScreen(padre);
	
	// datos tecnicos sobre la pantalla
	WidthDensity = WidthMMOfScreen(screen)/(double)WidthOfScreen(screen);
	HeightDensity = HeightMMOfScreen(screen)/(double)HeightOfScreen(screen);

	// tapiz virtual para dibujar (lo inicializa)
	tapiz = XCreatePixmap(dpy, RootWindowOfScreen (screen),
								1, 1,
								DefaultDepthOfScreen(screen));
	sizeX = 1; sizeY = 1;

	// Crea los contextos graficos
	XGCValues gcv;
	gcv.foreground = colorBg;
	gcv.background = colorBg;
	gcErase = XCreateGC(XtDisplay (padre), RootWindowOfScreen(screen),
				GCForeground|GCBackground, &gcv);
	gcv.foreground = colorFg;
	gcCopy = XCreateGC(XtDisplay (padre), RootWindowOfScreen(screen),
				GCForeground|GCBackground, &gcv);
				
	// inicializa otros datos (por si las moscas)
	Seleccionado = NULL;
}

void cGrafica::Resize(Dimension ancho, Dimension alto) 
	{
	// Libera el anterior tapiz
	XFreePixmap(dpy, tapiz);
	// Crea una nuevo del nuevo tamagno
	tapiz = XCreatePixmap(dpy, RootWindowOfScreen(XtScreen(Padre)),
				ancho, alto,
				DefaultDepthOfScreen(XtScreen(Padre)));
	// limpia el tapiz
	XFillRectangle (dpy, tapiz, gcErase, 0, 0, ancho, alto);
	// actualiza los valores del tapiz
	sizeX = (int) ancho; sizeY = (int) alto;
	}

void cGrafica::ShowTapiz() 
	{
	XCopyArea(dpy, tapiz, XtWindow(Padre), gcCopy, 0, 0, sizeX, sizeY, 0, 0);
	}

void cGrafica:: ShowTapiz(int src_x, int src_y, int width, int height) 
	{
	XCopyArea(dpy, tapiz, XtWindow(Padre), gcCopy, src_x, src_y,
				width, height, src_x, src_y);
	}

//*************** CREATE NEW GRAPHIC ***************
bool cGrafica::CreateNewGraphic(int priority, char *name, char keyNumber) 
	{
	// crea un nuevo nodo para esta grafica
	NodoGrafica *nodo = AddNodoGrafica(&ListaGraficas, priority);
	if (nodo == NULL) return false;
	// rellena los campos con los datos para la identificacion de la grafica
	nodo->keyName = CopyString(name);
	nodo->keyNumber = keyNumber;
	
	// por ser el ultimo nodo creado lo deja como seleccionado
	// (es muy probable que ahora se quiera rellenar el resto de campos)
	Seleccionado = nodo;
	
	return true;
	}

//*************** SELECCIONA GRAFICA *****************
//Seleccionamos una de las gráficas, bien por número o por nombre
//NOTA: char number???
bool cGrafica::SeleccionaGrafica(char number) 
	{
		//NUEVO
		//NOTA: En general soluciona el problema que teníamos con preparaDatos,
		//		pero en algunos casos nos da fallo! -> fbege155.asf.
		//		El fallo ocurre simplemente por hacer el new NodoGrafica!
	if (Seleccionado !=NULL)	
		Seleccionado=NULL;
	Seleccionado = GetNodoGrafica(&ListaGraficas, number);
//	printf("Hemos seleccionado %d-%s\n", Seleccionado->keyNumber, Seleccionado->keyName);
	if (Seleccionado == NULL) return false;
	return true;
	}

bool cGrafica::SeleccionaGrafica(char *name) 
	{
	Seleccionado = GetNodoGrafica(&ListaGraficas, name);
	if (Seleccionado == NULL) {printf("no hay nodo seleccionado\n");return false;}
	return true;
	}
//***************************



void cGrafica::SetZoomX(double newZoomX) 
	{
	zoomX = newZoomX;
	}

void cGrafica::SetZoomY(double newZoomY) 
	{
	zoomY = newZoomY;
	}

double cGrafica::GetZoomX()
	{
	return zoomX;
	}
		 
	
void cGrafica::SetMinX(double newMinX) 
	{
	minX = newMinX;
	// Busca el primer elemento que aparece en el tapiz (segun el nuevo
	// valor minimo) de todas las graficas disponibles
	NodoGrafica **guide = &(ListaGraficas.First);
	while ((*guide) != NULL) 
		{
		// search first element viewable
		if (((*guide)->DatosX != NULL)&&((*guide)->Activa)) 
			{
			// optimization: start search in actual value of start
			if ((*guide)->inicioX >= (*guide)->nDatos)
				(*guide)->inicioX = 0;
			else if ((*guide)->DatosX[(*guide)->inicioX] >= newMinX)
				(*guide)->inicioX = 0;
			while (((*guide)->inicioX < (*guide)->nDatos) && ((*guide)->DatosX[(*guide)->inicioX] < newMinX))
				(*guide)->inicioX++;
			}
		// pass to next graphic
		guide = &((*guide)->Next);
		}	
		
	}

void cGrafica::SetMinY(double newMinY) 
	{
	minY = newMinY;
	switch (escala) 
		{
		case 2:
			minEscY = sqrt(newMinY);
			break;
		case 3:
			minEscY = log010(newMinY);
			break;
		default:
			minEscY = newMinY;
			break;
		}
	}

//NUEVO: ***************** SET INICIO ***************
// Establece el primer canal en el que se va a escribir
void cGrafica::setInicio(int inicio)
	{
	if(Seleccionado != NULL)	Seleccionado->inicioX=inicio;
	}

//****************** SET APPARIENCE *******************
/*	Establece la apariencia del gráfico: color de fondo, de frente, ancho de líneas y estilo
	*/
void cGrafica::SetApparience(unsigned long colorFg, unsigned long colorBg, int anchoLinea,
			char tipo) 
	{
	if (Seleccionado != NULL) 
		{
		Seleccionado->estilo = tipo;

		XGCValues gcv;
		gcv.foreground = colorFg;
		gcv.background = colorBg;
		gcv.line_width = anchoLinea;
		Seleccionado->gcDraw = XCreateGC(dpy, RootWindowOfScreen(XtScreen(Padre)), GCForeground|GCBackground|GCLineWidth, &gcv);
		}
	}

//****************** CHANGE SCALE **********************
//Cambia la escala del eje de la y. 
//"newScale" nos dice el tipo de escala al que lo cambiaremos:
//	2->Cuadrática, 3->Logarítmica (base 10), cualquier otro valor->Lineal.

void cGrafica::ChangeScale(int newScale) 
	{
	escala = newScale;
	
	//Calculamos los valores mínimos de la y al escalar.
	switch (escala) 
		{
		case 2:
			minEscY = sqrt(minY);
			break;
		case 3:
			minEscY = log010(minY);
			break;
		default:
			minEscY = minY;
			break;
		}
	
	// Convierte todos los valores de las graficas a la nueva escala
	NodoGrafica **guide = &(ListaGraficas.First);//NOTA: Ojo->Cada nodo es una gráfica completa!
	//NOTA: No sé para qué necesitamos una lista de gráficas, ¿no nos valdría con una sola?
	while ((*guide) != NULL)//mientras tengamos nodos en la lista 
		{
		if (((*guide)->DatosY != NULL)&&((*guide)->nDatos > 0)) 
			{
			// Circula por todos los datos pasándolos a la nueva escala
			switch(escala) 
				{
				case 2: // Escala raiz cuadrada
					for (int i = 0; i < (*guide)->nDatos; i++)
						(*guide)->DatosEscY[i] = sqrt(fabs((*guide)->DatosY[i]));
					(*guide)->minEscY = sqrt(fabs((*guide)->minY));
					(*guide)->maxEscY = sqrt(fabs((*guide)->maxY));
					break;
				case 3: // Escala logaritmica
					for (int i = 0; i < (*guide)->nDatos; i++)
						(*guide)->DatosEscY[i] = log010(fabs((*guide)->DatosY[i]));
					(*guide)->minEscY = log010(fabs((*guide)->minY));
					(*guide)->maxEscY = log010(fabs((*guide)->maxY));
					break;
				default:
					for (int i = 0; i < (*guide)->nDatos; i++)
						(*guide)->DatosEscY[i] = (*guide)->DatosY[i];
					(*guide)->minEscY = (*guide)->minY;
					(*guide)->maxEscY = (*guide)->maxY;
					break;
				}
			}
		// pasamos al siguiente gráfico
		guide = &((*guide)->Next);
		}		
	}

//******************** SET ACTIVE **************************
void cGrafica::SetActive(bool state) 
	{
	if (Seleccionado != NULL) 
		{
//		printf("Ponemos gráfica seleccionada a %d (es %d, %s)\n", state, Seleccionado->keyNumber, Seleccionado->keyName);
		Seleccionado->Activa = state;
		if ((state)&&(Seleccionado->nDatos > 0)) 
			{
	//		printf("Actualizamos el valor mínimo a mostrar\n");
			// actualiza el valor minimo a mostrar
			if (Seleccionado->DatosX[Seleccionado->inicioX] >= minX)
				Seleccionado->inicioX = 0;
			while ((Seleccionado->inicioX < Seleccionado->nDatos)&&
					(Seleccionado->DatosX[Seleccionado->inicioX] < minX))
				Seleccionado->inicioX++;			
			}
		//printf("El estado de la gráfica %s,%d es %d\n", Seleccionado->keyName, Seleccionado->keyNumber, Seleccionado->Activa);
		}
	}

//************* UPDATE TAPIZ ******************
/* Recorre todos los gráficos que se tienen que 
	pintar y los repinta.
	*/
void cGrafica::UpdateTapiz() 
	{
	// limpia el tapiz
	XFillRectangle (dpy, tapiz, gcErase, 0, 0, sizeX, sizeY);
	// Va pasando por todas las graficas hasta pintarlas
	NodoGrafica *guide = ListaGraficas.First;
		
	int i=0;
	while (guide != NULL)
		{
		// Check if graphic is viewable
//		printf("Gráfica %d: %s-%d\n", i++, guide->keyName, guide->keyNumber);
		//if(guide->keyNumber==29 && guide->DatosX!=NULL)	guide->Activa=true;
//		printf("Activa a %d\n", guide->Activa);
//		if((guide->DatosX != NULL))	printf("Tiene datos\n");
		if ((guide->DatosX != NULL)&&(guide->Activa)) 
			{
	//		printf("gráfica %d tiene datos y está activa\n", i);
			double factorX = sizeX*zoomX/(guide->maxX - guide->minX);
			double factorY = sizeY*zoomY/(guide->maxEscY - guide->minEscY);
			// Calcula el factor de punto gordo
			double factorPG = factorX*WidthDensity/0.5; // 1 punto cada 0'5 mm minimo, sino -> punto gordo
			factorPG = sqrt(factorPG*factorPG);// + factorY*HeightDensity/0.5);
			// Empieza a dibujar desde el primer valor segnalado
			int i = guide->inicioX;
			
			if (i >= guide->nDatos) 
				{
				// pass to next graphic
				guide = guide->Next;
				continue;
				}
				
			if (guide->estilo == 0) 
				{
				// Dibuja puntos
					
				int posX = (int)((guide->DatosX[i] - minX)*factorX);
				// Para optimizar la representacion se repite todo el codigo
				// para punto fino y para punto gordo
//NOTA: En periodo de optimización, para un visionado óptimo
//de los puntos. Las únicas representaciones que dan buena
//respuesta son el punto, el círculo de 3 (equivalente al rectángulo de 2)
//y el rectángulo de 3. 	
//Se podría hacer una última amplicación a rectángulos de 4 a un límite de
//10*límite o algo así.					
//				if (factorPG <= 1.5*limitePG) 
				if (false) 
					{
					// Punto fino
					while (posX < sizeX) 
						{
						int posY = sizeY - 1 - (int)((guide->DatosEscY[i] - guide->minEscY - minEscY)*factorY);
						if ((posY >= 0)&&(posY <= sizeY))
							XDrawPoint (dpy, tapiz, guide->gcDraw, posX, posY);
						i++;
						if (i >= guide->nDatos)
							break;
						posX = (int)((guide->DatosX[i] - minX)*factorX);
						}
					} 
				else 
					{
					// Punto Gordo
					if(factorPG <= 5*limitePG)
						{
						while (posX < sizeX) 
							{
							int posY = sizeY - 1 - (int)((guide->DatosEscY[i] - guide->minEscY - minEscY)*factorY);
							if ((posY >= 0)&&(posY <= sizeY))
								XFillRectangle (dpy, tapiz, guide->gcDraw, posX-1, posY-1, 2, 2);
							i++;
							if (i >= guide->nDatos)
								break;
							posX = (int)((guide->DatosX[i] - minX)*factorX);
							}
						}
					else
						{
						while (posX < sizeX) 
							{
							int posY = sizeY - 1 - (int)((guide->DatosEscY[i] - guide->minEscY - minEscY)*factorY);
							if ((posY >= 0)&&(posY <= sizeY))
								XFillRectangle (dpy, tapiz, guide->gcDraw, posX-1, posY-1, 3, 3);
							i++;
							if (i >= guide->nDatos)
								break;
							posX = (int)((guide->DatosX[i] - minX)*factorX);
							}
						}
					}
				}	
			else 
				{
				// Dibuja Lineas
				if (i > 0) i--; // pone el punto anterior para dibujar la recta desde fuera
				int posX1 = (int)((guide->DatosX[i] - minX)*factorX);
				int posY1 = sizeY - 1 - (int)((guide->DatosEscY[i] - guide->minEscY - minEscY)*factorY);
				i++;
				
				if (i < guide->nDatos) 
					{
					int posX2 = (int)((guide->DatosX[i] - minX)*factorX);
					while (posX1 < sizeX) 
						{
						int posY2 = sizeY - 1 - (int)((guide->DatosEscY[i] - guide->minEscY - minEscY)*factorY);
						if (guide->DatosX[i] - guide->DatosX[i-1] < 1.5) // condicion de linea continua
							if (!(((posY1 > sizeY)&&(posY2 > sizeY))||
 								  ((posY1 < 0)&&(posY2 < 0))))
								XDrawLine (dpy, tapiz, guide->gcDraw, posX1, posY1, posX2, posY2);
						i++;
						if (i >= guide->nDatos)		break;
						posX1 = posX2;
						posY1 = posY2;
						posX2 = (int)((guide->DatosX[i] - minX)*factorX);
						}
					}
				}
			}//if(graphic viewable)
		// pass to next graphic
		guide = guide->Next;
		}
	}

	/*
void cGrafica::UpdateTapiz() 
	{
	// limpia el tapiz
	XFillRectangle (dpy, tapiz, gcErase, 0, 0, sizeX, sizeY);
	// Va pasando por todas las graficas hasta pintarlas
	NodoGrafica **guide = &(ListaGraficas.First);
	
	int i=0;
	while ((*guide) != NULL) 
		{
		// Check if graphic is viewable
		printf("Gráfica %d: %s-%d\n", i++, (*guide)->keyName, (*guide)->keyNumber);
		printf("Activa a %d\n", (*guide)->Activa);
//		if(((*guide)->DatosX != NULL))	printf("Tiene datos\n");
		if (((*guide)->DatosX != NULL)&&((*guide)->Activa)) 
			{
	//		printf("gráfica %d tiene datos y está activa\n", i);
			double factorX = sizeX*zoomX/((*guide)->maxX - (*guide)->minX);
			double factorY = sizeY*zoomY/((*guide)->maxEscY - (*guide)->minEscY);
			// Calcula el factor de punto gordo
			double factorPG = factorX*WidthDensity/0.5; // 1 punto cada 0'5 mm minimo, sino -> punto gordo
			factorPG = sqrt(factorPG*factorPG);// + factorY*HeightDensity/0.5);
			// Empieza a dibujar desde el primer valor segnalado
			int i = (*guide)->inicioX;
			
			if (i >= (*guide)->nDatos) 
				{
				// pass to next graphic
				guide = &((*guide)->Next);
				continue;
				}
				
			if ((*guide)->estilo == 0) 
				{
				// Dibuja puntos
					
				int posX = (int)(((*guide)->DatosX[i] - minX)*factorX);
				// Para optimizar la representacion se repite todo el codigo
				// para punto fino y para punto gordo
//NOTA: En periodo de optimización, para un visionado óptimo
//de los puntos. Las únicas representaciones que dan buena
//respuesta son el punto, el círculo de 3 (equivalente al rectángulo de 2)
//y el rectángulo de 3. 	
//Se podría hacer una última amplicación a rectángulos de 4 a un límite de
//10*límite o algo así.					
				if (factorPG <= 1.5*limitePG) 
					{
					// Punto fino
					while (posX < sizeX) 
						{
						int posY = sizeY - 1 - (int)(((*guide)->DatosEscY[i] - (*guide)->minEscY - minEscY)*factorY);
						if ((posY >= 0)&&(posY <= sizeY))
							XDrawPoint (dpy, tapiz, (*guide)->gcDraw, posX, posY);
						i++;
						if (i >= (*guide)->nDatos)
							break;
						posX = (int)(((*guide)->DatosX[i] - minX)*factorX);
						}
					} 
				else 
					{
					// Punto Gordo
					if(factorPG <= 5*limitePG)
						{
						while (posX < sizeX) 
							{
							int posY = sizeY - 1 - (int)(((*guide)->DatosEscY[i] - (*guide)->minEscY - minEscY)*factorY);
							if ((posY >= 0)&&(posY <= sizeY))
								XFillRectangle (dpy, tapiz, (*guide)->gcDraw, posX-1, posY-1, 2, 2);
							i++;
							if (i >= (*guide)->nDatos)
								break;
							posX = (int)(((*guide)->DatosX[i] - minX)*factorX);
							}
						}
					else
						{
						while (posX < sizeX) 
							{
							int posY = sizeY - 1 - (int)(((*guide)->DatosEscY[i] - (*guide)->minEscY - minEscY)*factorY);
							if ((posY >= 0)&&(posY <= sizeY))
								XFillRectangle (dpy, tapiz, (*guide)->gcDraw, posX-1, posY-1, 3, 3);
							i++;
							if (i >= (*guide)->nDatos)
								break;
							posX = (int)(((*guide)->DatosX[i] - minX)*factorX);
							}
						}
					}
				}	
			else 
				{
				// Dibuja Lineas
				if (i > 0) i--; // pone el punto anterior para dibujar la recta desde fuera
				int posX1 = (int)(((*guide)->DatosX[i] - minX)*factorX);
				int posY1 = sizeY - 1 - (int)(((*guide)->DatosEscY[i] - (*guide)->minEscY - minEscY)*factorY);
				i++;
				
				if (i < (*guide)->nDatos) 
					{
					int posX2 = (int)(((*guide)->DatosX[i] - minX)*factorX);
					while (posX1 < sizeX) 
						{
						int posY2 = sizeY - 1 - (int)(((*guide)->DatosEscY[i] - (*guide)->minEscY - minEscY)*factorY);
						if ((*guide)->DatosX[i] - (*guide)->DatosX[i-1] < 1.5) // condicion de linea continua
							if (!(((posY1 > sizeY)&&(posY2 > sizeY))||
 								  ((posY1 < 0)&&(posY2 < 0))))
								XDrawLine (dpy, tapiz, (*guide)->gcDraw, posX1, posY1, posX2, posY2);
						i++;
						if (i >= (*guide)->nDatos)		break;
						posX1 = posX2;
						posY1 = posY2;
						posX2 = (int)(((*guide)->DatosX[i] - minX)*factorX);
						}
					}
				}
			}//if(graphic viewable)
		// pass to next graphic
		guide = &((*guide)->Next);
		}
	}
	*/

void cGrafica::SetGraphX(double graphMinX, double graphMaxX) 
	{
	if (Seleccionado != NULL) 
		{
		Seleccionado->minX = graphMinX;
		Seleccionado->maxX = graphMaxX;
		}
	}

void cGrafica::SetGraphY(double graphMinY, double graphMaxY) 
	{
	if (Seleccionado != NULL) 
		{
		Seleccionado->minY = graphMinY;
		Seleccionado->maxY = graphMaxY;
		switch(escala) 
			{
			case 2: // Escala raiz cuadrada
				Seleccionado->minEscY = sqrt(fabs(Seleccionado->minY));
				Seleccionado->maxEscY = sqrt(fabs(Seleccionado->maxY));
				break;
			case 3: // Escala logaritmica
				Seleccionado->minEscY = log010(fabs(Seleccionado->minY));
				Seleccionado->maxEscY = log010(fabs(Seleccionado->maxY));
				break;
			default:
				Seleccionado->minEscY = Seleccionado->minY;
				Seleccionado->maxEscY = Seleccionado->maxY;
				break;
			}
		}
	}

//********************* PREPARA DATOS ****************************
// Reserva memoria para todos los datos
// NOTA: Función Conflictiva, muy a menudo tenemos  Violaciones de Segmento por su culpa
bool cGrafica::PreparaDatos(int numDatos) 
	{
//	printf("PREPARA DATOS\n");
	if (Seleccionado != NULL) 
		{
		// borra los datos que hubiera antes
		// NOTA: Podemos llegar a funcionar sin hacer este borrado previo y
		//		parece que tenemos menos errores, pero aumenta el memory leaking
		//		de forma alarmante. Quizá también el uso abusivo que hacemos de
		//		ella la lleve a petar en algún momento (debido a las gráficas de
		//		salida en tiempo real del mejor individuo -> numGaussianas+1 llamadas
		//		por generación)
		//NOTA:	Lo que parece que pasa es que esta función, por la cantidad de memoria
		//		que maneja, es un detector muy bueno de errores en ejecución con respecto
		//		a la memoria en otras zonas del programa. Si peta aquí, pasar el valgrind
		//		y seguro que acemos algún acceso incorrecto de R/W
	//	delete [] Seleccionado->keyName;
	//	printf("Datos X\n");
		if(Seleccionado->DatosX!=NULL)		
			delete [] Seleccionado->DatosX;
	//	printf("DatosY\n");
		if(Seleccionado->DatosY!=NULL)		
			delete [] Seleccionado->DatosY;
	//	printf("DatosEscY\n");
		if(Seleccionado->DatosEscY!=NULL)	
			delete [] Seleccionado->DatosEscY;
		Seleccionado->Activa = false;
		Seleccionado->inicioX = 0;
		//
		if (numDatos > 0) 
			{
			// Reserva la memoria para los nuevos datos
		//	printf("Reservamos la memoria: DatosX\n");
			Seleccionado->DatosX = new double [numDatos];
		//	printf("Reservamos la memoria: DatosY\n");
			Seleccionado->DatosY = new double [numDatos];
		//	printf("Reservamos la memoria: DatosEscY\n");
			Seleccionado->DatosEscY = new double [numDatos];
			if ((Seleccionado->DatosX == NULL)||(Seleccionado->DatosY == NULL)||(Seleccionado->DatosEscY == NULL)) 
				{
//				printf("ERROR: No hay memoria para reservar DatosX, DatosY, DatosEscY\n");

		//		delete [] Seleccionado->keyName;	
				delete [] Seleccionado->DatosX;
				delete [] Seleccionado->DatosY;
				delete [] Seleccionado->DatosEscY;
				Seleccionado->nDatos = 0;
				Seleccionado->DatosX = NULL;
				Seleccionado->DatosY = NULL;
				Seleccionado->DatosEscY = NULL;
				return false;
				}
	//		printf("Fin de la reserva\n");
			}
		//Lo que queremos es borrarlos
		else 	
			{
	//		Seleccionado->keyName = NULL;
			Seleccionado->DatosX = NULL;
			Seleccionado->DatosY = NULL;
			Seleccionado->DatosEscY = NULL;
			}
		Seleccionado->nDatos = numDatos;
//		printf("Fin de preparadatos\n");
		return true;
		}
//		printf("Fin de preparadatos\n");
	return false;
	}

//************************** COPIA TERMINADA **************************
// Rellena el vector con la información de la Y escalada a la raiz cuadrada,
// el logaritmo o escala lineal
void cGrafica::CopiaTerminada() 
	{
	if (Seleccionado != NULL) 
		{
		// Reconstruye los datos escalados para esta seleccion
		for(int i=0;i<Seleccionado->nDatos;i++)	
			Seleccionado->DatosEscY[i]=0;
		switch(escala) 
			{
			case 2: // Escala raiz cuadrada
				for (int i = 0; i < Seleccionado->nDatos; i++)
					Seleccionado->DatosEscY[i] = sqrt(fabs(Seleccionado->DatosY[i]));
				Seleccionado->minEscY = sqrt(fabs(Seleccionado->minY));
				Seleccionado->maxEscY = sqrt(fabs(Seleccionado->maxY));
				break;
			case 3: // Escala logaritmica
				for (int i = 0; i < Seleccionado->nDatos; i++)
					Seleccionado->DatosEscY[i] = log010(fabs(Seleccionado->DatosY[i]));
				Seleccionado->minEscY = log010(fabs(Seleccionado->minY));
				Seleccionado->maxEscY = log010(fabs(Seleccionado->maxY));
				break;
			default: // Escala lineal
				for (int i = 0; i < Seleccionado->nDatos; i++)
					Seleccionado->DatosEscY[i] = Seleccionado->DatosY[i];
				Seleccionado->minEscY = Seleccionado->minY;
				Seleccionado->maxEscY = Seleccionado->maxY;
				break;
			}
		}
	}

double *cGrafica::pDatosX() 
	{
	if (Seleccionado != NULL)	return Seleccionado->DatosX;
	else				return NULL;
	}

double *cGrafica::pDatosY() 
	{
	if (Seleccionado != NULL)	return Seleccionado->DatosY;
	else				return NULL;
	}

double *cGrafica::pDatosEscY() 
	{
	if (Seleccionado != NULL)	return Seleccionado->DatosEscY;
	else				return NULL;
	}

void cGrafica::CreaCursor(unsigned long colorSup, unsigned long colorInf) 
	{
	XGCValues gcv;
	
	gcv.line_style = LineOnOffDash;
	gcv.line_width = 2;
	gcv.background = ColorFondo.pixel;

	gcv.foreground = colorSup;
	gcCursorUp = XCreateGC(dpy, RootWindowOfScreen(XtScreen(Padre)),
		GCForeground|GCBackground|GCLineStyle|GCLineWidth, &gcv);

	gcv.foreground = colorInf;
	gcCursorDown = XCreateGC(dpy, RootWindowOfScreen(XtScreen(Padre)),
		GCForeground|GCBackground|GCLineStyle|GCLineWidth, &gcv);
	
	hayCursor = true;
	}

void cGrafica::SetLimitsCursor(double cMinX, double cMaxX, double cMinY, double cMaxY) {
	cursorMinX = cMinX;
	cursorMaxX = cMaxX;
	cursorMinY = cMinY;
	cursorMaxY = cMaxY;
	hayCursorLimits = true;	
}

void cGrafica::DibujaCursor(double posX, double posY) 
	{
	if ((hayCursor)&&(hayCursorLimits)) 
		{
		double cMinEscY, cMaxEscY;
		switch (escala) 
			{
			case 2:
				cMinEscY = sqrt(cursorMinY);
				cMaxEscY = sqrt(cursorMaxY);
				posY = sqrt(fabs(posY));
				break;
			case 3:
				cMinEscY = log010(cursorMinY);
				cMaxEscY = log010(cursorMaxY);
				posY = log010(fabs(posY));
				break;
			default:
				cMinEscY = cursorMinY;
				cMaxEscY = cursorMaxY;
				break;
			}
		// Dibuja una linea vertical en la posX pasada
		double factorX = sizeX*zoomX/(cursorMaxX - cursorMinX);
		double factorY = sizeY*zoomY/(cMaxEscY - cMinEscY);
		int posicX = (int)((posX - minX)*factorX);
		if ((posicX >= 0)&&(posicX < sizeX)) 
			{
			int posicY = sizeY - 1 - (int)((posY - cMinEscY - minEscY)*factorY);
			if ((posicY >= 0)&&(posicY <= sizeY)) 
				{
				// Estan las dos partes del cursor
				XDrawLine(dpy, XtWindow(Padre), gcCursorUp, posicX, 0, posicX, posicY);
				XDrawLine(dpy, XtWindow(Padre), gcCursorDown, posicX, posicY+1, posicX, sizeY);
				}
			else if (posicY < 0)
				// solo esta la parte superior del cursor
				XDrawLine(dpy, XtWindow(Padre), gcCursorDown, posicX, 0, posicX, sizeY);
			else
				// esta solo la parte inferior del cursor
				XDrawLine(dpy, XtWindow(Padre), gcCursorUp, posicX, 0, posicX, sizeY);
			}
		}
	}
