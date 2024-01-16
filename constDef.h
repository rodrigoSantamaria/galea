#ifndef __CONSTDEF_H__
#define __CONSTDEF_H__

// Definicion de menus
const int 	MENU_FILE_OPEN 				= 0,
			MENU_FILE_OPEN_SPECTRUM 	= 1,
			MENU_FILE_OPEN_BACKGROUND 	= 2,
			MENU_FILE_SAVE 				= 3,
			MENU_FILE_SAVE_AS 			= 4,
			MENU_FILE_SAVE_TO 			= 5,
			MENU_FILE_QUIT 				= 6;
			
const 
char  MENU_FILE_OPEN_NAME[] 						= "Open ... ",
				MENU_FILE_OPEN_SPECTRUM_NAME[] 		= "Open Spectrum ... ",
				MENU_FILE_OPEN_BACKGROUND_NAME[] 	= "Open Background ... ",
				MENU_FILE_SAVE_NAME[] 				= "Save",
				MENU_FILE_SAVE_AS_NAME[] 			= "Save As ... ",
				MENU_FILE_SAVE_TO_NAME[] 			= "Save To Background DB ... ",
				MENU_FILE_QUIT_NAME[] 				= "Quit";

const int 	MENU_CALIBRATION_OPEN 			 = 0,
				MENU_CALIBRATION_OPEN_ENERGY = 1,
				MENU_CALIBRATION_OPEN_SHAPE  = 2,
				MENU_CALIBRATION_SAVE 		 = 3,
				MENU_CALIBRATION_SAVE_ENERGY = 4,
				MENU_CALIBRATION_SAVE_SHAPE  = 5;
const 
char  MENU_CALIBRATION_OPEN_NAME[] 					= "Open Calibration... ",
				MENU_CALIBRATION_OPEN_ENERGY_NAME[] = "Open Energy Calibration ... ",
				MENU_CALIBRATION_OPEN_SHAPE_NAME[] 	= "Open Shape Calibration ... ",
				MENU_CALIBRATION_SAVE_NAME[] 		= "Save Calibration ... ",
				MENU_CALIBRATION_SAVE_ENERGY_NAME[] = "Save Energy Calibration ... ",
				MENU_CALIBRATION_SAVE_SHAPE_NAME[] 	= "Save Shape Calibration ... ";

const int 	MENU_ANALYSIS_LOAD_EFFICIENCY = 0;
const int 	MENU_ANALYSIS_LOAD_BACKGROUND = 1;
const int 	MENU_ANALYSIS_LOAD_IDENTIFICA = 2;
const int 	MENU_ANALYSIS_SAVE 			  = 3;
const int 	MENU_ANALYSIS_DELETE_PEAKS 	  = 4;
const int 	MENU_ANALYSIS_OPTIONS 	  	  = 5;

const 
char  MENU_ANALYSIS_SAVE_NAME[] 			= "Save Analysis... ",
	  MENU_ANALYSIS_LOAD_EFFICIENCY_NAME[] 	= "Load Efficiency Parameters... ",
	  MENU_ANALYSIS_LOAD_BACKGROUND_NAME[] 	= "Load Background File... ",
	  MENU_ANALYSIS_LOAD_IDENTIFICA_NAME[] 	= "Load Identification File... ",
	  MENU_ANALYSIS_DELETE_PEAKS_NAME[] 	= "Delete All Peaks",
	  MENU_ANALYSIS_OPTIONS_NAME[] 	= "Adjust options...";


// Definicion de extensiones de ficheros
const 
char  EXT_FICH_GENERAL[] 		= ".asf", // fichero de analisis de espectro de analgam
		EXT_FICH_SPECTRUM[] 	= ".dat", // fichero de espectros ASCII ya procesado
		EXT_FICH_BACKGROUND[] 	= ".dat", // fichero de fondos(?)
		EXT_FICH_BDBACKGROUND[] = ".fbd", // fichero de fondos
		EXT_FICH_IDENTIFICA[]   = ".db",  // fichero de identificaciones
		EXT_FICH_CAL[] 			= ".acc", // fichero de calibracion
		EXT_FICH_CALENERGIA[] 	= ".ace", // fichero de calibracion de energia
		EXT_FICH_CALFORMA[] 	= ".acf", // fichero de calibracion de forma
		EXT_FICH_ANALISIS[] 	= ".txt"; // fichero de resultados del analisis
const 
char  FILTER_FICH_GENERAL[] 		= "*.asf", // fichero de analisis de espectro de galea
		FILTER_FICH_SPECTRUM[] 		= "*.dat", // fichero de espectros ASCII ya procesado
		//En lo que renovemos los compiladores de puccini, usamos *.dat com filtro en
