#include "trap.h"

int f[15];
int ans[] = {1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880, 3628800, 39916800, 479001600};

int fact(int n) {
	if(n == 0 || n == 1) return 1;
	else return fact(n - 1) * n;
}

int main() {
	int i;
	for(i = 0; i < 13; i ++) {
		//if (i==8) set_bp();
		f[i] = fact(i);
		nemu_assert(f[i] == ans[i]);
		if (i==7) set_bp();
	}

	HIT_GOOD_TRAP;
	return 0;
}

