#include "cpu/exec/helper.h"
#define DATA_BYTE 4
#include "cpu/exec/template-start.h"

uint32_t pop(){
	uint32_t result;
	result=MEM_R(cpu.esp,R_SS);
	cpu.esp+=DATA_BYTE;
	return result;
}
make_helper(iret) {
	cpu.eip = pop();
	cpu.CS.Selector.val = pop();
	cpu.CS.valid = false;
	cpu.EFLAGS.eflags = pop();
	print_asm("iret");
	return 0;
}

#include "cpu/exec/template-end.h"
