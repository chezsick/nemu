#include "cpu/exec/template-start.h"

#define instr jmp
/*
static void do_execute() {
	if (cpu.EFLAGS.Z===1){
		cpu.eip+=(int)op_src->val;
		if (ops_decoded.is_data_size_16)
			cpu.eip&=0x0000ffff;
	}
	print_asm_template1();
}

//make_instr_helper(i)
make_instr_helper(i)
make_instr_helper(rm)
*/
make_helper(concat(jmp_i_,SUFFIX)){
	REG(eip)+=(int)op_src->val;
	if (ops_decoded.is_data_size_16)
                REG(eip)&=0x0000ffff;
	print_asm_template1();
	return 0;
}
make_helper(concat(jmp_rm_,SUFFIX)){
	if (ops_decoded.is_data_size_16)
		REG(eip)=op_src->val&0x0000ffff;
	else
		REG(eip)=op_src->val;
	print_asm_template1();
	return 0;	
}
#include "cpu/exec/template-end.h"
