#include "cpu/exec/template-start.h"

#define instr mov

static void do_execute() {
	//printf("in mov: %x, %x\n",op_dest->val,op_src->val);
	OPERAND_W(op_dest, op_src->val);
	//if (DATA_BYTE==1) 
	//	printf("%x to %x(%c)\n",op_src->val,op_dest->addr,op_src->val);
	print_asm_template2();
}

make_instr_helper(i2r)
make_instr_helper(i2rm)
make_instr_helper(r2rm)
make_instr_helper(rm2r)

make_helper(concat(mov_a2moffs_, SUFFIX)) {
	swaddr_t addr = instr_fetch(eip + 1, 4);
	//MEM_W(addr, REG(R_EAX));
	swaddr_write(addr, DATA_BYTE, REG(R_EAX), R_DS);
	print_asm("mov" str(SUFFIX) " %%%s,0x%x", REG_NAME(R_EAX), addr);
	return 5;
}

make_helper(concat(mov_moffs2a_, SUFFIX)) {
	swaddr_t addr = instr_fetch(eip + 1, 4);
	//REG(R_EAX) = MEM_R(addr);
	REG(R_EAX) = swaddr_read(addr, DATA_BYTE, R_DS);
	print_asm("mov" str(SUFFIX) " 0x%x,%%%s", addr, REG_NAME(R_EAX));
	return 5;
}

#include "cpu/exec/template-end.h"
