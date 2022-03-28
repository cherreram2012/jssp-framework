#include <time.h>
#include <limits.h>
#include "CGrasp_PR_Restarts.h"

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
CGraph_PR_Restarts::CGraph_PR_Restarts(CLocalSearchStrategy *locals, CNeighborhoobStrategy *neighbor, CTerminator *term, CPathRelinking *prelinking, CRestartStrategy *restarts, float alfa)
	: CGrasp_PR(locals, neighbor, term, prelinking, alfa)
{
	Restarts = restarts;
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
CGraph_PR_Restarts::~CGraph_PR_Restarts(void)
{
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
CSchedule CGraph_PR_Restarts::Execute(float *fitness)
{
	clock_t start, curr;
	CSchedule S, Sprime, Sstart;
	float f, fprime, fstart, fworst;

	CheckDataConsistency();

	GT->SetSeed(iSeed);
	Stats.Initialize();
	Restarts->MarkAt(Stats);
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

			// Garantizo que en la llamada a path-relinking siempre S(g) sea mejor que S(i)
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
		// Paso 4: actualizo a la mejor solucion encontrada.
		if (f < fstart)
		{
			fstart = f;
			Sstart = S;
			Stats.n_improves++;
			Stats.best_score = fstart;
			Stats.sum_best_scores += fstart;
			Restarts->MarkAt(Stats, true);
		}

		//--------
		// Paso 5: actualizo las estadisticas.
		Stats.n_iterations++;
		Stats.sum_all_scores += f;
		if (fworst > Stats.worst_score) Stats.worst_score = fworst;
		curr = clock();
		Stats.elapsed_time = (double)(curr - start) / CLOCKS_PER_SEC;

		//--------
		// Paso 6: Verifico necesidad de aplicar un Restart o actualizar el conjunto Elite!!!.
		if (Restarts->isDone(Stats))
		{
			E->Restart();
			Restarts->MarkAt(Stats);
		}
		else
		{
			E->UpdateElite(S, f);
		}
	}

	Stats.best_solution = Sstart;
	if (fitness) *fitness = fstart;
	delete E;

	return Sstart;
}