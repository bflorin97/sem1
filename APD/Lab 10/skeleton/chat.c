#include<mpi.h>
#include<stdio.h>

/**
 * @author cristian.chilipirea
 * Run: mpirun -np 2 ./a.out
 */

int main(int argc, char * argv[]) {
	int rank;
	int nProcesses;
	int v;
	MPI_Init(&argc, &argv);
	MPI_Status stat;

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &nProcesses);
	printf("Hello from %i/%i\n", rank, nProcesses);
    MPI_Request request;

    char msg[100] = strdup("Hello from the other side");
    if (rank == 0) {
        MPI_Isend(msg, strlen(msg), MPI_CHAR, 1, 0, MPI_COMM_WORLD, request);
    }

    if (rank == 1) {
        MPI_Irecv(msg, strlen(msg), MPI_CHAR, )
    }

    if (rank == 1) {

    }

	MPI_Finalize();
	return 0;
}
