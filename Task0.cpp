#include <iostream>
#include "omp.h"

using namespace std;

int main()
{
	setlocale(LC_ALL, "Russian");
	cout << "Ogo" << endl;
	omp_set_num_threads(6); // кол-во параллельных потоков
	double t1 = omp_get_wtime();
	int n, i, s = 0;
#pragma omp parallel private(i,n) num_threads(3) reduction(+: s)//shared(n) - глобальная private(n) - локальная, создаются копии
	{
		n = omp_get_thread_num();
#pragma omp for
			for (i = 0; i < 10; i++) {
				cout << " нить " << n << "обрабатываемый элемент " << i << endl;
				s += i;
			}
	}
//#pragma omp parallel num_threads(2)
//	{
//		cout << "Новая параллельная часть " << omp_get_thread_num() << endl;
//	}
	cout << "s = " << s << endl;
	double t2 = omp_get_wtime(); // для засечки времени
	cout << "Последовательная часть" << endl;
    return 0;
}