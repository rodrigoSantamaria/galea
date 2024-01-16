#ifndef __INITFILE_H__
#define __INITFILE_H__

extern void toUp(char *string);
extern char *CopyInUpper(const char *string);


/*****************************************************
 ***																***
 ***						INTERNAL LISTS						***
 ***																***
 *****************************************************/
typedef struct IniNodeList 
	{
	char *VarName, *VarValue;
	IniNodeList *Next;
	} IniNodeList;

typedef struct tListIniElements 
	{
	unsigned int numElements;
	IniNodeList *First;
	} tListIniElements;

/*****************************************************
 ***																***
 ***						  INIT FILE							***
 ***																***
 *****************************************************/

class cInitFile {
   /***********
    * GENERAL *
    ***********/
	public:
      cInitFile();
      ~cInitFile();
		bool Load(const char *fileName);
		bool Save(const char *fileName);
		bool IsChanged()					{ return Changes; }
		char *GetIniValue(const char *VarName);
		void SetIniValue(const char *VarName, const char *VarValue);
		
	private:
		bool Changes;
		tListIniElements List;
		void PutIniValue(const char *VarName, const char *VarValue);
	};
#endif
