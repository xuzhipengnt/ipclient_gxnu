cc := gcc

main: main.o utils.o md5.o
	cc -o ipclient main.o utils.o md5.o
	chmod +x ipclient

main.o: main.o
	cc -c main.c

utils.o: utils.c utils.h
	cc -c utils.c

md5.o: md5.c md5.h
	cc -c md5.c

clean:
	rm -rf *.o
	rm ipclient

