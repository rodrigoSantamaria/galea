#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ficheros.h"

/*const int naturales[] = {
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
	 235, 92, 0,
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
	 	// familia del U238 [49]
	 250, 98, 0,
	 246, 96, 0,
	 242, 94, 0,
	 238, 92, 0,//U238 [52]
	 234, 90, 0,
	 242, 95, 1,
	 242, 95, 0,
	 242, 96, 0,
	 238, 93, 0,
	 238, 94, 0,
	 234, 91, 1,
	 234, 91, 0,
	 234, 92, 0,//U234 [61]
	 230, 90, 0,//Th230 [62]
	 226, 88, 0,//Ra226 [63]
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
	 206, 82, 0,
	 	// familia del Potasio[78]
  	 40, 19, 0 //k40
	 };*/
const int naturales[] = {
		// familia del Th232 [0]
	 252, 98, 0, 0,
	 248, 96, 0, 0,
	 244, 94, 0, 1,
	 240, 92, 0, 0,
	 240, 93, 0, 0,
	 240, 94, 0, 0,
	 244, 96, 0, 0,
	 236, 92, 0, 0,
	 232, 90, 0, 1,//Th232 [8]
	 228, 88, 0, 0,
	 228, 89, 0, 0,
	 240, 96, 0, 0,
	 236, 94, 0, 0,
	 232, 92, 0, 0,
	 228, 90, 0, 0,
	 224, 88, 0, 0,
	 220, 86, 0, 0,
	 216, 84, 0, 0,
	 212, 82, 0, 0,
	 212, 83, 0, 0,
	 212, 84, 0, 0,
	 208, 81, 0, 0,
	 208, 82, 0, 0,
	 	// familia del U235 [23]
	 251, 98, 0, 0,
	 247, 96, 0, 0,
	 243, 94, 0, 0,
	 243, 95, 0, 0,
	 239, 92, 0, 0,
	 239, 93, 0, 0,
	 243, 96, 0, 0,
	 239, 94, 0, 1,
	 235, 92, 0, 1,//U235 [31]//NOTA: No sé por qué ponía que era metaestable!
	 231, 90, 0, 0,
	 231, 91, 0, 0,
	 227, 89, 0, 0,
	 227, 90, 0, 0,
	 223, 87, 0, 0,
	 223, 88, 0, 0,
	 219, 85, 0, 0,
	 219, 86, 0, 0,
	 215, 83, 0, 0,
	 215, 84, 0, 0,
	 215, 85, 0, 0,
	 211, 82, 0, 0,
	 211, 83, 0, 0,
	 207, 81, 0, 0,
	 211, 84, 0, 0,
	 207, 82, 0, 0,
	 	// familia del U238 [48]
	 250, 98, 0, 0,
	 246, 96, 0, 0,
	 242, 94, 0, 0,
	 238, 92, 0, 1,//U238 [51]
	 234, 90, 0, 0,
	 242, 95, 1, 0,
	 242, 95, 0, 0,
	 242, 96, 0, 0,
	 238, 93, 0, 0,
	 238, 94, 0, 0,
	 234, 91, 1, 0,
	 234, 91, 0, 0,
	 234, 92, 0, 0,//U234 [60]
	 230, 90, 0, 0,//Th230 [61]
	 226, 88, 0, 1,//Ra226 [62]
	 222, 86, 0, 0,
	 218, 84, 0, 0,
	 218, 85, 0, 0,
	 214, 82, 0, 0,
	 214, 83, 0, 0,
	 218, 86, 0, 0,
	 214, 84, 0, 0,
	 210, 81, 0, 0,
	 210, 82, 0, 0,
	 206, 80, 0, 0,
	 210, 83, 0, 0,
	 206, 81, 0, 0,
	 210, 84, 0, 0,
	 206, 82, 0, 0,
	 	// familia del Potasio[77]
  	 40, 19, 0, 0 //k40
	 };
//NOTA: No sale la familia radiactiva del Neptunio, supongo que no es natural
const unsigned int numNaturales = 78; //Los 78 que aparecen en la lista de arriba
unsigned int keysNaturales[numNaturales];
bool listaEquilibrio=false;
	 
