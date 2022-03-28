#pragma once
#ifndef CGrasp_PR_Restarts_H
#define CGrasp_PR_Restarts_H

#include "CGrasp_PR.h"
#include "CRestarts.h"

//------------------------------------------------------------------------------
//	ClassName: CGraph_PR_Restarts
//
//  Description: Represent an implementation for a GRASP + Path-Relinking + Restarts.
//
//  Revision: 13/06/2019 
//------------------------------------------------------------------------------
class CGraph_PR_Restarts : public CGrasp_PR
{
	private:
		CRestartStrategy *Restarts;

	public:
		//--- Constructor and destructor ---//
		CGraph_PR_Restarts ( CLocalSearchStrategy *locals, CNeighborhoobStrategy *neighbor, CTerminator *term, CPathRelinking *prelinking, CRestartStrategy *restarts, float alfa );
		virtual ~CGraph_PR_Restarts ( void );

		virtual CSchedule Execute ( float *fitness = nullptr );
};

#endif