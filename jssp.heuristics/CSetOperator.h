#pragma once
#ifndef CSetOperator_H
#define CSetOperator_H

#include <map>
#include <vector>
#include "../jssp.model/CSchedule.h"

using namespace std;

class CSetOperator
{
	public:
		typedef struct {
			int index;
			unsigned int idS1;
		} ATOM;

		typedef struct {
			int index;
			unsigned int idS1;
			unsigned int idS2;
		} DUPLEX_ATOM;

	public:
		static map<int, vector<ATOM>>				 Diff		( const CSchedule &S1, const CSchedule &S2 );
		static map<int, vector<ATOM>>				 Inters	( const CSchedule &S1, const CSchedule &S2 );
		static map<int, vector<DUPLEX_ATOM>> Union	( const CSchedule &S1, const CSchedule &S2 );
		static map<int, vector<DUPLEX_ATOM>> SymmetricDiff ( const CSchedule &S1, const CSchedule &S2 );
};
#endif