#include "cpu/exec/helper.h"


/* for instruction encoding overloading */
#define instr cwtlcltd

make_helper(cwtlcltd){
	if (ops_decoded.is_data_size_16){
		reg_w(R_DX)=(reg_w(R_AX)<0)?0xffff:0;
	}
	else{
		reg_l(R_EDX)=(reg_l(R_EAX)<0)?0xffffffff:0;
	}
	return 1;
}
