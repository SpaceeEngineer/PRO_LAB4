//Proc3.cpp

#include "Header.h"


void CPU_3()
{
	MPI_Barrier(MPI_COMM_WORLD);
	cout << endl;
	MPI_Status status;
	
	long double** A;
	long double** B;

	InitMatrix(A, (row_1 / 8) * 5, column_1);    //140 ����� 248 ��������
	InitMatrix(B, row_2, (column_2 / 8) * 5);	 //248 ����� 155 ��������
	int x = 0;
	for (int i = ((row_1 / 8) * 3) + 6; i < row_1; i++)
	{
		MPI_Recv(&(A[x][0]), column_1, MPI_LONG_DOUBLE, 2, tagA3, MPI_COMM_WORLD, &status);
		x++;
	}
		
	//WriteToFile("Matrix", 3, A, (row_1 / 8) * 5, column_1, "Matrix A : ");

	for (int i = 0; i < row_2; i++)
		MPI_Recv(&(B[i][0]), (column_2 / 8) * 5, MPI_LONG_DOUBLE, 2, tagB3, MPI_COMM_WORLD, &status);
	//WriteToFile("Matrix", 3, B, row_2, (column_2 / 8) * 5, "Matrix B : ");


	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//									�������� ������� � �� 1/8
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	long double** Ax3;

	InitMatrix(Ax3, row_1 / 8, column_1);

	for (int i = 0; i < (row_1 / 8) ; i++)
	{
		for (int j = 0; j < column_1; j++)
		{
			Ax3[i][j] = A[i][j];
		}
	}
	WriteToFile("Matrix", 3, Ax3, row_1 / 8, column_1, "Matrix Ax3 : ");
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//									�������� ������� B �� 1/8 
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	long double** Bx3;

	InitMatrix(Bx3, row_2, column_2 / 8);

	for (int i = 0; i < row_2; i++)
	{
		for (int j = 0; j < (column_2 / 8); j++)
		{
			Bx3[i][j] = B[i][j];
		}
	}
	WriteToFile("Matrix", 3, Bx3, row_2, column_2 / 8, "Matrix Bx3 : ");
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//									³���������� ����� ������� � �� �������� ������� ��� 1/8 �������
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	long double** A4;
	InitMatrix(A4, (row_1 / 8) * 4, column_1);		// 112 ����� 248 ��������
	int l = 0;
	for (int i = (row_1 / 8); i < (row_1 / 8) * 5; i++)		
	{
		for (int j = 0; j < column_1; j++)
		{
			A4[l][j] = A[i][j];
		}
		l++;
	}
	//WriteToFile("Matrix", 3, A4, (row_1 / 8) * 4, column_1, "Matrix A4: ");
	for (int i = 0; i < (row_1 / 8) * 4; i++)
	{
		MPI_Send(&(A4[i][0]), column_1, MPI_LONG_DOUBLE, 4, tagA4, MPI_COMM_WORLD);
	}
	cout << "Send submatrix A4 from processor with rank 3 to processor with rank 4" << endl;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										³���������� ����� ������� B �� �������� ������� ��� 1/8 �������
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	long double** B4;
	InitMatrix(B4, row_2, (column_2 / 8) * 4);
	int k = 0;
	for (int i = 0; i < row_2; i++)
	{
		for (int j = (column_2 / 8); j < (column_2 / 8) * 5; j++)
		{
			B4[i][k] = B[i][j];
			k++;
		}
		k = 0;
	}
	//WriteToFile("Matrix", 3, B4, row_2, (column_2 / 8) * 4, "Matrix B4: ");
	for (int i = 0; i < row_2; i++)
	{
		MPI_Send(&(B4[i][0]), (column_2 / 8) * 4, MPI_LONG_DOUBLE, 4, tagB4, MPI_COMM_WORLD);
	}
	cout << "Send submatrix B4 from processor with rank 3 to processor with rank 4" << endl;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
//										�������� ������� Ax3 * Bx3		DONE
/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Ax3_Bx3;
	InitMatrix(Ax3_Bx3, row_1 / 8, column_2 / 8);
	Ax3_Bx3 = MulMatrixes(row_1 / 8, column_2 / 8, Ax3, Bx3);
	WriteToFile("Matrix", 3, Ax3_Bx3, row_1 / 8, column_2 / 8, "Matrix Ax3 * Bx3 : ");
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
		//										³���������� ����� Bx3 � 4 ������
		/////////////////////////////////////////////////////////////////////////////////////////////////////////
	for (int i = 0; i < row_2; i++)
	{
		MPI_Send(&(Bx3[i][0]), column_2 / 8, MPI_LONG_DOUBLE, 4, tagBx3, MPI_COMM_WORLD);
	}
	cout << "Send Bx3 from processor with rank 3 to processor with rank 4" << endl;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										�������� ����� Bx2 �� 2 �������
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Bx2;
	InitMatrix(Bx2, row_2, column_2 / 8);
	for (int i = 0; i < row_2; i++)
	{
		MPI_Recv(&(Bx2[i][0]), column_2 / 8, MPI_LONG_DOUBLE, 2, tagBx2, MPI_COMM_WORLD, &status);
	}
	WriteToFile("Matrix", 3, Bx2, row_2, column_2 / 8, "Matrix Bx2 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										�������� ������� Ax3 * Bx2
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Ax3_Bx2;
	InitMatrix(Ax3_Bx2, row_1 / 8, column_2 / 8);
	Ax3_Bx2 = MulMatrixes(row_1 / 8, column_2 / 8, Ax3, Bx2);
	WriteToFile("Matrix", 3, Ax3_Bx2, row_1 / 8, column_2 / 8, "Matrix Ax3 * Bx2 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
//										³���������� ����� Bx2 � 4 ������
/////////////////////////////////////////////////////////////////////////////////////////////////////////
	for (int i = 0; i < row_2; i++)
	{
		MPI_Send(&(Bx2[i][0]), column_2 / 8, MPI_LONG_DOUBLE, 4, tagBx2, MPI_COMM_WORLD);
	}
	cout << "Send Bx2 from processor with rank 3 to processor with rank 4" << endl;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										�������� ����� Bx1 �� 2 �������
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Bx1;
	InitMatrix(Bx1, row_2, column_2 / 8);
	for (int i = 0; i < row_2; i++)
	{
		MPI_Recv(&(Bx1[i][0]), column_2 / 8, MPI_LONG_DOUBLE, 2, tagBx1, MPI_COMM_WORLD, &status);
	}
	WriteToFile("Matrix", 3, Bx1, row_2, column_2 / 8, "Matrix Bx1 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										�������� ������� Ax3 * Bx1
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Ax3_Bx1;
	InitMatrix(Ax3_Bx1, row_1 / 8, column_2 / 8);
	Ax3_Bx1 = MulMatrixes(row_1 / 8, column_2 / 8, Ax3, Bx1);
	WriteToFile("Matrix", 3, Ax3_Bx1, row_1 / 8, column_2 / 8, "Matrix Ax3 * Bx1 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										³���������� ����� Bx1 � 4 ������
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	for (int i = 0; i < row_2; i++)
	{
		MPI_Send(&(Bx1[i][0]), column_2 / 8, MPI_LONG_DOUBLE, 4, tagBx1, MPI_COMM_WORLD);
	}
	cout << "Send Bx1 from processor with rank 3 to processor with rank 4" << endl;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										�������� ����� Bx0 �� 2 �������		DONE
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Bx0;
	InitMatrix(Bx0, row_2, (column_2 / 8) + 3);
	for (int i = 0; i < row_2; i++)
	{
		MPI_Recv(&(Bx0[i][0]), (column_2 / 8) + 3, MPI_LONG_DOUBLE, 2, tagBx0, MPI_COMM_WORLD, &status);
	}
	WriteToFile("Matrix", 3, Bx0, row_2, (column_2 / 8) + 3, "Matrix Bx0 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										�������� ������� Ax3 * Bx0		DONE
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Ax3_Bx0;
	InitMatrix(Ax3_Bx0, row_1 / 8, (column_2 / 8) + 3);
	Ax3_Bx0 = MulMatrixes(row_1 / 8, (column_2 / 8) + 3, Ax3, Bx0);
	WriteToFile("Matrix", 3, Ax3_Bx0, row_1 / 8, (column_2 / 8) + 3, "Matrix Ax3 * Bx0 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										³���������� ����� Bx0 � 4 ������		DONE
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	for (int i = 0; i < row_2; i++)
	{
		MPI_Send(&(Bx0[i][0]), (column_2 / 8) + 3, MPI_LONG_DOUBLE, 4, tagBx0, MPI_COMM_WORLD);
	}
	cout << "Send Bx0 from processor with rank 3 to processor with rank 4" << endl;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										�������� ����� Bx7 �� 2 �������
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Bx7;
	InitMatrix(Bx7, row_2, column_2 / 8);
	for (int i = 0; i < row_2; i++)
	{
		MPI_Recv(&(Bx7[i][0]), column_2 / 8 , MPI_LONG_DOUBLE, 2, tagBx7, MPI_COMM_WORLD, &status);
	}
	WriteToFile("Matrix", 3, Bx7, row_2, column_2 / 8, "Matrix Bx7 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										�������� ������� Ax3 * Bx7		DONE
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Ax3_Bx7;
	InitMatrix(Ax3_Bx7, row_1 / 8, column_2 / 8);
	Ax3_Bx7 = MulMatrixes(row_1 / 8, column_2 / 8, Ax3, Bx7);
	WriteToFile("Matrix", 3, Ax3_Bx7, row_1 / 8, column_2 / 8, "Matrix Ax3 * Bx7 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										³���������� ����� Bx7 � 5 ������
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	for (int i = 0; i < row_2; i++)
	{
		MPI_Send(&(Bx7[i][0]), column_2 / 8, MPI_LONG_DOUBLE, 5, tagBx7, MPI_COMM_WORLD);
	}
	cout << "Send Bx7 from processor with rank 3 to processor with rank 5" << endl;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										�������� ����� Bx5 �� 1 �������
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Bx5;
	InitMatrix(Bx5, row_2, column_2 / 8);
	for (int i = 0; i < row_2; i++)
	{
		MPI_Recv(&(Bx5[i][0]), column_2 / 8, MPI_LONG_DOUBLE, 1, tagBx5, MPI_COMM_WORLD, &status);
	}
	WriteToFile("Matrix", 3, Bx5, row_2, column_2 / 8, "Matrix Bx5 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										�������� ������� Ax3 * Bx5
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Ax3_Bx5;
	InitMatrix(Ax3_Bx5, row_1 / 8, column_2 / 8);
	Ax3_Bx5 = MulMatrixes(row_1 / 8, column_2 / 8, Ax3, Bx5);
	WriteToFile("Matrix", 3, Ax3_Bx5, row_1 / 8, column_2 / 8, "Matrix Ax3 * Bx5 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										³���������� ����� Bx5 � 4 ������
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	for (int i = 0; i < row_2; i++)
	{
		MPI_Send(&(Bx5[i][0]), column_2 / 8, MPI_LONG_DOUBLE, 4, tagBx5, MPI_COMM_WORLD);
	}
	cout << "Send Bx5 from processor with rank 3 to processor with rank 4" << endl;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										�������� ����� Bx4 �� 2 �������
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Bx4;
	InitMatrix(Bx4, row_2, column_2 / 8);
	for (int i = 0; i < row_2; i++)
	{
		MPI_Recv(&(Bx4[i][0]), column_2 / 8, MPI_LONG_DOUBLE, 2, tagBx4, MPI_COMM_WORLD, &status);
	}
	WriteToFile("Matrix", 3, Bx4, row_2, column_2 / 8, "Matrix Bx4 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										�������� ������� Ax3 * Bx4
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Ax3_Bx4;
	InitMatrix(Ax3_Bx4, row_1 / 8, column_2 / 8);
	Ax3_Bx4 = MulMatrixes(row_1 / 8, column_2 / 8, Ax3, Bx4);
	WriteToFile("Matrix", 3, Ax3_Bx4, row_1 / 8, column_2 / 8, "Matrix Ax3 * Bx4 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										³���������� ����� Bx4 � 1 ������
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	for (int i = 0; i < row_2; i++)
	{
		MPI_Send(&(Bx4[i][0]), column_2 / 8, MPI_LONG_DOUBLE, 1, tagBx4, MPI_COMM_WORLD);
	}
	cout << "Send Bx4 from processor with rank 3 to processor with rank 1" << endl;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										�������� ����� Bx6 �� 5 �������
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Bx6;
	InitMatrix(Bx6, row_2, column_2 / 8);
	for (int i = 0; i < row_2; i++)
	{
		MPI_Recv(&(Bx6[i][0]), column_2 / 8, MPI_LONG_DOUBLE, 5, tagBx6, MPI_COMM_WORLD, &status);
	}
	WriteToFile("Matrix", 3, Bx6, row_2, column_2 / 8, "Matrix Bx6 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										�������� ������� Ax3 * Bx6		DONE
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** Ax3_Bx6;
	InitMatrix(Ax3_Bx6, row_1 / 8, column_2 / 8);
	Ax3_Bx6 = MulMatrixes(row_1 / 8, column_2 / 8, Ax3, Bx6);
	WriteToFile("Matrix", 3, Ax3_Bx6, row_1 / 8, column_2 / 8, "Matrix Ax3 * Bx6 : ");

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//										������ ��������� �� ����	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	long double** proc3_res;
	InitMatrix(proc3_res, row_1 / 8, column_2);
	int point = 0;
	for (int i = 0; i < (row_1 / 8); i++)
	{
		for (int j = 0; j < (column_2 / 8) + 3; j++)	//��� ���������� ������� �� 0 �� 34 �������� �������
		{
			proc3_res[i][j] = Ax3_Bx0[i][j];
		}
	}

	for (int i = 0; i < (row_1 / 8); i++)
	{
		for (int j = (column_2 / 8) + 3; j < ((column_2 / 8) * 2) + 3; j++)
		{
			proc3_res[i][j] = Ax3_Bx1[i][point];
			point++;
		}
		point = 0;
	}
	for (int i = 0; i < (row_1 / 8); i++)
	{
		for (int j = ((column_2 / 8) * 2) + 3; j < ((column_2 / 8) * 3) + 3; j++)
		{
			proc3_res[i][j] = Ax3_Bx2[i][point];

			point++;
		}
		point = 0;
	}
	for (int i = 0; i < (row_1 / 8); i++)
	{
		for (int j = ((column_2 / 8) * 3) + 3; j < ((column_2 / 8) * 4) + 3; j++)
		{
			proc3_res[i][j] = Ax3_Bx3[i][point];
			point++;
		}
		point = 0;
	}
	for (int i = 0; i < (row_1 / 8); i++)
	{
		for (int j = ((column_2 / 8) * 4) + 3; j < ((column_2 / 8) * 5) + 3; j++)
		{
			proc3_res[i][j] = Ax3_Bx4[i][point];
			point++;
		}
		point = 0;
	}
	for (int i = 0; i < (row_1 / 8); i++)
	{
		for (int j = ((column_2 / 8) * 5) + 3; j < ((column_2 / 8) * 6) + 3; j++)
		{
			proc3_res[i][j] = Ax3_Bx5[i][point];
			point++;
		}
		point = 0;
	}
	for (int i = 0; i < (row_1 / 8); i++)
	{
		for (int j = ((column_2 / 8) * 6) + 3; j < ((column_2 / 8) * 7) + 3; j++)
		{
			proc3_res[i][j] = Ax3_Bx6[i][point];
			point++;
		}
		point = 0;
	}
	for (int i = 0; i < (row_1 / 8); i++)
	{
		for (int j = ((column_2 / 8) * 7) + 3; j < column_2; j++)
		{
			proc3_res[i][j] = Ax3_Bx7[i][point];

			point++;
		}
		point = 0;
	}

	WriteToFile("Matrix", 3, proc3_res, (row_1 / 8), column_2, "Matrix proc3_res : ");

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

	for (int i = 0; i < row_1 / 8; i++)
	{
		MPI_Recv(&(proc5_res[i][0]), column_2, MPI_LONG_DOUBLE, 4, tagA5, MPI_COMM_WORLD, &status);
		MPI_Recv(&(proc7_res[i][0]), column_2, MPI_LONG_DOUBLE, 4, tagA7, MPI_COMM_WORLD, &status);
		MPI_Recv(&(proc6_res[i][0]), column_2, MPI_LONG_DOUBLE, 4, tagA6, MPI_COMM_WORLD, &status);
		MPI_Recv(&(proc4_res[i][0]), column_2, MPI_LONG_DOUBLE, 4, tagA4, MPI_COMM_WORLD, &status);
	}

	for (int i = 0; i < (row_1 / 8); i++)
	{
		MPI_Send(&(proc7_res[i][0]), column_2, MPI_LONG_DOUBLE, 2, tagA7, MPI_COMM_WORLD);
		MPI_Send(&(proc5_res[i][0]), column_2, MPI_LONG_DOUBLE, 2, tagA5, MPI_COMM_WORLD);
		MPI_Send(&(proc6_res[i][0]), column_2, MPI_LONG_DOUBLE, 2, tagA6, MPI_COMM_WORLD);
		MPI_Send(&(proc4_res[i][0]), column_2, MPI_LONG_DOUBLE, 2, tagA4, MPI_COMM_WORLD);
		MPI_Send(&(proc3_res[i][0]), column_2, MPI_LONG_DOUBLE, 2, tagA3, MPI_COMM_WORLD);
	}
	cout << "Send proc3_res, proc4_res, proc5_res, pro67_res, proc7_res from processor with rank 3 to processor with rank 2" << endl;
}