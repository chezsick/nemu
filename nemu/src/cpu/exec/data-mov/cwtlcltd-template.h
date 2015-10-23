#include "cpu/exec/template-start.h"

#define instr cwtlcltd

static void do_execute() {
	if (DATA_BYTE==2){
		REG(R_DX)=(MSB(REG(R_AX))==1)?0xffff:0;
	}
	else{
		REG(R_EDX)=(MSB(REG(R_EAX))==1)?0xffffffff:0;
	}
	fuckyou;
	print_asm_template1();
}
make_instr_helper(r)
#include "cpu/exec/template-end.h"
