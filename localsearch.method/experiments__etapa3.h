#pragma once
#ifndef experiments__etapa3_H
#define experiments__etapa3_H

#include <map>
#include <iostream>
#include "CLocalSearchForExperiment.h"
#include "../jssp.model/CPieceCollection.h"
#include "../jssp.model/CMachineCollection.h"
#include "../jssp.heuristics/CNeighborhood.h"
#include "../chm.utilities/PrecisionClock.h"

#define TO_FILE 1
#define TO_SCREEN 0
#define MAXIMIZE_Z 0

void runExperiment_LocalSearch_value__etapa3					  ( map<char *, char *> data_files, int seed );
void runExperiment_CmaxVsStep_FirstImproving__etapa3		( const char *file_instance, enum SolveConflict solve, int seed );
void runExperiment_CmaxVsStep_BestImproving__etapa3			( const char *file_instance, enum SolveConflict solve, int seed );

float testLocalSearch_CmaxVsStep__etapa3								( ofstream &output, CPieceCollection *cJ, CMachineCollection *cM, CNeighborhoobStrategy *neighborhood, const CSchedule &entry_point, enum LocalSearchType type );
void testTwoPhases_Constructive_LocalSearch__etapa3			( map<char *, char *> data_files, ofstream &output, enum SolveConflict solve, enum LocalSearchType localsearch_type, enum NeighborhoobStrategy neigh_type, int seed );
void phaseContructive_GTAlgorithm_bestone_value__etapa3 ( const char *file_instance, int n_run, enum SolveConflict solve, CSchedule *best_schedule, float *best_cmax, double *utime, double *time, int seed );
void phaseLocalSearch_bestone_value__etapa3							( const char *file_instance, const CSchedule entry_point, enum LocalSearchType localsearch_type, enum NeighborhoobStrategy neigh_type, CSchedule *best_schedule, float *best_cmax, double *utime, double *time);
#endif