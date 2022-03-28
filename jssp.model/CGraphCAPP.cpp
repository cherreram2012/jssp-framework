#include <time.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "CGraphCAPP.h"

#define DEBUG_HOLE 0

///==================================================================================///
///==================================================================================///
///																																									 ///
///													CGraph Class Definition														 ///
///																																									 ///
///==================================================================================///
///==================================================================================///

//----------------------------------------------------------------------------------
//	Funcion de borrado rapido del grafo. Esta funcion usa 1/3 de MxN para el borrado
//  de grafo, bastante rapido por cierto. Esto se logra subdividiendo la tarea en 
//	4 segmentos distintos, los cuales se ejecutan en paralelo.
//----------------------------------------------------------------------------------
void CGraphCAPP::FastClear (void)
{
	int a, b, c, d, midL, midR, end, t;

	//================
	// Paso #1:
	//================
	t    = iJobs * iMachines + iRecirc;
	midL = t / 2 - ((t % 2 == 0) ? 1 : 0);
	midR = t / 2;
	end  = t - 1;
	//-------
	// Estrategia: a-> <-b, c-> <-d
	for (a = 0, b = midL, c = midR, d = end; a < b || c < d; a++, b--, c++, d--)
	{
		//--- 1er 1/4 ---//
		TD[a].ID			     =  0;
		TD[a].MachineGroup =  NULL;
		TR[a].RTime	       =  0;
		TR[a].DTime	       =  0;
		TR[a].Machine	     = -1;

		//--- 2do 1/4 ---//
		TD[b].ID			     =  0;
		TD[b].MachineGroup =  NULL;
		TR[b].RTime	       =  0;
		TR[b].DTime	       =  0;
		TR[b].Machine	     = -1;

		//--- 3er 1/4 ---//
		TD[c].ID			     =  0;
		TD[c].MachineGroup =  NULL;
		TR[c].RTime	       =  0;
		TR[c].DTime	       =  0;
		TR[c].Machine	     = -1;

		//--- 4to 1/4 ---//
		TD[d].ID			     =  0;
		TD[d].MachineGroup =  NULL;
		TR[d].RTime	       =  0;
		TR[d].DTime	       =  0;
		TR[d].Machine	     = -1;
	}

	//================
	// Paso #2:
	//================
	end = iJobs - 1;
	//-------
	// Estrategia: a-> , <-d
	for (a = 0, d = end; a <= d; a++, d--)
	{
		//--- 1er 1/2 ---//
		Di[a].idJob = 0;
		Di[a].value = 0;
		Ci[a]				= 0;
					
		indexOpB[a] = 0;
		j_block[a]  = 0;

		//--- 2do 1/2 ---//
		Di[d].idJob = 0;
		Di[d].value = 0;
		Ci[d]				= 0;
					
		indexOpB[d] = 0;
		j_block[d]  = 0;
	}

	//================
	// Paso #3:
	//================
	end = iMachines + iRecirc - 1;
	//-------
	// Estrategia: a-> , <-d
	for (a = 0, d = end; a <= d; a++, d--)
	{
		//--- 1er 1/2 ---//
		m_block[a] = -1;

		//--- 2do 1/2 ---//
		m_block[d] = -1;
	}

	//================
	// Paso #4:
	//================
	end = iMeasureCount - 1;
	//-------
	// Estrategia: a-> , <-d
	if (TM_ON)
	{
		for (int m = 0; m < iMachines; m++)
		{
			for (a = 0, d = end; a <= d; a++, d--)
			{
				TM_ON[m][a] = 0;
				TM_ON[m][d] = 0;
			}
		}
	}
}

//----------------------------------------------------------------------------------
//	Funcion de Utileria. Esta funcion crea TR que es sinomino de recorrer el grafo.
//----------------------------------------------------------------------------------
void CGraphCAPP::CreateTR (void)
{
	int op;
	STRUCT_DATA_MACHINE dM, dM_best;
	float TOpAnterior, TPiAnterior, TPiTemp;
	
	dM						= TD[0].MachineGroup->GetMachineByIndex(0);
	TR[0].RTime   = dM.Time;
	TR[0].DTime   = dM.Time;
 	TR[0].Machine = dM.Machine;
	Cmax          = TR[0].RTime;

	for (int i = 1; i < (iJobs * iMachines + iRecirc); i++)
	{		
		op = TD[i].MachineGroup->GetMachineCount();
		dM = TD[i].MachineGroup->GetMachineByIndex(0);

		//--- Tratamiento para las Unique Machine (UM) ---//
		TOpAnterior = getTimeBackOp(i, TD[i].ID);
		TPiAnterior = getTimeBackJob(i, dM.Machine);
			
		//--- Tratamiento para las Paralell Machines (PM) ---//
		// Reglas de Despacho (Dispatching Rules) programadas para el tratamiento de 
		// las Maquinas en Paralelo:
		//	 [1]. First In First Out        (FIFO): primero en llegar primero en ejecutar
		//	 [2]. Shortest Processing Time  (SPT) : ejecutar primero la operacion mas corta
		//	 [3]. Earliest Completion Time  (ECT) : ejecutar primero lo que acaba antes
		//	 [4]. Start As Early As Posibble(SEP) : comenzar tan rapido como sea posible
		dM_best = dM;
		for (int k = 1; k < op; k++)	//---[1]:(FIFO), [2]:(SPT)---------------------------
		{			
			dM = TD[i].MachineGroup->GetMachineByIndex(k);
			TPiTemp = getTimeBackJob(i, dM.Machine);

			//----------[4]:(SEP)-----||-------------------[3]:(ECT)--------------------------
			if (TPiTemp < TPiAnterior && (TPiTemp + dM.Time) < (TPiAnterior + dM_best.Time))
			//if (TPiTemp < TPiAnterior)
			{			
				TPiAnterior = TPiTemp;		
				dM_best     = dM;
			}
		}

		if (dM_best.Machine == -1) continue;

		if (TOpAnterior > TPiAnterior)
		{
			TR[i].DTime   = dM_best.Time;
			TR[i].Machine = dM_best.Machine;
			TR[i].RTime = (dM.Time) ? TOpAnterior + dM_best.Time : 0.0;			
		}
		else
		{
			TR[i].DTime   = dM_best.Time;
      TR[i].Machine = dM_best.Machine;
			TR[i].RTime = (dM.Time) ? TPiAnterior + dM_best.Time : 0.0;			
		}

		if (TR[i].RTime > Cmax) Cmax = TR[i].RTime;
	}
}