typedef struct TEmision {
	float energia, incEnergia;
	float probabilidad, incProbabilidad;
	unsigned char tipoEmision;
	unsigned short int posicion;
	unsigned int clave;
	TEmision *siguiente;
} TEmision;

typedef struct {
	TEmision *inicio;
	int numElementos;
	unsigned short int *resumen;
	unsigned short int numResumen;
} TListEmision;

typedef struct TInEmision {
	TEmision *emision;
	TInEmision *siguiente;
} TInEmision;

typedef struct {
	TInEmision *inicio;
	int numElementos;
} TListaInEmision;

typedef struct TElement {
	unsigned char numProtons;
	unsigned short int numAtomic;
	unsigned int clave;
	unsigned short int posicion;
	char Name[3];
	float lifeTime, incLifeTime;
	bool metaestable;
	bool natural;
	bool equilibrio;//NUEVO: Indica si el elemento es una lista de descendientes
	unsigned short posMatriz;
	TElement *siguiente;
	TListaInEmision Emisiones;
} TElement;

typedef struct {
	TElement *inicio;
	int numElementos;
	unsigned short int *matrizEmisiones;
	unsigned short int *resumen;
} TListElement;

typedef struct {
	unsigned int clave;
	char Name[3];
	float lifeTime, incLifeTime;
	bool Natural; 
	bool Equi;//NUEVO
	unsigned short int listaEmisiones;
} TFileElement;

typedef struct {
	float energia, incEnergia;
	float probabilidad, incProbabilidad;
	unsigned char tipoEmision;
	unsigned short int posEmisor;
} TFileEmision;


void PasaNaturalesAKeys() 
	{
	for (int i = 0; i < numNaturales; i++)
/*		keysNaturales[i] = naturales[i*3]*512 +
					naturales[i*3 + 1]*2 +
					naturales[i*3 + 2];*/
		{
		keysNaturales[i] = naturales[i*4]*1024 +
					naturales[i*4 + 1]*4 +
					naturales[i*4 + 2];
		if(naturales[i*4+3]==1)	keysNaturales[i]+=2;
	/*	printf("%d, %d, %d, %d\n", naturales[i*4], naturales[i*4 + 1],
					naturales[i*4 + 2], naturales[i*4+3]);*/
		}
	}

bool IsNatural(unsigned int key) {
	for (int i = 0; i < numNaturales; i++)
		if (key == keysNaturales[i]) return true;
	return false;
}

unsigned int SizeOfElement() {
	TFileElement pru;
	unsigned int temp = sizeof(pru.clave) + sizeof(pru.Name);
	temp += sizeof(pru.lifeTime)*2;
	temp += 1 + sizeof(pru.listaEmisiones);
	return temp;
}
unsigned int SizeOfEmision() {
	TFileEmision pru;
	unsigned int temp = sizeof(pru.energia)*4;
	temp += sizeof(pru.tipoEmision) + sizeof(pru.posEmisor);
	return temp;
}

void SinMemoria() {
	printf("ERROR: falta de memoria\n");
	exit(-1);
}

void AddEmisionToElement(TElement *nodo, TEmision *valor) {
	TInEmision **puntero;
	
	puntero = &(nodo->Emisiones.inicio);
	while ((*puntero) != NULL)
		puntero = &((*puntero)->siguiente);
	(*puntero) = new TInEmision;
	if (*puntero == NULL) SinMemoria();
	(*puntero)->emision = valor;
	(*puntero)->siguiente = NULL;
	nodo->Emisiones.numElementos++;
}

void DelEmisionsInElements(TElement *nodo) {
	TInEmision **puntero;
	TInEmision *borrar;
	
	puntero = &(nodo->Emisiones.inicio);
	while ((*puntero) != NULL) {
		borrar = (*puntero);
		puntero = &((*puntero)->siguiente);
		delete borrar;
	}
}

