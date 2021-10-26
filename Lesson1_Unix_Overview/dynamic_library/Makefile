all: main.o
	LD_LIBRARY_PATH=.:
	export LD_LIBRARY_PATH
	ldd main
	chmod +x main
	./main

main.o: main.c freq.o libfreq.so
	gcc main.c -o main -L. -lfreq

libfreq.so: freq.o
	gcc -shared freq.o -o libfreq.so

freq.o: freq.c freq.h
	gcc -c -fpic freq.c
