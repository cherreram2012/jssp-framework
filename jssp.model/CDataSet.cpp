#include <iostream>
#include "CDataSet.h"
#include "../chm.utilities/CustomConsole.h"

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
CDataSet::CDataSet(int jobs)
{
	iJobs = jobs;
	iMachines = 0;
	iRecirc = 0;
	JobsArray = new STRUCT_JOB[iJobs];

	opIndex = new int[iJobs];
	for (int i = 0; i < iJobs; i++) {
		opIndex[i] = 0;
		JobsArray[i].OpCount = 0;
		JobsArray[i].Operations = nullptr;
	}
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
CDataSet::~CDataSet(void)
{
	for (int i = 0; i < iJobs; i++) delete []JobsArray[i].Operations;

	delete[]JobsArray;
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
void CDataSet::SetRecircCount(int recirc)
{
	iRecirc = recirc;
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
void CDataSet::SetMachinesCount(int machines)
{
	iMachines = machines;
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
int CDataSet::GetJobsCount(void) const
{
	return iJobs;
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
int CDataSet::GetMachinesCount(void) const
{
	return iMachines;
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
int CDataSet::GetRecircCount(void) const
{
	return iRecirc;
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
int CDataSet::GetOperationCount(int indexjob) const
{
	if (indexjob >= iJobs) throw std::exception("[CSPDataSet::getOperationCount]: parámetro <indexjob> fuera de rango.");

	return JobsArray[indexjob].OpCount;
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
bool CDataSet::GetOperationByIndex(int indexjob, int indexop, int *machine, float *time) const
{
	if (indexjob >= iJobs || indexop >= JobsArray[indexjob].OpCount) return false;

	if (time)	*time = JobsArray[indexjob].Operations[indexop].Time;
	if (machine) *machine = JobsArray[indexjob].Operations[indexop].Machine;	

	return true;
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
void CDataSet::AllocateSpace(int indexjob, int opcount)
{
	if (indexjob >= iJobs) throw std::exception("[CSPDataSet::allocateSpace]: parámetro <indexjob> fuera de rango.");

	JobsArray[indexjob].OpCount = opcount;
	JobsArray[indexjob].Operations = new STRUCT_OPERATION[opcount];
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
void CDataSet::AddOperation(int indexjob, int idmachine, float time)
{
	if (indexjob >= iJobs) throw std::exception("[CSPDataSet::addOperation]: parámetro <indexjob> fuera de rango.");

	if (opIndex[indexjob] >= JobsArray[indexjob].OpCount) return;

	JobsArray[indexjob].Operations[opIndex[indexjob]].Machine = idmachine;
	JobsArray[indexjob].Operations[opIndex[indexjob]].Time = time;

	opIndex[indexjob]++;
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
std::ostream& operator << (std::ostream &stream, const CDataSet &dataset)
{
	float time;
	int jobs, operations, machine;

	jobs = dataset.GetJobsCount();

	CustomConsole::ForegroundColor(ColorDarkCyan);
	stream << "N-Jobs: " << jobs << std::endl;
	CustomConsole::LastState();
	for (int j = 0; j < jobs; j++)
	{
		operations = dataset.GetOperationCount(j);
		CustomConsole::ForegroundColor(ColorDarkCyan);
		stream << " [" << j << "]";
		CustomConsole::LastState();
		stream << " ->";
		CustomConsole::ForegroundColor(ColorDarkYellow);
		stream << " op:" << operations;
		CustomConsole::LastState();
		stream << " |";
		for (int op = 0; op < operations; op++)
		{
			dataset.GetOperationByIndex(j, op, &machine, &time);						
			stream << "| ";
			CustomConsole::ForegroundColor(ColorGreen);
			stream << "m:" << machine << " (";
			stream.width(3);
			stream << time << ") ";
			stream.width(0);
			CustomConsole::LastState();
		}
		stream << " ||" << std::endl;
	}
	
	return stream;
}