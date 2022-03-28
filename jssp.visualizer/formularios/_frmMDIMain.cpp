#include <vcl.h>
#include <time.h>
#include <stdio.h>

#pragma hdrstop

#include "_frmMDIMain.h"
#include "_frmSequences.h"
#include "_frmAboutBox.h"
#include "_frmGDiagram.h"
#include "_frmEnergyChart.h"
#include "class_RegSequence.h"
//#include "gceUtiles.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
#pragma link "CGAUGES"
#pragma resource "*.dfm"

TfrmMDIMain *frmMDIMain;
//---------------------------------------------------------------------------

/***** Chequea la estructura del fichero de datos para determinar si se *****/
/***** pueden cargar los datos sin problemas.                           *****/
int CheckDataFile (AnsiString filename)
{
  int npiece = -1, nmachine = -1;
  char string[LINE_LENGTH];
  bool piece_mark = false, machine_mark = false;

  // ---------------------------------------------------------------------- //
  // Nota: El chequeo transita por una serie de filtros, en caso de que no  //
  //       pase por uno de los filtros entonces el fichero no es valido.    //
  // ---------------------------------------------------------------------- //
  // FILTRO #1: ¿El fichero existe? *********************************************
  if (filename == "") return 0;   // Error!!!. Ruta invalida.

  FILE *file = fopen(filename.c_str(), "rt");
  if (!file) return 0;      // Error!!!. El fichero no se puede abrir.

  // FILTRO #2: ¿Se encontro la marca indica la cantidad de maquinas? ***********
  //            ¿Es igual el numero de maquinas declaradas a las existentes? ****
  while (!feof(file) && nmachine == -1)
  {
	  fgets(string, LINE_LENGTH, file);
		char *token = strtok(string, ":");

		if (strstr(token, "Machines") != NULL)
		{
      machine_mark = true;
			token = strtok(NULL, ":");
			//Trim(token);
			nmachine = (token) ? atoi(token) : 0;
		}
  }
  while(!feof(file) && nmachine > 0)
  {
    fgets(string, LINE_LENGTH, file);

		if (strstr(string, "[MACHINES]") != NULL)
		{
			while(!feof(file) && nmachine > 0 && strstr(string, "END MACHINE") == NULL)
			{
				fgets(string, LINE_LENGTH, file);
				//Trim(string);
				if (strcmp(string, "\n") != 0 && strcmp(string, "") != 0 &&
            strstr(string, "END MACHINE") == NULL)
					nmachine--;
			}
		}
  }
  if (nmachine > 0 || machine_mark == false)
  {
    fclose(file);
    return 0;    // Error!!!. No coincide la cantidad de maquinas declaradas
                 //  con las existentes en el fichero.
  }

  // FILTRO #3: ¿Se encontro la marca indica la cantidad de piezas? *************
  //            ¿Es igual el numero de piezas declaradas a las existentes? ******
  rewind(file);
  while (!feof(file) && npiece == -1)
	{
		fgets(string, LINE_LENGTH, file);
		char *token = strtok(string, ":");

		if (strstr(token, "[PIECE]") != NULL)
		{
      piece_mark = true;
			token = strtok(NULL, ":");
			//Trim(token);
			npiece = (token) ? atoi(token) : 0;
		}
	}
  while (!feof(file) && npiece > 0)
	{
		fgets(string, LINE_LENGTH, file);

		if (strstr(string, "Name") != NULL)
			npiece--;
  }
  if (npiece > 0 || piece_mark == false)
  {
    fclose(file);
    return 0;     // Error!!!. No coincide la cantidad de piezas declaradas
                  // con las existentes en el fichero.
  }

  fclose(file);                                                 
  return 1;   // OK!!!. Fichero valido.
}
//---------------------------------------------------------------------------

/***** *****/
void TfrmMDIMain::ConvertStringToSequence (AnsiString str, unsigned *Sequence)
{
  int index = 0, length;
  AnsiString img;

  // Hay que hacer una copia de la cadena original para que el pasarsela al "strtok"
  // no quede modificada fuera de la funcion
  img.sprintf("%s", str.c_str());
  char *token = strtok(img.c_str(), " ");
	Sequence[index] = atoi(token);
  index++;

  if (Problem_Type == PFSS) length = Npiece + Recirc.tRecirc;
  else length = Npiece * Nmachine + Recirc.tRecirc;

  while((token = strtok(NULL, " ")) != NULL && index < length)
  {
    Sequence[index] = atoi(token);
    index++;
  }
}
//---------------------------------------------------------------------------

