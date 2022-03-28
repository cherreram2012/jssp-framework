#include <map>
#include <cstring>
#include <iostream>
#include "CTerminator.h"
#include "CStatistics.h"
#include "CGrasp_PR.h"
#include "CGraspBasic.h"
#include "CPathRelinking.h"
#include "experiments_GRASP_PR.h"
#include "experiments_GRASP_BASIC.h"
#include "experiments_GRASP_PR_IBUIT.h"
#include "../jssp.model/CHndFile.h"
#include "../jssp.model/CPieceCollection.h"
#include "../jssp.model/CMachineCollection.h"
#include "../jssp.heuristics/CSetOperator.h"
#include "../jssp.heuristics/CLocalSearch.h"
#include "../jssp.heuristics/CNeighborhood.h"
#include "../jssp.heuristics/CRestrictedNeighbor.h"
#include "../chm.utilities/CustomConsole.h"
#include "../chm.utilities/PrecisionClock.h"

using namespace std;

void Experimentos_Etapa4__projeto(void);
void Experimentos_Etapa5__projeto(void);
void Experimentos_Etapa6__projeto(void);

void DebugSection__CreateGraph(void);
void DebugSection__GraspBasico(void);
void DebugSection__GraspPR(void);
void DebugSection__Pathrelinking(void);
void DebugSection__Memcpy(void);

void initilize(int array[], int size);
bool compare(int a[], int b[], int size);
void copy_value(int dest[], int orig[], int size);
void copy_memory(int dest[], int orig[], int size);

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
	
	{"la21", "..\\datafile\\JSSP\\jobshop\\la21.txt"},
	{"la24", "..\\datafile\\JSSP\\jobshop\\la24.txt"},
	{"la25", "..\\datafile\\JSSP\\jobshop\\la25.txt"},

  {"la38", "..\\datafile\\JSSP\\jobshop\\la38.txt"}, // |				15 x 15				 |			 1196				|				----			 |			 1209				|
	{"la39", "..\\datafile\\JSSP\\jobshop\\la39.txt"}, // |				15 x 15				 |			 ----				|				----			 |			 1233				|
	{"la40", "..\\datafile\\JSSP\\jobshop\\la40.txt"}, // |				15 x 15				 |			 1222				|				----			 |			*1222				|
	{"abz7", "..\\datafile\\JSSP\\jobshop\\abz7.txt"}, // |				20 x 15				 |			  656				|				 656			 |			----				|
	{"abz8", "..\\datafile\\JSSP\\jobshop\\abz8.txt"}, // |				20 x 15				 |			  646				|				 665			 |			----				|
	{"abz9", "..\\datafile\\JSSP\\jobshop\\abz9.txt"}  // |				20 x 15				 |			  662				|				 679			 |			----				|
};																								 // +----------------------+------------------+------------------+------------------+

map<char *, char *> instances_ajuste = {						 // +----------------------+------------------+------------------+------------------+
	/*** Complexidade baixa ***/											 // | -- INSTANTCE_SIZE -- | --LOWER_BOUND -- | --UPPER_BOUND -- | -- BEST FOUND -- |	
	{"la01", "..\\datafile\\JSSP\\jobshop\\la01.txt"}, // |				10 x 5				 |			 ----				|				----			 |			 *666		 		|
	{"la02", "..\\datafile\\JSSP\\jobshop\\la02.txt"}, // |				10 x 5				 |			 ----				|				----			 |			 *655				|	
	/*** Complexidade media ***/											 // +----------------------+------------------+------------------+------------------+	
	{"la15", "..\\datafile\\JSSP\\jobshop\\la15.txt"}, // |				20 x 5				 |			 ----				|				----			 |			*1207				|
	{"la16", "..\\datafile\\JSSP\\jobshop\\la16.txt"}, // |				10 x 10				 |			 ----				|				----			 |			 *945				|
	/*** Complexidade alta ***/												 // +----------------------+------------------+------------------+------------------+
	{"la38", "..\\datafile\\JSSP\\jobshop\\la38.txt"}, // |				15 x 15				 |			 1196				|				----			 |			 1209				|	
	{"abz7", "..\\datafile\\JSSP\\jobshop\\abz7.txt"}, // |				20 x 15				 |			  656				|				 656			 |			----				|
};																									 // +----------------------+------------------+------------------+------------------+

