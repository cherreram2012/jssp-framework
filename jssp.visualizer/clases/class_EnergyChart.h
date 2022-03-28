#ifndef class_EnergyChartH
#define class_EnergyChartH

#include <vcl.h>
#include "TGDRuler.h"
#include "CSPGraph_PM.h"

#define CHART_WIDTH  880

class TEnergyChart : public TObserverEnergy
{
  private:
    int       iLeft;
    int       iTop;
    int       iIntervals;
    float     fCmaxTime;
    TForm    *hndForm;
    TShape   *shpChartNoFrame;
    TLabel   *lblChartNo;
    TLabel   *lblPowerLabel;
    TGDRuler *PowerRuler;
    TGDRuler *TimeRuler;
    TImage   *imgBackground;
    TShape   *shpCenterChart;
    TLabel   *lblCenterChart;
    TLabel   *lblTimeLabel;
    TLabel   *lblStaticEc;
    TLabel   *lblEcValue;
    TLabel   *lblStaticCmax;
    TLabel   *lblCmaxValue;
    TPanel   *pnlEnergyAve;
    TShape   **PowerArray;
    TCheckBox *chkEcLine;

  private:
    int TimeToPixel                     ( TGDRuler *Ruler, float time );
    void __fastcall IntervalsMouseMove  ( TObject *Sender, TShiftState Shift, int X, int Y );
    void __fastcall BackgroundMouseMove ( TObject *Sender, TShiftState Shift, int X, int Y );
    void __fastcall CheckClick          ( TObject *Sender );

  public:
    TEnergyChart ( TForm *form = NULL );
    ~TEnergyChart();

    /* Set Functions */
    void SetLeft            ( int left );
    void SetTop             ( int top );
    void SetDiagramNumber   ( int number );
    void SetDataChart       ( float cmax, int intervals );
    void SetMaxValueOfPower ( int value );
    void SetMaxValueOfTime  ( int value );

    /* Get Functions */
    int GetLeft             ( void );
    int GetTop              ( void );
    int GetWidth            ( void );
    int GetHeight           ( void );
    int GetIntervals        ( void );
    TObserverEnergy *GetHndDiagram  ( void );

    void DrawOutLine        ( void );
    void DrawEnergyAve      ( float value );
    void GraphInterval      ( int interval, float machine_power, char *machine_list[], int machine_on );
    void PrintECValue       ( float value );
    void PrintCmaxValue     ( float value );
};
//---------------------------------------------------------------------------

#endif
