#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "CHndFile.h"
#include "../chm.utilities/gceUtiles.h"

//----------------------------------------------------------------------------
// Devuelve true si el argumento 'string' es una palabra reservada.
//----------------------------------------------------------------------------
bool CHndFile::isKeyWord (const char *string)
{
	for (int i = 0; i < 13; i++)
		if (strcmp(KeyWord[i], string) == 0) return 1;

	return 0;
}

//----------------------------------------------------------------------------
// Constructor por defecto.
//----------------------------------------------------------------------------
CHndFile::CHndFile (void)
{
	sFilename = NULL;
	file = NULL;

	strcpy(KeyWord[0],  "Pieces");
	strcpy(KeyWord[1],  "Machines");
	strcpy(KeyWord[2],  "[MACHINES]");
	strcpy(KeyWord[3],  "[NOMINAL-ENERGY]");
	strcpy(KeyWord[4],  "[WORK-CENTER]");
	strcpy(KeyWord[5],  "[PIECE]");
	strcpy(KeyWord[6],  "[End]");
	strcpy(KeyWord[7],  "Name");
	strcpy(KeyWord[8],  "ID");
	strcpy(KeyWord[9],  "Priority");
	strcpy(KeyWord[10], "DueDate");
	strcpy(KeyWord[11], "Penality");
	strcpy(KeyWord[12], "Reward");
}

//----------------------------------------------------------------------------
// Constructor.
//----------------------------------------------------------------------------
CHndFile::CHndFile (const char *path)
{
	file = NULL;
	sFilename = new char[strlen(path) + 1];
	strcpy(sFilename, path);
}

//----------------------------------------------------------------------------
// Destructor.
//----------------------------------------------------------------------------
CHndFile::~CHndFile (void)
{
	Close();

	// Libero la memoria del arreglo que contiene el nombre del fichero
	if (sFilename) delete []sFilename;
}

//----------------------------------------------------------------------------
// Establece la ruta del fichero.
//----------------------------------------------------------------------------
void CHndFile::SetPath (const char *path)
{
	if (sFilename == NULL)
	{
		sFilename = new char[strlen(path) + 1];
		strcpy(sFilename, path);
	}
	else
	{
		delete []sFilename;
		sFilename = new char[strlen(path) + 1];
		strcpy(sFilename, path);
	}
}

//----------------------------------------------------------------------------
// Devuelve la ruta del fichero.
//---------------------------------------------------------------------------- 
const char *CHndFile::GetPath (void) const
{
	return (sFilename) ? sFilename : '\0';
}

//----------------------------------------------------------------------------
// Devuelve el nombre del fichero. Si el parametro <extension> es TRUE la
// extension en incluida, casi sea FALSE, no la extension del fichero no forma
// parte del nombre.
//---------------------------------------------------------------------------- 
const char *CHndFile::GetFilename(bool extension) const
{
	char *name;
	int len_paht, len_name, i_point;

	if (!sFilename) return NULL;

	len_name = 0;
	len_paht = strlen(sFilename);
	for (int i = len_paht - 1; i >= 0; i--)
	{
		if (sFilename[i] == '\\') break;
		len_name++;
	}

	name = new char[len_name + 1];
	if (extension)
	{
		strcpy_s(name, len_name + 1, (sFilename + (len_paht - len_name)));
		name[len_name] = '\0';
	}
	else
	{
		strncpy_s(name, len_name + 1, (sFilename + (len_paht - len_name)), len_name - 4);
		name[len_name - 4] = '\0';
	}
	

	return name;
}

//----------------------------------------------------------------------------
//	Lee del fichero la cantidad de piezas. Devuelve 0 si la lectura fallo  
//	o no se encontro la etiqueta 'Pieces' y un valor mayor que cero que
//	representa la cantidad de piezas.
//----------------------------------------------------------------------------
int CHndFile::ReadPieceCount (void)
{
	char r_string[LINE_LENGTH];
	//FILE *file = fopen(sFilename, "rt");

	if (!file)
		file = fopen(sFilename, "rt");
	else
		fseek(file, 0, SEEK_SET);

	// Salgo de la funcion si no se pudo abrir el file
	if (!file) return 0;

	while (!feof(file))
	{
		fgets(r_string, LINE_LENGTH, file);	
			
		char *token = strtok(r_string, ":");			
			
		// Verifico si el token obtenido es la marca de: cantidad de piezas
		if (strstr(token, "Pieces") != NULL)
		{
			token = strtok(NULL, ":");
			Trim(token);
			//fclose(file);

			return (token) ? atoi(token) : 0;
		}
	}

	return 0;
}

