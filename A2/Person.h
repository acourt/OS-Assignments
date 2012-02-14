#ifndef _PERSON_H_
#define _PERSON_H_

#define NUM_FLOORS

#include <pthread.h>

#include "Elevator.h"

using namespace std;	

static int PersonCount;
class Elevator;

class Person {
	
	public:
	
		Person();
		~Person();
		
		pthread_t personThread;
		
		void StartWorking();
		
	private:
		Elevator * elevator;
		int currentFloor;
		int ticksUntilUseElevator;
		
	protected:
		int personID;	
		
};

#endif // _PERSON_H_
