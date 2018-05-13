#include <omp.h> 
#include <stdio.h>
#include <windows.h>
#include <math.h>
#include <time.h>

#define NUM_THREADS 4

LARGE_INTEGER nFreq;//cpu frequency  
LARGE_INTEGER starttime;//begin
LARGE_INTEGER endtime;//end

bool isPrime(int);

int main()
{
	int n = 100000;
	int sum = 0;
	double t1, t2;
	QueryPerformanceFrequency(&nFreq);//��ȡcpuƵ��
	QueryPerformanceCounter(&starttime);//�㷨��ʼʱ��
	omp_set_num_threads(NUM_THREADS);
#pragma omp parallel for reduction(+:sum)
	for (int i = 1;i <= n;i += 2) {
		if (isPrime(i))
			sum++;
	}
	QueryPerformanceCounter(&endtime);
	t1 = (endtime.QuadPart - starttime.QuadPart) / (double)nFreq.QuadPart;
	printf("С�ڵ���%d����������Ϊ��%d\n", n, sum);
	printf("����ʱ��%f\n", t1);
	//����
	QueryPerformanceCounter(&starttime);
	sum = 0;
	for (int i = 1;i <= n;i += 2) {
		if (isPrime(i))
			sum++;
	}
	QueryPerformanceCounter(&endtime);
	t2 = (endtime.QuadPart - starttime.QuadPart) / (double)nFreq.QuadPart;
	printf("С�ڵ���%d����������Ϊ��%d\n", n, sum);
	printf("����ʱ��%f\n", t2);
	printf("���ٱȣ�%f\n", t2 / t1);

	system("pause");
	return 0;
}

bool isPrime(int num) {
	int mid = floor(sqrt((double)num));
	for (int i = 2;i <= mid;i++) {
		if (num % i == 0) {
			return false;
		}
	}
	return true;
}
