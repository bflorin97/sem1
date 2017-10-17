/******************************************************************************
* FILE: rowColumnSort.c
* AUTHOR: Cristian Chilipirea
* Skeleton for shear sort
******************************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#define N 25
#define SQRTN 5 // has to be sqrt(N)

void displayMatrix(int m[][SQRTN]) {
	// DO NOT MODIFY
	int i,j;
	for(i = 0; i < SQRTN; i++) {
		for(j = 0; j < SQRTN; j++) {
			printf("%i ", m[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

void copyMatrixInVectorSnake(int *v, int m[][SQRTN]) {
	// DO NOT MODIFY
	int i, j;
	for(i=0;i<SQRTN;i++) {
		if(i%2 == 0) {
			for(j=0;j<SQRTN;j++) {
				v[i*SQRTN+j] = m[i][j];
			}
		} else {
			for(j=SQRTN;j>0;j--) {
				v[i*SQRTN+(SQRTN-j)] = m[i][j-1];
			}
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
	for(i = 0; i < N; i++) {
		printf("%i ", v[i]);
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
	srand(42);
	int m[SQRTN][SQRTN];
	int v[N];
	int vQSort[N];
	int i,j;
	int sorted = 0;
	// generate the vector v with random values
	// DO NOT MODIFY
	for(i = 0; i < SQRTN; i++)
		for(j = 0; j < SQRTN; j++)
			m[i][j] = rand()%10;

	// make copy to check it against qsort
	// DO NOT MODIFY
	copyMatrixInVectorSnake(vQSort, m);
	qsort(vQSort, N, sizeof(int), cmp);


	// sort the matrix m using ShearSort
	while (!sorted) {
		sorted = 1;
		int aux;
		int sortedRow;
		int sortedCol;

		#pragma omp parallel private(sortedRow, aux) shared(sorted)
		{
			#pragma omp for
			for (int i = 0; i < SQRTN; i++) {
				int sgn = 0;

				if (i % 2 == 0) {
					sgn = -1;
				} else {
					sgn = 1;
				}
				do {
					sortedRow = 1;
					for (int j = 0; j < SQRTN-1; j++) {
						if (sgn * (m[i][j] - m[i][j+1]) < 0) {
							aux = m[i][j];
							m[i][j] = m[i][j+1];
							m[i][j+1] = aux;

							sortedRow = 0;
							sorted = 0;
						}
					}
				} while (!sortedRow);
			}
		}

		#pragma omp parallel private(sortedCol, aux) shared(sorted)
		{
			#pragma omp for
			for (int i = 0; i < SQRTN; i++) {
				do {
					sortedCol = 1;
					for (int j = 0; j < SQRTN-1; j++) {
						if (m[j][i] > m[j+1][i]) {
							aux = m[j][i];
							m[j][i] = m[j+1][i];
							m[j+1][i] = aux;

							sortedCol = 0;
							sorted = 0;
						}
					}
				} while (!sortedCol);
			}
		}
	}

	displayMatrix(m);
	copyMatrixInVectorSnake(v, m);
	displayVector(v);
	displayVector(vQSort);
	compareVectors(v,vQSort);

	return 0;
}
