#include <vcl.h>
#pragma hdrstop

#include "_frmEnergyChart.h"
#include "class_EnergyChart.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TGDRuler"
#pragma resource "*.dfm"

TfrmEnergyChart *frmEnergyChart;

//---------------------------------------------------------------------------
__fastcall TfrmEnergyChart::TfrmEnergyChart(TComponent* Owner)
  : TForm(Owner)
{
}
//---------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
//    PUBLIC FUNCTIONS

/***** Establece la cantidad de diagramas que tendra el Visor de Diagramas *****/
/***** de Gantt.                                                           *****/
void TfrmEnergyChart::CreateDiagrams (int count)
{
  int height;

  dCount = count;
  Diagramas = new TEnergyChart *[dCount];
  for (int i = 0; i < dCount; i++)
  {
    Diagramas[i] = new TEnergyChart(this);
    Diagramas[i]->SetDiagramNumber(i + 1);
    Diagramas[i]->SetLeft(73);

    if (i == 0)
    {
      height = Diagramas[0]->GetHeight();
      Diagramas[0]->SetTop(2);
    }
    else
      Diagramas[i]->SetTop(Diagramas[i - 1]->GetTop() + height + SPACE_BETWEEN_ENERGY_CHART);
  }
}
//---------------------------------------------------------------------------

void TfrmEnergyChart::SetMaxValueOfPower (float value)
{
  int power_top_value = 0;

  while (value > power_top_value) power_top_value += 5;

  for (int i = 0; i < dCount; i++) Diagramas[i]->SetMaxValueOfPower(power_top_value);
}
//---------------------------------------------------------------------------

void TfrmEnergyChart::SetMaxValueTime (float value)
{
  int time_top_value = 0;

  while (value > time_top_value) time_top_value += 10;

  for (int i = 0; i < dCount; i++) Diagramas[i]->SetMaxValueOfTime(time_top_value);
}
//---------------------------------------------------------------------------

/***** Devuelve un handler del diagrama el cual sera utilizado para registarlo *****/
/***** en la clase CSPGraph.                                                   *****/
TObserverEnergy *TfrmEnergyChart::GetHndDiagramByIndex (int index)
{
  if (index < 0 && index >= dCount)
    throw Exception("Indice fuera de rango !!!");

  return Diagramas[index]->GetHndDiagram();
}
//---------------------------------------------------------------------------

void __fastcall TfrmEnergyChart::FormDestroy(TObject *Sender)
{
  for(int i = 0; i < dCount; i++)
    delete Diagramas[i];
  delete []Diagramas;
}
//---------------------------------------------------------------------------

void __fastcall TfrmEnergyChart::FormClose(TObject *Sender,
      TCloseAction &Action)
{
  Action = caFree;
}
//---------------------------------------------------------------------------

