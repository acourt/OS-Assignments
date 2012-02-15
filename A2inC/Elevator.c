#include "Elevator.h"
#include "Person.h"
/* Since the elevator is a singleton, we don't need to take in the pointer to the struct as a parameter */


int Elevator_Init()
{
    elevator.position = 0;
    elevator.clockPeriod = 1;
    pthread_create(&(elevator.clockThread), NULL, &ClockRun, NULL);
    
    return 0;
}


int ElevatorRequest(struct Person * person, int destination)
{
    return destination;
}
void ElevatorRelease(struct Person * person)
{
    
}

// Clock methods
void StartClock()
{
    
}
void AddToNotifyList(struct Person * person)
{
    
}
void* ClockRun(void * dummyParam)
{
    for(;;) {
        // Clock cycle
		sleep(elevator.clockPeriod);
		pthread_cond_signal(&cond_clock_notify );
	}
}