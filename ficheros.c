#include <stdio.h>
#include "ficheros.h"
#include <string>

size_t fileWriteBool(bool valor, FILE *stream) 
	{
	char flag = (valor) ? -1 : 0;
	size_t temp = fwrite(&flag, sizeof(flag), 1, stream);
	return temp;
	}

size_t fileReadBool(bool& valor, FILE *stream) 
	{
	char flag;
	size_t temp = fread(&flag, sizeof(flag), 1, stream);
	valor = (flag != 0) ? true : false;
	return temp;
	}


//********************* DA NOMBRE FILE ***********************
/*	Construye el nombre de un fichero con su ruta completa a partir del
nombre del fichero y del directorio.
*/
char *DaNombreFile(char *nombreCompleto, char *nombreDirectorio) 
	{
	printf("da nombre file\n");
	char *nombre = nombreCompleto + strlen(nombreDirectorio);
	char *nuevo = new char[strlen(nombre) + 1];
	strcpy(nuevo, nombre);
	printf("devolvemos nuevo: %s\n", nuevo);
	return nuevo;
	}

//********************* DA FICHERO ****************************************
/*	A partir de un nombre de fichero con directorio y todo nos da el nombre
	sólo del fichero, quitando el resto de la ruta
	*/
char *daFichero(char *nombreCompleto)
	{
//	printf("DA FICHERO\n");
	string cadena(nombreCompleto);
//	cout << cadena << endl;
	int pos=0;
	while( (pos=cadena.find("/")) != string::npos)	
		{
		cadena=cadena.substr(pos+1);
		cout << pos << endl;
		//cout << cadena << endl;
		}
//	cout << cadena << endl;
//	printf("Terminamos\n");
	return (char *)cadena.c_str();
	}

//********************* DA FICHERO SIN EXTENSION *******************************
/*	A partir de un nombre de fichero con directorio y todo nos da el nombre
	sólo del fichero, quitando el resto de la ruta. Quita además la extensión
	*/
char *daFicheroSinExtension(char *nombreCompleto)
	{
	string cadena(nombreCompleto);
	int pos=0;
	while( (pos=cadena.find("/")) != string::npos)	
		{
		cadena=cadena.substr(pos+1);
		cout << pos << endl;
		//cout << cadena << endl;
		}
	pos=cadena.find(".");
	if(pos!=string::npos)	cadena=cadena.substr(0,pos);
	cout << "El nombre del fichero sin extensión es " << cadena << endl;
	return (char *)cadena.c_str();
	}
	
//********************* QUITAR COLUMNA ***************************************
/*	Dentro de un fichero encolumnado mediante tabuladores, elimina la columna
	que se encuentra en el lugar especificado
	*/
/*void quitarColumna(char *fichero, int pos)
	{
	char buffer[1024];
	int nf=0;
	fstream file(nombre, ios::in);
	string finfich("EOF");
	ofstream ofile("temp.fbd", ios::trunc);
	numColumnas=numeroColumnas(fichero);
		
	if (!file.is_open()) 	
		{ 
		cout << "Error opening file" << nombre << endl; 
		return -1;
		}

	if(leerElemento(fichero, 0, pos)==-1)
		{
		cout << "Column " << pos << " does not exist" << endl; 
		return 0;
		}
		
	int cont=0;
	while(!file.eof())	
		{
		string cadena("");
		for(int i=0;i<numColumnas;i++)
			{
			string add(leerElementoCadena(fichero, cont, pos));
			}				
		cont++;
		ofile
		}
	file.close();

	ifstream ifile2("temp.fbd");
	ofstream ofile2(nombre, ios::trunc);
	
	//Pasamos del fichero temporal al fichero final
	if (!ifile2.is_open() || !ofile2.is_open() ) 	{ cout << "Error opening files"; exit(1); }
	while (! ifile2.eof() )
		{
		ifile2.getline (buffer,1024);
		ofile2 << buffer << "\n";
		}
	ofile2.close();
	ifile2.close();
	remove("temp.fbd");
		
	ifile.close();
	ofile.close();

	return nf;
	}
*/

