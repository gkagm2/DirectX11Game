#pragma once

#ifdef DLL_EXPORTS
#define MYDLL __declspec(dllexport)
#else
#define MYDLL __declspec(dllimport)
#endif

extern "C" MYDLL int Mul(int a, int b);