#ifndef __CURSOR_H__
#define __CURSOR_H__

#include <Xm/Xm.h>
// esta esctructura guarda datos para utilizar en el cursor
class cCursor  {
   /*************
    *   DATOS   *
    *************/
   private:
		unsigned int anchoRealD, altoRealD; /* tamagno de la superficie total en
			pixels (el tapiz virtual en pixels). Se supone el origen en (0,0) en
			la esquina inferior izquierda */
		unsigned int anchoReal, altoReal; /* tamagno de la superficie de dibujo
			real en pixels */
		unsigned int posMinX, posMinY; /* posicion de la esquina inferior
			izquierda de la ventana de visualizacion en el tapiz en pixels */
		unsigned int cursorPX; // posicion en pixels del cursor
		int cursor; // posicion en unidades de datos del cursor
		GC gcUp; // para dibujar por encima del numero de cuentas
		GC gcDown; // para dibujar por debajo del numero de cuentas
   public:
		int maximo, minimo; /* valores maximo y minimo que puede tomar en cursor
			en unidades de datos */
		double anchoVirtual; /* tamagno del tapiz donde se pone
			este cursor */
		GC gc; // GC para dibujar en la ventana

   /************************
    * Control de los datos *
    ************************/

   public:
		unsigned int  posicion(); /* devuelve la posicion del cursor */
		int CambiaCursor(unsigned int); /* indica la nueva posicion del cursor
			en unidades de datos */
		int CambiaCursorPX(unsigned int); /* indica la nueva posicion del cursor
			dando el punto x donde se pulsa de la ventana de visualizacion */

   /*********************
    * Control del tapiz *
    *********************/

   public:
		void DefineVentanaTotal(unsigned int, unsigned int); /* define el tamagno de la ventana tapiz
			en pixels */
		void DefineAnchoTotal(double); /* define el ancho de la ventana tapiz
			en pixels */
		void DefineAltoTotal(double); /* define el alto de la ventana tapiz
			en pixels */
		void DefineVentanaReal(unsigned int, unsigned int); /* define el tamagno de la ventana que
			sirve para "mirar" el tapiz virtual */
		void DefinePosicionVentanaReal(unsigned int, unsigned int); /* coloca la
			ventana de visualizacion en una posicion del tapiz virtual en pixels */
		void DefinePosicionXReal(double); /* coloca la coordenada
			X de la ventana de visualizacion en una posicion del tapiz virtual en pixels */
		void DefinePosicionYReal(double); /* coloca la coordenada
			Y de la ventana de visualizacion en una posicion del tapiz virtual en pixels */
		void CreaGC(Display *, Widget); /* crea un GC para poder dibujar */
		void DibujaCursor(Display *, Pixmap, Window, int); /* dibuja el cursor en el pixmap
			tapiz virtual y en la ventana de visualizacion */

	private:
		void CambiaVentana(); /* Calcula la posicion X del cursor para la
			ventana tapiz */


   /***********
    * GENERAL *
    ***********/
   public:
      cCursor();
      ~cCursor();

};

#endif
