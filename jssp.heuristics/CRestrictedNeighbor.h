#pragma once
#ifndef RestrictedNeighbor_H
#define RestrictedNeighbor_H

#include <vector>
#include "CSetOperator.h"
#include "../jssp.model/CSchedule.h"
#include "../jssp.model/CPieceCollection.h"
#include "../jssp.model/CMachineCollection.h"

class RestrictedNeighbor 
{
	private:
		
		CSchedule Sprime;
		CSchedule Sinitial;
		CSchedule Sguiding; // Use only for debug actions.
		CGraphCAPP *Graph;
		CPieceCollection *CollectionJ;
		CMachineCollection *CollectionM;
		map<int, vector<CSetOperator::DUPLEX_ATOM>> SymDiff;

	private:
		void CheckDataConsistency		( void );
		void ForwardFixInviability  ( CSchedule *S, int m_except, vector<CSetOperator::DUPLEX_ATOM> &vdiff );
		void BackwardFixInviability ( CSchedule *S, int m_except, vector<CSetOperator::DUPLEX_ATOM> &vdiff );
		CSchedule ReplaceMachineSet ( const CSchedule &S, int m_replace, vector<CSetOperator::DUPLEX_ATOM> &vdiff );

	public:
		//--- Constructor and destructor ---//
		 RestrictedNeighbor ( void );
		~RestrictedNeighbor ( void );

		//---  ---//
		CSchedule Argmin ( float *fo );
		int				 Moves ( const CSchedule &Si, const CSchedule &Sg );

		//---  ---//
		void UserData ( CPieceCollection *cJ, CMachineCollection *cM );
};

#endif