map<char *, char *> instances_teste = {						 // +----------------------+------------------+------------------+------------------+
	/*** Complexidade baixa ***/											 // | -- INSTANTCE_SIZE -- | --LOWER_BOUND -- | --UPPER_BOUND -- | -- BEST FOUND -- |
	{"ft06", "..\\datafile\\JSSP\\jobshop\\ft06.txt"}, // |				 6 x 6				 |			  *55				|				----			 |			  *55				|
	{"la03", "..\\datafile\\JSSP\\jobshop\\la03.txt"}, // |				10 x 5				 |			 ----				|				----			 |			 *597				|
	{"la04", "..\\datafile\\JSSP\\jobshop\\la04.txt"}, // |				10 x 5				 |			 ----				|				----			 |			 *590				|
	{"la05", "..\\datafile\\JSSP\\jobshop\\la05.txt"}, // |				10 x 5				 |			 ----				|				----			 |			 *593				|
	{"la06", "..\\datafile\\JSSP\\jobshop\\la06.txt"}, // |				15 x 5				 |			 ----				|				----			 |			 *926				|
	/*** Complexidade media ***/											 // +----------------------+------------------+------------------+------------------+
	{"ft20", "..\\datafile\\JSSP\\jobshop\\ft20.txt"}, // |				20 x 5				 |		  *1165				|				----			 |			*1165				|
	{"la17", "..\\datafile\\JSSP\\jobshop\\la17.txt"}, // |				10 x 10				 |			 ----				|				----			 |			 *784				|
	{"la18", "..\\datafile\\JSSP\\jobshop\\la18.txt"}, // |				10 x 10				 |			 ----				|				----			 |			 *848				|
	{"la19", "..\\datafile\\JSSP\\jobshop\\la19.txt"}, // |				10 x 10				 |			 ----				|				----			 |			 *842				|
	{"la20", "..\\datafile\\JSSP\\jobshop\\la20.txt"}, // |				10 x 10				 |			 ----				|				----			 |			 *902				|
	/*** Complexidade alta ***/												 // +----------------------+------------------+------------------+------------------+
	{"ft10", "..\\datafile\\JSSP\\jobshop\\ft10.txt"}, // |				10 x 10				 |			 *930				|				----			 |			 *930				|
	{"la39", "..\\datafile\\JSSP\\jobshop\\la39.txt"}, // |				15 x 15				 |			 ----				|				----			 |			 1233				|
	{"la40", "..\\datafile\\JSSP\\jobshop\\la40.txt"}, // |				15 x 15				 |			 1222				|				----			 |			*1222				|	
	{"abz8", "..\\datafile\\JSSP\\jobshop\\abz8.txt"}, // |				20 x 15				 |			  646				|				 665			 |			----				|
	{"abz9", "..\\datafile\\JSSP\\jobshop\\abz9.txt"}  // |				20 x 15				 |			  662				|				 679			 |			----				|
};																								 // +----------------------+------------------+------------------+------------------+

map<char *, char *> test = {						 
	{"ft10", "..\\datafile\\JSSP\\jobshop\\ft10.txt"}, 
	{"la38", "..\\datafile\\JSSP\\jobshop\\la38.txt"}, 
	{"la39", "..\\datafile\\JSSP\\jobshop\\la39.txt"}, 
	{"la40", "..\\datafile\\JSSP\\jobshop\\la40.txt"}, 
	{"abz7", "..\\datafile\\JSSP\\jobshop\\abz7.txt"}, 
	{"abz8", "..\\datafile\\JSSP\\jobshop\\abz8.txt"}, 
	{"abz9", "..\\datafile\\JSSP\\jobshop\\abz9.txt"}  
};

int main (void)
{
	CustomConsole::ForegroundColor(ColorDarkCyan);
	cout << "+------------------------------------------------------+" << endl
			 << "| PROJETO: Metaheuristica GRASP aplicada ao Scheduling |" << endl
			 << "+------------------------------------------------------+" << endl << endl;
	CustomConsole::LastState();
	
	try
	{
#if 1
		/********** RUN COMPUTATIONAL TEST **********/
		//Experimentos_Etapa6__projeto();
		//Experimentos_Etapa5__projeto();
		Experimentos_Etapa4__projeto();
#else		
		//DebugSection__GraspBasico();
		DebugSection__GraspPR();
		//DebugSection__Pathrelinking();
		//DebugSection__Memcpy();
		//DebugSection__CreateGraph();
#endif
	}
	catch (std::exception e) {
		CustomConsole::ForegroundColor(ColorRed);
		cout << "EXCEPTION CAPTURADA: " << endl << " - " << e.what() << endl;
		CustomConsole::LastState();
	}
	
	cout << endl << endl;
	system("pause");

	return 0;
}

