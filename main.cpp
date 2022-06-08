//main.cpp

#include "Header.h"



int main(int argc, char* argv[])
{
	int size, rank;
	MPI_Status Status;
	MPI_Init(&argc, &argv);				  // Initialize MPI program
	MPI_Comm_size(MPI_COMM_WORLD, &size); // Define number of process
	MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Define runk of process

	if (size != 8)
	{
		if (rank == 0)   printf("Error: 8 processes required instead of %d, abort", size);
		MPI_Barrier(MPI_COMM_WORLD);
		cout << endl;
		MPI_Abort(MPI_COMM_WORLD, MPI_ERR_OTHER); /* Целочисленный код ошибки */
		return -1;
	}
	if (rank == 0)
		CPU_0();

	if (rank == 1)
		CPU_1();

	if (rank == 2)
		CPU_2();
	
	if (rank == 3)
		CPU_3();
	
	if (rank == 4)
		CPU_4();
	
	if (rank == 5)
		CPU_5();
	
	if (rank == 6)
		CPU_6();
	
	if (rank == 7)
		CPU_7();
		
	MPI_Finalize(); //End of MPI program
	return 0;
}

