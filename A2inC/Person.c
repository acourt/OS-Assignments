#include "Person.h"



int person_count = 0;
void Person_Init(Person* person)
{
    person->ID = person_count++;
    person->ticks_until_use_elevator = (rand() % elevator.num_floors);
    person->current_floor = (rand() % elevator.num_floors);
}


void Person_Start(Person* person)
{
    pthread_create(&(person->person_thread), NULL, &Person_Work, (void*)person);
}

void* Person_Work(void* arg)
{
    Person* working_person = (Person*) arg ;
	
	
	for(;;)
	{
		pthread_mutex_lock(&person_lock);
		// Wait for clock pulse
		pthread_cond_wait( &cond_clock_notify, &person_lock);
		pthread_mutex_unlock(&person_lock);
        
		// Tick has occured
		if(working_person->ticks_until_use_elevator == 0) {
			// Generate next destination
			int destination = (rand() % elevator.num_floors);
            working_person->current_floor = ElevatorRequest(working_person, destination);
            ElevatorRelease(working_person);
            working_person->ticks_until_use_elevator = 5 /*(rand() % elevator.num_floors)*/;
		}
		else {
            printf("Person %d; Position: %d; Ticks: %d;\n", working_person->ID, working_person->current_floor, working_person->ticks_until_use_elevator);
			working_person->ticks_until_use_elevator--;
		}
	}
}
