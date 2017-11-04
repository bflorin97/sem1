#include "main.h"
#include <stdio.h>

int index_in_bounds(int x, int dx, int limit) {
	return x + dx >= limit ? x + dx - limit :
	x + dx < 0 ? x + dx + limit : x + dx;
}

/* decodifica un intreg intr-o directie :
0 - 'N'
1 - 'S'
2 - 'V'
3 - 'E'
*/
char itoc(int d) {
	return d == 0 ? 'N' :
	d == 1 ? 'S' :
	d == 2 ? 'V' :
	d == 3 ? 'E' : ' ';
}

void run_simulation(int num_lines, int num_cols, int **world, int num_snakes,
	struct snake *snakes, int step_count, char *file_name)
{
	// TODO: Implement Parallel Snake simulation using the default (env. OMP_NUM_THREADS)
	// number of threads.
	//
	// DO NOT include any I/O stuff here, but make sure that world and snakes
	// parameters are updated as required for the final state.

	int step, k;
	int gameOver = 0;
	int snakeEnd = 0;
	char lastDirection = ' ';
	char direction;
	int i, j;

	int y, x;
	int di, dj;
	int d;

	// omp_set_num_threads(4);

	for (step = 0; step < step_count && !gameOver; step++) {

		/* --- check for collisions ---
			Pot aparea coliziuni intre threaduri deoarece doua thread-uri pot
			sa modifice si verifice simultan in acelasi loc din matrice.
		*/

		int v = num_snakes;
		// #pragma omp parallel for num_threads(4)
		for (k = 0; k < v; k++) {
			snakeEnd = 0;
			lastDirection = ' ';
			direction = snakes[k].direction;

			i = snakes[k].head.line;
			j = snakes[k].head.col;

			direction = snakes[k].direction;
			i = snakes[k].head.line;
			j = snakes[k].head.col;

			di = direction == 'N' ? -1 : direction == 'S' ? 1 : 0;
			dj = direction == 'V' ? -1 : direction == 'E' ? 1 : 0;

			y = index_in_bounds(i, di, num_lines);
			x = index_in_bounds(j, dj, num_cols);

			// #pragma omp critical
			// {
				if (world[y][x] != 0) {
					gameOver = 1;
					// break;
					v = 0;
				} else {
					world[y][x] = -1;
				}
			// }
		}

		// for (int ii = 0; ii < num_lines; ii++) {
		// 	for (int jj = 0; jj < num_cols; jj++) {
		// 		printf("%2d ", world[ii][jj]);
		// 	}
		// 	printf("\n");
		// }
		//
		// printf("gameOver:[%d]; step:[%d]\n", gameOver, step);
		// scanf("%c", &direction);

		/*  --- update snakes ---
		 	Este garantat ca nu exista coliziune cu alt sarpe sau cu el
			insusi => mai multe thread-uri nu vor incerca sa acceseze
			aceleasi resurse.
		*/
		if (gameOver) {
			continue;
		}

		for (int k = 0; k < num_snakes; k++) {
			snakeEnd = 0;
			lastDirection = ' ';
			direction = snakes[k].direction;

			i = snakes[k].head.line;
			j = snakes[k].head.col;

			direction = snakes[k].direction;
			i = snakes[k].head.line;
			j = snakes[k].head.col;

			di = direction == 'N' ? -1 : direction == 'S' ? 1 : 0;
			dj = direction == 'V' ? -1 : direction == 'E' ? 1 : 0;

			y = index_in_bounds(i, di, num_lines);
			x = index_in_bounds(j, dj, num_cols);

			snakes[k].head.line = y;
			snakes[k].head.col = x;

			world[i][j] = 0;
			world[y][x] = snakes[k].encoding;
			lastDirection = direction;

			while (!snakeEnd) {

				di = dj = -1;
				for (d = 0; d < 4; d++) {
					direction = itoc(d);

					if (direction == lastDirection) {
						continue;
					}

					di = direction == 'N' ? -1 : direction == 'S' ? 1 : 0;
					dj = direction == 'V' ? -1 : direction == 'E' ? 1 : 0;

					y = index_in_bounds(i, di, num_lines);
					x = index_in_bounds(j, dj, num_cols);

					if (world[y][x] == snakes[k].encoding) {
						break;
					}
				}

				snakeEnd = d == 4;

				if (snakeEnd) {
					world[i][j] = 0;
					break;
				}

				world[i][j] = snakes[k].encoding;
				world[y][x] = 0;

				i = index_in_bounds(i, di, num_lines);
				j = index_in_bounds(j, dj, num_cols);

				lastDirection = direction == 'N' ? 'S' :
					direction == 'S' ? 'N' :
					direction == 'E' ? 'V' :
					direction == 'V' ? 'E' : ' ';
			}
		}
	}

	for (i = 0; i < num_lines; i++) {
		for (j = 0; j < num_lines; j++) {
			if (world[i][j] == -1) {
				world[i][j] = 0;
			}
		}
	}
	// for (int ii = 0; ii < num_lines; ii++) {
	// 	for (int jj = 0; jj < num_cols; jj++) {
	// 		printf("%d ", world[ii][jj]);
	// 	}
	// 	printf("\n");
	// }
	//
	// printf("gameOver:[%d]; step:[%d]\n", gameOver, step);
	printf("steps:[%d]\n", step);
}
