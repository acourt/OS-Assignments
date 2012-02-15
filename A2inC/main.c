//
//  main.c
//  Elevator
//
//  Created by Alexandre Courtemanche on 12-02-14.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include <stdio.h>
#include "Person.h"
#include "Elevator.h"

void printUsage();
void printUsage()
{
    printf("USAGE: Elevator <Number of people> <Number of floors>");
    printf("Optional argument: -v <Level of verbosity>");
}

// Construct the Elevator object
static struct Elevator elevator;

int main (int argc, const char * argv[])
{
    // Check that the input of the program is correct
	if(argc < 2)	
	{
		printUsage();
		exit(-1);
	}
	

	
	// Construct the Person objects
	//List personList;
	
	
	
	// Start Simulation
	
	return 0;
}

