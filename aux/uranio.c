#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "math1.h"
int main()
	{
	int max=1;
	
	//lds de nuestros radionúclidos		
	float ld[]={};
	
	float tv[]={1136961};
		
	//Actividad del Pb214 a 351 y 52
	float area52[]={303722};
	float sarea52[]={1415};
	float area351[]={137220};
	float sarea351[]={404};
	
	//probabilidades de emisión
	float pth230=0.0038;
	float pu234=0.00123;
	float spu234=0.00002;
	float pu235=0.0508;
	float pu238=0.041;
	float ppb210=0.041;

	//eficiencias en organicas, aguas
/*	float eth230=0.1358;
	float eu234=0.1362;
	float eu235=0.127;
	float eu238=0.1359;
	float epb210=0.137;
*/
	//eficiencias
	/*float ec =0.08482;//351
	float sec=0.00096;//351
	float ed =0.1983;//53
	float sed=0.0028;//53
	*/
	// ef-hb11
	float ec =0.0622;//351
	float sec=0.0024;//351
	float ed =0.1362;//53
	float sed=0.0018;//53
	
	/*ef-dpg
	float ec =0.0906;//351
	float sec=0.0050;//351
	float ed =0.1357;//53
	float sed=0.0034;//53
	*/
	
	
	float pc=0.358; //Probabilidad de emisión del Pb214 a 351
	float spc=0.007;
	float pd=0.0112;//Probabilidad de emisión del U234 a 53.226
	float spd=0.0004;
	//0.0112/0.358 * 0.1983/0.08482
	//actividades y áreas a calcular
	float aPb[max];
	float au234[max];
	float nu234[max];
	
	float saPb[max];
	float sau234[max];
	float snu234[max];
	
	float fondo=0;
	float sfondo=0;
	
	for(int i=0;i<max;i++)	aPb[i]=saPb[i]=au234[i]=sau234[i]=nu234[i]=snu234[i]=0.0;
	
	//Calculamos el área del Pb que nos molesta
	for(int i=0;i<max;i++)
		{
		aPb[i]=(area351[i]*pd*ed)/(pc*ec);
		saPb[i]=aPb[i]*sqrt(pow(sarea351[i]/area351[i],2)+pow(spd/pd,2)+
			pow(sed/ed,2)+pow(spc/pc,2)+pow(sec/ec,2));
		printf("área del Pb en la muestra %d es %f(+-%f)\n", i, aPb[i], saPb[i]);
		}
	//Calculamos el área del U234 que nos queda
	for(int i=0;i<max;i++)
		{
		nu234[i]=area52[i]-aPb[i]-fondo;
		snu234[i]=sqrt(sarea52[i]*sarea52[i]+saPb[i]*saPb[i]+sfondo*sfondo);
		if(nu234[i]<0)	nu234[i]=0;
		printf("área del U234 que nos queda en %d es %f(+-%f)\n", i, nu234[i], snu234[i]);
		}
	
	//Ahora por último calculamos la actividad
	for(int i=0;i<max;i++)
		{
		au234[i]=nu234[i]/((pu234)*ed*tv[i]);
		sau234[i]=au234[i]*sqrt(pow(snu234[i]/nu234[i],2)+pow(spu234/pu234,2)+pow(sed/ed,2));
		printf("actividad de U234 en %d es %f(+-%f) Bq\n", i, au234[i], sau234[i]);
		}
	for(int i=0;i<max;i++)
		{
		char texto[20];
		PresentaNumero(au234[i], sau234[i], texto);
		printf("%s\n", texto);
		}
	
	return 0;		
	}
