#include<mpi.h>
#include<stdio.h>

/**
 * @author cristian.chilipirea
 * Run: mpirun -np 4 ./a.out
 */

int graph[][2] = { { 0, 1 },
                  { 1, 0 }, { 1, 2 }, { 1, 3 },
                  { 2, 1 }, { 2, 3 },
                  { 3, 1 }, { 3, 2 } };


int main(int argc, char * argv[]) {
	int rank;
	int nProcesses;
    int parent[4];

	MPI_Init(&argc, &argv);
	MPI_Status status;
	MPI_Request request;


	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &nProcesses);
	printf("Hello from %i/%i\n", rank, nProcesses);

    for (int i = 0; i < 4; i++) {
        parent[i] = -1;
    }

    if (rank == 0) {
        for (int i = 0; i < 8; i++) {
            int ans;
            if (graph[i][0] == rank) {
                int new_parent[4];
                MPI_Send(&parent, 4, MPI_INT, graph[i][1], 0, MPI_COMM_WORLD);
                MPI_Recv(&new_parent, 4, MPI_INT, graph[i][1], 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                for (int j = 0; j < 4; j++) {
                    if (parent[j] == -1) {
                        parent[j] = new_parent[j];
                    }
                }
            }
        }
    } else {
        int new_parent[4];
        MPI_Status status;
        MPI_Recv(&parent, 4, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
        parent[rank] = status.MPI_SOURCE;
        int nodes = 0;

        for (int i = 0; i < 8; i++) {
            if (graph[i][0] == rank && graph[i][1] != parent[rank]) {
                MPI_Send(&parent, 4, MPI_INT, graph[i][1], 0, MPI_COMM_WORLD);
            }
        }

        for (int i = 0; i < 8; i++) {
            if (graph[i][0] == rank && graph[i][1] != parent[rank]) {
                MPI_Recv(&new_parent, 4, MPI_INT, graph[i][1], 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                for (int j = 0; j < 4; j++) {
                    if (parent[j] == -1) {
                        parent[j] = new_parent[j];
                    }
                }
            }
        }

        MPI_Send(&parent, 4, MPI_INT, parent[rank], 0, MPI_COMM_WORLD);
    }

    printf("from %d: ", rank);
    for (int i = 0; i < 4; i++) {
        printf("%d ", parent[i]);
    }
    printf("\n");

    int path[4];
    int my_rank = 3;
    int n = 0;
    int ans;
    path[n++] = 3;
    do {
        my_rank = parent[my_rank];
        path[n++] = my_rank;
    } while (my_rank != 0);

    for (int i = 0; i < n; i++) {
        if (i == 0) {
            if (path[i] == rank) {
                MPI_Send(&rank, 1, MPI_INT, path[i+1], 0, MPI_COMM_WORLD);
            }
        } else if (path[i] == rank) {
            MPI_Recv(&ans, 1, MPI_INT, path[i-1], 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Send(&rank, 1, MPI_INT, path[i+1], 0, MPI_COMM_WORLD);
            printf("Hello from %d\n", rank);
        }
    }
    printf("\n");
	printf("Bye from %i/%i\n", rank, nProcesses);
	MPI_Finalize();
	return 0;
}
