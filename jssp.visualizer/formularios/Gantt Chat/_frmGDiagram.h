
#ifndef _frmGDiagramVisorH
#define _frmGDiagramVisorH
//---------------------------------------------------------------------------

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>

#include "class_TGanttDiagram.h"
#include <ExtCtrls.hpp>
#include <Dialogs.hpp>

#define SPACE_BETWEEN_GANTT_CHART 50
//---------------------------------------------------------------------------

class TfrmGanttDiagram : public TForm
{
__published:	// IDE-managed Components
  TColorDialog *ColorDialog1;
  void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
  void __fastcall FormMouseMove(TObject *Sender, TShiftState Shift, int X,
          int Y);
  void __fastcall FormDestroy(TObject *Sender);

private:	// User declarations
  int dCount;
  TGanttDiagram **Diagramas;

public:		// User declarations
  __fastcall TfrmGanttDiagram(TComponent* Owner);
  void CreateDiagrams        ( int count );
  void SetMaxValueOfAllRuler ( float maxValue );
  void SetMachineLabels      ( const CMachineCollection_PM *M, const CWorkCenterCollection *WC );
  void SetRegisterItems      ( REGISTER_ITEM *rItem, int count );
  void ShowObjetive          ( bool cmax, bool fmax, bool idle, bool emax, bool tmax, bool ne, bool nt, bool energy);
  void ShowMeasureMark       ( bool );

  TObserverGantt *GetHndDiagramByIndex (int index);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmGanttDiagram *frmGanttDiagram;
//---------------------------------------------------------------------------
#endif

