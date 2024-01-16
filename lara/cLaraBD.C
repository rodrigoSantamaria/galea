#include <math.h>
#include "math1.h"
#include <stdio.h>
#include <string.h>
#include "cLaraBD.H"
#include "ficheros.h"

//Información sobre las familias del los 6 elementos naturales
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


/*****************************************************
 ***												***
 ***						INTERNAL LISTS			***
 ***												***
 *****************************************************/
 
typedef struct NodeList 
	{
	unsigned short int value;
	NodeList *next;
	} NodeList;

typedef struct 
	{
	NodeList *first;
	int numElements;
	} tListResults;
	
void AddResultToList (tListResults &list, NodeList *node);
void DelList (tListResults &list);
unsigned short int *ListToMatrix(tListResults *list);
unsigned short int *ListToMatrix(NodeList *list, int max);
tListResults MatrixToList(unsigned int nElements, unsigned short int *matrix);

void AddResultToList (tListResults &list, NodeList *node) 
	{
	//NOTA: A esta función no le puedo quitar el nodo **guide porque nos saca 
	//		mal las listas si lo hago
	NodeList **guide;
	
	guide = &(list.first);
	while ((*guide) != NULL) 
		{
		// check for sort
		if ((*guide)->value >= node->value) 
			{
			if ((*guide)->value == node->value) 
				{ // avoid repeated elements
				delete node; // warning!
				return;
				}
			node->next = *guide;
			*guide = node;
			list.numElements++;
			return;
			}
		// pass to next element
		guide = &((*guide)->next);
		}
	// put element in the last place of the list
	node->next = NULL;
	*guide = node;
	list.numElements++;
	}

//************** DEL LIST ************
void DelList (tListResults &list) 
	{
	NodeList *guide;
	NodeList *del;

	guide = list.first;
	while (guide != NULL) 
		{
		del = guide;
		guide = guide->next;
		delete del;
		}
	list.numElements = 0;
	list.first = NULL;
	}
	
unsigned short int *ListToMatrix(tListResults *list) 
	{
	// alloc memory for matrix
	unsigned short int *matrix = new unsigned short int[list->numElements];
	if (matrix == NULL) return NULL;
	
	// pass elements from list to matrix
	/*
	NodeList **guide;
	
	guide = &(list->first);
	int i = 0;
	while ((*guide) != NULL) 
		{
		matrix[i] = (*guide)->value;
		guide = &((*guide)->next);
		i++;
		}
	*/
	NodeList *guide;
	
	guide = list->first;
	int i = 0;
	while (guide != NULL) 
		{
		matrix[i] = guide->value;
		guide = guide->next;
		i++;
		}
		
	return matrix;
	}	

unsigned short int *ListToMatrix(NodeList *list, int max) 
	{
	// alloc memory for matrix
	unsigned short int *matrix = new unsigned short int[max];
	if (matrix == NULL) return NULL;
	
	// pass elements from list to matrix
	NodeList *guide=list;
	
	int i = 0;
	printf("Imprimiendo MATRIZ\n");
	while (guide != NULL) 
		{
		matrix[i] = guide->value;
		printf("%d\t", matrix[i]);
		guide = guide->next;
		i++;
		}
	return matrix;
}	

//****************** MATRIX TO LIST ************
// This funtion make a list of results from a search intern matrix
tListResults MatrixToList(unsigned int nElements, unsigned short int *matrix) 
	{
	tListResults list = { NULL, 0 };
	
	for (int i = 0; i < nElements; i++) 
		{
		NodeList *node = new NodeList;
		if (node == NULL) 
			{
			delete list.first; list.numElements = 0;
			return list;
			}
		node->next = NULL; node->value = matrix[i];
		AddResultToList(list, node);
		}
	
	return list;
	}


/*****************************************************
 ***																***
 ***						SEARCH LARA BD						***
 ***																***
 *****************************************************/
/* ***************************
	**     General Methods   **
	***************************/
	// Constructors
cSearchLaraBD::cSearchLaraBD() 
	{
	numFound = 0;
	Results = NULL;
	Guide = 0;
    }

cSearchLaraBD::cSearchLaraBD(unsigned int nFound, unsigned short int *results) 
	{
	Results = results;
	if (results != NULL)
		numFound = nFound;
	else numFound = 0;
	Guide = 0;
	}

	// Copy Constructor
cSearchLaraBD::cSearchLaraBD(const cSearchLaraBD& copy) 
	{
	// Initialize dates
	Results = NULL;
	numFound = 0;
	Guide = 0;
	
	if ((copy.numFound == 0)||(copy.Results == NULL)) return; // no data in object to copy

	// alloc memory for new data
	Results = new unsigned short int [copy.numFound];
	if (Results == NULL) return;
	
	// make a copy of results
	//printf("Usamos memcpy\n");
	memcpy(Results, copy.Results, copy.numFound*sizeof(unsigned short int));
	numFound = copy.numFound;
	Guide = copy.Guide;
	}

	// Destructor
cSearchLaraBD::~cSearchLaraBD() 
	{
	if (Results != NULL) delete [] Results;
	}

	
	/* ***************************
	**       Operators       **
	***************************/

	// Operator add
	// Añade a la clase de búsqueda los elementos que estén en first
cSearchLaraBD cSearchLaraBD::operator+(const cSearchLaraBD first) const 
	{
	unsigned int nElements;
	unsigned short *matrix;
	cSearchLaraBD result; // object result

	// search for minimum transfer
	tListResults firstList, lastList;
	if (first.numFound > numFound) 
		{
	 	nElements = first.numFound; matrix = first.Results;
		firstList = MatrixToList(nElements, matrix);
 		nElements = numFound; matrix = Results;
		lastList = MatrixToList(nElements, matrix);
		}
	else 
		{
	 	nElements = first.numFound; matrix = first.Results;
		lastList = MatrixToList(nElements, matrix);
 		nElements = numFound; matrix = Results;
		firstList = MatrixToList(nElements, matrix);
		}

	// add last list to first list
	NodeList **guide;
	guide = &(lastList.first);
	while ((*guide) != NULL) 
		{
		// create a new node
		NodeList *newNode = new NodeList;
		if (newNode == NULL) break; // memory failure, no more adds
		newNode->value = (*guide)->value;
		AddResultToList(firstList, newNode);
		guide = &((*guide)->next); // pass to next element
		}
	
	// convert first list in matrix form
	matrix = ListToMatrix(&firstList);
	if (matrix != NULL) 
		{ // protect against memory failure
		result.numFound = firstList.numElements;
		result.Results = matrix;
		}

	// delete usage memory
	DelList(firstList); DelList(lastList);
	
	return result;
	}

	// ********************** Operator substract
	// Elimina de la clase de búsqueda los elementos que también estén en substract
/*cSearchLaraBD cSearchLaraBD::operator-(const cSearchLaraBD substract) const 
	{
	unsigned int nElements;
	unsigned short *matrix;
	cSearchLaraBD result; // object result
	
 	nElements = numFound; matrix = Results;
	tListResults firstList = MatrixToList(nElements, matrix);

	// track the list for coincidences
	NodeList **guide; int guide2 = 0;
	guide = &(firstList.first);
	NodeList *inicio=*guide;
	while((*guide)->next != NULL)
		{
		for(guide2=0;guide2<substract.numFound;guide2++)
			{
			if( (substract.Results[guide2]==(*guide)->next->value) )
				{//Elemento repetido
				NodeList *delNode=(*guide)->next;
				(*guide)->next = (*guide)->next->next;
				delete delNode;	
				firstList.numElements--;
				if(firstList.numElements==0)	break; //No more elements in list to be substracted					
				}
			}
		if((*guide)->next != NULL)	(*guide) = (*guide)->next;
		}

	NodeList *temp=inicio;
	matrix = ListToMatrix(temp, firstList.numElements);
	if (matrix != NULL) 
		{ // protect against memory failure
		result.numFound = firstList.numElements;
		result.Results = matrix;
		}
	// delete usage memory
	DelList(firstList);
	
	return result;
	}*/
	// ********************** Operator substract
	// Elimina de la clase de búsqueda los elementos que también estén en substract
