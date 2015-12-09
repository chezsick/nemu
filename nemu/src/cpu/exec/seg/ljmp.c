#include "cpu/exec/helper.h"

make_helper(jmp_ptr){
	cpu.eip = instr_fetch(eip+1, 4);
	//cpu.eip-=7;
	cpu.sreg[R_CS].Selector.val = instr_fetch(eip+5, 2);
	print_asm("ljmp $%x, $%x", cpu.sreg[R_CS].Selector.val, cpu.eip);
	return 0;
}
