//---------------------------------------------------------------------------

#ifndef _frmEnergyChartH
#define _frmEnergyChartH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "TGDRuler.h"
#include <ExtCtrls.hpp>

#include "CHndFile_PM.h"
#include "CPieceCollection_PM.h"
#include "CSPGraph_PM.h"
#include "class_EnergyChart.h"
#include <Grids.hpp>

#define SPACE_BETWEEN_ENERGY_CHART 30
//---------------------------------------------------------------------------

class TfrmEnergyChart : public TForm
{
__published:	// IDE-managed Components
  void __fastcall FormDestroy(TObject *Sender);
  void __fastcall FormClose(TObject *Sender, TCloseAction &Action);

private:	// User declarations
  int dCount;
  int Intervals;
  TEnergyChart **Diagramas;

public:		// User declarations
  __fastcall TfrmEnergyChart(TComponent* Owner);
  void CreateDiagrams     ( int count );
  void SetMaxValueOfPower ( float value );
  void SetMaxValueTime    ( float value );

  TObserverEnergy *GetHndDiagramByIndex (int index);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmEnergyChart *frmEnergyChart;
//---------------------------------------------------------------------------
#endif
