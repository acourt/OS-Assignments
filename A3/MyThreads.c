#include "MyThreads.h"


ucontext_t main_context;		  /* the main context  */
void *main_stack;				 /* main stack */

ucontext_t scheduler_context;	/* the scheduler context */
void *scheduler_stack;			/* main stack */

mythread_control_block *cur_context_ctrl_block;

int num_threads = 0;
int num_running_threads = 0;
int quantum_size = 100; // Quantum size in microseconds


Semaphore* semaphore_table[255];
int next_semaphore_index = 0;

// Thread control block table
mythread_control_block* thread_table[255];
int next_table_index = 0;
// The run queue
List* run_queue;


void setup_signals(void)
{
	struct itimerval it;
	sigset(SIGALRM, scheduler);
	
	// Setup the timer 
	it.it_interval.tv_sec = 0;
	it.it_interval.tv_usec = 100*quantum_size;
	it.it_value.tv_sec = 0;
	it.it_value.tv_usec = 100*quantum_size;
	if (setitimer(ITIMER_REAL, &it, NULL) ) perror("setitiimer");
}



int init_my_threads()
{
	// Create the run queue
	run_queue = list_create(NULL);
	
	// Clear the main's mask
	sigemptyset(&main_context.uc_sigmask);

	// Create the sceduler context
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
		control_block->state = RUNNABLE;
		control_block->context.uc_stack.ss_sp = sp;
		control_block->context.uc_stack.ss_size = stacksize;
		control_block->context.uc_stack.ss_flags = 0;
		control_block->context.uc_link = &main_context;
		makecontext(&(control_block->context), threadfunc, 0);
		sigemptyset(&(control_block->context.uc_sigmask));
		
		num_running_threads++;
		
		// Add to the list of running threads
		list_append(run_queue, control_block);
		thread_table[next_table_index++] = control_block;
	}
	
	return rvalue;
}


void runthreads()
{
	// Take the first context in line
	cur_context_ctrl_block = list_shift(run_queue);
	cur_context_ctrl_block->state = RUNNING;

	sigrelse(SIGALRM);

	swapcontext(&main_context, &(cur_context_ctrl_block->context));
	
	while(num_running_threads != 0)
	{
		setcontext(&scheduler_context);
	}
	
	// Disable signals since we are at the end of the running of the threads
	sigemptyset(&main_context.uc_sigmask);
	return;	
}

void scheduler()
{
	// If at the end of the run it is still runnable, but it back in the run queue
	//printf("Thread %d is back and is %s\n", cur_context_ctrl_block->thread_id,
	//							state_string[cur_context_ctrl_block->state]);
	
	if((cur_context_ctrl_block->state == RUNNABLE) || 
		(cur_context_ctrl_block->state == RUNNING))
	{
		cur_context_ctrl_block->state = RUNNABLE;
		list_append(run_queue, cur_context_ctrl_block);
	}
	
	// Assume the thread ran for its full quantum
	cur_context_ctrl_block->run_time = cur_context_ctrl_block->run_time + quantum_size;
	
	// If all threads have exited
	if(num_running_threads == 0)
	{
		setcontext(&main_context);
		// Deactivate timer and return to main.
	}

	// Save the old context
	mythread_control_block* previous_context_ctrl_block = cur_context_ctrl_block;
	// Take the next context in line
	cur_context_ctrl_block = list_shift(run_queue);
	cur_context_ctrl_block->state = RUNNING;
	
	//printf("Scheduling out %d\n", cur_context_ctrl_block->thread_id);
	
	// Execute the next thread
	swapcontext(&(previous_context_ctrl_block->context) ,&(cur_context_ctrl_block->context)); 
}

void set_quantum_size(int quantum)
{
	quantum_size = quantum;
	
	// Set up the signals
	setup_signals();
	// Block all signals until threads are ready to run
	sighold(SIGALRM);
}

void exit_my_thread()
{
	// Set the thread to the exit state and go back to the scheduler
	cur_context_ctrl_block->state = EXIT;
	num_running_threads--;
	setcontext(&scheduler_context);
}

void my_threads_state()
{
	mythread_control_block* thread;
	int i;
	for(i=0; i < next_table_index; i++)
	{
		thread = thread_table[i];
		printf("%s\t%s\t%d\t\n",
				thread->thread_name,
				state_string[thread->state],
				thread->run_time		
		);
	}
}


int create_semaphore(int value)
{
	Semaphore* sema = malloc(sizeof(Semaphore));
	sema->value = value;
	sema->initial_value = value;
	sema->wait_queue = list_create(NULL);
	
	semaphore_table[next_semaphore_index++] = sema;
	
	return (next_semaphore_index -1);
}

void semaphore_wait(int semaphore)
{
	if(semaphore_table[semaphore] == NULL)
	{
		perror("Error: Tried to wait on a semaphore that was not initialized");
		exit(-1);
	}
	
	// Retrieve the indexed semaphore
	Semaphore * sema = semaphore_table[semaphore];
	
	// Mask signal interrupts
	sighold(SIGALRM);
	
	// P
	//printf("%d is waiting, %d to %d\n", cur_context_ctrl_block->thread_id, sema->value, sema->value-1);
	sema->value--;
	
	if(sema->value < 0)
	{
		// Wait in the semaphore's wait queue queue
		list_append(sema->wait_queue, cur_context_ctrl_block);
		//printf("%d is Blocked on sema's value %d\n", cur_context_ctrl_block->thread_id, sema->value);
		cur_context_ctrl_block->state = BLOCKED;
	}
	
	// Unmask signal interrupts
	sigrelse(SIGALRM);
	
	// Wait until the thread becomes unblocked
	while(cur_context_ctrl_block->state == BLOCKED)
	{
		//swapcontext(&(cur_context_ctrl_block->context), &scheduler_context);
	} 
}


void semaphore_signal(int semaphore)
{
	//printf("Thread %d is Signaling\n", cur_context_ctrl_block->thread_id);
	if(semaphore_table[semaphore] == NULL)
	{
		printf("Error: Tried to signal on a semaphore that was not initialized\n");
		exit(-1);
	}
	
	// Retrieve the indexed semaphore
	Semaphore * sema = semaphore_table[semaphore];
	
	// Mask signal interrupts
	sighold(SIGALRM);
	printf("%d is signaling, %d to %d\n", cur_context_ctrl_block->thread_id, sema->value, sema->value+1);
	// V
	sema->value++;
	
	// Unqueue the next thread and put it in the run queue
	mythread_control_block* next_in_line = list_shift(sema->wait_queue);

	if(next_in_line != NULL)
	{
		printf("Thread is %d pulled out of wait queue.\n", next_in_line->thread_id);
		next_in_line->state = RUNNABLE;
		list_append(run_queue, next_in_line);
	}
	
	// UnMask signal interrupts
	sigrelse(SIGALRM);
	
}

void destroy_semaphore(int semaphore)
{
	Semaphore* sema = semaphore_table[semaphore];
	
	if(sema == NULL)
	{
		printf("Error: Tried to delete a semaphore that was not initialized\n"); 
	}
	else if(!list_empty(sema->wait_queue))
	{
		printf("Error: There are still threads waiting on this semaphore\n"); 
	}
	
	if(sema->initial_value != sema->value)
	{
		printf("Warning: the initial value of the semaphore is not equal to the final value\n");
	}
	
	free(semaphore_table[semaphore]);

}

