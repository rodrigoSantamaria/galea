#include "LinkedList.h"
	
//************** LINKED LIST ***********
//	Constructor por defecto
LinkedList::LinkedList()
	{
	inicio=NULL;
	ultimo=NULL;
	actual=NULL;
	}

//********** ~LINKED LIST *************
LinkedList::~LinkedList()
	{
	deleteAll();
	}

//************ ADD LAST **************
void LinkedList::addLast(void *nuevo)
	{
	if(inicio==NULL)	
		{
		inicio=new Node(nuevo);
		inicio->setNext(NULL);
		ultimo=inicio;
		ultimo->setNext(NULL);
		}
	else
		{
		actual=new Node(nuevo);
		if(actual==NULL)	{printf("ERROR: No hay memoria para otro Node\n"); return;}
		actual->setNext(NULL);
		ultimo->setNext(actual);
		ultimo=actual;
		if(inicio->getNext()==NULL)	inicio->setNext(ultimo);
		}
	}

//************ ADD **************
void LinkedList::add(void *nuevo, int pos)
	{
	if(pos>numElements())	
		{
		printf("ERROR: LinkedList.add() BadPosition\n");
		return;
		}
	actual=NULL;
	for(int i=0;i<pos;i++)
		if(actual==NULL)	actual=inicio;
		else				actual=actual->getNext();
	
	Node *temp=new Node(nuevo);
	if(actual==NULL)	
		{
		temp->setNext(inicio);
		inicio=temp;
		}
	else				
		{
		temp->setNext(actual->getNext());
		actual->setNext(temp);
		}
	
	if(pos==numElements())	ultimo=temp;
	}
	
//************ PUSH **************
//Adición en forma de pila
void LinkedList::push(void *nuevo)
	{
	addLast(nuevo);
	}
	
//************ ADD FIRST **************
void LinkedList::addFirst(void *nuevo)
	{
	if(emptyList())	
		{
		inicio=new Node(nuevo);
		inicio->setNext(NULL);
		ultimo=inicio;
		ultimo->setNext(NULL);
		}
	else
		{
		actual=new Node(nuevo);
		actual->setNext(inicio);
		inicio=actual;
		}
	}

//************* EMPTY LIST ************
bool LinkedList::emptyList()
	{
	if(inicio==NULL)	return true;
	else				return false;
	}

//************ NUM ELEMENTS ************
int LinkedList::numElements()
	{
	int i=0;
	if(emptyList())	return i;
	actual=inicio;
	while(actual->getNext()!=NULL)	
		{
		i++;
		actual=actual->getNext();
		}
	return ++i;
	}
	
//*********** POP ***********
//Borrado en forma de pila, devolviendo el valor
void *LinkedList::pop()
	{
	if(emptyList())	return 0;
	else
		{
		if(numElements()==1)
			{
			void *valor=ultimo->getDato();
			delete ultimo;
			ultimo=NULL;
			actual=NULL;
			inicio=NULL;
			return valor;
			}
		else
			{
			actual=inicio;
			while(actual->getNext()!=ultimo)
				actual=actual->getNext();
			void *valor=ultimo->getDato();
			delete ultimo;
			ultimo=actual;
			ultimo->setNext(NULL);
			return valor;
			}
		}
	}

//*********** DELETE NODE ***********
// Considera la primera posición como cero y la
// última como n-1.
void LinkedList::deleteNode(int pos)
	{
	if(emptyList())			{printf("Lista vacía, salimos sin borrar\n");	return;}
	if(pos>=numElements()) 	{printf("pos fuera (%d vs %d), salimos sin borrar\n", pos, numElements()); return;}
	else
		{
		actual=inicio;
		for(int i=0;i<(pos-1);i++) //Nos quedamos en la posición anterior al elemento a borrar
			actual=actual->getNext();
		
		Node *aBorrar;
		if(pos==0)		
			{
			aBorrar=actual;
			inicio=actual->getNext();
			}
		else			
			{
			aBorrar=actual->getNext();
			actual->setNext(actual->getNext()->getNext());
			if(pos==numElements())	ultimo=actual;
			}
		aBorrar->setNext(NULL);
		delete aBorrar;	
		}
	}
	
//*********** DELETE LAST ***********
void LinkedList::deleteLast()
	{
	if(emptyList())	return;
	else
		{
		if(numElements()==1)
			{
			delete ultimo;
			ultimo=NULL;
			actual=NULL;
			inicio=NULL;
			}
		else
			{
			actual=inicio;
			while(actual->getNext()!=ultimo)
				actual=actual->getNext();
			delete ultimo;
			ultimo=actual;
			ultimo->setNext(NULL);
			}
		}
	}

