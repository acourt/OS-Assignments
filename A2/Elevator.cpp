#include "Elevator.h"
#include "unistd.h"

using namespace std;

Elevator::Elevator()
{
	position = 0;
	cond_clock_notify = PTHREAD_COND_INITIALIZER;
	pthread_create(&clockThread, NULL, &Elevator::ClockRun, NULL);
}

Elevator::~Elevator()
{
	
}

void Elevator::ClockRun()
{
	for(;;) {
	// Clock cycle
		sleep(clockPeriod);
		pthread_cond_signal( &cond_clock_notify );
	}
}

int Elevator::Request(int destination)
{
	
	return destination;
}

void Elevator::Release()
{

}
