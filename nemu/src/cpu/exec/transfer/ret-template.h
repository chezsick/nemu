#include "cpu/exec/template-start.h"

#define instr ret

static void do_execute() {
	if (DATA_BYTE==2){
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
	cpu.esp+=op_src->val;
	print_asm_template1();
}

make_instr_helper(i)
#include "cpu/exec/template-end.h"
