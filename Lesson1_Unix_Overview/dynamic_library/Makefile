all: main
	chmod +x main
	./main
	sudo rm -r /usr/lib/libfreq.so

main: libfreq.so
	sudo cp libfreq.so /usr/lib
	gcc main.c -o main -L. -lfreq

libfreq.so: freq.o
	gcc freq.o -shared -o libfreq.so

freq.o:
	gcc -c freq.c -fpic

clean:
	rm -r freq.o libfreq.so main
