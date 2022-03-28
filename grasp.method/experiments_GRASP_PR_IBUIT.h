#pragma once
#ifndef experiments_etapa6_IBUIT_H
#define experiments_etapa6_IBUIT_H

#include <map>
#include <iostream>
#include "CTerminator.h"
#include "CPathRelinking.h"
#include "../jssp.model/CPieceCollection.h"
#include "../jssp.heuristics/CLocalSearch.h"
#include "../jssp.heuristics/CNeighborhood.h"

#define TO_FILE 1
#define TO_SCREEN 0

void run_Grasp_with_PathRelinking_IBUIT(char *namePR, map<char *, char *> data_files, CLocalSearchStrategy *localsearch, CNeighborhoobStrategy *neighbor, CTerminator *terminator, CPathRelinking *prelinking, float alfa, int seed);

CStatistics test_Grasp_IBUIT(const char *file_instance, CLocalSearchStrategy *localsearch, CNeighborhoobStrategy *neighbor, CTerminator *terminator, CPathRelinking *prelinking, float alfa, int seed);

#endif