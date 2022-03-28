#pragma once
#ifndef CSPDataSet_H
#define CSPDataSet_H

#include <iostream>

//------------------------------------------------------------------------------
//	ClassName  : CSPDataSet
//
//  Description: .
//
//  Revision   : 07/04/2019 
//------------------------------------------------------------------------------
class CDataSet {
	typedef struct Operation {
		float Time;
		int Machine;

		//--- struct constructor ---//
		struct Operation() {
			Time = 0;
			Machine = -1;
		}
	} STRUCT_OPERATION;

	typedef struct Job {
		int OpCount;
		STRUCT_OPERATION *Operations;

		//--- struct constructor ---//
		struct Job() {
			Operations = nullptr;
		}
	} STRUCT_JOB;

	private:
		int iJobs;
		int iMachines;
		int iRecirc;
		int *opIndex;
		struct Job *JobsArray;

	public:
		//--- Constructor and destructor ---//
		 CDataSet ( int njobs );
		~CDataSet ( void );

		//--- Set functions ---//
		void SetRecircCount   ( int recirc );
		void SetMachinesCount ( int machines );

		//--- Get functions ---//
		 int GetJobsCount			   ( void ) const;
		 int GetMachinesCount	   ( void ) const;
		 int GetRecircCount      ( void ) const;
		 int GetOperationCount	 ( int indexjob ) const;
		bool GetOperationByIndex ( int indexjob, int indexop, int *idmachine, float *time ) const;

		//--- ? functions ---//
		void AllocateSpace ( int indexjob, int opcount );
		void AddOperation  ( int indexjob, int idmachine, float time );

		//--- Overload funtions ---//
		friend std::ostream& operator << (std::ostream &stream, const CDataSet &dataset);
};

#endif 

