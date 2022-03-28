#ifndef _frmSequencesH
#define _frmSequencesH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
#include <Buttons.hpp>
#include "CSPIN.h"
//---------------------------------------------------------------------------

class TfrmSequences : public TForm
{
__published:	// IDE-managed Components
  TStringGrid *strgrdSequence;
  TLabel *Label1;
  TBitBtn *btnNew;
  TBitBtn *btnAcept;
  TBitBtn *btnCancel;
  TLabel *Label2;
  TGroupBox *GroupBox1;
  TLabel *Label3;
  TEdit *Edit1;
  void __fastcall btnCancelClick(TObject *Sender);
  void __fastcall btnNewClick(TObject *Sender);
  void __fastcall btnAceptClick(TObject *Sender);
  void __fastcall strgrdSequenceKeyPress(TObject *Sender, char &Key);

private:	// User declarations
  int Filtrado (void);

public:		// User declarations
  __fastcall TfrmSequences(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmSequences *frmSequences;
//---------------------------------------------------------------------------
#endif
