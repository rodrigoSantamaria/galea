#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>

char *CopyString(const char *string) {
	// make a copy
	//printf("COPIANDO CADENA\n");
	char *name = new char[strlen(string)+1];
	if (name == NULL) return NULL;
	//printf("reservada memoria\n");
	strcpy(name, string);
	//printf("copiada la cadena\n");
	return name;	
}

void toUp(char *string) {
	char *guide;
	
	guide = string;
	while (*guide != 0) {
		*guide = toupper(*guide);
		guide++;
	}
}

char *CopyInUpper(const char *string) {
	// make a copy in uppercase
	char *name = CopyString(string);
	if (name == NULL) return NULL;
	toUp(name);
	return name;	
}
