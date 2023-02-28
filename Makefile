all: prodcons.o main.c
	gcc -Wall -g -o prodcons main.c prodcons.o -lpthread

prodcons.o: prodcons.h prodcons.c
	gcc -Wall -g -c prodcons.c

clean:
	rm -rf *.o prodcons