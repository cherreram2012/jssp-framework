#pragma once
#ifndef CGrasp_EvPR_H
#define CGrasp_EvPR_H

#include "CGrasp_PR.h"
#include "CEliteSet.h"
#include "CPathRelinking.h"

//------------------------------------------------------------------------------
//	ClassName: CGraph_EvPR
//
//  Description: Represent an implementation for a GRASP + Evolutionary Path-Relinking .
//
//  Revision: 13/06/2019 
//------------------------------------------------------------------------------
class CGraph_EvPR : public CGrasp_PR
{
	public:
		//--- Constructor and destructor ---//
		CGraph_EvPR(CLocalSearchStrategy *locals, CNeighborhoobStrategy *neighbor, CTerminator *term, CPathRelinking *prelinking, float alfa);
		virtual ~CGraph_EvPR(void);

		virtual CSchedule Execute(float *fitness = nullptr);
};

#endif