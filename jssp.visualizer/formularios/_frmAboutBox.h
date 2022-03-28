#ifndef _frmAboutBoxH
#define _frmAboutBoxH
//----------------------------------------------------------------------------
#include <vcl\System.hpp>
#include <vcl\Windows.hpp>
#include <vcl\SysUtils.hpp>
#include <vcl\Classes.hpp>
#include <vcl\Graphics.hpp>
#include <vcl\Forms.hpp>
#include <vcl\Controls.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Buttons.hpp>
#include <vcl\ExtCtrls.hpp>
//----------------------------------------------------------------------------

class TfrmAboutBox : public TForm
{
__published:
	TPanel *Panel1;
	TImage *ProgramIcon;
	TLabel *ProductName;
	TLabel *Version;
	TLabel *Copyright;
	TLabel *Comments;
	TButton *OKButton;
  TLabel *Label2;
  TLabel *Label3;
  void __fastcall FormClose(TObject *Sender, TCloseAction &Action);

private:

public:
	virtual __fastcall TfrmAboutBox(TComponent* AOwner);
};
//----------------------------------------------------------------------------
extern PACKAGE TfrmAboutBox *frmAboutBox;
//----------------------------------------------------------------------------
#endif    
