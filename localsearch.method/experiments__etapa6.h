#pragma once
#ifndef experiments_Advanced_H
#define experiments_Advanced_H

#include <map>
#include <iostream>
#include "CLocalSearchForExperiment.h"
#include "../jssp.model/CPieceCollection.h"
#include "../jssp.model/CMachineCollection.h"
#include "../jssp.heuristics/CNeighborhood.h"
#include "../chm.utilities/PrecisionClock.h"

#define TO_FILE 1
#define TO_SCREEN 0

void runExperiment_CmaxVsStep_IBUIT__etapa6(const char *file_instance, enum SolveConflict solve, int seed);

float testLocalSearch_CmaxVsStep__etapa6(ofstream &output, CPieceCollection *cJ, CMachineCollection *cM, CNeighborhoobStrategy *neighborhood, const CSchedule &entry_point, enum LocalSearchType type);
#endif