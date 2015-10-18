#include "cpu/exec/template-start.h"
#define instr lea

static void do_execute() {
	DATA_TYPE mem=instr_fetch(cpu.eip +1,DATA_BYTE);
	if (DATA_BYTE==2){
		OPERAND_W(op_dest,mem&0xffff);
	}
	else {
		//printf("before lea: %x, %x\n",op_src->val,op_src2->val);
		op_dest->val=op_src->val+mem;
		//DATA_TYPE mem=instr_fetch(eip+1,DATA_BYTE);
		//OPERAND_W(op_dest,mem);
		//printf("after lea: %x, %x\n",cpu.eax,cpu.edx);
	}
	print_asm_template2();
}

make_instr_helper(rm2r)
#include "cpu/exec/template-end.h"
