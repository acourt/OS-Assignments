//
//  main.c
//  Elevator
//
//  Created by Alexandre Courtemanche on 12-02-14.
//  Copyright 2012 McGill University. All rights reserved.
//


#include "Person.h"
#include "Elevator.h"

#define NUM_PERSONS 2


void printUsage();
void printUsage()
{
    printf("USAGE: Elevator <Number of people> <Number of floors>");
    printf("Optional argument: -v <Level of verbosity>");
}

// Construct the Elevator object
struct Elevator elevator;
pthread_cond_t cond_clock_notify = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_upsweep = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_downsweep = PTHREAD_COND_INITIALIZER;


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
    
    Elevator_Init();
    for (i=0; i< NUM_PERSONS; i++) {
        persons[i] = malloc(sizeof(Person));
        Person_Init(persons[i]);
    }
    for (i=0; i< NUM_PERSONS; i++) {
        Person_Start(persons[i]);
    }
    
    
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











