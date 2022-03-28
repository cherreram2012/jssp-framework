#pragma hdrstop

#include <math.h>
#include "class_TGanttDiagram.h"
#include "_frmMDIMain.h"
//#include "gceUtiles.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------
//  Funcion de Utileria. Devuelve la altura del texto en el Canvas pasado
//  por parametro.
//---------------------------------------------------------------------------
int TGanttDiagram::TextHeight (TCanvas *C, AnsiString Text)
{
  return C->TextHeight(Text);
}

//---------------------------------------------------------------------------
//  Funcion de Utileria. Convierte de Unidades de tiempo a pixel.
//---------------------------------------------------------------------------
int TGanttDiagram::TimeToPixel (float time)
{
  // NOTA: Al convertir de unidades de tiempo a pixels, redondeo al entero inmediato
  // inferior con <floor> pues si redondeo al inmediato superior con ceil el diagrama
  // de Gantt queda con los valores alterados.
  return floor((time * (Ruler->Width - Ruler->GetTopScaleRuler())) / Ruler->MaxValue);
}

//---------------------------------------------------------------------------
//  Funcion de Utileria. Dibuja la zona de las etiquetas del diagrama.
//---------------------------------------------------------------------------
void TGanttDiagram::DrawFrameLabels (void)
{
  TRect Frame = imgLabels->Canvas->ClipRect;

  imgLabels->Canvas->Pen->Style   = psSolid;
  imgLabels->Canvas->Pen->Color   = clBlack;
  imgLabels->Canvas->Brush->Color = clCream;
  imgLabels->Canvas->Rectangle(Frame);
}

//---------------------------------------------------------------------------
//  Funcion de Utileria. Dibuja el marco de la zona del grafico.
//---------------------------------------------------------------------------
void TGanttDiagram::DrawFrameGraph (void)
{
  TRect Frame = imgGraph->Canvas->ClipRect;

  imgGraph->Canvas->Pen->Style   = psSolid;
  imgGraph->Canvas->Pen->Color   = clBlack;
  imgGraph->Canvas->Brush->Color = clCream;
  imgGraph->Canvas->Rectangle(Frame);
}

