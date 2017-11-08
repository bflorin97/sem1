#include "main.h"
#include <stdio.h>
#include <omp.h>

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

	int step, k;
	int gameOver = 0;

	// omp_set_num_threads(4);

	for (step = 0; step < step_count && !gameOver; step++) {

		/* --- check for collisions ---
			Pot aparea coliziuni intre threaduri deoarece doua thread-uri pot
			sa modifice si verifice simultan in acelasi loc din matrice.
		*/


		#pragma omp parallel num_threads(4)
		{
			int v = num_snakes;
			#pragma omp for
			for (k = 0; k < num_snakes; k++) {
				int snakeEnd = 0;
				char lastDirection = ' ';
				char direction = snakes[k].direction;
				int i = snakes[k].head.line;
				int j = snakes[k].head.col;
				int di = direction == 'N' ? -1 : direction == 'S' ? 1 : 0;
				int dj = direction == 'V' ? -1 : direction == 'E' ? 1 : 0;
				int y = index_in_bounds(i, di, num_lines);
				int x = index_in_bounds(j, dj, num_cols);

				#pragma omp critical
				{
					if (world[y][x] != 0) {
						num_snakes = -1;
						printf("snake[%d] from [%d,%d] says that collide with [%d,%d] at [%d]\n",
							k, i, j, y, x, world[y][x]);
					} else {
						world[y][x] = -1;
					}
				}
			}

			#pragma omp for
			for (int k = 0; k < num_snakes; k++) {

				int snakeEnd = 0;
				char lastDirection = ' ';
				char direction = snakes[k].direction;
				int i = snakes[k].head.line;
				int j = snakes[k].head.col;
				int di = direction == 'N' ? -1 : direction == 'S' ? 1 : 0;
				int dj = direction == 'V' ? -1 : direction == 'E' ? 1 : 0;
				int y = index_in_bounds(i, di, num_lines);
				int x = index_in_bounds(j, dj, num_cols);

				snakes[k].head.line = y;
				snakes[k].head.col = x;

				world[i][j] = 0;
				world[y][x] = snakes[k].encoding;
				lastDirection = direction;

				while (!snakeEnd) {

					int d;
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
	}

	int ii, jj;
	for (ii = 0; ii < num_lines; ii++) {
		for (jj = 0; jj < num_lines; jj++) {
			if (world[ii][jj] == -1) {
				world[ii][jj] = 0;
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
