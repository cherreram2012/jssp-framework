#pragma once
#ifndef CPathRelinking_H
#define CPathRelinking_H

#include "../jssp.model/CSchedule.h"
#include "../jssp.heuristics/CLocalSearch.h"
#include "../jssp.heuristics/CNeighborhood.h"
#include "../jssp.heuristics/CRestrictedNeighbor.h"

//------------------------------------------------------------------------------
//	ClassName: CPathRelinking
//
//  Description: Represent a base class for all Path-Relinkings techniques.
//
//  Revision: 30/05/2019 
//------------------------------------------------------------------------------
class CPathRelinking 
{
	protected:
		CSchedule S;
		CSchedule Sstart;
		CGraphCAPP *Graph;
		RestrictedNeighbor Rneighbor;
		CPieceCollection *CollectionJ;
		CMachineCollection *CollectionM;
		CLocalSearchStrategy *localSearch;
		CNeighborhoobStrategy *neighborhood;

	protected:
		void CheckDataConsistency(void);

	public:
		//--- Constructor and destructor ---//
		CPathRelinking ( void );
		CPathRelinking ( CLocalSearchStrategy *locals, CNeighborhoobStrategy *neighbor );
		virtual ~CPathRelinking ( void );

		//--- Function virtual pure ---/
		virtual CSchedule Relink ( const CSchedule &Si, const CSchedule &Sg, float *fo = nullptr ) = 0;

		//--- General functions ---//
		void LocalSearch  ( CLocalSearchStrategy *locals );
		void Neighborhood ( CNeighborhoobStrategy *neighbor );
		void UserData		  ( CPieceCollection *cJ, CMachineCollection *cM );
};

//------------------------------------------------------------------------------
//	ClassName: CForward_PR
//
//  Description: Represent a derivated class for Forward path-relinking technique.
//
//  Revision: 30/05/2019 
//------------------------------------------------------------------------------
class CForward_PR : public CPathRelinking
{
	public:
		//--- Constructor and destructor ---//
		 CForward_PR ( void );
		 CForward_PR ( CLocalSearchStrategy *locals, CNeighborhoobStrategy *neighbor );
		~CForward_PR ( void );

		virtual CSchedule Relink (const CSchedule &Si, const CSchedule &Sg, float *fo = nullptr);
};

//------------------------------------------------------------------------------
//	ClassName: CBackward_PR
//
//  Description: .
//
//  Revision: 23/05/2019 
//------------------------------------------------------------------------------
class CBackward_PR : public CPathRelinking
{
	public:
		//--- Constructor and destructor ---//
		 CBackward_PR ( void );
		 CBackward_PR ( CLocalSearchStrategy *locals, CNeighborhoobStrategy *neighbor );
		~CBackward_PR ( void );

		virtual CSchedule Relink (const CSchedule &Si, const CSchedule &Sg, float *fo = nullptr);
};

//------------------------------------------------------------------------------
//	ClassName: CBackAndForward_PR
//
//  Description: .
//
//  Revision: 23/05/2019 
//------------------------------------------------------------------------------
class CBackAndForward_PR : public CPathRelinking
{
	public:
		//--- Constructor and destructor ---//
		 CBackAndForward_PR ( void );
		 CBackAndForward_PR ( CLocalSearchStrategy *locals, CNeighborhoobStrategy *neighbor );
		~CBackAndForward_PR ( void );

		virtual CSchedule Relink (const CSchedule &Si, const CSchedule &Sg, float *fo = nullptr);
};

//------------------------------------------------------------------------------
//	ClassName: Mixed_PR
//
//  Description: .
//
//  Revision: 23/05/2019 
//------------------------------------------------------------------------------
class CMixed_PR : public CPathRelinking
{
	public:
		//--- Constructor and destructor ---//
		 CMixed_PR ( void );
		 CMixed_PR ( CLocalSearchStrategy *locals, CNeighborhoobStrategy *neighbor );
		~CMixed_PR ( void );

		virtual CSchedule Relink (const CSchedule &Si, const CSchedule &Sg, float *fo = nullptr);
};

//------------------------------------------------------------------------------
//	ClassName: CTruncated_PR 
//
//  Description: .
//
//  Revision: 23/05/2019 
//------------------------------------------------------------------------------
class CTruncated_PR : public CPathRelinking
{
	public:
		//--- Constructor and destructor ---//
		 CTruncated_PR ( void );
		 CTruncated_PR ( CLocalSearchStrategy *locals, CNeighborhoobStrategy *neighbor );
		~CTruncated_PR ( void );

		virtual CSchedule Relink (const CSchedule &Si, const CSchedule &Sg, float *fo = nullptr);
};

//------------------------------------------------------------------------------
//	ClassName: CExternal_PR
//
//  Description: .
//
//  Revision: 23/05/2019 
//------------------------------------------------------------------------------
class CExternal_PR : public CPathRelinking
{
public:
	//--- Constructor and destructor ---//
	 CExternal_PR ( void );
	 CExternal_PR ( CLocalSearchStrategy *locals, CNeighborhoobStrategy *neighbor );
	~CExternal_PR ( void );

	virtual CSchedule Relink (const CSchedule &Si, const CSchedule &Sg, float *fo = nullptr);
};

//------------------------------------------------------------------------------
//	ClassName: CRandomized_PR
//
//  Description: .
//
//  Revision: 23/05/2019 
//------------------------------------------------------------------------------
class CRandomized_PR : public CPathRelinking
{
	public:
		//--- Constructor and destructor ---//
		 CRandomized_PR ( void );
		 CRandomized_PR ( CLocalSearchStrategy *locals, CNeighborhoobStrategy *neighbor );
		~CRandomized_PR ( void );

		virtual CSchedule Relink (const CSchedule &Si, const CSchedule &Sg, float *fo = nullptr);
};
#endif