CC = gcc
CFLAGS = -Wall -Wextra

all: central estafeta gerador

central: central.c estrutura.h
	$(CC) $(CFLAGS) central.c -o central

estafeta: estafeta.c
	$(CC) $(CFLAGS) estafeta.c -o estafeta

gerador: gerador.c
	$(CC) $(CFLAGS) gerador.c -o gerador

clean:
	rm -f central estafeta gerador *.o

.PHONY: all clean