/****************** QUITAR ÚLTIMA COLUMNA *******************
/*	Elimina la última columna de un fichero
*/
int quitarUltimaColumna(char *nombre)
	{
	printf("QUITAR ÚLTIMA COLUMNA ++++**********************\n");
	char buffer[1024];
	int nf=0;
	ifstream ifile(nombre);
	string finfich("EOF");
	ofstream ofile("temp.fbd", ios::trunc);
	int numColumnas=numeroColumnas(nombre);
	
	printf("Tenemos %d columnas\n", numColumnas);
	if (!ifile.is_open()) 	
		{ 
		cout << "Error opening file" << nombre << endl; 
		return -1;
		}

	if(numColumnas<=0)
		{
		cout << "File has no columns" << endl; 
		return 0;
		}
		
	while(!ifile.eof())	
		{
		ifile.getline (buffer,1024);
		string cadena(buffer);

		int ind=0;
//		cout << "Revisando cadena |" << cadena << "|" << endl;
		//Buscamos posibles tabuladores después de la última columna
		ind=cadena.rfind("\t");
		if((ind+1)>=cadena.length())//Si los tabuladores que hay están al final
			{					//Los quitamos
			for(int k=ind;k>(ind-10);k--)
				{
				if(cadena[k]=='\t')	cadena=cadena.substr(0, k);
				else 				break;
				}
			}
		//Buscamos ahora el último tabulador bueno y borramos a partir de ahí
		ind=cadena.rfind("\t");
		if(ind!=string::npos)	cadena=cadena.substr(0,ind);
		//Quitamos algún último tabulador residual
		ind=cadena.rfind("\t");
		while( ind!=string::npos && ind>=cadena.length()-1)
			{
			cadena=cadena.substr(0,ind);
			ind=cadena.rfind("\t");
			}
		cout << "Cadena final queda |" << cadena << "|" << endl;
		ofile << cadena << endl;
		}
	ifile.close();
	ofile.close();
		
	printf("Abriendo ficheros\n");
	ifstream ifile2("temp.fbd");
	ofstream ofile2(nombre, ios::trunc);
	
	cout << "Abrimos fichero temp.fbd---------------------" << endl;
	//Pasamos del fichero temporal al fichero final
	if (!ifile2.is_open() || !ofile2.is_open() ) 	{ cout << "Error opening files"; exit(1); }
	while (! ifile2.eof() )
		{
		ifile2.getline (buffer,1024);
		printf("%s\n", buffer);
		string cadena(buffer);
		if(cadena==finfich)	{ofile2 << "EOF" << endl; break;}
		else				ofile2 << buffer << endl;
		}
	ofile2.close();
	ifile2.close();
	remove("temp.fbd");
		
	ifile.close();
	ofile.close();

	return nf;
	}

//******************** IMPRIME FICHERO ************************
/*	Escribe por pantalla la información de un fichero
	*/
int imprimirFichero(char *nombre)
	{
	char buffer[1024];
	int nf=0;
	ifstream ifile(nombre);
	string finfich("EOF");
	int numColumnas=numeroColumnas(nombre);
		
	if (!ifile.is_open()) 	
		{ 
		cout << "Error opening file" << nombre << endl; 
		return -1;
		}

	while(!ifile.eof())	
		{
		ifile.getline (buffer,1024);
		string cadena(buffer);
		cout << cadena << endl;
		}
	ifile.close();
	}
	
//******************** 	NUMERO COLUMNAS ***********************
/* Devuelve el número de columnas de un fichero tabulado menos 2
	*/
int numeroColumnas(char *nombre)
	{
	char buffer[1024];
	int nc=0;
	bool primerTab=true;
	fstream file(nombre, ios::in);
	
	if (!file.is_open())	
		{
		//sprintf(mensajeError, "Error opening file %s", nombre);
		//Aviso(main_w, mensajeError);
		cout << "Error opening file" << nombre; 
		return -1;
		}
	file.getline (buffer,1024);
	string cadena(buffer);
	
	for(int i=0;i<cadena.length();i++)
		{
		char c=cadena.at(i);
		if(c=='\t' && primerTab)
			{
			nc++;
			primerTab=false;
			}
		else 
			{
			if(c!='\t' && !primerTab)	primerTab=true;
			}
		}
	
	file.close();
	return (nc-2);//No se cuentan la columna de las energías ni la de las reacciones		
	}
	
