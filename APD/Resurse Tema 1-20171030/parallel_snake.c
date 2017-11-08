#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int index_in_bounds(int x, int dx, int limit) {
	return x + dx >= limit ? x + dx - limit :
	x + dx < 0 ? x + dx + limit : x + dx;
}

/* Decode int to direction:
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
	struct snake *snakes, int step_count, char *file_name) {

	int step, k;
	int num_threads = omp_get_max_threads();

	for (step = 0; step < step_count; step++) {

		#pragma omp parallel num_threads(num_threads)
		{
			/******************************************************
			Check collisions for each snake
			******************************************************/
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
					} else {
						world[y][x] = -1;
					}
				}
			}

			/******************************************************
			A collision was detected at step k+1 so i shoul stop
			the algorithm here to save matrix from step k
			******************************************************/
			if (num_snakes < 0) {
				step_count = -1;
			}

			/******************************************************
			Update & move snakes acording to direction
			******************************************************/
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

	/******************************************************
	Clean up the matrix (if a collision was detected, then
	the next position of each snake will be in matrix as -1)
	******************************************************/

	int i, j;
	for (i = 0; i < num_lines; i++) {
		for (j = 0; j < num_lines; j++) {
			if (world[i][j] == -1) {
				world[i][j] = 0;
			}
		}
	}
}
