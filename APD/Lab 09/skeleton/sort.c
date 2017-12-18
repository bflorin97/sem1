#include<mpi.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#define N 10

/**
 * @author cristian.chilipirea
 *
 */


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

int main(int argc, char * argv[]) {
	int rank;
	int nProcesses;
	MPI_Init(&argc, &argv);

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &nProcesses);
	printf("Hello from %i/%i\n", rank, nProcesses);

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

	displayVector(v);

	int myValue[10], myIndex = 0;
	int recvValue;
	int isOver = 0;

	if(rank==0) { // This code is run by a single process
		for (i = 0; i < N; i++){
			recvValue = v[i];
			if (myIndex == 0) {
				myValue[myIndex++] = recvValue;
			} else if (myValue[myIndex - 1] > recvValue) {
				int index = myIndex - 1;
				while (index >= 0 && myValue[index] > recvValue) {
					index --;
				}
				myValue[myIndex] = myValue[index+1];
				myValue[index+1] = recvValue;
				myIndex ++;
			} else {
				printf("A sends=%d\n", recvValue);
				MPI_Send(&recvValue, 1, MPI_INT, 1, 1, MPI_COMM_WORLD);
			}
			printf("A index=%d\n", myIndex);
		}
		printf("done\n");
		for (int j = 0; j < myIndex; j++) {
			printf("%d ", myValue[j]);
			v[j] = myValue[j];
		}
		printf("\n");
		MPI_Bcast(&isOver, 1, MPI_INT, 0, MPI_COMM_WORLD);
		MPI_Send(&myIndex, 1, MPI_INT, 1, 2, MPI_COMM_WORLD);
		displayVector(v);
	}
	if (rank == 1) {
		while(!isOver) {
			MPI_Recv(&recvValue, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			printf("B recvs=%d\n", recvValue);
			if (myIndex == 0) {
				myValue[myIndex++] = recvValue;
			} else {
				int index = myIndex - 1;
				while (index >= 0 && myValue[index] > recvValue) {
					index--;
				}
				myValue[myIndex] = myValue[index+1];
				myValue[index+1] = recvValue;
				myIndex ++;
			}
			printf("B index=%d\n", myIndex);
			for (int j = 0; j < myIndex; j++) {
				printf("%d ", myValue[j]);
			}
			printf("\n");
		}
		int offset;
		MPI_Recv(&offset, 1, MPI_INT, 0, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		for (int j = 0; j < myIndex; j++) {
			v[j + offset] = myValue[j];
		}
		displayVector(v);
	}

	displayVector(vQSort);
	compareVectors(v, vQSort);

	MPI_Finalize();
	return 0;
}