//----------------------------------------------------------------------------------
// ETAPA #5. IMPLEMENTACION DE GRASP + RESTART, GRASP + EvPR, GRASP+IBUIT.
//----------------------------------------------------------------------------------
void Experimentos_Etapa6__projeto(void)
{
	//- Local search strategies -//
	CFirstImprovingIBUIT FI_ibuit;
	CBestImprovingIBUIT BI_ibuit;
	//- Neigborhood strategies -//
	CLinearNoAdjacentNeighbor N3;
	//- GRASP Stop Condictons objects -//
	StopByTime byTime(0.444);
	StopByTarget byTarget(55);
	StopByIteration byIteration(100);
	//- GRASP Path-Relinking techniques -//
	CMixed_PR Mixed;
	CForward_PR Forward;
	CBackward_PR Backward;
	CBackAndForward_PR BackForward;

	/* Experimento #2: aplicação das tecnicas Forward, Backward, Back-and-forward e Mixed para as 21 instancias usando UBUIT. */
	run_Grasp_with_PathRelinking_IBUIT("backward", test, &FI_ibuit, &N3, &byIteration, &Backward, 0.6, 1);
	run_Grasp_with_PathRelinking_IBUIT("back-and-forward", test, &FI_ibuit, &N3, &byIteration, &BackForward, 0.6, 1);
	run_Grasp_with_PathRelinking_IBUIT("mixed", test, &FI_ibuit, &N3, &byIteration, &Mixed, 0.6, 1);
	run_Grasp_with_PathRelinking_IBUIT("forward", test, &FI_ibuit, &N3, &byIteration, &Forward, 0.6, 1);

	/* Experimento #3: grasp+path-relinking com restarts */
}

//----------------------------------------------------------------------------------
// ETAPA #4. IMPLEMENTACION DE GRASP + PATH-RELINKING.
//----------------------------------------------------------------------------------
void Experimentos_Etapa5__projeto(void)
{
	//- Local search strategies -//
	CFirstImprovingSearch FI;
	//- Neigborhood strategies -//
	CLinearNoAdjacentNeighbor N3;
	//- GRASP Stop Condictons objects -//
	StopByTime byTime(0.444);
	StopByTarget byTarget(55);
	StopByIteration byIteration(3000);
	//- GRASP Path-Relinking techniques -//
	CMixed_PR Mixed;
	CForward_PR Forward;
	CBackward_PR Backward;
	CBackAndForward_PR BackForward;

	/* Experimento #1: aplicação das tecnicas Forward, Backward, Back-and-forward e Mixed para as 21 instancias. */
	run_Grasp_with_PathRelinking("forward", instances, &FI, &N3, &byIteration, &Forward, 0.6, 1);
	run_Grasp_with_PathRelinking("backward", instances, &FI, &N3, &byIteration, &Backward, 0.6, 1);
	run_Grasp_with_PathRelinking("back-and-forward", instances, &FI, &N3, &byIteration, &BackForward, 0.6, 1);
	run_Grasp_with_PathRelinking("mixed", instances, &FI, &N3, &byIteration, &Mixed, 0.6, 1);

	/* Experimento #2-A: Distribution time */
	//StopByTarget byTgt1A(58);
	//runDistributionTime_A_Grasp_with_PR(instances["ft06"], &FI, &N3, &byTgt1A, 250);

	//StopByTarget byTgt2A(1413);
	//runDistributionTime_A_Grasp_with_PR(instances["ft20"], &FI, &N3, &byTgt2A, 250);

	//StopByTarget byTgt3A(1082);
	//runDistributionTime_A_Grasp_with_PR(instances["ft10"], &FI, &N3, &byTgt3A, 250);

	/* Experimento #2-B: Distribution time */
	//StopByTarget byTgt1B(55);
	//runDistributionTime_B_Grasp_with_PR(instances["ft06"], &FI, &N3, &byTgt1B, 250);

	//StopByTarget byTgt3B(1056);
	//runDistributionTime_B_Grasp_with_PR(instances["ft10"], &FI, &N3, &byTgt3B, 250);

	//StopByTarget byTgt2B(1365);
	//runDistributionTime_B_Grasp_with_PR(instances["ft20"], &FI, &N3, &byTgt2B, 250);
}

