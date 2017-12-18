#include<mpi.h>
#include<stdio.h>

/**
 * @author cristian.chilipirea
 * Run: mpirun -np 10 ./a.out
 */

int graph[][2] = { { 0, 1 }, { 0, 4 }, { 0, 5 },
                  { 1, 0 }, { 1, 2 }, { 1, 6 },
                  { 2, 1 }, { 2, 3 }, { 2, 7 },
                  { 3, 2 }, { 3, 4 }, { 3, 8 },
                  { 4, 0 }, { 4, 3 }, { 4, 9 },
                  { 5, 0 }, { 5, 7 }, { 5, 8 },
                  { 6, 1 }, { 6, 8 }, { 6, 9 },
                  { 7, 2 }, { 7, 5 }, { 7, 9 },
                  { 8, 3 }, { 8, 5 }, { 8, 6 },
                  { 9, 4 }, { 9, 6 }, { 9, 7 } };


int main(int argc, char * argv[]) {
	int rank;
	int nProcesses;

	MPI_Init(&argc, &argv);
	MPI_Status status;
	MPI_Request request;


	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &nProcesses);
	printf("Hello from %i/%i\n", rank, nProcesses);

    int leader = rank;
    int recvLeader;

    for (int i = 0; i < 10; i++) {
        MPI_Send(&leader, 1, MPI_INT, graph[rank * 3 + 0][1], 0, MPI_COMM_WORLD);
        MPI_Send(&leader, 1, MPI_INT, graph[rank * 3 + 1][1], 0, MPI_COMM_WORLD);
        MPI_Send(&leader, 1, MPI_INT, graph[rank * 3 + 2][1], 0, MPI_COMM_WORLD);

        MPI_Recv(&recvLeader, 1, MPI_INT, graph[rank * 3 + 0][1], 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        if (recvLeader > leader) {
            leader = recvLeader;
        }
        MPI_Recv(&recvLeader, 1, MPI_INT, graph[rank * 3 + 1][1], 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        if (recvLeader > leader) {
            leader = recvLeader;
        }
        MPI_Recv(&recvLeader, 1, MPI_INT, graph[rank * 3 + 2][1], 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        if (recvLeader > leader) {
            leader = recvLeader;
        }
    }

    float num = 0, recvnum;
    if (rank == leader) {
        num = 1;
    }

    for (int i = 0; i < 100; i++) {
        for (int j = 0 ; j < 3; j++) {
            MPI_Send(&num, 1, MPI_INT, graph[rank * 3 + j][1], 0, MPI_COMM_WORLD);
            MPI_Recv(&recvnum, 1, MPI_INT, graph[rank * 3 + j][1], 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            num = (num + recvnum)/2;
        }
    }

    printf("total: %f\n", 1/num);
	// printf("Bye from %i/%i\n", rank, nProcesses);
	MPI_Finalize();
	return 0;
}
