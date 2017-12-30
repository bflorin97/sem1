#

build: main.o
	lex tema1.l
	gcc lex.yy.c main.o -o tema1

run: build
	./tema1 < example.h

main.o: main.c main.h
	gcc -Wall -c main.c main.h

clean:
	rm tema1 lex.yy.c main.o
