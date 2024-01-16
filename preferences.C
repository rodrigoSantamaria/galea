#include "preferences.h"
#include <stdio.h>
#include "strings1.h"
#include <Xm/XmosP.h>

const char *PrefFileName = ".Galea";
const char *defaultDBName = "lara.DB";


/******************************************************
 ***												***
 ***						PREFERENCES				***
 ***												***
 *****************************************************/
/* ***************************
	**     General Methods   **
	***************************/
	// Constructor
cPreferences::cPreferences() 
	{
	HomeDirName = (char *) _XmOSGetHomeDirName();
	FileName = new char[strlen(HomeDirName)+strlen(PrefFileName)+2];
	strcpy(FileName, HomeDirName);
	strcat(FileName, "/");
	strcat(FileName, PrefFileName);
	printf("%s\n", FileName);
	
	InstallDir = NULL;
	LaraDBDir = NULL;
//	LaraDBFile = NULL;
	AnalgamSpecDir = NULL;
	SpecDir = NULL;
	CalibDir = NULL;
	BackBDDir = NULL;
	}

	// Destructor
cPreferences::~cPreferences() 
	{
	if(FileName!=NULL)		delete [] FileName;
	if(HomeDirName!=NULL)	XtFree(HomeDirName);
	// Directories
	if(InstallDir!=NULL)	delete [] InstallDir;
	if(LaraDBDir!=NULL)		delete [] LaraDBDir;
//	if(LaraDBFile!=NULL)		delete [] LaraDBFile;
	if(AnalgamSpecDir!=NULL)	delete [] AnalgamSpecDir;
	if(SpecDir!=NULL)		delete [] SpecDir;
	if(CalibDir!=NULL)		delete [] CalibDir;
	if(BackBDDir!=NULL)		delete [] BackBDDir;
	// Colors
	}	

/* ******************************
	**     Management Methods   **
	******************************/

//**************** GENERAL DIR *********************
/*	Si relative no tiene barra al principio, se considera
una dirección perteneciente al directorio home, así que se le
añade el path del home.
*/
char *cPreferences::GeneralDir(char *relative) 
	{
	printf("GENERALDIR\n");
	if (relative[0] != '/') 
		{
		//	char newString[strlen(relative) + strlen(HomeDirName) + 2];
		printf("Creamos cadena\n");
		char *newString=new char[strlen(relative) + strlen(HomeDirName) + 2];
		printf("Copiamos direcotrio raiz\n");
		strcpy(newString, HomeDirName);
		printf("Creamos una nueva cadena\n");
		strcat(newString, "/");
		printf("Añadimos a la nueva cadena\n");
		strcat(newString, relative);
		printf("Borramos cadena\n");
		delete [] relative;
		printf("Igualamos cadenas\n");
		relative = newString;
		}
	printf("Devolvemos %s\n", relative);
	return relative;
	}


