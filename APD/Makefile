exec=lab
FLAGS=-fopenmp -lm

default: run

run: build
	./$(exec)

build:
	clear
	gcc -o $(exec) $(file) $(FLAGS)
