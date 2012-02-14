#ifndef _PERSON_H_
#define _PERSON_H_

#define NUM_FLOORS 10

#include <pthread.h>
#include <stdlib.h>

#include "Elevator.h"

using namespace std;	

static int PersonCount;
class Elevator;

class Person {
	
	public:
	
		Person(Elevator * elevator);
		~Person();
		
		pthread_t personThread;
		
		void StartWorking();
        void Word();
	private:
		Elevator * elevator;
		int currentFloor;
		int ticksUntilUseElevator;
		
	protected:
		int personID;	
		
};

#endif // _PERSON_H_
