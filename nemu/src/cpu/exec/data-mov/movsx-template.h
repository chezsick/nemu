#include "cpu/exec/template-start.h"

#define instr movsb

static void do_execute() {
	DATA_TYPE_S result;
	int shift=(DATA_BYTE-1)*8;
	//printf("sf:%d DA:%d SRC:%x\n",shift,DATA_BYTE,op_src->val);
	result=((DATA_TYPE_S)op_src->val<<shift)>>shift;
	//printf("RE:%x\n",result);
	OPERAND_W(op_dest, result);
	print_asm_template2();
}

make_instr_helper(rm2r)
#undef instr

#define instr movsw
static void do_execute() {
	DATA_TYPE_S result=((DATA_TYPE_S)op_src->val<<16)>>16;
	OPERAND_W(op_dest,result);
	print_asm_template2();
}
make_instr_helper(rm2r)
#undef instr

#include "cpu/exec/template-end.h"
