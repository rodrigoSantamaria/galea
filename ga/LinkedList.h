/*					LINKED LIST
18-X Esta clase representa una lista enlazada y las distintas operaciones
que se realizan sobre ella: adición, lectura, borrado y modificación
Cada nodo de la lista enlazada será de tipo Node.
26-X Todos los métodos implementados.
*/
#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

//Para manejo de ficheros
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

#include "Node.h"

class LinkedList
	{
	private:
		Node *inicio;
		Node *ultimo;
		Node *actual;
	public:
		LinkedList();
		~LinkedList();
	
		bool emptyList();
		int  numElements();
	
		void addLast(void *nuevo); 
		void  add(void *nuevo, int pos);
		void addFirst(void *nuevo);
		
		void deleteNode(int pos);
		void deleteLast();
		void deleteFirst();
		void deleteAll();
	
		void *pop();
		void push(void *);
		
		bool isElementInList(void *);
		
		void copy(LinkedList *);
	
		void *getNode(int pos);
		void *getFirst();
		void *getLast();
		
		void setNode(int, void *);
		
		void print();
		//void switchNodes(int pos1, int pos2);
	};

#endif //LINKEDLIST_H
