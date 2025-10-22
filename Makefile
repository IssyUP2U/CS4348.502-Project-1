CC = gcc
CFLAGS = -std=gnu11 -Wall -Wextra -O2

all: driver logger encrypt

driver: driver.c
	$(CC) $(CFLAGS) -o driver driver.c

logger: logger.c
	$(CC) $(CFLAGS) -o logger logger.c

encrypt: encrypt.c
	$(CC) $(CFLAGS) -o encrypt encrypt.c

clean:
	rm -f driver logger encrypt *.o
