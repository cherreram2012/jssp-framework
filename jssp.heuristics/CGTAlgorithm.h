#pragma once
#ifndef CGTAlgorithm_H
#define CGTAlgorithm_H

#include "../jssp.model/CDataSet.h"
#include "../jssp.model/CSchedule.h"

enum SolveConflict {
	NONE = 0, RANDOM = 1, SPT_GREEDY = 2, SEP_GREEDY = 4, ECT_GREEDY = 8, SPT_RANDOM = 16, SEP_RANDOM = 32, ECT_RANDOM = 64
};

//------------------------------------------------------------------------------
//	ClassName: GTAlgorithmBase
//
//  Description: Abstract base class for G&T Algorithm.
//
//  Revision: 08/04/2019 
//------------------------------------------------------------------------------
class CGTAlgorithmBase {
protected:
	typedef struct Atom {
		int j;
		int m;
		int oj;
		float rj;
		float pj;
		float cj;
	} ATOM_SCHEDULE;

	typedef struct AtomList {
		ATOM_SCHEDULE Atom;
		struct AtomList *Prev;
		struct AtomList *Next;

		//--- struct constructor ---//
		struct AtomList() {
			Prev = nullptr;
			Next = nullptr;
		}
	} ATOM_SCHEDULE_LIST;
	typedef ATOM_SCHEDULE_LIST *ATOM_SCHEDULE_LIST_PTR;

	protected:
		int iJobs;
		int iMachines;
		int iRecirc;
		float *fAcumM;
		const CDataSet *Dataset;

	protected:
		ATOM_SCHEDULE_LIST *create__C				 ( void );
				 ATOM_SCHEDULE	calculate__tCmin ( ATOM_SCHEDULE_LIST_PTR *C, enum SolveConflict criteria );
									 int	obtain__G        ( ATOM_SCHEDULE_LIST_PTR G[], ATOM_SCHEDULE_LIST_PTR C, int machine, enum SolveConflict criteria, 
									 											   int *op_spt, int *op_sep, int *op_ect);
								   //int	obtain__G        ( ATOM_SCHEDULE_LIST_PTR G[], ATOM_SCHEDULE_LIST_PTR C, ATOM_SCHEDULE m_star, enum SolveConflict criteria,
									 //												 int *op_spt, int *op_sep, int *op_ect);
									void  deletefrom__C		 ( ATOM_SCHEDULE_LIST_PTR *C, ATOM_SCHEDULE_LIST_PTR g );
									void  update__C				 ( ATOM_SCHEDULE_LIST_PTR *C );

	public:
		 //--- Constructor and Destructor ---//
		  CGTAlgorithmBase				 ( const CDataSet &dset );
		 virtual ~CGTAlgorithmBase ( void );

		 //--- Function virtual pure ---//
		 virtual CSchedule GenerateSchedule (enum SolveConflict criteria = RANDOM) = 0;

		 //--- Set functions ---//
		 void SetSeed ( int seed );
};

//------------------------------------------------------------------------------
//	ClassName: CGTAlgorithmClassic
//
//  Description: Implement a original G&T algorithm proposed by Gilffer & Thompson (1960).
//	
//	Reference:
//		- Giffler, B.Thomson, G.L.Algorithms for Solving Production Scheduling Problems.
//		  Operations Reseach 8, 487 - 503, 1960.
//
//  Revision: 16/04/2019 
//------------------------------------------------------------------------------
/*class CGTAlgorithmClassic : public CGTAlgorithmBase {
public:
	//--- Constructor and Destructor ---//
	CGTAlgorithmClassic(const CDataSet &dset);
	virtual ~CGTAlgorithmClassic(void);

	//---  ---//
	virtual CSchedule GenerateSchedule(enum SolveConflict criteria = RANDOMIZED);
};*/

//------------------------------------------------------------------------------
//	ClassName: GTAlgorithmHybrid
//
//  Description: Implement a hybric G&T algorithm proposed by Bierwirth & Mattfeld (1999).
//
//	Reference:
//			- Bierwirth, C. and Mattfeld D.C.Production Scheduling and Reschedunling with Genetic
//				Algorithms.Evolutinary Computation 7(1) : 1 - 17. 1999
//
//  Revision: 06/04/2019 
//------------------------------------------------------------------------------
class CGTAlgorithmHybrid : public CGTAlgorithmBase {
private:
	float Alfa;

public:
	//--- Constructor and Destructor ---//
	CGTAlgorithmHybrid(const CDataSet &dset);
	virtual ~CGTAlgorithmHybrid(void);

	//--- Set functions ---//
	void SetParamAlfa(float alfa);

	//--- Get functions ---//
	float GetParamAlfa(void);
};

//------------------------------------------------------------------------------
//	ClassName: GTAlgorithmMixed
//
//  Description: Implements a G&T algorithm using dispacthing rules for solve
//							 the undetermined step.
//	
//	Reference: Proposed by Carlos R. Herrera Márquez
//
//  Revision: 08/04/2019 
//------------------------------------------------------------------------------
class CGTAlgorithm_DRules : public CGTAlgorithmBase {
	public:
		//--- Constructor and Destructor ---//
		 CGTAlgorithm_DRules				  ( const CDataSet &dset );
		virtual ~CGTAlgorithm_DRules ( void );

		//---  ---//
		virtual CSchedule GenerateSchedule(enum SolveConflict criteria);
};

//------------------------------------------------------------------------------
//	ClassName: GTAlgorithm
//
//  Description: .
//	
//	Reference:
//		- 
//
//  Revision: 08/04/2019 
//------------------------------------------------------------------------------
class CGTAlgorithmAlfaRCL : public CGTAlgorithmBase {
	private:
		float Alfa;

	private:
		int	obtain__G_with_cost   ( ATOM_SCHEDULE_LIST_PTR G[], ATOM_SCHEDULE_LIST_PTR C, int machine, enum SolveConflict criteria, 
															  float *customin, float *customax);
		//int	obtain__G_with_cost		( ATOM_SCHEDULE_LIST_PTR G[], ATOM_SCHEDULE_LIST_PTR C, ATOM_SCHEDULE m_star, enum SolveConflict criteria,
		//														float *customin, float *customax);
		int reduce__G_rcl         ( ATOM_SCHEDULE_LIST_PTR rG[], const ATOM_SCHEDULE_LIST_PTR G[], int size_G, float customin, float customax, 
																enum SolveConflict criteria);

	public:
		//--- Constructor and Destructor ---//
		CGTAlgorithmAlfaRCL				  ( const CDataSet &dset, float alfa );
		virtual ~CGTAlgorithmAlfaRCL ( void );

		//---  ---//
		virtual CSchedule GenerateSchedule(enum SolveConflict criteria);
};


#endif 

