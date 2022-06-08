//Proc0.cpp

#include "Header.h"




void CPU_0()
{
	MPI_Barrier(MPI_COMM_WORLD);
	cout << endl;
	MPI_Status status;
	clock_t start;

	long double** A;
	long double** B;

	Input(A, B);
	start = clock();

	//Calculate_Time();

	//WriteToFile("Matrix", 0, A, row_1, column_1, "Matrix A: ");
	//WriteToFile("Matrix", 0, B, row_2, column_2, "Matrix B: ");


	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//									�������� ������� � �� 1/8   DONE
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Ax0;									// �� 34 �����
	InitMatrix(Ax0, (row_1 / 8) + 6, column_1);        // 34 ����� 248 ��������

	for (int i = 0; i < (row_1 / 8) + 6; i++)
	{
		for (int j = 0; j < column_1; j++)
		{
			Ax0[i][j] = A[i][j];
		}
	}
	WriteToFile("Matrix", 0, Ax0, (row_1 / 8) + 6, column_1, "Matrix Ax0:");
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//									�������� ������� B �� 8 ��������	DONE
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	long double** Bx0;

	InitMatrix(Bx0, row_2, (column_2 / 8) + 3);	 // 248 ����� 34 ��������
	for (int i = 0; i < row_2; i++)
	{
		for (int j = 0; j < (column_2 / 8) + 3; j++)
		{
			Bx0[i][j] = B[i][j];
		}
	}
	WriteToFile("Matrix", 0, Bx0, row_2, (column_2 / 8) + 3, "Matrix Bx0:");
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//									³���������� ����� ������� � �� �������� ������� ��� 1/8 �������  DONE
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	long double** A1;
	InitMatrix(A1, (row_1 / 8) * 7, column_1);		// 196 ����� 248 ��������
	int l = 0;
	for (int i = (row_1 / 8) + 6; i < row_1; i++)		//�� 34 ����� �������� ������� �1
	{
		for (int j = 0; j < column_1; j++)
		{
			A1[l][j] = A[i][j];
		}
		l++;
	}
	//WriteToFile("Matrix", 0, A1, (row_1 / 8) * 7, column_1, "Matrix A1:");
	for (int i = 0; i < (row_1 / 8) * 7; i++)
	{
		MPI_Send(&(A1[i][0]), column_1, MPI_LONG_DOUBLE, 1, tagA1, MPI_COMM_WORLD);
	}
	cout << "Send submatrix A1 from processor with rank 0 to processor with rank 1" << endl;


	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										³���������� ����� ������� B �� �������� ������� ��� 1/8 �������		DONE
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	long double** B1;
	InitMatrix(B1, row_2, (column_2 / 8) * 7);		// 248 ����� 217 ��������
	int k = 0;
	for (int i = 0; i < row_2; i++)
	{
		for (int j = (column_2 / 8) + 3; j < column_2; j++)		//�� 34 ������� �������� ������� �1
		{
			B1[i][k] = B[i][j];
			k++;
		}
		k = 0;
	}
	//WriteToFile("Matrix", 0, B1, row_2, (column_2 / 8) * 7, "Matrix B1:");
	for (int i = 0; i < row_2; i++)
	{
		MPI_Send(&(B1[i][0]), (column_2 / 8) * 7, MPI_LONG_DOUBLE, 1, tagB1, MPI_COMM_WORLD);
	}
	cout << "Send submatrix B1 from processor with rank 0 to processor with rank 1" << endl;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										�������� ������� Ax0 * Bx0		DONE	CHECKED
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	long double** Ax0_Bx0;
	InitMatrix(Ax0_Bx0, (row_1 / 8) + 6, (column_2 / 8) + 3);
	Ax0_Bx0 = MulMatrixes((row_1 / 8) + 6, (column_2 / 8) + 3, Ax0, Bx0);
	WriteToFile("Matrix", 0, Ax0_Bx0, (row_1 / 8) + 6, (column_2 / 8) + 3, "Matrix Ax0 * Bx0 : ");


	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										³���������� ����� Bx0 � 1 ������
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	for (int i = 0; i < row_2; i++)
	{
		MPI_Send(&(Bx0[i][0]), (column_2 / 8) + 3, MPI_LONG_DOUBLE, 1, tagBx0, MPI_COMM_WORLD);
	}
	cout << "Send Bx0 from processor with rank 0 to processor with rank 1" << endl;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										�������� ����� Bx7 �� 7 �������	DONE
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Bx7;
	InitMatrix(Bx7, row_2, column_2 / 8);
	for (int i = 0; i < row_2; i++)
	{
		MPI_Recv(&(Bx7[i][0]), column_2 / 8, MPI_LONG_DOUBLE, 7, tagBx7, MPI_COMM_WORLD, &status);
	}
	WriteToFile("Matrix", 0, Bx7, row_2, column_2 / 8, "Matrix Bx7 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										�������� ������� Ax0 * Bx7		DONE
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Ax0_Bx7;
	InitMatrix(Ax0_Bx7, (row_1 / 8) + 6, column_2 / 8);
	Ax0_Bx7 = MulMatrixes((row_1 / 8) + 6, column_2 / 8, Ax0, Bx7);
	WriteToFile("Matrix", 0, Ax0_Bx7, (row_1 / 8) + 6, column_2 / 8, "Matrix Ax0 * Bx7 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
//										³���������� ����� Bx7 � 1 ������	DONE
/////////////////////////////////////////////////////////////////////////////////////////////////////////
	for (int i = 0; i < row_2; i++)
	{
		MPI_Send(&(Bx7[i][0]), column_2 / 8, MPI_LONG_DOUBLE, 1, tagBx7, MPI_COMM_WORLD);
	}
	cout << "Send Bx7 from processor with rank 0 to processor with rank 1" << endl;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										�������� ����� Bx6 �� 7 �������		DONE
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Bx6;
	InitMatrix(Bx6, row_2, column_2 / 8);
	for (int i = 0; i < row_2; i++)
	{
		MPI_Recv(&(Bx6[i][0]), column_2 / 8, MPI_LONG_DOUBLE, 7, tagBx6, MPI_COMM_WORLD, &status);
	}
	WriteToFile("Matrix", 0, Bx6, row_2, column_2 / 8, "Matrix Bx6 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										�������� ������� Ax0 * Bx6		DONE
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Ax0_Bx6;
	InitMatrix(Ax0_Bx6, (row_1 / 8) + 6, column_2 / 8);
	Ax0_Bx6 = MulMatrixes((row_1 / 8) + 6, column_2 / 8, Ax0, Bx6);
	WriteToFile("Matrix", 0, Ax0_Bx6, (row_1 / 8) + 6, column_2 / 8, "Matrix Ax0 * Bx6 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										³���������� ����� Bx6 � 1 ������	DONE
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	for (int i = 0; i < row_2; i++)
	{
		MPI_Send(&(Bx6[i][0]), column_2 / 8, MPI_LONG_DOUBLE, 1, tagBx6, MPI_COMM_WORLD);
	}
	cout << "Send Bx6 from processor with rank 0 to processor with rank 1" << endl;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										�������� ����� Bx5 �� 7 �������	DONE
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Bx5;
	InitMatrix(Bx5, row_2, column_2 / 8);
	for (int i = 0; i < row_2; i++)
	{
		MPI_Recv(&(Bx5[i][0]), column_2 / 8, MPI_LONG_DOUBLE, 7, tagBx5, MPI_COMM_WORLD, &status);
	}
	WriteToFile("Matrix", 0, Bx5, row_2, column_2 / 8, "Matrix Bx5 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										�������� ������� Ax0 * Bx5		DONE
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Ax0_Bx5;
	InitMatrix(Ax0_Bx5, (row_1 / 8) + 6, column_2 / 8);
	Ax0_Bx5 = MulMatrixes((row_1 / 8) + 6, column_2 / 8, Ax0, Bx5);
	WriteToFile("Matrix", 0, Ax0_Bx5, (row_1 / 8) + 6, column_2 / 8, "Matrix Ax0 * Bx5 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										³���������� ����� Bx5 � 1 ������		DONE
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	for (int i = 0; i < row_2; i++)
	{
		MPI_Send(&(Bx5[i][0]), column_2 / 8, MPI_LONG_DOUBLE, 1, tagBx5, MPI_COMM_WORLD);
	}
	cout << "Send Bx5 from processor with rank 0 to processor with rank 1" << endl;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										�������� ����� Bx4 �� 7 �������	DONE
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Bx4;
	InitMatrix(Bx4, row_2, column_2 / 8);
	for (int i = 0; i < row_2; i++)
	{
		MPI_Recv(&(Bx4[i][0]), column_2 / 8, MPI_LONG_DOUBLE, 7, tagBx4, MPI_COMM_WORLD, &status);
	}
	WriteToFile("Matrix", 0, Bx4, row_2, column_2 / 8, "Matrix Bx4 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										�������� ������� Ax0 * Bx4		DONE
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Ax0_Bx4;
	InitMatrix(Ax0_Bx4, (row_1 / 8) + 6, column_2 / 8);
	Ax0_Bx4 = MulMatrixes((row_1 / 8) + 6, column_2 / 8, Ax0, Bx4);
	WriteToFile("Matrix", 0, Ax0_Bx4, (row_1 / 8) + 6, column_2 / 8, "Matrix Ax0 * Bx4 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										³���������� ����� Bx4 � 2 ������
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	for (int i = 0; i < row_2; i++)
	{
		MPI_Send(&(Bx4[i][0]), column_2 / 8, MPI_LONG_DOUBLE, 2, tagBx4, MPI_COMM_WORLD);
	}
	cout << "Send Bx4 from processor with rank 0 to processor with rank 2" << endl;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										�������� ����� Bx2 �� 6 �������		DONE
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Bx2;
	InitMatrix(Bx2, row_2, column_2 / 8);
	for (int i = 0; i < row_2; i++)
	{
		MPI_Recv(&(Bx2[i][0]), column_2 / 8, MPI_LONG_DOUBLE, 6, tagBx2, MPI_COMM_WORLD, &status);
	}
	WriteToFile("Matrix", 0, Bx2, row_2, column_2 / 8, "Matrix Bx2 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										�������� ������� Ax0 * Bx2	DONE
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Ax0_Bx2;
	InitMatrix(Ax0_Bx2, (row_1 / 8) + 6, column_2 / 8);
	Ax0_Bx2 = MulMatrixes((row_1 / 8) + 6, column_2 / 8, Ax0, Bx2);
	WriteToFile("Matrix", 0, Ax0_Bx2, (row_1 / 8) + 6, column_2 / 8, "Matrix Ax0 * Bx2 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										³���������� ����� Bx2 � 1 ������
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	for (int i = 0; i < row_2; i++)
	{
		MPI_Send(&(Bx2[i][0]), column_2 / 8, MPI_LONG_DOUBLE, 1, tagBx2, MPI_COMM_WORLD);
	}
	cout << "Send Bx2 from processor with rank 0 to processor with rank 1" << endl;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										�������� ����� Bx1 �� 7 �������		DONE
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Bx1;
	InitMatrix(Bx1, row_2, column_2 / 8);
	for (int i = 0; i < row_2; i++)			//�� ���������
	{
		MPI_Recv(&(Bx1[i][0]), column_2 / 8, MPI_LONG_DOUBLE, 7, tagBx1, MPI_COMM_WORLD, &status);
	}
	WriteToFile("Matrix", 0, Bx1, row_2, column_2 / 8, "Matrix Bx1 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										�������� ������� Ax0 * Bx1		DONE
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Ax0_Bx1;
	InitMatrix(Ax0_Bx1, (row_1 / 8) + 6, column_2 / 8);
	Ax0_Bx1 = MulMatrixes((row_1 / 8) + 6, column_2 / 8, Ax0, Bx1);
	WriteToFile("Matrix", 0, Ax0_Bx1, (row_1 / 8) + 6, column_2 / 8, "Matrix Ax0 * Bx1 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										³���������� ����� Bx1 � 6 ������
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	for (int i = 0; i < row_2; i++)
	{
		MPI_Send(&(Bx1[i][0]), column_2 / 8, MPI_LONG_DOUBLE, 6, tagBx1, MPI_COMM_WORLD);
	}
	cout << "Send Bx1 from processor with rank 0 to processor with rank 6" << endl;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										�������� ����� Bx3 �� 2 �������		DONE
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Bx3;
	InitMatrix(Bx3, row_2, column_2 / 8);
	for (int i = 0; i < row_2; i++)
	{
		MPI_Recv(&(Bx3[i][0]), column_2 / 8, MPI_LONG_DOUBLE, 2, tagBx3, MPI_COMM_WORLD, &status);
	}
	WriteToFile("Matrix", 0, Bx3, row_2, column_2 / 8, "Matrix Bx3 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										�������� ������� Ax0 * Bx3		DONE
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Ax0_Bx3;
	InitMatrix(Ax0_Bx3, (row_1 / 8) + 6, column_2 / 8);
	Ax0_Bx3 = MulMatrixes((row_1 / 8) + 6, column_2 / 8, Ax0, Bx3);
	WriteToFile("Matrix", 0, Ax0_Bx3, (row_1 / 8) + 6, column_2 / 8, "Matrix Ax0 * Bx3 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										������ ��������� �� ����	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** proc0_res;
	InitMatrix(proc0_res, (row_1 / 8) + 6, column_2);
	int point = 0;
	for (int i = 0; i < (row_1 / 8) + 6; i++)
	{
		for (int j = 0; j < (column_2 / 8) + 3; j++)	//��� ���������� ������� �� 0 �� 34 �������� �������
		{
			proc0_res[i][j] = Ax0_Bx0[i][j];
		}
	}
	
	for (int i = 0; i < (row_1 / 8) + 6; i++)
	{
		for (int j = (column_2 / 8) + 3; j < ((column_2 / 8)  * 2) + 3; j++)
		{
			proc0_res[i][j] = Ax0_Bx1[i][point];
			point++;
		}
		point = 0;
	}
	for (int i = 0; i < (row_1 / 8) + 6; i++)
	{
		for (int j = ((column_2 / 8) * 2) + 3; j < ((column_2 / 8) * 3) + 3; j++)
		{
			proc0_res[i][j] = Ax0_Bx2[i][point];
			
			point++;
		}
		point = 0;
	}
	for (int i = 0; i < (row_1 / 8) + 6; i++)
	{
		for (int j = ((column_2 / 8) * 3) + 3; j < ((column_2 / 8) * 4) + 3; j++)
		{
			proc0_res[i][j] = Ax0_Bx3[i][point];
			point++;
		}
		point = 0;
	}
	for (int i = 0; i < (row_1 / 8) + 6; i++)
	{
		for (int j = ((column_2 / 8) * 4) + 3; j < ((column_2 / 8) * 5) + 3; j++)
		{
			proc0_res[i][j] = Ax0_Bx4[i][point];
			point++;
		}
		point = 0;
	}
	for (int i = 0; i < (row_1 / 8) + 6; i++)
	{
		for (int j = ((column_2 / 8) * 5) + 3; j < ((column_2 / 8) * 6) + 3; j++)
		{
			proc0_res[i][j] = Ax0_Bx5[i][point];
			point++;
		}
		point = 0;
	}
	for (int i = 0; i < (row_1 / 8) + 6; i++)
	{
		for (int j = ((column_2 / 8) * 6) + 3; j < ((column_2 / 8) * 7) + 3; j++)
		{
			proc0_res[i][j] = Ax0_Bx6[i][point];
			point++;
		}
		point = 0;
	}
	for (int i = 0; i < (row_1 / 8) + 6; i++)
	{
		for (int j = ((column_2 / 8) * 7) + 3; j < column_2; j++)
		{
			proc0_res[i][j] = Ax0_Bx7[i][point];
			
			point++;
		}
		point = 0;
	}
	
	WriteToFile("Matrix", 0, proc0_res, (row_1/8) + 6 , column_2, "Matrix Proc0_res : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										��������� � ������� ��������� �� ���������	
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
	long double** proc1_res;
	InitMatrix(proc1_res, row_1 / 8, column_2);

	for (int i = 0; i < row_1 / 8; i++)
	{
		MPI_Recv(&(proc5_res[i][0]), column_2, MPI_LONG_DOUBLE, 1, tagA5, MPI_COMM_WORLD, &status);
		MPI_Recv(&(proc7_res[i][0]), column_2, MPI_LONG_DOUBLE, 1, tagA7, MPI_COMM_WORLD, &status);
		MPI_Recv(&(proc6_res[i][0]), column_2, MPI_LONG_DOUBLE, 1, tagA6, MPI_COMM_WORLD, &status);
		MPI_Recv(&(proc4_res[i][0]), column_2, MPI_LONG_DOUBLE, 1, tagA4, MPI_COMM_WORLD, &status);
		MPI_Recv(&(proc3_res[i][0]), column_2, MPI_LONG_DOUBLE, 1, tagA3, MPI_COMM_WORLD, &status);
		MPI_Recv(&(proc2_res[i][0]), column_2, MPI_LONG_DOUBLE, 1, tagA2, MPI_COMM_WORLD, &status);
		MPI_Recv(&(proc1_res[i][0]), column_2, MPI_LONG_DOUBLE, 1, tagA1, MPI_COMM_WORLD, &status);
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										��'����� ������� � ���������
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	long double** Result_Matrix;
	InitMatrix(Result_Matrix, row_1, column_2);
	point = 0;
	k = 0;
	for (int i = 0; i < row_1; i++)
	{
		for (int j = 0; j < column_2; j++)
		{
			if (i < (row_1 / 8) + 6)
			{
				Result_Matrix[i][j] = proc0_res[point][j];			// ���� ����� 34 
			}
			else if (i < ((row_1 / 8) * 2) + 6)
			{
				if (k == 0)
				{
					point = 0;
					k++;
				}
				Result_Matrix[i][j] = proc1_res[point][j];
			}
			else if (i < ((row_1 / 8) * 3) + 6)
			{
				if (k == 1)
				{
					point = 0;
					k++;
				}
				Result_Matrix[i][j] = proc2_res[point][j];
			}
			else if (i < ((row_1 / 8) * 4) + 6)
			{
				if (k == 2)
				{
					point = 0;
					k++;
				}
				Result_Matrix[i][j] = proc3_res[point][j];
			}
			else if (i < ((row_1 / 8) * 5) + 6)
			{
				if (k == 3)
				{
					point = 0;
					k++;
				}
				Result_Matrix[i][j] = proc4_res[point][j];
			}
			else if (i < ((row_1 / 8) * 6) + 6)
			{
				if (k == 4)
				{
					point = 0;
					k++;
				}
				Result_Matrix[i][j] = proc5_res[point][j];
			}
			else if (i < ((row_1 / 8) * 7) + 6)
			{
				if (k == 5)
				{
					point = 0;
					k++;
				}
				Result_Matrix[i][j] = proc6_res[point][j];
			}
			else
			{
				if (k == 6)
				{
					point = 0;
					k++;
				}
				Result_Matrix[i][j] = proc7_res[point][j];
			}
		}
		point++;
	}
	WriteToFile("Matrix", 10, Result_Matrix, row_1, column_2, "Matrix Result: ");

	clock_t end = clock();
	double seconds = (double)(end - start) / CLOCKS_PER_SEC;
	cout << endl;
	cout << "Parallel Time: " << seconds << endl << endl;

	clock_t start_seq = clock();
	long double** seq_res;
	InitMatrix(seq_res, row_1, column_2);
	seq_res = MulMatrixes(row_1, column_2, A, B);
	clock_t end_seq = clock();
	double seconds_seq = (double)(end_seq - start_seq) / CLOCKS_PER_SEC;
	cout << endl;
	cout << "Sequential Time: " << seconds_seq << endl << endl;


}