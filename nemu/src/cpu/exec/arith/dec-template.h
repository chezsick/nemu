#include "cpu/exec/template-start.h"

#define instr dec

static void do_execute () {
	DATA_TYPE result = op_src->val - 1;
	OPERAND_W(op_src, result);

	/* TODO: Update EFLAGS. */
	panic("please implement me");
	cpu.EFLAGS.SF=MSB(result);
	//cpu.EFLAGS.CF=(result>op_dest->val)?1:0;
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
		b=op_src->val>>(i*4)&0xf;
		cpu.EFLAGS.AF=(a>b)?1:0;

	}
	cpu.EFLAGS.OF=(result==0x7fffffff)?1:0;

	print_asm_template1();
}

make_instr_helper(rm)
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(r)
#endif

#include "cpu/exec/template-end.h"
