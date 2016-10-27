#include "FLOAT.h"

FLOAT F_mul_F(FLOAT a, FLOAT b) {
    long long  result =(long long ) a *(long long) b;
	return result>>16;
}

FLOAT F_div_F(FLOAT a, FLOAT b) {
	 
  /*  FLOAT result;
    FLOAT remain;
    int t = ((a>>31)&1)?0xffffffff:0;
	asm volatile ("idivl %2": "=a"(result) ,"=d"(remain): "r"(b),"a"(a),"d"(t));

    return (result<<16)+remain;
    */
    // int remainder[2];
     int r,i,f;
     asm volatile ("idivl %2" : "=a"(i), "=d"(r) : "r"((b)), "a"((a)), "d"(a>>31));
     asm volatile ("idivl %2" : "=a"(f), "=d"(r) : "r"((b)), "a"(r<<16), "d"(r>>31));
     return (i<<16 + f);

}


FLOAT f2F(float a) {
    int v = *(int *)&a;
    int s = v >>31;
    int e = (v >> 23) & 0xff - 127;
    int m = v & 0x7fffff;
    int r = (v&0x7fffff) + 0x800000;
    FLOAT result;
    result = (16+e-23)>0 ? (r<<(16+r-23)) : (r>>23-e-16);
    if (s)
    result = -result;
    return result;
}

FLOAT Fabs(FLOAT a) {
    if(a>0) return a;
    else return -a;
}

/* Functions below are already implemented */

FLOAT sqrt(FLOAT x) {
	FLOAT dt, t = int2F(2);

	do {
		dt = F_div_int((F_div_F(x, t) - t), 2);
		t += dt;
	} while(Fabs(dt) > f2F(1e-4));

	return t;
}

FLOAT pow(FLOAT x, FLOAT y) {
	/* we only compute x^0.333 */
	FLOAT t2, dt, t = int2F(2);

	do {
		t2 = F_mul_F(t, t);
		dt = (F_div_F(x, t2) - t) / 3;
		t += dt;
	} while(Fabs(dt) > f2F(1e-4));

	return t;
}