//----------------------------------------------------------------------------------
// ETAPA #3. IMPLEMENTACION DE GRASP BASICO.
//----------------------------------------------------------------------------------
void Experimentos_Etapa4__projeto(void)
{
	//- Local search strategies -//
	CBestImprovingSearch BI;
	CFirstImprovingSearch FI;
	//- Neigborhood strategies -//
	CLinearAdjacentNeighbor N1;
	CCircularAdjacentNeighbor N2;
	CLinearNoAdjacentNeighbor N3;
	//- GRASP Stop Condictons objects -//
	StopByTime byTime(0.444);
	StopByTarget byTarget(1100);
	StopByIteration byIteration(3000);

	/* Experimento #1-A: geração de valores de referencia para o parametro Alfa = 0.2. */
	//runReference_Alfa_Grasp(instances_ajuste, &FI, &N3, &byIteration, 0.2, 1);

	/* Experimento #1-B: geração dos valores para o ajusto do parametro Alfa = {0.4, 0.6, 0.8, 1.0} */
	//StopByTime byT1(1.18);
	runAdjust_Alfa_Grasp(instances["la21"], &FI, &N3, &byIteration, 1);

	//StopByTime byT2(1.05);
	//runAdjust_Alfa_Grasp(instances["la02"], &FI, &N3, &byT2, 1);

	//StopByTime byT3(2.40);
	//runAdjust_Alfa_Grasp(instances["la15"], &FI, &N3, &byT3, 1);

	//StopByTime byT4(18.10);
	//runAdjust_Alfa_Grasp(instances["la16"], &FI, &N3, &byT4, 1);

	//StopByTime byT5(124.22);
	//runAdjust_Alfa_Grasp(instances["la38"], &FI, &N3, &byT5, 1);

	//StopByTime byT6(248.22);
	//runAdjust_Alfa_Grasp(instances["abz7"], &FI, &N3, &byT6, 1);

	/* Experimento #2: Solução das instancias de test usando o valor de alfa determinado no experimento #1 */
	//runGrasp_for_InstanceTeste(instances_teste, &FI, &N3, &byIteration, 0.6, 1);

	/* Experimento #3: Distribution time */
	//StopByTarget byTgt1(58);
	//runDistributionTime(instances["ft06"], &FI, &N3, &byTgt1, 250);

	//StopByTarget byTgt2(1413);
	//runDistributionTime(instances["ft20"], &FI, &N3, &byTgt2, 250);

	//StopByTarget byTgt3(1082);
	//runDistributionTime(instances["ft10"], &FI, &N3, &byTgt3, 250);
}

//==================================================================================
// SECCIONES DE DEBUGUEO....
//==================================================================================
void DebugSection__CreateGraph(void)
{
	float cmax;
	int jobs, machines;
	CSchedule schedule;
	CStatistics stats;
	//- JSSP objects -//
	CHndFile file(instances["ft06"]);
	file.ReadInstanceSize(&jobs, &machines);
	CMachineCollection cM(machines);
	CPieceCollection cJ(jobs, machines, false);
	file.ReadMachineData(&cM);
	file.ReadPieceData(&cJ, cM);
	CDataSet dataset(jobs);
	cJ.FillDataSet(&dataset, cM);
	CGTAlgorithm_DRules gt(dataset);
	//CGTAlgorithmAlfaRCL gt(dataset, 1.0);
	CGraphCAPP graph(jobs, machines, 0);
	CGraphCAPP graph_ibuit(jobs, machines, 0, true);

	double time;
	gt.SetSeed(0);
	//schedule = gt.GenerateSchedule(SEP_GREEDY);
	schedule = gt.GenerateSchedule(SEP_RANDOM);

	TIME_HIGH_PERFORMANCE(cJ.Create_NxM_Graph(&graph, &cM, schedule), time);
	TIME_HIGH_PERFORMANCE(cJ.Create_NxM_Graph(&graph_ibuit, &cM, schedule), time);
	//TIME_HIGH_PERFORMANCE(cJ.Create_JSS_Graph(&graph, &cM, schedule), time);

	//fixed(cout);
	//cout << "graph creation time: " << time << endl << endl;
	//defaultfloat(cout);

	cmax = graph.Makespan();
	cout << "[SEM IBUIT]" << endl << "Cmax: " << cmax << endl << graph.GetSchedule() << endl << endl;
	graph.ClearGraph();
	//cout << "obteniendo el schedule que contiene el grafo: " << endl << graph.GetSchedule();

	cmax = graph_ibuit.Makespan();
	cout << "[COM IBUIT]" << endl << "Cmax: " << cmax << endl << graph_ibuit.GetSchedule() << endl << endl;
	//cout << "obteniendo el schedule que contiene el grafo: " << endl << graph.GetSchedule();
	graph.ClearGraph();
}

