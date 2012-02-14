#include <stdlib.h>
#include <iostream>
#include <pthread.h>
#include "Elevator.h"
#include "Person.h"


using namespace std;
void printUsage();
void printUsage()
{
	cout << "USAGE: Elevator <Number of people> <Number of floors>" << endl;
	cout << "Optional argument: -v <Level of verbosity>" << endl;
}

int main(int argc, char* argv[])
{
	// Check that the input of the program is correct
	if(argc < 2)	
	{
		printUsage();
		exit(-1);
	}
	
	// Construct the Elevator object
	Elevator elevator;
	
	// Construct the Person objects
	//List personList;
	
	
	
	// Start Simulation
	
	return 0;
}