//----------------------------------------------------------------------------------
//  Funcion de Utileria. Esta funcion crea la matriz de adyacencia que contiene las
//  maquinas encendidas.
//----------------------------------------------------------------------------------
void CGraphCAPP::CreateTM_ON (void)
{
	int f, c;
	float step, instant, init, end;

	if (!isGraphTraveled) TravelGraph();

	if (!TM_ON)
	{
		TM_ON = new int*[iMachines];
		for (f = 0; f < iMachines; f++)
			TM_ON[f] = new int[iMeasureCount];
	}

	// Inicializo la tabla con el metodo rapido
  int a, d;
  end = iMeasureCount - 1;
  for (int m = 0; m < iMachines; m++)
	{ // Estrategia: a-> , <-d
	  for (a = 0, d = end; a <= d; a++, d--)
		{
		  TM_ON[m][a] = 0;
			TM_ON[m][d] = 0;
    }
  }

  step = Cmax / iMeasureCount;

	// Se determinan las maquinas encendidas en los intervalos calculados
	for (f = 0; f < iMachines; f++)
	{
		for (c = 0; c < (int)iMeasureCount; c++)
		{
			for (int op = 0; op < (iJobs * iMachines + iRecirc); op++)
			{
				if (f == TR[op].Machine)
				{
					init    = TR[op].RTime - TR[op].DTime;
					end     = TR[op].RTime;
					instant = (float)(step * (c + 1));
				
					// ¿Está la maquina analizada encendida?
					if ((instant > init || EqualCero(instant - init)) && (instant < end || EqualCero(instant - end)))
					{
						TM_ON[f][c] = 1;
						break;
					}
				}
			}
		}
	}
}

//----------------------------------------------------------------------------------
//	Funcion de Utileria. Recorre el grafo invocando a CreateTR. Aplica la tecnica de 
//	rescheduling, mantiene un record de las veces que se mejoro una secuencia mediante
//	el rescheduling y crea la tabla para el calculo del consumo de energia.
//----------------------------------------------------------------------------------
void CGraphCAPP::TravelGraph (void)
{
	clock_t start, end;

	stepIBUIT = 0;
	timeIBUIT = 0;

	CreateTR();
	isGraphTraveled = true;
	
	if (applyIBUIT == true)
	{				
		ibuit_ok = false; 		

		start = clock();
		while(IBUIT_Technique());	// Se aplica la tecnica de IBUIT mientras mejore algun objetivo.
		end = clock();

		timeIBUIT = (double)(end - start);
	}

	if (Performance != NULL && ibuit_ok == true) (*Performance)++;
		
	//CalculateCi();		// Calculo la permanencia de los trabajos en el sistema.
  //CreateTM_ON();		// Creo la matriz de adyacencia para el calculo del consumo de energia.	
}

//----------------------------------------------------------------------------------
//	Funcion de Utileria. Aplica a la secuencia el tecnica IBUIT (Improved Based in Use of the Idle Time)
//----------------------------------------------------------------------------------
bool CGraphCAPP::IBUIT_Technique(void)
{
	unsigned id = 0;
	int space, indexOpC, machine;
	int ne, nt, re_ne, re_nt, re_ImpAnd = 0;
	float cmax, fmax, lmax, emax, tmax, imax, pw, re_cmax, re_fmax, re_lmax, re_emax, re_tmax, re_imax, re_pw;

	for (indexOpC = 1; indexOpC < (iJobs * iMachines + iRecirc); indexOpC++)
	{
		id = TD[indexOpC].ID;
		machine = TR[indexOpC].Machine;
		space = SearchSpace(indexOpC, id, machine, indexOpB);

		// ¿Se encontro algun espacio?
		if (space)
		{
			for (int i = 0; i < space; i++)
			{
				if (ExistsHole(indexOpB[i], machine))
				{
					CGraphCopier::Duplicate(imgGraph, *this);
					imgGraph->ApplyIBUIT(false);

					if (flagCmax) cmax = Makespan();
					if (flagFmax) fmax = FlowTime();
					if (flagLmax) lmax = Lateness();
					if (flagEmax) emax = Earliness();
					if (flagTmax) tmax = Tardines();
					if (flagImax) imax = Idle();
					if (flagNe)		ne = EarlyJobsCount();
					if (flagNt)		nt = TardinesJobsCount();
					if (flagPw)		pw = EnergyConsumption();

#if DEBUG_HOLE
					printf("\n----- HUECO ENCONTRADO -----\n");
					printf("PA.Id      : %d\n   Time    : %.1f\n   Machine : %d\n\n", TD[indexOpC].ID, TR[indexOpC].DTime, TR[indexOpC].Machine);
					printf("   Position: %d\n", indexOpC);

					printf("\n\nPB.Id      : %d\n   Time    : %.1f\n   Machine : %d\n\n", TD[indexOpB[i]].ID, TR[indexOpB[i]].DTime, TR[indexOpB[i]].Machine);
					printf("   Position: %d\n\n", indexOpB[i]);

					printf("ANTES  : ");
					for (int j = 0; j < (iJobs * iMachines + iRecirc); j++)printf("%d ", Sequence[j]);
					printf("\nCmax: %0.1f", cmax);
#endif

					imgGraph->ApplyForcedLeftJump(indexOpC, indexOpB[i]);

					if (flagCmax) re_cmax = imgGraph->Makespan();
					if (flagFmax) re_fmax = imgGraph->FlowTime();
					if (flagLmax) re_lmax = imgGraph->Lateness();
					if (flagEmax) re_emax = imgGraph->Earliness();
					if (flagTmax) re_tmax = imgGraph->Tardines();
					if (flagImax) re_imax = imgGraph->Idle();
					if (flagNe)		re_ne = imgGraph->EarlyJobsCount();
					if (flagNt)		re_nt = imgGraph->TardinesJobsCount();
					if (flagPw)		re_pw = imgGraph->EnergyConsumption();

					switch (ImpType)
					{
						case itAtLeastOne:
							if ((flagCmax && re_cmax <= cmax) || (flagFmax && re_fmax <= fmax) ||
								(flagLmax && re_lmax <= lmax) || (flagEmax && re_emax >= emax) ||
								(flagTmax && re_tmax <= tmax) || (flagImax && re_imax <= imax) ||
								(flagNe   && re_ne >= ne) || (flagNt   && re_nt <= nt) ||
								(flagPw   && re_pw <= pw))
							{
								ibuit_ok = true;
								ApplyForcedLeftJump(indexOpC, indexOpB[i]);

								// Se actualiza el cromosoma pasado al grafo
								if (Genome != NULL && (ProbType == FSS || ProbType == GFSS || ProbType == JSS || ProbType == FFSS || ProbType == FJSS))
								{
									GA1DArrayGenome <unsigned> &G = STA_CAST(GA1DArrayGenome <unsigned> &, *Genome);
									for (int i = 0; i < (iJobs * iMachines + iRecirc); i++) G.gene(i, Sequence[i]);
								}

#if DEBUG_HOLE
							printf("\n\nDESPUES: ");
							for (int j = 0; j < (iJobs * iMachines + iRecirc); j++)printf("%d ", Sequence[j]);
							printf("\nCmax: %0.1f", re_cmax);
							printf("\nCon exito!!!!!!!\n\n\n");
							getch();
#endif
								imgGraph->ClearGraph(); stepIBUIT++;
								return 1;
							}
#if DEBUG_HOLE
							else
							{
								printf("\n\nDESPUES: ");
								const unsigned *seq = imgGraph->GetSequence();
								for (int k = 0; k < (iJobs * iMachines + iRecirc); k++) printf("%d ", seq[k]);
								printf("\nCmax: %0.1f", re_cmax);
								printf("\nNo mejora!!!!!!!\n\n\n");
								getch();
							}
#endif
						break;

					case itAllAtSameTime:
						if (flagCmax && re_cmax <= cmax) re_ImpAnd += 2;
						if (flagFmax && re_fmax <= fmax) re_ImpAnd += 4;
						if (flagLmax && re_lmax <= lmax) re_ImpAnd += 8;
						if (flagEmax && re_emax >= emax) re_ImpAnd += 16;
						if (flagTmax && re_tmax <= tmax) re_ImpAnd += 32;
						if (flagImax && re_imax <= imax) re_ImpAnd += 64;
						if (flagNe	 && re_ne >= ne)   re_ImpAnd += 128;
						if (flagNt	 && re_nt <= nt)	 re_ImpAnd += 256;
						if (flagPw	 && re_pw <= pw)	 re_ImpAnd += 512;

						if (re_ImpAnd == ImpAnd)
						{
							ibuit_ok = true;
							ApplyForcedLeftJump(indexOpC, indexOpB[i]);

							// Se actualiza el cromosoma pasado al grafo
							if (Genome != NULL && (ProbType == FSS || ProbType == GFSS || ProbType == JSS || ProbType == FFSS || ProbType == FJSS))
							{
								GA1DArrayGenome <unsigned> &G = STA_CAST(GA1DArrayGenome <unsigned> &, *Genome);
								for (int i = 0; i < (iJobs * iMachines + iRecirc); i++) G.gene(i, Sequence[i]);
							}

							imgGraph->ClearGraph(); stepIBUIT++;
							return 1;
						}
						break;
					}
				}

				imgGraph->ClearGraph();
			}
		}
	}

	return 0;
}

