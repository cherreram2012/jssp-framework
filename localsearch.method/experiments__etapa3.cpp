#include <time.h>
#include <limits>
#include <stdio.h>
#include <string>
#include <iostream>
#include "experiments__etapa3.h"
#include "../jssp.model/CHndFile.h"
#include "../jssp.heuristics/CGTAlgorithm.h"
#include "../jssp.heuristics/CMultiStartAlgorithm.h"
#include "../mersenne_twister/mt19937ar.h"

#define LINE 128
#define GT_CLASSIC 1
#define GT_SEMI_SEP 1

using namespace std;

//----------------------------------------------------------------------------------
//
void runExperiment_LocalSearch_value__etapa3(map<char *, char *> data_files, int seed)
{
#if GT_CLASSIC
	//*****
	std::cout << "GTClassic + FirstImproving + LinearAdjacent (21-instances)" << endl;
	ofstream fOut_A1_B1_C1("..\\.estudo-computacional\\3.localsearch-methods\\GTClassic + FirstImproving + LinearAdjacent (21-instances).txt");
	testTwoPhases_Constructive_LocalSearch__etapa3(data_files, fOut_A1_B1_C1, RANDOM, FIRST_IMPROVING, LINEAR_ADJACENT, seed);
	fOut_A1_B1_C1.close();

	//*****
	std::cout << "GTClassic + FirstImproving + CircularAdjacent (21-instances)" << endl;
	ofstream fOut_A1_B1_C2("..\\.estudo-computacional\\3.localsearch-methods\\GTClassic + FirstImproving + CircularAdjacent (21-instances).txt");
	testTwoPhases_Constructive_LocalSearch__etapa3(data_files, fOut_A1_B1_C2, RANDOM, FIRST_IMPROVING, CIRCULAR_ADJACENT, seed);
	fOut_A1_B1_C2.close();

	//*****
	std::cout << "GTClassic + FirstImproving + LinearNoAdjacent (21-instances)" << endl;
	ofstream fOut_A1_B1_C3("..\\.estudo-computacional\\3.localsearch-methods\\GTClassic + FirstImproving + LinearNoAdjacent (21-instances).txt");
	testTwoPhases_Constructive_LocalSearch__etapa3(data_files, fOut_A1_B1_C3, RANDOM, FIRST_IMPROVING, LINEAR_NO_ADJACENT, seed);
	fOut_A1_B1_C3.close();

	//*****
	std::cout << "GTClassic + BestImproving + LinearAdjacent (21-instances)" << endl;
	ofstream fOut_A1_B2_C1("..\\.estudo-computacional\\3.localsearch-methods\\GTClassic + BestImproving + LinearAdjacent (21-instances).txt");
	testTwoPhases_Constructive_LocalSearch__etapa3(data_files, fOut_A1_B2_C1, RANDOM, BEST_IMPROVING, LINEAR_ADJACENT, seed);
	fOut_A1_B2_C1.close();

	//*****
	std::cout << "GTClassic + BestImproving + CircularAdjacent (21-instances)" << endl;
	ofstream fOut_A1_B2_C2("..\\.estudo-computacional\\3.localsearch-methods\\GTClassic + BestImproving + CircularAdjacent (21-instances).txt");
	testTwoPhases_Constructive_LocalSearch__etapa3(data_files, fOut_A1_B2_C2, RANDOM, BEST_IMPROVING, CIRCULAR_ADJACENT, seed);
	fOut_A1_B2_C2.close();

	//*****
	std::cout << "GTClassic + BestImproving + LinearNoAdjacent (21-instances)" << endl;
	ofstream fOut_A1_B2_C3("..\\.estudo-computacional\\3.localsearch-methods\\GTClassic + BestImproving + LinearNoAdjacent (21-instances).txt");
	testTwoPhases_Constructive_LocalSearch__etapa3(data_files, fOut_A1_B2_C3, RANDOM, BEST_IMPROVING, LINEAR_NO_ADJACENT, seed);
	fOut_A1_B2_C3.close();
#endif

#if GT_SEMI_SEP
	//*****
	std::cout << "GTSemiSEP + FirstImproving + LinearAdjacent (21-instances)" << endl;
	ofstream fOut_A2_B1_C1("..\\.estudo-computacional\\3.localsearch-methods\\GTSemiSEP + FirstImproving + LinearAdjacent (21-instances).txt");
	testTwoPhases_Constructive_LocalSearch__etapa3(data_files, fOut_A2_B1_C1, SEP_RANDOM, FIRST_IMPROVING, LINEAR_ADJACENT, seed);
	fOut_A2_B1_C1.close();

	//*****
	std::cout << "GTSemiSEP + FirstImproving + CircularAdjacent (21-instances)" << endl;
	ofstream fOut_A2_B1_C2("..\\.estudo-computacional\\3.localsearch-methods\\GTSemiSEP + FirstImproving + CircularAdjacent (21-instances).txt");
	testTwoPhases_Constructive_LocalSearch__etapa3(data_files, fOut_A2_B1_C2, SEP_RANDOM, FIRST_IMPROVING, CIRCULAR_ADJACENT, seed);
	fOut_A2_B1_C2.close();

	//*****
	std::cout << "GTSemiSEP + FirstImproving + LinearNoAdjacent (21-instances)" << endl;
	ofstream fOut_A2_B1_C3("..\\.estudo-computacional\\3.localsearch-methods\\GTSemiSEP + FirstImproving + LinearNoAdjacent (21-instances).txt");
	testTwoPhases_Constructive_LocalSearch__etapa3(data_files, fOut_A2_B1_C3, SEP_RANDOM, FIRST_IMPROVING, LINEAR_NO_ADJACENT, seed);
	fOut_A2_B1_C3.close();

	//*****
	std::cout << "GTSemiSEP + BestImproving + LinearAdjacent (21-instances)" << endl;
	ofstream fOut_A2_B2_C1("..\\.estudo-computacional\\3.localsearch-methods\\GTSemiSEP + BestImproving + LinearAdjacent (21-instances).txt");
	testTwoPhases_Constructive_LocalSearch__etapa3(data_files, fOut_A2_B2_C1, SEP_RANDOM, BEST_IMPROVING, LINEAR_ADJACENT, seed);
	fOut_A2_B2_C1.close();

	//*****
	std::cout << "GTSemiSEP + BestImproving + CircularAdjacent (21-instances)" << endl;
	ofstream fOut_A2_B2_C2("..\\.estudo-computacional\\3.localsearch-methods\\GTSemiSEP + BestImproving + CircularAdjacent (21_instances).txt");
	testTwoPhases_Constructive_LocalSearch__etapa3(data_files, fOut_A2_B2_C2, SEP_RANDOM, BEST_IMPROVING, CIRCULAR_ADJACENT, seed);
	fOut_A2_B2_C2.close();

	//*****
	std::cout << "GTSemiSEP + BestImproving + LinearNoAdjacent (21-instances)" << endl;
	ofstream fOut_A2_B2_C3("..\\.estudo-computacional\\3.localsearch-methods\\GTSemiSEP + BestImproving + LinearNoAdjacent (21-instances).txt");
	testTwoPhases_Constructive_LocalSearch__etapa3(data_files, fOut_A2_B2_C3, SEP_RANDOM, BEST_IMPROVING, LINEAR_NO_ADJACENT, seed);
	fOut_A2_B2_C3.close();
#endif
}

