#include <assert.h>
#include "gaModule.h"

namespace GAModule {	// Start the NameSpace GAModule
	//------------------------------------------------------------------------------
	//	Variables Globales usadas por el GA
	//------------------------------------------------------------------------------
	float Bound = 0;
	CGraphCAPP *gaGraph;
	unsigned long IBUIT_Performance = 0;
	CMachineCollection *gaMaC;
	CPieceCollection *gaPiC;
	PRIORITY_LIST_PTR MyPriorityList = NULL;

	//------------------------------------------------------------------------------
	//	Esta funcion permite establecer desde fuera de este modulo el objeto grafo. 
	//------------------------------------------------------------------------------
	void SetGraph	(CGraphCAPP *Obj)
	{
		gaGraph = Obj;
	}

	//------------------------------------------------------------------------------
	//	Esta funcion permite establecer desde fuera de este modulo el objeto que 
	//	contiene la colecion de piezas.
	//------------------------------------------------------------------------------
	void SetPieceCollection (CPieceCollection *Obj)
	{
		gaPiC = Obj;
	}

	//------------------------------------------------------------------------------
	//	Esta funcion permite establecer desde fuera de este modulo el objeto que 
	//	contiene la colecion de maquinas.
	//------------------------------------------------------------------------------
	void SetMachineCollection (CMachineCollection *Obj)
	{
		gaMaC = Obj;
	}

	//------------------------------------------------------------------------------
	//	Establece la lista de prioridades que se usa para genera una secuencia
	//  con prioridad.
	//------------------------------------------------------------------------------
	void SetPriorityList (PRIORITY_LIST_PTR list)
	{
		MyPriorityList = list;
	}

	//------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------
	void SetBound (float value)
	{
		Bound = value;
	}

	//------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------
	unsigned long GetPerfomanceRescheduling (void)
	{
		return IBUIT_Performance;
	}
	
	//------------------------------------------------------------------------------
	//	Libera la lista de prioridades establecida.
	//------------------------------------------------------------------------------
	void ClearPriorityList (void)
	{
		FreePriorityList(&GAModule::MyPriorityList);
	}
}	// 	End the NameSpace GAModule


//======================================================================================================//
//------------------------------------------------------------------------------------------------------//
//																																																			//
//											Definicion de Funciones para los dos tipos de problemas.												//
//																																																			//
//------------------------------------------------------------------------------------------------------//
//======================================================================================================//

//------------------------------------------------------------------------------------------------
//	Extrae los genes de un objeto "GA1DArrayGenome" y los copia a array de unsigned.
//------------------------------------------------------------------------------------------------
void GetGenes (const GA1DArrayGenome <unsigned> &g, unsigned *Array) 
{
	for (int i = 0; i < g.length(); i++) Array[i] = g.gene(i);		
}


//======================================================================================================//
//------------------------------------------------------------------------------------------------------//
//																																																			//
//													Definicion de Funciones del GA para problemas FlowShop.											//
//																																			 																//
//------------------------------------------------------------------------------------------------------//	
//======================================================================================================//

//------------------------------------------------------------------------------------------------
//	Devuelve la posicion que ocupa un gen determindo dentro de un cromosoma
//  representado por un arreglo de unsigned.
//------------------------------------------------------------------------------------------------
int n_SearchGene (const unsigned *A_Genome, int length, unsigned gene, int init, int end)
{
	if (init < -1) init = -1;
	if (end < -1)  end  = -1;
	if (init >= length) init = length - 1;
	if (end  >= length) end  = length - 1;

	if (init == -1 && end == -1)
	{
		init = 0;
		end  = length;
		for (int i = init; i < end; i++)
			if (A_Genome[i] == gene) return i;				
	}
	else if (init == -1 && end != -1)
	{
		init = 0;
		for (int i = init; i < end; i++)
			if (A_Genome[i] == gene) return i;				
	}
	else if (init != -1 && end == -1)
	{
		end = length;
		for (int i = init; i < end; i++)
			if (A_Genome[i] == gene) return i;				
	}
	else
	{
		if (init > end) SWAP(init, end);
		for (int i = init; i < end; i++)
			if (A_Genome[i] == gene) return i;				
	}

	return -1;
}

//------------------------------------------------------------------------------------------------
//	Devuelve la posicion que ocupa un gen determindo dentro cromosoma
//  representado por un objeto GA1DArrayGenome.
//------------------------------------------------------------------------------------------------
int n_SearchGene (const GA1DArrayGenome <unsigned> &genome, unsigned gene, int init, int end)
{
	if (init < -1) init = -1;
	if (end < -1) end   = -1;
	if (init >= genome.length()) init = genome.length() - 1;
	if (end  >= genome.length()) end  = genome.length() - 1;

	if (init == -1 && end == -1)
	{
		init = 0;
		end  = genome.length();
		for (int i = init; i < end; i++)
			if (genome.gene(i) == gene)
				return i;
	}
	else if (init == -1 && end != -1)
	{
		init = 0;
		for (int i = init; i < end; i++)
			if (genome.gene(i) == gene)
				return i;
	}
	else if (init != -1 && end == -1)
	{
		end = genome.length();
		for (int i = init; i < end; i++)
			if (genome.gene(i) == gene)
				return i;
	}
	else
	{
		if (init > end) SWAP(init, end);
		for (int i = init; i < end; i++)
			if (genome.gene(i) == gene)
				return i;
	}

	return -1;
}

//------------------------------------------------------------------------------------------------
//	Genera cromosomas de manera aletoria.
//------------------------------------------------------------------------------------------------
void n_RandomGenome (unsigned *A_Genome, int Length, STRUCT_PROBLEM_DATA Inst)
{
	//--- Declaracion de funciones locales ---//
	int   PriorityCount        ( void );
	float NextPriority         ( void );
	int   SamePrioritySubList  ( float priority, PRIORITY_LIST_PTR *SubList );
	PRIORITY_LIST GetNodeByPos ( int pos, const PRIORITY_LIST_PTR SubList );
	
	int i, j, X;
	unsigned tmp;
	
	GARandomSeed();

	// Paso #1: Inicializo el array de genes con los IDs de los trabajos
	for (i = 0; i < Inst.Size.Piece; i++) A_Genome[i] = i + 1; 

	// Paso #2: Realizo intercambios al azar para obtener un cromosoma aleatorio.
	// Nota: esta solucion reemplazo aquella que habia copiado de Tomas, la cual
	//			 tenia el problema de la posposicion indefinida.	
	for (i = 0; i < Inst.Size.Piece; i++)
	{
		X = GARandomInt(0, Inst.Size.Piece - 1);
		
		tmp = A_Genome[i];
		A_Genome[i] = A_Genome[X];
		A_Genome[X] = tmp;
	}

	// Paso #3: Trato la recirculacion
	unsigned n_recirc = Inst.Recirc.tRecirc; 
	if (n_recirc != 0 && Inst.Recirc.P_Recirc != NULL && (Inst.Size.Piece + n_recirc) == Length)
	{		
		int index = Inst.Size.Piece;

		for (i = 0; i < Inst.Recirc.pRecirc; i++)
		{
			for (j = 0; j < Inst.Recirc.P_Recirc[i].nRecirc; j++)
			{
				A_Genome[index] = Inst.Recirc.P_Recirc[i].PieceID;
				index++;
			}
		}

		for (i = Inst.Size.Piece; i < Length; i++)
		{
			X = GARandomInt(Inst.Size.Piece, Length - 1);

			tmp = A_Genome[i];
			A_Genome[i] = A_Genome[X];
			A_Genome[X] = tmp;
		}
	}

	// Si las piezas presentan prioridad, entonces creo un mecanismo para tratar
	// de garantizar el orden de las piezas segun su prioridad. Y si hay piezas
	// con igual valor de prioridad, entonces se garantiza el posible orden de 
	// manera aleatoria.
	int c1, c2;
	unsigned *Indices;
	unsigned Equals, Idx;
	PRIORITY_LIST Node;
	PRIORITY_LIST_PTR PrioritySubList = NULL;
	
	//-- Preparo el mecanismo que producira los pequeños cambios que garantizaran la prioridad --//
	i = 0;
	c1 = PriorityCount();
	for (int j = 0; j < c1; j++)
	{
		float p = NextPriority();
		c2 = SamePrioritySubList(p, &PrioritySubList);
		if (c2 > 1)
		{
			Indices = new unsigned [c2];
			for(int m = 0; m < c2; m++) Indices[m] = 0; //Inicializo los indices a 0

			for (int k = 0; k < c2; k++)
			{
				do 
				{
					Equals = Idx = 0;
					X = GARandomInt(1, c2);
					while (Indices[Idx]) if (X == Indices[Idx++]) Equals = 1;	
				} 
				while (Equals);	
				Indices[k] = X;

				Node = GetNodeByPos(Indices[k], PrioritySubList);
				Idx = n_SearchGene(A_Genome, Inst.Size.Piece, Node.IdPiece);
				if (Idx != -1 && Node.IdPiece != A_Genome[i])
				{
					X = A_Genome[i];
					A_Genome[i] = A_Genome[Idx];
					A_Genome[Idx] = X;
				}

				i++;
			}
			delete []Indices;
			FreePriorityList(&PrioritySubList);
		}
		else
		{
			Idx = n_SearchGene(A_Genome, Inst.Size.Piece, PrioritySubList->IdPiece);
			if (Idx != -1 && PrioritySubList->IdPiece != A_Genome[i])
			{
				X = A_Genome[i];
				A_Genome[i] = A_Genome[Idx];
				A_Genome[Idx] = X;
			}

			i++;
			FreePriorityList(&PrioritySubList);
		}
	}
}

