#include "cpu/exec/template-start.h"

#define instr lea

static void do_execute() {
	if (DATA_BYTE==2){
		op_dest->val=op_src->val&0xffff;
	}
	else {
		op_dest->val=op_src->val;
	}
	print_asm_template2();
}

make_instr_helper(rm2r)
#include "cpu/exec/template-end.h"
