#ifndef CMachineCollection_PMH
#define CMachineCollection_PMH

#ifndef LENGTH
	#define LENGTH 64
#endif

//------------------------------------------------------------------------------
//	ClassName  : CMachineCollection_PM
//
//  Description: Representa una coleccion de maquinas.
// 
//  Revision   : 13/06/2014 
//------------------------------------------------------------------------------

class CMachineCollection_PM {
	friend void PrintMachineInformation ( const CMachineCollection_PM &Obj ); 

	private:
		int    iSize;
		int    iIndex;
		float  *NominalPower;
		char  **MachineList;

	public:
		//--- Constructor and Destructor ---//
		 CMachineCollection_PM			 ( int count );		
		~CMachineCollection_PM			 ( void );	  					

		int         AddMachine       ( char *nameMachine );
		int         SetNominalPower  ( unsigned index, float NominalPow );
		int         GetIndexByName   ( const char *nameMachine ) const;		
		int         GetMachineCount  ( void ) const;
		int         GetMaxNameLength ( void ) const;
		float       GetNominalPower  ( unsigned index ) const;
		const char *GetNameByIndex   ( unsigned index ) const;
		
		bool				isFull					 ( void ) const;
		bool				isEmpty					 ( void ) const;
		void				Clear						 ( void );
};
#endif