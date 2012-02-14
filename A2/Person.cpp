#include "Person.h"

Person::Person(Elevator * elevator)
{
	this->elevator = elevator;
	this->currentFloor = rand() % NUM_FLOORS;
	this->personID = PersonCount++;
}

Person::~Person()
{
	PersonCount--;
}

void Person::StartWorking()
{
	pthread_create(&personThread, NULL, &(Person::Work), NULL);
}

void Person::Work()
{
	pthread_mutex_t dummy_mutex;
	pthread_mutex_init(&dummy_mutex);
	
	for(;;)
	{
		pthread_mutex_lock(&dummy_mutex);
		// Wait for clock pulse
		pthread_cond_wait(&(elevator->cond_clock_notify), &dummy_mutex);
		
		// Tick has occured
		if(ticksUntilUseElevator == 0) {
			// Generate next destination
			int destination = rand() % NUM_FLOORS;
			elevator->Request(destination);
		}
		else {
			
		}
		
		ticksUntilUseElevator--;
		
		pthread_mutex_unlock(&dummy_mutex);
		
	}
}


	




