//Proc1.cpp

#include "Header.h"


void CPU_1()
{
	MPI_Barrier(MPI_COMM_WORLD);
	cout << endl;
	MPI_Status status;
	
	long double** A;
	long double** B;

	InitMatrix(A, (row_1 / 8) * 7, column_1);		//196 рядків 248 стовпців
	InitMatrix(B, row_2, (column_2 / 8) * 7);		//248 рядків 217 стовпців
	int x = 0;
	for (int i = (row_1 / 8) + 6; i < row_1; i++)			// приймаю від 34 рядка
	{
		MPI_Recv(&(A[x][0]), column_1, MPI_LONG_DOUBLE, 0, tagA1, MPI_COMM_WORLD, &status);
		x++;
	}   
	//WriteToFile("Matrix", 1, A, (row_1 / 8) * 7, column_1, "Matrix A: ");

	for (int i = 0; i < row_2; i++)
	{
		MPI_Recv(&(B[i][0]), (column_2 / 8) * 7, MPI_LONG_DOUBLE, 0, tagB1, MPI_COMM_WORLD, &status);
	}
	//WriteToFile("Matrix", 1, B, row_2, (column_2 / 8) * 7, "Matrix B: ");

	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//									Розрізаємо матрицю А на 1/8		DONE
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	long double** Ax1;

	InitMatrix(Ax1, row_1 / 8, column_1);

	for (int i = 0; i < row_1 / 8; i++)			//Записую в матрицю Ах1 наступні 28 елементів
	{											// Так як матриця була прийнята від 34 елемента зчитую від 0 елемента до 28
		for (int j = 0; j < column_1; j++)
		{
			Ax1[i][j] = A[i][j];
		}
	}
	WriteToFile("Matrix", 1, Ax1, row_1 / 8, column_1, "Matrix Ax1 : ");
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//									Розрізаємо матрицю B на 1/8													DONE
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	long double** Bx1;

	InitMatrix(Bx1, row_2, column_2 / 8);

	for (int i = 0; i < row_2; i++)							//Записую в матрицю Bх1 наступні 31 елемент
	{														// Так як матриця була прийнята від 34 елемента зчитую від 0 елемента до 31
		for (int j = 0 ; j < column_2 / 8; j++)
		{
			Bx1[i][j] = B[i][j];
		}
	}
	WriteToFile("Matrix", 1, Bx1, row_2, column_2 / 8, "Matrix Bx1 : ");
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//									Відправляємо смуги матриці А по процесам лишаючи собі 1/8 матриці			DONE
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	long double** A2;
	InitMatrix(A2, (row_1 / 8) * 6, column_1);		// 168 рядків 248 стовпців
	int l = 0;
	for (int i = (row_1 / 8); i < (row_1 / 8) * 7; i++)		//від 34 рядка заповняю матрицю А1
	{
		for (int j = 0; j < column_1; j++)
		{
			A2[l][j] = A[i][j];
		}
		l++;
	}
	//WriteToFile("Matrix", 1, A2, (row_1 / 8) * 6, column_1, "Matrix A2:");
	for (int i = 0; i < (row_1 / 8) * 6; i++)				//Від 62 елемента
	{
		MPI_Send(&(A2[i][0]), column_1, MPI_LONG_DOUBLE, 2, tagA2, MPI_COMM_WORLD);
	}
	cout << "Send submatrix A2 from processor with rank 1 to processor with rank 2" << endl;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										Відправляємо смуги матриці B по процесам лишаючи собі 1/8 матриці		DONE
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	long double** B2;
	InitMatrix(B2, row_2, (column_2 / 8) * 6);			//248 рядків і 186 стовпців
	int k = 0;
	for (int i = 0; i < row_2; i++)
	{
		for (int j = (column_2 / 8); j < (column_2 / 8) * 7; j++)
		{
			B2[i][k] = B[i][j];
			k++;
		}
		k = 0;
	}
	//WriteToFile("Matrix", 1, B2, row_2, (column_2 / 8) * 6, "Matrix B2:");
	for (int i = 0; i < row_2; i++)
	{
		MPI_Send(&(B2[i][0]), (column_2 / 8) * 6, MPI_LONG_DOUBLE, 2, tagB2, MPI_COMM_WORLD);
	}
	cout << "Send submatrix B2 from processor with rank 1 to processor with rank 2" << endl;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
//										Множення матриці Ax1 * Bx1		DONE
/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Ax1_Bx1;
	InitMatrix(Ax1_Bx1, row_1 / 8, column_2 / 8);
	Ax1_Bx1 = MulMatrixes(row_1 / 8, column_2 / 8, Ax1, Bx1);
	WriteToFile("Matrix", 1, Ax1_Bx1, row_1 / 8, column_2 / 8, "Matrix Ax1 * Bx1 : ");
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
		//										Відправляємо смугу Bx1 в 2 процес		DONE
		/////////////////////////////////////////////////////////////////////////////////////////////////////////
	for (int i = 0; i < row_2; i++)
	{
			MPI_Send(&(Bx1[i][0]), column_2 / 8, MPI_LONG_DOUBLE, 2, tagBx1, MPI_COMM_WORLD);
	}
	cout << "Send Bx1 from processor with rank 1 to processor with rank 2" << endl;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										Приймаємо смугу Bx0 від 1 процесу		DONE
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Bx0;
	InitMatrix(Bx0, row_2, (column_2 / 8) + 3);
	for (int i = 0; i < row_2; i++)		
	{
			MPI_Recv(&(Bx0[i][0]), (column_2 / 8) + 3, MPI_LONG_DOUBLE, 0, tagBx0, MPI_COMM_WORLD, &status);
	}
	WriteToFile("Matrix", 1, Bx0, row_2,(column_2 / 8) + 3, "Matrix Bx0 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										Множення матриці Ax1 * Bx0		DONE
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Ax1_Bx0;
	InitMatrix(Ax1_Bx0, row_1 / 8, (column_2 / 8) + 3);
	Ax1_Bx0 = MulMatrixes(row_1 / 8, (column_2 / 8) + 3, Ax1, Bx0);
	WriteToFile("Matrix", 1, Ax1_Bx0, row_1 / 8, (column_2 / 8) + 3, "Matrix Ax1 * Bx0 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
//										Відправляємо смугу Bx0 в 2 процес		DONE
/////////////////////////////////////////////////////////////////////////////////////////////////////////
	for (int i = 0; i < row_2; i++)
	{
		MPI_Send(&(Bx0[i][0]), (column_2 / 8) + 3, MPI_LONG_DOUBLE, 2, tagBx0, MPI_COMM_WORLD);
	}
	cout << "Send Bx0 from processor with rank 1 to processor with rank 2" << endl;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										Приймаємо смугу Bx7 від 0 процесу		DONE
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Bx7;
	InitMatrix(Bx7, row_2, column_2 / 8);
	for (int i = 0; i < row_2; i++)
	{
		MPI_Recv(&(Bx7[i][0]), column_2 / 8, MPI_LONG_DOUBLE, 0, tagBx7, MPI_COMM_WORLD, &status);
	}
	WriteToFile("Matrix", 1, Bx7, row_2, column_2 / 8, "Matrix Bx7 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										Множення матриці Ax1 * Bx7		DONE
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Ax1_Bx7;
	InitMatrix(Ax1_Bx7, row_1 / 8, column_2 / 8);
	Ax1_Bx7 = MulMatrixes(row_1 / 8, column_2 / 8, Ax1, Bx7);
	WriteToFile("Matrix", 1, Ax1_Bx7, row_1 / 8, column_2 / 8, "Matrix Ax1 * Bx7 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										Відправляємо смугу Bx7 в 2 процес		DONE
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	for (int i = 0; i < row_2; i++)
	{
		MPI_Send(&(Bx7[i][0]), column_2 / 8, MPI_LONG_DOUBLE, 2, tagBx7, MPI_COMM_WORLD);
	}
	cout << "Send Bx7 from processor with rank 1 to processor with rank 2" << endl;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										Приймаємо смугу Bx6 від 0 процесу		DONE
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Bx6;
	InitMatrix(Bx6, row_2, column_2 / 8);
	for (int i = 0; i < row_2; i++)			
	{
		MPI_Recv(&(Bx6[i][0]), column_2 / 8, MPI_LONG_DOUBLE, 0, tagBx6, MPI_COMM_WORLD, &status);
	}
	WriteToFile("Matrix", 1, Bx6, row_2, column_2 / 8, "Matrix Bx6 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										Множення матриці Ax1 * Bx6		DONE
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Ax1_Bx6;
	InitMatrix(Ax1_Bx6, row_1 / 8, column_2 / 8);
	Ax1_Bx6 = MulMatrixes(row_1 / 8, column_2 / 8, Ax1, Bx6);
	WriteToFile("Matrix", 1, Ax1_Bx6, row_1 / 8, column_2 / 8, "Matrix Ax1 * Bx6 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										Відправляємо смугу Bx6 в 2 процес		DONE
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	for (int i = 0; i < row_2; i++)
	{
		MPI_Send(&(Bx6[i][0]), column_2 / 8, MPI_LONG_DOUBLE, 2, tagBx6, MPI_COMM_WORLD);
	}
	cout << "Send Bx6 from processor with rank 1 to processor with rank 2" << endl;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										Приймаємо смугу Bx5 від 0 процесу		DONE
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Bx5;
	InitMatrix(Bx5, row_2, column_2 / 8);
	for (int i = 0; i < row_2; i++)			
	{
		MPI_Recv(&(Bx5[i][0]), column_2 / 8, MPI_LONG_DOUBLE, 0, tagBx5, MPI_COMM_WORLD, &status);
	}
	WriteToFile("Matrix", 1, Bx5, row_2, column_2 / 8, "Matrix Bx5 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										Множення матриці Ax1 * Bx5		DONE
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Ax1_Bx5;
	InitMatrix(Ax1_Bx5, row_1 / 8, column_2 / 8);
	Ax1_Bx5 = MulMatrixes(row_1 / 8, column_2 / 8, Ax1, Bx5);
	WriteToFile("Matrix", 1, Ax1_Bx5, row_1 / 8, column_2 / 8, "Matrix Ax1 * Bx5 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										Відправляємо смугу Bx5 в 3 процес		DONE
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	for (int i = 0; i < row_2; i++)
	{
		MPI_Send(&(Bx5[i][0]), column_2 / 8, MPI_LONG_DOUBLE, 3, tagBx5, MPI_COMM_WORLD);
	}
	cout << "Send Bx5 from processor with rank 1 to processor with rank 2" << endl;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										Приймаємо смугу Bx3 від 7 процесу		DONE
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Bx3;
	InitMatrix(Bx3, row_2, column_2 / 8);
	for (int i = 0; i < row_2; i++)			
	{
		MPI_Recv(&(Bx3[i][0]), column_2 / 8, MPI_LONG_DOUBLE, 7, tagBx3, MPI_COMM_WORLD, &status);
	}
	WriteToFile("Matrix", 1, Bx3, row_2, column_2 / 8, "Matrix Bx3 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										Множення матриці Ax1 * Bx3		DONE
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Ax1_Bx3;
	InitMatrix(Ax1_Bx3, row_1 / 8, column_2 / 8);
	Ax1_Bx3 = MulMatrixes(row_1 / 8, column_2 / 8, Ax1, Bx3);
	WriteToFile("Matrix", 1, Ax1_Bx3, row_1 / 8, column_2 / 8, "Matrix Ax1 * Bx3 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										Відправляємо смугу Bx3 в 2 процес		DONE
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	for (int i = 0; i < row_2; i++)
	{
		MPI_Send(&(Bx3[i][0]), column_2 / 8, MPI_LONG_DOUBLE, 2, tagBx3, MPI_COMM_WORLD);
	}
	cout << "Send Bx3 from processor with rank 1 to processor with rank 2" << endl;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										Приймаємо смугу Bx2 від 0 процесу		DONE
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Bx2;
	InitMatrix(Bx2, row_2, column_2 / 8);
	for (int i = 0; i < row_2; i++)			
	{
		MPI_Recv(&(Bx2[i][0]), column_2 / 8, MPI_LONG_DOUBLE, 0, tagBx2, MPI_COMM_WORLD, &status);
	}
	WriteToFile("Matrix", 1, Bx2, row_2, column_2 / 8, "Matrix Bx2 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										Множення матриці Ax1 * Bx2		DONE
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Ax1_Bx2;
	InitMatrix(Ax1_Bx2, row_1 / 8, column_2 / 8);
	Ax1_Bx2 = MulMatrixes(row_1 / 8, column_2 / 8, Ax1, Bx2);
	WriteToFile("Matrix", 1, Ax1_Bx2, row_1 / 8, column_2 / 8, "Matrix Ax1 * Bx2 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										Відправляємо смугу Bx2 в 7 процес		DONE
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	for (int i = 0; i < row_2; i++)
	{
		MPI_Send(&(Bx2[i][0]), column_2 / 8, MPI_LONG_DOUBLE, 7, tagBx2, MPI_COMM_WORLD);
	}
	cout << "Send Bx2 from processor with rank 1 to processor with rank 7" << endl;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										Приймаємо смугу Bx4 від 3 процесу		DONE
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Bx4;
	InitMatrix(Bx4, row_2, column_2 / 8);
	for (int i = 0; i < row_2; i++)
	{
		MPI_Recv(&(Bx4[i][0]), column_2 / 8, MPI_LONG_DOUBLE, 3, tagBx4, MPI_COMM_WORLD, &status);
	}
	WriteToFile("Matrix", 1, Bx4, row_2, column_2 / 8, "Matrix Bx4 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										Множення матриці Ax1 * Bx4		DONE
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Ax1_Bx4;
	InitMatrix(Ax1_Bx4, row_1 / 8, column_2 / 8);
	Ax1_Bx4 = MulMatrixes(row_1 / 8, column_2 / 8, Ax1, Bx4);
	WriteToFile("Matrix", 1, Ax1_Bx4, row_1 / 8, column_2 / 8, "Matrix Ax1 * Bx4 : ");
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
		//										Збираю підматрицю до купи	
		/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** proc1_res;
	InitMatrix(proc1_res, row_1 / 8, column_2);
	int point = 0;
	for (int i = 0; i < (row_1 / 8); i++)
	{
		for (int j = 0; j < (column_2 / 8) + 3; j++)	//Тоді заповняємо матрицю від 0 до 34 елемента стовпця
		{
			proc1_res[i][j] = Ax1_Bx0[i][j];
		}
	}

	for (int i = 0; i < (row_1 / 8); i++)
	{
		for (int j = (column_2 / 8) + 3; j < ((column_2 / 8) * 2) + 3; j++)
		{
			proc1_res[i][j] = Ax1_Bx1[i][point];
			point++;
		}
		point = 0;
	}
	for (int i = 0; i < (row_1 / 8); i++)
	{
		for (int j = ((column_2 / 8) * 2) + 3; j < ((column_2 / 8) * 3) + 3; j++)
		{
			proc1_res[i][j] = Ax1_Bx2[i][point];

			point++;
		}
		point = 0;
	}
	for (int i = 0; i < (row_1 / 8); i++)
	{
		for (int j = ((column_2 / 8) * 3) + 3; j < ((column_2 / 8) * 4) + 3; j++)
		{
			proc1_res[i][j] = Ax1_Bx3[i][point];
			point++;
		}
		point = 0;
	}
	for (int i = 0; i < (row_1 / 8); i++)
	{
		for (int j = ((column_2 / 8) * 4) + 3; j < ((column_2 / 8) * 5) + 3; j++)
		{
			proc1_res[i][j] = Ax1_Bx4[i][point];
			point++;
		}
		point = 0;
	}
	for (int i = 0; i < (row_1 / 8); i++)
	{
		for (int j = ((column_2 / 8) * 5) + 3; j < ((column_2 / 8) * 6) + 3; j++)
		{
			proc1_res[i][j] = Ax1_Bx5[i][point];
			point++;
		}
		point = 0;
	}
	for (int i = 0; i < (row_1 / 8); i++)
	{
		for (int j = ((column_2 / 8) * 6) + 3; j < ((column_2 / 8) * 7) + 3; j++)
		{
			proc1_res[i][j] = Ax1_Bx6[i][point];
			point++;
		}
		point = 0;
	}
	for (int i = 0; i < (row_1 / 8); i++)
	{
		for (int j = ((column_2 / 8) * 7) + 3; j < column_2; j++)
		{
			proc1_res[i][j] = Ax1_Bx7[i][point];

			point++;
		}
		point = 0;
	}

	WriteToFile("Matrix", 1, proc1_res, (row_1 / 8), column_2, "Matrix Proc1_res : ");

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
	long double** proc2_res;
	InitMatrix(proc2_res, row_1 / 8, column_2);

	for (int i = 0; i < row_1 / 8; i++)
	{
		MPI_Recv(&(proc5_res[i][0]), column_2, MPI_LONG_DOUBLE, 2, tagA5, MPI_COMM_WORLD, &status);
		MPI_Recv(&(proc7_res[i][0]), column_2, MPI_LONG_DOUBLE, 2, tagA7, MPI_COMM_WORLD, &status);
		MPI_Recv(&(proc6_res[i][0]), column_2, MPI_LONG_DOUBLE, 2, tagA6, MPI_COMM_WORLD, &status);
		MPI_Recv(&(proc4_res[i][0]), column_2, MPI_LONG_DOUBLE, 2, tagA4, MPI_COMM_WORLD, &status);
		MPI_Recv(&(proc3_res[i][0]), column_2, MPI_LONG_DOUBLE, 2, tagA3, MPI_COMM_WORLD, &status);
		MPI_Recv(&(proc2_res[i][0]), column_2, MPI_LONG_DOUBLE, 2, tagA2, MPI_COMM_WORLD, &status);
	}

	for (int i = 0; i < (row_1 / 8); i++)
	{
		MPI_Send(&(proc7_res[i][0]), column_2, MPI_LONG_DOUBLE, 0, tagA7, MPI_COMM_WORLD);
		MPI_Send(&(proc5_res[i][0]), column_2, MPI_LONG_DOUBLE, 0, tagA5, MPI_COMM_WORLD);
		MPI_Send(&(proc6_res[i][0]), column_2, MPI_LONG_DOUBLE, 0, tagA6, MPI_COMM_WORLD);
		MPI_Send(&(proc4_res[i][0]), column_2, MPI_LONG_DOUBLE, 0, tagA4, MPI_COMM_WORLD);
		MPI_Send(&(proc3_res[i][0]), column_2, MPI_LONG_DOUBLE, 0, tagA3, MPI_COMM_WORLD);
		MPI_Send(&(proc2_res[i][0]), column_2, MPI_LONG_DOUBLE, 0, tagA2, MPI_COMM_WORLD);
		MPI_Send(&(proc1_res[i][0]), column_2, MPI_LONG_DOUBLE, 0, tagA1, MPI_COMM_WORLD);
	}
	cout << "Send proc1_res, proc2_res, proc3_res, proc4_res, proc5_res, pro67_res, proc7_res from processor with rank 1 to processor with rank 0" << endl;
}