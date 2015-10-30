#include "cpu/exec/template-start.h"

#define instr movs

make_helper(concat(movs_, SUFFIX)) {
	//esi for source-index
	uint32_t src_index=cpu.esi;
	//edi for destination-index
	uint32_t des_index=cpu.edi;
	uint32_t incdec=0;
	MEM_W(des_index,MEM_R(src_index));
	if (cpu.EFLAGS.DF==0) incdec=DATA_BYTE;else incdec=-DATA_BYTE;
	cpu.esi=src_index+incdec;
	cpu.edi=des_index+incdec;
	//print_asm_template2();
	print_asm("movs %%esi to %%edi\n");
	return 1;
}

//make_instr_helper(r2rm)


#include "cpu/exec/template-end.h"
