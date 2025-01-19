#include "mtxutl.hpp"

char* AllocateCharVec(size_t len)
{
	char* vec;
	vec = (char*)calloc(len, sizeof(char));
	if (vec == nullptr)
	{
		fprintf(stderr, "Error: Allocation error (char). Program will exit.\n");
		exit(1);
	}
	return vec;
}

void FreeCharVec(char* v)
{
	free(v);
}

int* AllocateIntVec(size_t len)
{
	int* vec;
	vec = (int*)calloc(len, sizeof(int));
	if (vec == nullptr)
	{
		fprintf(stderr, "Error: Allocation error (vector). Program will exit.\n");
		exit(1);
	}
	return vec;
}

void FreeIntVec(int* v)
{
	free(v);
}

int** AllocateIntMtx(size_t a, size_t b)
{
	size_t i;
	int** mtx;

	mtx = (int**)calloc(a + 1, sizeof(int*));
	if (mtx == nullptr)
	{
		fprintf(stderr, "Error: Allocation error (matrix). Program will exit.\n");
		exit(1);
	}
	if (b)
	{
		for (i = 0; i < a; ++i) mtx[i] = AllocateIntVec(b);
	}
	else
	{
		for (i = 0; i < a; ++i) mtx[i] = nullptr;
	}
	mtx[a] = nullptr;

	return mtx;
}

void FreeIntMtx(int** m)
{
	size_t i;

	for (i = 0; m[i]; ++i)
		if (m[i]) { free(m[i]); m[i] = nullptr; }
	free(m);
}



