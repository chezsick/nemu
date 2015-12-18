#include "cpu/exec/helper.h"
#include "cpu/decode/modrm.h"
#include <assert.h>
make_helper(lgdt_m) {
	if (instr_fetch(eip+1, 1) == 0x15){
		swaddr_t addr = instr_fetch(eip+2, 4);
		cpu.gdtr.limit = swaddr_read(addr, 2, R_SS);
        	cpu.gdtr.base = swaddr_read(addr+2, 4, R_SS);
       		print_asm("lgdt");
        	return 6;
	}
	else {
		cpu.gdtr.limit = swaddr_read(cpu.eax, 2, R_SS);
		cpu.gdtr.base = swaddr_read(cpu.eax, 2, R_SS);
		print_asm("lgdt (%%eax)");
		return 2;
	}
	return 0;
}

make_helper(lidt_m){
	if (instr_fetch(eip+1, 1) == 0x18) {
		cpu.idtr.limit = swaddr_read(cpu.eax, 2, R_SS);
		cpu.idtr.base = swaddr_read(cpu.eax+2, 4, R_SS);
		print_asm("lidt (%%eax)");
		return 2;
	}
	else {
		swaddr_t addr = instr_fetch(eip+2, 4);
		//int len=read_ModR_M(eip+1, &addr, &addr);
		cpu.idtr.limit = swaddr_read(addr, 2, R_SS);
		cpu.idtr.base = swaddr_read(addr+2, 4, R_SS);
		print_asm("lidt");
		return 6;
	}
	return 0;
}
