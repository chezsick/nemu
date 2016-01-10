#include "cpu/exec/helper.h"
#include "../../lib-common/x86-inc/mmu.h"
#include <setjmp.h>
extern jmp_buf jbuf;
#define push(x){\
	cpu.esp-=4;\
	swaddr_write(cpu.esp, 4, x, R_SS);\
}


void raise_intr(uint8_t NO) {
		/* TODO: Trigger an interrupt/exception with ``NO''.
		 * That is, use ``NO'' to index the IDT.
		*/	 	
		cpu.eip += 2;
		push(cpu.EFLAGS.eflags);
		push(cpu.CS.Selector.val);
		push(cpu.eip);
		GateDesc gd;
		int i;
		for (i = 0; i < sizeof(GateDesc); i++){
			*((uint8_t*)(&gd)+i) = lnaddr_read(cpu.idtr.base+NO*sizeof(GateDesc)+i, 1);
		}
		cpu.CS.Selector.val = gd.segment;
		cpu.CS.valid = false;
		swaddr_t addr = (gd.offset_31_16<<16) | gd.offset_15_0;
		cpu.eip = addr;
		/* Jump back to cpu_exec() */

		longjmp(jbuf, 1);
}

make_helper(int_i){
	//Log("ini, eip:%x",eip);
	uint8_t imm = instr_fetch(eip+1, 1);
	raise_intr(imm);
	print_asm("int\t $0x%02x", imm);
	return 0;
}
