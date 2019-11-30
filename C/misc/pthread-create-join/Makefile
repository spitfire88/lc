build: main.o
	gcc -o main main.o -DREENTRANT -lpthread
main.o: main.c
	gcc -c main.c
run:
	./main
clean:
	rm main.o main
