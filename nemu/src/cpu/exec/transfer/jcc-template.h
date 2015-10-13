#include "cpu/exec/template-start.h"

#define instr je

static void do_execute() {
	if (cpu.EFLAGS.ZF==0){
		cpu.eip+=(int)op_src->val;
		if (ops_decoded.is_data_size_16)
			cpu.eip&=0x0000ffff;
	}
	print_asm_template2();
}
//make_instr_helper(i)
make_instr_helper(r)
//make_instr_helper(m)
#include "cpu/exec/template-end.h"
