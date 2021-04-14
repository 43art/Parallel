#include <math.h>
#include <iostream>
#include <fstream>
#include "mpi.h"

using namespace std;

int main(int argc, char* argv[]) {
	int world_size, rank, next, prev, tag1 = 1, tag2 = 2, n = 100, buf[100], sbuf[100], procsum = 0, totalsum = 0, kol = 0, *x;
	double starttime, endtime;
	x = new int[n];
	for (int i = 0; i < n; i++) {
		x[i] = i;
	}

	MPI_Request reqs[2];
	MPI_Status stats[2];

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	starttime = MPI_Wtime();

	prev = rank - 1;
	next = rank + 1;

	if (rank == 0) {
		prev = world_size - 1;
	}
	if (rank == (world_size - 1)) {
		next = 0;
	}

	int k = n / world_size;
	int ibeg = k * rank;
	int iend = k * (rank + 1);

	if (rank == world_size - 1) {
		iend = n;
	}

	MPI_Irecv(&procsum, 1, MPI_INT, next, tag1, MPI_COMM_WORLD, &reqs[0]);

	cout << "Process number: " << rank << " send message " << procsum << " to process number " << next << endl;

	MPI_Irecv(&sbuf[rank], 1, MPI_INT, prev, tag1, MPI_COMM_WORLD, &reqs[1]);

	MPI_Waitall(2, reqs, stats);

	cout << "Process number: " << rank << " recieve message " << sbuf[rank] << " from process number " << prev << endl;

	endtime = MPI_Wtime();
	cout << "Time of process number " << rank << " is: " << endtime - starttime << endl;

	MPI_Finalize();
	return 0;
}