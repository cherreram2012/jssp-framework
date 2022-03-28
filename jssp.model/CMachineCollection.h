#pragma once
#ifndef CMachineCollection_H
#define CMachineCollection_H

#ifndef STR_LENGTH
	#define STR_LENGTH 64
#endif

//------------------------------------------------------------------------------
//	ClassName  : CMachineCollection
//
//  Description: Representa una coleccion de maquinas.
// 
//  Revision   : 13/06/2014 
//------------------------------------------------------------------------------

class CMachineCollection {
	friend void PrintMachineInformation ( const CMachineCollection &Obj ); 

	private:
		int    iSize;
		int    iIndex;
		float  *NominalPower;
		char  **MachineList;

	public:
		//--- Constructor and Destructor ---//
		 CMachineCollection			 ( int count );		
		~CMachineCollection			 ( void );	  					

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