#include "cpu/exec/template-start.h"
#define instr lea

static void do_execute() {
	if (DATA_BYTE==2){
		op_dest->val=op_src->val&0xffff;
	}
	else {
		printf("before lea: %x, %x\n",op_src->val,op_src2->val);
		//op_dest->val=op_src->val;
		OPERAND_W(op_dest,op_src->val+op_src2->val);
		printf("after lea: %x, %x\n",cpu.eax,cpu.edx);
	}
	print_asm_template2();
}

make_instr_helper(rm2r)
#include "cpu/exec/template-end.h"
