run_my_ls: my_ls.o
	chmod +x my_ls
	./my_ls

run_cp: implement_cp.o
	chmod +x implement_cp
	./implement_cp

run_pp: parent_process.o
	chmod +x parent_process
	./parent_process

run_zp: zombie_process.o
	chmod +x zombie_process
	./zombie_process

run_mp: manage_process.o
	chmod +x manage_process
	./manage_process

manage_process.o: manage_process.c
	gcc -c manage_process.c
	gcc manage_process.o -o manage_process

parent_process.o: parent_process.c
	gcc -c parent_process.c
	gcc parent_process.o -o parent_process

zombie_process.o: zombie_process.c
	gcc -c zombie_process.c
	gcc zombie_process.o -o zombie_process

my_ls.o: my_ls.c
	gcc -c my_ls.c
	gcc my_ls.o -o my_ls

implement_cp.o: implement_cp.c
	gcc -c implement_cp.c
	gcc implement_cp.o -o implement_cp

clean_ls:
	rm -r my_ls.o my_ls

clean_cp:
	rm -r implement_cp.o implement_cp

clean_pp:
	rm -r parent_process.o parent_process

clean_zp:
	rm -r zombie_process.o zombie_process

clean_mp:
	rm -r manage_process.o manage_process
