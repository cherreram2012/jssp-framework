#include <map>
#include <limits>
#include <string>
#include <iostream>
#include <time.h>
#include <stdio.h>
#include "experiments.h"
#include "../jssp.model/CHndFile.h"
#include "../jssp.heuristics/CGTAlgorithm.h"
#include "../jssp.heuristics/CMultiStartAlgorithm.h"
#include "../mersenne_twister/mt19937ar.h"

#define LINE 128

using namespace std;

//----------------------------------------------------------------------------------
//
void runExperiment_GreedySPT_bestone_value(map<char *, char *>data_files, int n_run)
{
	float cmax;
	CSchedule schedule;
	map<char *, CSchedule> schedules;
	map<char *, CSchedule>::iterator better;
	map<char *, char *>::iterator instance;
	ofstream fOut("..\\.estudo-computacional\\2.constructive-methods\\SPT_21_instances.txt");

	cout << "[SPT best-one-value for 21 instances]" << endl;

	// Registro los valores de cmax de las mejores soluciones encontradas.
	instance = data_files.begin();
	for (; instance != data_files.end(); instance++)
	{
		test_GTAlgorithm_bestone_value(instance->second, n_run, SPT_GREEDY, &schedule, &cmax);
		schedules[instance->first] = schedule;
		fOut << instance->first << ": " << cmax << endl;
	}

	// Registro las secuencias de las mejores soluciones encontradas.
	fOut << endl << endl << "SCHEDULES LIST:" << endl;
	better = schedules.begin();
	for (; better != schedules.end(); better++)
		fOut << "[" << better->first << "]" << endl << better->second << endl << endl;

	fOut.close();
}

//----------------------------------------------------------------------------------
//
void runExperiment_GreedySEP_bestone_value(map<char *, char *>data_files, int n_run)
{
	float cmax;
	CSchedule schedule;
	map<char *, CSchedule> schedules;
	map<char *, CSchedule>::iterator better;
	map<char *, char *>::iterator instance;
	ofstream fOut("..\\.estudo-computacional\\2.constructive-methods\\SEP_21_instances.txt");

	cout << "[SEP best-one-value for 21 instances]" << endl;

	// Registro los valores de cmax de las mejores soluciones encontradas.
	instance = data_files.begin();
	for (; instance != data_files.end(); instance++)
	{
		test_GTAlgorithm_bestone_value(instance->second, n_run, SEP_GREEDY, &schedule, &cmax);
		schedules[instance->first] = schedule;
		fOut << instance->first << ": " << cmax << endl;
	}

	// Registro las secuencias de las mejores soluciones encontradas.
	fOut << endl << endl << "SCHEDULES LIST:" << endl;
	better = schedules.begin();
	for (; better != schedules.end(); better++)
		fOut << "[" << better->first << "]" << endl << better->second << endl << endl;

	fOut.close();
}

//----------------------------------------------------------------------------------
//
void runExperiment_GreedyECT_bestone_value(map<char *, char *>data_files, int n_run)
{
	float cmax;
	CSchedule schedule;
	map<char *, CSchedule> schedules;
	map<char *, CSchedule>::iterator better;
	map<char *, char *>::iterator instance;
	ofstream fOut("..\\.estudo-computacional\\2.constructive-methods\\ECT_21_instances.txt");

	cout << "[ECT best-one-value for 21 instances]" << endl;

	// Registro los valores de cmax de las mejores soluciones encontradas.
	instance = data_files.begin();
	for (; instance != data_files.end(); instance++)
	{
		test_GTAlgorithm_bestone_value(instance->second, n_run, ECT_GREEDY, &schedule, &cmax);
		schedules[instance->first] = schedule;
		fOut << instance->first << ": " << cmax << endl;
	}

	// Registro las secuencias de las mejores soluciones encontradas.
	fOut << endl << endl << "SCHEDULES LIST:" << endl;
	better = schedules.begin();
	for (; better != schedules.end(); better++)
		fOut << "[" << better->first << "]" << endl << better->second << endl << endl;

	fOut.close();
}

