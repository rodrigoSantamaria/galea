#include "InitFile.h"
#include <stdio.h>
#include <string.h>
#include "strings1.h"

/*****************************************************
 ***																***
 ***						INTERNAL LISTS						***
 ***																***
 *****************************************************/

	// Internal list make his own internal copy. It is program
	// responsibility to free results
	
void DelIniNode(IniNodeList *node) {
	delete [] node->VarName;
	delete [] node->VarValue;
	delete node;
}

IniNodeList *CreateNewIniNode(const char *name, const char *value) {
	// Create new variable
	IniNodeList *newNode = new IniNodeList;
	if (newNode == NULL) return NULL; // memory failure, losted information
	newNode->VarName = NULL; newNode->VarValue = NULL;
	if (((newNode->VarName = new char [strlen(name)+1]) == NULL)||
		((newNode->VarValue = new char [strlen(value)+1]) == NULL)) {
		// memory failure, losted information
		DelIniNode(newNode);
		return NULL;
	}
	newNode->Next = NULL;
	strcpy(newNode->VarName, name);
	strcpy(newNode->VarValue, value);
	return newNode;
}

void AddIniElement(tListIniElements& list, const char *name, const char *value) {
	if (strlen(name) == 0) return; // no admit empty strings for variable name

	IniNodeList **guide;
	
	guide = &(list.First);
	while ((*guide) != NULL) {
		// check for sort or existent variable
		int comp = strcmp(name, (*guide)->VarName);
		if (comp == 0) {
			// Existing Variable, only change value
			delete [] (*guide)->VarValue;
			delete [] name;
			if (((*guide)->VarValue = new char [strlen(value)+1]) == NULL)
				return; // memory failure, losted information
			strcpy((*guide)->VarValue, value);
			return;
		} else if (comp < 0) {
			// Alphabetic orden. Create new variable
			IniNodeList *newNode = CreateNewIniNode(name, value);
			delete [] name;
			if (newNode == NULL) return; // memory failure, losted information
			newNode->Next = *guide;
			*guide = newNode;
			list.numElements++;
			return;
		}
		// pass to next element
		guide = &((*guide)->Next);
	}
	// put element in the last place of the list
	IniNodeList *newNode = CreateNewIniNode(name, value);
	delete [] name;
	if (newNode == NULL) return; // memory failure, losted information
	newNode->Next = NULL;
	*guide = newNode;
	list.numElements++;
}

void DeleteIniList(tListIniElements &list) {
/*	IniNodeList **guide;
	
	guide = &(list.First);
	while ((*guide) != NULL) {
		IniNodeList *DelNode = *guide;
		guide = &((*guide)->Next);
		DelIniNode(DelNode);
		list.numElements--;
	}
	list.First = NULL;*/
	IniNodeList *guide;
	
	guide = list.First;
	while (guide != NULL) 
		{
		IniNodeList *DelNode = guide;
		guide = guide->Next;
		DelIniNode(DelNode);
		list.numElements--;
		}
	list.First = NULL;
}

//******************* GET ELEMENT VALUE ********************
/* Busca "name" en su lista de elementos y si lo encuentra devuelve
su valor. Si no devuelve NULL.
*/
char *GetElementValue(tListIniElements &list, const char *name) 
	{
	//printf("GET ELEMENT VALUE\n");
	IniNodeList **guide;
	//printf("Buscando %s\n", name);
	guide = &(list.First);
	while ((*guide) != NULL) 
		{
		// check for sort or existent variable
		int comp = strcmp(name, (*guide)->VarName);
		//printf("Un valor de la lista es %s\n", (*guide)->VarName);
		if (comp == 0) 
			{
			//printf("La variable existe!\n");
			// Existing Variable, return a copy of its value
			delete [] name;
			char *copy = new char [strlen((*guide)->VarValue)+1];
			if (copy == NULL) return NULL; // memory failure, losted information
			strcpy(copy, (*guide)->VarValue);
			return copy;
			}
		else if (comp < 0) 
			{
			// No existing variable, return NULL string
			delete [] name;
			return NULL;
			}
		// pass to next element
		guide = &((*guide)->Next);
		}
	// No existing variable, return NULL string
	delete [] name;
	return NULL;
	}

