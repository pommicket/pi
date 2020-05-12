CFLAGS=-O3 -lgmp -lm -Wall -Wextra -Wpedantic -pedantic -std=c89
pi: main.c
	$(CC) $(CFLAGS) main.c -o pi 