//----------------------------------------------------------------------------------
//	Funcion de utileria. Calcula la permanencia de cada trabajo en el 
//  sistema a partir de los datos en TR. Los valores calculados son ubicados 
//  en el dato miembro 'Cir[]'.
//  Nota: Para que los calculos de permanencia real sean calculados correctamente
//  la funcion debe ser llamada una vez recorrido el grafo. 	
//----------------------------------------------------------------------------------
void CGraphCAPP::CalculateCi (void)
{
	int j;
	unsigned id;
	float init_time, end_time, time_td;
	
	for (int i = 0; i < iJobs; i++)
	{
		id = Di[i].idJob;

		// Se busca la primera operacion de la pieza con 'id'
		for(j = 0; j < iJobs * iMachines + iRecirc; j++)
		{
			if (id == TD[j].ID)
			{
				time_td   = TR[j].DTime;
				init_time = TR[j].RTime;
				break;
			}
		}

		// Se busca la ultima operacion de la pieza con 'id'
		for(j = iJobs * iMachines + iRecirc - 1; j >= 0 ; j--)
		{
			if (id == TD[j].ID && TR[j].DTime != 0.0)
			{
				end_time = TR[j].RTime;
				break;
			}
		}

		Ci[i] = end_time - (init_time - time_td);
	}
}

//----------------------------------------------------------------------------------
//	Funcion de utileria. Devuelve la posicion que tiene la pieza determina por
//	'id' dentro del vector DueDate	
//----------------------------------------------------------------------------------
int CGraphCAPP::IndexInDueDateVector (unsigned id)
{	
	if (id > 0 && id <= (unsigned)iJobs) return Di[id - 1].idJob;
				
	return -1;
}

//----------------------------------------------------------------------------------
//	Constructor de la clase.
//----------------------------------------------------------------------------------
CGraphCAPP::CGraphCAPP (int piece, int machine, int recirc, bool apply_ibuit)
{
	// Valido que la cantidad de operaciones y de piezas sea mayor que cero. De 
	// no ser asi aborto la ejecucion del programa 
	assert(machine > 0 && piece > 0);

	iJobs					= piece;
	iMachines			= machine;
	iRecirc				= recirc;
	Cmax					= 0.0;
	iMeasureCount	= 10;
	isGraphTraveled	= false;
  TM_ON         = NULL;
	JobsList			= NULL;
	hndGantt			= NULL;
  hndEnergy     = NULL;
	imgGraph			= NULL;
	MachineList		= NULL;
	Performance		= NULL;
	Genome				= NULL;
	ProbType			= NonProblem;
	applyIBUIT    = apply_ibuit;
	ImpType				= itAtLeastOne;
	ImpAnd				= 0;
	flagCmax			= 1;		
	flagFmax			=	0;
	flagLmax			=	0;
	flagEmax			=	0;
	flagTmax			=	0;
	flagImax			= 0;
	flagNe				=	0;
	flagNt				=	0;
	flagPw				=	0;
	ibuit_ok			= false;
	schedule.SetDimension(NxM_SCHEDULE, iJobs, iMachines, iRecirc);

  TD			 = new STRUCT_NODE	      [iJobs * iMachines + iRecirc];
	TR       = new STRUCT_NODE_RESULT [iJobs * iMachines + iRecirc];
	Di			 = new STRUCT_DUEDATE     [iJobs];
	Sequence = new unsigned				    [iJobs * iMachines + iRecirc];
	Ci			 = new float				      [iJobs];

	indexOpB = new int      [iJobs];
	m_block  = new int      [iMachines + 1 + iRecirc];
	j_block  = new unsigned [iJobs + 1]; 

	// Borrado rapido del grafo y todos los demas datos
	FastClear();

	if (apply_ibuit) imgGraph = new CGraphCAPP(piece, machine, recirc, false);
}

//----------------------------------------------------------------------------------
// Constructor de Copia.
//----------------------------------------------------------------------------------
CGraphCAPP::CGraphCAPP (const CGraphCAPP &Obj)
{
#if 0  
	iJobs				= 0;
	iMachines		= 0;
	ImpAnd			= 0;	
	TD					= NULL;			
	TR					= NULL;
	Ci					= NULL;
	Di					=	NULL;
	Sequence		= NULL;    		
	TM_ON       = NULL;
	JobsList		= NULL;
	hndGantt		= NULL;
  hndEnergy   = NULL;
	imgGraph		= NULL;
	Genome			= NULL;
	MachineList = NULL;
	Performance	= NULL;

	CGraphCopier::Duplicate(Obj, &(*this));

	if (applyIBUIT) imgGraph = new CGraph(iJobs, iMachines, recirc, false);

	indexOpB = new int [iJobs];
	m_block  = new int [iMachines + 1 + iRecirc];
	j_block  = new unsigned [iJobs + 1];	
#endif

	//const unsigned *sequence;
	
	iJobs					= Obj.GetPieceCount();
	iMachines			= Obj.GetMachineCount();
	Cmax					= 0.0;
	iMeasureCount	= Obj.GetEnergyMeasureCount();  
	isGraphTraveled	= false;
	applyIBUIT    = Obj.ApplyIBUIT();
	ProbType      = Obj.GetProblemType();
	//sequence      = Obj.GetSequence();
  TM_ON         = NULL;
	JobsList			= NULL;
	hndGantt			= NULL;
  hndEnergy     = NULL;
	imgGraph			= NULL;
	Performance		= NULL;
	Genome			  = NULL;
	MachineList		= Obj.GetMachineInformation();
	ImpAnd	      = 0;
	ibuit_ok			= false;
	ImpType       = Obj.GetImproveType();
	SetJobsList(Obj.GetJobsList());
	Obj.GetImproveObjetive(&flagCmax, &flagFmax, &flagLmax, &flagEmax, &flagTmax, &flagImax, &flagNe, &flagNt, &flagPw);

	if (flagCmax) ImpAnd += 2; 
	if (flagFmax) ImpAnd += 4; 
	if (flagLmax) ImpAnd += 8;
	if (flagEmax) ImpAnd += 16;
	if (flagTmax) ImpAnd += 32;
	if (flagImax) ImpAnd += 64;
	if (flagNe)	  ImpAnd += 128;
	if (flagNt)	  ImpAnd += 256;
	if (flagPw)	  ImpAnd += 512;

	// Valido que la cantidad de maquinas y de piezas sea mayor que cero. De 
	// no ser asi aborto la ejecucion del programa 
	assert(iMachines > 0 && iJobs > 0);

	TD			 = new STRUCT_NODE		    [iJobs * iMachines + iRecirc];
	TR			 = new STRUCT_NODE_RESULT [iJobs * iMachines + iRecirc];
	Di			 = new STRUCT_DUEDATE     [iJobs];
	Sequence = new unsigned				    [iJobs * iMachines + iRecirc];
	Ci			 = new float					    [iJobs];
	
	indexOpB = new int [iJobs];
	j_block  = new unsigned [iJobs + 1];
	m_block  = new int [iMachines + 1 + iRecirc];

	// Borrado rapido del grafo y todos los demas datos
	FastClear();
	
	if (applyIBUIT) imgGraph = new CGraphCAPP(iJobs, iMachines, iRecirc, false);
}