/*****************************************************
 ***																***
 ***						INIT FILE							***
 ***																***
 *****************************************************/
/* ***************************
	**     General Methods   **
	***************************/
	// Constructors
cInitFile::cInitFile() {
	Changes = false;	// init object, there are no changes
	List.numElements = 0; List.First = NULL;
}

	// Destructor
cInitFile::~cInitFile() {
	// Detour for give private variables
	tListIniElements *list = &List;
	DeleteIniList(*list);
}

/* ******************************
	**     Management Methods   **
	******************************/

char *GetVarName(const char *string) {
	// search '='
	char *guide = strchr(string, '=');
	// Move back while there are spaces
	guide--;
	while ((*guide == ' ')&&(guide != string))
		guide--;
		
	if ((*guide == ' ')||(*guide == '=')) return NULL; // there isn't variable name
	
	// return name
	int size = (int) (guide - string) + 1;
	char *copy = new char [size+1];
	if (copy == NULL) return NULL; // memory failure
	strncpy(copy, string, size);
	copy[size] = '\0';
	return copy;
}

char *GetVarValue(const char *string) {
	// search '='
	char *guide = strchr(string, '=');
	guide++;
	while (*guide == ' ') guide++; // There is a final character at least
	
	// final character of this string is before \n or \0 character
	char *final = guide;
	while ((*final != '\0')&&(*final != '\n')) final++;

	// return value
	int size = (int) (final - guide);
	char *copy = new char [size + 1];
	if (copy == NULL) return NULL; // memory failure
	strncpy(copy, guide, size);
	copy[size] = '\0';
	return copy;
}

bool cInitFile::Load(const char *fileName) {
	// Open init file
	FILE *iniFile;
	iniFile = fopen(fileName, "r");
	if (iniFile == NULL) return false; // No exist preferences file
	
	char buffer[201];
	while (!feof(iniFile)) {
		if ((fgets(buffer, 200, iniFile) != NULL)&&
			 (strchr(buffer, '=') != NULL)) {
			// in this line there is a variable
			// let's allow parser works
			char *nameVar = GetVarName(buffer);
			char *valueVar = GetVarValue(buffer);
			PutIniValue(nameVar, valueVar);
			delete [] nameVar; delete [] valueVar;
		}
	}
	
	
	fclose(iniFile);
	return true;
}

bool cInitFile::Save(const char *fileName) {
	// Open preferences file
	FILE *iniFile;
	iniFile = fopen(fileName, "w");
	if (iniFile == NULL) return false; // File not reacheable
	
	// Write all variables and values
	IniNodeList **guide;
	
	guide = &(List.First);
	while ((*guide) != NULL) {
		fprintf(iniFile, "%s = %s\n", (*guide)->VarName, (*guide)->VarValue);
		guide = &((*guide)->Next);
	}
	
	fclose(iniFile);
	Changes = false;
	return true;
}

//***************** GET INI VALUE **************
/* Devuelve el valor del elemento de nombre VarName
*/
char *cInitFile::GetIniValue(const char *VarName) 
	{
	// Detour for give private variables
	tListIniElements *list = &List;
	char *Name = CopyInUpper(VarName);
	printf("Buscamos el valor de la variable %s\n", Name);
	if (Name == NULL) 
		{printf("Nombre nulo, devolvemos NULL como valor\n");
			return NULL;
		}
	return GetElementValue(*list, Name);
	}

void cInitFile::SetIniValue(const char *VarName, const char *VarValue) {
	if ((VarName == NULL)||(strlen(VarName) == 0)){
		printf("mal rollete\n");
		return;
	}
	// Detour for give private variables
	tListIniElements *list = &List;
	char *Name = CopyInUpper(VarName);
	if (Name == NULL) {
		printf("mal rollete 2\n");
		return;
	}
	AddIniElement(*list, Name, VarValue);
	Changes = true;
}

void cInitFile::PutIniValue(const char *VarName, const char *VarValue) {
	if ((VarName == NULL)||(strlen(VarName) == 0)) return;
	// Detour for give private variables
	tListIniElements *list = &List;
	char *Name = CopyInUpper(VarName);
	if (Name == NULL) return;
	AddIniElement(*list, Name, VarValue);
}
