#ifndef gaModuleH
#define gaModuleH

#include <vector>

#include "../jssp.model/CGraphCAPP.h"
#include "../jssp.model/CPieceCollection.h"
#include "../jssp.model/CMachineCollection.h"

// GAlib Header file
#include <ga/ga.h>

//--- For CBuilder Compiler ---//
//#include <ga/GASimpleGA.h>	    
//#include <ga/GA1DArrayGenome.h>

//#define SWAP(a, b) {unsigned tmp = a; a = b; b = tmp;}

#define REPETITION_IN_GENE 1 

enum CROSS_METHOD {
  ONE_POINT = 0, TWO_POINT, THREE_POINT, PMX, ZIG_ZAG, PARTIAL_MATCH
};

enum SELECTION_METHOD {
  ROULETTE_WHEEL = 0, TOURNAMENT, RANK, UNIFORM, DETERMINISTIC_SAMPLING, STOCHASTIC_SAMPLING
};

//---------------------------------------------------------------------------
// Declaracion de una lista simplemente enlazada para almacenar genes
//--------------------------------------------------------------------------- 
struct GeneList {
	unsigned Gene;
	struct   GeneList *Next;
};
typedef struct GeneList GENE_LIST;
typedef GENE_LIST* GENE_LIST_PTR;

//---------------------------------------------------------------------------
//	Declaracion de Funciones de Proposito General
//---------------------------------------------------------------------------
namespace GAModule {
	void					SetGraph									( CGraphCAPP *Obj );
	void					SetPieceCollection				( CPieceCollection *Obj );
	void					SetMachineCollection			( CMachineCollection *Obj );
	void					SetPriorityList						( PRIORITY_LIST_PTR list );
	void					SetBound									( float value );
	unsigned long GetPerfomanceRescheduling ( void );
	void					ClearPriorityList					( void );
}

//---------------------------------------------------------------------------
//	Declaracion de Funciones comunes para los dos tipos de problemas
//---------------------------------------------------------------------------
void GetGenes	( const GA1DArrayGenome <unsigned> &g, unsigned *Array );

typedef int (*n_CrossOperatorFunc) (const GAGenome &, const GAGenome &, GAGenome *, GAGenome *);

typedef std::vector < GA1DArrayGenome <unsigned> > GenomesVector;

//--------------------------------------------------------------------------------------------------------------------//
//																																																										//
//			Declaracion de funciones del GA	para problemas FlowShop.																											//
//																																																										//
//--------------------------------------------------------------------------------------------------------------------//
int		n_SearchGene          ( const unsigned *A_Genome, int length, unsigned gene, int init = -1, int end = -1 );
int		n_SearchGene          ( const GA1DArrayGenome <unsigned> &genome, unsigned gene, int init = -1, int end = -1 );

void	n_RandomGenome			  ( unsigned *A_Genome, int Length, STRUCT_PROBLEM_DATA Inst );
void	n_Initializer				  ( GAGenome &g );
float	n_Fitness						  ( GAGenome &genome );
int		n_OnePointCrossover	  ( const GAGenome& p1, const GAGenome& p2, GAGenome* c1, GAGenome* c2 );
int		n_TwoPointCrossover	  ( const GAGenome& p1, const GAGenome& p2, GAGenome* c1, GAGenome* c2 );
int		n_ThreePointCrossover ( const GAGenome& p1, const GAGenome& p2, GAGenome* c1, GAGenome* c2 );
int		n_PMXCrossover			  ( const GAGenome& p1, const GAGenome& p2, GAGenome* c1, GAGenome* c2 );
int		n_ZigZagCrossover		  ( const GAGenome& p1, const GAGenome& p2, GAGenome* c1, GAGenome* c2 );
bool	fGATerminateByBound	  ( GAGeneticAlgorithm &ga );

bool	n_isValidGenome       ( const GA1DArrayGenome <unsigned> &genome );
bool	n_isValidSequence     ( const GA1DArrayGenome <unsigned> &genome, unsigned *IG );
void	n_FixGenome           ( GA1DArrayGenome <unsigned> &genome, unsigned InvalidGene );
bool	fEqualGenome          ( const GA1DArrayGenome <unsigned> &G1, const GA1DArrayGenome <unsigned> &G2 );

const n_CrossOperatorFunc n_CrossOperator[6] = {
  n_OnePointCrossover,
  n_TwoPointCrossover,
  n_ThreePointCrossover,
  n_PMXCrossover,  
  n_ZigZagCrossover,
	GA1DArrayGenome<unsigned>::PartialMatchCrossover,
};

//--------------------------------------------------------------------------------------------------------------------//
//																																																										//
//											Declaracion de funciones del GA	para problems JobShop.																				//
//																																																										//
//--------------------------------------------------------------------------------------------------------------------// 
void	nxm_RandomGenomeCR						( unsigned *A_Genome, STRUCT_PROBLEM_DATA Inst );
void	nxm_RandomGenomeSR						( unsigned *A_Genome, STRUCT_PROBLEM_DATA Inst );
void	nxm_Initializer								(	GAGenome &g );
float	nxm_Fitness										( GAGenome &genome );
int		nxm_OnePointCrossoverGene			( const GAGenome& p1, const GAGenome& p2, GAGenome* c1, GAGenome* c2 );
int		nxm_OnePointCrossoverAllele		( const GAGenome& p1, const GAGenome& p2, GAGenome* c1, GAGenome* c2 );
int		nxm_TwoPointCrossoverGene			( const GAGenome& p1, const GAGenome& p2, GAGenome* c1, GAGenome* c2 );
int		nxm_TwoPointCrossoverAllele		( const GAGenome& p1, const GAGenome& p2, GAGenome* c1, GAGenome* c2 );
int		nxm_ThreePointCrossoverGene		( const GAGenome& p1, const GAGenome& p2, GAGenome* c1, GAGenome* c2 );
int		nxm_ThreePointCrossoverAllele ( const GAGenome& p1, const GAGenome& p2, GAGenome* c1, GAGenome* c2 );
int		nxm_SwapMutatorGene						( GAGenome &g, float pmut );
bool	nxm_GATerminateByBound				( GAGeneticAlgorithm &ga );

bool	nxm_isValidGenome							( const GA1DArrayGenome <unsigned> &genome );
bool	nxm_isValidSequenceCR					( const GA1DArrayGenome <unsigned> &genome, unsigned *JixMoreOp, unsigned *JixLessOP );
bool	nxm_isValidSequenceSR					( const GA1DArrayGenome <unsigned> &genome, int *IG, unsigned *JixMoreOp, unsigned *JixLessOP );
void	nxm_FixGenomeCR								( GA1DArrayGenome <unsigned> &genome, unsigned JixMoreOp, unsigned JixLessOP );
void	nxm_FixGenomeSR								( GA1DArrayGenome <unsigned> &genome, unsigned JixMoreOp, unsigned JixLessOP );

void Print_NxM_Cromosome1 (const GA1DArrayGenome <unsigned> &g);
			
#endif