#include "cpu/exec/template-start.h"
#define instr lea

static void do_execute() {
	if (DATA_BYTE==2){
		OPERAND_W(op_dest,op_src->imm&0xffff);
	}
	else {
		OPERAND_W(op_dest,op_src->simm);
	}
	print_asm_template2();
}

make_instr_helper(rm2r)
#include "cpu/exec/template-end.h"