//----------------------------------------------------------------------------------
// Destructor de la clase.
//----------------------------------------------------------------------------------
CGraphCAPP::~CGraphCAPP (void)
{	
	hndGantt		= NULL;
  hndEnergy   = NULL;
	MachineList	= NULL;
	Performance	= NULL;
	Genome		  = NULL;

	delete []TD;
	delete []TR;
	delete []Sequence;
	delete []Di;
	delete []Ci;
	delete []indexOpB;
	delete []j_block;
	delete []m_block;

	if (TM_ON) 
	{
		for (int m = 0; m < iMachines; m++) delete []TM_ON[m];
		delete []TM_ON;
	}

	if (JobsList) delete []JobsList;
	if (imgGraph) delete imgGraph;
}

//----------------------------------------------------------------------------
//	Establece la secuencia que representa el grafo.
//----------------------------------------------------------------------------
void CGraphCAPP::SetSequence (const unsigned *sequence)
{
	for (int i = 0; i < iJobs * iMachines + iRecirc; i++)	Sequence[i] = sequence[i];
}

//----------------------------------------------------------------------------------
//	Establece la informacion relacionada con las maquinas. 
//----------------------------------------------------------------------------------
void CGraphCAPP::SetMachineInformation (const CMachineCollection *Obj)
{
	MachineList = Obj;
}

//----------------------------------------------------------------------------
//	Establece la lista de trabajos que es usada por los objetivos Fmax, Lmax, 
//	Emax, Tmax al evaluar la secuencia para estos trabajos.
//----------------------------------------------------------------------------
void CGraphCAPP::SetJobsList (unsigned *jobs_list)
{
	if (jobs_list)
	{	
		JobsList = new unsigned [iJobs];
		for (int i = 0; i < iJobs; i++)	JobsList[i] = jobs_list[i];
	}
}

//----------------------------------------------------------------------------
//	Establece el tipo de mejora para los objetivos.
//----------------------------------------------------------------------------
void CGraphCAPP::SetImproveType (enum ImproveType type)
{
	ImpType = type;
}

//----------------------------------------------------------------------------
//	Establece el intervalo que sera usado para el calculo del consumo de 
//	Energia. Por defecto esta valor es '10'.
//----------------------------------------------------------------------------
void CGraphCAPP::SetEnergyMeasureCount (unsigned count)
{
	// Verifico si el valor pasado por parametro es distinto al que actualmente
	// se conoce. Si esto ocurre significa que cambio el numero de lecturas.
	if (iMeasureCount != count)
	{
		iMeasureCount = count;

		// Si ya se reservo memoria para la tabla, libero y creo de nuevo
		if (TM_ON)
		{
			for (int m = 0; m < iMachines; m++) delete []TM_ON[m];
			delete []TM_ON;
			TM_ON = NULL;
		}

		if (!TM_ON)
		{
			TM_ON = new int*[iMachines];
			for (int f = 0; f < iMachines; f++)
				TM_ON[f] = new int[iMeasureCount];
		}
	}

	Cmax = 0.0;
	isGraphTraveled = false;
}

//----------------------------------------------------------------------------
//	Establece los objetivos que se trataran de mejorar con la tecnica IBUIT.
//----------------------------------------------------------------------------
void CGraphCAPP::SetImproveObjetive (bool cmax, bool fmax, bool lmax, bool emax, bool tmax, bool imax, bool ne, bool nt, bool pw)
{
	flagCmax = cmax;		
	flagFmax = fmax;
	flagLmax = lmax;
	flagEmax = emax;
	flagTmax = tmax;
	flagImax = imax;
	flagNe	 = ne;
	flagNt	 = nt;
	flagPw	 = pw;

	ImpAnd = 0;
	if (flagCmax) ImpAnd += 2; 
	if (flagFmax) ImpAnd += 4; 
	if (flagLmax) ImpAnd += 8;
	if (flagEmax) ImpAnd += 16;
	if (flagTmax) ImpAnd += 32;
	if (flagImax) ImpAnd += 64;
	if (flagNe)	  ImpAnd += 128;
	if (flagNt)	  ImpAnd += 256;
	if (flagPw)	  ImpAnd += 512;
}

//----------------------------------------------------------------------------
//	Agrega un valor de duedate a la pieza determinada por 'id'.	
//----------------------------------------------------------------------------
bool CGraphCAPP::SetDueDate (unsigned id, float di)
{
	if (id > 0 && id <= (unsigned)iJobs)
	{
		Di[id - 1].idJob = id;
		Di[id - 1].value = di;
	
		return 1;
	}

	return 0;
}

//----------------------------------------------------------------------------
//	Establece el vinculo a una variable donde almacenar el record de las veces
//	que fue satisfactorio aplicar la tecnica IBUIT.	
//----------------------------------------------------------------------------
void CGraphCAPP::SetLinkToPerformance	(unsigned long &count)
{
	Performance = &count;
}

//----------------------------------------------------------------------------
//	Establece el vinculo a un objeto genoma, que sera usado para informar al 
//  algoritmo genetico los cambios ocurridos en una secuencia despues de 
//	aplicada la tecnica IBUIT.	
//----------------------------------------------------------------------------
void CGraphCAPP::SetLinkToGenome (GAGenome &g)
{
	Genome = &g;
}	

//----------------------------------------------------------------------------
//	Devuelve la lista de trabajos que es usada por los objetivos Fmax, Lmax, 
//	Emax, Tmax al evaluar la secuencia para estos trabajos.
//----------------------------------------------------------------------------
unsigned *CGraphCAPP::GetJobsList (void) const
{
	return JobsList;
}

