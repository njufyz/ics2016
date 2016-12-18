#include <stdio.h>
#include "FLOAT.h"

#include "trap.h"


int main() {
    init_FLOAT_vfprintf();
    int x = f2F(7.9);
    printf("%f\n",FLOAT_ARG(x)); 
    return 0;
}