cSearchLaraBD cSearchLaraBD::operator-(const cSearchLaraBD substract) const 
	{
	unsigned int nElements;
	unsigned short *matrix;
	cSearchLaraBD result; // object result
	
 	nElements = numFound; matrix = Results;
	tListResults firstList = MatrixToList(nElements, matrix);

	// track the list for coincidences
	NodeList **guide; int guide2 = 0;
	guide = &(firstList.first);
	while ((*guide) != NULL) 
		{
		// move guide2 to value equal or greater than guide
		while ((guide2 < substract.numFound)&&(substract.Results[guide2] < (*guide)->value))
			guide2++;
		if (guide2 == substract.numFound) break; // no more elements to substract in second search
		if (substract.Results[guide2] == (*guide)->value) 
			{ // element to substract
			NodeList *delNode = *guide;
			*guide = (*guide)->next; // bypass this element
			delete delNode; // del coincident element
			guide2++;
			firstList.numElements--;
			if (firstList.numElements == 0) break; // no more elements in list to be substracted
			}
		else
			guide = &((*guide)->next); // pass to next element
		}
	
	// convert first list in matrix form
	matrix = ListToMatrix(&firstList);
	if (matrix != NULL) 
		{ // protect against memory failure
		result.numFound = firstList.numElements;
		result.Results = matrix;
		}
	
	// delete usage memory
	DelList(firstList);
	
	return result;
	}

	// **************** Operator intersection
	// Deja en la clase de búsqueda sólo los elementos que también estén en last
cSearchLaraBD cSearchLaraBD::operator*(const cSearchLaraBD last) const 
	{
	unsigned int nElements;
	unsigned short *matrix;
	cSearchLaraBD result; // object result
	
	tListResults coincidentList = { NULL, 0 };

	// track the list for coincidences
	int guide = 0; int guide2 = 0;
	while (guide < numFound) 
		{
		// move guide2 to a element with value equal or greater
		// than the one signed by guide
		while ((guide2 < last.numFound)&&(last.Results[guide2] < Results[guide]))
			guide2++;
		if (guide2 == last.numFound) break; // no more elements to substract in second search
		if (last.Results[guide2] == Results[guide]) 
			{ // element coincident
			// Create a new node to add this element
			NodeList *newNode = new NodeList;
			if (newNode == NULL) break; // memory failure, no more adds
			newNode->value = Results[guide];
			AddResultToList(coincidentList, newNode);
			guide2++;
			}
		guide++;// pass to next element
		}
	
	// convert last list in matrix form
	matrix = ListToMatrix(&coincidentList);
	if (matrix != NULL) 
		{ // protect against memory failure
		result.numFound = coincidentList.numElements;
		result.Results = matrix;
		}
	
	// delete usage memory
	DelList(coincidentList);
	
	return result;
	}

//****************** DELETE REPEATED EMISSIONS *********************
/* En el caso de emisiones de descendientes puede que tengamos la misma
	emisión dos veces en las dos búsquedas que hacemos (una de naturales y
	otra general). Debemos eliminar las que nos aparecen como emisión de 
	descendientes y no del antecesor último (U, Th o Ra)
	Lo que hace la función es devolver una búsqueda que es la misma que la
	que se llama menos los elementos que tengan emisiones coincidentes con
	la búsqueda de naturales.
	NOTA: Por alguna razón, pasarle la base de datos entera hace que ésta
	luego de errores de segmentación en accesos posteriores, por eso he 
	optado por pasarle sólo el array de emisiones copiado mediante la llamada
	GetEmissions que he creado para ello (ver, por ejemplo, su uso en VisIdent.C,
	función ShowCandidates().
	*/
cSearchLaraBD cSearchLaraBD::deleteRepeatedEmissions(cSearchLaraBD naturales, TFileEmissionLaraBD *tfile)
	{
	unsigned int nElements;
	unsigned short *matrix;
	cSearchLaraBD result; // object result
	
 	nElements = numFound; matrix = Results;
	tListResults firstList = MatrixToList(nElements, matrix);
	// track the list for coincidences
	NodeList **guide; int guide2 = 0;
	guide = &(firstList.first);
	while ((*guide) != NULL) 
		{
		// move guide2 to value equal or greater than guide
		while ((guide2 < naturales.numFound)&&(naturales.Results[guide2] < (*guide)->value))
			guide2++;
		if (guide2 == naturales.numFound) break; // no more elements to substract in second search
		//Si ese elemento lo tenemos en la segunda consulta
		if (tfile[naturales.Results[guide2]].energy == tfile[(*guide)->value].energy ) 
			{ // Lo quitamos
			printf("Vamos a borrar el elemento %d\n", (*guide)->value);
			NodeList *delNode = *guide;
			printf("Saltamos al siguiente\n");
			*guide = (*guide)->next; // bypass this element
			delete delNode; // del coincident element
			printf("Aumentamos guide2 y disminuimos el número de elementos\n");
			guide2++;
			firstList.numElements--;
			if (firstList.numElements == 0) break; // no more elements in list to be substracted
			}
		else
			guide = &((*guide)->next); // pass to next element
		}
	
	// convert first list in matrix form
	matrix = ListToMatrix(&firstList);
	if (matrix != NULL) 
		{ // protect against memory failure
		result.numFound = firstList.numElements;
		result.Results = matrix;
		}
	
	// delete usage memory
	DelList(firstList);
	
	return result;
	
	}

	// ********************* Operator Assign
	// Copia en la clase de búsqueda los elementos de copy
cSearchLaraBD& cSearchLaraBD::operator=(const cSearchLaraBD& copy) 
	{
	if (this != &copy) 
		{ // avoid autoassign
		// delete used memory
		delete [] Results;

		if ((copy.numFound == 0)||(copy.Results == NULL)) 
			{ // no data in object to copy
			Results = NULL;
			numFound = 0; Guide = 0;
			}
		else 
			{
			// alloc memory for new data
			Results = new unsigned short int [copy.numFound];
			if (Results != NULL) 
				{
				// make a copy of results
				//printf("usamos memcpy\n");
				memcpy(Results, copy.Results, copy.numFound*sizeof(unsigned short int));
				numFound = copy.numFound;
				Guide = copy.Guide;
				}
			}
		}
	return *this;
	}


	
/* ***************************
	**     Results Methods   **
	***************************/
int cSearchLaraBD::GetFirstResult() 
	{
	if (numFound > 0) 
		{
		Guide = 1;
		return Results[0];
		}
	return -1;
	}

int cSearchLaraBD::GetLastResult() 
	{
	if (numFound > 0)	return Results[(Guide=numFound)-1];
	return -1;
	}

int cSearchLaraBD::GetNextResult() 
	{
	if (numFound > 0)	if (Guide < numFound)	return Results[Guide++];
	return -1;
	}

int cSearchLaraBD::GetPriorResult() 
	{
	if (numFound > 0)	if (Guide > 1)	return Results[--Guide];
	return -1;
	}

int cSearchLaraBD::GetResult(unsigned int pos) 
	{
	if( (numFound > 0) && ((pos > 0) || (pos <= numFound)) ) 
		{
		Guide = pos;
		return Results[Guide-1];
		}
	return -1;
	}

int cSearchLaraBD::GetFastResult(unsigned int pos) 
	{
	if ( (numFound > 0)&& ((pos > 0) || (pos <= numFound)) )	return Results[pos-1];
	return -1;
	}


	
/*****************************************************
 ***																***
 ***								LARA BD						***
 ***																***
 *****************************************************/
/* ***************
		constants
	***************/
const char *defaultFile = "lara.BD";
const char *signature = "analgamXXXXXX";

/* ***************************
	**       file Read       **
	***************************/

//************ SIZEOF TFILE ELEMENT LARABD *************
// Calcula el número de elementos de la base de datos
// NOTA: No entiendo cómo lo hace
size_t sizeofTFileElementLaraBD() 
	{
	TFileElementLaraBD p;
	size_t temp = sizeof(p.key) + sizeof(p.name) + 1; // +1 -> bool
	temp += sizeof(p.decayTime)*2 + sizeof(p.listEmissions);
	return temp;
	}

size_t sizeofTFileEmissionLaraBD() 
	{
	TFileEmissionLaraBD p;
	size_t temp = sizeof(p.energy)*4 + sizeof(p.kindEmission);
	temp += sizeof(p.posEmissor);
	return temp;
	}

