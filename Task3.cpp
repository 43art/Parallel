#include <iostream>
#include<mpi.h>

using namespace std;

int main(int argc, char* argv[]) {
	int N = 100, k, rank, world_size, ibeg, iend;
	double ProcSum = 0.0;
	double TotalSum;
	double startwtime = 0.0, endwtime;
	MPI_Status status;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	cout << "Process " << rank << " of " << world_size << " Started " << endl;

	double x[100];
	if (rank == 0) {
		for (int i = 0; i < N; i++) {
			x[i] = i;
		}
		startwtime = MPI_Wtime();
	}

	if (world_size > N)
		world_size = N;

	MPI_Bcast(x, N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	k = N / world_size;
	ibeg = k * rank;
	iend = k * (rank + 1);
	if (rank == world_size - 1)
		iend = N;

	for (int i = ibeg; i < iend; i++) {
		ProcSum = ProcSum + x[i];
	}

	if (rank == 0) {
		TotalSum = ProcSum;
		for (int i = 1; i < world_size; i++) {
			MPI_Recv(&ProcSum, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, &status);
			endwtime = MPI_Wtime();

			cout << "Sum of process number " << i << " = " << ProcSum << endl;
			TotalSum = TotalSum + ProcSum;
		}
	}
	else {
		MPI_Send(&ProcSum, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
	}
	if (rank == 0) {
		printf("\nTotal Sum = %10.f", TotalSum);
		cout << " time is " << endwtime - startwtime << endl;
	}
	MPI_Finalize();
}