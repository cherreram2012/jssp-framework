#pragma once
#ifndef CSPGraphCAPP_H
#define CSPGraphCAPP_H

#include "CGraphBase.h"
#include <ga/GA1DArrayGenome.h>

//------------------------------------------------------------------------------
//	ClassName  : CSPGraphCAPP
//
//  Description: Representa un grafo para el JobShop Scheduling Problem (JSSP) que puede
//							 ser incluido en un sistema CAPP. 
//						   Es compatible con la representacion de secuencias del tipo <N> y <N x M>,
//							 y con las siguientes variantes del problema: PFSS, FSS, GFSS, JSS, FJSS.
//
//  Revision   : 17/04/2019 
//------------------------------------------------------------------------------
class CGraphCAPP : public CGraphBase {
	/* Declaracion de un item para DUE-DATE */
	typedef struct {
		unsigned idJob;
		float	value;
	} STRUCT_DUEDATE;

	// Clase amiga
	friend class CGraphCopier; 

	private:	// Declaracion de datos miembros								
		int			  ImpAnd;			
		int      **TM_ON;
		float		 *Ci;		
		unsigned *JobsList;		  
		unsigned  iMeasureCount;
		bool		  flagCmax;
		bool		  flagFmax;	
		bool		  flagLmax;
		bool		  flagEmax;
		bool		  flagTmax;
		bool		  flagImax;
		bool		  flagNe;
		bool		  flagNt;
		bool		  flagPw;		

		GAGenome			 *Genome;
		CGraphCAPP	   *imgGraph;
		STRUCT_DUEDATE *Di;

	private:	// Declaracion funciones de utileria	
		virtual void CreateTR		 ( void );
		virtual void TravelGraph ( void );
					  void CreateTM_ON ( void );				
					  void FastClear	 ( void );

		//---	IBUIT Technique Functions  ---//
		virtual bool IBUIT_Technique ( void );
		void ApplyForcedLeftJump(int opCurrent, int opBack);

		//--- Funciones para los objetivo 'FlowTime', 'Lateness', 'Earliness' y 'Tardines' ---//
		void  CalculateCi					 ( void );
		int   IndexInDueDateVector ( unsigned id );

	public:
		//--- Constructors and Destructor ---//
		 CGraphCAPP													( int piece, int machine, int recirc, bool apply_ibuit = false );		
		 CGraphCAPP													( const CGraphCAPP &Obj );
		~CGraphCAPP													( void );													

		//--- Set Functions ---/
		void SetSequence										( const unsigned *sequence );				

		void SetMachineInformation					( const CMachineCollection *Obj );
		void SetJobsList										( unsigned *jobs_list );
		void SetImproveType									( enum ImproveType type );
		
		void SetEnergyMeasureCount					( unsigned count );
		void SetImproveObjetive							( bool cmax, bool fmax, bool lmax, bool emax, bool tmax, bool imax, bool ne, bool nt, bool pw);
		bool SetDueDate											( unsigned id, float di );		
		void SetLinkToPerformance						( unsigned long &count );
		void SetLinkToGenome								( GAGenome &g );
			
		//--- Get Functions ---//
		unsigned *GetJobsList								( void ) const;
		unsigned  GetEnergyMeasureCount			( void ) const;
		void      GetValue									( int index, unsigned *id, const CMachineGroupLite **machines ) const;		
		void		  GetDueDate								( int index, unsigned *id, float *di ) const;		
		void		  GetImproveObjetive				( bool *cmax, bool *fmax, bool *lmax, bool *emax, bool *tmax, bool *imax, bool *ne, bool *nt, bool *pw) const;
		TObserverGantt  *GetGantChartView		( void ) const;
    TObserverEnergy *GetEnergyChartView	( void ) const;
		unsigned long *GetLinkToPerformance	( void ) const;
		const CMachineCollection *GetMachineInformation ( void ) const;

		//--- IBUIT Functions ---//
		bool ApplyIBUIT										( void ) const;
		void ApplyIBUIT										( bool apply );		
		
		//--- Objective Functions ---//		
		virtual float Makespan						( void );
		float FlowTime										( unsigned *jobs_list = NULL );	// Objetivo Fmax	
		float Lateness										( unsigned *jobs_list = NULL );	// Objetivo	Lmax
		float Earliness										( unsigned *jobs_list = NULL );	// Objetivo	Emax
		float Tardines										( unsigned *jobs_list = NULL );	// Objetivo	Tmax
		float Idle												( void );												// Objetivo	Imax
		int		EarlyJobsCount							( void );												// Objetivo	Ne
		int		TardinesJobsCount						( void );												// Objetivo	Nt		
		virtual float EnergyConsumption		( void );										    // Objetivo	Pw	
    float GetMaxEnergyConsumption			( void );
		

		//--- Graphics Function ---//	
		virtual void  CreateGanttChart  ( void );
		void  CreateEnergyChart ( void );

		//--- ---//
		//void  CopyGraph													( const CSPGraph &Obj );
		void  ClearGraph												( void );

		// Temporal
		void tmpPrint_TD (int init_machine_index_in = 0);
		void tmpPrint_TR (int n = -1, int init_machine_index_in = 0);
		void tmpPrint_DD (void);
		void tmpPrint_Pw (int **);
		void IBUIT_Performance (int *step, double *time) { *step = stepIBUIT; *time = timeIBUIT; }
};

//------------------------------------------------------------------------------
//	ClassName  : CGraphCopier
//
//  Description: Clases duplicador de grafos. Clase amiga de la clase CSPGraph. 
//
//  Revision   : 19/06/2014 
//------------------------------------------------------------------------------
class CGraphCopier {
	public:
		static void Duplicate ( CGraphCAPP *dest, const CGraphCAPP &src );
};
#endif
