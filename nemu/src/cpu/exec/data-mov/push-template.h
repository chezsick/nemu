#include "cpu/exec/template-start.h"

#define instr push

static void do_execute() {
	uint32_t n=4;
	if (DATA_BYTE==2) n=2; 
	cpu.esp-=n;
	MEM_W(cpu.esp,op_src->val);
	print_asm_template1();
}
make_instr_helper(rm)
#include "cpu/exec/template-end.h"
