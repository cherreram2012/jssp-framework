#include <limits.h>
#include "CRestrictedNeighbor.h"
#include "../mersenne_twister/mt19937ar.h"
#include "../chm.utilities/CustomConsole.h"

#define RANDOM_SENSE_FIX 1
#define APPLIED_FORWARD_FIX 1
#define CHECK_AND_SHOW_SCHEDULE_VALIDITY 0

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
void RestrictedNeighbor::CheckDataConsistency(void)
{
	if (!CollectionJ || !CollectionM)
		throw std::exception("RestrictedNeighborhood::Argmin(): before execute the argmin procedure, you must define { a) user data objects. }");
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
CSchedule RestrictedNeighbor::ReplaceMachineSet(const CSchedule &S, int m_replace, vector<CSetOperator::DUPLEX_ATOM> &vdiff)
{
	CSchedule rS = S;	
	vector<CSetOperator::DUPLEX_ATOM>::iterator itV;

	// Phase 1: Atomos de S(g) reemplazan los atomos de S(i) para que S(i) este un paso mas cerca de S(g).
	for (itV = vdiff.begin(); itV != vdiff.end(); itV++) rS.SequenceId[(*itV).index] = (*itV).idS2;

	// Phase 2: 
#if RANDOM_SENSE_FIX
	bool flipcoin = (genrand_int32() % 2 == 0) ? true : false;

	if (flipcoin)
		ForwardFixInviability(&rS, m_replace, vdiff);
	else
		BackwardFixInviability(&rS, m_replace, vdiff);
#else
	#if APPLIED_FORWARD_FIX
		ForwardFixInviability(&rS, m_replace, vdiff);
	#else
		BackwardFixInviability(&rS, m_replace, vdiff);
	#endif
#endif	

	return rS;
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
void RestrictedNeighbor::ForwardFixInviability(CSchedule *S, int m_except, vector<CSetOperator::DUPLEX_ATOM> &vdiff)
{
	int i, equals;
	int *opCounter;
	vector<int> overplus;
	vector<int>::iterator itP;
	vector<CSetOperator::DUPLEX_ATOM>::iterator itV;
	map<int, vector<CSetOperator::DUPLEX_ATOM>>::iterator itM;

	opCounter = new int[S->iJobs + 1]{ 0 };

	//--------
	// Paso 1: Computo la cantidad de operaciones en los m-conjunto iguales.
	for (int j = 0; j < S->iJobs; j++)
	{
		i = m_except * S->iJobs + j;
		opCounter[S->SequenceId[i]]++;
	}

	equals = S->iMachines - SymDiff.size();
	for (int m = 0; m < S->iMachines && equals > 0; m++)
	{
		if (m == m_except) continue;

		// Si no existe en el diccionario una entrada con key igual a <m>
		// es porque estoy en un M-conjunto que pertenece a las igualdades.
		if (SymDiff.find(m) == SymDiff.end())
		{
			for (int j = 0; j < S->iJobs; j++)
			{
				i = m * S->iJobs + j;
				opCounter[S->SequenceId[i]]++;
			}
			equals--;
		}
	}

	//--------
	// Paso 2: Computo la cantidad de operaciones en los m-conjunto distintos 
	// (en sentido left-right).
	for (itM = SymDiff.begin(); itM != SymDiff.end(); itM++)
	{
		if ((*itM).first == m_except) continue;

		for (int j = 0; j < S->iJobs; j++)
		{
			i = (*itM).first * S->iJobs + j;

			// ¿Tiene el job actual todas tus M-ocurrencias?
			if (opCounter[S->SequenceId[i]] == S->iMachines)
				overplus.push_back(i);
			else
				opCounter[S->SequenceId[i]]++;
		}
	}

	//--------
	// Paso 3: Reconstruyo el schedule reemplazando los atomos que salieron de S(i)
	// producto a la asignacion del M-conjunto proveniente de S(g) en aquellos lugares
	// donde <S->SequenceId> se torna invalido.
	itV = vdiff.begin();
	itP = overplus.begin();
	while (itV != vdiff.end() && itP != overplus.end())
	{
		if (opCounter[(*itV).idS1] < S->iMachines)
		{
			S->SequenceId[(*itP)] = (*itV).idS1;
			opCounter[(*itV).idS1]++;
			itV++;
			itP++;
		}
		else
			itV++;
	}

	delete []opCounter;
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
void RestrictedNeighbor::BackwardFixInviability(CSchedule *S, int m_except, vector<CSetOperator::DUPLEX_ATOM> &vdiff)
{
	int i, equals;
	int *opCounter;
	vector<int> overplus;
	vector<int>::iterator itP;
	vector<CSetOperator::DUPLEX_ATOM>::iterator itV;
	map<int, vector<CSetOperator::DUPLEX_ATOM>>::reverse_iterator itM;

	opCounter = new int[S->iJobs + 1]{ 0 };

	//--------
	// Paso 1: Computo la cantidad de operaciones en los m-conjunto iguales.
	for (int j = 0; j < S->iJobs; j++)
	{
		i = m_except * S->iJobs + j;
		opCounter[S->SequenceId[i]]++;
	}

	equals = S->iMachines - SymDiff.size();
	for (int m = 0; m < S->iMachines && equals > 0; m++)
	{
		if (m == m_except) continue;

		// Si no existe en el diccionario una entrada con key igual a <m>
		// es porque estoy en un M-conjunto que pertenece a las igualdades.
		if (SymDiff.find(m) == SymDiff.end())
		{
			for (int j = 0; j < S->iJobs; j++)
			{
				i = m * S->iJobs + j;
				opCounter[S->SequenceId[i]]++;
			}
			equals--;
		}
	}

	//--------
	// Paso 2: Computo la cantidad de operaciones en los m-conjunto distintos.
	// (en sentido right-left).
	for (itM = SymDiff.rbegin(); itM != SymDiff.rend(); itM++)
	{
		if ((*itM).first == m_except) continue;

		for (int j = S->iJobs - 1; j >= 0; j--)
		{
			i = (*itM).first * S->iJobs + j;

			// ¿Tiene el job actual todas tus M-ocurrencias?
			if (opCounter[S->SequenceId[i]] == S->iMachines)
				overplus.push_back(i);
			else
				opCounter[S->SequenceId[i]]++;
		}
	}

	//--------
	// Paso 3: Reconstruyo el schedule reemplazando los atomos que salieron de S(i)
	// producto a la asignacion del M-conjunto proveniente de S(g) en aquellos lugares
	// donde <S->SequenceId> se torna invalido.
	itV = vdiff.begin();
	itP = overplus.begin();
	while (itV != vdiff.end() && itP != overplus.end())
	{
		if (opCounter[(*itV).idS1] < S->iMachines)
		{
			S->SequenceId[(*itP)] = (*itV).idS1;
			opCounter[(*itV).idS1]++;
			itV++;
			itP++;
		}
		else
			itV++;
	}

	delete[]opCounter;
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
RestrictedNeighbor::RestrictedNeighbor()
{
	Graph = nullptr;
	CollectionJ = nullptr;
	CollectionM = nullptr;
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
RestrictedNeighbor::~RestrictedNeighbor(void)
{
	if (Graph) delete Graph;
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
CSchedule RestrictedNeighbor::Argmin(float *fo)
{
	float f, fprime;	
	CSchedule S, Sprime;	
	map<int, vector<CSetOperator::DUPLEX_ATOM>>::iterator itM;

	CheckDataConsistency();
	
	fprime = numeric_limits<float>::infinity();
	for (itM = SymDiff.begin(); itM != SymDiff.end(); itM++)
	{
		S = ReplaceMachineSet(Sinitial, itM->first, itM->second);
		CollectionJ->Create_NxM_Graph(Graph, CollectionM, S);
		f = Graph->Makespan();
		Graph->ClearGraph();

		if (f < fprime)
		{
			fprime = f;
			Sprime = S;
		}

#if CHECK_AND_SHOW_SCHEDULE_VALIDITY
		if (S.IsValid())
			std::cout << "[" << itM->first << "] -> cmax: " << f << endl << S << endl << endl;
		else
		//if (!S.IsValidWithoutRecirc())
		{
			std::cout << "---------------------------------" << endl
				<< "OJO. Schedule invalido en Argmin." << endl
				<< "m: [" << itM->first << "]" << endl
				<< "S-i: " << Sinitial << endl << endl
				<< "S-g: " << Sguiding << endl << endl;
			CustomConsole::ForegroundColor(ColorRed);
			std::cout << "S-p: " << S << endl;
			CustomConsole::LastState();
			std::cout << "---------------------------------" << endl << endl;
			system("pause");
		}
#endif
	}

	if (fo) *fo = fprime;
	return Sprime;
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
int	RestrictedNeighbor::Moves(const CSchedule &Si, const CSchedule &Sg)
{
	Sinitial = Si;
	Sguiding = Sg;
	SymDiff = CSetOperator::SymmetricDiff(Sinitial, Sg);

	return SymDiff.size();
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
void RestrictedNeighbor::UserData(CPieceCollection *cJ, CMachineCollection *cM)
{
	if (!cJ) throw std::exception("CLocalSearch::CLocalSearch(): parameter <cJ> may not be null.");
	if (!cM) throw std::exception("CLocalSearch::CLocalSearch(): parameter <cM> may not be null.");

	CollectionJ = cJ;
	CollectionM = cM;

	if (Graph) delete Graph;
	Graph = new CGraphCAPP(cJ->GetPieceCount(), cM->GetMachineCount(), 0, false);
}