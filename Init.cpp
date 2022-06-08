//Init.cpp

#include "Header.h"

void InitMatrix(long double**& Name, int size_row,int size_column)
{
	Name = new long double* [size_row];
	for (int i = 0; i < size_row; i++)
	{
		Name[i] = new long double [size_column];
	}
}

