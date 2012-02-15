#include "Person.h"

void Person_Init(struct Person* person)
{
    person->ID = person_count++;
    person->current_floor = rand() % NUM_FLOORS;
}


void Person_Start(struct Person* person)
{
    pthread_create(&(person->person_thread), NULL, &Person_Work, (void*)&person);
}

void* Person_Work(void* person)
{
    person = (struct Person*) person;
    pthread_mutex_t dummy_mutex;
	pthread_mutex_init(&dummy_mutex, NULL);
	
	for(;;)
	{
		pthread_mutex_lock(&dummy_mutex);
		// Wait for clock pulse
		pthread_cond_wait(&(elevator->cond_clock_notify), &dummy_mutex);
		
		// Tick has occured
		if(person->ticksUntilUseElevator == 0) {
			// Generate next destination
			int destination = rand() % NUM_FLOORS;
			person->elevator->Request(destination);
		}
		else {
			
		}
		
		person->ticksUntilUseElevator--;
		
		pthread_mutex_unlock(&dummy_mutex);
		
	}
}
    return NULL;
}
