#include<mpi.h>
#include<stdio.h>
#include<stdlib.h>

/**
 * @author cristian.chilipirea
 *
 */

int main(int argc, char * argv[]) {
	int rank;
	int nProcesses;
	int polynomialSize;
	int i;

	MPI_Init(&argc, &argv);

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &nProcesses);

	printf("Hello from %i/%i\n", rank, nProcesses);


	FILE * polFunctionFile = fopen("a1.txt","rt");
	fscanf(polFunctionFile, "%i", &polynomialSize);
	float * a = malloc(sizeof(float)*polynomialSize);
	for(i=0;i<polynomialSize;i++) {
		fscanf(polFunctionFile, "%f", a+i);
	}
	fclose(polFunctionFile);

	if(rank==0) { // This code is run by a single process
		for(int x=0; x<3; x+=1) {
			MPI_Send(&x, 1, MPI_INT, 1, 1, MPI_COMM_WORLD);
		}
	}
	else if (rank == 1) {
		for (int j = 0; j < 3; j++) {
			float sum = 0;
			float xPowI = 1;
			int x = 0;
			MPI_Recv(&x, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			printf("x = %d ", x);
			for(i=0;i<polynomialSize;i++) {
				sum += xPowI * a[i];
				xPowI *= x; // more efficient than pow(x,i);
			}
			printf("sum = %f \n", sum);
		}
	}

	printf("\n");
	MPI_Finalize();
	return 0;
}
