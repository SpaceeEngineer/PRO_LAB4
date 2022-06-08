//Input.cpp

#include "Header.h"

void Fill_1(long double**& A, long double**& B)
{
	InitMatrix(A, row_1, column_1);
	InitMatrix(B, row_2, column_2);
	//generate A
	for (int i = 0; i < row_1; i++)
	{
		for (int j = 0; j < column_1; j++)
		{
			 A[i][j] = 1;
			// printf("A[%d][%d]= %d", i, j,j);
			// printf("\t");
		}
		//printf("\t");
		//printf("\n");
	}
	//generate B
	for (int i = 0; i < row_2; i++)
	{
		for (int j = 0; j < column_2; j++)
		{
			 B[i][j] = 1;
			// printf("B[%d][%d]= %d", i, j, j);
			// printf("\t");
		}
	//	printf("\t");
	//	printf("\n");
	}
}

void Fill_1_3(long double**& A, long double**& B)
{
	InitMatrix(A, row_1, column_1);
	InitMatrix(B, row_2, column_2);
	//generate A
	for (int i = 0; i < row_1; i++)
	{
		for (int j = 0; j < column_1; j++)
		{
			A[i][j] = 1;
			// printf("A[%d][%d]= %d", i, j,j);
			// printf("\t");
		}
		//printf("\t");
		//printf("\n");
	}
	//generate B
	for (int i = 0; i < row_2; i++)
	{
		for (int j = 0; j < column_2; j++)
		{
			B[i][j] = 3;
			// printf("B[%d][%d]= %d", i, j, j);
			// printf("\t");
		}
		//	printf("\t");
		//	printf("\n");
	}
}

void rand_0(long double**& A, long double**& B)
{
	srand(time(NULL));
	InitMatrix(A, row_1, column_1);
	InitMatrix(B, row_2, column_2);
	//generate A
	for (int i = 0; i < row_1; i++)
	{
		for (int j = 0; j < column_1; j++)
		{
			A[i][j] = 1 + rand() % 9;;
			// printf("A[%d][%d]= %d", i, j,j);
			// printf("\t");
		}
		//printf("\t");
		//printf("\n");
	}
	//generate B
	for (int i = 0; i < row_2; i++)
	{
		for (int j = 0; j < column_2; j++)
		{
			B[i][j] = 1 + rand() % 9;;
			// printf("B[%d][%d]= %d", i, j, j);
			// printf("\t");
		}
		//	printf("\t");
		//	printf("\n");
	}
}

void rand_1(long double**& A, long double**& B)
{
	srand(time(NULL));
	InitMatrix(A, row_1, column_1);
	InitMatrix(B, row_2, column_2);
	//generate A
	for (int i = 0; i < row_1; i++)
	{
		for (int j = 0; j < column_1; j++)
		{
			A[i][j] = 10 + rand() % 90;
			// printf("A[%d][%d]= %d", i, j,j);
			// printf("\t");
		}
		//printf("\t");
		//printf("\n");
	}
	//generate B
	for (int i = 0; i < row_2; i++)
	{
		for (int j = 0; j < column_2; j++)
		{
			B[i][j] = 10 + rand() % 90;
			// printf("B[%d][%d]= %d", i, j, j);
			// printf("\t");
		}
		//	printf("\t");
		//	printf("\n");
	}
}

void rand_2(long double**& A, long double**& B)
{
	srand(time(NULL));
	InitMatrix(A, row_1, column_1);
	InitMatrix(B, row_2, column_2);
	//generate A
	for (int i = 0; i < row_1; i++)
	{
		for (int j = 0; j < column_1; j++)
		{
			A[i][j] = 100 + rand() % 9900;
			// printf("A[%d][%d]= %d", i, j,j);
			// printf("\t");
		}
		//printf("\t");
		//printf("\n");
	}
	//generate B
	for (int i = 0; i < row_2; i++)
	{
		for (int j = 0; j < column_2; j++)
		{
			B[i][j] = 100 + rand() % 9900;
			// printf("B[%d][%d]= %d", i, j, j);
			// printf("\t");
		}
		//	printf("\t");
		//	printf("\n");
	}
}

void Input(long double**& A, long double**& B)
{
	int flag = 1;
	cout << " Enter 1 - fill matrix 1, 2 - fill matrix A 1 and B 3, 3 - positive rand 1..9, 4 - integer 10...99, 5 - double 100...9999  " << endl;
	cin >> flag;

	switch (flag)
	{
	case 1:
	{
		Fill_1(A, B);
		break;
	}
	case 2:
	{
		Fill_1_3(A, B);
		break;
	}
	case 3:
	{
		rand_0(A, B);
		break;
	}
	case 4:
	{
		rand_1(A, B);
		break;
	}
	case 5:
	{
		rand_2(A, B);
		break;
	}
	default:
	{
		Fill_1(A, B);
		break;
	}
	cout << endl;
	}
}