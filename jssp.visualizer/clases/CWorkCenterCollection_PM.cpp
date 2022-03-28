#include "CWorkCenterCollection_PM.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

///==================================================================================///
///==================================================================================///
///																																									 ///
///													CWorkCenter Class Definition								 						 ///
///																																									 ///
///==================================================================================///
///==================================================================================///

//----------------------------------------------------------------------------
//	Constructor por defecto
//----------------------------------------------------------------------------
CWorkCenter::CWorkCenter ()
{
	N_Machines			 = 0;
	OperationName[0] = '\0';
	startPtr				 = NULL;
	lastPtr					 = NULL;
	MachinesArray    = NULL;	
	fastAccessMode   = false;
	dataChance       = true;	
}

//----------------------------------------------------------------------------
//	Constructor de Copia. 
//----------------------------------------------------------------------------
CWorkCenter::CWorkCenter (const CWorkCenter &Obj)
{
	int count;

	startPtr			 = NULL;
	lastPtr				 = NULL;
	MachinesArray  = NULL;
	N_Machines		 = 0;
	fastAccessMode = false;
	strcpy(OperationName, Obj.GetOperationName());;

	count	= Obj.GetMachineCount();
	for (int i = 0; i < count; i++) AddMachine(Obj.GetMachineByIndex(i));
}

//----------------------------------------------------------------------------
//	Destructor de la clase.
//----------------------------------------------------------------------------
CWorkCenter::~CWorkCenter ( )
{
	ClearCenter();
	DisableFastAccessMode();
}

//----------------------------------------------------------------------------
//	Agrega una maquina al WorkCenter.
//----------------------------------------------------------------------------
int CWorkCenter::AddMachine (const char *machine_name)
{
	MACHINES_LIST_PTR newNode;

	// Creo el nuevo nodo
	newNode = new MACHINES_LIST;

	if (newNode)
	{
		strcpy(newNode->MachineName, machine_name);
		newNode->Next = NULL;
	}
	else return 0;
	
	if (startPtr == NULL)					/* Lista vacia */
	{
		startPtr = newNode;
		lastPtr  = startPtr; 
	}
	else
	{			
		lastPtr->Next = newNode;		/* Insercion siempre al final */
		lastPtr = newNode;
	}

	N_Machines++;		
	dataChance = true;
	fastAccessMode = false;

	return 1;
}

//----------------------------------------------------------------------------
//	Actualiza el campo nombre de la maquina espeficada por el parametro
//	'machine_name'.
//----------------------------------------------------------------------------
int CWorkCenter::UpdMachine (unsigned index, const char *machine_name)
{
	MACHINES_LIST_PTR currentPtr;

	currentPtr = startPtr;
	for (unsigned i = 0; i < index && currentPtr != NULL; i++) currentPtr = currentPtr->Next;
	
	if (currentPtr) 
	{
		strcpy(currentPtr->MachineName, machine_name);
		dataChance = true;
		fastAccessMode = false;

		return 1;
	}

	return 0;
}

//----------------------------------------------------------------------------
//	Elimina la maquina especificada por el parametro index.
//----------------------------------------------------------------------------
int CWorkCenter::DelMachine (unsigned index)
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
		backPtr->Next = currentPtr->Next;
		delete currentPtr;

		N_Machines--;
	}
	
	dataChance = true;
	fastAccessMode = false;
	return N_Machines;
}

//----------------------------------------------------------------------------
//	Establece el nombre de la operacion que caracteriza al workcenter.
//----------------------------------------------------------------------------
void CWorkCenter::SetOperationName (const char *op_name)
{
	strcpy(OperationName, op_name);
}

//----------------------------------------------------------------------------
//	Devuelve la cantidad de maquinas que contiene el WorkCenter.
//----------------------------------------------------------------------------
int	CWorkCenter::GetMachineCount (void) const
{
	return N_Machines;
}

//----------------------------------------------------------------------------
//	Devuelve el nombre de la operacion que identificara al WorkCenter.
//----------------------------------------------------------------------------
const char *CWorkCenter::GetOperationName (void) const
{
	return OperationName;
}

//----------------------------------------------------------------------------
//	Devuelve el nombre del campo MachineName de la maquina especificada por
//	el parametro 'index'.
//----------------------------------------------------------------------------
const char *CWorkCenter::GetMachineByIndex (unsigned index) const
{
	MACHINES_LIST_PTR currentPtr;

	if (fastAccessMode == true)
	{
		return (index < (unsigned)N_Machines) ? MachinesArray[index] : '\0';
	}

	currentPtr = startPtr;
	for (unsigned i = 0; i < index && currentPtr != NULL; i++) currentPtr = currentPtr->Next;

	return (currentPtr) ? currentPtr->MachineName : '\0';
}

