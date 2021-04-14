#include <mpi.h>
#include <iostream>

using namespace std;

int main(int argc, char* argv[]) {
	int N = 100, M = 100, k1, rank, world_size, ibeg, iend;
	double Max = 0, TotalMax;
	double startwtime = 0.0, endwtime;
	MPI_Status status;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	cout << "Process " << rank << " of " << world_size << " started" << endl;
	
	double x[100][100];
	if (rank == 0) {
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < M; j++) {
				x[i][j] = i + j;
			}
		}
		startwtime = MPI_Wtime();
	}
	if (world_size > N) {
		world_size = N;
	}
	MPI_Bcast(x, N * M, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	k1 = N / world_size;
	ibeg = k1 * rank;
	iend = k1 * (rank + 1);
	if (rank == world_size - 1) {
		iend = N;
	}
	for (int i = ibeg; i < iend; i++) {
		for (int j = 0; j < M; j++) {
			if (abs(x[i][j] > Max)) {
				Max = abs(x[i][j]);
			}
		}
	}

	if (rank == 0) {
		for (int i = 0; i < k1; i++) {
			for (int j = 0; j < M; j++) {
				if (abs(x[i][j] > Max)) {
					Max = abs(x[i][j]);
				}
			}
		}
		cout << "Max of process number " << 0 << " = " << Max << endl;
		TotalMax = Max;
		for (int i = 1; i < world_size; i++) {
			MPI_Recv(&Max, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, &status);
			endwtime = MPI_Wtime();

			cout << "Max of process number " << i << " = " << Max << endl;
			if (TotalMax < Max) {
				TotalMax = Max;
			}
		}
	}
	else {
		MPI_Send(&Max, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
	}
	if (rank == 0) {
		printf("\nTotal Sum = %10.2f", TotalMax);
		cout << " time is " << endwtime - startwtime << endl;
	}

	MPI_Finalize();

	return 0;
}