//		FILTER_FICH_SPECTRUM[] 		= "*.asc", // fichero de espectros ASCII
		FILTER_FICH_BACKGROUND[] 	= "*.dat", // fichero de fondos ASCII
		FILTER_FICH_BDBACKGROUND[] 	= "*.fbd", // fichero de base de datos de fondos ASCII
		FILTER_FICH_IDENTIFICA[] 	= "*.db",  // fichero de identificaciones ASCII
		FILTER_FICH_EFFICIENCY[] 	= "*.par", // fichero de base de datos de fondos ASCII
		FILTER_FICH_CAL[] 			= "*.acc", // fichero de calibracion
		FILTER_FICH_CALENERGIA[] 	= "*.ace", // fichero de calibracion de energia
		FILTER_FICH_CALFORMA[] 		= "*.acf", // fichero de calibracion de forma
		FILTER_FICH_ANALISIS[] 		= "*.txt"; // fichero de resultados del analisis

// Definicion de cabecera de fichero .asf
const 
char  CABECERA_ASF[] = "analgamfilev1.0";

// Definciones de titulos de ventanas
const 
char	SIN_FICHERO[] 				= "(empty)",
		PRECALIBRATION_TITLE[] 		= "Precalibration",
		CALIBRATION_EDIT_TITLE[] 	= "Calibration Edit",
		ANALYSIS_EDIT_TITLE[] 		= "Analysis Edit",
			// lectura/salvado ficheros
		LOAD_SPECTRUM_TITLE[] 		= "Load Spectrum File",
		LOAD_BACKGROUND_TITLE[] 	= "Load Background File",
		LOAD_BDBACKGROUND_TITLE[] 	= "Load Background File",
		LOAD_IDENTIFICA_TITLE[] 	= "Load Identification File",
		LOAD_FILE_TITLE[] 			= "Load Galea Spectrum File",
		SAVE_FILE_TITLE[] 			= "Save Galea Spectrum File",
		LOAD_CAL_TITLE[] 			= "Load Galea Calibration File",
		LOAD_CALENERGIA_TITLE[] 	= "Load Galea Calibration Energy File",
		LOAD_CALFORMA_TITLE[] 		= "Load Galea Calibration Shape File",
		SAVE_CAL_TITLE[] 			= "Save Galea Calibration File",
		SAVE_CALENERGIA_TITLE[] 	= "Save Galea Calibration Energy File",
		SAVE_CALFORMA_TITLE[] 		= "Save Galea Calibration Shape File",
		SAVE_BDBACKGROUND_TITLE[] 	= "Save DataBase Background",
		SAVE_ANALISIS_TITLE[] 		= "Save Result Analysis File",
		LOAD_EFFICIENCY_TITLE[] 	= "Load Efficiency Parameters",
		AVISOS[] 					= "Warning", 
		OVERWRITE_TITLE[] 			= "Overwrite File",
		CHANGES_TITLE[] 			= "Save Changes",
		EXITING_TITLE[] 			= "Exiting",
		APPLICATION_TITLE[] 		= "Galea v2.1 (abril-2012)";//Usamos para cambiar las versiones

// Definiciones de lugares de errores
const 
char  ERROR_SAVE_FILE[] 		= "SAVE FILE ERROR: ",
		ERROR_LOAD_FILE[] 		= "LOAD FILE ERROR: ",
		ERROR_LOAD_SPECTRUM[] 	= "LOAD SPECTRUM ERROR: ",
		ERROR_LOAD_BACKGROUND[] = "LOAD BACKGROUND ERROR: ",
		ERROR_CONTINUUM[] 		= "CONTINUUM ERROR: ",
		ERROR_CALIBRATION[] 	= "CALIBRATION ERROR: ",
		ERROR_ANALYSIS[] 		= "ANALYSIS ERROR: ";

// Definiciones de errores
const 
char  ERROR_NO_MEMORY_F[] 			= "no memory avalaible (%s)\n",
		ERROR_NO_MEMORY[] 			= "no memory avalaible\n",
		ERROR_NO_SAVE_FILE[] 		= "save file failure\n",
		ERROR_NO_LOAD_SPECTRUM[] 	= "load spectrum failure in %s\n",
		ERROR_FICHERO_VACIO[] 		= "spectrum file empty -> %s\n",
		ERROR_CORRUPT_SPECTRUM[] 	= "corrupt spectrum in %s\n",
		ERROR_NO_LOAD_FILE[] 		= "load file failure in %s\n",
		ERROR_NO_ASF_FILE[] 		= "%s is not an galean file\n",
		ERROR_CORRUPT_FILE[] 		= "%s is a corrupt file\n",
		ERROR_TOO_PEAKS[] 			= "too much peaks selected\n",
		ERROR_TOO_INTERVALS[] 		= "too much intervals selected\n",
		ERROR_GENERAL[] 			= "general failure\n";


