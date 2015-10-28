#include "cpu/exec/template-start.h"

//#define instr stos

make_helper(concat(stos_, SUFFIX)) {
	//printf("eip=%x\n",cpu.eip);
	uint32_t destreg=cpu.edi;
	if (DATA_BYTE==1){
		MEM_W(destreg,REG(R_AL));
		if (cpu.EFLAGS.DF==0) destreg+=1;else destreg-=1;
	}
	else{
//make_helper(concat(stos_, SUFFIX)){
		//uint32_t destreg=cpu.edi;
		if (DATA_BYTE==2){
			MEM_W(destreg,REG(R_AX));
			if (cpu.EFLAGS.DF==0) destreg+=2;else destreg-=2;
		}
		else {
			MEM_W(destreg,REG(R_EAX));
			if (cpu.EFLAGS.DF==0) destreg+=4;else destreg-=4;
		}
	}	
	cpu.edi=destreg;
	//print_asm_template2();
	print_asm("stos %%esi,%%edi\n");
	return 1;
}

#include "cpu/exec/template-end.h"