//----------------------------------------------------------------------------------
//
void runExperiment_GTAlgorithm_bestone_value(map<char *, char *> data_files, int n_run)
{
	float cmax;
	CSchedule schedule;
	map<char *, CSchedule> schedules;
	map<char *, CSchedule>::iterator better;
	map<char *, char *>::iterator instance;
	ofstream fOut("..\\.estudo-computacional\\2.constructive-methods\\GTAlgorithm_21_instances.txt");

	cout << "[GT-Classic best-one-value for 21 instances]" << endl;

	// Registro los valores de cmax de las mejores soluciones encontradas.
	instance = data_files.begin();
	for (; instance != data_files.end(); instance++)
	{
		test_GTAlgorithm_bestone_value(instance->second, n_run, RANDOM, &schedule, &cmax);
		schedules[instance->first] = schedule;
		fOut << instance->first << ": " << cmax << endl;
	}

	// Registro las secuencias de las mejores soluciones encontradas.
	fOut << endl << endl << "SCHEDULES LIST:" << endl;
	better = schedules.begin();
	for (; better != schedules.end(); better++)
		fOut << "[" << better->first << "]" << endl << better->second << endl << endl;

	fOut.close();
}

//----------------------------------------------------------------------------------
//
void runExperiment_SemiSEP_bestone_value(map<char *, char *> data_files, int n_run)
{
	float cmax;
	CSchedule schedule;
	map<char *, CSchedule> schedules;
	map<char *, CSchedule>::iterator better;
	map<char *, char *>::iterator instance;
	ofstream fOut("..\\.estudo-computacional\\2.constructive-methods\\SemiSEP_21_instances.txt");

	cout << "[Semi-SEP best-one-value for 21 instances]" << endl;

	// Registro los valores de cmax de las mejores soluciones encontradas.
	instance = data_files.begin();
	for (; instance != data_files.end(); instance++)
	{
		test_GTAlgorithm_bestone_value(instance->second, n_run, SEP_RANDOM, &schedule, &cmax);
		schedules[instance->first] = schedule;
		fOut << instance->first << ": " << cmax << endl;
	}

	// Registro las secuencias de las mejores soluciones encontradas.
	fOut << endl << endl << "SCHEDULES LIST:" << endl;
	better = schedules.begin();
	for (; better != schedules.end(); better++)
		fOut << "[" << better->first << "]" << endl << better->second << endl << endl;

	fOut.close();
}

//----------------------------------------------------------------------------------
//
void runExperiment_GTAlfaRCL_bestone_value(map<char *, char *> data_files, int n_run, float alfa, enum SolveConflict solve)
{
	float cmax;
	CSchedule schedule;
	map<char *, CSchedule> schedules;
	map<char *, CSchedule>::iterator better;
	map<char *, char *>::iterator instance;

	char output[LINE];
	sprintf_s(output, LINE, "..\\.estudo-computacional\\2.constructive-methods\\Alfa-RCL-%.2f(sep)_21_instances.txt", alfa);
	ofstream fOut(output);

	cout << "[Alfa-RCL "<< "(" << alfa << ") " << "best-one-value for 21 instances]" << endl;

	// Registro los valores de cmax de las mejores soluciones encontradas.
	instance = data_files.begin();
	for (; instance != data_files.end(); instance++)
	{
		test_GTAlfaRCL_bestone_value(instance->second, n_run, alfa, solve, &schedule, &cmax);
		schedules[instance->first] = schedule;
		fOut << instance->first << ": " << cmax << endl;
	}

	// Registro las secuencias de las mejores soluciones encontradas.
	fOut << endl << endl << "SCHEDULES LIST:" << endl;
	better = schedules.begin();
	for (; better != schedules.end(); better++)
		fOut << "[" << better->first << "]" << endl << better->second << endl << endl;

	fOut.close();
}