//----------------------------------------------------------------------------------
//
void runExperiment_CmaxVsStep_FirstImproving__etapa3(const char *file_instance, enum SolveConflict solve, int seed)
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
	CLinearAdjacentNeighbor N1;
	CCircularAdjacentNeighbor N2;
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
	cout << "[" << nameAlg << "]" << endl;
	gt->SetSeed(seed);
	schedule = gt->GenerateSchedule(solve);
	cJ->Create_NxM_Graph(graph, cM, schedule);
	cmax_F1 = graph->Makespan();
	graph->ClearGraph();

#if TO_FILE
	sprintf_s(filename, "..\\.estudo-computacional\\3.localsearch-methods\\gnuplot\\%s_constructive_%s.dat", nameAlg, file.GetFilename(false));
	ofstream fOut_phase1(filename);
	fOut_phase1 << 0 << "\t" << cmax_F1 << "\t" << 0;
	fOut_phase1.close();
#endif

#if TO_SCREEN
	cout << "cmax: " << cmax << endl << endl;
#endif

	//--------------------------------
	// Phase #2 - Local Search Method.
	//*****
	cout << "+-------------------------------+";
	cout << endl << "| Phase 2: LOCAL SEARCH METHOD  |" << endl;
	cout << "+-------------------------------+" << endl;
	cout << nameAlg << " + FirstImproving + LinearAdjacent " << endl;
	sprintf_s(filename, LINE, "..\\.estudo-computacional\\3.localsearch-methods\\gnuplot\\%s_FirstImproving_LinearAdjacent_%s.dat", nameAlg, file.GetFilename(false));
	ofstream fOut_N1(filename);
	fOut_N1 << 0 << "\t" << cmax_F1 << "\t" << 0 << endl;
	cmax_F2 = testLocalSearch_CmaxVsStep__etapa3(fOut_N1, cJ, cM, &N1, schedule, FIRST_IMPROVING);
	fOut_N1.close();

