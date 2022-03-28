#include "CTerminator.h"
#include "CStatistics.h"
#include "CGrasp_PR.h"
#include "../jssp.model/CHndFile.h"
#include "../jssp.model/CPieceCollection.h"
#include "../jssp.model/CMachineCollection.h"
#include "experiments_GRASP_PR.h"

#define LINE 256

//----------------------------------------------------------------------------------
//
void run_Grasp_with_PathRelinking(char *namePR, map<char *, char *> data_files, CLocalSearchStrategy *localsearch, CNeighborhoobStrategy *neighbor, CTerminator *terminator, CPathRelinking *prelinking, float alfa, int seed)
{
	char filename[LINE];
	CStatistics stats;
	map<char *, char *>::iterator instance;

	sprintf_s(filename, LINE, "..\\.estudo-computacional\\5.grasp-path_relinking\\experiment #1\\%s_21_instances.txt", namePR);
	ofstream output(filename);
	
	cout << "[" << namePR << "]" << endl;
	instance = data_files.begin();
	for (; instance != data_files.end(); instance++)
	{
		cout << instance->first;
		stats = test_Grasp(instance->second, localsearch, neighbor, terminator, prelinking, alfa, seed);
		cout << " -> ok (" << stats.getElapsedTime() << ")" << endl;

		output << "[" << instance->first << "]" << endl;
		if (stats.getBestSolution().IsValid())
		{
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
		else
			cout << "Schedule invalid: " << stats.getBestSolution();
	}
	output.close();
	cout << endl << endl;
}

//----------------------------------------------------------------------------------
// experiment A (ttt-plot) - GRASP vs GRASP+PR
void runDistributionTime_A_Grasp_with_PR(const char *file_instance, CLocalSearchStrategy *localsearch, CNeighborhoobStrategy *neighbor, CTerminator *terminator, int n_trials)
{
	CStatistics stats;
	char filename[LINE];
	CHndFile file(file_instance);
	map<char *, char *>::iterator instance;
	//- GRASP Path-Relinking techniques -//
	CMixed_PR Mixed;
	CForward_PR Forward;
	CBackward_PR Backward;
	CBackAndForward_PR BackForward;

	//---------
	// FORWARD
	//---------
	sprintf_s(filename, LINE, "..\\.estudo-computacional\\5.grasp-path_relinking\\experiment #2 (ttt-plot) - GRASP vs GRASP+PR\\_%s_\\runtime_%s_forward.dat", file.GetFilename(false), file.GetFilename(false));
	ofstream outputF(filename);

	cout << "FORWARD" << endl;
	for (int run = 0; run < n_trials; run++)
	{
		cout << run << " ";
		stats = test_Grasp(file_instance, localsearch, neighbor, terminator, &Forward, 0.6, run);
		cout << stats.getElapsedTime() << " (" << stats.getBestScore() << ")" << endl;
		outputF << stats.getElapsedTime() << endl;
	}
	cout << endl << endl;
	outputF.close();

	//---------
	// BACKWARD
	//---------
	sprintf_s(filename, LINE, "..\\.estudo-computacional\\5.grasp-path_relinking\\experiment #2 (ttt-plot) - GRASP vs GRASP+PR\\_%s_\\runtime_%s_backward.dat", file.GetFilename(false), file.GetFilename(false));
	ofstream outputB(filename);

	cout << "BACKWARD" << endl;
	for (int run = 0; run < n_trials; run++)
	{
		cout << run << " ";
		stats = test_Grasp(file_instance, localsearch, neighbor, terminator, &Backward, 0.6, run);
		cout << stats.getElapsedTime() << " (" << stats.getBestScore() << ")" << endl;
		outputB << stats.getElapsedTime() << endl;
	}
	cout << endl << endl;
	outputB.close();

	//---------
	// BACK-AND-FORWARD
	//---------
	sprintf_s(filename, LINE, "..\\.estudo-computacional\\5.grasp-path_relinking\\experiment #2 (ttt-plot) - GRASP vs GRASP+PR\\_%s_\\runtime_%s_back-and-forward.dat", file.GetFilename(false), file.GetFilename(false));
	ofstream outputBF(filename);

	cout << "BACK-AND-FORWARD" << endl;
	for (int run = 0; run < n_trials; run++)
	{
		cout << run << " ";
		stats = test_Grasp(file_instance, localsearch, neighbor, terminator, &BackForward, 0.6, run);
		cout << stats.getElapsedTime() << " (" << stats.getBestScore() << ")" << endl;
		outputBF << stats.getElapsedTime() << endl;
	}
	cout << endl << endl;
	outputBF.close();

	//---------
	// MIXED
	//---------
	sprintf_s(filename, LINE, "..\\.estudo-computacional\\5.grasp-path_relinking\\experiment #2 (ttt-plot) - GRASP vs GRASP+PR\\_%s_\\runtime_%s_mixed.dat", file.GetFilename(false), file.GetFilename(false));
	ofstream outputM(filename);

	cout << "MIXED" << endl;
	for (int run = 0; run < n_trials; run++)
	{
		cout << run << " ";
		stats = test_Grasp(file_instance, localsearch, neighbor, terminator, &Mixed, 0.6, run);
		cout << stats.getElapsedTime() << " (" << stats.getBestScore() << ")" << endl;
		outputM << stats.getElapsedTime() << endl;
	}
	cout << endl << endl;
	outputM.close();
}

//----------------------------------------------------------------------------------
// experiment #3 (ttt-plot) - GRASP+PR
void runDistributionTime_B_Grasp_with_PR(const char *file_instance, CLocalSearchStrategy *localsearch, CNeighborhoobStrategy *neighbor, CTerminator *terminator, int n_trials)
{
	CStatistics stats;
	char filename[LINE];
	CHndFile file(file_instance);
	map<char *, char *>::iterator instance;
	//- GRASP Path-Relinking techniques -//
	CMixed_PR Mixed;
	CForward_PR Forward;
	CBackward_PR Backward;
	CBackAndForward_PR BackForward;

	//---------
	// FORWARD
	//---------
	sprintf_s(filename, LINE, "..\\.estudo-computacional\\5.grasp-path_relinking\\experiment #3 (ttt-plot) - GRASP+PR\\_%s_\\runtime_%s_forward.dat", file.GetFilename(false), file.GetFilename(false));
	ofstream outputF(filename);

	cout << "FORWARD" << endl;
	for (int run = 0; run < n_trials; run++)
	{
		cout << run << " ";
		stats = test_Grasp(file_instance, localsearch, neighbor, terminator, &Forward, 0.6, run);
		cout << stats.getElapsedTime() << " (" << stats.getBestScore() << ")" << endl;
		outputF << stats.getElapsedTime() << endl;
	}
	cout << endl << endl;
	outputF.close();

	//---------
	// BACKWARD
	//---------
	sprintf_s(filename, LINE, "..\\.estudo-computacional\\5.grasp-path_relinking\\experiment #3 (ttt-plot) - GRASP+PR\\_%s_\\runtime_%s_backward.dat", file.GetFilename(false), file.GetFilename(false));
	ofstream outputB(filename);

	cout << "BACKWARD" << endl;
	for (int run = 0; run < n_trials; run++)
	{
		cout << run << " ";
		stats = test_Grasp(file_instance, localsearch, neighbor, terminator, &Backward, 0.6, run);
		cout << stats.getElapsedTime() << " (" << stats.getBestScore() << ")" << endl;
		outputB << stats.getElapsedTime() << endl;
	}
	cout << endl << endl;
	outputB.close();

	//---------
	// BACK-AND-FORWARD
	//---------
	sprintf_s(filename, LINE, "..\\.estudo-computacional\\5.grasp-path_relinking\\experiment #3 (ttt-plot) - GRASP+PR\\_%s_\\runtime_%s_back-and-forward.dat", file.GetFilename(false), file.GetFilename(false));
	ofstream outputBF(filename);

	cout << "BACK-AND-FORWARD" << endl;
	for (int run = 0; run < n_trials; run++)
	{
		cout << run << " ";
		stats = test_Grasp(file_instance, localsearch, neighbor, terminator, &BackForward, 0.6, run);
		cout << stats.getElapsedTime() << " (" << stats.getBestScore() << ")" << endl;
		outputBF << stats.getElapsedTime() << endl;
	}
	cout << endl << endl;
	outputBF.close();

	//---------
	// MIXED
	//---------
	sprintf_s(filename, LINE, "..\\.estudo-computacional\\5.grasp-path_relinking\\experiment #3 (ttt-plot) - GRASP+PR\\_%s_\\runtime_%s_mixed.dat", file.GetFilename(false), file.GetFilename(false));
	ofstream outputM(filename);

	cout << "MIXED" << endl;
	for (int run = 0; run < n_trials; run++)
	{
		cout << run << " ";
		stats = test_Grasp(file_instance, localsearch, neighbor, terminator, &Mixed, 0.6, run);
		cout << stats.getElapsedTime() << " (" << stats.getBestScore() << ")" << endl;
		outputM << stats.getElapsedTime() << endl;
	}
	cout << endl << endl;
	outputM.close();
}

//----------------------------------------------------------------------------------
//
CStatistics test_Grasp(const char *file_instance, CLocalSearchStrategy *localsearch, CNeighborhoobStrategy *neighbor, CTerminator *terminator, CPathRelinking *prelinking, float alfa, int seed)
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

	CGrasp_PR grasp(localsearch, neighbor, terminator, prelinking, alfa);
	grasp.SetSeed(seed);
	grasp.UserData(&cJ, &cM);

	schedule = grasp.Execute(&cmax);
	return grasp.Statistics();
}