//----------------------------------------------------------------------------------
//
void runExperiment_Ocurrences_per_algorithm(const char *file_instance, int n_run, float optimal_value)
{
	CHndFile file;
	char instance[8];
	int jobs, machines;
	clock_t start, end;
	char f_output[LINE];
	CPieceCollection *cJ;
	CMachineCollection *cM;

	file.SetPath(file_instance);
	file.ReadInstanceSize(&jobs, &machines);
	strcpy_s(instance, 8, file.GetFilename(false));
	cM = new CMachineCollection(machines);
	cJ = new CPieceCollection(jobs, machines, false);
	file.ReadMachineData(cM);
	file.ReadPieceData(cJ, *cM);

#if MAXIMIZE_Z
	char z[] = "maxZ";
	sprintf_s(f_output, LINE, "..\\.estudo-computacional\\gnuplot\\%s-otimo-%s.dat", instance, z);
	ofstream fOut(f_output);
	fOut << (1 / optimal_value) << "\t" << n_run;
#else
	char z[] = "minZ";
	sprintf_s(f_output, LINE, "..\\.estudo-computacional\\2.constructive-methods\\gnuplot\\%s-otimo-%s.dat", instance, z);
	ofstream fOut(f_output);
	fOut << optimal_value << "\t" << n_run;
#endif 

	/*start = clock();
	sprintf_s(f_output, LINE, "..\\.estudo-computacional\\gnuplot\\%s-ocurrences-PURE_RAMDOM-for-%s.dat", instance, z);
	test_Randomized_ocurrences(cJ, cM, n_run, f_output);
	end = clock();
	cout << "PURE-RAMDOM: (" << (double)(end - start) / CLOCKS_PER_SEC << " sec)" << endl;*/

	start = clock();
	sprintf_s(f_output, LINE, "..\\.estudo-computacional\\2.constructive-methods\\gnuplot\\%s-ocurrences-SPT-for-%s.dat", instance, z);
	test_GTAlgorithm_ocurrences(cJ, cM, n_run, SPT_GREEDY, f_output);
	end = clock();
	cout << "SPT: (" << (double)(end - start) / CLOCKS_PER_SEC << " sec)" << endl;

	start = clock();
	sprintf_s(f_output, LINE, "..\\.estudo-computacional\\2.constructive-methods\\gnuplot\\%s-ocurrences-SEP-for-%s.dat", instance, z);
	test_GTAlgorithm_ocurrences(cJ, cM, n_run, SEP_GREEDY, f_output);
	end = clock();
	cout << "SEP: (" << (double)(end - start) / CLOCKS_PER_SEC << " sec)" << endl;

	start = clock();
	sprintf_s(f_output, LINE, "..\\.estudo-computacional\\2.constructive-methods\\gnuplot\\%s-ocurrences-ECT-for-%s.dat", instance, z);
	test_GTAlgorithm_ocurrences(cJ, cM, n_run, ECT_GREEDY, f_output);
	end = clock();
	cout << "ECT: (" << (double)(end - start) / CLOCKS_PER_SEC << " sec)" << endl;

	start = clock();
	sprintf_s(f_output, LINE, "..\\.estudo-computacional\\2.constructive-methods\\gnuplot\\%s-ocurrences-GT_CLASSIC-for-%s.dat", instance, z);
	test_GTAlgorithm_ocurrences(cJ, cM, n_run, RANDOM, f_output);
	end = clock();
	cout << "GT-CLASSIC: (" << (double)(end - start) / CLOCKS_PER_SEC << " sec)" << endl;

	start = clock();
	sprintf_s(f_output, LINE, "..\\.estudo-computacional\\2.constructive-methods\\gnuplot\\%s-ocurrences-GT_SEP_RANDOM-for-%s.dat", instance, z);
	test_GTAlgorithm_ocurrences(cJ, cM, n_run, SEP_RANDOM, f_output);
	end = clock();
	cout << "GT-SEP-RANDOM: (" << (double)(end - start) / CLOCKS_PER_SEC << " sec)" << endl;

	/*start = clock();
	sprintf_s(f_output, LINE, "..\\.estudo-computacional\\gnuplot\\%s-ocurrences-GT_ECT_RANDOMIZED-for-%s.dat", instance, z);
	test_GTAlgorithm_ocurrences(cJ, cM, n_run, ECT_RANDOMIZED, f_output);
	end = clock();
	cout << "GT-ECT-RANDOMIZED: (" << (double)(end - start) / CLOCKS_PER_SEC << " sec)" << endl;*/

	fOut.close();
	delete cJ, cM;
}

