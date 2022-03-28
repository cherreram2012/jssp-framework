#pragma once
#ifndef experiments_etapa4_H
#define experiments_etapa4_H

#include <map>
#include <iostream>
#include "CTerminator.h"
#include "../jssp.heuristics/CLocalSearch.h"
#include "../jssp.heuristics/CNeighborhood.h"

#define TO_FILE 1
#define TO_SCREEN 0

void runReference_Alfa_Grasp	  ( map<char *, char *> data_files, CLocalSearchStrategy *localsearch, CNeighborhoobStrategy *neighbor, CTerminator *terminator, float alfa, int seed );
void runGrasp_for_InstanceTeste ( map<char *, char *> data_files, CLocalSearchStrategy *localsearch, CNeighborhoobStrategy *neighbor, CTerminator *terminator, float alfa, int seed);
void runAdjust_Alfa_Grasp			  ( const char *file_instance, CLocalSearchStrategy *localsearch, CNeighborhoobStrategy *neighbor, CTerminator *terminator, int seed );
void runDistributionTime				( const char *file_instance, CLocalSearchStrategy *localsearch, CNeighborhoobStrategy *neighbor, CTerminator *terminator, int n_trials);

CStatistics test_Grasp ( const char *file_instance, CLocalSearchStrategy *localsearch, CNeighborhoobStrategy *neighbor, CTerminator *terminator, float alfa, int seed );
#endif