#include "CGraphBase.h"

//----------------------------------------------------------------------------------
//	Funcion de Utileria. Devuelve el tiempo de la operacion anterior misma pieza. 
//----------------------------------------------------------------------------------
float CGraphBase::getTimeBackOp(int indexPi, unsigned id)
{
	for (int i = indexPi - 1; i >= 0; i--)
		if (TD[i].ID == id)	return TR[i].RTime;

	return 0;
}

//----------------------------------------------------------------------------------
//	Funcion de Utileria. Devuelve el tiempo de la pieza antetior misma maquina.
//----------------------------------------------------------------------------------
float CGraphBase::getTimeBackJob(int indexPi, int machine, float *initTime)
{
	for (int i = indexPi - 1; i >= 0; i--)
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
int CGraphBase::getBackOpIndex(int indexPi, unsigned id, float *initTime)
{
	for (int i = indexPi - 1; i >= 0; i--)
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
int CGraphBase::getBackJobIndex(int indexPi, int machine, float *initTime)
{
	for (int i = indexPi - 1; i >= 0; i--)
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
//	Funcion de Utileria. Aplica a la secuencia el tecnica IBUIT (Improved Based in Use of the Idle Time)
//----------------------------------------------------------------------------------
bool CGraphBase::IBUIT_Technique(void)
{
	unsigned id = 0;
	int space, indexOpC, machine;
	int ne, nt, re_ne, re_nt, re_ImpAnd = 0;
	float cmax, re_cmax;
	//float cmax, fmax, lmax, emax, tmax, imax, pw, re_cmax, re_fmax, re_lmax, re_emax, re_tmax, re_imax, re_pw;

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
					//CGraphCopier::Duplicate(imgGraph, *this);
					//imgGraph->ApplyIBUIT(false);

					cmax = Makespan();
					/*if (flagCmax) cmax = Makespan();
					if (flagFmax) fmax = FlowTime();
					if (flagLmax) lmax = Lateness();
					if (flagEmax) emax = Earliness();
					if (flagTmax) tmax = Tardines();
					if (flagImax) imax = Idle();
					if (flagNe)		ne = EarlyJobsCount();
					if (flagNt)		nt = TardinesJobsCount();
					if (flagPw)		pw = EnergyConsumption();*/

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

					//imgGraph->ApplyForcedLeftJump(indexOpC, indexOpB[i]);

					//re_cmax = imgGraph->Makespan();
					/*if (flagCmax) re_cmax = imgGraph->Makespan();
					if (flagFmax) re_fmax = imgGraph->FlowTime();
					if (flagLmax) re_lmax = imgGraph->Lateness();
					if (flagEmax) re_emax = imgGraph->Earliness();
					if (flagTmax) re_tmax = imgGraph->Tardines();
					if (flagImax) re_imax = imgGraph->Idle();
					if (flagNe)		re_ne = imgGraph->EarlyJobsCount();
					if (flagNt)		re_nt = imgGraph->TardinesJobsCount();
					if (flagPw)		re_pw = imgGraph->EnergyConsumption();*/

					/*switch (ImpType)
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
								}*/

#if DEBUG_HOLE
							printf("\n\nDESPUES: ");
							for (int j = 0; j < (iJobs * iMachines + iRecirc); j++)printf("%d ", Sequence[j]);
							printf("\nCmax: %0.1f", re_cmax);
							printf("\nCon exito!!!!!!!\n\n\n");
							getch();
#endif
							/*imgGraph->ClearGraph(); stepIBUIT++;
							return 1;
						}*/
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
						/*break;

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
					}*/
				}

				//imgGraph->ClearGraph();
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
int CGraphBase::SearchSpace(int indexPi, unsigned id, int machine, int *indexPiBack)
{
	float t_startPiActual, TOpAnterior, t_startBP;
	int pos, space = 0;

	t_startPiActual = TR[indexPi].RTime - TR[indexPi].DTime;

	// Se calcula el tiempo de la operacion anterior
	pos = getBackOpIndex(indexPi, id);
	TOpAnterior = (pos != -1) ? TR[pos].RTime : 0;

	// Se buscan posibles espacios para todas las piezas anteriores
	pos = getBackJobIndex(indexPi, machine, &t_startBP);
	while (pos != -1)
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

//----------------------------------------------------------------------------------
//	Funcion de utileria. Esta funcion hace un desplazamiento a la izquierda de la 
//  operacion determinada por 'pos'. Devuelve la nueva posicion despues de realizada
//	la operacion. O -1 en caso de que el desplazamiento no haya sido realizado.	
//----------------------------------------------------------------------------------
int CGraphBase::LeftOffSet(int pc_pos, unsigned *j_block, int *m_block, int j_count, int m_count, int target, int m_fija, STRUCT_OPERATION Pb, int *pb_pos)
{
	int result;
	float tmpInit;
	unsigned tmpId, tmpAtom;
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
		tmpOp.ID = TD[pc_pos - 1].ID;
		tmpMGroup = TD[pc_pos - 1].MachineGroup;
		tmpOp.Machine = TR[pc_pos - 1].Machine;
		tmpOp.RTime = TR[pc_pos - 1].RTime;
		tmpInit = TR[pc_pos - 1].DTime;
		tmpId = Sequence[pc_pos - 1];
		tmpAtom = schedule.GetIdByIndex(pc_pos - 1);

		TD[pc_pos - 1].ID = TD[pc_pos].ID;
		TD[pc_pos - 1].MachineGroup = TD[pc_pos].MachineGroup;
		TR[pc_pos - 1].Machine = TR[pc_pos].Machine;
		TR[pc_pos - 1].RTime = TR[pc_pos].RTime;
		TR[pc_pos - 1].DTime = TR[pc_pos].DTime;
		Sequence[pc_pos - 1] = Sequence[pc_pos];
		schedule.SetIdByIndex(pc_pos - 1, TD[pc_pos].ID);

		TD[pc_pos].ID = tmpOp.ID;
		TD[pc_pos].MachineGroup = tmpMGroup;
		TR[pc_pos].Machine = tmpOp.Machine;
		TR[pc_pos].RTime = tmpOp.RTime;
		TR[pc_pos].DTime = tmpInit;
		Sequence[pc_pos] = tmpId;
		schedule.SetIdByIndex(pc_pos, tmpAtom);

		// Que ingenioso!!!. Con esta solucion me quite de arriba la funcion <PosInSequence>
		// que hacia una busqueda para ubicar la posicion de la pieza a saltar. 
		// La funcion trabajaba bien, el problema era que debido a las reiteradas llamadas, 
		// una por cada intento de movimiento, la tecnica IBUIT se tornaba algo costosa.
		//
		// Si la pieza ubicada a la derecha del intercambio es precisamente
		// la pieza a saltar, entonces actualizo la posicion en la que quedo finalmente.
		if (Pb.ID == TD[pc_pos].ID				 &&
			Pb.Machine == TR[pc_pos].Machine &&
			Pb.RTime == TR[pc_pos].RTime	   &&
			Pb.DTime == TR[pc_pos].DTime)
		{
			*pb_pos = pc_pos;
		}

		return pc_pos - 1;
	}

	return -1;
}

//----------------------------------------------------------------------------
//	Funcion de utileria. Determina si existe un hueco en el espacio
//	determinado entre la pieza determinada por parametro 'indexPi' y 
//  alguna de las demas piezas anteriores en la maquina determinada por 'machine'.
//  Nota: existe hueco si ninguna de las piezas anteriores tiene su tiempo final
//			  igual al tiempo de inicio de la pieza 'indexPi'.
//----------------------------------------------------------------------------
bool CGraphBase::ExistsHole(int indexPi, int machine)
{
	int pos;
	float t_startPiActual;

	t_startPiActual = TR[indexPi].RTime - TR[indexPi].DTime;

	pos = getBackJobIndex(indexPi, machine);
	while (pos != -1)
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
void CGraphBase::Internal_IBUIT(int opCurrent, int opBack)
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

	Pc.ID = TD[opCurrent].ID;
	Pc.Machine = TR[opCurrent].Machine;
	Pc.RTime = TR[opCurrent].RTime;
	Pc.DTime = TR[opCurrent].DTime;

	Pb.ID = TD[opBack].ID;
	Pb.Machine = TR[opBack].Machine;
	Pb.RTime = TR[opBack].RTime;
	Pb.DTime = TR[opBack].DTime;

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
//	Funcion de Utileria. Devuelve 1 si las dos operaciones pasadas por parametro son
//	igual, 0 en caso contrario. Para que las operaciones sean iguales, ID, Machine, 
//	RTime y DTime tienen que ser iguales.
//----------------------------------------------------------------------------------
bool CGraphBase::EqualOperation(STRUCT_OPERATION Op1, STRUCT_OPERATION Op2)
{
	return (Op1.ID == Op2.ID && Op1.Machine == Op2.Machine && Op1.RTime == Op2.RTime && Op1.DTime == Op2.DTime) ? 1 : 0;
}

//----------------------------------------------------------------------------------
//	Establece los datos (time y maquina) para un trabajo determinado.
//----------------------------------------------------------------------------------
void CGraphBase::SetNodeValue(int index, unsigned id, const CMachineGroupLite *machines)
{
	if (index >= 0 && index < (iJobs * iMachines + iRecirc))
	{
		TD[index].ID = id;
		TD[index].MachineGroup = machines;

		schedule.SetIdByIndex(index, id);
	}
}

//----------------------------------------------------------------------------
//	Establece el tipo de problema que representa el grafo.
//----------------------------------------------------------------------------
void CGraphBase::SetProblemType(enum ProblemType type)
{
	ProbType = type;
}

//----------------------------------------------------------------------------
//	Establece el manipulador de la interfaz donde sera vizualizado el diagrama
//	de Gantt.
//----------------------------------------------------------------------------
void CGraphBase::SetGantChartView(TObserverGantt *handler)
{
	hndGantt = handler;
}

//----------------------------------------------------------------------------
//	Establece el manipulador de la interfaz donde sera vizualizado el grafico
//	de Energia.
//----------------------------------------------------------------------------
void CGraphBase::SetEnergyChartView(TObserverEnergy *handler)
{
	hndEnergy = handler;
}

//----------------------------------------------------------------------------
//	Devuelve la cantidad de piezas que contiene el grafo.
//----------------------------------------------------------------------------
int CGraphBase::GetPieceCount(void) const
{
	return iJobs;
}

//----------------------------------------------------------------------------
//	Devuelve la cantidad de maquinas que contiene el grafo.
//----------------------------------------------------------------------------
int CGraphBase::GetMachineCount(void) const
{
	return iMachines;
}

//----------------------------------------------------------------------------
//	Devuelve la secuencia que representa al grafo.
//----------------------------------------------------------------------------
const unsigned *CGraphBase::GetSequence(void) const
{
	return Sequence;
}

//----------------------------------------------------------------------------
//	Devuelve el tipo de mejora para los objetivos
//----------------------------------------------------------------------------
enum ImproveType CGraphBase::GetImproveType(void) const
{
	return ImpType;
}

//----------------------------------------------------------------------------
//	Devuelve el tipo de problema que representa el grafo.
//----------------------------------------------------------------------------
enum ProblemType CGraphBase::GetProblemType(void) const
{
	return ProbType;
}

//----------------------------------------------------------------------------
//	Devuelve el schedule.
//----------------------------------------------------------------------------
const CSchedule &CGraphBase::GetSchedule(void) const
{
	return schedule;
}

//----------------------------------------------------------------------------
//	Devuelve el tipo de schedule que representa el grafo.
//----------------------------------------------------------------------------
enum ScheduleType CGraphBase::SchedulePlannig(void)
{
	unsigned id = 0;
	int space, indexOpC, machine, semi_active = 0;
	float TDPiActual, t_startBP, TPiAnt_Ant, TOPAnterior;

	// Si todavia no se ha recorrido el grafo, lo recorro ahora
	if (!isGraphTraveled) TravelGraph();

	for (indexOpC = 1; indexOpC < (iJobs * iMachines + iRecirc); indexOpC++)
	{
		id = TD[indexOpC].ID;
		machine = TR[indexOpC].Machine;
		space = SearchSpace(indexOpC, id, machine, indexOpB);

		if (space)
		{
			for (int i = 0; i < space; i++)
			{
				if (ExistsHole(indexOpB[i], machine))
				{
					TDPiActual = TR[indexOpC].DTime;
					TOPAnterior = getTimeBackOp(indexOpC, TD[indexOpC].ID);
					t_startBP = TR[indexOpB[i]].RTime - TR[indexOpB[i]].DTime;
					TPiAnt_Ant = getTimeBackJob(indexOpB[i], machine);

					// ¿Cabe la pieza actual en el hueco encontrado sin que retrase 
					// el comienzo de alguna otra pieza?
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
void CGraphBase::CreateGanttChart(void)
{
	int j, op;

	// Si todavia no se ha recorrido el grafo, se corre ahora
	if (!isGraphTraveled) TravelGraph();

	hndGantt->PrintCmaxValue(Makespan());
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

	/*hndGantt->SetMeasureMark(iMeasureCount);  // Establece y prepara las marcas de las lecturas
																						// del consumo de energia
	float step = Cmax / iMeasureCount;
	for (int i = 0; i < (int)iMeasureCount; i++)
		hndGantt->GraphMeasureMark(i, step * (i + 1));*/

	delete[]seq_op_num;
	delete[]op_job;
}

