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
#define IDLE -1

// Elevator destination
#define NONE -1

extern struct Elevator elevator;
extern pthread_cond_t cond_clock_notify;
extern pthread_cond_t cond_upsweep;
extern pthread_cond_t cond_downsweep;
extern pthread_cond_t cond_request;

struct Elevator {
    int position;
    int clockPeriod;
    int direction;
    int upsweep_final_dest;
    int downsweep_final_dest;
    
    int upsweep_count;
    int downsweep_count;
    
    pthread_t clockThread;
    pthread_mutex_t elevator_mutex;
    
};

int Elevator_Init();
/*int ElevatorRequest( Person * person, int destination);
void ElevatorRelease( Person * person);*/

// Clock methods
void StartClock();
//void AddToNotifyList(Person * person);
void* ClockRun(void *);


#endif // _ELEVATOR_H_
