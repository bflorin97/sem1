#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    int rank;
    int v = 5;
    int nProcesses;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nProcesses);
    printf("Hello from %i/%i\n", rank, nProcesses);

    if (rank == 0) {
        MPI_Send(&v, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
    }
    else if (rank == 1) {
        MPI_Recv(&v, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("v = %d\n", v);
    }

    MPI_Finalize();
    return 0;
}