TEmision* DevuelveEmisionInElement(TElement elemento, int pos = 0){
	TInEmision **puntero;
	
	if ((pos == 0)||(pos > elemento.Emisiones.numElementos)) pos = elemento.Emisiones.numElementos;
	puntero = &(elemento.Emisiones.inicio);
	int i = 1;
	while (((*puntero) != NULL)&&(i < pos)) {
		// pasa al siguiente elemento
		puntero = &((*puntero)->siguiente);
		i++;
	}
	return (*puntero)->emision;
}

void AddElement(TListElement &lista, TElement *nodo) {
	// se recibe el elemento y se coloca segun el numero atomico
	TElement **puntero;
	
	puntero = &(lista.inicio);
	int i = 1;
	while ((*puntero) != NULL) {
		// comprueba si hay que colocarlo antes
		if ((*puntero)->clave > nodo->clave) {
			nodo->siguiente = *puntero;
			*puntero = nodo;
			lista.numElementos++;
			return;
		}

		// pasa al siguiente elemento
		puntero = &((*puntero)->siguiente);
		i++;
	}
	// pone el elemento al final de la lista
	nodo->siguiente = NULL;
	*puntero = nodo;
	lista.numElementos++;
}

TElement* DevuelveElement (TListElement &lista, int pos = 0){
	TElement **puntero;
	
	if ((pos == 0)||(pos > lista.numElementos)) pos = lista.numElementos;
	puntero = &(lista.inicio);
	int i = 1;
	while (((*puntero) != NULL)&&(i < pos)) {
		// pasa al siguiente elemento
		puntero = &((*puntero)->siguiente);
		i++;
	}
	return *puntero;
}

void borraInElemento(TListaInEmision &lista) {
	TInEmision **puntero;
	TInEmision *borrar;
	
	puntero = &(lista.inicio);
	while((*puntero) != NULL) {
		borrar = (*puntero);
		// pasa al siguiente elemento
		puntero = &((*puntero)->siguiente);
		// borra el puntero
		delete borrar;
	}
}

void BorraElemento(TListElement &lista) {
	TElement **puntero;
	TElement *borrar;
	
	puntero = &(lista.inicio);
	while ((*puntero) != NULL) {
		borrar = (*puntero);
		// pasa al siguiente elemento
		puntero = &((*puntero)->siguiente);
		// borra el puntero
		borraInElemento(borrar->Emisiones);
		delete borrar;
	}
	
	delete lista.matrizEmisiones;
}

void AddEmision(TListEmision &lista, TEmision *nodo) {
	// se recibe la emision y se coloca segun su energia
	TEmision **puntero;
	
	puntero = &(lista.inicio);
	int i = 1;
	while ((*puntero) != NULL) {
		// comprueba si hay que colocarlo antes
		if ((*puntero)->energia > nodo->energia) {
			nodo->siguiente = *puntero;
			*puntero = nodo;
			lista.numElementos++;
			return;
		}

		// pasa al siguiente elemento
		puntero = &((*puntero)->siguiente);
		i++;
	}
	// pone el elemento al final de la lista
	nodo->siguiente = NULL;
	*puntero = nodo;
	lista.numElementos++;
}

TEmision* DevuelveEmision (TListEmision &lista, int pos = 0)
	{
	TEmision **puntero;
	
	if ((pos == 0)||(pos > lista.numElementos)) pos = lista.numElementos;
	puntero = &(lista.inicio);
	int i = 1;
	while (((*puntero) != NULL)&&(i < pos)) 
		{
		// pasa al siguiente elemento
		puntero = &((*puntero)->siguiente);
		i++;
		}
	return *puntero;
	}

void BorraEmision(TListEmision &lista) 
	{
	TEmision **puntero;
	TEmision *borrar;
	
	puntero = &(lista.inicio);
	while ((*puntero) != NULL) 
		{
		borrar = (*puntero);
		// pasa al siguiente elemento
		puntero = &((*puntero)->siguiente);
		// borra el puntero
		delete borrar;
		}
	}

