.PHONY: clean

input=teste/input2
output=output
runde=400

run: build
	./parallel_snake $(input) $(output) $(runde)


build:
	clear
	gcc parallel_snake.c main.c -o parallel_snake -fopenmp -lm


clean:
	rm -f parallel_snake
