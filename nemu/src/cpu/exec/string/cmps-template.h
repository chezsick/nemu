#include "cpu/exec/template-start.h"


make_helper(concat(cmps_, SUFFIX)) {
	
	uint32_t src_index=cpu.esi;
	uint32_t dest_index=cpu.edi;
	uint32_t incdec=0;
	uint32_t result=MEM_R(src_index)-MEM_R(dest_index);
	if (cpu.EFLAGS.DF==0) incdec=DATA_BYTE;else incdec=-DATA_BYTE;
	
	cpu.EFLAGS.SF=MSB(result);
	cpu.EFLAGS.CF=(result>MEM_R(dest_index))?1:0;
	cpu.EFLAGS.ZF=(result==0)?1:0;

	DATA_TYPE p=result;
	p=p&0xff;
	p^=p>>4;
	p^=p>>2;
	p^=p>>1;
	cpu.EFLAGS.PF=(p==0)?1:0;

	uint32_t i;
	for (i=0;i<DATA_BYTE;i++){
		DATA_TYPE a,b;
		a=result>>(i*4)&0xf;
		b=MEM_R(dest_index)>>(i*4)&0xf;
		cpu.EFLAGS.AF=(a>b)?1:0;

	}
	if (MSB(MEM_R(src_index))!=MSB(MEM_R(dest_index))&&MSB(result)==MSB(MEM_R(src_index)))
		cpu.EFLAGS.OF=1;
	else 	
		cpu.EFLAGS.OF=0;


	cpu.esi=src_index+incdec;
	cpu.edi=dest_index+incdec;
	print_asm("cmps %%edi,%%esi\n");
	return 1;
}

#include "cpu/exec/template-end.h"
