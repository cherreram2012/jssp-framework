#include <vcl.h>
#pragma hdrstop

#include "_frmAboutBox.h"
//---------------------------------------------------------------------
#pragma resource "*.dfm"

TfrmAboutBox *frmAboutBox;
 
__fastcall TfrmAboutBox::TfrmAboutBox(TComponent* AOwner)
	: TForm(AOwner)
{
}
//---------------------------------------------------------------------

void __fastcall TfrmAboutBox::FormClose(TObject *Sender,
      TCloseAction &Action)
{
  Action = caFree;
}
//---------------------------------------------------------------------------

