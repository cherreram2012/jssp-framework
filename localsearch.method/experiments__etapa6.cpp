#include <time.h>
#include <limits>
#include <stdio.h>
#include <string>
#include <iostream>
#include "experiments__etapa6.h"
#include "../jssp.model/CHndFile.h"
#include "../jssp.heuristics/CGTAlgorithm.h"
#include "../jssp.heuristics/CMultiStartAlgorithm.h"
#include "../mersenne_twister/mt19937ar.h"

#define LINE 256
#define GT_CLASSIC 1
#define GT_SEMI_SEP 1

using namespace std;

//----------------------------------------------------------------------------------
//
void runExperiment_CmaxVsStep_IBUIT__etapa6(const char *file_instance, enum SolveConflict solve, int seed)
{
	char nameAlg[32], filename[LINE];
	float  cmax_F1, cmax_F2;
	int jobs, machines;
	CHndFile file;
	CDataSet *dataset;
	CGraphCAPP *graph;
	CSchedule schedule;
	CPieceCollection *cJ;
	CMachineCollection *cM;
	CGTAlgorithm_DRules *gt;
	CLinearNoAdjacentNeighbor N3;

	file.SetPath(file_instance);
	file.ReadInstanceSize(&jobs, &machines);
	cM = new CMachineCollection(machines);
	cJ = new CPieceCollection(jobs, machines, false);
	dataset = new CDataSet(jobs);
	file.ReadMachineData(cM);
	file.ReadPieceData(cJ, *cM);
	cJ->FillDataSet(dataset, *cM);
	gt = new CGTAlgorithm_DRules(*dataset);
	graph = new CGraphCAPP(jobs, machines, 0);

	strcpy_s(nameAlg, 32, (solve == RANDOM) ? "GTClassic" : "GTSemiSEP");

	//--------------------------------
	// Phase #1 - Constructive Method.
	cout << "+------------------------------+" << endl;
	cout << "| Phase 1: CONSTRUCTIVE METHOD |" << endl;
	cout << "+------------------------------+" << endl;
	cout << "[" << nameAlg << "]" << endl << endl << endl;
	gt->SetSeed(seed);
	schedule = gt->GenerateSchedule(solve);
	cJ->Create_NxM_Graph(graph, cM, schedule);
	cmax_F1 = graph->Makespan();
	graph->ClearGraph();

	sprintf_s(filename, "..\\.estudo-computacional\\6.improving-some-thing-in-grasp\\experiment #1 (ibuit-local-search)\\%s_constructive_%s.dat", nameAlg, file.GetFilename(false));
	ofstream fOut_f1(filename);
	fOut_f1 << 0 << "\t" << cmax_F1 << "\t" << 0;
	fOut_f1.close();

	//--------------------------------
	// Phase #2 - Local Search Method.
	cout << "+-------------------------------+";
	cout << endl << "| Phase 2: LOCAL SEARCH METHOD  |" << endl;
	cout << "+-------------------------------+" << endl;
	//*****
	cout << endl << nameAlg << " + FirstImproving + IBUIT " << endl;
	sprintf_s(filename, LINE, "..\\.estudo-computacional\\6.improving-some-thing-in-grasp\\experiment #1 (ibuit-local-search)\\%s_FirstImproving_IBUIT_%s.dat", nameAlg, file.GetFilename(false));
	ofstream fOut_f2_fi(filename);
	fOut_f2_fi << 0 << "\t" << cmax_F1 << "\t" << 0 << endl;
	cmax_F2 = testLocalSearch_CmaxVsStep__etapa6(fOut_f2_fi, cJ, cM, &N3, schedule, FIRST_IMPROVING);
	fOut_f2_fi.close();

	//*****
	cout << endl  << nameAlg << " + BestImproving + IBUIT " << endl;
	sprintf_s(filename, LINE, "..\\.estudo-computacional\\6.improving-some-thing-in-grasp\\experiment #1 (ibuit-local-search)\\%s_BestImproving_IBUIT_%s.dat", nameAlg, file.GetFilename(false));
	ofstream fOut_f2_bi(filename);
	fOut_f2_bi << 0 << "\t" << cmax_F1 << "\t" << 0 << endl;
	cmax_F2 = testLocalSearch_CmaxVsStep__etapa6(fOut_f2_bi, cJ, cM, &N3, schedule, BEST_IMPROVING);
	fOut_f2_bi.close();

	delete cM, cJ, gt, graph, dataset;
}

//----------------------------------------------------------------------------------
//
float testLocalSearch_CmaxVsStep__etapa6(ofstream &output, CPieceCollection *cJ, CMachineCollection *cM, CNeighborhoobStrategy *neighborhood, const CSchedule &entry_point, enum LocalSearchType type)
{
	float cmax;
	CSchedule schedule;
	CLocalSearchForExperiment::LOCAL_SEARCH_STATISTCS stats;
	CLocalSearchForExperiment localsearch(neighborhood, cJ, cM, true);

	switch (type)
	{
		case FIRST_IMPROVING:
			schedule = localsearch.FirstImproving_IBUIT(entry_point, &cmax);
			break;

		case BEST_IMPROVING:
			schedule = localsearch.BestImproving_IBUIT(entry_point, &cmax);
			break;

		default:
			throw std::exception("Test LocalSearch Cmax vs Step(): <type> parameter no valid.");
			break;
	}

	stats = localsearch.Statistics();

#if TO_FILE
	for (int i = 0; i < (int)stats.StepsCount; i++)
		output << i + 1 << "\t" << stats.F_Historial[i] << "\t" << stats.T_Historial[i] << endl;
#endif

#if TO_SCREEN
	cout << "cmax: " << cmax << "    { " << stats.StepsCount << " }" << endl << endl;
#endif

	return cmax;
}