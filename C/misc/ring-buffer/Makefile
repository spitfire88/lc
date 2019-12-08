build: main.o
	gcc -o main main.o -DREENTRANT -lpthread
main.o: main.c
	gcc -c main.c
debug: debug.o
	gcc -o main debug.o -DREENTRANT -lpthread
debug.o: main.c
	gcc -c main.c -g -o debug.o
run:
	./main
clean:
	rm -f *.o main
edit:
	vim main.c
