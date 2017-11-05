#include "main.h"
#include <stdio.h>
#include <stdlib.h>

int index_in_bounds(int x, int dx, int limit) {
	return x + dx >= limit ? x + dx - limit :
	x + dx < 0 ? x + dx + limit : x + dx;
}

void print_matrix(int** world, int num_lines, int num_cols) {
	for (int ii = 0; ii < num_lines; ii++) {
		for (int jj = 0; jj < num_cols; jj++) {
			printf("%2d ", world[ii][jj]);
		}
		printf("\n");
	}
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

	int step, k;
	int gameOver = 0;
	int snakeEnd = 0;
	char lastDirection = ' ';
	char direction;
	int i, j;
	int y, x;
	int di, dj;
	int d;

	struct snake* tails = (struct snake *)malloc(num_snakes * sizeof(struct snake));

	/******************************************************
	   I go throw every snake to find out the position
	   of it's tail and save it in tails
	******************************************************/
	for (int k = 0; k < num_snakes; k++) {
		snakeEnd = 0;
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
				tails[k].head.line = i;
				tails[k].head.col = j;
				break;
			}

			i = index_in_bounds(i, di, num_lines);
			j = index_in_bounds(j, dj, num_cols);

			lastDirection = direction == 'N' ? 'S' :
				direction == 'S' ? 'N' :
				direction == 'E' ? 'V' :
				direction == 'V' ? 'E' : ' ';
		}
	}


	/******************************************************
		Move snakes acording to direction
	******************************************************/
	for (step = 0; step < step_count; step++) {

		/******************************************************
			TODO
			as putea pur si simplu sa consider capul sarpelui
			cu un pas in fata, iar la faza de update sa mut
			efectiv sarpele la momentul urmator (pentru a
			scapa de '-1' care ramane in matrice)
		******************************************************/


		/******************************************************
		Check collisions for each snake
		******************************************************/
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
				break;
				v = 0;
			} else {
				world[y][x] = -1;
			}
			// }
		}


		/******************************************************
			A collision was detected at step k+1 and it stoped
			here to save matrix from step k
		******************************************************/
		if (gameOver) {
			break;
		}

		/******************************************************
			Update each snake, moving 1 step forward
		******************************************************/
		for (k = 0; k < num_snakes; k++) {

			i = snakes[k].head.line;
			j = snakes[k].head.col;
			direction = snakes[k].direction;

			di = direction == 'N' ? -1 : direction == 'S' ? 1 : 0;
			dj = direction == 'V' ? -1 : direction == 'E' ? 1 : 0;

			y = index_in_bounds(i, di, num_lines);
			x = index_in_bounds(j, dj, num_cols);

			world[y][x] = snakes[k].encoding;
			snakes[k].head.line = y;
			snakes[k].head.col = x;

			i = tails[k].head.line;
			j = tails[k].head.col;
			for (d = 0; d < 4; d++) {
				direction = itoc(d);

				di = direction == 'N' ? -1 : direction == 'S' ? 1 : 0;
				dj = direction == 'V' ? -1 : direction == 'E' ? 1 : 0;

				y = index_in_bounds(i, di, num_lines);
				x = index_in_bounds(j, dj, num_cols);

				if (world[y][x] == snakes[k].encoding) {
					break;
				}
			}
			world[i][j] = 0;
			tails[k].head.line = y;
			tails[k].head.col = x;
		}
	}

	/******************************************************
		Clean up '-1' from matrix
	******************************************************/
	for (i = 0; i < num_lines; i++) {
		for (j = 0; j < num_cols; j++) {
			if (world[i][j] == -1) {
				world[i][j] = 0;
			}
		}
	}
}
