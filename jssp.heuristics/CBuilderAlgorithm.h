#pragma once
#ifndef CBuilderAlgorithm_H
#define CBuilderAlgorithm_H

#include "../jssp.model/CSchedule.h"

//------------------------------------------------------------------------------
//	ClassName: CBuilderAlgorithm
//
//  Description: Abstract base class for constructive algorithms of Scheduling.
//
//  Revision: 09/06/2019 
//------------------------------------------------------------------------------
class CBuilderAlgorithm
{
	protected:
		virtual CSchedule GenerateSchedule( void ) = 0;
};

#endif