void DebugSection__GraspBasico(void)
{
	/********** DEBUG SECTION for GRASP Development **********/
	float cmax;
	int jobs, machines;
	CSchedule schedule;
	CStatistics stats;
	//- JSSP objects -//
	CHndFile file(instances["ft10"]);
	file.ReadInstanceSize(&jobs, &machines);
	CMachineCollection cM(machines);
	CPieceCollection cJ(jobs, machines, false);
	file.ReadMachineData(&cM);
	file.ReadPieceData(&cJ, cM);
	//- Local search strategies -//
	CFirstImprovingSearch FI;
	CBestImprovingSearch BI;
	//- Neigborhood strategies -//
	CLinearAdjacentNeighbor N1;
	CCircularAdjacentNeighbor N2;
	CLinearNoAdjacentNeighbor N3;
	//- GRASP Stop Conditions objects -//
	StopByTime byTime(0.444);
	StopByTarget byTarget(55);
	StopByIteration byIteration(2);
	//- GRASP Path-Relinking techniques -//
	CMixed_PR Mixed;
	CForward_PR Forward;
	CBackward_PR Backward;
	CBackAndForward_PR BackForward;

	CGraspBasic grasp(&FI, &N3, &byIteration, 1.0);
	grasp.SetSeed(1);
	grasp.UserData(&cJ, &cM);
	schedule = grasp.Execute(&cmax);
	stats = grasp.Statistics();

	if (schedule.IsValid())
	{
		cout << "Cmax: " << cmax << endl << schedule << endl << endl;
		cout << "[Statistics]:" << endl
			<< "  - Best Cmax: " << stats.getBestScore() << endl
			<< "  - Best Average: " << stats.getAverageBestScore() << endl
			<< "  - Worst Cmax: " << stats.getWorstScore() << endl
			<< "  - Total Average: " << stats.getAverageScore() << endl
			<< "  - No. Improves: " << stats.getNoImproves() << endl
			<< "  - Iterations: " << stats.getIterations() << endl
			<< "  - Time: " << stats.getElapsedTime() << " (sec)";
	}
	else
	{
		CustomConsole::ForegroundColor(ColorRed);
		std::cout << "ERROR!!!!" << endl << schedule << endl << endl << "[-] no es valido.";
		CustomConsole::LastState();
	}

	cout << endl << endl;
}

void DebugSection__GraspPR(void)
{
	/********** DEBUG SECTION for GRASP Development **********/
	float cmax;
	int jobs, machines;
	CSchedule schedule;
	CStatistics stats;
	//- JSSP objects -//
	CHndFile file(instances["ft10"]);
	file.ReadInstanceSize(&jobs, &machines);
	CMachineCollection cM(machines);
	CPieceCollection cJ(jobs, machines, false);
	file.ReadMachineData(&cM);
	file.ReadPieceData(&cJ, cM);
	//- Local search strategies -//
	CFirstImprovingSearch FI;
	CBestImprovingSearch BI;
	CFirstImprovingIBUIT FI_ibuit;
	CBestImprovingIBUIT BI_ibuit;
	//- Neigborhood strategies -//
	CLinearAdjacentNeighbor N1;
	CCircularAdjacentNeighbor N2;
	CLinearNoAdjacentNeighbor N3;
	//- GRASP Stop Conditions objects -//
	StopByTime byTime(0.444);
	StopByTarget byTarget(55);
	StopByIteration byIteration(500);
	//- GRASP Path-Relinking techniques -//
	CMixed_PR Mixed;
	CForward_PR Forward;
	CBackward_PR Backward;
	CBackAndForward_PR BackForward;

	CGrasp_PR grasp(&FI_ibuit, &N3, &byIteration, &BackForward, 0.6);
	grasp.SetSeed(1);
	grasp.UserData(&cJ, &cM);
	schedule = grasp.Execute(&cmax);
	stats = grasp.Statistics();

	if (schedule.IsValid())
	{
		cout << "Cmax: " << cmax << endl << schedule << endl << endl;
		cout << "[Statistics]:" << endl
			<< "  - Best Cmax: " << stats.getBestScore() << endl
			<< "  - Best Average: " << stats.getAverageBestScore() << endl
			<< "  - Worst Cmax: " << stats.getWorstScore() << endl
			<< "  - Total Average: " << stats.getAverageScore() << endl
			<< "  - No. Improves: " << stats.getNoImproves() << endl
			<< "  - Iterations: " << stats.getIterations() << endl
			<< "  - Time: " << stats.getElapsedTime() << " (sec)";
	}
	else
	{
		CustomConsole::ForegroundColor(ColorRed);
		std::cout << "ERROR!!!!" << endl << schedule << endl << endl << "[-] no es valido.";
		CustomConsole::LastState();
	}

	cout << endl << endl;
}