//----------------------------------------------------------------------------
//	Lee del fichero la cantidad de maquinas. Devuelve 0 si la lectura fallo,  
//	o si no se encontro la etiqueta 'Machines' y un valor mayor que cero que
//	representa la cantidad de maquinas.
//----------------------------------------------------------------------------
int CHndFile::ReadMachineCount (void)
{
	char r_string[LINE_LENGTH];
	//FILE *file = fopen(sFilename, "rt");

	if (!file)
		file = fopen(sFilename, "rt");
	else
		fseek(file, 0, SEEK_SET);

	// Salgo de la funcion si no se pudo abrir el file
	if (!file) return 0;

	while (!feof(file))
	{
		fgets(r_string, LINE_LENGTH, file);
			
		char *token = strtok(r_string, ":");			
			
		// Verifico si la linea leida es la marca de: cantidad de operaciones
		if (strstr(r_string, "Machines") != NULL)
		{
			token = strtok(NULL, ":");
			Trim(token);
			//fclose(file);

			return (token) ? atoi(token) : 0;
		}
	}

	return 0;
}

//----------------------------------------------------------------------------
//	Lee del fichero las macros Pieces y Machine, los valores retornados
//	son devueltos por referencia.
//----------------------------------------------------------------------------
int CHndFile::ReadInstanceSize (int *job, int *machine)
{
	bool foundJ, foundM;
	char r_string[LINE_LENGTH];
	//FILE *file = fopen(sFilename, "rt");

	if (!file)
		file = fopen(sFilename, "rt");
	else
		fseek(file, 0, SEEK_SET);

	// Salgo de la funcion si no se pudo abrir el file
	if (job && machine) *job = *machine = -1;
	if (!file) return 0;

	foundJ = foundM = 0;
	while (!feof(file) || (foundJ != true && foundM != true))
	{
		fgets(r_string, LINE_LENGTH, file);			
		
		char *token = strtok(r_string, ":");			
			
		// Verifico si el token obtenido es la marca de: cantidad de piezas
		if (strstr(token, "Pieces") != NULL)
		{
			token = strtok(NULL, ":");
			Trim(token);

			if (job) *job = (token) ? atoi(token) : 0;
			foundJ = true;
		}
		if (strstr(r_string, "Machines") != NULL)
		{
			token = strtok(NULL, ":");
			Trim(token);

			if(machine) *machine = (token) ? atoi(token) : 0;
			foundM = true;
		}
	}

	//fclose(file);
	return (foundJ && foundM) ? 1 : 0;
}

//----------------------------------------------------------------------------
// Lee del fichero la definicion de las maquinas. Devuelve 1 si la lectura fue 
// satisfactoria, 0 en caso contrario.
//----------------------------------------------------------------------------
int CHndFile::ReadMachineData (CMachineCollection *m_collection)
{
	int count, index = 0, mCount;
	char r_string[LINE_LENGTH];
	bool go = false;
	//FILE *file = fopen(sFilename, "rt");
	
	if (!file)
		file = fopen(sFilename, "rt");
	else
		fseek(file, 0, SEEK_SET);

	// Salgo de la funcion si no se pudo abrir el file
	if (!file) return 0;
	
	/* Leo la definicion de las maquinas */
	count = mCount = ReadMachineCount();			// Obtengo la cantidad de maquinas definidas 
	
	while(!feof(file) && go == false)
	{
		fgets(r_string, LINE_LENGTH, file);
		Trim(r_string);
				
		// Es la linea leida la marca de definicion de maquinas?
		if (strcmp(r_string, "[MACHINES]") == 0)
		{
			while(!feof(file) && count > 0)
			{
				fgets(r_string, LINE_LENGTH, file);
				Trim(r_string);
					
				// Salgo de esta seccion si se encontro la marca que limita la definicion de maquinas
				if (strcmp(r_string, "[End]") == 0)
				{ 
					go = true;
					break;
				}
					
				if (*r_string != '\0')
				{
					m_collection->AddMachine(r_string);
					count--;
				}
			}

			go = true;
		}
	}

	/* Leo los valores de potencia nominal de las maquinas */
	count  = mCount;

	while(!feof(file))
	{
		fgets(r_string, LINE_LENGTH, file);
		Trim(r_string);

		// Es la linea leida la marca de potencia nominal?
		if (strcmp(r_string, "[NOMINAL-ENERGY]") == 0)
		{
			while(!feof(file) && count > 0)
			{
				fgets(r_string, LINE_LENGTH, file);
				Trim(r_string);
					
				// Salgo de la funcion si se encontro la marca que limita la definicion
				// de la potencia nominal
				if (strcmp(r_string, "[End]") == 0)
				{
					//fclose(file);
					return 1;
				}
					
				if (*r_string != '\0')
				{
					m_collection->SetNominalPower(index, (float)atof(r_string));
					count--;
					index++;
				}
			}

			//fclose(file);
			return 1;
		}
	}

	return 0;
}

