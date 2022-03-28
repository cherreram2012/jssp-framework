#ifndef CMachineGroup_PMH
#define CMachineGroup_PMH

#include "CMachineCollection_PM.h"

#ifndef LENGTH
	#define LENGTH 64
#endif

struct Data_Machine {
	int   Machine;	
	char  MachineName[LENGTH];
	float Time;	

	// Constructor de la struct
	struct Data_Machine() {
		Machine        = -1;
		MachineName[0] = '\0';
		Time           = 0.0;
	}
};

typedef struct Data_Machine STRUCT_DATA_MACHINE;

//------------------------------------------------------------------------------
//	ClassName  : CMachineGroupLite
//
//  Description: Representa una etapa en el Flexible Flow/Job Shop Schedunling.
// 
//  Revision   : 08/06/2014 
//------------------------------------------------------------------------------

class CMachineGroupLite {
	private:
		int Index;
		int N_Machines;
		STRUCT_DATA_MACHINE *MachineGroup;

	public:
		CMachineGroupLite											( int n_machines );
		CMachineGroupLite											( const CMachineGroupLite &Obj );
		~CMachineGroupLite										( void );

		int AddMachine												( int machine, float time );
		int UpdMachine												( unsigned index, int machine, float time );

		int									GetMachineCount   ( void ) const;	
		STRUCT_DATA_MACHINE GetMachineById		( int id ) const;
		STRUCT_DATA_MACHINE GetMachineByIndex ( unsigned index ) const;

		void ClearGroup										    ( void );

		CMachineGroupLite &operator =					( const CMachineGroupLite &Obj );
		bool							 operator ==				( const CMachineGroupLite &Obj );
};

//------------------------------------------------------------------------------
//	ClassName  : CMachineGroupExtended
//
//  Description: Representa una etapa para el Flexible Flow/Job Shop Schedunling.
// 
//  Revision   : 13/06/2014 
//------------------------------------------------------------------------------

class CMachineGroupExtended {
	struct MachineList {
	  char   MachineName[LENGTH];
		float  Time;
		struct MachineList *Next;
	};

	typedef struct MachineList MACHINES_LIST;
	typedef MACHINES_LIST *MACHINES_LIST_PTR;

	private:
		int  N_Machines;
		bool dataChance;
		bool AddInOrder;
		CMachineGroupLite *GroupLite;
		MACHINES_LIST_PTR startPtr, lastPtr;

	public:
		CMachineGroupExtended											 ( bool add_in_order = true );
		CMachineGroupExtended											 ( const CMachineGroupExtended &Obj );
		~CMachineGroupExtended										 ( void );

		int AddMachine														 ( const char *machine_name, float time );
		int UpdMachine													 	 ( unsigned index, const char *machine_name, float time );
		int DelMachine														 ( unsigned index );

		int									     GetMachineCount   ( void ) const;	
		STRUCT_DATA_MACHINE      GetMachineByIndex ( unsigned index ) const;
		const CMachineGroupLite *GetGroupLite      ( const CMachineCollection_PM &M_list );
		
		bool  IsAddedInOrder											 ( void ) const;
		void  ClearGroup									         ( void );

		CMachineGroupExtended &operator =					 ( const CMachineGroupExtended &Obj );
		bool								   operator ==         ( const CMachineGroupExtended &Obj );
};
#endif