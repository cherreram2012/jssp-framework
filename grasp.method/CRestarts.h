#pragma once
#ifndef CRestarter_H
#define CRestarter_H

#include "CStatistics.h"

//------------------------------------------------------------------------------
//	ClassName: CTerminator
//
//  Description: .
//
//  Revision: 14/06/2019 
//------------------------------------------------------------------------------
class CRestartStrategy {
	protected:
		CStatistics lastMark;
		CStatistics improvMark;

	public:
		void MarkAt ( const CStatistics &stats, bool improv = false );

		virtual bool isDone ( const CStatistics &current ) = 0;
};

//------------------------------------------------------------------------------
//	ClassName: CTerminator
//
//  Description: .
//
//  Revision: 14/06/2019 
//------------------------------------------------------------------------------

class CRestartByTime : public CRestartStrategy {
	private:
		double time;

	public:
		//--- Constructor and destructor ---//
		 CRestartByTime ( double t );
		~CRestartByTime ( void );

		virtual bool isDone ( const CStatistics &stats );
};

//------------------------------------------------------------------------------
//	ClassName: CTerminator
//
//  Description: .
//
//  Revision: 14/06/2019 
//------------------------------------------------------------------------------

class CRestartByIterations : public CRestartStrategy {
	private:
		unsigned int iterations;

	public:
		//--- Constructor and destructor ---//
		 CRestartByIterations ( unsigned int it );
		~CRestartByIterations ( void);

		virtual bool isDone(const CStatistics &stats);
};

//------------------------------------------------------------------------------
//	ClassName: CTerminator
//
//  Description: .
//
//  Revision: 14/06/2019 
//------------------------------------------------------------------------------

class CRestartByNoImprove : public CRestartStrategy {
	private:
		unsigned int k_interations;

	public:
		//--- Constructor and destructor ---//
		CRestartByNoImprove  ( unsigned int k_it );
		~CRestartByNoImprove ( void );

		virtual bool isDone ( const CStatistics &stats );
};
#endif