#include <stdio.h>
#include "FLOAT.h"

#include "trap.h"


int main() {
	printf("%d + %d = %d\n", 1, 1, 2);
    printf("%d\n", 1);

    int x = f2F(7.9);
    int y = int2F(200);
    printf("%d\n",y - x); 
    return 0;
}