//----------------------------------------------------------------------------
//	Determina si la maquina (segun nombre) pasado por parametro existe en 
//	el WorkCenter. Devuelve 1 si la maquinas pertenece al work-center, 0 en
//  caso cntrario.
//----------------------------------------------------------------------------
bool CWorkCenter::ExistsMachineInCenter	(const char *machine_name)
{
	MACHINES_LIST_PTR currentPtr;

	currentPtr = startPtr;
	for (int i = 0; currentPtr != NULL; i++) 
	{
		if (strcmp(currentPtr->MachineName, machine_name) == 0) return true;

		currentPtr = currentPtr->Next;
	}

	return false;
}

//----------------------------------------------------------------------------
//	Habilita el Modo de Acceso Rapido para esta clase.
// 
//	Nota: el Modo de Acceso Rapido lo que hace es indexar los elementos que 
//				contiene el WorkCenter para aumentar la velocidad de accesos a los 
//				elementos via su indice.
//----------------------------------------------------------------------------
void CWorkCenter::EnableFastAccessMode (void)
{
	int i;
	MACHINES_LIST_PTR currentPtr; 

	if (fastAccessMode) return;
	if (dataChance) DisableFastAccessMode();

	dataChance = false;
	fastAccessMode = true;

	MachinesArray = new char*[N_Machines];
	for (i = 0; i < N_Machines; i++) MachinesArray[i] = new char [LENGTH];

	currentPtr = startPtr;
	for (i = 0; i < N_Machines && currentPtr != NULL; i++)
	{
		strcpy(MachinesArray[i], currentPtr->MachineName);
		currentPtr = currentPtr->Next;
	}
}

//----------------------------------------------------------------------------
//	Desabilita el Modo de Acceso Rapido para esta clase.
//----------------------------------------------------------------------------
void CWorkCenter::DisableFastAccessMode	(void)
{
	fastAccessMode = false;

	if (MachinesArray)
	{
		for (int i = 0; i < N_Machines; i++) delete []MachinesArray[i];
		delete []MachinesArray;
	}

	MachinesArray = NULL;
}

//----------------------------------------------------------------------------
//	Limpia los datos del WorkCenter.
//----------------------------------------------------------------------------
void CWorkCenter::ClearCenter (void)
{
	MACHINES_LIST_PTR deleteNode;

	deleteNode = startPtr;

	while (deleteNode)
	{
		startPtr = startPtr->Next;
		delete deleteNode;	
		deleteNode = startPtr;
	}

	N_Machines       = 0;
	OperationName[0] = '\0';
	startPtr         = NULL;
	lastPtr          = NULL;

	DisableFastAccessMode();
}

//----------------------------------------------------------------------------
//	Sobrecarga del operador Asignación.
//----------------------------------------------------------------------------
CWorkCenter &CWorkCenter::operator = (const CWorkCenter &Obj)
{
	int count;

	count = Obj.GetMachineCount();
	this->ClearCenter();
	strcpy(this->OperationName, Obj.GetOperationName());	

	for (int i = 0; i < count; i++) this->AddMachine(Obj.GetMachineByIndex(i));
	
	return (*this);
}

//----------------------------------------------------------------------------
//	Sobrecarga del operador aritmetico Igualdad.
//----------------------------------------------------------------------------
bool CWorkCenter::operator == (const CWorkCenter &Obj)
{
	// Paso #1: Comparo si las cantidad de maquinas en ambos work-center es igual
	if (this->N_Machines != Obj.GetMachineCount()) return false;

	// Paso #2: Comparo cada maquina que contienen los work-center.
	for (int i = 0; i < this->N_Machines; i++)	
		if (strcmp(this->GetMachineByIndex(i), Obj.GetMachineByIndex(i)) != 0) return false;

	return true;
}

///==================================================================================///
///==================================================================================///
///																																									 ///
///													CWorkCenterCollection Class Definition						 		   ///
///																																									 ///
///==================================================================================///
///==================================================================================///

//----------------------------------------------------------------------------
//	Constructor por defecto.
//----------------------------------------------------------------------------
CWorkCenterCollection::CWorkCenterCollection (void)
{
	N_WorkCenter	 = 0;
	startPtr			 = NULL;
	lastPtr				 = NULL;
	WCenterArray	 = NULL;
	fastAccessMode = false;
	dataChance     = true;
}

