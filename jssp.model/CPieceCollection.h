#pragma once
#ifndef CPieceCollection_H
#define CPieceCollection_H

#include "CDataSet.h"
#include "CSchedule.h"
#include "CGraphCAPP.h"
#include "CMachineGroup.h"
#include "CMachineCollection.h"

#ifndef STR_LENGTH
	#define STR_LENGTH 64
#endif

//------------------------------------------------------------------------------
//	ClassName  : CPieceTechnologicalRoute
//
//  Description: Describe a una pieza {nombre, id, prioridad, ...} con su ruta 
//               tecnologica por etapas {[m1, t1], [m2, t2], [m3, t3], ...}.
// 
//  Revision   : 02/08/2014 
//------------------------------------------------------------------------------

class CPieceTechnologicalRoute {	
	typedef struct StagesList {
		CMachineGroupExtended *Stage;
		struct StagesList *Next;
	} STAGES_LIST;
	typedef STAGES_LIST *STAGES_LIST_PTR;

	private:	// Declaracion de datos miembros
		unsigned Id;
		char		 PieceName[STR_LENGTH];
		float		 Priority;
		float		 DueDate;
		float		 Penality;
		float		 Reward;
		int			 OperationsCount;
		int			 NullOperationsCount;
		STAGES_LIST_PTR StagesRoute, LastStage;
		CMachineGroupExtended *clear;

	//--- Declaracion funciones de utilería ---//
	private:	
		bool isNullOperation (const CMachineGroupExtended *stage);

	public:
		//--- Constructor and Destructor ---//
		 CPieceTechnologicalRoute	( void );
		 CPieceTechnologicalRoute	( const CPieceTechnologicalRoute &Obj );
		~CPieceTechnologicalRoute	( void );

		int  AddMachineGroup ( const CMachineGroupExtended &Obj );
		void UpdMachineGroup ( unsigned index, const CMachineGroupExtended &Obj );
		void DeleteRoute		 ( void );
		void ClearPiece			 ( void );

		//--- Set Functions ---//
		void SetPieceName			( const char *name );
		void SetPieceID				( unsigned id );
		void SetPiecePriority	( float p );
		void SetDueDate				( float di );
		void SetPenality			( float pe );
		void SetReward				( float re ); 		

		//--- Get Functions ---//
		const char *GetPieceName					( void ) const;	
		unsigned    GetPieceID						( void ) const;
		float       GetPiecePriority			( void ) const;
		float			  GetDueDate						( void ) const;
		float			  GetPenality						( void ) const;
		float			  GetReward							( void ) const;
		float			  GetTechnologicalTime	( void ) const;
		int         GetOperationCount			( void ) const;
		int         GetNullOperationCount	( void ) const;
		
		CMachineGroupExtended *GetMachinesGroupByIndex ( unsigned index ) const;

		bool PresentMachine			 ( const char *machine_name, int *pos = NULL ) const;
		bool IsValidDueDateValue ( void );
		
		//--- Overload of Operators ---//
		CPieceTechnologicalRoute &operator =	( const CPieceTechnologicalRoute &Obj );
		bool										  operator ==	( const CPieceTechnologicalRoute &Obj );
		
		void PrintTechRouteInformation (void);		// Note: Use only for console applications
};


//------------------------------------------------------------------------------
//	ClassName  : CPieceCollection
//
//  Description: Contiene informacion sobre piezas y sus rutas tecnologicas.
// 
//  Revision   : 02/08/2014
//------------------------------------------------------------------------------

//--- Definicion de la struct INSTANCE_SIZE ---//
typedef struct {
	int Piece;
	int Machine;
} STRUCT_INSTANCE_SIZE;

//--- Definicion de la struct PIECE_RECIRC ---//
typedef struct {
	unsigned PieceID;
	int      nRecirc;			/* No. de Operaciones que recirculan para la pieza actual */
} STRUCT_PIECE_RECIRC;

//---  Definicion de la struct RECIRCULATION ---//
typedef struct Recirculation {
	int pRecirc;					/* No. de piezas que recirculan */
	int tRecirc;					/* No. total de operaciones que recirculan */
	STRUCT_PIECE_RECIRC *P_Recirc;

	/* Contructor de la Struct */
	struct Recirculation () {
		tRecirc  = 0;
		pRecirc  = 0;
		P_Recirc = NULL;
	}
} STRUCT_RECIRCULATION;

