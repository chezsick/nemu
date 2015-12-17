#include "cpu/exec/helper.h"
#include "cpu/decode/modrm.h"
#include <assert.h>
make_helper(lgdt_m) {
	//assert(0);
         swaddr_t addr=instr_fetch(eip+2, 4);
         cpu.gdtr.limit = swaddr_read(addr, 2, R_SS);
         cpu.gdtr.base = swaddr_read(addr+2, 4, R_SS);
         print_asm("lgdt");
         return 6;
}

make_helper(lidt_m){
	swaddr_t addr=instr_fetch(eip+2, 4);
	cpu.idtr.limit = swaddr_read(addr, 2, R_SS);
	cpu.idtr.base = swaddr_read(addr+2, 4, R_SS);
	print_asm("lidt");
	return 6; 
}