size_t sizeofTHeaderLaraBD() 
	{
	THeaderLaraBD p;
	return sizeof(p.signature) + sizeof(p.offElements)*5;
	}

//************** FREADHEADER *********************
/** Lee de la cabecera de la base de datos distinta información:
	-
	- 
*/
THeaderLaraBD freadHeader(FILE *handle) 
	{
	THeaderLaraBD Header;
	printf("Leemos la cabecera\n");
	fileRead(&Header.signature, 1, sizeof(Header.signature), handle);
	printf("Signature: %d\n", Header.signature);
	fileRead(&Header.offElements, sizeof(Header.offElements), 1, handle);
	printf("offElements: %d\n", Header.offElements);
	fileRead(&Header.offEmissions, sizeof(Header.offEmissions), 1, handle);
	printf("offEmissions: %d\n", Header.offEmissions);
	fileRead(&Header.offListEmissions, sizeof(Header.offListEmissions), 1, handle);
	printf("offListEmissions: %d\n", Header.offListEmissions);
	fileRead(&Header.offSummaryElements, sizeof(Header.offSummaryElements), 1, handle);
	printf("offSummaryElements: %d\n", Header.offSummaryElements);
	fileRead(&Header.offSummaryEmissions, sizeof(Header.offSummaryEmissions), 1, handle);
	printf("offSummaryEmissions: %d\n", Header.offSummaryEmissions);
	
	return Header;
	}

//******************* FREAD FILE ELEMENT LARABD *******************
/**  Leemos un elemento de fichero de LaraBD
*/
TFileElementLaraBD freadFileElementLaraBD(FILE *handle) 
	{
	TFileElementLaraBD Element;
	fileRead(&Element.key, sizeof(Element.key), 1, handle);		
	fileRead(&Element.name, 1, sizeof(Element.name), handle);
	fileRead(&Element.decayTime, sizeof(Element.decayTime), 1, handle);
	fileRead(&Element.incDecayTime, sizeof(Element.incDecayTime), 1, handle);
	fileReadBool(Element.Natural, handle);
	fileRead(&Element.listEmissions, sizeof(Element.listEmissions), 1, handle);
	fileReadBool(Element.equilibrio, handle);
	return Element;
	}
	
//******************* FREAD FILE EMISSION LARABD *******************
/**  Leemos una emisión de fichero de LaraBD
*/
TFileEmissionLaraBD freadFileEmissionLaraBD(FILE *handle) 
	{
	TFileEmissionLaraBD Emision;
	
	
	fileRead(&Emision.energy, sizeof(Emision.energy), 1, handle);
	
	fileRead(&Emision.incEnergy, sizeof(Emision.incEnergy), 1, handle);
	fileRead(&Emision.probability, sizeof(Emision.probability), 1, handle);
	fileRead(&Emision.incProbability, sizeof(Emision.incProbability), 1, handle);
	fileRead(&Emision.kindEmission, sizeof(Emision.kindEmission), 1, handle);
	fileRead(&Emision.posEmissor, sizeof(Emision.posEmissor), 1, handle);

	return Emision;
	}

/* ***************************
	**     General Methods   **
	***************************/
	// Constructor
cLaraBD::cLaraBD() 
	{
	nElements = 0; nEmissions = 0;
	Elements = NULL; Emissions = NULL; 
	nListEmissions = 0; nSummaryElements = 0; nSummaryEmissions = 0;
	listEmissions = NULL; summaryElements = NULL; summaryEmissions = NULL;

	// Don't initialized yet
	State = false;
	}


//********************* LOAD ***********************
/** Carga la base de datos
	Parece que el fichero lara tiene la siguiente estructura
	
	signature offElements offEmissions offListEmissions offSummaryElements offSummaryEmissions
	[...]
	offElements
	[...]->clave nombre vmedia incvmedia natural lista emisiones
	offEmissions
	[...]->energía incenergia probabilidad incprobabilidad tipo posEmisor
	offListEmissions
	[..]
	offSummaryElements
	[...]
	offSummaryEmissions
*/
void cLaraBD::Load(char *otherAdress)
	{//=NULL
	// Clean possible anterior uses
	DeleteLaraBD();
	cLaraBD();
	printf("%s - otherAdress\n", otherAdress);
	printf("%s - defaultFile\n", defaultFile);
	// Take default name if no name is passed
	const char *fileName = (otherAdress != NULL) ? otherAdress : defaultFile;

	// Open database file
	FILE *laraBD;
	laraBD = fopen(fileName, "rb");
	printf("Abriendo base de datos en %s\n", fileName);
	if (laraBD == NULL) 
		{
		printf("Lara DataBase: No file found\n");
		return;
		}
	
	// get file size
	fseek(laraBD, 0, SEEK_END);
	unsigned int sizeFile = ftell(laraBD);
	fseek(laraBD, 0, SEEK_SET);
		
	// Load header file
	THeaderLaraBD Header = freadHeader(laraBD);
		
	if (feof(laraBD)) 
		{ // eof reached soon
		printf("Emission DataBase: File not valid\n");
		fclose(laraBD);
		return;
		}
	
	// Check Signature
	if (strncmp(signature, Header.signature, 7) != 0) 
		{
		printf("Emission DataBase: File not valid\n");
		fclose(laraBD);
		return;
		}
	
	// Load Blocks
	//El número de elementos se calcula como el trozo que ocupan los elementos partido
	//entre lo que ocupa cada elemento.
	nElements = (Header.offEmissions - Header.offElements)/sizeofTFileElementLaraBD();
	//El número de emisiones se calcula como el trozo que ocupan las emisiones 
	//dividido entre el tamaño de cada emisión.
	nEmissions = (Header.offListEmissions - Header.offEmissions)/sizeofTFileEmissionLaraBD();
	//Lo mismo con las otras tres cantidades
	nListEmissions = (Header.offSummaryElements - Header.offListEmissions)/sizeof(unsigned short int);
	nSummaryElements = (Header.offSummaryEmissions - Header.offSummaryElements)/sizeof(unsigned short int);
	nSummaryEmissions = (sizeFile - Header.offSummaryEmissions)/sizeof(unsigned short int);
	//Hacemos listas para todas esas cosas
	Elements = new TFileElementLaraBD[nElements];
	Emissions = new TFileEmissionLaraBD[nEmissions];
	listEmissions = new unsigned short int[nListEmissions];
	summaryElements = new unsigned short int[nSummaryElements];
	summaryEmissions = new unsigned short int[nSummaryEmissions];
	
	//Si alguno de los valores no existe o no se ha podido crear finalizamos con error
	if ((Elements == NULL)||(Emissions == NULL)||(listEmissions == NULL)||(summaryElements == NULL)||(summaryEmissions == NULL)) 
		{
		delete [] Elements; delete [] Emissions; delete [] listEmissions;
		delete [] summaryElements; delete [] summaryEmissions;
		printf("ERROR: Fallo al leer la base de datos\n");
		fclose(laraBD);
		return;
		}
	
	//Leemos y vamos almacenando las listas de elementos y emisiones		
	for (int i = 0; i < nElements; i++)		Elements[i] = freadFileElementLaraBD(laraBD);
	for (int i = 0; i < nEmissions; i++)	Emissions[i] = freadFileEmissionLaraBD(laraBD);
	fileRead(listEmissions, sizeof(listEmissions[0]), nListEmissions, laraBD);
	fileRead(summaryElements, sizeof(summaryElements[0]), nSummaryElements, laraBD);
	fileRead(summaryEmissions, sizeof(summaryEmissions[0]), nSummaryEmissions, laraBD);
	// close file handle
	fclose(laraBD);
	
	// Echo emission database used
	char buffer[80];
	strncpy(buffer, Header.signature, sizeof(Header.signature));
	buffer[sizeof(Header.signature)] = '\0';
	printf("Emission DataBase: %s Database loaded\n", &buffer[7]);
	State = true;
	
	}

	// Destructor
cLaraBD::~cLaraBD() 
	{
	DeleteLaraBD();
	}

