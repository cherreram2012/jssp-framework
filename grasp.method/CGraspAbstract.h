#pragma once
#ifndef GraspAbstract_H
#define GraspAbstract_H

#include "CTerminator.h"
#include "CStatistics.h"
#include "../jssp.model/CPieceCollection.h"
#include "../jssp.model/CMachineCollection.h"
#include "../jssp.heuristics/CGTAlgorithm.h"
#include "../jssp.heuristics/CLocalSearch.h"
#include "../jssp.heuristics/CNeighborhood.h"

//------------------------------------------------------------------------------
//	ClassName: GraspAbstract
//
//  Description: Represent an abstract implementation for a GRASP metaheuristic.
//
//  Revision: 16/05/2019 
//------------------------------------------------------------------------------
class CGraspAbstract
{
	protected:
		int iSeed;
		float fAlfa;
		CDataSet *Dataset;
		CGraphCAPP *Graph;
		CStatistics Stats;
		CTerminator *Terminator;
		CGTAlgorithmAlfaRCL *GT;
		CPieceCollection *CollectionJ;
		CMachineCollection *CollectionM;
		CLocalSearchStrategy *LocalSearch;
		CNeighborhoobStrategy *Neighborhood;

	protected:
		void CheckDataConsistency();

	public:
		//--- Constructor and destructor ---//
		CGraspAbstract (CLocalSearchStrategy *locals, CNeighborhoobStrategy *neighbor, CTerminator *term, float alfa);
		virtual ~CGraspAbstract (void);

		//--- Function virtual pure ---//
		virtual CSchedule Execute (float *fitness = nullptr) = 0;

		//--- Set functions ---//
		void SetSeed (int seed);

		//--- General functions ---//
		virtual void UserData (CPieceCollection *cJ, CMachineCollection *cM);
		CStatistics Statistics (void) const;
};
#endif