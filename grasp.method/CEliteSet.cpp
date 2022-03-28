#include <limits.h>
#include "CEliteSet.h"
#include "../mersenne_twister/mt19937ar.h"
#include "../jssp.heuristics/CSetOperator.h"

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
float CEliteSet::DeltaDiff(const CSchedule &S1, const CSchedule &S2)
{
	float diff;
	int jobs, machines;
	map<int, vector<CSetOperator::DUPLEX_ATOM>> SymDiff;
	map<int, vector<CSetOperator::DUPLEX_ATOM>>::iterator it;

	diff = 0.0f;
	jobs = S1.GetJobsCount();
	machines = S1.GetMachinesCount();
	SymDiff = CSetOperator::SymmetricDiff(S1, S2);

	for (it = SymDiff.begin(); it != SymDiff.end(); it++)
		diff += ((float)it->second.size() / (float)jobs);

	return diff / (float)machines;
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
float CEliteSet::GanmaMinFor(const CSchedule &S, float f)
{
	float g, ganma;

	ganma = numeric_limits<float>::infinity();
	for (int i = 0; i <= Index; i++)
	{
		if (f != EliteArray[i].f)
			g = 1.0f;	// diferencia maxima
		else
			g = DeltaDiff(S, EliteArray[i].S); // diferencia parcial

		if (g < ganma) ganma = g;
	}

	return ganma;
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
int CEliteSet::ArgMinForGanma(const CSchedule &S, float f)
{
	int k, count;
	float g, ganma;

	count = Count();
	ganma = numeric_limits<float>::infinity();
	for (int i = 0; i < count; i++)
	{
		if (EliteArray[i].f >= f)
		{
			g = DeltaDiff(S, EliteArray[i].S);

			if (g < ganma)
			{
				k = i;
				ganma = g;
			}
		}
	}

	return k;
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
void CEliteSet::UpdateBestAndWorst_Index(void)
{
	int count;

	fWorst = 0.0f;
	count = Count();
	fBest = numeric_limits<float>::infinity();
	for (int i = 0; i < count; i++)
	{
		if (EliteArray[i].f < fBest)
		{
			fBest = EliteArray[i].f;
			sBest = EliteArray[i].S;
			indexBest = i;
		}
		if (EliteArray[i].f > fWorst)
		{
			fWorst = EliteArray[i].f;
			sWorst = EliteArray[i].S;
			indexWorst = i;
		}
	}
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
CEliteSet::CEliteSet(unsigned int size)
{
	uSize			 = size;
	Index			 = -1;
	indexBest  = -1;
	indexWorst = -1;
	GanmaDiff	 = 0.0f;
	fWorst		 = 0.0f;
	fBest			 = numeric_limits<float>::infinity();
	EliteArray = new ELITE_ITEM[uSize];
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
CEliteSet::~CEliteSet(void)
{
	if (EliteArray) delete []EliteArray;
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
int CEliteSet::UpdateElite(const CSchedule &S, float f)
{
	int pos;
	float ganma;
	bool isadd = false;

	pos = -1;
	if (Count() < (int)uSize)
	{
		if (isEmpty())
		{
			Index++;
			EliteArray[Index].f = f;
			EliteArray[Index].S = S;
			isadd = true;
		}
		else
		{
			ganma = GanmaMinFor(S, f);
			if (ganma > GanmaDiff)
			{
				Index++;
				EliteArray[Index].f = f;
				EliteArray[Index].S = S;
				isadd = true;
			}
		}	

		if (isadd)
		{
			pos = Index;
			if (f < fBest)
			{
				fBest = f;
				sBest = S;
				indexBest = Index;				
			}
			if (f > fWorst)
			{
				fWorst = f;
				sWorst = S;
				indexWorst = Index;
			}
		}
	}
	else
	{
		ganma = GanmaMinFor(S, f);

		if (f < fWorst && ganma > GanmaDiff)
		{
			int k = ArgMinForGanma(S, f);
			EliteArray[k].f = f;
			EliteArray[k].S = S;
			isadd = true;
			pos = k;
		}

		// Actualizo los indices de la mejor/peor solucion del conjunto elite (E).
		if (isadd) UpdateBestAndWorst_Index();
	}

	return pos;
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
int CEliteSet::Count(void) const
{
	return (Index != -1) ? Index + 1 : 0;
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
bool CEliteSet::isEmpty(void) const
{
	return Index == -1;
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
CSchedule CEliteSet::RandomSelect(float *f) const
{		
	if (Index == -1) throw std::exception("CEliteSet::RandomSelect(): unable to random select. Elite set is empty.");

	int i = genrand_int32() % (Index + 1);

	*f = EliteArray[i].f;
	return EliteArray[i].S;
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
CSchedule CEliteSet::getBest (float *f) const
{
	if (f) *f = fBest;
	return sBest;
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
CSchedule CEliteSet::getWorst (float *f) const
{
	if (f) *f = fWorst;
	return sWorst;
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
void CEliteSet::Restart(void)
{
	Index = -1;
	delete[]EliteArray;
	EliteArray = new ELITE_ITEM[uSize];
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
void CEliteSet::ClearAll(void)
{
	delete[]EliteArray;

	Index = -1;
	indexBest = -1;
	indexWorst = -1;
	fBest = 0.0f;
	fWorst = 0.0f;
	EliteArray = new ELITE_ITEM[uSize];
}