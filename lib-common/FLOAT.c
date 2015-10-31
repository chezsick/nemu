#include "FLOAT.h"

FLOAT F_mul_F(FLOAT a, FLOAT b) {
	//nemu_assert(0);
	//return 0;
	long long  result=a*b;
	result>>=16;
	return (FLOAT)result;
}

FLOAT F_div_F(FLOAT a, FLOAT b) {
	//nemu_assert(0);
	//return 0;
	FLOAT c=(a/b)<<16;
	FLOAT d=(a%b);
	int i;
	for(i=0;i<16;i++){
		c+=(2*d/b)<<(16-i);
		d=2*d/b;
	}
	return c;
}

FLOAT f2F(float a) {
	//nemu_assert(0);
	//return 0;
	/*
	int sign=(a>>31)&1;
	signed char expo=(a>>23)&0xff;
	int mant=(a<<9)>>9;
	FLOAT result;
	if (expo>0){
		result=0;
	}*/
	return (FLOAT)(a*0x10000);

}

FLOAT Fabs(FLOAT a) {

	//nemu_assert(0);
	//return 0;
	if (!!(a>>31)) a=~a+1;
	return a;

}

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

