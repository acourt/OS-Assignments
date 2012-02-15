#ifndef _ELEVATOR_H_
#define _ELEVATOR_H_

#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include "Person.h"

extern struct Elevator elevator;
extern pthread_cond_t cond_clock_notify;

struct Elevator {
    int position;
    int clockPeriod;

    pthread_t clockThread;
};

int ElevatorRequest(struct Person * person, int destination);
void ElevatorRelease(struct Person * person);

// Clock methods
void StartClock();
void AddToNotifyList(struct Person * person);
void* ClockRun(void *);


#endif // _ELEVATOR_H_