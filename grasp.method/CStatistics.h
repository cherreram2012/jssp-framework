#pragma once
#ifndef CStatistics_H
#define CStatistics_H

#include "../jssp.model/CSchedule.h"

//------------------------------------------------------------------------------
//	ClassName: CStatistics
//
//  Description: .
//
//  Revision: 16/05/2019 
//------------------------------------------------------------------------------
class CStatistics
{
	private:
		float best_score;
		float worst_score;
		float sum_best_scores;
		float sum_all_scores;
		double elapsed_time;
		unsigned long n_restarts;
		unsigned long n_improves;
		unsigned long n_iterations;
		CSchedule best_solution;
	
	public:
		//--- Constructor and destructor ---//
		 CStatistics(void) { best_score = 0.0f; worst_score = 0.0f; sum_best_scores = 0.0f;  sum_all_scores = 0.0f; elapsed_time = 0.0f; n_iterations = 0; n_improves = 0; n_restarts = 0; }
		 ~CStatistics(void) {}

		//--- Get Functions ---//
		float				  getBestScore				( void ) const { return best_score; }
		float				  getWorstScore				( void ) const { return worst_score;	}
		float				  getAverageScore			( void ) const { return sum_all_scores / n_iterations; }
		float				  getAverageBestScore ( void ) const { return sum_best_scores / n_improves;	}
		double				getElapsedTime			( void ) const { return elapsed_time; }
		unsigned long getNoImproves				( void ) const { return n_improves; }
		unsigned long getNoRestarts				( void ) const { return n_restarts; }
		unsigned long getIterations				( void ) const { return n_iterations; }
		CSchedule		  getBestSolution			( void ) const { return best_solution; }

		//--- General functions ---//
		void Initialize(void) { best_score = 0.0f; worst_score = 0.0f; sum_best_scores = 0.0f;  sum_all_scores = 0.0f; elapsed_time = 0.0f; n_iterations = 0; n_improves = 0; n_restarts = 0; }

		friend class CGrasp_PR;
		friend class CGraph_EvPR;
		friend class CGraspBasic;
		friend class CGraph_PR_Restarts;
};
#endif