//*********** DELETE FIRST ***********
void LinkedList::deleteFirst()
	{
	if(emptyList())	return;
	if(numElements()==1)
		{
		delete inicio;
		inicio=NULL;
		actual=NULL;
		ultimo=NULL;
		}
	else
		{
		actual=inicio;
		inicio=inicio->getNext();
		actual->setNext(NULL);
		delete actual;
		}
	}

//************** DELETE ALL *********
void LinkedList::deleteAll()
	{
//FELIPE COMENTADO PORQUE CASCA
	int i=0;
	printf("DELETE ALL con %d els\n", numElements());
	//printf("Tenemos %d elementos\n", numElements());
	while(!emptyList())		
		{
		//printf("Borrando el último elemento (%d)\n", (numElements()-1));
		deleteNode(numElements()-1);
		}

	}
	
//*************** COPY ****************
void LinkedList::copy(LinkedList *lista)
	{
	if(!emptyList())	deleteAll();
	int MAX=lista->numElements();
	for(int i=0;i<MAX;i++)
		{
		addFirst(lista->pop());//Sacamos el último y lo ponemos el primero<-esto copia y borra
		}
	}
		
//*************** GET NODE ************
void *LinkedList::getNode(int pos)
	{
	int i=0;
	actual=inicio;
	if(inicio==NULL)	printf("Inicio es 0!!!\n");
//	printf("el tamaño de la lista es %d\n", numElements());
	while(i<pos && actual->getNext()!=NULL)
		{
		actual=actual->getNext();
		i++;
		}
	if(actual==NULL)	{printf("ERROR: Nodo en posición %d no existe\n", pos); return NULL;}
	return actual->getDato();
	}

//*************** SET NODE ************
void LinkedList::setNode(int pos, void *datoNuevo)
	{
	int i=0;
	actual=inicio;
	while(i<pos && actual->getNext()!=NULL)
		{
		actual=actual->getNext();
		i++;
		}
	if(actual==NULL)	{printf("ERROR: Nodo en posición %d no existe\n", pos); return;}
	actual->setDato(datoNuevo);
	}

//************* GET FIRST ***********
void *LinkedList::getFirst()
	{
	if(inicio!=NULL)	return inicio->getDato();
	else		
		{
		printf("ERROR: Lista Vacía\n");
		return NULL;
		}
	}

//************* GET LAST ************
void *LinkedList::getLast()
	{
	return ultimo->getDato();
	}

//************* IS ELEMENT IN LIST ***********
bool LinkedList::isElementInList(void *element)
	{
//	printf("IS ELEMENT %d in list\n", (int)element);
	if(inicio!=NULL)
		{
		actual=inicio;
		while(actual->getNext()!=NULL)
			{
			void *value=actual->getDato();
			if(value==element)		return true;
			actual=actual->getNext();
			}
		//Comprobamos el último
		void *value=actual->getDato();
		if(value==element)			return true;
		}
	return false;
	}
	
//********** PRINT ***************
// Esta función sólo funciona si se trata de una lista de
// cadenas.
void LinkedList::print()
	{
	if(inicio!=NULL)
		{
		actual=inicio;
		while(actual->getNext()!=NULL)
			{
			cout << (char *)(actual->getDato()) << endl;
			actual=actual->getNext();
			}
		cout << (char *)(actual->getDato()) << endl;
		}
	else cout << "Lista vacía" << endl;
	}
	
//**************** SWITCH NODES *****************
/*void LinkedList::switchNodes(int pos1, int pos2)
	{
	if(pos1>numElements())
		{
		cout << "ERROR: " << pos1 << " mayor que el tamaño de la lista (" << numElements() << ")" << endl;
		return;
		}			
	if(pos2>numElements())
		{
		cout << "ERROR: " << pos2 << " mayor que el tamaño de la lista (" << numElements() << ")" << endl;
		return;
		}			
   if(pos1==pos2)	return;
	
	actual=inicio;
	void *t1, *t2;
	
	if(pos1>pos2)	
		{
		int temp=pos2;
		pos2=pos1;
		pos1=temp;
		}
		
	t1=getNode(pos1);
	t2=getNode(pos2);
	deleteNode(pos1);
	deleteNode(pos2-1);//Hay una posición menos porque hemos borrado el otro
	add(t1, pos2-1);
	add(t2, pos1);
	}
*/
