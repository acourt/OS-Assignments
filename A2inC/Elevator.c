#include "Elevator.h"
#include "Person.h"
/* Since the elevator is a singleton, we don't need to take in the pointer to the struct as a parameter */


int Elevator_Init()
{
    elevator.position = 0;
    elevator.clockPeriod = 1;
    elevator.direction = UP;
    pthread_mutex_init(&(elevator.elevator_mutex), NULL);
    pthread_create(&(elevator.clockThread), NULL, &ClockRun, NULL);
    
    return 0;
}


int ElevatorRequest(struct Person * person, int destination)
{
    pthread_mutex_lock(&(elevator.elevator_mutex));
	
    // If my destination if up and the elevator is not there yet
    if (elevator.position <= destination) {
        // Wait for the upsweep to take me to destination
        while (elevator.position < destination) {
            printf("Person: %d Destination: %d Waiting on the upsweep.\n", person->ID, destination);
            pthread_cond_wait(&cond_upsweep, &(elevator.elevator_mutex));
        }
    }
    else{
        while (elevator.position > destination) {
            printf("Person: %d Destination: %d  Waiting on the downsweep.\n", person->ID, destination);
            pthread_cond_wait(&cond_downsweep, &(elevator.elevator_mutex));
        }
    }
    pthread_mutex_unlock(&(elevator.elevator_mutex));
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
        printf("Elevator Position: %d, Direction: %d\n", elevator.position, elevator.direction);
        pthread_cond_broadcast(&cond_clock_notify);
        if (elevator.direction == UP) {
            pthread_cond_broadcast(&cond_upsweep);
        }
        else
            pthread_cond_broadcast(&cond_downsweep);
        
		sleep(elevator.clockPeriod);
        
        // Update the elevator's position
		if(elevator.direction == UP)
        {
            // If at the top of the building, change to downsweep mode
            if (elevator.position == NUM_FLOORS -1) {
                elevator.direction = DOWN;
            }
            else
                elevator.position++;
        }
        else {
            // If at the bottom floor, change to upsweep mode
            if (elevator.position == 0) {
                elevator.direction = UP;
            }
            else
                elevator.position--;
        }
	}
}










