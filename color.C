#include <stdio.h>
#include "color.h"

XColor color[18];
String colors[] = {"Black", "Red", "Green", "Blue", "White", "Navy", "Orange",
						"Yellow", "Pink", "Magenta", "Cyan", "Brown", "Grey",
						"LimeGreen", "Turquoise", "Violet", "Wheat", "Purple"};

void CargaColores(Display* dpy, Widget w) {
	XColor 	 unused;
	Colormap  cmap = DefaultColormapOfScreen(XtScreen(w));

	for (int i = Negro; i <= Purpura; i++) {
		if (!XAllocNamedColor(dpy, cmap, colors[i], &color[i], &unused)) {
			printf("Can't alloc %s\n", colors[i]);
		}
	}
}

