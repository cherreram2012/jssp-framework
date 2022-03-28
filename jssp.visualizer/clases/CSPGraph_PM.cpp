#include "CSPGraph_PM.h"
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <assert.h>
#include <string.h>
#include <time.h>

#define DEBUG_HOLE 0

///==================================================================================///
///==================================================================================///
///																																									 ///
///													CSPGraph_PM Class Definition														 ///
///																																									 ///
///==================================================================================///
///==================================================================================///

//----------------------------------------------------------------------------------
//	Funcion de Utileria. Devuelve el tiempo de la operacion anterior misma pieza. 
//----------------------------------------------------------------------------------
float CSPGraph_PM::getTimeBackOp(int indexPi, unsigned id)
{
	for(int i = indexPi - 1 ; i >= 0; i--)
		if (TD[i].ID == id)	return TR[i].RTime;

	return 0;
}

//----------------------------------------------------------------------------------
//	Funcion de Utileria. Devuelve el tiempo de la pieza antetior misma maquina.
//----------------------------------------------------------------------------------
float CSPGraph_PM::getTimeBackJob(int indexPi, int machine, float *initTime)
{
	for(int i = indexPi - 1; i >= 0 ; i--)
	{
		if (TR[i].Machine == machine)
		{
			if (initTime) *initTime = TR[i].RTime - TD[i].MachineGroup->GetMachineById(machine).Time;
			return TR[i].RTime;
		}
	}

	return 0;
}

//----------------------------------------------------------------------------------
//	Funcion de Utileria. Devuelve el indice de la operacion anterior de la pieza
//	determinada por 'indexPi' e Id determinado por 'id'.
//----------------------------------------------------------------------------------
int CSPGraph_PM::getBackOpIndex (int indexPi, unsigned id, float *initTime)
{
	for(int i = indexPi - 1 ; i >= 0; i--)
	{
		if (TD[i].ID == id && TR[i].RTime != 0)
		{
			if (initTime) *initTime = TR[i].RTime - TR[i].DTime;

			return i;
		}
	}

	return -1;
}

//----------------------------------------------------------------------------------
//	Funcion de Utileria. Devuelve el indice de la pieza anterior a la pieza
//	determinada por 'indexPi' en la maquina determinada por 'machine'.
//----------------------------------------------------------------------------------
int CSPGraph_PM::getBackJobIndex (int indexPi, int machine, float *initTime)
{
	for(int i = indexPi - 1 ; i >= 0; i--)
	{
		if (TR[i].Machine == machine && TR[i].RTime != 0)
		{
			if (initTime) *initTime = TR[i].RTime - TR[i].DTime;

			return i;
		}
	}

	return -1;
}

//----------------------------------------------------------------------------------
//	Funcion de Utileria. Devuelve 1 si las dos operaciones pasadas por parametro son
//	igual, 0 en caso contrario. Para que las operaciones sean iguales, ID, Machine, 
//	RTime y DTime tienen que ser iguales.
//----------------------------------------------------------------------------------
bool CSPGraph_PM::EqualOperation (STRUCT_OPERATION Op1, STRUCT_OPERATION Op2)
{
	return (Op1.ID == Op2.ID && Op1.Machine == Op2.Machine && Op1.RTime == Op2.RTime && Op1.DTime == Op2.DTime) ? 1 : 0;
}

