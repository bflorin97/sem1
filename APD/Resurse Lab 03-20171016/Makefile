exec=lab

default: run

run: build
	./$(exec)

build:
	clear
	gcc -lm -fopenmp -o $(exec) $(file)
