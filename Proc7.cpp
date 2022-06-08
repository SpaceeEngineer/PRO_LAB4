//Proc7.cpp

#include "Header.h"


void CPU_7()
{
	MPI_Barrier(MPI_COMM_WORLD);
	cout << endl;
	MPI_Status status;
	
	long double** A;
	long double** B;

	InitMatrix(A, (row_1 / 8) * 2, column_1);   //62 ����� 248 ��������
	InitMatrix(B, row_2, (column_2 / 8) * 2);	//248 ����� 62 ��������
	int x = 0;
	for (int i = ((row_1 / 8) * 6) + 6; i < row_1; i++)
	{
		MPI_Recv(&(A[x][0]), column_1, MPI_LONG_DOUBLE, 6, tagA7, MPI_COMM_WORLD, &status);
		x++;
	}
	WriteToFile("Matrix", 7, A, (row_1 / 8) * 2, column_1, "Matrix A: ");

	for (int i = 0; i < row_2; i++)
		MPI_Recv(&(B[i][0]), (column_2 / 8) * 2, MPI_LONG_DOUBLE, 6, tagB7, MPI_COMM_WORLD, &status);
	WriteToFile("Matrix", 7, B, row_2, (column_2 / 8) * 2, "Matrix B: ");


	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//									�������� ������� � �� 1/8
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	long double** Ax7;

	InitMatrix(Ax7, row_1 / 8, column_1);

	for (int i = 0; i < (row_1 / 8); i++)
	{
		for (int j = 0; j < column_1; j++)
		{
			Ax7[i][j] = A[i][j];
		}
	}
	WriteToFile("Matrix", 7, Ax7, (row_1 / 8), column_1, "Matrix Ax7 : ");
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//									�������� ������� B �� 1/8 
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	long double** Bx7;

	InitMatrix(Bx7, row_2, column_2 / 8);

	for (int i = 0; i < row_2; i++)
	{
		for (int j = 0; j < column_2 / 8; j++)
		{
			Bx7[i][j] = B[i][j];
		}
	}
	WriteToFile("Matrix", 7, Bx7, row_2, column_2 / 8, "Matrix Bx7 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//									³���������� ����� ������� � �� �������� ������� ��� 1/8 �������
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	long double** A5;
	InitMatrix(A5, row_1 / 8, column_1);		// 196 ����� 248 ��������
	int l = 0;
	for (int i = (row_1 / 8); i < (row_1 / 8) * 2; i++)		//�� 34 ����� �������� ������� �1
	{
		for (int j = 0; j < column_1; j++)
		{
			A5[l][j] = A[i][j];
		}
		l++;
	}
	WriteToFile("Matrix", 7, A5, row_1 / 8, column_1, "Matrix A5: ");
	for (int i = 0; i < row_1 / 8; i++)
	{
		MPI_Send(&(A5[i][0]), column_1, MPI_LONG_DOUBLE, 5, tagA5, MPI_COMM_WORLD);
	}
	cout << "Send submatrix A5 from processor with rank 7 to processor with rank 5" << endl;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										³���������� ����� ������� B �� �������� ������� ��� 1/8 �������	DONE
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	long double** B5;
	InitMatrix(B5, row_2, column_2 / 8);
	int k = 0;
	for (int i = 0; i < row_2; i++)
	{
		for (int j = (column_2 / 8); j < (column_2 / 8) * 2; j++)
		{
			B5[i][k] = B[i][j];
			k++;
		}
		k = 0;
	}
	WriteToFile("Matrix", 7, B5, row_2, column_2 / 8, "Matrix B5: ");
	for (int i = 0; i < row_2; i++)
	{
		MPI_Send(&(B5[i][0]), column_2 / 8, MPI_LONG_DOUBLE, 5, tagB5, MPI_COMM_WORLD);
	}
	cout << "Send submatrix B5 from processor with rank 7 to processor with rank 5" << endl;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										�������� ������� Ax7 * Bx7		DONE
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	long double** Ax7_Bx7;
	InitMatrix(Ax7_Bx7, row_1 / 8, column_2 / 8);
	Ax7_Bx7 = MulMatrixes(row_1 / 8, column_2 / 8, Ax7, Bx7);
	WriteToFile("Matrix", 7, Ax7_Bx7, row_1 / 8, column_2 / 8, "Matrix Ax7 * Bx7 : ");
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
		//										³���������� ����� Bx7 � 0 ������
		/////////////////////////////////////////////////////////////////////////////////////////////////////////
	for (int i = 0; i < row_2; i++)
	{
		MPI_Send(&(Bx7[i][0]), column_2 / 8, MPI_LONG_DOUBLE, 0, tagBx7, MPI_COMM_WORLD);
	}
	cout << "Send Bx7 from processor with rank 7 to processor with rank 0" << endl;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										�������� ����� Bx6 �� 6 �������
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Bx6;
	InitMatrix(Bx6, row_2, column_2 / 8);
	for (int i = 0; i < row_2; i++)
	{
		MPI_Recv(&(Bx6[i][0]), column_2 / 8, MPI_LONG_DOUBLE, 6, tagBx6, MPI_COMM_WORLD, &status);
	}
	WriteToFile("Matrix", 7, Bx6, row_2, column_2 / 8, "Matrix Bx6 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										�������� ������� Ax7 * Bx6		DONE
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Ax7_Bx6;
	InitMatrix(Ax7_Bx6, row_1 / 8, column_2 / 8);
	Ax7_Bx6 = MulMatrixes(row_1 / 8, column_2 / 8, Ax7, Bx6);
	WriteToFile("Matrix", 7, Ax7_Bx6, row_1 / 8, column_2 / 8, "Matrix Ax7 * Bx6 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
//										³���������� ����� Bx6 � 0 ������
/////////////////////////////////////////////////////////////////////////////////////////////////////////
	for (int i = 0; i < row_2; i++)
	{
		MPI_Send(&(Bx6[i][0]), column_2 / 8, MPI_LONG_DOUBLE, 0, tagBx6, MPI_COMM_WORLD);
	}
	cout << "Send Bx6 from processor with rank 7 to processor with rank 0" << endl;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										�������� ����� Bx5 �� 6 �������
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Bx5;
	InitMatrix(Bx5, row_2, column_2 / 8);
	for (int i = 0; i < row_2; i++)
	{
		MPI_Recv(&(Bx5[i][0]), column_2 / 8, MPI_LONG_DOUBLE, 6, tagBx5, MPI_COMM_WORLD, &status);
	}
	WriteToFile("Matrix", 7, Bx5, row_2, column_2 / 8, "Matrix Bx5 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										�������� ������� Ax7 * Bx5		DONE
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Ax7_Bx5;
	InitMatrix(Ax7_Bx5, row_1 / 8, column_2 / 8);
	Ax7_Bx5 = MulMatrixes(row_1 / 8, column_2 / 8, Ax7, Bx5);
	WriteToFile("Matrix", 7, Ax7_Bx5, row_1 / 8, column_2 / 8, "Matrix Ax7 * Bx5 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										³���������� ����� Bx5 � 0 ������
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	for (int i = 0; i < row_2; i++)
	{
		MPI_Send(&(Bx5[i][0]), column_2 / 8, MPI_LONG_DOUBLE, 0, tagBx5, MPI_COMM_WORLD);
	}
	cout << "Send Bx5 from processor with rank 7 to processor with rank 0" << endl;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										�������� ����� Bx4 �� 6 �������
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Bx4;
	InitMatrix(Bx4, row_2, column_2 / 8);
	for (int i = 0; i < row_2; i++)
	{
		MPI_Recv(&(Bx4[i][0]), column_2 / 8, MPI_LONG_DOUBLE, 6, tagBx4, MPI_COMM_WORLD, &status);
	}
	WriteToFile("Matrix", 7, Bx4, row_2, column_2 / 8, "Matrix Bx4 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										�������� ������� Ax7 * Bx4		DONE
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Ax7_Bx4;
	InitMatrix(Ax7_Bx4, row_1 / 8, column_2 / 8);
	Ax7_Bx4 = MulMatrixes(row_1 / 8, column_2 / 8, Ax7, Bx4);
	WriteToFile("Matrix", 7, Ax7_Bx4, row_1 / 8, column_2 / 8, "Matrix Ax7 * Bx4 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										³���������� ����� Bx4 � 0 ������
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	for (int i = 0; i < row_2; i++)
	{
		MPI_Send(&(Bx4[i][0]), column_2 / 8, MPI_LONG_DOUBLE, 0, tagBx4, MPI_COMM_WORLD);
	}
	cout << "Send Bx4 from processor with rank 7 to processor with rank 0" << endl;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										�������� ����� Bx3 �� 6 �������
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Bx3;
	InitMatrix(Bx3, row_2, column_2 / 8);
	for (int i = 0; i < row_2; i++)
	{
		MPI_Recv(&(Bx3[i][0]), column_2 / 8, MPI_LONG_DOUBLE, 6, tagBx3, MPI_COMM_WORLD, &status);
	}
	WriteToFile("Matrix", 7, Bx3, row_2, column_2 / 8, "Matrix Bx3 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										�������� ������� Ax7 * Bx3
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Ax7_Bx3;
	InitMatrix(Ax7_Bx3, row_1 / 8, column_2 / 8);
	Ax7_Bx3 = MulMatrixes(row_1 / 8, column_2 / 8, Ax7, Bx3);
	WriteToFile("Matrix", 7, Ax7_Bx3, row_1 / 8, column_2 / 8, "Matrix Ax7 * Bx3 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										³���������� ����� Bx3 � 1 ������
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	for (int i = 0; i < row_2; i++)
	{
		MPI_Send(&(Bx3[i][0]), column_2 / 8, MPI_LONG_DOUBLE, 1, tagBx3, MPI_COMM_WORLD);
	}
	cout << "Send Bx3 from processor with rank 7 to processor with rank 1" << endl;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										�������� ����� Bx1 �� 5 �������
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Bx1;
	InitMatrix(Bx1, row_2, column_2 / 8);
	for (int i = 0; i < row_2; i++)
	{
		MPI_Recv(&(Bx1[i][0]), column_2 / 8, MPI_LONG_DOUBLE, 5, tagBx1, MPI_COMM_WORLD, &status);
	}
	WriteToFile("Matrix", 7, Bx1, row_2, column_2 / 8, "Matrix Bx1 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										�������� ������� Ax7 * Bx1		DONE
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Ax7_Bx1;
	InitMatrix(Ax7_Bx1, row_1 / 8, column_2 / 8);
	Ax7_Bx1 = MulMatrixes(row_1 / 8, column_2 / 8, Ax7, Bx1);
	WriteToFile("Matrix", 7, Ax7_Bx1, row_1 / 8, column_2 / 8, "Matrix Ax7 * Bx1 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										³���������� ����� Bx1 � 0 ������
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	for (int i = 0; i < row_2; i++)
	{
		MPI_Send(&(Bx1[i][0]), column_2 / 8, MPI_LONG_DOUBLE, 0, tagBx1, MPI_COMM_WORLD);
	}
	cout << "Send Bx1 from processor with rank 7 to processor with rank 0" << endl;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										�������� ����� Bx0 �� 6 �������	DONE
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Bx0;
	InitMatrix(Bx0, row_2, (column_2 / 8) + 3);
	for (int i = 0; i < row_2; i++)
	{
		MPI_Recv(&(Bx0[i][0]), (column_2 / 8) + 3, MPI_LONG_DOUBLE, 6, tagBx0, MPI_COMM_WORLD, &status);
	}
	WriteToFile("Matrix", 7, Bx0, row_2, (column_2 / 8) + 3, "Matrix Bx0 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										�������� ������� Ax7 * Bx0	DONE
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Ax7_Bx0;
	InitMatrix(Ax7_Bx0, row_1 / 8, (column_2 / 8) + 3);
	Ax7_Bx0 = MulMatrixes(row_1 / 8, (column_2 / 8) + 3, Ax7, Bx0);
	WriteToFile("Matrix", 7, Ax7_Bx0, row_1 / 8, (column_2 / 8) + 3, "Matrix Ax7 * Bx0 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										³���������� ����� Bx0 � 5 ������		DONE
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	for (int i = 0; i < row_2; i++)
	{
		MPI_Send(&(Bx0[i][0]), (column_2 / 8) + 3, MPI_LONG_DOUBLE, 5, tagBx0, MPI_COMM_WORLD);
	}
	cout << "Send Bx0 from processor with rank 7 to processor with rank 5" << endl;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										�������� ����� Bx2 �� 1 �������
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Bx2;
	InitMatrix(Bx2, row_2, column_2 / 8);
	for (int i = 0; i < row_2; i++)
	{
		MPI_Recv(&(Bx2[i][0]), column_2 / 8, MPI_LONG_DOUBLE, 1, tagBx2, MPI_COMM_WORLD, &status);
	}
	WriteToFile("Matrix", 7, Bx2, row_2, column_2 / 8, "Matrix Bx2 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										�������� ������� Ax7 * Bx2
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Ax7_Bx2;
	InitMatrix(Ax7_Bx2, row_1 / 8, column_2 / 8);
	Ax7_Bx2 = MulMatrixes(row_1 / 8, column_2 / 8, Ax7, Bx2);
	WriteToFile("Matrix", 7, Ax7_Bx2, row_1 / 8, column_2 / 8, "Matrix Ax7 * Bx2 : ");
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
//										������ ��������� �� ����	
/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** proc7_res;
	InitMatrix(proc7_res, row_1 / 8, column_2);
	int point = 0;
	for (int i = 0; i < (row_1 / 8); i++)
	{
		for (int j = 0; j < (column_2 / 8) + 3; j++)	//��� ���������� ������� �� 0 �� 34 �������� �������
		{
			proc7_res[i][j] = Ax7_Bx0[i][j];
		}
	}

	for (int i = 0; i < (row_1 / 8); i++)
	{
		for (int j = (column_2 / 8) + 3; j < ((column_2 / 8) * 2) + 3; j++)
		{
			proc7_res[i][j] = Ax7_Bx1[i][point];
			point++;
		}
		point = 0;
	}
	for (int i = 0; i < (row_1 / 8); i++)
	{
		for (int j = ((column_2 / 8) * 2) + 3; j < ((column_2 / 8) * 3) + 3; j++)
		{
			proc7_res[i][j] = Ax7_Bx2[i][point];

			point++;
		}
		point = 0;
	}
	for (int i = 0; i < (row_1 / 8); i++)
	{
		for (int j = ((column_2 / 8) * 3) + 3; j < ((column_2 / 8) * 4) + 3; j++)
		{
			proc7_res[i][j] = Ax7_Bx3[i][point];
			point++;
		}
		point = 0;
	}
	for (int i = 0; i < (row_1 / 8); i++)
	{
		for (int j = ((column_2 / 8) * 4) + 3; j < ((column_2 / 8) * 5) + 3; j++)
		{
			proc7_res[i][j] = Ax7_Bx4[i][point];
			point++;
		}
		point = 0;
	}
	for (int i = 0; i < (row_1 / 8); i++)
	{
		for (int j = ((column_2 / 8) * 5) + 3; j < ((column_2 / 8) * 6) + 3; j++)
		{
			proc7_res[i][j] = Ax7_Bx5[i][point];
			point++;
		}
		point = 0;
	}
	for (int i = 0; i < (row_1 / 8); i++)
	{
		for (int j = ((column_2 / 8) * 6) + 3; j < ((column_2 / 8) * 7) + 3; j++)
		{
			proc7_res[i][j] = Ax7_Bx6[i][point];
			point++;
		}
		point = 0;
	}
	for (int i = 0; i < (row_1 / 8); i++)
	{
		for (int j = ((column_2 / 8) * 7) + 3; j < column_2; j++)
		{
			proc7_res[i][j] = Ax7_Bx7[i][point];

			point++;
		}
		point = 0;
	}

	WriteToFile("Matrix", 7, proc7_res, (row_1 / 8), column_2, "Matrix proc7_res : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										��������� � ������� ��������� �� ���������	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	long double** proc5_res;
	InitMatrix(proc5_res, row_1 / 8, column_2);
	for (int i = 0; i < row_1 / 8; i++)
	{
		MPI_Recv(&(proc5_res[i][0]), column_2, MPI_LONG_DOUBLE, 5, tagA5, MPI_COMM_WORLD, &status);
	}
	//WriteToFile("Matrix", 7, proc5_res, (row_1 / 8), column_2, "Matrix proc5_res : ");

	for (int i = 0; i < (row_1 / 8); i++)
	{
		MPI_Send(&(proc7_res[i][0]), column_2, MPI_LONG_DOUBLE, 6, tagA7, MPI_COMM_WORLD);
		MPI_Send(&(proc5_res[i][0]), column_2, MPI_LONG_DOUBLE, 6, tagA5, MPI_COMM_WORLD);
	}
	cout << "Send proc5_res, proc7_res from processor with rank 7 to processor with rank 6" << endl;
}