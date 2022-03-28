#include "CGraspAbstract.h"

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
void CGraspAbstract::CheckDataConsistency()
{
	if (!GT || !CollectionJ || !CollectionM || !Dataset || !Graph)
		throw std::exception("CGraspAbstract::Execute(): before execute grasp procedure, you must define { a) collection pieces, b) collection machines } objects.");
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
CGraspAbstract::CGraspAbstract(CLocalSearchStrategy *locals, CNeighborhoobStrategy *neighbor, CTerminator *term, float alfa)
{
	if (!locals) throw std::exception("CGraspAbstract::CGraspSimple(): parameter <locals> may not be null.");
	if (!neighbor) throw std::exception("CGraspAbstract::CGraspSimple(): parameter <neighbor> may not be null.");

	GT = nullptr;
	Graph = nullptr;
	Dataset = nullptr;
	CollectionJ = nullptr;
	CollectionM = nullptr;

	iSeed = 0;
	alfa = fabs(alfa);
	if (alfa > 1.0f) alfa = 1.0f;
	fAlfa = alfa;
	Terminator = term;
	LocalSearch = locals;
	Neighborhood = neighbor;
	LocalSearch->NeighborStrategy(neighbor);
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
CGraspAbstract::~CGraspAbstract(void)
{
	if (GT) delete GT;
	if (Graph) delete Graph;
	if (Dataset) delete Dataset;
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
void CGraspAbstract::SetSeed(int seed)
{
	iSeed = seed;
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
void CGraspAbstract::UserData(CPieceCollection *cJ, CMachineCollection *cM)
{
	if (!cJ) throw std::exception("CGraspAbstract::UserData(): parameter <cJ> may not be null.");
	if (!cM) throw std::exception("CGraspAbstract::UserData(): parameter <cM> may not be null.");

	CollectionJ = cJ;
	CollectionM = cM;
	LocalSearch->UserData(CollectionJ, CollectionM);

	if (Dataset) delete Dataset;
	Dataset = new CDataSet(CollectionJ->GetPieceCount());
	CollectionJ->FillDataSet(Dataset, *CollectionM);

	if (GT) delete GT;
	GT = new CGTAlgorithmAlfaRCL(*Dataset, fAlfa);

	if (Graph) delete Graph;
	Graph = new CGraphCAPP(cJ->GetPieceCount(), cM->GetMachineCount(), 0, false);
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
CStatistics CGraspAbstract::Statistics(void) const
{
	return Stats;
}