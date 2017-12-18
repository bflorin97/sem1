#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    int rank;
    int v;
    int nProcesses;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nProcesses);
    printf("Hello from %i/%i\n", rank, nProcesses);

    if (rank == 0) {
        v = 5;
    }

    MPI_Bcast(&v, 1, MPI_INT, 0, MPI_COMM_WORLD);
    printf("v = %d\n", v);

    MPI_Finalize();
    return 0;
}
