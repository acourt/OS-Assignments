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
    printf("USAGE: Elevator <Number of people> <Number of floors>");
    printf("Optional argument: -v <Level of verbosity>");
}

// Construct the Elevator object
struct Elevator elevator;
pthread_cond_t cond_clock_notify = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_upsweep = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_downsweep = PTHREAD_COND_INITIALIZER;

int main (int argc, const char * argv[])
{
    // Check that the input of the program is correct
	/*if(argc < 2)	
	{
		printUsage();
		exit(-1);
	}*/
    
    
    
	Elevator_Init();
    /*struct Person person;
    Person_Init(&person);
	Person_Start(&person);*/
    
	// Construct the Person objects
	//List personList;
	while (1) {
        
    }
	
	
	// Start Simulation
	
	return 0;
}

