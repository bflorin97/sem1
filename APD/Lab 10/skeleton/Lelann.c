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

    int list[10], n = -1;
    int lastRank, nextRank, recvRank;
    int leader;

    // list[++n] = rank;
    //
    // if (rank == 0) {
    //     MPI_Send(&lastRank, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    // }

    for (int i = 0; i < nProcesses; i++) {
        int value;
        lastRank = rank - 1 < 0 ? nProcesses - 1 : rank - 1;
        nextRank = rank + 1 == nProcesses ? 0 : rank + 1;

        if (i == 0) {
            list[++n] = rank;
            MPI_Send(&rank, 1, MPI_INT, nextRank, 0, MPI_COMM_WORLD);
        } else {
            MPI_Recv(&value, 1, MPI_INT, lastRank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            list[++n] = value;
            MPI_Send(&value, 1, MPI_INT, nextRank, 0, MPI_COMM_WORLD);
        }

        // printf("recv %d from %d\n", recvRank, lastRank);
        // printf("send %d to %d\n", rank, nextRank);
    }

    leader = rank;
    for (int i = 0; i <= n; i++) {
        if (leader < list[i]) {
            leader = list[i];
        }
    }

    printf("rank: %d are leader-ul: %d\n", rank, leader);

	MPI_Finalize();
	return 0;
}