//------------------------------------------------------------------------------------------------
//	Inicializa un cromosoma.
//------------------------------------------------------------------------------------------------
void n_Initializer(GAGenome &genome) 
{
	GA1DArrayGenome <unsigned> &g = STA_CAST (GA1DArrayGenome <unsigned> &, genome);
	STRUCT_PROBLEM_DATA *inst = (STRUCT_PROBLEM_DATA *)g.userData();
	unsigned length = g.length();
	unsigned *Array = new unsigned [length];
			
	n_RandomGenome(Array, length, *inst);
	for(unsigned i = 0; i < length; i++) g.gene(i, Array[i]);

	delete []Array;
}

//------------------------------------------------------------------------------------------------
//	Funcion Objetivo.
//------------------------------------------------------------------------------------------------
float n_Fitness (GAGenome &genome)
{
	//-- Declaracion de funciones locales ---//
	float PriorityEvaluation (const unsigned *A_Genome, int length);
	
	GA1DArrayGenome <unsigned> &g = STA_CAST(GA1DArrayGenome <unsigned> &, genome);
	//GA1DArrayGenome <unsigned> &g = static_cast<GA1DArrayGenome <unsigned> &>(genome);
	//GA1DArrayGenome <unsigned> &g = (GA1DArrayGenome <unsigned> &)genome;
	float makespan, eval_priority;
	unsigned *Array = new unsigned [g.length()];

	makespan = eval_priority = 0;
	
	if (n_isValidGenome(g))
	{
		// Evaluo el Makespan de la secuencia
		GetGenes(g, Array);
		GAModule::gaPiC->Create_N_Graph(GAModule::gaGraph, GAModule::gaMaC, Array);
		GAModule::gaGraph->SetLinkToGenome(static_cast <GAGenome &> (genome));
		GAModule::gaGraph->SetLinkToPerformance(GAModule::IBUIT_Performance);
		makespan = 1.0f / GAModule::gaGraph->Makespan();	
		GAModule::gaGraph->ClearGraph();

		// Evaluo la secuencia atendiando al cumplimiento de la prioridad
		eval_priority = PriorityEvaluation(Array, g.length());

		delete []Array;
		return makespan + eval_priority;
	}

	return 0.0f;
}

//------------------------------------------------------------------------------------------------
//	Operador genetico de cruce. Un Punto de cruce.
//------------------------------------------------------------------------------------------------
int n_OnePointCrossover (const GAGenome &p1, const GAGenome &p2, GAGenome *c1, GAGenome *c2)
{
  unsigned InvalidGene;
	int nc = 0, a, length;
	const GA1DArrayGenome <unsigned> &mom = STA_CAST (const GA1DArrayGenome <unsigned> &, p1);
  const GA1DArrayGenome <unsigned> &dad = STA_CAST (const GA1DArrayGenome <unsigned> &, p2);
	STRUCT_PROBLEM_DATA *inst = (STRUCT_PROBLEM_DATA *)p1.userData();
	
	if (mom.length() != dad.length())
	{
		GAErr(GA_LOC, mom.className(), "one-point cross", gaErrBadParentLength);
    return nc;
  }

	a	= GARandomInt(0, (*inst).Size.Piece - 1);
	length = mom.length();	

	if (c1 && c2)
	{
		GA1DArrayGenome <unsigned> &sis = STA_CAST(GA1DArrayGenome <unsigned> &, *c1);
    GA1DArrayGenome <unsigned> &bro = STA_CAST(GA1DArrayGenome <unsigned> &, *c2);

		sis.userData(&(*inst));
		bro.userData(&(*inst));

		// Hago el cruzamiento
		sis.copy(mom, 0, 0, a);
		bro.copy(dad, 0, 0, a);
		sis.copy(dad, a, a, length);
		bro.copy(mom, a, a, length);

		// Chequeo validez del hijo # 1 y arreglo malformaciones
		if (!n_isValidSequence(sis, &InvalidGene)) n_FixGenome(sis, InvalidGene);
			
		// Chequeo validez del hijo # 2 y arreglo malformaciones
		if (!n_isValidSequence(bro, &InvalidGene)) n_FixGenome(bro, InvalidGene);
			
		nc = 2;
	}

	return nc;
}

//------------------------------------------------------------------------------------------------
//	Operador genetico de cruce. Dos Puntos de cruce.
//------------------------------------------------------------------------------------------------
int n_TwoPointCrossover (const GAGenome& p1, const GAGenome& p2, GAGenome* c1, GAGenome* c2)
{
	unsigned invalidGene;
	int nc = 0, a, b, length;
	const GA1DArrayGenome <unsigned> &mom = STA_CAST(const GA1DArrayGenome <unsigned> &, p1);
  const GA1DArrayGenome <unsigned> &dad = STA_CAST(const GA1DArrayGenome <unsigned> &, p2);
	STRUCT_PROBLEM_DATA *inst = (STRUCT_PROBLEM_DATA *)p1.userData();
	
	if (mom.length() != dad.length())
	{
		GAErr(GA_LOC, mom.className(), "two-point cross", gaErrBadParentLength);
    return nc;
  }

	length = mom.length();
	a = GARandomInt(0, (*inst).Size.Piece - 1);
	b = GARandomInt(0, (*inst).Size.Piece - 1);

	while(a == b) b = GARandomInt(0, (*inst).Size.Piece - 1);
	if (a > b) SWAP(a, b);  

	if (c1 && c2)
	{
		GA1DArrayGenome <unsigned> &sis = STA_CAST(GA1DArrayGenome <unsigned> &, *c1);
    GA1DArrayGenome <unsigned> &bro = STA_CAST(GA1DArrayGenome <unsigned> &, *c2);

		sis.userData(&(*inst));
		bro.userData(&(*inst));

		// Hago el cruzamiento
		sis.copy(mom, 0, 0, a);					//-- Fase # 1 --//
		bro.copy(dad, 0, 0, a);

		sis.copy(dad, a, a, b - a);			//-- Fase # 2 --//
		bro.copy(mom, a, a, b - a);

		sis.copy(mom, b, b, length);		//-- Fase # 3 --//
		bro.copy(dad, b, b, length);

		// Chequeo validez del hijo # 1 y arreglo malformaciones
		if (!n_isValidSequence(sis, &invalidGene)) n_FixGenome(sis, invalidGene);
			
		// Chequeo validez del hijo # 2 y arreglo malformaciones
		if (!n_isValidSequence(bro, &invalidGene)) n_FixGenome(bro, invalidGene);
			
		nc = 2;
	}

	return nc;
}

//------------------------------------------------------------------------------------------------
//	Operador genetico de cruce. Tres Puntos de cruce.
//------------------------------------------------------------------------------------------------
int n_ThreePointCrossover (const GAGenome& p1, const GAGenome& p2, GAGenome* c1, GAGenome* c2)
{
	unsigned invalidGene;
	int nc = 0, a, b, c, length;	
	const GA1DArrayGenome <unsigned> &mom = STA_CAST(const GA1DArrayGenome <unsigned> &, p1);
  const GA1DArrayGenome <unsigned> &dad = STA_CAST(const GA1DArrayGenome <unsigned> &, p2);
	STRUCT_PROBLEM_DATA *inst = (STRUCT_PROBLEM_DATA *)p1.userData();

	if (mom.length() != dad.length())
	{
		GAErr(GA_LOC, mom.className(), "three-point cross", gaErrBadParentLength);
    return nc;
  }

	length = mom.length();
	a = GARandomInt(0, (*inst).Size.Piece - 1);
	b = GARandomInt(0, (*inst).Size.Piece - 1);
	c = GARandomInt(0, (*inst).Size.Piece - 1);	

	while(a == b) b = GARandomInt(0, (*inst).Size.Piece - 1);
	while(b == c) c = GARandomInt(0, (*inst).Size.Piece - 1);
	while(c == a) a = GARandomInt(0, (*inst).Size.Piece - 1);
	if (c < a) SWAP(a, c);
	if (a > b) SWAP(a, b);
	if (b > c) SWAP(b, c); 

	if (c1 && c2)
	{
		GA1DArrayGenome <unsigned> &sis = STA_CAST(GA1DArrayGenome <unsigned> &, *c1);
    GA1DArrayGenome <unsigned> &bro = STA_CAST(GA1DArrayGenome <unsigned> &, *c2);

		sis.userData(&(*inst));
		bro.userData(&(*inst));

		// Hago el cruzamiento
		sis.copy(mom, 0, 0, a);					//-- Fase # 1 --//
		bro.copy(dad, 0, 0, a);

		sis.copy(dad, a, a, b - a);			//-- Fase # 2 --//
		bro.copy(mom, a, a, b - a);

		sis.copy(dad, b, b, c - b);			//-- Fase # 3 --//
		bro.copy(mom, b, b, c - b);

		sis.copy(mom, c, c, length);		//-- Fase # 4 --//
		bro.copy(dad, c, c, length);

		// Chequeo validez del hijo # 1 y arreglo malformaciones
		if (!n_isValidSequence(sis, &invalidGene)) n_FixGenome(sis, invalidGene);
			
		// Chequeo validez del hijo # 2 y arreglo malformaciones
		if (!n_isValidSequence(bro, &invalidGene)) n_FixGenome(bro, invalidGene);	

		nc = 2;
	}

	return nc;
}

