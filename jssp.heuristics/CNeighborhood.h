#pragma once
#ifndef CNeighborhood_H
#define CNeighborhood_H

#include "../jssp.model/CSchedule.h"
#include "../jssp.model/CSchedule.h"
#include "../jssp.model/CPieceCollection.h"
#include "../jssp.model/CMachineCollection.h"

enum NeighborhoobStrategy { LINEAR_ADJACENT, CIRCULAR_ADJACENT, LINEAR_NO_ADJACENT };

//------------------------------------------------------------------------------
//	ClassName: CNeighborhoobStrategy
//
//  Description: Abstract base class for Neigborhoob Technique.
//
//  Revision: 05/05/2019 
//------------------------------------------------------------------------------
class CNeighborhoobStrategy 
{
	protected:
		int iJobs;
		int iMachines;
		int iRecirc;
		CSchedule EntryPoint;
		bool existsEntryPoint;

	public:
		//--- Constructor and destructor ---//
		CNeighborhoobStrategy					 ( void );
		virtual ~CNeighborhoobStrategy ( void );

		//--- Function virtual pure ---//
		virtual int Cardinality         ( void ) = 0;
		virtual CSchedule VisitNeighbor ( int i ) = 0;

		//--- Set functions ---//
		virtual void SetEntryPoint(const CSchedule &S);
};

//------------------------------------------------------------------------------
//	ClassName: CLinearAdjacentNeighbor
//
//  Description: Concrete class for Linear Adjacent Neighborhood structure.
//
//  Revision: 05/05/2019 
//------------------------------------------------------------------------------
class CLinearAdjacentNeighbor : public CNeighborhoobStrategy
{
	public:
		//--- Constructor and destructor ---//
		 CLinearAdjacentNeighbor ( void );
		~CLinearAdjacentNeighbor ( void );

		//---  ---//
		virtual int Cardinality				  ( void );
		virtual CSchedule VisitNeighbor ( int i );
};

//------------------------------------------------------------------------------
//	ClassName: CCircularAdjacentNeighbor
//
//  Description: Concrete class for Circular Adjacent Neighborhood structure.
//
//  Revision: 05/05/2019 
//------------------------------------------------------------------------------
class CCircularAdjacentNeighbor : public CNeighborhoobStrategy
{
	public:
		//--- Constructor and destructor ---//
		 CCircularAdjacentNeighbor ( void );
		~CCircularAdjacentNeighbor ( void );

		//---  ---//
		virtual int Cardinality				  ( void );
		virtual CSchedule VisitNeighbor ( int i );
};

//------------------------------------------------------------------------------
//	ClassName: CLinearNoAdjacentNeighbor
//
//  Description: Concrete class for Linear No-adjacent Neighborhood structure.
//
//  Revision: 07/05/2019 
//------------------------------------------------------------------------------
class CLinearNoAdjacentNeighbor : public CNeighborhoobStrategy
{
	private:
		struct NeighborMappig {
			int X;
			int Y;
		};
	
	private:
		struct NeighborMappig *neighbormap;

	public:
		//--- Constructor and destructor ---//
		 CLinearNoAdjacentNeighbor ( void );
		~CLinearNoAdjacentNeighbor ( void );

		//---  ---//
		virtual void SetEntryPoint			( const CSchedule &S );
		virtual int Cardinality				  ( void );
		virtual CSchedule VisitNeighbor ( int i );
};
#endif