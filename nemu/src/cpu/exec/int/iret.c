#include "cpu/exec/helper.h"
#define DATA_BYTE 4
#include "cpu/exec/template-start.h"

uint32_t pop(){
	uint32_t result;
	result=MEM_R(cpu.esp,R_SS);
	cpu.esp+=4;
	return result;
}
make_helper(iret) {
	cpu.eip = pop()-1;
	cpu.CS.Selector.val = pop();
	cpu.CS.valid = false;
	cpu.EFLAGS.eflags = pop();
	print_asm("iret");
	return 1;
}

#include "cpu/exec/template-end.h"
