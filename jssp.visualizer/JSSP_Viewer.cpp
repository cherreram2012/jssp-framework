//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
USEFORM("formularios\_frmMDIMain.cpp", frmMDIMain);
USEFORM("formularios\_frmAboutBox.cpp", frmAboutBox);
USEFORM("formularios\_frmSequences.cpp", frmSequences);
USEFORM("formularios\Gantt Chat\_frmGDiagram.cpp", frmGanttDiagram);
USEFORM("formularios\Energy Chart\_frmEnergyChart.cpp", frmEnergyChart);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
  try
  {
     Application->Initialize();
     Application->CreateForm(__classid(TfrmMDIMain), &frmMDIMain);
     Application->CreateForm(__classid(TfrmSequences), &frmSequences);
     Application->Run();
  }
  catch (Exception &exception)
  {
     Application->ShowException(&exception);
  }
  catch (...)
  {
     try
     {
       throw Exception("");
     }
     catch (Exception &exception)
     {
       Application->ShowException(&exception);
     }
  }
  return 0;
}
//---------------------------------------------------------------------------
