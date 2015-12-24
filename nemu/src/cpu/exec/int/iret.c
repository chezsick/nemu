#include "cpu/exec/helper.h"
#define DATA_BYTE 4
#include "cpu/exec/template-start.h"

#define pop() swaddr_read(cpu.esp, 4, R_SS); cpu.esp+=4;

make_helper(iret) {
	cpu.eip = pop();
	cpu.CS.Selector.val = pop();
	cpu.CS.valid = false;
	cpu.EFLAGS.eflags = pop();

	/*
	uint32_t gdt_off = cpu.gdtr.base + (cpu.sreg[R_CS].index*8);
	uint32_t base = lnaddr_read(gdt_off+2, 2);
	base = base + (lnaddr_read(gdt_off+4, 1)<<16) + (lnaddr_read(gdt_off+7)<<24);
	cpu.sreg[R_CS].base = base;//load base
	uint32_t limit = lnaddr_read(gdt_off, 2) + ((lnaddr_read(gdt_off+6, 1)&0xf)<<16);
	cpu.sreg[R_SS].limit= limit<< ((lnaddr_read(gdt_off+6, 1)>>7) *0xc);	//load limit
	*/
	print_asm("iret");
	return 0;
}

#include "cpu/exec/template-end.h"
