#include <conio.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <ctype.h>

#include "../jssp.model/CHndFile.h"
#include "../jssp.model/CGraphCAPP.h"
#include "../jssp.model/CMachineGroup.h"
#include "../jssp.model/CPieceCollection.h"
#include "../jssp.model/CMachineCollection.h"
#include "../jssp.model/CWorkCenterCollection.h"
#include "../chm.utilities/gceUtiles.h"
#include "set-path-data.h"

// My GA module Header file
//#include <ga/ga.h>
#include "gaModule.h"

#define MEJORES 10
#define SAVE_TO_FILE 0

FILE *file;
unsigned *Sequence;
CHndFile hndFile;
CGraphCAPP *Graph, *tmpGraph;
CPieceCollection *CollectionP;
CMachineCollection *CollectionM;

bool MemberVector    ( const GenomesVector V, const GAGenome &G );
bool EqualGenome		 ( const GA1DArrayGenome <unsigned> &G1, const GA1DArrayGenome <unsigned> &G2 );
void PrintIndividual ( unsigned generation, const GAGenome &G, bool to_file = 0 );
void PrintResumen    ( STRUCT_PROBLEM_DATA inst, const GASimpleGA &ga, const GAPopulation &Pop, const GA1DArrayGenome <unsigned> &worse_genome, float elapsed_time);

// Temporal
void ConvertArrayToGenome (GA1DArrayGenome <unsigned> &G, const unsigned *Array);

