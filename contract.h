#ifndef CONTRACT_H
#define CONTRACT_H
#include <windows.h>


// экспорт/импорт для dll
#ifdef BUILD_DLL
  #define API __declspec(dllexport)
#else
  #define API __declspec(dllimport)
#endif

API float Pi(int k);
API int* Sort(int* array, int size);

#endif
