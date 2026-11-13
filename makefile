CC = gcc
CFLAGS = -Wall -Wextra

all: central

central: central.c estrutura.h
	$(CC) $(CFLAGS) central.c -o central

clean:
	rm -f central *.o

.PHONY: all clean