unsigned short int DaPosicionElement(TListElement &lista, unsigned int clave){
	TElement **puntero;
	
	puntero = &(lista.inicio);
	int i = 0;
	while (((*puntero) != NULL)&&(i < lista.numElementos)) {
		if ((*puntero)->clave == clave) return (*puntero)->posicion;
		// pasa al siguiente elemento
		puntero = &((*puntero)->siguiente);
		i++;
	}
	return 0;
}

unsigned char  DaTipoEmision(char *nombre) 
	{
	if (strncmp(nombre, "G", 1) ==0)
		return 0;
	else if (strncmp(nombre, "XL",2) == 0)
		return 1;
	else if (strncmp(nombre, "XKa1",4) == 0)
		return 2;
	else if (strncmp(nombre, "XKa2",4) == 0)
		return 3;
	else if (strncmp(nombre, "XKb1",4) == 0)
		return 4;
	else if (strncmp(nombre, "XKb2",4) == 0)
		return 5;
	else
		printf("tipo de emision desconocida -%s-\n", nombre);
	return 0;
	}

double EscalaTiempo(char escala) {
	switch(escala) {
		case 'A': return 31556926.0;
		case 'J': return 86400;
		case 'H': return 3600.0;
		case 'M': return 60.0;
		case 'S': return 1.0;
		default : return 1.0;
	}
}


const char *NUCL = "NUCL", *GAMA = "GAMA", *REF = "REF", *EQUI = "EQUI";
TListElement Lista = {NULL, 0};
TListEmision ListaE = { NULL, 0};

typedef struct {
	char identidad[13];
	unsigned int Elementos;
	unsigned int Emisiones;
	unsigned int Matriz;
	unsigned int resumenElementos;
	unsigned int resumenEmisiones;
} TCabecera;

unsigned int SizeOfCabecera() {
	TCabecera pru;
	unsigned int temp = sizeof(pru.identidad) + sizeof(pru.Elementos)*5;
	return temp;
}

void fwriteCabecera(TCabecera cabecera, FILE *stream) {
	fileWrite(&cabecera.identidad, sizeof(cabecera.identidad), 1, stream);
	fileWrite(&cabecera.Elementos, sizeof(cabecera.Elementos), 1, stream);
	fileWrite(&cabecera.Emisiones, sizeof(cabecera.Emisiones), 1, stream);
	fileWrite(&cabecera.Matriz, sizeof(cabecera.Matriz), 1, stream);
	fileWrite(&cabecera.resumenElementos, sizeof(cabecera.resumenElementos), 1, stream);
	fileWrite(&cabecera.resumenEmisiones, sizeof(cabecera.resumenEmisiones), 1, stream);
}

void fwriteElement(TFileElement elemento, FILE *stream) {
	fileWrite(&elemento.clave, sizeof(elemento.clave), 1, stream);
	fileWrite(&elemento.Name, sizeof(elemento.Name), 1, stream);
	fileWrite(&elemento.lifeTime, sizeof(elemento.lifeTime), 1, stream);
	fileWrite(&elemento.incLifeTime, sizeof(elemento.incLifeTime), 1, stream);
	fileWriteBool(elemento.Natural, stream);
	fileWrite(&elemento.listaEmisiones, sizeof(elemento.listaEmisiones), 1, stream);
	fileWriteBool(elemento.Equi, stream);
}

void fwriteEmision(TFileEmision emision, FILE *stream) {
	fileWrite(&emision.energia, sizeof(emision.energia), 1, stream);
	fileWrite(&emision.incEnergia, sizeof(emision.incEnergia), 1, stream);
	fileWrite(&emision.probabilidad, sizeof(emision.probabilidad), 1, stream);
	fileWrite(&emision.incProbabilidad, sizeof(emision.incProbabilidad), 1, stream);
	fileWrite(&emision.tipoEmision, sizeof(emision.tipoEmision), 1, stream);
	fileWrite(&emision.posEmisor, sizeof(emision.posEmisor), 1, stream);
}

