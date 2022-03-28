#include "CTerminator.h"
#include "CStatistics.h"
#include "CGrasp_PR.h"
#include "../jssp.model/CHndFile.h"
#include "../jssp.model/CPieceCollection.h"
#include "../jssp.model/CMachineCollection.h"
#include "experiments_GRASP_PR_IBUIT.h"

#define LINE 256

//----------------------------------------------------------------------------------
//
void run_Grasp_with_PathRelinking_IBUIT(char *namePR, map<char *, char *> data_files, CLocalSearchStrategy *localsearch, CNeighborhoobStrategy *neighbor, CTerminator *terminator, CPathRelinking *prelinking, float alfa, int seed)
{
	char filename[LINE];
	CStatistics stats;
	map<char *, char *>::iterator instance;

	sprintf_s(filename, LINE, "..\\.estudo-computacional\\6.improving-some-thing-in-grasp\\experiment #2 (grasp+pr+ibuit)\\%s_21_instances.txt", namePR);
	ofstream output(filename);

	cout << "[" << namePR << "]" << endl;
	instance = data_files.begin();
	for (; instance != data_files.end(); instance++)
	{
		cout << instance->first;
		stats = test_Grasp_IBUIT(instance->second, localsearch, neighbor, terminator, prelinking, alfa, seed);
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
//
CStatistics test_Grasp_IBUIT(const char *file_instance, CLocalSearchStrategy *localsearch, CNeighborhoobStrategy *neighbor, CTerminator *terminator, CPathRelinking *prelinking, float alfa, int seed)
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