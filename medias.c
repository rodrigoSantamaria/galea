#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "math1.h"
int main()
	{
	/*//Ac228 (Thu60)
		int max=23;
		float e[]={8.3,9.4,9.7,10.0,9.3,10,10.8,11.4,10,13,16,10.6,9,9,11.3,12,14.5,10,15.0,16,14.6,14,14};
		float se[]={1,0.7,0.6,0.8,0.7,2,0.6,0.4,3,1,1,0.9,2,3,0.9,1,0.3,4,0.3,4,0.8,1,3};
	*/
	/*AC228(ThU50)
		int max=14;
		float e[]={21,19,17,19,18,23,17,20,22,18,18,27.4,27.8,28};
		float se[]={4,2,1,2,1,6,2,1,6,3,4,0.8,0.8,12};
*/
/*	Ac228(ThU40)
		int max=23;
		float e[]={11,9.3,10.0,9.9,9.5,11,10.0,10.8,10,11,9,10.3,11,12.3,12,14.1,14.4,18,12,12,14.3,16,14};
		float se[]={1,0.6,0.6,0.7,0.6,2,0.5,0.4,2,1,2,0.6,2,0.8,1,0.3,0.3,3,2,7,0.7,1,2};
	*/
		int max=1;
//		float e[]= {1.02,1.091,0.911};
//		float se[]={0.18,0.088,0.083};
		float e[]= {0.063};
		float se[]={0.055};
		float alicuota=0.0293;	
	float media=0;
	float s1=0;
	float s2=0;
	float smedia=0;
	float temp=0;
	char texto[20];
	
	int i=0;
	
	//Si tenemos alícuota, antes hacemos la conversión
	if(alicuota>0)
		{
		for(i=0;i<max;i++)
			{
			e[i]=e[i]/alicuota;
			se[i]=se[i]/alicuota;
			PresentaNumero(e[i], se[i], texto);
			printf("%s\n", texto);
			}
		}
	
	for(i=0;i<max;i++)
		{
		//printf("media+=%f/%f²\n", e[i], se[i]);
		media+=e[i]/(se[i]*se[i]);
		temp+=1.0/(se[i]*se[i]);
		}
	media/=temp;
	for(i=0;i<max;i++)	s1+=((e[i]-media)*(e[i]-media))/(se[i]*se[i]);	
	//printf("smedia(%f) se divide entre %f y entre %d\n", smedia, temp, max-1);
	s1/=temp;
	s2=1/temp;
	
	//printf("Media fluctuaba entre ser %f y %f\n", s1, s2);
	smedia=sqrt(maxim(s1,s2));
	printf("%f (%f)\n", media, smedia);

	PresentaNumero(media, smedia, texto);
	printf("%s\n", texto);
		
	return 0;		
	}
