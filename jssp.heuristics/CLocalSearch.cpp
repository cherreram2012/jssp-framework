#include <time.h>
#include <limits>
#include "CLocalSearch.h"

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
void CLocalSearchStrategy::CheckDataConsistency(void)
{
	if (!CollectionJ || !CollectionM || !Neighborhood)
		throw std::exception("LocalSearchStrategy::Execute(): before execute the local search procedure, you must define { a) neighborhood strategy object, b) user data objects. }");
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
CLocalSearchStrategy::CLocalSearchStrategy(void)
{
	Graph = nullptr;
	CollectionJ = nullptr;
	CollectionM = nullptr;
	Neighborhood = nullptr;
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
CLocalSearchStrategy::~CLocalSearchStrategy(void)
{
	if (Graph) delete Graph;
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
void CLocalSearchStrategy::NeighborStrategy(CNeighborhoobStrategy *neighbor)
{
	if (!neighbor) throw std::exception("CLocalSearch::CLocalSearch(): parameter <neighbors> may not be null.");

	Neighborhood = neighbor;
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
CLocalSearchIBUIT::CLocalSearchIBUIT(void)
{
	GraphPrime = nullptr;
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
CLocalSearchIBUIT::~CLocalSearchIBUIT(void)
{
	if (GraphPrime) delete GraphPrime;
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
void CLocalSearchStrategy::UserData(CPieceCollection *cJ, CMachineCollection *cM)
{
	if (!cJ) throw std::exception("CLocalSearch::CLocalSearch(): parameter <cJ> may not be null.");
	if (!cM) throw std::exception("CLocalSearch::CLocalSearch(): parameter <cM> may not be null.");

	CollectionJ = cJ;
	CollectionM = cM;

	if (Graph) delete Graph;
	Graph = new CGraphCAPP(cJ->GetPieceCount(), cM->GetMachineCount(), 0, false);	
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
void CLocalSearchIBUIT::UserData(CPieceCollection *cJ, CMachineCollection *cM)
{
	if (!cJ) throw std::exception("CLocalSearch::CLocalSearch(): parameter <cJ> may not be null.");
	if (!cM) throw std::exception("CLocalSearch::CLocalSearch(): parameter <cM> may not be null.");

	CollectionJ = cJ;
	CollectionM = cM;

	if (Graph) delete Graph;
	Graph = new CGraphCAPP(cJ->GetPieceCount(), cM->GetMachineCount(), 0, false);

	if (GraphPrime) delete GraphPrime;
	GraphPrime = new CGraphCAPP(cJ->GetPieceCount(), cM->GetMachineCount(), 0, true);
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
CFirstImprovingSearch::CFirstImprovingSearch(void) 
{
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
CFirstImprovingSearch::~CFirstImprovingSearch(void)
{
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
CSchedule CFirstImprovingSearch::Execute(const CSchedule &entry, float *fo)
{
	int card, last;
	float f, fprime;
	bool improved;
	CSchedule S, Sprime;

	CheckDataConsistency();

	S = entry;
	Neighborhood->SetEntryPoint(S);
	card = Neighborhood->Cardinality();

	CollectionJ->Create_NxM_Graph(Graph, CollectionM, S);
	f = Graph->Makespan();
	Graph->ClearGraph();

	last = -1;
	improved = true;
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
					Neighborhood->SetEntryPoint(S);
					last = i;
					i = 0;
				}
			}
		}
	}

	if (fo) *fo = f;
	return S;
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
CBestImprovingSearch::CBestImprovingSearch(void) 
{
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
CBestImprovingSearch::~CBestImprovingSearch(void)
{
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
CSchedule CBestImprovingSearch::Execute(const CSchedule &entry, float *fo)
{
	int card;
	float f, fprime, fbest;
	bool improved;
	CSchedule S, Sprime, Sbest;
	
	CheckDataConsistency();

	S = entry;
	Neighborhood->SetEntryPoint(S);
	card = Neighborhood->Cardinality();

	CollectionJ->Create_NxM_Graph(Graph, CollectionM, S);
	f = Graph->Makespan();
	Graph->ClearGraph();

	improved = true;
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
			}
		}

		if (fbest < f)
		{
			S = Sbest;
			f = fbest;
			improved = true;
			Neighborhood->SetEntryPoint(S);
		}
	}

	if (fo) *fo = f;
	return S;
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
CFirstImprovingIBUIT::CFirstImprovingIBUIT(void)
{
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
CFirstImprovingIBUIT::~CFirstImprovingIBUIT(void)
{
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
CSchedule CFirstImprovingIBUIT::Execute(const CSchedule &entry, float *fo)
{
	int card, last;
	float f, fprime;
	bool improved;
	CSchedule S, Sprime;

	CheckDataConsistency();

	S = entry;
	Neighborhood->SetEntryPoint(S);
	card = Neighborhood->Cardinality();

	CollectionJ->Create_NxM_Graph(Graph, CollectionM, S);
	f = Graph->Makespan();
	Graph->ClearGraph();

	/*CollectionJ->Create_NxM_Graph(GraphPrime, CollectionM, S);
	f = GraphPrime->Makespan();
	S = GraphPrime->GetSchedule();
	GraphPrime->ClearGraph();
	Neighborhood->SetEntryPoint(S);
	card = Neighborhood->Cardinality();*/

	last = -1;
	improved = true;
	while (improved)
	{
		improved = false;
		for (int i = 0; i < card && improved == false; i++)
		{
			if (i != last)
			{
				// run a neighbor evaluation .
				Sprime = Neighborhood->VisitNeighbor(i);
				CollectionJ->Create_NxM_Graph(GraphPrime, CollectionM, Sprime);
				fprime = GraphPrime->Makespan();
				Sprime = GraphPrime->GetSchedule();
				GraphPrime->ClearGraph();

				// updating to best neighbor  f(S') < f(S)
				if (fprime < f)
				{
					S = Sprime;
					f = fprime;
					improved = true;
					Neighborhood->SetEntryPoint(S);
					last = i;
					i = 0;
				}
			}
		}
	}

	if (fo) *fo = f;
	return S;
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
CBestImprovingIBUIT::CBestImprovingIBUIT(void)
{
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
CBestImprovingIBUIT::~CBestImprovingIBUIT(void)
{
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
CSchedule CBestImprovingIBUIT::Execute(const CSchedule &entry, float *fo)
{
	int card;
	float f, fprime, fbest;
	bool improved;
	CSchedule S, Sprime, Sbest;

	CheckDataConsistency();

	S = entry;
	Neighborhood->SetEntryPoint(S);
	card = Neighborhood->Cardinality();

	CollectionJ->Create_NxM_Graph(Graph, CollectionM, S);
	f = Graph->Makespan();
	Graph->ClearGraph();

	/*CollectionJ->Create_NxM_Graph(GraphPrime, CollectionM, S);
	f = GraphPrime->Makespan();
	S = GraphPrime->GetSchedule();
	GraphPrime->ClearGraph();
	Neighborhood->SetEntryPoint(S);
	card = Neighborhood->Cardinality();*/

	improved = true;
	while (improved)
	{
		improved = false;
		fbest = numeric_limits<float>::infinity();
		for (int i = 0; i < card; i++)
		{
			// run a neighbor evaluation.
			Sprime = Neighborhood->VisitNeighbor(i);
			CollectionJ->Create_NxM_Graph(GraphPrime, CollectionM, Sprime);
			fprime = GraphPrime->Makespan();
			Sprime = GraphPrime->GetSchedule();
			GraphPrime->ClearGraph();

			// updating to best neighbor  f(S') < f(S)
			if (fprime < fbest)
			{
				Sbest = Sprime;
				fbest = fprime;
			}
		}

		if (fbest < f)
		{
			S = Sbest;
			f = fbest;
			improved = true;
			Neighborhood->SetEntryPoint(S);
		}
	}

	if (fo) *fo = f;
	return S;
}