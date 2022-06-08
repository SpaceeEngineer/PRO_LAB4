//DataFile.cpp

#include "Header.h"

void WriteToFile(string tagOfData, int numOfProc, long double** Datas, int size_row, int size_column, string nameOfDatas)
{
	string path = "";
	if (numOfProc == 0) path = "Processor0.txt";
	if (numOfProc == 1) path = "Processor1.txt";
	if (numOfProc == 2) path = "Processor2.txt";
	if (numOfProc == 3) path = "Processor3.txt";
	if (numOfProc == 4) path = "Processor4.txt";
	if (numOfProc == 5) path = "Processor5.txt";
	if (numOfProc == 6) path = "Processor6.txt";
	if (numOfProc == 7) path = "Processor7.txt";
	if (numOfProc == 10) path = "Result.txt";
	//Визначення який тип даних записується в файл
	int WhatIs = (tagOfData == "Matrix") ? 1 : 0;

	ofstream file;
	file.open(path, ios_base::app);
	file << nameOfDatas << "\n";
	long double decimal, fraction;

	switch (WhatIs)
	{
	case 0 :
	{
		cout << "Incorrect type of Data" << endl;
		break;
	}
	case 1:
	{
		for (int i = 0; i < size_row; i++)
		{
			for (int j = 0; j < size_column; j++) 
			{
				fraction = modfl(Datas[i][j], &decimal);
				if (fraction == 0)
					file << "\t" << setprecision(0) << fixed << Datas[i][j];
				else file << "\t" << setprecision(1) << fixed << Datas[i][j];
			}
			file << endl;
			
		}
		break;
	}

	}
		
	file.close();
}