#include <stdio.h>
#include <string.h>
#include "FLOAT.h"

#include "trap.h"

char buf[128];

int main()
{
        sprintf(buf,"%d",-1);
        nemu_assert(strcmp(buf,"-1")==0);
        return 0;
}

