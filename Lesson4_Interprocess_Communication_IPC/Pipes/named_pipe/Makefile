run_sv: fifo_server.o
	chmod +x fifo_server
	./fifo_server

run_cli: fifo_client.o
	chmod +x fifo_client
	./fifo_client

fifo_server.o: fifo_server.c
	gcc -c fifo_server.c
	gcc fifo_server.o -o fifo_server

fifo_client.o: fifo_client.c
	gcc -c fifo_client.c
	gcc fifo_client.o -o fifo_client

clean_sv:
	rm -r fifo_server.o fifo_server

clean_cli:
	rm -r fifo_client.o fifo_client
