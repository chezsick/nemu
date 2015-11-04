#include "cpu/exec/template-start.h"

#define instr jmp

static void do_execute() {
	if (ops_decoded.opcode ==0xeb||ops_decoded.opcode==0xe9){
		cpu.eip+=op_src->val;
		if (DATA_BYTE==2){
			cpu.eip&=0x0000ffff;
		}
	}
	else{
		if (DATA_BYTE==2){
			cpu.eip=MEM_R(op_src->val)&0x0000ffff;
		}
		else{
			cpu.eip=MEM_R(op_src->val);
		}
	}
	print_asm_template1();
}

//make_instr_helper(i)
make_instr_helper(i)
make_instr_helper(rm)
/*
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
	
	if (ops_decoded.is_data_size_16){
		cpu.eip=MEM_R(op_dest->val)&0x0000ffff;
	}
	else{
		cpu.eip=MEM_R(op_dest->val);
	}
	printf("%s(%d)\n",op_src->str,DATA_BYTE);
	print_asm_template1();
	//print_asm("jmp" str(SUFFIX) " %s",op_src->str);
	return 2;	
}*/
#include "cpu/exec/template-end.h"
