#ifndef class_TGanttDiagramH
#define class_TGanttDiagramH

#include <vcl.h>
#include "TGDRuler.h"
#include "CSPGraph_PM.h"
#include "class_RegSequence.h"
#include "CMachineCollection_PM.h"
#include "CWorkCenterCollection_PM.h"

#define FRAME_LABELS_WIDTH     100
#define FRAME_GRAPH_WIDTH      880
#define RULER_HEIGHT            25
#define MACHINE_LINE_HEIGHT     28  // El alto menor tiene que ser 15 el mayor 50
#define SOBREMEDIDA              6
#define WORKCENTER_LINE_HEIGHT  20


//------------------------------------------------------------------------------
//	ClassName  : TGanttDiagram
//
//  Description: Representa un grafico de Gantt. Tiene una regla para
//               para las escala del grafico, una zona para las etiquetas
//               de las tareas o recursos que van a graficar y el area
//               del grafico.
// 
//  Revision   : 13/06/2014 
//------------------------------------------------------------------------------

class TGanttDiagram: public TObserverGantt {
  private:
    int             iLeft;
    int             iTop;
    int             iRowCount;
    int             iItemCount;
    int             iHeight;
    int             iMeasureMarkCount;
    int             iFrameGraphWidth;
    int            *TopMachineMask;
    TForm          *hndForm;
    TImage         *imgGraph;
    TImage         *imgLabels;
    TGDRuler       *Ruler;
    TLabel         *lblGraphNumber;
    TImage         *imgBackground;
    TLabel         *lblScheduleType;
    TLabel         *lblStaticCmax;
    TLabel         *lblStaticFmax;
    //TLabel       *lblStaticLmax;
    TLabel         *lblStaticIdle;
    TLabel         *lblStaticEmax;
    TLabel         *lblStaticTmax;
    TLabel         *lblStaticNt;
    TLabel         *lblStaticNe;
    TLabel         *lblStaticEnergy;
    TLabel         *lblCmaxValue;
    TLabel         *lblFmaxValue;
    //TLabel       *lblLmaxValue;
    TLabel         *lblIdleValue;
    TLabel         *lblEmaxValue;
    TLabel         *lblTmaxValue;
    TLabel         *lblEnergyValue;
    TLabel         *lblNeValue;
    TLabel         *lblNtValue;
    TLabel         *lblTitleRuler;
    TShape         **MeasureMark;
    REGISTER_ITEM  *RegItems;
    const CMachineCollection_PM *MachineList;
    const CWorkCenterCollection *WCList;

  private:  //  Utility Functions
    int  TextHeight           ( TCanvas *C, AnsiString Text );
    int  TimeToPixel          ( float time );
    void DrawFrameLabels      ( void );
    void DrawFrameGraph       ( void );
    void DrawFrameWorkCenter  ( void );
    void CreateTopMachineMask ( void );

  protected:
    void __fastcall myMouseMove (TObject *Sender, TShiftState Shift, int X, int Y);

  public:
     TGanttDiagram ( TForm *form = NULL, int form_width = 0 );   /* Constructor */
    ~TGanttDiagram ();                       /* Destructor  */

    /* Set Functions */
    void SetLeft                       ( int left );
    void SetTop                        ( int top );
    void SetMaxRulerValue              ( float value );
    void SetScaleDivisions             ( int divisions );
    void SetMachineAndWorkCenterLabels ( const CMachineCollection_PM *mList, const CWorkCenterCollection *wcList );
    void SetRegisterItems              ( REGISTER_ITEM *regItem, int itemCount );
    void SetDiagramNumber              ( int number );
    void SetMeasureMark                ( int count );

    void PrintCmaxValue                ( float cmax );
    void PrintFmaxValue                ( float fmax );
    //void PrintLmaxValue              ( float lmax );
    void PrintIdleValue                ( float idle );
    void PrintEmaxValue                ( float emax );
    void PrintTmaxValue                ( float tmax );
    void PrintPwValue                  ( float pw );
    void PrintNtValue                  ( int nt );
    void PrintNeValue                  ( int ne );
    void PrintSchedule                 ( enum ScheduleType type );

    void ShowCmax                      ( bool );
    void ShowFmax                      ( bool );
    //void ShowLmax                    ( bool );
    void ShowIdle                      ( bool );
    void ShowEmax                      ( bool );
    void ShowTmax                      ( bool );
    void ShowNE                        ( bool );
    void ShowNT                        ( bool );
    void ShowEnergy                    ( bool );
    void ShowMeasureMark               ( bool );

    /* Get Functions */
    int        GetLeft                 ( void );
    int        GetTop                  ( void );
    int        GetMaxValueScale        ( void );
    int        GetScaleDivisions       ( void );
    int        GetHeight               ( void );
    TObserverGantt *GetHndDiagram      ( void );

    // Drawing Functions
    void GraphTime                     ( int machine, float startT, float elapsedT, unsigned ID, int op );
    void GraphMeasureMark              ( int interval, float pos_time );
};
//---------------------------------------------------------------------------
#endif
 