//------------------------------------------------------------------------------------------------
//	Operador genetico de cruce. Cruce Parcialmente Mapeado (PMX).
//------------------------------------------------------------------------------------------------
int n_PMXCrossover (const GAGenome& p1, const GAGenome& p2, GAGenome* c1, GAGenome* c2)
{
	unsigned invalidGene;
	int nc = 0, a, b, length, index;	
	const GA1DArrayGenome <unsigned> &mom = STA_CAST(const GA1DArrayGenome <unsigned> &, p1);
  const GA1DArrayGenome <unsigned> &dad = STA_CAST(const GA1DArrayGenome <unsigned> &, p2);
	STRUCT_PROBLEM_DATA *inst = (STRUCT_PROBLEM_DATA *)p1.userData();

	if (mom.length() != dad.length())
	{
		GAErr(GA_LOC, mom.className(), "pmx cross", gaErrBadParentLength);
    return nc;
  }

	length = mom.length();
	a = GARandomInt(0, (*inst).Size.Piece - 1);
	b = GARandomInt(0, (*inst).Size.Piece - 1);
	while (a == b) b = GARandomInt(0, (*inst).Size.Piece - 1);
	if (a > b) SWAP(a, b);

	if (c1 && c2)
	{
		GA1DArrayGenome <unsigned> &sis = STA_CAST(GA1DArrayGenome <unsigned> &, *c1);
    GA1DArrayGenome <unsigned> &bro = STA_CAST(GA1DArrayGenome <unsigned> &, *c2);

		sis.userData(&(*inst));
		bro.userData(&(*inst));

		// Hago el cruzamiento
		// Paso # 1 - Se copia la subcadena A-B a cada unos de los hijos 
		sis.copy(mom, a, a, b - a + 1);
		bro.copy(dad, a, a, b - a + 1);

		// Paso # 2 
		for (int i = a; i <= b; i++)
		{
			if (n_SearchGene(mom, dad.gene(i), a, b) == -1)		//-- Copio los genes del padre que no coinciden con los --//
			{																									//-- copiados desde la madre en el intervalo A - B.     --//			
				index = n_SearchGene(dad, mom.gene(i));
				if (index != -1) sis.gene(index, dad.gene(i));
			}

			if (n_SearchGene(dad, mom.gene(i), a, b) == -1)		//-- Copio los genes de la madre que no coinciden con los --//
			{																									//-- copiados desde el padre en el intervalo A - B.       --//			
				index = n_SearchGene(mom, dad.gene(i));
				if (index != -1) bro.gene(index, mom.gene(i));
			}
		}

		// Paso # 3 - El resto de los genes se copia directamente de la siguiente manera:
		// dad --> sis, mom --> bro
		for (int j = 0; j < length; j++)
		{
			if (sis.gene(j) == 0) sis.gene(j, dad.gene(j));
			if (bro.gene(j) == 0) bro.gene(j, mom.gene(j));
		}

		// Chequeo validez del hijo # 1 y arreglo malformaciones
		if (!n_isValidSequence(sis, &invalidGene)) n_FixGenome(sis, invalidGene);
				
		// Chequeo validez del hijo # 2 y arreglo malformaciones
		if (!n_isValidSequence(bro, &invalidGene)) n_FixGenome(bro, invalidGene);
			
		nc = 2;
	}

	return nc;
}

//------------------------------------------------------------------------------------------------
//	Operador genetico de cruce. Cruce tipo Zig-Zag.
//------------------------------------------------------------------------------------------------ 
int n_ZigZagCrossover (const GAGenome& p1, const GAGenome& p2, GAGenome* c1, GAGenome* c2)
{
	GABool fM, fD;
	int nc = 0, iM, iD, iS, length;	
	const GA1DArrayGenome <unsigned> &mom = STA_CAST(const GA1DArrayGenome <unsigned> &, p1);
  const GA1DArrayGenome <unsigned> &dad = STA_CAST(const GA1DArrayGenome <unsigned> &, p2);
	STRUCT_PROBLEM_DATA *inst = (STRUCT_PROBLEM_DATA *)p1.userData();

	if (mom.length() != dad.length())
	{
		GAErr(GA_LOC, mom.className(), "zig-zag cross", gaErrBadParentLength);
    return nc;
  }

	if (c1 && c2)
	{
		GA1DArrayGenome <unsigned> &sis = STA_CAST(GA1DArrayGenome <unsigned> &, *c1);
    GA1DArrayGenome <unsigned> &bro = STA_CAST(GA1DArrayGenome <unsigned> &, *c2);

		sis.userData(&(*inst));
		bro.userData(&(*inst));

		//---------------------------//
		//--- Hago el cruzamiento ---//
		//---------------------------//
		//-- Creo el hijo # 1 --//
		iM = iD = iS = 0;
		fM = gaTrue;
		fD = gaFalse;
		length = (*inst).Size.Piece;

		//unsigned M[20], D[20], S[20], B[20];
		//GetGenes(mom, M);
		//GetGenes(dad, D);

		while (iS < length)
		{
			// La madre aporta un gen
			if (fM == gaTrue && n_SearchGene(sis, mom.gene(iM)) == -1)
			{
				sis.gene(iS, mom.gene(iM)); //GetGenes(sis, S);
				fM = gaFalse;
				fD = gaTrue;
				iM++;
				iS++;
			}
			else if (fM) iM++;

			// El padre aporta un gen
			if (fD == gaTrue && n_SearchGene(sis, dad.gene(iD)) == -1)
			{
				sis.gene(iS, dad.gene(iD)); //GetGenes(sis, S);
				fD = gaFalse;
				fM = gaTrue;
				iD++;
				iS++;
			}
			else if (fD) iD++;
		}

		//-- Creo el hijo # 2 --//
		iM = iD = iS = 0;
		fD = gaTrue;
		fM = gaFalse;
		length = (*inst).Size.Piece;
		
		while (iS < length)
		{
			// El padre aporta un gen
			if (fD == gaTrue && n_SearchGene(bro, dad.gene(iD)) == -1)
			{
				bro.gene(iS, dad.gene(iD)); //GetGenes(bro, B);
				fD = gaFalse;
				fM = gaTrue;
				iD++;
				iS++;
			}
			else if (fD) iD++;

			// La madre aporta un gen
			if (fM == gaTrue && n_SearchGene(bro, mom.gene(iM)) == -1)
			{
				bro.gene(iS, mom.gene(iM)); //GetGenes(bro, B);
				fM = gaFalse;
				fD = gaTrue;
				iM++;
				iS++;
			}
			else if (fM) iM++;
		}

		nc = 2;
	}

	return nc;
}

//------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------
bool fGATerminateByBound(GAGeneticAlgorithm &ga)
{
	float makespan = 0;	
	unsigned *Array;
	STRUCT_PROBLEM_DATA *inst = (STRUCT_PROBLEM_DATA *)ga.statistics().bestIndividual(0).userData();
	GA1DArrayGenome <unsigned> genome((*inst).Size.Piece);

	genome = ga.statistics().bestIndividual(0);

	if (n_isValidGenome(genome))
	{
		Array  = new unsigned [genome.length()];
		GetGenes(genome, Array);
		GAModule::gaPiC->Create_N_Graph(GAModule::gaGraph, GAModule::gaMaC, Array);
		makespan = GAModule::gaGraph->Makespan();	
		GAModule::gaGraph->ClearGraph();

		delete []Array;
	}
	
	return (makespan < GAModule::Bound) ? gaTrue : gaFalse;
}

