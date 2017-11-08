
input=big_input
output=output
runde=20000

run: build
	./parallel_snake $(input) $(output) $(runde)

build:
	clear
	gcc parallel_snake.c main.c -o parallel_snake -fopenmp -lm

# gcc test.c -o test -fopenmp -lm
# ./test

clean:
	rm -f parallel_snake

.PHONY: clean
