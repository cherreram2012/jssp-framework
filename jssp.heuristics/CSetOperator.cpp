#include "CSetOperator.h"

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
map<int, vector<CSetOperator::ATOM>> CSetOperator::Diff(const CSchedule &S1, const CSchedule &S2)
{
	map<int, vector<CSetOperator::ATOM>> Diff;
	typedef pair<int, vector<CSetOperator::ATOM>> D_Pair;
	int i, job, machine, recirc, job2, machine2, recirc2;

	S1.GetDimension(&job, &machine, &recirc);
	S2.GetDimension(&job2, &machine2, &recirc2);

	if (job != job2 || machine != machine2 || recirc != recirc2)
		throw std::exception("[CSetOperator::Diff]: unable to determine the difference. S1 and S2 must be the same dimension {n_jobs, n_machines, n_recirc}.");

	// Comparacion de las N x M operaciones iniciales. 
	ATOM atom;
	for (int m = 0; m < machine; m++)
	{
		vector <ATOM> dAtoms;
		for (int j = 0; j < job; j++)
		{
			i = m * job + j;
			if (S1.SequenceId[i] != S2.SequenceId[i]) 
			{
				atom.index = i;
				atom.idS1 = S1.SequenceId[i];
				dAtoms.push_back(atom);
			}			
		}
		if (dAtoms.size() > 0) Diff.insert(D_Pair(m, dAtoms));
	}

	// Comparacion entre las operaciones que recirculan.
	vector <ATOM> dAtoms;
	for (int r = 0; r < recirc; r++)
	{
		i = job * machine + r;
		if (S1.SequenceId[i] != S2.SequenceId[i]) 
		{
			atom.index = i;
			atom.idS1 = S1.SequenceId[i];
			dAtoms.push_back(atom);
		}
	}
	if (dAtoms.size() > 0) Diff.insert(D_Pair(machine + 1, dAtoms));

	return Diff;
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
static map<int, vector<CSetOperator::ATOM>>	Inters(const CSchedule &S1, const CSchedule &S2)
{
	map<int, vector<CSetOperator::ATOM>> Inters;

	return Inters;
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
static map<int, vector<CSetOperator::DUPLEX_ATOM>> Union(const CSchedule &S1, const CSchedule &S2)
{
	map<int, vector<CSetOperator::DUPLEX_ATOM>> Union;

	return Union;
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
map<int, vector<CSetOperator::DUPLEX_ATOM>> CSetOperator::SymmetricDiff(const CSchedule &S1, const CSchedule &S2)
{
	map<int, vector<CSetOperator::DUPLEX_ATOM>> sDiff;
	typedef pair<int, vector<CSetOperator::DUPLEX_ATOM>> SD_Pair;
	int i, job, machine, recirc, job2, machine2, recirc2;

	S1.GetDimension(&job, &machine, &recirc);
	S2.GetDimension(&job2, &machine2, &recirc2);

	if (job != job2 || machine != machine2 || recirc != recirc2)
		throw std::exception("[CSetOperator::Diff]: unable to determine the difference. S1 and S2 must be the same dimension {n_jobs, n_machines, n_recirc}.");

	// Comparacion de las N x M operaciones iniciales. 
	DUPLEX_ATOM atom;
	for (int m = 0; m < machine; m++)
	{
		vector <DUPLEX_ATOM> dAtoms;
		for (int j = 0; j < job; j++)
		{
			i = m * job + j;
			if (S1.SequenceId[i] != S2.SequenceId[i])
			{
				atom.index = i;
				atom.idS1 = S1.SequenceId[i];
				atom.idS2 = S2.SequenceId[i];
				dAtoms.push_back(atom);
			}
		}
		if (dAtoms.size() > 0) sDiff.insert(SD_Pair(m, dAtoms));
	}

	// Comparacion entre las operaciones que recirculan.
	vector <DUPLEX_ATOM> dAtoms;
	for (int r = 0; r < recirc; r++)
	{
		i = job * machine + r;
		if (S1.SequenceId[i] != S2.SequenceId[i])
		{
			atom.index = i;
			atom.idS1 = S1.SequenceId[i];
			atom.idS2 = S2.SequenceId[i];
			dAtoms.push_back(atom);
		}
	}
	if (dAtoms.size() > 0) sDiff.insert(SD_Pair(machine + 1, dAtoms));

	return sDiff;
}