void cLaraBD::DeleteLaraBD() 
	{
	if (Elements 			!= NULL)	delete [] Elements;
	if (Emissions			!= NULL)	delete [] Emissions;
	if (listEmissions		!= NULL)	delete [] listEmissions;
	if (summaryElements	!= NULL)	delete [] summaryElements;
	if (summaryEmissions != NULL) delete [] summaryEmissions;

	// Initialized Variables
	nElements = 0; nEmissions = 0;
	Elements = NULL; Emissions = NULL; 
	nListEmissions = 0; nSummaryElements = 0; nSummaryEmissions = 0;
	listEmissions = NULL; summaryElements = NULL; summaryEmissions = NULL;
	State = false;
	}
	
/* ***************************
	**    Elements Methods   **
	***************************/
//NOTA: np y na es un jaleo y lo he cambiado ya varias veces de sitio
	// Lo que funciona bien es lo que está comentado (1024 para los protones y
	// 0000000FF para el atómico)
unsigned short int cLaraBD::GetMassNumberFromKey(unsigned int key) 
	{
	return (int) (key / 1024);
	}

unsigned short int cLaraBD::GetAtomicNumberFromKey(unsigned int key) 
	{
	key = (key>>2) & 0x00000FF;
	return key;
	}

bool cLaraBD::IsMetaestableFromKey(unsigned int key) 
	{
	return ((key%2)==1);
	}

bool cLaraBD::IsEquiFromKey(unsigned int key) 
	{
	if ( (key & 0x00000002) == 2 )	return true;
	else					return false;
	}

unsigned short int cLaraBD::GetAtomicNumber(unsigned short int element) {
	if (element >= nElements) return 0;
	unsigned int key = Elements[element].key;
	return GetAtomicNumberFromKey(key);
}

unsigned short int cLaraBD::GetMassNumber(unsigned short int element) 
	{
	if (element >= nElements) return 0;
	unsigned int key = Elements[element].key;
	return GetMassNumberFromKey(key);
	}

//Devuelven el tiempo de desintegración en segundos
float cLaraBD::GetDecayTime(unsigned short int element) 
	{
	if (element >= nElements) return 0.0;
	return Elements[element].decayTime;
	}

float cLaraBD::GetIncDecayTime(unsigned short int element) 
	{
	if (element >= nElements) return 0.0;
	return Elements[element].incDecayTime;
	}
//

bool cLaraBD::IsMetaestable(unsigned short int element) 
	{
	if (element >= nElements) return false;
	unsigned int key = Elements[element].key;
	return IsMetaestableFromKey(key);
	}

bool cLaraBD::IsEqui(unsigned short int element) 
	{
	if (element >= nElements) return false;
	unsigned int key = Elements[element].key;
	return IsEquiFromKey(key);
	}

bool cLaraBD::IsNatural(unsigned short int element) 
	{
	if (element >= nElements) return false;
	return Elements[element].Natural;
	}

//************ GET NAME ELEMENT ******************
void cLaraBD::GetNameElement(unsigned short int element, char *name) 
	{
	if (element >= nElements) 
		{
		strcpy(name, "  ");
		return;
		}
	//
	//printf("nombre: |%s|\n", Elements[element].name);
	if(Elements[element].name[0]==' ')	
		{
		char temp[2];
		temp[0]=Elements[element].name[1];
		temp[1]='\0';
		strcpy(name, temp);
		}
	else	strcpy(name, Elements[element].name);
	//
	//strcpy(name, Elements[element].name);
	return;
	}

TFileEmissionLaraBD* cLaraBD::GetEmissions() 
	{
	return Emissions;
	}
	
/* ****************************
	**    Emissions Methods   **
	****************************/
unsigned short int cLaraBD::GetEmissor(unsigned short int emission) 
	{
//	printf("Emission: %d\tNumEmissions: %d\n", emission, nEmissions);
	if (emission >= nEmissions) return 0; // bug devolution -> return -1 for better dessign
//	printf("Emisor: %d", Emissions[emission].energy);
	return Emissions[emission].posEmissor;
	}

float cLaraBD::GetEnergy(unsigned short int emission) 
	{
	if (emission >= nEmissions) return 0.0;
	return Emissions[emission].energy;
	}

float cLaraBD::GetIncEnergy(unsigned short int emission) 
	{
	if (emission >= nEmissions) return 0.0;
	return Emissions[emission].incEnergy;
	}

float cLaraBD::GetProbability(unsigned short int emission) 
	{
	if (emission >= nEmissions) return 0.0;
	return Emissions[emission].probability;
	}

float cLaraBD::GetIncProbability(unsigned short int emission) 
	{
	if (emission >= nEmissions) return 0.0;
	return Emissions[emission].incProbability;
	}
	
char cLaraBD::GetKindEmission(unsigned short int emission) 
	{
	if (emission >= nEmissions)
		{ 
		printf("ERROR: Emission number greater than maximun emission\n");
		return '\0';
		}
	return Emissions[emission].kindEmission;
	}

/* ***************************
	**    Searchs Methods    **
	***************************/

