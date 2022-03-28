#include "CMachineGroup_PM.h"
#include <stdlib.h>
#include <string.h>

///==================================================================================///
///==================================================================================///
///																																									 ///
///													CMachineGroupLite Class Definition								 		   ///
///																																									 ///
///==================================================================================///
///==================================================================================///

//----------------------------------------------------------------------------
//	Constructor de la clase.
//----------------------------------------------------------------------------
CMachineGroupLite::CMachineGroupLite (int n_machines)
{	
	Index = 0;
	N_Machines = n_machines;

	MachineGroup = new STRUCT_DATA_MACHINE[N_Machines];
}

//----------------------------------------------------------------------------
//	Constructor de Copia
//----------------------------------------------------------------------------
CMachineGroupLite::CMachineGroupLite (const CMachineGroupLite &Obj)
{	
	STRUCT_DATA_MACHINE item;
	
	Index = 0;
	N_Machines = Obj.GetMachineCount();

	for (int i = 0; i < N_Machines; i++)
	{
		item = Obj.GetMachineByIndex(i); 
		this->AddMachine(item.Machine, item.Time);
	}
}

//----------------------------------------------------------------------------
//	Destructor de la clase
//----------------------------------------------------------------------------
CMachineGroupLite::~CMachineGroupLite (void)
{
	delete []MachineGroup;
}

//----------------------------------------------------------------------------
//	Agrega una tupla (machine, time) a la etapa-liviana del Flexible 
//	Scheduling Problem. 
//----------------------------------------------------------------------------
int CMachineGroupLite::AddMachine (int machine, float time)
{
	if (Index >= N_Machines) return 0;
	
	MachineGroup[Index].Machine = machine;
	MachineGroup[Index].Time    = time;
	Index++;
	
	return 1;
}

//----------------------------------------------------------------------------
//	Actualiza los datos de la tupla (machine, time) de la etapa-liviana 
//	del Flexible Scheduling Problem indexado por el parametro 'index'.
//----------------------------------------------------------------------------
int CMachineGroupLite::UpdMachine (unsigned index, int machine, float time)
{
  if (index < (unsigned)N_Machines)
  {
	  MachineGroup[index].Machine = machine;
	  MachineGroup[index].Time    = time;

		return 1;
  }

	return 0;
}

//----------------------------------------------------------------------------
//	Duevuelve la cantidad de maquinas que tiene la etapa-liviana de
//	del Flexible Scheduling Problem.
//----------------------------------------------------------------------------
int	CMachineGroupLite::GetMachineCount (void) const	
{
	return N_Machines;
}

//----------------------------------------------------------------------------
//	Devuelve la tupla (machine, time) de la maquina segun si Id.
//----------------------------------------------------------------------------
STRUCT_DATA_MACHINE CMachineGroupLite::GetMachineById (int id)  const
{
	STRUCT_DATA_MACHINE tmp;

	for (int i = 0; i < N_Machines; i++)	
		if (MachineGroup[i].Machine == id) return MachineGroup[i];
	
	return tmp;
}

//----------------------------------------------------------------------------
//	Devuelve la tupla (machine, time) indicada por el parametro 'index'.
//----------------------------------------------------------------------------
STRUCT_DATA_MACHINE CMachineGroupLite::GetMachineByIndex (unsigned index) const
{
	STRUCT_DATA_MACHINE tmp;

	if (index < (unsigned)N_Machines) return MachineGroup[index];

	return tmp;
}

//----------------------------------------------------------------------------
//	Limpia el Grupo+Liviano de Maquinas.
//----------------------------------------------------------------------------
void CMachineGroupLite::ClearGroup (void)
{
	Index = 0;

	for (int a = 0, b = N_Machines - 1; a <= b; a++, b--)
	{
		//--- 1er 1/2 ---//
		MachineGroup[a].Machine        = -1;
		MachineGroup[a].MachineName[0] = '\0';
		MachineGroup[a].Time           = 0.0;

		//--- 2do 1/2 ---//
		MachineGroup[b].Machine        = -1;
		MachineGroup[b].MachineName[0] = '\0';
		MachineGroup[b].Time           = 0.0;
	}
}

//----------------------------------------------------------------------------
//	Sobrecarga del operador 'Asignacion'.
//----------------------------------------------------------------------------
CMachineGroupLite &CMachineGroupLite::operator = (const CMachineGroupLite &Obj)
{
	STRUCT_DATA_MACHINE item;
		
	this->Index = 0;
	this->N_Machines = Obj.GetMachineCount();

	for (int i = 0; i < this->N_Machines; i++)
	{
		item = Obj.GetMachineByIndex(i); 
		this->AddMachine(item.Machine, item.Time);
	}

	return (*this);
}

