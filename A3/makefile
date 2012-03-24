CC=gcc

all:
	$(CC) -g -DHAVE_PTHREAD_RWLOCK=1 main.c MyThreads.c -lm -lslack -o main.o

swap:
	$(CC) -DHAVE_PTHREAD_RWLOCK=1 swap.c -o swap.o

test:
	$(CC) -g -DHAVE_PTHREAD_RWLOCK=1 test.c -lm -lslack -o test.o
	
