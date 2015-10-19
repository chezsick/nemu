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
	//cpu.eip+=op_src->val;
	//printf("%x\n",cpu.eip);
	cpu.eip+=instr_fetch(eip + 1, DATA_BYTE);
	//printf("%x\n",cpu.eip);
	if (ops_decoded.is_data_size_16)
                cpu.eip&=0x0000ffff;
	print_asm("jmp" str(SUFFIX) " 0x%x",cpu.eip);
	//print_asm_template1();
	return 1+DATA_BYTE;
}
make_helper(concat(jmp_rm_,SUFFIX)){
	if (ops_decoded.is_data_size_16)
		cpu.eip=op_src->val&0x0000ffff;
	else
		cpu.eip=op_src->val;
	print_asm_template1();
	return 1+DATA_BYTE;	
}
#include "cpu/exec/template-end.h"
