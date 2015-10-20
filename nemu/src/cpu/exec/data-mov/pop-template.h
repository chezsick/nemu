#include "cpu/exec/template-start.h"

#define instr pop

static void do_execute() {
	uint32_t n=4;
	REG(R_ESP)=REG(R_EBP);
	if (ops_decoded.is_data_size_16) n=2;
	OPERAND_W(op_dest,MEM_R(cpu.esp));
	cpu.esp+=n;
	print_asm_template1();
}
//make_instr_helper(i)
make_instr_helper(r)
make_instr_helper(rm)
#include "cpu/exec/template-end.h"
