#ifndef _ELEVATOR_H_
#define _ELEVATOR_H_

#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include "Person.h"

// Elevator directions
#define UP 1
#define DOWN 0

extern struct Elevator elevator;
extern pthread_cond_t cond_clock_notify;
extern pthread_cond_t cond_upsweep;
extern pthread_cond_t cond_downsweep;

struct Elevator {
    int position;
    int clockPeriod;
    int direction;
    pthread_t clockThread;
    pthread_mutex_t elevator_mutex;
    
};

int Elevator_Init();
int ElevatorRequest(struct Person * person, int destination);
void ElevatorRelease(struct Person * person);

// Clock methods
void StartClock();
void AddToNotifyList(struct Person * person);
void* ClockRun(void *);


#endif // _ELEVATOR_H_