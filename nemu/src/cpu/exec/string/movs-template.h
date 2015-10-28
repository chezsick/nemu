#include "cpu/exec/template-start.h"

#define instr movs

make_helper(movs) {
	ops_decoded.is_data_size_16=(ops_decoded.opcode== 0xa5)?0:1;
	//esi for source-index
	uint32_t src_index=cpu.esi;
	//edi for destination-index
	uint32_t des_index=cpu.edi;
	uint32_t incdec=0;
	if (ops_decoded){
		MEM_W(des_index,MEM_R(src_index));
		if (cpu.EFLAGS.DF==0) incdec=1;else incdec=-1;
	}
	else {
		if (DATA_BYTE==2){
			MEM_W(des_index,MEM_R(src_index));			
			if (cpu.EFLAGS.DF==0) incdec=2;else incdec=-2;
		}
		else {
			MEM_W(des_index,MEM_R(src_index));
			if (cpu.EFLAGS.DF==0) incdec=4;else incdec=-4;
		}
	}
	cpu.esi=src_index+incdec;
	cpu.edi=des_index+incdec;
	//print_asm_template2();
	print_asm("movs %%esi to %%edi\n");
	return 1;
}

//make_instr_helper(r2rm)


//#include "cpu/exec/template-end.h"
