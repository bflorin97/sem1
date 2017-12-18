#include <mpi.h>
#include <stdio.h>

#define N 8

int main(int argc, char** argv) {
    int rank;
    int v[N];
    int p[N/4];
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

    MPI_Scatter(&v, N/4, MPI_INT, &p, N/4, MPI_INT, 0, MPI_COMM_WORLD);

    for (int i = 0; i < N/4; i++) {
        p[i] += 42;
    }

    MPI_Gather(&p, N/4, MPI_INT, &v, N/4, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        for (int i = 0; i < N; i++) {
            printf("v[%i] = %d\n", i, p[i]);
        }
    }

    MPI_Finalize();
    return 0;
}
