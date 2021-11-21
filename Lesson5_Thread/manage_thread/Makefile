all: pthread_join
	chmod +x pthread_join
	./pthread_join

pthread_join: pthread_join.c
	gcc -pthread -o pthread_join pthread_join.c

clean:
	rm -r pthread_join
