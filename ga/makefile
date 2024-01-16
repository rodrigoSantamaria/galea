pro : Node.o LinkedList.o Prueba.o;
		c++ Node.o LinkedList.o Prueba.o -o pro

Node.o : Node.cc Node.h
		c++ -c Node.cc
LinkedList.o : LinkedList.cc LinkedList.h Node.h
		c++ -c LinkedList.cc
Prueba.o : Prueba.cc LinkedList.h
		c++ -c Prueba.cc
