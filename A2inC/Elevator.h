#ifndef _ELEVATOR_H_
#define _ELEVATOR_H_

#include <pthread.h>
#include "Person.h"

struct Elevator {
    int position;
    int clockPeriod;

    pthread_t clockThread;
	// Persons wait on this clock pulse to continue
	pthread_cond_t cond_clock_notify;
};

int Request(struct Elevator * elevator);
void Release();

// Clock methods
void StartClock();
void AddToNotifyList(struct Person * person);
void ClockRun();


#endif // _ELEVATOR_H_