//----------------------------------------------------------------------------------
//	Funcion de borrado rapido del grafo. Esta funcion usa 1/3 de MxN para el borrado
//  de grafo, bastante rapido por cierto. Esto se logra subdividiendo la tarea en 
//	4 segmentos distintos, los cuales se ejecutan en paralelo.
//----------------------------------------------------------------------------------
void CSPGraph_PM::FastClear (void)
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
		Di[a].Value = 0;
		Ci[a]				= 0;
					
		indexOpB[a] = 0;
		j_block[a]  = 0;

		//--- 2do 1/2 ---//
		Di[d].idJob = 0;
		Di[d].Value = 0;
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
void CSPGraph_PM::CreateTR (void)
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
		//	 [1]. First In First Out       (FIFO): primero en llegar primero en ejecutar
		//	 [2]. Shortest Processing Time (SPT) : ejecutar primero la operacion mas corta
		//	 [3]. Earliest Completion Time (ECT) : ejecutar primero lo que acaba antes
		dM_best = dM;
		for (int k = 1; k < op; k++)
		{			
			dM = TD[i].MachineGroup->GetMachineByIndex(k);
			TPiTemp = getTimeBackJob(i, dM.Machine);

			if (TPiTemp < TPiAnterior && (TPiTemp + dM.Time) < (TPiAnterior + dM_best.Time))
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
void CSPGraph_PM::CreateTM_ON (void)
{
	int f, c;
	float step, instant, init, end;

	if (!isRunnerGraph) RunnerGraph();

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
void CSPGraph_PM::RunnerGraph (void)
{
	clock_t start, end;

	stepIBUIT = 0;
	timeIBUIT = 0;

	CreateTR();
	isRunnerGraph = true;
	
	if (applyIBUIT == true)
	{				
		ibuit_ok = false; 		

		start = clock();
		while(IBUIT_Technique());	// Se aplica la tecnica de IBUIT mientras mejore algun objetivo.
		end = clock();

		timeIBUIT = (double)(end - start);
	}

	if (Performance != NULL && ibuit_ok == true) (*Performance)++;
		
	CalculateCi();		// Calculo la permanencia de los trabajos en el sistema.
  CreateTM_ON();		// Creo la matriz de adyacencia para el calculo del consumo de energia.
}

//----------------------------------------------------------------------------------
//	Funcion de Utileria. Aplica a la secuencia el tecnica IBUIT (Improved Based in Use of the Idle Time)
//----------------------------------------------------------------------------------
bool CSPGraph_PM::IBUIT_Technique (void)
{
	unsigned id = 0;
	int space, indexOpC, machine;
	int ne, nt, re_ne, re_nt, re_ImpAnd = 0;
	float cmax, fmax, lmax, emax, tmax, imax, pw, re_cmax, re_fmax, re_lmax, re_emax, re_tmax, re_imax, re_pw;
	
	for (indexOpC = 1; indexOpC < (iJobs * iMachines + iRecirc); indexOpC++)	
	{																									
		id      = TD[indexOpC].ID;
		machine = TR[indexOpC].Machine;
		space		= SearchSpace(indexOpC, id, machine, indexOpB);

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
					if (flagNe)		ne	 = EarlyJobsCount();
					if (flagNt)		nt	 = TardinesJobsCount();
					if (flagPw)		pw	 = EnergyConsumption();

					imgGraph->ApplyForcedLeftJump(indexOpC, indexOpB[i]);

					if (flagCmax) re_cmax = imgGraph->Makespan();
					if (flagFmax) re_fmax = imgGraph->FlowTime();
					if (flagLmax) re_lmax = imgGraph->Lateness();
					if (flagEmax) re_emax = imgGraph->Earliness();
					if (flagTmax) re_tmax = imgGraph->Tardines();
					if (flagImax) re_imax = imgGraph->Idle();
					if (flagNe)		re_ne	  = imgGraph->EarlyJobsCount();
					if (flagNt)		re_nt	  = imgGraph->TardinesJobsCount();
					if (flagPw)		re_pw	  = imgGraph->EnergyConsumption();

					switch (ImpType)
					{
						case itAtLeastOne:							
							if ((flagCmax && re_cmax <= cmax) || (flagFmax && re_fmax <= fmax) || 
								  (flagLmax && re_lmax <= lmax) || (flagEmax && re_emax >= emax) || 
									(flagTmax && re_tmax <= tmax) || (flagImax && re_imax <= imax) || 
									(flagNe   && re_ne	 >= ne)   || (flagNt   && re_nt	  <= nt)   || 
									(flagPw   && re_pw	 <= pw))
							{
								ibuit_ok = true;
								ApplyForcedLeftJump(indexOpC, indexOpB[i]);	
								
								imgGraph->ClearGraph(); stepIBUIT++;
								return 1;
							}
						break;

						case itAllAtSameTime:						
							if (flagCmax && re_cmax <= cmax) re_ImpAnd += 2; 
							if (flagFmax && re_fmax <= fmax) re_ImpAnd += 4; 
							if (flagLmax && re_lmax <= lmax) re_ImpAnd += 8;
							if (flagEmax && re_emax >= emax) re_ImpAnd += 16;
							if (flagTmax && re_tmax <= tmax) re_ImpAnd += 32;
							if (flagImax && re_imax <= imax) re_ImpAnd += 64;
							if (flagNe	 && re_ne		>= ne)   re_ImpAnd += 128;
							if (flagNt	 && re_nt	  <= nt)	 re_ImpAnd += 256;
							if (flagPw	 && re_pw		<= pw)	 re_ImpAnd += 512;

							if (re_ImpAnd == ImpAnd)
							{
								ibuit_ok = true;
								ApplyForcedLeftJump(indexOpC, indexOpB[i]);

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

//----------------------------------------------------------------------------
//	Funcion de utileria. Determina si existe algun espacio entre el tiempo
//	de la operacion anterior-pieza actual y el tiempo de inicio de alguna
//	de las piezas anteriores. Devuelve por referencia los indices de las piezas
//  anteriores para las cuales se encontro un espacio.
//----------------------------------------------------------------------------
int CSPGraph_PM::SearchSpace (int indexPi, unsigned id, int machine, int *indexPiBack)
{
	float t_startPiActual, TOpAnterior, t_startBP;
	int pos, space = 0;

	t_startPiActual = TR[indexPi].RTime - TR[indexPi].DTime;

	// Se calcula el tiempo de la operacion anterior
	pos = getBackOpIndex(indexPi, id);
	TOpAnterior = (pos != -1) ? TR[pos].RTime : 0;

	// Se buscan posibles espacios para todas las piezas anteriores
	pos = getBackJobIndex(indexPi, machine, &t_startBP);
	while(pos != -1)
	{
		// ¿Es la 1ra operacion de la pieza actual la que se analiza?
		if (TOpAnterior == 0.0 && t_startBP != 0)
		{
			int tmp_pos = getBackJobIndex(pos, machine);
			
			if (tmp_pos != -1 && !EqualCero(TR[tmp_pos].RTime - t_startBP))
			{
				indexPiBack[space] = pos;
				space++;
			}
		}
		else if (t_startBP != 0 && t_startBP < t_startPiActual)
		{	
			if (t_startBP - TOpAnterior > 0.0)	// Espacio encontrado !!!
			{
				if (ExistsHole(pos, machine))		  // Esta verificacion es para resolver el caso
				{																  // especial que se produce cuando la operacion
					indexPiBack[space] = pos;			  // de la pieza anterior analizada es la 1ra 
					space++;											  // operacion de esta pieza.
				}
			}
		}

		pos = getBackJobIndex(pos, machine, &t_startBP);
	}

	return space;
}

//----------------------------------------------------------------------------
//	Funcion de utileria. Determina si existe un hueco en el espacio
//	determinado entre la pieza determinada por parametro 'indexPi' y 
//  alguna de las demas piezas anteriores en la maquina determinada por 'machine'.
//  Nota: existe hueco si ninguna de las piezas anteriores tiene su tiempo final
//			  igual al tiempo de inicio de la pieza 'indexPi'.
//----------------------------------------------------------------------------
bool CSPGraph_PM::ExistsHole (int indexPi, int machine)
{
	int pos;
	float t_startPiActual;

	t_startPiActual = TR[indexPi].RTime - TR[indexPi].DTime;

	pos = getBackJobIndex(indexPi, machine);
	while(pos != -1)
	{
		// Si la diferencia es cero es porque se encontro una pieza anterior (en la maquina
		// especificada por 'machine') que su tiempo (Ci) es igual al tiempo de inicio de 
		// la pieza actual. Lo que indica que no hay hueco.
		if (EqualCero(TR[pos].RTime - t_startPiActual)) return 0;

		pos = getBackJobIndex(pos, machine);
	}

	return 1;
}

//----------------------------------------------------------------------------------
//	Funcion de utileria. Esta funcion es la que realiza la tecnica IBUIT. 	
//----------------------------------------------------------------------------------
void CSPGraph_PM::Internal_IBUIT (int opCurrent, int opBack)
{
	STRUCT_OPERATION Pc, Pb;
	int i, posC, posB, major;

	// Que ingenioso!!!. Dos inicializaciones con un ciclo minimo.
	major = (iJobs > iMachines) ? iJobs : iMachines + iRecirc;
	for (i = 0; i < major; i++)
	{
		if (i < iJobs)     j_block[i] = 0;
		if (i < iMachines) m_block[i] = -1;
	}

	Pc.ID			 = TD[opCurrent].ID;
	Pc.Machine = TR[opCurrent].Machine;
	Pc.RTime	 = TR[opCurrent].RTime;
	Pc.DTime   = TR[opCurrent].DTime;

	Pb.ID			 = TD[opBack].ID;
	Pb.Machine = TR[opBack].Machine;
	Pb.RTime	 = TR[opBack].RTime;
	Pb.DTime   = TR[opBack].DTime;

	/* Rescheduling por desplazamiento */			
	posC = opCurrent;
	posB = opBack;
		
	j_block[0] = Pc.ID;
	m_block[0] = Pc.Machine;
	while (posC > posB && posC != -1)
	{					
		posC = LeftOffSet(posC, j_block, m_block, 1, 1, posB, Pc.Machine, Pb, &posB);
			
		// Nota: i = 1, porque los bloques de maquinas y piezas comienzan con 
		//			 el ID y la Machine de la pieza actual (Pc).	
		for (i = 1; i < major; i++)
		{
			if (i < iJobs)     j_block[i] = 0;
			if (i < iMachines) m_block[i] = -1;
		}			
	}
}

//----------------------------------------------------------------------------------
//	Funcion de utileria. Esta funcion hace un desplazamiento a la izquierda de la 
//  operacion determinada por 'pos'. Devuelve la nueva posicion despues de realizada
//	la operacion. O -1 en caso de que el desplazamiento no haya sido realizado.	
//----------------------------------------------------------------------------------
int CSPGraph_PM::LeftOffSet (int pc_pos, unsigned *j_block, int *m_block, int j_count, int m_count, int target, int m_fija, STRUCT_OPERATION Pb, int *pb_pos)
{
	int result;
	float tmpInit;
	unsigned tmpId;
	bool j_found, m_found;
	STRUCT_OPERATION tmpOp;
	const CMachineGroupLite *tmpMGroup;	
	
	// Caso base
	if (pc_pos == 0) return -1; 
	
	result = 0;
	j_found = m_found = false;
	
	// Determino si la pieza de la operacion anterior esta en el bloque de jobs
	for (int j = 0; j < j_count; j++)
		if (TD[pc_pos - 1].ID == j_block[j]) j_found = true;

	// Determino si la maquina de la operacion anterior esta en el bloque de machines
	for (int m = 0; m < m_count; m++)
		if (TR[pc_pos - 1].Machine != m_fija && TR[pc_pos - 1].Machine == m_block[m]) m_found = true;

	if (!j_found && m_found)
	{
		j_block[j_count] = TD[pc_pos - 1].ID;
		j_count++;
	}
	if (!m_found && j_found && TR[pc_pos - 1].Machine != -1)
	{
		m_block[m_count] = TR[pc_pos - 1].Machine;
		m_count++;
	} 	
		
	// Llamada recursiva!!!
	if ((j_found == true || m_found == true) && pc_pos - 1 != target)
	{
		result = LeftOffSet(pc_pos - 1, j_block, m_block, j_count, m_count, target, m_fija, Pb, pb_pos);
	}

	// Desplazamiento a la izquierda de la operacion 'pos'
	if (result != -1)
	{
		tmpOp.ID      = TD[pc_pos - 1].ID; 
		tmpMGroup			= TD[pc_pos - 1].MachineGroup;
		tmpOp.Machine = TR[pc_pos - 1].Machine;
		tmpOp.RTime   = TR[pc_pos - 1].RTime;
		tmpInit       = TR[pc_pos - 1].DTime;
		tmpId					= Sequence[pc_pos - 1]; 
			
		TD[pc_pos - 1].ID           = TD[pc_pos].ID; 
		TD[pc_pos - 1].MachineGroup = TD[pc_pos].MachineGroup;
		TR[pc_pos - 1].Machine      = TR[pc_pos].Machine;
		TR[pc_pos - 1].RTime        = TR[pc_pos].RTime;
		TR[pc_pos - 1].DTime        = TR[pc_pos].DTime;
		Sequence[pc_pos - 1]		    = Sequence[pc_pos];

		TD[pc_pos].ID			      = tmpOp.ID; 	
		TD[pc_pos].MachineGroup = tmpMGroup;
		TR[pc_pos].Machine      = tmpOp.Machine;
		TR[pc_pos].RTime        = tmpOp.RTime;
		TR[pc_pos].DTime        = tmpInit;
		Sequence[pc_pos]		    = tmpId;

		// Que ingenioso!!!. Con esta solucion me quite de arriba la funcion <PosInSequence>
		// que hacia una busqueda para ubicar la posicion de la pieza a saltar. 
		// La funcion trabajaba bien, el problema era que debido a las reiteradas llamadas, 
		// una por cada intento de movimiento, la tecnica IBUIT se tornaba algo costosa.
		//
		// Si la pieza ubicada a la derecha del intercambio es precisamente
		// la pieza a saltar, entonces actualizo la posicion en la que quedo finalmente.
		if (Pb.ID      == TD[pc_pos].ID			 && 
				Pb.Machine == TR[pc_pos].Machine && 
				Pb.RTime   == TR[pc_pos].RTime	 && 
				Pb.DTime   == TR[pc_pos].DTime)
		{
			*pb_pos = pc_pos;
		}

		return pc_pos - 1;
	}

	return -1;	
}

//----------------------------------------------------------------------------------
//	Funcion de utileria. Calcula la permanencia de cada trabajo en el 
//  sistema a partir de los datos en TR. Los valores calculados son ubicados 
//  en el dato miembro 'Cir[]'.
//  Nota: Para que los calculos de permanencia real sean calculados correctamente
//  la funcion debe ser llamada una vez recorrido el grafo. 	
//----------------------------------------------------------------------------------
void CSPGraph_PM::CalculateCi (void)
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
int CSPGraph_PM::IndexInDueDateVector (unsigned id)
{	
	if (id > 0 && id <= (unsigned)iJobs) return Di[id - 1].idJob;
				
	return -1;
}

//----------------------------------------------------------------------------------
//	Constructor de la clase.
//----------------------------------------------------------------------------------
CSPGraph_PM::CSPGraph_PM (int piece, int machine, int recirc, bool apply_ibuit)
{
	// Valido que la cantidad de operaciones y de piezas sea mayor que cero. De 
	// no ser asi aborto la ejecucion del programa 
	assert(machine > 0 && piece > 0);

	iJobs					= piece;
	iMachines			= machine;
	iRecirc				= recirc;
	Cmax					= 0.0;
	iMeasureCount	= 10;
	isRunnerGraph	= false;
  TM_ON         = NULL;
	JobsList			= NULL;
	hndGantt			= NULL;
  hndEnergy     = NULL;
	imgGraph			= NULL;
	MachineList		= NULL;
	Performance		= NULL;
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

	if (apply_ibuit) imgGraph = new CSPGraph_PM(piece, machine, recirc, false);
}

//----------------------------------------------------------------------------------
// Constructor de Copia.
//----------------------------------------------------------------------------------
CSPGraph_PM::CSPGraph_PM (const CSPGraph_PM &Obj)
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

	if (applyIBUIT) imgGraph = new CSPGraph_PM(iJobs, iMachines, recirc, false);

	indexOpB = new int [iJobs];
	m_block  = new int [iMachines + 1 + iRecirc];
	j_block  = new unsigned [iJobs + 1];	
#endif

	//const unsigned *sequence;
	
	iJobs					= Obj.GetPieceCount();
	iMachines			= Obj.GetMachineCount();
	Cmax					= 0.0;
	iMeasureCount	= Obj.GetEnergyMeasureCount();  
	isRunnerGraph	= false;
	applyIBUIT    = Obj.ApplyIBUIT();
	ProbType      = Obj.GetProblemType();
	//sequence      = Obj.GetSequence();
  TM_ON         = NULL;
	JobsList			= NULL;
	hndGantt			= NULL;
  hndEnergy     = NULL;
	imgGraph			= NULL;
	Performance		= NULL;
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
	
	if (applyIBUIT) imgGraph = new CSPGraph_PM(iJobs, iMachines, iRecirc, false);
}

//----------------------------------------------------------------------------------
// Destructor de la clase.
//----------------------------------------------------------------------------------
CSPGraph_PM::~CSPGraph_PM (void)
{	
	hndGantt		= NULL;
  hndEnergy   = NULL;
	MachineList	= NULL;
	Performance	= NULL;

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

//----------------------------------------------------------------------------------
//	Establece los datos (time y maquina) para un trabajo determinado.
//----------------------------------------------------------------------------------
void CSPGraph_PM::SetNodeValue (int index, unsigned id, const CMachineGroupLite *machines)
{
	if (index >= 0 && index < (iJobs * iMachines + iRecirc))
	{
		TD[index].ID = id;
		TD[index].MachineGroup = machines;
	}
}

//----------------------------------------------------------------------------
//	Establece la secuencia que representa el grafo.
//----------------------------------------------------------------------------
void CSPGraph_PM::SetSequence (const unsigned *sequence)
{
	for (int i = 0; i < iJobs * iMachines + iRecirc; i++)	Sequence[i] = sequence[i];
}

//----------------------------------------------------------------------------
//	Establece el manipulador de la interfaz donde sera vizualizado el diagrama
//	de Gantt.
//----------------------------------------------------------------------------
void CSPGraph_PM::SetGantChartView (TObserverGantt *handler)
{
	hndGantt = handler;
}

//----------------------------------------------------------------------------
//	Establece el manipulador de la interfaz donde sera vizualizado el grafico
//	de Energia.
//----------------------------------------------------------------------------
void CSPGraph_PM::SetEnergyChartView	(TObserverEnergy *handler)
{
  hndEnergy = handler;
}

//----------------------------------------------------------------------------------
//	Establece la informacion relacionada con las maquinas. 
//----------------------------------------------------------------------------------
void CSPGraph_PM::SetMachineInformation (const CMachineCollection_PM *Obj)
{
	MachineList = Obj;
}

//----------------------------------------------------------------------------
//	Establece la lista de trabajos que es usada por los objetivos Fmax, Lmax, 
//	Emax, Tmax al evaluar la secuencia para estos trabajos.
//----------------------------------------------------------------------------
void CSPGraph_PM::SetJobsList (unsigned *jobs_list)
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
void CSPGraph_PM::SetImproveType (enum ImproveType type)
{
	ImpType = type;
}

//----------------------------------------------------------------------------
//	Establece el tipo de problema que representa el grafo.
//----------------------------------------------------------------------------
void CSPGraph_PM::SetProblemType	(enum ProblemType type)
{
	ProbType = type;
}

//----------------------------------------------------------------------------
//	Establece el intervalo que sera usado para el calculo del consumo de 
//	Energia. Por defecto esta valor es '10'.
//----------------------------------------------------------------------------
void CSPGraph_PM::SetEnergyMeasureCount (unsigned count)
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
	isRunnerGraph = false;
}

//----------------------------------------------------------------------------
//	Establece los objetivos que se trataran de mejorar con la tecnica IBUIT.
//----------------------------------------------------------------------------
void CSPGraph_PM::SetImproveObjetive (bool cmax, bool fmax, bool lmax, bool emax, bool tmax, bool imax, bool ne, bool nt, bool pw)
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
bool CSPGraph_PM::SetDueDate (unsigned id, float di)
{
	if (id > 0 && id <= (unsigned)iJobs)
	{
		Di[id - 1].idJob = id;
		Di[id - 1].Value = di;
	
		return 1;
	}

	return 0;
}

//----------------------------------------------------------------------------
//	Establece el vinculo a una variable donde almacenar el record de las veces
//	que fue satisfactorio aplicar la tecnica IBUIT.	
//----------------------------------------------------------------------------
void CSPGraph_PM::SetLinkToPerformance	(unsigned long &count)
{
	Performance = &count;
}

//----------------------------------------------------------------------------
//	Devuelve la cantidad de piezas que contiene el grafo.
//----------------------------------------------------------------------------
int CSPGraph_PM::GetPieceCount (void) const
{
	return iJobs;
}

//----------------------------------------------------------------------------
//	Devuelve la cantidad de maquinas que contiene el grafo.
//----------------------------------------------------------------------------
int CSPGraph_PM::GetMachineCount (void) const
{
	return iMachines;
}

//----------------------------------------------------------------------------
//	Devuelve la secuencia que representa al grafo.
//----------------------------------------------------------------------------
const unsigned *CSPGraph_PM::GetSequence (void) const
{
	return Sequence;
}

//----------------------------------------------------------------------------
//	Devuelve el tipo de mejora para los objetivos
//----------------------------------------------------------------------------
enum ImproveType CSPGraph_PM::GetImproveType (void) const
{
	return ImpType;
}

//----------------------------------------------------------------------------
//	Devuelve el tipo de problema que representa el grafo.
//----------------------------------------------------------------------------
enum ProblemType CSPGraph_PM::GetProblemType	(void) const
{
	return ProbType;
}

//----------------------------------------------------------------------------
//	Devuelve la lista de trabajos que es usada por los objetivos Fmax, Lmax, 
//	Emax, Tmax al evaluar la secuencia para estos trabajos.
//----------------------------------------------------------------------------
unsigned *CSPGraph_PM::GetJobsList (void) const
{
	return JobsList;
}

//----------------------------------------------------------------------------
//	Devuelve la cantidad de mediciones que se usan para al calculo de consumo
//  de energia.
//----------------------------------------------------------------------------
unsigned CSPGraph_PM::GetEnergyMeasureCount (void) const
{
	return iMeasureCount;
}

//----------------------------------------------------------------------------
//	Devuelve por referencia los datos de la posicion en el grafo determinada por
//	index.
//----------------------------------------------------------------------------
void CSPGraph_PM::GetValue (int index, unsigned *id, const CMachineGroupLite **machines) const
{
	if (id) *id =  0;
	if (machines) *machines = NULL;

	if (index >= 0 && index < iJobs * iMachines + iRecirc) 
	{
		if (id) *id	= TD[index].ID;
		if (machines) *machines = TD[index].MachineGroup;		
	}
}

//----------------------------------------------------------------------------
//	Devuelve por referencia el valor de duedate y el id de la pieza determinada
//	por el parametro 'index'.
//----------------------------------------------------------------------------
void CSPGraph_PM::GetDueDate (int index, unsigned *id, float *di) const
{
	if (index >= 0 && index < iJobs)
	{
		*id = Di[index].idJob; 
		*di = Di[index].Value;
	}
	else
	{
		*id = 0; 
		*di = 0;
	}
}

//----------------------------------------------------------------------------
//	Devuelve por referencia el estado de los objetivos indicando si el objetivo
//	se tiene un cuenta para mejorar al aplicar la tecnica IBUIT. 
//----------------------------------------------------------------------------
void CSPGraph_PM::GetImproveObjetive (bool *cmax, bool *fmax, bool *lmax, bool *emax, bool *tmax, bool *imax, bool *ne, bool *nt, bool *pw) const
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
TObserverGantt *CSPGraph_PM::GetGantChartView (void) const
{
	return hndGantt;
}

//----------------------------------------------------------------------------
//	Devuelve el manipulador de la interfaz donde sera vizualizado el diagrama
//	de Energia.
//----------------------------------------------------------------------------
TObserverEnergy *CSPGraph_PM::GetEnergyChartView	(void) const
{
	return hndEnergy;
}

//----------------------------------------------------------------------------
//	Devuelve el puntero a la variable donde se lleva el record de las veces 
//	que satisfactorio la aplicacion de la tecnica IBUIT.
//----------------------------------------------------------------------------
unsigned long *CSPGraph_PM::GetLinkToPerformance (void) const
{
	return Performance;
}

//----------------------------------------------------------------------------
//	Devuelve el puntero al objeto que contiene la informacion de la maquinas.
//----------------------------------------------------------------------------
const CMachineCollection_PM *CSPGraph_PM::GetMachineInformation (void) const
{
	return MachineList;
}

//----------------------------------------------------------------------------
//	Devuelve si se aplica o no el re-scheduling al recorrer el grafo.
//----------------------------------------------------------------------------
bool CSPGraph_PM::ApplyIBUIT (void) const
{
	return applyIBUIT;
}

//----------------------------------------------------------------------------
//	Establece si se aplica o no la tecnica IBUIT al recorrer el grafo.
//----------------------------------------------------------------------------
void CSPGraph_PM::ApplyIBUIT (bool apply)
{
	applyIBUIT = apply;

	if (applyIBUIT == true && imgGraph == NULL)
	{		
		imgGraph = new CSPGraph_PM(iJobs, iMachines, iRecirc, false);
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
void CSPGraph_PM::ApplyForcedLeftJump (int opCurrent, int opBack)
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
float CSPGraph_PM::Makespan (void)
{	
	// Si todavia no se ha recorrido el grafo, lo recorro ahora
	if (!isRunnerGraph) RunnerGraph(); 

	return Cmax;
}

//----------------------------------------------------------------------------
//	Calcula el flujo total de los trabajos pasados en el argumento 'jobs_list'
//  en el sistema.
//----------------------------------------------------------------------------
float CSPGraph_PM::FlowTime (unsigned *jobs_list)
{
	float sum_Pr = 0;
	int k;

	// Protejo la ejecucion frente a una ausencia de la lista de trabajo,
	// la cual es imprescindible para evaluar este objetivo.
	assert(JobsList != NULL || jobs_list != NULL);

	// Si todavia no se ha recorrido el grafo, lo recorro ahora
	if (!isRunnerGraph) RunnerGraph(); 
	
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
float CSPGraph_PM::Lateness (unsigned *jobs_list)
{
	float Li, L = 0.0;
	int k;

	// Protejo la ejecucion frente a una ausencia de la lista de trabajo,
	// la cual es imprescindible para evaluar este objetivo.
	assert(JobsList != NULL || jobs_list != NULL);

	// Si todavia no se ha recorrido el grafo, lo recorro ahora
	if (!isRunnerGraph) RunnerGraph();
	
	for (int i = 0; i < iJobs; i++)
	{
		if (jobs_list) 
			k = IndexInDueDateVector(jobs_list[i]);
		else
			k = IndexInDueDateVector(JobsList[i]);

		if (k != -1)
		{
			Li = (!EqualCero(Ci[k] - Di[k].Value)) ? Ci[k] - Di[k].Value: 0.0f;
			L += Li;
		}
	}
	
	return L;
}

//----------------------------------------------------------------------------
//	Calcula el Adelanto 'E' = |-Lmax| de los trabajos pasados por parametro.
//----------------------------------------------------------------------------
float CSPGraph_PM::Earliness (unsigned *jobs_list)
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
float CSPGraph_PM::Tardines (unsigned *jobs_list)
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
float CSPGraph_PM::Idle (void)
{
	unsigned id = 0;
	int space, indexOpC, machine;
	float TOpActual, t_startBP, TPiAnt_Ant, idle = 0;

	// Si todavia no se ha recorrido el grafo, lo recorro ahora
	if (!isRunnerGraph) RunnerGraph(); 

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
float CSPGraph_PM::EnergyConsumption (void)
{
	float consume = 0;

	// Si todavia no se ha recorrido el grafo, lo recorro ahora
	if (!isRunnerGraph) RunnerGraph();

	for (int i = 0; i < (int)iMeasureCount; i++)
		for (int m = 0; m < iMachines; m++)
      if (TM_ON[m][i]) consume += (MachineList->GetNominalPower(m) * Cmax / iMeasureCount);
	
	return consume;
}

//----------------------------------------------------------------------------
//	Devuelve la cantidad de trabajos con tiempo de finalizacion adelantados
//	o en tiempo. 
//----------------------------------------------------------------------------
int	CSPGraph_PM::EarlyJobsCount (void)
{
	int count = 0;
	
	// Si todavia no se ha recorrido el grafo, lo recorro ahora
	if (!isRunnerGraph) RunnerGraph();
	
	for (int i = 0; i < iJobs; i++) 
		if (Ci[i] - Di[i].Value <= 0) count++;

	return count;
}

//----------------------------------------------------------------------------
//	Devuelve la cantidad de trabajos fuera del plazo de entrega. 
//----------------------------------------------------------------------------
int	CSPGraph_PM::TardinesJobsCount (void)
{
	int count = 0;
	
	// Si todavia no se ha recorrido el grafo, lo recorro ahora
	if (!isRunnerGraph) RunnerGraph();
	
	for (int i = 0; i < iJobs; i++) 
		if (Ci[i] - Di[i].Value > 0) count++;

	return count;
}

//----------------------------------------------------------------------------
//	Devuelve el tipo de schedule que representa el grafo.
//----------------------------------------------------------------------------
enum ScheduleType CSPGraph_PM::SchedulePlannig (void)
{
	unsigned id = 0;
	int space, indexOpC, machine, semi_active = 0;
	float TDPiActual, t_startBP, TPiAnt_Ant, TOPAnterior;

	// Si todavia no se ha recorrido el grafo, lo recorro ahora
	if (!isRunnerGraph) RunnerGraph(); 

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
					TDPiActual  = TR[indexOpC].DTime;
					TOPAnterior = getTimeBackOp(indexOpC,  TD[indexOpC].ID);
					t_startBP   = TR[indexOpB[i]].RTime - TR[indexOpB[i]].DTime;
					TPiAnt_Ant  = getTimeBackJob(indexOpB[i], machine);
					
					// ¿Cabe la pieza actual en el hueco encontrado sin que retrase 
					// el comienzo de alguna otra piezas?
					float time;
					if (TOPAnterior > TPiAnt_Ant)
						time = (t_startBP - TOPAnterior) - TDPiActual;
					else
						time = (t_startBP - TPiAnt_Ant) - TDPiActual;

					if (time > 0.0 || EqualCero(time))	
						semi_active++;
					else 
						return stActive;
				}
			}
		}
	}

	return (semi_active) ? stSemiActive : stNonDelay;
}

//----------------------------------------------------------------------------
//	Muestra un diagrama de Gantt en el visor designado por <HndGantt>  
//	GraphTime (int operation, float startT, float elapsedT, unsigned ID)
//----------------------------------------------------------------------------
void CSPGraph_PM::CreateGanttChart (void)
{
	int j, op;

	// Si todavia no se ha recorrido el grafo, se corre ahora
	if (!isRunnerGraph) RunnerGraph();

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
  int *op_job     = new int[iJobs + 1];

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

  // Tratamiento de la RECIRCULACION, si la hay
  if (iRecirc)
  {
    int init = iJobs * iMachines;
    int end = init + iRecirc;

    for (int r = init; r < end; r++)
      if (TR[r].RTime)
				hndGantt->GraphTime(TR[r].Machine, TR[r].RTime - TR[r].DTime, TR[r].DTime, Sequence[r], seq_op_num[r]);
  }

  hndGantt->SetMeasureMark(iMeasureCount);  // Establece y prepara las marcas de las lecturas
																						// del consumo de energia
  float step = Cmax / iMeasureCount;
  for (int i = 0; i < (int)iMeasureCount; i++)
    hndGantt->GraphMeasureMark(i, step * (i + 1));

  delete []seq_op_num;
  delete []op_job;
}

//----------------------------------------------------------------------------
//	Muestra un grafico de energia en el visor designado por <hndEnergy>
//	GraphInterval (int interval, float machine_power, char *machine_list[], int machine_on)
//----------------------------------------------------------------------------
void CSPGraph_PM::CreateEnergyChart (void)
{
  int i, machine_on, index, len;
  float consume, ec;
  char **list;

  list = new char *[iMachines];
  len  = MachineList->GetMaxNameLength();
  for (i = 0; i < iMachines; i++) list[i] = new char [len + 1];

	// Si todavia no se ha recorrido el grafo, se corre ahora
	if (!isRunnerGraph) RunnerGraph();

  ec = EnergyConsumption();
  hndEnergy->SetDataChart(Cmax, iMeasureCount);

	for (i = 0; i < (int)iMeasureCount; i++)
  {
    machine_on = 0;
    consume    = 0;
    index      = 0;
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
  delete []list;
}

//----------------------------------------------------------------------------
//  Devuelve el consumo de potencia maximo.
//----------------------------------------------------------------------------
float CSPGraph_PM::GetMaxEnergyConsumption (void)
{
  float consume = 0.0, aux;

	// Si todavia no se ha recorrido el grafo, se corre ahora
	if (!isRunnerGraph) RunnerGraph();

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
void CSPGraph_PM::CopyGraph (const CSPGraph_PM &Obj)
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
void CSPGraph_PM::ClearGraph (void)
{
	// Pongo las variables de intercambio y control a su estado inicial
	Cmax					= 0.0;
	isRunnerGraph = false;
	Performance		= NULL;
	hndGantt			= NULL;
  hndEnergy     = NULL;
	ProbType		  = NonProblem;
	
	// Borrado rapido del grafo y todos los demas datos
	FastClear();
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
void CGraphCopier::Duplicate (CSPGraph_PM *dest, const CSPGraph_PM &src)
{
	// Valido que la cantidad de maquinas y de piezas sea mayor que cero. De 
	// no ser asi aborto la ejecucion del programa 
	assert(src.iJobs > 0 && src.iMachines > 0);

	// Copio los valores de intercambio	
	dest->ImpAnd				= 0;
	dest->Cmax					= 0.0;
	dest->isRunnerGraph = false;
	dest->applyIBUIT    = (dest->applyIBUIT) ? src.applyIBUIT : false;
	dest->iRecirc				= src.iRecirc;
	dest->iMeasureCount = src.iMeasureCount;
	dest->ProbType		  = src.ProbType;	
	dest->hndGantt			= src.hndGantt;
  dest->hndEnergy     = src.hndEnergy;
	dest->MachineList	  = src.MachineList;
	dest->ImpType			  = src.ImpType;
	dest->Performance	  = NULL;

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

		dest->TD			 = new CSPGraph_PM::STRUCT_NODE		     [dest->iJobs * dest->iMachines + dest->iRecirc];
		dest->TR			 = new CSPGraph_PM::STRUCT_NODE_RESULT [dest->iJobs * dest->iMachines + dest->iRecirc];
		dest->Di			 = new CSPGraph_PM::STRUCT_DUEDATE     [dest->iJobs];
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
			dest->Di[i].Value = src.Di[i].Value;

			dest->indexOpB[i] = 0;
			dest->j_block[i]  = 0;
		}

		if (i < dest->iMachines + dest->iRecirc) dest->m_block[i] = -1;
	}
}