//----------------------------------------------------------------------------
//	Constructor de copia.
//----------------------------------------------------------------------------
CWorkCenterCollection::CWorkCenterCollection (const CWorkCenterCollection &Obj)
{
	N_WorkCenter	 = 0;
	startPtr			 = NULL;
	lastPtr				 = NULL;
	WCenterArray	 = NULL;
	fastAccessMode = false;
	
	for (int i = 0; i < N_WorkCenter; i++) AddWorKCenter(Obj.GetWorkCenterByIndex(i));
}

//----------------------------------------------------------------------------
//	Destructor de la clase.
//----------------------------------------------------------------------------
CWorkCenterCollection::~CWorkCenterCollection	(void)
{
	ClearCollection();
	DisableFastAccessMode();
}

//----------------------------------------------------------------------------
//	Agrega un WorkCenter a la coleccion.
//----------------------------------------------------------------------------
int CWorkCenterCollection::AddWorKCenter (const CWorkCenter &Obj)
{
	WORKCENTER_LIST_PTR newNode;

	// Creo el nuevo nodo
	newNode = new WORKCENTER_LIST;

	if (newNode)
	{
		newNode->WCenter = Obj;
		newNode->Next    = NULL;
	}
	else return 0;
	
	if (startPtr == NULL)					/* Lista vacia */
	{
		startPtr = newNode;
		lastPtr  = startPtr; 
	}
	else
	{			
		lastPtr->Next = newNode;		/* Insercion siempre al final */
		lastPtr = newNode;
	}

	N_WorkCenter++;		
	dataChance = true;
	fastAccessMode = false;

	return 1;
}

//----------------------------------------------------------------------------
//	Actualiza el WorkCenter especificado por el parametro 'index'.
//----------------------------------------------------------------------------
int CWorkCenterCollection::UdpWorkCenter (unsigned index, const CWorkCenter &Obj)
{
	WORKCENTER_LIST_PTR currentPtr;

	currentPtr = startPtr;
	for (unsigned i = 0; i < index && currentPtr != NULL; i++) currentPtr = currentPtr->Next;
	
	if (currentPtr) 
	{
		currentPtr->WCenter = Obj;
		dataChance = true;
		fastAccessMode = false;

		return 1;
	}

	return 0;
}

//----------------------------------------------------------------------------
//	Elimina de la coleccion el WorkCenter especificado por el parametro 'index'.
//----------------------------------------------------------------------------
int CWorkCenterCollection::DelWorkCenter (unsigned index)
{
	WORKCENTER_LIST_PTR currentPtr, backPtr;

	backPtr = currentPtr = startPtr;
	for (unsigned i = 0; i < index && currentPtr != NULL; i++)
	{
		backPtr    = currentPtr;
		currentPtr = currentPtr->Next;
	}

	if (currentPtr) 
	{
		backPtr->Next = currentPtr->Next;
		delete currentPtr;

		N_WorkCenter--;
	}
	
	dataChance = true;
	fastAccessMode = false;

	return N_WorkCenter;
}

//----------------------------------------------------------------------------
//	Devuelve la cantidad de WorkCenter que contiene la coleccion.
//----------------------------------------------------------------------------
int	CWorkCenterCollection::GetWorkCenterCount	(void) const
{
	return N_WorkCenter;
}

//----------------------------------------------------------------------------
//	Devuelve la cantidad de maquinas que contiene la coleccion.
//----------------------------------------------------------------------------
int CWorkCenterCollection::GetTotalMachine (void) const
{
	int count = 0;
	WORKCENTER_LIST_PTR currentPtr;

	currentPtr = startPtr;	
	while (currentPtr != NULL)
	{
		count += currentPtr->WCenter.GetMachineCount();
		currentPtr = currentPtr->Next;
	}

	return count;
}

//----------------------------------------------------------------------------
//	Devuelve el WorkCenter determinado por el parametro 'index'.
//----------------------------------------------------------------------------
CWorkCenter CWorkCenterCollection::GetWorkCenterByIndex (unsigned index) const
{
	CWorkCenter empty;
	WORKCENTER_LIST_PTR currentPtr;

	if (fastAccessMode == true)
	{
		return (index < (unsigned)N_WorkCenter) ? WCenterArray[index] : empty;
	}

	currentPtr = startPtr;
	for (unsigned i = 0; i < index && currentPtr != NULL; i++) currentPtr = currentPtr->Next;

	return (currentPtr) ? currentPtr->WCenter : empty;
}

