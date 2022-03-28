#pragma once
#ifndef CTerminator_H
#define CTerminator_H

#include "CStatistics.h"

//------------------------------------------------------------------------------
//	ClassName: CTerminator
//
//  Description: .
//
//  Revision: 16/05/2019 
//------------------------------------------------------------------------------
class CTerminator {
	public:
		virtual bool isDone (const CStatistics &stats ) = 0;
};

//------------------------------------------------------------------------------
//	ClassName: StopByIteration
//
//  Description: .
//
//  Revision: 16/05/2019 
//------------------------------------------------------------------------------
class StopByIteration : public CTerminator
{
	private:
		unsigned long n_iterations;

	public:
		 StopByIteration ( unsigned long it );
		~StopByIteration ( void );

		virtual bool isDone ( const CStatistics &stats );
};

//------------------------------------------------------------------------------
//	ClassName: StopByTarget
//
//  Description: .
//
//  Revision: 16/05/2019 
//------------------------------------------------------------------------------
class StopByTarget : public CTerminator
{
	private:
		float target;

	public:
		 StopByTarget ( float tgt );
		~StopByTarget ( void );

		virtual bool isDone ( const CStatistics &stats );
};

//------------------------------------------------------------------------------
//	ClassName: StopByTime
//
//  Description: .
//
//  Revision: 16/05/2019 
//------------------------------------------------------------------------------
class StopByTime : public CTerminator 
{
	private:
		double time;

	public:
		 StopByTime ( double t );
		~StopByTime ( void );

		virtual bool isDone ( const CStatistics &stats );
};
#endif