run_wr: shm_write.o
	chmod +x shm_write
	./shm_write

run_rd: shm_read.o
	chmod +x shm_read
	./shm_read

shm_write.o: shm_write.c
	gcc -c shm_write.c
	gcc shm_write.o -o shm_write

shm_read.o: shm_read.c
	gcc -c shm_read.c
	gcc shm_read.o -o shm_read

clean_wr:
	rm -r shm_write.o shm_write

clean_rd:
	rm -r shm_read.o shm_read

