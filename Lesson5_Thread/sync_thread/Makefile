mutex: thr_mutex
	chmod +x thr_mutex
	./thr_mutex

cond_var: thr_cond_var
	chmod +x thr_cond_var
	./thr_cond_var

thr_mutex: thr_mutex.c
	gcc -pthread -o thr_mutex thr_mutex.c

thr_cond_var: thr_cond_var.c
	gcc -pthread -o thr_cond_var thr_cond_var.c

clean_mutex:
	rm -r thr_mutex

clean_cv:
	rm -r thr_cond_var
