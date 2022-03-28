#include "CPieceCollection_PM.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <conio.h>

///==================================================================================///
///==================================================================================///
///																																									 ///
///										Class CPieceTechnologicalRoute_PM Definition									 ///	
///																																									 ///
///==================================================================================///
///==================================================================================///

//----------------------------------------------------------------------------
//	Funcion de Utileria. Determina si la etapa (stage) pasada por parametro
//	representa una operacion nula, si esto ocurre devuelve TRUE, o FALSE en 
//	caso contrario.
//  Nota: <Operacion Nula>: si todas las maquinas de la etapa tienen tiempo de 
//				procesamiento igual 0. O sea, que esta operacion no se procesa en 
//				ninguna de las maquinas declaradas.
//----------------------------------------------------------------------------
bool CPieceTechnologicalRoute_PM::isNullOperation (const CMachineGroupExtended *mgroup)
{
	int op_total, op_null;
		
	op_null  = 0;
	op_total = mgroup->GetMachineCount();
	
	for (int i = 0; i < op_total; i++)
		if (mgroup->GetMachineByIndex(i).Time == 0) op_null++;

	return (op_null == op_total) ? 1 : 0;
}

//----------------------------------------------------------------------------
//	Constructor de la clase.
//----------------------------------------------------------------------------
CPieceTechnologicalRoute_PM::CPieceTechnologicalRoute_PM (void)
{
	*PieceName					= '\0';
	Id									= 0;								
	Priority						= 0.0;
	DueDate							= 0.0;
	Penality						= 0.0;
	Reward					    = 0.0;
	OperationsCount     = 0;
	NullOperationsCount = 0;
	StagesRoute					= NULL;
	LastStage           = NULL;
	clear								= new CMachineGroupExtended();
}

//----------------------------------------------------------------------------
//	Constructor de Copia.	
//----------------------------------------------------------------------------
CPieceTechnologicalRoute_PM::CPieceTechnologicalRoute_PM (const CPieceTechnologicalRoute_PM &Obj)
{
	int count;

	strcpy(this->PieceName, Obj.GetPieceName());
	this->Id									= Obj.GetPieceID();
	this->Priority						= Obj.GetPiecePriority();
	this->DueDate							= Obj.GetDueDate();
	this->Penality						= Obj.GetPenality();
	this->Reward							= Obj.GetReward();
  this->StagesRoute	        = NULL;
	this->LastStage           = NULL;
 	this->OperationsCount     = 0;
	this->NullOperationsCount = 0;
	this->clear								= new CMachineGroupExtended();

	count = Obj.GetOperationCount();
	this->DeleteRoute();	

	for (int i = 0; i < count; i++)
		this->AddMachineGroup(*Obj.GetMachinesGroupByIndex(i));	
}

//----------------------------------------------------------------------------
//	Destructor de la clase.
//----------------------------------------------------------------------------
CPieceTechnologicalRoute_PM::~CPieceTechnologicalRoute_PM (void)
{
	delete clear;

	DeleteRoute();		// Libero la memoria usada en la lista
}

//----------------------------------------------------------------------------
//	Agrega una etapa a la ruta tecnologica de la pieza actual.
//----------------------------------------------------------------------------
int CPieceTechnologicalRoute_PM::AddMachineGroup (const CMachineGroupExtended &Obj)
{
	STAGES_LIST_PTR newNode;

	// Creo el nuevo nodo
	newNode = new STAGES_LIST;

	if (newNode)
	{
		newNode->Stage = new CMachineGroupExtended(Obj);
		newNode->Next  = NULL;
	}
	else return 0;

	// Inserto el nodo siempre al final de la lista
	if (StagesRoute == NULL)	// Lista vacia
	{
		StagesRoute = newNode;
		LastStage   = StagesRoute;
	}
	else
	{
		LastStage->Next = newNode;
		LastStage       = newNode;
	}
	OperationsCount++;

	if (isNullOperation(newNode->Stage)) NullOperationsCount++;
	
	return 1;
}

//----------------------------------------------------------------------------
//	Actualiza una etapa especificada por el parametro <index>.
//----------------------------------------------------------------------------
void CPieceTechnologicalRoute_PM::UpdMachineGroup (unsigned index, const CMachineGroupExtended &Obj)
{	
	
	/*OPERATION_LIST_PTR ptrCurrent;
	
	ptrCurrent = Route;

	// Busco la operacion a actualizar
	while(ptrCurrent != NULL && strcmp(machine_name, ptrCurrent->MachineName) != 0)
		ptrCurrent = ptrCurrent->Next;

	// Actualizo el valor del tiempo
	if (ptrCurrent)
	{
		ptrCurrent->Time = new_time;
	}*/
}

//----------------------------------------------------------------------------
//	Limpia la ruta tecnologica de la pieza, liberando la memoria usada por 
//	la lista.
//----------------------------------------------------------------------------
void CPieceTechnologicalRoute_PM::DeleteRoute (void)
{
	STAGES_LIST_PTR deleteNode2;

	//Vacio la lista
	deleteNode2 = StagesRoute;

	while (deleteNode2)
	{
		StagesRoute = StagesRoute->Next;
		delete deleteNode2;
		deleteNode2 = StagesRoute;		
	}

	OperationsCount     = 0;
	NullOperationsCount = 0;
	StagesRoute = NULL;
	LastStage   = NULL;
}