//----------------------------------------------------------------------------
//	Sobrecarga del operador aritmetico de igualdad.
//----------------------------------------------------------------------------
bool CMachineGroupLite::operator == (const CMachineGroupLite &Obj)
{
	STRUCT_DATA_MACHINE DM1, DM2;
	
	// Paso #1: Comparo la cantidad de máquinas en el grupo.
	if (this->N_Machines != Obj.GetMachineCount()) return false;

	// Paso #2: Comparo los datos de cada maquina.
	for (int i = 0; i < this->N_Machines; i++)
	{
		DM1 = this->GetMachineByIndex(i);
		DM2 = Obj.GetMachineByIndex(i);

		if (DM1.Machine != DM2.Machine || DM1.Time != DM2.Time) return false;
	}

	return true;
}

///==================================================================================///
///==================================================================================///
///																																									 ///
///											CMachineGroupExtended Class Definition											 ///
///																																									 ///
///==================================================================================///
///==================================================================================///

//----------------------------------------------------------------------------
//	Constructor de la clase.
//----------------------------------------------------------------------------
CMachineGroupExtended::CMachineGroupExtended (bool add_in_order)
{
	N_Machines = 0;
	startPtr   = NULL;	
	lastPtr    = NULL;
	GroupLite  = NULL;
	AddInOrder = add_in_order;
	dataChance = true;
}

//----------------------------------------------------------------------------
//	Constructor de Copia.
//----------------------------------------------------------------------------
CMachineGroupExtended::CMachineGroupExtended (const CMachineGroupExtended &Obj)
{
	int count;
	STRUCT_DATA_MACHINE item;

	N_Machines = 0;
	startPtr   = NULL;
	lastPtr    = NULL;
	GroupLite  = NULL;	
	AddInOrder = Obj.IsAddedInOrder();

	count = Obj.GetMachineCount();
	for (int i = 0; i < count; i++)
	{
		item = Obj.GetMachineByIndex(i);
		AddMachine(item.MachineName, item.Time);					
	}
}

//----------------------------------------------------------------------------
//	Destructor de la clase.
//----------------------------------------------------------------------------
CMachineGroupExtended::~CMachineGroupExtended (void)
{
	ClearGroup();
}

//----------------------------------------------------------------------------
//	Agrega una tupla (nombre-maquinas, tiempo) a la etapa.
//----------------------------------------------------------------------------
int CMachineGroupExtended::AddMachine (const char *machine_name, float time)
{
	MACHINES_LIST_PTR newNode, currentPtr, backPtr;

	// Creo el nuevo nodo
	newNode = new MACHINES_LIST;

	if (newNode)
	{
		strcpy(newNode->MachineName, machine_name);
		newNode->Time = time;
		newNode->Next = NULL;
	}
	else return 0;
	
	if (startPtr == NULL)									 /* Lista vacia */
	{
		startPtr = newNode;
		lastPtr  = startPtr; 
	}
	else
	{
		if (AddInOrder)
		{			
			backPtr = currentPtr = startPtr;
			while(currentPtr->Next != NULL && currentPtr->Time < newNode->Time)
			{
				backPtr    = currentPtr;
				currentPtr = currentPtr->Next;
			}
		
			if (backPtr == currentPtr && newNode->Time <= currentPtr->Time)	/* Inserción al inicio */
			{
				newNode->Next = startPtr;
				startPtr = newNode;
			}
			else if (currentPtr->Next != NULL)															/* Inserción en el medio */
			{
				newNode->Next = currentPtr;
				backPtr->Next = newNode;
			}		
			else																														/* Inserción al final  */
			{
				currentPtr->Next = newNode;
			}
		}
		else																															/* Inserción siempre al final  */
		{
			lastPtr->Next = newNode;
			lastPtr = newNode;
		}
	}

	dataChance = true;
	N_Machines++;	
	return 1;
}

//----------------------------------------------------------------------------
//	Actualiza los datos (nombre-maquina, tiempo) de la máquina determinada 
//	por el parámetro <index>.
//----------------------------------------------------------------------------
int CMachineGroupExtended::UpdMachine (unsigned index, const char *machine_name, float time)
{
	MACHINES_LIST_PTR currentPtr;

	currentPtr = startPtr;
	for (unsigned i = 0; i < index && currentPtr != NULL; i++) currentPtr = currentPtr->Next;

	if (currentPtr) 
	{
		dataChance = true;
		strcpy(currentPtr->MachineName, machine_name);
		currentPtr->Time;

		return 1;
	}

	return 0;
}

//----------------------------------------------------------------------------
//	Elimina la máquina especificada por el parametro <index> de la etapa.
//----------------------------------------------------------------------------
int CMachineGroupExtended::DelMachine (unsigned index)
{
	MACHINES_LIST_PTR currentPtr, backPtr;

	backPtr = currentPtr = startPtr;
	for (unsigned i = 0; i < index && currentPtr != NULL; i++)
	{
		backPtr    = currentPtr;
		currentPtr = currentPtr->Next;
	}

	if (currentPtr) 
	{
		dataChance = true;
		backPtr->Next = currentPtr->Next;
		delete currentPtr;

		N_Machines--;		
	}

	return N_Machines;
}

