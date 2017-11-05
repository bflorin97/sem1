
input=teste/big_input
output_test=teste/big_output
output=output
runde=20000

run: build
	./parallel_snake $(input) $(output) $(runde)

build:
	clear
	gcc parallel_snake.c main.c -o parallel_snake -fopenmp -lm

diff: run
	diff $(output) $(output_test) -q

clean:
	rm -f parallel_snake

.PHONY: clean