void cPreferences::Load(Display *dpy, Widget w) 
	{
	printf("El fichero de preferencias es %s\n", FileName);
	bool IniExist = IniFile.Load(FileName);
	if (!IniExist)
		printf("Preferences Error: preferences file not reachable\n");
	
	// Fill preferences with value in ini file. Use default value
	// if not exist value in ini file.

	// Fill directories entries
	InstallDir = IniFile.GetIniValue("Dir_Install");
	if (InstallDir == NULL) 
		{
		InstallDir = CopyString("");
		IniFile.SetIniValue("Dir_Install", InstallDir);
		}
	LaraDBDir = IniFile.GetIniValue("Dir_LaraDB");
	if (LaraDBDir == NULL)			LaraDBDir = CopyString("");
	CentroidDBDir = IniFile.GetIniValue("Dir_CentroidDB");
	if (CentroidDBDir == NULL)			CentroidDBDir = CopyString("");
	AnalgamSpecDir = IniFile.GetIniValue("Dir_AnalSpec");
	if (AnalgamSpecDir == NULL)		AnalgamSpecDir = CopyString("");
	SpecDir = IniFile.GetIniValue("Dir_Spectrum");
	if (SpecDir == NULL)			SpecDir = CopyString("");
	CalibDir = IniFile.GetIniValue("Dir_Calibration");
	if (CalibDir == NULL)			CalibDir = CopyString("");
	BackBDDir = IniFile.GetIniValue("Dir_Background");
	if (BackBDDir == NULL)			BackBDDir = CopyString("");
	EfficiencyDir = IniFile.GetIniValue("Dir_Efficiency");
	if (EfficiencyDir == NULL)		EfficiencyDir = CopyString("");

	// Fill Color entries
	Colormap  cmap = DefaultColormapOfScreen(XtScreen(w));
	char *ColorString;
	ColorString = IniFile.GetIniValue("Color_GraphicFG");
	if (!XParseColor(dpy, cmap, ColorString, &cGrafFg)) 
		{
		XParseColor(dpy, cmap, "rgb:FF/FF/FF", &cGrafFg); //white
		XAllocColor(dpy, cmap, &cGrafFg);
		}
	else XAllocColor(dpy, cmap, &cGrafFg);
	delete [] ColorString;
	ColorString = IniFile.GetIniValue("Color_GraphicBG");
	if (!XParseColor(dpy, cmap, ColorString, &cGrafBg)) 
		{
		printf("No tenemos color para el grÃ¡fico, ponemos negro\n");
		XParseColor(dpy, cmap, "rgb:00/00/00", &cGrafBg); //black
		XAllocColor(dpy, cmap, &cGrafBg);
		} 
	else 
		{
		printf("Tenemos color para el grÃ¡fico, lo ponemos\n");
		XAllocColor(dpy, cmap, &cGrafBg);
		}
	delete [] ColorString;
	ColorString = IniFile.GetIniValue("Color_ScaleFG");
	if (!XParseColor(dpy, cmap, ColorString, &cZoomFg)) 
		{
		XParseColor(dpy, cmap, "rgb:00/00/00", &cZoomFg); //black
		XAllocColor(dpy, cmap, &cZoomFg);
		}
	else XAllocColor(dpy, cmap, &cZoomFg);
	delete [] ColorString;
	ColorString = IniFile.GetIniValue("Color_ScaleBG");
	if (!XParseColor(dpy, cmap, ColorString, &cZoomBg)) 
		{
		XParseColor(dpy, cmap, "rgb:FF/FF/FF", &cZoomBg); //white
		XAllocColor(dpy, cmap, &cZoomBg);
		} 
	else XAllocColor(dpy, cmap, &cZoomBg);
	delete [] ColorString;
	ColorString = IniFile.GetIniValue("Color_Ch_Peaks");
	if (!XParseColor(dpy, cmap, ColorString, &cChPeaks)) 
		{
		XParseColor(dpy, cmap, "rgb:00/FF/00", &cChPeaks); //green
		XAllocColor(dpy, cmap, &cChPeaks);
		}
	else XAllocColor(dpy, cmap, &cChPeaks);
	delete [] ColorString;
	ColorString = IniFile.GetIniValue("Color_Ch_Continuum");
	if (!XParseColor(dpy, cmap, ColorString, &cChContinuum)) 
		{
		XParseColor(dpy, cmap, "rgb:FF/00/FF", &cChContinuum); //violet
		XAllocColor(dpy, cmap, &cChContinuum);
		}
	else XAllocColor(dpy, cmap, &cChContinuum);
	delete [] ColorString;
	ColorString = IniFile.GetIniValue("Color_Adjust_Continuum");
	if (!XParseColor(dpy, cmap, ColorString, &cAdjustCont)) 
		{
		XParseColor(dpy, cmap, "rgb:00/FF/FF", &cAdjustCont); //cyan
		XAllocColor(dpy, cmap, &cAdjustCont);
		}
	else XAllocColor(dpy, cmap, &cAdjustCont);
	delete [] ColorString;
	ColorString = IniFile.GetIniValue("Color_Adjust_Peak");
	if (!XParseColor(dpy, cmap, ColorString, &cAdjustPeak)) 
		{
		XParseColor(dpy, cmap, "rgb:FF/00/00", &cAdjustPeak); //red
		XAllocColor(dpy, cmap, &cAdjustPeak);
		}
	else XAllocColor(dpy, cmap, &cAdjustPeak);


	delete [] ColorString;
	ColorString = IniFile.GetIniValue("Color_Adjust_Selected_Peak");
	if (!XParseColor(dpy, cmap, ColorString, &cAdjustSelectedPeak)) 
		{
		XParseColor(dpy, cmap, "rgb:FF/FF/00", &cAdjustSelectedPeak); //yellow
		XAllocColor(dpy, cmap, &cAdjustSelectedPeak);
		}
	else XAllocColor(dpy, cmap, &cAdjustSelectedPeak);

	delete [] ColorString;
	ColorString = IniFile.GetIniValue("Color_Partial_Peak");
	if (!XParseColor(dpy, cmap, ColorString, &cGeneticPeak)) 
		{
		XParseColor(dpy, cmap, "rgb:00/FF/55", &cGeneticPeak); //Verde un poco oscuro
		XAllocColor(dpy, cmap, &cGeneticPeak);
		}
	else XAllocColor(dpy, cmap, &cGeneticPeak);

	delete [] ColorString;
	ColorString = IniFile.GetIniValue("Color_Background");
	if (!XParseColor(dpy, cmap, ColorString, &cBackground)) 
		{
		XParseColor(dpy, cmap, "rgb:00/FF/00", &cBackground); //Azul marino
		XAllocColor(dpy, cmap, &cBackground);
		}
	else XAllocColor(dpy, cmap, &cBackground);


	delete [] ColorString;
	ColorString = IniFile.GetIniValue("Color_CursorUp");
	if (!XParseColor(dpy, cmap, ColorString, &cCursorUp)) 
		{
		XParseColor(dpy, cmap, "rgb:FF/00/00", &cCursorUp); //red
		XAllocColor(dpy, cmap, &cCursorUp);
		}
	else XAllocColor(dpy, cmap, &cCursorUp);
	delete [] ColorString;
	ColorString = IniFile.GetIniValue("Color_CursorDown");
	if (!XParseColor(dpy, cmap, ColorString, &cCursorDown)) 
		{
		XParseColor(dpy, cmap, "rgb:FF/FF/00", &cCursorDown);//yellow
		XAllocColor(dpy, cmap, &cCursorDown);
		}
	else XAllocColor(dpy, cmap, &cCursorDown);
	delete [] ColorString;
	
	// Other graphics parameters
	char *temp = IniFile.GetIniValue("Graph_TamPoint");
	if ((temp != NULL)&&(strlen(temp) > 0)) limBigPoint = atof(temp);
	else limBigPoint = 3.0;
	delete [] temp;

	temp = IniFile.GetIniValue("Graph_LineWidth");
	if ((temp != NULL)&&(strlen(temp) > 0)) lineWidth = atoi(temp);
	else lineWidth = 1;
	delete [] temp;
	
	// Create a new preferences file if it doesn't exist or there are changes
	if ((!IniExist)||(IniFile.IsChanged()))		IniFile.Save(FileName);
	}

