//********************** ADD MEDIA **********************
/*	En función a la calibración que se haya hecho del fondo, a los picos
	identificados y a las posibles modificaciones del usuario, añadimos a la
	base de datos del fondo las cuentas de los picos apropiados.
	*/
void addMedia(char *nombre, int numEnergias, int numColumnas)
	{
	//NOTA: CREA LAS COLUMNAS NECESARIAS
  	//Recorremos la lista de picos analizados en el espectro y vamos añadiéndolos
	//a fbege.fbd en una nueva columna
	char buffer[1024];
	double ef;
	int ind;
	double area, incertidumbre;
	bool estaEnergia=false;
	bool ultimaEnergia=false;
		
	ifstream ifile(nombre);
//	ofstream ofile("temp.fbd", ios::trunc);
		
	if (!ifile.is_open() ) 	{ cout << "Error opening files"; exit(1); }

	string finfich("EOF");
	string vacia("");

	ifile.getline (buffer,1024);
	string cadena(buffer);

//	sprintf(buffer, "%s\taverage", cadena.c_str());
//	ofile << buffer << endl;
	
	double *energias, *medias, *incertidumbres;
	medias=new double[numEnergias];
	incertidumbres=new double[numEnergias];
	energias=new double[numEnergias];

	if(medias==NULL || incertidumbres ==NULL || energias==NULL)
		{
		printf("ERROR: No hay memoria para reservar double[%d]\n", numEnergias);
		return;
		}
		
//	calculaMediaBDFondo(nombre, numEnergias, numColumnas+1, energias, medias, incertidumbres);	//Tiene una columna más de las iniciales (la nueva)	

	for(int i=0;i<numEnergias;i++)
  		{
		//Recorremos el fichero hasta llegar a la línea correspondiente
		ifile.getline (buffer,1024);
		string cadena(buffer);
			
		if(cadena!=finfich && cadena!=vacia)
			{
			if((ind=cadena.find("\t"))!=-1)	string cadenaEnergia(cadena.substr(0,ind));
	
			sprintf(buffer, "%s\t%.0f(%.0f)\t", cadena.c_str(), medias[i]*(1000000/Datos.tiempo), incertidumbres[i]*(1000000/Datos.tiempo) );
			ofile << buffer << endl;
			}//if compare con linea final
			
		}//while(fich)
	ofile << "EOF" << endl;
	//Ahora sólo tenemos que volcar el contenido del fichero temporal en 
	//la base de datos
	//NOTA: Todo este rodeo de guardar primero en un archivo temporal y luego
	//		volcarlo se debe a la dificultad para añadir directamente en fbege.fbd
	//		sin borrar nada.
	ofile.close();
	ifile.close();
		
	//Pasamos la información del fichero temporal al fichero permanente
	ifstream ifile2("temp.fbd");
	ofstream ofile2(nombre, ios::trunc);
		
	if (!ifile2.is_open() || !ofile2.is_open() ) 	{ cout << "Error opening files"; exit(1); }
	while (! ifile2.eof() )
		{
		ifile2.getline (buffer,1024);
		ofile2 << buffer << endl;
		}
	ofile2.close();
	ifile2.close();
	remove("temp.fbd");
		
	printf("Fichero con la media\n");
	imprimirFichero(nombre);
	}	



//*************** LEER ELEMENTO ****************
/* Devuelve el valor double contenido en la posición i,j del fichero tabulado
	llamado nombre
*/	
double leerElemento(char *nombre, int fila, int columna)
	{
	char buffer[1024];
	fstream file(nombre, ios::in);
	int ind=0;
//	printf("LEER ELEMENTO %d,%d de %s\n", fila, columna, nombre);	
	for(int i=0;i<fila;i++)		file.getline (buffer,1024);
	
	//Cogemos la fila que queríamos
	file.getline (buffer,1024);
	string cadena(buffer);
	//cout << cadena << endl;
	//Quitamos las columnas anteriores
	for(int j=0;j<columna;j++)
		{
		if(cadena.length()<=0)	return -1;
		ind=cadena.find("\t");
		cadena=cadena.substr(ind+1, cadena.length()-ind-1);
		for(int k=0;k<10;k++)
			{
			if(cadena[0]=='\t')	cadena=cadena.substr(1, cadena.length()-1);
			else 				break;
			}
		}
	//Queda quitar las columnas posteriores
	ind=cadena.find("\t");
	cadena=cadena.substr(0,ind);
	//cout << cadena << endl;
	file.close();
	//printf("Devolvemos el valor %f\n", atof((char *)cadena.c_str()) );
	return (atof((char *)cadena.c_str()));
	}
	


void determinarNaturalesDeFondo(float energia, LinkedList *emisiones, char *fichero)
	{
	printf("DETERMINAR NATURALES ALREDEDOR DE %.2f con %s\n", energia, fichero);	
	if(energia<=0)	{printf("ERROR: Energía no válida (cero)\n");return;}
		
//--->	preparaFichero(fichero);
	
	int filas=numeroFilas(fichero);
	int j=0;
	char *nom=NULL;
	char *tex=NULL;
	double elAnt=0;//Para volver atrás si el siguiente está más lejos que éste
	bool anteriorEsCero=true;
	for(int i=0;i<filas;i++)
		{
//		printf("fila %d\n", i);
		double el=leerElemento(fichero, i, 0);			//lee la energía que es la primera columna
		double elOriginal=el;
		if(el != 0 && el>=energia) //Avanzamos en el fichero hasta llegar a la zona de energías interesante		
			{
			if(fabs(elAnt-energia)<fabs(el-energia) && i>0)	
				{
				if(anteriorEsCero)	i-=2;
				else				i--;//Si hay menos distancia al anterior que a este comenzamos por el anterior
				el=elAnt;
				}
			NodoEmision *ne=new NodoEmision();

			tex=leerElementoEntreParentesis(fichero, i, 1);//lee el elemento de la segunda columna
			int p=tomarNumero(tex);
			nom=tomarNombre(tex);	//hay que ver si esto nos sirve porque en la base de datos de fondo está al revés. Primero el número y luego el nombre

			//recuperamos la probabilidad de LARA porque no la tenemos en la BD fondo
			cSearchLaraBD founds = LaraBD.SearchEmissions(el-0.1, el+0.1,-1,-1,-1,-1,-1,-1,-1,false,false,false,false,nom, false, false);
		//NOTA: Añadimos una pequeña restriccion para que no nos identifique con picos muy lejanos!
			if(energia>el-0.5 && energia<el+0.5)			{
			if(founds.GetNumItemsFound())				{
				printf("vamos a ver si es añadible\n");
				ne->codElemento=founds.GetFirstResult();
				if(founds.GetNumItemsFound()>1)	ne->codElemento=founds.GetNextResult();
				ne->Energia=el;

				ne->probabilidad = GetProbability(ne->codElemento);
				//Sólo añadimos si está relativamente cerca (< 1keV)
				if(ne->Energia - energia < 1.0)		{printf("Añadimos, %f-%f\n", ne->Energia, energia);	emisiones->addLast(ne);}
				else								{printf("No añadimos, %f-%f\n", ne->Energia, energia);}
				}				}
			}

	}
	return;
	}

