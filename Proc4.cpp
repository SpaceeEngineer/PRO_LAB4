//Proc4.cpp

#include "Header.h"


void CPU_4()
{
	MPI_Barrier(MPI_COMM_WORLD);
	cout << endl;
	MPI_Status status;
	
	long double** A;
	long double** B;

	InitMatrix(A, (row_1 / 8) * 4, column_1);    //112 рядків 248 стовпців
	InitMatrix(B, row_2, (column_2 / 8) * 4);	 //248 рядків 124 стовпців
	int x = 0;
	for (int i = ((row_1 / 8) * 4) + 6; i < row_1; i++)
	{
		MPI_Recv(&(A[x][0]), column_1, MPI_LONG_DOUBLE, 3, tagA4, MPI_COMM_WORLD, &status);
		x++;
	}
		
	//WriteToFile("Matrix", 4, A, (row_1 / 8) * 4, column_1, "Matrix A : ");

	for (int i = 0; i < row_2; i++)
		MPI_Recv(&(B[i][0]), (column_2 / 8) * 4, MPI_LONG_DOUBLE, 3, tagB4, MPI_COMM_WORLD, &status);
	//WriteToFile("Matrix", 4, B, row_2, (column_2 / 8) * 4, "Matrix B : ");


	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//									Розрізаємо матрицю А на 1/8
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	long double** Ax4;

	InitMatrix(Ax4, row_1 / 8, column_1);

	for (int i = 0; i < (row_1 / 8); i++)
	{
		for (int j = 0; j < column_1; j++)
		{
			Ax4[i][j] = A[i][j];
		}
	}
	WriteToFile("Matrix", 4, Ax4, row_1 / 8, column_1, "Matrix Ax4 : ");
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//									Розрізаємо матрицю B на 1/8 
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	long double** Bx4;

	InitMatrix(Bx4, row_2, column_2 / 8);

	for (int i = 0; i < row_2; i++)
	{
		for (int j = 0; j < (column_2 / 8); j++)
		{
			Bx4[i][j] = B[i][j];
		}
	}
	WriteToFile("Matrix", 4, Bx4, row_2, column_2 / 8, "Matrix Bx4 : ");
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//									Відправляємо смуги матриці А по процесам лишаючи собі 1/8 матриці
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	long double** A6;
	InitMatrix(A6, (row_1 / 8) * 3, column_1);		// 196 рядків 248 стовпців
	int l = 0;
	for (int i = (row_1 / 8); i < (row_1 / 8) * 4; i++)		//від 34 рядка заповняю матрицю А1
	{
		for (int j = 0; j < column_1; j++)
		{
			A6[l][j] = A[i][j];
		}
		l++;
	}
	//WriteToFile("Matrix", 4, A6, (row_1 / 8) * 3, column_1, "Matrix A6: ");
	//===================================================================================================
	//												TODO: 0-1-2-3-4-6-7-5
	//====================================================================================================
	//for (int i = 0; i < (row_1 / 8) * 3; i++)
	//{
	//	MPI_Send(&(A6[i][0]), column_1, MPI_LONG_DOUBLE, 5, tagA6, MPI_COMM_WORLD);
	//}
	for (int i = 0; i < (row_1 / 8) * 3; i++)
	{
		MPI_Send(&(A6[i][0]), column_1, MPI_LONG_DOUBLE, 6, tagA6, MPI_COMM_WORLD);
	}
	cout << "Send submatrix A6 from processor with rank 4 to processor with rank 6" << endl;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										Відправляємо смуги матриці B по процесам лишаючи собі 1/8 матриці
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	long double** B6;
	InitMatrix(B6, row_2, (column_2 / 8) * 3);
	int k = 0;
	for (int i = 0; i < row_2; i++)
	{
		for (int j = (column_2 / 8); j < (column_2 / 8) * 4; j++)
		{
			B6[i][k] = B[i][j];
			k++;
		}
		k = 0;
	}
	//WriteToFile("Matrix", 4, B6, row_2, (column_2 / 8) * 3, "Matrix B6: ");
	for (int i = 0; i < row_2; i++)
	{
		MPI_Send(&(B6[i][0]), (column_2 / 8) * 3, MPI_LONG_DOUBLE, 6, tagB6, MPI_COMM_WORLD);
	}
	cout << "Send submatrix B6 from processor with rank 4 to processor with rank 6" << endl;
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
//										Множення матриці Ax4 * Bx4		DONE
/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Ax4_Bx4;
	InitMatrix(Ax4_Bx4, row_1 / 8, column_2 / 8);
	Ax4_Bx4 = MulMatrixes(row_1 / 8, column_2 / 8, Ax4, Bx4);
	WriteToFile("Matrix", 4, Ax4_Bx4, row_1 / 8, column_2 / 8, "Matrix Ax4 * Bx4 : ");
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
		//										Відправляємо смугу Bx4 в 5 процес
		/////////////////////////////////////////////////////////////////////////////////////////////////////////
	for (int i = 0; i < row_2; i++)
	{
		MPI_Send(&(Bx4[i][0]), column_2 / 8, MPI_LONG_DOUBLE, 5, tagBx4, MPI_COMM_WORLD);
	}
	cout << "Send Bx4 from processor with rank 4 to processor with rank 5" << endl;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										Приймаємо смугу Bx3 від 3 процесу
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Bx3;
	InitMatrix(Bx3, row_2, column_2 / 8);
	for (int i = 0; i < row_2; i++)
	{
		MPI_Recv(&(Bx3[i][0]), column_2 / 8, MPI_LONG_DOUBLE, 3, tagBx3, MPI_COMM_WORLD, &status);
	}
	WriteToFile("Matrix", 4, Bx3, row_2, column_2 / 8, "Matrix Bx3 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										Множення матриці Ax4 * Bx3
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Ax4_Bx3;
	InitMatrix(Ax4_Bx3, row_1 / 8, column_2 / 8);
	Ax4_Bx3 = MulMatrixes(row_1 / 8, column_2 / 8, Ax4, Bx3);
	WriteToFile("Matrix", 4, Ax4_Bx3, row_1 / 8, column_2 / 8, "Matrix Ax4 * Bx3 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
//										Відправляємо смугу Bx3 в 5 процес
/////////////////////////////////////////////////////////////////////////////////////////////////////////
	for (int i = 0; i < row_2; i++)
	{
		MPI_Send(&(Bx3[i][0]), column_2 / 8, MPI_LONG_DOUBLE, 5, tagBx3, MPI_COMM_WORLD);
	}
	cout << "Send Bx3 from processor with rank 4 to processor with rank 5" << endl;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										Приймаємо смугу Bx2 від 3 процесу
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Bx2;
	InitMatrix(Bx2, row_2, column_2 / 8);
	for (int i = 0; i < row_2; i++)
	{
		MPI_Recv(&(Bx2[i][0]), column_2 / 8, MPI_LONG_DOUBLE, 3, tagBx2, MPI_COMM_WORLD, &status);
	}
	WriteToFile("Matrix", 4, Bx2, row_2, column_2 / 8, "Matrix Bx2 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										Множення матриці Ax4 * Bx2
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Ax4_Bx2;
	InitMatrix(Ax4_Bx2, row_1 / 8, column_2 / 8);
	Ax4_Bx2 = MulMatrixes(row_1 / 8, column_2 / 8, Ax4, Bx2);
	WriteToFile("Matrix", 4, Ax4_Bx2, row_1 / 8, column_2 / 8, "Matrix Ax4 * Bx2 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										Відправляємо смугу Bx2 в 5 процес
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	for (int i = 0; i < row_2; i++)
	{
		MPI_Send(&(Bx2[i][0]), column_2 / 8, MPI_LONG_DOUBLE, 5, tagBx2, MPI_COMM_WORLD);
	}
	cout << "Send Bx2 from processor with rank 4 to processor with rank 5" << endl;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										Приймаємо смугу Bx1 від 3 процесу
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Bx1;
	InitMatrix(Bx1, row_2, column_2 / 8);
	for (int i = 0; i < row_2; i++)
	{
		MPI_Recv(&(Bx1[i][0]), column_2 / 8, MPI_LONG_DOUBLE, 3, tagBx1, MPI_COMM_WORLD, &status);
	}
	WriteToFile("Matrix", 4, Bx1, row_2, column_2 / 8, "Matrix Bx1 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										Множення матриці Ax4 * Bx1
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Ax4_Bx1;
	InitMatrix(Ax4_Bx1, row_1 / 8, column_2 / 8);
	Ax4_Bx1 = MulMatrixes(row_1 / 8, column_2 / 8, Ax4, Bx1);
	WriteToFile("Matrix", 4, Ax4_Bx1, row_1 / 8, column_2 / 8, "Matrix Ax4 * Bx1 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										Відправляємо смугу Bx1 в 5 процес
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	for (int i = 0; i < row_2; i++)
	{
		MPI_Send(&(Bx1[i][0]), column_2 / 8, MPI_LONG_DOUBLE, 5, tagBx1, MPI_COMM_WORLD);
	}
	cout << "Send Bx1 from processor with rank 4 to processor with rank 5" << endl;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										Приймаємо смугу Bx0 від 3 процесу		DONE
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Bx0;
	InitMatrix(Bx0, row_2, (column_2 / 8) + 3);
	for (int i = 0; i < row_2; i++)
	{
		MPI_Recv(&(Bx0[i][0]), (column_2 / 8) + 3, MPI_LONG_DOUBLE, 3, tagBx0, MPI_COMM_WORLD, &status);
	}
	WriteToFile("Matrix", 4, Bx0, row_2, (column_2 / 8) + 3, "Matrix Bx0 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										Множення матриці Ax4 * Bx0		DONE
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Ax4_Bx0;
	InitMatrix(Ax4_Bx0, row_1 / 8, (column_2 / 8) + 3);
	Ax4_Bx0 = MulMatrixes(row_1 / 8, (column_2 / 8) + 3, Ax4, Bx0);
	WriteToFile("Matrix", 4, Ax4_Bx0, row_1 / 8, (column_2 / 8) + 3, "Matrix Ax4 * Bx0 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										Відправляємо смугу Bx0 в 6 процес		DONE
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	for (int i = 0; i < row_2; i++)
	{
		MPI_Send(&(Bx0[i][0]), (column_2 / 8) + 3, MPI_LONG_DOUBLE, 6, tagBx0, MPI_COMM_WORLD);
	}
	cout << "Send Bx0 from processor with rank 4 to processor with rank 6" << endl;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										Приймаємо смугу Bx6 від 2 процесу
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Bx6;
	InitMatrix(Bx6, row_2, column_2 / 8);
	for (int i = 0; i < row_2; i++)
	{
		MPI_Recv(&(Bx6[i][0]), column_2 / 8, MPI_LONG_DOUBLE, 2, tagBx6, MPI_COMM_WORLD, &status);
	}
	WriteToFile("Matrix", 4, Bx6, row_2, column_2 / 8, "Matrix Bx6 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										Множення матриці Ax4 * Bx6
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Ax4_Bx6;
	InitMatrix(Ax4_Bx6, row_1 / 8, column_2 / 8);
	Ax4_Bx6 = MulMatrixes(row_1 / 8, column_2 / 8, Ax4, Bx6);
	WriteToFile("Matrix", 4, Ax4_Bx6, row_1 / 8, column_2 / 8, "Matrix Ax4 * Bx6 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										Відправляємо смугу Bx6 в 5 процес
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	for (int i = 0; i < row_2; i++)
	{
		MPI_Send(&(Bx6[i][0]), column_2 / 8, MPI_LONG_DOUBLE, 5, tagBx6, MPI_COMM_WORLD);
	}
	cout << "Send Bx6 from processor with rank 4 to processor with rank 5" << endl;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										Приймаємо смугу Bx5 від 3 процесу
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Bx5;
	InitMatrix(Bx5, row_2, column_2 / 8);
	for (int i = 0; i < row_2; i++)
	{
		MPI_Recv(&(Bx5[i][0]), column_2 / 8, MPI_LONG_DOUBLE, 3, tagBx5, MPI_COMM_WORLD, &status);
	}
	WriteToFile("Matrix", 4, Bx5, row_2, column_2 / 8, "Matrix Bx5 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										Множення матриці Ax4 * Bx5
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Ax4_Bx5;
	InitMatrix(Ax4_Bx5, row_1 / 8, column_2 / 8);
	Ax4_Bx5 = MulMatrixes(row_1 / 8, column_2 / 8, Ax4, Bx5);
	WriteToFile("Matrix", 4, Ax4_Bx5, row_1 / 8, column_2 / 8, "Matrix Ax4 * Bx5 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										Відправляємо смугу Bx5 в 2 процес
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	for (int i = 0; i < row_2; i++)
	{
		MPI_Send(&(Bx5[i][0]), column_2 / 8, MPI_LONG_DOUBLE, 2, tagBx5, MPI_COMM_WORLD);
	}
	cout << "Send Bx5 from processor with rank 4 to processor with rank 2" << endl;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										Приймаємо смугу Bx7 від 6 процесу		DONE
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Bx7;
	InitMatrix(Bx7, row_2, column_2 / 8);
	for (int i = 0; i < row_2; i++)
	{
		MPI_Recv(&(Bx7[i][0]), column_2 / 8, MPI_LONG_DOUBLE, 6, tagBx7, MPI_COMM_WORLD, &status);
	}
	WriteToFile("Matrix", 4, Bx7, row_2, column_2 / 8, "Matrix Bx7 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										Множення матриці Ax4 * Bx7		DONE
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Ax4_Bx7;
	InitMatrix(Ax4_Bx7, row_1 / 8, column_2 / 8);
	Ax4_Bx7 = MulMatrixes(row_1 / 8, column_2 / 8, Ax4, Bx7);
	WriteToFile("Matrix", 4, Ax4_Bx7, row_1 / 8, column_2 / 8, "Matrix Ax4 * Bx7 : ");
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										Збираю підматрицю до купи	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** proc4_res;
	InitMatrix(proc4_res, row_1 / 8, column_2);
	int point = 0;
	for (int i = 0; i < (row_1 / 8); i++)
	{
		for (int j = 0; j < (column_2 / 8) + 3; j++)	//Тоді заповняємо матрицю від 0 до 34 елемента стовпця
		{
			proc4_res[i][j] = Ax4_Bx0[i][j];
		}
	}

	for (int i = 0; i < (row_1 / 8); i++)
	{
		for (int j = (column_2 / 8) + 3; j < ((column_2 / 8) * 2) + 3; j++)
		{
			proc4_res[i][j] = Ax4_Bx1[i][point];
			point++;
		}
		point = 0;
	}
	for (int i = 0; i < (row_1 / 8); i++)
	{
		for (int j = ((column_2 / 8) * 2) + 3; j < ((column_2 / 8) * 3) + 3; j++)
		{
			proc4_res[i][j] = Ax4_Bx2[i][point];

			point++;
		}
		point = 0;
	}
	for (int i = 0; i < (row_1 / 8); i++)
	{
		for (int j = ((column_2 / 8) * 3) + 3; j < ((column_2 / 8) * 4) + 3; j++)
		{
			proc4_res[i][j] = Ax4_Bx3[i][point];
			point++;
		}
		point = 0;
	}
	for (int i = 0; i < (row_1 / 8); i++)
	{
		for (int j = ((column_2 / 8) * 4) + 3; j < ((column_2 / 8) * 5) + 3; j++)
		{
			proc4_res[i][j] = Ax4_Bx4[i][point];
			point++;
		}
		point = 0;
	}
	for (int i = 0; i < (row_1 / 8); i++)
	{
		for (int j = ((column_2 / 8) * 5) + 3; j < ((column_2 / 8) * 6) + 3; j++)
		{
			proc4_res[i][j] = Ax4_Bx5[i][point];
			point++;
		}
		point = 0;
	}
	for (int i = 0; i < (row_1 / 8); i++)
	{
		for (int j = ((column_2 / 8) * 6) + 3; j < ((column_2 / 8) * 7) + 3; j++)
		{
			proc4_res[i][j] = Ax4_Bx6[i][point];
			point++;
		}
		point = 0;
	}
	for (int i = 0; i < (row_1 / 8); i++)
	{
		for (int j = ((column_2 / 8) * 7) + 3; j < column_2; j++)
		{
			proc4_res[i][j] = Ax4_Bx7[i][point];

			point++;
		}
		point = 0;
	}

	WriteToFile("Matrix", 4, proc4_res, (row_1 / 8), column_2, "Matrix proc4_res : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										Перекидаю і приймаю підматриці між процесами	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	long double** proc5_res;
	InitMatrix(proc5_res, row_1 / 8, column_2);
	long double** proc7_res;
	InitMatrix(proc7_res, row_1 / 8, column_2);
	long double** proc6_res;
	InitMatrix(proc6_res, row_1 / 8, column_2);

	for (int i = 0; i < row_1 / 8; i++)
	{
		MPI_Recv(&(proc5_res[i][0]), column_2, MPI_LONG_DOUBLE, 6, tagA5, MPI_COMM_WORLD, &status);
		MPI_Recv(&(proc7_res[i][0]), column_2, MPI_LONG_DOUBLE, 6, tagA7, MPI_COMM_WORLD, &status);
		MPI_Recv(&(proc6_res[i][0]), column_2, MPI_LONG_DOUBLE, 6, tagA6, MPI_COMM_WORLD, &status);
	}

	for (int i = 0; i < (row_1 / 8); i++)
	{
		MPI_Send(&(proc7_res[i][0]), column_2, MPI_LONG_DOUBLE, 3, tagA7, MPI_COMM_WORLD);
		MPI_Send(&(proc5_res[i][0]), column_2, MPI_LONG_DOUBLE, 3, tagA5, MPI_COMM_WORLD);
		MPI_Send(&(proc6_res[i][0]), column_2, MPI_LONG_DOUBLE, 3, tagA6, MPI_COMM_WORLD);
		MPI_Send(&(proc4_res[i][0]), column_2, MPI_LONG_DOUBLE, 3, tagA4, MPI_COMM_WORLD);
	}
	cout << "Send proc4_res, proc5_res, pro67_res, proc7_res from processor with rank 4 to processor with rank 3" << endl;
}