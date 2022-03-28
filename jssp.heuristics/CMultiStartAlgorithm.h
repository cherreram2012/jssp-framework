#pragma once
#ifndef CMultiStartAlgorithm_H
#define CMultiStartAlgorithm_H

#include "../jssp.model/CSchedule.h"

class CMultiStartAlgorithm
{
	private:
		int iJobs;
		int iMachines;
		int iRecirc;
		unsigned *SequenceId;

	private:
		void swap ( unsigned *a, unsigned *b );

	public:
		//--- Constructor and Destructor ---//
		 CMultiStartAlgorithm ( int njobs, int nmachines, int nrecirc = 0 );
		~CMultiStartAlgorithm ( void );

		//--- Set functions ---//
		void SetSeed (int seed);

		//--- General purpose functions ---//
		CSchedule GenerateSchedule ( ScheduleLengthType type );
};
#endif 

