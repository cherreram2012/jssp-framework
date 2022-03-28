#pragma once
#ifndef CSPGraphBase_H
#define CSPGraphBase_H

#include <math.h>
#include "CSchedule.h"
#include "CMachineGroup.h"
#include "CMachineCollection.h"

#define  NULL 0
#define  CeroReal 1E-3

enum ImproveType { itAtLeastOne, itAllAtSameTime };

enum ScheduleType { stSemiActive = 1, stActive, stNonDelay };

enum ProblemType { NonProblem = 0, PFSS, FSS, GFSS, HFSS, FFSS, JSS, HJSS, FJSS, OSS };

//--------------------------------------------------------------------------------------
//	ClassName  : TObserverGantt
//
//  Description: Permite el vinculo del grafo y la visualizacion del diagrama de gantt
//               en la interfaz de la aplicacion.
//
//	Patron     : Observer.
// 
//  Revision   : 21/04/2012 
//--------------------------------------------------------------------------------------
class TObserverGantt {
public:
	virtual void PrintCmaxValue(float cmax) = 0;
	virtual void PrintFmaxValue(float fmax) = 0;
	//virtual void PrintLmaxValue ( float lmax )						 = 0;
	virtual void PrintIdleValue(float idle) = 0;
	virtual void PrintEmaxValue(float emax) = 0;
	virtual void PrintTmaxValue(float tmax) = 0;
	virtual void PrintPwValue(float pw) = 0;
	virtual void PrintNeValue(int   ne) = 0;
	virtual void PrintNtValue(int   nt) = 0;
	virtual void PrintSchedule(enum ScheduleType type) = 0;
	virtual void SetMeasureMark(int count) = 0;
	virtual void GraphMeasureMark(int interval, float pos_time) = 0;
	virtual void GraphTime(int machine, float startT, float elapsedT, unsigned ID, int op) = 0;
};


//-------------------------------------------------------------------------------------
//	ClassName  : TObserverEnergy
//
//  Description: Permite el vinculo del grafo y la visualizacion del grafico de carga
//               en la interfaz de la aplicacion.
// 
//	Patron     : Observer. 
//
//  Revision   : 21/04/2012 
//--------------------------------------------------------------------------------------
class TObserverEnergy {
public:
	virtual void DrawOutLine(void) = 0;
	virtual void DrawEnergyAve(float value) = 0;
	virtual void PrintECValue(float value) = 0;
	virtual void PrintCmaxValue(float value) = 0;
	virtual void SetDataChart(float cmax, int intervals) = 0;
	virtual void GraphInterval(int interval, float machine_power, char *machine_list[], int machine_on) = 0;
};

//--------------------------------------------------------------------------------------
//	ClassName  : CSPGraphBase
//
//  Description: Representa un grafo base abstracto para el Job Shop Scheduling Problem.
//
//  Revision   : 17/04/2019 
//--------------------------------------------------------------------------------------
class CGraphBase {
	protected:
		/*** Declaracion del NODO para el grafo ***/
		typedef struct {
			unsigned ID;
			const CMachineGroupLite *MachineGroup;
		} STRUCT_NODE;

		/*** Declaracion de NODO para el subgrafo solucion ***/
		typedef struct {
			float RTime;
			float DTime;
			  int Machine;
				int prevJob;
				int prevOp;
		} STRUCT_NODE_RESULT;

		/*** Declaracion de un tipo OPERATION ***/
		typedef struct {
			unsigned ID;
				 float RTime;
				 float DTime;
					 int Machine;
		} STRUCT_OPERATION;

	protected:
		/* data member for instance representation */
		float	Cmax;		
		int	  iJobs;
		int	  iMachines;
		int	  iRecirc;
		int	  stepIBUIT;
		bool  isGraphTraveled;

		/* data member for IBUIT */
		int		*indexOpB;
		int		*m_block;
		bool	 ibuit_ok;
		bool	 applyIBUIT;
		double timeIBUIT;
		unsigned *j_block;
		unsigned *Sequence;
		CSchedule schedule;
	  unsigned long *Performance;
		enum ImproveType ImpType;

		/*  */
		STRUCT_NODE				 *TD;		
		STRUCT_NODE_RESULT *TR;		
		enum ProblemType    ProbType;		
		TObserverGantt		 *hndGantt;	
		TObserverEnergy		 *hndEnergy;
		const CMachineCollection *MachineList;

	protected:
		virtual void CreateTR				( void ) = 0;
		virtual void TravelGraph		( void ) = 0;
					 float getTimeBackOp	( int indexPi, unsigned id );
					 float getTimeBackJob	( int indexPi, int machine, float *initTime = NULL );

		//---	Protected IBUIT Technique Functions  ---//
		virtual bool IBUIT_Technique ( void );
						 int getBackOpIndex  ( int indexPi, unsigned id, float *initTime = NULL );
						 int getBackJobIndex ( int indexPi, int machine, float *initTime = NULL );
						 int SearchSpace		 ( int indexPi, unsigned id, int machine, int *indexPiBack );
						 int LeftOffSet			 ( int pc_pos, unsigned *j_block, int *m_block, int j_count, int m_count, int target, int m_fija, STRUCT_OPERATION Pb, int *pb_pos );
						bool ExistsHole			 ( int indexPi, int machine );
						void Internal_IBUIT	 ( int opCurrent, int opBack );

		//--- Others functions ---//
		inline bool EqualOperation ( STRUCT_OPERATION Op1, STRUCT_OPERATION Op2 );
		inline int  EqualCero      ( float x ) { return fabs(x) < CeroReal ? 1 : 0; }
		inline int  MayorOIgual    ( float x, float y ) { return x > y || EqualCero(x - y); }

	public:
		//--- Set Functions ---/
		void SetNodeValue				( int index, unsigned id, const CMachineGroupLite *machines );
		void SetProblemType			( enum ProblemType type );
		void SetGantChartView		( TObserverGantt *handler );
		void SetEnergyChartView ( TObserverEnergy *handler );

		//--- Get Functions ---//
		int								GetPieceCount   ( void ) const;
		int							  GetMachineCount ( void ) const;
		const unsigned   *GetSequence     ( void ) const;
		enum ImproveType  GetImproveType  ( void ) const;
		enum ProblemType  GetProblemType  ( void ) const;
		const CSchedule  &GetSchedule     ( void ) const;

		//--- Objective Function ---//	
		virtual float			Makespan				( void ) = 0;		// Objetivo Cmax
		enum ScheduleType SchedulePlannig ( void );

		//--- Graphic Function ---//	
		virtual void CreateGanttChart ( void );
};
#endif