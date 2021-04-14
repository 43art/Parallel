#include <stdio.h>
#include <mpi.h>
#include <vector>
#include <iostream>

/*int proc_count, proc_this, n;
const int k = 10;

using namespace std;

int main(int argc, char** argv)
{	
	setlocale(LC_ALL, "Russian");

	cout << "¬ведите количество процессоров, используемых на этапе работы программы: \n";

	cin >> n;

	//cout << "¬ведите размерность вектора: \n";

	//cin >> k;

	int a[k];

	for (int i = 0; i < n; i++) {
		a[i] = i + 1;
	}

	MPI_Init(&argc, &argv);

	MPI_Status status;

	MPI_Comm_size(MPI_COMM_WORLD, &proc_count);

	MPI_Comm_rank(MPI_COMM_WORLD, &proc_this);

	printf("Hello, IТm processor No_%d from %d \n", proc_this, proc_count);

	if (proc_this == 0) {
		MPI_Bcast(a, k, MPI_INT, 0, MPI_COMM_WORLD);
	}

	MPI_Finalize();

	return 0;
}
*/

using namespace std;
const int Tag = 0;
const int root = 0;

double sum_array(double* array, int n) {
    double sum = 0;
    for (int i = 0; i < n; ++i) {
        sum += array[i];
    }
    return sum;
}

int main() {
    int rank, commSize;

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &commSize);

    double* arr, sum = 0, buffer;
    int n;
    MPI_Status status;

    if (root == rank) {
        cin >> n;

        arr = new double[n];
        for (int i = 0; i < n; ++i)
            arr[i] = i + 1;

        int partSize = n / commSize;

        int shift = n % commSize;
        for (int i = root + 1; i < commSize; ++i) {
            MPI_Send(arr + shift + partSize * i, partSize, MPI_DOUBLE, i, Tag, MPI_COMM_WORLD);
        }

        sum = sum_array(arr, shift + partSize);

        for (int i = root + 1; i < commSize; ++i) {
            MPI_Recv(&buffer, 1, MPI_DOUBLE, i, Tag, MPI_COMM_WORLD, &status);
            sum += buffer;
        }
    }
    else {
        MPI_Probe(root, Tag, MPI_COMM_WORLD, &status);
        MPI_Get_count(&status, MPI_DOUBLE, &n);

        arr = new double[n];
        MPI_Recv(arr, n, MPI_DOUBLE, root, Tag, MPI_COMM_WORLD, &status);

        sum = sum_array(arr, n);

        MPI_Send(&sum, 1, MPI_DOUBLE, root, Tag, MPI_COMM_WORLD);
    }

    delete[] arr;

    cout << "Ќомер потока " << rank << " : " << sum << endl;
    MPI_Finalize();

    return 0;
}