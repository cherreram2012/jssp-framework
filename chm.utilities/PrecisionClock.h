#pragma once
#ifndef HighPerformanceCounter_H
#define HighPerformanceCounter_H

#include <windows.h>

#define TIME_HIGH_PERFORMANCE(section_code, var)    \
{																										\
  LARGE_INTEGER start, end;													\
  QueryPerformanceCounter(&start);									\
  section_code;                                     \
  QueryPerformanceCounter(&end);										\
  var = performancecounter_diff(&start, &end);			\
}

double performancecounter_diff(LARGE_INTEGER *a, LARGE_INTEGER *b);

#endif