//////////////////////////////////////////////////////////////////////////////
// Main Function
void main (void)
{
	STRUCT_PROBLEM_DATA Inst;
	PRIORITY_LIST_PTR priority_list;

	if (SAVE_TO_FILE) file = fopen("evolution.txt", "w"); 

	SetDataPath(&hndFile);
	
	printf("FILE: %s\n\n", hndFile.GetPath());
	hndFile.ReadInstanceSize(&Inst.Size.Piece, &Inst.Size.Machine);	

	CollectionM	= new CMachineCollection (Inst.Size.Machine);
	CollectionP	= new CPieceCollection   (Inst.Size.Piece, Inst.Size.Machine, false);
		
	hndFile.ReadMachineData(CollectionM);								
	hndFile.ReadPieceData(CollectionP, *CollectionM);
	CollectionP->GetRecirculationData(&Inst.Recirc);
	CollectionP->GetPriorityList(&priority_list);

	Graph		 = new CGraphCAPP (Inst.Size.Piece, Inst.Size.Machine, Inst.Recirc.tRecirc);
	tmpGraph = new CGraphCAPP (Inst.Size.Piece, Inst.Size.Machine, Inst.Recirc.tRecirc);
	Sequence = new unsigned [Inst.Size.Piece * Inst.Size.Machine + Inst.Recirc.tRecirc];

	Graph->ApplyIBUIT(true);
	Graph->SetImproveType(itAtLeastOne);
	Graph->SetImproveObjetive(1, 0, 0, 0, 0, 0, 0, 0, 0);
	
#if 0 // Seccion de Depuracion.....................	
	//int recirc = CollectionP->GetRecirculateOpCount();
	//GA1DArrayGenome <unsigned> genome(0);
	//unsigned *seq = new unsigned [Inst.Piece + Inst.Machine + recirc];

	float cmax;
	const unsigned *tmp;
	unsigned seq[] = {4, 15, 13, 6, 2, 8, 9, 1, 5, 3, 11, 10, 12, 14, 7, 6, 10, 5, 15, 11, 5, 3, 6, 12, 7, 12, 11, 2, 13, 9, 6, 14, 3, 13, 7, 9, 14, 3, 11, 8, 7, 8, 8, 8, 10, 4, 5, 1, 3, 15, 12, 6, 13, 1, 9, 15, 3, 15, 4, 5, 7, 11, 7, 2, 14, 3, 10, 10, 9, 6, 9, 4, 1, 5, 5, 3, 1, 4, 11, 2, 6, 12, 14, 4, 7, 2, 9, 2, 6, 15, 13, 7, 14, 5, 1, 8, 4, 13, 12, 10, 13, 2, 15, 14, 11, 15, 13, 1, 8, 14, 12, 9, 11, 6, 12, 10, 1, 10, 2, 4, 9, 8};
		
	CSPGraph Graph00(Inst.Size.Piece, Inst.Size.Machine, Inst.Recirc.tRecirc ,false);
	CollectionP->Create_NxM_Graph(&Graph00, CollectionM, seq);
	cmax = Graph00.Makespan();
	tmp = Graph00.GetSequence();
	printf("CMax: %.1f\n", cmax);
	printf("Antes:\n");
	for (int i = 0; i < Inst.Size.Piece * Inst.Size.Machine + Inst.Recirc.tRecirc; i++) printf("%d ", tmp[i]);
	printf("\n\n");

	//Graph->SetLinkToGenome
	CollectionP->Create_NxM_Graph(Graph, CollectionM, seq);
	Graph->ApplyIBUIT(true);
	cmax = Graph->Makespan();
	printf("CMax: %.1f\n", cmax);
	tmp = Graph->GetSequence();
	printf("Despues:\n");
	for (int i = 0; i < Inst.Size.Piece * Inst.Size.Machine + Inst.Recirc.tRecirc; i++) printf("%d ", tmp[i]);
	printf("\n\n");

	GA1DArrayGenome <unsigned> G(Inst.Size.Piece * Inst.Size.Machine + Inst.Recirc.tRecirc);
	G.userData(&Inst);
	ConvertArrayToGenome(G, seq);
	PrintIndividual(1, G, false);*/

	//---------------------------------------------------------------------
	int step = 0;
	float cmax = 0;
	double time;

	// NO borrar, mostrar a Jose
	//unsigned seq[] = {47,34,23,20,37,7,65,72,35,85,57,55,31,3,44,84,39,76,5,67,61,14,28,71,75,32,98,54,62,83,6,59,73,42,88,81,21,69,9,29,100,82,33,96,56,48,43,95,74,8,92,18,94,66,90,68,87,22,40,49,17,27,11,26,97,30,58,78,64,63,60,79,46,12,86,91,16,50,19,13,99,52,53,93,1,15,89,41,4,25,24,80,51,45,70,2,38,77,10,36,51,57,72,91,37,76,69,98,64,30,78,87,89,65,61,4,8,63,56,41,46,26,40,60,67,50,5,1,54,90,73,45,94,42,70,92,81,12,21,33,19,24,99,96,2,80,34,75,35,47,83,20,62,44,38,17,13,53,85,6,52,59,55,43,86,15,28,11,95,25,100,68,48,32,58,82,18,7,66,27,16,88,71,22,97,74,10,49,36,14,23,77,39,3,84,93,31,79,9,29,59,41,28,42,85,68,15,79,54,91,10,46,66,81,90,78,45,80,18,86,40,20,71,3,67,74,12,49,63,16,52,87,82,19,14,72,53,100,48,57,36,30,73,55,39,21,94,56,98,38,6,75,62,95,1,84,64,97,76,25,23,8,69,29,70,65,93,83,2,13,92,22,51,27,26,44,60,96,9,17,99,7,58,43,88,11,33,5,50,34,37,47,32,61,77,89,35,31,24,4,18,71,97,49,57,59,86,56,34,35,12,2,64,5,16,27,22,87,37,36,61,4,70,52,98,66,60,72,53,1,62,26,47,94,73,79,74,25,81,100,69,21,44,99,46,50,45,9,54,39,23,40,89,42,48,78,84,55,63,38,29,83,13,88,24,51,91,82,6,75,20,77,14,33,58,10,80,3,68,93,65,31,90,67,76,95,32,15,43,8,19,96,30,11,28,17,92,85,7,41,62,30,84,10,40,82,78,77,35,59,90,36,47,71,56,57,32,98,48,49,67,21,79,13,81,39,52,29,66,9,55,15,53,45,26,54,89,85,20,19,46,23,94,22,16,72,70,87,91,12,38,11,80,100,75,99,74,64,76,18,73,63,43,2,4,5,25,97,37,1,83,68,3,8,65,33,41,42,17,93,34,61,28,24,31,7,95,86,50,92,27,60,88,6,51,14,96,69,58,44,53,64,96,70,36,76,43,71,41,97,67,1,77,32,81,21,26,25,12,24,35,19,73,83,93,16,95,31,82,92,54,57,20,3,65,88,44,55,69,91,48,68,18,29,90,39,52,94,86,37,15,62,56,6,60,5,47,42,66,45,51,34,22,23,61,75,14,27,78,89,13,59,87,8,79,100,74,98,63,2,50,40,7,33,80,10,99,30,11,72,84,58,4,46,28,85,17,38,9,49,48,87,54,37,89,75,3,86,20,50,16,38,47,98,84,61,18,21,8,82,4,56,25,33,51,19,39,35,83,99,6,28,63,64,13,34,9,79,67,60,58,2,32,66,55,45,14,44,29,41,85,10,30,31,57,23,36,94,22,43,52,53,91,96,62,46,73,97,100,69,71,24,7,65,77,68,92,59,70,1,74,17,80,88,93,5,76,95,72,49,90,42,27,26,78,15,40,11,12,81,45,78,56,23,81,29,11,58,88,72,48,79,3,32,13,1,46,28,66,30,55,95,90,49,62,85,93,37,100,53,38,73,31,94,71,52,7,76,15,97,16,26,65,54,51,40,19,27,57,36,43,74,83,33,25,61,75,87,50,69,77,22,34,4,35,91,67,6,60,86,24,21,84,98,99,80,8,64,96,68,70,2,18,20,10,92,42,89,44,63,41,39,14,82,47,12,17,5,9,59,23,80,19,25,26,96,72,61,50,92,27,100,74,51,79,82,3,81,33,46,66,15,65,12,97,91,99,20,35,75,62,98,86,94,44,37,8,24,31,22,55,49,38,68,90,28,57,11,59,70,16,83,53,84,93,18,2,41,9,42,63,29,45,43,6,4,5,40,58,47,34,64,36,87,13,60,10,69,39,71,17,48,7,30,89,95,77,88,85,78,52,76,67,73,54,56,14,1,21,32,10,1,52,44,21,33,87,62,71,91,57,51,90,96,76,32,61,11,45,88,84,34,17,100,30,37,5,63,98,7,28,58,74,14,69,29,31,64,23,47,41,16,13,67,22,89,99,19,95,65,42,48,54,56,39,77,2,46,20,53,73,25,8,86,50,15,75,79,18,36,94,60,80,66,26,27,55,68,6,35,38,43,12,9,4,24,93,83,78,81,40,59,70,92,49,82,97,3,72,85};
	unsigned seq[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100};
	//unsigned seq[] = {3,1,1,6,6,5,5,4,7,2,2,10,6,9,6,9,3,3,5,8,9,5,8,2,3,1,5,5,7,8,10,3,1,8,7,7,1,6,8,10,4,3,2,2,9,4,4,2,6,9,7,8,1,7,4,9,10,4,10,10};
	
	CSPGraph graph(Inst.Size.Piece, Inst.Size.Machine);
	//CSPGraph graph(*Graph);
	CollectionP->Create_NxM_Graph(&graph, CollectionM, seq);
	graph.ApplyIBUIT(false);
	cmax = graph.Makespan();
	printf("CMax: %.1f\n", cmax);
	graph.IBUIT_Performance(&step, &time);
	printf("\n\n---------------------\n");
	printf("| IBUIT-PERFORMANCE |\n");
	printf("---------------------\n");
	printf(" - Steps: %d\n"
				 " - Time : %.2f msec\n"
				 "        : %.2f sec\n\n", step, time, time / CLOCKS_PER_SEC);

	//---------------------------------------------------------------------
	CollectionP->GetRecirculateData(&Inst.Recirc);
	printf("Total de piezas que recirculan     : %d\n", Inst.Recirc.pRecirc);
	printf("Total de operaciones que recirculan: %d\n\n", Inst.Recirc.tRecirc);
	
	printf("Detalles de la recirculacion:\n");	
	for(int i = 0; i < Inst.Recirc.pRecirc; i++)
	{
		printf(" Id    : %d\n", Inst.Recirc.P_Recirc[i].PieceID);
		printf(" Recirc: %d\n\n", Inst.Recirc.P_Recirc[i].nRecirc);
	}
	printf("\n\n");
#else
	//=========================================================================//
	// Uso del Algoritmo Genetico																							 //	
	//=========================================================================//
	clock_t start, end;
	GenomesVector GV;
	float Score = 0.0, LastScore = 0.0, worseScore, LastworseScore;

	GAModule::SetGraph(Graph);
	GAModule::SetPieceCollection(CollectionP);
	GAModule::SetMachineCollection(CollectionM);
	GAModule::SetPriorityList(priority_list);
	GAModule::SetBound(56); 

	GAParameterList params;
  GASimpleGA::registerDefaultParameters(params); 
  params.set(gaNpMutation,      0.01);
  params.set(gaNpCrossover,     0.85);
  params.set(gaNnGenerations,   1000);
  params.set(gaNpopulationSize, 170);
  params.set(gaNnBestGenomes,   MEJORES);
  params.set(gaNelitism,        gaTrue); //gaFalse gaTrue	

	/* Note: The default selection method is RouletteWheel. */
	//GARouletteWheelSelector selector;
	//GATournamentSelector selector;
	GARankSelector selector;
	//GAUniformSelector selector;
	//GASRSSelector selector;
	//GADSSelector selector;

	/* Esquemas de Escalado */
	//GANoScaling scaling_scheme;
  GALinearScaling scaling_scheme;
  //GASigmaTruncationScaling scaling_scheme;
  //GAPowerLawScaling scaling_scheme;
  //GASharing  scaling_scheme;

	// Se declara un genome sin longitud
	GA1DArrayGenome <unsigned> genome(0);
	GA1DArrayGenome <unsigned> worse_genome(0);
	genome.userData(&Inst);
	worse_genome.userData(&Inst);

	switch (CollectionP->ProblemType())
	{
		case PFSS:
			printf("Problem Type: PERMUTATIONAL FLOW SHOP\n\n");
			printf("Pieces  : %d\n", Inst.Size.Piece);
			printf("Machines: %d\n\n", Inst.Size.Machine);
			printf("Piezas que recirculan     : %d\n", Inst.Recirc.pRecirc);
			printf("Operaciones que recirculan: %d\n\n", Inst.Recirc.tRecirc);
		break;

		case FSS:
			printf("Problem Type: FLOW SHOP\n\n");
			printf("Pieces  : %d\n", Inst.Size.Piece);
			printf("Machines: %d\n\n", Inst.Size.Machine);
			printf("Piezas que recirculan     : %d\n", Inst.Recirc.pRecirc);
			printf("Operaciones que recirculan: %d\n\n", Inst.Recirc.tRecirc);
		break;

		case GFSS:
			printf("Problem Type: GENERALIZED FLOW SHOP\n\n");
			printf("Pieces  : %d\n", Inst.Size.Piece);
			printf("Machines: %d\n\n", Inst.Size.Machine);
			printf("Piezas que recirculan     : %d\n", Inst.Recirc.pRecirc);
			printf("Operaciones que recirculan: %d\n\n", Inst.Recirc.tRecirc);
		break;

		case JSS:
			printf("Problem Type: JOB SHOP\n\n");
			printf("Pieces  : %d\n", Inst.Size.Piece);
			printf("Machines: %d\n\n", Inst.Size.Machine);
			printf("Piezas que recirculan     : %d\n", Inst.Recirc.pRecirc);
			printf("Operaciones que recirculan: %d\n\n", Inst.Recirc.tRecirc);
		break;

		case FJSS:
			printf("Problem Type: FLEXIBLE JOB SHOP\n\n");
			printf("Pieces  : %d\n", Inst.Size.Piece);
			printf("Machines: %d\n\n", Inst.Size.Machine);
			printf("Piezas que recirculan     : %d\n", Inst.Recirc.pRecirc);
			printf("Operaciones que recirculan: %d\n\n", Inst.Recirc.tRecirc);
		break;
	}

	switch (CollectionP->ProblemType())
	{
		case PFSS:
			if(SAVE_TO_FILE) 
			{
				fprintf(file, "Name datafile: %s\n", hndFile.GetPath());
				fprintf(file, "Problem Type: FLOW SHOP\n");
				fprintf(file, "Pieces  : %d\n", Inst.Size.Piece);
				fprintf(file, "Machines: %d\n\n", Inst.Size.Machine);
				fprintf(file, "Piezas que recirculan     : %d\n", Inst.Recirc.pRecirc);
				fprintf(file, "Operaciones que recirculan: %d\n\n", Inst.Recirc.tRecirc);
			}
			Graph->ApplyIBUIT(false);
			
			genome.length			(Inst.Size.Piece + Inst.Recirc.tRecirc);
			genome.evaluator	(n_Fitness);
			genome.initializer(n_Initializer);
			//genome.crossover  (fOnePointCrossover);
			genome.crossover  (n_TwoPointCrossover);
			//genome.crossover  (fThreePointCrossover);
			//genome.crossover  (fPMXCrossover);
			//genome.crossover  (fZigZagCrossover);
			//genome.crossover  (GA1DArrayGenome<unsigned>::PartialMatchCrossover);
			genome.mutator    (GA1DArrayGenome<unsigned>::SwapMutator);
			
			worse_genome.length		(Inst.Size.Piece);
			worse_genome.evaluator(n_Fitness);
		break;

		case FSS: case GFSS: case JSS: case FJSS:
			if(SAVE_TO_FILE) 
			{
				fprintf(file, "Name datafile: %s\n", hndFile.GetPath());
				fprintf(file, "Problem Type: JOB SHOP\n");
				fprintf(file, "Pieces  : %d\n", Inst.Size.Piece);
				fprintf(file, "Machines: %d\n\n", Inst.Size.Machine);
			}

			genome.length			(Inst.Size.Piece * Inst.Size.Machine + Inst.Recirc.tRecirc);
			genome.evaluator	(nxm_Fitness);
			genome.initializer(nxm_Initializer);
			//genome.crossover  (nxm_OnePointCrossoverGene);
			//genome.crossover  (nxm_OnePointCrossoverAllele);
			//genome.crossover  (nxm_TwoPointCrossoverGene);
			//genome.crossover  (nxm_TwoPointCrossoverAllele);
			genome.crossover  (nxm_ThreePointCrossoverGene);
			//genome.crossover  (jThreePointCrossoverAllele);
			genome.mutator    (GA1DArrayGenome<unsigned>::SwapMutator);
			
			worse_genome.length		(Inst.Size.Piece * Inst.Size.Machine + Inst.Recirc.tRecirc);
			worse_genome.evaluator(nxm_Fitness);
		break;
	};

	GAPopulation Pop;
	GASimpleGA ga(genome);
	ga.parameters(params);
	ga.selector(selector);
	ga.scaling(scaling_scheme);
	ga.initialize();

#if 0		// 0 -> Ejecutar hasta el número de iteraciones
				// 1 -> Ejecutar hasta que se alcance una cota dada
	if (CollectionP->ProblemTypeInstance() == sptFlowShop) 
		ga.terminator(fGATerminateByBound);
	else
		ga.terminator(jGATerminateByBound);
#endif

	Pop = ga.population();
	worse_genome = Pop.worst();
	LastworseScore = worseScore = worse_genome.score();

	//--------- Evolucion del AG ---------//
	start = clock();	// Tiempo antes la optimizacion
	while (!ga.done()) 
	{		
		genome = ga.statistics().bestIndividual();
		Score = genome.score();
		//if (Score != 0 && Score >= LastScore && !MemberVector(GV, genome))
		if (Score != 0 && Score > LastScore)
		{
			printf("Score: %f\n", Score);
			PrintIndividual(ga.generation(), genome);	//SAVE_TO_FILE			
			//GV.push_back((GA1DArrayGenome <unsigned>)genome);
		}
		LastScore = Score;

		Pop = ga.population();
		genome = Pop.worst();
		worseScore = genome.score();
		if (worseScore < LastworseScore)
		{
			worse_genome = genome;
			LastworseScore = worseScore;
		}

		ga.step();
	}
	end = clock();	// Tiempo despues de la optimizacion
	//-------------------------//

	// Imprimo el resumen de la evolucion
	PrintResumen(Inst, ga, Pop, worse_genome,  end - start);
#endif
		
	// Libero la memoria usada.
	delete Graph;
	delete tmpGraph;
	delete CollectionP;	
	delete CollectionM;	
	delete []Sequence;
	delete []Inst.Recirc.P_Recirc;
	GAModule::ClearPriorityList();
	if (SAVE_TO_FILE) fclose(file);

	system("pause");
}
// End Main
//////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------
//
void PrintIndividual (unsigned generation, const GAGenome &G, bool to_file)
{
	//GA1DArrayGenome <unsigned> &genome = STA_CAST(GA1DArrayGenome <unsigned> &, G);
	GA1DArrayGenome <unsigned> &genome = (GA1DArrayGenome <unsigned> &)G;
	struct InstanceSize *inst = (struct InstanceSize *)genome.userData();
	float cmax;

	tmpGraph->ClearGraph();
	GetGenes(genome, Sequence);
	switch (CollectionP->ProblemType())
	{
		case PFSS: CollectionP->Create_N_Graph(tmpGraph, CollectionM, Sequence);   break;
		case FSS: case GFSS: case JSS: case FJSS: CollectionP->Create_NxM_Graph(tmpGraph, CollectionM, Sequence); break;
	};

	cmax = tmpGraph->Makespan();
	printf("Generation: [%d]   Cmax: %.2f\n", generation, cmax);
	for(int i = 0; i < genome.length(); i++) printf("%3d", Sequence[i]);
	printf("\n\n");
	
	if (to_file)
	{
		fprintf(file, "%d    %0.f\n", generation, cmax);
		fprintf(file, "Generation: [%d]    Cmax: %0.1f\n", generation, cmax);
		for(int i = 0; i < genome.length(); i++) fprintf(file, "%d ", Sequence[i]);
		fprintf(file, "\n\n");
	}
}