void VuelcaDatos() {
	FILE *laraBD;
	char *identidad = "analgamLaraDB";
	
	// abre el fichero Lara.txt
	laraBD = fopen("lara.DB", "wb");
	
	TElement *elemento = DevuelveElement(Lista, 0);
	int nResumenElementos = elemento->numAtomic / 10 + 1;

	// Escribe la cabecera
	printf("\tescribiendo cabecera\n");
	{
		TCabecera cabecera;
		strncpy(cabecera.identidad, identidad, strlen(identidad));
		cabecera.Elementos = SizeOfCabecera();
		cabecera.Emisiones = Lista.numElementos*SizeOfElement() + cabecera.Elementos;
		cabecera.Matriz = cabecera.Emisiones + ListaE.numElementos*SizeOfEmision();
		cabecera.resumenElementos = cabecera.Matriz + sizeof(unsigned short int)*(ListaE.numElementos + Lista.numElementos);
		cabecera.resumenEmisiones = cabecera.resumenElementos + sizeof(unsigned short int)*nResumenElementos;
		fwriteCabecera(cabecera, laraBD);
	}
	
	
	// Escribe los elementos
	printf("\tescribiendo elementos (%d)\n", Lista.numElementos);
	for (int i = 1; i <= Lista.numElementos; i++) {
		TElement *elemento = DevuelveElement(Lista, i);
		TFileElement vuelca = { elemento->clave, "  ",
					elemento->lifeTime, elemento->incLifeTime, elemento->natural, elemento->equilibrio,  
					elemento->posMatriz};
		strncpy(vuelca.Name, elemento->Name, 3);
		fwriteElement(vuelca, laraBD);
	}
		
	// Escribe las emisiones
	printf("\tescribiendo emisiones (%d)\n", ListaE.numElementos);
	for (int i = 1; i <= ListaE.numElementos; i++) 
		{
		TEmision *emision = DevuelveEmision(ListaE, i);
		TFileEmision vuelca = 
					{ 
					emision->energia, emision->incEnergia, 
					emision->probabilidad, emision->incProbabilidad, 
					emision->tipoEmision, DaPosicionElement(Lista, emision->clave)
					};
		fwriteEmision(vuelca, laraBD);
		}
	
	// Escribe la matriz de las emisiones
	printf("\tescribiendo matriz emisiones (%d)\n", ListaE.numElementos + Lista.numElementos);
	for (int i = 0; i < ListaE.numElementos + Lista.numElementos; i++) 
		fileWrite(&Lista.matrizEmisiones[i], sizeof(unsigned short int), 1, laraBD);
	
	// Escrivbe el resumen de elementos
	printf("\tescribiendo resumen elementos (%d)\n", nResumenElementos);
	fileWrite(Lista.resumen, sizeof(unsigned short int), nResumenElementos, laraBD);
	
	// Escribe el resumen de elementos
	printf("\tescribiendo resumen emisiones (%d)\n", ListaE.numResumen);
	fileWrite(ListaE.resumen, sizeof(unsigned short int), ListaE.numResumen, laraBD);

	// Cierra el fichero
	fclose(laraBD);
	
}

