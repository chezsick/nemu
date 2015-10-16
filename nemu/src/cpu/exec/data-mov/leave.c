#include "cpu/exec/helper.h"
/*
#define DATA_BYTE 1
#include "cpu/exec/template-start.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "cpu/exec/template-start.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "cpu/exec/template-start.h"
#undef DATA_BYTE
*/
/* for instruction encoding overloading */
//make_helper_v(push_i)

#define instr leave
make_helper(leave){
	reg_l(R_ESP)=reg_l(R_EBP);
	if (ops_decoded.is_data_size_16) {
		reg_w(R_BP)=swaddr_read(reg_l(R_ESP),2);
		reg_l(R_ESP)+=2;
	}
	else{
		reg_l(R_EBP)=swaddr_read(reg_l(R_ESP),4);
		reg_l(R_ESP)+=4;
	}
	print_asm("leave");
	return 1;

}
//#include "cpu/exec/template-end.h"

