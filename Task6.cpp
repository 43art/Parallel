#include <iostream>
#include "mpi.h"
#include <fstream>
using namespace std;

int main(int argc, char* argv[]) {

	int numtasks, rank, sendcount, recvcount, source;
	int n = 20;
	int size = 4;
	int *sendbuf;
	sendbuf = new int[n];
	for (int i = 0; i < n; i++) {
		sendbuf[i] = i;
	}

	int *recvbuf;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
	int totalsum = 0;
	MPI_Bcast(sendbuf, n, MPI_INT, 0, MPI_COMM_WORLD);
	int k = n / numtasks;
	int ibeg = k * rank;
	int iend = k * (rank + 1);
	if (n % numtasks == 0) {
		sendcount = k;
		recvcount = k * (rank + 1);
		recvbuf = new int[k];

		MPI_Gather(&sendbuf[ibeg], sendcount, MPI_INT, recvbuf, recvcount, MPI_INT, 0, MPI_COMM_WORLD);

		for (int i = ibeg; i < iend; i++) {
			totalsum += recvbuf[i];
		}
		printf("Rank= %d. Send: %d %d %d %d \n", rank, sendbuf[ibeg], sendbuf[ibeg + 1], sendbuf[ibeg + 2], sendbuf[ibeg + 3]);
		printf("Rank= %d. Results: %d %d %d %d \n", rank, recvbuf[0], recvbuf[1], recvbuf[2], recvbuf[3]);
		printf("Sum = %d \n", totalsum);
	}
	else {
		printf("Число процессов должно быть кратно %d. \n", n);
	}
	MPI_Finalize();
}