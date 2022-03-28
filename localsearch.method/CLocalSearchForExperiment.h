#pragma once
#ifndef CLocalSearchForExperiment_H
#define CLocalSearchForExperiment_H

#include <vector>
#include "../jssp.model/CSchedule.h"
#include "../jssp.model/CPieceCollection.h"
#include "../jssp.model/CMachineCollection.h"
#include "../jssp.heuristics/CNeighborhood.h"

enum LocalSearchType { FIRST_IMPROVING, BEST_IMPROVING };

//------------------------------------------------------------------------------
//	ClassName: CLocalSearch
//
//  Description: First implementations for a Local Search Strategies.
//
//  Revision: 10/05/2019 
//------------------------------------------------------------------------------
class CLocalSearchForExperiment {
public:
	typedef struct Statistics {
		double ElapsedTime;
		unsigned int StepsCount;
		unsigned int NeighborVisited;
		vector<float> F_Historial;
		vector<double> T_Historial;
		vector<CSchedule> S_Historial;

		struct Statistics()
		{
			ElapsedTime = 0.0f;
			StepsCount = 0;
			NeighborVisited = 0;
		}
	} LOCAL_SEARCH_STATISTCS;

	private:
		bool SaveHistorial;
		CGraphCAPP *Graph;
		CGraphCAPP *GraphIbuit;
		LOCAL_SEARCH_STATISTCS Stats;
		CNeighborhoobStrategy *Neighborhood;
		CPieceCollection *CollectionJ;
		CMachineCollection *CollectionM;

	private:
		void InitializeStatistics ( void );

	public:
		//--- Constructor and destructor ---//
		CLocalSearchForExperiment(CNeighborhoobStrategy *neighbors, CPieceCollection *cJ, CMachineCollection *cM, bool save_historial = false);
		~CLocalSearchForExperiment();

		//--- Set Functions ---//
		void SetNeighborStrategy ( CNeighborhoobStrategy *neighbors );

		//--- Search Strategies ---//
		CSchedule FirstImproving ( const CSchedule &entry, float *fo = nullptr );
		CSchedule BestImproving  ( const CSchedule &entry, float *fo = nullptr );

		CSchedule FirstImproving_IBUIT(const CSchedule &entry, float *fo = nullptr);
		CSchedule BestImproving_IBUIT(const CSchedule &entry, float *fo = nullptr);

		//--- General Purpose Functions ---//
		LOCAL_SEARCH_STATISTCS Statistics ( void ) const;
};

#endif