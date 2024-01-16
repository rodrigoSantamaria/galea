#include "Node.h"
#include "Individuo.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

//Para manejo de ficheros
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

//************* NODE ****************
/* Constructor por defecto.
	Deja a NULL tanto la información como el elemento siguiente
	*/
/*Node::Node()
	{
	dato = NULL;
	next = NULL;
	}*/
	
//************ NODE *******************
/*	Constructor copia. Apunta a la información contenida en datoNuevo
	El elemento siguiente seguirá siendo NULL
	*/
Node::Node(void *datoNuevo)
	{
	dato = datoNuevo;
	next = NULL;
	}
	
//************** ~NODE *******************
/*  Destructor por defecto. Borra el enlace al elemento siguiente
	*/
Node::~Node()
	{
	next=NULL;
	dato=NULL;
	//delete [] (void *)(dato);//--->Desde aquí el dato no se puede borrar, hay que borrarlo desde
		//fuera, desde donde sepan qué tipo tiene.
	}

//************* GET NEXT ***********
//	Devuelve el enlace al nodo siguiente
Node *Node::getNext()
	{
	return next;	
	}

//************* SET NEXT ***********
//	Establece como nodo siguiente pNuevo
void Node::setNext(Node *pNuevo)
	{
	next= pNuevo;	
	}

//************* GET DATO ***********
//	Devuelve el puntero al dato almacenado
void *Node::getDato()
	{
	return dato;
	}

//************* SET DATO ***********
//	Devuelve el puntero al dato almacenado
void Node::setDato(void *datoNuevo)
	{
	//if(dato!=NULL)	delete dato;
	//if(dato!=NULL)	free(dato);
	dato=datoNuevo;
	}