void DebugSection__Pathrelinking(void)
{
	/********** DEBUG SECTION for SYMMETRIC DIFFERENCE OPERATION **********/
	/*float cmax;
	int jobs, machines;
	CSchedule s1, s2;
	CHndFile file(instances["ft06"]);
	file.ReadInstanceSize(&jobs, &machines);
	CMachineCollection cM(machines);
	CPieceCollection cJ(jobs, machines, false);
	file.ReadMachineData(&cM);
	file.ReadPieceData(&cJ, cM);
	CDataSet dataset(jobs);
	cJ.FillDataSet(&dataset, cM);
	CGTAlgorithmEnriched gt(dataset);
	CGraphCAPP graph(jobs, machines, 0);

	gt.SetSeed(1);
	s1 = gt.GenerateSchedule(RANDOMIZED);
	cJ.Create_NxM_Graph(&graph, &cM, s1);
	cmax = graph.Makespan();
	cout << "Cmax S1: " << cmax << endl << s1 << endl << endl;
	graph.ClearGraph();

	gt.SetSeed(4);
	s2 = gt.GenerateSchedule(RANDOMIZED);
	cJ.Create_NxM_Graph(&graph, &cM, s2);
	cmax = graph.Makespan();
	cout << "Cmax S2: " << cmax << endl << s2 << endl << endl;
	graph.ClearGraph();

	vector<CSetOperator::ATOM> Vdiff;
	map<int, vector<CSetOperator::ATOM>> Mdiff;
	vector<CSetOperator::ATOM>::iterator itVd;
	map<int, vector<CSetOperator::ATOM>>::iterator itMd;

	Mdiff = CSetOperator::Diff(s1, s2);
	CustomConsole::ForegroundColor(ColorDarkGreen);
	cout << endl << "[S1 - S2] Step: " << Mdiff.size() << endl << endl;
	CustomConsole::LastState();
	itMd = Mdiff.begin();
	for (; itMd != Mdiff.end(); itMd++)
	{
		Vdiff = itMd->second;
		itVd = Vdiff.begin();
		cout << "diff: (" << Vdiff.size() << ") -> ";
		for (; itVd != Vdiff.end(); itVd++) cout << (*itVd).idS1 << "[" << (*itVd).index << "]  ";
		cout << endl;
	}

	Mdiff = CSetOperator::Diff(s2, s1);
	CustomConsole::ForegroundColor(ColorDarkGreen);
	cout << endl << endl << "[S2 - S1] Step: " << Mdiff.size() << endl << endl;
	CustomConsole::LastState();
	itMd = Mdiff.begin();
	for (; itMd != Mdiff.end(); itMd++)
	{
		Vdiff = itMd->second;
		itVd = Vdiff.begin();
		cout << "diff: (" << Vdiff.size() << ") -> ";
		for (; itVd != Vdiff.end(); itVd++) cout << (*itVd).idS1 << "[" << (*itVd).index << "]  ";
		cout << endl;
	}

	vector<CSetOperator::DUPLEX_ATOM> Vsym;
	map<int, vector<CSetOperator::DUPLEX_ATOM>> Msym;
	vector<CSetOperator::DUPLEX_ATOM>::iterator itVs;
	map<int, vector<CSetOperator::DUPLEX_ATOM>>::iterator itMs;

	Msym = CSetOperator::SymmetricDiff(s1, s2);
	CustomConsole::ForegroundColor(ColorDarkGreen);
	cout << endl << endl << "[(S1 - S2) U (S2 - S1)] Step: " << Msym.size() << endl << endl;
	CustomConsole::LastState();
	itMs = Msym.begin();
	for (; itMs != Msym.end(); itMs++)
	{
		Vsym = itMs->second;
		itVs = Vsym.begin();
		cout << "diff: (" << Vsym.size() << ") -> ";
		for (; itVs != Vsym.end(); itVs++) cout << "(" << (*itVs).idS1 << ", " << (*itVs).idS2 << ")  ";
		cout << endl;
	}*/

	/********** DEBUG SECTION for PATH RELINKING TECHNIQUE **********/
	float cmax;
	int jobs, machines;
	CSchedule s1, s2;
	CHndFile file(instances["ft06"]);
	file.ReadInstanceSize(&jobs, &machines);
	CMachineCollection cM(machines);
	CPieceCollection cJ(jobs, machines, false);
	file.ReadMachineData(&cM);
	file.ReadPieceData(&cJ, cM);
	CDataSet dataset(jobs);
	cJ.FillDataSet(&dataset, cM);
	CGTAlgorithm_DRules gt(dataset);
	CGraphCAPP graph(jobs, machines, 0);

	/*cout << "En el MAIN -------------------------" << endl << endl;
	gt.SetSeed(1);
	s1 = gt.GenerateSchedule(RANDOMIZED);
	cJ.Create_NxM_Graph(&graph, &cM, s1);
	cmax = graph.Makespan();
	cout << "Cmax S1: " << cmax << endl << s1 << endl << endl;
	graph.ClearGraph();

	gt.SetSeed(4);
	s2 = gt.GenerateSchedule(RANDOMIZED);
	cJ.Create_NxM_Graph(&graph, &cM, s2);
	cmax = graph.Makespan();
	cout << "Cmax S2: " << cmax << endl << s2 << endl << endl;
	graph.ClearGraph();
	cout << "------------------------------------" << endl << endl;

	RestrictedNeighbor RN;
	RN.UserData(&cJ, &cM);
	cout << "dist (S1->S2): " << RN.Moves(s1, s2) << endl << endl;
	cout << "Dentro Argmin -------------------------" << endl << endl;
	RN.Argmin(&cmax);
	cout << "------------------------------------" << endl << endl;*/

	/********** DEBUG SECTION for PATH RELIKING STATIC SCHEDULE  **********/
	// (ft06) - ERROR en M-conjunto 0. Arreglado 02/06/2019.
	unsigned int seq1[] = { 1, 2, 1, 3, 3, 6, 2, 6, 4, 6, 3, 5, 4, 1, 2, 5, 4, 3, 3, 4, 1, 4, 3, 5, 5, 1, 5, 6, 4, 6, 6, 5, 1, 2, 2, 2 };
	unsigned int seq2[] = { 1, 4, 4, 5, 1, 6, 6, 5, 3, 1, 5, 3, 4, 6, 3, 5, 4, 1, 2, 3, 6, 1, 5, 2, 5, 6, 2, 4, 1, 6, 3, 2, 4, 3, 2, 2 };

	// (ft06) - ERROR en M-conjunto 3. Arreglado 04/06/2019 (11:23am).
	//unsigned int seq1[] = { 4, 1, 6, 3, 6, 5, 2, 2, 2, 1, 6, 3, 3, 1, 5, 5, 3, 4, 6, 2, 3, 4, 3, 6, 5, 6, 1, 4, 2, 1, 4, 5, 5, 1, 4, 2 };
	//unsigned int seq2[] = { 4, 1, 6, 3, 6, 5, 1, 1, 5, 5, 5, 1, 4, 3, 4, 4, 2, 5, 2, 6, 2, 6, 6, 6, 3, 2, 2, 3, 4, 1, 5, 2, 1, 4, 3, 3 };

	// (ft06) - ERROR en M-conjunto 5. Arreglado 04/06/2019 (6:15pm).
	//unsigned int seq1[] = { 4, 1, 6, 3, 6, 5, 6, 1, 3, 1, 6, 2, 4, 3, 4, 4, 2, 5, 6, 2, 1, 5, 3, 6, 3, 2, 2, 3, 4, 1, 5, 5, 5, 1, 4, 2 };
	//unsigned int seq2[] = { 4, 1, 6, 3, 6, 5, 1, 1, 5, 5, 5, 1, 4, 3, 4, 4, 2, 5, 2, 6, 2, 6, 6, 6, 3, 2, 2, 3, 4, 1, 5, 2, 1, 4, 3, 3 };

	// (la01) - ERROR en M-conjunto 0. Arreglado 04/06/2019 (10:06pm).
	//unsigned int seq1[] = { 1, 9, 5, 8, 10, 10, 3, 1, 1, 6, 9, 2, 9, 8, 2, 4, 10, 2, 4, 10, 4, 10, 2, 8, 6, 3, 9, 1, 9, 6, 2, 5, 5, 7, 6, 8, 1, 5, 4, 4, 7, 7, 3, 6, 7, 3, 7, 5, 3, 8 };
	//unsigned int seq2[] = { 7, 2, 6, 6, 1, 7, 8, 3, 5, 5, 7, 3, 8, 9, 8, 7, 8, 1, 5, 2, 3, 6, 4, 2, 3, 3, 2, 9, 7, 8, 4, 2, 10, 4, 5, 6, 10, 6, 1, 10, 4, 10, 9, 1, 4, 5, 9, 1, 10, 9 };

	s1.SetDimension(NxM_SCHEDULE, jobs, machines);
	s2.SetDimension(NxM_SCHEDULE, jobs, machines);
	for (int i = 0; i < jobs * machines; i++)
	{
		s1.AddToSchedule(seq1[i]);
		s2.AddToSchedule(seq2[i]);
	}

	/*gt.SetSeed(1);
	s1 = gt.GenerateSchedule(RANDOMIZED);
	gt.SetSeed(4);
	s2 = gt.GenerateSchedule(RANDOMIZED);*/
	cout << "En el MAIN -------------------------" << endl << endl;
	cJ.Create_NxM_Graph(&graph, &cM, s1);
	cmax = graph.Makespan();
	cout << "Cmax S1: " << cmax << endl << s1 << endl << endl;
	graph.ClearGraph();

	cJ.Create_NxM_Graph(&graph, &cM, s2);
	cmax = graph.Makespan();
	cout << "Cmax S2: " << cmax << endl << s2 << endl << endl;
	graph.ClearGraph();
	cout << "------------------------------------" << endl << endl;

	RestrictedNeighbor RN;
	vector<CSetOperator::DUPLEX_ATOM> Vsym;
	map<int, vector<CSetOperator::DUPLEX_ATOM>> Msym;
	vector<CSetOperator::DUPLEX_ATOM>::iterator itVs;
	map<int, vector<CSetOperator::DUPLEX_ATOM>>::iterator itMs;
	RN.UserData(&cJ, &cM);
	Msym = CSetOperator::SymmetricDiff(s1, s2);
	cout << "dist (S1->S2): " << RN.Moves(s1, s2) << endl << endl;
	itMs = Msym.begin();
	for (; itMs != Msym.end(); itMs++)
	{
		Vsym = itMs->second;
		itVs = Vsym.begin();
		cout << "m [" << itMs ->first << "]: (" << Vsym.size() << ") -> ";
		for (; itVs != Vsym.end(); itVs++) cout << "(" << (*itVs).idS1 << ", " << (*itVs).idS2 << ")  ";
		cout << endl;
	}

	cout << endl << endl << "|== Invocando Argmin ==============|" << endl;
	RN.Argmin(&cmax);
	cout << "------------------------------------" << endl << endl;
}