void cPreferences::Save() 
	{
	if (!IniFile.Save(FileName))	printf("Preferences Error: preferences file not updateable\n");
	}

char *cPreferences::GetInstallDir() 
	{
	char *temp = CopyString(InstallDir);

	// Return the new value
	return GeneralDir(temp);
	}

char *cPreferences::GetLaraDBDir() 
	{
	printf("Tomando LARA\n");
	char *temp;
	if (strlen(LaraDBDir) > 0)
		 temp = CopyString(LaraDBDir);
	else temp = CopyString(InstallDir);

	//printf("GENERAL DIR: %s\n", GeneralDir(temp));
	printf("Devolvemos generalDIR de %s\n", temp);
	// Return the new value
	return GeneralDir(temp);
	}

char *cPreferences::GetCentroidDBDir() 
	{
	char *temp;
	if (strlen(CentroidDBDir) > 0)	temp = CopyString(CentroidDBDir);
	else 				temp = CopyString(InstallDir);//return NULL;
	// Return the new value
	return GeneralDir(temp);
	}

//**************** GET LARA DB FILE *************
char *cPreferences::GetLaraDBFile() 
	{
	char *temp = GetLaraDBDir();
	// append name
	char *newString = new char[strlen(temp) + strlen(defaultDBName) + 2];
	strcpy(newString, temp);
	strcat(newString, defaultDBName);
	delete [] temp;
	
	// Return the new value
	return newString;
	}

char *cPreferences::GetAnalSpecDir() 
	{
	char *temp;
	if (strlen(AnalgamSpecDir) > 0)
		temp = CopyString(AnalgamSpecDir);
	else return NULL;

	// Return the new value
	return GeneralDir(temp);
	}

char *cPreferences::GetSpecDir() 
	{
	char *temp;
	if (strlen(SpecDir) > 0)		temp = CopyString(SpecDir);
	else 							return NULL;

	// Return the new value
	return GeneralDir(temp);
	}

char *cPreferences::GetCalibDir() 
	{
	char *temp;
	if (strlen(CalibDir) > 0)		temp = CopyString(CalibDir);
	else 							return NULL;

	// Return the new value
	return GeneralDir(temp);
	}

char *cPreferences::GetBackBDDir() 
	{
	char *temp;
	if (strlen(BackBDDir) > 0)		temp = CopyString(BackBDDir);
	else 							return NULL;

	// Return the new value
	return GeneralDir(temp);
	}

char *cPreferences::GetEfficiencyDir() 
	{
	char *temp;
	if (strlen(EfficiencyDir) > 0)		temp = CopyString(EfficiencyDir);
	else 							return NULL;

	// Return the new value
	return GeneralDir(temp);
	}
