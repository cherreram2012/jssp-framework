#include <time.h>
#include <limits>
#include "CLocalSearchForExperiment.h"
#include "../chm.utilities/PrecisionClock.h"

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
void CLocalSearchForExperiment::InitializeStatistics(void)
{
	Stats.ElapsedTime = 0.0f;
	Stats.StepsCount = 0;
	Stats.NeighborVisited = 0;
	Stats.F_Historial.clear();
	Stats.S_Historial.clear();
	Stats.T_Historial.clear();
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
CLocalSearchForExperiment::CLocalSearchForExperiment(CNeighborhoobStrategy *neighbors, CPieceCollection *cJ, CMachineCollection *cM, bool save_historial)
{
	if (!cJ) throw std::exception("CLocalSearch::CLocalSearch(): parameter <cJ> may not be null.");
	if (!cM) throw std::exception("CLocalSearch::CLocalSearch(): parameter <cM> may not be null.");
	if (!neighbors) throw std::exception("CLocalSearch::CLocalSearch(): parameter <neighbors> may not be null.");
	
	CollectionJ  = cJ;
	CollectionM  = cM;
	Neighborhood = neighbors;
	SaveHistorial = save_historial;

	Graph = new CGraphCAPP(cJ->GetPieceCount(), cM->GetMachineCount(), 0, false);
	GraphIbuit = new CGraphCAPP(cJ->GetPieceCount(), cM->GetMachineCount(), 0, true);
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
CLocalSearchForExperiment::~CLocalSearchForExperiment()
{
	delete Graph;
	delete GraphIbuit;
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
void CLocalSearchForExperiment::SetNeighborStrategy(CNeighborhoobStrategy *neighbors)
{
	Neighborhood = neighbors;
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
CSchedule CLocalSearchForExperiment::FirstImproving(const CSchedule &entry, float *fo)
{
	double t;
	int card, last;
	float f, fprime;
	bool improved;
	CSchedule S, Sprime;
	LARGE_INTEGER start, end, curr;

	//InitializeStatistics();

	S = entry;
	Neighborhood->SetEntryPoint(S);
	card = Neighborhood->Cardinality();

	CollectionJ->Create_NxM_Graph(Graph, CollectionM, S);
	f = Graph->Makespan();
	Graph->ClearGraph();

	last = -1;
	improved = true;
	QueryPerformanceCounter(&start);
	while (improved)
	{
		improved = false;
		for (int i = 0; i < card && improved == false; i++)
		{
			if (i != last)
			{
				// run a neighbor evaluation .
				Sprime = Neighborhood->VisitNeighbor(i);
				CollectionJ->Create_NxM_Graph(Graph, CollectionM, Sprime);
				fprime = Graph->Makespan();
				Graph->ClearGraph();

				// updating to best neighbor  f(S') < f(S)
				if (fprime < f)
				{
					S = Sprime;
					f = fprime;
					improved = true;
					QueryPerformanceCounter(&curr);
					Neighborhood->SetEntryPoint(S);
					last = i;
					i = 0;
					Stats.StepsCount++;
					if (SaveHistorial)
					{
						t = performancecounter_diff(&start, &curr);
						Stats.F_Historial.push_back(f);
						Stats.T_Historial.push_back(t);
						Stats.S_Historial.push_back(S);
					}
				}
			}
		}
	}
	QueryPerformanceCounter(&end);
	Stats.ElapsedTime = performancecounter_diff(&start, &end);

	if (fo) *fo = f;
	return S;
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
CSchedule CLocalSearchForExperiment::BestImproving(const CSchedule &entry, float *fo)
{
	double t;
	int card;
	float f, fprime, fbest;
	bool improved;
	CSchedule S, Sprime, Sbest;
	LARGE_INTEGER start, end, curr;

	//InitializeStatistics();

	S = entry;
	Neighborhood->SetEntryPoint(S);
	card = Neighborhood->Cardinality();

	CollectionJ->Create_NxM_Graph(Graph, CollectionM, S);
	f = Graph->Makespan();
	Graph->ClearGraph();

	improved = true;
	QueryPerformanceCounter(&start);
	while (improved)
	{
		improved = false;
		fbest = numeric_limits<float>::infinity();
		for (int i = 0; i < card; i++)
		{
			// run a neighbor evaluation.
			Sprime = Neighborhood->VisitNeighbor(i);
			CollectionJ->Create_NxM_Graph(Graph, CollectionM, Sprime);
			fprime = Graph->Makespan();
			Graph->ClearGraph();

			// updating to best neighbor  f(S') < f(S)
			if (fprime < fbest)
			{
				Sbest = Sprime;
				fbest = fprime;
				Stats.NeighborVisited++;
			}
		}

		if (fbest < f)
		{
			S = Sbest;
			f = fbest;
			improved = true;
			QueryPerformanceCounter(&curr);
			Neighborhood->SetEntryPoint(S);
			Stats.StepsCount++;
			if (SaveHistorial)
			{
				t = performancecounter_diff(&start, &curr);
				Stats.F_Historial.push_back(f);
				Stats.T_Historial.push_back(t);
				Stats.S_Historial.push_back(S);
			}
		}
	}
	QueryPerformanceCounter(&end);
	Stats.ElapsedTime = performancecounter_diff(&start, &end);

	if (fo) *fo = f;
	return S;
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
CSchedule CLocalSearchForExperiment::FirstImproving_IBUIT(const CSchedule &entry, float *fo)
{
	double t;
	int card, last;
	float f, fprime;
	bool improved;
	CSchedule S, Sprime;
	LARGE_INTEGER start, end, curr;

	//InitializeStatistics();

	S = entry;
	Neighborhood->SetEntryPoint(S);
	card = Neighborhood->Cardinality();

	CollectionJ->Create_NxM_Graph(Graph, CollectionM, S);
	f = Graph->Makespan();
	Graph->ClearGraph();

	last = -1;
	improved = true;
	QueryPerformanceCounter(&start);
	while (improved)
	{
		improved = false;
		for (int i = 0; i < card && improved == false; i++)
		{
			if (i != last)
			{
				// run a neighbor evaluation .
				Sprime = Neighborhood->VisitNeighbor(i);
				//CollectionJ->Create_NxM_Graph(Graph, CollectionM, Sprime);
				//fprime = Graph->Makespan();
				//Graph->ClearGraph();
				CollectionJ->Create_NxM_Graph(GraphIbuit, CollectionM, Sprime);
				fprime = GraphIbuit->Makespan();
				Sprime = GraphIbuit->GetSchedule();
				GraphIbuit->ClearGraph();

				// updating to best neighbor  f(S') < f(S)
				if (fprime < f)
				{
					S = Sprime;
					f = fprime;
					improved = true;
					QueryPerformanceCounter(&curr);
					Neighborhood->SetEntryPoint(S);
					last = i;
					i = 0;
					Stats.StepsCount++;
					if (SaveHistorial)
					{
						t = performancecounter_diff(&start, &curr);
						Stats.F_Historial.push_back(f);
						Stats.T_Historial.push_back(t);
						Stats.S_Historial.push_back(S);
					}
				}
			}
		}
	}
	QueryPerformanceCounter(&end);
	Stats.ElapsedTime = performancecounter_diff(&start, &end);

	if (fo) *fo = f;
	return S;
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
CSchedule CLocalSearchForExperiment::BestImproving_IBUIT(const CSchedule &entry, float *fo)
{
	double t;
	int card;
	float f, fprime, fbest;
	bool improved;
	CSchedule S, Sprime, Sbest;
	LARGE_INTEGER start, end, curr;

	//InitializeStatistics();

	S = entry;
	Neighborhood->SetEntryPoint(S);
	card = Neighborhood->Cardinality();

	CollectionJ->Create_NxM_Graph(Graph, CollectionM, S);
	f = Graph->Makespan();
	Graph->ClearGraph();

	improved = true;
	QueryPerformanceCounter(&start);
	while (improved)
	{
		improved = false;
		fbest = numeric_limits<float>::infinity();
		for (int i = 0; i < card; i++)
		{
			// run a neighbor evaluation.
			Sprime = Neighborhood->VisitNeighbor(i);
			//CollectionJ->Create_NxM_Graph(Graph, CollectionM, Sprime);
			//fprime = Graph->Makespan();
			//Graph->ClearGraph();
			CollectionJ->Create_NxM_Graph(GraphIbuit, CollectionM, Sprime);
			fprime = GraphIbuit->Makespan();
			Sprime = GraphIbuit->GetSchedule();
			GraphIbuit->ClearGraph();

			// updating to best neighbor  f(S') < f(S)
			if (fprime < fbest)
			{
				Sbest = Sprime;
				fbest = fprime;
				Stats.NeighborVisited++;
			}
		}

		if (fbest < f)
		{
			S = Sbest;
			f = fbest;
			improved = true;
			QueryPerformanceCounter(&curr);
			Neighborhood->SetEntryPoint(S);
			Stats.StepsCount++;
			if (SaveHistorial)
			{
				t = performancecounter_diff(&start, &curr);
				Stats.F_Historial.push_back(f);
				Stats.T_Historial.push_back(t);
				Stats.S_Historial.push_back(S);
			}
		}
	}
	QueryPerformanceCounter(&end);
	Stats.ElapsedTime = performancecounter_diff(&start, &end);

	if (fo) *fo = f;
	return S;
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
CLocalSearchForExperiment::LOCAL_SEARCH_STATISTCS CLocalSearchForExperiment::Statistics(void) const
{
	return Stats;
}