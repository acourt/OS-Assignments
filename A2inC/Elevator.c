#include "Elevator.h"
#include "Person.h"
/* Since the elevator is a singleton, we don't need to take in the pointer to the struct as a parameter */


int Elevator_Init()
{
    elevator.position = 0;
    elevator.clockPeriod = 1;
    elevator.direction = IDLE;
    
    elevator.upsweep_final_dest = NONE;
    elevator.downsweep_final_dest = NONE;
    
    elevator.upsweep_count = 0;
    elevator.downsweep_count = 0;
    
    pthread_mutex_init(&(elevator.elevator_mutex), NULL);
    pthread_create(&(elevator.clockThread), NULL, &ClockRun, NULL);
    
    return 0;
}


int ElevatorRequest( Person * person, int destination)
{
    pthread_mutex_lock(&(elevator.elevator_mutex));
    
    // Adjust the elevator's destination so that I can get picked up
    if (elevator.position < person->current_floor && // If I'm above the elevator
        elevator.upsweep_final_dest < person->current_floor) // If the elevator isn't going up enough to get me
    {
        elevator.upsweep_final_dest = person->current_floor;
    }
    else if((elevator.position > person->current_floor && // If I'm below the elevator
            elevator.downsweep_final_dest > person->current_floor) // If the elevator isn't going down enough to get me
            || elevator.downsweep_final_dest == NONE) // Or if the elevator isn't moving at all
    {
        elevator.downsweep_final_dest = person->current_floor;
    }
    
    
    // Wait for the elevator to come to you.
    while (elevator.position != person->current_floor) {
        printf("Person: %d Position: %d Destination: %d Waiting for elevator.\n", person->ID, person->current_floor, destination);
        pthread_cond_wait(&cond_request, &(elevator.elevator_mutex));
    }
    
    // If my destination is up and the elevator is not there yet
    if (elevator.position < destination) {
        elevator.upsweep_count++;
        
        // If the elevator doesn't go far enough to reach my destination
        if (elevator.upsweep_final_dest < destination) {
            //printf("upsweep_final_dest: %d", elevator.upsweep_final_dest);
            elevator.upsweep_final_dest = destination;
        }
        
        // Wait for the upsweep to take me to destination
        printf("Person: %d Destination: %d Waiting on the upsweep.\n", person->ID, destination);
        while (elevator.position != destination) {
            pthread_cond_wait(&cond_upsweep, &(elevator.elevator_mutex));
            printf("Person: %d Destination: %d  Upsweeping.\n", person->ID, destination);
        }
        elevator.upsweep_count--;
    }
    else if(elevator.position > destination){
        elevator.downsweep_count++;
        
        // If the elevator doesn't go far enough to reach my destination
        if (elevator.downsweep_final_dest > destination ||
            elevator.downsweep_final_dest == NONE) {
            //printf("downsweep_final_dest: %d", elevator.downsweep_final_dest);
            elevator.downsweep_final_dest = destination;
        }
        
        // Wait for the upsweep to take me to destination
        printf("Person: %d Destination: %d  Waiting on the downsweep.\n", person->ID, destination);
        while (elevator.position != destination) {
            pthread_cond_wait(&cond_downsweep, &(elevator.elevator_mutex));
            printf("Person: %d Destination: %d  Downsweeping.\n", person->ID, destination);
        }
        elevator.downsweep_count--;
    }
    
    pthread_mutex_unlock(&(elevator.elevator_mutex));
    return destination;
}

void ElevatorRelease( Person * person)
{
    
}


void* ClockRun(void * dummyParam)
{
    // Clock Cycle
    for(;;) {
        
        
        
        // Broadcast clock pulses to all listening threads
        printf("Elevator Position: %d, Direction: %d upsweepFinal: %d downsweepFinal: %d\n", elevator.position, elevator.direction, 
               elevator.upsweep_final_dest, elevator.downsweep_final_dest);
        pthread_cond_broadcast(&cond_clock_notify);
        pthread_cond_broadcast(&cond_upsweep);
        pthread_cond_broadcast(&cond_downsweep);
        pthread_cond_broadcast(&cond_request);
        /*if (elevator.direction == UP) {
            printf("Broadcasting upsweep\n");
            pthread_cond_broadcast(&cond_upsweep);
        }
        else if(elevator.direction == DOWN){
            printf("Broadcasting downsweep\n");
            pthread_cond_broadcast(&cond_downsweep);
        }*/
        
        
        
        // Give time for the threads to execute what they need
		sleep(elevator.clockPeriod);
        //int prompt = 0;
        printf("Clock pulse done\n");
        //scanf("%d", &prompt);
        
        
        
        // Perform Logic that drives elevator
        pthread_mutex_lock(&(elevator.elevator_mutex));
        // Check if there are any requests while idle
        if (elevator.direction == IDLE) {
            if (elevator.upsweep_final_dest != NONE) {
                elevator.direction = UP;
            }
            else if(elevator.downsweep_final_dest != NONE) {
                elevator.direction = DOWN;
            }
        }
        
        // Perform sweeping
        if (elevator.direction == UP) {
            if (elevator.position < elevator.upsweep_final_dest) {
                elevator.position++;
            }
            else if(elevator.position == elevator.upsweep_final_dest) {
                // Clear the final destination for upsweep
                elevator.upsweep_final_dest = NONE;
                // Check if there are any requests for downsweep
                if (elevator.downsweep_final_dest != NONE) {
                    elevator.direction = DOWN;
                }
                else {
                    elevator.direction = IDLE;
                }
            }
        }
        else if(elevator.direction == DOWN) {
            if (elevator.position > elevator.downsweep_final_dest && elevator.downsweep_final_dest != NONE) {
                elevator.position--;
            }
            else if(elevator.position == elevator.downsweep_final_dest) {
                // Clear the final destination for downsweep
                elevator.downsweep_final_dest = NONE;
                // Check if there are any requests for upsweep
                if (elevator.upsweep_final_dest != NONE) {
                    elevator.direction = UP;
                }
                else {
                    elevator.direction = IDLE;
                }
            }
            
        }
        pthread_mutex_unlock(&(elevator.elevator_mutex));
    } // for(;;)
}










