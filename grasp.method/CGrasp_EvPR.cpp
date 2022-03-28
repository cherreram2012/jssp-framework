#include <time.h>
#include <limits.h>
#include "CGrasp_EvPR.h"

CGraph_EvPR::CGraph_EvPR(CLocalSearchStrategy *locals, CNeighborhoobStrategy *neighbor, CTerminator *term, CPathRelinking *prelinking, float alfa)
	: CGrasp_PR(locals, neighbor, term, prelinking, alfa)
{
}

CGraph_EvPR::~CGraph_EvPR(void)
{
}

CSchedule CGraph_EvPR::Execute(float *fitness)
{
	clock_t start, curr;
	CSchedule S, Sprime;
	float f, fprime, fworst;

	CheckDataConsistency();

	GT->SetSeed(iSeed);
	Stats.Initialize();
	E = new CEliteSet(elite_size);
	fprime = Stats.best_score = numeric_limits<float>::infinity();

	start = clock();
	while (!Terminator->isDone(Stats))
	{
		//--------
		// Paso 1: método construtivo.


		//--------
		// Paso 2: método de busqueda local.


		//--------
		// Paso 3: se aplica path-relinking seleccionando aleatoriamente 
		//				 un elemento del conjunto elite.
	}

	//Sprime = E->getBest(&fprime);
	//if (fitness) *fitness = fprime;
	//delete E;

	return Sprime;
}

