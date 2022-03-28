#include "CPathRelinking.h"
#include "../chm.utilities/CustomConsole.h"

#define CHECK_AND_SHOW_SCHEDULE_VALIDITY 0
#define CHECK_AND_PREVENT_INFINITE_CYCLE 1

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
void CPathRelinking::CheckDataConsistency(void)
{
	if (!CollectionJ || !CollectionM)
		throw std::exception("CPathRelinking::Relink(): before execute the path-relinking technique, you must define user data objects.");

	if (!localSearch)
		throw std::exception("CPathRelinking::Relink(): before execute the path-relinking technique, you must define a local search strategy.");

	if (!neighborhood)
		throw std::exception("CPathRelinking::Relink(): before execute the path-relinking technique, you must define a neighborhoob strategy.");
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
CPathRelinking::CPathRelinking(void)
{
	Graph = nullptr;
	CollectionJ = nullptr;
	CollectionM = nullptr;
	localSearch = nullptr;
	neighborhood = nullptr;
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
CPathRelinking::CPathRelinking(CLocalSearchStrategy *locals, CNeighborhoobStrategy *neighbor)
{
	if (!locals) throw std::exception("CPathRelinking::CPathRelinking(): parameter <locals> may not be null.");
	if (!neighbor) throw std::exception("CPathRelinking::CPathRelinking(): parameter <neighbor> may not be null.");

	Graph = nullptr;
	CollectionJ = nullptr;
	CollectionM = nullptr;
	localSearch = locals;
	neighborhood = neighbor;
	localSearch->NeighborStrategy(neighbor);
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
CPathRelinking::~CPathRelinking(void)
{
	if (Graph) delete Graph;
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
void CPathRelinking::LocalSearch(CLocalSearchStrategy *locals)
{
	localSearch = locals;
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
void CPathRelinking::Neighborhood(CNeighborhoobStrategy *neighbor)
{
	neighborhood = neighbor;
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
void CPathRelinking::UserData(CPieceCollection *cJ, CMachineCollection *cM)
{
	if (!cJ) throw std::exception("CPathRelinking::UserData(): parameter <cJ> may not be null.");
	if (!cM) throw std::exception("CPathRelinking::UserData(): parameter <cM> may not be null.");

	CollectionJ = cJ;
	CollectionM = cM;
	Rneighbor.UserData(cJ, cM);
	localSearch->UserData(cJ, cM);

	if (Graph) delete Graph;
	Graph = new CGraphCAPP(cJ->GetPieceCount(), cM->GetMachineCount(), 0, false);
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
CForward_PR::CForward_PR(void) : CPathRelinking()
{
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
CForward_PR::CForward_PR(CLocalSearchStrategy *locals, CNeighborhoobStrategy *neighbor)
	: CPathRelinking(locals, neighbor)
{
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
CForward_PR::~CForward_PR(void)
{
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
CSchedule CForward_PR::Relink(const CSchedule &Si, const CSchedule &Sg, float *fo)
{
	int step;
	int diff_max;
	float f, fstart;

	CheckDataConsistency();
	diff_max = Si.GetMachinesCount() + 1;

	S = Si;
	Sstart = S;
	//fstart = numeric_limits<float>::infinity();
	CollectionJ->Create_NxM_Graph(Graph, CollectionM, S);
	fstart = Graph->Makespan();
	Graph->ClearGraph();
	while ((step = Rneighbor.Moves(S, Sg)) >= 1)
	{
		S = Rneighbor.Argmin(&f);
		if (f < fstart)
		{
			fstart = f;
			Sstart = S;
		}

#if CHECK_AND_PREVENT_INFINITE_CYCLE
		//cout << endl << endl << "[" << step << "]" << endl << endl;
		// Condicion de control para detectar ciclos infinitos de la funcion Move().
		// Nota: tras comprobar que se Argmin() genera el 100% de las veces schedules 
		//	validos, esta comprobacion puede eliminarse. (10:20pm, 04-06-2019).
		if (diff_max < 0)
		{
			CustomConsole::ForegroundColor(ColorDarkGreen);
			std::cout	<< endl
								<< "S(i): " << Si << endl << endl
								<< "S(g): " << Sg << endl << endl;
			CustomConsole::LastState();
			throw std::exception("Infinite ciclye detected in CForward_PR::Relink(). Execution stoped");
		}
		diff_max--;
#endif
	} 

#if CHECK_AND_SHOW_SCHEDULE_VALIDITY
	if (!S.IsValidWithoutRecirc() || !Sprime.IsValidWithoutRecirc())
	{
		std::cout << "INSIDE FORWARD PATH RELINKING:" << endl << "Invalid schedule detected!!!" << endl << endl;
		std::cout << "[S]:" << endl << S << endl << endl << "[Sprime]: " << endl << Sprime << endl << endl << endl;
		system("pause");
		std::cout << endl;
	}
#endif
#if 0
	std:cout << endl << "OJO - Forward Path Relinking before internal LocalSearch" << endl << "Cmax: " << fprime << endl << Sprime << endl << endl;
#endif

	Sstart = localSearch->Execute(Sstart, &fstart);

	if (fo) *fo = fstart;
	return Sstart;
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
CBackward_PR::CBackward_PR(void) : CPathRelinking()
{
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
CBackward_PR::CBackward_PR(CLocalSearchStrategy *locals, CNeighborhoobStrategy *neighbor)
	: CPathRelinking(locals, neighbor)
{
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
CBackward_PR::~CBackward_PR(void)
{
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
CSchedule CBackward_PR::Relink(const CSchedule &Si, const CSchedule &Sg, float *fo)
{
	float f;
	CSchedule S;
	CForward_PR forward;

	CheckDataConsistency();
	forward.LocalSearch(localSearch);
	forward.Neighborhood(neighborhood);
	forward.UserData(CollectionJ, CollectionM);

	S = forward.Relink(Sg, Si, &f);

	if (fo) *fo = f;
	return S;
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
CBackAndForward_PR::CBackAndForward_PR(void) : CPathRelinking()
{
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
CBackAndForward_PR::CBackAndForward_PR(CLocalSearchStrategy *locals, CNeighborhoobStrategy *neighbor)
	: CPathRelinking(locals, neighbor)
{
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
CBackAndForward_PR::~CBackAndForward_PR(void)
{
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
CSchedule CBackAndForward_PR::Relink(const CSchedule &Si, const CSchedule &Sg, float *fo)
{
	float fF, fB;
	CSchedule SF, SB;
	CForward_PR forward;
	CBackward_PR backward;

	CheckDataConsistency();
	forward.LocalSearch(localSearch);
	forward.Neighborhood(neighborhood);
	forward.UserData(CollectionJ, CollectionM);
	backward.LocalSearch(localSearch);
	backward.Neighborhood(neighborhood);
	backward.UserData(CollectionJ, CollectionM);

	SB = backward.Relink(Si, Sg, &fB);
	SF = forward.Relink(Si, Sg, &fF);

	if (fB < fF)
	{
		if (fo) *fo = fB;
		return SB;
	}

	if (fo) *fo = fF;
	return SF;
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
CMixed_PR::CMixed_PR(void) : CPathRelinking()
{
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
CMixed_PR::CMixed_PR(CLocalSearchStrategy *locals, CNeighborhoobStrategy *neighbor)
	: CPathRelinking(locals, neighbor)
{
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
CMixed_PR::~CMixed_PR()
{
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
CSchedule CMixed_PR::Relink(const CSchedule &Si, const CSchedule &Sg, float *fo)
{
	int step;
	int diff_max;
	float f, fstart;
	CSchedule St, Sg2;

	CheckDataConsistency();
	diff_max = Si.GetMachinesCount() + 1;

	S = Si;
	Sg2 = Sg;
	Sstart = S;
	//fstart = numeric_limits<float>::infinity();
	CollectionJ->Create_NxM_Graph(Graph, CollectionM, S);
	fstart = Graph->Makespan();
	Graph->ClearGraph();
	while ((step = Rneighbor.Moves(S, Sg2)) >= 1)
	{
		S = Rneighbor.Argmin(&f);
		if (f < fstart)
		{
			fstart = f;
			Sstart = S;
		}
		St = S;
		S = Sg2;
		Sg2 = St;
	}

	Sstart = localSearch->Execute(Sstart, &fstart);

	if (fo) *fo = fstart;
	return Sstart;
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
CTruncated_PR::CTruncated_PR(void) : CPathRelinking()
{
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
CTruncated_PR::CTruncated_PR(CLocalSearchStrategy *locals, CNeighborhoobStrategy *neighbor)
	: CPathRelinking(locals, neighbor)
{
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
CTruncated_PR::~CTruncated_PR(void)
{
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
CSchedule CTruncated_PR::Relink(const CSchedule &Si, const CSchedule &Sg, float *fo)
{
	float f, fprime;

	if (fo) *fo = f;
	return Sstart;
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
CExternal_PR::CExternal_PR(void) : CPathRelinking()
{
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
CExternal_PR::CExternal_PR(CLocalSearchStrategy *locals, CNeighborhoobStrategy *neighbor)
	: CPathRelinking(locals, neighbor)
{
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
CExternal_PR::~CExternal_PR(void)
{
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
CSchedule CExternal_PR::Relink(const CSchedule &Si, const CSchedule &Sg, float *fo)
{
	float f, fprime;

	if (fo) *fo = f;
	return Sstart;
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
CRandomized_PR::CRandomized_PR(void) : CPathRelinking()
{
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
CRandomized_PR::CRandomized_PR(CLocalSearchStrategy *locals, CNeighborhoobStrategy *neighbor)
	: CPathRelinking(locals, neighbor)
{
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
CRandomized_PR::~CRandomized_PR(void)
{
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
CSchedule CRandomized_PR::Relink(const CSchedule &Si, const CSchedule &Sg, float *fo)
{
	float f, fprime;

	if (fo) *fo = f;
	return Sstart;
}
