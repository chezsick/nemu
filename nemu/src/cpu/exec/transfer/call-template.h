#include "cpu/exec/template-start.h"

#define instr call

static void do_execute() {
	if (ops_decoded.is_data_size_16){ //n=2;
		cpu.esp-=2;
		MEM_W(cpu.esp,cpu.eip&0xff);
		cpu.eip+=op_src->val&0xffff;	
	}
	else {
		cpu.esp-=4;
		MEM_W(cpu.esp,cpu.eip);
		cpu.eip+=op_src->val;
	}
	print_asm_template1();
}

make_instr_helper(i)
#include "cpu/exec/template-end.h"
