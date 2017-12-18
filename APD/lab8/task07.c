#include <mpi.h>
#include <stdio.h>

#define MAX_RANK 4

int main(int argc, char** argv) {
    int rank;
    int v;
    int nProcesses;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nProcesses);
    printf("Hello from %i/%i\n", rank, nProcesses);

    if (rank == 0) {
        v = 0;
    }

    MPI_Barrier(MPI_COMM_WORLD);

    if (rank > 0) {
        MPI_Recv(&v, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        v++;
        int recv = rank + 1;
        if (recv == MAX_RANK) {
            recv = 0;
        }
        MPI_Send(&v, 1, MPI_INT, recv, 0, MPI_COMM_WORLD);
    }

    if (rank == 0) {
        MPI_Send(&v, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        MPI_Recv(&v, 1, MPI_INT, MAX_RANK - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    printf("%d: v = %d\n", rank, v);

    MPI_Finalize();
    return 0;
}
