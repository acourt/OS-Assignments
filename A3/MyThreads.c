#include "MyThreads.h"


ucontext_t main_context;		  /* the main context  */
void *main_stack;				 /* main stack */

ucontext_t scheduler_context;	/* the scheduler context */
void *scheduler_stack;			/* main stack */

mythread_control_block *cur_context_ctrl_block;

int num_threads = 0;
int num_running_threads = 0;
int quantum_size = 1000; // Quantum size in microseconds


Semaphore* semaphore_table[255];
int next_semaphore_index = 0;

List* threads;
List* run_queue;
List* semaphores;


void setup_signals(void)
{
	struct itimerval it;
	sigset(SIGALRM, scheduler);
	
	/* setup our timer */
	it.it_interval.tv_sec = 0;
	it.it_interval.tv_usec = quantum_size * 1000;
	it.it_value.tv_sec = 0;
	it.it_value.tv_usec = quantum_size *1000;
	if (setitimer(ITIMER_REAL, &it, NULL) ) perror("setitiimer");
	
}



int init_my_threads()
{
	// Create the run queue
	threads = list_create(NULL);
	run_queue = list_create(NULL);

	getcontext(&scheduler_context);
	scheduler_stack = malloc(INT_STACKSIZE);
	scheduler_context.uc_stack.ss_sp = scheduler_stack;
	scheduler_context.uc_stack.ss_size = INT_STACKSIZE;
	scheduler_context.uc_stack.ss_flags = 0;
	sigemptyset(&scheduler_context.uc_sigmask);
	makecontext(&scheduler_context, scheduler, 0);
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
		control_block->context.uc_link = &main_context;
		makecontext(&(control_block->context), threadfunc, 0);
		sigemptyset(&(control_block->context.uc_sigmask));
		
		num_running_threads++;
		
		// Add to the list of running threads
		list_append(threads, control_block);
		
	}
	
	return rvalue;
}


void runthreads()
{
	printf("Running threads\n");
	
	// Set up the signal and start the scheduler
	setup_signals();
	printf("Swap\n");
	// At the end of the program, the scheduler will come back to this point
	swapcontext(&main_context, &scheduler_context);
	
	// Set the returning thread's state to EXIT
	sighold(SIGALRM);
	cur_context_ctrl_block->state = EXIT;
	sigrelse(SIGALRM);
	
	while(num_running_threads != 0)
	{
		// waste cycles and wait until the signal handler makes it go back into the scheduler
	}
	
	// Disable signals since we are at the end of the running of the threads
	sigemptyset(&main_context.uc_sigmask);
	return;	
}

void scheduler()
{
	printf("scheduling out thread %d\n", cur_context_ctrl_block->thread_id);
	
	// If at the end of the run it is still runnable, but it back in the run queue
	if(cur_context_ctrl_block->state == RUNNABLE || RUNNING)
	{
		cur_context_ctrl_block->state = RUNNABLE;
		list_append(run_queue, cur_context_ctrl_block);
	}
	else if(cur_context_ctrl_block->state == BLOCKED)
	{
		// if it's blocked do nothing
	}
	
	if(num_running_threads == 0)
	{
		// Deactivate timer and return to main.
	}
	
	// Take the next context in line
	cur_context_ctrl_block = list_shift(run_queue);
	cur_context_ctrl_block->state = RUNNING;
	
	printf("scheduling in thread %d\n", cur_context_ctrl_block->thread_id);
	
	setcontext(&(cur_context_ctrl_block->context)); /* go */
}

void set_quantum_size(int quantum)
{
	quantum_size = quantum;
}

void exit_my_thread()
{
	cur_context_ctrl_block->state = EXIT;
	num_running_threads--;
	setcontext(&(cur_context_ctrl_block->context));
}

void my_threads_state()
{
	mythread_control_block* thread;
	int i;
	for(i=0; i < (list_length(threads)); i++)
	{
		thread = list_item(threads, i);
		printf("Thread %d is state %d\n", thread->thread_id, thread->state);
	}
}


int create_semaphore(int value)
{
	Semaphore* sema = malloc(sizeof(Semaphore));
	sema->value = value;
	sema->wait_queue = list_create(NULL);
	
	semaphore_table[next_semaphore_index++] = sema;
	
	return (next_semaphore_index -1);
}

void semaphore_wait(int semaphore)
{
	if(semaphore_table[semaphore] == NULL)
	{
		printf("Error: Tried to wait on a semaphore that was not initialized");
		exit(-1);
	}
	
	// Retrieve the indexed semaphore
	Semaphore * sema = semaphore_table[semaphore];
	
	// Mask signal interrupts
	sighold(SIGALRM);
	
	// P
	sema->value--;
	
	if(sema->value < 0)
	{
		// Wait in the semaphore's wait queue queue
		list_append(sema->wait_queue, cur_context_ctrl_block);
		cur_context_ctrl_block->state = BLOCKED;
	}
	
	// Unmask signal interrupts
	sigrelse(SIGALRM);
	
	// Wait until the thread becomes unblocked
	while(cur_context_ctrl_block->state != RUNNABLE)
	{
		// Switch to the scheduler program
	} 
}


void semaphore_signal(int semaphore)
{
	if(semaphore_table[semaphore] == NULL)
	{
		printf("Error: Tried to wait on a semaphore that was not initialized");
		exit(-1);
	}
	
	// Retrieve the indexed semaphore
	Semaphore * sema = semaphore_table[semaphore];
	
	// Mask signal interrupts
	sighold(SIGALRM);
	
	mythread_control_block* next_in_line = list_shift(sema->wait_queue);
	
	// If there are no more threads waiting on this semaphores
	if(next_in_line == NULL)
	{
		sema->value++;
	}
	else // Unqueue the next thread and put it in the run queue
	{
		next_in_line->state = RUNNABLE;
		list_append(run_queue, cur_context_ctrl_block);
	}
	
	// UnMask signal interrupts
	sigrelse(SIGALRM);
	
}

void destroy_semaphore(int semaphore)
{
	
}