//----------------------------------------------------------------------------
//	Devuelve la cantidad de mediciones que se usan para al calculo de consumo
//  de energia.
//----------------------------------------------------------------------------
unsigned CGraphCAPP::GetEnergyMeasureCount (void) const
{
	return iMeasureCount;
}

//----------------------------------------------------------------------------
//	Devuelve por referencia los datos de la posicion en el grafo determinada por
//	index.
//----------------------------------------------------------------------------
void CGraphCAPP::GetValue (int index, unsigned *id, const CMachineGroupLite **machines) const
{
	if (index >= 0 && index < iJobs * iMachines + iRecirc) 
	{
		if (id) *id	= TD[index].ID;
		if (machines) *machines = TD[index].MachineGroup;		
	}
	else
	{
		if (id) *id = 0;
		if (machines) *machines = NULL;
	}
}

//----------------------------------------------------------------------------
//	Devuelve por referencia el valor de duedate y el id de la pieza determinada
//	por el parametro 'index'.
//----------------------------------------------------------------------------
void CGraphCAPP::GetDueDate (int index, unsigned *id, float *di) const
{
	if (index >= 0 && index < iJobs)
	{
		*id = Di[index].idJob; 
		*di = Di[index].value;
	}
	else
	{
		if (id) *id = 0; 
		if (di) *di = 0;
	}
}

//----------------------------------------------------------------------------
//	Devuelve por referencia el estado de los objetivos indicando si el objetivo
//	se tiene un cuenta para mejorar al aplicar la tecnica IBUIT. 
//----------------------------------------------------------------------------
void CGraphCAPP::GetImproveObjetive (bool *cmax, bool *fmax, bool *lmax, bool *emax, bool *tmax, bool *imax, bool *ne, bool *nt, bool *pw) const
{
	*cmax = flagCmax;
	*fmax = flagFmax;
	*lmax = flagLmax;
	*emax = flagEmax;
	*tmax = flagTmax;
	*imax = flagImax;
	*ne	  = flagNe;
	*nt	  = flagNt;
	*pw	  = flagPw;	
}

//----------------------------------------------------------------------------
//	Devuelve el manipulador de la interfaz donde sera vizualizado el diagrama
//	de Gantt.
//----------------------------------------------------------------------------
TObserverGantt *CGraphCAPP::GetGantChartView (void) const
{
	return hndGantt;
}

//----------------------------------------------------------------------------
//	Devuelve el manipulador de la interfaz donde sera vizualizado el diagrama
//	de Energia.
//----------------------------------------------------------------------------
TObserverEnergy *CGraphCAPP::GetEnergyChartView	(void) const
{
	return hndEnergy;
}

//----------------------------------------------------------------------------
//	Devuelve el puntero a la variable donde se lleva el record de las veces 
//	que satisfactorio la aplicacion de la tecnica IBUIT.
//----------------------------------------------------------------------------
unsigned long *CGraphCAPP::GetLinkToPerformance (void) const
{
	return Performance;
}

//----------------------------------------------------------------------------
//	Devuelve el puntero al objeto que contiene la informacion de la maquinas.
//----------------------------------------------------------------------------
const CMachineCollection *CGraphCAPP::GetMachineInformation (void) const
{
	return MachineList;
}

//----------------------------------------------------------------------------
//	Devuelve si se aplica o no el re-scheduling al recorrer el grafo.
//----------------------------------------------------------------------------
bool CGraphCAPP::ApplyIBUIT (void) const
{
	return applyIBUIT;
}

//----------------------------------------------------------------------------
//	Establece si se aplica o no la tecnica IBUIT al recorrer el grafo.
//----------------------------------------------------------------------------
void CGraphCAPP::ApplyIBUIT (bool apply)
{
	applyIBUIT = apply;

	if (applyIBUIT == true && imgGraph == NULL)
	{		
		imgGraph = new CGraphCAPP(iJobs, iMachines, iRecirc, false);
	}
	else if (applyIBUIT == false && imgGraph != NULL)
	{		
		delete imgGraph;
		imgGraph = NULL;
	}
}

//----------------------------------------------------------------------------------
//	Aplica a la secuencia el tecnica de rescheduling: Forced-LeftJump
//----------------------------------------------------------------------------------
void CGraphCAPP::ApplyForcedLeftJump (int opCurrent, int opBack)
{
	Internal_IBUIT(opCurrent, opBack);
	
	// Limpiamos TR
	for (int i = 0; i < (iJobs * iMachines + iRecirc); i++) 
	{
		TR[i].RTime   = 0.0;
		TR[i].Machine = -1;
		TR[i].DTime   = 0.0;
	}
	
	CreateTR();
}

//----------------------------------------------------------------------------
//	Calcula el Cmax (makespan).
//----------------------------------------------------------------------------
float CGraphCAPP::Makespan (void)
{	
	// Si todavia no se ha recorrido el grafo, lo recorro ahora
	if (!isGraphTraveled) TravelGraph(); 

	return Cmax;
}

//----------------------------------------------------------------------------
//	Calcula el flujo total de los trabajos pasados en el argumento 'jobs_list'
//  en el sistema.
//----------------------------------------------------------------------------
float CGraphCAPP::FlowTime (unsigned *jobs_list)
{
	float sum_Pr = 0;
	int k;

	// Protejo la ejecucion frente a una ausencia de la lista de trabajo,
	// la cual es imprescindible para evaluar este objetivo.
	assert(JobsList != NULL || jobs_list != NULL);

	// Si todavia no se ha recorrido el grafo, lo recorro ahora
	if (!isGraphTraveled) TravelGraph(); 
	
	for (int i = 0; i < iJobs; i++)
	{
		if (jobs_list)
			k = IndexInDueDateVector(jobs_list[i]);
		else
			k = IndexInDueDateVector(JobsList[i]);

		if (k != -1) sum_Pr += Ci[k];
	}

	return sum_Pr;
}

//----------------------------------------------------------------------------
//	Calcula el Retraso (Lateness) de los trabajos pasados por parametros.
//----------------------------------------------------------------------------
float CGraphCAPP::Lateness (unsigned *jobs_list)
{
	float Li, L = 0.0;
	int k;

	// Protejo la ejecucion frente a una ausencia de la lista de trabajo,
	// la cual es imprescindible para evaluar este objetivo.
	assert(JobsList != NULL || jobs_list != NULL);

	// Si todavia no se ha recorrido el grafo, lo recorro ahora
	if (!isGraphTraveled) TravelGraph();
	
	for (int i = 0; i < iJobs; i++)
	{
		if (jobs_list) 
			k = IndexInDueDateVector(jobs_list[i]);
		else
			k = IndexInDueDateVector(JobsList[i]);

		if (k != -1)
		{
			Li = (!EqualCero(Ci[k] - Di[k].value)) ? Ci[k] - Di[k].value: 0.0f;
			L += Li;
		}
	}
	
	return L;
}

