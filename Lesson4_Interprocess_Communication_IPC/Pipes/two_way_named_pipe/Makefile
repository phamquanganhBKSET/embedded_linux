run_send: user_send.o
	chmod +x user_send
	./user_send

run_rc: user_receive.o
	chmod +x user_receive
	./user_receive

user_send.o: user_send.c
	gcc -c user_send.c
	gcc user_send.o -o user_send

user_receive.o: user_receive.c
	gcc -c user_receive.c
	gcc user_receive.o -o user_receive

clean_send:
	rm -r user_send.o user_send

clean_rc:
	rm -r user_receive.o user_recieve
