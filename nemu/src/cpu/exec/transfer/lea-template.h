#include "cpu/exec/template-start.h"

#define instr lea

static void do_execute() {
	if (DATA_BYTE==2){
		OPERAND_W(op_dest,((int)op_src)&0xffff);
	}
	else {
		OPERAND_W(op_dest,(int)op_src);
	}
	print_asm_template2();
}

make_instr_helper(rm2r)
#include "cpu/exec/template-end.h"
