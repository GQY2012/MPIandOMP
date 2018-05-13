#include <omp.h> 
#include <stdio.h>
#include <windows.h>
#include <time.h>

#define NUM_THREADS 4
LARGE_INTEGER nFreq;//cpu frequency  
LARGE_INTEGER starttime;//begin
LARGE_INTEGER endtime;//end

double f(double);

int main()
{
	int n = 1000000;
	double l;
	double x, pi, sum = 0.0;
	double t1, t2;

	QueryPerformanceFrequency(&nFreq);//��ȡcpuƵ��
	QueryPerformanceCounter(&starttime);//�㷨��ʼʱ��
	l = 1.0 / (double)n;
	omp_set_num_threads(NUM_THREADS);
#pragma omp parallel for reduction(+:sum) private(x) 
	for (int i = 0;i < n; i++) {
		x = ((double)i + 0.5)*l;
		sum = sum + f(x);
	}
	pi = l * sum;
	QueryPerformanceCounter(&endtime);
	t1 = (endtime.QuadPart - starttime.QuadPart) / (double)nFreq.QuadPart;
	printf("pi:%f\n", pi);
	printf("����ʱ��%f\n", t1);
	//����
	QueryPerformanceCounter(&starttime);
	sum = 0;
	for (int i = 0;i < n; i++) {
		x = ((double)i + 0.5)*l;
		sum = sum + f(x);
	}
	pi = l * sum;
	QueryPerformanceCounter(&endtime);
	t2 = (endtime.QuadPart - starttime.QuadPart) / (double)nFreq.QuadPart;
	printf("pi:%f\n", pi);
	printf("����ʱ��%f\n", t2);
	printf("���ٱȣ�%f\n", t2 / t1);

	system("pause");
	return 0;
}

double f(double x) {
	return 4 / (1 + x * x);
}
