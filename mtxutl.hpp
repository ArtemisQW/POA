#ifndef __MTXUTL__
#define __MTXUTL__

#include <cstdlib>
#include <cstdio>



char* AllocateCharVec(size_t len);
void FreeCharVec(char* v);

int* AllocateIntVec(size_t len);
void FreeIntVec(int* v);

int** AllocateIntMtx(size_t a, size_t b);
void FreeIntMtx(int** m);

#endif