//*************** NUMERO FILAS *****************
//Devuelve el número de filas del fichero nombre
//(hasta la línea EOF).
int numeroFilas(char *nombre)
	{
	char buffer[1024];
	int nf=0;
	fstream file(nombre, ios::in);
	string finfich("EOF");

	if (!file.is_open()) 	
		{ 
//		sprintf(mensajeError, "Error opening file %s", nombre);
//		Aviso(main_w, mensajeError);
		cout << "Error opening file" << nombre; 
		return -1;
		}
	while(!file.eof())	
		{
		file.getline (buffer,1024);
		string cadena(buffer);
		if(cadena==finfich)	break;
		nf++;
		}
	file.close();
	return nf;
	}
	
//********************* DA DIRECTORIO ****************************************
/*	A partir de un nombre de fichero con directorio y todo nos da el nombre
	sólo del directorio, quitando el resto de la ruta
	*/
char *daDirectorio(char *nombreCompleto)
	{
	string cadena(nombreCompleto);
	int pos=0;
	while( (pos=cadena.find("/"))	!=0)	;
	cadena=cadena.substr(0, pos);
	return (char *)cadena.c_str();
	}
	
//********************** DA EXTENSION FILE *********************
char *DaExtensionFile(char *nombreCompleto, char *nombreDirectorio) 
	{
	// coge unicamente el nombre
	char *nombre = nombreCompleto + strlen(nombreDirectorio);

	// partiendo desde atras busca un punto en el nombre
	bool bSeguir = true;
	char *puntero = nombre + strlen(nombre) - 1;	
	while ((puntero >= nombre)&&(bSeguir)) 
		{
		if (puntero[0] == '.') bSeguir = false;
		else puntero--;
		}
	
	// si encontro un punto lo coge como la extension y lo pasa
	if (!bSeguir) 
		{
		char *nuevo = new char[strlen(puntero) + 1];
		strcpy(nuevo, puntero);
		return nuevo;
		}
	else
		return NULL;
	}


#ifdef platform_big_endian
	// Arquitectura Big Endian

void XCHG_BYTE(void *ptr, size_t size) {
	unsigned char *principio = (unsigned char*) ptr;
	unsigned char *final = (unsigned char*) ptr + size - 1;
   for (int i = 1; i <= size/2; i++) {
	   unsigned char swap = *principio;
		*(principio++) = *final;
		*(final--) = swap;
	}
}


//*********************** FILE READ ********************
/* size: Tamaño de cada dato a leer.
	nmemb: número de datos a leer.
	stream: flujo de datos del que se leen los datos
	ptr: puntero donde se almacenan los datos.
*/
size_t fileRead(void *ptr, size_t size, size_t nmemb, FILE *stream) 
	{
	// emula la lectura de un fichero en notacion little endian
	// para ello lee cada byte y lo coloca en la celda de memoria
	// correspondiente:
	// ***** ATENCION **** sin deteccion de errores
	unsigned char *puntero = (unsigned char*) ptr;
	for (int i = 1; i <= nmemb; i++) 
		{
		// lee cada bloque
   		fread(puntero, size, 1, stream);
		// y recoloca los bytes (todos los tamagnos disponibles en size
		// seran siempre una potencia de 2)
		if (size > 1) XCHG_BYTE(puntero, size);
		puntero += size;
		}
	return nmemb;
	}

//*********************** FILE WRITE **********************
size_t fileWrite(const void *ptr, size_t size, size_t nmemb, FILE *stream) 
	{
	// emula la escritura de un fichero en notacion little endian
	// para ello escribe cada byte en la posicion correspondiente:
	// ***** ATENCION **** sin deteccion de errores
	unsigned char *puntero = (unsigned char*) ptr;
	for (int i = 1; i <= nmemb; i++) 
		{
		// escribe cada byte de cada bloque
		unsigned char *final = puntero + size - 1;
		while (final >= puntero)
			fwrite((final--), 1, 1, stream);
		puntero += size;
		}
	return nmemb;
	}

#else
	// Arquitectura Little Endian (supuesta)

void XCHG_BYTE(void *ptr, size_t size) {
}

size_t fileRead(void *ptr, size_t size, size_t nmemb, FILE *stream) {
   return fread(ptr, size, nmemb, stream);
}

size_t fileWrite(const void *ptr, size_t size, size_t nmemb, FILE *stream) {
   return fwrite(ptr, size, nmemb, stream);
}
#endif
