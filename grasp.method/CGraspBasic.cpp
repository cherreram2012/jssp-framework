#include <time.h>
#include <limits.h>
#include "CGraspBasic.h"

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
CGraspBasic::CGraspBasic(CLocalSearchStrategy *locals, CNeighborhoobStrategy *neighbor, CTerminator *term, float alfa)
	: CGraspAbstract(locals, neighbor, term, alfa)
{
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
CGraspBasic::~CGraspBasic(void)
{
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
CSchedule CGraspBasic::Execute(float *fitness)
{
	clock_t start, curr;
	CSchedule S, Sprime;
	float f, fprime, fworst;
	
	CheckDataConsistency();

	GT->SetSeed(iSeed);
	Stats.Initialize();
	fprime = Stats.best_score = numeric_limits<float>::infinity();

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
		// Paso 3: actualizo a la mejor solucion encontrada.
		if (f < fprime)
		{
			fprime = f;
			Sprime = S;
			Stats.n_improves++;
			Stats.best_score = fprime;
			Stats.sum_best_scores += fprime;
		}

		//--------
		// Paso 4: actualizo las estadisticas.
		Stats.n_iterations++;
		Stats.sum_all_scores += f;
		if (fworst > Stats.worst_score) Stats.worst_score = fworst;
		curr = clock();
		Stats.elapsed_time = (double)(curr - start) / CLOCKS_PER_SEC;
	}

	Stats.best_solution = Sprime;
	if (fitness) *fitness = fprime;

	return Sprime;
}