//----------------------------------------------------------------------------
//	Devuelve la cantidad de maquinas que contiene la etapa. 
//----------------------------------------------------------------------------
int CMachineGroupExtended::GetMachineCount (void) const
{
	return N_Machines;
}

//----------------------------------------------------------------------------
//	Devuelve los datos de una machina de la etapa especificada por el 
//	parámetro <index>.
//----------------------------------------------------------------------------
STRUCT_DATA_MACHINE CMachineGroupExtended::GetMachineByIndex (unsigned index) const
{
	STRUCT_DATA_MACHINE tmp;
	MACHINES_LIST_PTR currentPtr;

	currentPtr = startPtr;
	for (unsigned i = 0; i < index && currentPtr != NULL; i++) currentPtr = currentPtr->Next;

	if (currentPtr) 
	{
		strcpy(tmp.MachineName, currentPtr->MachineName);
		tmp.Time = currentPtr->Time;		
	}

	return tmp;
}

//----------------------------------------------------------------------------
//	Devuelve un objeto liviano de tipo Group-Machine.
//----------------------------------------------------------------------------
const CMachineGroupLite *CMachineGroupExtended::GetGroupLite (const CMachineCollection_PM &M_list)
{
	float time;
	int machine;
	MACHINES_LIST_PTR currentPtr;
		
	// Si los datos cambiaron y ya se habia creado el Grupo-Liviano, entonces lo elimino 
	// y lo vuelvo a crear.
	if (dataChance == true && GroupLite != NULL) 
	{
		delete GroupLite;
		GroupLite = NULL;
	}

	if (N_Machines > 0 && dataChance == true)
	{		
		GroupLite = new CMachineGroupLite(N_Machines);

		currentPtr = startPtr; 
		while(currentPtr != NULL)
		{
			machine = M_list.GetIndexByName(currentPtr->MachineName);
			time    = currentPtr->Time;

			GroupLite->AddMachine(machine, time);
			currentPtr = currentPtr->Next;
		}

		dataChance = false;
	}

	if (N_Machines == 0 && dataChance == true)
	{
		GroupLite = new CMachineGroupLite(N_Machines);
    dataChance = false;
	}

	return GroupLite;
}

//----------------------------------------------------------------------------
//	Devuelve TRUE si al agregar una máquina a la etapa, la nueva entrada
//  queda ordenada con respecto a las maquinas que ya contiene el objeto.
//	Devuelve FALSE en caso contrario.
//----------------------------------------------------------------------------
bool CMachineGroupExtended::IsAddedInOrder (void) const
{
	return AddInOrder;
}

//----------------------------------------------------------------------------
//	Limpia el contenido de la etapa.
//----------------------------------------------------------------------------
void CMachineGroupExtended::ClearGroup (void)
{
	MACHINES_LIST_PTR deleteNode;

	deleteNode = startPtr;

	while (deleteNode)
	{
		startPtr = startPtr->Next;
		delete deleteNode;	
		deleteNode = startPtr;
	}
	
	N_Machines = 0;
	startPtr   = NULL;
	lastPtr    = NULL;
	dataChance = true;
}

//----------------------------------------------------------------------------
//	Sobrecarga del operador Asignacion.
//----------------------------------------------------------------------------
CMachineGroupExtended &CMachineGroupExtended::operator = (const CMachineGroupExtended &Obj)
{
	int count;
	STRUCT_DATA_MACHINE item;

	count = Obj.GetMachineCount();

	this->ClearGroup();
	this->AddInOrder = Obj.IsAddedInOrder();

	for (int i = 0; i < count; i++)
	{
		item = Obj.GetMachineByIndex(i);
		this->AddMachine(item.MachineName, item.Time);	
	}

	return (*this);
}

//----------------------------------------------------------------------------
// Sobrecarga del operador aritmetico de igualdad.
//----------------------------------------------------------------------------
bool CMachineGroupExtended::operator == (const CMachineGroupExtended &Obj)
{
	bool exist;
	STRUCT_DATA_MACHINE DM1, DM2;

	// Paso #1: Comparo pa cantidad de maquinas en el grupo
	if (this->N_Machines != Obj.GetMachineCount()) return false;

	// Paso #2: Comparo las maquinas que contienen los grupos
	for (int i = 0; i < this->N_Machines; i++)
	{
		exist = false;
		DM1 = this->GetMachineByIndex(i);
		
		for (int j = 0; j < this->N_Machines; j++)
		{
			DM2 = Obj.GetMachineByIndex(j);
			if (strcmp(DM1.MachineName, DM2.MachineName) == 0 && DM1.Time == DM2.Time) exist = true;
		}

		if (!exist) return false;
	}
	
	// Si todo es exactamente igual, pues entonces devuelve TRUE.
	return true;
}