/*					NODE
18-X Clase que implementa un nodo de una lista enlazada
Contendrá un puntero a la información y otro puntero
al siguiente elemento de la lista
*/
#ifndef NODE_H
#define NODE_H

#include <stdio.h>

class Node
	{
	private:
		Node *next;
		void *dato;
	public:
		Node(void *);
		~Node();
	
		Node *getNext();
		void setNext(Node *);
		void *getDato();
		void setDato(void *);
	};

#endif //NODE_H
