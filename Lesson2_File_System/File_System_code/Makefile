readfile: readfile.o
	chmod +x readfile
	./readfile

readfolder: readfolder.o
	chmod +x readfolder
	./readfolder

readfile.o: readfile.c
	gcc -c readfile.c
	gcc readfile.o -o readfile

readfolder.o: readfolder.c
	gcc -c readfolder.c
	gcc readfolder.o -o readfolder

clean_readfile:
	rm -r readfile.o readfile

clean_readfolder:
	rm -r readfolder.o readfolder