int TfrmMDIMain::GetItemCountInSequence (AnsiString seq)
{
  int count = 0;
  char *str = new char[seq.Length()+1];

  strcpy(str, seq.c_str());

  char *token = strtok(str, " ");
  if (token) count++;
  while((token = strtok(NULL, " ")) != NULL)
    count++;

  delete []str;
  return count;
}
//---------------------------------------------------------------------------

/***** Determina si el genoma pasado por parametro es valido *****/
/*bool TfrmMDIMain::isValidGenome (const unsigned *genome, int length, int job)
{
	for (int i = 0; i < length; i++)
		if (genome[i] == 0 || genome[i] > (unsigned)job) return false;

	return true;
}*/
//---------------------------------------------------------------------------

/***** Determina si la secuencia pasada por parametro es valida atendiendo a los ids que presenta *****/
bool TfrmMDIMain::isValidSequence (const unsigned *sequence, int job, int machine)
{
  for (int i = 0; i < job * machine; i++)
  {
    if (sequence[i] > (unsigned)job)  return false;
  }

  return true;
}

/***** Determina si la secuencia pasada por parametro es valida a la cantidad de operaciones*****/
bool TfrmMDIMain::isValidOperationCount (const unsigned *sequence, int job, int machine)
{
  int *counter = new int[job+1];

  for (int i = 0; i < job + 1; i++) counter[i] = 0;
  
  for (int i = 0; i < job * machine; i++)
  {
    counter[sequence[i]]++;
    if (counter[sequence[i]] > machine) return false;
  }

  delete []counter;
  return true;
}
/////////////////////////////////////////////////////////////////////////////
//    FORM CONSTRUCTOR

__fastcall TfrmMDIMain::TfrmMDIMain(TComponent* Owner)
  : TForm(Owner)
{
  MachineList = NULL;
  CollectionP = NULL;
  sequence    = NULL;
}
//---------------------------------------------------------------------------

/***** Muestra en el panel deslizable los datos cargados. *****/
void TfrmMDIMain::ShowLoadedDataInPanel (void)
{
  TTreeNode *Item, *subItem;
  int nodes = -1;
  AnsiString string, value, operations;
  CMachineGroupExtended MG;
  CPieceTechnologicalRoute_PM P;

  //pnlLoadedData->Visible = true;
  TreeView->Items->Clear();  

  if (Npiece != CollectionP->GetPieceCount())
    Npiece = CollectionP->GetPieceCount();
  if (Nmachine != MachineList->GetMachineCount())
    Nmachine = MachineList->GetMachineCount();

  string.sprintf("Jobs: %d", Npiece);
  TreeView->Items->Add(NULL, string);
  nodes++;
  string.sprintf("Machines: %d", Nmachine);
  TreeView->Items->Add(NULL, string);
  nodes++;

  TreeView->Items->Add(NULL, "");
  nodes++;
  TreeView->Items->Add(NULL, "Machines Lists");
  nodes++;
  Item = TreeView->Items->Item[nodes];
  for (int i = 0; i < Nmachine; i++)
  {
    string.sprintf("%s", MachineList->GetNameByIndex(i));
    TreeView->Items->AddChild(Item, string);
    nodes++;
  }

  int wc_count = WorkCenters.GetWorkCenterCount();
  if (wc_count)
  {
    TreeView->Items->Add(NULL, "");
    nodes++;
    TreeView->Items->Add(NULL, "Work Centers");
    nodes++;
    Item = TreeView->Items->Item[nodes];
    CWorkCenter WC;

    for (int i = 0; i < wc_count; i++)
    {
      string.sprintf("Center #%d:", i+1);
      TreeView->Items->AddChild(Item, string);
      nodes++;

      WC = WorkCenters.GetWorkCenterByIndex(i);
      int machine = WC.GetMachineCount();
      subItem = TreeView->Items->Item[nodes];
      string.sprintf("Name: %s", WC.GetOperationName());
      TreeView->Items->AddChild(subItem, string);
      nodes++;
      for (int j = 0; j < machine; j++)
      {
        string.sprintf("%s", WC.GetMachineByIndex(j));
        TreeView->Items->AddChild(subItem, string);
        nodes++;
      }
    }
  }

  TreeView->Items->Add(NULL, "");
  nodes++;
  for (int j = 0; j < Npiece; j++)
  {                                  //int op = P.GetOperationCount();
    P = CollectionP->GetPieceByIndex(j);
    //string.sprintf("%s (%d)", P.GetPieceName(), op);
    string.sprintf("%s", P.GetPieceName());
    TreeView->Items->Add(NULL, string);
    nodes++;
    Item = TreeView->Items->Item[nodes];

    int op = P.GetOperationCount();
    for (int k = 0; k < op; k++)
    {
      MG = *P.GetMachinesGroupByIndex(k);

      operations = "";
      int count = MG.GetMachineCount();
      for (int m = 0; m < count; m++)
      {
        value.sprintf("%f", MG.GetMachineByIndex(m).Time);
        string.sprintf("{%s: %s}%s", MG.GetMachineByIndex(m).MachineName, KillCero(value.c_str()), (m < count-1) ? " - " : " ");
        operations += string;
      }
      //operations.sprintf("%dº Operation");
      TreeView->Items->AddChild(Item, operations);
      nodes++;
    }
  }
}
//---------------------------------------------------------------------------

