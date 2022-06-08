//Proc5.cpp

#include "Header.h"


void CPU_5()
{
	MPI_Barrier(MPI_COMM_WORLD);
	cout << endl;
	MPI_Status status;
	
	long double** A;
	long double** B;

	InitMatrix(A, (row_1 / 8), column_1);			//28 рядків 248 стовпців
	InitMatrix(B, row_2, column_2 / 8);				//248 рядків 31 стовпців
	int x = 0;
	for (int i = ((row_1 / 8) * 7) + 6; i < row_1; i++)
	{
		MPI_Recv(&(A[x][0]), column_1, MPI_LONG_DOUBLE, 7, tagA5, MPI_COMM_WORLD, &status);
		x++;
	}
	WriteToFile("Matrix", 5, A, (row_1 / 8), column_1, "Matrix A: ");

	for (int i = 0; i < row_2; i++)
		MPI_Recv(&(B[i][0]), column_2 / 8, MPI_LONG_DOUBLE, 7, tagB5, MPI_COMM_WORLD, &status);
	WriteToFile("Matrix", 5, B, row_2, column_2 / 8, "Matrix B: ");


	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//									Розрізаємо матрицю А на 1/8
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	long double** Ax5;

	InitMatrix(Ax5, row_1 / 8, column_1);

	for (int i = 0; i < (row_1 / 8); i++)
	{
		for (int j = 0; j < column_1; j++)
		{
			Ax5[i][j] = A[i][j];
		}
	}
	WriteToFile("Matrix", 5, Ax5, row_1 / 8, column_1, "Matrix Ax5 : ");
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//									Розрізаємо матрицю B на 1/8 
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	long double** Bx5;

	InitMatrix(Bx5, row_2, column_2 / 8);

	for (int i = 0; i < row_2; i++)
	{
		for (int j = 0; j < (column_2 / 8) ; j++)
		{
			Bx5[i][j] = B[i][j];
		}
	}
	WriteToFile("Matrix", 5, Bx5, row_2, column_2 / 8, "Matrix Bx5 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										Множення матриці Ax5 * Bx5
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Ax5_Bx5;
	InitMatrix(Ax5_Bx5, row_1 / 8, column_2 / 8);
	Ax5_Bx5 = MulMatrixes(row_1 / 8, column_2 / 8, Ax5, Bx5);
	WriteToFile("Matrix", 5, Ax5_Bx5, row_1 / 8, column_2 / 8, "Matrix Ax5 * Bx5 : ");
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
		//										Відправляємо смугу Bx5 в 6 процес
		/////////////////////////////////////////////////////////////////////////////////////////////////////////
	for (int i = 0; i < row_2; i++)
	{
		MPI_Send(&(Bx5[i][0]), column_2 / 8, MPI_LONG_DOUBLE, 6, tagBx5, MPI_COMM_WORLD);
	}
	cout << "Send Bx5 from processor with rank 5 to processor with rank 6" << endl;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										Приймаємо смугу Bx4 від 4 процесу
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Bx4;
	InitMatrix(Bx4, row_2, column_2 / 8);
	for (int i = 0; i < row_2; i++)
	{
		MPI_Recv(&(Bx4[i][0]), column_2 / 8, MPI_LONG_DOUBLE, 4, tagBx4, MPI_COMM_WORLD, &status);
	}
	WriteToFile("Matrix", 5, Bx4, row_2, column_2 / 8, "Matrix Bx4 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										Множення матриці Ax5 * Bx4
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Ax5_Bx4;
	InitMatrix(Ax5_Bx4, row_1 / 8, column_2 / 8);
	Ax5_Bx4 = MulMatrixes(row_1 / 8, column_2 / 8, Ax5, Bx4);
	WriteToFile("Matrix", 5, Ax5_Bx4, row_1 / 8, column_2 / 8, "Matrix Ax5 * Bx4 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
//										Відправляємо смугу Bx4 в 6 процес
/////////////////////////////////////////////////////////////////////////////////////////////////////////
	for (int i = 0; i < row_2; i++)
	{
		MPI_Send(&(Bx4[i][0]), column_2 / 8, MPI_LONG_DOUBLE, 6, tagBx4, MPI_COMM_WORLD);
	}
	cout << "Send Bx4 from processor with rank 5 to processor with rank 6" << endl;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										Приймаємо смугу Bx3 від 4 процесу
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Bx3;
	InitMatrix(Bx3, row_2, column_2 / 8);
	for (int i = 0; i < row_2; i++)
	{
		MPI_Recv(&(Bx3[i][0]), column_2 / 8, MPI_LONG_DOUBLE, 4, tagBx3, MPI_COMM_WORLD, &status);
	}
	WriteToFile("Matrix", 5, Bx3, row_2, column_2 / 8, "Matrix Bx3 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										Множення матриці Ax5 * Bx3
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Ax5_Bx3;
	InitMatrix(Ax5_Bx3, row_1 / 8, column_2 / 8);
	Ax5_Bx3 = MulMatrixes(row_1 / 8, column_2 / 8, Ax5, Bx3);
	WriteToFile("Matrix", 5, Ax5_Bx3, row_1 / 8, column_2 / 8, "Matrix Ax5 * Bx3 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										Відправляємо смугу Bx3 в 6 процес
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	for (int i = 0; i < row_2; i++)
	{
		MPI_Send(&(Bx3[i][0]), column_2 / 8, MPI_LONG_DOUBLE, 6, tagBx3, MPI_COMM_WORLD);
	}
	cout << "Send Bx3 from processor with rank 5 to processor with rank 6" << endl;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										Приймаємо смугу Bx2 від 4 процесу
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Bx2;
	InitMatrix(Bx2, row_2, column_2 / 8);
	for (int i = 0; i < row_2; i++)
	{
		MPI_Recv(&(Bx2[i][0]), column_2 / 8, MPI_LONG_DOUBLE, 4, tagBx2, MPI_COMM_WORLD, &status);
	}
	WriteToFile("Matrix", 5, Bx2, row_2, column_2 / 8, "Matrix Bx2 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										Множення матриці Ax5 * Bx2
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Ax5_Bx2;
	InitMatrix(Ax5_Bx2, row_1 / 8, column_2 / 8);
	Ax5_Bx2 = MulMatrixes(row_1 / 8, column_2 / 8, Ax5, Bx2);
	WriteToFile("Matrix", 5, Ax5_Bx2, row_1 / 8, column_2 / 8, "Matrix Ax5 * Bx2 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										Відправляємо смугу Bx2 в 6 процес
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	for (int i = 0; i < row_2; i++)
	{
		MPI_Send(&(Bx2[i][0]), column_2 / 8, MPI_LONG_DOUBLE, 6, tagBx2, MPI_COMM_WORLD);
	}
	cout << "Send Bx2 from processor with rank 5 to processor with rank 6" << endl;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										Приймаємо смугу Bx1 від 4 процесу
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Bx1;
	InitMatrix(Bx1, row_2, column_2 / 8);
	for (int i = 0; i < row_2; i++)
	{
		MPI_Recv(&(Bx1[i][0]), column_2 / 8, MPI_LONG_DOUBLE, 4, tagBx1, MPI_COMM_WORLD, &status);
	}
	WriteToFile("Matrix", 5, Bx1, row_2, column_2 / 8, "Matrix Bx1 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										Множення матриці Ax5 * Bx1
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Ax5_Bx1;
	InitMatrix(Ax5_Bx1, row_1 / 8, column_2 / 8);
	Ax5_Bx1 = MulMatrixes(row_1 / 8, column_2 / 8, Ax5, Bx1);
	WriteToFile("Matrix", 5, Ax5_Bx1, row_1 / 8, column_2 / 8, "Matrix Ax5 * Bx1 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										Відправляємо смугу Bx1 в 7 процес
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	for (int i = 0; i < row_2; i++)
	{
		MPI_Send(&(Bx1[i][0]), column_2 / 8, MPI_LONG_DOUBLE, 7, tagBx1, MPI_COMM_WORLD);
	}
	cout << "Send Bx1 from processor with rank 5 to processor with rank 7" << endl;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										Приймаємо смугу Bx7 від 3 процесу
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Bx7;
	InitMatrix(Bx7, row_2, column_2 / 8);
	for (int i = 0; i < row_2; i++)
	{
		MPI_Recv(&(Bx7[i][0]), column_2 / 8, MPI_LONG_DOUBLE, 3, tagBx7, MPI_COMM_WORLD, &status);
	}
	WriteToFile("Matrix", 5, Bx7, row_2, column_2 / 8, "Matrix Bx7 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										Множення матриці Ax5 * Bx7
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Ax5_Bx7;
	InitMatrix(Ax5_Bx7, row_1 / 8, column_2 / 8);
	Ax5_Bx7 = MulMatrixes(row_1 / 8, column_2 / 8, Ax5, Bx7);
	WriteToFile("Matrix", 5, Ax5_Bx7, row_1 / 8, column_2 / 8, "Matrix Ax5 * Bx7 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										Відправляємо смугу Bx7 в 6 процес
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	for (int i = 0; i < row_2; i++)
	{
		MPI_Send(&(Bx7[i][0]), column_2 / 8, MPI_LONG_DOUBLE, 6, tagBx7, MPI_COMM_WORLD);
	}
	cout << "Send Bx7 from processor with rank 5 to processor with rank 6" << endl;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										Приймаємо смугу Bx6 від 4 процесу
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Bx6;
	InitMatrix(Bx6, row_2, column_2 / 8);
	for (int i = 0; i < row_2; i++)
	{
		MPI_Recv(&(Bx6[i][0]), column_2 / 8, MPI_LONG_DOUBLE, 4, tagBx6, MPI_COMM_WORLD, &status);
	}
	WriteToFile("Matrix", 5, Bx6, row_2, column_2 / 8, "Matrix Bx6 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										Множення матриці Ax5 * Bx6
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Ax5_Bx6;
	InitMatrix(Ax5_Bx6, row_1 / 8, column_2 / 8);
	Ax5_Bx6 = MulMatrixes(row_1 / 8, column_2 / 8, Ax5, Bx6);
	WriteToFile("Matrix", 5, Ax5_Bx6, row_1 / 8, column_2 / 8, "Matrix Ax5 * Bx6 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										Відправляємо смугу Bx6 в 3 процес
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	for (int i = 0; i < row_2; i++)
	{
		MPI_Send(&(Bx6[i][0]), column_2 / 8, MPI_LONG_DOUBLE, 3, tagBx6, MPI_COMM_WORLD);
	}
	cout << "Send Bx6 from processor with rank 5 to processor with rank 3" << endl;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										Приймаємо смугу Bx0 від 7 процесу  DONE
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Bx0;
	InitMatrix(Bx0, row_2, (column_2 / 8) + 3);
	for (int i = 0; i < row_2; i++)
	{
		MPI_Recv(&(Bx0[i][0]), (column_2 / 8) + 3, MPI_LONG_DOUBLE, 7, tagBx0, MPI_COMM_WORLD, &status);
	}
	WriteToFile("Matrix", 5, Bx0, row_2, (column_2 / 8) + 3, "Matrix Bx0 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										Множення матриці Ax5 * Bx0
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Ax5_Bx0;
	InitMatrix(Ax5_Bx0, row_1 / 8, (column_2 / 8) + 3);
	Ax5_Bx0 = MulMatrixes(row_1 / 8, (column_2 / 8) + 3, Ax5, Bx0);
	WriteToFile("Matrix", 5, Ax5_Bx0, row_1 / 8, (column_2 / 8) + 3, "Matrix Ax5 * Bx0 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
//										Збираю підматрицю до купи	
/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** proc5_res;
	InitMatrix(proc5_res, row_1 / 8, column_2);
	int point = 0;
	for (int i = 0; i < (row_1 / 8); i++)
	{
		for (int j = 0; j < (column_2 / 8) + 3; j++)	//Тоді заповняємо матрицю від 0 до 34 елемента стовпця
		{
			proc5_res[i][j] = Ax5_Bx0[i][j];
		}
	}

	for (int i = 0; i < (row_1 / 8); i++)
	{
		for (int j = (column_2 / 8) + 3; j < ((column_2 / 8) * 2) + 3; j++)
		{
			proc5_res[i][j] = Ax5_Bx1[i][point];
			point++;
		}
		point = 0;
	}
	for (int i = 0; i < (row_1 / 8); i++)
	{
		for (int j = ((column_2 / 8) * 2) + 3; j < ((column_2 / 8) * 3) + 3; j++)
		{
			proc5_res[i][j] = Ax5_Bx2[i][point];

			point++;
		}
		point = 0;
	}
	for (int i = 0; i < (row_1 / 8); i++)
	{
		for (int j = ((column_2 / 8) * 3) + 3; j < ((column_2 / 8) * 4) + 3; j++)
		{
			proc5_res[i][j] = Ax5_Bx3[i][point];
			point++;
		}
		point = 0;
	}
	for (int i = 0; i < (row_1 / 8); i++)
	{
		for (int j = ((column_2 / 8) * 4) + 3; j < ((column_2 / 8) * 5) + 3; j++)
		{
			proc5_res[i][j] = Ax5_Bx4[i][point];
			point++;
		}
		point = 0;
	}
	for (int i = 0; i < (row_1 / 8); i++)
	{
		for (int j = ((column_2 / 8) * 5) + 3; j < ((column_2 / 8) * 6) + 3; j++)
		{
			proc5_res[i][j] = Ax5_Bx5[i][point];
			point++;
		}
		point = 0;
	}
	for (int i = 0; i < (row_1 / 8); i++)
	{
		for (int j = ((column_2 / 8) * 6) + 3; j < ((column_2 / 8) * 7) + 3; j++)
		{
			proc5_res[i][j] = Ax5_Bx6[i][point];
			point++;
		}
		point = 0;
	}
	for (int i = 0; i < (row_1 / 8); i++)
	{
		for (int j = ((column_2 / 8) * 7) + 3; j < column_2; j++)
		{
			proc5_res[i][j] = Ax5_Bx7[i][point];

			point++;
		}
		point = 0;
	}

	WriteToFile("Matrix", 5, proc5_res, (row_1 / 8), column_2, "Matrix proc5_res : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										Перекидаю підматриці між процесами	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	for (int i = 0; i < (row_1 / 8); i++)
	{
		MPI_Send(&(proc5_res[i][0]), column_2, MPI_LONG_DOUBLE, 7, tagA5, MPI_COMM_WORLD);
	}
	cout << "Send proc5_res from processor with rank 5 to processor with rank 7" << endl;

}