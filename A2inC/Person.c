#include "Person.h"

void Person_Init(struct Person* person)
{
    person->ID = person_count++;
    person->ticks_until_use_elevator = rand() % NUM_FLOORS;
    person->current_floor = rand() % NUM_FLOORS;
}


void Person_Start(struct Person* person)
{
    pthread_create(&(person->person_thread), NULL, &Person_Work, (void*)&person);
}

void* Person_Work(void* person)
{
    struct Person *working_person = (struct Person*) person;
    
    pthread_mutex_t dummy_mutex;
	pthread_mutex_init(&dummy_mutex, NULL);
	
	for(;;)
	{
		pthread_mutex_lock(&dummy_mutex);
		// Wait for clock pulse
		pthread_cond_wait( &cond_clock_notify, &dummy_mutex);
		
		// Tick has occured
		if(working_person->ticks_until_use_elevator == 0) {
			// Generate next destination
			int destination = rand() % NUM_FLOORS;
            working_person->current_floor = ElevatorRequest(working_person, destination);
            ElevatorRelease(working_person);
            working_person->ticks_until_use_elevator = rand() % NUM_FLOORS;
		}
		else {
            printf("Person %d: Ticks left before use elevator: %d", working_person->ID, working_person->ticks_until_use_elevator);
			working_person->ticks_until_use_elevator--;
		}
		
		pthread_mutex_unlock(&dummy_mutex);
	}
}
