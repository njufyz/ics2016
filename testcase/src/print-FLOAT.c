#include <stdio.h>
#include <string.h>
#include "FLOAT.h"

#include "trap.h"

char buf[128];

int main() {
	init_FLOAT_vfprintf();

#ifdef LINUX_RT
//	printf("%f\n", FLOAT_ARG(0x00010000));
//	printf("%f\n", FLOAT_ARG(0x00013333));
//    printf("%f %f\n", FLOAT_ARG(0x00010000),FLOAT_ARG(0x00013333));
    printf("%f %llu\n", FLOAT_ARG(0xfffecccd), 1234567890);

#else
//	sprintf(buf, "%f %f", FLOAT_ARG(0x00010000),FLOAT_ARG(0x00013333));
//	nemu_assert(strcmp(buf, "1.000000 1.199996") == 0);
	
//	sprintf(buf, "%d %f", 123,FLOAT_ARG(0x10000));
//	nemu_assert(strcmp(buf, "123 1.000000") == 0);

//	sprintf(buf, "%f %f",FLOAT_ARG(0x10000),FLOAT_ARG(0x10000));
  //  nemu_assert(strcmp(buf, "1.000000 1.000000") == 0);

     sprintf(buf,"%f %d",FLOAT_ARG(0x10000),123);
     nemu_assert(strcmp(buf,"1.000000 123")==0);
    
//	sprintf(buf, "%d %f", 123456,FLOAT_ARG(0xfffecccd));
//    nemu_assert(strcmp(buf, "123456 -1.199996") == 0);
#endif

	return 0;
}
