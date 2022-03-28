#pragma once
#ifndef CSPGraphJSS_H
#define CSPGraphJSS_H

#include "CSchedule.h"
#include "CGraphBase.h"

//------------------------------------------------------------------------------
//	ClassName  : CSPGraphJSS
//
//  Description: Representa un grafo para la variante JSS (Job Shop Scheduling)
//
//  Revision   : 09/06/2019 
//------------------------------------------------------------------------------
class CGraphJSS : public CGraphBase {
	private:
		typedef struct {
			     int machine;
			   float time;
			unsigned id;
		} NODE;

	public:
		CGraphJSS();
};
#endif