/***** Muestra en el panel deslizable la simbologia utilizada. *****/
void TfrmMDIMain::ShowSimbologyInPanel (void)
{
  CPieceTechnologicalRoute_PM P;

  // Establesco la cantidad de filas 
  strgrdSimbology->RowCount = Npiece;
  strgrdSimbology->Invalidate();

  for (int i = 0; i < Npiece; i++)
  {
    P = CollectionP->GetPieceByIndex(i);
    strgrdSimbology->Cells[1][i] = P.GetPieceName();
  }
}
//---------------------------------------------------------------------------

/***** Carga los datos que se necesitan y los almacena en los buffer disponibles.  *****/
void TfrmMDIMain::LoadData (void)
{
  //  Establesco el path del file
  hdFile.SetPath(strDataFile.c_str());

	// Obtengo la cantidad de piezas y operaciones
  hdFile.ReadInstanceSize(&Npiece, &Nmachine);

  // Reservo memoria
  if (MachineList)
  {
    delete MachineList;
    MachineList = new CMachineCollection_PM(Nmachine);
  }
  else MachineList = new CMachineCollection_PM(Nmachine);

  if (CollectionP)
  {
    delete CollectionP;
    CollectionP = new CPieceCollection_PM(Npiece, Nmachine);
  }
  else CollectionP = new CPieceCollection_PM(Npiece, Nmachine);

  if (ListID)
  {
    delete []ListID;
    ListID = new unsigned [Npiece];
  }
  else ListID = new unsigned [Npiece];

  // Leo los datos
  WorkCenters.ClearCollection();
	hdFile.ReadMachineData(MachineList);
  hdFile.ReadWorkCenterData(&WorkCenters);
	hdFile.ReadPieceData(CollectionP, *MachineList);
  CollectionP->ApplyFifoRuler(opnApplyRulerFIFO->Checked);
  Problem_Type = CollectionP->ProblemType();
  CollectionP->GetAllID(ListID);

  WorkCenters.EnableFastAccessMode();
}
//---------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
//    FORM's EVENTS

