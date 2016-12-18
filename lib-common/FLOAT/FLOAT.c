#include "FLOAT.h"

FLOAT F_mul_F(FLOAT a, FLOAT b) {
    long long  result =(long long ) a *(long long) b;
	return result>>16;
}

FLOAT F_div_F(FLOAT a, FLOAT b) {
     int r,i,f;
     asm volatile ("idivl %2" : "=a"(i), "=d"(r) : "r"((b)), "a"((a)), "d"(a>>31));
     asm volatile ("idivl %2" : "=a"(f), "=d"(r) : "r"((b)), "a"(r<<16), "d"((r<<16)>>31));
     return (i<<16 + f);
}


FLOAT f2F(float a) {
/*    int v = *(int *)&a;
    int s = v >>31;
    int e = (v >> 23) & 0xff - 127;
    int r = (v&0x7fffff) + 0x800000;
    FLOAT result;
    result = (16+e-23)>0 ? (r<<(16+e-23)) : (r>>23-e-16);
    if (s)
    result = -result;
    return result;
    */
    int i, uf, m, e, s, ans;
    uf = *(int*)&a;
    m = uf & ((1 << 23) - 1);
    e = ((uf >> 23) & ((1 << 8) - 1)) - 127;
    s = uf >> 31;
    ans = 1;
    for(i = 1; i < e + 17; i++ ) {
        ans = (ans << 1) + ((m & (1 << 22)) >> 22);
        if (ans < 0) return 0x80000000;
        m <<=  1;
    }
    if (s == 1) ans = ~ans + 1;
    return (FLOAT)(ans);
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