//----------------------------------------------------------------------------
// Lee del fichero la definicion de los work-center o etapas. Devuelve 1 si 
// la lectura fue satisfactoria, 0 en caso contrario.
//----------------------------------------------------------------------------
int CHndFile::ReadWorkCenterData (CWorkCenterCollection *wc_collection)
{
	CWorkCenter WC;
	char r_string[LINE_LENGTH];
	//FILE *file = fopen(sFilename, "rt");
	
	if (!file)
		file = fopen(sFilename, "rt");
	else
		fseek(file, 0, SEEK_SET);

	// Salgo de la funcion si no se pudo abrir el file
	if (!file) return 0;
	
	while(!feof(file))
	{
		fgets(r_string, LINE_LENGTH, file);
		Trim(r_string);
				
		// Es la linea leida la marca de definicion de maquinas?
		if (strcmp(r_string, "[WORK-CENTER]") == 0)
		{
			while(!feof(file))
			{
				fgets(r_string, LINE_LENGTH, file);
								
				char *token = strtok(r_string, ":");
				Trim(token);

				if (*token == '\0') continue;

				if (strcmp(token, "Name") == 0)
				{
					token = strtok(NULL, ":");
					Trim(token);

					WC.SetOperationName(token);					
				}
				else if(strcmp(token, "[End]") == 0)
				{
					if (WC.GetMachineCount() > 0) wc_collection->AddWorKCenter(WC);
					WC.ClearCenter();
					break;
				}
				else if (!isKeyWord(token))
				{
					int c_token;
					char **token_list = ExplodeString(token, '|', &c_token);

					for (int i = 0; i < c_token; i++)
					{
						Trim(token_list[i]);
						if (*token_list[i] != '\0' && wc_collection->ExistsMachineInCollection(token_list[i]) == false) 
							WC.AddMachine(token_list[i]);
					}

					delete []token_list;
				}
			}		
		}
	}

	return 1;
}

//----------------------------------------------------------------------------
// Lee del fichero los datos de las piezas. Devuelve 1 si la lectura fue 
// satisfactoria, 0 en caso contrario.
//----------------------------------------------------------------------------
int CHndFile::ReadPieceData (CPieceCollection *p_collection, const CMachineCollection &m_collection)
{
	int maq_ok;
	float value;	
	unsigned id;
	//FILE *file = fopen(sFilename, "rt");
	char r_string[LINE_LENGTH], str_tmp[LINE_LENGTH];
	CPieceTechnologicalRoute P;	
	CMachineGroupExtended MGroup(true);

	// Protejo la funcion frente a una lista de maquinas vacia. Por lo que si esto 
	// ocurre aborto la ejecucion del programa.
	assert(m_collection.GetMachineCount() > 0);

	if (!file)
		file = fopen(sFilename, "rt");
	else
		fseek(file, 0, SEEK_SET);

	// Salgo de la funcion si no se pudo abrir el file
	if (!file) return 0;

	while (!feof(file))
	{
		fgets(r_string, LINE_LENGTH, file);	
		Trim(r_string);

		if (strcmp(r_string, "[PIECE]") == 0)
		{
			while (!feof(file))
			{
				fgets(r_string, LINE_LENGTH, file);
				Trim(r_string);

				if (*r_string != '\0')
				{
					strcpy(str_tmp, r_string);
					char *token = strtok(str_tmp, ":");
					Trim(token);

					//==== ¿Que etiqueta se leyo? ====/					
					if (strcmp(token, "[End]") == 0)							/* Fin de dato de la Pieza */
					{
						p_collection->AddPiece(P);
						P.ClearPiece();
						break;
					}
					else if (strcmp(token, "Name") == 0)					/* Nombre de la Pieza */
					{
						token = strtok(NULL, ":");
						Trim(token);
						P.SetPieceName(token);
					}
					else if (strcmp(token, "ID") == 0)						/* Id de la Pieza */
					{
						token = strtok(NULL, ":");
						Trim(token);						
						id = (token) ? atoi(token) : 0;
						P.SetPieceID(id);
					}
					else if (strcmp(token, "Priority") == 0)			/* Prioridad de la pieza */
					{
						token = strtok(NULL, ":");
						Trim(token);
						value = (token) ? (float)atof(token) : 0.0f;
						P.SetPiecePriority(value);
					}
					else if (strcmp(token, "DueDate") == 0)				/* Valor de duedate */
					{
						token = strtok(NULL, ":");
						Trim(token);
						value = (token) ? (float)atof(token) : 0.0f;
						P.SetDueDate(value);
					}
					else if (strcmp(token, "Penality") == 0)			/* Valor de penalizacion, si se pasa del duedate */
					{
						token = strtok(NULL, ":");
						Trim(token);
						value = (token) ? (float)atof(token) : 0.0f;
						P.SetPenality(value);
					}
					else if (strcmp(token, "Reward") == 0)				/* Valor de premio si se adelanta el duedate */
					{
						token = strtok(NULL, ":");
						Trim(token);
						value = (token) ? (float)atof(token) : 0.0f;
						P.SetReward(value);
					}
					else if (!isKeyWord(token))										/* ¿Es una operacion? --> Si no es una palabra clave */
					{
						int c_token;
						char **token_list = ExplodeString(r_string, '|', &c_token);
						
						for (int k = 0; k < c_token; k++)
						{
							char *m_name = strtok(token_list[k], ":");					
							char *time = strtok(NULL, ":");

							Trim(m_name);
							Trim(time);

							if (*m_name != '\0')
							{
								// Verifico que la maquina leida es valida
								maq_ok = m_collection.GetIndexByName(m_name);

								if (maq_ok != -1)
								{
									value = (*time != '\0') ? (float)atof(time) : 0.0f;
									MGroup.AddMachine(m_name, value);
								}
							}
						}
						
						P.AddMachineGroup(MGroup);
						MGroup.ClearGroup();
						delete []token_list; 
					}
				}
			}
		}
	}

	//fclose(file);
	return 1;
}

