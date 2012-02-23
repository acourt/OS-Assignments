#ifndef _PERSON_H_
#define _PERSON_H_

#define NUM_FLOORS 10

#include <pthread.h>
#include <stdlib.h>

#include "Elevator.h"



extern struct Elevator elevator;


typedef struct  {
    int current_floor;
    int ticks_until_use_elevator;
    int ID;
    
    pthread_t person_thread;
} Person;
//typedef struct Person Person;

void Person_Init(Person* person);
void Person_Start(Person* person);
void *Person_Work(void* person);

#endif // _PERSON_H_