//********************* SEARCH ELEMENTS ************************	
cSearchLaraBD cLaraBD::SearchElements(int minAtomic , int maxAtomic ,
			int minProton , int maxProton ,
			bool bMetaestable , bool vMetaestable ,
			bool bNatural , bool vNatural , char* name) 
	{//Los int=-1, los bool =false y el char*=NULL!

	cSearchLaraBD search;

	//***** DETERMINACIÓN DEL INTERVALO DE BÚSQUEDA
	// El intervalo de búsqueda lo determinan los cuatro primeros 
	// parámetros. A partir de ellos calcularemos el primer y el último
	// elemento entre los que realizar la búsqueda.
	// No podemos coger directamente el número que nos pasan como parámetro
	// porque para cada número atómico podemos tener distintos elementos
	// con distinos números de protones, etc.
//		printf("SEARCH ELEMENTS DE VERDAD\n");
	int firstElement = 0, lastElement = 0;
	// the first four parameters indicate the extension of the search
	// determination of first element
	if (minAtomic == -1)	firstElement = 0; // put the search start point in first element
	else 
		{
		minAtomic = abs(minAtomic);//Por si los valores negativos
		//Dividiendo por diez el número mínimo nos aseguramos que cogeremos
		//todas las claves que puedan contener ese número atómico
		unsigned short int div10 = minAtomic / 10; //rough approximation
		if (div10 >= nSummaryElements) return search;
		firstElement = summaryElements[div10];
		//Buscamos el primer elemento cuya clave contenga el número atómico minAtomic
		while ((GetAtomicNumberFromKey(Elements[firstElement].key) < minAtomic)&&
				(firstElement < nElements))
			{
			firstElement++;
			}
		if (firstElement == nElements) return search;//Si el mínimo está fuera de límites devolvemos null
		}

//	printf("EMPEZAMOS A BUSCAR POR PARTES\n");
	if (minProton != -1) 
		{
		minProton = abs(minProton);
			while ((GetMassNumberFromKey(Elements[firstElement].key) < minProton)&&
					(firstElement < nElements))
				firstElement++;
		}
	// determination of last element	
	if (maxAtomic == -1)
		lastElement = nElements - 1; // put the search end point in last element
	else 
		{
		maxAtomic = abs(maxAtomic);
		unsigned short int div10 = maxAtomic / 10; // rough approximation
		if (div10 >= nSummaryElements) lastElement = nElements - 1;
		else 
			{
			lastElement=nElements-1;
			//printf("la llave del último vale %d\n", Elements[lastElement].key);
			while( (GetAtomicNumberFromKey(Elements[lastElement].key) > maxAtomic) && lastElement > firstElement )
				lastElement--;				
			}
		}

	//Similar a la búsqueda del primer elemento 
	if (maxProton != -1) 
		{
		maxProton = abs(maxProton);
		if ((maxAtomic == -1)&&(maxProton > 4)) 
			{ // problems here, table ir ordered by atomic number
			// find maxAtomic near to maxProton
			unsigned short int startAtomic = maxProton*2;
			unsigned short int div10 = abs(startAtomic) / 10;
			unsigned short int first;
			if (div10 >= nSummaryElements) first = nElements - 1;
			else 
				{
				first = summaryElements[div10]; // rough estimation
				while ((GetAtomicNumberFromKey(Elements[first].key) < startAtomic)&&
						(first < nElements))
					first++;
				}
			unsigned short int endAtomic = (int) (minProton*2.61) + 1;
			div10 = abs(endAtomic) / 10;
			unsigned short int last;
			if (div10 >= nSummaryElements) last = nElements-1;
			else 
				{
				last = summaryElements[div10]; // rough estimation
				while ((GetAtomicNumberFromKey(Elements[last].key) < endAtomic)&&
						(last < nElements))
					last++;
				}
			if (first > last) lastElement = nElements - 1;
			else 
				{
				// search the first ocurrence (or at least minimum value greater than num protons)
				unsigned short int maximum = last++;
				unsigned short int maximumProton = GetMassNumberFromKey(Elements[maximum].key);
				for (int i = last; i >= first; i--) 
					{
					unsigned short int temp = GetMassNumberFromKey(Elements[i].key);
					if (temp <= maxProton) 
						{
						maximum = i;
						break;
						}
					if ((temp > maxProton)&&(temp < maximumProton)) 
						{ // found better aproximation to maxProton
						maximumProton = temp;
						maximum = i;
						}
					}
				// here, we have the better approximation to maxProton in maximum
				lastElement = maximum;
				}
			}
		else if (maxAtomic == -1) 
			{
			if (maxProton == 4) lastElement = 0;
			else return search;
			}
		else 
			{
			// TODO: optimize code
			while ((GetMassNumberFromKey(Elements[lastElement].key) > maxProton)&&
					(lastElement >= 0))
				lastElement--;
			if (lastElement < 0) return search;
			}
		}
//	printf("Después de buscar según el máximo nos quedamos por arriba en %d %d\n", GetAtomicNumber(lastElement), GetMassNumber(lastElement));
	if (lastElement < firstElement) return search;

	// Preceding calculus for maximum and minimum protons are only approximate.
	// They are used to decrease search interval.
	// Next calculus must check this values too before adding to search results
	
	//Chequeo de todas las características que nos quedan de la búsqueda,
	//incluida una validación del número de protones
	//Si alguna de estas validaciones es incorrecta, ese elemento se salta
	//y no se introduce en el resultado final de la búsqueda
	//
	firstElement=0;
	lastElement=nElements-1;
	//printf("Intervalo de búsqueda: [%d,%d]\n", firstElement, lastElement);
	tListResults resultsList = { NULL, 0};
	for (int i = firstElement; i <= lastElement; i++) 
		{
		TFileElementLaraBD *element = &Elements[i]; // fast "copy", only redirection
		unsigned short int numProton = GetMassNumberFromKey(element->key);
		unsigned short int numAtomic = GetAtomicNumberFromKey(element->key);
		if ((minAtomic != -1)&&(numAtomic < minAtomic))
			{/*printf("No llega a minAtomic\n");*/ continue;}
		if ((maxAtomic != -1)&&(numAtomic > maxAtomic))
			{/*printf("Supera maxAtomic\n");*/ continue;}
		if ((minProton != -1)&&(numProton < minProton))
			{/*printf("No llega a minProton\n");*/ continue;}
		if ((maxProton != -1)&&(numProton > maxProton))
			{/*printf("Supera maxProton\n");*/ continue;}
		if ((bMetaestable)&&(IsMetaestableFromKey(element->key) != vMetaestable))
			{/*printf("No es metaestable %d\n", i);*/ continue;}
	/*	if ((bEquilibrio)&&(IsEquiFromKey(element->key) != vEquilibrio))
			{printf("No está en equilibrio %d\n", i);continue;}		*/
		if ((bNatural)&&(element->Natural != vNatural))
			{/*printf("No es natural %d\n", i);*/ continue;}
		if (((name != NULL)&&(strlen(name) > 0))&&(strcmp(element->name, name) != 0))
			{/*printf("Nombre incorrecto\n");*/ continue;} 	
		NodeList *newResult = new NodeList;
		newResult->value = i;
		newResult->next = NULL;
		AddResultToList(resultsList, newResult);
		}
	// Reserve memory for results
	search.Results = ListToMatrix(&resultsList);
	if (search.Results != NULL) // protect against memory failure
		search.numFound = resultsList.numElements;
	DelList(resultsList);
//	printf("Volvemos\n");
	return search;
	}

//******************* SEARCH EMISSIONS *************************
/** Busca los elementos con emisiones más probables según los distintos parámetros
	- minEnergy/maxEnergy: intervalo de energías de emisión
	- minProb/maxProb: intervalo de probabilidades de emisión
	- kindEmission: Tipo de emisión que buscamos (X, Gamma, etc.)
	- minAtomic/maxAtomic: intervalo de número atómico del emisor
	- minProton/maxProton:						de protones del emisor 
	- bMetaestable: Indica si buscamos la metaestabilidad del emisor
	- vMetaestable: Si el anterior esta activo, indica el valor de metaestabilidad que buscamos
	- bNatural: Indica si buscamos naturalidad del emisor
	- vNatural: Si el anterior está activo, indica el valor de naturalidad
				a true, buscamos elementos naturales, a false, no naturales
				Si bNatural tiene un valor distinto de true, el valor que
				haya en vNatural se ignora
	- name: Busca por el nombre del elemento tal y como se escribe en la tabla
			periódica
			NOTA: Los elementos de una sola letra (U, P, K) deben buscarse pasando
				la cadena " U", " P", " K"!
	- equilibrio: Busca sólo en las listas de equilibrio para descendientes de los
				  elementos naturales (busca emisiones directas e indirectas)
	
	
  Devolvemos un objeto cSearchLaraBD con los elementos encontrados
  NOTA: Antes todos los parámetros venían con valor por defecto a -1/false/null
  	Por errores en compilación quitamos los valores por defecto. 
	El resultado es el mismo, si llamamos a la función sin esos parámetros
	nos los va a poner a -1.
 */
cSearchLaraBD cLaraBD::SearchEmissions(float minEnergy , float maxEnergy,
			float minProb , float maxProb , int kindEmission ,
			int minAtomic , int maxAtomic ,
			int minProton , int maxProton ,
			bool bMetaestable , bool vMetaestable ,
			bool bNatural , bool vNatural , char* name,
			bool bEquilibrio, bool vEquilibrio)//int y float =-1, bool=false y char*=NULL
	{
	//printf("Empezamos la búsqueda\n");
	cSearchLaraBD search;

	int firstElement = 0, lastElement = 0;
	// the first four parameters indicate the extension of the search
	// determination of first element
	if (minEnergy <= 0) 
		// put the search start point in first element
		firstElement = 0;
	else 	
		{
		unsigned short int div10 = (int) floor(minEnergy / 10.0); // rough approximation
		if (div10 >= nSummaryEmissions) return search;
		firstElement = summaryEmissions[div10];
		//Recorremos la lista de emisiones hasta llegar a la primera en ese rango
		while ((firstElement < nEmissions) && (Emissions[firstElement].energy < minEnergy))	
			firstElement++;
		if (firstElement == nEmissions) return search;
		}
	// determination of last element	
	if (maxEnergy <= 0)	lastElement = nEmissions - 1; // put the search end point in last element
	else 	
		{
		unsigned short int div10 = (int)floor(maxEnergy / 10.0); // rough approximation
		if (div10 >= nSummaryEmissions) lastElement = nEmissions - 1;
		//Hacemos lo mismo para la energía máxima.
		else 	
			{
			lastElement = summaryEmissions[div10];
			while ((lastElement < nEmissions) && (Emissions[lastElement].energy <= maxEnergy))  lastElement++;

/*			NOTA: Este trozo es el que sustituimos por el de arriba con buen
				  resultado en la búsqueda de elementos.
			lastElement=nElements;
			while( (GetAtomicNumberFromKey(Elements[lastElement].key) > maxAtomic) && lastElement > firstElement )
				lastElement--;				*/
			lastElement--;
			}
		}
	//En caso de que el intervalo sea erróneo devolvemos null.
	if (lastElement < firstElement) return search;

	//Recorremos para todos los elementos
	tListResults resultsList = {NULL, 0};
//	printf("Intervalo de búsqueda: [%d,%d]\n", firstElement, lastElement);
	//NOTA: OJO!!Temporal, para pruebas
	//firstElement=0; lastElement=nEmissions-1;
	//
	for (int i = firstElement; i <= lastElement; i++) 
		{
		TFileEmissionLaraBD *emission = &Emissions[i]; // fast "copy", only redirection
		//Hace comprobaciones con todos los parámetros
		if ((minProb > 0)&&(emission->probability < minProb))
			{
	//		printf("Menor probabilidad\n"); 
			continue;
			}
		if ((maxProb > 0)&&(emission->probability > maxProb))
			{
		//	printf("Mayor probabilidad\n"); 
			continue;
			}
		//Si el tipo de emisión existe y equivale al de esta emisión, seguimos
		if ((kindEmission != -1)&&(emission->kindEmission != kindEmission))
			{
		//	printf("Distinta emisión \n"); 
			continue;
			}
		//Tomamos el elemento padre de la emisión
		TFileElementLaraBD *element = &Elements[emission->posEmissor];
		unsigned short int numAtomic = GetAtomicNumberFromKey(element->key);
		if ((minAtomic != -1)&&(numAtomic < minAtomic))
			{
		//	printf("Menor numero atomico\n"); 
			continue;
			}
		if ((maxAtomic != -1)&&(numAtomic > maxAtomic))
			{
		//	printf("Mayor número atómico\n"); 
			continue;
			}
		unsigned short int numProton = GetMassNumberFromKey(element->key);
		if ((minProton != -1)&&(numProton < minProton))
			{
		//	printf("Menor número másico %d (min %d)\n", numProton, minProton); 
			continue;
			}
		if ((maxProton != -1)&&(numProton > maxProton))
			{
		//	printf("Mayor número másico (max=%d) num=%d\n", maxProton, numProton); 
			continue;
			}
		if ((bMetaestable)&&(IsMetaestableFromKey(element->key) != vMetaestable))
			{
		//	printf("Metaestabilidad contraria a la buscada\n"); 
			continue;
			}
		if ((bNatural)&&(element->Natural != vNatural))
			{
		//	printf("Naturalidad contraria a la buscada\n"); 
			continue;
			}
		if (((name != NULL)&&(strlen(name) > 0))&&(strcmp(element->name, name) != 0))
			{
		//	printf("El nombre no es el mismo: %s--%s.\n", element->name, name);
			continue;
			}
		if ((bEquilibrio)&&(IsEquiFromKey(element->key) != vEquilibrio))
		//if ((equilibrio)&&(IsEquiFromKey(element->key) != true))
			{
		//	printf("Equilibrio distinto del deseado\n"); 
			continue;
			}
			
		NodeList *newResult = new NodeList;
		newResult->value = i;
		newResult->next = NULL;
		AddResultToList(resultsList, newResult);
		}
	// Reserve memory for results
	search.Results = ListToMatrix(&resultsList);
	if (search.Results != NULL) // protect against memory failure
		search.numFound = resultsList.numElements;
	
	DelList(resultsList);

	return search;
}

