exec=lab

default: run

run: build
	./$(exec)

build:
	clear
	gcc -fopenmp -o $(exec) $(file)
