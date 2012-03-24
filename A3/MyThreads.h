#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <slack/std.h>
#include <slack/list.h>
#include <sys/time.h>
#include <ucontext.h>
#define THREAD_NAME_LEN 255
#define INT_STACKSIZE 4096


typedef enum {
	RUNNING = 0,
	RUNNABLE,
	BLOCKED,
	EXIT
} State;

void scheduler();

typedef struct _semaphore {
	int initial_value;	
	int value;
	List* wait_queue;
} Semaphore;

typedef struct _mythread_control_block {
    ucontext_t context;
    char thread_name[THREAD_NAME_LEN];
    int thread_id;
	State state;
	int run_time;
} mythread_control_block;


static char state_string[4][10]= { "RUNNING", "RUNNABLE", "BLOCKED", "EXIT" };

/*  This function initializes all the global data structures for the thread system.
    Mythreads package will maintain many global data structures such as the runqueue,
    a table for thread control blocks. It is your responsibility to define the actual 
    data structures. One of the constraints you have the need to accommodate ucontext_t
    inside the data structures.
 */
int init_my_threads();

/*  This function creates a new thread. It returns an integer that points to the thread 
    control block that is allocated to the newly created thread in the thread control 
    block table. If the library is unable to create the new thread, it returns -1 and 
    prints out an error message. This function is responsible for allocating the stack 
    and setting up the user context appropriately. The newly created thread starts running 
    the threadfunc function when it starts. The threadname is stored in the thread control 
    block and is printed for information purposes. A newly created thread is in the RUNNABLE 
    state when it is inserted into the system. Depending on your system design, the newly 
    created thread might be included in the runqueue.
 */
int create_my_thread(char *threadname, void (*threadfunc)(), int stacksize);

void exit_my_thread();

void runthreads();

void set_quantum_size(int quantum);

int create_semaphore(int value);

void semaphore_wait(int semaphore);

void semaphore_signal(int semaphore);

void destroy_semaphore(int semaphore);

void my_threads_state();





