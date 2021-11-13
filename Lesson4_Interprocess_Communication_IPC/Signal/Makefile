all: signal.o
	chmod +x signal
	./signal

signal.o: signal.c
	gcc -c signal.c
	gcc signal.o -o signal

clean:
	rm -r signal.o signal