//----------------------------------------------------------------------------
//	Calcula el Adelanto 'E' = |-Lmax| de los trabajos pasados por parametro.
//----------------------------------------------------------------------------
float CGraphCAPP::Earliness (unsigned *jobs_list)
{
	float L;

	// Protejo la ejecucion frente a una ausencia de la lista de trabajo,
	// la cual es imprescindible para evaluar este objetivo.
	assert(JobsList != NULL || jobs_list != NULL);
	
	if (jobs_list) 
		L = Lateness(jobs_list);
	else
		L = Lateness(JobsList);

	return (L < 0) ? (float)fabs(L): 0.0f;
}

//----------------------------------------------------------------------------
//	Calcula la Tardanza (Tardiness) 'T' = Lmax de los trabajos pasados por 
//  parametros.
//----------------------------------------------------------------------------
float CGraphCAPP::Tardines (unsigned *jobs_list)
{
	float L;

	// Protejo la ejecucion frente a una ausencia de la lista de trabajo,
	// la cual es imprescindible para evaluar este objetivo.
	assert(JobsList != NULL || jobs_list != NULL);

	if (jobs_list) 
		L = Lateness(jobs_list);
	else
		L = Lateness(JobsList);

	return (L > 0) ? L: 0.0f;
}

//----------------------------------------------------------------------------
//	Calcula el Ocio de las maquinas (Idle).  
//----------------------------------------------------------------------------
float CGraphCAPP::Idle (void)
{
	unsigned id = 0;
	int space, indexOpC, machine;
	float TOpActual, t_startBP, TPiAnt_Ant, idle = 0;

	// Si todavia no se ha recorrido el grafo, lo recorro ahora
	if (!isGraphTraveled) TravelGraph(); 

	for (indexOpC = 1; indexOpC < (iJobs * iMachines + iRecirc); indexOpC++)	
	{																									
		id      = TD[indexOpC].ID;
		machine = TR[indexOpC].Machine;
		space		= SearchSpace(indexOpC, id, machine, indexOpB);

		if (space)
		{
			for (int i = 0; i < space; i++)
			{
				if (ExistsHole(indexOpB[i], machine))
				{					
					t_startBP  = TR[indexOpB[i]].RTime - TR[indexOpB[i]].DTime;
					TOpActual  = getTimeBackOp(indexOpC, id);
					TPiAnt_Ant = getTimeBackJob(indexOpB[i], machine);

					if (TOpActual > TPiAnt_Ant)
						idle += (t_startBP - TOpActual);
					else
						idle += (t_startBP - TPiAnt_Ant);
				}
			}
		}
	}

	return idle;
}

//----------------------------------------------------------------------------
//	Calcula el consumo de energia de la secuencia.
//----------------------------------------------------------------------------
float CGraphCAPP::EnergyConsumption (void)
{
	float consume = 0;

	// Si todavia no se ha recorrido el grafo, lo recorro ahora
	if (!isGraphTraveled) TravelGraph();

	for (int i = 0; i < (int)iMeasureCount; i++)
		for (int m = 0; m < iMachines; m++)
      if (TM_ON[m][i]) consume += (MachineList->GetNominalPower(m) * Cmax / iMeasureCount);
	
	return consume;
}

//----------------------------------------------------------------------------
//	Devuelve la cantidad de trabajos con tiempo de finalizacion adelantados
//	o en tiempo. 
//----------------------------------------------------------------------------
int	CGraphCAPP::EarlyJobsCount (void)
{
	int count = 0;
	
	// Si todavia no se ha recorrido el grafo, lo recorro ahora
	if (!isGraphTraveled) TravelGraph();
	
	for (int i = 0; i < iJobs; i++) 
		if (Ci[i] - Di[i].value <= 0) count++;

	return count;
}

//----------------------------------------------------------------------------
//	Devuelve la cantidad de trabajos fuera del plazo de entrega. 
//----------------------------------------------------------------------------
int	CGraphCAPP::TardinesJobsCount (void)
{
	int count = 0;
	
	// Si todavia no se ha recorrido el grafo, lo recorro ahora
	if (!isGraphTraveled) TravelGraph();
	
	for (int i = 0; i < iJobs; i++) 
		if (Ci[i] - Di[i].value > 0) count++;

	return count;
}

//----------------------------------------------------------------------------
//	Muestra un diagrama de Gantt en el visor designado por <HndGantt>  
//	GraphTime (int operation, float startT, float elapsedT, unsigned ID)
//----------------------------------------------------------------------------
void CGraphCAPP::CreateGanttChart(void)
{
	int j, op;

	// Si todavia no se ha recorrido el grafo, se corre ahora
	if (!isGraphTraveled) TravelGraph();

	hndGantt->PrintCmaxValue(Makespan());
	hndGantt->PrintFmaxValue(FlowTime(JobsList));
	//hndGantt->PrintLmaxValue(Lateness(JobsList));
	hndGantt->PrintIdleValue(Idle());
	hndGantt->PrintEmaxValue(Earliness(JobsList));
	hndGantt->PrintTmaxValue(Tardines(JobsList));
	hndGantt->PrintNeValue(EarlyJobsCount());
	hndGantt->PrintNtValue(TardinesJobsCount());
	hndGantt->PrintPwValue(EnergyConsumption());
	hndGantt->PrintSchedule(SchedulePlannig());

	int *seq_op_num = new int[iJobs * iMachines + iRecirc];
	int *op_job = new int[iJobs + 1];

	op_job[0] = 0;
	for (j = 1; j <= iJobs; j++) op_job[j] = 1;

	for (op = 0; op < iJobs * iMachines + iRecirc; op++)
	{
		seq_op_num[op] = op_job[Sequence[op]];
		op_job[Sequence[op]]++;
	}

	for (op = 0; op < iMachines; op++)
		for (j = 0; j < iJobs; j++)
			if (TR[op * iJobs + j].RTime)
				hndGantt->GraphTime(TR[op * iJobs + j].Machine, TR[op * iJobs + j].RTime - TR[op * iJobs + j].DTime,
					TR[op * iJobs + j].DTime, Sequence[op * iJobs + j], seq_op_num[op * iJobs + j]);

	hndGantt->SetMeasureMark(iMeasureCount);  // Establece y prepara las marcas de las lecturas
																						// del consumo de energia
	float step = Cmax / iMeasureCount;
	for (int i = 0; i < (int)iMeasureCount; i++)
		hndGantt->GraphMeasureMark(i, step * (i + 1));

	delete[]seq_op_num;
	delete[]op_job;
}

