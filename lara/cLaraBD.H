/* ***********
		types
	***********/
typedef struct 
	{
	char signature[13];
	unsigned int offElements;//Offset al que se encuentran los elementos en la BD
	unsigned int offEmissions;//Offset al que se encuentran las emisiones en la BD
	unsigned int offListEmissions;
	unsigned int offSummaryElements;
	unsigned int offSummaryEmissions;
	} THeaderLaraBD;



//********** TFILE ELEMENT LARABD ********
// Contiene la información de un elemento: nº atómico, nº de protones, nombre, tiempo de decaimiento.
// También contiene una lista a todas sus emisiones
typedef struct 
	{
	unsigned int key; // Atomic number | Protons Number | Metastable
	char name[3];		// Element Name
	float decayTime, incDecayTime; // Element's decay time (e incertidumbre)
	bool Natural;		// Natural element
	unsigned short int listEmissions; // offset of Emissions list
	bool equilibrio; //Indica si es una lista de descendientes o una de emisiones directas
	} TFileElementLaraBD;

//********** TFILE EMISSION LARABD ********
// Cada emisión contiene su energía, su probabilidad de emisión, 
// el tipo (gamma o tipo de x) y elemento emisor
typedef struct 
	{
	float energy, incEnergy; // energy emission
	float probability, incProbability; // emission probabililty
	unsigned char kindEmission; // Gamma, X Ray:
			// 0 -> Gamma
			// 1 -> XKa1
			// 2 -> XKa2
			// 3 -> XKb1
			// 4 -> XKb2
	unsigned short int posEmissor; // Element emissor related
	} TFileEmissionLaraBD;


class cSearchLaraBD; // class header (see below) 

/*****************************************************
 ***												***
 ***						   LARA BD				***
 ***												***
 *****************************************************/

class cLaraBD {
   /***********
    * GENERAL *
    ***********/
   public:
      cLaraBD();
      ~cLaraBD();
	void Load(char *otherAdress = NULL);
	bool GetIsRunning()  			{return State;};
	unsigned short int GetNumElementsBD()	{return nElements;};
	unsigned short int GetNumEmissionsBD() {return nEmissions;};

   /************
    * Elements *
    ************/
	public:
		unsigned short int GetAtomicNumber(unsigned short int);
		unsigned short int GetMassNumber(unsigned short int);
		float GetDecayTime(unsigned short int);
		float GetIncDecayTime(unsigned short int);
		bool IsMetaestable(unsigned short int);
		bool IsNatural(unsigned short int);
		bool IsEqui(unsigned short int);
		void GetNameElement(unsigned short int, char *);

   /*************
    * Emissions *
    *************/
	public:
		unsigned short int GetEmissor(unsigned short int);
		float GetEnergy(unsigned short int);
		float GetIncEnergy(unsigned short int);
		float GetProbability(unsigned short int);
		float GetIncProbability(unsigned short int);
		char GetKindEmission(unsigned short int);
		TFileEmissionLaraBD* GetEmissions();

   /************
    * Searchs  *
    ************/

	public:
		cSearchLaraBD SearchElements(int = -1, int = -1, int = -1, int = -1,
								bool = false, bool = false, bool = false, bool = false,
								char* = NULL);
			// Elements Search:
			//  - min Atomic number: int, -1 deactivates this value in the search
			//  - max Atomic number: int, -1 deactivates this value in the search
			//  - min Proton number: int, -1 deactivates this value in the search
			//  - max Proton number: int, -1 deactivates this value in the search
			//  - Metaestable search: bool, false deactivates metaestable elements search
			//  - Metaestable value: bool, value of metaestable in the search
			//  - Natural search: bool, false deactivates natural elements search
			//  - Natural value: bool, value of natural in the search
			//  - Name value: char *, name of element to search,
			//		NULL or "" deactivates this value in the search
		cSearchLaraBD SearchEmissions(float = -1, float = -1,
								float = -1, float = -1, int = -1,
								int = -1, int = -1, int = -1, int = -1,
								bool = false, bool = false, bool = false, bool = false,
								char* = NULL,
								bool = false, bool=false);
			// Emissions Search:
			//  - min Energy: float, < 0 deactivates this value in the search
			//  - max Energy: float, < 0 deactivates this value in the search
			//  - min Probability: float, < 0 deactivates this value in the search
			//  - max Probability: float, < 0 deactivates this value in the search
			//  - Kind Emission: int, -1 deactivates this value in the search (see type definition of Emissions)
			//  - min Atomic number: int, -1 deactivates this value in the search
			//  - max Atomic number: int, -1 deactivates this value in the search
			//  - min Proton number: int, -1 deactivates this value in the search
			//  - max Proton number: int, -1 deactivates this value in the search
			//  - Metaestable search: bool, false deactivates metaestable elements search
			//  - Metaestable value: bool, value of metaestable in the search
			//  - Natural search: bool, false deactivates natural elements search
			//  - Natural value: bool, value of natural in the search
			//  - Name value: char *, name of element to search,
			//						NULL or "" deactivates this value in the search
		cSearchLaraBD SearchEmissions(float, float, float, float, int);
		