//----------------------------------------------------------------------------------
//
void runExperiment_Time_for_value(const char *file_instance, double seconds, enum SolveConflict solver)
{
	long interation;
	double elapsed;
	float cmax, best_cmax;
	char instance[8];
	int jobs, machines;
	char f_output[LINE];
	CHndFile file;
	CGraphCAPP *graph;
	CPieceCollection *cJ;
	CMachineCollection *cM;
	CGTAlgorithm_DRules *gt;
	clock_t start, current, end;

	file.SetPath(file_instance);
	file.ReadInstanceSize(&jobs, &machines);
	strcpy_s(instance, 8, file.GetFilename(false));
	cM = new CMachineCollection(machines);
	cJ = new CPieceCollection(jobs, machines, false);
	file.ReadMachineData(cM);
	file.ReadPieceData(cJ, *cM);
	
	sprintf_s(f_output, LINE, "..\\.estudo-computacional\\2.constructive-methods\\gnuplot\\%s-time-for-value-%s.dat", file.GetFilename(false), 
		(solver == RANDOM) ? "RANDOMIZED" : (solver == SEP_RANDOM) ? "SEP_RANDOMIZED" : "NONE");
	ofstream fOut(f_output);

	CDataSet dataset(jobs);

	cJ->FillDataSet(&dataset, *cM);

	gt = new CGTAlgorithm_DRules(dataset);
	graph = new CGraphCAPP(jobs, machines, 0);

	best_cmax = numeric_limits<float>::infinity();
	cout << "INIT: 0.0 sec" << endl << endl;
	elapsed = 0;
	interation = 0;
	start = clock();
	while (elapsed < seconds)
	{
		CSchedule schedule = gt->GenerateSchedule(solver);
		cJ->Create_NxM_Graph(graph, cM, schedule);
		cmax = graph->Makespan();

		current = clock();
		elapsed = (double)(current - start) / CLOCKS_PER_SEC;
		if (cmax < best_cmax)
		{
			best_cmax = cmax;
#if TO_SCREEN
			cout << elapsed << " => " << best_cmax << endl;
#endif

#if TO_FILE
			fOut << elapsed << "\t" << best_cmax << endl;
#endif
		}

		graph->ClearGraph();
		interation++;
	}

	end = clock();
	cout << endl << "END: " << (double)(end - start) / CLOCKS_PER_SEC << " sec" << endl
			 << "Iterations: " << interation << endl;
	fOut.close();
}