//----------------------------------------------------------------------------
//	Muestra un grafico de energia en el visor designado por <hndEnergy>
//	GraphInterval (int interval, float machine_power, char *machine_list[], int machine_on)
//----------------------------------------------------------------------------
void CGraphCAPP::CreateEnergyChart(void)
{
	int i, machine_on, index, len;
	float consume, ec;
	char **list;

	list = new char *[iMachines];
	len = MachineList->GetMaxNameLength();
	for (i = 0; i < iMachines; i++) list[i] = new char[len + 1];

	// Si todavia no se ha recorrido el grafo, se corre ahora
	if (!isGraphTraveled) TravelGraph();

	ec = this->EnergyConsumption();
	hndEnergy->SetDataChart(Cmax, iMeasureCount);

	for (i = 0; i < (int)iMeasureCount; i++)
	{
		machine_on = 0;
		consume = 0;
		index = 0;
		for (int m = 0; m < iMachines; m++)
		{
			if (TM_ON[m][i])
			{
				machine_on++;
				consume += (MachineList->GetNominalPower(m) * Cmax / iMeasureCount);
				const char *tmp = MachineList->GetNameByIndex(m);
				strcpy(list[index], tmp);
				index++;
			}
		}
		hndEnergy->GraphInterval(i, consume, list, machine_on);

		for (int j = 0; j < iMachines; j++) *list[j] = '\0';
	}

	hndEnergy->DrawEnergyAve(ec / iMeasureCount);
	hndEnergy->DrawOutLine();
	hndEnergy->PrintECValue(ec);
	hndEnergy->PrintCmaxValue(Cmax);

	for (int k = 0; k < iMachines; k++) delete list[k];
	delete[]list;
}

//----------------------------------------------------------------------------
//  Devuelve el consumo de potencia maximo.
//----------------------------------------------------------------------------
float CGraphCAPP::GetMaxEnergyConsumption(void)
{
	float consume = 0.0, aux;

	// Si todavia no se ha recorrido el grafo, se corre ahora
	if (!isGraphTraveled) TravelGraph();

	for (int i = 0; i < (int)iMeasureCount; i++)
	{
		aux = 0.0;
		for (int m = 0; m < iMachines; m++)
			if (TM_ON[m][i]) aux += (MachineList->GetNominalPower(m) * Cmax / iMeasureCount);

		if (aux > consume) consume = aux;
	}

	return consume;
}

//----------------------------------------------------------------------------
//	Esta funcion hace una copia a nivel de valores del grafo pasado por 
//  parametro.
//----------------------------------------------------------------------------
#if 0
void CGraph::CopyGraph (const CGraph &Obj)
{
	const unsigned *sequence;

	// Valido que la cantidad de maquinas y de piezas sea mayor que cero. De 
	// no ser asi aborto la ejecucion del programa 
	assert(Obj.GetMachineCount() > 0 && Obj.GetPieceCount() > 0);

	// Copio los valores de intercambio
	ImpAnd				= 0;
	Cmax					= 0.0;
	isRunnerGraph	= false;
	applyIBUIT    = (applyIBUIT) ? Obj.ApplyIBUIT() : false;
	ProbType			= Obj.GetProblemType();
	sequence      = Obj.GetSequence();
	hndGantt			= Obj.GetGantChartView();
  hndEnergy     = Obj.GetEnergyChartView();
	MachineList		= Obj.GetMachineInformation();
	ImpType				= Obj.GetImproveType();
	Performance		= Obj.GetLinkToPerformance();

	/***** 
		Nota: No se establece ningun valor para los siguientes datos miembros, pues ellos no 
					son variables de intercambio de informacion sino variables de control en el grafo.
			- TM_ON
			- imgGraph
	*****/

	SetJobsList(Obj.GetJobsList());
	SetEnergyMeasureCount(Obj.GetEnergyMeasureCount());
	Obj.GetImproveObjetive(&flagCmax, &flagFmax, &flagLmax, &flagEmax, &flagTmax, &flagImax, &flagNe, &flagNt, &flagPw);

	if (flagCmax) ImpAnd += 2; 
	if (flagFmax) ImpAnd += 4; 
	if (flagLmax) ImpAnd += 8;
	if (flagEmax) ImpAnd += 16;
	if (flagTmax) ImpAnd += 32;
	if (flagImax) ImpAnd += 64;
	if (flagNe)	  ImpAnd += 128;
	if (flagNt)	  ImpAnd += 256;
	if (flagPw)	  ImpAnd += 512;

	if (iJobs	!= Obj.GetPieceCount() || iMachines != Obj.GetMachineCount())
	{
		iJobs			= Obj.GetPieceCount();
		iMachines	= Obj.GetMachineCount();
		
		if (TD)				delete []TD;
		if (TR)				delete []TR;
		if (Sequence) delete []Sequence;
		if (Di)				delete []Di;
		if (Ci)				delete []Ci;
		
		TD			 = new STRUCT_NODE		    [iJobs * iMachines + iRecirc];
		TR			 = new STRUCT_NODE_RESULT	[iJobs * iMachines + iRecirc];
		Di			 = new STRUCT_DUEDATE     [iJobs];
		Sequence = new unsigned				    [iJobs * iMachines + iRecirc];
		Ci			 = new float					    [iJobs];
	}

	for(int i = 0; i < (iJobs * iMachines + iRecirc); i++)
	{
		Obj.GetValue(i, &TD[i].ID, &TD[i].MachineGroup);
		TR[i].RTime	= 0.0;
		TR[i].DTime	= 0.0;
		Sequence[i] = sequence[i];
	}

	for(int j = 0; j < iJobs; j++)
	{
		Ci[j] = 0;
		Obj.GetDueDate(j, &Di[j].idJob, &Di[j].Value);
	}
}
#endif

//----------------------------------------------------------------------------
//	Limpia la tabla asignando 0 a los valores de la tabla y -1 a los ordenes de 
//  las operaciones.
//----------------------------------------------------------------------------
void CGraphCAPP::ClearGraph (void)
{
	// Pongo las variables de intercambio y control a su estado inicial
	Cmax					= 0.0;
	isGraphTraveled = false;
	Performance		= NULL;
	hndGantt			= NULL;
  hndEnergy     = NULL;
	Genome				= NULL;
	ProbType		  = NonProblem;
	
	// Borrado rapido del grafo y todos los demas datos
	FastClear();
}



//------ Funciones Temporales
void CGraphCAPP::tmpPrint_TD (int init_machine_index_in)
{
	STRUCT_DATA_MACHINE data;

	// Se imprime la secuencia
	int i;
	printf("Seq: ");
	for (i = 0; i < iMachines * iJobs + iRecirc; i++)
	{
		if (i != 0 && (i + 1) % iJobs == 0 && (i + 1) < iMachines * iJobs)
			printf("%d - ", TD[i].ID);
		else
			printf("%d ", TD[i].ID);

	}
	printf("\n\n");
	
	// Se imprime la maquina de la operacion
	printf("Group:\n");
	for (i = 0; i < iMachines * iJobs + iRecirc; i++)
	{
		int op = TD[i].MachineGroup->GetMachineCount();

		printf("[%2d] Id: %d --> ", i, TD[i].ID);
		printf("|%d| ", op);
		for (int j = 0; j < op; j++)
		{
			data = TD[i].MachineGroup->GetMachineByIndex(j);
			printf("%d  %.1f | ", data.Machine + init_machine_index_in, data.Time);
		}
		printf("\n");
	}
	printf("\n\n");
}

