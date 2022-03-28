#pragma once
#ifndef CWorkCenterCollection_H
#define CWorkCenterCollection_H

#ifndef STR_LENGTH
	#define STR_LENGTH 64
#endif

//------------------------------------------------------------------------------
//	ClassName  : CWorkCenter
//
//  Description: Representa Centro de trabajo (o Etapa, como tambien se le conoce) 
//							 donde se agrupan maquinas bajo cietos criterios.
//
//							 Nota: dentro de un centro de trabajo o etapa las maquinas
//										 pueden ser identicas y no identicas(similares o distintas).									
// 
//  Revision   : 13/06/2014 
//------------------------------------------------------------------------------

class CWorkCenter {
	struct MachineList {
	  char MachineName[STR_LENGTH];
		struct MachineList *Next;
	};

	typedef struct MachineList MACHINES_LIST;
	typedef MACHINES_LIST *MACHINES_LIST_PTR;

	private:
		int   N_Machines;
		bool  dataChance;
		bool  fastAccessMode;
		char  OperationName[STR_LENGTH];
		char **MachinesArray;
		MACHINES_LIST_PTR startPtr, lastPtr;
	
	public:
		 CWorkCenter									( void );
		 CWorkCenter									( const CWorkCenter &Obj );
		~CWorkCenter									( void ); 

		int AddMachine								( const char *machine_name );
		int UpdMachine								( unsigned index, const char *machine_name );
		int DelMachine								( unsigned index );

		void SetOperationName					( const char *op_name );

		int					GetMachineCount		( void ) const;
		const char *GetOperationName  ( void ) const;
		const char *GetMachineByIndex ( unsigned index ) const;
		
		bool ExistsMachineInCenter	  ( const char *machine_name );
		void EnableFastAccessMode			( void );
		void DisableFastAccessMode		( void );
		void ClearCenter							( void );

		CWorkCenter &operator =				( const CWorkCenter &Obj );
		bool				 operator ==			( const CWorkCenter &Obj );
};


//------------------------------------------------------------------------------
//	ClassName  : CWorkCenterCollection
//
//  Description: Representa una coleccion de objectos WorkCenter.
// 
//  Revision   : 13/06/2014 
//------------------------------------------------------------------------------
class CWorkCenterCollection {
	struct WorkCenterList {
	  CWorkCenter WCenter;
		struct WorkCenterList *Next;
	};

	typedef struct WorkCenterList WORKCENTER_LIST;
	typedef WORKCENTER_LIST *WORKCENTER_LIST_PTR;

	private:
		int  N_WorkCenter;	
		bool dataChance;
		bool fastAccessMode;		
		CWorkCenter *WCenterArray;
		WORKCENTER_LIST_PTR startPtr, lastPtr;

	public:
		 CWorkCenterCollection						 ( void );
		 CWorkCenterCollection						 ( const CWorkCenterCollection &Obj );
		~CWorkCenterCollection						 ( void );

		int AddWorKCenter									 ( const CWorkCenter &Obj );
		int UdpWorkCenter									 ( unsigned index, const CWorkCenter &Obj );
		int DelWorkCenter									 ( unsigned index );

		int					GetWorkCenterCount		 ( void ) const;
		int					GetTotalMachine				 ( void ) const;
		CWorkCenter GetWorkCenterByIndex   ( unsigned index ) const;

		bool ExistsMachineInCollection		 ( const char *machine_name ) const;
		void EnableFastAccessMode					 ( void );
		void DisableFastAccessMode				 ( void );
		void ClearCollection					     ( void );

		CWorkCenterCollection &operator =  ( const CWorkCenterCollection &Obj );
		bool									 operator == ( const CWorkCenterCollection &Obj );

		void PrintData (void) const;	// Use only for console aplications
};
#endif