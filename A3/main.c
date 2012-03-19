//
//  main.c
//  MyThreads
//
//  Created by Alexandre Courtemanche on 12-03-07.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include <stdio.h>
#include <slack/std.h>
#include <slack/list.h>
#include <sys/time.h>


int main (int argc, const char * argv[])
{
	
	List *l = list_create(NULL);
	int i= 100;
	int j = 200;
	int k = 300;
	
	// Append integers to list
	l = list_append_int(l,i);
	l = list_append_int(l,j);
	l = list_append_int(l,k);
	
	// Dequeue integers
	int dequeued = list_shift_int(l);
	printf("Value of dequeued integer = %d\n", dequeued);
	
	return 0;
}

