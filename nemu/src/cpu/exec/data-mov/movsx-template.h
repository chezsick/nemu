#include "cpu/exec/template-start.h"

#define instr movsx

static void do_execute() {
	DATA_TYPE result;
	int shift=op_dest->type-op_src->type;
	result=(op_src->val<<shift)>>shift;
	OPERAND_W(op_dest, result);
	print_asm_template2();
}

make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"