//******************* SEARCH EMISSIONS **********************
/*	Versión para buscar directamente a través del código del elemento
*/
cSearchLaraBD cLaraBD::SearchEmissionsByCode(float minEnergy, float maxEnergy, 
								float minProb, float maxProb, int codElemento)
	{
	int a=GetAtomicNumber(codElemento);
	int m=GetMassNumber(codElemento);
	bool meta=IsMetaestable(codElemento);
	bool eq=IsEqui(codElemento);
	bool n=IsNatural(codElemento);
	//char *name=new char[3];
	//GetNameElement(codElemento, name);
	return SearchEmissions(minEnergy,maxEnergy,minProb,maxProb,0,a,a,m,m,true,meta,true,n,NULL,true, eq);  
	}
	
//******************* SEARCH EMISSIONS INTERNAL*************************
/**Como la anterior, pero devuelve una lista interna
 */
unsigned short int *cLaraBD::SearchEmissionsInternal(float minEnergy , float maxEnergy,
			float minProb , float maxProb , int kindEmission ,
			int minAtomic , int maxAtomic ,
			int minProton , int maxProton ,
			bool bMetaestable , bool vMetaestable ,
			bool bNatural , bool vNatural , char* name,
			bool bEquilibrio, bool vEquilibrio)//int y float =-1, bool=false y char*=NULL
	{
	//printf("Empezamos la búsqueda\n");
	//cSearchLaraBD search;
	//Recorremos para todos los elementos
	tListResults search = {NULL, 0};

	int firstElement = 0, lastElement = 0;
	// the first four parameters indicate the extension of the search
	// determination of first element
	if (minEnergy <= 0) 
		// put the search start point in first element
		firstElement = 0;
	else 	
		{
		unsigned short int div10 = (int) floor(minEnergy / 10.0); // rough approximation
		if (div10 >= nSummaryEmissions) return ListToMatrix(&search);
		firstElement = summaryEmissions[div10];
		//Recorremos la lista de emisiones hasta llegar a la primera en ese rango
		while ((firstElement < nEmissions) && (Emissions[firstElement].energy < minEnergy))	
			firstElement++;
		if (firstElement == nEmissions) return ListToMatrix(&search);
		}
	// determination of last element	
	if (maxEnergy <= 0)	lastElement = nEmissions - 1; // put the search end point in last element
	else 	
		{
		unsigned short int div10 = (int)floor(maxEnergy / 10.0); // rough approximation
		if (div10 >= nSummaryEmissions) lastElement = nEmissions - 1;
		//Hacemos lo mismo para la energía máxima.
		else 	
			{
			lastElement = summaryEmissions[div10];
			while ((lastElement < nEmissions) && (Emissions[lastElement].energy <= maxEnergy))  lastElement++;

/*			NOTA: Este trozo es el que sustituimos por el de arriba con buen
				  resultado en la búsqueda de elementos.
			lastElement=nElements;
			while( (GetAtomicNumberFromKey(Elements[lastElement].key) > maxAtomic) && lastElement > firstElement )
				lastElement--;				*/
			lastElement--;
			}
		}
	//En caso de que el intervalo sea erróneo devolvemos null.
	if (lastElement < firstElement) return ListToMatrix(&search);

	//Recorremos para todos los elementos
	tListResults resultsList = {NULL, 0};
//	printf("Intervalo de búsqueda: [%d,%d]\n", firstElement, lastElement);
	//NOTA: OJO!!Temporal, para pruebas
	//firstElement=0; lastElement=nEmissions-1;
	//
	for (int i = firstElement; i <= lastElement; i++) 
		{
		TFileEmissionLaraBD *emission = &Emissions[i]; // fast "copy", only redirection
		//Hace comprobaciones con todos los parámetros
		if ((minProb > 0)&&(emission->probability < minProb))
			{
	//		printf("Menor probabilidad\n"); 
			continue;
			}
		if ((maxProb > 0)&&(emission->probability > maxProb))
			{
		//	printf("Mayor probabilidad\n"); 
			continue;
			}
		//Si el tipo de emisión existe y equivale al de esta emisión, seguimos
		if ((kindEmission != -1)&&(emission->kindEmission != kindEmission))
			{
		//	printf("Distinta emisión \n"); 
			continue;
			}
		//Tomamos el elemento padre de la emisión
		TFileElementLaraBD *element = &Elements[emission->posEmissor];
		unsigned short int numAtomic = GetAtomicNumberFromKey(element->key);
		if ((minAtomic != -1)&&(numAtomic < minAtomic))
			{
		//	printf("Menor numero atomico\n"); 
			continue;
			}
		if ((maxAtomic != -1)&&(numAtomic > maxAtomic))
			{
		//	printf("Mayor número atómico\n"); 
			continue;
			}
		unsigned short int numProton = GetMassNumberFromKey(element->key);
		if ((minProton != -1)&&(numProton < minProton))
			{
		//	printf("Menor número másico %d (min %d)\n", numProton, minProton); 
			continue;
			}
		if ((maxProton != -1)&&(numProton > maxProton))
			{
		//	printf("Mayor número másico (max=%d) num=%d\n", maxProton, numProton); 
			continue;
			}
		if ((bMetaestable)&&(IsMetaestableFromKey(element->key) != vMetaestable))
			{
		//	printf("Metaestabilidad contraria a la buscada\n"); 
			continue;
			}
		if ((bNatural)&&(element->Natural != vNatural))
			{
		//	printf("Naturalidad contraria a la buscada\n"); 
			continue;
			}
		if (((name != NULL)&&(strlen(name) > 0))&&(strcmp(element->name, name) != 0))
			{
		//	printf("El nombre no es el mismo: %s--%s.\n", element->name, name);
			continue;
			}
		if ((bEquilibrio)&&(IsEquiFromKey(element->key) != vEquilibrio))
			{
		//	printf("Equilibrio distinto del deseado\n"); 
			continue;
			}
			
		NodeList *newResult = new NodeList;
		newResult->value = i;
		newResult->next = NULL;
		AddResultToList(resultsList, newResult);
		}
	return ListToMatrix(&resultsList);
}