int main() 
	{
	FILE *lara;
	char buffer[100], numero[4]= "   ", vida[11]= "          ";
	int items = 0;
	
	// naturales
	printf("Construyendo matriz de naturales\n");
	PasaNaturalesAKeys();
	
	// tamagnos
	printf("tamagno estructura elementos: %d  File: %d\n", sizeof(TElement), SizeOfElement());
	printf("tamagno estructura emisiones: %d  File: %d\n", sizeof(TEmision), SizeOfEmision());

	// abre el fichero Lara.txt
	lara = fopen("lara.txt", "r");
	
	printf("leyendo fichero\n");
	while (fgets(buffer, 80, lara) != NULL) 
		{
		// lo primero leido es un nucleo (empieza por NUCL)
		// si no es un pasa a la siguiente
		if (strncmp(&buffer[2], NUCL, strlen(NUCL)) == 0) 
			{
			TElement *leido;
			bool grabarElemento = true;
			listaEquilibrio=false;
			//NOTA: Las listas de descendientes no las mete en lara.DB!!!
			//		Quizás deberíamos introducirlas también
			if (strncmp(&buffer[57], EQUI, strlen(EQUI)) == 0)
			//NUEVO:	
				listaEquilibrio=true;
			
			if (grabarElemento) 
				{
				// Es un nuclido, lee la linea segun el formato
				leido = new TElement;
				if (leido == NULL) SinMemoria();
				// inicializa algunos datos
				leido->Emisiones.numElementos = 0;
				leido->Emisiones.inicio = NULL;
				// copia el nombre
				leido->Name[0] = buffer[16]; leido->Name[1] = buffer[17];
				leido->Name[3] = '\0';
				// copia el numero de protones
				strncpy(numero, &buffer[12], 3);
				leido->numProtons = atoi(numero);
				// copia el numero atomico
				strncpy(numero, &buffer[19], 3);
				leido->numAtomic = atoi(numero);
				
				leido->metaestable = (buffer[23] == 'M') ? true : false;
				
				// copia del periodo de vida
				strncpy(vida, &buffer[29], 10);
				leido->lifeTime = atof(vida)*EscalaTiempo(buffer[40]);
				// copia la incertidumbre del periodo de vida
				strncpy(vida, &buffer[46], 10);
				leido->incLifeTime = atof(vida)*EscalaTiempo(buffer[40]);
				
		
				//
				if(listaEquilibrio)	leido->equilibrio=true;
				else				leido->equilibrio=false;
				//NOTA: En la clave deberíamos meter lo de si es lista de 
				//		descendientes.
				//leido->clave = leido->numAtomic*512 + leido->numProtons*2;
				leido->clave = leido->numAtomic*1024 + leido->numProtons*4;
				if(leido->equilibrio)	leido->clave+=2;
				if (leido->metaestable) leido->clave++;
				//if(leido->equilibrio)	printf("Elemento en equilibrio\n");
				//if(leido->metaestable)	printf("Elemento metaestable\n");
				//if(leido->clave==0x3ad70)	leido->clave=0x3ad73;
				printf("Clave de %d-%d: %x\n", leido->numAtomic, leido->numProtons, leido->clave);
				leido->natural = IsNatural(leido->clave);
				
				}
			
			// Ahora lee las emisiones
			fgets(buffer, 80, lara);
			if (strncmp(&buffer[2], GAMA, strlen(GAMA)) == 0) 
				{
				if(listaEquilibrio)	printf("Tenemos emisiones gamma\n");
				// hay emisiones gamma
				fgets(buffer, 80, lara);
				while (buffer[2] == ' ') 
					{
					if (grabarElemento) 
						{
						// recoge los datos y los guarda en la lista
						TEmision *leidoE = new TEmision;
						if (leidoE == NULL) SinMemoria();
						char tipo[10], numero2[12]= "           ";
						strncpy(numero2, &buffer[3], 11);
						leidoE->energia = atof(numero2);
						strncpy(numero2, &buffer[17], 11);
						leidoE->incEnergia = atof(numero2);
						strncpy(numero2, &buffer[31], 11);
						leidoE->probabilidad = atof(numero2);
						strncpy(numero2, &buffer[45], 11);
						leidoE->incProbabilidad = atof(numero2);
						// recoge tipo emision
						strcpy(tipo, &buffer[57]);
						// le quita el ultimo elemento
						tipo[strlen(tipo)-2] = '\0';
						leidoE->tipoEmision = DaTipoEmision(tipo);
						// indica el elemento duegno de esta emision
						leidoE->clave = leido->clave;
						// agnade la emision a la libreria
//						printf("%d %e %d\n", items, leidoE->energia, ListaE.numElementos);
						AddEmision(ListaE, leidoE);
						// agnade la emision al elemeto
						AddEmisionToElement(leido, leidoE);
						}
					fgets(buffer, 80, lara);
					}
				}
			else 
				{
				if(listaEquilibrio)	printf("No Tenemos emisiones gamma\n");
				// no hay emisiones gamma, hay que leer hasta el final del
				// elemento
				while (strncmp(&buffer[2], REF, strlen(REF)) != 0)
					fgets(buffer, 80, lara);
				fgets(buffer, 80, lara); // lee la linea extra de la fecha
				}
			
			// añade el elemento
			if (grabarElemento) AddElement(Lista, leido);
			}
		}
	
	// cierra el fichero
	fclose(lara);
	
	// indica la posicion de las emisiones
	printf("colocando elementos\n");
	{
		int i = 0;
		TElement **puntero;	
		puntero = &(Lista.inicio);
		while ((*puntero) != NULL) 
			{
			// pone la posicion en el elemento
			(*puntero)->posicion = i;
			// pasa al siguiente elemento
			puntero = &((*puntero)->siguiente);
			i++;
			}
	}

	// indica la posicion de las emisiones
	printf("colocando emisiones\n");
	{
		int i = 0;
		TEmision **puntero;	
		puntero = &(ListaE.inicio);
		while ((*puntero) != NULL) 
			{
			// pone la posicion en la emision
			(*puntero)->posicion = i;
			// pasa al siguiente elemento
			puntero = &((*puntero)->siguiente);
			i++;
			}
	}
	
	//creando la matriz de emisiones
	printf("colocando emisiones en matriz\n");
	{
		int numElementos = Lista.numElementos + ListaE.numElementos;
		Lista.matrizEmisiones = new unsigned short int [numElementos];
		if (Lista.matrizEmisiones == NULL) SinMemoria();
		int k = 0;
		for (int i = 1; i <= Lista.numElementos; i++) 
			{
			TElement *elemento = DevuelveElement(Lista, i);
			Lista.matrizEmisiones[k] = elemento->Emisiones.numElementos;
			elemento->posMatriz = k;
			k++;
			for (int j = 1; j <= elemento->Emisiones.numElementos; j++) 
				{
				TEmision *emision = DevuelveEmisionInElement(*elemento, j);
				Lista.matrizEmisiones[k] = emision->posicion;
				k++;
				}
			}
	}
	
	// Creando el resumen de elementos
	printf("creando resumen de elementos\n");
	{
		TElement *elemento = DevuelveElement(Lista, 0);
		int numElementos = elemento->numAtomic / 10 + 1;
		Lista.resumen = new unsigned short int[numElementos];
		int tope = 0; int pos = 0;
		for (int i = 1; i <= Lista.numElementos; i++) 
			{
			elemento = DevuelveElement(Lista, i);
			if (elemento->numAtomic >= tope) 
				{
				tope += 10;
				Lista.resumen[pos] = elemento->posicion;
				pos++;
				}
			}
	}

	// Creando el resumen de emisiones
	printf("creando resumen de emisiones\n");
	{
		TEmision *emision = DevuelveEmision(ListaE, 0);
		int numEmisiones = (int)(emision->energia / 10.0) + 1;
		ListaE.resumen = new unsigned short int[numEmisiones];
		float tope = 0; int pos = 0;
		for (int i = 1; i <= ListaE.numElementos; i++) 
			{
			emision = DevuelveEmision(ListaE, i);
			if (emision->energia >= tope) 
				{
				tope += 10.0;
				ListaE.resumen[pos] = emision->posicion;
				pos++;
				}
			}
		ListaE.numResumen = pos;
	}

	// info
	printf("Elementos encontrados: %d\n", Lista.numElementos);
	printf("emisiones gamma leidas: %d\n", ListaE.numElementos);
	
	// ejemplo
	TElement *elemento = DevuelveElement(Lista, 12);
	printf("Ejemplo: El elemento %s tiene %d emisiones:\n", elemento->Name, elemento->Emisiones.numElementos);
	for (int i = 1; i <= elemento->Emisiones.numElementos; i++) 
		{
		TEmision *emision = DevuelveEmisionInElement(*elemento, i);
		printf("\t%e\n", emision->energia);
		}
	
	// volcado de datos
	printf("Creando base de datos\n");
	VuelcaDatos();
	printf("Finalizada creacion base de datos\n");
	
	BorraElemento(Lista);
	BorraEmision(ListaE);
	return 0;
}
