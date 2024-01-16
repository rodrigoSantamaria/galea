#ifndef __IDENTIFY_H__
#define __IDENTIFY_H__

#include "picos.h"
#include <Xm/Xm.h>

XmString XmStringNameElementSup(unsigned short int element, char *littleFont, char *normalFont = "");
XmString XmStringNameElement(unsigned short int element, char *littleFont, char *normalFont = "");
XmString XmStringNameElementPlus(unsigned short int element, char *littleFont, char *normalFont );
//char *superindice(int);
wchar_t *superindice(int);

/*****************************************************
 ***																***
 ***				IDENTIFY WIDGETS PACKET					***
 ***																***
 *****************************************************/

class cVisIdentify {
   /***********
    * GENERAL *
    ***********/
   public:
		cVisIdentify();
		~cVisIdentify();
		void SetFonts(XmFontList FontList) 		{ Fonts = FontList; }
		void SetFontBold(char *name);
		void SetFontLittle(char *name);
		void CreateWidgets(Widget parent);
		void SetPeakList(tListaPicos *List) 	{ peakList = List; }
		void ShowCandidates(int numInterval = 0, int numPeak = 0);
		void ShowCandidatesNodo(NodoPico *peak);
		void ShowCandidatesCalibracion(int numInterval = 0, int numPeak = 0);
		void ShowCandidatesAdv(int amin, int amax, float pmin, float pmax,float emin, float emax, int prmin, int prmax,bool buscaNatu, bool natu, bool buscaMeta, bool meta,int tipoEmision, char *nombre);
		void ChangeSelectedEmission(int newPos);
		unsigned short int GetSelectedEmission() {return selectedEmission;}
		bool IsElementInList(unsigned short int element, int numInterval, int numPeak);
		bool IsEmissionInList(unsigned short int emission, int &numInterval, int &numPeak);
		Widget getFoundsList();
		
   /***********
    * PRIVATE *
    ***********/

	private:
			// Widgets
		Widget father, rcGround;
		Widget lbEmissions, lstList;
		Widget frmButtons, chkAutoSort, btChangeSearch;
			//Widgets de análisis
		Widget lbElemFBD;
			// Fonts
		XmFontList Fonts;
		char *BoldName, *LittleName;
			// Peaks
		tListaPicos *peakList;
			// Search
		cSearchLaraBD founds;	 //Elementos encontrados naturales
		cSearchLaraBD foundsNoNat;//Elementos encontrados no naturales
		cSearchLaraBD foundsDesc;//Elementos encontrados naturales descendientes
		cSearchLaraBD foundsTotal;//Elementos naturales y no naturales, sin repeticiones
		int nInterval, nPeak;
			// internal list
		void *listEmissions;
		int selectedEmission;
		int *emisionesOrdenadas;
		
		XmString GetEntryListXmString(unsigned short int emission);
		double GetScore(unsigned short int emission, int numInterval, int numPeak);
		
		/***************
			DEBUG
		****************/
		void MuestraIdentificados();
		
		void addLista(cSearchLaraBD, bool, int, int);
		};

#endif
