all: main.o
	chmod +x main
	./main

main.o: main.c freq.o
	gcc -c main.c
	gcc main.o -omain libfreq.a

libfreq.a: freq.o
	ar cvr libfreq.a freq.o

freq.o: freq.c
	gcc -c freq.c
