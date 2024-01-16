#ifndef __PREFERENCES_H__
#define __PREFERENCES_H__

#include "InitFile.h"
#include <Xm/Xm.h>

extern char *CopyString(const char *string);

/*****************************************************
 ***																***
 ***						PREFERENCES							***
 ***																***
 *****************************************************/

class cPreferences 
	{
   /***********
    * GENERAL *
    ***********/
	public:
      cPreferences();
      ~cPreferences();
		void Load(Display *dpy, Widget w);
		void Save();
		
			// Directories
		char *GetInstallDir(); // Installation Analgam Directory
		char *GetLaraDBDir(); // Lara Database Directory
		char *GetLaraDBFile(); // Lara Database File
		char *GetCentroidDBDir(); // Centroids Database Directory
		char *GetAnalSpecDir(); // Analgam spectrum Directory
		char *GetSpecDir(); // Spectrum Directory
		char *GetCalibDir(); // Calibration Directory
		char *GetBackBDDir(); // Background Databases Directory
		char *GetEfficiencyDir(); // Efficiency Parameters Directory
			
			// Colors
		XColor GetGrafFgColor()			{ return cGrafFg; } // Graphic Carpet Foreround Color
		XColor GetGrafBgColor()			{ return cGrafBg; } // Graphic Carpet Background Color
		XColor GetZoomFgColor()			{ return cZoomFg; } // Zoom Widget Foreground Color
		XColor GetZoomBgColor()			{ return cZoomBg; } // Zoom Widget Background Color
		XColor GetChPeaksColor()		{ return cChPeaks; } // Peaks Channels Color
		XColor GetChContColor() 		{ return cChContinuum; } // Continuum Channels Color
		XColor GetAdjustContColor()	{ return cAdjustCont; } // Adjusted Continuum Color
		XColor GetAdjustPeakColor()	{ return cAdjustPeak; } // Adjusted Peaks Color
		XColor GetAdjustSelectedPeakColor()	{ return cAdjustSelectedPeak; } // Adjusted Selected Peak Color
		XColor GetGeneticPeakColor()	{ return cGeneticPeak; } // Adjusted Selected Peak Color
		XColor GetBackgroundColor()	{ return cBackground; } // Background Color
		XColor GetCursorUpColor()		{ return cCursorUp; } // Cursor Up Color
		XColor GetCursorDownColor()	{ return cCursorDown; } // Cursor Down Color
			// Other Graphics
		double GetLimitBigPoint()		{ return limBigPoint; } // visual limit to draw big points
		int    GetLineWidth()			{ return lineWidth; } // line width of fits and continuum adjust. Recommended and default value: 1
		
	
	private:
		cInitFile IniFile;
		char *FileName;
		char *HomeDirName;

			// Directories
		char *InstallDir; // Installation Analgam Directory
		char *LaraDBDir; // Lara Database Directory
		char *LaraDBFile; // Lara Database File
		char *CentroidDBDir; //Natural centroids Database Directory
		char *AnalgamSpecDir; // Analgam spectrum Directory
		char *SpecDir; // Spectrum Directory
		char *CalibDir; // Calibration Directory
		char *BackBDDir; // Background Databases Directory
		char *EfficiencyDir; // Efficiency Parameters Directory

			// Colors
		XColor cGrafFg, cGrafBg;
		XColor cZoomFg, cZoomBg;
		XColor cChPeaks, cChContinuum;
		XColor cAdjustCont, cAdjustPeak, cAdjustSelectedPeak;
		XColor cGeneticPeak;
		XColor cBackground;
		XColor cCursorUp, cCursorDown;
		
			// Other Graphics
		double limBigPoint;
		int    lineWidth;
		
		char *GeneralDir(char *relative); // convert a relative directory in general directory
	};

#endif
