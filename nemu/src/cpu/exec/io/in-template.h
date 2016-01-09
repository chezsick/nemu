#include "cpu/exec/template-start.h"

#define instr in
/*
static void do_execute() {
	DATA_TYPE result =op_src->val&op_dest->val;
	//OPERAND_W(op_dest,result);
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
*/
make_helper(concat(in_i82a_, SUFFIX)) {
	uint8_t imm = instr_fetch(eip+1, 1);
	REG(R_EAX) = pio_read(imm, DATA_BYTE);
	print_asm("in" str(SUFFIX) " 0x%x, %%%s", imm, REG_NAME(R_EAX));
	return 2;
}

make_helper(concat(in_d2a_, SUFFIX)) {
	REG(R_EAX) = pio_read(reg_w(R_DX), DATA_BYTE);
	print_asm("in" str(SUFFIX) " (%%dx), %%%s", REG_NAME(R_EAX));
	return 1;
}
#include "cpu/exec/template-end.h"
