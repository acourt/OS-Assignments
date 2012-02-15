#ifndef _PERSON_H_
#define _PERSON_H_

#define NUM_FLOORS 10

#include <pthread.h>
#include <stdlib.h>

#include "Elevator.h"


static int person_count = 0;
extern struct Elevator elevator;


struct Person {
    int current_floor;
    int ticks_until_use_elevator;
    int ID;
    
    pthread_t person_thread;
};

void Person_Init(struct Person* person);
void Person_Start(struct Person* person);
void *Person_Work(void* person);

#endif // _PERSON_H_
