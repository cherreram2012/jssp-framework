#include "CMultiStartAlgorithm.h"
#include "../mersenne_twister/mt19937ar.h"

void CMultiStartAlgorithm::swap(unsigned *a, unsigned *b)
{

}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
CMultiStartAlgorithm::CMultiStartAlgorithm(int njobs, int nmachines, int nrecirc)
{
	iJobs = njobs;
	iMachines = nmachines;
	iRecirc = nrecirc;
	init_genrand(1);

	SequenceId = new unsigned[iJobs * iMachines + iRecirc];
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
CMultiStartAlgorithm::~CMultiStartAlgorithm(void)
{
	delete[]SequenceId;
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
void CMultiStartAlgorithm::SetSeed(int seed)
{
	init_genrand(seed);
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
CSchedule CMultiStartAlgorithm::GenerateSchedule(ScheduleLengthType type)
{
	int j, size;
	unsigned tmp;
	CSchedule schedule(type, iJobs, iMachines, iRecirc);
	
	size = (type == N_SCHEDULE) ? iJobs : iJobs * iMachines + iRecirc;
	
	// Etapa 1.
	for (int i = 0; i < size; i++) SequenceId[i] = i % iJobs + 1;	
	
	// Etapa 2.
	for (int i = 0; i < size; i++) 
	{
		j = genrand_int32() % size;		
		tmp = SequenceId[i];
		SequenceId[i] = SequenceId[j];
		SequenceId[j] = tmp;

		schedule.SetIdByIndex(i, SequenceId[i]);
		schedule.SetIdByIndex(j, SequenceId[j]);
	}

	return schedule;
}