//----------------------------------------------------------------------------
//	Determina si la maquina (segun su nombre) pasada como parametro existe 
//  en alguno de los WrkCenter que contiene que coleccion. Devuelve 1 si se
//	encuentra la maquina en algu work-center, 0 en caso contrario.
//----------------------------------------------------------------------------
bool CWorkCenterCollection::ExistsMachineInCollection	(const char *machine_name) const
{
	WORKCENTER_LIST_PTR currentPtr;

	currentPtr = startPtr;
	for (int i = 0; currentPtr != NULL; i++) 
	{
		if (currentPtr->WCenter.ExistsMachineInCenter(machine_name)) return true;

		currentPtr = currentPtr->Next;
	}

	return false;
}

//----------------------------------------------------------------------------
//	Habilita el Modo de Acceso Rapido para esta clase.
// 
//	Nota: el Modo de Acceso Rapido lo que hace es indexar los elementos que 
//				contiene el WorkCenter para aumentar la velocidad de accesos a los 
//				elementos via su indice.	
//----------------------------------------------------------------------------
void CWorkCenterCollection::EnableFastAccessMode (void)
{
	int i;
	WORKCENTER_LIST_PTR currentPtr; 

	if (fastAccessMode) return;
	if (dataChance) DisableFastAccessMode();
		
	dataChance = false;
	fastAccessMode = true;
	WCenterArray = new CWorkCenter[N_WorkCenter];
	
	currentPtr = startPtr;
	for (i = 0; i < N_WorkCenter && currentPtr != NULL; i++)
	{
		WCenterArray[i] = currentPtr->WCenter;
		currentPtr = currentPtr->Next;
	}
}

//----------------------------------------------------------------------------
//	Desabilita el Modo de Acceso Rapido para esta clase.	
//----------------------------------------------------------------------------
void CWorkCenterCollection::DisableFastAccessMode	(void)
{
	fastAccessMode = false;

	if (WCenterArray)	delete []WCenterArray;

	WCenterArray = NULL;
}

//----------------------------------------------------------------------------
//	Limpia los datos que contiene la coleccion.
//----------------------------------------------------------------------------
void CWorkCenterCollection::ClearCollection (void)
{
	WORKCENTER_LIST_PTR deleteNode;

	deleteNode = startPtr;

	while (deleteNode)
	{
		startPtr = startPtr->Next;
		delete deleteNode;	
		deleteNode = startPtr;
	}

	N_WorkCenter = 0;
	startPtr     = NULL;
	lastPtr      = NULL;	

	DisableFastAccessMode();
}

//----------------------------------------------------------------------------
//	Sobrecarga del operador de Asignacion.
//----------------------------------------------------------------------------
CWorkCenterCollection &CWorkCenterCollection::operator = (const CWorkCenterCollection &Obj)
{
	int count;

	count = Obj.GetWorkCenterCount();
	for (int i = 0; i < count; i++) this->AddWorKCenter(Obj.GetWorkCenterByIndex(i));
	
	return (*this);
}

//----------------------------------------------------------------------------
//	Sobrecarga del operador aritmetico Igualdad.
//----------------------------------------------------------------------------
bool CWorkCenterCollection::operator == (const CWorkCenterCollection &Obj)
{
	bool exist;

	// paso #1: Comparo si la cantidad de work-center en ambas colecciones es 
	//					la misma.
	if (this->N_WorkCenter != Obj.GetWorkCenterCount()) return false;

	// Paso #2: Comparo los work-centers de ambas colecciones
	for (int i = 0; i < this->N_WorkCenter; i++)
	{
		exist = false;
		for (int j = 0; j < this->N_WorkCenter; j++)
		{
			if (this->GetWorkCenterByIndex(j) == Obj.GetWorkCenterByIndex(j)) exist = true;
		}

		if (!exist) return false;
	}

	return true;
}

//---------
// Funcion temporal para imprimir los datos de la coleccion de workcenters.
void CWorkCenterCollection::PrintData (void) const
{
	int m;
	CWorkCenter WC;

	printf("\nWORKCENTERS: (%d)\n", N_WorkCenter);
	for (int i = 0; i < N_WorkCenter; i++)
	{
		WC = GetWorkCenterByIndex(i);
		m = WC.GetMachineCount();

		printf("WC #%d: (%d)\n", i+1, m);
		printf("  Name: %s\n", WC.GetOperationName());
		for (int j = 0; j < m; j++)	printf("  %s\n", WC.GetMachineByIndex(j));
		printf("\n");
	}
	printf("\n");
}