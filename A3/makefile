CC=gcc

all:
	$(CC) -DHAVE_PTHREAD_RWLOCK=1 main.c MyThreads.c -lslack -o main.o
	
swap:
	$(CC) -DHAVE_PTHREAD_RWLOCK=1 swap.c -o swap.o
