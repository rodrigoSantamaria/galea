#include "LinkedList.h"

int main()
	{
/*	LinkedList *lista1, *lista2;
	int a=0;
		
	lista1=new LinkedList();
	a=0;
	lista1->addLast((void *)a);
	a=1;
	lista1->addLast((void *)a);
	a=2;
	lista1->addLast((void *)a);
	a=3;
	lista1->addLast((void *)a);
		
	lista2=new LinkedList();
	a=9;
	lista2->addLast((void *)a);
	a=9;
	lista2->addLast((void *)a);
	a=9;
	lista2->addLast((void *)a);
	a=9;
	lista2->addLast((void *)a);
	
	for(int i=0;i<lista2->numElements();i++)
		{
		int temp=(int)lista2->getNode(i);
		//sprintf(temp, "%d", i+4);
		//temp=i+4;
		}
	
	printf("Lista 1\n");
	for(int i=0;i<lista1->numElements();i++)	printf("%d\t", (int)lista1->getNode(i));		
	printf("\nLista 2\n");
	for(int i=0;i<lista2->numElements();i++)	printf("%d\t", (int)lista2->getNode(i));		
	
	for(int i=0;i<lista2->numElements();i++)
		{
		//char *b=(char *)lista1->getNode(i);
		//char *c=(char *)lista2->getNode(i);
		lista1->setNode(i,lista2->getNode(i));
		}
	
	printf("\nLista 1\n");
	for(int i=0;i<lista1->numElements();i++)	printf("%d\t", (int)lista1->getNode(i));		
	printf("\nLista 2\n");
	for(int i=0;i<lista2->numElements();i++)	printf("%d\t", (int)lista2->getNode(i));		
		
	delete lista1;*/
	
	GA2=new GeneticAlgorithm2(100,60,90,1,numGaussianas,v,ancho,this,li,centroides,taus, sigmas, restricciones);

	}
