#include "FLOAT.h"

FLOAT F_mul_F(FLOAT a, FLOAT b) {
    FLOAT result = a * b;
	return (FLOAT)result>>16;
}

FLOAT F_div_F(FLOAT a, FLOAT b) {
	/* Dividing two 64-bit integers needs the support of another library
	 * `libgcc', other than newlib. It is a dirty work to port `libgcc'
	 * to NEMU. In fact, it is unnecessary to perform a "64/64" division
	 * here. A "64/32" division is enough.
	 *
	 * To perform a "64/32" division, you can use the x86 instruction
	 * `div' or `idiv' by inline assembly. We provide a template for you
	 * to prevent you from uncessary details.
	 *
	 *     asm volatile ("??? %2" : "=a"(???), "=d"(???) : "r"(???), "a"(???), "d"(???));
	 *
	 * If you want to use the template above, you should fill the "???"
	 * correctly. For more information, please read the i386 manual for
	 * division instructions, and search the Internet about "inline assembly".
	 * It is OK not to use the template above, but you should figure
	 * out another way to perform the division.
	 */
    FLOAT result;
    FLOAT remain;
    int t = ((a>>31)&1)?0xffffffff:0;
	asm volatile ("idivl %2": "=a"(result) ,"=d"(remain): "r"(b),"a"(a),"d"(t));
    return (result<<16)+remain;
}

FLOAT f2F(float a) {
	/* You should figure out how to convert `a' into FLOAT without
	 * introducing x87 floating point instructions. Else you can
	 * not run this code in NEMU before implementing x87 floating
	 * point instructions, which is contrary to our expectation.
	 *
	 * Hint: The bit representation of `a' is already on the
	 * stack. How do you retrieve it to another variable without
	 * performing arithmetic operations on it directly?
	 */
    int v = *(int *)&a;
    int s = v >>31;
    int e = (v >> 23) & 0xff - 127;
    int m = v & 0x7fffff;
    int result = 1;
    int i=1;
    for(;i<e+16+1;i++)
    {
        result = (result << 1) + ((m & (1<<22)) >> 22);
        if((result >>31)&1) 
        {
            FLOAT I = 0x80000000;
            return I;
        }
        m <<= 1;
    }
     if(!(s&1)) result = -result;
	return result;
}

FLOAT Fabs(FLOAT a) {
 return (a>0)?a:-a;	
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

