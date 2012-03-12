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

int main (int argc, const char * argv[])
{
    List *l;
    l = list_append_int(l, 100);
    printf("Hello, World!\n");
    return 0;
}

