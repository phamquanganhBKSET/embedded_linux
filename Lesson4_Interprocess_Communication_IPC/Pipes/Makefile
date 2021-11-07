run_sp: simple_pipe.o
	chmod +x simple_pipe
	./simple_pipe

run_pwp: pipe_with_process.o
	chmod +x pipe_with_process
	./pipe_with_process

run_twp: two_ways_pipe.o
	chmod +x two_ways_pipe
	./two_ways_pipe

simple_pipe.o: simple_pipe.c
	gcc -c simple_pipe.c
	gcc simple_pipe.o -o simple_pipe

pipe_with_process.o: pipe_with_process.c
	gcc -c pipe_with_process.c
	gcc pipe_with_process.o -o pipe_with_process

two_ways_pipe.o: two_ways_pipe.c
	gcc -c two_ways_pipe.c
	gcc two_ways_pipe.o -o two_ways_pipe

clean_sp:
	rm -r simple_pipe.o simple_pipe

clean_pwp:
	rm -r pipe_with_process.o pipe_with_process

clean_twp:
	rm -r two_ways_pipe.o two_ways_pipe