//------------------------------------------------------------------------------------------------
//	Determina si el genoma pasado por parametro es valido.
//
//	Genoma valido: si no contiene ceros o un numero mayor que la cantidad
//								 de trabajos establecidos en la optimizacion.
//------------------------------------------------------------------------------------------------
bool n_isValidGenome (const GA1DArrayGenome <unsigned> &genome)
{
	unsigned length;
	STRUCT_PROBLEM_DATA *inst = (STRUCT_PROBLEM_DATA *)genome.userData();
	
	length = (*inst).Size.Piece;
	for (unsigned i = 0; i < length; i++)
		if (genome.gene(i) == 0 || genome.gene(i) > length) return false;

	return true;
} 

//------------------------------------------------------------------------------------------------
//	Determina si el genome pasado por parametro representa una secuencia de
//  de fabricacion valida.
//
//	Secuencia de fabricacion valida: si no presente trabajos repetidos.
//------------------------------------------------------------------------------------------------
bool n_isValidSequence (const GA1DArrayGenome <unsigned> &genome, unsigned *IG)
{
	int repeat, length;
	STRUCT_PROBLEM_DATA *inst = (STRUCT_PROBLEM_DATA *)genome.userData();

	length = (*inst).Size.Piece;
	for (int i = 0; i < length; i++)
	{
		repeat = 0;
		for (int j = 0; j < length; j++)
		{
			if (genome.gene(i) == genome.gene(j)) repeat++;
		}

		// Si repeat es mayor que 1 significa que el genome representa una secuencia
		// de fabricacion invalida. Genes repetidos.
		if (repeat > 1)
		{
			*IG = genome.gene(i); 
			return false; 
		}
	}
	
	*IG = -1;
	return true;
}

//------------------------------------------------------------------------------------------------
//	Ajusta un cromosoma hasta convertirlo en una secuencia de fabricacion
//	valida arreglando las malformaciones provenientes de los operadores de 
//	cruce.
//------------------------------------------------------------------------------------------------
void n_FixGenome (GA1DArrayGenome <unsigned> &genome, unsigned invalid_gene)
{
	//-- Declaracion de funciones locales --//
	void Add          ( GENE_LIST_PTR *L, unsigned Item );
	void DelEqual     ( GENE_LIST_PTR *L, unsigned Item );
	void FreeGeneList ( GENE_LIST_PTR *L );
	
	STRUCT_PROBLEM_DATA *inst = (STRUCT_PROBLEM_DATA *)genome.userData();

	GENE_LIST_PTR List = NULL;
	unsigned i, length = (*inst).Size.Piece;
	
	// Inicializo la lista imagen de genes validos
	for (i = 0; i < length; i++) Add(&List, i + 1);		

	// Identifico cuales son los genes que le falta al genoma para que sea valido
	for (i = 0; i < length; i++) DelEqual(&List, genome.gene(i));
		
	// Corrijo las malformaciones en el genoma
	for (i = length - 1; i >= 0; i--)
	{
		if (genome.gene(i) == invalid_gene)
		{
			genome.gene(i, List->Gene);
			break;
		}
	}

	// Recursivamente ajusto algun otro gen invalido que tenga este cromosoma 
	unsigned aux;
	if (!n_isValidSequence(genome, &aux)) n_FixGenome(genome, aux);
			
	// Libero la memoria utilizada por la lista
	FreeGeneList(&List);
}

//------------------------------------------------------------------------------------------------
//	Devuelve 'true' si los genomas pasados por parametros son iguales, 'false'
//	en caso contrario.
//------------------------------------------------------------------------------------------------
bool fEqualGenome (const GA1DArrayGenome <unsigned> &G1, const GA1DArrayGenome <unsigned> &G2)
{
	if (G1.length() != G2.length()) 
		return false;

	for (int i = 0; i < G1.length(); i++)
		if (G1.gene(i) != G2.gene(i)) return false;
	
	return true;
}


//======================================================================================================//
//------------------------------------------------------------------------------------------------------//
//																																																			//
//												Definicion de Funciones del GA para problemas JobShop.												//
//																																			 																//
//------------------------------------------------------------------------------------------------------//	
//======================================================================================================//

//------------------------------------------------------------------------------------------------
//	Devuelve la posicion que ocupa un gen determindo dentro de un cromosoma
//  representado por un arreglo de unsigned.
//------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------
//	Devuelve la posicion que ocupa un gen determindo dentro cromosoma
//  representado por un objeto GA1DArrayGenome.
//------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------
//	Genera cromosomas de manera aletoria repitiendo alelos en el gen.
//------------------------------------------------------------------------------------------------
void nxm_RandomGenomeCR (unsigned *A_Genome, STRUCT_PROBLEM_DATA Inst)
{
	unsigned X, tmp;
	int i, j, piece, machine, length;

	piece   = Inst.Size.Piece;
	machine = Inst.Size.Machine;
	length  = Inst.Size.Piece * Inst.Size.Machine + Inst.Recirc.tRecirc;

	GARandomSeed();

	// Paso #1: Inicializo los alelos con los Id de las piezas
	for (int m = 0; m < machine; m++)
		for (int p = 0; p < piece; p++)
			A_Genome[m * piece + p] = p + 1;

	// Paso #2: 
	for (i = 0; i < piece * machine; i++)
	{
		X = GARandomInt(0, piece * machine - 1);
		
		tmp = A_Genome[i];
		A_Genome[i] = A_Genome[X];
		A_Genome[X] = tmp;
	}

	// Paso #3: Trato la recirculacion
	unsigned n_recirc = Inst.Recirc.tRecirc; 
	if (n_recirc != 0 && Inst.Recirc.P_Recirc != NULL)
	{		
		int index = piece * machine;

		for (i = 0; i < Inst.Recirc.pRecirc; i++)
		{
			for (j = 0; j < Inst.Recirc.P_Recirc[i].nRecirc; j++)
			{
				A_Genome[index] = Inst.Recirc.P_Recirc[i].PieceID;
				index++;
			}
		}

		for (i = piece * machine; i < length; i++)
		{
			X = GARandomInt(piece * machine, length - 1);

			tmp = A_Genome[i];
			A_Genome[i] = A_Genome[X];
			A_Genome[X] = tmp;
		}
	}
}

//------------------------------------------------------------------------------------------------
//	Genera cromosomas de manera aletoria sin repetir de alelos en el gen.
//------------------------------------------------------------------------------------------------
void nxm_RandomGenomeSR (unsigned *A_Genome, STRUCT_PROBLEM_DATA Inst)
{
	unsigned *genes, tmp;
	int i, j, length, X, piece, machine;

	piece   = Inst.Size.Piece;
	machine = Inst.Size.Machine;
	genes	  = new unsigned[piece];
	length  = Inst.Size.Piece * Inst.Size.Machine + Inst.Recirc.tRecirc; 
	
	GARandomSeed();

	// Genero un cromosoma aleatorio
	for (int m = 0; m < machine; m++)
	{
		n_RandomGenome(genes, piece, Inst);
		for (int i = m * piece; i < m * piece + piece; i++)
		{
			A_Genome[i] = genes[i - (m * piece)];
			genes[i - (m * piece)] = 0;
		}
	}

	// Paso #2: Trato la recirculacion
	unsigned n_recirc = Inst.Recirc.tRecirc; 
	if (n_recirc != 0 && Inst.Recirc.P_Recirc != NULL)
	{		
		int index = piece * machine;

		for (i = 0; i < Inst.Recirc.pRecirc; i++)
		{
			for (j = 0; j < Inst.Recirc.P_Recirc[i].nRecirc; j++)
			{
				A_Genome[index] = Inst.Recirc.P_Recirc[i].PieceID;
				index++;
			}
		}

		for (i = piece * machine; i < length; i++)
		{
			X = GARandomInt(piece * machine, length - 1);

			tmp = A_Genome[i];
			A_Genome[i] = A_Genome[X];
			A_Genome[X] = tmp;
		}
	}

	delete []genes;
}

//------------------------------------------------------------------------------------------------
//	Inicializa un cromosoma.
//------------------------------------------------------------------------------------------------
void nxm_Initializer (GAGenome &genome)
{
	unsigned *Array, length;
	GA1DArrayGenome <unsigned> &g = STA_CAST (GA1DArrayGenome <unsigned> &, genome); 
	STRUCT_PROBLEM_DATA *inst = (STRUCT_PROBLEM_DATA *)g.userData();
	
	length = g.length();
	Array  = new unsigned [length];	

#if	REPETITION_IN_GENE
	nxm_RandomGenomeCR(Array, *inst);
#else
	nxm_RandomGenomeSR(Array, *inst);
#endif

	for(unsigned i = 0; i < length; i++) g.gene(i, Array[i]);

	delete []Array;
}

