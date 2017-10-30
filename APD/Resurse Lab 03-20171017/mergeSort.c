/******************************************************************************
* FILE: mergeSort.c
* AUTHOR: Cristian Chilipirea
* Generates, sorts using merge sort and displays a vector
******************************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#define N 1048576 // needs to be power of 2

void merge(int A[], int Left, int Right, int End, int B[]) {
	// DO NOT MODIFY
	int iL = Left;
	int iR = Right;
	int j;

	for (j = Left; j < End; j++)
	{
		if (iL < Right && (iR >= End || A[iL] <= A[iR])) {
			B[j] = A[iL];
			iL++;
		} else {
			B[j] = A[iR];
			iR++;
		}
	}
}

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
	int *v = (int*)malloc(sizeof(int)*N);
	int *vQSort = (int*)malloc(sizeof(int)*N);
	int *aux;
	int *vNew = (int*)malloc(sizeof(int)*N);
	int i, width;

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

	for (width = 1; width < N; width = 2 * width) {

		#pragma omp parallel for
		for (i = 0; i < N; i = i + 2 * width) {
			merge(v, i, i+width, i + 2*width, vNew);
		}
		aux = v;
		v= vNew;
		vNew=aux;
	}


	//displayVector(vQSort);
	compareVectors(v, vQSort);

	return 0;
}
