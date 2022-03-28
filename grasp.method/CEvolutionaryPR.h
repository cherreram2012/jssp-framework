#pragma once
#ifndef CEvolutionary_PR_H
#define CEvolutionary_PR_H

#include "CEliteSet.h"
#include "CPathRelinking.h"

class CEvolutionaryPR 
{
	private:
		CPathRelinking *PRelinking;

	public:
		//--- Constructor and destructor ---//
		 CEvolutionaryPR();
		~CEvolutionaryPR();
};
#endif