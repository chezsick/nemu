#include "cpu/exec/template-start.h"

#define instr test

static void do_execute() {
	DATA_TYPE result =op_src->val&op_src2->val;
	OPERAND_W(op_dest,result);
	cpu.EFLAGS.OF=0;
	cpu.EFLAGS.CF=0;
	cpu.EFLAGS.SF=MSB(result);
	cpu.EFLAGS.ZF=(result==0)?1:0;
	
	DATA_TYPE p=result;
	p=p&0xff;
	p^=p>>4;
	p^=p>>2;
	p^=p>>1;
	cpu.EFLAGS.PF=(p==0)?1:0;

	print_asm_template2();
}

make_instr_helper(i2a)
make_instr_helper(i2rm)
make_instr_helper(r2rm)
#include "cpu/exec/template-end.h"
