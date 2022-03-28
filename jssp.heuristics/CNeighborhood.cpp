#include "CNeighborhood.h"

//----------------------------------------------------------------------------------
// Default Constructor for derivated class.
//----------------------------------------------------------------------------------
CNeighborhoobStrategy::CNeighborhoobStrategy(void)
{
	iJobs     = 0;
	iMachines = 0;
	iRecirc   = 0;
	existsEntryPoint = false;
}

//----------------------------------------------------------------------------------
// Virtual destructor.
//----------------------------------------------------------------------------------
CNeighborhoobStrategy::~CNeighborhoobStrategy()
{
}

//----------------------------------------------------------------------------------
// Set the entry point for neighborhood search.
//----------------------------------------------------------------------------------
void CNeighborhoobStrategy::SetEntryPoint(const CSchedule &S)
{
	EntryPoint = S;
	existsEntryPoint = true;
	S.GetDimension(&iJobs, &iMachines, &iRecirc);
}

//----------------------------------------------------------------------------------
// Default Constructor for derivated class.
//----------------------------------------------------------------------------------
CLinearAdjacentNeighbor::CLinearAdjacentNeighbor(void) : CNeighborhoobStrategy()
{
}

//----------------------------------------------------------------------------------
// Destructor for derivated class.
//----------------------------------------------------------------------------------
CLinearAdjacentNeighbor::~CLinearAdjacentNeighbor(void)
{
}

//----------------------------------------------------------------------------------
// Return the cardinality of the adjacent linear neighbor technique. 
//----------------------------------------------------------------------------------
int CLinearAdjacentNeighbor::Cardinality(void)
{
	if (!existsEntryPoint) throw std::exception("CAdjacentLinearNeighbor::Cardinality(): you must specified a entry point schedule.");

	return iMachines - 1;
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
CSchedule CLinearAdjacentNeighbor::VisitNeighbor(int i)
{	
	if (!existsEntryPoint) throw std::exception("CAdjacentLinearNeighbor::VisitNeighbor(): you must specified a entry point schedule.");

	CSchedule neighbor = EntryPoint;

	i = i % Cardinality();
	neighbor.SwapSubsetM(i, i + 1);

	return neighbor;
}

//----------------------------------------------------------------------------------
// Default Constructor for derivated class.
//----------------------------------------------------------------------------------
CCircularAdjacentNeighbor::CCircularAdjacentNeighbor(void) : CNeighborhoobStrategy()
{
}

//----------------------------------------------------------------------------------
// Destructor for derivated class.
//----------------------------------------------------------------------------------
CCircularAdjacentNeighbor::~CCircularAdjacentNeighbor(void)
{
}

//----------------------------------------------------------------------------------
// Return the cardinality of the adjacent circular neighbor technique. 
//----------------------------------------------------------------------------------
int CCircularAdjacentNeighbor::Cardinality(void)
{
	if (!existsEntryPoint) throw std::exception("CAdjacentCircularNeighbor::Cardinality(): you must specified a entry point schedule.");

	return iMachines;
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
CSchedule CCircularAdjacentNeighbor::VisitNeighbor(int i)
{
	CSchedule neighbor = EntryPoint;

	i = i % Cardinality();
	neighbor.SwapSubsetM(i, (i + 1) % iMachines);

	return neighbor;
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
CLinearNoAdjacentNeighbor::CLinearNoAdjacentNeighbor(void) : CNeighborhoobStrategy()
{
	neighbormap = nullptr;
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
CLinearNoAdjacentNeighbor::~CLinearNoAdjacentNeighbor(void)
{
	delete []neighbormap;
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
void CLinearNoAdjacentNeighbor::SetEntryPoint(const CSchedule &S)
{
	int job, machine, recirc, card, offset, x, y;

	EntryPoint = S;
	existsEntryPoint = true;
	S.GetDimension(&job, &machine, &recirc);

	// Solo calculo el mapiamento de la vecindad si la dimension de entry-point dado 
	// es distinto al ultimo conocido.
	if (job != iJobs || machine != iMachines || recirc != iRecirc)
	{
		iJobs = job;
		iMachines = machine;
		iRecirc = recirc;
		card = this->Cardinality();

		if (neighbormap) delete[]neighbormap;
		neighbormap = new struct NeighborMappig[card];
		
		x = 0;
		offset = 1;
		y = offset;		
		for (int i = 0; i < card; i++)
		{
			neighbormap[i].X = x;
			neighbormap[i].Y = y++;
			
			if (y > iMachines - 1)
			{
				x++;
				offset++;
				y = offset;
			}
		}
	}	
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
int CLinearNoAdjacentNeighbor::Cardinality(void)
{
	return (iMachines * (iMachines - 1)) / 2;
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
CSchedule CLinearNoAdjacentNeighbor::VisitNeighbor(int i)
{
	CSchedule neighbor = EntryPoint;

	i = i % Cardinality();
	neighbor.SwapSubsetM(neighbormap[i].X, neighbormap[i].Y);

	return neighbor;
}