		cSearchLaraBD SearchEmissionsAndDescendants(float = -1, float = -1,
								float = -1, float = -1, int = -1,
								int = -1, int = -1, int = -1, int = -1,
								bool = false, bool = false, bool = false, bool = false,
								char* = NULL,
								bool = false, bool=false);
								//Como el anterior, pero también buscamos emisiones
								//de los descendientes
		cSearchLaraBD SearchEmissionsByCode(float, float, float, float, int);
		
		unsigned short int *SearchEmissionsInternal(float = -1, float = -1,
								float = -1, float = -1, int = -1,
								int = -1, int = -1, int = -1, int = -1,
								bool = false, bool = false, bool = false, bool = false,
								char* = NULL,
								bool = false, bool=false);
								//Como el anterior, pero también buscamos emisiones
								//de los descendientes
		
		int daEmisionConMayorProbabilidad(int);
		int daEmisionConMayorProbabilidad(int, float, float);
		void daEmisionesConMayorProbabilidad(int,float,float, int *, float);
		void daEmisionesConMayorProbabilidad(int,float,float, int *, int, bool, bool);
		//int cLaraBD::numEmisionesGrandes(int elemento, float eInf, float eSup, float probMin);
		int numEmisionesGrandes(int elemento, float eInf, float eSup, float probMin);
	
  /***********
    * PRIVATE *
    ***********/

	private:
		bool State;
		unsigned short int nElements, nEmissions;
		TFileElementLaraBD *Elements;
		TFileEmissionLaraBD *Emissions;
		unsigned short int 	nListEmissions,  		*listEmissions,
									nSummaryElements, 	*summaryElements,
									nSummaryEmissions,	*summaryEmissions;
		unsigned short int GetAtomicNumberFromKey(unsigned int);
		unsigned short int GetMassNumberFromKey(unsigned int);
		bool IsMetaestableFromKey(unsigned int);
		bool IsEquiFromKey(unsigned int);
		void DeleteLaraBD();
};


/*****************************************************
 ***																***
 ***						SEARCH LARA BD						***
 ***																***
 *****************************************************/
 
 // Clase con distintas funciones de búsqueda en la base de datos Lara
 class cSearchLaraBD {
	friend cSearchLaraBD cLaraBD::SearchElements(int, int, int, int, bool, bool, bool, bool, char*);
	friend cSearchLaraBD cLaraBD::SearchEmissions(float, float, float, float, int, int, int, int, int, bool, bool, bool, bool, char*, bool, bool);
	friend cSearchLaraBD cLaraBD::SearchEmissionsByCode(float, float, float, float, int);
	friend cSearchLaraBD cLaraBD::SearchEmissionsAndDescendants(float, float, float, float, int, int, int, int, int, bool, bool, bool, bool, char*, bool, bool);
	//friend tListResults cLaraBD::SearchEmissionsInternal(float, float, float, float, int, int, int, int, int, bool, bool, bool, bool, char*, bool, bool);
	public:
			// general
		cSearchLaraBD();	// constructor
		~cSearchLaraBD(); // destructor
		cSearchLaraBD(const cSearchLaraBD& copy); // copy constructor
		
		// operators
//		cSearchLaraBD deleteRepeatedEmissions(cSearchLaraBD, cLaraBD);	
		cSearchLaraBD deleteRepeatedEmissions(cSearchLaraBD, TFileEmissionLaraBD*);	
		cSearchLaraBD operator+(const cSearchLaraBD) const;
		cSearchLaraBD operator-(const cSearchLaraBD) const;
		cSearchLaraBD operator*(const cSearchLaraBD) const;
		//cSearchLaraBD& cSearchLaraBD::operator=(const cSearchLaraBD&);
		cSearchLaraBD& operator=(const cSearchLaraBD&);
			// search management
		unsigned int GetNumItemsFound()			{ return numFound; }; // return the number of items found in this search
		int GetFirstResult(); // return the first item in this search (-1 if no exists)
		int GetLastResult(); // return the last item in this search (-1 if no exists)
		int GetNextResult(); // return the next item in relation with the last consultation (-1 if no exists)
		int GetPriorResult(); // return the previous item in raltion with the last consultation (-1 if no exists)
		int GetResult(unsigned int); // return the item placed in the passed value (-1 if no exists)
		int GetFastResult(unsigned int); // same that GetResult but not count like a consultation (-1 if no exists)

	private:
		cSearchLaraBD(unsigned int, unsigned short int *); // To create with all data
		unsigned int numFound; // items found in search
		unsigned short int *Results; // search results
		int Guide; // actual position of guide
};
