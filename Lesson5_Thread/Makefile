all: matrix_dot
	chmod +x matrix_dot
	./matrix_dot

matrix_dot: matrix_dot.c
	gcc -pthread -o matrix_dot matrix_dot.c

clean:
	rm -r matrix_dot

