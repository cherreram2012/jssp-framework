#include "PrecisionClock.h"

//----------------------------------------------------------------------------------
// Retorna "B - A" en segundos com  precision de microsegundos. Tecnologia HIGH_PERFORMACE_COUNTER.
double performancecounter_diff(LARGE_INTEGER *A, LARGE_INTEGER *B)
{
	LARGE_INTEGER freq;
	QueryPerformanceFrequency(&freq);

	return (double)(B->QuadPart - A->QuadPart) / (double)freq.QuadPart;
}
