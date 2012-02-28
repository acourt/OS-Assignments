//
//  main.c
//  Elevator
//
//  Created by Alexandre Courtemanche on 12-02-14.
//  Copyright 2012 McGill University. All rights reserved.
//


#include "Person.h"
#include "Elevator.h"



void printUsage();
void printUsage()
{
    printf("USAGE: Elevator <Number of people> <Number of floors> <Maximum Capacity>\n  ");
}

// Construct the Elevator object
struct Elevator elevator;
pthread_cond_t cond_clock_notify = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_request = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_upsweep = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_downsweep = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_full_elevator = PTHREAD_COND_INITIALIZER;
// Person mutex
pthread_mutex_t person_lock;


Person* persons[50];

int main (int argc, const char * argv[])
{
    printf("Stuff");
    int i;
    // Check that the input of the program is correct
	if(argc < 3)	
	{
		printUsage();
		exit(-1);
	}
	printf("Stuff");
	int num_people = 5 /*atoi(argv[1])*/;
	int num_floors = 10/*atoi(argv[2])*/;
	int max_capacity = 3 /*atoi(argv[3])*/;
    
    
    pthread_cond_init(&cond_clock_notify, NULL);
    pthread_cond_init(&cond_request, NULL);
    pthread_cond_init(&cond_upsweep, NULL);
    pthread_cond_init(&cond_downsweep, NULL);
    pthread_cond_init(&cond_full_elevator, NULL);
    //printf("%d\n%d\n%d\n", num_people, num_floors, max_capacity);
    printf("Stuff");
    pthread_mutex_init(&person_lock, NULL);
    for (i=0; i< num_people; i++) {
        printf("Stuff");
        persons[i] = malloc(sizeof(Person));
        Person_Init(persons[i]);
    }
    
    for (i=0; i< num_people; i++) {
        printf("Starting person %d\n",i);
        Person_Start(persons[i]);
    }
    
    Elevator_Init(num_floors, max_capacity);
    
	// Construct the Person objects
	while (1) {
        
    }
	
    for (i=0; i< num_people; i++) {
        free(persons[i]);
    }	
    
    
	// Start Simulation
	
	return 0;
}











