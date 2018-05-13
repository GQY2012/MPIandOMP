#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <mpi.h>
#include <math.h>

double f(double);

int main(int argc,char *argv[]) {
	double starttime = 0.0, endtime = 0.0;
	double pi = 0.0, l = 0.0, sum = 0.0, x = 0.0;
	int numprocs = 0, myid = 0;
	int n = 0;
	double t1, t2;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

	if (myid == 0) {
		printf("输入n：\n");
		scanf("%d", &n);
		starttime = MPI_Wtime();
	}

	MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
	l = 1.0 / (double)n;//每个小矩形的宽度
	for (int i = myid + 1;i < n;i += numprocs) {
		x = l * ((double)i + 0.5);
		sum += f(x);
	}
	sum *= l;
	MPI_Reduce(&sum, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
	if (myid == 0) {
		endtime = MPI_Wtime();
		printf("pi：%f\n",pi);
		t1 = endtime - starttime;
		printf("并行时间：%fs\n", t1);
	}

	//串行
	sum = 0;
	starttime = MPI_Wtime();
	if (myid == 0) {
		l = 1.0 / (double)n;//每个小矩形的宽度
		for (int i = 1;i < n;i++) {
			x = l * ((double)i - 0.5);
			sum += f(x);
		}
		sum *= l;
		endtime = MPI_Wtime();
		printf("pi：%f\n", pi);
		t2 = endtime - starttime;
		printf("串行时间：%f\n", t2);
		printf("加速比：%f\n", t2 / t1);
	}

	MPI_Finalize();

	return 0;
}

double f(double x) {
	return 4 / (1 + x * x);
}