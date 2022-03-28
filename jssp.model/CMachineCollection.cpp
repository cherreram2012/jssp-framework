#include "CMachineCollection.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>

//----------------------------------------------------------------------------
//	Constructor.
//----------------------------------------------------------------------------
CMachineCollection::CMachineCollection (int count)
{
	// Valido que la cantidad de operaciones sea mayor que cero. De no ser asi
	// aborto la ejecucion del programa
	assert(count > 0);

	iIndex = 0;
	iSize = count;
	NominalPower  = new float [iSize];
	MachineList   = new char *[iSize];

	for (int i = 0; i < iSize; i++)
		NominalPower[i] = 0.0;
}

//----------------------------------------------------------------------------
//	Destructor.
//----------------------------------------------------------------------------
CMachineCollection::~CMachineCollection (void)
{
	for (int i = 0; i < iIndex; i++)
		delete []MachineList[i];

	delete []NominalPower;
	delete []MachineList;
}

//----------------------------------------------------------------------------
//	Agrega una operacion al listado de definiciones y devuelve el subindice 
//	que le corresponde en el listado. En caso de no poderse agregar devuelve -1.
//----------------------------------------------------------------------------
int CMachineCollection::AddMachine (char *nameMachine)
{
	if (iIndex < iSize)
	{
		MachineList[iIndex] = new char[STR_LENGTH];
		strcpy(MachineList[iIndex], nameMachine);
		iIndex++;

		return iIndex - 1;
	}

	return -1;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CMachineCollection::SetNominalPower (unsigned index, float NominalPow)
{	
	if (index < (unsigned)iIndex)
	{
		NominalPower[index] = NominalPow;
		return 1;
	}

	return 0;
}

//----------------------------------------------------------------------------
//	Devuelve el subindice que tiene la operacion dentro del listado de definiciones.
//  En caso de no existir en el listado devuelve -1.
//----------------------------------------------------------------------------
int CMachineCollection::GetIndexByName (const char *nameMachine) const
{
	for (int i = 0; i < iIndex; i++)
		if (strcmp(MachineList[i], nameMachine) == 0)
			return i;
		
	return -1;
}

//----------------------------------------------------------------------------
//	Devuelve el nombre de la operacion que corresponde con el subindice pasado 
//  por parametro. O NULL para un subindice invalido.
//----------------------------------------------------------------------------
const char *CMachineCollection::GetNameByIndex (unsigned index) const
{	
	return (index < (unsigned)iIndex) ? MachineList[index] : '\0';
}

//----------------------------------------------------------------------------
//	Devuelve la potencia nominal de una maquina-herramienta.
//----------------------------------------------------------------------------
float CMachineCollection::GetNominalPower (unsigned index) const
{
	return (index < (unsigned)iIndex) ? NominalPower[index] : 0.0;
}

//----------------------------------------------------------------------------
//	Devuelve cuantas maquinas tiene la coleccion.
//----------------------------------------------------------------------------
int CMachineCollection::GetMachineCount (void) const
{
	return iIndex;
}

//----------------------------------------------------------------------------
//	
//----------------------------------------------------------------------------
int CMachineCollection::GetMaxNameLength (void) const
{
  int len = 0, c;

  for (int i = 0; i < iSize; i++)
  {
    c = strlen(GetNameByIndex(i));
    if (c > len) len = c;
  }

  return len;
}

//----------------------------------------------------------------------------
//	Devuelve 1 si el listado de operaciones esta lleno, 0 en caso contrario.
//----------------------------------------------------------------------------
bool CMachineCollection::isFull (void) const
{
	return (iIndex == iSize) ? 1 : 0;
}

//----------------------------------------------------------------------------
//	Devuelve 1 si el listado de operaciones esta vacia, 0 en caso contrario.
//----------------------------------------------------------------------------
bool CMachineCollection::isEmpty (void) const
{
	return (iIndex == 0) ? 1 : 0;
}

//----------------------------------------------------------------------------
//	Funcion Amiga de la clase CMachineCollection. Imprime los datos miembros 
//  del objeto pasado por parametro.
//----------------------------------------------------------------------------
void PrintMachineInformation(const CMachineCollection &Obj)
{
	printf("MACHINE INFORMATION\n\n");
	printf("Machines: %d\n\n", Obj.iIndex);

	for(int i = 0; i < Obj.iIndex; i++)
		printf(" %s\n", Obj.MachineList[i]);

	printf("\nPotencia Nominal\n");
	for(int j = 0; j < Obj.iIndex; j++)
		printf(" %.1f\n", Obj.NominalPower[j]);

	printf("\n\n");
}
