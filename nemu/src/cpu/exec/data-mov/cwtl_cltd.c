#include "cpu/exec/helper.h"


/* for instruction encoding overloading */
#define instr cwtl_cltd
make_helper(cwtl_clte){
	if (ops_decoded.is_data_size_16){
		reg_w(R_AX)=(short int)reg_b(R_AL);
	}
	else{
		reg_l(R_EAX)=(int)reg_l(R_AX);
	}
	return 1;
}