#if TO_FILE
	/*sprintf_s(filename, "..\\.estudo-computacional\\3.localsearch-methods\\gnuplot\\%s_localsearch_%s.dat", nameAlg, file.GetFilename(false));
	ofstream fOut_phase2(filename);
	fOut_phase2 << "\t" << cmax;
	fOut_phase2.close();*/
#endif

#if !TO_FILE
	std::remove(filename);
#endif

	//*****
	cout << nameAlg << " + FirstImproving + CircularAdjacent " << endl;
	sprintf_s(filename, LINE, "..\\.estudo-computacional\\3.localsearch-methods\\gnuplot\\%s_FirstImproving_CircularAdjacent_%s.dat", nameAlg, file.GetFilename(false));
	ofstream fOut_N2(filename);
	fOut_N2 << 0 << "\t" << cmax_F1 << "\t" << 0 << endl;
	cmax_F2 = testLocalSearch_CmaxVsStep__etapa3(fOut_N2, cJ, cM, &N2, schedule, FIRST_IMPROVING);
	fOut_N2.close();
#if !TO_FILE
	std::remove(filename);
#endif

	//*****
	cout << nameAlg << " + FirstImproving + LinearNoAdjacent " << endl;
	sprintf_s(filename, LINE, "..\\.estudo-computacional\\3.localsearch-methods\\gnuplot\\%s_FirstImproving_LinearNoAdjacent_%s.dat", nameAlg, file.GetFilename(false));
	ofstream fOut_N3(filename);
	fOut_N3 << 0 << "\t" << cmax_F1 << "\t" << 0 << endl;
	cmax_F2 = testLocalSearch_CmaxVsStep__etapa3(fOut_N3, cJ, cM, &N3, schedule, FIRST_IMPROVING);
	fOut_N3.close();
#if !TO_FILE
	std::remove(filename);
#endif
	delete cM, cJ, gt, graph, dataset;
}

//----------------------------------------------------------------------------------
//
void runExperiment_CmaxVsStep_BestImproving__etapa3(const char *file_instance, enum SolveConflict solve, int seed)
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
	CLinearAdjacentNeighbor N1;
	CCircularAdjacentNeighbor N2;
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
	cout << "+-------------------------------+" << endl;
	cout << "| Phase 1: CONSTRUCTIVE METHOD  |" << endl;
	cout << "+-------------------------------+" << endl;
	cout << "[" << nameAlg << "]" << endl;
	gt->SetSeed(seed);
	schedule = gt->GenerateSchedule(solve);
	cJ->Create_NxM_Graph(graph, cM, schedule);
	cmax_F1 = graph->Makespan();
	graph->ClearGraph();

#if TO_FILE
	sprintf_s(filename, LINE, "..\\.estudo-computacional\\3.localsearch-methods\\gnuplot\\%s_constructive_%s.dat", nameAlg, file.GetFilename(false));
	ofstream fOut_phase1(filename);
	fOut_phase1 << 0 << "\t" << cmax_F1 << "\t" << 0;
	fOut_phase1.close();
#endif

#if TO_SCREEN
	cout << "cmax: " << cmax << endl << endl;
#endif

	//--------------------------------
	// Phase #2 - Local Search Method.
	//*****
	cout << "+-------------------------------+";
	cout << endl << "| Phase 2: LOCAL SEARCH METHOD  |" << endl;
	cout << "+-------------------------------+" << endl;
	cout << nameAlg << " + BestImproving + LinearAdjacent " << endl;
	sprintf_s(filename, LINE, "..\\.estudo-computacional\\3.localsearch-methods\\gnuplot\\%s_BestImproving_LinearAdjacent_%s.dat", nameAlg, file.GetFilename(false));
	ofstream fOut_N1(filename);
	fOut_N1 << 0 << "\t" << cmax_F1 << "\t" << 0 << endl;
	cmax_F2 = testLocalSearch_CmaxVsStep__etapa3(fOut_N1, cJ, cM, &N1, schedule, BEST_IMPROVING);
	fOut_N1.close();
