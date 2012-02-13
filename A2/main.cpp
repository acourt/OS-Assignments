#include <stdlib.h>
#include <iostream>
#include "Elevator.h"

using namespace std;

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
	
	// Construct the people objects
	
	// Start Simulation
	
	return 0;
}


