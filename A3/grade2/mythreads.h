/*
 * =====================================================================================
 *
 *       Filename:  mythreads.h
 *
 *    Description:  Fake MyThreads Library Header File.
 *
 *        Version:  1.0
 *        Created:  03/08/2012 11:54:01 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Xing Shi Cai (XSC), xingshi.cai@mail.mcgill.ca
 *   Organization:  McGill Univ.
 *
 * =====================================================================================
 */

int init_my_threads();
int create_my_thread(char *threadname, void (*threadfunc)(), int stacksize);
void exit_my_thread();
void runthreads();
void set_quantum_size(int quantum);
int create_semaphore(int value);
void semaphore_wait(int semaphore);
void semaphore_signal(int semaphore);
void destroy_semaphore(int semaphore);
void my_threads_state();
