thr: thread
	chmod +x thread
	./thread

thr_arg: thread_arg
	chmod +x thread_arg
	./thread_arg

thread: thread.c
	gcc -pthread -o thread thread.c

thread_arg: thread_arg.c
	gcc -pthread -o thread_arg thread_arg.c

clean:
	rm -r thread thread_arg