//---------------------------------------------------------------------------
//
void PrintResumen (STRUCT_PROBLEM_DATA inst, const GASimpleGA &ga, const GAPopulation &Pop, const GA1DArrayGenome <unsigned> &worse_genome, float elapsed_time)
{
	// Se imprime el resumen de la optimizacion
	unsigned *A;
	int size_genome;	
	CGraphCAPP grafo(inst.Size.Piece, inst.Size.Machine, inst.Recirc.tRecirc, false);

	if (CollectionP->ProblemType() == PFSS) 
		size_genome = inst.Size.Piece + inst.Recirc.tRecirc;
	else 
		size_genome = inst.Size.Piece * inst.Size.Machine + inst.Recirc.tRecirc;

	GA1DArrayGenome <unsigned> genome(size_genome);	
	A = new unsigned[size_genome];

	printf("[%d] Mejores secuencias encontradas:\n\n", MEJORES);
	if(SAVE_TO_FILE) fprintf(file, "[%d] Mejores secuencias encontradas:\n\n", MEJORES);

	for(int k = 0; k < MEJORES; k++) 
	{
		genome = ga.statistics().bestIndividual(k);
		GetGenes(genome, A);
		for (int i = 0; i < size_genome; i++) printf("%3d", A[i]);
		if(SAVE_TO_FILE) for (int i = 0; i < size_genome; i++) fprintf(file, "%3d", A[i]);
		if (CollectionP->ProblemType() == PFSS) 
			CollectionP->Create_N_Graph(&grafo, CollectionM, A);
		else 
			CollectionP->Create_NxM_Graph(&grafo, CollectionM, A);
		printf("     Cmax: %.1f\n", grafo.Makespan());
		if(SAVE_TO_FILE) fprintf(file, "     Cmax: %.1f\n", grafo.Makespan());
		grafo.ClearGraph();
	}

	printf("\nPeor secuencia encontrada:\n");
	if(SAVE_TO_FILE) fprintf(file, "\nPeor secuencia encontrada:\n");
	GetGenes(worse_genome, A);
	if (CollectionP->ProblemType() == PFSS)
		CollectionP->Create_N_Graph(&grafo, CollectionM, A);
	else 
		CollectionP->Create_NxM_Graph(&grafo, CollectionM, A);
	for (int i = 0; i < size_genome; i++) printf("%3d", A[i]);
	if(SAVE_TO_FILE) for (int i = 0; i < size_genome; i++) fprintf(file, "%3d", A[i]);
	printf("     Cmax: %.0f\n\n", grafo.Makespan());
	if(SAVE_TO_FILE) fprintf(file,"     Cmax: %.0f\n\n", grafo.Makespan());
	grafo.ClearGraph();
	delete []A;

	int ngen = ga.statistics().generation();
	int npop = Pop.size();
	int ncross = ga.statistics().crossovers();
	int nmut = ga.statistics().mutations();
	int perform = GAModule::GetPerfomanceRescheduling();
	printf("\n\nCrossovers  : %d\n", ncross);
	printf("Mutation    : %d\n", nmut);
	printf("Generations: %d\n", ngen);
	printf("Populations: %d\n", npop);
	//printf("Replacements: %d\n", ga.statistics().replacements());
	//printf("Selections  : %d", ga.statistics().selections());
	printf("Perfomance IBUIT: %d\n", perform);
	printf("Porcentaje: %.2f%\n", (float)perform / (ngen * npop));
	if(SAVE_TO_FILE)
	{
		fprintf(file, "\n\nCrossovers  : %d\n", ncross);
		fprintf(file, "Mutation    : %d\n", nmut);
		fprintf(file, "Generations: %d\n", ngen);
		fprintf(file, "Perfomance IBUIT: %d\n", perform);
		fprintf(file, "Porcentaje: %.2f%\n", (float)perform / (ngen * Pop.size()));
	}

	// Imprimo el tiempo de procesado
	printf("\n\nTiempo de procesamiento: %.3f sec\n", (double)(elapsed_time) / CLOCKS_PER_SEC);
	printf("                       : %.2f  min\n\n", (double)((elapsed_time) / CLOCKS_PER_SEC) / 60);
	if(SAVE_TO_FILE)
	{
		fprintf(file, "\n\nTiempo de procesamiento: %.3f sec\n", (double)(elapsed_time) / CLOCKS_PER_SEC);
		fprintf(file, "                       : %.2f  min\n\n", (double)((elapsed_time) / CLOCKS_PER_SEC) / 60);
	}
}

//---------------------------------------------------------------------------
//
bool MemberVector (const GenomesVector V, const GAGenome &G)
{
	//GA1DArrayGenome <unsigned> &genome = STA_CAST(GA1DArrayGenome <unsigned> &, G);
	GA1DArrayGenome <unsigned> &genome = (GA1DArrayGenome <unsigned> &)G;
	
	for(unsigned i = 0; i < V.size(); i++)
		if (EqualGenome(V[i], genome)) return true;

	return false;  
}

//---------------------------------------------------------------------------
//
bool EqualGenome (const GA1DArrayGenome <unsigned> &G1, const GA1DArrayGenome <unsigned> &G2)
{
	if (G1.length() != G2.length()) 
		return false;

	for (int i = 0; i < G1.length(); i++)
		if (G1.gene(i) != G2.gene(i)) return false;
	
	return true;
}

//---------------------------------------------------------------------------
//
void ConvertArrayToGenome (GA1DArrayGenome <unsigned> &G, const unsigned *Array)
{
	for (int i = 0; i < G.length(); i++)
		G.gene(i, Array[i]);
}


