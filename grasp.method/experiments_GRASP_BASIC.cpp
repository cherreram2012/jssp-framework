#include "CTerminator.h"
#include "CStatistics.h"
#include "CGraspBasic.h"
#include "experiments_GRASP_BASIC.h"
#include "../jssp.model/CHndFile.h"
#include "../jssp.model/CPieceCollection.h"
#include "../jssp.model/CMachineCollection.h"

#define LINE 128

//----------------------------------------------------------------------------------
//
void runReference_Alfa_Grasp (map<char *, char *> data_files, CLocalSearchStrategy *localsearch, CNeighborhoobStrategy *neighbor, CTerminator *terminator, float alfa, int seed)
{
	CStatistics stats;
	map<char *, char *>::iterator instance;

	ofstream output("..\\.estudo-computacional\\4.grasp-implementation\\experiment #1 (adjust Alfa)\\reference-alfa_instances-6_alfa-0.2.txt");
	instance = data_files.begin();
	for (; instance != data_files.end(); instance++)
	{
		cout << instance->first;
		stats = test_Grasp(instance->second, localsearch, neighbor, terminator, alfa, seed);
		cout << " -> ok (" << stats.getElapsedTime() << ")" << endl;

		output << "[" << instance->first << "]" << endl;
		output << "<statistics>:" << endl
			<< "  - Best Cmax: " << stats.getBestScore() << endl
			<< "  - Best Average: " << stats.getAverageBestScore() << endl
			<< "  - Worst Cmax: " << stats.getWorstScore() << endl
			<< "  - Total Average: " << stats.getAverageScore() << endl
			<< "  - No. Improves: " << stats.getNoImproves() << endl
			<< "  - Iterations: " << stats.getIterations() << endl
			<< "  - Restarts: " << stats.getNoRestarts() << endl
			<< "  - Time: " << stats.getElapsedTime() << " (sec)" << endl << endl
			<< stats.getBestSolution() << endl << endl;
	}
	output.close();
}

//----------------------------------------------------------------------------------
//
void runGrasp_for_InstanceTeste(map<char *, char *> data_files, CLocalSearchStrategy *localsearch, CNeighborhoobStrategy *neighbor, CTerminator *terminator, float alfa, int seed)
{	
	CStatistics stats;
	char filename[LINE];
	map<char *, char *>::iterator instance;

	sprintf_s(filename, LINE, "..\\.estudo-computacional\\4.grasp-implementation\\experiment #2 (using Alfa)\\grasp_for_instances_teste_using-%.1f.txt", alfa);
	ofstream output(filename);

	instance = data_files.begin();
	for (; instance != data_files.end(); instance++)
	{
		cout << instance->first;
		stats = test_Grasp(instance->second, localsearch, neighbor, terminator, alfa, seed);
		cout << " -> ok (" << stats.getElapsedTime() << ")" << endl;

		output << "[" << instance->first << "]" << endl;
		output << "<statistics>:" << endl
			<< "  - Best Cmax: " << stats.getBestScore() << endl
			<< "  - Best Average: " << stats.getAverageBestScore() << endl
			<< "  - Worst Cmax: " << stats.getWorstScore() << endl
			<< "  - Total Average: " << stats.getAverageScore() << endl
			<< "  - No. Improves: " << stats.getNoImproves() << endl
			<< "  - Iterations: " << stats.getIterations() << endl
			<< "  - Time: " << stats.getElapsedTime() << " (sec)" << endl << endl
			<< stats.getBestSolution() << endl << endl;
	}
	
	output.close();
}

//----------------------------------------------------------------------------------
//
void runAdjust_Alfa_Grasp(const char *file_instance, CLocalSearchStrategy *localsearch, CNeighborhoobStrategy *neighbor, CTerminator *terminator, int seed)
{
	const int alfaCount = 1;
	//const float alfaArray[alfaCount] = { 0.4, 0.6, 0.8, 1.0 };
	const float alfaArray[alfaCount] = { 0.6 };
	CStatistics stats;
	char filename[LINE];
	CHndFile file(file_instance);

	sprintf_s(filename, LINE, "..\\.estudo-computacional\\4.grasp-implementation\\experiment #1 (adjust Alfa)\\adjust-alfa_instance-%s.txt", file.GetFilename(false));
	ofstream output(filename);
	cout << "[" << file.GetFilename(false) << "]" << endl;
	for (int i = 0; i < alfaCount; i++)
	{
		cout << "Alfa: " << alfaArray[i];
		stats = test_Grasp(file_instance, localsearch, neighbor, terminator, alfaArray[i], seed);
		cout << " -> ok (" << stats.getElapsedTime() << ")" << endl;

		output << "[" << alfaArray[i] << "]" << endl;
		output << "<statistics>:" << endl
			<< "  - Best Cmax: " << stats.getBestScore() << endl
			<< "  - Best Average: " << stats.getAverageBestScore() << endl
			<< "  - Worst Cmax: " << stats.getWorstScore() << endl
			<< "  - Total Average: " << stats.getAverageScore() << endl
			<< "  - No. Improves: " << stats.getNoImproves() << endl
			<< "  - Iterations: " << stats.getIterations() << endl
			<< "  - Time: " << stats.getElapsedTime() << " (sec)" << endl << endl
			<< stats.getBestSolution() << endl << endl;
	}
	cout << endl;
	output.close();
}

//----------------------------------------------------------------------------------
//
void runDistributionTime(const char *file_instance, CLocalSearchStrategy *localsearch, CNeighborhoobStrategy *neighbor, CTerminator *terminator, int n_trials)
{
	const int alfaCount = 5;
	const float alfaArray[alfaCount] = { 0.2, 0.4, 0.6, 0.8, 1.0 };
	CStatistics stats;
	char filename[LINE];
	CHndFile file(file_instance);

	for (int i = 0; i < alfaCount; i++)
	{
		sprintf_s(filename, LINE, "..\\.estudo-computacional\\4.grasp-implementation\\experiment #3 (ttt-plot)\\_%s_\\runtime_%s_using-%.1f.dat", file.GetFilename(false), file.GetFilename(false), alfaArray[i]);
		ofstream output(filename);

		cout << "[" << file.GetFilename(false) << "] -> (" << alfaArray[i] << ") " << endl;
		for (int run = 0; run < n_trials; run++)
		{
			cout << run << " ";
			stats = test_Grasp(file_instance, localsearch, neighbor, terminator, alfaArray[i], run);
			cout << stats.getElapsedTime() << " (" << stats.getBestScore() <<")" <<endl;
			output << stats.getElapsedTime() << endl;
		}

		cout << endl;
		output.close();
	}
}

//----------------------------------------------------------------------------------
//
CStatistics test_Grasp(const char *file_instance, CLocalSearchStrategy *localsearch, CNeighborhoobStrategy *neighbor, CTerminator *terminator, float alfa, int seed)
{
	float cmax;
	int jobs, machines;
	CSchedule schedule;
	CHndFile file(file_instance);
	file.ReadInstanceSize(&jobs, &machines);
	CMachineCollection cM(machines);
	CPieceCollection cJ(jobs, machines, false);
	file.ReadMachineData(&cM);
	file.ReadPieceData(&cJ, cM);

	CGraspBasic grasp(localsearch, neighbor, terminator, alfa);
	grasp.SetSeed(seed);
	grasp.UserData(&cJ, &cM);

	schedule = grasp.Execute(&cmax);
	return grasp.Statistics();
}