//---------------------------------------------------------------------------
//  Funcion de Utileria. Dibuja las zonas que marcan los work-centers
//---------------------------------------------------------------------------
void TGanttDiagram::DrawFrameWorkCenter (void)
{
  CWorkCenter WC;
  AnsiString wc_label;
  TRect wc_labels_rect, wc_graph_rect;
  int top, mCount, last_mCount, centerCount, text_height, center;

  top = 0;
  last_mCount = 0;
  centerCount = WCList->GetWorkCenterCount();

  wc_labels_rect.Left   = GetLeft();
  wc_labels_rect.Right  = FRAME_LABELS_WIDTH;

  wc_graph_rect.Left   = GetLeft();
  wc_graph_rect.Right  = iFrameGraphWidth;

  imgLabels->Canvas->Font->Size   = 8;
  imgLabels->Canvas->Font->Style  = TFontStyles()<< fsBold;
  imgLabels->Canvas->Pen->Style   = psSolid;
  imgLabels->Canvas->Pen->Color   = clBlack;
  imgLabels->Canvas->Brush->Color = clSilver;
  imgLabels->Canvas->Brush->Style = bsDiagCross;

  imgGraph->Canvas->Font->Style  = TFontStyles()<< fsBold;
  imgGraph->Canvas->Pen->Style   = psSolid;
  imgGraph->Canvas->Pen->Color   = clBlack;
  imgGraph->Canvas->Brush->Color = clSilver;
  imgGraph->Canvas->Brush->Style = bsDiagCross;

  for (int i = 0; i < centerCount; i++)
  {
    WC = WCList->GetWorkCenterByIndex(i);

    mCount = WC.GetMachineCount();
    top   += last_mCount * (MACHINE_LINE_HEIGHT + SOBREMEDIDA);

    wc_labels_rect.Top    = top;
    wc_graph_rect.Top     = top;
    wc_labels_rect.Bottom = top + WORKCENTER_LINE_HEIGHT;
    wc_graph_rect.Bottom  = top + WORKCENTER_LINE_HEIGHT;

    imgLabels->Canvas->Rectangle(wc_labels_rect);
    imgGraph->Canvas->Rectangle(wc_graph_rect);

    wc_label.sprintf("Centro #%d:", i+1);
    text_height = TextHeight(imgLabels->Canvas, wc_label);
    center = (WORKCENTER_LINE_HEIGHT - text_height) / 2 + 1;
    imgLabels->Canvas->TextOutA(3, top + center, wc_label);

    wc_label.sprintf("%s", WC.GetOperationName());
    text_height = TextHeight(imgGraph->Canvas, wc_label);
    center = (WORKCENTER_LINE_HEIGHT - text_height) / 2 + 1;
    imgGraph->Canvas->TextOutA(3, top + center, wc_label);

    last_mCount = mCount;
    top += WORKCENTER_LINE_HEIGHT;
  }
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void TGanttDiagram::CreateTopMachineMask (void)
{
  CWorkCenter WC;
  int top, mCount, last_mCount, centerCount, index;

  top         = 0;
  last_mCount = 0;
  mCount      = MachineList->GetMachineCount() ;
  centerCount = WCList->GetWorkCenterCount();

  TopMachineMask = new int[mCount];

  if (centerCount) /* Hay definidos work-center */
  {
    for (int i = 0; i < centerCount; i++)
    {
      WC = WCList->GetWorkCenterByIndex(i);
      mCount = WC.GetMachineCount();

      top += last_mCount * (MACHINE_LINE_HEIGHT + SOBREMEDIDA);
      for (int j = 0; j < mCount; j++)
      {
        index = MachineList->GetIndexByName(WC.GetMachineByIndex(j));
        TopMachineMask[index] = j * (MACHINE_LINE_HEIGHT + SOBREMEDIDA) + (top + WORKCENTER_LINE_HEIGHT);
      }

      last_mCount = mCount;
      top += WORKCENTER_LINE_HEIGHT;
    }
  }
  else  /* No hay work-centers definidos*/
  {
    for (int i = 0; i < mCount; i++) TopMachineMask[i] = i * (MACHINE_LINE_HEIGHT + SOBREMEDIDA);
  }
}

//---------------------------------------------------------------------------
//  Evento MouseMove para el diagrama de Gantt
//---------------------------------------------------------------------------
void __fastcall TGanttDiagram::myMouseMove (TObject *Sender, TShiftState Shift, int X, int Y)
{
  // Oculto con efecto los paneles
  if (frmMDIMain->pnlLoadedData->Width == 200 && !frmMDIMain->spdbtnFijador1->Down)
  {
    frmMDIMain->tmrShowDC->Enabled = false;
    frmMDIMain->tmrHideDC->Enabled = true;
  }
  if (frmMDIMain->pnlSimbology->Width == 200 && !frmMDIMain->spdbtnFijador2->Down)
  {
    frmMDIMain->tmrShowSU->Enabled  = false;
    frmMDIMain->tmrHideSU->Enabled  = true;
  }
}

//---------------------------------------------------------------------------
//  Constructor de la clase.
//---------------------------------------------------------------------------
TGanttDiagram::TGanttDiagram (TForm *form, int form_width)
{
  iLeft             = 0;
  iTop              = 0;
  iRowCount         = 0;
  iItemCount        = 0;
  iMeasureMarkCount = 0;
  iFrameGraphWidth  = (form_width) ? (form_width - (FRAME_LABELS_WIDTH + 34)) : FRAME_GRAPH_WIDTH;
  hndForm           = form;
  MeasureMark       = NULL;
  TopMachineMask    = NULL;

  Ruler           = new TGDRuler(hndForm);
  imgGraph        = new TImage  (hndForm);
  imgLabels       = new TImage  (hndForm);
  lblGraphNumber  = new TLabel  (hndForm);
  lblStaticCmax   = new TLabel  (hndForm);
  lblStaticFmax   = new TLabel  (hndForm);
  //lblStaticLmax   = new TLabel  (hndForm);
  lblStaticIdle   = new TLabel  (hndForm);
  lblStaticEmax   = new TLabel  (hndForm);
  lblStaticTmax   = new TLabel  (hndForm);
  lblStaticNt     = new TLabel  (hndForm);
  lblStaticNe     = new TLabel  (hndForm);
  lblStaticEnergy = new TLabel  (hndForm);
  lblCmaxValue    = new TLabel  (hndForm);
  lblFmaxValue    = new TLabel  (hndForm);
  //lblLmaxValue    = new TLabel  (hndForm);
  lblIdleValue    = new TLabel  (hndForm);
  lblEmaxValue    = new TLabel  (hndForm);
  lblTmaxValue    = new TLabel  (hndForm);
  lblEnergyValue  = new TLabel  (hndForm);
  lblNeValue      = new TLabel  (hndForm);
  lblNtValue      = new TLabel  (hndForm);
  lblScheduleType = new TLabel  (hndForm);
  lblTitleRuler   = new TLabel  (hndForm);

  Ruler->Parent = hndForm;
  Ruler->Left   = FRAME_LABELS_WIDTH - 1;
  Ruler->Top    = 2;
  Ruler->Width  = iFrameGraphWidth;
  Ruler->Height = RULER_HEIGHT;

  imgLabels->Parent = hndForm;
  imgLabels->Left   = 0;
  imgLabels->Top    = RULER_HEIGHT - 1;
  imgLabels->Width  = FRAME_LABELS_WIDTH;

  imgGraph->Parent  = hndForm;
  imgGraph->Left    = FRAME_LABELS_WIDTH - 1;
  imgGraph->Top     = RULER_HEIGHT - 1;
  imgGraph->Width   = iFrameGraphWidth;
  imgGraph->Cursor  = crCross;
  imgGraph->OnMouseMove = myMouseMove;  

  // Configuro los labels del diagrama
  lblGraphNumber->Parent      = hndForm;
  lblGraphNumber->Left        = 2;
  lblGraphNumber->Top         = 4;
  lblGraphNumber->Font->Color = clGreen;
  lblGraphNumber->Font->Size  = 10;

  lblScheduleType->Parent      = hndForm;
  lblScheduleType->Left        = 10;
  lblScheduleType->Font->Size  = 9;
  lblScheduleType->ShowHint    = true;
  lblScheduleType->Hint        = "Schedule Type";
  lblScheduleType->Font->Style = lblScheduleType->Font->Style << fsBold;

  lblStaticCmax->Parent      = hndForm;
  lblStaticCmax->Left        = 100;
  lblStaticCmax->Font->Size  = 9;
  lblStaticCmax->ShowHint    = true;
  lblStaticCmax->Caption     = "Cmax =";
  lblStaticCmax->Hint        = "Makespan";
  lblStaticCmax->Font->Style = lblStaticCmax->Font->Style << fsBold;

  lblStaticFmax->Parent      = hndForm;
  lblStaticFmax->Left        = 210;
  lblStaticFmax->Font->Size  = 9;
  lblStaticFmax->ShowHint    = true;
  lblStaticFmax->Caption     = "Fmax =";
  lblStaticFmax->Hint        = "Flow Time";
  lblStaticFmax->Font->Style = lblStaticFmax->Font->Style << fsBold;

  /*lblStaticLmax->Parent      = hndForm;
  lblStaticLmax->Left        = 320;
  lblStaticLmax->Font->Size  = 9;
  lblStaticLmax->ShowHint    = true;
  lblStaticLmax->Caption     = "Lmax =";
  lblStaticLmax->Hint        = "Lateness";
  lblStaticLmax->Font->Style = lblStaticLmax->Font->Style << fsBold;*/

  lblStaticIdle->Parent      = hndForm;
  lblStaticIdle->Left        = 320;
  lblStaticIdle->Font->Size  = 9;
  lblStaticIdle->ShowHint    = true;
  lblStaticIdle->Caption     = "Imax =";
  lblStaticIdle->Hint        = "Idle Time";
  lblStaticIdle->Font->Style = lblStaticIdle->Font->Style << fsBold;

  lblStaticEmax->Parent      = hndForm;
  lblStaticEmax->Left        = 430;
  lblStaticEmax->Font->Size  = 9;
  lblStaticEmax->ShowHint    = true;
  lblStaticEmax->Caption     = "Emax =";
  lblStaticEmax->Hint        = "Earliness";
  lblStaticEmax->Font->Style = lblStaticEmax->Font->Style << fsBold;

  lblStaticTmax->Parent      = hndForm;
  lblStaticTmax->Left        = 550;
  lblStaticTmax->Font->Size  = 9;
  lblStaticTmax->ShowHint    = true;
  lblStaticTmax->Caption     = "Tmax =";
  lblStaticTmax->Hint        = "Tardiness";
  lblStaticTmax->Font->Style = lblStaticTmax->Font->Style << fsBold;

  lblStaticNe->Parent        = hndForm;
  lblStaticNe->Left          = 670;
  lblStaticNe->Font->Size    = 9;
  lblStaticNe->ShowHint      = true;
  lblStaticNe->Caption       = "NE =";
  lblStaticNe->Hint          = "Early jobs";
  lblStaticNe->Font->Style   = lblStaticNe->Font->Style << fsBold;

  lblStaticNt->Parent        = hndForm;
  lblStaticNt->Left          = 770;
  lblStaticNt->Font->Size    = 9;
  lblStaticNt->ShowHint      = true;;
  lblStaticNt->Caption       = "NT =";
  lblStaticNt->Hint          = "Late jobs";
  lblStaticNt->Font->Style   = lblStaticNt->Font->Style << fsBold;

  lblStaticEnergy->Parent      = hndForm;
  lblStaticEnergy->Left        = 870;
  lblStaticEnergy->Font->Size  = 9;
  lblStaticEnergy->ShowHint    = true;
  lblStaticEnergy->Caption     = "Ec =";
  lblStaticEnergy->Hint        = "Energy consumption";
  lblStaticEnergy->Font->Style = lblStaticEnergy->Font->Style << fsBold;

  lblCmaxValue->Parent     = hndForm;
  lblCmaxValue->Left       = lblStaticCmax->Left + 45;
  lblCmaxValue->Font->Size = 9;
  lblCmaxValue->Caption    = "?";

  lblFmaxValue->Parent     = hndForm;
  lblFmaxValue->Left       = lblStaticFmax->Left + 45;
  lblFmaxValue->Font->Size = 9;
  lblFmaxValue->Caption    = "?";

  /*lblLmaxValue->Parent     = hndForm;
  lblLmaxValue->Left       = lblStaticLmax->Left + 45;
  lblLmaxValue->Font->Size = 9;
  lblLmaxValue->Caption    = "?";*/

  lblIdleValue->Parent     = hndForm;
  lblIdleValue->Left       = lblStaticIdle->Left + 45;
  lblIdleValue->Font->Size = 9;
  lblIdleValue->Caption    = "?";

  lblEmaxValue->Parent     = hndForm;
  lblEmaxValue->Left       = lblStaticEmax->Left + 45;
  lblEmaxValue->Font->Size = 9;
  lblEmaxValue->Caption    = "?";

  lblTmaxValue->Parent     = hndForm;
  lblTmaxValue->Left       = lblStaticTmax->Left + 45;
  lblTmaxValue->Font->Size = 9;
  lblTmaxValue->Caption    = "?";

  lblNeValue->Parent       = hndForm;
  lblNeValue->Left         = lblStaticNe->Left + 35;
  lblNeValue->Alignment    = taLeftJustify;
  lblNeValue->Font->Size   = 9;
  lblNeValue->Caption      = "?";

  lblNtValue->Parent       = hndForm;
  lblNtValue->Left         = lblStaticNt->Left + 35;
  lblNtValue->Font->Size   = 9;
  lblNtValue->Caption      = "?";

  lblEnergyValue->Parent     = hndForm;
  lblEnergyValue->Left       = lblStaticEnergy->Left + 34;
  lblEnergyValue->Font->Size = 9;
  lblEnergyValue->Caption    = "?";

  lblTitleRuler->Parent      = hndForm;
  lblTitleRuler->Left        = Ruler->Left + 3;
  lblTitleRuler->Top         = Ruler->Top + 6;
  lblTitleRuler->Transparent = true;
  lblTitleRuler->Caption     = "Time";
  lblTitleRuler->Font->Color = clBlack;
}

//---------------------------------------------------------------------------
//  Destructor de la clase.
//---------------------------------------------------------------------------
TGanttDiagram::~TGanttDiagram ()
{
  delete Ruler;
  delete imgGraph;
  delete imgLabels;
  delete lblGraphNumber;
  delete lblScheduleType;
  delete lblStaticCmax;
  delete lblStaticFmax;
  //delete lblStaticLmax;
  delete lblStaticIdle;
  delete lblStaticEmax;
  delete lblStaticTmax;
  delete lblStaticNt;
  delete lblStaticNe;
  delete lblStaticEnergy;
  delete lblCmaxValue;
  delete lblFmaxValue;
  //delete lblLmaxValue;
  delete lblIdleValue;
  delete lblEmaxValue;
  delete lblTmaxValue;
  delete lblNeValue;
  delete lblNtValue;
  delete lblEnergyValue;
  delete lblTitleRuler;
  delete []TopMachineMask;
}

//---------------------------------------------------------------------------
//  Establece el desplazamiento horizontal del diagrama.
//---------------------------------------------------------------------------
void TGanttDiagram::SetLeft (int left)
{
  iLeft                 = left;
  Ruler->Left           = iLeft + FRAME_LABELS_WIDTH - 1;
  imgLabels->Left       = iLeft;
  imgGraph->Left        = iLeft + FRAME_LABELS_WIDTH - 1;
  lblTitleRuler->Left   = Ruler->Left + 3;
  lblGraphNumber->Left  = iLeft + 2;

  lblScheduleType->Left = iLeft + 10;
  lblStaticCmax->Left   = iLeft + 100;
  lblCmaxValue->Left    = lblStaticCmax->Left + 45;
  lblStaticFmax->Left   = iLeft + 210;
  lblFmaxValue->Left    = lblStaticFmax->Left + 45;
  //lblStaticLmax->Left = iLeft + 320;
  //lblLmaxValue->Left  = lblStaticLmax->Left + 45;
  lblStaticIdle->Left   = iLeft + 320;
  lblIdleValue->Left    = lblStaticIdle->Left + 45;
  lblStaticEmax->Left   = iLeft + 430;
  lblEmaxValue->Left    = lblStaticEmax->Left + 45;
  lblStaticTmax->Left   = iLeft + 550;
  lblTmaxValue->Left    = lblStaticTmax->Left + 45;
  lblStaticNe->Left     = iLeft + 670;
  lblNeValue->Left      = lblStaticNe->Left + 35;
  lblStaticNt->Left     = iLeft + 770;
  lblNtValue->Left      = lblStaticNt->Left + 35;
  lblStaticEnergy->Left = iLeft + 870;
  lblEnergyValue->Left  = lblStaticEnergy->Left + 34;
}

//---------------------------------------------------------------------------
//  Establece el desplazamiento vertical del diagrama.
//---------------------------------------------------------------------------
void TGanttDiagram::SetTop (int top)
{
  iTop                 = top;
  Ruler->Top           = iTop;
  imgLabels->Top       = iTop + RULER_HEIGHT - 1;
  imgGraph->Top        = iTop + RULER_HEIGHT - 1;
  lblGraphNumber->Top  = iTop + 4;
  lblScheduleType->Top = iTop + GetHeight() + 5;
  lblStaticCmax->Top   = lblScheduleType->Top;
  lblStaticFmax->Top   = lblScheduleType->Top;
  //lblStaticLmax->Top   = lblScheduleType->Top;
  lblStaticIdle->Top   = lblScheduleType->Top;
  lblStaticEmax->Top   = lblScheduleType->Top;
  lblStaticTmax->Top   = lblScheduleType->Top;
  lblStaticNt->Top     = lblScheduleType->Top;
  lblStaticNe->Top     = lblScheduleType->Top;
  lblStaticEnergy->Top = lblScheduleType->Top;
  lblCmaxValue->Top    = lblStaticCmax->Top;
  lblFmaxValue->Top    = lblStaticFmax->Top;
  //lblLmaxValue->Top    = lblStaticLmax->Top;
  lblIdleValue->Top    = lblStaticIdle->Top;
  lblEmaxValue->Top    = lblStaticEmax->Top;
  lblTmaxValue->Top    = lblStaticTmax->Top;
  lblNeValue->Top      = lblStaticNt->Top;
  lblNtValue->Top      = lblStaticNe->Top;
  lblEnergyValue->Top  = lblStaticEnergy->Top;
  lblTitleRuler->Top   = Ruler->Top + 6;
}

//---------------------------------------------------------------------------
//  Establece el mayor valor de escala en la regla del diagrama.
//---------------------------------------------------------------------------
void TGanttDiagram::SetMaxRulerValue (float value)
{
  Ruler->MaxValue = value;
}

//---------------------------------------------------------------------------
//  Establece la cantidad de divisiones que tiene la escala de la regla.
//---------------------------------------------------------------------------
void TGanttDiagram::SetScaleDivisions (int divisions)
{
  Ruler->Divisions = divisions;
}

//---------------------------------------------------------------------------
//  Establece el titulo de las etiquetas de las tareas.
//---------------------------------------------------------------------------
void TGanttDiagram::SetMachineAndWorkCenterLabels (const CMachineCollection_PM *mList, const CWorkCenterCollection *wcList )
{
  CWorkCenter WC;
  int text_height, middle, centerCount, mCount, index;

  MachineList = mList;
  WCList      = wcList;
  iRowCount   = MachineList->GetMachineCount();
  centerCount = WCList->GetWorkCenterCount();

  // Calculo la altura del diagrama
  iHeight = iRowCount * (MACHINE_LINE_HEIGHT + SOBREMEDIDA) + centerCount * WORKCENTER_LINE_HEIGHT;
  imgLabels->Height = iHeight;
  imgGraph->Height  = iHeight;

  // Ubico los labels de salida en su lugar segun el ancho del diagrama
  lblScheduleType->Top = iTop + GetHeight() + 5;
  lblStaticCmax->Top   = lblScheduleType->Top;
  lblStaticFmax->Top   = lblScheduleType->Top;
  //lblStaticLmax->Top   = lblScheduleType->Top;
  lblStaticIdle->Top   = lblScheduleType->Top;
  lblStaticEmax->Top   = lblScheduleType->Top;
  lblStaticTmax->Top   = lblScheduleType->Top;
  lblStaticNt->Top     = lblScheduleType->Top;
  lblStaticNe->Top     = lblScheduleType->Top;
  lblStaticEnergy->Top = lblScheduleType->Top;
  lblCmaxValue->Top    = lblStaticCmax->Top;
  lblFmaxValue->Top    = lblStaticFmax->Top;
  //lblLmaxValue->Top    = lblStaticLmax->Top;
  lblIdleValue->Top    = lblStaticIdle->Top;
  lblEmaxValue->Top    = lblStaticEmax->Top;
  lblTmaxValue->Top    = lblStaticTmax->Top;
  lblNeValue->Top      = lblStaticNt->Top;
  lblNtValue->Top      = lblStaticNe->Top;
  lblEnergyValue->Top  = lblStaticEnergy->Top;

  // Repinto los marcos del diagrama
  DrawFrameLabels();
  DrawFrameGraph();
  
  //
  CreateTopMachineMask();

  if (centerCount)
  {
    DrawFrameWorkCenter();

    // Establesco el patron de las lineas para el interlineado del grafico
    imgGraph->Canvas->Pen->Style    = psDot;
    imgLabels->Canvas->Font->Size   = 10;
    imgLabels->Canvas->Font->Style  = TFontStyles();
    imgLabels->Canvas->Brush->Color = clCream;
    imgGraph->Canvas->Brush->Color  = clCream;
        
    index = 0;
    for (int i = 0; i < centerCount; i++)
    {
      WC = WCList->GetWorkCenterByIndex(i);
      mCount = WC.GetMachineCount();

      for (int j = 0; j < mCount; j++)
      {
        index = MachineList->GetIndexByName(WC.GetMachineByIndex(j));

        if (j)
        {
          imgLabels->Canvas->MoveTo(0, TopMachineMask[index]);
          imgLabels->Canvas->LineTo(FRAME_LABELS_WIDTH, TopMachineMask[index]);
          imgGraph->Canvas->MoveTo(0, TopMachineMask[index]);
          imgGraph->Canvas->LineTo(iFrameGraphWidth, TopMachineMask[index]);
        }

        // Escribo los nombre de las maquinas
        text_height = TextHeight(imgLabels->Canvas, WC.GetMachineByIndex(j));
        middle = ((MACHINE_LINE_HEIGHT + SOBREMEDIDA) - text_height) / 2;
        imgLabels->Canvas->TextOutA(3, TopMachineMask[index] + middle, WC.GetMachineByIndex(j));
      }
    } 
  }
  else
  {
    // Establesco el patron de las lineas para el interlineado del grafico
    imgGraph->Canvas->Pen->Style    = psDot;
    imgLabels->Canvas->Font->Size   = 10;
    imgLabels->Canvas->Font->Style  = TFontStyles();
    imgLabels->Canvas->Brush->Color = clCream;
    imgGraph->Canvas->Brush->Color  = clCream;

    for (int i = 0; i < iRowCount; i++)
    {
      if (i)
      {
        // Dibujo las lineas del interlineado del grafico
        imgLabels->Canvas->MoveTo(0, TopMachineMask[i]);
        imgLabels->Canvas->LineTo(FRAME_LABELS_WIDTH, TopMachineMask[i]);
        imgGraph->Canvas->MoveTo(0, TopMachineMask[i]);
        imgGraph->Canvas->LineTo(iFrameGraphWidth, TopMachineMask[i]);
      }
            
      // Escribo los nombre de las maquinas
      text_height = TextHeight(imgLabels->Canvas, MachineList->GetNameByIndex(i));
      middle = ((MACHINE_LINE_HEIGHT + SOBREMEDIDA) - text_height) / 2;
      imgLabels->Canvas->TextOutA(3, (MACHINE_LINE_HEIGHT + SOBREMEDIDA) * i + middle, MachineList->GetNameByIndex(i));
    }
  }
}

//---------------------------------------------------------------------------
//  Establece el registro de los items.
//---------------------------------------------------------------------------
void TGanttDiagram::SetRegisterItems (REGISTER_ITEM *regItem, int itemCount)
{
  RegItems   = regItem;
  iItemCount = itemCount;
}

//---------------------------------------------------------------------------
//  Establece el nunmero del diagrama que se muestra en la interfaz.
//---------------------------------------------------------------------------
void TGanttDiagram::SetDiagramNumber (int number)
{
  lblGraphNumber->Caption = "Diagrama #" + IntToStr(number);
}

//---------------------------------------------------------------------------
//  Establece la cantidad de marcas de lectura a mostrar en el grafico. 
//---------------------------------------------------------------------------
void TGanttDiagram::SetMeasureMark (int count)
{
  iMeasureMarkCount = count;

  MeasureMark = new TShape *[iMeasureMarkCount];
  for (int i = 0; i < iMeasureMarkCount; i++)
  {
    MeasureMark[i]               = new TShape (hndForm);
    MeasureMark[i]->Parent       = hndForm;
    MeasureMark[i]->Left         = GetLeft() + FRAME_LABELS_WIDTH;
    MeasureMark[i]->Top          = GetTop() - 1 + RULER_HEIGHT;
    MeasureMark[i]->Width        = 1;
    MeasureMark[i]->Height       = GetHeight() - 25;
    MeasureMark[i]->Pen->Style   = psDot;
    MeasureMark[i]->Pen->Mode    = pmMergePenNot;
    MeasureMark[i]->Brush->Color = clCream;
    MeasureMark[i]->Visible      = false; 
  }
}

//---------------------------------------------------------------------------
//  Imprime en la interfaz el valor de makespan
//---------------------------------------------------------------------------
void TGanttDiagram::PrintCmaxValue (float cmax)
{
  AnsiString str;
  str.sprintf("%.1f", cmax);
  lblCmaxValue->Caption = KillCero(str.c_str());
}

//---------------------------------------------------------------------------
//  Imprime en la interfaz el valor de
//---------------------------------------------------------------------------
void TGanttDiagram::PrintFmaxValue (float fmax)
{
  AnsiString str;
  str.sprintf("%.1f", fmax);
  lblFmaxValue->Caption = KillCero(str.c_str());
}

//---------------------------------------------------------------------------
//  Imprime en la interfaz el valor de
//---------------------------------------------------------------------------
/*void TGanttDiagram::PrintLmaxValue (float lmax)
{
  AnsiString str;
  str.sprintf("%.1f", lmax);
  lblLmaxValue->Caption = KillCero(str.c_str());
}*/

//---------------------------------------------------------------------------
//  Imprime en la interfaz el valor de
//---------------------------------------------------------------------------
void TGanttDiagram::PrintIdleValue (float idle)
{
  AnsiString str;
  str.sprintf("%.1f", idle);
  lblIdleValue->Caption = KillCero(str.c_str());
}

//---------------------------------------------------------------------------
//  Imprime en la interfaz el valor de
//---------------------------------------------------------------------------
void TGanttDiagram::PrintEmaxValue (float emax)
{
  AnsiString str;
  str.sprintf("%.1f", emax);
  lblEmaxValue->Caption = KillCero(str.c_str());
}

//---------------------------------------------------------------------------
//  Imprime en la interfaz el valor de
//---------------------------------------------------------------------------
void TGanttDiagram::PrintTmaxValue (float tmax)
{
  AnsiString str;
  str.sprintf("%.1f", tmax);
  lblTmaxValue->Caption = KillCero(str.c_str());
}

//---------------------------------------------------------------------------
//  Imprime en la interfaz el valor de consumo de energia
//---------------------------------------------------------------------------
void TGanttDiagram::PrintPwValue (float pw)
{
  AnsiString str;
  str.sprintf("%.1f", pw);
  lblEnergyValue->Caption = KillCero(str.c_str());
}

//---------------------------------------------------------------------------
//  Imprime en la interfaz el valor de
//---------------------------------------------------------------------------
void TGanttDiagram::PrintNeValue (int ne)
{
  lblNeValue->Caption = ne;
}

//---------------------------------------------------------------------------
//  Imprime en la interfaz el valor de
//---------------------------------------------------------------------------
void TGanttDiagram::PrintNtValue (int nt)
{
  lblNtValue->Caption = nt;
}

//---------------------------------------------------------------------------
//  Imprime en la interfaz
//---------------------------------------------------------------------------
void TGanttDiagram::PrintSchedule (enum ScheduleType type)
{
  switch (type)
	{
		case stSemiActive:
			lblScheduleType->Caption = "Semi-Active";
		break;

		case stActive:
			lblScheduleType->Caption = "Active";
		break;

		case stNonDelay:
			lblScheduleType->Caption = "Non-Delay";
		break;
	}
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void TGanttDiagram::ShowCmax (bool show)
{
  lblStaticCmax->Visible = show;
  lblCmaxValue->Visible  = show;
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void TGanttDiagram::ShowFmax (bool show)
{
  lblStaticFmax->Visible = show;
  lblFmaxValue->Visible  = show;
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
/*void TGanttDiagram::ShowLmax (bool show)
{
  lblStaticLmax->Visible = show;
  lblLmaxValue->Visible  = show;
}*/

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void TGanttDiagram::ShowIdle (bool show)
{
  lblStaticIdle->Visible = show;
  lblIdleValue->Visible  = show;
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void TGanttDiagram::ShowEmax (bool show)
{
  lblStaticEmax->Visible = show;
  lblEmaxValue->Visible  = show;
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void TGanttDiagram::ShowTmax (bool show)
{
  lblStaticTmax->Visible = show;
  lblTmaxValue->Visible  = show;
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void TGanttDiagram::ShowNE (bool show)
{
  lblStaticNe->Visible = show;
  lblNeValue->Visible  = show;
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void TGanttDiagram::ShowNT (bool show)
{
  lblStaticNt->Visible = show;
  lblNtValue->Visible  = show;
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void TGanttDiagram::ShowEnergy (bool show)
{
  lblStaticEnergy->Visible = show;
  lblEnergyValue->Visible  = show;
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void TGanttDiagram::ShowMeasureMark (bool show)
{
  for (int i = 0; i < iMeasureMarkCount; i++) MeasureMark[i]->Visible = show;
}

//---------------------------------------------------------------------------
//  Devuelve el desplazamiento horizontal del diagrama.
//---------------------------------------------------------------------------
int TGanttDiagram::GetLeft (void)
{
  return iLeft;
}

//---------------------------------------------------------------------------
//  Devuelve el desplazamiento vertical del diagram.
//---------------------------------------------------------------------------
int TGanttDiagram::GetTop (void)
{
  return iTop;
}

//---------------------------------------------------------------------------
//  Devuelve el mayor valor de escala que tiene la regla del diagrama.
//---------------------------------------------------------------------------
int TGanttDiagram::GetMaxValueScale (void)
{
  return Ruler->MaxValue;
}

//---------------------------------------------------------------------------
//  Devuelve la cantidad de divisiones que tiene la escala de la regla.
//---------------------------------------------------------------------------
int TGanttDiagram::GetScaleDivisions (void)
{
  return Ruler->Divisions;
}

//---------------------------------------------------------------------------
//  Devuelve la altura del diagrama.
//---------------------------------------------------------------------------
int TGanttDiagram::GetHeight (void)
{
  return iHeight + RULER_HEIGHT;
}

//---------------------------------------------------------------------------
//  Devuelve un handler del diagrama que es usado para registrarse en la
//  clase CTableTP que es quien tiene la informacion con los datos a
//  graficar.
//---------------------------------------------------------------------------
TObserverGantt *TGanttDiagram::GetHndDiagram (void)
{
  return this;
}

//---------------------------------------------------------------------------
//  Ubica un tiempo individual en el diagrama.
//---------------------------------------------------------------------------
void TGanttDiagram::GraphTime (int machine, float startT, float elapsedT, unsigned ID, int op)
{
  TShape *Shape;
  TColor tColor;
  AnsiString tooltip;

  if (!iItemCount)
    throw Exception("No se ha establecido el número de trabajos que se desea mostrar.");

  // Configuro el color del tiempo a mostrar
  tColor = RegItems[ID-1].Color;

  Shape               = new TShape (hndForm);
  Shape->Parent       = hndForm;
  Shape->Left         = GetLeft() + FRAME_LABELS_WIDTH + TimeToPixel(startT) - 1;
  Shape->Top          = GetTop() - 1 + RULER_HEIGHT + TopMachineMask[machine] + (SOBREMEDIDA / 2);
  Shape->Width        = TimeToPixel(elapsedT) + 2;
  Shape->Height       = MACHINE_LINE_HEIGHT;
  Shape->ShowHint     = true;
  Shape->Brush->Color = tColor;
  Shape->Cursor       = crCross;
  tooltip.sprintf("Job  : %d\n"
                  "Op   : %dº\n"
                  "Time: %.1f\n"
                  "Ci    : %0.1f", ID, op, elapsedT, startT + elapsedT);
  Shape->Hint = tooltip;
}

//---------------------------------------------------------------------------
//  Ubica una marca de lectura en el diagrama.
//---------------------------------------------------------------------------
void TGanttDiagram::GraphMeasureMark (int interval, float pos_time)
{
  int left;

  left = GetLeft() + FRAME_LABELS_WIDTH;
  MeasureMark[interval]->Left = left + TimeToPixel(pos_time);
}