//******************* SEARCH EMISSIONS AND DESCENDANTS*************************
/** Como SearchEmissions, pero para cada emisión, si es un natural, le añade
	las emisiones de sus descendientes, si existieran para esas mismas energías
 */
cSearchLaraBD cLaraBD::SearchEmissionsAndDescendants(float minEnergy , float maxEnergy,
			float minProb , float maxProb , int kindEmission ,
			int minAtomic , int maxAtomic ,
			int minProton , int maxProton ,
			bool bMetaestable , bool vMetaestable ,
			bool bNatural , bool vNatural , char* name,
			bool bEquilibrio, bool vEquilibrio)//int y float =-1, bool=false y char*=NULL
	{
	cSearchLaraBD search;

	int firstElement = 0, lastElement = 0;
	// the first four parameters indicate the extension of the search
	// determination of first element
	if (minEnergy <= 0) 
		// put the search start point in first element
		firstElement = 0;
	else 	
		{
		unsigned short int div10 = (int) floor(minEnergy / 10.0); // rough approximation
		if (div10 >= nSummaryEmissions) return search;
		firstElement = summaryEmissions[div10];
		//Recorremos la lista de emisiones hasta llegar a la primera en ese rango
		while ((firstElement < nEmissions) && (Emissions[firstElement].energy < minEnergy))	
			firstElement++;
		if (firstElement == nEmissions) return search;
		}
	// determination of last element	
	if (maxEnergy <= 0)	lastElement = nEmissions - 1; // put the search end point in last element
	else 	
		{
		unsigned short int div10 = (int)floor(maxEnergy / 10.0); // rough approximation
		if (div10 >= nSummaryEmissions) lastElement = nEmissions - 1;
		//Hacemos lo mismo para la energía máxima.
		else 	
			{
			lastElement = summaryEmissions[div10];
			while ((lastElement < nEmissions) && (Emissions[lastElement].energy <= maxEnergy))  lastElement++;

			lastElement--;
			}
		}
	//En caso de que el intervalo sea erróneo devolvemos null.
	if (lastElement < firstElement) return search;

	//Recorremos para todos los elementos
	tListResults resultsList = {NULL, 0};
//	printf("Intervalo de búsqueda: [%d,%d]\n", firstElement, lastElement);
	for (int i = firstElement; i <= lastElement; i++) 
		{
		TFileEmissionLaraBD *emission = &Emissions[i]; // fast "copy", only redirection
		//Hace comprobaciones con todos los parámetros
		if ((minProb > 0)&&(emission->probability < minProb))
			{
	//		printf("Menor probabilidad\n"); 
			continue;
			}
		if ((maxProb > 0)&&(emission->probability > maxProb))
			{
		//	printf("Mayor probabilidad\n"); 
			continue;
			}
		//Si el tipo de emisión existe y equivale al de esta emisión, seguimos
		if ((kindEmission != -1)&&(emission->kindEmission != kindEmission))
			{
		//	printf("Distinta emisión \n"); 
			continue;
			}
		//Tomamos el elemento padre de la emisión
		TFileElementLaraBD *element = &Elements[emission->posEmissor];
		unsigned short int numAtomic = GetAtomicNumberFromKey(element->key);
		if ((minAtomic != -1)&&(numAtomic < minAtomic))
			{
		//	printf("Menor numero atomico\n"); 
			continue;
			}
		if ((maxAtomic != -1)&&(numAtomic > maxAtomic))
			{
		//	printf("Mayor número atómico\n"); 
			continue;
			}
		unsigned short int numProton = GetMassNumberFromKey(element->key);
		if ((minProton != -1)&&(numProton < minProton))
			{
		//	printf("Menor número másico %d (min %d)\n", numProton, minProton); 
			continue;
			}
		if ((maxProton != -1)&&(numProton > maxProton))
			{
		//	printf("Mayor número másico (max=%d) num=%d\n", maxProton, numProton); 
			continue;
			}
		if ((bMetaestable)&&(IsMetaestableFromKey(element->key) != vMetaestable))
			{
		//	printf("Metaestabilidad contraria a la buscada\n"); 
			continue;
			}
		if ((bNatural)&&(element->Natural != vNatural))
			{
		//	printf("Naturalidad contraria a la buscada\n"); 
			continue;
			}
		if (((name != NULL)&&(strlen(name) > 0))&&(strcmp(element->name, name) != 0))
			{
		//	printf("El nombre no es el mismo: %s--%s.\n", element->name, name);
			continue;
			}
		if ((bEquilibrio)&&(IsEquiFromKey(element->key) != vEquilibrio))
			{
			//printf("El equilibrio no es %d\n", vEquilibrio); 
			continue;
			}
			
		if(numProton==226 || numProton==230 || numProton==232 || 
		   numProton==234 || numProton==235 || numProton==238 )
			{
			double energiaPadre=emission->energy;
	//		printf("DEBERÍAMOS MOSTRAR SUS HIJOS, ENERGÍA: %f\n", energiaPadre);
			int minEl, maxEl;
			
			switch(numProton)
				{
				case 226:
					minEl=familias[5*2];	maxEl=familias[5*2+1];
					break;
				case 230:
					minEl=familias[4*2];	maxEl=familias[4*2+1];
					break;
				case 232:
					minEl=familias[0];	maxEl=familias[1];
					break;
				case 234:
					minEl=familias[3*2];	maxEl=familias[3*2+1];
					break;
				case 235:
					minEl=familias[1*2];	maxEl=familias[1*2+1];
					break;
				case 238:
					minEl=familias[2*2];	maxEl=familias[2*2+1];
					break;
				}
			int cont=0;
			//printf("Buscando desde %d a %d\n", minEl, maxEl);
			for(int i=minEl;i<maxEl;i+=3)//Para todos sus descendientes buscamos el asunto
				{
				cSearchLaraBD f;
				unsigned short int *tlr;
				tlr=SearchEmissionsInternal(energiaPadre-0.03 , energiaPadre+0.03,-1,-1,-1,naturales[i+1],naturales[i+1],naturales[i],naturales[i],false,false,false,false,"",false,false);
				f=SearchEmissions(energiaPadre-0.03 , energiaPadre+0.03,-1,-1,-1,naturales[i+1],naturales[i+1],naturales[i],naturales[i],false,false,false,false,"",false,false);
				//printf("Para el elemento %d,%d hay %d coincidencias\n", naturales[i], naturales[i+1], f.GetNumItemsFound());
				for(int j=0;j<f.GetNumItemsFound();j++)
					{
//					printf("Añadimos un nuevo nodo con la nueva coincidencia\n");
					if(j<f.GetNumItemsFound()-1 && f.GetNumItemsFound()>1 && i==minEl)//Si está en equilibrio consigo mismo no ponemos el de equilibrio
						j++;
					NodeList *newResult = new NodeList;
					newResult->value = tlr[j];
					newResult->next = NULL;
					AddResultToList(resultsList, newResult);
					}
				}
			}
		////NUEVO ^^^

			
		}
	// Reserve memory for results
	search.Results = ListToMatrix(&resultsList);
	if (search.Results != NULL) // protect against memory failure
		search.numFound = resultsList.numElements;
	
	DelList(resultsList);

	return search;
}


//************ DA EMISION CON MAYOR PROBABILIDAD **********
/*	Devuelve la emisión de mayor probabilidad en todo el espectro
	*/
int cLaraBD::daEmisionConMayorProbabilidad(int elemento)
	{
	return daEmisionConMayorProbabilidad(elemento, -1,-1);
	}
	
//************ DA EMISION CON MAYOR PROBABILIDAD **********
/*	Devuelve la emisión gamma de mayor probabilidad en [eInf, eSup] keV
	*/