//----------------------------------------------------------------------------
//	Limpia todos los datos del objeto.
//----------------------------------------------------------------------------
void CPieceTechnologicalRoute_PM::ClearPiece (void)
{
	*PieceName = '\0';
	Id         = 0;								
	Priority   = 0.0;
	DueDate		 = 0.0;
	Penality   = 0.0; 
	Reward     = 0.0;

	DeleteRoute();
}

//----------------------------------------------------------------------------
//	Establece el nombre de la pieza.
//----------------------------------------------------------------------------
void CPieceTechnologicalRoute_PM::SetPieceName (const char *name)
{
	strcpy(PieceName, name);
}

//----------------------------------------------------------------------------
//	Establece el Id de la pieza.
//----------------------------------------------------------------------------
void CPieceTechnologicalRoute_PM::SetPieceID (unsigned id)
{
	Id = id;								
}

//----------------------------------------------------------------------------
//	Establece la prioridad de la pieza.
//----------------------------------------------------------------------------
void CPieceTechnologicalRoute_PM::SetPiecePriority (float p)
{
	Priority = p;
}

//----------------------------------------------------------------------------
//	Establece el valor de la fecha de compromiso de la pieza.
//----------------------------------------------------------------------------
void CPieceTechnologicalRoute_PM::SetDueDate (float di)
{
	DueDate = di;
}

//----------------------------------------------------------------------------
//	Establece el valor de penalizacion por retrazo en la fecha de compromiso.
//----------------------------------------------------------------------------
void CPieceTechnologicalRoute_PM::SetPenality (float pe)
{
	Penality = pe;
}

//----------------------------------------------------------------------------
//	Establece el valor de recompensa por adelanto a la fecha de compromiso.
//----------------------------------------------------------------------------
void CPieceTechnologicalRoute_PM::SetReward (float re)
{
	Reward = re;
}

//----------------------------------------------------------------------------
//	Devuelve el nombre de la pieza.
//----------------------------------------------------------------------------
const char *CPieceTechnologicalRoute_PM::GetPieceName (void) const
{
	return PieceName;
}

//----------------------------------------------------------------------------
//	Devuelve el Id de la pieza.
//----------------------------------------------------------------------------
unsigned CPieceTechnologicalRoute_PM::GetPieceID (void) const
{
	return Id;
}

//----------------------------------------------------------------------------
//	Devuelve la prioridad de la pieza.
//----------------------------------------------------------------------------
float CPieceTechnologicalRoute_PM::GetPiecePriority (void) const
{
	return Priority;
}

//----------------------------------------------------------------------------
//	Devuelve el valor de due date de trabajo.
//----------------------------------------------------------------------------
float	CPieceTechnologicalRoute_PM::GetDueDate (void) const
{
	return DueDate;
}

//----------------------------------------------------------------------------
//	Devuelve el indice en que es penalizado en trabajo en caso de vencer su
//	fecha de entrega (DueDate).
//----------------------------------------------------------------------------
float	CPieceTechnologicalRoute_PM::GetPenality (void) const
{
	return Penality;	
}

//----------------------------------------------------------------------------
//	Devuelve el indice en que es premiado en trabajo en caso de adelantarse a
//	su fecha de entrega (DueDate).
//----------------------------------------------------------------------------
float	CPieceTechnologicalRoute_PM::GetReward (void) const
{
	return Reward;
}

//----------------------------------------------------------------------------
//	Devuelve el tiempo teorico de procesamiento de la ruta tecnologica.
//----------------------------------------------------------------------------
float	CPieceTechnologicalRoute_PM::GetTechnologicalTime (void) const
{
	float cii = 0;
	/*OPERATION_LIST_PTR ptrCurrent;
	
	ptrCurrent = Route;

	while (ptrCurrent != NULL)
	{
		cii += ptrCurrent->Time;

		ptrCurrent = ptrCurrent->Next;
	}*/

	return cii;
}

//----------------------------------------------------------------------------
//	Devuelve la cantidad de operaciones de la pieza.
//----------------------------------------------------------------------------
int CPieceTechnologicalRoute_PM::GetOperationCount (void) const
{
	return OperationsCount;
}

