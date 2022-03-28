#pragma once
#ifndef CGrasp_PR_H
#define CGrasp_PR_H

#include "CEliteSet.h"
#include "CPathRelinking.h"
#include "CGraspAbstract.h"

//------------------------------------------------------------------------------
//	ClassName: CGrasp_PR
//
//  Description: Represent an implementation for a GRASP with Path Relinking.
//
//  Revision: 16/05/2019 
//------------------------------------------------------------------------------
class CGrasp_PR : public CGraspAbstract
{
	protected:
		const unsigned int default_elite_size = 15;

	protected:
		CEliteSet *E;
		unsigned int elite_size;
		CPathRelinking *PRelinking;

	public:
		//--- Constructor and destructor ---//
		CGrasp_PR(CLocalSearchStrategy *locals, CNeighborhoobStrategy *neighbor, CTerminator *term, CPathRelinking *prelinking, float alfa);
		virtual ~CGrasp_PR(void);

		virtual CSchedule Execute(float *fitness = nullptr);

		//--- General purpose functions ---//
		void EliteSize (unsigned int size);
		virtual void UserData(CPieceCollection *cJ, CMachineCollection *cM);
};

#endif