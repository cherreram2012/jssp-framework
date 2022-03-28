#include <time.h>
#include <limits.h>
#include "CGrasp_PR.h"

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
CGrasp_PR::CGrasp_PR(CLocalSearchStrategy *locals, CNeighborhoobStrategy *neighbor, CTerminator *term, CPathRelinking *prelinking, float alfa)
	: CGraspAbstract(locals, neighbor, term, alfa)
{
	E = nullptr;
	PRelinking = prelinking;
	PRelinking->LocalSearch(locals);
	PRelinking->Neighborhood(neighbor);
	elite_size = default_elite_size;
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
CGrasp_PR::~CGrasp_PR(void)
{
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
CSchedule CGrasp_PR::Execute(float *fitness)
{
	clock_t start, curr;
	CSchedule S, Sprime;
	float f, fprime, fworst, fstart;

	CheckDataConsistency();

	GT->SetSeed(iSeed);
	Stats.Initialize();
	E = new CEliteSet(elite_size);
	fstart = fprime = Stats.best_score = numeric_limits<float>::infinity();

	start = clock();
	while (!Terminator->isDone(Stats))
	{
		//--------
		// Paso 1: método construtivo.
		S = GT->GenerateSchedule(SEP_GREEDY);
		CollectionJ->Create_NxM_Graph(Graph, CollectionM, S);
		f = Graph->Makespan();
		fworst = f;
		Graph->ClearGraph();

		//--------
		// Paso 2: método de busqueda local.
		S = LocalSearch->Execute(S, &f);

		//--------
		// Paso 3: se aplica path-relinking seleccionando aleatoriamente 
		//				 un elemento del conjunto elite.
		if (E->Count() > 0)
		{
			Sprime = E->RandomSelect(&fprime);

			// Garantizo que en la llamada a path-relinking siempre S(g) sea mejor que S(i).
			if (fprime < f)
				Sprime = PRelinking->Relink(S, Sprime, &fprime);
			else
				Sprime = PRelinking->Relink(Sprime, S, &fprime);
				
			if (fprime < f)
			{
				f = fprime;
				S = Sprime;
			}
		}
		else
		{
			Stats.n_improves++;
			Stats.sum_best_scores += f;
		}

		//--------
		// Paso 4: actualizo a la mejor solucion encontrada entre Paso-2 vs Paso-3.
		if (f < fstart)
		{
			fstart = f;
			Stats.n_improves++;
			Stats.best_score = fstart;
			Stats.sum_best_scores += fstart;
		}

		//--------
		// Paso 5: se actualiza el conjunto elite.
		E->UpdateElite(S, f);

		//--------
		// Paso 6: actualizo las estadisticas.
		Stats.n_iterations++;		
		Stats.sum_all_scores += f;
		if (fworst > Stats.worst_score) Stats.worst_score = fworst;
		curr = clock();
		Stats.elapsed_time = (double)(curr - start) / CLOCKS_PER_SEC;
	}

	Sprime = E->getBest(&fprime);
	Stats.best_score = fprime;
	Stats.best_solution = Sprime;
	if (fitness) *fitness = fprime;
	delete E;

	return Sprime;
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
void CGrasp_PR::EliteSize(unsigned int size)
{
	elite_size = size;
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
void CGrasp_PR::UserData(CPieceCollection *cJ, CMachineCollection *cM)
{
	CGraspAbstract::UserData(cJ, cM);
	PRelinking->UserData(cJ, cM);
}