//----------------------------------------------------------------------------
//	Devuelve la cantidad de operaciones nulas de la pieza.
//----------------------------------------------------------------------------
int CPieceTechnologicalRoute_PM::GetNullOperationCount (void) const
{
	return NullOperationsCount;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
CMachineGroupExtended *CPieceTechnologicalRoute_PM::GetMachinesGroupByIndex (unsigned index) const
{
	STAGES_LIST_PTR ptrCurrent;

	// Busco la posicion de la etapa
	ptrCurrent = StagesRoute;
	for (unsigned i = 0; i < index && ptrCurrent != NULL; i++)
		ptrCurrent = ptrCurrent->Next;

	if (ptrCurrent) return ptrCurrent->Stage;

	return clear;
}

//----------------------------------------------------------------------------
//	Devuelve '1' si la pieza presenta en su ruta tecnologica una maquina igual
//	a la maquina pasada por parametro, '0'.	Devuelve por referencia la posicion
//	que ocupa esta maquina en la ruta tecnologica.
//----------------------------------------------------------------------------
bool CPieceTechnologicalRoute_PM::PresentMachine	(const char *machine_name, int *pos) const
{
	/*int op = GetOperationCount();

	for (int i = 0; i < op; i++)
	{
		if (strcmp(GetOperationByIndex(i).MachineName, machine_name) == 0)
		{
			if (pos) *pos = i;
			return 1;
		}
	}

	*pos = -1;*/
	return 0;
}

//----------------------------------------------------------------------------
//	Determina si el valor de duedate establecido es valido.
//
//	Nota: Valor de duedate valido: tiene que ser mayor o igual que el tiempo
//        teorico de fabricacion. Debido a que no se puede terminar una 
//				trabajo mas rapido de lo que establece su ruta tecnologico, pues 
//				se incurriria facilmente en una violacion tecnologica.
//----------------------------------------------------------------------------
bool CPieceTechnologicalRoute_PM::IsValidDueDateValue (void)
{
	return (DueDate != 0.0 && DueDate < GetTechnologicalTime()) ? false : true;
}

//----------------------------------------------------------------------------
//	Sobrecarga del operador de asignacion.	
//----------------------------------------------------------------------------
CPieceTechnologicalRoute_PM &CPieceTechnologicalRoute_PM::operator = (const CPieceTechnologicalRoute_PM &Obj)
{
	int count;

	strcpy(this->PieceName, Obj.GetPieceName());
	this->Id       = Obj.GetPieceID();
	this->Priority = Obj.GetPiecePriority();
	this->DueDate  = Obj.GetDueDate();
	this->Penality = Obj.GetPenality();
	this->Reward   = Obj.GetReward();

	count = Obj.GetOperationCount();
	this->DeleteRoute();	

	for (int i = 0; i < count; i++)
		this->AddMachineGroup(*Obj.GetMachinesGroupByIndex(i));
	
	return (*this);	
}

//----------------------------------------------------------------------------
// Sobrecarga del operador aritmetico de igualdad.
//----------------------------------------------------------------------------
bool CPieceTechnologicalRoute_PM::operator == (const CPieceTechnologicalRoute_PM &Obj)
{
	CMachineGroupExtended *MG1, *MG2;

	// Paso #1: Comparo los datos generales		  
	if (strcmp(this->PieceName, Obj.GetPieceName()) != 0)	return false;
	if (this->Id							!= Obj.GetPieceID())				return false;
	if (this->Priority				!= Obj.GetPiecePriority())	return false;
	if (this->DueDate					!= Obj.GetDueDate())				return false;
	if (this->Penality				!= Obj.GetPenality())				return false;
	if (this->Reward          != Obj.GetReward())					return false;
	if (this->OperationsCount != Obj.GetOperationCount()) return false;

	// Paso #2: Comparo los grupos de maquinas
	for (int i = 0; i < this->OperationsCount; i++)
	{
		MG1 = this->GetMachinesGroupByIndex(i); 
		MG2 = Obj.GetMachinesGroupByIndex(i);

		if (!((*MG1) == (*MG2))) return false;
	}

	// Si todo es exactamente igual, pues entonces devuelve TRUE.
	return true;
}

//----------------------------------------------------------------------------
//	Imprime la informacion referente a la pieza y su ruta tecnologica.
//----------------------------------------------------------------------------
void CPieceTechnologicalRoute_PM::PrintTechRouteInformation (void)
{
	int op;
	STRUCT_DATA_MACHINE DM;
	STAGES_LIST_PTR ptrCurrent;
	
	printf("Piece Name: %s\n", PieceName);
	printf("Piece Id: %d\n", Id);
	printf("Priority: %.2f\n", Priority);
	printf("DueDate: %.2f\n", DueDate);
	printf("Penality: %.2f\n", Penality);
	printf("Reward: %.2f\n", Reward);

	printf("Operations: %d\n\n", OperationsCount);

	ptrCurrent = StagesRoute;
	printf("TECHNOLOGICAL ROUTE\n");
	while (ptrCurrent != NULL)
	{
		op = ptrCurrent->Stage->GetMachineCount();
		printf(" %d --> ", op);
		for (int i = 0; i < op; i++)
		{
			DM = ptrCurrent->Stage->GetMachineByIndex(i);
			printf("%s: %.1f%s", DM.MachineName, DM.Time, (i < op-1) ? " | " : " ");		
		}
		
		printf("\n");
		ptrCurrent = ptrCurrent->Next;
	}
}


///==================================================================================///
///==================================================================================///
///																																									 ///
///										Class CPieceCollection_PM Definition													 ///	
///																																									 ///
///==================================================================================///
///==================================================================================///

/* Libera la memoria asignada a la lista de prioridades */
void FreePriorityList (PRIORITY_LIST_PTR *L)
{
	PRIORITY_LIST_PTR ptrCurrent, ptrDel;

	ptrCurrent = ptrDel = *L;
	while (ptrCurrent != NULL)
	{
		ptrDel = ptrCurrent;
		ptrCurrent = ptrCurrent->Next;
		delete ptrDel;
	}
	*L = NULL;
}

//----------------------------------------------------------------------------
//	Funcion de Utileria. Devuelve '1' si las piezas pasadas por parametro tienen
//	un flujo tecnologico unidireccional.
//	Flujo tecnologico unidireccional: es si ambas piezas presentan los mismos 
//																		patrones de visita en las maquinas o los 
//																		mismos ordenes de llegada.	
//----------------------------------------------------------------------------
bool CPieceCollection_PM::IsOneWayFlow (const CPieceTechnologicalRoute_PM &P1, const CPieceTechnologicalRoute_PM &P2)
{
	/*int op1, pos, aux;

	op1 = P1.GetOperationCount();

	for (int i = 2; i < op1; i++)
	{
		// ¿Tiene presencia la operacion actual de P1 en P2?
		if (P2.PresentMachine(P1.GetOperationByIndex(i).MachineName, &pos)) 
		{
			for (int j = i - 1; j > 0; j--)
			{
				P2.PresentMachine(P1.GetOperationByIndex(j).MachineName, &aux);

				if (aux > pos) return 0;
			}			
		}
	}*/

	return 1;
}

//----------------------------------------------------------------------------
//	Funcion de Utileria.
//----------------------------------------------------------------------------
void CPieceCollection_PM::CleanNullOperations (CPieceTechnologicalRoute_PM **cleanArray)
{
	int op_count;
	STRUCT_DATA_MACHINE DM;
	CMachineGroupExtended CleanGroup, *CurrentGroup;

	ArrayPieceAlreadyClean = true;

	if (*cleanArray) 
	{
		delete [](*cleanArray);
		*cleanArray = new CPieceTechnologicalRoute_PM[iPieceCount];
	}
	else
	{
		*cleanArray = new CPieceTechnologicalRoute_PM[iPieceCount];
	}

	for (int i = 0; i < iPieceCount; i++)
	{
		(*cleanArray)[i].SetPieceName(ArrayPiece[i].GetPieceName());
		(*cleanArray)[i].SetPieceID(ArrayPiece[i].GetPieceID());
		(*cleanArray)[i].SetPiecePriority(ArrayPiece[i].GetPiecePriority());
		(*cleanArray)[i].SetDueDate(ArrayPiece[i].GetDueDate());
		(*cleanArray)[i].SetPenality(ArrayPiece[i].GetPenality());
		(*cleanArray)[i].SetReward(ArrayPiece[i].GetReward());

		// Se copian las rutas tecnologicas de cada pieza sin las operaciones nulas
		op_count = ArrayPiece[i].GetOperationCount();
		for (int j = 0; j < op_count; j++)
		{
			CurrentGroup = ArrayPiece[i].GetMachinesGroupByIndex(j);
			
			for (int k = 0; k < CurrentGroup->GetMachineCount(); k++)
			{
				DM = CurrentGroup->GetMachineByIndex(k);
				if (DM.Time != 0) CleanGroup.AddMachine(DM.MachineName, DM.Time);
			}
			
			if (CleanGroup.GetMachineCount()) (*cleanArray)[i].AddMachineGroup(CleanGroup);
			
			CleanGroup.ClearGroup();
		}
	}
}

//----------------------------------------------------------------------------
//	Constructor de la clase.	
//----------------------------------------------------------------------------
CPieceCollection_PM::CPieceCollection_PM (int pieces, int machines, bool fifo_ruler)
{
	// Protejo la ejecucion del programa, frente a un valor menor o igual a cero.
	assert(pieces > 0);
	
	Index									  = 0;
	iPieceCount					    = pieces;
	iMachinesCount          = machines;
	problemTypeDetermined   = false;
	ArrayPieceAlreadyClean  = false;
	RecircAlreadyCalculated = false;
	FIFO_Ruler              = fifo_ruler;
	ProbType                = NonProblem;
	cleanArrayPiece				  = NULL;
	ArrayPiece						  = new CPieceTechnologicalRoute_PM[iPieceCount];
}

//----------------------------------------------------------------------------
//	Constructor de Copia.
//----------------------------------------------------------------------------
CPieceCollection_PM::CPieceCollection_PM (const CPieceCollection_PM &Obj)
{	
	this->Index             = 0;
	this->iPieceCount       = Obj.GetPieceCount();
	problemTypeDetermined   = false;
	ArrayPieceAlreadyClean  = false;
	RecircAlreadyCalculated = false;
	ProbType                = NonProblem;
	FIFO_Ruler              = Obj.ApplyFifoRuler();
	cleanArrayPiece				  = NULL;
	this->ArrayPiece        = new CPieceTechnologicalRoute_PM[iPieceCount];	

	for (int i = 0; i < iPieceCount; i++) this->ArrayPiece[i] = Obj.GetPieceByIndex(i);
}

//----------------------------------------------------------------------------
//	Destructor de la clase.	
//----------------------------------------------------------------------------
CPieceCollection_PM::~CPieceCollection_PM ()
{
	delete []ArrayPiece;
	delete []cleanArrayPiece;
}	
		
//----------------------------------------------------------------------------
//	Agrega una pieza a la coleccion. Devuelve 1 si la insercion resulto satisfactoria
//	0 en caso contrario.
//----------------------------------------------------------------------------
int CPieceCollection_PM::AddPiece (const CPieceTechnologicalRoute_PM &Obj)
{
	if (Index < iPieceCount)
	{
		ArrayPiece[Index]       = Obj;
		ProbType                = NonProblem;
		problemTypeDetermined   = false;
		ArrayPieceAlreadyClean  = false;
		RecircAlreadyCalculated = false;
		Index++;

		return 1;
	}

	return 0;
}

//----------------------------------------------------------------------------
//	Actualiza la pieza determinada por el argumento 'index' en la coleccion.	
//----------------------------------------------------------------------------
void CPieceCollection_PM::UpdatePiece (int index, const CPieceTechnologicalRoute_PM &Obj)
{
	if (index > 0 && index < iPieceCount) 
	{	
		ArrayPiece[index]       = Obj;
		ProbType                = NonProblem;
		problemTypeDetermined   = false;
		ArrayPieceAlreadyClean  = false;
		RecircAlreadyCalculated = false;
	}
}

//----------------------------------------------------------------------------
//	Establece si se debe aplicar o no la regla FIFO al determinar el tipo de
//  problema que representan los datos guardados.	
//----------------------------------------------------------------------------
void CPieceCollection_PM::ApplyFifoRuler (bool fifo_ruler)
{
	FIFO_Ruler            = fifo_ruler;
	problemTypeDetermined = false;
  ProbType              = NonProblem;
}

//----------------------------------------------------------------------------
//	Devuelve (TRUE) si se aplica regla FIFO al determinar el tipo de
//  problema,que representan los datos guardados, (FALSE) en caso contrario.	
//----------------------------------------------------------------------------
bool CPieceCollection_PM::ApplyFifoRuler (void) const
{
	return FIFO_Ruler;
}

//----------------------------------------------------------------------------
//	Devuelve la cantidad de piezas que contiene la coleccion.	
//----------------------------------------------------------------------------
int CPieceCollection_PM::GetPieceCount (void) const
{
	return iPieceCount;
}

//----------------------------------------------------------------------------
//	Devuelve por referencia un array con todos los Ids de las piezas de la 
//	coleccion.
//----------------------------------------------------------------------------
void CPieceCollection_PM::GetAllID (unsigned *id_array) const
{
	for (int i = 0; i < Index; i++) id_array[i] = ArrayPiece[i].GetPieceID();
}

//----------------------------------------------------------------------------
//  Devuelve la cantidad de operaciones que recircula.
void CPieceCollection_PM::GetRecirculationData (STRUCT_RECIRCULATION *sRecirc)
{
	STRUCT_PIECE_RECIRC *tmpRecirc;
	int i, t_recirc = 0, p_recirc = 0, op_count, result;
		
	tmpRecirc = new STRUCT_PIECE_RECIRC[iPieceCount];

	CleanNullOperations(&cleanArrayPiece);

	// Paso #1: realizo calculos preliminares sobre la recirculacion. 
	for (i = 0; i < iPieceCount; i++)
	{
		op_count = cleanArrayPiece[i].GetOperationCount();
		result   = op_count - iMachinesCount;

		tmpRecirc[i].PieceID = cleanArrayPiece[i].GetPieceID();
		tmpRecirc[i].nRecirc = (result > 0) ? result : 0;

		p_recirc += (result > 0) ? 1 : 0;
		t_recirc += (result > 0) ? result : 0;
	}

	// Paso #2: Una vez obtenidos los valores necesarios, los saco por referencia.
	RecircAlreadyCalculated = true;

	if (sRecirc != NULL)
	{
		int j = 0;
		
		if (sRecirc->P_Recirc)   delete []sRecirc->P_Recirc;
		if (RecircData.P_Recirc) delete []RecircData.P_Recirc;

		sRecirc->P_Recirc   = new STRUCT_PIECE_RECIRC [p_recirc];
		RecircData.P_Recirc = new STRUCT_PIECE_RECIRC [p_recirc];

		sRecirc->pRecirc   = p_recirc;
		sRecirc->tRecirc   = t_recirc;
		RecircData.pRecirc = p_recirc;
		RecircData.tRecirc = t_recirc;

		for (i = 0; i < iPieceCount; i++)
		{
			if (tmpRecirc[i].nRecirc)
			{
				sRecirc->P_Recirc[j].PieceID   = tmpRecirc[i].PieceID;
				sRecirc->P_Recirc[j].nRecirc   = tmpRecirc[i].nRecirc;
				RecircData.P_Recirc[j].PieceID = tmpRecirc[i].PieceID;
				RecircData.P_Recirc[j].nRecirc = tmpRecirc[i].nRecirc;

				j++;
			}
		}
	}
	else
	{
		int j = 0;
		
		if (RecircData.P_Recirc) delete []RecircData.P_Recirc;

		RecircData.P_Recirc = new STRUCT_PIECE_RECIRC [p_recirc];

		RecircData.pRecirc = p_recirc;
		RecircData.tRecirc = t_recirc;

		for (i = 0; i < iPieceCount; i++)
		{
			if (tmpRecirc[i].nRecirc)
			{
				RecircData.P_Recirc[j].PieceID = tmpRecirc[i].PieceID;
				RecircData.P_Recirc[j].nRecirc = tmpRecirc[i].nRecirc;

				j++;
			}
		}
	}
	
	delete []tmpRecirc;
}

//----------------------------------------------------------------------------
//	Devuelve una lista simplemente enlazada con los valores de prioridad ordenados
//	descendentemente (mayor a menor).
//----------------------------------------------------------------------------
void CPieceCollection_PM::GetPriorityList (PRIORITY_LIST_PTR *List)
{
	PRIORITY_LIST_PTR NewNode, tmpList = NULL, CurrentNode, BackNode;

	for (int i = 0; i < iPieceCount; i++)
	{
		if (ArrayPiece[i].GetPiecePriority() != 0.0)
		{
			NewNode = new PRIORITY_LIST;
			// Protejo la ejecucion por si no se pudo reservar de memoria 
			assert(NewNode != NULL);	
			
			NewNode->IdPiece  = ArrayPiece[i].GetPieceID();
			NewNode->Priority = ArrayPiece[i].GetPiecePriority();
			NewNode->Next     = NULL;

			// Inserto el nuevo elemento de forma ordenada
			if (tmpList == NULL)
			{
				tmpList = NewNode;
			}
			else
			{
				CurrentNode = BackNode = tmpList;
				// Busco la posicion donde insertar el nuevo nodo
				while (CurrentNode != NULL && CurrentNode->Priority >= NewNode->Priority)
				{
					BackNode = CurrentNode;
					CurrentNode = CurrentNode->Next;
				}
				
				// Inserto el nuevo nodo en la cabeza de la lista
				if (CurrentNode == tmpList)
				{
					NewNode->Next = CurrentNode;
					tmpList = NewNode;
				}
				else	// Inserto el nuevo nodo en la cola de la lista 
				{
					NewNode->Next = CurrentNode;
					BackNode->Next = NewNode;
				}
			}
		}
	}

	*List = tmpList;
}

//----------------------------------------------------------------------------
//	Devuelve la pieza determinada por el argumento 'index'. El valor de este 
//  argumento va desde 0 hasta total_piezas - 1.	
//----------------------------------------------------------------------------		
CPieceTechnologicalRoute_PM CPieceCollection_PM::GetPieceByIndex (unsigned index) const
{	
	CPieceTechnologicalRoute_PM clear;

	if (index < (unsigned)iPieceCount) return ArrayPiece[index];
		
	return clear;
}

//----------------------------------------------------------------------------
//	Devuelve la pieza que tiene el Id igual al argumento 'id'. 
//----------------------------------------------------------------------------	
CPieceTechnologicalRoute_PM CPieceCollection_PM::GetPieceByID (unsigned id) const
{	
	CPieceTechnologicalRoute_PM clear;

	for(int i = 0; i < Index; i++)
		if (ArrayPiece[i].GetPieceID() == id) return ArrayPiece[i];
		
	return clear;
}

//----------------------------------------------------------------------------
//	Devuelve la pieza que tiene su nombre igual al pasado por parametro. 	
//----------------------------------------------------------------------------
CPieceTechnologicalRoute_PM CPieceCollection_PM::GetPieceByName (const char *name) const
{	
	CPieceTechnologicalRoute_PM clear;

	for(int i = 0; i < Index; i++)
		if (strcmp(ArrayPiece[i].GetPieceName(), name) == 0) return ArrayPiece[i];
		
	return clear;
}

//----------------------------------------------------------------------------
//	Crea un grafo para la instancia PFSS usando una representacion de Trabajos.
//----------------------------------------------------------------------------
void CPieceCollection_PM::Create_N_Graph (CSPGraph_PM *Graph, const CMachineCollection_PM *MachineList, const unsigned *sequence)
{
	unsigned *new_sequence;
	int  machineCount;
	int *control_OP;
	const CMachineGroupLite *M;	
	CPieceTechnologicalRoute_PM Piece;
	
	// Determino cuantas maquinas hay declaradas
	machineCount = MachineList->GetMachineCount();

	// Se crea un arreglo de piezas que no contiene operaciones nulas.
	// Nota: se define <operaciones nulas> como aquellas operaciones que su tiempo 
	//       de procesamiento es cero.
	if (!ArrayPieceAlreadyClean) CleanNullOperations(&cleanArrayPiece);

	// Verifico que ya se haya calculado la informacion referente a la recirculacion de
	// las piezas en el sistema. En caso de que no se halla calculado, lo calculo ahora
	if (!RecircAlreadyCalculated) GetRecirculationData(NULL);

	// Se convierte la secuencia de la foma {N} a {N x N}. Esto permite usar el mismo
	// grafo que se usa para instancias JobShop usarlo para instancias Permutational-FlowShop
	new_sequence = new unsigned [iPieceCount * machineCount];
	for (int m = 0; m < machineCount; m++)
		for (int p = 0; p < iPieceCount; p++)
			new_sequence[m * iPieceCount + p] = sequence[p];

	// Termino de convertir la secuencia {N} a {N x M}, lo que ahora agregandole la parte
	// de la RECIRCULACION, si la tiene!!!. Por lo tanto se convierte de la forma 
	// {N + k} a {N x M + k}
	if (RecircData.pRecirc != 0 && RecircData.P_Recirc != NULL)
	{
		int j_n = iPieceCount;
		int j_nxm = iPieceCount * machineCount;
		int end = j_nxm + RecircData.tRecirc;

		for (; j_nxm < end; j_nxm++, j_n++)
			new_sequence[j_nxm] = sequence[j_n];
	}

	// Le establezco al grafo la secuencia que lo representa
	Graph->SetSequence(new_sequence);

	// Inicializo la estructura de control de la operaciones
	control_OP = new int[iPieceCount];
	for (int i = 0; i < iPieceCount; i++)
	{
		control_OP[i] = 0;

		// Se establecen los valores de DueDate de los trabajos
		Graph->SetDueDate(cleanArrayPiece[i].GetPieceID(), cleanArrayPiece[i].GetDueDate());
	}

	// Establesco el tipo de problema que representa el grafo
	Graph->SetProblemType(PFSS);

	// Establezco la informacion de las maquinas
	Graph->SetMachineInformation(MachineList);

	// Construyo el grafo segun la secuencia pasada por parametro
	for (int j = 0; j < iPieceCount * machineCount; j++)
	{
		for (int k = 0; k < iPieceCount; k++)
		{			
			if (new_sequence[j] == cleanArrayPiece[k].GetPieceID())
			{			
				int op = control_OP[k];	

				M = cleanArrayPiece[k].GetMachinesGroupByIndex(op)->GetGroupLite(*MachineList);
				Graph->SetNodeValue(j, new_sequence[j], M);
				control_OP[k]++;
				break;
			}
		}
	}		

	// Proceso la informacion referente a la reciculacion de piezas en el sistema. SI LA HAY!!!
	if (RecircData.pRecirc != 0 && RecircData.P_Recirc != NULL)
	{
		int init = iPieceCount * machineCount;
		int end = init + RecircData.tRecirc;

		for (int r = init; r < end; r++)
		{
			for (int p = 0; p < iPieceCount; p++)
			{			
				if (new_sequence[r] == cleanArrayPiece[p].GetPieceID())
				{			
					int op = control_OP[p];	

					M = cleanArrayPiece[p].GetMachinesGroupByIndex(op)->GetGroupLite(*MachineList);
					Graph->SetNodeValue(r, new_sequence[r], M);
					control_OP[p]++;
					break;
				}
			}
		}	
	}

	delete []control_OP;	
	delete []new_sequence;
}

//----------------------------------------------------------------------------
//	Crea un grafo para instancias FSS, GFSS y JSS usando una representacion de 
//  operaciones con repeticion.
//----------------------------------------------------------------------------
void CPieceCollection_PM::Create_NxM_Graph (CSPGraph_PM *Graph, const CMachineCollection_PM *MachineList, const unsigned *sequence)
{
	int machineCount;
	int *control_OP;
	const CMachineGroupLite *M;
	CPieceTechnologicalRoute_PM Piece;

	// Determino cuantas maquinas hay declaradas
	machineCount = MachineList->GetMachineCount();

	// Se crea un arreglo de piezas que no contiene operaciones nulas.
	// Nota: se define <operaciones nulas> como aquellas operaciones que su tiempo 
	//       de procesamiento es cero.
	if (!ArrayPieceAlreadyClean) CleanNullOperations(&cleanArrayPiece);

	// Verifico que ya se haya calculado la informacion referente a la recirculacion de
	// las piezas en el sistema. En caso de que no se halla calculado, lo calculo ahora
	if (!RecircAlreadyCalculated) GetRecirculationData(NULL);

	// Le establezco al grafo la secuencia que lo representa
	Graph->SetSequence(sequence);

	// Inicializo la estructura de control de la operaciones
	control_OP = new int[iPieceCount];
	for (int i = 0; i < iPieceCount; i++)
	{
		control_OP[i] = 0;

		// Se establecen los valores de DueDate de los trabajos
		Graph->SetDueDate(cleanArrayPiece[i].GetPieceID(), cleanArrayPiece[i].GetDueDate());
	}

	// Establesco el tipo de problema que representa el grafo
	Graph->SetProblemType(ProblemType());

	// Establezco la informacion de las maquinas
	Graph->SetMachineInformation(MachineList);

	// Construyo el grafo segun la secuencia pasada por parametro
	for (int j = 0; j < iPieceCount * machineCount; j++)
	{
		for (int k = 0; k < iPieceCount; k++)
		{			
			if (sequence[j] == cleanArrayPiece[k].GetPieceID())
			{
				int op = control_OP[k];
				
				M = cleanArrayPiece[k].GetMachinesGroupByIndex(op)->GetGroupLite(*MachineList);
				Graph->SetNodeValue(j, sequence[j], M);
				control_OP[k]++;
				break;
			}
		}
	}

	// Trato la parte de la sequencia que representa la RECIRCULACION. SI LA HAY!!!
	if (RecircData.pRecirc != 0 && RecircData.P_Recirc != NULL)
	{
		int init = iPieceCount * machineCount;
		int end = init + RecircData.tRecirc;

		for (int r = init; r < end; r++)
		{
			for (int p = 0; p < iPieceCount; p++)
			{			
				if (sequence[r] == cleanArrayPiece[p].GetPieceID())
				{
					int op = control_OP[p];
				
					M = cleanArrayPiece[p].GetMachinesGroupByIndex(op)->GetGroupLite(*MachineList);
					Graph->SetNodeValue(r, sequence[r], M);
					control_OP[p]++;
					break;
				}
			}
		}
	}

	delete []control_OP;
}

//----------------------------------------------------------------------------
//	Devuelve 'TRUE' si los valores de duedate son validos. Para que sea 
//	valido este tiene que ser mayor o igual que el valor ideal de procesamiento 
//	de cada trabajo en el sistema. Devuelve 'falso' en caso contrario.
//----------------------------------------------------------------------------
bool CPieceCollection_PM::CheckDueDateValues (int *id_wrong_piece)
{
	int id;	
	bool duedate_ok;

	for (int i = 0; i < iPieceCount; i++)
	{
		id = ArrayPiece[i].GetPieceID();
		duedate_ok = ArrayPiece[i].IsValidDueDateValue();

		if (id_wrong_piece != NULL) *id_wrong_piece = id;
		if (!duedate_ok) return 0;
	}

	return 1;
}

//----------------------------------------------------------------------------
//	Devuelve el tipo de problema segun la instancia (FSSP, JSSP)	
//----------------------------------------------------------------------------
enum ProblemType CPieceCollection_PM::ProblemType (void)
{
	int op;
	bool parallel_machine = false;
	CMachineGroupExtended *MGroup, *initMGroup;
	CPieceTechnologicalRoute_PM first, other;

	// Si el tipo de problema fue determinado previamente, se devuelve el valor calculado
	// sin necesidad de re-calcularlo. Esto se hace para ganar en eficiencia.
	if (problemTypeDetermined) return ProbType;

	for (int i = 0; i < iPieceCount; i++)
	{
		op = ArrayPiece[i].GetOperationCount();
		for (int j = 0; j < op; j++)
		{
			MGroup = ArrayPiece[i].GetMachinesGroupByIndex(j);
			if (MGroup->GetMachineCount() > 1)
			{
				parallel_machine = true;
				break;
			}
		}

		if (parallel_machine) break;
	}

	// ¿Hay Maquinas en Paralelo (PM)?
	if (parallel_machine)
	{
		// Paso #1: ¿Flexible FlowShop o Flexible JobShop?
		for (int i = 0; i < iPieceCount; i++)
		{
			op = ArrayPiece[i].GetOperationCount();
			initMGroup = ArrayPiece[i].GetMachinesGroupByIndex(0);

			for (int j = 1; j < op; j++)
			{
				MGroup = ArrayPiece[i].GetMachinesGroupByIndex(j);

				if (!((*initMGroup) == (*MGroup))) return FJSS;				
			}
		}

		return FFSS;
	}
	// Si no hay Maquinas en Paralelo (MP), entonces es Maquina Unica (UM)
	else
	{
		// PASO #1: ¿Hay comienzo simultaneo?
		first = ArrayPiece[0];
		for (int p = 1; p < iPieceCount; p++)
		{
			other = ArrayPiece[p];		
			//if (strcmp(first.GetOperationByIndex(0).MachineName, other.GetOperationByIndex(0).MachineName) != 0)
			{
				ProbType = JSS;
				problemTypeDetermined = true;
			
				return ProbType;
			}
		}

		// PASO #2: Se determina si todas las piezas presentan un flujo unidireccional
		for (int i = 0; i < iPieceCount; i++)
		{
			for (int j = i; j < iPieceCount; j++)
			{
				//if (!IsOneWayFlow(ArrayPiece[i], ArrayPiece[j])) 
				{
					ProbType = JSS;
					problemTypeDetermined = true;
				
					return ProbType;
				}
			}
		}

		// PASO #3:	Se determina si existen operaciones explicitamente nulas (son aquellas que estan
		//          declaradas con tiempo cero), u operaciones implicitamente nulas (son aquellas)
		//					que no estas declaradas, implica que la cantidad de operaciones del trabajo actual
		//					es menor que la cantidad de maquinas definidas, por lo que se consideran que tienen 
		//					operaciones con tiempo cero.
		for (int p = 0; p < iPieceCount; p++)
		{
			if (ArrayPiece[p].GetOperationCount() < iMachinesCount || ArrayPiece[p].GetNullOperationCount() != 0) 
			{
				ProbType = GFSS;
				problemTypeDetermined = true;

				return ProbType;	
			}
		}

		ProbType = (FIFO_Ruler) ? PFSS : FSS;
		problemTypeDetermined = true;
	}

	return ProbType;
}

//----------------------------------------------------------------------------
//	Sobrecarga del operador de asignacion.	
//----------------------------------------------------------------------------
CPieceCollection_PM &CPieceCollection_PM::operator = (const CPieceCollection_PM &Obj)
{
	delete []ArrayPiece;

	this->Index       = 0;
	this->iPieceCount = Obj.GetPieceCount();
	this->ArrayPiece  = new CPieceTechnologicalRoute_PM[iPieceCount];  

	for (int i = 0; i < iPieceCount; i++) this->ArrayPiece[i] = Obj.GetPieceByIndex(i);

	return (*this);
}

//----------------------------------------------------------------------------
// Sobrecarga del operador aritmetico de igualdad.
//----------------------------------------------------------------------------
bool CPieceCollection_PM::operator == (const CPieceCollection_PM &Obj)
{
	CPieceTechnologicalRoute_PM P;

	// Paso #1: Comparacion de la cantidad de piezas que almacenan las colecciones.
	if (this->iPieceCount != Obj.GetPieceCount()) return false;

	// Paso #2:
	for (int i = 0; i < this->iPieceCount; i++)
	{
		P = Obj.GetPieceByID(this->ArrayPiece[i].GetPieceID());

		if (!(this->ArrayPiece[i] == P)) return false;
	}

	// Si todo es exactamente igual, pues entonces devuelve TRUE.
	return true;
}

//----------------------------------------------------------------------------
//	Limpia los datos de la coleccion de piezas.	
//----------------------------------------------------------------------------
void CPieceCollection_PM::ClearCollection (void)
{
	Index = 0;
	ProbType = NonProblem;
	problemTypeDetermined = false;

	for (int i = 0; i < iPieceCount; i++) ArrayPiece[i].ClearPiece(); 
}

//----------------------------------------------------------------------------
//	Imprime la informacion que contiene la coleccion de piezas.	
//----------------------------------------------------------------------------
void PrintPieceInformation (const CPieceCollection_PM &Obj)
{
	for (int i = 0; i < Obj.iPieceCount; i++)
	{
		Obj.ArrayPiece[i].PrintTechRouteInformation(); getch();
		printf("----------------------------------\n\n");
	}
}