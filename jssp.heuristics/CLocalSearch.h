#pragma once
#ifndef CLocalSearch_H
#define CLocalSearch_H

#include <vector>
#include "CNeighborhood.h"
#include "../jssp.model/CSchedule.h"
#include "../jssp.model/CPieceCollection.h"
#include "../jssp.model/CMachineCollection.h"

//enum LocalSearchStrategyEnum { FIRST_IMPROVING, BEST_IMPROVING };

//------------------------------------------------------------------------------
//	ClassName: LocalSearchStrategy
//
//  Description: Abstract base class for Local Search Strategies.
//
//  Revision: 16/05/2019 
//------------------------------------------------------------------------------
class CLocalSearchStrategy
{
	protected:
		CGraphCAPP *Graph;		
		CPieceCollection *CollectionJ;
		CMachineCollection *CollectionM;
		CNeighborhoobStrategy *Neighborhood;

	protected:
		void CheckDataConsistency(void);

	public:
		//--- Constructor and destructor ---//
		CLocalSearchStrategy					( void );
		virtual ~CLocalSearchStrategy ( void );

		//--- Function virtual pure ---//
		virtual CSchedule Execute ( const CSchedule &entry, float *fo = nullptr ) = 0;

		void NeighborStrategy ( CNeighborhoobStrategy *neighbor ); 
		virtual void UserData ( CPieceCollection *cJ, CMachineCollection *cM );
};

//------------------------------------------------------------------------------
//	ClassName: CLocalSearchIBUIT
//
//  Description: Abstract base class for Local Search with IBUIT tecnique.
//
//  Revision: 16/05/2019 
//------------------------------------------------------------------------------
class CLocalSearchIBUIT : public CLocalSearchStrategy
{
	protected:
		CGraphCAPP *GraphPrime;

	public:
		//--- Constructor and destructor ---//
		 CLocalSearchIBUIT				 ( void );
		virtual ~CLocalSearchIBUIT ( void );

		virtual void UserData(CPieceCollection *cJ, CMachineCollection *cM);
};

//------------------------------------------------------------------------------
//	ClassName: CFirstImprovingSearch
//
//  Description: Concrete class for Firt Improving Local Search Strategy.
//
//  Revision: 16/05/2019 
//------------------------------------------------------------------------------
class CFirstImprovingSearch : public CLocalSearchStrategy {
	public:
		//--- Constructor and destructor ---//
		 CFirstImprovingSearch ( void );
		virtual ~CFirstImprovingSearch ( void );

		virtual CSchedule Execute ( const CSchedule &entry, float *fo = nullptr );
};

//------------------------------------------------------------------------------
//	ClassName: BestImprovingSearch
//
//  Description: Concrete class for Best Improving Local Search Strategy.
//
//  Revision: 16/05/2019 
//------------------------------------------------------------------------------
class CBestImprovingSearch : public CLocalSearchStrategy {
	public:
		//--- Constructor and destructor ---//
		 CBestImprovingSearch	( void );
		 virtual ~CBestImprovingSearch ( void );

		virtual CSchedule Execute ( const CSchedule &entry, float *fo = nullptr );
};

//------------------------------------------------------------------------------
//	ClassName: 
//
//  Description: Concrete class for Firt Improving Local Search Strategy 
//						   appliying IBUIT Technique.
//
//  Revision: 07/06/2019 
//------------------------------------------------------------------------------
class CFirstImprovingIBUIT : public CLocalSearchIBUIT
{
	public:
		//--- Constructor and destructor ---//
		 CFirstImprovingIBUIT ( void );
		 virtual ~CFirstImprovingIBUIT ( void );

		virtual CSchedule Execute ( const CSchedule &entry, float *fo = nullptr );
};

//------------------------------------------------------------------------------
//	ClassName: 
//
//  Description: Concrete class for Best Improving Local Search Strategy 
//						   appliying IBUIT Technique.
//
//  Revision: 07/06/2019 
//------------------------------------------------------------------------------
class CBestImprovingIBUIT : public CLocalSearchIBUIT
{
	public:
		//--- Constructor and destructor ---//
		 CBestImprovingIBUIT ( void );
		 virtual ~CBestImprovingIBUIT ( void );

		virtual CSchedule Execute ( const CSchedule &entry, float *fo = nullptr );
};

//------------------------------------------------------------------------------
//	ClassName: CVariableDescentSearch
//
//  Description: Concrete class for Variable Descent Local Search Strategy.
//
//  Revision: 16/05/2019 
//------------------------------------------------------------------------------
class CVariableDescentSearch : public CLocalSearchStrategy {
public:
	//--- Constructor and destructor ---//
	 CVariableDescentSearch ( void );
	 virtual ~CVariableDescentSearch ( void );

	virtual CSchedule Execute ( const CSchedule &entry, float *fo = nullptr );
};
#endif