void CGraphCAPP::tmpPrint_TR (int n, int init_machine_index_in)
{
	int i;

	if (n == -1)
	{
		printf("\n  Index:");
		for (i = 0; i < iMachines * iJobs + iRecirc; i++)	printf("%7d", i);
	
		printf("\n  RTime:");
		for (i = 0; i < iMachines * iJobs + iRecirc; i++)	printf("%7.1f", TR[i].RTime);

		printf("\nMachine:");
		for (i = 0; i < iMachines * iJobs + iRecirc; i++)	printf("%7d", TR[i].Machine + init_machine_index_in);

		printf("\n  DTime:");
		for (i = 0; i < iMachines * iJobs + iRecirc; i++)	printf("%7.1f", TR[i].DTime);
		printf("\n\n");	
	}
	else
	{
		int index;
		int c = iMachines * iJobs / n;

		for (int k = 0; k < c; k++)
		{
			
			printf("\n  Index:");
			for (i = 0; i < n; i++)	
			{
				index = k * n + i;
				printf("%3s[%2d]", " ", index);
			}

			printf("\n  RTime:");
			for (i = 0; i < n; i++)
			{
				index = k * n + i;
				printf("%7.1f", TR[index].RTime);
			}

			printf("\nMachine:");
			for (i = 0; i < n; i++)
			{
				index = k * n + i;
				printf("%7d", TR[index].Machine + init_machine_index_in);
			}

			printf("\n  DTime:");
			for (i = 0; i < n; i++)
			{
				index = k * n + i;
				printf("%7.1f", TR[index].DTime);
			}
			printf("\n");

			system("pause");
			printf("\n\n");
		}
	}
}

void CGraphCAPP::tmpPrint_DD (void)
{
	for (int i = 0; i < iJobs; i++)
	{
		printf("Id: %d    Di: %.1f\n", Di[i].idJob, Di[i].value);
	}
}

void CGraphCAPP::tmpPrint_Pw (int **tmp)
{
	float interval;
	int i, j;

	interval = Makespan() / iMeasureCount;
	printf("%6s", " ");
	for (i = 1; i <= (int)iMeasureCount; i++)
		printf("%-8.1f", interval * i);
	printf("\n");

	for (i = 0; i < iMachines; i++)
	{
		printf(" %-6s", MachineList->GetNameByIndex(i));	
		
		//printf("%6s", " ");
		for (j = 0; j < (int)iMeasureCount; j++)
			printf("%-8d", tmp[i][j]);
		printf("\n");
	}
	printf("\n");
}

///==================================================================================///
///==================================================================================///
///																																									 ///
///													Class CGraphCopier Definition														 ///	
///																																									 ///
///==================================================================================///
///==================================================================================///

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void CGraphCopier::Duplicate (CGraphCAPP *dest, const CGraphCAPP &src)
{
	// Valido que la cantidad de maquinas y de piezas sea mayor que cero. De 
	// no ser asi aborto la ejecucion del programa 
	assert(src.iJobs > 0 && src.iMachines > 0);

	// Copio los valores de intercambio	
	dest->ImpAnd				= 0;
	dest->Cmax					= 0.0;
	dest->isGraphTraveled = false;
	dest->applyIBUIT    = (dest->applyIBUIT) ? src.applyIBUIT : false;
	dest->iRecirc				= src.iRecirc;
	dest->iMeasureCount = src.iMeasureCount;
	dest->ProbType		  = src.ProbType;	
	dest->hndGantt			= src.hndGantt;
  dest->hndEnergy     = src.hndEnergy;
	dest->MachineList	  = src.MachineList;
	dest->ImpType			  = src.ImpType;
	dest->Performance	  = NULL;
	dest->Genome				= NULL;

	//=============// 
	//	Nota: No se establece ningun valor para los siguientes datos miembros, pues ellos no 
	//				son variables de intercambio de informacion sino variables de control en el grafo.
	//		- TM_ON
	//		- imgGraph
	//=============//

	dest->SetJobsList(src.JobsList);
	dest->SetEnergyMeasureCount(src.iMeasureCount);
	
	dest->flagCmax = src.flagCmax;
	dest->flagFmax = src.flagFmax;
	dest->flagLmax = src.flagLmax;
	dest->flagEmax = src.flagEmax;
	dest->flagTmax = src.flagTmax;
	dest->flagImax = src.flagImax;
	dest->flagNe   = src.flagNe;	
	dest->flagNt   = src.flagNt;	
	dest->flagPw   = src.flagPw;	

	if (dest->flagCmax) dest->ImpAnd += 2; 
	if (dest->flagFmax) dest->ImpAnd += 4; 
	if (dest->flagLmax) dest->ImpAnd += 8;
	if (dest->flagEmax) dest->ImpAnd += 16;
	if (dest->flagTmax) dest->ImpAnd += 32;
	if (dest->flagImax) dest->ImpAnd += 64;
	if (dest->flagNe)	  dest->ImpAnd += 128;
	if (dest->flagNt)	  dest->ImpAnd += 256;
	if (dest->flagPw)	  dest->ImpAnd += 512;

	if (src.iJobs	!= dest->iJobs || src.iMachines != dest->iMachines)
	{
		dest->iJobs			= src.iJobs;
		dest->iMachines	= src.iMachines;
				
		if (dest->TD)				delete []dest->TD;
		if (dest->TR)				delete []dest->TR;
		if (dest->Sequence) delete []dest->Sequence;
		if (dest->Di)				delete []dest->Di;
		if (dest->Ci)				delete []dest->Ci;
		if (dest->indexOpB) delete []dest->indexOpB;
		if (dest->m_block)  delete []dest->m_block;
		if (dest->j_block)  delete []dest->j_block;

		dest->TD			 = new CGraphCAPP::STRUCT_NODE		     [dest->iJobs * dest->iMachines + dest->iRecirc];
		dest->TR			 = new CGraphCAPP::STRUCT_NODE_RESULT [dest->iJobs * dest->iMachines + dest->iRecirc];
		dest->Di			 = new CGraphCAPP::STRUCT_DUEDATE     [dest->iJobs];
		dest->Sequence = new unsigned												 [dest->iJobs * dest->iMachines + dest->iRecirc];
		dest->Ci			 = new float													 [dest->iJobs];

		dest->indexOpB = new int      [dest->iJobs];
		dest->m_block  = new int      [dest->iMachines + 1 + dest->iRecirc];
		dest->j_block  = new unsigned [dest->iJobs + 1];
	}

	for(int i = 0; i < (dest->iJobs * dest->iMachines + dest->iRecirc); i++)
	{
		dest->TD[i].ID           = src.TD[i].ID;
		dest->TD[i].MachineGroup = src.TD[i].MachineGroup;
		dest->TR[i].RTime	       = src.TR[i].RTime;
		dest->TR[i].DTime	       = src.TR[i].DTime;
		dest->TR[i].Machine      = src.TR[i].Machine;
		dest->Sequence[i]        = src.Sequence[i];

		// Buscando eficiencia en el metodo de copiado, uso un solo ciclo para
		// copiar todos los datos.
		if (i < dest->iJobs)
		{
			dest->Ci[i]       = src.Ci[i];
			dest->Di[i].idJob = src.Di[i].idJob;
			dest->Di[i].value = src.Di[i].value;

			dest->indexOpB[i] = 0;
			dest->j_block[i]  = 0;
		}

		if (i < dest->iMachines + dest->iRecirc) dest->m_block[i] = -1;
	}
}
