#include <limits>
#include <conio.h>
#include "CGTAlgorithm.h"
#include "../mersenne_twister/mt19937ar.h"
#include "../chm.utilities/CustomConsole.h"

using namespace std;

//==================================================================================//
//																																									//
//												Class GTAlgorithmBase definition													//	
//																																									//
//==================================================================================//
//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
CGTAlgorithmBase::ATOM_SCHEDULE_LIST *CGTAlgorithmBase::create__C(void)
{
	int m;
	float t;
	ATOM_SCHEDULE_LIST_PTR hptr, nptr;

	hptr = nullptr;
	for (int i = 0; i < iJobs; i++)
	{		
		Dataset->GetOperationByIndex(i, 0, &m, &t);
		nptr = new ATOM_SCHEDULE_LIST();
		// actualizacion de los campos.
		nptr->Atom.j = i+1;
		nptr->Atom.m = m;
		nptr->Atom.oj = 0;
		nptr->Atom.pj = t;
		nptr->Atom.rj = .0f;
		nptr->Atom.cj = .0f;
		// paso de doblemente enlace.
		nptr->Next = hptr;
		if (hptr) hptr->Prev = nptr;		
		hptr = nptr;
	}
	
	return hptr;
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
CGTAlgorithmBase::ATOM_SCHEDULE CGTAlgorithmBase::calculate__tCmin(ATOM_SCHEDULE_LIST_PTR *C, enum SolveConflict criteria)
{
	float rx;
	bool flipcoin, semiGreedy;
	ATOM_SCHEDULE minor;
	ATOM_SCHEDULE_LIST_PTR cptr = *C;

	semiGreedy = (criteria == RANDOM || criteria == SPT_RANDOM || criteria == SEP_RANDOM || criteria == ECT_RANDOM) ? true : false;

	if (cptr)
	{
		// Decision GREDDY (parecia un error, pero no lo es). Puede usarse para realizar algun estudio.
		//cptr->Atom.cj = cptr->Atom.rj + cptr->Atom.pj;

		// Decision GREEDY-ADAPTATIVE
		rx = (fAcumM[cptr->Atom.m] > cptr->Atom.rj) ? fAcumM[cptr->Atom.m] : cptr->Atom.rj;
		cptr->Atom.cj = rx + cptr->Atom.pj;		
		minor = cptr->Atom;
		cptr = cptr->Next;
	} 

	while (cptr)
	{
		// Decision GREDDY (parecia un error, pero no lo es). Puede usarse para realizar algun estudio.
		//cptr->Atom.cj = cptr->Atom.rj + cptr->Atom.pj;

		// Decision GREEDY-ADAPTATIVE
		rx = (fAcumM[cptr->Atom.m] > cptr->Atom.rj) ? fAcumM[cptr->Atom.m] : cptr->Atom.rj;
		cptr->Atom.cj = rx + cptr->Atom.pj;
		if (cptr->Atom.cj < minor.cj) {
			minor = cptr->Atom;
		}
		else if (semiGreedy && cptr->Atom.cj == minor.cj) {
			// Incorporacion de Decision SEMI-GREEDY
			flipcoin = (genrand_int32() % 2 == 0) ? true : false;
			if (flipcoin) minor = cptr->Atom;
		}

		cptr = cptr->Next;
	}

	return minor;
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
int CGTAlgorithmBase::obtain__G(ATOM_SCHEDULE_LIST_PTR G[], ATOM_SCHEDULE_LIST_PTR C, int machine, enum SolveConflict criteria,
															 int *op_spt, int *op_sep, int *op_ect)
//int GTAlgorithmBase::obtain__G(ATOM_SCHEDULE_LIST_PTR G[], ATOM_SCHEDULE_LIST_PTR C, ATOM_SCHEDULE m_star, enum SolveConflict criteria,
//	int *op_spt, int *op_sep, int *op_ect)
{
	bool flipcoin;
	int index = 0;
	ATOM_SCHEDULE_LIST_PTR spt, sep, ect, cptr = C;

	spt = sep = ect = nullptr;
	while (cptr)
	{
		if (cptr->Atom.m == machine)
		//if (cptr->Atom.m == m_star.m && cptr->Atom.rj < m_star.cj)
		{		
			flipcoin = (genrand_int32() % 2 == 0) ? true : false;

			//-- Filtro el indice para la mejor SPT rule. --//
			if (!spt)
			{
				spt = cptr;
				*op_spt = index;
			}
			else if (cptr->Atom.pj < spt->Atom.pj)
			{
				spt = cptr;
				*op_spt = index;
			}
			else if (cptr->Atom.pj == spt->Atom.pj && criteria == SPT_RANDOM && flipcoin)
			{
				spt = cptr;
				*op_spt = index;
			}
			//-- Filtro el indice para la mejor SEP rule. --//
			if (!sep)
			{
				sep = cptr;
				*op_sep = index;
			}
			//else if ((fAcumM[cptr->Atom.m] + cptr->Atom.rj) < (fAcumM[sep->Atom.m] + sep->Atom.rj))
			else if (cptr->Atom.rj < sep->Atom.rj)
			{
				sep = cptr;
				*op_sep = index;
			}
			//else if ((fAcumM[cptr->Atom.m] + cptr->Atom.rj) == (fAcumM[sep->Atom.m] + sep->Atom.rj) && criteria == SEP_RANDOMIZED && flipcoin)
			else if (cptr->Atom.rj == sep->Atom.rj && criteria == SEP_RANDOM && flipcoin)
			{
				sep = cptr;
				*op_sep = index;
			}
			//-- Filtro el indice para la mejor ECT rule. --//
			if (!ect)
			{
				ect = cptr;
				*op_ect = index;
			}
			//else if ((fAcumM[cptr->Atom.m] + cptr->Atom.cj) < (fAcumM[ect->Atom.m] + ect->Atom.cj))
			else if (cptr->Atom.cj < ect->Atom.cj)
			{
				ect = cptr;
				*op_ect = index;
			}
			//else if ((fAcumM[cptr->Atom.m] + cptr->Atom.cj) == (fAcumM[ect->Atom.m] + ect->Atom.cj) && criteria == ECT_RANDOMIZED && flipcoin)
			else if (cptr->Atom.cj == ect->Atom.cj && criteria == ECT_RANDOM && flipcoin)
			{
				ect = cptr;
				*op_ect = index;
			}
			
			G[index] = cptr;
			index++;
		}
		cptr = cptr->Next;
	}

	return index;
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
void CGTAlgorithmBase::deletefrom__C(ATOM_SCHEDULE_LIST_PTR *C, ATOM_SCHEDULE_LIST_PTR g)
{
	ATOM_SCHEDULE_LIST_PTR dptr = g;

	if (g == nullptr) return;

	// Caso 1: <g> apunta a head, y la lista solo contiene un nodo. 
	if (g->Prev == nullptr && g->Next == nullptr)
	{
		*C = nullptr;
	}
	// Caso 2: <g> apunta a head, y la lista contiene más de un nodo.
	else if (g->Prev == nullptr && g->Next != nullptr)
	{
		*C = g->Next;
		g->Next->Prev = nullptr;
	}
	// Caso 3: <g> apunta al ultimo nodo de la lista.
	else if (g->Prev != nullptr && g->Next == nullptr)
	{
		g->Prev->Next = nullptr;
	}
	// Caso 4: <g> apunta a nodo intermedio.
	else
	{
		g->Prev->Next = g->Next;
		g->Next->Prev = g->Prev;
	}

	delete dptr;
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
CGTAlgorithmBase::CGTAlgorithmBase(const CDataSet &dset)
{
	Dataset   = &dset;	
	iJobs     = dset.GetJobsCount();
	iRecirc   = dset.GetRecircCount();
	iMachines = dset.GetMachinesCount();
	init_genrand(1);
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
CGTAlgorithmBase::~CGTAlgorithmBase(void) 
{
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
void CGTAlgorithmBase::SetSeed(int seed)
{
	init_genrand(seed);
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
CGTAlgorithm_DRules::CGTAlgorithm_DRules(const CDataSet &dset) :
	CGTAlgorithmBase(dset) 
{ 
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
CGTAlgorithm_DRules::~CGTAlgorithm_DRules(void)
{
	CGTAlgorithmBase::~CGTAlgorithmBase();
}

//----------------------------------------------------------------------------------
// Implement a original G&T Algorithm propused by Gilffer and Thompson.
//----------------------------------------------------------------------------------
CSchedule CGTAlgorithm_DRules::GenerateSchedule(enum SolveConflict criteria)
{
	float t, cj;
	int j, m, op, succ, size_G, spt, sep, ect;
	ATOM_SCHEDULE m_star;
	ATOM_SCHEDULE_LIST_PTR conj_C, *conj_G;
	CSchedule schedule(NxM_SCHEDULE, iJobs, iMachines, iRecirc);
	
	fAcumM = new float[iMachines] {0};
	conj_G = new ATOM_SCHEDULE_LIST_PTR[iJobs];

	//--------
	// Step-1. Let C contain the first schedulable operation of each job.
	//				 Let rjm = 0 for all operation (j, m) in C ==> [rjm is the ready time (earliest start time) for de operation (j, m)].
	conj_C = create__C();
	do
	{
		//--------
		// Step-2. Calculate t(C) = min(j,m) e C {rjm + pjm} ==> [pjm is the processing time of operation (j,m)].
		//				 Let m* denote the machine on which the minimun is achieved.
		m_star = calculate__tCmin(&conj_C, criteria);

		//--------
		// Step-3. Let G denote the conflict set of all operations (j, m*) on machine m* such that rjm* < t(C).
		size_G = obtain__G(conj_G, conj_C, m_star.m, criteria, &spt, &sep, &ect);
		//size_G = obtain__G(conj_G, conj_C, m_star, criteria, &spt, &sep, &ect);

		//--------
		// Step-4. Select one operation g from G using some criteria and schedule it.
		switch (criteria)
		{
			/* Randomized */
			case RANDOM:
				op = genrand_int32() % size_G;
			break;
			/* Greedy & Semi-Greedy => (Shortest Processing Time) */
			case SPT_GREEDY: case SPT_RANDOM:
				op = spt;	
			break;
			/* Greedy & Semi-Greedy => (Start as Early as Possible) */
			case SEP_GREEDY: case SEP_RANDOM:
				op = sep;	
			break;
			/* Greedy & Semi-Greedy => (Earlist Completion Time) */
			case ECT_GREEDY: case ECT_RANDOM:
				op = ect;	
			break;			
			/* Error */
			default:
				throw std::exception("GTAlgorithmOriginal::GenerateSchedule(): <criteria> parameter only supports the values {SPT, SEP, ECT, SPT_RANDOMIZED, SEP_RANDOMIZED, ECT_RANDOMIZED, RANDOMIZED, ALFA}.");
		}
		schedule.AddToSchedule(conj_G[op]->Atom.j);
		
#if 0 //---TMP-------------------------------------------------------------------------------------------------
		ATOM_SCHEDULE_LIST_PTR cptr = conj_C;
		std::cout << "-------------------------" << std::endl 
							<< "simbol: j|m (pj)(rj)(cj)"  << std::endl
							<< "-------------------------" << std::endl;

		std::cout << "m-Acum: "; for (int i = 0; i < iMachines; i++) std::cout << "m" << i << ":" << fAcumM[i] << "  ";
		std::cout << std::endl << std::endl << "C = ";
		while (cptr)
		{
			std::cout << "j:" << cptr->Atom.j << "|m:" << cptr->Atom.m 
				<< " t(" << cptr->Atom.pj << ")" 
				<< "r(" << cptr->Atom.rj << ")"
				<< "c(" << cptr->Atom.cj << ")  ";
			cptr = cptr->Next;
		}		
		CustomConsole::ForegroundColor(ColorGreen);
		std::cout << std::endl << std::endl << "M* = " << "j:" << m_star.j << "|m:" << m_star.m 
			<< " t(" << m_star.pj << ")" 
			<< "r(" << m_star.rj << ")"
			<< "c(" << m_star.cj << ")";
		CustomConsole::ForegroundColor(ColorDarkYellow);
		std::cout << std::endl << std::endl << "G = ";
		for (int i = 0; i < size_G; i++) 
			std::cout << "j:" << conj_G[i]->Atom.j << "|m:" << conj_G[i]->Atom.m 
			<< " t(" << conj_G[i]->Atom.pj << ")"
			<< "r(" << conj_G[i]->Atom.rj << ")"
			<< "c(" << conj_G[i]->Atom.cj << ")  ";
		CustomConsole::Default();
		CustomConsole::ForegroundColor(ColorRed);
		std::cout << std::endl << std::endl << "choice in G: " << "j:" << conj_G[op]->Atom.j << "|m:" << conj_G[op]->Atom.m 
			<< " t(" << conj_G[op]->Atom.pj << ")"
			<< "r(" << conj_G[op]->Atom.rj << ")"
			<< "c(" << conj_G[op]->Atom.cj << ")";
		CustomConsole::LastState();
		std::cout << std::endl << std::endl << schedule << std::endl << std::endl << std::endl;
#endif //-----------------------------------------------------------------------------------------------------------------
		
		fAcumM[conj_G[op]->Atom.m] += conj_G[op]->Atom.pj; // OJO-NOTA: pertenece al Paso-4 pero tengo que ponerlo provisional luego de imprimir, para poder debuguear

		//--------
		// Step-5. Delete the operation g from C and include (prepare) its immediate successor in C.
		j = conj_G[op]->Atom.j;
		cj = conj_G[op]->Atom.cj;
		succ = conj_G[op]->Atom.oj + 1;
		deletefrom__C(&conj_C, conj_G[op]);

		//--------
		// Step-6. Update rjm in C and return to Step-2 until all operations are scheduled.		
		if (Dataset->GetOperationByIndex(j-1, succ, &m, &t))
		{
			ATOM_SCHEDULE_LIST_PTR nptr;

			nptr = new ATOM_SCHEDULE_LIST();
			// actualizacion de los campos.
			nptr->Atom.j = j;
			nptr->Atom.m = m;
			nptr->Atom.oj = succ;
			nptr->Atom.pj = t;
			nptr->Atom.rj = cj;
			nptr->Atom.cj = .0f;
			// paso de doblemente enlace.
			nptr->Next = conj_C;
			if (conj_C) conj_C->Prev = nptr;
			conj_C = nptr;
		}
	}
	while (conj_C);

	delete []conj_G;
	delete []fAcumM;
	return schedule;
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
CGTAlgorithmAlfaRCL::CGTAlgorithmAlfaRCL(const CDataSet &dset, float alfa) :
	CGTAlgorithmBase(dset)
{
	Alfa = alfa;
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
CGTAlgorithmAlfaRCL::~CGTAlgorithmAlfaRCL(void)
{
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
int	CGTAlgorithmAlfaRCL::obtain__G_with_cost(ATOM_SCHEDULE_LIST_PTR G[], ATOM_SCHEDULE_LIST_PTR C, int machine, enum SolveConflict criteria,
																					  float *customin, float *customax)
//int	GTAlgorithmAlfaRCL::obtain__G_with_cost(ATOM_SCHEDULE_LIST_PTR G[], ATOM_SCHEDULE_LIST_PTR C, ATOM_SCHEDULE m_star, enum SolveConflict criteria,
//																						float *customin, float *customax)
{
	int index = 0;
	ATOM_SCHEDULE_LIST_PTR cptr = C;
	
	*customax = 0;
	*customin = numeric_limits<float>::infinity();
	while (cptr)
	{
		if (cptr->Atom.m == machine)
		//if (cptr->Atom.m == m_star.m && cptr->Atom.rj < m_star.cj)
		{
			switch (criteria)
			{
				case SPT_GREEDY:
					if (cptr->Atom.pj < *customin) *customin = cptr->Atom.pj;
					if (cptr->Atom.pj > *customax) *customax = cptr->Atom.pj;
				break;

				case SEP_GREEDY:
					if (cptr->Atom.rj < *customin) *customin = cptr->Atom.rj;
					if (cptr->Atom.rj > *customax) *customax = cptr->Atom.rj;
				break;

				case ECT_GREEDY: 
					if (cptr->Atom.cj < *customin) *customin = cptr->Atom.cj;
					if (cptr->Atom.cj > *customax) *customax = cptr->Atom.cj;
				break;
			}

			G[index] = cptr;
			index++;
		}
		cptr = cptr->Next;
	}

	return index;
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
int CGTAlgorithmAlfaRCL::reduce__G_rcl(ATOM_SCHEDULE_LIST_PTR rG[], const ATOM_SCHEDULE_LIST_PTR G[], int size_G, float customin, float customax, enum SolveConflict criteria)
{
	int index = 0;
	float min, max;

	min = customin;
	max = customin + Alfa * (customax - customin);
	for (int i = 0; i < size_G; i++)
	{
		switch (criteria)
		{
			case SPT_GREEDY: 
				if (G[i]->Atom.pj >= min && G[i]->Atom.pj <= max)
				{
					rG[index] = G[i];
					index++;
				}
			break;
			
			case SEP_GREEDY: 
				if (G[i]->Atom.rj >= min && G[i]->Atom.rj <= max)
				{
					rG[index] = G[i];
					index++;
				}
			break;

			case ECT_GREEDY:
				if (G[i]->Atom.cj >= min && G[i]->Atom.cj <= max)
				{
					rG[index] = G[i];
					index++;
				}
			break;
		}
	}

	return index;
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
CSchedule CGTAlgorithmAlfaRCL::GenerateSchedule(enum SolveConflict criteria)
{
	int j, m, op, succ, size_G, size_rG;
	float t, cj, custoMin, custoMax;
	ATOM_SCHEDULE m_star;
	ATOM_SCHEDULE_LIST_PTR conj_C, *conj_G, *conj_rG;
	CSchedule schedule(NxM_SCHEDULE, iJobs, iMachines, iRecirc);

	if (criteria != SPT_GREEDY && criteria != SEP_GREEDY && criteria != ECT_GREEDY)
		throw std::exception("GTAlgorithmAlfaRCL::GenerateSchedule(): <criteria> parameter only supports the values {SPT, SEP, ECT}");

	 fAcumM = new float[iMachines] {0};
	 conj_G = new ATOM_SCHEDULE_LIST_PTR[iJobs];
	conj_rG = new ATOM_SCHEDULE_LIST_PTR[iJobs];

	//--------
	// Step-1. Let C contain the first schedulable operation of each job.
	//				 Let rjm = 0 for all operation (j, m) in C ==> [rjm is the ready time (earliest start time) for de operation (j, m)].
	conj_C = create__C();
	do
	{
		//--------
		// Step-2. Calculate t(C) = min(j,m) e C {rjm + pjm} ==> [pjm is the processing time of operation (j,m)].
		//				 Let m* denote the machine on which the minimun is achieved.
		m_star = calculate__tCmin(&conj_C, criteria);

		//--------
		// Step-3. Let G denote the conflict set of all operations (j, m*) on machine m* such that rjm* < t(C).
		size_G = obtain__G_with_cost(conj_G, conj_C, m_star.m, criteria, &custoMin, &custoMax);
		//size_G = obtain__G_with_cost(conj_G, conj_C, m_star, criteria, &custoMin, &custoMax);

		//--------
		// Step-4. Reduce G using alfa for RCL [cmin <= ci <= cmax + Alfa(cmax - cmin)]. 
		//				 Note: cmin and cmax refers to Cust, and not to Makespan (or Cmax).
		size_rG = reduce__G_rcl(conj_rG, conj_G, size_G, custoMin, custoMax, criteria);

		//--------
		// Step-5. Select one operation g from G using randomized procedure and schedule it.
		op = (Alfa > 0) ? genrand_int32() % size_rG : 0;
		schedule.AddToSchedule(conj_rG[op]->Atom.j);

#if 0 //---TMP-------------------------------------------------------------------------------------------------
		ATOM_SCHEDULE_LIST_PTR cptr = conj_C;
		std::cout << "-------------------------" << std::endl
			<< "simbol: j|m (pj)(rj)(cj)" << std::endl
			<< "-------------------------" << std::endl;

		std::cout << "m-Acum: "; for (int i = 0; i < iMachines; i++) std::cout << "m" << i << ":" << fAcumM[i] << "  ";
		std::cout << std::endl << std::endl << "C = ";
		while (cptr)
		{
			std::cout << "j:" << cptr->Atom.j << "|m:" << cptr->Atom.m
				<< " t(" << cptr->Atom.pj << ")"
				<< "r(" << cptr->Atom.rj << ")"
				<< "c(" << cptr->Atom.cj << ")  ";
			cptr = cptr->Next;
		}
		CustomConsole::ForegroundColor(ColorGreen);
		std::cout << std::endl << std::endl << "M* = " << "j:" << m_star.j << "|m:" << m_star.m
			<< " t(" << m_star.pj << ")"
			<< "r(" << m_star.rj << ")"
			<< "c(" << m_star.cj << ")";
		CustomConsole::ForegroundColor(ColorDarkYellow);
		std::cout << std::endl << std::endl << "G = ";
		for (int i = 0; i < size_G; i++)
			std::cout << "j:" << conj_G[i]->Atom.j << "|m:" << conj_G[i]->Atom.m
			<< " t(" << conj_G[i]->Atom.pj << ")"
			<< "r(" << conj_G[i]->Atom.rj << ")"
			<< "c(" << conj_G[i]->Atom.cj << ")  ";
		CustomConsole::Default();
		CustomConsole::ForegroundColor(ColorDarkCyan);
		std::cout << std::endl << std::endl << "custoMin: " << custoMin << "  |  custoMax: " << custoMax << std::endl;
		std::cout << std::endl << "size_rG: " << size_rG << std::endl;
		std::cout << std::endl << "rG = ";
		for (int i = 0; i < size_rG; i++)
			std::cout << "j:" << conj_rG[i]->Atom.j << "|m:" << conj_rG[i]->Atom.m
			<< " t(" << conj_rG[i]->Atom.pj << ")"
			<< "r(" << conj_rG[i]->Atom.rj << ")"
			<< "c(" << conj_rG[i]->Atom.cj << ")  ";
		CustomConsole::Default();
		CustomConsole::ForegroundColor(ColorRed);
		std::cout << std::endl << std::endl << "choice in rG: " << "j:" << conj_rG[op]->Atom.j << "|m:" << conj_rG[op]->Atom.m
			<< " t(" << conj_rG[op]->Atom.pj << ")"
			<< "r(" << conj_rG[op]->Atom.rj << ")"
			<< "c(" << conj_rG[op]->Atom.cj << ")";
		CustomConsole::LastState();
		std::cout << std::endl << std::endl << schedule << std::endl << std::endl << std::endl;
#endif //-----------------------------------------------------------------------------------------------------------------

		fAcumM[conj_rG[op]->Atom.m] += conj_rG[op]->Atom.pj; // OJO-NOTA: pertenece al Paso-4 pero tengo que ponerlo provisional luego de imprimir, para poder debuguear

		//--------
		// Step-6. Delete the operation g from C and include (prepare) its immediate successor in C.
		j = conj_rG[op]->Atom.j;
		cj = conj_rG[op]->Atom.cj;
		succ = conj_rG[op]->Atom.oj + 1;
		deletefrom__C(&conj_C, conj_rG[op]);

		//--------
		// Step-7. Update rjm in C and return to Step-2 until all operations are scheduled.	
		if (Dataset->GetOperationByIndex(j - 1, succ, &m, &t))
		{
			ATOM_SCHEDULE_LIST_PTR nptr;

			nptr = new ATOM_SCHEDULE_LIST();
			// actualizacion de los campos.
			nptr->Atom.j = j;
			nptr->Atom.m = m;
			nptr->Atom.oj = succ;
			nptr->Atom.pj = t;
			nptr->Atom.rj = cj;
			nptr->Atom.cj = .0f;
			// paso de doblemente enlace.
			nptr->Next = conj_C;
			if (conj_C) conj_C->Prev = nptr;
			conj_C = nptr;
		}
	}
	while (conj_C);

	delete []conj_G;
	delete []conj_rG;
	delete []fAcumM;
	return schedule;
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
CGTAlgorithmHybrid::CGTAlgorithmHybrid(const CDataSet &dset) :
	CGTAlgorithmBase(dset)
{
	// Note: Alfa = 1 -> null reduction: the algorithm generates the same space search 
	//			 that o original algorithm version propused by Gilffer and Thompson.
	Alfa = 1.0;	
}



//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
CGTAlgorithmHybrid::~CGTAlgorithmHybrid(void)
{

}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
void CGTAlgorithmHybrid::SetParamAlfa(float alfa)
{
	if (!(alfa >= 0 && alfa <= 1))
		throw std::exception("GTAlgorithmHybrid::SetParamAlfa(): <alfa> parameter only supports the closed continuous interval [0f; 1f].");

	Alfa = alfa;
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
float CGTAlgorithmHybrid::GetParamAlfa(void)
{
	return Alfa;
}


