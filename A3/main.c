//
//  main.c
//  MyThreads
//
//  Created by Alexandre Courtemanche on 12-03-07.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include <stdio.h>
#include <slack/std.h>
#include <slack/list.h>
#include <sys/time.h>
#include <stdlib.h>     
#include <math.h>

#include "MyThreads.h"


/* global vars */

/* semaphores are declared global so they can be accessed 
   in main() and in thread routine,
   here, the semaphore is used as a mutex */

int counter_mutex;

/* shared variables */
int counter; 
double result = 0.0;

void handler ()
{
    int i;
    for(i=0; i < 5; i++)
    {
        /* If you remove this protection, you should be able to see different
         * out of every time you run this program.  With this protection, you
         * should always be able to see result to be 151402.656521 */
		
        semaphore_wait(counter_mutex);       /* down semaphore */
		
        /* START CRITICAL REGION */
        int j;
        for (j = 0; j < 1000; j++) {
            result = result + sin(counter) * tan(counter);
        }
        counter++;
        /* END CRITICAL REGION */    
		
        semaphore_signal(counter_mutex);       /* up semaphore */
    }
    exit_my_thread(); /* exit thread */
}


int main()
{
    int thread_num = 10;
    int j;
    char* thread_names[] = {
        "thread 0",
        "thread 1",
        "thread 2",
        "thread 3",
        "thread 4",
        "thread 5",
        "thread 6",
        "thread 7",
        "thread 8",
        "thread 9"
    };

    /* Initialize MyThreads library. */
    init_my_threads();

    /* 250 ms */
    set_quantum_size(10);

    counter_mutex = create_semaphore(1);

    for(j=0; j<thread_num; j++)
    {
        create_my_thread(thread_names[j], (void *) &handler, 4096);
    }

    /* Print threads informations before run */
    my_threads_state();

	printf("");
    /* When this function returns, all threads should have exited. */
    runthreads();
    
    destroy_semaphore(counter_mutex);

    /* Print threads informations after run */
    my_threads_state();

    printf("The counter is %d\n", counter);
    printf("The result is %f\n", result);

    exit(0);
}









