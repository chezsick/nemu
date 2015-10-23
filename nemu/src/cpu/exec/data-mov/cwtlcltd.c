#include "cpu/exec/helper.h"

//#define MSB(n) ((DATA_TYPE)(n) >> ((DATA_BYTE << 3) -1))
/* for instruction encoding overloading */
#define instr leave
make_helper(cwtlcltd){
	if (ops_decoded.is_data_size_16){
		reg_w(R_DX)=(reg_w(R_AX)>>15)?0xffff:0;
		print_asm("cwtl");
	}
	else{
		reg_l(R_EDX)=(reg_l(R_EAX)>>31)?0xffffffff:0;
		print_asm("cltd");	
	}
	return 1;
	

}
