#ifndef CSPGraph_PMH
#define CSPGraph_PMH

#include <math.h>
#include "CMachineGroup_PM.h"
#include "CMachineCollection_PM.h"

#define  NULL 0
#define  CeroReal 1E-3

enum ProblemType  { NonProblem = 0, PFSS, FSS, GFSS, HFSS, FFSS, JSS, HJSS, FJSS, OSS };

enum ScheduleType { stSemiActive = 1, stActive, stNonDelay };

enum ImproveType	{ itAtLeastOne, itAllAtSameTime }; 

//------------------------------------------------------------------------------
//	ClassName  : TObserverGantt
//
//  Description: Permite el vinculo del grafo y la visualizacion del diagrama de gannt
//               en la interfaz de la aplicacion.
//
//	Patron     : Observer.
// 
//  Revision   : 21/04/2012 
//------------------------------------------------------------------------------
class TObserverGantt {
	public:		
    virtual void PrintCmaxValue   ( float cmax )						 = 0;
    virtual void PrintFmaxValue   ( float fmax )						 = 0;
		//virtual void PrintLmaxValue ( float lmax )						 = 0;
    virtual void PrintIdleValue   ( float idle )						 = 0;
	  virtual void PrintEmaxValue   ( float emax )						 = 0;
	  virtual void PrintTmaxValue   ( float tmax )						 = 0;
		virtual void PrintPwValue     ( float pw )							 = 0;
	  virtual void PrintNeValue     ( int   ne )							 = 0;
    virtual void PrintNtValue     ( int   nt )							 = 0;
    virtual void PrintSchedule    ( enum ScheduleType type ) = 0;
 		virtual void SetMeasureMark   ( int count )              = 0;
    virtual void GraphMeasureMark ( int interval, float pos_time ) = 0;
		virtual void GraphTime        ( int machine, float startT, float elapsedT, unsigned ID, int op ) = 0;
};


//------------------------------------------------------------------------------
//	ClassName  : TObserverEnergy
//
//  Description: Permite el vinculo del grafo y la visualizacion del grafico de carga
//               en la interfaz de la aplicacion.
// 
//	Patron     : Observer. 
//
//  Revision   : 21/04/2012 
//------------------------------------------------------------------------------
class TObserverEnergy {
  public:
    virtual void DrawOutLine    ( void )				= 0;
    virtual void DrawEnergyAve  ( float value )	= 0;
    virtual void PrintECValue   ( float value )	= 0;
    virtual void PrintCmaxValue ( float value )	= 0;
		virtual void SetDataChart   ( float cmax, int intervals ) = 0;
    virtual void GraphInterval  ( int interval, float machine_power, char *machine_list[], int machine_on ) = 0;    
};


//------------------------------------------------------------------------------
//	ClassName  : CSPGraph_PM
//
//  Description: Representa un grafo para el JobShop Scheduling Problem (JSSP). 
//						   Es compatible con la representacion de secuencias del tipo <N> y <N x M>,
//							 y con las siguientes variantes del problema: PFSS, FSS, GFSS, JSS.
//
//  Revision   : 02/08/2014 
//------------------------------------------------------------------------------

class CSPGraph_PM {
	/* Declaracion del NODO para el grafo */
	typedef struct {
		unsigned ID;
		const CMachineGroupLite *MachineGroup;		
	} STRUCT_NODE;

	/* Declaracion de NODO para el subgrafo solucion */
	typedef struct {
		int Machine;
		float RTime;
		float DTime;
	} STRUCT_NODE_RESULT;

	/* Declaracion de un tipo OPERATION */
	typedef struct {
		unsigned ID;
		int Machine;
		float RTime;
		float DTime;
	} STRUCT_OPERATION;

	/* Declaracion de un item para DUE-DATE */
	typedef struct {
		unsigned idJob;
		float	Value;
	} STRUCT_DUEDATE;

	// Clase amiga
	friend class CGraphCopier; 

	private:	// Declaracion de datos miembros								
		int			  iJobs;
		int			  iMachines;
		int				iRecirc;
		int			  ImpAnd;	
		int      *indexOpB;
		int			 *m_block;
		int      **TM_ON;
		float		 *Ci;
		float		  Cmax;
		unsigned *JobsList;
		unsigned *Sequence;    
		unsigned *j_block;
		unsigned  iMeasureCount;
		bool		  ibuit_ok;		
		bool		  applyIBUIT;
		bool		  flagCmax;
		bool		  flagFmax;	
		bool		  flagLmax;
		bool		  flagEmax;
		bool		  flagTmax;
		bool		  flagImax;
		bool		  flagNe;
		bool		  flagNt;
		bool		  flagPw;		
		bool		  isRunnerGraph;
		
		STRUCT_NODE			   *TD;		
		STRUCT_DUEDATE	   *Di;
		STRUCT_NODE_RESULT *TR;
		enum ProblemType	  ProbType;
		enum ImproveType    ImpType;
		unsigned long		   *Performance;
		CSPGraph_PM	       *imgGraph;
		TObserverGantt	   *hndGantt;
    TObserverEnergy    *hndEnergy;
		const CMachineCollection_PM *MachineList;

		int stepIBUIT;
		double timeIBUIT;