void __fastcall TfrmMDIMain::FormCreate(TObject *Sender)
{
  // Establezco el largo inicial para los paneles desplazables
  pnlLoadedData->Width = 0;
  pnlSimbology->Width  = 0;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMDIMain::FormCloseQuery(TObject *Sender,
      bool &CanClose)
{
  if (Application->MessageBoxA("¿Desea salir de la aplicación?",
                                "SecProMaq para secuencias estaticas",
                                MB_ICONINFORMATION + MB_YESNO) == IDNO )
    CanClose = false;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMDIMain::FormClose(TObject *Sender,
       TCloseAction &Action)
{
  // Libero la memoria mis objetos
  if (MachineList) delete MachineList;
  if (CollectionP) delete CollectionP;
  CRegisterItems::Free();
}
//---------------------------------------------------------------------------

void __fastcall TfrmMDIMain::FormMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
  // Oculto con efecto los paneles
  if (pnlLoadedData->Width == 200 && X > pnlLoadedData->Width && !spdbtnFijador1->Down)
  {
    tmrShowDC->Enabled = false;
    tmrHideDC->Enabled = true;
  }
  if (pnlSimbology->Width == 200 && X > pnlSimbology->Width && !spdbtnFijador2->Down)
  {
    tmrShowSU->Enabled  = false;
    tmrHideSU->Enabled  = true;
  }
}
//---------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
//    TIMER's EVENTS

void __fastcall TfrmMDIMain::tmrShowDCTimer(TObject *Sender)
{
  // Muestro con efecto el panel con los datos cargados del fichero de datos
  if (width_DC < WIDTH_PANEL)
  {
    width_DC += 20;
    pnlLoadedData->Width = width_DC;
  }
  else
    tmrShowDC->Enabled = false;

}
//---------------------------------------------------------------------------

void __fastcall TfrmMDIMain::tmrHideDCTimer(TObject *Sender)
{
  // Oculto con efecto el panel con los datos cargados del fichero de datos
  if (pnlLoadedData->Width > 0)
  {
    width_DC -= 20;
    pnlLoadedData->Width = width_DC;
  }
  else
  {
    AnsiString path = ExtractFilePath(Application->ExeName);
    imgLashDC->Picture->LoadFromFile(path + "img\\pdc.bmp");
    tmrHideDC->Enabled = false;
  }
}
//---------------------------------------------------------------------------

void __fastcall TfrmMDIMain::tmrShowSUTimer(TObject *Sender)
{
  // Muestro con efecto el panel de la simbologia
  if (width_SU < WIDTH_PANEL)
  {
    width_SU += 20;
    pnlSimbology->Width = width_SU;
  }
  else
    tmrShowSU->Enabled = false;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMDIMain::tmrHideSUTimer(TObject *Sender)
{
  // Oculto con efecto el panel de la simbologia
  if (width_SU > 0)
  {
    width_SU -= 20;
    pnlSimbology->Width = width_SU;
  }
  else
  {
    AnsiString path = ExtractFilePath(Application->ExeName);
    imgLashSU->Picture->LoadFromFile(path + "img\\psu.bmp");
    tmrHideSU->Enabled = false;
  }
}
//---------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
//    MAIN-MENU CONTROL

//**************************** Menu FILE ***********************************/

void __fastcall TfrmMDIMain::opnOpenProjectClick(TObject *Sender)
{
  AnsiString path = ExtractFilePath(Application->ExeName);
  strDataFile = "";

  if (OpenDialogDataFile->Execute())
  {
    strDataFile = OpenDialogDataFile->FileName;

    // Verifico la autenticidad del fichero de datos
    if (CheckDataFile(strDataFile))
    {
      if (CRegisterItems::isRegisterItems()) CRegisterItems::Free();
      LoadData();
      // Registro los genes para ser usado por el objeto Diagrama de Gantt
      CRegisterItems::RegSequence(ListID, Npiece);
      RegItem = CRegisterItems::GetReg();

      ShowLoadedDataInPanel();
      ShowSimbologyInPanel();
      spdbtnGanttDiagram->Enabled = true;
      spdbtnGanttDiagram->Glyph->LoadFromFile(path + "img\\gantt_enable.bmp");

      switch (Problem_Type)
      {
        case PFSS: StatusBar->Panels->Items[1]->Text = " PFSS"; break;
        case FSS : StatusBar->Panels->Items[1]->Text = " FSS";  break;
        case GFSS: StatusBar->Panels->Items[1]->Text = " GFSS"; break;
        case JSS : StatusBar->Panels->Items[1]->Text = " JSS";  break;
        case FJSS: StatusBar->Panels->Items[1]->Text = " FJSS"; break;
      }
      StatusBar->Panels->Items[3]->Text = " " + strDataFile;

      //---------  Para debuguear !!!
      /*unsigned sequence1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
                              1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
                              1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
                              1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
                              1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
                              1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
      unsigned sequence2[] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};

      frmGanttDiagram = new TfrmGanttDiagram(Application);
      frmGanttDiagram->CreateDiagrams(1);
      frmGanttDiagram->SetMachineLabels(MachineList, &WorkCenters);
      frmGanttDiagram->SetRegisterItems(RegItem, Npiece);

      CSPGraph_PM Grafo1 (Npiece, Nmachine, opnApplyRescheduling->Checked);
      //CSPGraph_PM Grafo2 (Npiece, Nmachine, opnApplyRescheduling->Checked);

      Grafo1.SetJobsList(ListID);
      //Grafo2.SetJobsList(ListID);

      //CollectionP->Create_N_Graph(&Grafo1, MachineList, sequence1);
      CollectionP->Create_NxM_Graph(&Grafo1, MachineList, sequence1);
      frmGanttDiagram->SetMaxValueOfAllRuler(Grafo1.Makespan());
      Grafo1.SetGantChartView(frmGanttDiagram->GetHndDiagramByIndex(0));
      Grafo1.CreateGanttChart();

      //CollectionP->Create_N_Graph(&Grafo2, MachineList, sequence2);
      //Grafo2.SetGantChartView(frmGanttDiagram->GetHndDiagramByIndex(1));
      //Grafo2.CreateGanttChart(); */
      //---------      
    }
    else
    {
      StatusBar->Panels->Items[1]->Text = " None";
      StatusBar->Panels->Items[3]->Text = " Error";
      Application->MessageBoxA("El fichero de datos no pudo ser cargado", "El fichero de datos no es válido",
                               MB_ICONWARNING);
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TfrmMDIMain::opnExitClick(TObject *Sender)
{
  Close();
}
//---------------------------------------------------------------------------

//**************************** Menu VIEW ***********************************/

void __fastcall TfrmMDIMain::opnViewGDiagramClick(TObject *Sender)
{
  CSPGraph_PM *Grafo;
  unsigned *list_jobs;
  int dg_count = 0, i, element_count;
  float maxMakespan = 0, maxEnergy = 0, tmp;
  TStrings *valid_SequenceStringList = new TStringList();
  bool cmax, fmax, idle, emax, tmax, ne, nt, energy;
  
  list_jobs = new unsigned [CollectionP->GetPieceCount()];
  CollectionP->GetAllID(list_jobs);
  CollectionP->GetRecirculationData(&Recirc);

  if (Problem_Type == PFSS) sequence = new unsigned [Npiece + Recirc.tRecirc];
  else sequence = new unsigned [Npiece * Nmachine + Recirc.tRecirc];

  if (frmSequences->ShowModal() == mrOk)
  {
    // Se determina cuantas cadenas representan secuencias validos. Esto lo hacemos
    // para saber en realidad cuantos Diagramas de Gantt crear.
    for (i = 0; i < SequenceStringList->Count; i++)
    {
      element_count = GetItemCountInSequence(SequenceStringList->Strings[i]);
      if (element_count == Npiece && Problem_Type != PFSS)
      {
        AnsiString msg;

        msg.sprintf("Longitud de la secuencia incorrecta.\n\n"
                    "La secuencia #%d tiene una longitud distinta a la requerida para el cálculo.\n"
                    " - Longitud introducida: %d\n"
                    " - Longitud requerida  : %d", i + 1,
                    element_count, (Problem_Type == PFSS) ? Npiece + Recirc.tRecirc : Npiece * Nmachine + Recirc.tRecirc);
        Application->MessageBoxA(msg.c_str(), "Visor de Secuencias", MB_ICONWARNING);
        return;
      }

      ConvertStringToSequence(SequenceStringList->Strings[i], sequence);
      if (!isValidSequence(sequence, Npiece, Nmachine))
      {
        AnsiString msg;

        msg.sprintf("Error en ID de pieza.\n\n"
                    "En la secuencia #%d se detectó un id de pieza inválido.\n"
                    " - Los IDs de pieza para esta corrida deben estar entre 1 y %d", i + 1, Npiece);
        Application->MessageBoxA(msg.c_str(), "Visor de Secuencias", MB_ICONWARNING);
        return;
      }
      // SECTION AGREGADA EN BRASIL (02-06-2019)
      // Compruebo validos por no recirculacion. Significa que las piezas pueden tener no maximo
      // m operaciones, con m igual a cantidad de maquinas.
      if (opnNoRecirc->Checked)
      {
        if (!isValidOperationCount(sequence, Npiece, Nmachine))
        {
          AnsiString msg;

          msg.sprintf("Hay id de piezas que tienen mas de M operaciones, lo que invalida la secuencia.\n\n");
          Application->MessageBoxA(msg.c_str(), "Visor de Secuencias", MB_ICONWARNING);
          return;
        }
      }

      CSPGraph_PM obj(Npiece, Nmachine, Recirc.tRecirc, opnApplyRescheduling->Checked);
      obj.SetEnergyMeasureCount(MeasureCount);
      if (Problem_Type == PFSS)
        CollectionP->Create_N_Graph(&obj, MachineList, sequence);
      else
        CollectionP->Create_NxM_Graph(&obj, MachineList, sequence);

      tmp = obj.Makespan();
      if (tmp > maxMakespan) maxMakespan = tmp;
      tmp = obj.GetMaxEnergyConsumption();
      if (tmp > maxEnergy) maxEnergy = tmp;

      valid_SequenceStringList->Add(SequenceStringList->Strings[i]);
      dg_count++;
    }

    // Verifico si entre
    int diferent = SequenceStringList->Count - valid_SequenceStringList->Count;
    if (diferent)
    {
      AnsiString msg;
      msg.sprintf("Se eliminaron: %d secuencias por ser invalidas.", diferent);
      Application->MessageBoxA(msg.c_str(), "Diagrama de Gantt", MB_ICONEXCLAMATION);
    }

    if (valid_SequenceStringList->Count)
    {
      submnuObjetive->Enabled = true;

      /*** Se crean los diagramas de gantt. ***/
      frmGanttDiagram = new TfrmGanttDiagram(Application);
      frmGanttDiagram->CreateDiagrams(dg_count);
      frmGanttDiagram->SetMachineLabels(MachineList, &WorkCenters);
      frmGanttDiagram->SetRegisterItems(RegItem, Npiece);

      frmEnergyChart = new TfrmEnergyChart(Application);
      frmEnergyChart->CreateDiagrams(dg_count);

      if (opnApplyRescheduling->Checked)
        frmGanttDiagram->Caption = "Diagramas de Gantt - [Aplicando IBUIT]";
      else
        frmGanttDiagram->Caption = "Diagramas de Gantt";

      frmGanttDiagram->SetMaxValueOfAllRuler(maxMakespan);
      frmEnergyChart->SetMaxValueOfPower(maxEnergy);
      frmEnergyChart->SetMaxValueTime(maxMakespan);

      for (i = 0; i < dg_count; i++)
      {
        Grafo = new CSPGraph_PM (Npiece, Nmachine, Recirc.tRecirc, opnApplyRescheduling->Checked);
        ConvertStringToSequence(valid_SequenceStringList->Strings[i], sequence);

        if (Problem_Type == PFSS)
          CollectionP->Create_N_Graph(Grafo, MachineList, sequence);
        else
          CollectionP->Create_NxM_Graph(Grafo, MachineList, sequence);

        Grafo->SetGantChartView(frmGanttDiagram->GetHndDiagramByIndex(i));
        Grafo->SetEnergyChartView(frmEnergyChart->GetHndDiagramByIndex(i));
        Grafo->SetJobsList(list_jobs);
        Grafo->SetEnergyMeasureCount(MeasureCount);
        Grafo->CreateGanttChart();
        Grafo->CreateEnergyChart();
        delete Grafo;
      }

      cmax = opnCmax->Checked; fmax   = opnFmax->Checked;
      idle = opnIdle->Checked; emax   = opnEmax->Checked;
      tmax = opnTmax->Checked; ne     = opnNE->Checked;
      nt   = opnNT->Checked;   energy = opnEn->Checked;
      frmGanttDiagram->ShowObjetive(cmax, fmax, idle, emax, tmax, ne, nt, energy);
      frmGanttDiagram->ShowMeasureMark(opnViewMark->Checked);
      frmGanttDiagram->Show();
      
      if (opnEn->Checked)
      {
        frmEnergyChart->FormStyle = fsMDIChild;
        frmEnergyChart->Show();
      }
      strgrdSimbology->Invalidate();
    }
    else Application->MessageBoxA("No se encontraron secuencias válidas representar.",
                                  "Grafico de Gantt", MB_ICONWARNING);

    delete SequenceStringList;
  }

  delete []sequence;
  delete []list_jobs;
	delete valid_SequenceStringList;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMDIMain::opnViewToolsBarClick(TObject *Sender)
{
  ToolBar->Visible = opnViewToolsBar->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMDIMain::opnViewStatusBarClick(TObject *Sender)
{
  StatusBar->Visible = opnViewStatusBar->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMDIMain::opnHelpAboutClick(TObject *Sender)
{
  frmAboutBox = new TfrmAboutBox(Application);
  frmAboutBox->ShowModal();
}
//---------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
//    BUTTONs

void __fastcall TfrmMDIMain::spdbtnFijador1Click(TObject *Sender)
{
  AnsiString path = ExtractFilePath(Application->ExeName);

  // Nueva forma jejejejeje
  if (dynamic_cast <TSpeedButton *>(Sender)->Down)
    dynamic_cast <TSpeedButton *>(Sender)->Glyph->LoadFromFile(path + "img\\man_izq.bmp");
  else
    dynamic_cast <TSpeedButton *>(Sender)->Glyph->LoadFromFile(path + "img\\noman_izq.bmp");
}
//---------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
//    STRING-GRID

void __fastcall TfrmMDIMain::strgrdSimbologyDrawCell(TObject *Sender,
      int ACol, int ARow, TRect &Rect, TGridDrawState State)
{
  if (!CRegisterItems::isRegisterItems() && ACol == 0)
  {
    strgrdSimbology->Canvas->Pen->Color   = clBlack;
    strgrdSimbology->Canvas->Brush->Color = clWhite;
    strgrdSimbology->Canvas->Rectangle(Rect);
  }
  else if (CRegisterItems::isRegisterItems() && ACol == 0)
  {
    strgrdSimbology->Canvas->Pen->Color   = clBlack;
    strgrdSimbology->Canvas->Brush->Color = RegItem[ARow].Color;
    strgrdSimbology->Canvas->Rectangle(Rect);
  }
}
//---------------------------------------------------------------------------

void __fastcall TfrmMDIMain::imgLashDCClick(TObject *Sender)
{
  AnsiString path = ExtractFilePath(Application->ExeName);

  imgLashDC->Picture->LoadFromFile(path + "img\\pdc_s.bmp");
  // Mostrar panel con los datos cargados
  if (pnlLoadedData->Width != 200)
  {
    pnlLoadedData->Width   = 0;
    width_DC               = 0;
    pnlLoadedData->Visible = true;
    pnlLoadedData->Align   = alLeft;
    pnlLoadedData->Align   = alNone;
    tmrShowDC->Enabled    = true;
  }

  imgLashSU->Picture->LoadFromFile(path + "img\\psu.bmp");
  // Ocultar panel de la simbologia
  if (pnlSimbology->Width == 200)
  {
    pnlSimbology->Width = 200;
    width_SU            = 200;
    tmrHideSU->Enabled  = true;
  }

  path = ExtractFilePath(Application->ExeName);
  spdbtnFijador2->Down = false;
  spdbtnFijador2->Glyph->LoadFromFile(path + "img\\noman_izq.bmp");
}
//---------------------------------------------------------------------------

void __fastcall TfrmMDIMain::imgLashSUClick(TObject *Sender)
{
  AnsiString path = ExtractFilePath(Application->ExeName);

  imgLashSU->Picture->LoadFromFile(path + "img\\psu_s.bmp");
  // Mostrar panel de la simbologia
  if (pnlSimbology->Width != 200)
  {
    pnlSimbology->Width   = 0;
    width_SU              = 0;
    pnlSimbology->Visible = true;
    pnlSimbology->Align   = alLeft;
    pnlSimbology->Align   = alNone;
    tmrShowSU->Enabled    = true;
  }

  imgLashDC->Picture->LoadFromFile(path + "img\\pdc.bmp");
  // Ocultar panel con los datos cargados
  if (pnlLoadedData->Width == 200)
  {
    pnlLoadedData->Width = 200;
    width_DC             = 200;
    tmrHideDC->Enabled   = true;
  }

  path = ExtractFilePath(Application->ExeName);
  spdbtnFijador1->Down = false;
  spdbtnFijador1->Glyph->LoadFromFile(path + "img\\noman_izq.bmp");
}
//---------------------------------------------------------------------------

void __fastcall TfrmMDIMain::opnAllClick(TObject *Sender)
{
 static bool check = 0;

  check = !check;

  if (check)
  {
    opnCmax->Checked = true;
    opnFmax->Checked = true;
    opnIdle->Checked = true;
    opnEmax->Checked = true;
    opnTmax->Checked = true;
    opnNE->Checked   = true;
    opnNT->Checked   = true;
    opnEn->Checked   = true;
  }
  else
  {
    opnFmax->Checked = false;
    opnIdle->Checked = false;
    opnEmax->Checked = false;
    opnTmax->Checked = false;
    opnNE->Checked   = false;
    opnNT->Checked   = false;
    opnEn->Checked   = false;
  }

  bool cmax, fmax, idle, emax, tmax, ne, nt, energy;

  cmax   = opnCmax->Checked;
  fmax   = opnFmax->Checked;
  idle   = opnIdle->Checked;
  emax   = opnEmax->Checked;
  tmax   = opnTmax->Checked;
  ne     = opnNE->Checked;
  nt     = opnNT->Checked;
  energy = opnEn->Checked;
  frmGanttDiagram->ShowObjetive(cmax, fmax, idle, emax, tmax, ne, nt, energy);
  frmGanttDiagram->ShowMeasureMark(opnViewMark->Checked);
  
  if (energy)
  {
    frmEnergyChart->FormStyle = fsMDIChild;
    frmEnergyChart->Show();
  }
  else
  {
    frmEnergyChart->FormStyle = fsNormal;
    frmEnergyChart->Hide();
  }
}
//---------------------------------------------------------------------------

void __fastcall TfrmMDIMain::opnCmaxClick(TObject *Sender)
{
  bool cmax, fmax, idle, emax, tmax, ne, nt, energy;

  cmax   = opnCmax->Checked;
  fmax   = opnFmax->Checked;
  idle   = opnIdle->Checked;
  emax   = opnEmax->Checked;
  tmax   = opnTmax->Checked;
  ne     = opnNE->Checked;
  nt     = opnNT->Checked;
  energy = opnEn->Checked;
  frmGanttDiagram->ShowObjetive(cmax, fmax, idle, emax, tmax, ne, nt, energy);
  frmGanttDiagram->ShowMeasureMark(opnViewMark->Checked);
  
  if (energy)
  {
    frmEnergyChart->FormStyle = fsMDIChild;
    frmEnergyChart->Show();
  }
  else
  {
    frmEnergyChart->FormStyle = fsNormal;
    frmEnergyChart->Hide();
  }
}
//---------------------------------------------------------------------------

void __fastcall TfrmMDIMain::opnViewMarkClick(TObject *Sender)
{
  opnViewMark->Checked = !opnViewMark->Checked;
  frmGanttDiagram->ShowMeasureMark(opnViewMark->Checked);
}
//---------------------------------------------------------------------------

void __fastcall TfrmMDIMain::opnApplyRulerFIFOClick(TObject *Sender)
{
  opnApplyRescheduling->Checked = false;
  opnApplyRescheduling->Enabled = !opnApplyRulerFIFO->Checked;

  if (CollectionP)
  {
    CollectionP->ApplyFifoRuler(opnApplyRulerFIFO->Checked);
    Problem_Type = CollectionP->ProblemType();
    switch (Problem_Type)
    {
      case PFSS: StatusBar->Panels->Items[1]->Text = " PFSS"; break;
      case FSS : StatusBar->Panels->Items[1]->Text = " FSS";  break;
      case GFSS: StatusBar->Panels->Items[1]->Text = " GFSS"; break;
      case JSS : StatusBar->Panels->Items[1]->Text = " JSS";  break;
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TfrmMDIMain::opnNoRecircClick(TObject *Sender)
{
  opnNoRecirc->Checked = !opnNoRecirc->Checked;
}
//---------------------------------------------------------------------------

