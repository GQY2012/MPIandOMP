#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <mpi.h>
#include <math.h>

bool isPrime(int);

int main(int argc, char *argv[]) {
	int myid, numprocs;
	int n = 0;
	int sum_per_proc = 0,sum = 0;
	double starttime, endtime;
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
	for (int i = myid * 2 + 1;i <= n;i += numprocs * 2) {
		if (isPrime(i)) {
			sum_per_proc++;
		}
	}
	MPI_Reduce(&sum_per_proc, &sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
	if (myid == 0) {
		endtime = MPI_Wtime();
		printf("小于等于%d的素数总数为：%d\n", n, sum);
		t1 = endtime - starttime;
		printf("并行时间为：%fs\n", t1);
	}
	//串行
	sum = 0;
	starttime = MPI_Wtime();
	if (myid == 0){
		for (int i = 1;i <= n;i += 2) {
			if (isPrime(i))
				sum++;
		}
		endtime = MPI_Wtime();
		printf("小于等于%d的素数总数为：%d\n",n, sum);
		t2 = endtime - starttime;
		printf("串行时间：%f\n", t2);
		printf("加速比：%f\n", t2 / t1);
	}

	MPI_Finalize();
	
	return 0;
}

bool isPrime(int num) {
	int mid = floor(sqrt((double) num));
	for (int i = 2;i <= mid;i++) {
		if (num % i == 0) {
			return false;
		}
	}
	return true;
}