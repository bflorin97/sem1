/******************************************************************************
* FILE: bubbleSort.c
* AUTHOR: Cristian Chilipirea
* Generates, sorts using Bubble sort and displays a vector
******************************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#define N 10000

void compareVectors(int * a, int * b) {
	// DO NOT MODIFY
	int i;
	for(i = 0; i < N; i++) {
		if(a[i]!=b[i]) {
			printf("Sorted incorrectly\n");
			return;
		}
	}
	printf("Sorted correctly\n");
}

void displayVector(int * v) {
	// DO NOT MODIFY
	int i;
	int displayWidth = 2 + log10(v[N-1]);
	for(i = 0; i < N; i++) {
		printf("%*i", displayWidth, v[i]);
	}
	printf("\n");
}

int cmp(const void *a, const void *b) {
	// DO NOT MODIFY
	int A = *(int*)a;
	int B = *(int*)b;
	return A-B;
}

int main(int argc, char *argv[]) {
	int sorted = 0;
	int aux;
	int *v = (int*)malloc(sizeof(int)*N);
	int *vQSort = (int*)malloc(sizeof(int)*N);
	int i;

	// generate the vector v with random values
	// DO NOT MODIFY
	srand(42);
	for(i = 0; i < N; i++)
		v[i] = rand()%N;

	// make copy to check it against qsort
	// DO NOT MODIFY
	for(i = 0; i < N; i++)
		vQSort[i] = v[i];
	qsort(vQSort, N, sizeof(int), cmp);

	// sort the vector v
	// PARALLELIZE ME
	while(!sorted) {
		sorted = 1;
		#pragma omp parallel private(aux)
		{
			#pragma omp for
			for(i = 0; i < N-1 / 2; i++) {
				int indexE = 2 * i;
				if(v[indexE] > v[indexE + 1]) {
					aux = v[indexE];
					v[indexE] = v[indexE + 1];
					v[indexE + 1] = aux;
					sorted = 0;
				}
			}

			#pragma omp for
			for(i = 1; i < N / 2; i++) {
				int indexO = 2 * i - 1;
				if (i + 1 < N && v[indexO] > v[indexO + 1]) {
					aux = v[indexO];
					v[indexO] = v[indexO + 1];
					v[indexO + 1] = aux;
					sorted = 0;
				}
			}
		}
	}

	//displayVector(vQSort);
	compareVectors(v, vQSort);

	return 0;
}
