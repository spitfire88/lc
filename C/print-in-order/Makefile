build: main.o
	gcc -o main main.o -DREENTRANT -lpthread

debug: debug.o
	gcc -o main debug.o -DREENTRANT -lpthread

debug.o: main.c
	gcc -c main.c -o debug.o -g

main.o: main.c
	gcc -c main.c

clean:
	rm main *.o

run:
	./main

edit:
	vim main.c
