#include <stdio.h>
#include <conio.h>
#include "../jssp.model/CHndFile.h"

void main (void)
{
	CHndFile A;
	int P, M;

	A.SetPath("datafile\\JSSP\\jobshop\\4x4_1.txt");
	printf("FILE: %s\n\n", A.GetPath());
	A.ReadInstanceSize(&P, &M);	

	printf("METODO BRKGA Nuevo\n");	
	_getch();
}