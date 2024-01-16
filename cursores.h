#ifndef __CURSORES_H__
#define __CURSORES_H__

// Definicion de cursores
extern void PreparaCursores();
			/* inicializa los cursores disponibles (necesario antes de llamar
				al resto de funciones. */
extern void PonCursorNormal();
			// Pone un cursor normal en la ventana global (la tipica flecha)
extern void PonCursorEspera();
			// Pone un cursor de espera en la ventana global (el reloj)

#endif
