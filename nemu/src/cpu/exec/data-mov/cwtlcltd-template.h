#include "cpu/exec/template-start.h"

#define instr cwtlcltd

static void do_execute() {
	if (DATA_BYTE==2){
		REG(R_DX)=((short)REG(R_AX)<0)?0xffff:0;
	}
	else{
		REG(R_EDX)=((int)REG(R_EAX)<0)?0xffffffff:0;
	}
	
	print_asm_template1();
}
make_instr_helper(r)
#include "cpu/exec/template-end.h"
