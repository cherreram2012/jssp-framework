#include <map>
#include <iostream>
#include "experiments__etapa3.h"
#include "experiments__etapa6.h"
#include "CLocalSearchForExperiment.h"
#include "../jssp.model/CHndFile.h"
#include "../jssp.model/CPieceCollection.h"
#include "../jssp.model/CMachineCollection.h"
#include "../jssp.heuristics/CGTAlgorithm.h"
#include "../jssp.heuristics/CNeighborhood.h"
#include "../jssp.heuristics/CMultiStartAlgorithm.h"
#include "../chm.utilities/CustomConsole.h"
#include "../chm.utilities/PrecisionClock.h"

using namespace std;

//----------------------------------------------------------------------------------
//
int main (void)
{
	map<char *, char *> instances = {										 // +----------------------+------------------+------------------+------------------+
		/*** Complexidade baixa ***/											 // | -- INSTANTCE_SIZE -- | --LOWER_BOUND -- | --UPPER_BOUND -- | -- BEST FOUND -- |
		{"ft06", "..\\datafile\\JSSP\\jobshop\\ft06.txt"}, // |				 6 x 6				 |			  *55				|				----			 |			  *55				|
		{"la01", "..\\datafile\\JSSP\\jobshop\\la01.txt"}, // |				10 x 5				 |			 ----				|				----			 |			 *666		 		|
		{"la02", "..\\datafile\\JSSP\\jobshop\\la02.txt"}, // |				10 x 5				 |			 ----				|				----			 |			 *655				|
		{"la03", "..\\datafile\\JSSP\\jobshop\\la03.txt"}, // |				10 x 5				 |			 ----				|				----			 |			 *597				|
		{"la04", "..\\datafile\\JSSP\\jobshop\\la04.txt"}, // |				10 x 5				 |			 ----				|				----			 |			 *590				|
		{"la05", "..\\datafile\\JSSP\\jobshop\\la05.txt"}, // |				10 x 5				 |			 ----				|				----			 |			 *593				|
		{"la06", "..\\datafile\\JSSP\\jobshop\\la06.txt"}, // |				15 x 5				 |			 ----				|				----			 |			 *926				|
		/*** Complexidade media ***/											 // +----------------------+------------------+------------------+------------------+
		{"ft20", "..\\datafile\\JSSP\\jobshop\\ft20.txt"}, // |				20 x 5				 |		  *1165				|				----			 |			*1165				|
		{"la15", "..\\datafile\\JSSP\\jobshop\\la15.txt"}, // |				20 x 5				 |			 ----				|				----			 |			*1207				|
		{"la16", "..\\datafile\\JSSP\\jobshop\\la16.txt"}, // |				10 x 10				 |			 ----				|				----			 |			 *945				|
		{"la17", "..\\datafile\\JSSP\\jobshop\\la17.txt"}, // |				10 x 10				 |			 ----				|				----			 |			 *784				|
		{"la18", "..\\datafile\\JSSP\\jobshop\\la18.txt"}, // |				10 x 10				 |			 ----				|				----			 |			 *848				|
		{"la19", "..\\datafile\\JSSP\\jobshop\\la19.txt"}, // |				10 x 10				 |			 ----				|				----			 |			 *842				|
		{"la20", "..\\datafile\\JSSP\\jobshop\\la20.txt"}, // |				10 x 10				 |			 ----				|				----			 |			 *902				|
		/*** Complexidade alta ***/												 // +----------------------+------------------+------------------+------------------+
		{"ft10", "..\\datafile\\JSSP\\jobshop\\ft10.txt"}, // |				10 x 10				 |			 *930				|				----			 |			 *930				|
		{"la38", "..\\datafile\\JSSP\\jobshop\\la38.txt"}, // |				15 x 15				 |			 1196				|				----			 |			 1209				|
		{"la39", "..\\datafile\\JSSP\\jobshop\\la39.txt"}, // |				15 x 15				 |			 ----				|				----			 |			 1233				|
		{"la40", "..\\datafile\\JSSP\\jobshop\\la40.txt"}, // |				15 x 15				 |			 1222				|				----			 |			*1222				|
		{"abz7", "..\\datafile\\JSSP\\jobshop\\abz7.txt"}, // |				20 x 15				 |			  656				|				 656			 |			----				|
		{"abz8", "..\\datafile\\JSSP\\jobshop\\abz8.txt"}, // |				20 x 15				 |			  646				|				 665			 |			----				|
		{"abz9", "..\\datafile\\JSSP\\jobshop\\abz9.txt"}  // |				20 x 15				 |			  662				|				 679			 |			----				|
		};																								 // +----------------------+------------------+------------------+------------------+

	map<char *, char *> test = {									 
		//{"ft06", "..\\datafile\\JSSP\\jobshop\\ft06.txt"}, 
		{"ft20", "..\\datafile\\JSSP\\jobshop\\ft20.txt"}, 
		//{"ft10", "..\\datafile\\JSSP\\jobshop\\ft10.txt"}, 
	};												

	CustomConsole::ForegroundColor(ColorDarkCyan);
	cout << "+---------------------------------------------------+" << endl 
			 << "| PROJETO: Metodos busqueda local para o Scheduling |" << endl 
			 << "+---------------------------------------------------+" << endl << endl;
	CustomConsole::LastState();

	try
	{
#if 1
		/********** RUN COMPUTATIONAL TEST **********/
		//runExperiment_LocalSearch_value__etapa3(test, 8);
		//runExperiment_LocalSearch_value__etapa3(instances, 1);

		//runExperiment_CmaxVsStep_FirstImproving__etapa3(instances["ft06"], RANDOM, 1);  // OK
		//runExperiment_CmaxVsStep_BestImproving__etapa3 (instances["ft06"], RANDOM, 1);	// OK
		//runExperiment_CmaxVsStep_FirstImproving__etapa3(instances["ft20"], RANDOM, 8);	// OK
		//runExperiment_CmaxVsStep_BestImproving__etapa3 (instances["ft20"], RANDOM, 8);	// OK
		//runExperiment_CmaxVsStep_FirstImproving__etapa3(instances["ft10"], RANDOM, 8);	// OK
		//runExperiment_CmaxVsStep_BestImproving__etapa3 (instances["ft10"], RANDOM, 8);	// OK

		//runExperiment_CmaxVsStep_FirstImproving__etapa3(instances["ft06"], SEP_RANDOM, 0);  // OK
		//runExperiment_CmaxVsStep_BestImproving__etapa3 (instances["ft06"], SEP_RANDOM, 0);  // OK
		//runExperiment_CmaxVsStep_FirstImproving__etapa3(instances["ft20"], SEP_RANDOM, 12); // OK
		//runExperiment_CmaxVsStep_BestImproving__etapa3 (instances["ft20"], SEP_RANDOM, 12); // OK
		//runExperiment_CmaxVsStep_FirstImproving__etapa3(instances["ft10"], SEP_RANDOM, 9);  // OK
		//runExperiment_CmaxVsStep_BestImproving__etapa3 (instances["ft10"], SEP_RANDOM, 9);  // OK

		//runExperiment_CmaxVsStep_IBUIT__etapa6(instances["ft06"], RANDOM, 1);	// OK
		//runExperiment_CmaxVsStep_IBUIT__etapa6(instances["ft10"], RANDOM, 8);

		//runExperiment_CmaxVsStep_IBUIT__etapa6(instances["ft06"], SEP_RANDOM, 0);	// OK
		runExperiment_CmaxVsStep_IBUIT__etapa6(instances["ft20"], SEP_RANDOM, 12);
		//runExperiment_CmaxVsStep_IBUIT__etapa6(instances["ft10"], SEP_RANDOM, 9); 
#else		
		/********** DEBUG SECTION for Schedule-Swap-technique **********/		
		/*i = 2;
		j = 5;
		jobs = 4;
		machines = 3;
		
		CMultiStartAlgorithm R(job, machine);
		schedule = R.GenerateSchedule(NxM_SCHEDULE);
		cout << "J: " << jobs << " | M: " << machines << endl;
		cout << "sA: " << i << ", sB: " << j << endl << endl;
		cout << "init: " << schedule << endl;
		//schedule.SwapAtom(4, 10);
		schedule.SwapSubsetM(i, j);
		cout << "swap: " << schedule << endl << endl;*/

		/********** DEBUG SECTION for Neighborhoob Technique **********/
		/*i = 2;
		j = 5;
		jobs = 4;
		machines = 3;
		CNeighborhoobBase *neighborhoob;

		neighborhoob = new CAdjacentLinearNeighbor();
		CMultiStartAlgorithm R(job, machine);
		schedule = R.GenerateSchedule(NxM_SCHEDULE);
		CustomConsole::ForegroundColor(ColorDarkYellow);
		cout << schedule << endl << endl; 
		CustomConsole::LastState();

		neighborhoob->SetEntryPoint(schedule);
		int c = neighborhoob->Cardinality();
		for (int i = 0; i < c; i++) {
			neigh = neighborhoob->VisitNeighbor(i);
			cout << neigh << endl << endl;
		}
		delete neighborhoob;*/

		/********** DEBUG SECTION for Local Search Technique **********/	
		/*float cmax;
		int jobs, machines;
		CSchedule schedule, neigh;
		CHndFile file(instances["ft20"]);
		file.ReadInstanceSize(&jobs, &machines);
		CDataSet dataset(jobs);
		CMachineCollection cM(machines);
		CPieceCollection cJ(jobs, machines, false);
		CGraphCAPP graph(jobs, machines, 0);
		file.ReadMachineData(&cM);
		file.ReadPieceData(&cJ, cM);
		cJ.FillDataSet(&dataset, cM);
		CGTAlgorithmClassic gt(dataset);
		
		//unsigned seq[] = {3, 6, 6, 1, 2, 3, 1, 6, 2, 4, 3, 5, 4, 5, 1, 3, 4, 2, 6, 3, 5, 4, 4, 6, 2, 6, 5, 1, 3, 5, 4, 5, 1, 2, 1, 2 };
		//schedule.SetDimension(NxM_SCHEDULE, 4, 5);
		//schedule.SetDimension(NxM_SCHEDULE, jobs, machines);
		//for (int i = 0; i < jobs * machines; i++) schedule.AddToSchedule(seq[i]);
		//gt.SetSeed(2);
		schedule = gt.GenerateSchedule(RANDOMIZED);
		cJ.Create_NxM_Graph(&graph, &cM, schedule);
		cmax = graph.Makespan();
		CustomConsole::ForegroundColor(ColorGreen);
		cout << "[constructive]:" << endl << "Cmax: " << cmax << endl << schedule;
		CustomConsole::LastState();

		//CMultiStartAlgorithm R(jobs, machines);
		//schedule = R.GenerateSchedule(NxM_SCHEDULE);
		CLinearAdjacentNeighbor N1;
		CCircularAdjacentNeighbor N2;
		CLinearNoAdjacentNeighbor N3;
		CLocalSearch localsearch(&N1, &cJ, &cM);
		localsearch.FirstImproving(schedule);
		//localsearch.BestImproving(schedule);
		cout << endl;*/

		/********** DEBUG SECTION for improving instances tf10, tf20 **********/
		float cmax;
		int jobs, machines;
		CSchedule schedule;
		CHndFile file(instances["ft20"]);
		file.ReadInstanceSize(&jobs, &machines);
		CDataSet dataset(jobs);
		CMachineCollection cM(machines);
		CPieceCollection cJ(jobs, machines, false);
		CGraphCAPP graph(jobs, machines, 0);
		file.ReadMachineData(&cM);
		file.ReadPieceData(&cJ, cM);
		cJ.FillDataSet(&dataset, cM);
		CGTAlgorithmClassic gt(dataset);
		CLinearAdjacentNeighbor N1;
		CCircularAdjacentNeighbor N2;
		CLinearNoAdjacentNeighbor N3;
		CLocalSearch localsearch_N1(&N1, &cJ, &cM);
		CLocalSearch localsearch_N2(&N2, &cJ, &cM);
		CLocalSearch localsearch_N3(&N3, &cJ, &cM);
		CLocalSearch::LOCAL_SEARCH_STATISTCS stats;
		int seeds = 30;

		/*cout << "GT CLASSICO" << endl;
		cout << "+-----------------------------------------------------------------------+" << endl;
		cout << "| Seed  " << "| Phase 1 \t" << "| Linear \t" << "| Circular \t" << "| NoLinear \t|" << endl;
		cout << "+-----------------------------------------------------------------------+" << endl;
		for (int i = 0; i <= seeds; i++)
		{
			gt.SetSeed(i);
			schedule = gt.GenerateSchedule(RANDOMIZED);
			cJ.Create_NxM_Graph(&graph, &cM, schedule);
			cmax = graph.Makespan();
			graph.ClearGraph();						
			cout << "  "  << i <<  "\t " << cmax;			

			//localsearch_N1.FirstImproving(schedule, &cmax);
			localsearch_N1.BestImproving(schedule, &cmax);
			stats = localsearch_N1.Statistics();
			cout << "\t\t  " << cmax << "\t { " << stats.StepsCount << " }";

			//localsearch_N2.FirstImproving(schedule, &cmax);
			localsearch_N2.BestImproving(schedule, &cmax);
			stats = localsearch_N2.Statistics();
			cout << "\t\t  " << cmax << "\t { " << stats.StepsCount << " }";

			//localsearch_N3.FirstImproving(schedule, &cmax);
			localsearch_N3.BestImproving(schedule, &cmax);
			stats = localsearch_N3.Statistics();
			cout << "\t\t  " << cmax << "\t { " << stats.StepsCount << " }";

			cout << endl;
		}*/

		cout <<endl << endl << "GT SEMI-SEP" << endl;
		cout << "+-----------------------------------------------------------------------+" << endl;
		cout << "| Seed  " << "| Phase 1 \t" << "| Linear \t" << "| Circular \t" << "| NoLinear \t|" << endl;
		cout << "+-----------------------------------------------------------------------+" << endl;
		for (int i = 0; i <= seeds; i++)
		{
			gt.SetSeed(i);
			schedule = gt.GenerateSchedule(SEP_RANDOMIZED);
			cJ.Create_NxM_Graph(&graph, &cM, schedule);
			cmax = graph.Makespan();
			graph.ClearGraph();
			cout << "  " << i << "\t " << cmax;

			//localsearch_N1.FirstImproving(schedule, &cmax);
			localsearch_N1.BestImproving(schedule, &cmax);
			stats = localsearch_N1.Statistics();
			cout << "\t\t  " << cmax << "\t { " << stats.StepsCount << " }";

			//localsearch_N2.FirstImproving(schedule, &cmax);
			localsearch_N2.BestImproving(schedule, &cmax);
			stats = localsearch_N2.Statistics();
			cout << "\t\t  " << cmax << "\t { " << stats.StepsCount << " }";

			//localsearch_N3.FirstImproving(schedule, &cmax);
			localsearch_N3.BestImproving(schedule, &cmax);
			stats = localsearch_N3.Statistics();
			cout << "\t\t  " << cmax << "\t { " << stats.StepsCount << " }";

			cout << endl;
		}

		cout << endl;

		/********** DEBUG SECTION for Macro Implementation **********/
		/*double a;
		TIME_HIGH_PERFORMANCE(for (int i = 0; i <= 10000; i++);, a);

		cout << a << endl << endl;*/

#endif
	}
	catch (std::exception e) {
		CustomConsole::ForegroundColor(ColorRed);
		cout << "EXCEPTION CAPTURADA: " << endl << " - " << e.what() << endl;
		CustomConsole::LastState();
	}

	//system("pause");	
	return 0;
}
