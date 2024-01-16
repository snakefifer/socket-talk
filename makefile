CFLAGS=-g -c -Wall -Wextra -Wwrite-strings
LFLAGS=-g

server: server.o syscalls.o serverFuncs.o
	gcc ${LFLAGS} -o server server.o syscalls.o serverFuncs.o
server.o: server.c
	gcc ${CFLAGS} server.c
syscalls.o: syscalls.c
	gcc ${CFLAGS} syscalls.c
serverFuncs.o: serverFuncs.c
	gcc ${CFLAGS} serverFuncs.c
clean:
	rm -f *.o
	rm -f server