#include "cpu/exec/template-start.h"

#define instr cmp

static void do_execute() {
	DATA_TYPE_S result = op_dest->val - (DATA_TYPE_S)op_src->val;
	//OPERAND_W(op_dest, result);
	//printf("%d-%d=%d\n",op_dest->val,op_src->val,result);

	cpu.EFLAGS.SF=MSB(result);
	cpu.EFLAGS.CF=(result>op_dest->val)?1:0;
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
		b=op_dest->val>>(i*4)&0xf;
		cpu.EFLAGS.AF=(a>b)?1:0;

	}
	if (MSB(op_src->val)!=MSB(op_dest->val)&&MSB(result)==MSB(op_src->val))
		cpu.EFLAGS.OF=1;
	else 	
		cpu.EFLAGS.OF=0;

	print_asm_template2();
}

#if DATA_BYTE==2 || DATA_BYTE==4
make_instr_helper(si2rm)
#endif

make_instr_helper(i2a)
make_instr_helper(i2rm)
//make_instr_helper(si2rm)
make_instr_helper(r2rm)
make_instr_helper(rm2r)

/*
make_helper(concat(sub_a2moffs_, SUFFIX)) {
	swaddr_t addr = instr_fetch(eip + 1, 4);
	MEM_W(addr, REG(R_EAX));

	print_asm("sub" str(SUFFIX) " %%%s,0x%x", REG_NAME(R_EAX), addr);
	return 5;
}

make_helper(concat(sub_moffs2a_, SUFFIX)) {
	swaddr_t addr = instr_fetch(eip + 1, 4);
	REG(R_EAX) = MEM_R(addr);

	print_asm("sub" str(SUFFIX) " 0x%x,%%%s", addr, REG_NAME(R_EAX));
	return 5;
}*/

#include "cpu/exec/template-end.h"
