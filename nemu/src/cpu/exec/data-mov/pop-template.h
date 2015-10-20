#include "cpu/exec/template-start.h"

#define instr pop

static void do_execute() {
	printf("old esp:%x\n",cpu.esp);
	OPERAND_W(op_dest,MEM_R(cpu.esp));
	//printf("old esp:%x\n",cpu.esp);
	cpu.esp+=DATA_BYTE;
	printf("new esp:%x\n",cpu.esp);
	print_asm_template1();
}
//make_instr_helper(i)
make_instr_helper(r)
make_instr_helper(rm)
#include "cpu/exec/template-end.h"
