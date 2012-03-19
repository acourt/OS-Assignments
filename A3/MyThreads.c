#include "MyThreads.h"

/*
typedef struct _mythread_control_block {
    ucontext_t context;
    char thread_name[THREAD_NAME_LEN];
    int thread_id;
} mythread_control_block;*/

int num_threads = 0;

List* thread_list;

int init_my_threads()
{
	// Create the run queue
	threadList = list_create(NULL);
	
	// Set up the prememption process
	struct itimerval tval;
	
	sigset(SIGALRM, scheduler);
	
}

int create_my_thread(char *threadname, void (*threadfunc)(), int stacksize)
{
	int rvalue = 0;
	char* sp;
	// Allocate memory dedicated to the thread control block that is associated with this thread.
	mythread_control_block* control_block = malloc(sizeof(mythread_control_block));
	if(strlen(threadname) > THREAD_NAME_LEN)
		rvalue = -1;
	else {
		strcpy (control_block->thread_name, threadname);
		control_block->thread_id = num_threads++;
		
		// Set up the thread context
		sp = malloc(stacksize);
		control_block->context.ss_sp = sp;
		control_block->context.ss_size = stacksize;
		make_context(&(control_block->context), threadfunc, 0);
		//control_block->ucontext_t.
		
		// Add to the list of running threads
		list_append(thread_list, mythread_control_block);
		
	}
	
	return rvalue;
}

void runthreads()
{
	while(1)
	{
		
	}
}

// list_shift();
























