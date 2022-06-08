//Proc2.cpp

#include "Header.h"


void CPU_2()
{
	MPI_Barrier(MPI_COMM_WORLD);
	cout << endl;
	MPI_Status status;
	
	long double** A;
	long double** B;

	InitMatrix(A, (row_1 / 8) * 6, column_1);    //168 рядків 248 стовпців
	InitMatrix(B, row_2, (column_2 / 8) * 6);	 //248 рядків 186 стовпців
	int x = 0;
	for (int i = ((row_1 / 8) * 2) + 6; i < row_1; i++)		//приймаю від 62 елемента
	{
		MPI_Recv(&(A[x][0]), column_1, MPI_LONG_DOUBLE, 1, tagA2, MPI_COMM_WORLD, &status);
		x++;
	}
	//WriteToFile("Matrix", 2, A, (row_1 / 8) * 6, column_1, "Matrix A: ");

	for (int i = 0; i < row_2; i++)
		MPI_Recv(&(B[i][0]), (column_2 / 8) * 6, MPI_LONG_DOUBLE, 1, tagB2, MPI_COMM_WORLD, &status);
	//WriteToFile("Matrix", 2, B, row_2, (column_2 / 8) * 6, "Matrix B: ");
	

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//									Розрізаємо матрицю А на 1/8
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	long double** Ax2;

	InitMatrix(Ax2, row_1 / 8, column_1);

	for (int i = 0; i < (row_1 / 8) ; i++)
	{
		for (int j = 0; j < column_1; j++)
		{
			Ax2[i][j] = A[i][j];
		}
	}
	WriteToFile("Matrix", 2, Ax2, row_1 / 8, column_1, "Matrix Ax2 : ");
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//									Розрізаємо матрицю B на 1/8		DONE
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	long double** Bx2;

	InitMatrix(Bx2, row_2, column_2 / 8);

	for (int i = 0; i < row_2; i++)
	{
		for (int j = 0; j < column_2 / 8; j++)
		{
			Bx2[i][j] = B[i][j];
		}
	}
	WriteToFile("Matrix", 2, Bx2, row_2, column_2 / 8, "Matrix Bx2 : ");
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//									Відправляємо смуги матриці А по процесам лишаючи собі 1/8 матриці
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	long double** A3;
	InitMatrix(A3, (row_1 / 8) * 5, column_1);		// 140 рядків 248 стовпців
	int l = 0;
	for (int i = (row_1 / 8); i < (row_1 / 8) * 6; i++)		//від 28 рядка заповняю матрицю А3
	{
		for (int j = 0; j < column_1; j++)
		{
			A3[l][j] = A[i][j];
			
		}
		l++;
	}
	//WriteToFile("Matrix", 2, A3, (row_1 / 8) * 5, column_1, "Matrix A3: ");
	for (int i = 0; i < (row_1 / 8) * 5; i++)	//від 90 елемента
	{
		MPI_Send(&(A3[i][0]), column_1, MPI_LONG_DOUBLE, 3, tagA3, MPI_COMM_WORLD);
	}
	cout << "Send submatrix A3 from processor with rank 2 to processor with rank 3" << endl;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										Відправляємо смуги матриці B по процесам лишаючи собі 1/8 матриці		DONE
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	long double** B3;
	InitMatrix(B3, row_2, (column_2 / 8) * 5);
	int k = 0;
	for (int i = 0; i < row_2; i++)
	{
		for (int j = (column_2 / 8); j < (column_2 / 8) * 6; j++)
		{
			B3[i][k] = B[i][j];
			k++;
		}
		k = 0;
	}
	//WriteToFile("Matrix", 2, B3, row_2, (column_2 / 8) * 5, "Matrix B3: ");
	for (int i = 0; i < row_2; i++)
	{
		MPI_Send(&(B3[i][0]), (column_2 / 8) * 5, MPI_LONG_DOUBLE, 3, tagB3, MPI_COMM_WORLD);
	}
	cout << "Send submatrix B3 from processor with rank 2 to processor with rank 3" << endl;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
//										Множення матриці Ax2 * Bx2		DONE
/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Ax2_Bx2;
	InitMatrix(Ax2_Bx2, row_1 / 8, column_2 / 8);
	Ax2_Bx2 = MulMatrixes(row_1 / 8, column_2 / 8, Ax2, Bx2);
	WriteToFile("Matrix", 2, Ax2_Bx2, row_1 / 8, column_2 / 8, "Matrix Ax2 * Bx2 : ");
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
		//										Відправляємо смугу Bx2 в 3 процес
		/////////////////////////////////////////////////////////////////////////////////////////////////////////
	for (int i = 0; i < row_2; i++)
	{
		MPI_Send(&(Bx2[i][0]), column_2 / 8, MPI_LONG_DOUBLE, 3, tagBx2, MPI_COMM_WORLD);
	}
	cout << "Send Bx2 from processor with rank 2 to processor with rank 3" << endl;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										Приймаємо смугу Bx1 від 1 процесу
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Bx1;
	InitMatrix(Bx1, row_2, column_2 / 8);
	for (int i = 0; i < row_2; i++)
	{
		MPI_Recv(&(Bx1[i][0]), column_2 / 8, MPI_LONG_DOUBLE, 1, tagBx1, MPI_COMM_WORLD, &status);
	}
	WriteToFile("Matrix", 2, Bx1, row_2, column_2 / 8, "Matrix Bx1 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										Множення матриці Ax2 * Bx1		DONE
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Ax2_Bx1;
	InitMatrix(Ax2_Bx1, row_1 / 8, column_2 / 8);
	Ax2_Bx1 = MulMatrixes(row_1 / 8, column_2 / 8, Ax2, Bx1);
	WriteToFile("Matrix", 2, Ax2_Bx1, row_1 / 8, column_2 / 8, "Matrix Ax2 * Bx1 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
//										Відправляємо смугу Bx1 в 3 процес
/////////////////////////////////////////////////////////////////////////////////////////////////////////
	for (int i = 0; i < row_2; i++)
	{
		MPI_Send(&(Bx1[i][0]), column_2 / 8, MPI_LONG_DOUBLE, 3, tagBx1, MPI_COMM_WORLD);
	}
	cout << "Send Bx1 from processor with rank 2 to processor with rank 3" << endl;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										Приймаємо смугу Bx0 від 1 процесу
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Bx0;
	InitMatrix(Bx0, row_2, (column_2 / 8) + 3);
	for (int i = 0; i < row_2; i++)
	{
		MPI_Recv(&(Bx0[i][0]), (column_2 / 8) + 3, MPI_LONG_DOUBLE, 1, tagBx0, MPI_COMM_WORLD, &status);
	}
	WriteToFile("Matrix", 2, Bx0, row_2, (column_2 / 8) + 3, "Matrix Bx0 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										Множення матриці Ax2 * Bx0		DONE
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Ax2_Bx0;
	InitMatrix(Ax2_Bx0, row_1 / 8, (column_2 / 8) + 3);
	Ax2_Bx0 = MulMatrixes(row_1 / 8, (column_2 / 8) + 3, Ax2, Bx0);
	WriteToFile("Matrix", 2, Ax2_Bx0, row_1 / 8, (column_2 / 8) + 3, "Matrix Ax2 * Bx0 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										Відправляємо смугу Bx0 в 3 процес
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	for (int i = 0; i < row_2; i++)
	{
		MPI_Send(&(Bx0[i][0]), (column_2 / 8) + 3, MPI_LONG_DOUBLE, 3, tagBx0, MPI_COMM_WORLD);
	}
	cout << "Send Bx0 from processor with rank 2 to processor with rank 3" << endl;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										Приймаємо смугу Bx7 від 1 процесу
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Bx7;
	InitMatrix(Bx7, row_2, column_2 / 8);
	for (int i = 0; i < row_2; i++)			
	{
		MPI_Recv(&(Bx7[i][0]), column_2 / 8, MPI_LONG_DOUBLE, 1, tagBx7, MPI_COMM_WORLD, &status);
	}
	WriteToFile("Matrix", 2, Bx7, row_2, column_2 / 8, "Matrix Bx7 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										Множення матриці Ax2 * Bx7
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Ax2_Bx7;
	InitMatrix(Ax2_Bx7, row_1 / 8, column_2 / 8);
	Ax2_Bx7 = MulMatrixes(row_1 / 8, column_2 / 8, Ax2, Bx7);
	WriteToFile("Matrix", 2, Ax2_Bx7, row_1 / 8, column_2 / 8, "Matrix Ax2 * Bx7 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										Відправляємо смугу Bx7 в 3 процес
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	for (int i = 0; i < row_2; i++)
	{
		MPI_Send(&(Bx7[i][0]), column_2 / 8, MPI_LONG_DOUBLE, 3, tagBx7, MPI_COMM_WORLD);
	}
	cout << "Send Bx7 from processor with rank 2 to processor with rank 3" << endl;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										Приймаємо смугу Bx6 від 1 процесу	DONE
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Bx6;
	InitMatrix(Bx6, row_2, column_2 / 8);
	for (int i = 0; i < row_2; i++)			
	{
		MPI_Recv(&(Bx6[i][0]), column_2 / 8, MPI_LONG_DOUBLE, 1, tagBx6, MPI_COMM_WORLD, &status);
	}
	WriteToFile("Matrix", 2, Bx6, row_2, column_2 / 8, "Matrix Bx6 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										Множення матриці Ax2 * Bx6		DONE
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Ax2_Bx6;
	InitMatrix(Ax2_Bx6, row_1 / 8, column_2 / 8);
	Ax2_Bx6 = MulMatrixes(row_1 / 8, column_2 / 8, Ax2, Bx6);
	WriteToFile("Matrix", 2, Ax2_Bx6, row_1 / 8, column_2 / 8, "Matrix Ax2 * Bx6 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										Відправляємо смугу Bx6 в 4 процес
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	for (int i = 0; i < row_2; i++)
	{
		MPI_Send(&(Bx6[i][0]), column_2 / 8, MPI_LONG_DOUBLE, 4, tagBx6, MPI_COMM_WORLD);
	}
	cout << "Send Bx6 from processor with rank 2 to processor with rank 3" << endl;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										Приймаємо смугу Bx4 від 0 процесу
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Bx4;
	InitMatrix(Bx4, row_2, column_2 / 8);
	for (int i = 0; i < row_2; i++)
	{
		MPI_Recv(&(Bx4[i][0]), column_2 / 8, MPI_LONG_DOUBLE, 0, tagBx4, MPI_COMM_WORLD, &status);
	}
	WriteToFile("Matrix", 2, Bx4, row_2, column_2 / 8, "Matrix Bx4 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										Множення матриці Ax2 * Bx4
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Ax2_Bx4;
	InitMatrix(Ax2_Bx4, row_1 / 8, column_2 / 8);
	Ax2_Bx4 = MulMatrixes(row_1 / 8, column_2 / 8, Ax2, Bx4);
	WriteToFile("Matrix", 2, Ax2_Bx4, row_1 / 8, column_2 / 8, "Matrix Ax2 * Bx4 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										Відправляємо смугу Bx4 в 3 процес
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	for (int i = 0; i < row_2; i++)
	{
		MPI_Send(&(Bx4[i][0]), column_2 / 8, MPI_LONG_DOUBLE, 3, tagBx4, MPI_COMM_WORLD);
	}
	cout << "Send Bx4 from processor with rank 2 to processor with rank 3" << endl;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										Приймаємо смугу Bx3 від 1 процесу
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Bx3;
	InitMatrix(Bx3, row_2, column_2 / 8);
	for (int i = 0; i < row_2; i++)
	{
		MPI_Recv(&(Bx3[i][0]), column_2 / 8, MPI_LONG_DOUBLE, 1, tagBx3, MPI_COMM_WORLD, &status);
	}
	WriteToFile("Matrix", 2, Bx3, row_2, column_2 / 8, "Matrix Bx3 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										Множення матриці Ax2 * Bx3
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Ax2_Bx3;
	InitMatrix(Ax2_Bx3, row_1 / 8, column_2 / 8);
	Ax2_Bx3 = MulMatrixes(row_1 / 8, column_2 / 8, Ax2, Bx3);
	WriteToFile("Matrix", 2, Ax2_Bx3, row_1 / 8, column_2 / 8, "Matrix Ax2 * Bx3 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										Відправляємо смугу Bx3 в 0 процес
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	for (int i = 0; i < row_2; i++)
	{
		MPI_Send(&(Bx3[i][0]), column_2 / 8, MPI_LONG_DOUBLE, 0, tagBx3, MPI_COMM_WORLD);
	}
	cout << "Send Bx3 from processor with rank 2 to processor with rank 0" << endl;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										Приймаємо смугу Bx5 від 4 процесу
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Bx5;
	InitMatrix(Bx5, row_2, column_2 / 8);
	for (int i = 0; i < row_2; i++)
	{
		MPI_Recv(&(Bx5[i][0]), column_2 / 8, MPI_LONG_DOUBLE, 4, tagBx5, MPI_COMM_WORLD, &status);
	}
	WriteToFile("Matrix", 2, Bx5, row_2, column_2 / 8, "Matrix Bx5 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										Множення матриці Ax2 * Bx5		DONE
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Ax2_Bx5;
	InitMatrix(Ax2_Bx5, row_1 / 8, column_2 / 8);
	Ax2_Bx5 = MulMatrixes(row_1 / 8, column_2 / 8, Ax2, Bx5);
	WriteToFile("Matrix", 2, Ax2_Bx5, row_1 / 8, column_2 / 8, "Matrix Ax2 * Bx5 : ");
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										Збираю підматрицю до купи	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** proc2_res;
	InitMatrix(proc2_res, row_1 / 8, column_2);
	int point = 0;
	for (int i = 0; i < (row_1 / 8); i++)
	{
		for (int j = 0; j < (column_2 / 8) + 3; j++)	//Тоді заповняємо матрицю від 0 до 34 елемента стовпця
		{
			proc2_res[i][j] = Ax2_Bx0[i][j];
		}
	}

	for (int i = 0; i < (row_1 / 8); i++)
	{
		for (int j = (column_2 / 8) + 3; j < ((column_2 / 8) * 2) + 3; j++)
		{
			proc2_res[i][j] = Ax2_Bx1[i][point];
			point++;
		}
		point = 0;
	}
	for (int i = 0; i < (row_1 / 8); i++)
	{
		for (int j = ((column_2 / 8) * 2) + 3; j < ((column_2 / 8) * 3) + 3; j++)
		{
			proc2_res[i][j] = Ax2_Bx2[i][point];

			point++;
		}
		point = 0;
	}
	for (int i = 0; i < (row_1 / 8); i++)
	{
		for (int j = ((column_2 / 8) * 3) + 3; j < ((column_2 / 8) * 4) + 3; j++)
		{
			proc2_res[i][j] = Ax2_Bx3[i][point];
			point++;
		}
		point = 0;
	}
	for (int i = 0; i < (row_1 / 8); i++)
	{
		for (int j = ((column_2 / 8) * 4) + 3; j < ((column_2 / 8) * 5) + 3; j++)
		{
			proc2_res[i][j] = Ax2_Bx4[i][point];
			point++;
		}
		point = 0;
	}
	for (int i = 0; i < (row_1 / 8); i++)
	{
		for (int j = ((column_2 / 8) * 5) + 3; j < ((column_2 / 8) * 6) + 3; j++)
		{
			proc2_res[i][j] = Ax2_Bx5[i][point];
			point++;
		}
		point = 0;
	}
	for (int i = 0; i < (row_1 / 8); i++)
	{
		for (int j = ((column_2 / 8) * 6) + 3; j < ((column_2 / 8) * 7) + 3; j++)
		{
			proc2_res[i][j] = Ax2_Bx6[i][point];
			point++;
		}
		point = 0;
	}
	for (int i = 0; i < (row_1 / 8); i++)
	{
		for (int j = ((column_2 / 8) * 7) + 3; j < column_2; j++)
		{
			proc2_res[i][j] = Ax2_Bx7[i][point];

			point++;
		}
		point = 0;
	}

	WriteToFile("Matrix", 2, proc2_res, (row_1 / 8), column_2, "Matrix proc2_res : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										Перекидаю і приймаю підматриці між процесами	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	long double** proc5_res;
	InitMatrix(proc5_res, row_1 / 8, column_2);
	long double** proc7_res;
	InitMatrix(proc7_res, row_1 / 8, column_2);
	long double** proc6_res;
	InitMatrix(proc6_res, row_1 / 8, column_2);
	long double** proc4_res;
	InitMatrix(proc4_res, row_1 / 8, column_2);
	long double** proc3_res;
	InitMatrix(proc3_res, row_1 / 8, column_2);

	for (int i = 0; i < row_1 / 8; i++)
	{
		MPI_Recv(&(proc5_res[i][0]), column_2, MPI_LONG_DOUBLE, 3, tagA5, MPI_COMM_WORLD, &status);
		MPI_Recv(&(proc7_res[i][0]), column_2, MPI_LONG_DOUBLE, 3, tagA7, MPI_COMM_WORLD, &status);
		MPI_Recv(&(proc6_res[i][0]), column_2, MPI_LONG_DOUBLE, 3, tagA6, MPI_COMM_WORLD, &status);
		MPI_Recv(&(proc4_res[i][0]), column_2, MPI_LONG_DOUBLE, 3, tagA4, MPI_COMM_WORLD, &status);
		MPI_Recv(&(proc3_res[i][0]), column_2, MPI_LONG_DOUBLE, 3, tagA3, MPI_COMM_WORLD, &status);
	}

	for (int i = 0; i < (row_1 / 8); i++)
	{
		MPI_Send(&(proc7_res[i][0]), column_2, MPI_LONG_DOUBLE, 1, tagA7, MPI_COMM_WORLD);
		MPI_Send(&(proc5_res[i][0]), column_2, MPI_LONG_DOUBLE, 1, tagA5, MPI_COMM_WORLD);
		MPI_Send(&(proc6_res[i][0]), column_2, MPI_LONG_DOUBLE, 1, tagA6, MPI_COMM_WORLD);
		MPI_Send(&(proc4_res[i][0]), column_2, MPI_LONG_DOUBLE, 1, tagA4, MPI_COMM_WORLD);
		MPI_Send(&(proc3_res[i][0]), column_2, MPI_LONG_DOUBLE, 1, tagA3, MPI_COMM_WORLD);
		MPI_Send(&(proc2_res[i][0]), column_2, MPI_LONG_DOUBLE, 1, tagA2, MPI_COMM_WORLD);
	}
	cout << "Send proc2_res, proc3_res, proc4_res, proc5_res, pro67_res, proc7_res from processor with rank 2 to processor with rank 1" << endl;
}