//----------------------------------------------------------------------------
// Lee del fichero los datos de las piezas. Devuelve 1 si la lectura fue 
// satisfactoria, -1 en caso contrario.
//----------------------------------------------------------------------------
int CHndFile::WriteData (const CMachineCollection &MachineList, const CPieceCollection &DataBase)
{  
	STRUCT_DATA_MACHINE DM;
	//FILE *file = fopen(sFilename, "wt");	
  CMachineGroupExtended *MGroup;
	CPieceTechnologicalRoute P;
	int i, pCount, mCount, opCount;

	if (!file)
		file = fopen(sFilename, "wt");
	else
		fseek(file, 0, SEEK_END);

  // Verifico que ha haya abierto correctamente el fichero, de no ser asi
  // retorno la funcion con un valor que indica el error
  if (!file) return 0;

	pCount = DataBase.GetPieceCount();
	mCount = MachineList.GetMachineCount();

	//	Se escribe los datos generales de la instancia
  fprintf(file, "Pieces  : %d\n", pCount);
  fprintf(file, "Machines: %d\n", mCount);
  
	//	Se escribe en el file los datos de las maquinas
	fprintf(file, "\n[MACHINES]\n");  
	for (i = 0; i < mCount; i++) 
		fprintf(file, "%s\n", MachineList.GetNameByIndex(i)); 
  fprintf(file, "[End]\n");
  
	//	Se escribe en el file las potencias nominales de las maquinas
	fprintf(file, "\n[NOMINAL-ENERGY]\n");  
	for (i = 0; i < mCount; i++) 
		fprintf(file, "%.f\n", MachineList.GetNominalPower(i));
	fprintf(file, "[End]");

  for (int p = 0; p < pCount; p++)
  {
    P = DataBase.GetPieceByIndex(p);
    fprintf(file, "\n\n[PIECE]\n");
		fprintf(file, "Name: %s\n", P.GetPieceName());
    fprintf(file, "ID: %d\n", P.GetPieceID());
		fprintf(file, "Priority: %.1f\n", P.GetPiecePriority());
		fprintf(file, "DueDate : %.1f\n", P.GetDueDate());
		fprintf(file, "Penality: %.1f\n", P.GetPenality());
		fprintf(file, "Reward  : %.1f\n\n", P.GetReward());
		
		opCount = P.GetOperationCount();
		for (int s = 0; s < opCount; s++)
		{
			MGroup = P.GetMachinesGroupByIndex(s);
		  mCount = MGroup->GetMachineCount();

			for (int m = 0; m < mCount; m++)
			{
				DM = MGroup->GetMachineByIndex(m);
				
				fprintf(file, "%s: %.1f", DM.MachineName, DM.Time);				
				if (mCount > 1 && m < mCount - 1) fprintf(file, " | ");				
			}

			fprintf(file, "\n");
		}
    
		fprintf(file, "[End]");
  }

  //fclose(file);
  return 1;
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
bool CHndFile::Close(void)
{
	int status = 0;

	if (file) status = fclose(file);
	file = NULL;

	return status == 0;
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
std::ostream& operator << (std::ostream &stream, const CHndFile &hnd)
{
	stream << hnd.GetPath();

	return stream;
}