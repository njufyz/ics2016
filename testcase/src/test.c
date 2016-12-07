#include <stdio.h>
#include <string.h>
#include "FLOAT.h"

#include "trap.h"

char buf[128];

int main()
{
        buf[0] = '1';
        buf[1] = '2';
        buf[3] = 0;
        printf("%s",buf);
        //nemu_assert(strcmp(buf,"-1")==0);
        return 0;
}

