#include <stdio.h>
#include <string.h>
#include "FLOAT.h"

#include "trap.h"

char buf[30];

int main() {
	init_FLOAT_vfprintf();
    snprintf(buf, 30, "%d", 12);
    printf("%s", buf);
    return 0;
}
