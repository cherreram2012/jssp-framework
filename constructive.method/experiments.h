#pragma once
#ifndef experiments_H
#define experiments_H

#include <map>
#include "../jssp.model/CPieceCollection.h"
#include "../jssp.model/CMachineCollection.h"

#define TO_FILE 0
#define TO_SCREEN 1
#define MAXIMIZE_Z 0

void runExperiment_GreedySPT_bestone_value		( map<char *, char *> data_files, int n_run );
void runExperiment_GreedySEP_bestone_value		( map<char *, char *> data_files, int n_run );
void runExperiment_GreedyECT_bestone_value		( map<char *, char *> data_files, int n_run );
void runExperiment_GTAlgorithm_bestone_value  ( map<char *, char *> data_files, int n_run );
void runExperiment_SemiSEP_bestone_value			( map<char *, char *> data_files, int n_run );
void runExperiment_GTAlfaRCL_bestone_value		( map<char *, char *> data_files, int n_run, float alfa, enum SolveConflict solve );
void runExperiment_Ocurrences_per_algorithm   ( const char *file_instance, int n_run, float optimal_value );
void runExperiment_Time_for_value							( const char *file_instance, double seconds, enum SolveConflict solver );
void runExperiment_Iterations_for_value				( const char *file_instance, long iterations, enum SolveConflict solver );

void test_GTAlfaRCL_bestone_value						  ( const char *file_instance, int n_run, float alfa, enum SolveConflict solve, CSchedule *best_schedule, float *best_cmax );
void test_GTAlgorithm_bestone_value						( const char *file_instance, int n_run, enum SolveConflict solve, CSchedule *best_schedule, float *best_cmax );
void test_Randomized_ocurrences								( CPieceCollection *cJ, CMachineCollection *cM, int n_run, const char *outname = nullptr );
void test_GTAlgorithm_ocurrences							( CPieceCollection *cJ, CMachineCollection *cM, int n_run, enum SolveConflict solver, const char *outname = nullptr );
void test_GTAlfaRCL_ocurrences								( CPieceCollection *cJ, CMachineCollection *cM, int n_run, float alfa, enum SolveConflict solver, const char *outname = nullptr );
#endif
