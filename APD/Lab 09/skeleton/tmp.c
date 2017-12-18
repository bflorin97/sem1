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
    int n;
    int i;
    int j;
    int x;

    FILE * polFunctionFile = fopen("a1.txt","rt");
    fscanf(polFunctionFile, "%i", &polynomialSize);
    float * a = malloc(sizeof(float)*polynomialSize);
    for(i=0;i<polynomialSize;i++) {
        fscanf(polFunctionFile, "%f", a+i);
    }
    fclose(polFunctionFile);

	MPI_Init(&argc, &argv);

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &nProcesses);
	printf("Hello from %i/%i\n", rank, nProcesses);

    float sum = 0;
    float xPowI = 1;

    for(i=0;i<polynomialSize;i++) {
        sum += xPowI * a[i];
        xPowI *= rank; // rank instead of X
    }
	//printf("rank: %d sum: %d\n", rank, sum);
    printf("sum=%f\n", sum);
	if(rank==0) {
        float total = sum;
		for(x=1; x<nProcesses; x+=1) {
            float partialSum = 0;
            MPI_Recv(&partialSum, 1, MPI_INT, x, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("received from: %d sum: %f\n", x, partialSum);
            total += partialSum;
		}
        printf("%f\n", total);
    } else {
        MPI_Send(&sum, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
    }

	MPI_Finalize();
	return 0;
}
