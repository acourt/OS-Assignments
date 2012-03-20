#include "MyThreads.h"

/*
typedef struct _mythread_control_block {
    ucontext_t context;
    char thread_name[THREAD_NAME_LEN];
    int thread_id;
} mythread_control_block;*/


ucontext_t signal_context;		  /* the interrupt context */
void *signal_stack;				 /* global interrupt stack */

mythread_control_block *cur_context_ctrl_block;

int num_threads = 0;
int quantum_size = 1; // Quantum size in nanoseconds

List* threads;

List* semaphores;



/*
   Timer interrupt handler.
   Creates a new context to run the scheduler in, masks signals, then swaps
   contexts saving the previously executing thread and jumping to the
   scheduler.
   */
void timer_interrupt(int j, siginfo_t *si, void *old_context)
{
    /* Create new scheduler context */
    getcontext(&signal_context);
    signal_context.uc_stack.ss_sp = signal_stack;
    signal_context.uc_stack.ss_size = INT_STACKSIZE;
    signal_context.uc_stack.ss_flags = 0;
    sigemptyset(&signal_context.uc_sigmask);
    makecontext(&signal_context, scheduler, 0);
	
    /* save running thread, jump to scheduler */
    swapcontext(&(cur_context_ctrl_block->context),&signal_context);
}


/* Set up SIGALRM signal handler */
    void
setup_signals(void)
{
	struct sigaction act;
	struct itimerval it;
	
	act.sa_sigaction = timer_interrupt;
	sigemptyset(&act.sa_mask);
	act.sa_flags = SA_RESTART | SA_SIGINFO;
	
	if(sigaction(SIGALRM, &act, NULL) != 0) {
		perror("Signal handler");
	}
	
	/* setup our timer */
	it.it_interval.tv_sec = 0;
	it.it_interval.tv_usec = quantum_size * 1000;
	it.it_value = it.it_interval;
	if (setitimer(ITIMER_REAL, &it, NULL) ) perror("setitiimer");
}



int init_my_threads()
{
	// Create the run queue
	threads = list_create(NULL);
	
	// Create the semaphores
	
	// Set up the prememption process
	struct itimerval tval;
	setup_signals();
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
		if (sp == NULL) {
			perror("Error Allocating Memory for thread");
			exit(1);
		}
		getcontext(&(control_block->context));
		control_block->context.uc_stack.ss_sp = sp;
		control_block->context.uc_stack.ss_size = stacksize;
		control_block->context.uc_stack.ss_flags = 0;
		sigemptyset(&(control_block->context.uc_sigmask));
		makecontext(&(control_block->context), threadfunc, 0);
		
		// Add to the list of running threads
		list_append(threads, control_block);
		
	}
	
	return rvalue;
}

int create_semaphore(int value)
{
	Semaphore* sema = malloc(sizeof(Semaphore));
	sema->value = value;
	sema->wait_queue = list_create(NULL);
	return sema;
}

void semaphore_wait(int semaphore)
{
	// Mask all interrupts
}

void runthreads()
{

}

void scheduler()
{
	printf("scheduling out thread %d\n", cur_context_ctrl_block->thread_id);
	
	// Scheduling algorithm
	
	printf("scheduling in thread %d\n", cur_context_ctrl_block->thread_id );

	setcontext(&(cur_context_ctrl_block->context)); /* go */
}