//------------------------------------------------------------------------------------------------
//	Funcion Objetivo.
//------------------------------------------------------------------------------------------------
float nxm_Fitness (GAGenome &genome)
{
	//-- Declaracion de funciones locales --//
	//float PriorityEvaluation (const unsigned *A_Genome, int length);
	
	GA1DArrayGenome <unsigned> &g = STA_CAST(GA1DArrayGenome <unsigned> &, genome);
	//GA1DArrayGenome <unsigned> &g = static_cast<GA1DArrayGenome <unsigned> &>(genome);
	//GA1DArrayGenome <unsigned> &g = (GA1DArrayGenome <unsigned> &)genome;
	float score, makespan, eval_priority;
	unsigned *Array = new unsigned [g.length()];

	makespan = eval_priority = 0;
	
	if (nxm_isValidGenome(g))
	{
		// Evaluo el Makespan de la secuencia
		GetGenes(g, Array);
		GAModule::gaPiC->Create_NxM_Graph(GAModule::gaGraph, GAModule::gaMaC, Array);
		GAModule::gaGraph->SetLinkToGenome(static_cast <GAGenome &> (genome));
		GAModule::gaGraph->SetLinkToPerformance(GAModule::IBUIT_Performance);
		
		makespan = GAModule::gaGraph->Makespan();
		score = 1.0f / makespan;	
		
		GAModule::gaGraph->ClearGraph();

		// Evaluo la secuencia atendiando al cumplimiento de la prioridad
		//eval_priority = PriorityEvaluation(Array, g.length());

		delete []Array;
		return score + eval_priority;
	}

	return 0.0f;
}

//------------------------------------------------------------------------------------------------
//	Operador genetico de cruce a nivel de gen. Un Punto de cruce.
//------------------------------------------------------------------------------------------------
int nxm_OnePointCrossoverGene (const GAGenome& p1, const GAGenome& p2, GAGenome* c1, GAGenome* c2)
{
	unsigned jixMoreOp, jixLessOp;
	int nc = 0, a, length, piece, machine;	
	const GA1DArrayGenome <unsigned> &mom = STA_CAST (const GA1DArrayGenome <unsigned> &, p1);
  const GA1DArrayGenome <unsigned> &dad = STA_CAST (const GA1DArrayGenome <unsigned> &, p2);
	STRUCT_PROBLEM_DATA *inst = (STRUCT_PROBLEM_DATA *)p1.userData();
	
	if (mom.length() != dad.length())
	{
		GAErr(GA_LOC, mom.className(), "one-point cross of gene level", gaErrBadParentLength);
    return nc;
  }

	length  = mom.length();
	piece		= (*inst).Size.Piece;
	machine = (*inst).Size.Machine; 
	a				= GARandomInt(0, machine - 1);

	if (c1 && c2)
	{
		GA1DArrayGenome <unsigned> &sis = STA_CAST(GA1DArrayGenome <unsigned> &, *c1);
    GA1DArrayGenome <unsigned> &bro = STA_CAST(GA1DArrayGenome <unsigned> &, *c2);
		
		sis.userData(&(*inst));
		bro.userData(&(*inst));

		a = ((a + 1) * piece);	// Esto convierte de pos-gen a pos-allele

		// Hago el cruzamiento
		sis.copy(mom, 0, 0, a);
		bro.copy(dad, 0, 0, a);
		sis.copy(dad, a, a, length);
		bro.copy(mom, a, a, length);

		// Chequeo validez del hijo # 1 y arreglo malformaciones
		if (!nxm_isValidSequenceCR(sis, &jixMoreOp, &jixLessOp)) 
			nxm_FixGenomeCR(sis, jixMoreOp, jixLessOp);
			
		// Chequeo validez del hijo # 2 y arreglo malformaciones
		if (!nxm_isValidSequenceCR(bro, &jixMoreOp, &jixLessOp)) 
			nxm_FixGenomeCR(bro, jixMoreOp, jixLessOp);
			
		nc = 2;
	}

	return nc;
}

//------------------------------------------------------------------------------------------------
//	Operador genetico de cruce a nivel de alelo. Un Punto de cruce.
//------------------------------------------------------------------------------------------------
int nxm_OnePointCrossoverAllele (const GAGenome& p1, const GAGenome& p2, GAGenome* c1, GAGenome* c2)
{
	unsigned jixMoreOp, jixLessOp;
	int nc = 0, a, length, piece, machine;	
	const GA1DArrayGenome <unsigned> &mom = STA_CAST (const GA1DArrayGenome <unsigned> &, p1);
  const GA1DArrayGenome <unsigned> &dad = STA_CAST (const GA1DArrayGenome <unsigned> &, p2);
	STRUCT_PROBLEM_DATA *inst = (STRUCT_PROBLEM_DATA *)p1.userData();
	
	if (mom.length() != dad.length())
	{
		GAErr(GA_LOC, mom.className(), "one-point cross of allele level", gaErrBadParentLength);
    return nc;
  }

	length  = mom.length();
	piece		= (*inst).Size.Piece;
	machine = (*inst).Size.Machine; 
	a				= GARandomInt(0, piece * machine - 1);

	if (c1 && c2)
	{
		GA1DArrayGenome <unsigned> &sis = STA_CAST(GA1DArrayGenome <unsigned> &, *c1);
    GA1DArrayGenome <unsigned> &bro = STA_CAST(GA1DArrayGenome <unsigned> &, *c2);
		
		sis.userData(&(*inst));
		bro.userData(&(*inst));

		// Hago el cruzamiento
		sis.copy(mom, 0, 0, a);
		bro.copy(dad, 0, 0, a);
		sis.copy(dad, a, a, length);
		bro.copy(mom, a, a, length);

		// Chequeo validez del hijo # 1 y arreglo malformaciones
		if (!nxm_isValidSequenceCR(sis, &jixMoreOp, &jixLessOp)) 
			nxm_FixGenomeCR(sis, jixMoreOp, jixLessOp);

		// Chequeo validez del hijo # 2 y arreglo malformaciones
		if (!nxm_isValidSequenceCR(bro, &jixMoreOp, &jixLessOp)) 
			nxm_FixGenomeCR(bro, jixMoreOp, jixLessOp);
			
		nc = 2;
	}

	return nc;
}

//------------------------------------------------------------------------------------------------
//	Operador genetico de cruce a nivel de gen. Dos Punto de cruce.
//------------------------------------------------------------------------------------------------
int nxm_TwoPointCrossoverGene (const GAGenome& p1, const GAGenome& p2, GAGenome* c1, GAGenome* c2)
{
	unsigned jixMoreOp, jixLessOp;
	int nc = 0, a, b, length, piece, machine;	
	const GA1DArrayGenome <unsigned> &mom = STA_CAST (const GA1DArrayGenome <unsigned> &, p1);
  const GA1DArrayGenome <unsigned> &dad = STA_CAST (const GA1DArrayGenome <unsigned> &, p2);
	STRUCT_PROBLEM_DATA *inst = (STRUCT_PROBLEM_DATA *)p1.userData();

	if (mom.length() != dad.length())
	{
		GAErr(GA_LOC, mom.className(), "two-point cross of gene level", gaErrBadParentLength);
    return nc;
  }

	length  = mom.length();
	piece		= (*inst).Size.Piece;
	machine = (*inst).Size.Machine; 
	a				= GARandomInt(0, machine - 1);
	b				= GARandomInt(0, machine - 1);

	while(a == b) b = GARandomInt(0, machine - 1);
	if (a > b) SWAP(a, b);

	if (c1 && c2)
	{
		GA1DArrayGenome <unsigned> &sis = STA_CAST(GA1DArrayGenome <unsigned> &, *c1);
    GA1DArrayGenome <unsigned> &bro = STA_CAST(GA1DArrayGenome <unsigned> &, *c2);

		sis.userData(&(*inst));
		bro.userData(&(*inst));

		a = ((a + 1) * piece);	// Esto convierte de pos-gen a pos-allele
		b = ((b + 1) * piece);	// Esto convierte de pos-gen a pos-allele

		// Hago el cruzamiento
		sis.copy(mom, 0, 0, a);					//-- Fase # 1 --//
		bro.copy(dad, 0, 0, a);

		sis.copy(dad, a, a, b - a);			//-- Fase # 2 --//
		bro.copy(mom, a, a, b - a);

		sis.copy(mom, b, b, length);		//-- Fase # 3 --//
		bro.copy(dad, b, b, length);

		// Chequeo validez del hijo # 1 y arreglo malformaciones
		if (!nxm_isValidSequenceCR(sis, &jixMoreOp, &jixLessOp)) 
			nxm_FixGenomeCR(sis, jixMoreOp, jixLessOp);
			
		// Chequeo validez del hijo # 2 y arreglo malformaciones
		if (!nxm_isValidSequenceCR(bro, &jixMoreOp, &jixLessOp)) 
			nxm_FixGenomeCR(bro, jixMoreOp, jixLessOp);
			
		nc = 2;
	}

	return nc;
}