//--- Definicion de la struct DATA_PROBLEM ---//
typedef struct {
	STRUCT_INSTANCE_SIZE Size;
	STRUCT_RECIRCULATION Recirc;
} STRUCT_PROBLEM_DATA;

//--- Definicion de una Lista Simplemente Enlazada para almacenar valores de prioridad ---//
struct PriorityList{
	unsigned IdPiece;
	float    Priority;
	struct   PriorityList *Next;
};
typedef struct PriorityList PRIORITY_LIST;
typedef PRIORITY_LIST* PRIORITY_LIST_PTR;

//--- Libera la memoria asignada a la lista ---//
void FreePriorityList (PRIORITY_LIST_PTR *L);

class CPieceCollection {
	friend void PrintPieceInformation ( const CPieceCollection &Obj );

	private:	// Declaracion de datos miembros	
		int  Index;
		int  iPieceCount;
		int  iMachinesCount;		
		bool FIFO_Ruler;
		bool ArrayPieceAlreadyClean;
		bool RecircAlreadyCalculated;
		bool problemTypeDetermined;		
		enum ProblemType ProbType;		
		STRUCT_RECIRCULATION RecircData;
		CPieceTechnologicalRoute *ArrayPiece;
		CPieceTechnologicalRoute *cleanArrayPiece;

	private:	// Declaracion funciones de utileria
		bool IsOneWayFlow        ( const CPieceTechnologicalRoute &P1, const CPieceTechnologicalRoute &P2 );
		void CleanNullOperations ( CPieceTechnologicalRoute **cleanArray );

	public:
		//--- Constructors and Destructor ---//
		 CPieceCollection	( int pieces, int machines, bool fifo_ruler = false );
		 CPieceCollection	( const CPieceCollection &Obj );
		~CPieceCollection	( void );
		
		 int AddPiece				( const CPieceTechnologicalRoute &Obj );
		void UpdatePiece		( int index, const CPieceTechnologicalRoute &Obj );
		void ApplyFifoRuler	( bool fifo_ruler );
		bool ApplyFifoRuler	( void ) const;

		//--- Get Functions ---//
		 int GetPieceCount				( void ) const;		    
		void GetAllID						  ( unsigned *id_array ) const;
		void GetRecirculationData ( STRUCT_RECIRCULATION *sRecirc );
		void GetPriorityList			( PRIORITY_LIST_PTR *List );
		void FillDataSet						( CDataSet *dataset, const CMachineCollection &MachineList );
		
		CPieceTechnologicalRoute GetPieceByID    ( unsigned id ) const;	
		CPieceTechnologicalRoute GetPieceByName  ( const char *name ) const;
		CPieceTechnologicalRoute GetPieceByIndex ( unsigned index ) const;							
												
		//---  ---//
		bool CheckDueDateValues	( int *id_wrong_piece = NULL );		
		void Create_N_Graph			( CGraphCAPP *Graph, const CMachineCollection *MachineList, const unsigned *sequence ); 
		void Create_NxM_Graph		( CGraphCAPP *Graph, const CMachineCollection *MachineList, const unsigned *sequence ); 		

		void Create_N_Graph		  ( CGraphCAPP *Graph, const CMachineCollection *MachineList, CSchedule schedule );
		void Create_NxM_Graph   ( CGraphCAPP *Graph, const CMachineCollection *MachineList, CSchedule schedule );

		void Create_JSS_Graph  ( CGraphCAPP *Graph, const CMachineCollection *MachineList, CSchedule schedule );
		void Create_FJSS_Graph ( CGraphCAPP *Graph, const CMachineCollection *MachineList, CSchedule schedule );
		void Create_PFSS_Graph ( CGraphCAPP *Graph, const CMachineCollection *MachineList, CSchedule schedule );

		//---  ---//
		enum ProblemType ProblemType ( void );
					  		
		//--- Overload of Operators ---//
		CPieceCollection &operator =	( const CPieceCollection &Obj );
		bool						  operator ==	( const CPieceCollection &Obj );

		void ClearCollection ( void );
};

void PrintPieceInformation ( const CPieceCollection &Obj );
#endif
