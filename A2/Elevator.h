#ifndef _ELEVATOR_H_
#define _ELEVATOR_H_

#include <pthread.h>
#include "Person.h"

class Elevator {
	
	public:
	
	// Variables
	int position;
	int clockPeriod;
	
	pthread_t clockThread;
	// Persons wait on this clock pulse to continue
	pthread_cond_t cond_clock_notify;
	
	// Methods
	Elevator();
	
	~Elevator();
	
	int Request(int destination);	
	void Release();	

	// Clock methods
	void StartClock();
	void AddToNotifyList(Person * person);
	
	
	private:
	void ClockRun();

};

#endif // _ELEVATOR_H_