void DebugSection__Memcpy(void)
{
	/********** DEBUG SECTION for memcpy  **********/
	const int size = 1000000;
	int *A, *B, *C;
	double time = 0.0f;

	A = new int[size];
	B = new int[size];
	C = new int[size];

	initilize(A, size);

	cout << "SIZE: " << size << endl << endl;

	TIME_HIGH_PERFORMANCE(copy_value(B, A, size), time)
	//cout.precision(10);
	cout << "(array): A & B (copy by value):" << endl;
	cout << "time: " << fixed << time << endl;
	if (compare(A, B, size))
	{
		CustomConsole::ForegroundColor(ColorGreen);
		cout << "[ok] - Los array son iguales" << endl;
		CustomConsole::LastState();
	}
	else
	{
		CustomConsole::ForegroundColor(ColorRed);
		cout << "[fail] - Los array son distintos" << endl;
		CustomConsole::LastState();
	}

	TIME_HIGH_PERFORMANCE(copy_memory(C, A, size), time)
	//cout.precision(10);
	cout << endl << endl << "(array): A & C (copy by memory):" << endl;
	cout << "time: " << fixed << time << endl;

	if (compare(A, B, size))
	{
		CustomConsole::ForegroundColor(ColorGreen);
		cout << "[ok] - Los array son iguales" << endl;
		CustomConsole::LastState();
	}
	else
	{
		CustomConsole::ForegroundColor(ColorRed);
		cout << "[fail] - Los array son distintos" << endl;
		CustomConsole::LastState();
	}

	delete A, B, C;
}

void initilize(int array[], int size)
{
	for (int i = 0; i < size; i++) array[i] = i + 1;
}

bool compare(int a[], int b[], int size)
{
	for (int i = 0; i < size; i++) 
		if (a[i] != b[i]) return false;

	return true;
}

void copy_value(int dest[], int orig[], int size)
{
	for (int i = 0; i < size; i++) dest[i] = orig[i];
}

void copy_memory(int dest[], int orig[], int size)
{
	memcpy(dest, orig, sizeof(int) * size);
}