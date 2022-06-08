//Header.h

#pragma once
#include <iostream>
#include "mpi.h"
#include "mspms.h"
#include "pmidbg.h"
#include <fstream>
#include <iomanip>
#include <ctime>
#include <string>
#include <Windows.h>
#pragma comment( lib, "C:\\Program Files (x86)\\Microsoft SDKs\\MPI\\Lib\\x86\\msmpi.lib" )

using namespace std;

#define row_1		(230)
#define column_1	(248)
#define row_2		(248)
#define column_2    (251)

//Tags
#define tagA1 1
#define tagA2 2
#define tagA3 3
#define tagA4 4
#define tagA5 5
#define tagA6 6
#define tagA7 7

#define tagB1 1
#define tagB2 2
#define tagB3 3
#define tagB4 4
#define tagB5 5
#define tagB6 6
#define tagB7 7

#define tagAx0 0
#define tagAx1 1
#define tagAx2 2
#define tagAx3 3
#define tagAx4 4
#define tagAx5 5
#define tagAx6 6
#define tagAx7 7

#define tagBx0 0
#define tagBx1 1
#define tagBx2 2
#define tagBx3 3
#define tagBx4 4
#define tagBx5 5
#define tagBx6 6
#define tagBx7 7




//write to file
void WriteToFile(string tagOfData, int numOfProc, long double** Datas, int size_row,int size_column, string nameOfDatas);

//CPUs'
void CPU_0();
void CPU_1();
void CPU_2();
void CPU_3();
void CPU_4();
void CPU_5();
void CPU_6();
void CPU_7();



//input
void Fill_1(long double**& A, long double**& B);
void Fill_1_3(long double**& A, long double**& B);
void rand_0(long double**& A, long double**& B);
void rand_1(long double**& A, long double**& B);
void rand_2(long double**& A, long double**& B);

void Input(long double**& A, long double**& B);

void InitMatrix(long double**& Name, int size_row, int size_column);

//Operations
long double** MulMatrixes(int size_row, int size_column, long double** matrix1, long double** matrix2);