//------------------------------------------------------------------------------------------------
//	Operador genetico de cruce a nivel de alelo. Dos Punto de cruce.
//------------------------------------------------------------------------------------------------
int	nxm_TwoPointCrossoverAllele (const GAGenome& p1, const GAGenome& p2, GAGenome* c1, GAGenome* c2)
{
	unsigned jixMoreOp, jixLessOp;
	int nc = 0, a, b, length, piece, machine;	
	const GA1DArrayGenome <unsigned> &mom = STA_CAST (const GA1DArrayGenome <unsigned> &, p1);
  const GA1DArrayGenome <unsigned> &dad = STA_CAST (const GA1DArrayGenome <unsigned> &, p2);
	STRUCT_PROBLEM_DATA *inst = (STRUCT_PROBLEM_DATA *)p1.userData();

	if (mom.length() != dad.length())
	{
		GAErr(GA_LOC, mom.className(), "two-point cross of allele level", gaErrBadParentLength);
    return nc;
  }

	length  = mom.length();
	piece		= (*inst).Size.Piece;
	machine = (*inst).Size.Machine; 
	a				= GARandomInt(0, piece * machine - 1);
	b				= GARandomInt(0, piece * machine - 1);

	while(a == b) b = GARandomInt(0, machine - 1);
	if (a > b) SWAP(a, b);

	if (c1 && c2)
	{
		GA1DArrayGenome <unsigned> &sis = STA_CAST(GA1DArrayGenome <unsigned> &, *c1);
    GA1DArrayGenome <unsigned> &bro = STA_CAST(GA1DArrayGenome <unsigned> &, *c2);

		sis.userData(&(*inst));
		bro.userData(&(*inst));

		// Hago el cruzamiento
		sis.copy(mom, 0, 0, a);					//-- Fase # 1 --//
		bro.copy(dad, 0, 0, a);

		sis.copy(dad, a, a, b - a);			//-- Fase # 2 --//
		bro.copy(mom, a, a, b - a);

		sis.copy(mom, b, b, length);		//-- Fase # 3 --//
		bro.copy(dad, b, b, length);

		// Chequeo validez del hijo # 1 y arreglo malformaciones
		if (!nxm_isValidSequenceCR(sis, &jixMoreOp, &jixLessOp)) 
			nxm_FixGenomeCR(sis, jixMoreOp, jixLessOp);
			
		// Chequeo validez del hijo # 2 y arreglo malformaciones
		if (!nxm_isValidSequenceCR(bro, &jixMoreOp, &jixLessOp)) 
			nxm_FixGenomeCR(bro, jixMoreOp, jixLessOp);
			
		nc = 2;
	}

	return nc;
}

//------------------------------------------------------------------------------------------------
//	Operador genetico de cruce a nivel de gen. Tres Punto de cruce.
//------------------------------------------------------------------------------------------------
int	nxm_ThreePointCrossoverGene (const GAGenome& p1, const GAGenome& p2, GAGenome* c1, GAGenome* c2)
{
	unsigned jixMoreOp, jixLessOp;
	int nc = 0, a, b, c, length, piece, machine;
	const GA1DArrayGenome <unsigned> &mom = STA_CAST (const GA1DArrayGenome <unsigned> &, p1);
  const GA1DArrayGenome <unsigned> &dad = STA_CAST (const GA1DArrayGenome <unsigned> &, p2);
	STRUCT_PROBLEM_DATA *inst = (STRUCT_PROBLEM_DATA *)p1.userData();

	if (mom.length() != dad.length())
	{
		GAErr(GA_LOC, mom.className(), "three-point cross of gene level", gaErrBadParentLength);
    return nc;
  };

	length  = mom.length();
	piece		= (*inst).Size.Piece;
	machine = (*inst).Size.Machine; 
	a				= GARandomInt(0, machine - 1);
	b				= GARandomInt(0, machine - 1);
	c				= GARandomInt(0, machine - 1);

	while(a == b) b = GARandomInt(0, machine - 1);
	while(b == c) c = GARandomInt(0, machine - 1);
	while(c == a) a = GARandomInt(0, machine - 1);
	if (c < a) SWAP(a, c);
	if (a > b) SWAP(a, b);
	if (b > c) SWAP(b, c);

	if (c1 && c2)
	{
		GA1DArrayGenome <unsigned> &sis = STA_CAST(GA1DArrayGenome <unsigned> &, *c1);
    GA1DArrayGenome <unsigned> &bro = STA_CAST(GA1DArrayGenome <unsigned> &, *c2);

		sis.userData(&(*inst));
		bro.userData(&(*inst));

		a = ((a + 1) * piece);	// Esto convierte de pos-gen a pos-allele
		b = ((b + 1) * piece);	// Esto convierte de pos-gen a pos-allele
		c = ((c + 1) * piece);	// Esto convierte de pos-gen a pos-allele

		// Hago el cruzamiento
		sis.copy(mom, 0, 0, a);					//-- Fase # 1 --//
		bro.copy(dad, 0, 0, a);

		sis.copy(dad, a, a, b - a);			//-- Fase # 2 --//
		bro.copy(mom, a, a, b - a);

		sis.copy(dad, b, b, c - b);			//-- Fase # 3 --//
		bro.copy(mom, b, b, c - b);

		sis.copy(mom, c, c, length);		//-- Fase # 4 --//
		bro.copy(dad, c, c, length);

		// Chequeo validez del hijo # 1 y arreglo malformaciones
		if (!nxm_isValidSequenceCR(sis, &jixMoreOp, &jixLessOp)) 
			nxm_FixGenomeCR(sis, jixMoreOp, jixLessOp);
			
		// Chequeo validez del hijo # 2 y arreglo malformaciones
		if (!nxm_isValidSequenceCR(bro, &jixMoreOp, &jixLessOp)) 
			nxm_FixGenomeCR(bro, jixMoreOp, jixLessOp);
			
		nc = 2;
	}

	return nc;
}

//------------------------------------------------------------------------------------------------
//	Operador genetico de cruce a nivel de alelo. Tres Punto de cruce.
//------------------------------------------------------------------------------------------------
int	nxm_ThreePointCrossoverAllele (const GAGenome& p1, const GAGenome& p2, GAGenome* c1, GAGenome* c2)
{
	unsigned jixMoreOp, jixLessOp;
	int nc = 0, a, b, c, length, piece, machine;
	const GA1DArrayGenome <unsigned> &mom = STA_CAST (const GA1DArrayGenome <unsigned> &, p1);
  const GA1DArrayGenome <unsigned> &dad = STA_CAST (const GA1DArrayGenome <unsigned> &, p2);
	STRUCT_PROBLEM_DATA *inst = (STRUCT_PROBLEM_DATA *)p1.userData();

	if (mom.length() != dad.length())
	{
		GAErr(GA_LOC, mom.className(), "three-point cross of gene level", gaErrBadParentLength);
    return nc;
  };

	length  = mom.length();
	piece		= (*inst).Size.Piece;
	machine = (*inst).Size.Machine; 
	a				= GARandomInt(0, piece * machine - 1);
	b				= GARandomInt(0, piece * machine - 1);
	c				= GARandomInt(0, piece * machine - 1);

	while(a == b) b = GARandomInt(0, machine - 1);
	while(b == c) c = GARandomInt(0, machine - 1);
	while(c == a) a = GARandomInt(0, machine - 1);
	if (c < a) SWAP(a, c);
	if (a > b) SWAP(a, b);
	if (b > c) SWAP(b, c);

	if (c1 && c2)
	{
		GA1DArrayGenome <unsigned> &sis = STA_CAST(GA1DArrayGenome <unsigned> &, *c1);
    GA1DArrayGenome <unsigned> &bro = STA_CAST(GA1DArrayGenome <unsigned> &, *c2);

		sis.userData(&(*inst));
		bro.userData(&(*inst));

		// Hago el cruzamiento
		sis.copy(mom, 0, 0, a);					//-- Fase # 1 --//
		bro.copy(dad, 0, 0, a);

		sis.copy(dad, a, a, b - a);			//-- Fase # 2 --//
		bro.copy(mom, a, a, b - a);

		sis.copy(dad, b, b, c - b);			//-- Fase # 3 --//
		bro.copy(mom, b, b, c - b);

		sis.copy(mom, c, c, length);		//-- Fase # 4 --//
		bro.copy(dad, c, c, length);

		// Chequeo validez del hijo # 1 y arreglo malformaciones
		if (!nxm_isValidSequenceCR(sis, &jixMoreOp, &jixLessOp)) 
			nxm_FixGenomeCR(sis, jixMoreOp, jixLessOp);
			
		// Chequeo validez del hijo # 2 y arreglo malformaciones
		if (!nxm_isValidSequenceCR(bro, &jixMoreOp, &jixLessOp)) 
			nxm_FixGenomeCR(bro, jixMoreOp, jixLessOp);
			
		nc = 2;
	}

	return nc;
}