//----------------------------------------------------------------------------------
//
void runExperiment_Iterations_for_value(const char *file_instance, long iterations, enum SolveConflict solver)
{
	float cmax, best_cmax;
	char instance[8];
	int jobs, machines;
	char f_output[LINE];
	CHndFile file;
	CGraphCAPP *graph;
	CPieceCollection *cJ;
	CMachineCollection *cM;
	CGTAlgorithm_DRules *gt;
	clock_t start, end;

	file.SetPath(file_instance);
	file.ReadInstanceSize(&jobs, &machines);
	strcpy_s(instance, 8, file.GetFilename(false));
	cM = new CMachineCollection(machines);
	cJ = new CPieceCollection(jobs, machines, false);
	file.ReadMachineData(cM);
	file.ReadPieceData(cJ, *cM);

	sprintf_s(f_output, LINE, "..\\.estudo-computacional\\2.constructive-methods\\gnuplot\\%s-iteration-for-value-%s.dat", file.GetFilename(false),
		(solver == RANDOM) ? "RANDOMIZED" : (solver == SEP_RANDOM) ? "SEP_RANDOMIZED" : "NONE");
	ofstream fOut(f_output);

	CDataSet dataset(jobs);

	cJ->FillDataSet(&dataset, *cM);

	gt = new CGTAlgorithm_DRules(dataset);
	graph = new CGraphCAPP(jobs, machines, 0);

	best_cmax = numeric_limits<float>::infinity();
	cout << "INIT: 0.0 sec" << endl << endl;
	start = clock();
	for(int i = 1; i <= iterations; i++)
	{
		CSchedule schedule = gt->GenerateSchedule(solver);
		cJ->Create_NxM_Graph(graph, cM, schedule);
		cmax = graph->Makespan();

		if (cmax < best_cmax)
		{
			best_cmax = cmax;
#if TO_SCREEN
			cout << i << " => " << best_cmax << endl;
#endif

#if TO_FILE
			fOut << i << "\t" << best_cmax << endl;
#endif
		}

		graph->ClearGraph();
	}

	end = clock();
	cout << endl << "END: " << (double)(end - start) / CLOCKS_PER_SEC << " sec" << endl << endl;
	fOut.close();
}

//----------------------------------------------------------------------------------
//
void test_GTAlgorithm_bestone_value(const char *file_instance, int n_run, enum SolveConflict solve, CSchedule *best_schedule, float *best_cmax)
{
	float  cmax;
	int jobs, machines;
	CHndFile file;
	CDataSet *dataset;
	CGraphCAPP *graph;
	clock_t start, end;
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

	*best_cmax = numeric_limits<float>::infinity();
	cout << file.GetFilename(false);
	start = clock();
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
	end = clock();
	cout << ": ok" << " (" << (double)(end - start) / CLOCKS_PER_SEC << " sec)" << endl;	
	delete cM, cJ, gt, graph, dataset;
}

//----------------------------------------------------------------------------------
//
void test_GTAlfaRCL_bestone_value(const char *file_instance, int n_run, float alfa, enum SolveConflict solve, CSchedule *best_schedule, float *best_cmax)
{
	float  cmax;
	int jobs, machines;
	CHndFile file;
	CDataSet *dataset;
	CGraphCAPP *graph;
	clock_t start, end;
	CPieceCollection *cJ;
	CMachineCollection *cM;
	CGTAlgorithmAlfaRCL *gt;

	file.SetPath(file_instance);
	file.ReadInstanceSize(&jobs, &machines);
	cM = new CMachineCollection(machines);
	cJ = new CPieceCollection(jobs, machines, false);
	dataset = new CDataSet(jobs);
	file.ReadMachineData(cM);
	file.ReadPieceData(cJ, *cM);
	cJ->FillDataSet(dataset, *cM);
	gt = new CGTAlgorithmAlfaRCL(*dataset, alfa);
	graph = new CGraphCAPP(jobs, machines, 0);

	*best_cmax = numeric_limits<float>::infinity();
	cout << file.GetFilename(false);
	start = clock();
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
	end = clock();
	cout << ": ok" << " (" << (double)(end - start) / CLOCKS_PER_SEC << " sec)" << endl;
	delete cM, cJ, gt, graph, dataset;
}

//----------------------------------------------------------------------------------
//
void test_Randomized_ocurrences(CPieceCollection *cJ, CMachineCollection *cM, int n_run, const char *outname)
{
	float cmax;
	int jobs, machines;
	CGraphCAPP *graph;
	map<float, int> ocurrences;
	map<float, int>::iterator item;

	jobs = cJ->GetPieceCount();
	machines = cM->GetMachineCount();
	graph = new CGraphCAPP(jobs, machines, 0);

	CMultiStartAlgorithm random(jobs, machines);
	for (int i = 0; i < n_run; i++)
	{
		CSchedule schedule = random.GenerateSchedule(NxM_SCHEDULE);
		cJ->Create_NxM_Graph(graph, cM, schedule);
		cmax = graph->Makespan();
		graph->ClearGraph();

		item = ocurrences.find(cmax);
		if (item != ocurrences.end())
			item->second++;
		else
			ocurrences[cmax] = 1;

#if TO_SCREEN
		cout << "Cmax: " << cmax << endl << schedule << endl << endl;
#endif
	}

#if TO_FILE
	if (!outname) return;

	ofstream fOut(outname);
	item = ocurrences.begin();
	for (; item != ocurrences.end(); item++)
#if MAXIMIZE_Z
		fOut << (1 / item->first) << "\t" << item->second << endl;
#else
		fOut << item->first << "\t" << item->second << endl;
#endif
	fOut.close();
#endif

	delete graph;
}