#if !TO_FILE
	std::remove(filename);
#endif

	//*****
	cout << nameAlg << " + BestImproving + CircularAdjacent " << endl;
	sprintf_s(filename, LINE, "..\\.estudo-computacional\\3.localsearch-methods\\gnuplot\\%s_BestImproving_CircularAdjacent_%s.dat", nameAlg, file.GetFilename(false));
	ofstream fOut_N2(filename);
	fOut_N2 << 0 << "\t" << cmax_F1 << "\t" << 0 << endl;
	cmax_F2 = testLocalSearch_CmaxVsStep__etapa3(fOut_N2, cJ, cM, &N2, schedule, BEST_IMPROVING);
	fOut_N2.close();
#if !TO_FILE
	std::remove(filename);
#endif

	//*****
	cout << nameAlg << " + BestImproving + LinearNoAdjacent " << endl;
	sprintf_s(filename, LINE, "..\\.estudo-computacional\\3.localsearch-methods\\gnuplot\\%s_BestImproving_LinearNoAdjacent_%s.dat", nameAlg, file.GetFilename(false));
	ofstream fOut_N3(filename);
	fOut_N3 << 0 << "\t" << cmax_F1 << "\t" << 0 << endl;
	cmax_F2 = testLocalSearch_CmaxVsStep__etapa3(fOut_N3, cJ, cM, &N3, schedule, BEST_IMPROVING);
	fOut_N3.close();
#if !TO_FILE
	std::remove(filename);
#endif

	delete cM, cJ, gt, graph, dataset;
}

//----------------------------------------------------------------------------------
//
float testLocalSearch_CmaxVsStep__etapa3(ofstream &output, CPieceCollection *cJ, CMachineCollection *cM, CNeighborhoobStrategy *neighborhood, const CSchedule &entry_point, enum LocalSearchType type)
{
	float cmax;
	CSchedule schedule;	
	CLocalSearchForExperiment::LOCAL_SEARCH_STATISTCS stats;
	CLocalSearchForExperiment localsearch(neighborhood, cJ, cM, true);

	switch (type)
	{
		case FIRST_IMPROVING:
			schedule = localsearch.FirstImproving(entry_point, &cmax);
			break;

		case BEST_IMPROVING:
			schedule = localsearch.BestImproving(entry_point, &cmax);
			break;

		default:
			throw std::exception("Test LocalSearch Cmax vs Step(): <type> parameter no valid.");
			break;
	}

	stats = localsearch.Statistics();

#if TO_FILE
	for (int i = 0; i < (int)stats.StepsCount; i++)
		output << i+1 << "\t" << stats.F_Historial[i] << "\t" << stats.T_Historial[i] << endl;
#endif

#if TO_SCREEN
	cout << "cmax: " << cmax << "    { " << stats.StepsCount << " }" << endl << endl;
#endif

	return cmax;
}

//----------------------------------------------------------------------------------
//
void testTwoPhases_Constructive_LocalSearch__etapa3(map<char *, char *> data_files, ofstream &output, enum SolveConflict solve, enum LocalSearchType localsearch_type, enum NeighborhoobStrategy neigh_type, int seed)
{
	float cmax;
	double time, utime;
	CSchedule schedule;
	map<char *, CSchedule> constructive_schedules;
	map<char *, CSchedule> localsearch_schedules;
	map<char *, CSchedule>::iterator better;
	map<char *, char *>::iterator instance;
	
	output << "phase-#: Cmax ---- time(seg)" << endl;
	instance = data_files.begin();
	for (; instance != data_files.end(); instance++)
	{
		phaseContructive_GTAlgorithm_bestone_value__etapa3(instance->second, 1, solve, &schedule, &cmax, &utime, &time, seed);
		constructive_schedules[instance->first] = schedule;
		output << "[" << instance->first << "]" << ": ---------------------" << endl
			<< "phase-1: " << cmax << "\t" << utime << "\t" << time << endl;

		phaseLocalSearch_bestone_value__etapa3(instance->second, schedule, localsearch_type, neigh_type, &schedule, &cmax, &utime, &time);
		localsearch_schedules[instance->first] = schedule;
		output << "phase-2: " << cmax << "\t" << utime << "\t" << time << endl << endl;
	}

	// Registro las secuencias de las mejores soluciones encontradas.
	output << endl << endl << "CONSTRUCTIVE SCHEDULES LIST:" << endl;
	better = constructive_schedules.begin();
	for (; better != constructive_schedules.end(); better++)
		output << "[" << better->first << "]:" << endl << better->second << endl << endl;

	output << endl << endl << "LOCAL SEARCH SCHEDULES LIST:" << endl;
	better = localsearch_schedules.begin();
	for (; better != localsearch_schedules.end(); better++)
		output << "[" << better->first << "]:" << endl << better->second << endl << endl;
}