//------------------------------------------------------------------------------------------------
//	Determina si el genoma pasado por parametro es valido.
//
//	Genoma valido: si no contiene ceros o un numero mayor que la cantidad
//								 de trabajos establecidos en la optimizacion.
//------------------------------------------------------------------------------------------------
bool nxm_isValidGenome (const GA1DArrayGenome <unsigned> &genome)
{
	int length;
	STRUCT_PROBLEM_DATA *inst = (STRUCT_PROBLEM_DATA *)genome.userData();
	
	length = genome.length();

	for (int i = 0; i < length; i++)
		if (genome.gene(i) == 0 || genome.gene(i) > (unsigned)(*inst).Size.Piece) return false;

	return true;
}

//------------------------------------------------------------------------------------------------
//	Determina si el genome pasado por parametro representa una secuencia de
//  de fabricacion valida.
//
//	Secuencia de fabricacion valida: si las operaciones para un trabajo es igual
//	que la cantidad de maquinas (N_op_J = m) ya que una de las restricciones
//	del problema es que no hay re-circulacion.
//------------------------------------------------------------------------------------------------
bool nxm_isValidSequenceCR (const GA1DArrayGenome <unsigned> &genome, unsigned *JixMoreOp, unsigned *JixLessOP)
{
	int length, i, j;
	int *Op_x_Job;
	int more, less;
	bool flag = true;	
	STRUCT_PROBLEM_DATA *inst = (STRUCT_PROBLEM_DATA *)genome.userData();

	more     = less = 0;
	length   = genome.length();
	Op_x_Job = new int[(*inst).Size.Piece + 1];

	// Nota: se usa el indice del arreglo para referirse a los trabajos 
	//			 por lo que el indice 0 no es usado.
	Op_x_Job[0] = 0;

	// Establesco la cantidad bruta de operaciones para cada pieza.
	for (i = 1; i <= (*inst).Size.Piece; i++)
	{
		Op_x_Job[i] = (*inst).Size.Machine;

		//if ((*inst).Recirc.pRecirc != 0 && (*inst).Recirc.P_Recirc != NULL)
		//{
			for (j = 0; j <= (*inst).Recirc.pRecirc; j++)
			{
				if (i == (*inst).Recirc.P_Recirc[j].PieceID)
				{
					Op_x_Job[i] += (*inst).Recirc.P_Recirc[j].nRecirc;
					break;
				}
			}
		//}
	}
		
	// Se determina la presencia de operaciones x pieza en el cromosoma.
	for (i = 0; i < length; i++) Op_x_Job[genome.gene(i)]--;
			
	// Si alguna pieza tiene un valor distinto de cero significa que la
	// secuencia de fabricacion es invalida. O sea, a algun trabajo le sobran
	// operaciones y a otro le faltan.
	for (i = 1; i <= (*inst).Size.Piece && flag == true; i++) 
		if (Op_x_Job[i] != 0) flag = false;
		
	// Se determina los 1ros dos trabajos que hacen invalida a la secuencia de
	// fabricacion por que les sobran y le falten operaciones respectivamente.
	if (flag == false)
	{
		i = 1;
		while (i <= (*inst).Size.Piece && (more == 0 || less == 0))
		{
			if (Op_x_Job[i] < 0 && more == 0) more = i; 
			if (Op_x_Job[i] > 0 && less == 0) less = i;
			i++;
		}

		*JixMoreOp = more;
		*JixLessOP = less;
	}

	delete []Op_x_Job;
	return flag;
}

//------------------------------------------------------------------------------------------------
//
//
//
//------------------------------------------------------------------------------------------------
bool nxm_isValidSequenceSR	(const GA1DArrayGenome <unsigned> &genome, int *IG, unsigned *JixMoreOp, unsigned *JixLessOP)
{
	bool flag = true;

	return flag;
}

//------------------------------------------------------------------------------------------------
//	Ajusta un cromosoma hasta convertirlo en una secuencia de fabricacion
//	valida arreglando las malformaciones provenientes de los operadores de 
//	cruce.
//------------------------------------------------------------------------------------------------
void nxm_FixGenomeCR (GA1DArrayGenome <unsigned> &genome, unsigned JixMoreOp, unsigned JixLessOP)
{
	unsigned jix_more_op, jix_less_op;
	int length, op_more, op_less, fixM, fixL, i;	
	STRUCT_PROBLEM_DATA *inst = (STRUCT_PROBLEM_DATA *)genome.userData();

	op_more = op_less = 0;
	length  = genome.length();
	
	for (i = 0; i < length; i++)
	{
		if (genome.gene(i) == JixMoreOp) op_more++;
		if (genome.gene(i) == JixLessOP) op_less++;
	}
		
	bool flagM, flagL;

	flagM = flagL = false;
	for (i = 0; i < (*inst).Recirc.pRecirc; i++)
	{
		if (JixMoreOp == (*inst).Recirc.P_Recirc[i].PieceID)
		{
			fixM = op_more - ((*inst).Size.Machine + (*inst).Recirc.P_Recirc[i].nRecirc);
			flagM = true;
			continue;
		}

		if (JixLessOP == (*inst).Recirc.P_Recirc[i].PieceID)
		{
			fixL = (*inst).Size.Machine - (op_less - (*inst).Recirc.P_Recirc[i].nRecirc); 
			flagL = true;
			continue;
		}
	}
	
	if (flagM == false) fixM = op_more - (*inst).Size.Machine;
	if (flagL == false) fixL = (*inst).Size.Machine - op_less; 

	for (i = length - 1; i >= 0 && fixM > 0 && fixL > 0; i--)
	{
		if (genome.gene(i) == JixMoreOp)
		{
			genome.gene(i, JixLessOP);
			fixM--;
			fixL--;
		}
	}

	// Recursivamente arreglo el resto de las malformaciones del cromosoma
	if (!nxm_isValidSequenceCR(genome, &jix_more_op, &jix_less_op)) 
		nxm_FixGenomeCR(genome, jix_more_op, jix_less_op);
}

//------------------------------------------------------------------------------------------------
//
//
//
//------------------------------------------------------------------------------------------------
void nxm_FixGenomeSR (GA1DArrayGenome <unsigned> &genome, unsigned JixMoreOp, unsigned JixLessOP)
{

}

//------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------
int nxm_SwapMutatorGene(GAGenome &g, float pmut)
{
	STRUCT_PROBLEM_DATA *inst = (STRUCT_PROBLEM_DATA *)g.userData();
	GA1DArrayGenome <unsigned> &genome = STA_CAST (GA1DArrayGenome <unsigned> &, g);
  
	int gene = (*inst).Size.Machine;
	float nMut = pmut * STA_CAST(float, genome.length());

	/*if(nMut < 1.0) // we have to do a flip test on each bit
	{		
    nMut = 0;
    for(i=length; i>=0; i--)
		{
      if(GAFlipCoin(pmut))
			{
				child.swap(i, GARandomInt(0, length));
				nMut++;
      }
    }
  }
  else
	{				// only flip the number of bits we need to flip
    for(n=0; n<nMut; n++)
      child.swap(GARandomInt(0, length), GARandomInt(0, length));
  }*/
  
	return(STA_CAST(int, nMut));
}

//------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------
bool nxm_GATerminateByBound(GAGeneticAlgorithm &ga)
{
	float makespan = 0;	
	unsigned *Array;
	STRUCT_PROBLEM_DATA *inst = (STRUCT_PROBLEM_DATA *)ga.statistics().bestIndividual(0).userData();
	GA1DArrayGenome <unsigned> genome((*inst).Size.Piece * (*inst).Size.Machine);

	genome = ga.statistics().bestIndividual(0);

	if (nxm_isValidGenome(genome))
	{
		Array  = new unsigned [genome.length()];
		GetGenes(genome, Array);
		GAModule::gaPiC->Create_NxM_Graph(GAModule::gaGraph, GAModule::gaMaC, Array);
		GAModule::gaGraph->SetLinkToGenome(static_cast <GAGenome &> (genome));
		makespan = GAModule::gaGraph->Makespan();	
		GAModule::gaGraph->ClearGraph();

		delete []Array;
	}
	
	return (makespan < GAModule::Bound) ? gaTrue : gaFalse;
}

//======================================================================================================//
//------------------------------------------------------------------------------------------------------//
//																																																			//
//										Funciones para la manipulacion de listas de Genes y Listas de											//
//										Prioridad.																																				//
//																																			 																//
//------------------------------------------------------------------------------------------------------//	
//======================================================================================================//

