#pragma hdrstop

#include "class_EnergyChart.h"
#include "_frmMDIMain.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//  Funcion de Utileria. Convierte de Unidades de tiempo a pixel.
//---------------------------------------------------------------------------
int TEnergyChart::TimeToPixel (TGDRuler *Ruler, float time)
{
  int pixel;

  // NOTA: Al convertir de unidades de tiempo a pixels, redondeo al entero inmediato
  // inferior con <floor> pues si redondeo al inmediato superior con ceil el diagrama
  // de Gantt queda con los valores alterados.
  if (Ruler->Orientation == roHorizontal)
    pixel = floor((time * (Ruler->Width - Ruler->GetTopScaleRuler())) / Ruler->MaxValue);
  else
    pixel = floor((time * (Ruler->Height - Ruler->GetTopScaleRuler())) / Ruler->MaxValue);

  return pixel;
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void __fastcall TEnergyChart::IntervalsMouseMove (TObject *Sender, TShiftState Shift, int X, int Y)
{
  PowerArray[((TShape *)Sender)->Tag]->Brush->Color = (TColor)0x009DCEFF;

  for (int i = 0; i < iIntervals; i++)
    if (i != ((TShape *)Sender)->Tag) PowerArray[i]->Brush->Color = (TColor)0x00C6E2FF;
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void __fastcall TEnergyChart::BackgroundMouseMove (TObject *Sender, TShiftState Shift, int X, int Y)
{
  for (int i = 0; i < iIntervals; i++) PowerArray[i]->Brush->Color = (TColor)0x00C6E2FF;
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void __fastcall TEnergyChart::CheckClick (TObject *Sender)
{
  pnlEnergyAve->Visible = chkEcLine->Checked;
}

//---------------------------------------------------------------------------
//  Constructor de la clase.
//---------------------------------------------------------------------------
TEnergyChart::TEnergyChart (TForm *form)
{
  iLeft      = 0;
  iTop       = 0;
  iIntervals = 10;
  hndForm    = form;

  shpChartNoFrame = new TShape   (hndForm);
  lblChartNo      = new TLabel   (hndForm);
  lblPowerLabel   = new TLabel   (hndForm);
  PowerRuler      = new TGDRuler (hndForm);
  TimeRuler       = new TGDRuler (hndForm);
  imgBackground   = new TImage   (hndForm);
  shpCenterChart  = new TShape   (hndForm);
  lblCenterChart  = new TLabel   (hndForm);
  lblTimeLabel    = new TLabel   (hndForm);
  lblStaticEc     = new TLabel   (hndForm);
  lblEcValue      = new TLabel   (hndForm);
  lblStaticCmax   = new TLabel   (hndForm);
  lblCmaxValue    = new TLabel   (hndForm);
  pnlEnergyAve    = new TPanel   (hndForm);
  chkEcLine       = new TCheckBox(hndForm);

  shpChartNoFrame->Parent       = hndForm;
  shpChartNoFrame->Left         = 0;
  shpChartNoFrame->Top          = 0;
  shpChartNoFrame->Width        = 80;
  shpChartNoFrame->Height       = 20;
  shpChartNoFrame->Brush->Color = clBtnFace;

  lblChartNo->Parent            = hndForm;
  lblChartNo->Left              = 6;
  lblChartNo->Top               = 2;
  lblChartNo->Font->Color       = clGreen;
  lblChartNo->Font->Size        = 10;
  lblChartNo->Caption           = "Gráfico #?";

  lblPowerLabel->Parent         = hndForm;
  lblPowerLabel->Left           = 2;
  lblPowerLabel->Top            = 24;
  lblPowerLabel->Font->Size     = 8;
  lblPowerLabel->Caption        = "Potencia";

  PowerRuler->Parent            = hndForm;
  PowerRuler->Left              = 0;
  PowerRuler->Top               = 39;
  PowerRuler->Orientation       = roVertical;
  PowerRuler->Width             = 27;
  PowerRuler->Height            = 251;
  PowerRuler->DirectionType     = dtRightToLeft;
  PowerRuler->Divisions         = 5;

  TimeRuler->Parent             = hndForm;
  TimeRuler->Left               = PowerRuler->Left + PowerRuler->Width - 1;
  TimeRuler->Top                = PowerRuler->Top + PowerRuler->Height - 1;
  TimeRuler->Orientation        = roHorizontal;
  TimeRuler->Width              = CHART_WIDTH;
  TimeRuler->Height             = 25;
  TimeRuler->ScaleSide          = osTopRight;
  TimeRuler->Divisions          = 10;

  imgBackground->Parent         = hndForm;
  imgBackground->Left           = PowerRuler->Left + PowerRuler->Width - 1;
  imgBackground->Top            = PowerRuler->Top;
  imgBackground->Height         = PowerRuler->Height;
  imgBackground->Width          = TimeRuler->Width;
  imgBackground->Canvas->Brush->Color = clWhite;
  imgBackground->Canvas->Rectangle(0, 0, imgBackground->Width, imgBackground->Height);
  imgBackground->OnMouseMove    = BackgroundMouseMove;

  shpCenterChart->Parent        = hndForm;
  shpCenterChart->Left          = PowerRuler->Left;
  shpCenterChart->Top           = PowerRuler->Top + PowerRuler->Height - 1;
  shpCenterChart->Height        = TimeRuler->Height;
  shpCenterChart->Width         = PowerRuler->Width;

  lblCenterChart->Parent        = hndForm;
  lblCenterChart->Left          = shpCenterChart->Left + 11;
  lblCenterChart->Top           = shpCenterChart->Top + 5;
  lblCenterChart->Transparent   = true;
  lblCenterChart->Caption       = "0";

  lblTimeLabel->Parent          = hndForm;
  lblTimeLabel->Left            = TimeRuler->Left + TimeRuler->Width - 38;
  lblTimeLabel->Top             = TimeRuler->Top + TimeRuler->Height + 2;
  lblTimeLabel->Font->Size      = 8;
  lblTimeLabel->Caption         = "Tiempo";

  lblStaticEc->Parent           = hndForm;
  lblStaticEc->Left             = shpCenterChart->Left + shpCenterChart->Width + 2;
  lblStaticEc->Top              = shpCenterChart->Top + shpCenterChart->Height + 3;
  lblStaticEc->Font->Size       = 9;
  lblStaticEc->Font->Style      = lblStaticEc->Font->Style << fsBold;
  lblStaticEc->Caption          = "Ec = ";

  lblEcValue->Parent            = hndForm;
  lblEcValue->Left              = lblStaticEc->Left + lblStaticEc->Width;
  lblEcValue->Top               = lblStaticEc->Top;
  lblEcValue->Font->Size        = 9;
  lblEcValue->Caption           = "0";

  lblStaticCmax->Parent         = hndForm;
  lblStaticCmax->Left           = lblEcValue->Left + 70;
  lblStaticCmax->Top            = lblStaticEc->Top;
  lblStaticCmax->Font->Size     = 9;
  lblStaticCmax->Font->Style    = lblStaticCmax->Font->Style << fsBold;
  lblStaticCmax->Caption        = "Cmax = ";

  lblCmaxValue->Parent          = hndForm;
  lblCmaxValue->Left            = lblStaticCmax->Left + lblStaticCmax->Width;
  lblCmaxValue->Top             = lblStaticCmax->Top;
  lblCmaxValue->Font->Size      = 9;
  lblCmaxValue->Caption         = "0";

  pnlEnergyAve->Parent          = hndForm;
  pnlEnergyAve->Left            = imgBackground->Left + 2;
  pnlEnergyAve->Top             = imgBackground->Top + imgBackground->Height - 4;
  pnlEnergyAve->Height          = 2;
  pnlEnergyAve->Width           = imgBackground->Width - 4;
  pnlEnergyAve->BevelOuter      = bvNone;
  pnlEnergyAve->Color           = clBlue;

  chkEcLine->Parent             = hndForm;
  chkEcLine->Left               = shpChartNoFrame->Left + shpChartNoFrame->Width;
  chkEcLine->Top                = shpChartNoFrame->Top + 3;
  chkEcLine->Font->Size         = 9;
  chkEcLine->Caption            = "Mostrar promedio de Ec en el gráfico.";
  chkEcLine->OnClick            = CheckClick;
  chkEcLine->Width              = 240; 
  chkEcLine->Checked            = true;
}

//---------------------------------------------------------------------------
//  Destructor de la clase.
//---------------------------------------------------------------------------
TEnergyChart::~TEnergyChart()
{
  hndForm = NULL;

  delete shpChartNoFrame;
  delete lblChartNo;
  delete lblPowerLabel;
  delete imgBackground;
  delete TimeRuler;
  delete PowerRuler;
  delete shpCenterChart;
  delete lblCenterChart;
  delete lblTimeLabel;
  delete lblStaticEc;
  delete lblEcValue;
  delete lblStaticCmax;
  delete lblCmaxValue;
  delete pnlEnergyAve;
  delete chkEcLine;
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void TEnergyChart::SetLeft (int left)
{
  iLeft = left;
  
  shpChartNoFrame->Left = iLeft;
  lblChartNo->Left      = iLeft + 6;
  lblPowerLabel->Left   = iLeft + 2;
  PowerRuler->Left      = iLeft;
  TimeRuler->Left       = PowerRuler->Left + PowerRuler->Width - 1;
  imgBackground->Left   = PowerRuler->Left + PowerRuler->Width - 1;
  shpCenterChart->Left  = PowerRuler->Left;
  lblCenterChart->Left  = shpCenterChart->Left + 11;
  lblTimeLabel->Left    = TimeRuler->Left + TimeRuler->Width - 36;
  lblStaticEc->Left     = shpCenterChart->Left + shpCenterChart->Width + 2;
  lblEcValue->Left      = lblStaticEc->Left + lblStaticEc->Width;
  lblStaticCmax->Left   = lblEcValue->Left + 70;
  lblCmaxValue->Left    = lblStaticCmax->Left + lblStaticCmax->Width;
  pnlEnergyAve->Left    = imgBackground->Left + 2;
  chkEcLine->Left       = shpChartNoFrame->Left + shpChartNoFrame->Width + 40;
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void TEnergyChart::SetTop (int top)
{
  iTop = top;

  shpChartNoFrame->Top = iTop;
  lblChartNo->Top      = iTop + 2;
  lblPowerLabel->Top   = iTop + 24;
  PowerRuler->Top      = iTop + 39;
  TimeRuler->Top       = PowerRuler->Top + PowerRuler->Height - 1;
  imgBackground->Top   = PowerRuler->Top;
  shpCenterChart->Top  = PowerRuler->Top + PowerRuler->Height - 1;
  lblCenterChart->Top  = shpCenterChart->Top + 5;
  lblTimeLabel->Top    = TimeRuler->Top + TimeRuler->Height + 3;
  lblStaticEc->Top     = shpCenterChart->Top + shpCenterChart->Height + 3;
  lblEcValue->Top      = lblStaticEc->Top;
  lblStaticCmax->Top   = lblStaticEc->Top;
  lblCmaxValue->Top    = lblStaticCmax->Top;
  pnlEnergyAve->Top    = imgBackground->Top + imgBackground->Height - 4;
  chkEcLine->Top       = shpChartNoFrame->Top + 3;
}

//---------------------------------------------------------------------------
//  Establece el nunmero del diagrama que se muestra en la interfaz.
//---------------------------------------------------------------------------
void TEnergyChart::SetDiagramNumber (int number)
{
  lblChartNo->Caption = "Gráfico #" + IntToStr(number);
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void TEnergyChart::SetDataChart (float cmax, int intervals)
{
  int width, height, left_margin;

  fCmaxTime  = cmax;
  iIntervals = intervals;
  width      = TimeToPixel(TimeRuler, cmax / iIntervals);
  PowerArray = new TShape *[iIntervals];

  left_margin = PowerRuler->Left + PowerRuler->Width;
  for (int i = 0; i < iIntervals; i++)
  {
    PowerArray[i]               = new TShape(hndForm);
    PowerArray[i]->Parent       = hndForm;
    PowerArray[i]->Tag          = i;
    PowerArray[i]->Brush->Color =(TColor)0x00C6E2FF;
    PowerArray[i]->Pen->Style   = psSolid;
    PowerArray[i]->Pen->Color   =(TColor)0x00C6E2FF; // clBlack
    PowerArray[i]->Cursor       = crDrag;
    PowerArray[i]->OnMouseMove  = IntervalsMouseMove;
    PowerArray[i]->Top          = imgBackground->Top+2;
    PowerArray[i]->Width        = width;
    PowerArray[i]->Height       = height;
    PowerArray[i]->ShowHint     = true;

    if (i == 0)
      PowerArray[i]->Left = left_margin+1;
    else
      PowerArray[i]->Left = PowerArray[i-1]->Left + width;
  }

  int dif = TimeToPixel(TimeRuler, cmax) - (PowerArray[iIntervals - 1]->Left + width - left_margin);
  while (dif > 0)
  {
    for (int j = 0; j < iIntervals; j++)
    {
      if (j == 0)
        PowerArray[j]->Width++;
      else
      {
        PowerArray[j]->Width++;
        PowerArray[j]->Left = PowerArray[j-1]->Left + PowerArray[j]->Width;
      }

      if (dif) dif--;
      else PowerArray[j]->Left = PowerArray[j-1]->Left + width;
    }
  }
  PowerArray[iIntervals - 1]->Width--;
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void TEnergyChart::SetMaxValueOfPower (int value)
{
  PowerRuler->MaxValue = value;
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void TEnergyChart::SetMaxValueOfTime (int value)
{
  TimeRuler->MaxValue = value;
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
int TEnergyChart::GetLeft (void)
{
  return iLeft;
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
int TEnergyChart::GetTop (void)
{
  return iTop;
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
int TEnergyChart::GetWidth  (void)
{
  return PowerRuler->Width + imgBackground->Width;
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
int TEnergyChart::GetHeight (void)
{
  return lblStaticEc->Top + lblStaticEc->Height + 1;
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
int TEnergyChart::GetIntervals (void)
{
  return iIntervals;
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
TObserverEnergy *TEnergyChart::GetHndDiagram (void)
{
  return this;
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void TEnergyChart::DrawOutLine (void)
{
  int top, left, width;

  imgBackground->Canvas->Pen->Color = clBlack;
  imgBackground->Canvas->Pen->Style = psSolid;
  imgBackground->Canvas->Pen->Width = 2;

  for (int i = 0; i < iIntervals; i++)
  {
    //=== Se dibuja los contornos horizontales ===//
    top   = imgBackground->Height - PowerArray[i]->Height - 3;
    left  = PowerArray[i]->Left - (PowerRuler->Left + PowerRuler->Width);
    width = left + PowerArray[i]->Width;

    imgBackground->Canvas->MoveTo(left, top);
    imgBackground->Canvas->LineTo(width+2, top);

    //=== Se dibuja los contornos verticales ===//
    if (i == 0)
    {
      int to_r = imgBackground->Height - PowerArray[i + 1]->Height - 3;

      // Como el 0:0 del image-box esta en top-left y el dibujo se hace de abajo-arriba
      //
      width = (top > to_r) ? width : width + 2;
      imgBackground->Canvas->MoveTo(width, top);
      imgBackground->Canvas->LineTo(width, to_r-1);
    }
    else if (i == iIntervals-1)
    {
      int base = imgBackground->Height;

      imgBackground->Canvas->MoveTo(width+2, top);
      imgBackground->Canvas->LineTo(width+2, base);
    }
    else
    {
      int to_r = imgBackground->Height - PowerArray[i + 1]->Height - 3;
      int to_l = imgBackground->Height - PowerArray[i - 1]->Height - 3;

      // Dibulo en contorno vertical derecho
      imgBackground->Canvas->MoveTo(width+2, top);
      imgBackground->Canvas->LineTo(width+2, to_r-1);

      // Dibulo en contorno vertical izquierdo
      imgBackground->Canvas->MoveTo(left, top-1);
      imgBackground->Canvas->LineTo(left, to_l-1);
    }
  }
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void TEnergyChart::DrawEnergyAve (float value)
{
  int base, top;

  base = imgBackground->Top + imgBackground->Height;
  pnlEnergyAve->Top = base - TimeToPixel(PowerRuler, value) + 1;
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void TEnergyChart::GraphInterval (int interval, float consume, char *machine_list[], int machine_on)
{
   int base, top;
  AnsiString msg, str;

  // Se prepara la informacion que se mostrara en la Ayuda Sensitiva
  msg.sprintf("Máquinas Activas: %d", machine_on);
  for (int i = 0; i < machine_on; i++)
    msg = msg + AnsiString("\n") + AnsiString(" - ") + AnsiString(machine_list[i]);
  msg = msg + AnsiString("\nConsumo: ") + AnsiString(KillCero(str.sprintf("%.2f", consume).c_str()));

  base = imgBackground->Top + imgBackground->Height;
  top  = TimeToPixel(PowerRuler, consume) - 3;
  PowerArray[interval]->Top = base - top;
  PowerArray[interval]->Height = top - 2;
  PowerArray[interval]->Hint = msg;
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void TEnergyChart::PrintECValue (float value)
{
  AnsiString str;

  lblEcValue->Caption = KillCero(str.sprintf("%.1f", value).c_str());
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void TEnergyChart::PrintCmaxValue (float value)
{
  AnsiString str;

  lblCmaxValue->Caption = KillCero(str.sprintf("%.1f", value).c_str());
}

