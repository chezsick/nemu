#include "cpu/exec/helper.h"

/* for instruction encoding overloading */
#define instr cmps
make_helper(cmps){
	//uint32_t DATA_BYTE=ops_decoded.size;
	//printf("eip=%x\n",cpu.eip);
	swaddr_t destreg=cpu.edi;
	if (ops_decoded.opcode==0xaa){
		swaddr_write(destreg,1,cpu.gpr[0]._8[0]);
		if (cpu.EFLAGS.DF==0) destreg+=1;else destreg-=1;
	}
	else {
		if (ops_decoded.is_data_size_16){
			//MEM_W(destreg,REG(R_AX));
			swaddr_write(destreg,2,cpu.gpr[0]._16);
			if (cpu.EFLAGS.DF==0) destreg+=2;else destreg-=2;
		}
		else {
			//MEM_W(destreg,REG(R_EAX));
			swaddr_write(destreg,4,cpu.eax);
			if (cpu.EFLAGS.DF==0) destreg+=4;else destreg-=4;
		}
	}
	cpu.edi=destreg;
	//print_asm_template2();
	print_asm("stos %%esi,%%edi\n");
	return 1;
}

//make_helper_v(stos_r2rm)
//#include "cpu/exec/template-end.h"
