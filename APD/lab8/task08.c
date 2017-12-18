#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    int rank;
    int v = 5;
    int a, b;
    int nProcesses;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nProcesses);
    printf("Hello from %i/%i\n", rank, nProcesses);

    if (rank == 0) {
        v = 2;
        MPI_Send(&v, 1, MPI_INT, 2, 0, MPI_COMM_WORLD);
    } else if (rank == 1) {
        v = 3;
        MPI_Send(&v, 1, MPI_INT, 2, 1, MPI_COMM_WORLD);
    } else if (rank == 2) {
        MPI_Recv(&a, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&b, 1, MPI_INT, 1, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("a = %d, b = %d\n", a, b);
    }

    MPI_Finalize();
    return 0;
}
