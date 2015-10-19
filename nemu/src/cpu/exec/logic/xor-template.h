#include "cpu/exec/template-start.h"

#define instr xor

static void do_execute () {
	DATA_TYPE result = op_dest->val ^ op_src->val;
	OPERAND_W(op_dest, result);

	/* TODO: Update EFLAGS. */
	//panic("please implement me");
	DATA_TYPE p=result;
	p=p&0xff;
	p^=p>>4;
	p^=p>>2;
	p^=p>>1;	
	cpu.EFLAGS.SF=MSB(result);	
	cpu.EFLAGS.CF=0;
	cpu.EFLAGS.OF=0;
	if (result==0) 
		cpu.EFLAGS.ZF=1;
	else 
		cpu.EFLAGS.ZF=0;
	if (p==0)
		cpu.EFLAGS.PF=1;
	else 
		cpu.EFLAGS.PF=0;

	print_asm_template2();
}

make_instr_helper(i2a)
make_instr_helper(i2rm)
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(si2rm)
#endif
make_instr_helper(r2rm)
make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"
