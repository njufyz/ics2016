#include <stdio.h>
#include <string.h>
#include "FLOAT.h"

#include "trap.h"

char buf[128];

int main()
{
	sprintf(buf,"%d",-2);
	nemu_assert(strcmp(buf,"-2")==0);
	return 0;
}