// Definiciones de bloques de datos en fichero ASF
const int   BLOQUE_CIERRE 			= 0,
			BLOQUE_PRECALIBRACION 	= 1,
			BLOQUE_CONTINUO 		= 2,
			BLOQUE_CALIBRACION 		= 3,
			BLOQUE_VISUALIZACION 	= 4,
			BLOQUE_ANALISIS 		= 5,
			BLOQUE_ACTIVIDAD 		= 6;

// Deficiones de cadenas de ajuste del continuo
const 
char	CONTINUO_VACIO[] 	= "  Void",
		CONTINUO_BUSCADO[] 	= "  Searched",
		CONTINUO_AJUSTANDO[]= "  Adjusting",
		CONTINUO_AJUSTADO[] = "  Adjusted";
				
// Deficiones de cadenas de preguntas sobre ficheros
const 
char  	ASK_OVERWRITE[] = "Do you want overwrite %s ?",
		ASK_EXITING[] 	= "Do you want to save changes before exit?",
		ASK_CHANGES[] 	= "Do you want to save changes in %s ?";

//Información sobre las familias del los 6 elementos naturales -> en LARA.C
const bool bifurcaciones[] = 
		{
		0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,1,0,0,0,				//Familia del Th232
		0,0,0, 0, 1,0,1, 0, 0, 0, 0,1,0,1,1,0,0,0,1,0,1,0,0,0,	//Familia del U238
		0,0,0,1,0,0,0,0, 0,0,0,1,0,1,0,1,0,0,1,0,0,1,0,0,0		//Familia del U235
		};
		
const int naturales[] = 
	{
	// familia del Th232 [0]
	 252, 98, 0,
	 248, 96, 0,
	 244, 94, 0,
	 240, 92, 0,
	 240, 93, 0,
	 240, 94, 0,
	 244, 96, 0,
	 236, 92, 0,
	 232, 90, 0,//Th232 [8]
	 228, 88, 0,
	 228, 89, 0,
	 228, 90, 0,
	 224, 88, 0,
	 220, 86, 0,
	 216, 84, 0,
	 212, 82, 0,
	 212, 83, 0,
	 212, 84, 0,
	 208, 81, 0,
	 208, 82, 0,
	 	// familia del U238 [20]
	 250, 98, 0,
	 246, 96, 0,
	 242, 94, 0,
	 238, 92, 0,//U238 [23]
	 234, 90, 0,
	 234, 91, 1,
	 234, 91, 0,
	 234, 92, 0,//U234 [27]
	 230, 90, 0,//Th230 [28]
	 226, 88, 0,//Ra226 [29]
	 222, 86, 0,
	 218, 84, 0,
	 214, 82, 0,
	 218, 85, 0,
	 214, 83, 0,
	 218, 86, 0,
	 214, 84, 0,
	 210, 81, 0,
	 210, 82, 0,
	 206, 80, 0,
	 210, 83, 0,
	 206, 81, 0,
	 210, 84, 0,
	 206, 82, 0,//El Pb206 en la lista de Lara no lo tienen en cuenta por ser ya estable
	 			//Nosotros lo dejamos puesto, aunque no tiene emisiones asociadas
	 	// familia del U235 [44]
	 251, 98, 0,
	 247, 96, 0,
	 243, 94, 0,
	 243, 95, 0,
	 239, 92, 0,
	 239, 93, 0,
	 243, 96, 0,
	 239, 94, 0,
	 235, 92, 1,//U235 [52]
	 231, 90, 0,
	 231, 91, 0,
	 227, 89, 0,
	 227, 90, 0,
	 223, 87, 0,
	 223, 88, 0,
	 219, 85, 0,
	 219, 86, 0,
	 215, 83, 0,
	 215, 84, 0,
	 215, 85, 0,
	 211, 82, 0,
	 211, 83, 0,
	 207, 81, 0,
	 211, 84, 0,
	 207, 82, 0,
	 	// familia del Potasio[69]
  	 40, 19, 0 //k40
	 };
const unsigned int numNaturales = 70; 
const int cadenas[]= {
					 8*3,20*3,//Descendientes del Th232
					 52*3,68*3,//Descendientes del U235
					 23*3,43*3,//Descendientes del U238
					 };
const unsigned int numCadenas=3;

//CÓDIGOS DE LARA DE ALGUNOS ELEMENTOS MUY USADOS
const int URANIO235=354;
const int URANIO238=363;
					 
// Definicion de respuestas
#define SI 1
#define NO  2
#define CANCELAR 3

#endif
