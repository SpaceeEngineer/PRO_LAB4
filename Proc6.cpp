//Proc6.cpp

#include "Header.h"


void CPU_6()
{
	MPI_Barrier(MPI_COMM_WORLD);
	cout << endl;
	MPI_Status status;

	long double** A;
	long double** B;
	InitMatrix(A, (row_1 / 8) * 3, column_1);	//84 рядків 248 стовпців
	InitMatrix(B, row_2, (column_2 / 8) * 3);	//248 рядків 93 стовпців
	int x = 0;
	for (int i = ((row_1 / 8) * 5) + 6; i < row_1; i++)
	{
		MPI_Recv(&(A[x][0]), column_1, MPI_LONG_DOUBLE, 4, tagA6, MPI_COMM_WORLD, &status);
		x++;
	}
	WriteToFile("Matrix", 6, A, (row_1 / 8) * 3, column_1, "Matrix A: ");

	for (int i = 0; i < row_2; i++)
		MPI_Recv(&(B[i][0]), (column_2 / 8) * 3, MPI_LONG_DOUBLE, 4, tagB6, MPI_COMM_WORLD, &status);
	WriteToFile("Matrix", 6, B, row_2, (column_2 / 8) * 3, "Matrix B: ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//									Розрізаємо матрицю А на 1/8
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	long double** Ax6;

	InitMatrix(Ax6, row_1 / 8, column_1);

	for (int i = 0; i < (row_1 / 8); i++)
	{
		for (int j = 0; j < column_1; j++)
		{
			Ax6[i][j] = A[i][j];
		}
	}
	WriteToFile("Matrix", 6, Ax6, row_1 / 8, column_1, "Matrix Ax6 : ");
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//									Розрізаємо матрицю B на 1/8 
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	long double** Bx6;

	InitMatrix(Bx6, row_2, column_2 / 8);

	for (int i = 0; i < row_2; i++)
	{
		for (int j = 0; j < (column_2 / 8) ; j++)
		{
			Bx6[i][j] = B[i][j];
		}
	}
	WriteToFile("Matrix", 6, Bx6, row_2, column_2 / 8, "Matrix Bx6 : ");
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//									Відправляємо смуги матриці А по процесам лишаючи собі 1/8 матриці
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	long double** A7;
	InitMatrix(A7, (row_1 / 8) * 2, column_1);		
	int l = 0;
	for (int i = (row_1 / 8); i < (row_1 / 8) * 3; i++)		//від 34 рядка заповняю матрицю А1
	{
		for (int j = 0; j < column_1; j++)
		{
			A7[l][j] = A[i][j];
		}
		l++;
	}
	WriteToFile("Matrix", 6, A7, (row_1 / 8) * 2, column_1, "Matrix A7: ");
	for (int i = 0; i < (row_1 / 8) * 2; i++)
	{
		MPI_Send(&(A7[i][0]), column_1, MPI_LONG_DOUBLE, 7, tagA7, MPI_COMM_WORLD);
	}
	cout << "Send submatrix A7 from processor with rank 6 to processor with rank 7" << endl;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										Відправляємо смуги матриці B по процесам лишаючи собі 1/8 матриці
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	long double** B7;
	InitMatrix(B7, row_2, (column_2 / 8) * 2);
	int k = 0;
	for (int i = 0; i < row_2; i++)
	{
		for (int j = (column_2 / 8); j < (column_2 / 8) * 3; j++)
		{
			B7[i][k] = B[i][j];
			k++;
		}
		k = 0;
	}
	WriteToFile("Matrix", 6, B7, row_2, (column_2 / 8) * 2, "Matrix B7: ");
	for (int i = 0; i < row_2; i++)
	{
		MPI_Send(&(B7[i][0]), (column_2 / 8) * 2, MPI_LONG_DOUBLE, 7, tagB7, MPI_COMM_WORLD);
	}
	cout << "Send submatrix B7 from processor with rank 6 to processor with rank 7" << endl;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
//										Множення матриці Ax6 * Bx6
/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Ax6_Bx6;
	InitMatrix(Ax6_Bx6, row_1 / 8, column_2 / 8);
	Ax6_Bx6 = MulMatrixes(row_1 / 8, column_2 / 8, Ax6, Bx6);
	WriteToFile("Matrix", 6, Ax6_Bx6, row_1 / 8, column_2 / 8, "Matrix Ax6 * Bx6 : ");
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
		//										Відправляємо смугу Bx6 в 7 процес
		/////////////////////////////////////////////////////////////////////////////////////////////////////////
	for (int i = 0; i < row_2; i++)
	{
		MPI_Send(&(Bx6[i][0]), column_2 / 8, MPI_LONG_DOUBLE, 7, tagBx6, MPI_COMM_WORLD);
	}
	cout << "Send Bx6 from processor with rank 6 to processor with rank 7" << endl;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										Приймаємо смугу Bx5 від 5 процесу
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Bx5;
	InitMatrix(Bx5, row_2, column_2 / 8);
	for (int i = 0; i < row_2; i++)
	{
		MPI_Recv(&(Bx5[i][0]), column_2 / 8, MPI_LONG_DOUBLE, 5, tagBx5, MPI_COMM_WORLD, &status);
	}
	WriteToFile("Matrix", 6, Bx5, row_2, column_2 / 8, "Matrix Bx5 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										Множення матриці Ax6 * Bx5
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Ax6_Bx5;
	InitMatrix(Ax6_Bx5, row_1 / 8, column_2 / 8);
	Ax6_Bx5 = MulMatrixes(row_1 / 8, column_2 / 8, Ax6, Bx5);
	WriteToFile("Matrix", 6, Ax6_Bx5, row_1 / 8, column_2 / 8, "Matrix Ax6 * Bx5 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
//										Відправляємо смугу Bx5 в 7 процес
/////////////////////////////////////////////////////////////////////////////////////////////////////////
	for (int i = 0; i < row_2; i++)
	{
		MPI_Send(&(Bx5[i][0]), column_2 / 8, MPI_LONG_DOUBLE, 7, tagBx5, MPI_COMM_WORLD);
	}
	cout << "Send Bx5 from processor with rank 6 to processor with rank 7" << endl;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										Приймаємо смугу Bx4 від 5 процесу
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Bx4;
	InitMatrix(Bx4, row_2, column_2 / 8);
	for (int i = 0; i < row_2; i++)
	{
		MPI_Recv(&(Bx4[i][0]), column_2 / 8, MPI_LONG_DOUBLE, 5, tagBx4, MPI_COMM_WORLD, &status);
	}
	WriteToFile("Matrix", 6, Bx4, row_2, column_2 / 8, "Matrix Bx4 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										Множення матриці Ax6 * Bx4
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Ax6_Bx4;
	InitMatrix(Ax6_Bx4, row_1 / 8, column_2 / 8);
	Ax6_Bx4 = MulMatrixes(row_1 / 8, column_2 / 8, Ax6, Bx4);
	WriteToFile("Matrix", 6, Ax6_Bx4, row_1 / 8, column_2 / 8, "Matrix Ax6 * Bx4 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										Відправляємо смугу Bx4 в 7 процес
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	for (int i = 0; i < row_2; i++)
	{
		MPI_Send(&(Bx4[i][0]), column_2 / 8, MPI_LONG_DOUBLE, 7, tagBx4, MPI_COMM_WORLD);
	}
	cout << "Send Bx4 from processor with rank 6 to processor with rank 7" << endl;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										Приймаємо смугу Bx3 від 5 процесу
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Bx3;
	InitMatrix(Bx3, row_2, column_2 / 8);
	for (int i = 0; i < row_2; i++)
	{
		MPI_Recv(&(Bx3[i][0]), column_2 / 8, MPI_LONG_DOUBLE, 5, tagBx3, MPI_COMM_WORLD, &status);
	}
	WriteToFile("Matrix", 6, Bx3, row_2, column_2 / 8, "Matrix Bx3 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										Множення матриці Ax6 * Bx3
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Ax6_Bx3;
	InitMatrix(Ax6_Bx3, row_1 / 8, column_2 / 8);
	Ax6_Bx3 = MulMatrixes(row_1 / 8, column_2 / 8, Ax6, Bx3);
	WriteToFile("Matrix", 6, Ax6_Bx3, row_1 / 8, column_2 / 8, "Matrix Ax6 * Bx3 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										Відправляємо смугу Bx3 в 7 процес
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	for (int i = 0; i < row_2; i++)
	{
		MPI_Send(&(Bx3[i][0]), column_2 / 8, MPI_LONG_DOUBLE, 7, tagBx3, MPI_COMM_WORLD);
	}
	cout << "Send Bx3 from processor with rank 6 to processor with rank 7" << endl;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										Приймаємо смугу Bx2 від 5 процесу
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Bx2;
	InitMatrix(Bx2, row_2, column_2 / 8);
	for (int i = 0; i < row_2; i++)
	{
		MPI_Recv(&(Bx2[i][0]), column_2 / 8, MPI_LONG_DOUBLE, 5, tagBx2, MPI_COMM_WORLD, &status);
	}
	WriteToFile("Matrix", 6, Bx2, row_2, column_2 / 8, "Matrix Bx2 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										Множення матриці Ax6 * Bx2
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Ax6_Bx2;
	InitMatrix(Ax6_Bx2, row_1 / 8, column_2 / 8);
	Ax6_Bx2 = MulMatrixes(row_1 / 8, column_2 / 8, Ax6, Bx2);
	WriteToFile("Matrix", 6, Ax6_Bx2, row_1 / 8, column_2 / 8, "Matrix Ax6 * Bx2 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										Відправляємо смугу Bx2 в 0 процес
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	for (int i = 0; i < row_2; i++)
	{
		MPI_Send(&(Bx2[i][0]), column_2 / 8, MPI_LONG_DOUBLE, 0, tagBx2, MPI_COMM_WORLD);
	}
	cout << "Send Bx2 from processor with rank 6 to processor with rank 0" << endl;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										Приймаємо смугу Bx0 від 4 процесу   DONE
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Bx0;
	InitMatrix(Bx0, row_2, (column_2 / 8) + 3);
	for (int i = 0; i < row_2; i++)
	{
		MPI_Recv(&(Bx0[i][0]), (column_2 / 8) + 3, MPI_LONG_DOUBLE, 4, tagBx0, MPI_COMM_WORLD, &status);
	}
	WriteToFile("Matrix", 6, Bx0, row_2, (column_2 / 8) + 3, "Matrix Bx0 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										Множення матриці Ax6 * Bx0		DONE
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Ax6_Bx0;
	InitMatrix(Ax6_Bx0, row_1 / 8, (column_2 / 8) + 3);
	Ax6_Bx0 = MulMatrixes(row_1 / 8, (column_2 / 8) + 3, Ax6, Bx0);
	WriteToFile("Matrix", 6, Ax6_Bx0, row_1 / 8, (column_2 / 8) + 3, "Matrix Ax6 * Bx0 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										Відправляємо смугу Bx0 в 7 процес			DONE
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	for (int i = 0; i < row_2; i++)
	{
		MPI_Send(&(Bx0[i][0]), (column_2 / 8) + 3, MPI_LONG_DOUBLE, 7, tagBx0, MPI_COMM_WORLD);
	}
	cout << "Send Bx0 from processor with rank 6 to processor with rank 7" << endl;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										Приймаємо смугу Bx7 від 5 процесу
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Bx7;
	InitMatrix(Bx7, row_2, column_2 / 8);
	for (int i = 0; i < row_2; i++)
	{
		MPI_Recv(&(Bx7[i][0]), column_2 / 8, MPI_LONG_DOUBLE, 5, tagBx7, MPI_COMM_WORLD, &status);
	}
	WriteToFile("Matrix", 6, Bx7, row_2, column_2 / 8, "Matrix Bx7 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										Множення матриці Ax6 * Bx7
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Ax6_Bx7;
	InitMatrix(Ax6_Bx7, row_1 / 8, column_2 / 8);
	Ax6_Bx7 = MulMatrixes(row_1 / 8, column_2 / 8, Ax6, Bx7);
	WriteToFile("Matrix", 6, Ax6_Bx7, row_1 / 8, column_2 / 8, "Matrix Ax6 * Bx7 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										Відправляємо смугу Bx7 в 4 процес		DONE
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	for (int i = 0; i < row_2; i++)
	{
		MPI_Send(&(Bx7[i][0]), column_2 / 8, MPI_LONG_DOUBLE, 4, tagBx7, MPI_COMM_WORLD);
	}
	cout << "Send Bx7 from processor with rank 6 to processor with rank 4" << endl;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										Приймаємо смугу Bx1 від 0 процесу
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Bx1;
	InitMatrix(Bx1, row_2, column_2 / 8);
	for (int i = 0; i < row_2; i++)
	{
		MPI_Recv(&(Bx1[i][0]), column_2 / 8, MPI_LONG_DOUBLE, 0, tagBx1, MPI_COMM_WORLD, &status);
	}
	WriteToFile("Matrix", 6, Bx1, row_2, column_2 / 8, "Matrix Bx1 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										Множення матриці Ax6 * Bx1
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Ax6_Bx1;
	InitMatrix(Ax6_Bx1, row_1 / 8, column_2 / 8);
	Ax6_Bx1 = MulMatrixes(row_1 / 8, column_2 / 8, Ax6, Bx1);
	WriteToFile("Matrix", 6, Ax6_Bx1, row_1 / 8, column_2 / 8, "Matrix Ax6 * Bx1 : ");
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
//										Збираю підматрицю до купи	
/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** proc6_res;
	InitMatrix(proc6_res, row_1 / 8, column_2);
	int point = 0;
	for (int i = 0; i < (row_1 / 8); i++)
	{
		for (int j = 0; j < (column_2 / 8) + 3; j++)	//Тоді заповняємо матрицю від 0 до 34 елемента стовпця
		{
			proc6_res[i][j] = Ax6_Bx0[i][j];
		}
	}

	for (int i = 0; i < (row_1 / 8); i++)
	{
		for (int j = (column_2 / 8) + 3; j < ((column_2 / 8) * 2) + 3; j++)
		{
			proc6_res[i][j] = Ax6_Bx1[i][point];
			point++;
		}
		point = 0;
	}
	for (int i = 0; i < (row_1 / 8); i++)
	{
		for (int j = ((column_2 / 8) * 2) + 3; j < ((column_2 / 8) * 3) + 3; j++)
		{
			proc6_res[i][j] = Ax6_Bx2[i][point];

			point++;
		}
		point = 0;
	}
	for (int i = 0; i < (row_1 / 8); i++)
	{
		for (int j = ((column_2 / 8) * 3) + 3; j < ((column_2 / 8) * 4) + 3; j++)
		{
			proc6_res[i][j] = Ax6_Bx3[i][point];
			point++;
		}
		point = 0;
	}
	for (int i = 0; i < (row_1 / 8); i++)
	{
		for (int j = ((column_2 / 8) * 4) + 3; j < ((column_2 / 8) * 5) + 3; j++)
		{
			proc6_res[i][j] = Ax6_Bx4[i][point];
			point++;
		}
		point = 0;
	}
	for (int i = 0; i < (row_1 / 8); i++)
	{
		for (int j = ((column_2 / 8) * 5) + 3; j < ((column_2 / 8) * 6) + 3; j++)
		{
			proc6_res[i][j] = Ax6_Bx5[i][point];
			point++;
		}
		point = 0;
	}
	for (int i = 0; i < (row_1 / 8); i++)
	{
		for (int j = ((column_2 / 8) * 6) + 3; j < ((column_2 / 8) * 7) + 3; j++)
		{
			proc6_res[i][j] = Ax6_Bx6[i][point];
			point++;
		}
		point = 0;
	}
	for (int i = 0; i < (row_1 / 8); i++)
	{
		for (int j = ((column_2 / 8) * 7) + 3; j < column_2; j++)
		{
			proc6_res[i][j] = Ax6_Bx7[i][point];

			point++;
		}
		point = 0;
	}

	WriteToFile("Matrix", 6, proc6_res, (row_1 / 8), column_2, "Matrix proc6_res : ");
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										Перекидаю і приймаю підматриці між процесами	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	long double** proc5_res;
	InitMatrix(proc5_res, row_1 / 8, column_2);
	long double** proc7_res;
	InitMatrix(proc7_res, row_1 / 8, column_2);

	for (int i = 0; i < row_1 / 8; i++)
	{
		MPI_Recv(&(proc5_res[i][0]), column_2, MPI_LONG_DOUBLE, 7, tagA5, MPI_COMM_WORLD, &status);
		MPI_Recv(&(proc7_res[i][0]), column_2, MPI_LONG_DOUBLE, 7, tagA7, MPI_COMM_WORLD, &status);
	}

	for (int i = 0; i < (row_1 / 8); i++)
	{
		MPI_Send(&(proc7_res[i][0]), column_2, MPI_LONG_DOUBLE, 4, tagA7, MPI_COMM_WORLD);
		MPI_Send(&(proc5_res[i][0]), column_2, MPI_LONG_DOUBLE, 4, tagA5, MPI_COMM_WORLD);
		MPI_Send(&(proc6_res[i][0]), column_2, MPI_LONG_DOUBLE, 4, tagA6, MPI_COMM_WORLD);
	}
	cout << "Send proc5_res, pro67_res, proc7_res from processor with rank 6 to processor with rank 4" << endl;
}