#include <math.h>
#include <iostream>
#include <fstream>
#include "mpi.h"

using namespace std;

int main(int argc, char* argv[]) {
	int world_size, rank, next, prev, buf[2], tag1 = 1, tag2 = 2;
	double starttime, endtime;
	MPI_Request reqs[4];
	MPI_Status stats[4];

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

	MPI_Irecv(&buf[0], 1, MPI_INT, prev, tag1, MPI_COMM_WORLD, &reqs[0]);

	MPI_Irecv(&buf[1], 1, MPI_INT, next, tag2, MPI_COMM_WORLD, &reqs[1]);

	MPI_Isend(&rank, 1, MPI_INT, prev, tag2, MPI_COMM_WORLD, &reqs[2]);
	cout << "Process number: " << rank << " send message " << rank << " to process number " << prev << endl;

	MPI_Isend(&rank, 1, MPI_INT, next, tag1, MPI_COMM_WORLD, &reqs[3]);
	cout << "Process number: " << rank << " send message " << rank << " to process number " << next << endl;

	MPI_Waitall(4, reqs, stats);
	cout << "Process number: " << rank << " recieve message " << buf[0] << " from process number " << prev << endl;
	cout << "Process number: " << rank << " recieve message " << buf[1] << " from process number " << next << endl;

	printf("Node %d: all ok!\n", rank);

	endtime = MPI_Wtime();

	cout << "Time of process number: " << rank << " is " << endtime - starttime << endl;
	
	MPI_Finalize();

	return 0;
}