int cLaraBD::daEmisionConMayorProbabilidad(int elemento, float eInf, float eSup)
	{
	cSearchLaraBD founds;
	int m=GetMassNumber(elemento);
	int a=GetAtomicNumber(elemento);
	//printf("DA EMISIÓN CON MAYOR PROBABILIDAD PARA %d,%d en [%f,%f]\n", a, m, eInf, eSup);
	char name[3]="";
	GetNameElement(elemento, name);
	
//	founds = SearchEmissions(eInf, eSup, -1,-1,0, a,a,m,m,false,false,false,false,"", false, false);
	//NOTA: Probando, no contemplamos emisiones que puedan encontrarse en la zona de aniquilación
	founds = SearchEmissions(eInf, 505, -1,-1,0, a,a,m,m,false,false,false,false,"", false, false);
	founds = founds + SearchEmissions(515, eSup, -1,-1,0, a,a,m,m,false,false,false,false,"", false, false);
	
	int emi=founds.GetFirstResult();
	float prob=0;
	int emiMaxProb=0;
	
	for(int i=0;i<founds.GetNumItemsFound();i++)
		{
		if(GetProbability(emi)>prob)	
			{
			emiMaxProb=emi;
			prob=GetProbability(emi);
			}
		emi=founds.GetNextResult();
		}
	return emiMaxProb;	
	}

//**************** NUM EMISIONES GRANDES ******************
/*	Devuelve el número de emisiones en el rango [eInf,eSup] con probabilidad
	mayor que probMin pra el elemento.
	*/
int cLaraBD::numEmisionesGrandes(int elemento, float eInf, float eSup, float probMin)
	{
	cSearchLaraBD founds;
	int m=GetMassNumber(elemento);
	int a=GetAtomicNumber(elemento);
	//printf("DA EMISIÓN CON MAYOR PROBABILIDAD PARA %d,%d en [%f,%f]\n", a, m, eInf, eSup);
	
	char name[3];
	GetNameElement(elemento, name);
	
	//NOTA: Probando, no contemplamos emisiones que puedan encontrarse en la zona de aniquilación
//	founds = SearchEmissions(eInf, eSup, probMin,-1,0, a,a,m,m,false,false,false,false,"", false, false);
	founds = SearchEmissions(eInf, 505, probMin,-1,0, a,a,m,m,false,false,false,false,"", false, false);
	founds = founds + SearchEmissions(515, eSup, probMin,-1,0, a,a,m,m,false,false,false,false,"", false, false);
	
	return founds.GetNumItemsFound();
	}
	
//***************** DA EMISIONES CON MAYOR PROBABILIDAD ************************
/* Similar a daEmisionConMayorProbabilidad, pero en este caso damos todas las emisiones,
	ordenadas por probabilidad, hasta la probabilidad mínima que se pase como parámetro.
	Se reserva memoria y se almacenan en emisiones. Es responsabilidad de quien llame
	a esta función la liberación de dicha memoria
	*/
void cLaraBD::daEmisionesConMayorProbabilidad(int elemento,float eInf,float eSup, int *emisiones, float probMin)
	{
	cSearchLaraBD founds;
	int m=GetMassNumber(elemento);
	int a=GetAtomicNumber(elemento);
	//printf("DA EMISIÓN CON MAYOR PROBABILIDAD PARA %d,%d en [%f,%f]\n", a, m, eInf, eSup);
	
//	printf("pillamos nombre\n");
	char name[3];
	GetNameElement(elemento, name);
	
//	printf("buscamos emisiones\n");
	//NOTA: Probando, no contemplamos emisiones que puedan encontrarse en la zona de aniquilación
//	founds = SearchEmissions(eInf, eSup, probMin,-1,0, a,a,m,m,false,false,false,false,"", false, false);
	founds = SearchEmissions(eInf, 505, probMin,-1,0, a,a,m,m,false,false,false,false,"", false, false);
	founds = founds + SearchEmissions(515, eSup, probMin,-1,0, a,a,m,m,false,false,false,false,"", false, false);
	
	int tam=founds.GetNumItemsFound();
	//printf("Hemos encontrado %d emisiones\n", tam);
	
//	printf("Iniciamos temp\n");
	int temp[tam+1];
	for(int i=0;i<tam+1;i++)	temp[i]=i;
	//printf("Iniciamos emisiones\n");
	for(int i=0;i<tam;i++)	emisiones[i]=0;
	float probMax=0;
	int posProbMax=0;
	int emiProbMax=0;
		
	//Ordenamos las probabilidades
	//printf("Ordenamos las probabilidades\n");		
	for(int i=0;i<tam;i++)	
		{
		posProbMax=0;
		probMax=0;
		for(int j=1;j<=tam;j++)
			{
			if(temp[j]!=-1)
				{
				int emi=founds.GetResult(j);
				if(GetProbability(emi)>probMax)
					{
					emiProbMax=emi;
					probMax=GetProbability(emi);
					posProbMax=j;
					}
				}
			}
		emisiones[i]=emiProbMax;
		temp[posProbMax]=-1;//Para no tenerlo en cuenta pa la próxima
		}
	printf("Las probabilidades ordenadas son (hay %d):\n", tam);
	for(int i=0;i<tam;i++)	printf("%.3f ", GetProbability(emisiones[i]));
	printf("\n");
	return;	
	}

//***************** DA EMISIONES CON MAYOR PROBABILIDAD ************************
/* 	Versión que en vez de tomar todas las emisiones gamma hasta la probabilidad mínima
	que se pase como parámetro, toma todas hasta un número que se imponga, dentro
	como siempre del intervalo [eInf, eSup]
	Dependiendo de los dos últimos parámetros se buscarán todas, sólo las que 
	estén en equilibrio	o las que no.
	*/
void cLaraBD::daEmisionesConMayorProbabilidad(int elemento,float eInf,float eSup, int *emisiones, int numMax, bool beq, bool eq)
	{
	cSearchLaraBD founds;
	int m=GetMassNumber(elemento);
	int a=GetAtomicNumber(elemento);
	//printf("DA EMISIÓN CON MAYOR PROBABILIDAD PARA %d,%d en [%f,%f]\n", a, m, eInf, eSup);
	
//	printf("pillamos nombre\n");
	char name[3];
	GetNameElement(elemento, name);
	
//	printf("buscamos emisiones\n");
	//NOTA: Probando, no contemplamos emisiones que puedan encontrarse en la zona de aniquilación
//	founds = SearchEmissions(eInf, eSup, -1,-1,0, a,a,m,m,false,false,false,false,"", false, false);
	founds = SearchEmissions(eInf, 505, -1,-1,0, a,a,m,m,false,false,false,false,"", beq, eq);
	founds = founds + SearchEmissions(515, eSup, -1,-1,0, a,a,m,m,false,false,false,false,"", beq, eq);
	
	int tam=founds.GetNumItemsFound();
//	printf("Hemos encontrado %d emisiones\n", tam);
//	if(tam<numMax)	numMax=tam;//Si es menor, sólo llegamos hasta las que tengamos
//	printf("Iniciamos temp\n");
	int temp[tam+1];
	for(int i=0;i<tam+1;i++)	temp[i]=i;
	//printf("Iniciamos emisiones\n");
	for(int i=0;i<numMax;i++)	emisiones[i]=0;
	float probMax=0;
	int posProbMax=0;
	int emiProbMax=0;
		
	//Ordenamos las probabilidades
	//printf("Ordenamos las probabilidades\n");		
	int cont=0;
	for(int i=0;i<tam;i++)	
		{
		posProbMax=0;
		probMax=0;
		for(int j=1;j<=tam;j++)
			{
			if(temp[j]!=-1)
				{
				int emi=founds.GetResult(j);
				if(GetProbability(emi)>probMax)
					{
					emiProbMax=emi;
					probMax=GetProbability(emi);
					posProbMax=j;
					}
				}
			}
//		printf("añadimos la emisión %d con valor %.2f\n", cont, GetProbability(emiProbMax)); 
		emisiones[cont++]=emiProbMax;
		if(cont>=numMax)	return;
		temp[posProbMax]=-1;//Para no tenerlo en cuenta pa la próxima
		}
	printf("Las probabilidades ordenadas son (hay %d):\n", tam);
	for(int i=0;i<tam;i++)	printf("%f ", GetProbability(emisiones[i]));
	printf("\n");
	return;	
	}
