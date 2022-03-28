#pragma once
#ifndef GraspBasic_H
#define GraspBasic_H

#include "CGraspAbstract.h"

//------------------------------------------------------------------------------
//	ClassName: GraspBasic
//
//  Description: Represent an implementation for a basic GRASP metaheuristic.
//
//  Revision: 16/05/2019 
//------------------------------------------------------------------------------
class CGraspBasic : public CGraspAbstract
{
	public:
		//--- Constructor and destructor ---//
		CGraspBasic( CLocalSearchStrategy *locals, CNeighborhoobStrategy *neighbor, CTerminator *term, float alfa );
		virtual ~CGraspBasic( void );
		
		CSchedule Execute	( float *fitness = nullptr );
};

#endif