//----------------------------------------------------------------------------------
//
void test_GTAlgorithm_ocurrences(CPieceCollection *cJ, CMachineCollection *cM, int n_run, enum SolveConflict solver, const char *outname)
{
	float cmax;
	int jobs, machines;
	CGraphCAPP *graph;
	CGTAlgorithmBase *gt;
	map<float, int> ocurrences;
	map<float, int>::iterator item;

	jobs = cJ->GetPieceCount();
	machines = cM->GetMachineCount();
	CDataSet dataset(jobs);

	cJ->FillDataSet(&dataset, *cM);

	gt = new CGTAlgorithm_DRules(dataset);
	graph = new CGraphCAPP(jobs, machines, 0);

	for (int i = 0; i < n_run; i++)
	{
		CSchedule schedule = gt->GenerateSchedule(solver);
		cJ->Create_NxM_Graph(graph, cM, schedule);
		cmax = graph->Makespan();
		graph->ClearGraph();

		item = ocurrences.find(cmax);
		if (item != ocurrences.end())
			item->second++;
		else
			ocurrences[cmax] = 1;

#if TO_SCREEN
		cout << "Cmax: " << cmax << endl << schedule << endl << endl;
#endif
	}

#if TO_FILE
	if (!outname) return;

	ofstream fOut(outname);
	item = ocurrences.begin();
	for (; item != ocurrences.end(); item++)
#if MAXIMIZE_Z
		fOut << (1 / item->first) << "\t" << item->second << endl;
#else
		fOut << item->first << "\t" << item->second << endl;
#endif
	fOut.close();
#endif

	delete graph;
}

//----------------------------------------------------------------------------------
//
void test_GTAlfaRCL_ocurrences(CPieceCollection *cJ, CMachineCollection *cM, int n_run, float alfa, enum SolveConflict solver, const char *outname)
{
	float cmax;
	int jobs, machines;
	CGraphCAPP *graph;
	CGTAlgorithmBase *gt;
	map<float, int> ocurrences;
	map<float, int>::iterator item;

	jobs = cJ->GetPieceCount();
	machines = cM->GetMachineCount();
	CDataSet dataset(jobs);

	cJ->FillDataSet(&dataset, *cM);

	gt = new CGTAlgorithmAlfaRCL(dataset, alfa);
	graph = new CGraphCAPP(jobs, machines, 0);

	for (int i = 0; i < n_run; i++)
	{
		CSchedule schedule = gt->GenerateSchedule(solver);
		cJ->Create_NxM_Graph(graph, cM, schedule);
		cmax = graph->Makespan();
		graph->ClearGraph();

		item = ocurrences.find(cmax);
		if (item != ocurrences.end())
			item->second++;
		else
			ocurrences[cmax] = 1;

#if TO_SCREEN
		cout << "Cmax: " << cmax << endl << schedule << endl << endl;
#endif
	}

#if TO_FILE
	if (!outname) return;

	ofstream fOut(outname);
	item = ocurrences.begin();
	for (; item != ocurrences.end(); item++)
#if MAXIMIZE_Z
		fOut << (1 / item->first) << "\t" << item->second << endl;
#else
		fOut << item->first << "\t" << item->second << endl;
#endif
	fOut.close();
#endif

	delete graph;
}