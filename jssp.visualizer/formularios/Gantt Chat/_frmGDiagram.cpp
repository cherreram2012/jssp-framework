
#include <vcl.h>
#pragma hdrstop

#include "_frmGDiagram.h"
#include "_frmMDIMain.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TfrmGanttDiagram *frmGanttDiagram;

/////////////////////////////////////////////////////////////////////////////
//    FORM CONSTRUCTOR

__fastcall TfrmGanttDiagram::TfrmGanttDiagram(TComponent* Owner)
  : TForm(Owner)
{
}
//---------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
//    PUBLIC FUNCTIONS

/***** Establece la cantidad de diagramas que tendra el Visor de Diagramas *****/
/***** de Gantt.                                                           *****/
void TfrmGanttDiagram::CreateDiagrams (int count)
{
  //int width;

  dCount = count;
  //width = this->Width - ;

  Diagramas = new TGanttDiagram *[dCount];
  for (int i = 0; i < dCount; i++)
  {
    Diagramas[i] = new TGanttDiagram(this, this->Width);
    Diagramas[i]->SetDiagramNumber(i + 1);
  }
}
//---------------------------------------------------------------------------

/***** Establece valor del maximo de la regla de los diagramas *****/
void TfrmGanttDiagram::SetMaxValueOfAllRuler (float maxValue)
{
  int RulerTopValue = 0;

  while (maxValue > RulerTopValue) RulerTopValue += 10;

  for (int i = 0; i < dCount; i++) Diagramas[i]->SetMaxRulerValue(RulerTopValue);
}
//---------------------------------------------------------------------------

/***** Establece las etiquetas para las tareas del diagrama *****/
void TfrmGanttDiagram::SetMachineLabels (const CMachineCollection_PM *M, const CWorkCenterCollection *WC)
{
  int height;

  if (dCount)
  {
    Diagramas[0]->SetMachineAndWorkCenterLabels(M, WC);
    height = Diagramas[0]->GetHeight();
    Diagramas[0]->SetLeft(6);
    for (int i = 1; i < dCount; i++)
    {
      Diagramas[i]->SetMachineAndWorkCenterLabels(M, WC);
      Diagramas[i]->SetTop(Diagramas[i - 1]->GetTop() + height + SPACE_BETWEEN_GANTT_CHART);
      Diagramas[i]->SetLeft(6);
    }
  }
  else
    throw Exception("No se ha establecido la cantidad de graficos que tendra el Visor de "
                    "Diagramas de Gantt.");
}
//---------------------------------------------------------------------------

/***** Establece el registro de los elementos y la cantidad de estos *****/
void TfrmGanttDiagram::SetRegisterItems (REGISTER_ITEM *rItem, int count)
{
  if (dCount)
  {
    for (int i = 0; i < dCount; i++)
      Diagramas[i]->SetRegisterItems(rItem, count);
  }
  else
    throw Exception("No se ha establecido la cantidad de graficos que tendra el Visor de"
                    "Diagramas de Gantt.");
}
//---------------------------------------------------------------------------

/*****  *****/
void TfrmGanttDiagram::ShowObjetive (bool cmax, bool fmax, bool idle, bool emax, bool tmax, bool ne, bool nt, bool energy)
{
  if (dCount)
  {
    for (int i = 0; i < dCount; i++)
    {
      Diagramas[i]->ShowCmax(cmax);
      Diagramas[i]->ShowFmax(fmax);
      Diagramas[i]->ShowIdle(idle);
      Diagramas[i]->ShowEmax(emax);
      Diagramas[i]->ShowTmax(tmax);
      Diagramas[i]->ShowNE(ne);
      Diagramas[i]->ShowNT(nt);
      Diagramas[i]->ShowEnergy(energy);
    }
  }
  else
    throw Exception("No se ha establecido la cantidad de graficos que tendra el Visor de"
                    "Diagramas de Gantt.");
}
//---------------------------------------------------------------------------

/*****  *****/
/*****  *****/
void TfrmGanttDiagram::ShowMeasureMark (bool show)
{
  for(int i = 0; i < dCount; i++) Diagramas[i]->ShowMeasureMark(show);
}
//---------------------------------------------------------------------------

/***** Devuelve un handler del diagrama el cual sera utilizado para registarlo *****/
/***** en la clase CTableTP.                                                   *****/
TObserverGantt *TfrmGanttDiagram::GetHndDiagramByIndex (int index)
{
  if (index < 0 && index >= dCount) throw Exception("Indice fuera de rango !!!");

  return Diagramas[index]->GetHndDiagram();
}

/////////////////////////////////////////////////////////////////////////////
//    EVENTS FORM

void __fastcall TfrmGanttDiagram::FormDestroy(TObject *Sender)
{
  for(int i = 0; i < dCount; i++)
    delete Diagramas[i];
  delete []Diagramas;
}
//---------------------------------------------------------------------------

void __fastcall TfrmGanttDiagram::FormClose(TObject *Sender, TCloseAction &Action)
{
  Action = caFree;
}
//---------------------------------------------------------------------------

void __fastcall TfrmGanttDiagram::FormMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
  // Oculto con efecto los paneles
  if (frmMDIMain->pnlLoadedData->Width == 200 &&
      X > frmMDIMain->pnlLoadedData->Width && !frmMDIMain->spdbtnFijador1->Down)
  {
    frmMDIMain->tmrShowDC->Enabled = false;
    frmMDIMain->tmrHideDC->Enabled = true;
  }
  if (frmMDIMain->pnlSimbology->Width == 200 &&
      X > frmMDIMain->pnlSimbology->Width && !frmMDIMain->spdbtnFijador2->Down)
  {
    frmMDIMain->tmrShowSU->Enabled  = false;
    frmMDIMain->tmrHideSU->Enabled  = true;
  }
}
//---------------------------------------------------------------------------
