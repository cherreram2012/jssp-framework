#include <vcl.h>
#pragma hdrstop

#include "_frmSequences.h"
#include "_frmMDIMain.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "CSPIN"
#pragma resource "*.dfm"

/*** ***/
/*int TfrmSequences::Filtrado (void)
{
}*/
//---------------------------------------------------------------------------

TfrmSequences *frmSequences;

__fastcall TfrmSequences::TfrmSequences(TComponent* Owner)
  : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TfrmSequences::btnNewClick(TObject *Sender)
{
  static int i = 1;
  strgrdSequence->RowCount = ++i;
  strgrdSequence->SetFocus();
}
//---------------------------------------------------------------------------

void __fastcall TfrmSequences::btnAceptClick(TObject *Sender)
{
  TStrings *StringList = new TStringList();
  AnsiString str;

  // Limpio las entradas del StringGrid
  for (int i = 0; i < strgrdSequence->RowCount; i++)
  {
    str = strgrdSequence->Cells[0][i].Trim();
    if (!str.IsEmpty()) StringList->Add(str.UpperCase());
  }

  // Paso al formulario principal la lista de secuencias
  frmMDIMain->SequenceStringList = StringList;
  frmMDIMain->MeasureCount = StrToInt(Edit1->Text);
}
//---------------------------------------------------------------------------

void __fastcall TfrmSequences::btnCancelClick(TObject *Sender)
{
  Close();
}
//---------------------------------------------------------------------------

void __fastcall TfrmSequences::strgrdSequenceKeyPress(TObject *Sender,
      char &Key)
{
  if(Key == 13) btnNew->Click();
}
//---------------------------------------------------------------------------

