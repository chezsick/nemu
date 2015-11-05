#include "cpu/exec/template-start.h"

#define instr ret

make_helper(ret_i_w) {
	if (ops_decoded.is_data_size_16){
		//printf("DATA_BYTE=2\n");
		cpu.eip&=0xffffff00;
		cpu.eip|=swaddr_read(reg_l(R_ESP),2);
		reg_l(R_ESP)+=2;
		cpu.eip&=0x0000ffff;
	}
	else{
		//printf("%x\n",cpu.eip);
		cpu.eip=swaddr_read(reg_l(R_ESP),4);
		reg_l(R_ESP)+=4;
		//printf("%x\n",cpu.eip);
	}
	//print_asm("ret");
	//printf("datasize:%d\n",DATA_BYTE);
	cpu.esp+=op_src->val;
	//print_asm_template1();
	print_asm("retw $0x%x", op_src->val);
	return 3;
}

//make_instr_helper(i)

#include "cpu/exec/template-end.h"
