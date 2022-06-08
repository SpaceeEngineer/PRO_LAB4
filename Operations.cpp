//Operations.cpp

#include "Header.h"

long double** MulMatrixes(int size_row, int size_column, long double** matrix1, long double** matrix2)
{
	long double tmp = 0;
	long double size = 248;
	long double** result;
	InitMatrix(result, size_row, size_column);

	for (int k = 0; k < size_row; k++)
	{
		for (int l = 0; l < size_column; l++)
		{
			for (int s = 0; s < size; s++)
				tmp += matrix1[k][s] * matrix2[s][l];
			result[k][l] = tmp;
			tmp = 0;
		}
	}
	return result;//повертає матрицю
}