//----------------------------------------------------------------------------------
//
void phaseContructive_GTAlgorithm_bestone_value__etapa3(const char *file_instance, int n_run, enum SolveConflict solve, CSchedule *best_schedule, float *best_cmax, double *utime, double *time, int seed)
{
	clock_t start, end;
	LARGE_INTEGER ustart, uend;
	float  cmax;
	int jobs, machines;
	CHndFile file;
	CDataSet *dataset;
	CGraphCAPP *graph;
	CPieceCollection *cJ;
	CMachineCollection *cM;
	CGTAlgorithm_DRules *gt;

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

	gt->SetSeed(seed);
	start = clock();
	QueryPerformanceCounter(&ustart);
	*best_cmax = numeric_limits<float>::infinity();
	for (int i = 0; i < n_run; i++)
	{
		CSchedule schedule = gt->GenerateSchedule(solve);
		cJ->Create_NxM_Graph(graph, cM, schedule);
		cmax = graph->Makespan();
		graph->ClearGraph();

		if (cmax < *best_cmax)
		{
			*best_cmax = cmax;
			*best_schedule = schedule;
		}
	}
	QueryPerformanceCounter(&uend);
	end = clock();
	*time = (double)(end - start) / CLOCKS_PER_SEC;
	*utime = performancecounter_diff(&ustart, &uend);

	delete cM, cJ, gt, graph, dataset;
}

//----------------------------------------------------------------------------------
//
void phaseLocalSearch_bestone_value__etapa3(const char *file_instance, const CSchedule entry_point, enum LocalSearchType localsearch_type, enum NeighborhoobStrategy neigh_type, CSchedule *best_schedule, float *best_cmax, double *utime, double *time)
{
	clock_t start, end;
	LARGE_INTEGER ustart, uend;
	float  cmax;
	int jobs, machines;
	CHndFile file;
	CGraphCAPP *graph;
	CPieceCollection *cJ;
	CMachineCollection *cM;
	CLinearAdjacentNeighbor N1;
	CCircularAdjacentNeighbor N2;
	CLinearNoAdjacentNeighbor N3;
	CLocalSearchForExperiment *localsearch;

	file.SetPath(file_instance);
	file.ReadInstanceSize(&jobs, &machines);
	cM = new CMachineCollection(machines);
	cJ = new CPieceCollection(jobs, machines, false);
	file.ReadMachineData(cM);
	file.ReadPieceData(cJ, *cM);
	graph = new CGraphCAPP(jobs, machines, 0);

	switch (neigh_type)
	{
		case LINEAR_ADJACENT:
			localsearch = new CLocalSearchForExperiment(&N1, cJ, cM);
			break;
		
		case CIRCULAR_ADJACENT:
			localsearch = new CLocalSearchForExperiment(&N2, cJ, cM);
			break;
		
		case LINEAR_NO_ADJACENT:
			localsearch = new CLocalSearchForExperiment(&N3, cJ, cM);
			break;
		default:
			throw std::exception("<neigh_type> parameter only supports the values { LINEAR_ADJACENT, CIRCULAR_ADJACENT, LINEAR_NO_ADJACENT }");
	}

	if (localsearch_type == FIRST_IMPROVING)
	{
		start = clock();
		QueryPerformanceCounter(&ustart);
		*best_schedule = localsearch->FirstImproving(entry_point, &cmax);
		QueryPerformanceCounter(&uend);
		end = clock();
	}
	else if (localsearch_type == BEST_IMPROVING)
	{
		start = clock();
		QueryPerformanceCounter(&ustart);
		*best_schedule = localsearch->BestImproving(entry_point, &cmax);
		QueryPerformanceCounter(&uend);
		end = clock();
	}
	else
	{
		throw std::exception("<localsearch_type> parameter only supports the values { FIRST_IMPROVING, BEST_IMPROVING }");
	}

	*best_cmax = cmax;
	*time = (double)(end - start) / CLOCKS_PER_SEC;
	*utime = performancecounter_diff(&ustart, &uend);
	
	delete cM, cJ, graph, localsearch;
}

