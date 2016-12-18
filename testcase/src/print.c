#include <stdio.h>
#include "FLOAT.h"

#include "trap.h"


int main() {
	printf("%d + %d = %d\n", 1, 1, 2);

    int x = f2F(7.9);
    printf("%d",x);
    return 0;
}
