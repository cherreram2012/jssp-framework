#pragma once
#ifndef CSPSchedule_H
#define CSPSchedule_H

#include <iostream>

enum ScheduleLengthType { N_SCHEDULE = 1,  NxM_SCHEDULE = 2};

class CScheduleBase {

};

class CSetOperator;
class RestrictedNeighbor;

//------------------------------------------------------------------------------
//	ClassName: CSPSequence
//
//  Description: Implement an schedule t.
//
//  Revision: 08/04/2019 
//------------------------------------------------------------------------------
class CSchedule {
	private:
		int iIndex;
		int iJobs;
		int iMachines;
		int iRecirc;
		int iSize;
		unsigned *SequenceId;
		ScheduleLengthType LengthType;

	public:
		friend CSetOperator;
		friend RestrictedNeighbor;

		//--- Constructor and destructor ---//
		 CSchedule ( void );
		 CSchedule ( ScheduleLengthType type, int njobs, int nmachines, int nrecirc = 0 );
		 CSchedule ( const CSchedule &obj );
		~CSchedule ( void );

		//--- Set functions ---//
		void AddToSchedule ( unsigned id );
		void SetDimension  ( ScheduleLengthType type, int njobs, int nmachines, int nrecirc = 0 );
		void SetIdByIndex	 ( int index, unsigned id );
		
		//--- Get functions ---//
				 int GetSize					( void ) const;
				void GetDimension		  ( int *jobs, int *machines, int *recirc ) const;
		unsigned GetIdByIndex			( int index ) const;
		unsigned GetJobsCount			( void ) const;
		unsigned GetMachinesCount ( void ) const;
		ScheduleLengthType GetLengthType ( void ) const;

		//--- General purpose functions ---//
		void SwapAtom			 ( unsigned int indexA, unsigned int indexB );
		void SwapSubsetM	 ( unsigned int subsetA, unsigned int subsetB );
		bool IsValid			 ( bool recirc = false ) const;
		bool IsComplete		 ( void ) const;
		void ClearSchedule ( void );
		void ResectInsertionIndex ( void );

		//--- Overload funtions ---//
		bool operator == (const CSchedule &obj);
		CSchedule &operator = (const CSchedule &obj);
		friend std::ostream& operator << ( std::ostream &stream, const CSchedule &obj );
};
#endif 

