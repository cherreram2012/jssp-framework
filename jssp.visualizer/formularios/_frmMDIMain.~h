#ifndef _frmMDIMainH
#define _frmMDIMainH
//---------------------------------------------------------------------------

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>
#include <jpeg.hpp>
#include <ImgList.hpp>
#include <ComCtrls.hpp>
#include <ToolWin.hpp>
#include <Buttons.hpp>
#include <ActnList.hpp>
#include <Dialogs.hpp>
#include <Graphics.hpp>
#include <Grids.hpp>

#include "CHndFile_PM.h"
#include "CSPGraph_PM.h"
#include "CMachineGroup_PM.h"
#include "CPieceCollection_PM.h"
#include "CMachineCollection_PM.h"
#include "CWorkCenterCollection_PM.h"
#include "class_RegSequence.h"

#define WIDTH_PANEL 200
//---------------------------------------------------------------------------

int CheckDataFile ( AnsiString filename );

class TfrmMDIMain : public TForm
{
__published:	// IDE-managed Components
  TMainMenu *MainMenu;
  TMenuItem *mnuFile;
  TMenuItem *mnuHelp;
  TMenuItem *opnHelpAbout;
  TMenuItem *opnHelpContents;
  TMenuItem *opnOpenProject;
  TMenuItem *mnuView;
  TMenuItem *opnExit;
  TMenuItem *N4;
  TMenuItem *opnViewToolsBar;
  TMenuItem *opnViewStatusBar;
  TToolBar *ToolBar;
  TToolButton *ToolButton2;
  TSpeedButton *spdbtnOpenProject;
  TToolButton *ToolButton4;
  TStatusBar *StatusBar;
  TOpenDialog *OpenDialogDataFile;
  TSpeedButton *spdbtnGanttDiagram;
  TMenuItem *opnResult;
  TMenuItem *opnViewGDiagram;
  TTimer *tmrShowDC;
  TTimer *tmrHideDC;
  TPanel *pnlSimbology;
  TPanel *Panel2;
  TLabel *Label2;
  TStringGrid *strgrdSimbology;
  TTimer *tmrShowSU;
  TTimer *tmrHideSU;
  TPanel *pnlSlahs;
  TPanel *pnlLoadedData;
  TTreeView *TreeView;
  TPanel *pnlStatic;
  TLabel *Label1;
  TSpeedButton *spdbtnFijador1;
  TImage *imgLashDC;
  TImage *imgLashSU;
  TSpeedButton *spdbtnFijador2;
  TMenuItem *N1;
  TMenuItem *opnApplyRescheduling;
  TMenuItem *N2;
  TMenuItem *submnuObjetive;
  TMenuItem *opnCmax;
  TMenuItem *opnFmax;
  TMenuItem *opnIdle;
  TMenuItem *opnEmax;
  TMenuItem *opnTmax;
  TMenuItem *opnNE;
  TMenuItem *opnNT;
  TMenuItem *opnEn;
  TMenuItem *N3;
  TMenuItem *N5;
  TMenuItem *opnAll;
  TMenuItem *opnViewMark;
  TMenuItem *opnApplyRulerFIFO;
  TMenuItem *N6;
  TMenuItem *sbmnuOptionsScheduling;
  TMenuItem *opnNoRecirc;
  void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
  void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
  void __fastcall FormCreate(TObject *Sender);
  void __fastcall opnOpenProjectClick(TObject *Sender);
  void __fastcall opnExitClick(TObject *Sender);
  void __fastcall opnViewGDiagramClick(TObject *Sender);
  void __fastcall opnViewToolsBarClick(TObject *Sender);
  void __fastcall opnViewStatusBarClick(TObject *Sender);
  void __fastcall opnHelpAboutClick(TObject *Sender);
  void __fastcall spdbtnFijador1Click(TObject *Sender);
  void __fastcall tmrShowDCTimer(TObject *Sender);
  void __fastcall tmrHideDCTimer(TObject *Sender);
  void __fastcall FormMouseMove(TObject *Sender, TShiftState Shift, int X,
          int Y);
  void __fastcall tmrShowSUTimer(TObject *Sender);
  void __fastcall tmrHideSUTimer(TObject *Sender);
  void __fastcall strgrdSimbologyDrawCell(TObject *Sender, int ACol,
          int ARow, TRect &Rect, TGridDrawState State);
  void __fastcall imgLashDCClick(TObject *Sender);
  void __fastcall imgLashSUClick(TObject *Sender);
  void __fastcall opnAllClick(TObject *Sender);
  void __fastcall opnCmaxClick(TObject *Sender);
  void __fastcall opnViewMarkClick(TObject *Sender);
  void __fastcall opnApplyRulerFIFOClick(TObject *Sender);
  void __fastcall opnNoRecircClick(TObject *Sender);

private:	// User declarations
  int Npiece, Nmachine;
  STRUCT_RECIRCULATION Recirc;
  unsigned *sequence, *ListID;
  CHndFile_PM hdFile;
  REGISTER_ITEM *RegItem;
  CMachineCollection_PM *MachineList;
  CPieceCollection_PM *CollectionP;
  CWorkCenterCollection WorkCenters;
  enum ProblemType Problem_Type;

private:
  void ConvertStringToSequence (AnsiString str, unsigned *Sequence);
  int GetItemCountInSequence   (AnsiString seq);
  //bool isValidGenome  (const unsigned *genome, int length, int job);
  bool isValidSequence  (const unsigned *sequence, int job, int machine);
  bool isValidOperationCount (const unsigned *sequence, int job, int machine);

public: // User declarations
  AnsiString strDataFile;
  int width_DC, width_SU;
  int MeasureCount;
  TStrings *SequenceStringList;

public:
  __fastcall TfrmMDIMain(TComponent* Owner);
  void ShowLoadedDataInPanel ( void );
  void ShowSimbologyInPanel  ( void );
  void LoadData              ( void );
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmMDIMain *frmMDIMain;
//---------------------------------------------------------------------------

/*** Elimina los ceros despues del punto en una cadena ***/
char *KillCero (char *Str)
{
  char *Ptr = Str;
  register unsigned Len;

  if (strchr (Str, '.'))
  {
	  Len = strlen (Str);
	  while (--Len)
    {
      if (*(Ptr + Len) == '0') // Ptr[Len]==
          *(Ptr + Len) = '\0';
      else if (*(Ptr + Len) == '.')
      {
			  *(Ptr + Len) = '\0';
				break;
      }
		  else break;
    }
  }

  return Ptr;
}
#endif
