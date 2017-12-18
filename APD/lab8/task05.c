#include <mpi.h>
#include <stdio.h>

#define N 100

int main(int argc, char** argv) {
    int rank;
    int v[N];
    int nProcesses;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nProcesses);
    printf("Hello from %i/%i\n", rank, nProcesses);

    if (rank == 0) {
        for (int i = 0; i < N; i++) {
            v[i] = i;
        }
    }

    MPI_Bcast(&v, 100, MPI_INT, 0, MPI_COMM_WORLD);

    for (int i = 0; i < N; i++) {
        printf("v[%i] = %d\n", i, v[i]);
    }

    MPI_Finalize();
    return 0;
}