	private:	// Declaracion funciones de utileria
		float getTimeBackOp											( int indexPi, unsigned id );
		float getTimeBackJob										( int indexPi, int machine, float *initTime = NULL );
		int   getBackOpIndex										( int indexPi, unsigned id, float *initTime = NULL );
		int   getBackJobIndex										( int indexPi, int machine, float *initTime = NULL );		
		bool  EqualOperation										( STRUCT_OPERATION Op1, STRUCT_OPERATION Op2 );
		void  FastClear													( void );
		void  RunnerGraph												( void );
		void  CreateTR													( void );
		void  CreateTM_ON												( void );				

		//---	Funciones para la tecnica IBUIT ---//
		bool  IBUIT_Technique										( void );
		int   SearchSpace												( int indexPi, unsigned id, int machine, int *indexPiBack );
		bool  ExistsHole												( int indexPi, int machine );
		int		LeftOffSet												( int pc_pos, unsigned *j_block, int *m_block, int j_count, int m_count, int target, int m_fija, STRUCT_OPERATION Pb, int *pb_pos );
		void  Internal_IBUIT										( int opCurrent, int opBack );

		//--- Funciones para los objetivo 'FlowTime', 'Lateness', 'Earliness' y 'Tardines' ---//
		void  CalculateCi												( void );
		int   IndexInDueDateVector							( unsigned id );

		//--- Otras funciones ---//
		inline int EqualCero										( float x ) { return fabs(x) < CeroReal ? 1 : 0; }
		inline int MayorOIgual									( float x, float y ) { return x > y || EqualCero (x - y); }

	public:
		//--- Constructors and Destructor ---//
		 CSPGraph_PM														( int piece, int machine, int recirc, bool apply_ibuit = false );		
		 CSPGraph_PM														( const CSPGraph_PM &Obj );
		~CSPGraph_PM														( void );													

		//--- Set Functions ---/
		void SetNodeValue											  ( int index, unsigned id, const CMachineGroupLite *machines );
		void SetSequence												( const unsigned *sequence );				
		void SetGantChartView										( TObserverGantt *handler );
    void SetEnergyChartView									( TObserverEnergy *handler );
		void SetMachineInformation							( const CMachineCollection_PM *Obj );
		void SetJobsList												( unsigned *jobs_list );
		void SetImproveType											( enum ImproveType type );
		void SetProblemType											( enum ProblemType type );
		void SetEnergyMeasureCount							( unsigned count );
		void SetImproveObjetive									( bool cmax, bool fmax, bool lmax, bool emax, bool tmax, bool imax, bool ne, bool nt, bool pw);
		bool SetDueDate													( unsigned id, float di );		
		void SetLinkToPerformance								( unsigned long &count );
			
		//--- Get Functions ---//
		int								GetPieceCount					( void ) const;
		int							  GetMachineCount				( void ) const;
		const unsigned   *GetSequence						( void ) const;
		enum ImproveType  GetImproveType				( void ) const;
		enum ProblemType  GetProblemType				( void ) const;

		unsigned *GetJobsList										( void ) const;
		unsigned  GetEnergyMeasureCount					( void ) const;
		void      GetValue											( int index, unsigned *id, const CMachineGroupLite **machines ) const;		
		void		  GetDueDate										( int index, unsigned *id, float *di ) const;		
		void		  GetImproveObjetive						( bool *cmax, bool *fmax, bool *lmax, bool *emax, bool *tmax, bool *imax, bool *ne, bool *nt, bool *pw) const;
		TObserverGantt  *GetGantChartView				( void ) const;
    TObserverEnergy *GetEnergyChartView			( void ) const;
		unsigned long *GetLinkToPerformance			( void ) const;
		const CMachineCollection_PM *GetMachineInformation ( void ) const;

		//--- IBUIT Functions ---//
		bool ApplyIBUIT													( void ) const;
		void ApplyIBUIT												  ( bool apply );		
		void ApplyForcedLeftJump								( int opCurrent, int opBack );

		//--- Evaluate Functions ---//
		float Makespan													( void );												// Objetivo Cmax
		float FlowTime													( unsigned *jobs_list = NULL );	// Objetivo Fmax	
		float Lateness													( unsigned *jobs_list = NULL );	// Objetivo	Lmax
		float Earliness													( unsigned *jobs_list = NULL );	// Objetivo	Emax
		float Tardines													( unsigned *jobs_list = NULL );	// Objetivo	Tmax
		float Idle															( void );												// Objetivo	Imax
		float EnergyConsumption									( void );										    // Objetivo	Pw
		int		EarlyJobsCount										( void );												// Objetivo	Ne
		int		TardinesJobsCount									( void );												// Objetivo	Nt		
	
		enum  ScheduleType SchedulePlannig			( void );
		void  CreateGanttChart									( void );
    void  CreateEnergyChart									( void );
    float GetMaxEnergyConsumption						( void );

		//--- ---//
		//void  CopyGraph													( const CSPGraph_PM &Obj );
		void  ClearGraph												( void );

		// Temporal
		void IBUIT_Performance (int *step, double *time) { *step = stepIBUIT; *time = timeIBUIT; }
};

//------------------------------------------------------------------------------
//	ClassName  : CGraphCopier
//
//  Description: Patron SINGLETON para un duplicador de grafos. 
//						   Clase amiga de la clase CSPGraph_PM				 
//
//  Revision   : 19/06/2014 
//------------------------------------------------------------------------------
class CGraphCopier {
	public:
		static void Duplicate ( CSPGraph_PM *dest, const CSPGraph_PM &src );
};
#endif
