#include "cpu/exec/template-start.h"

#define instr movsb

static void do_execute() {
	DATA_TYPE result;
	int shift=(DATA_BYTE-1)*8;
	result=(op_src->val<<shift)>>shift;
	OPERAND_W(op_dest, result);
	print_asm_template2();
}

make_instr_helper(rm2r)
#undef instr

#define instr movsw
static void do_execute() {
	DATA_TYPE result=(op_src->val<<8)>>8;
	OPERAND_W(op_dest,result);
	print_asm_template2();
}
make_instr_helper(rm2r)
#undef instr

#include "cpu/exec/template-end.h"
