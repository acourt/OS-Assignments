//
//  main.c
//  Elevator
//
//  Created by Alexandre Courtemanche on 12-02-14.
//  Copyright 2012 McGill University. All rights reserved.
//


#include "Person.h"
#include "Elevator.h"

#define NUM_PERSONS 3


int print_level = 2;
int clock_ticks = 0;


void printUsage();
void printUsage()
{
    printf("USAGE: Elevator <Number of people> <Number of floors>");
    printf("Optional argument: -v <Level of verbosity>");
}

// Construct the Elevator object
struct Elevator elevator;
pthread_cond_t cond_clock_notify = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_request = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_upsweep = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_downsweep = PTHREAD_COND_INITIALIZER;

// Person mutex
pthread_mutex_t person_lock;


int* elevator_rider_ids;
int* elevator_riders_pointer;

Person* persons[50];

int main (int argc, const char * argv[])
{
    int i;
    // Check that the input of the program is correct
	/*if(argc < 2)	
	{
		printUsage();
		exit(-1);
	}*/
    
    int elevator_ids[ELEVATOR_RIDERS_STORAGE_SPACE];
    elevator_rider_ids = elevator_ids;
    elevator_riders_pointer = elevator_rider_ids;
    
    pthread_cond_init(&cond_clock_notify, NULL);
    pthread_cond_init(&cond_request, NULL);
    pthread_cond_init(&cond_upsweep, NULL);
    pthread_cond_init(&cond_downsweep, NULL);
    
    pthread_mutex_init(&person_lock, NULL);
    for (i=0; i< NUM_PERSONS; i++) {
        persons[i] = malloc(sizeof(Person));
        Person_Init(persons[i]);
    }
    for (i=0; i< NUM_PERSONS; i++) {
        printf("Starting person %d",i);
        Person_Start(persons[i]);
    }
    
    Elevator_Init();
    
    /*Person person;
    Person_Init(&person);
	Person_Start(&person);*/
    
	// Construct the Person objects
	//List personList;
	while (1) {
        
    }
	
    for (i=0; i< NUM_PERSONS; i++) {
        free(persons[i]);
    }	
    
    
	// Start Simulation
	
	return 0;
}











