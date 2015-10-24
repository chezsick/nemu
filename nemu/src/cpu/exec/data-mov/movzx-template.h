#include "cpu/exec/template-start.h"

#define instr movzx

static void do_execute() {
	DATA_TYPE result=0x0|op_src->val;
	//int shift=op_dest->type-op_src->type;
	//result=(op_src<<shift)>>shift;
	OPERAND_W(op_dest, result);
	print_asm_template2();
}

make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"
