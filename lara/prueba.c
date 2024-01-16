#include <stdio.h>
#include "cLaraBD.H"
/* Clase para pruebas con la base de datos lara */

const int naturales[] = 
	{
		// familia del Th232 [0]
	 252, 98, 0,
	 248, 96, 0,
	 244, 94, 0,
	 240, 92, 0,
	 240, 93, 0,
	 240, 94, 0,
	 244, 96, 0,
	 236, 92, 0,
	 232, 90, 0,//Th232 [8]
	 228, 88, 0,
	 228, 89, 0,
	 240, 96, 0,
	 236, 94, 0,
	 232, 92, 0,
	 228, 90, 0,
	 224, 88, 0,
	 220, 86, 0,
	 216, 84, 0,
	 212, 82, 0,
	 212, 83, 0,
	 212, 84, 0,
	 208, 81, 0,
	 208, 82, 0,
	 	// familia del U235 [23]
	 251, 98, 0,
	 247, 96, 0,
	 243, 94, 0,
	 243, 95, 0,
	 239, 92, 0,
	 239, 93, 0,
	 243, 96, 0,
	 239, 94, 0,
	 235, 92, 1,//U235 [31]
	 231, 90, 0,
	 231, 91, 0,
	 227, 89, 0,
	 227, 90, 0,
	 223, 87, 0,
	 223, 88, 0,
	 219, 85, 0,
	 219, 86, 0,
	 215, 83, 0,
	 215, 84, 0,
	 215, 85, 0,
	 211, 82, 0,
	 211, 83, 0,
	 207, 81, 0,
	 211, 84, 0,
	 207, 82, 0,
	 	// familia del U238 [48]
	 250, 98, 0,
	 246, 96, 0,
	 242, 94, 0,
	 238, 92, 0,//U238 [51]
	 234, 90, 0,
	 242, 95, 1,
	 242, 95, 0,
	 242, 96, 0,
	 238, 93, 0,
	 238, 94, 0,
	 234, 91, 1,
	 234, 91, 0,
	 234, 92, 0,//U234 [60]
	 230, 90, 0,//Th230 [61]
	 226, 88, 0,//Ra226 [62]
	 222, 86, 0,
	 218, 84, 0,
	 218, 85, 0,
	 214, 82, 0,
	 214, 83, 0,
	 218, 86, 0,
	 214, 84, 0,
	 210, 81, 0,
	 210, 82, 0,
	 206, 80, 0,
	 210, 83, 0,
	 206, 81, 0,
	 210, 84, 0,
	 206, 82, 0,//El Pb206 en la lista de Lara no lo tienen en cuenta por ser ya estable
	 			//Nosotros lo dejamos puesto, aunque no tiene emisiones asociadas
	 	// familia del Potasio[77]
  	 40, 19, 0 //k40
	 };
const unsigned int numNaturales = 78; 
const int familias[]={
					 8*3,23*3,//Descendientes del Th232
					 31*3,48*3,//Descendientes del U235
					 51*3,60*3,//Descendientes del U238
					 60*3,61*3,//Descendientes del U234
					 61*3,62*3,//Descendientes del Th230
					 62*3,77*3//Descendientes del Ra226
					 };
const unsigned int numFamilias=6;
char name[3];
cLaraBD LaraBD;
void imprimir(cSearchLaraBD);
void imprimirElemento(cSearchLaraBD);
					 
int main() {
	LaraBD.Load("lara.DB");

	if (!LaraBD.GetIsRunning()) 
		{
		printf("Programa abortado\n");
		return -1;
		}
	
	printf("num Elementos: %d\n", LaraBD.GetNumElementsBD());
	printf("num Emisiones: %d\n", LaraBD.GetNumEmissionsBD());
	
	cSearchLaraBD busqueda2, busqueda3, busqueda;
	cSearchLaraBD busqueda1, busqueda4, busqueda5, busqueda6;
	
	printf("programa finalizado\n");
	return 0;
}

void imprimirElemento(cSearchLaraBD busqueda)
	{
	int valor2 = busqueda.GetFirstResult();
	while (valor2 != -1) 
		{
		LaraBD.GetNameElement(valor2, name);
		printf("%d -%s- %d\n", LaraBD.GetAtomicNumber(valor2), name, LaraBD.GetMassNumber(valor2));
		valor2 = busqueda.GetNextResult();
		}
	printf("encontrados: %d\n", busqueda.GetNumItemsFound());
	}
	
void imprimir(cSearchLaraBD busqueda)
	{
	int valor2 = busqueda.GetFirstResult();
	while (valor2 != -1) 
		{
		printf("%f Kev %f%% (%c) ", LaraBD.GetEnergy(valor2), LaraBD.GetProbability(valor2), LaraBD.GetKindEmission(valor2));
		int valor = LaraBD.GetEmissor(valor2);
		LaraBD.GetNameElement(valor, name);
		printf("%d -%s- %d\n", LaraBD.GetAtomicNumber(valor), name, LaraBD.GetMassNumber(valor));
		valor2 = busqueda.GetNextResult();
		}
	printf("encontrados: %d\n", busqueda.GetNumItemsFound());
	}
