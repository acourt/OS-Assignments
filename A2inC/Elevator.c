#include "Elevator.h"
#include "Person.h"
/* Since the elevator is a singleton, we don't need to take in the pointer to the struct as a parameter */


int Elevator_Init()
{
    elevator.position = 0;
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
    
    // Store the id of the person who rode the elevator
    if(elevator_riders_pointer + ELEVATOR_RIDERS_STORAGE_SPACE - 1 >= elevator_rider_ids)
    {
        *elevator_riders_pointer = person->ID;
        elevator_riders_pointer++;
    }
    
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
        //printf("Person: %d Position: %d Destination: %d Waiting for elevator.\n", person->ID, person->current_floor, destination);
        pthread_cond_wait(&cond_request, &(elevator.elevator_mutex));
    }
    
    if(print_level == 2)
        printf("Person: %d Entered the elevator.\n", person->ID);
    
    // If my destination is up and the elevator is not there yet
    if (elevator.position < destination) {
        elevator.upsweep_count++;
        
        // If the elevator doesn't go far enough to reach my destination
        if (elevator.upsweep_final_dest < destination) {
            //printf("upsweep_final_dest: %d", elevator.upsweep_final_dest);
            elevator.upsweep_final_dest = destination;
        }
        
        // Wait for the upsweep to take me to destination
        while (elevator.position != destination) {
            pthread_cond_wait(&cond_upsweep, &(elevator.elevator_mutex));
            if(print_level == 1)
                printf("Person: %d is riding the elevator.\n", person->ID);
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
        while (elevator.position != destination) {
            pthread_cond_wait(&cond_downsweep, &(elevator.elevator_mutex));
            if(print_level == 1)
                printf("Person: %d is riding the elevator.\n", person->ID);
        }
        elevator.downsweep_count--;
    }
    
    pthread_mutex_unlock(&(elevator.elevator_mutex));
    return destination;
}

void ElevatorRelease( Person * person)
{
    if(print_level == 1 || print_level == 2)
        printf("Person %d got out of elevator\n", person->ID);
}


void* ClockRun(void * dummyParam)
{
    // Clock Cycle
    for(;;) {

        // Broadcast clock pulses to all listening threads
        
        if(print_level ==4) {
            printf("Elevator Position: %d, Direction: %d upsweepFinal: %d downsweepFinal: %d\n", elevator.position, elevator.direction, 
                   elevator.upsweep_final_dest, elevator.downsweep_final_dest);
        }
        pthread_cond_broadcast(&cond_clock_notify);
        pthread_cond_broadcast(&cond_upsweep);
        pthread_cond_broadcast(&cond_downsweep);
        pthread_cond_broadcast(&cond_request);
        clock_ticks++;

        
        
        // Give time for the threads to execute what they need
		usleep(1000*CLOCK_PERIOD);

        printf("Tick %d\n", clock_ticks);
        if(print_level == 3 && clock_ticks == 100)
        {
            int i;
            FILE* fd = fopen ( "Aggregated_Data.txt", "w" );
            for(i=0; i< elevator_riders_pointer - elevator_rider_ids; i++)
            {
                fprintf(fd, "Person %d entered elevator\n ", elevator_rider_ids[i]);
            }
            fclose(fd);
            
        }
        
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