//------------------------------------------------------------------------------------------------
// Agrega un nuevo nodo a la lista de genes.
//------------------------------------------------------------------------------------------------
void Add (GENE_LIST_PTR *L, unsigned Item)
{
	GENE_LIST_PTR NewNode = new GENE_LIST;
	GENE_LIST_PTR CurrentNode;

	// Inicializo el nuevo nodo
	if (NewNode)
	{
		NewNode->Gene = Item;
		NewNode->Next = NULL;
	}
	
	// Lo inserto en la lista
	if (*L == NULL)
		*L = NewNode;
	else
	{
		CurrentNode = *L;
		while (CurrentNode->Next != NULL)
			CurrentNode = CurrentNode->Next;

		CurrentNode->Next = NewNode;
	}
}

//------------------------------------------------------------------------------------------------
//	Elimina de la lista el elemento que sea igual a <Item>.
//------------------------------------------------------------------------------------------------
void DelEqual (GENE_LIST_PTR *L, unsigned Item)
{
	GENE_LIST_PTR ptrCurrent, ptrDel, ptrBck;
	
	ptrCurrent = *L;
	ptrBck = ptrCurrent;
	
	// El elemento a eleminar es el primero de la lista
	if (ptrCurrent != NULL && ptrCurrent->Gene == Item)
	{
		ptrDel = ptrCurrent;
		*L = ptrCurrent->Next;
		delete ptrDel;
	}
	else	// El elemento a eliminar esta en el resto de la lista
	{
		while (ptrCurrent != NULL && ptrCurrent->Gene != Item)
		{
			ptrBck = ptrCurrent;
			ptrCurrent = ptrCurrent->Next;
		}

		if (ptrCurrent != NULL && ptrCurrent->Gene == Item)
		{
			ptrDel = ptrCurrent;
			ptrBck->Next = ptrCurrent->Next;
			delete ptrDel;
		}
	}
}

//------------------------------------------------------------------------------------------------
//	Libera la memoria usada por la lista de genes.
//------------------------------------------------------------------------------------------------
void FreeGeneList (GENE_LIST_PTR *L)
{
	GENE_LIST_PTR ptrCurrent, ptrDel;

	ptrCurrent = ptrDel = *L;
	while (ptrCurrent != NULL)
	{
		ptrDel = ptrCurrent;
		ptrCurrent = ptrCurrent->Next;
		delete ptrDel;
	}
	*L = NULL;
}

//------------------------------------------------------------------------------------------------
// Devuelve la cantidad de nodos que presentan diferente valor de prioridad.
//------------------------------------------------------------------------------------------------
int PriorityCount (void)
{
	PRIORITY_LIST_PTR CurrentNode = GAModule::MyPriorityList;
	float priority;
	int count = 0;

	if (CurrentNode == NULL) return 0;
	
	priority = CurrentNode->Priority;
	CurrentNode = CurrentNode->Next;
	count++;
	while (CurrentNode != NULL)
	{
		if (priority != CurrentNode->Priority)
		{
			count++;
			priority = CurrentNode->Priority;
		}
		CurrentNode = CurrentNode->Next;
	}

	return count;
}

//------------------------------------------------------------------------------------------------
// Crea una sublista con las piezas que tienen la misma prioridad que el valor
// pasado por parametro.
//------------------------------------------------------------------------------------------------
int SamePrioritySubList (float priority, PRIORITY_LIST_PTR *SubList)
{
	PRIORITY_LIST_PTR NewNode, CurrentNode;
	static PRIORITY_LIST_PTR RememberNode = NULL;
	int count = 0;

	if (RememberNode == NULL)
		CurrentNode = GAModule::MyPriorityList;
	else
		CurrentNode = RememberNode;

	while (CurrentNode != NULL && CurrentNode->Priority == priority)
	{
		NewNode = new PRIORITY_LIST;
		assert(NewNode != NULL);	//-- Protejo la ejecucion por si no hay memoria disponible --//
		NewNode->IdPiece  = CurrentNode->IdPiece;
		NewNode->Priority = CurrentNode->Priority;
		NewNode->Next = NULL;

		// Inserto el nuevo nodo en la sublista
		if (*SubList == NULL)
			*SubList = NewNode;
		else
		{
			NewNode->Next = *SubList;
			*SubList = NewNode;
		}

		CurrentNode = CurrentNode->Next;
		count++;
	}

	RememberNode = CurrentNode;
	return count;
}

//------------------------------------------------------------------------------------------------
// Devuelve los valores de las prioridades que sean diferente de una en una 
// con sucesivas llamadas a esta funcion.
//------------------------------------------------------------------------------------------------
float NextPriority (void)
{
	static PRIORITY_LIST_PTR RememberNode = NULL;
	float priority;

	if (RememberNode == NULL) RememberNode = GAModule::MyPriorityList;	
	priority = RememberNode->Priority;
	while (RememberNode != NULL && priority == RememberNode->Priority)
		RememberNode = RememberNode->Next;

	return priority;
}

//------------------------------------------------------------------------------------------------
// Devuelve un nodo dado su posicion dentro de la sublista determinada por el
// parametro < pos >.
//------------------------------------------------------------------------------------------------
PRIORITY_LIST GetNodeByPos (int pos, const PRIORITY_LIST_PTR SubList)
{
	PRIORITY_LIST_PTR CurrentNode = SubList;

	for (int i = 1; i < pos; i++) CurrentNode = CurrentNode->Next;

	return *CurrentNode;
}

//------------------------------------------------------------------------------------------------
// Evalua una secuencia atendiendo a que tan cerca estan las piezas de cumplir 
// con una prioridad dada.
//------------------------------------------------------------------------------------------------
float PriorityEvaluation (const unsigned *A_Genome, int length)
{
	//--- Declaracion de funciones locales ---//
	void PriorityPositionRange (const PRIORITY_LIST AnalizedNode, int *same_priority, int *upper_priority);

	PRIORITY_LIST_PTR CurrentNode = GAModule::MyPriorityList;
	float value = 0;
	int same_P, upper_P, index;

	while(CurrentNode != NULL)
	{
		PriorityPositionRange(*CurrentNode, &same_P, &upper_P);
		index = n_SearchGene(A_Genome, length, CurrentNode->IdPiece);

		if (index < upper_P)											//-- Caso # 1: El gen analizado esta a la izquierda de su lugar o del --//
		{																					//--            rango de posibles posiciones que podria ocupar.       --//
			value += (1.0f / (upper_P - index + 1));
		}
		else if (index > upper_P + same_P)				//-- Caso # 2: El gen analizado esta a la derecha de su lugar o del   --//
		{																					//--            rango de posibles posiciones que podria ocupar.       --//
			value += (1.0f / (index + 1 - (upper_P + same_P)));
		}
		else																			//-- Caso # 3: En gen analizado esta en su lugar o en el rango de     --//
		{																					//--           posibles posiciones que podria ocupar.					  	    --//	
			value += 1.0f;
		}

		CurrentNode = CurrentNode->Next;
	}

	return value;
}

//------------------------------------------------------------------------------------------------
// Determina cuantos nodos tienen la misma prioridad que el nodo < AnalizedNode >
// pasado por parametro y cuantos nodos tienen una prioridad mayor a este.
//------------------------------------------------------------------------------------------------
void PriorityPositionRange (const PRIORITY_LIST AnalizedNode, int *same_priority, int *upper_priority)
{
	PRIORITY_LIST_PTR CurrentNode = GAModule::MyPriorityList;

	*same_priority = *upper_priority = 0;
	while(CurrentNode != NULL)
	{
		if (CurrentNode->IdPiece != AnalizedNode.IdPiece && CurrentNode->Priority > AnalizedNode.Priority)
			(*upper_priority)++;
		if (CurrentNode->IdPiece != AnalizedNode.IdPiece && CurrentNode->Priority == AnalizedNode.Priority)
			(*same_priority)++;

		CurrentNode = CurrentNode->Next;
	}
}

void Print_NxM_Cromosome1 (const GA1DArrayGenome <unsigned> &g)
{
	int p, m;
	unsigned *seq;
	STRUCT_PROBLEM_DATA *inst = (STRUCT_PROBLEM_DATA *)g.userData();

	p = (*inst).Size.Piece;
	m = (*inst).Size.Machine;
	seq = new unsigned[p * m + (*inst).Recirc.tRecirc];

	GetGenes(g, seq);

	for	(int i = 0; i < m; i++)
	{
		for	(int j = 0; j < p; j++)
		{
			printf("%d ", seq[i * p + j]);	
		}

		if (i < m - 1) printf("- ");
	}
	
	if ((*inst).Recirc.tRecirc)
	{
		printf("- { ");
		for (int k = p * m; k < p * m + (*inst).Recirc.tRecirc; k++)
		{
			printf("%d ", seq[k]);
		}
		printf("}");
	}

	printf("\n");

	delete []seq;
}