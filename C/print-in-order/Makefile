build: main.o
	gcc -o main main.o -DREENTRANT -lpthread

main.o: main.c
	gcc -c main.c

clean:
	rm main main.o

run:
	./main
