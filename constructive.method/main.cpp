#include <map>
#include <conio.h>
#include "experiments.h"
#include "../jssp.model/CHndFile.h"
#include "../jssp.model/CPieceCollection.h"
#include "../jssp.model/CMachineCollection.h"
#include "../jssp.heuristics/CMultiStartAlgorithm.h"
#include "../jssp.heuristics/CGTAlgorithm.h"
#include "../chm.utilities/CustomConsole.h"

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
	};																									 // +----------------------+------------------+------------------+------------------+

	CustomConsole::ForegroundColor(ColorDarkCyan);
	cout << "+-------------------------------------------------+" << endl 
			 << "| PROJETO: Metodos construtivos para o Scheduling |" << endl 
			 << "+-------------------------------------------------+" << endl << endl;
	CustomConsole::LastState();

	try
	{	
#if 0
		/********** RUN COMPUTATIONAL TEST **********/
		// RESULTADOS PARA ANALISIS DE MEJOR SOLUCION ENCONTRADA -------------------
		//runExperiment_SPT_bestone_value(instances, 5000);
		//runExperiment_SEP_bestone_value(instances, 5000);
		//runExperiment_ECT_bestone_value(instances, 5000);
		//runExperiment_SemiSEP_bestone_value(instances, 5000);
		//runExperiment_GTAlgorithm_bestone_value(instances, 10);
		//runExperiment_GTAlfaRCL_bestone_value(instances, 5000, 0.0, SEP);
		//runExperiment_GTAlfaRCL_bestone_value(instances, 5000, 0.2, SEP);
		//runExperiment_GTAlfaRCL_bestone_value(instances, 5000, 0.4, SEP);
		//runExperiment_GTAlfaRCL_bestone_value(instances, 5000, 0.8, SEP);
		//runExperiment_GTAlfaRCL_bestone_value(instances, 5000, 1.0, SEP);

		// RESULTADOS PARA ANALISIS DE OCURRENCIAS ----------------------------------
		//runExperiment_Ocurrences_per_algorithm(instances["ft06"], 5000, 55);
		//runExperiment_Ocurrences_per_algorithm(instances["ft10"], 5000, 930);
		//runExperiment_Ocurrences_per_algorithm(instances["ft20"], 5000, 1165);

		// RESULTADOS PARA ANALISIS DE CALIDAD-SOLUCION vs TIEMPO
		//runExperiment_Time_for_value(instances["ft06"], 10, RANDOMIZED);
		//runExperiment_Time_for_value(instances["ft06"], 100, SEP_RANDOMIZED);
		//runExperiment_Time_for_value(instances["ft10"], 100, RANDOMIZED);
		//runExperiment_Time_for_value(instances["ft10"], 100, SEP_RANDOMIZED);
		//runExperiment_Time_for_value(instances["ft20"], 100, RANDOMIZED);
		//runExperiment_Time_for_value(instances["ft20"], 100, SEP_RANDOMIZED);

		// RESULTADOS PARA ANALISIS DE CALIDAD-SOLUCION vs ITERACIONES
		//runExperiment_Iterations_for_value(instances["ft06"], 10000, RANDOMIZED);
		//runExperiment_Iterations_for_value(instances["ft06"], 10000, SEP_RANDOMIZED);
		//runExperiment_Iterations_for_value(instances["ft10"], 10000, RANDOMIZED);
		//runExperiment_Iterations_for_value(instances["ft10"], 10000, SEP_RANDOMIZED);
		//runExperiment_Iterations_for_value(instances["ft20"], 10000, RANDOMIZED);
		//runExperiment_Iterations_for_value(instances["ft20"], 10000, SEP_RANDOMIZED);
#else
		/********** DEBUG SECTION **********/
		int jobs, machines;
		CHndFile file(instances["ft06"]);
		file.ReadInstanceSize(&jobs, &machines);
		CMachineCollection cM(machines);
		CPieceCollection cJ(jobs, machines, false);
		file.ReadMachineData(&cM);
		file.ReadPieceData(&cJ, cM);
		//test_GTAlgorithm_ocurrences(&cJ, &cM, 10, ECT, "..\\.estudo-computacional\\ETC.txt");
		test_GTAlgorithm_ocurrences(&cJ, &cM, 10, RANDOM);
		//test_GTAlgorithm_ocurrences(&cJ, &cM, 10, SEP); cout << endl;
		//test_GTAlfaRCL_ocurrences(&cJ, &cM, 1, 0.0, SEP);
		//runExperiment_GTAlfaRCL_bestone_value(instances, 1, 0.0, SEP);
#endif
	}
	catch (std::exception e) {
		CustomConsole::ForegroundColor(ColorRed);
		cout << "EXCEPTION CAPTURADA: " << endl << " - " << e.what();
		CustomConsole::LastState();
	}

	//system("pause");
	//_getch();
	return 0;
}

