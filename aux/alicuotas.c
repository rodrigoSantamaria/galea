#include <stdio.h>
#include <stdlib.h>

#include "math1.h"
int main()
	{
	int max=4;
	float e[]= {1.33,1.032};
	float se[]={0.10,0.045};
	float alicuota=0.179;
	
	for(int i=0;i<max;i++)
		{
		char texto[20];
		PresentaNumero(e[i]/alicuota, se[i]/alicuota, texto);
		
		printf("%s\t\t\t%f(%f)\n", texto, e[i]/alicuota, se[i]/alicuota);
		}
	return 0;		
	}
